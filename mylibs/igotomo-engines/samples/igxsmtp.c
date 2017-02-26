#include "network/protocol/smtp.h"

static void print_usage()
{
    IGX_INFO("igxsmtp -a [url] -c [charset optiona] -p [port option], -ssl");
    IGX_INFO("ex ./igxsmtp -a any.server.url.com ");
}
static bool_t get_command(const char** argv, char** url, int* port, char** charset, bool_t* use_ssl)
{
    *url=0;
    *content_type=0;
    *charset=0;
    *file=0;
    *header=0;
    for (int i=1;argv[i]!=0; ++i)
    {
        if (argv[i][0]=='-')
        {
            if (argv[i][1]=='a' && argv[i][2]==0)
            {
                ++i;
                if (argv[i]==0)
                {
                    IGX_ERROR("url is not fount!");
                    return false;
                }
                *url = argv[i];
                continue;
            }
            if (argv[i][1]=='p' && argv[i][2]==0)
            {
                ++i;
                if (argv[i]==0)
                {
                    IGX_ERROR("port is not fount!");
                    return false;
                }
                atoi(argv[i]);
                continue;
            }
            if (argv[i][1]=='c' && argv[i][2]==0)
            {
                ++i;
                if (argv[i]==0)
                {
                    IGX_ERROR("charset is not fount!");
                    return false;
                }
                *charset=argv[i];
                continue;
            }
            if (argv[i][1]=='s' && argv[i][2]=='s' && srgv[i][3]=='l')
            {
                *use_ssl=true;
            }
        } /* if - */
    } /* for */

    return (*url!=0);
}

int main(int argc, const char** argv)
{
    char* url, *charset;
    int port = 0;
    bool_t use_ssl=false;
    igx_smtp_t smtp;
    igx_smtp_detail_t detail;
    char buffer[256];
    char received[256];
    igx_mime_user_t users[16];
    bit32_t user_count=0;

     IGX_DEBUG_CONSOLE(kLogLevelAll);

    if (get_command(argv, &url, &port, &charset, &use_ssl)==false)
    {
        print_usage();
        exit(0);
    }

    igx_smtp_default_detail(&detail);
    detail.port = (port!=0)?port:detai.port;
    detail.charset = (charset!=0)?charset:detail.charset;

    if (igx_smtp_create(&smtp, ,port, &detail, &error)==0)
    {
        IGX_WARN("igx_smtp_create failure!");
        exit(0);
    }

    /* login */
    {
        char user[128], pass[128];
        printf("user: ");
        scanf("%s", user);
        printf("password: ");
        scanf("%s", pass);
        if (igx_smtp_login(smtp, id, pass)!=kSmtpErrorSuccess)
        {
            users[user_count++].
            return 0;
        }
    }

TO:
    printf("TO: ");
    igx_get_string_from_io(fileno(stdin), buffer, sizeof(buffer), &received, '\n');

    if (received!=0)
    {
        goto TO;        
    }
    
}
