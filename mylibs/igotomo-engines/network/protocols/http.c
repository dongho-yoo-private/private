#include "network/igx_sock.h"
#include "network/protocols/http.h"
#include "debug/igx_debug.h"
#include "library/igx_memory.h"
#include "library/igx_string.h"
#include "library/igx_stdlib.h"
#include "data_struct/igx_linked_list.h"

/** 内部で管理するhttpハンドル */
typedef struct {
    igx_sock_t sock; /* 64byte */
    igx_http_detail_t detail; /* 20 bytes */
    igx_http_error_t error;
    struct {
        char* content_type;
        char* charset;
        char* hostname;
        igx_addrv4_t addr;
    } header; /* 28 bytes */
    struct {
        HITX  page;
        HITX  list;
    } mem; /* 16 bytes */
    struct {
        char* header;
        char* body;
        bit32_t header_size;
        bit32_t body_size;
    } work; /* 24 bytes */
    igx_http_response_t response; /* 24 byes */
} __igx_http_intern_t;

typedef enum {
    kCommandTypePost,
    kCommandTypeGet,
    kCommandTypePut,
    kCommandTypeDel
} igx_http_command_type_t;

static const struct {
    const char* post;
    const char* get;
} __common_header =
{
    "POST %s%s HTTP/1.1\r\nHost:%s\r\nPort:%d\r\nAccept-Charset: %s\r\nContent-Type: %s\r\nContent-Length:%d\r\n",
    "GET %s%s HTTP/1.1\r\nHost:%s\r\nPort:%d\r\nAccept-Charset:%s\r\n"
};


#define _IGX_HTTP_INTERN_
#include "http.static.ic"

#define HANDLE_CAST(value, handle) __igx_http_intern_t* value = (__igx_http_intern_t*)handle

