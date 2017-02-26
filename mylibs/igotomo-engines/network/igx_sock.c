#include "igx_sock.h"
#include "library/igx_string.h"
#include "library/igx_stdlib.h"
#include "library/igx_memory.h"
#include "data_struct/igx_linked_list.h"

/** @brief BIO_系に置き換えると問題ない？ */
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#if defined(__POSIX__)
#   include <unistd.h>
#   include <errno.h>
#   include <fcntl.h>
#   include <netdb.h>
#   include <signal.h>
#   include <sys/socket.h>
#   include <sys/types.h>
#endif

#if defined(_USE_KQUEUE_)
#   include <unistd.h>
#   include <sys/event.h>
#endif
#if defined(WITH_ANDROID)
#   include <time.h>
#elif defined(WITS)
#   include <sys/time.h>
#endif

#if defined(_USE_OPEN_SSL_)
#   include <openssl/crypto.h>
#   include <openssl/err.h>
#   include <openssl/pem.h>
#   include <openssl/rand.h>
#   include <openssl/x509.h>
#endif


#include "debug/igx_debug.h"

#if defined(IGX_SOCK_DEBUG)
#   define SSL_ERROR_PRINT() ERR_print_errors_fp(stderr)
#else
#   define SSL_ERROR_PRINT() 
#endif

/**@brief SSL通信を隠蔽する為の処置、通信を行う際にはSSLを使っているかどうか分かる必要がないため*/
typedef igx_sock_error_t (*igx_read_fnc_t)(void* sock, char* buffer, size_t size, int timeout, float* progress);
typedef igx_sock_error_t (*igx_write_fnc_t)(void* sock, const char* buffer, size_t size, float* progress);
typedef igx_sock_error_t (*igx_read_once_fnc_t)(void* sock, char* buffer, size_t size, size_t* recved, int timeout);

/**
* @brief ソケット通信を行うハンドル
*/
typedef struct {
    /**@brief ソケットのディスクリプター*/
	int sock;
    /** @brief recv **/
	igx_read_fnc_t read;
    /** @brief recv once **/
	igx_read_once_fnc_t read_once;
    /** @brief write **/
	igx_write_fnc_t write;
#ifdef _USE_KQUEUE_
    /** @brief kqueueハンドル */
	int fd;
#endif
#ifdef _USE_OPEN_SSL_
    /** @brief for openssl */
	SSL* ssl;
    /** @brief for openssl */
	SSL_CTX* ctx;
#endif
} __igx_sockssl_t;

static struct {
    HITX list;
} __ipaddress_cache;

/* ++++++++++++++++++ local functions begin. +++++++++++++++++++++++++ */

static bit32_t is_valid_sock(int sock)
{
    bit32_t error=0;
    socklen_t len;

    if (sock==0)
    {
        return 0;
    }

    if (getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, &len)!=0)
    {
        return errno;
    }

    if (error==0 ||
            error==EISCONN)
    {
        return 1;
    }

    IGX_WARN("socket invalid reason:%d", error);
    return 0;
}
static bit32_t is_ready_connect(int sock, bit32_t timeout)
{
    bit32_t res=0;
    fd_set fds;
    struct timeval tv;

    FD_ZERO(&fds);
    FD_SET(sock, &fds);

    tv.tv_sec=timeout/1000;
    tv.tv_usec=(timeout%1000)*1000; // to msec

    res = select(sock+1, &fds, &fds, 0, &tv);

    if (res<0)
    {
        IGX_ERROR("select error errno:%d", errno);
        return res;
    }

    if (res==0)
    {
        return 0;
    }

    if (FD_ISSET(sock, &fds)!=0)
    {
        if (is_valid_sock(sock))
        {
            return 1;
        }
    }

    return 0;
}
#if defined(_USE_KQUEUE_)
static bit32_t wait_sock_for_kqueue(int sock, bit32_t* kq, bit32_t timeout)
{
    bit32_t kQueue = *kq?*kq:kqueue();
    bit32_t r;
    struct kevent kev, kev2;
    struct timespec _timeout = {timeout/1000, (timeout%1000)*1000000};

    if (*kq==0)
    {
        EV_SET(&kev, sock, EVFILT_READ, EV_ADD, 0, 0, NULL);
    
        if (kevent(kQueue, &kev, 1, 0, 0, 0) == -1)
        {
            xNP_DEBUG("kevent error!\n");
            close(kQueue);
            return -1;
        }
        *kq=kQueue;
    }

    r = kevent(kQueue, 0, 0, &kev2, 1, timeout==0?0:&_timeout);

    if (r==-1)
    {
        xNP_DEBUG("kevent wait error!\n");
        close(kQueue);
        return -1;
    }

    // timeout
    if (r==0)
    {
#if 0
        close(kQueue);
#endif
        return 0;
    }
#if 0
    close(kQueue);
#endif
    return 1;
}
#endif /*_USE_KQUEUE_*/

