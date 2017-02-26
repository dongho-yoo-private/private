/** This header must be inclued by npCmn.c or npCmn.cpp
*
*/

#ifdef __IAM_NPCMN__

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>



// -----------------------------------------------------------
static int xIsValidSock(int sock)
{
	int error=0;
	unsigned int len;

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
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
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

#ifdef _USE_SELECT
// --------------------------------- static fucntionz
int xWaitSockForSelect(int sock, int timeout)
{
	return 0;
}
#endif

// --------------------------------- static fucntionz
static int xWaitSock(int sock, int timeout/*second*/, int* descriptor)
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
	int val, i;
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

			xNP_DEBUG("connect success...\n");
			xSockNonBlock(sock, 0);
			return sock;
		} /* if EINPROGRESS */
		return 0;
	} // connect retry or failure.

	// connect success.
	xNP_DEBUG("connect success...\n");
	return sock;
}

// ------------------------------------
static TNPError xWrite4SSL(SockPtr sock, const char* buffer, int size, float* fprogress)
{
	sockssl_t* s = (sockssl_t*)sock;
	int res; 
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
	int res; 
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
	int sum = 0;

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
		int pending, n;

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

#endif
