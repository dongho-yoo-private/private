#include <network/http/intern/np_cmn.h>

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <core/debug/debug_assert.h>
#include <core/debug/logging.h>
#include <core/library/lib_string.h>
#include <network/http/adk_np_types.h>
#include <network/http/intern/np_types.h>

//#include <debug_assert.h>
//#define xNP_ASSERT ADK_ASSERT


//#define __IAM_NPCMN__
//#include <npc.intern.h>




// -----------------------------------------------------------
static int xIsValidSock(int sock)
{
	int error=0;
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

	xSSL_DEBUG("socket invalid reason:%d\n", error);
	return 0;
	
}



// ------------------------------------------------------------ static fucntionz for npCmn
static int xIsReadyConnect(int sock, int timeout)
{
	int res=0;
	fd_set fds;
	struct timeval tv;

	FD_ZERO(&fds);
	FD_SET(sock, &fds);

	tv.tv_sec=timeout/1000;
	tv.tv_usec=(timeout%1000)*1000; // to msec

	res = select(sock+1, &fds, &fds, 0, &tv);

	if (res<0)
	{
		xNP_DEBUG("select error errno:%d\n", errno);
		return res;
	}

	if (res==0)
	{
		return 0;
	}

	if (FD_ISSET(sock, &fds)!=0)
	{
		if (xIsValidSock(sock))
		{
			return 1;
		}
	}

	return 0;
}

#ifdef _USE_KQUEUE_
#include <unistd.h>
#include <sys/event.h>
#if defined(WITH_ANDROID)
# include <time.h>
#elif defined(WITS)
# include <sys/time.h>
#endif
#include <fcntl.h>

static int xWaitSockForKqueue(int sock, int* kq, int timeout)
{
	int kQueue = *kq?*kq:kqueue();
	int r;
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

#ifdef _USE_EPOLL
// --------------------------------- static fucntionz
int xWaitSockForEPoll(int sock, int timeout)
{
	return 0;
}
#endif /* _USE_KQUEUE_ */

#ifdef _USE_SELECT_
// --------------------------------- static fucntionz
int xWaitSockForSelect(int sock, int timeout)
{
	fd_set fds;
	struct timeval tv;

	FD_ZERO(&fds);
	FD_SET(sock, &fds);

	tv.tv_sec=timeout/1000;
	tv.tv_usec=(timeout%1000)*1000; // to msec

	int res = select(sock+1, &fds, 0, 0, &tv);

	if (res<0)
	{
		xNP_DEBUG("select error errno:%d\n", errno);
		return res;
	}

	if (res==0)
	{
		return 0;
	}

	return 1;
}
#endif

// --------------------------------- static fucntionz
static int xWaitSock(int sock, int timeout/*milli seconds*/, int* descriptor)
{
	if (timeout==0)
	{
		return 1;
	}

#ifdef _USE_EPOLL_
	return xWaitSockForKEPoll(sock, timeout);
#endif

#ifdef _USE_KQUEUE_
	return xWaitSockForKqueue(sock, descriptor, timeout);
#endif

#ifdef _USE_SELECT_
	return xWaitSockForSelect(sock, timeout);
#endif
		
}

// -----------------------------------------------------------
static void xSockNonBlock(int sock, int nonblock)
{
	fcntl(sock, F_SETFL, nonblock?O_NONBLOCK:0);
}

// -----------------------------------------------------------
// コネクト時のタイムアウトが制御できず。
// どこかで修正する必要がある。
static int xConnect(address_t address, int port, int timeout, int* fd)
{
	int sock;
	struct sockaddr_in addr;
	int val;
	int retryCnt=timeout/1000;

#ifndef __HAVE_MSG_NOSIGNAL__
	signal(SIGPIPE, SIG_IGN);
#endif

	if ((sock= socket(AF_INET, SOCK_STREAM, 0))<0)	
	{
		xNP_DEBUG("socket create error !");
		return -1;
	}

	if (timeout!=0)
	{
		val = 1;
		// ソケットをノンブロック状態にする。
		//ioctl(sock, FIONBIO, &val);
		//fcntl(sock, F_SETFL, O_NONBLOCK);
		xSockNonBlock(sock, 1);
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
			xNP_DEBUG("connect error!\n");
			return -1;
		}

		if (errno==EAGAIN)
		{
			xNP_DEBUG("connect EAGAIN\n");
			goto __RETRY;
		}
		if (errno==EINPROGRESS)
		{
			int res;
			xNP_DEBUG("EINPROGRESS...... so retry....%d\n", (timeout/1000));
__WAIT:
			res = xIsReadyConnect(sock, 1000); // wait 1secs.
			if (res<0)
			{
				xNP_DEBUG("connect error... (%d)\n", errno);
				return -1;
			}
			if (res==0)
			{
				if (retryCnt--!=0)
				{
					xNP_DEBUG("timeout so... retry\n");
					goto __WAIT;
				}
				// retryCnt is zero.
				xNP_DEBUG("connect timeout (%d)\n", errno);
				return 0;
			}

			//xNP_DEBUG("connect success...\n");
			xSockNonBlock(sock, 0);
			return sock;
		} /* if EINPROGRESS */
		return 0;
	} // connect retry or failure.

	// connect success.
	//xNP_DEBUG("connect success...\n");
	return sock;
}