#if defined(_USE_EPOLL)
static bit32_t wait_sock_for_epoll(bit32_t sock, bit32_t timeout)
{
    return 0;
}
#endif /* _USE_KQUEUE_ */

#if defined(_USE_SELECT_)
static bit32_t wait_sock_for_select(int sock, bit32_t timeout)
{
    fd_set fds;
    struct timeval tv;

    FD_ZERO(&fds);
    FD_SET(sock, &fds);

    tv.tv_sec=timeout/1000;
    tv.tv_usec=(timeout%1000)*1000; // to msec

    bit32_t res = select(sock+1, &fds, 0, 0, &tv);

    if (res<0)
    {
        IGX_ERROR("select error errno:%d", errno);
        return res;
    }

    if (res==0)
    {
        return 0;
    }

    return 1;
}
#endif
static bit32_t wait_sock(int sock, bit32_t timeout/*milli seconds*/, int* descriptor)
{
    if (timeout==0)
    {
        return 1;
    }

#ifdef _USE_EPOLL_
    return wait_sock_for_epoll(sock, timeout);
#endif

#ifdef _USE_KQUEUE_
    return wait_sock_for_kqueue(sock, descriptor, timeout);
#endif

#ifdef _USE_SELECT_
    return wait_sock_for_select(sock, timeout);
#endif
}
static void sock_non_block(int sock, bit32_t nonblock)
{
    fcntl(sock, F_SETFL, nonblock?O_NONBLOCK:0);
}
static int connect_tcp(igx_addrv4_t address, bit32_t port, bit32_t timeout, int* fd)
{
    bit32_t sock;
    struct sockaddr_in addr;
    bit32_t val;
    bit32_t retryCnt=timeout/1000;

#ifndef __HAVE_MSG_NOSIGNAL__
    /* BSD系はこれを指定しないと、write時にサーバ側からデータがくるとシグナルが発生して、
    プロセスが終了する。*/
    signal(SIGPIPE, SIG_IGN);
#endif

    if ((sock= socket(AF_INET, SOCK_STREAM, 0))<0)    
    {
        IGX_CRITICAL("socket create error !");
        return kSockErrorUnknown;
    }

    if (timeout!=0)
    {
        val = 1;
        /*  タイムアウトにする場合には、一旦ソケットをノンブロック状態にする。*/
        sock_non_block(sock, 1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = address;

__RETRY:
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr))!=0)
    {
        if (timeout==0)
        {
            IGX_WARN("connect error!\n");
            return -1;
        }

        if (errno==EAGAIN)
        {
            IGX_WARN("connect EAGAIN\n");
            goto __RETRY;
        }
        if (errno==EINPROGRESS)
        {
            bit32_t res;
__WAIT:
            res = is_ready_connect(sock, 1000); 
            if (res<0)
            {
                IGX_ERROR("connect error... (%d)", errno);
                return -1;
            }
            if (res==0)
            {
                if (retryCnt--!=0)
                {
                    IGX_WARN("timeout so... retry");
                    goto __WAIT;
                }
                // retryCnt is zero.
                IGX_ERROR("connect timeout (%d)", errno);
                return 0;
            }

            sock_non_block(sock, 0);
            return sock;
        } /* if EINPROGRESS */
        return 0;
    } /* connect retry or failure. */
    
    //IGX_INFO("connect success");
    return sock;
}
static igx_sock_error_t __read(igx_sock_t* sock, char* buffer, size_t size, bit32_t timeout, float* fprogress)
{
    __igx_sockssl_t* s = (__igx_sockssl_t*)sock;
    bit32_t res; 
    bit32_t sum = 0;
    const bit32_t read_once_size = kIGX_SOCK_OPTIMIZED_PACKET_SIZE;


    if (fprogress!=0)
    {
        *fprogress=0;
    }
    
    while(sum!=size)
    {
        bit32_t n, ret;
#if defined(_USE_KQUEUE_)
        if ((res= wait_sock(s->sock, timeout, &s->fd))<0)
#else
        if ((res= wait_sock(s->sock, timeout, 0))<0)
#endif
        {
            return kSockErrorUnknown;
        }
        if (res==0)
        {
            return kSockErrorTimeout;
        }

        n = (size-sum)>read_once_size?read_once_size:(size-sum);
        ret = recv(s->sock, &buffer[sum], n, 0);
        sum+=ret;

        if (fprogress!=0)
        {
            *fprogress=((float)sum/(float)size);
        }
    }

    if (fprogress!=0)
    {
        *fprogress=1.0f;
    }

    return kSockErrorSuccess;
}
static igx_sock_error_t __write_4_ssl(__igx_sockssl_t* s, const char* buffer, bit32_t size, float* fprogress)
{
    bit32_t sum = 0;
    const bit32_t write_once_size = kIGX_SOCK_OPTIMIZED_PACKET_SIZE;

    if (fprogress)
    {
        *fprogress=0.0f;
    }

    while(sum!=size)
    {
        bit32_t n, ret;
        n = (size-sum)>write_once_size ?write_once_size:(size-sum);

_RETRY:
        ret = SSL_write(s->ssl, buffer+sum, n);

        if (n==SSL_ERROR_WANT_WRITE ||
                n==SSL_ERROR_WANT_READ)
        {
            goto _RETRY;
        }

        if (n<0)
        {
            SSL_ERROR_PRINT();
            return kSockErrorWrite;
        }

        if (n==0)
        {
            SSL_ERROR_PRINT();
            // TODO. これで良いかどうか
            return kSockErrorClosed;
        }

        sum+=ret;

        if (fprogress!=0)
        {
            *fprogress=((float)sum/(float)size);
        }
    }

    if (fprogress!=0)
    {
        *fprogress=1.0f;
    }

    return kSockErrorSuccess;

}
static igx_sock_error_t __write(igx_sock_t* sock, const char* buffer, bit32_t size, float* fprogress)
{
    __igx_sockssl_t* s = (__igx_sockssl_t*)sock;
    bit32_t sum = 0;
    const bit32_t write_once_size = kIGX_SOCK_OPTIMIZED_PACKET_SIZE;

    if (fprogress)
    {
        *fprogress=0.0f;
    }
    while(sum!=size)
    {
        bit32_t n, ret;

        n = (size-sum)>write_once_size?write_once_size:(size-sum);
#ifdef __HAVE_MSG_NOSIGNAL__
        /* BSD系でない場合は、これでOK */
        ret = send(s->sock, &buffer[sum], n, MSG_NOSIGNAL);
#else
        ret = send(s->sock, &buffer[sum], n, 0);
#endif
        sum+=ret;

        if (fprogress!=0)
        {
            *fprogress=((float)sum/(float)size);
        }
    }
    if (fprogress!=0)
    {
        *fprogress=1.0f;
    }

    return kSockErrorSuccess;
}
static igx_sock_error_t __read_once(igx_sock_t* sock, char* buffer, size_t size, bit32_t* received, bit32_t timeout)
{
    __igx_sockssl_t* s = (__igx_sockssl_t*)sock;
    bit32_t res; 

    *received=0;
#if defined(_USE_KQUEUE_)
    if ((res= wait_sock(s->sock, timeout, &s->fd))<0)
#else
    if ((res= wait_sock(s->sock, timeout, 0))<0)
#endif
    {
        return kSockErrorUnknown;
    }
    if (res==0)
    {
        return kSockErrorTimeout;
    }
    {
        bit32_t n = recv(s->sock, buffer, size, 0);
        if (n==0)
        {
            return kSockErrorClosed;;
        }
        if (n<0)
        {
            return kSockErrorRead;;
        }
        if (received!=0)
        {
            *received=n;
        }
    }

    return kSockErrorSuccess;
}
static igx_sock_error_t __read_once_4_ssl(igx_sock_t* sock, char* buffer, size_t size, bit32_t* received, bit32_t timeout)
{
    __igx_sockssl_t* s = (__igx_sockssl_t*)sock;
    bit32_t n=0, res;

    if (received!=0)
    {
        *received=0;
    }

#if defined(_USE_KQUEUE_)
    if ((res=wait_sock(s->sock, timeout, &s->fd))==0)
#else
    if ((res=wait_sock(s->sock, timeout, 0))==0)
#endif
    {
        return kSockErrorTimeout;
    }
    else if (res<0)
    {
        return kSockErrorUnknown;
    }

    {
RETRY:
        n = SSL_read(s->ssl, buffer, size);

#if 1    // SSL_pendingを使う場合は以下は要らない？
        if (n==SSL_ERROR_WANT_READ ||
                n==SSL_ERROR_WANT_WRITE)
        {
            IGX_DEBUG("WANT_READ...... %d\n", n);
            goto RETRY;
        }
#endif
        if (n==0)
        {
            return kSockErrorTimeout;
        }
        if (n<0)
        {
            return kSockErrorRead;
        }
    }

    if (received!=0)
    {
        *received=n;
    }
    return kSockErrorSuccess;
}
static igx_sock_error_t __read_4_ssl(igx_sock_t* sock, char* buffer, size_t size, bit32_t timeout, float* fprogress)
{
    __igx_sockssl_t* s = (__igx_sockssl_t*)sock;
    bit32_t sum = 0;
    bit32_t res;

    while(sum!=size)
    {
        bit32_t n;
#if defined(_KQUEUE_)
        if ((res=wait_sock(s->sock, timeout, &s->fd))==0)
#else
        if ((res=wait_sock(s->sock, timeout, 0))==0)
#endif
        {
            return kSockErrorTimeout;
        }
        else if (res<0)
        {
            return kSockErrorClosed;
        }

        //if (pending=SSL_pending(s->ssl)) -> 意味ない
        {
RETRY:
            n = SSL_read(s->ssl, &buffer[sum], size-sum);

#if 0        // SSL_pendingを使う場合は以下は要らない？
            if (n==SSL_ERROR_WANT_READ ||
                    n==SSL_ERROR_WANT_WRITE)
            {
                // sleep 入れるべき？
                goto RETRY;
            }
#endif
        }

        if (n==0)
        {
            // EOF
            return kSockErrorRead;
        }
        if (n<0)
        {
            // EOF
            return kSockErrorClosed;
        }

        sum+=n;
        
        if (fprogress!=0)
        {
            *fprogress=((float)sum/(float)size);
        }
    }

    if (fprogress!=0)
    {
        *fprogress=1.0;
    }

    return kSockErrorSuccess;
}

