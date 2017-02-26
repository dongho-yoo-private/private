#include "network/protocols/smtp.h"
#include "network/protocols/mime.h"
#include "network/igx_sock.h"
#include "library/igx_string.h"
#include "library/igx_memory.h"
#include "io/igx_encrypt.h"
#include "debug/igx_debug.h"

#if _OS_IS_==OS_IS_IOS||_OS_IS_==OS_ISMACOSX
#   include <sys/time.h>
#else
#   include <time.h>
#endif

/** smtp 一例
MAIL FROM: <sender@example.org>
250 ok
RCPT TO: <recipient1@example.com>
250 ok
RCPT TO: <recipient2@example.com>
250 ok
RCPT TO: <recipient3@example.com>
250 ok
DATA
354 go ahead
Date: 02 Sep 2011 01:45:59 -0500
Subject: test
From: "Sender" <sender@example.org>
To: "Recipient 1" <recipient1@example.com>
CC: "Recipient 2" <recipient2@example.com>

test
.
**/

static const igx_string_t kCmdAuthLogin = {"AUTH LOGIN", 10, 0};
static const igx_string_t kCmdAuthPlain = {"AUTH PLAIN", 10, 0};
static const igx_string_t kCmdData = {"DATA", 4, 0};
static const igx_string_t kCmdQuit = {"QUIT", 4, 0};
static const igx_string_t kCmdEhlo = {"EHLO", 4, 0};
static const igx_string_t kCmdStarttls = {"STARTTLS", 8, 0};
static const igx_string_t kCmdEof = {"\r\n.\r\n", 5, 0};

static const size_t kEmailIDLength = 127;
static const size_t kMAX_SMTP_ADD_FILE_COUNT = 32;

typedef struct {
    const char* filename;
    void* data;
    size_t data_size;
} __igx_smtp_file_t;

typedef struct {
    igx_sock_t sock;
    igx_addrv4_t server;
    HITX h_mem;
    __igx_smtp_file_t* files;
    bit32_t files_count;
    igx_mime_user_t*  users;
    bit32_t users_count;
    struct {
        igx_string_t name;
        igx_string_t email;
    }sender;
    igx_string_t subject;
    igx_string_t body;
    /*
    250-TURN
    250-SIZE 41943040
    250-ETRN
    250-PIPELINING
    250-DSN
    250-ENHANCEDSTATUSCODES
    250-8bitmime
    250-BINARYMIME
    250-CHUNKING
    250-VRFY
    250-TLS
    250-STARTTLS
    250-AUTH LOGIN PLAIN XOAUTH2
    */
    struct {
        unsigned turn:1;
        unsigned etrn:1;
        unsigned pipe_line:1;
        unsigned dsn:1;
        unsigned enhanced_status_close:1;
        unsigned mime8:1;
        unsigned binary_mime:1;
        unsigned chunking:1;
        unsigned vrfy:1;
        unsigned tls:1;
        unsigned starttls:1;
        unsigned auth_login:1;
        unsigned auth_plain:1;
        unsigned xauth2:1;
        unsigned reserved:18;
    }support;
    igx_smtp_error_t last_error;
    igx_smtp_detail_t detail;
} __igx_smtp_t;

static void* __smtp_malloc(__igx_smtp_t*s, size_t size)
{
    if (s->detail.use_mem_page==1 &&
            s->h_mem==0)
    {
        s->h_mem = igx_new_mem_page(0, 0, 8, true);
    }
    if (s->h_mem!=0)
    {
        return pmalloc_c(s->h_mem, size);
    }
    return IGX_MALLOC(size, "__smtp_malloc");
}
static void __smtp_free(__igx_smtp_t* s, void* p)
{
    if (s->detail.use_mem_page!=1)
    {
        IGX_FREE(p);
    }
    else
    {
        pfree_c(s->h_mem, p);
    }
}
static const char* __get_content_type(const char* filename)
{
    const size_t len = strlen(filename);
    const char* p = 0;
    bit32_t count=0, i;

    for (i=len-1; i>0; --i, ++count)
    {
        if (filename[i]=='.')
        {
            p=filename+i+1;
            break;
        }
    }

    if (p==0)
    {
        return "Application/Octet-Stream";
    }
   
#define FILE_FORMAT(extension, type) \
if (strncasecmp(p, extension, count)==0)\
{\
    return type;\
}
    /* for image */
    FILE_FORMAT("gif", "Image/Gif");
    FILE_FORMAT("jpg", "Image/Jpeg");
    FILE_FORMAT("tiff", "Image/Tiff");
    FILE_FORMAT("png", "Image/Png");

    /* for audio */
    FILE_FORMAT("mp3", "Audio");
    FILE_FORMAT("wav", "Audio");

    /* for vidio */
    FILE_FORMAT("mp4", "Video");
    FILE_FORMAT("avi", "Video");
    FILE_FORMAT("mpg", "Video/Mpeg");

    return "Application/Octet-Stream";
}