const char* igx_http_get_error_msg(igx_http_error_t code)
{
    switch(code)
    {
        case  kHttpErrorSuccess:
            return "no error";
        case kHttpErrorConnect:
            return "connect error";
        case kHttpErrorConnectTimeout:
            return "connect timeout";
        case kHttpErrorRequest:
            return "request error";
        case kHttpErrorResponse:
            return "response error";
        case kHttpErrorResponseTimeout:
            return "response timeout";
        case kHttpErrorUnknown:
            return "unknown";
        default:
            break;
    }

    return "";
}
igx_http_detail_t* igx_http_default_detail(igx_http_detail_t* detail)
{
    memset(detail, 0, sizeof(*detail));
    detail->port=80;
    detail->connect_timeout=10000;
    detail->read_timeout=10000;
    detail->keep_alive_timeout=0;
    detail->progress=0;
    detail->use_ssl=0;
    detail->use_mem_page=1;

    return detail;
}
igx_http_t* igx_http_create(igx_http_t* http, igx_addrv4_t addr, const char* hostname, igx_http_detail_t* detail, igx_http_error_t* error)
{
    HANDLE_CAST(s, http);
    igx_http_error_t _error;
    bit32_t _port=0;
    size_t hostname_len = hostname!=0?strlen(hostname):0;

    memset(s, 0, sizeof(*s));
    s->header.addr=addr;

#ifdef HTTP_DEBUG
    {
        /*char buffer[32];
        IGX_DEBUG("ip:%s\n", igx_sock_addr_2_ip(buffer, s->header.addr));*/
    }
#endif

    if (detail)
    {
        s->detail=*detail;
    }
    else
    {
        igx_http_default_detail(&s->detail);
    }

    /*if (s->detail.keep_alive_timeout!=0)*/
    {
        if (igx_http_connect(http)!=kHttpErrorSuccess)
        {
            *error = _error;
            return 0;
        }
    }
  
    *error = _error;

    if (s->detail.use_mem_page==1)
    {
        __init_mem_page(s);
    }


    if (hostname)
    {
        s->header.hostname = (char*)memcpy(__http_malloc(s, hostname_len+1), hostname, hostname_len+1);
    }
    
    return (igx_http_t*)s;
}
igx_http_error_t igx_http_connect(igx_http_t* http)
{
    HANDLE_CAST(s, http);
    igx_sock_error_t error;

    if (igx_sock_is_connected(&s->sock)==true)
    {
        return s->error = kHttpErrorSuccess;
    }

    if (igx_sock_connect(&s->sock, s->header.addr, s->detail.port, s->detail.connect_timeout*1000, s->detail.use_ssl, &error)==0)
    {
        return s->error = kHttpErrorConnect;
    }

    return s->error = kHttpErrorSuccess;
}
void igx_http_set_content_type(igx_http_t* http, const char* content_type)
{
   HANDLE_CAST(s, http);
   s->header.content_type=(char*)content_type;
   //IGX_INFO("content type : %s", s->header.content_type);
}
void igx_http_set_charset(igx_http_t* http, const char* charset)
{
   HANDLE_CAST(s, http);
   s->header.charset=(char*)charset;
}
void igx_http_add_header(igx_http_t* http, const char* key, const char* value)
{
    HANDLE_CAST(s, http);
    char buffer[4096];

    bit32_t n = sprintf(buffer, "%s:\"%s\"\r\n", key, value);
    igx_llist_add(s->mem.list, memcpy(__http_malloc(s, n+1), buffer, n+1), 0);
}
igx_http_error_t igx_http_post(igx_http_t* http, const char* path, const void* data, size_t data_size)
{
    HANDLE_CAST(s, http);
    char request_header[4096], *p_header, *p_path=(char*)path;
    size_t header_size;
    IGX_ASSERT(http);
    s->error = kHttpErrorSuccess;

RETRY:
    s->detail.redirect=0;
    p_header = __make_header(s, request_header, sizeof(request_header), p_path, kCommandTypePost, data_size, &header_size);
    s->error = igx_http_message(http, p_header, header_size, data, data_size);

    if (p_header!=(char*)request_header)
    {
        __http_free(s, p_header);
 
    }
    if (s->error==kHttpErrorRedirect)
    {
        if (__http_redirect(s, &p_path)==true)
        {
            goto RETRY;
        }
    }

    return s->error;
}
igx_http_error_t igx_http_get(igx_http_t* http, const char* path, size_t len)
{
    HANDLE_CAST(s, http);
    char request_header[4096], *p_header, *p_path = (char*)path;
    size_t header_size;
    IGX_ASSERT(http);
    s->error = kHttpErrorSuccess;

RETRY:
    s->detail.redirect=0;
    p_header = __make_header(s, request_header, sizeof(request_header), p_path, kCommandTypeGet, 0, &header_size);
    s->error = igx_http_message(http, p_header, header_size, 0, 0);

    if (p_header!=(char*)request_header)
    {
        __http_free(s, p_header);
    }

    if (s->error==kHttpErrorRedirect)
    {
        if (__http_redirect(s, &p_path)==true)
        {
            goto RETRY;
        }
    }

    return s->error;
}
igx_http_error_t igx_http_message(igx_http_t* http, const char* header, size_t header_size, const void* data, size_t size)
{
    HANDLE_CAST(s, http);
    IGX_ASSERT(s);

    s->error = __http_request(s, header, header_size, data, size);

    if (s->error!=kHttpErrorSuccess)
    {
        return s->error;
    }
    s->error = __http_response(s);

    return s->error;
}
const igx_http_response_t* igx_http_get_response(igx_http_t* http)
{
    HANDLE_CAST(s, http);
    return &s->response;
}
bool_t igx_http_is_connected(igx_http_t* http)
{
    HANDLE_CAST(s, http);
    return igx_sock_is_connected(&s->sock);
}
igx_http_error_t igx_http_get_last_error(igx_http_t* http)
{
    HANDLE_CAST(s, http);
    return s->error;
}
void igx_http_clear(igx_http_t* http)
{
    HANDLE_CAST(s, http);
    /* TODO. どのように使うかポリシーを考えるべき */
}
void igx_http_close(igx_http_t* http)
{
    HANDLE_CAST(s, http);
    __delete_mem_page(s);
    __delete_linked_list(s);
    igx_sock_close(&s->sock);
    memset(s, 0, sizeof(*s));
}
bool_t igx_http_url_parse(char* url, bool_t* protocol_is_ssl, bit32_t* port, char** host_name, char** path)
{
    char* p=(char*)url;
    char* protocol = strcasestr(url, "http"), *p_host_name=p, *p_port,*p_path; 
    bool_t no_path=false;

#define __INIT_STR(str) if (str!=0) *str=""
#define __INIT_INT(n) if (n!=0) *n=0

    /* 初期化 */
    __INIT_STR(host_name);
    __INIT_STR(path);
    __INIT_INT(port);
    __INIT_INT(protocol_is_ssl);

    if (protocol!=0)
    {
        char* p = protocol+4; /* + strlen("http") */
        bool_t found_s=false;
        if (*p=='s'||*p=='S')
        {
            ++p;
            found_s=true;
        }
        if (memcmp(p, "://", 3)==0)
        {
            if (protocol_is_ssl!=0)
            {
                if (found_s==true)
                {
                    *protocol_is_ssl=true;
                }
            }
            p+=3;
        }
        p_host_name=p;
    }

    /* この段階でpは "hostname:port/any/path?anyparameter"*/
    p_port = igx_str_ch(p_host_name, ':');
    if (p_port!=0)
    {
        if (port!=0)
        {
            if (sscanf(p_port, ":%d/", port)!=1)
            {
                no_path=true;
                if (sscanf(p_port, ":%d", port)!=1)
                {
                    //IGX_WARN("found not port! invalid url format %s", url);
                    //return false;
                }
            }
        }

        if (*port!=0)
        {
            *p_port=0; /* hostname\0port/any/path?anyparameter"*/
            p=++p_port;
        }
    }
    if (port!=0 && *port==0)
    {
        if (*protocol_is_ssl==true)
        {
            *port=kIGX_HTTPS_DEFAULT_PORT;
        }
        else
        {
            *port=kIGX_HTTP_DEFAULT_PORT;
        }
    }
    p=p_host_name;
    p_path = igx_str_ch(p, '/');
    if (p_path!=0)
    {
        /* "http://anyhonst.com:port" */
        *p_path=0; /* "http://anyhost.com\0port\0path*/
        ++p_path;
    }
    else
    {
        p_path="";
    }

    if (host_name!=0)
    {
        *host_name=p_host_name;
    }
    if (path!=0)
    {
        *path=p_path;
    }
   
    return true;
}