struct __addr_cache {
    igx_addrv4_t addr;
    size_t hostname_len;
    char hostname[4];
};
static igx_addrv4_t __search_host_name(const char* hostname)
{
    igx_llist_node_t* node;
    size_t len;
    if (__ipaddress_cache.list==0)
    {
        return 0;
    }
    len = strlen(hostname);
    node = igx_llist_get_top_from(__ipaddress_cache.list, 0);
    for (;node;node=node->next)
    {
        struct __addr_cache* addr = (struct __addr_cache*)node;
        if (addr->hostname_len==len &&
                    strcmp(addr->hostname, hostname)==0)
        {
            return addr->addr;
        }
    }
    return 0;
}
static bit32_t __register_hostname(const char* hostname, igx_addrv4_t address)
{
    struct __addr_cache* addr;
    size_t len = strlen(hostname);
    if (__ipaddress_cache.list==0)
    {
        __ipaddress_cache.list = igx_llist_new(32, true, 0, 0);
        IGX_ASSERT(__ipaddress_cache.list);
    }
    addr = (struct __addr_cache*)IGX_MALLOC(len+sizeof(igx_addrv4_t)+1 ,"__register_host_name");
    memcpy(addr->hostname, hostname, len+1);
    addr->addr=address;
    addr->hostname_len = len;
    return igx_llist_add(__ipaddress_cache.list, addr, 0);
}

