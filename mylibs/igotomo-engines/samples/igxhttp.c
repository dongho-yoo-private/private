#include "igx_types.h"
#include "network/igx_sock.h"
#include "network/protocols/http.h"
#include "io/igx_file.h"
#include "debug/igx_debug.h"
#include "library/igx_memory.h"

/**
* Usage:
* igxscoket -a url -p port [-ssl]
*/
static void print_usage()
{
    IGX_INFO("igxhttp -a [url] -t [content type optional] -c [charset optiona] -h [file additional header option] -f [file post only]");
    IGX_INFO("ex ./igxhttp -a https://any.server.url.com:444/api/login -t application/x-www-form-urlencoded -f data.file -c utf-8");
}
static bool_t get_command(char** argv, char** url, char** content_type, char** charset, char** file, char** header)
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
            if (argv[i][1]=='t' && argv[i][2]==0)
            {
                ++i;
                if (argv[i]==0)
                {
                    IGX_ERROR("content type is not fount!");
                    return false;
                }
                *content_type=argv[i];
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
            if (argv[i][1]=='h' && argv[i][2]==0)
            {
                ++i;
                if (argv[i]==0)
                {
                    IGX_ERROR("header file is not fount!");
                    return false;
                }
                *header=argv[i];
                continue;
            }
            if (argv[i][1]=='f' && argv[i][2]==0)
            {
                ++i;
                if (argv[i]==0)
                {
                    IGX_ERROR("post data is not fount!");
                    return false;
                }
                *file=argv[i];
                continue;
            }

        } /* if - */
    } /* for */

    return (*url!=0);
}

int main(int argc, char** argv)
{
    char* url, *content_type, *charset, *data_file, *header_file;
    char* hostname, *path;
    bool_t use_ssl=false;
    bit32_t port=0;
    igx_http_t http;
    igx_http_detail_t detail;
    igx_http_error_t error;
    igx_addrv4_t addr;
    HITX hmem = igx_new_mem_page(0, 0, 8, true);
    char url_buffer[1024];

    IGX_DEBUG_CONSOLE(kLogLevelAll);

    if (get_command(argv, &url, &content_type, &charset, &data_file, &header_file)==false)
    {
        print_usage();
        exit(0);
    }

    strcpy(url_buffer, url);
    if (igx_http_url_parse(url_buffer, &use_ssl, &port, &hostname, &path)==false)
    {
        print_usage();
        exit(0);
    }
    igx_http_default_detail(&detail);
    detail.use_ssl = use_ssl;
    detail.connect_timeout=0;
    if (port!=0)
    {
       detail.port=port;
    }
    else if (port==0&&use_ssl==1)
    {
        detail.port=kIGX_HTTPS_DEFAULT_PORT; /* 443 */
    }
    addr = igx_sock_get_host_addr(hostname);
    if (igx_http_create(&http, addr, hostname, &detail, &error)==0)
    {
        exit(0);
    }

    if (content_type!=0)
    {
        igx_http_set_content_type(&http, content_type);
    }

    if (data_file!=0)
    {
        size_t size;
        void* p = igx_file_load(data_file, 0, &size, 0);
        //IGX_DEBUG("%s", (char*)data_file);
        //igx_log_dump("file!!!!!", p, size, 16);
        //IGX_DEBUG("%s", (char*)p);
        //printf("%s", (char*)p);
        //getchar();
        error = igx_http_post(&http, path, p, size);

        if (error!=kHttpErrorSuccess)
        {
            IGX_ERROR("%s", igx_http_get_error_msg(error));
        }
        igx_file_unload(p);
    }
    else
    {
        error = igx_http_get(&http, path, strlen(path));
    }

    if (error!=kHttpErrorSuccess)
    {
        IGX_ERROR("%s", igx_http_get_error_msg(error));
        exit(0);
    }

    {
        const igx_http_response_t* response = igx_http_get_response(&http);
        IGX_DEBUG("++++++++++++++++++++++ response (%d) ++++++++++++++++++++++ ", response->size);
        IGX_DEBUG_LONG("%s\n", response->data);
    }

    //igx_memory_print("*");
    //igx_mem_page_print_status();
    igx_http_close(&http);
    //igx_memory_print("*");
    //igx_mem_page_print_status();

    return error;

}