// ------------------------------------
static TNPError xWrite4SSL(SockPtr sock, const char* buffer, int size, float* fprogress)
{
	sockssl_t* s = (sockssl_t*)sock;
	int sum = 0;

	if (fprogress)
		*fprogress=0;

	while(sum!=size)
	{
		int n, ret;

		n = (size-sum)>xNP_OPTIMIZE_PACKET_SIZE ?xNP_OPTIMIZE_PACKET_SIZE :(size-sum);

_RETRY:
		ret = SSL_write(s->ssl, &buffer[sum], n);

		if (n==SSL_ERROR_WANT_WRITE ||
				n==SSL_ERROR_WANT_READ)
		{
			goto _RETRY;
		}

		if (n<0)
		{
			xPRINT_SSL_ERROR();
			return kNPE_WRITE;
		}

		if (n==0)
		{
			xPRINT_SSL_ERROR();
			// TODO. これで良いかどうか
			return kNPE_PEER;
		}

		sum+=ret;

		if (fprogress)
			*fprogress=100.0*((float)sum/(float)size);
	}

	if (fprogress)
		*fprogress=100.0;

	return kNPE_SUCCESS;

}

// ------------------------------------
static TNPError xWrite(SockPtr sock, const char* buffer, int size, float* fprogress)
{
	sockssl_t* s = (sockssl_t*)sock;
	int sum = 0;

	if (fprogress)
		*fprogress=0;

	while(sum!=size)
	{
		int n, ret;

		n = (size-sum)>xNP_OPTIMIZE_PACKET_SIZE ?xNP_OPTIMIZE_PACKET_SIZE :(size-sum);
#ifdef __HAVE_MSG_NOSIGNAL__
		ret = send(s->sock, &buffer[sum], n, MSG_NOSIGNAL);
#else
		ret = send(s->sock, &buffer[sum], n, 0);
#endif
		sum+=ret;

		if (fprogress)
			*fprogress=100.0*((float)sum/(float)size);
	}

	if (fprogress)
		*fprogress=100.0;

	return kNPE_SUCCESS;
}

// ------------------------------------
static TNPError xReadOnce(SockPtr sock, char* buffer, int size, int* nRecved, int timeout)
{
	sockssl_t* s = (sockssl_t*)sock;
	int res; 

	*nRecved=0;

	if ((res= xWaitSock(s->sock, timeout, &s->fd))<0)
	{
		return kNPE_UNKNOWN;
	}
	if (res==0)
	{
		return kNPE_READ_TIMEOUT;
	}
	{
		int n = recv(s->sock, buffer, size, 0);
		if (n==0)
			return kNPE_PEER;
		if (n<0)
			return kNPE_READ;

		*nRecved=n;
	}

	return (kNPE_SUCCESS);
}

// -------------------------------
static TNPError xReadOnce4SSL(SockPtr sock, char* buffer, int size, int* nRecved, int timeout)
{
	sockssl_t* s = (sockssl_t*)sock;
	int n=0, res;
	*nRecved=0;

	if ((res=xWaitSock(s->sock, timeout, &s->fd))==0)
	{
		return kNPE_READ_TIMEOUT;
	}
	else if (res<0)
	{
		return kNPE_UNKNOWN;
	}

//	if (pending=SSL_pending(s->ssl))
	{
_xRETRY:
		n = SSL_read(s->ssl, buffer, size);

#if 1	// SSL_pendingを使う場合は以下は要らない？
		if (n==SSL_ERROR_WANT_READ ||
				n==SSL_ERROR_WANT_WRITE)
		{
			// sleep 入れるべき？
			xSSL_DEBUG("WANT_READ...... %d\n", n);
			goto _xRETRY;
		}
#endif

		if (n==0)
		{
			// EOF
			return kNPE_READ_TIMEOUT;
		}
		if (n<0)
		{
			// EOF
			return kNPE_READ;
		}
	}

	*nRecved=n;
	return kNPE_SUCCESS;
}