/* ++++++++++++++++++ local functions end. +++++++++++++++++++++++++ */


igx_addrv4_t igx_sock_get_host_addr(const char* hostname)
{
    bit32_t cnt=0, r;
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    struct sockaddr_in *addr_in;
    igx_addrv4_t ret = igx_sock_ip_2_addr(hostname);

    if (ret!=0)
    {
        return ret;
    }
    ret = __search_host_name(hostname);
    if (ret!=0)
    {
        return ret;
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC; //IPv4とIPv6どちらでもOK
    hints.ai_socktype = SOCK_STREAM; //TCP
    hints.ai_flags = 0;
    hints.ai_protocol = 0; //Any protocol

    //名前解決
    ret = getaddrinfo(hostname, 0, &hints, &result);

    if (ret!=0)
    {
        return 0;
    }

    for (rp = result; rp != 0; rp = rp->ai_next) 
    {
        addr_in = (struct sockaddr_in*)rp->ai_addr;    
        ret =addr_in->sin_addr.s_addr;
        if (ret!=0)    
            cnt++;
    }

    if (cnt==0)
    {
        freeaddrinfo(result);
        return 0;
    }

    srand(time(0));
    r = rand();
    r = r%cnt;
    cnt=0;

    /* ホストのIPが複数ある場合には、ランダムでどれかを選ぶ*/
    for (rp = result; rp != NULL; rp = rp->ai_next) 
    {
        addr_in = (struct sockaddr_in*)rp->ai_addr;
        if (r==cnt)
        {
            ret=addr_in->sin_addr.s_addr;
            break;
        }
        cnt++;
    }
    freeaddrinfo(result);
    __register_hostname(hostname, ret);

    return ret;
}
igx_addrv4_t igx_sock_ip_2_addr(const char* ip)
{
    bit32_t a[4];

    /* error check */
    if (sscanf(ip, "%d.%d.%d.%d", &a[0], &a[1], &a[2], &a[3])!=4)
    {
        return 0;

    }
    return inet_addr(ip);
}
char* igx_sock_addr_2_ip(igx_addrv4_t addr, char* ip)
{
    static char buffer[32];
    ubit8_t* p = (unsigned char*)&addr;

    sprintf(ip=ip?ip:buffer, "%u.%u.%u.%u", (ubit32_t)p[0], (ubit32_t)p[1], (ubit32_t)p[2], (ubit32_t)p[3]);
    return ip;
}
igx_sock_error_t igx_sock_negotiation(igx_sock_t* _sock)
{
    __igx_sockssl_t* s=(__igx_sockssl_t*)_sock;
    igx_sock_error_t error = kSockErrorSuccess;
    bit32_t nRand, i=0;
    SSL_CTX *ctx;
    SSL    *sslx;
    const char *seed = "ZPJSHD2HSDopHSFewqAJKAJKytFrJKAFuKytJAFJKAS4DFJ";
    const size_t seedLen = strlen(seed);
    char seedBuff[128], temp[16];
 
    if (s->ssl!=0)
    {
        return kSockErrorUnknown;
    }

    IGX_INFO("ssl!");

    srand(time(0));
    nRand = rand();

    /* まずはランダム公開キーを作成する */
    strcpy(seedBuff, seed);
    sprintf(temp, "%08x", nRand);

    /* 全部変えるのは効率が悪いから部分だけ変える。
     これで十分*/
    for (i=0; i<8; i++)
    {
        if (temp[i]<'A' || temp[i]>'z')
        {
            temp[i]-='0';
            temp[i]+='A';
        }
    } // for

    /* 乱数でシードをその都度生成する。
     このシードの値に基づいてopensslの中でパケットをエンコードする。*/
    strcpy(seedBuff, seed);
    strcpy(&seedBuff[seedLen-8], temp);

    RAND_seed(seedBuff, strlen(seedBuff));

    /* SSL ライブラリを初期化 */
    SSLeay_add_ssl_algorithms();
    /* これによって、エラー時にエラーの文字列が得られる。
     デバッグ時のみすべきかどうか。。。*/
#ifdef IGX_SOCK_DEBUG
     SSL_load_error_strings();
#endif
    /* 未だにこれが必要かどうか分からない。
       なくても良いような。。。*/
    //SSL_library_init();

    /* SSLのプロトコルを設定する。
       サーバ側がSSL2, SSL3、TLS1をサポートすれば大丈夫。*/
    ctx = SSL_CTX_new(SSLv23_client_method());

    if (ctx==0)
    {
        error=kSockErrorSSL;
        close(s->sock);
        IGX_CRITICAL("igx_sock_Connect() SSL_INVALID_VERSION");
        return error;
    }

    /* これはSSL通信時にエラーなどでnegotiationが再度必要になる場合
       リトライするかどうかを設定させるか、そのままエラーを返して終わらすか。
       設定させる方が無難 */
    SSL_CTX_set_mode(ctx, SSL_MODE_AUTO_RETRY);

    /* 上記の設定に基づいてsslの構造体を生成する。*/
    sslx = SSL_new(ctx);

    /* openSSLに先ほどコネクトしたディスクリプターを設定 */
    SSL_set_fd(sslx, s->sock);

    IGX_DEBUG("negotiation start!");
    {
        bit32_t ret = SSL_connect(sslx);

        /* handshakeまで行ったがプロトコルに問題のある場合、*/
        if (ret<=0)
        {
            error=kSockErrorSSL;
            SSL_ERROR_PRINT();
            close(s->sock);
            SSL_shutdown(sslx);
            SSL_free(sslx);
            SSL_CTX_free(ctx);
            IGX_CRITICAL("SSL Connect Error!");
            return error;
        }
    }

    IGX_ASSERT(s);
    s->ssl=sslx;
    s->ctx=ctx;

    s->read = (igx_read_fnc_t)__read_4_ssl;
    s->write =(igx_write_fnc_t)__write_4_ssl;
    s->read_once=(igx_read_once_fnc_t)__read_once_4_ssl;
    IGX_DEBUG("negotiation success!");

    return error;
}
igx_sock_t* igx_sock_connect(igx_sock_t* _sock, igx_addrv4_t addr, bit32_t port, bit32_t timeout/*secs*/, bool_t use_ssl, igx_sock_error_t* err)
{
    bit32_t fd=0;
    bit32_t sock = connect_tcp(addr, port, timeout, &fd);
    __igx_sockssl_t* s=(__igx_sockssl_t*)_sock;

    if (sock<0)
    {
        *err=kSockErrorConnect;
        return 0;
    }

    if (sock==0)
    {
        *err=kSockErrorTimeout;;
        return 0;
    }
    if (use_ssl==0)
    {
        IGX_ASSERT(s);
        memset(s, 0, sizeof(*s));
        s->sock=sock;
        s->read=(igx_read_fnc_t)__read;
        s->write=(igx_write_fnc_t)__write;
        s->read_once=(igx_read_once_fnc_t)__read_once;
    }
    else
    {
        igx_sock_error_t error;
        memset(s, 0, sizeof(*s));
        s->sock=sock;
        if ((*err=igx_sock_negotiation((igx_sock_t*)s))!=kSockErrorSuccess)
        {
            return 0;
        }
    }

#ifdef _USE_KQUEUE_
        s->fd=fd;
#endif
    return (igx_sock_t*)s;
}
igx_sock_error_t igx_sock_read(igx_sock_t* sock, char*buffer, bit32_t size, bit32_t timeout, float* progress)
{
    __igx_sockssl_t* s = (__igx_sockssl_t*)sock;
    IGX_ASSERT(s);
    return s->read((void*)s, buffer, size, timeout, progress);
}
igx_sock_error_t igx_sock_read_once(igx_sock_t* sock, char*buffer, size_t size, size_t* received, bit32_t timeout)
{    
    __igx_sockssl_t* s = (__igx_sockssl_t*)sock;
    IGX_ASSERT(s);
    if (received!=0)
    {
        *received=0;
    }
    return s->read_once((void*)s, buffer, size, received, timeout);
}
igx_sock_error_t igx_sock_write(igx_sock_t* sock, const char*buffer, size_t size, float* progress)
{
    __igx_sockssl_t* s = (__igx_sockssl_t*)sock;
    IGX_ASSERT(s);
    return s->write((void*)sock, buffer, size, progress);
}
void igx_sock_close(igx_sock_t* sock)
{
    __igx_sockssl_t* s=(__igx_sockssl_t*)sock;

    if (s==0)
    {
        return ;
    }

    if (s->ssl!=0)
    {
        SSL_shutdown(s->ssl);
        SSL_free(s->ssl);
        s->ssl=0;
    }
    if (s->ctx!=0)
    {
        SSL_CTX_free(s->ctx);
        s->ctx=0;
    }

#ifdef _USE_KQUEUE_
    if (s->fd!=0)
    {
        close(s->fd);
        s->fd=0;
    }
#endif

    if (s->sock!=0)
    {
        close(s->sock);
    }
}
bool_t igx_sock_is_connected(igx_sock_t* sock)
{
    return (sock==0)?0:is_valid_sock(((__igx_sockssl_t*)sock)->sock);
    
#if 0
    bit32_t timeout=0;
    __igx_sockssl_t* s = (__igx_sockssl_t* )hSock;
    bit32_t res = wait_sock(hSock, timeout, &s->fd);

    if (res==0)
        return true;

    return false;
#endif
}
bool_t igx_sock_wait(igx_sock_t* sock, bit32_t timeout)
{
    __igx_sockssl_t* s = (__igx_sockssl_t*)sock;

#if defined(_KQUEUE_)
    if (wait_sock(s->sock, timeout, &s->fd)<=0)
#else
    if (wait_sock(s->sock, timeout, 0)<=0)
#endif
    {
        return false;
    } 
    return true;    
}
/* ここの関数はアルゴリズムの検証必要あり。
   様々なパターンのurlに対応できているかどうか。*/
const char* igx_sock_url_parse(const char* url, char* protocol, char* hostname, bit32_t* port)
{
    char* p = (char*)url;
    char* url_start;
    bit32_t len=0;
    char* p_port;
    char* start_path_addr=0;

    if (protocol)
        *protocol=0;
    if (hostname)
        *hostname=0;
    if (port)
        *port=0;
    
    p = strstr(url, "://");

    if (p)
    {
        const bit32_t len=(p-url);
        if (protocol)
        {
          memcpy(protocol, url, len);
          protocol[len]=0;
        }
    }

    p = p?(p+3):(char*)url;
    url_start = p;

    while(*p!='/' && *p!=0) p++;

    if ((len=p-url_start)==0)
    {
        return 0;
    }

    /** http://xxxxxxxxxx/bbbbbb or http://xxxxxxxxx */
    if (*p=='/' || *p==0)
    {
        if (*p!=0)
        {
            start_path_addr=p;
        }
    }
    
    p_port = igx_str_ch(url_start, ':');
    
    if (p_port==0)
    {
        if (hostname)
        {
            memcpy(hostname, url_start, len);
            hostname[len]=0;
        }
        return start_path_addr;
    }
    else
    {
        if (hostname)
        {
            igx_strcpy_until_c(hostname, url_start, ':');
        }
    }
    
    {
        char  str_port[16]={0, };
            
        len = p_port-url_start;
        igx_strcpy_until_c(str_port, ++p_port, '/');
        
        if (str_port[0]==0)
        {
          strcpy(str_port, p_port);
        }
        
        if (port!=0)
        {
            if (igx_atoi(str_port, port)==false)
            {
                *port=0;
            }
        }
        
        return start_path_addr;
    }
  return start_path_addr;
}