static igx_smtp_error_t __send_and_receive2(__igx_smtp_t* s, char* data, size_t data_size, bit32_t* result, char* buffer, size_t buffer_size)
{
    const bit32_t zero = (bit32_t)'0';
    size_t received;
    memcpy(buffer, data, data_size);
    if (igx_sock_write(&s->sock,buffer,data_size, 0)!=kSockErrorSuccess)
    {
        return kSmtpErrorWrite;
    }
    if (buffer==0)
    {
        return kSmtpErrorSuccess;
    }
    if (igx_sock_read_once(&s->sock, buffer, buffer_size, &received, s->detail.read_timeout)!=kSockErrorSuccess)
    {
        return kSmtpErrorRead;
    }

    *result = ((bit32_t)buffer[0]-zero)*100+((bit32_t)buffer[1]-zero)*10+((bit32_t)buffer[0]-zero);
    return kSmtpErrorSuccess;
}
static igx_smtp_error_t __send_and_receive(__igx_smtp_t* s, char* data, size_t data_size, bit32_t* result, char* buffer, size_t buffer_size)
{
    const bit32_t zero = (bit32_t)'0';
    size_t received;
    memcpy(buffer, data, data_size);
    buffer[data_size]='\r';
    buffer[data_size+1]='\n';
    if (igx_sock_write(&s->sock,buffer,data_size+2, 0)!=kSockErrorSuccess)
    {
        return kSmtpErrorWrite;
    }
    if (igx_sock_read_once(&s->sock, buffer, buffer_size, &received, s->detail.read_timeout)!=kSockErrorSuccess)
    {
        return kSmtpErrorRead;
    }
    *result = ((bit32_t)buffer[0]-zero)*100+((bit32_t)buffer[1]-zero)*10+((bit32_t)buffer[0]-zero);
    return kSmtpErrorSuccess;
}
static bool_t __check_command(__igx_smtp_t* s, const char* cmd, const char* param1, const char* param2, const char* param3)
{
    int i;
    
    if (strcasecmp(cmd,  "TURN")==0)
    {
        s->support.turn=1;
        return true;
    }
    if (strcasecmp(cmd,  "ETRN")==0)
    {
        s->support.etrn=1;
        return true;
    }
    if (strcasecmp(cmd, "PIPELINING")==0)
    {
        s->support.pipe_line=1;
        return true;
    }
    if (strcasecmp(cmd, "DSN")==0)
    {
        s->support.dsn=1;
        return true;
    }
    if (strcasecmp(cmd, "ENHANCEDSTATUSCODES")==0)
    {
        s->support.enhanced_status_close=1;
        return true;
    }
    if (strcasecmp(cmd, "8bitmime")==0)
    {
        s->support.mime8=1;
        return true;
    }
    if (strcasecmp(cmd, "BINARYMIME")==0)
    {
        s->support.binary_mime=1;
        return true;
    }
    if (strcasecmp(cmd, "CHUNKING")==0)
    {
        s->support.chunking=1;
        return true;
    }
    if (strcasecmp(cmd, "VRFY")==0)
    {
        s->support.vrfy=1;
        return true;
    }
    if (strcasecmp(cmd, "TLS")==0)
    {
        s->support.tls=1;
        return true;
    }
    if (strcasecmp(cmd, "STARTTLS")==0)
    {
        s->support.starttls=1;
        return true;
    }
    if (strcasecmp(cmd, "AUTH")==0)
    {
        const char* params[] = {param1, param2, param3, 0};
        bit32_t i=0;
        for (; params[i]!=0; ++i)
        {
            if (strcasecmp(params[i], "LOGIN")==0)
            {
                s->support.auth_login=1;
            }
            else if (strcasecmp(params[i], "PLAIN")==0)
            {
                s->support.auth_plain=1;
            }
            else if (strcasecmp(params[i], "XOAUTH2")==0)
            {
                s->support.xauth2=1;
            }
        }
        return true;
    }
    return false;
}
static bit32_t __check_command_list(__igx_smtp_t* s)
{
    size_t received;
    char w_buffer[1024], *p;
    bit32_t ret=0;
    igx_string_t line;
    size_t command_length=0;

    /** ビットフィルドの初期化 **/
    s->support.turn=0;
    s->support.etrn=0;
    s->support.pipe_line=0;
    s->support.dsn=0;
    s->support.enhanced_status_close=0;
    s->support.mime8=0;
    s->support.binary_mime=0;
    s->support.chunking=0;
    s->support.vrfy=0;
    s->support.tls=0;
    s->support.starttls=0;
    s->support.auth_login=0;
    s->support.auth_plain=0;
    s->support.xauth2=0;
    s->support.reserved=0;

    command_length=kCmdEhlo.length;
    memcpy(w_buffer, kCmdEhlo.str, command_length);
    w_buffer[command_length++]='\r';
    w_buffer[command_length]='\n';

    if (igx_sock_write(&s->sock,w_buffer,command_length+2, 0)!=kSockErrorSuccess)
    {
        return kSmtpErrorWrite;
    }
    if (igx_sock_read_once(&s->sock, w_buffer, sizeof(w_buffer), &received, s->detail.read_timeout)!=kSockErrorSuccess)
    {
        return 0;
    }
    p = (char*)w_buffer;
    do
    {
        char command[32], param1[16], param2[16], param3[16];
        sscanf(w_buffer, "250-%s %s %s %s\r\n", command, param1, param2, param3);
        __check_command(s, command, param1, param2, param3);
        p+=line.length;
        ++ret;
    } while(igx_get_next_line(&line, p)!=0);

    return ret;
}
igx_smtp_detail_t* igx_smtp_default_detail(igx_smtp_detail_t* detail)
{
    memset(detail, 0, sizeof(*detail));
    detail->use_ssl=1;
    detail->use_mem_page=1;
    detail->port=587;
    detail->connect_timeout=1500;
    detail->read_timeout=5000;
    detail->charset="utf-8";

    return detail;
}
#define HANDLE_CAST(s, h) __igx_smtp_t* s = (__igx_smtp_t*)smtp;
igx_smtp_error_t igx_smtp_create(igx_smtp_t* smtp, igx_addrv4_t addr, igx_smtp_detail_t* detail)
{
    HANDLE_CAST(s, smtp);
    igx_sock_error_t sock_error;
    char working_buffer[1024];
    size_t received;

    if (detail==0)
    {
        igx_smtp_default_detail(&s->detail);
    }
    else
    {
        s->detail=*detail;
    }
    if (igx_sock_connect(&s->sock, addr, s->detail.port, s->detail.connect_timeout, s->detail.use_ssl, &sock_error)==0)
    {
        return kSmtpErrorConnect;
    }
    if (igx_sock_read_once(&s->sock, working_buffer, sizeof(working_buffer), &received, s->detail.read_timeout)!=kSockErrorSuccess)
    {
        return 1;
    }
    if (working_buffer[0]!='2' || 
            working_buffer[1]!='2' ||
                working_buffer[2]!='0')
    {
        return kSmtpErrorServer;
    }

    __check_command_list(s);
    return kSmtpErrorSuccess;
}
igx_smtp_error_t igx_smtp_login(igx_smtp_t* smtp, const char* id, const char* password)
{
    HANDLE_CAST(s, smtp);
    size_t received;
    char x[64];
    char* pBase64;
    char working_buffer[1024];
    bit32_t result;

LOGIN_:
    if  (s->support.auth_login==0 && s->support.auth_plain==0 && s->support.xauth2==0)
    {
        if (s->detail.use_ssl==0 && s->support.starttls==1)
        {
            if (__send_and_receive(s, kCmdStarttls.str, kCmdStarttls.length , &result, working_buffer, sizeof(working_buffer))!=kSockErrorSuccess)
            {
                return kSmtpErrorLogin;
            }
            if (result!=250)
            {
                IGX_WARN("%s", working_buffer);
                return kSmtpErrorLogin;
            }
            if (igx_sock_negotiation(&s->sock)!=kSockErrorSuccess)
            {
                return kSmtpErrorLogin;
            }
            __check_command_list(s);
            goto LOGIN_;
        }
        else
        {
            return kSmtpErrorSuccess;
        }
    }

    if (s->support.auth_login==1)
    {
        bit32_t result;
        char    base64[kEmailIDLength+1], working_buffer[1024];
        size_t base64_size = sizeof(base64)-1;
        igx_smtp_error_t error = __send_and_receive(s, kCmdAuthLogin.str, kCmdAuthLogin.length, &result, working_buffer, sizeof(working_buffer));
        if (error!=kSmtpErrorSuccess ||
                result!=334)
        {
            return kSmtpErrorLogin;
        }
        if (igx_encrypt_base64(base64, &base64_size, id, strlen(id))==0)
        {
            IGX_WARN("too long id %s", id);
            return kSmtpErrorId;
        }
        error = __send_and_receive(s, base64, base64_size, &result, working_buffer, sizeof(working_buffer));
        if (error!=kSmtpErrorSuccess||
                result!=334)
        {
            return kSmtpErrorId;
        }

        if (igx_encrypt_base64(base64, &base64_size, password, strlen(password))==0)
        {
            IGX_WARN("too long password %s", id);
            return kSmtpErrorPassword;
        }
        error = __send_and_receive(s, base64, base64_size, &result, working_buffer, sizeof(working_buffer));
        if (error!=kSmtpErrorSuccess||
                result!=235)
        {
            return kSmtpErrorPassword;
        }
    }
    else if (s->support.auth_plain==1)
    {
        /* TODO */
    }
    else
    {
        return kSmtpErrorLogin;
    }

    return kSmtpErrorSuccess;
}
bit32_t igx_smtp_add_user(igx_smtp_t* smtp, igx_mime_user_t* user, const bit32_t users_count)
{
    HANDLE_CAST(s, smtp);
    s->users=user;
    s->users_count=users_count;

    return users_count;
}
void igx_smtp_sender(igx_smtp_t* smtp, const char* email_address, const char* display_name)
{
    HANDLE_CAST(s, smtp);

    s->sender.email.str=(char*)email_address;
    s->sender.email.length = strlen(email_address);
    s->sender.name.str= (char*)display_name;
    s->sender.name.length = (display_name!=0)?strlen(display_name):0;

    return ;
}
bit32_t igx_smtp_add_file(igx_smtp_t* smtp, const char* filename, const void* data, const size_t size)
{
    HANDLE_CAST(s, smtp);

    if (s->files==0)
    {
        s->files = (__igx_smtp_file_t*)__smtp_malloc(s, sizeof(__igx_smtp_file_t)*kMAX_SMTP_ADD_FILE_COUNT);
    }

    if (s->files_count==kMAX_SMTP_ADD_FILE_COUNT)
    {
        IGX_WARN("the limit of add file count is %d", kMAX_SMTP_ADD_FILE_COUNT);
        return -1;
    }

    s->files[s->files_count].filename=filename;
    s->files[s->files_count].data = (void*)data;
    s->files[s->files_count++].data_size = size;
   
    return s->files_count;
}
void igx_smtp_subject(igx_smtp_t* smtp, const char* subject, const size_t subject_len)
{
    HANDLE_CAST(s, smtp);
    s->subject.str=(char*)subject;
    s->subject.length=(size_t)subject_len;
    return ;
}
void igx_smtp_message(igx_smtp_t* smtp, const char* body, const size_t body_len)
{
    HANDLE_CAST(s, smtp);
    s->body.str=(char*)body;
    s->body.length=(size_t)body_len;
    return ;
}
static igx_smtp_error_t __send_mail_from(__igx_smtp_t* s)
{
    char working_buffer[1024];
    bit32_t n=sprintf(working_buffer, "MAIL FROM: %s", s->sender.email.str);
    bit32_t result;
    bool_t is_body_7bit;

    s->last_error=0;
    if (__send_and_receive(s, working_buffer, n, &result, working_buffer, sizeof(working_buffer))!=kSockErrorSuccess)
    {
        return kSmtpErrorWrite;
    }
    if (result!=250)
    {
        s->last_error=result;
        IGX_WARN("__send_mail_from %d", result);
        return kSmtpErrorWrite;
    }

    return kSmtpErrorSuccess;
}
static igx_smtp_error_t __data(__igx_smtp_t* s)
{
    char working_buffer[4096];
    bit32_t n=0, result;
    bool_t is_body_7bit=false;

#define APPEND_STRING(text, len) memcpy(working_buffer+n, text, len), n+=len
#define APPEND_LINE_FEED() working_buffer[n++]='\r', working_buffer[n++]='\n'

    if (__send_and_receive(s, kCmdData.str, kCmdData.length, &result, working_buffer, sizeof(working_buffer))!=kSockErrorSuccess ||
        result!=354)
    {
        return kSmtpErrorData;
    }
    /* Date: */
    {
        const time_t ret = time(0);
        const char* _time = (const char*)ctime(&ret);
        const size_t len = strlen(_time);
        memcpy(working_buffer+n, "Date: ", 6);
        APPEND_STRING("Date", 6);
        n+=6;
        APPEND_STRING(_time, len-1);
        working_buffer[n++]='\r';
        working_buffer[n++]='\n';
    }
    /* From: */
    {
        const size_t from_len=6;
        APPEND_STRING("From: ", from_len);
        n+=igx_mime_user(working_buffer+n, sizeof(working_buffer)-1-n, &s->sender.name, &s->sender.email);
        working_buffer[n++]='\r', working_buffer[n++]='\n';
    } /* from */
    /* CC: */
    /* BCC: */
    {
        bit32_t added_count=0, i;
        const bit32_t count = s->users_count;

        /* To: */
        for (i=0; i<count; ++i)
        {
            if (s->users[i].type.to==1)
            {
                if (added_count!=0)
                {
                    working_buffer[n++]=',';
                }
                else
                {
                    APPEND_STRING("To: ", 4);
                }
                igx_string_t name = {(char*)s->users[i].display_name, s->users[i].display_name?strlen(s->users[i].display_name):0, 0};
                igx_string_t email = {(char*)s->users[i].mail_address, strlen(s->users[i].mail_address), 0};
                igx_mime_user(working_buffer+n, sizeof(working_buffer)-1-n, &name, &email);
                ++added_count;
            }
        }
        APPEND_LINE_FEED();
        /* CC: */
        for (i=0; i<count; ++i)
        {
            if (s->users[i].type.cc==1)
            {
                if (added_count!=0)
                {
                    working_buffer[n++]=',';
                }
                else
                {
                    APPEND_STRING("CC: ", 4);
                }
                igx_string_t name = {(char*)s->users[i].display_name, s->users[i].display_name?strlen(s->users[i].display_name):0, 0};
                igx_string_t email = {(char*)s->users[i].mail_address, strlen(s->users[i].mail_address), 0};
                igx_mime_user(working_buffer+n, sizeof(working_buffer)-1-n, &name, &email);
                ++added_count;
            }
        }
        APPEND_LINE_FEED();
        /* BCC: */
        for (i=0; i<count; ++i)
        {
            if (s->users[i].type.bcc==1)
            {
                if (added_count!=0)
                {
                    working_buffer[n++]=',';
                }
                else
                {
                    APPEND_STRING("BCC: ", 5);
                }
                igx_string_t name = {(char*)s->users[i].display_name, s->users[i].display_name?strlen(s->users[i].display_name):0, 0};
                igx_string_t email = {(char*)s->users[i].mail_address, strlen(s->users[i].mail_address), 0};
                igx_mime_user(working_buffer+n, sizeof(working_buffer)-1-n, &name, &email);
                ++added_count;
            }
        }
        APPEND_LINE_FEED();
    }
    /* Subject: */
    {
        APPEND_STRING("Subject: ", 9);
        if (igx_mime_is_7bit(s->subject.str, s->subject.length)==false)
        {
            size_t size = sizeof(working_buffer)-n-1;
            igx_mime_encrypt_base64(working_buffer+n, &size, s->subject.str, s->subject.length, s->detail.charset);
            n+=size;
        }
        else
        {
            memcpy(working_buffer+n, s->subject.str, s->subject.length);
            n+=s->subject.length;
            APPEND_STRING(s->subject.str, s->subject.length);
        }
        APPEND_LINE_FEED();
        APPEND_LINE_FEED();
    }

    is_body_7bit = igx_mime_is_7bit(s->body.str, s->body.length);

    /** 内容が本文のみの場合は、multi-partsを使う必要がない。**/
    if (s->files_count==0)
    {
        char buffer[512];
        bit32_t result, n=0;
        bool_t is_plain=false;

        /** mime8をサポートしている場合はそのままでよい **/
        if (s->support.mime8==1)
        {
            n+=sprintf(working_buffer, "MIME-Version:1.0\r\nContent-type:text/plain; charset=%s\r\nContent-Transfer-Encoding: 8bit\r\n\r\n", s->detail.charset);
            is_plain=true;
        }
        /** bodyが7bitでできている場合もそのままで送れる **/
        else if (is_body_7bit==true)
        {
            n+=sprintf(working_buffer, "MIME-Version:1.0\r\nContent-type:text/plain; charset=%s\r\nContent-Transfer-Encoding: 7bit\r\n\r\n", s->detail.charset);
            is_plain=true;
        }
        /** そうでもない場合は、base64にする **/
        else
        {
            n+=sprintf(working_buffer, "MIME-Version:1.0\r\nContent-type:text/plain; charset=%s\r\nContent-Transfer-Encoding: base64\r\n\r\n", s->detail.charset);
        }
       __send_and_receive2(s, working_buffer, n, &result, buffer, sizeof(buffer));
        if (result!=250)
        {
            return kSmtpErrorData;
        }

        if (is_plain==true)
        {
            __send_and_receive2(s, s->body.str, s->body.length, 0, 0, 0);
        }
        else
        {
            size_t base64_size = ((s->body.length<<2)/3)+16;
            char* temp = (char*)__smtp_malloc(s, base64_size);
            igx_encrypt_base64(temp, &base64_size, s->body.str, s->body.length);
            __send_and_receive2(s, temp, base64_size, 0, 0, 0);
            __smtp_free(s, temp);
        }
    }
    else
    {
        /** multipartsの場合 **/
        bool_t is_plain=false;
        n+=sprintf(working_buffer, "MIME-Version:1.0\r\nContent-type:multiparts/mixed; boundary=%p-%p\r\n\r\n", working_buffer, s);
        __send_and_receive2(s, working_buffer, n, 0, 0, 0);
        n=0;

        if (s->support.mime8==1)
        {
            n = sprintf(working_buffer, "--%p-%p\r\nContent-Type=text/plain; charset=%s\r\nContent-Transfer-Encoding:8bit\r\n", 
                                                                                                working_buffer, s, s->detail.charset);
            is_plain=true;                                                                                                        
        }
        else if (is_body_7bit==true)
        {
            n = sprintf(working_buffer, "--%p-%p\r\nContent-Type=text/plain; charset=%s\r\nContent-Transfer-Encoding:7bit\r\n", 
                                                                                                working_buffer, s, s->detail.charset);
            is_plain=true;                                                                                                        
        }
        else
        {
            n = sprintf(working_buffer, "--%p-%p\r\nContent-Type=text/plain; charset=%s\r\nContent-Transfer-Encoding:base64\r\n", 
                                                                                                 working_buffer, s, s->detail.charset);
        }

        /** 本文のヘッダーを送る **/
        __send_and_receive2(s, working_buffer, n, 0, 0, 0);

        if (is_plain==true)
        {
            /** エンコードする必要がない場合、本文を送る **/
            __send_and_receive2(s, s->body.str, s->body.length, 0, 0, 0);
        }
        else
        {
            bit32_t i;
            size_t data_size   = s->body.length;
            size_t base64_size = ((s->body.length<<2)/3)+16;
            char* temp = (char*)__smtp_malloc(s, base64_size);
            igx_encrypt_base64(temp, &base64_size, s->body.str, s->body.length);

            /** 本文はbase64形式 **/
            __send_and_receive2(s, temp, base64_size, 0, 0, 0);

            /** ファイルを送る
            ファイルはすべてbase64形式で送る **/
            for (i=0; i<s->files_count; ++i)
            {
                if (s->files[i].data_size>data_size)
                {
                    __smtp_free(s, temp);
                    base64_size = ((data_size<<2)/3)+16;
                    temp = (char*)__smtp_malloc(s, base64_size);
                }
                n = sprintf(working_buffer, "\r\n--%p-%p\r\nContent-Type=%s;\r\nContent-Transfer-Encoding:base64\r\n", 
                                                                                                 working_buffer, s, __get_content_type(s->files[i].filename));
                igx_encrypt_base64(temp, &base64_size, s->files[i].data, s->files[i].data_size);
                __send_and_receive2(s, temp, base64_size, 0, 0, 0);
            }
            __smtp_free(s, temp);
        }
    } /* multi parts */

    return kSmtpErrorSuccess;

}
static igx_smtp_error_t __rcpt_to(__igx_smtp_t* s)
{
    char working_buffer[1024];
    bit32_t n=0, i, result;
    for (i=0;i<s->users_count; ++i)
    {
        if (sizeof(working_buffer)<n+16)
        {
            if (__send_and_receive(s, working_buffer, n, &result, working_buffer, sizeof(working_buffer))!=kSockErrorSuccess ||
                    result!=250)
            {
                /** TOの数超過？**/
                return kSmtpErrorRcptTo;
            }
            n=0;
        }
        n+=sprintf(working_buffer+n, "RCPT TO:%s\r\n", s->users[i].mail_address);
    }
    if (__send_and_receive(s, working_buffer, n, &result, working_buffer, sizeof(working_buffer))!=kSockErrorSuccess ||
            result!=250)
    {
        return kSmtpErrorRcptTo;
    }

    return kSmtpErrorSuccess;
}
igx_smtp_error_t igx_smtp_send(igx_smtp_t* smtp)
{
    HANDLE_CAST(s, smtp);
    bit32_t result;

    /* MAIL FROM: */
    if (__send_mail_from(s)!=kSmtpErrorSuccess)
    {
        return kSmtpErrorMailFrom;
    }
    /* RCPT TO: */
    if (__rcpt_to(s)!=kSmtpErrorSuccess)
    {
        return kSmtpErrorRcptTo;
    }
    /* DATA */
    if (__data(s)!=kSmtpErrorSuccess)
    {
        return kSmtpErrorRcptTo;
    }
    /** 終了 **/
    {
        char working_buffer[256];
        __send_and_receive2(s, kCmdEof.str, kCmdEof.length, &result, working_buffer, sizeof(working_buffer));
        if (result==250)
        {
            if (__send_and_receive(s, kCmdQuit.str, kCmdQuit.length, &result, working_buffer, sizeof(working_buffer))!=kSmtpErrorSuccess ||
                 result!=255)
            {
                return kSmtpErrorQuit;
            }
        }
        else
        {
            return kSmtpErrorData;
        }
    }

    return kSmtpErrorSuccess;
}
const char* igx_smtp_error_message(igx_smtp_error_t error_no)
{
    switch(error_no)
    {
        case kSmtpErrorSuccess: return "成功"; 
        case kSmtpErrorConnect: return "コネクトエラー";
        case kSmtpErrorWrite: return "送信エラー";
        case kSmtpErrorRead: return "受信エラー";
        case kSmtpErrorServer: return "サーバ側エラー";
        case kSmtpErrorLogin: return "ログインエラー";
        case kSmtpErrorId: return "ID不正";
        case kSmtpErrorPassword: return "パスワード不正";
        case kSmtpErrorRcptTo: return "TOエラー";
        case kSmtpErrorMailFrom: return "MAIL FROMエラー";
        case kSmtpErrorData: return "ボディ送信エラー";
        case kSmtpErrorQuit: return "終了エラー";
        default : return "その他エラー";
    }
    return "";
}