// ------------------------------------
static TNPError xRead(SockPtr sock, char* buffer, int size, int timeout, float* fprogress)
{
	sockssl_t* s = (sockssl_t*)sock;
	int res; 
	int sum = 0;

	if (fprogress)
		*fprogress=0;
	
	while(sum!=size)
	{
		int n, ret;
		if ((res= xWaitSock(s->sock, timeout, &s->fd))<0)
		{
			return kNPE_UNKNOWN;
		}
		if (res==0)
		{
			// timeout.
			return kNPE_READ_TIMEOUT;
		}

		n = (size-sum)>xNP_OPTIMIZE_PACKET_SIZE ?xNP_OPTIMIZE_PACKET_SIZE :(size-sum);
		ret = recv(s->sock, &buffer[sum], n, 0);
		sum+=ret;

		if (fprogress)
			*fprogress=100.0*((float)sum/(float)size);
	}

	if (fprogress)
		*fprogress=100.0;

	return kNPE_SUCCESS;
}

// ------------------------------------
static TNPError xRead4SSL(SockPtr sock, char* buffer, int size, int timeout, float* fprogress)
{
	sockssl_t* s = (sockssl_t*)sock;
	int sum = 0;
	int res;

	while(sum!=size)
	{
		int n;

		if ((res=xWaitSock(s->sock, timeout, &s->fd))==0)
		{
			return kNPE_READ_TIMEOUT;
		}
		else if (res<0)
		{
			return kNPE_PEER;
		}

		//if (pending=SSL_pending(s->ssl)) -> 意味ない
		{
_xRETRY:
			n = SSL_read(s->ssl, &buffer[sum], size-sum);

#if 0		// SSL_pendingを使う場合は以下は要らない？
			if (n==SSL_ERROR_WANT_READ ||
					n==SSL_ERROR_WANT_WRITE)
			{
				// sleep 入れるべき？
				goto _xRETRY;
			}
#endif
		}

		if (n==0)
		{
			// EOF
			return kNPE_READ;
		}
		if (n<0)
		{
			// EOF
			return kNPE_PEER;
		}

		sum+=n;
		
		if (fprogress)
			*fprogress=100.0*((float)sum/(float)size);
	}

	if (fprogress)
		*fprogress=100.0;

	return kNPE_SUCCESS;
}


ADK_BEGIN_NAMESPACE

#if 0
#ifdef __POSIX__
#include <errno.h>
#endif
#endif


// --------------------------------------------------------
address_t NPCmnGetHostAdr(const char* hostname)
{
	int cnt=0, r;
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	struct sockaddr_in *addr_in;
	address_t ret = NPCmnIp2Adr(hostname);

	if (ret)
		return ret;

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

	for (rp = result; rp != NULL; rp = rp->ai_next) 
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

	// ホストのIPが複数ある場合には、ランダムでどれかを選ぶ
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
	{
		//char buffer[64];
		//xNP_DEBUG("ip:%s\n", NPCmnAdr2Ip(ret, buffer));
	}
	return ret;
}

// --------------------------------------------------------
address_t NPCmnIp2Adr(const char* ip)
{
	int a[4];
	if (sscanf(ip, "%d.%d.%d.%d", &a[0], &a[1], &a[2], &a[3])!=4)
	{
		return 0;
	}

	return inet_addr(ip);
}

// --------------------------------------------------------
char* NPCmnAdr2Ip(address_t addr, char* ip)
{
	static char buffer[32];
	unsigned char* p = (unsigned char*)&addr;

	sprintf(ip=ip?ip:buffer, "%d.%d.%d.%d", (int)p[0], (int)p[1], (int)p[2], (int)p[3]);
	return ip;
  
}

// --------------------------------------------------------
HandleSocket NPCmnConnect(address_t addr, S32 port, Bool useSSL, S32 timeout/*secs*/, TNPError* err)
{
	return NPCmnConnectInBuffer(0, addr, port, useSSL, timeout/*secs*/, err);
}

