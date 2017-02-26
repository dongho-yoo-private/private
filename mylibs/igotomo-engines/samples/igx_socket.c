#include "igx_types.h"
#include "network/igx_sock.h"
#include "io/igx_file.h"
#include "debug/igx_debug.h"
#include "library/igx_stdlib.h"

/**
* Usage:
* igxscoket -a url -p port [-ssl]
*/
static void print_usage()
{
    IGX_INFO("igxsocket -a [url] -p [port] -ssl [optional]");
}
static bool_t get_command(char** argv, char* url, bit32_t* port, bool_t* use_ssl)
{
    *url=0;
    *port=0;
    *use_ssl=0;
    for (int i=1;argv[i]!=0; ++i)
    {
        if (strcmp(argv[i], "-ssl")==0)
        {
            *use_ssl=true;
            continue;
        }
        if (argv[i][0]=='-')
        {
            if (argv[i][1]=='a' && argv[i][2]==0)
            {
                ++i;
                if (argv[i]==0)
                {
                    return false;
                }
                strcpy(url, argv[i]);
                continue;
            }
            if (argv[i][1]=='p' && argv[i][2]==0)
            {
                ++i;
                if (argv[i]==0)
                {
                    return false;
                }
                if (igx_atoi(argv[i], port)==false)
                {
                    return false;
                }
            }
            if (argv[i][0]=='s' && argv[i][1]=='s'&& argv[i][2]=='l' && argv[i][3]==0)
            {
                *use_ssl=true;
            }
        } /* if - */
    } /* for */

    IGX_DEBUG("url:%s, port:%d", url, *port);
    return (*url!=0 && *port != 0);
}

int main(int argc, char** argv)
{
    char url[1024];
    bit32_t port;
    bool_t use_ssl;
    igx_addrv4_t addr;
    igx_sock_t sock;
    igx_sock_error_t err;
    char buffer[8191];
    size_t received;

    igx_log_init(kLogOutputConsole,kLogLevelAll, 0);

    if (get_command(argv, url, &port, &use_ssl)==false)
    {
        print_usage();
        exit(0);
    }

    IGX_DEBUG("search host %s", url);
    addr = igx_sock_get_host_addr(url);

    if (addr==0)
    {
        IGX_ERROR("host %s is not found!", url);
        exit(0);
    }
    IGX_INFO("address %s", igx_sock_addr_2_ip(addr, buffer));

    if (igx_sock_connect(&sock, addr, port, 15000, use_ssl, &err)==0)
    {
        IGX_ERROR("connect %s failure! (%d)", url, err);
        exit(0);
    }
    IGX_INFO("connect success! ");

    while (true)
    {
        if ((err=igx_sock_read_once(&sock, buffer, sizeof(buffer), &received, 5000))!=kSockErrorSuccess)
        {
            if (err!=kSockErrorTimeout)
            {
                if (err==kSockErrorClosed)
                {
                    IGX_INFO("connect closed from remote");
                    break;
                }
                IGX_ERROR("read failure! (%d)", err);
                exit(0);
            }
            IGX_DEBUG("read timeout");
        }
    
        buffer[received]=0;
        IGX_INFO("%s", buffer);
   
RETRY:
        dprintf(fileno(stdout),".quit is exit\n");
        dprintf(fileno(stdout),"input> ");
        igx_get_string_from_io(fileno(stdin), buffer, sizeof(buffer), &received, '\n');
        if (received==0)
        {
            goto RETRY;
        }

        if (strncmp(buffer, ".quit", 5)==0)
        {
            break;
        }
        if (strncmp(buffer, ".ssl", 3)==0)
        {
            if (igx_sock_negotiation(&sock)==kSockErrorSuccess)
            {
                IGX_INFO("ssl negotiation successfull!");
                goto RETRY;
            }    
            IGX_WARN("ssl negotiation failure!");
            goto RETRY;
        }
        buffer[received-1]='\r';
        buffer[received]='\n';
        buffer[received+1]=0;
        igx_log_dump("send", buffer, received+1, 16);
    
        IGX_INFO("send %s", buffer);
        if (igx_sock_write(&sock, buffer, received+1, 0)!=kSockErrorSuccess)
        {
            IGX_ERROR("write failure! (%d)", err);
            exit(0);
        }
    }
    igx_sock_close(&sock);
    return 0;
}