// --------------------------------------------------------
int NPCmnGetHandleSize()
{
	return sizeof(sockssl_t);
}

// --------------------------------------------------------
HandleSocket NPCmnConnectInBuffer(void* buffer, address_t addr, S32 port, Bool useSSL, S32 timeout/*secs*/, TNPError* err)
{
	int fd=0;
	int sock = xConnect(addr, port, timeout, &fd);
	sockssl_t* s=(sockssl_t*)buffer;

	if (sock<0)
	{
		*err=kNPE_CONNECT;
		return 0;
	}

	if (sock==0)
	{
		*err=kNPE_CONNECT_TIMEOUT;
		return 0;
	}

	//xNP_DEBUG("NPCmnConnect(useSSL:%d)\n", useSSL);
	if (useSSL==0)
	{
		if (s==0)
		{
			s = (sockssl_t*)xNP_MALLOC(sizeof(*s));	
		}
		xNP_ASSERT(s);
		xNP_ZERO(s);
		s->sock=sock;
		s->pfnRead=xRead;
		s->pfnWrite=xWrite;
		s->pfnReadOnce=xReadOnce;
	}
	else
	{
		int nRand;
		int i=0;
		SSL_CTX *ctx;
		SSL	*sslx;
		const char *seed = "ZPJSHD2HSDopHSFewqAJKAJKytFrJKAFuKytJAFJKAS4DFJ";
		int seedLen = strlen(seed);
		char seedBuff[128];
		char temp[16];

		srand(time(0));
		nRand = rand();

		// まずはランダム公開キーを作成する
		strcpy(seedBuff, seed);
		sprintf(temp, "%08x", nRand);

		// 全部変えるのは効率が悪いから部分だけ返る。
		// これで十分
		for (i=0; i<8; i++)
		{
			if (temp[i]<'A' || temp[i]>'z')
			{
				temp[i]-='0';
				temp[i]+='A';
			}
		} // for

		// 乱数でシードをその都度生成する。
		// このシードの値に基づいてopensslの中でパケットをエンコードする。
		strcpy(seedBuff, seed);
		strcpy(&seedBuff[seedLen-8], temp);
    
		//xNP_DEBUG("NPCmnConnect(addr:%x), port:%d\n", addr, port);

		RAND_seed(seedBuff, strlen(seedBuff));
    
		// SSL ライブラリを初期化
		SSLeay_add_ssl_algorithms();
		// これによって、エラー時にエラーの文字列が得られる。
		// デバッグ時のみすべきかどうか。。。
		SSL_load_error_strings();
    
		// 未だにこれが必要かどうか分からない。
		// なくても良いような。。。
		//SSL_library_init();
    
		// SSLのプロトコルを設定する。
		// サーバ側がSSL2, SSL3、TLS1をサポートすれば大丈夫。
		ctx = SSL_CTX_new(SSLv23_client_method());
    
		if (ctx==0)
		{
			*err=KNPE_INVALID_SSL_VERSION;
			close(sock);
			xNP_DEBUG("NPCmnConnect() SSL_INVALID_VERSION\n");
			return 0;
		}
    
		// これはSSL通信時にエラーなどでnegotiationが再度必要になる場合
		// リトライするかどうかを設定させるか、そのままエラーを返して終わらすか。
		// 設定させる方が無難
		SSL_CTX_set_mode(ctx, SSL_MODE_AUTO_RETRY);
    
		// 上記の設定に基づいてsslの構造体を生成する。
		sslx = SSL_new(ctx);
    
		// openSSLに先ほどコネクトしたディスクリプターを設定
		SSL_set_fd(sslx, sock);
    
		// この中でnegotiationが始まり、SSLレベルのhandshakeをする。
		
		//xNP_DEBUG("NPCmnConnect() for ssl\n");
		{
			int ret = SSL_connect(sslx);
			xNP_DEBUG("NPCmnConnect() leave for ssl\n");
    
			// handshakeまで行ったがプロトコルに問題のある場合、
			if (ret<=0)
			{
				*err=kNPE_SSL;
				xPRINT_SSL_ERROR();
				close(sock);
				SSL_shutdown(sslx);
				SSL_free(sslx);
				SSL_CTX_free(ctx);
				return 0;
			}
		}
   
		if (s==0)
		{ 
			s = (sockssl_t*)xNP_MALLOC(sizeof(*s));
		}
		xNP_ASSERT(s);
		
		xNP_ZERO(s);
		s->sock=sock;
		s->ssl=sslx;
		s->ctx=ctx;

		s->pfnRead=xRead4SSL;
		s->pfnWrite=xWrite4SSL;
		s->pfnReadOnce=xReadOnce4SSL;
	}

	if (buffer!=0)
	{
		s->isUseBuffer=true;
	}


#ifdef _USE_KQUEUE_
		s->fd=fd;
#endif
	return s;
}

// -----------------------------------
TNPError NPCmnRead(HandleSocket hSock, char*buffer, int size, int timeout, float* progress)
{
	sockssl_t* s = (sockssl_t*)hSock;
	xNP_ASSERT(s);

	return s->pfnRead((SockPtr)hSock, buffer, size, timeout, progress);
}

// -----------------------------------
TNPError NPCmnReadOnce(HandleSocket hSock, char*buffer, S32 size, S32* nRecved, int timeout)
{	
	sockssl_t* s = (sockssl_t*)hSock;

	xNP_ASSERT(s);
	return s->pfnReadOnce((SockPtr)hSock, buffer, size, nRecved, timeout);
}

// -----------------------------------
TNPError NPCmnWrite(HandleSocket hSock, const char*buffer, S32 size, float* progress)
{
	sockssl_t* s = (sockssl_t*)hSock;
	xNP_ASSERT(s);
	return s->pfnWrite((SockPtr)hSock, buffer, size, progress);
}


// -----------------------------------
void NPCmnClose(HandleSocket hSock)
{
	sockssl_t* s=(sockssl_t*)hSock;
	if (hSock==0)
		return ;

	if (s->ssl)
	{
		SSL_shutdown(s->ssl);
		SSL_free(s->ssl);
		s->ssl=0;
	}
	if (s->ctx)
	{
		SSL_CTX_free(s->ctx);
		s->ctx=0;
	}

#ifdef _USE_KQUEUE_
	if (s->fd)
	{
		close(s->fd);
		s->fd=0;
	}
#endif

	if (s->sock)
	{
		close(s->sock);
	}


	if (s->isUseBuffer==false)
	{
		xNP_FREE(s);
	}
}

Bool NPCmnIsConnected(HandleSocket hSock)
{
	return hSock==0?0:xIsValidSock(((sockssl_t*)hSock)->sock);
	
#if 0
	int timeout=0;
	sockssl_t* s = (sockssl_t* )hSock;
	int res = xWaitSock(hSock, timeout, &s->fd);

	if (res==0)
		return true;

	return false;
#endif
}

Bool NPCmnWait(HandleSocket hSock, S32 timeout)
{
	sockssl_t* s = (sockssl_t*)hSock;

	if (xWaitSock(s->sock, timeout, &s->fd)<=0)
		return false;
	return true;	
}

// -----------------------------------------------------------
// ここの関数はアルゴリズムの検証必要あり。
// 様々なパターンのurlに対応できているかどうか。
const char* NPCmnUrlParse(const char* url, Char* protocol, Char* hostname, S32* port)
{
	char* p = (char*)url;
	char* pUrlStart;
	int len=0;
    char* pPort;
	char* start_path_addr=0;

	// initialize.
	if (protocol)
		*protocol=0;
	if (hostname)
		*hostname=0;
	if (port)
		*port=0;
	
	p = strstr(url, "://");

	if (p)
	{
		register int len=p-url;
    
    if (protocol)
    {
      memcpy(protocol, url, len);//, "://");
      protocol[len]=0;
    }
	}

	p = p?(p+3):(char*)url;
	pUrlStart = p;

	while(*p!='/' && *p!=0) p++;

	if ((len=p-pUrlStart)==0)
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
    
  pPort = strstr(pUrlStart, ":");
    
  if (pPort==0)
  {
    if (hostname)
    {
			memcpy(hostname, pUrlStart, len);
			hostname[len]=0;
    }
    return start_path_addr;
  }
  else
  {
    if (hostname)
    {
      LIB_strcpyu(hostname, pUrlStart, ":");
    }
  }
    
  {
    char  szPort[16]={0, };
        
    len = pPort-pUrlStart;
    LIB_strcpyu(szPort, ++pPort, "/");
    
    if (szPort[0]==0)
    {
      strcpy(szPort, pPort);
    }
    
    if (port)
    {
      *port=atoi(szPort);
    }
    
   return start_path_addr;
  }
  return start_path_addr;
}

ADK_END_NAMESPACE
