#include "yisock.h"
#include "yiencodex.h"
#include "yistd.h"

#ifdef __POSIX__
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <fcntl.h>
#endif

#define SSL_DEFAULT_TIMEOUT 100
#define SSL_ERR_TIMEOUT 10

#ifdef WIN32

#	define __close__ closesocket

#	ifndef __shift_mod__
#		define __shift_mod__(a, n) ((~(0xFFFFFFFF<<n))&a)
#	endif
#else
#	define __close__ close
#endif

#define NOENCODING 1


// --------------------------------
static void* __alloc__(int size)
{
#ifdef WIN32
	return HeapAlloc(GetProcessHeap(), 1, size);
#else
	return malloc(size);
#endif
}

// --------------------------------
static void __free__(void* p)
{
#ifdef WIN32
	HeapFree(GetProcessHeap(), 1, p);
#else
	free(p);
#endif
}

#define check_sum(buffer, size) (unsigned short)yichecksum(buffer, size, 4)

#ifdef WIN32

// --------------------------------
int yiInitWinsock(int h, int l)
{
	WORD wVersion = MAKEWORD(h, l);
	WSADATA wsaData;
	int nErrorStatus;

	nErrorStatus = WSAStartup(wVersion, &wsaData);

	if (nErrorStatus!=0)
	{
		WSACleanup();
		return -1;
	}
	
	return wsaData.wVersion;
	/*if (HIBYTE(wsaData.wVersion)==h &&
		LOBYTE(wsaData.wVersion)==l)
	{
		return wsaData.wVersion;
	}*/
	
}

// --------------------------------
void yiCloseWinsock()
{
	WSACleanup();
}
#endif

// -------------------------------
int yiudp(address_t address, int port)
{
	int sock;
	struct sockaddr_in addr;

	if ((sock=socket(AF_INET, SOCK_DGRAM, 0))<0)	
	{
		return -1;
	}

	if (port==-1)
	{
		return sock;
	}

	if (port!=0)
	{
		int on=1;
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on));
	}
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr))<0)
	{
		return -1;
	}	

	return sock;
}

// -------------------------------
int yiudpV6(address6_t address, int port)
{
	int sock;
  	struct sockaddr_in6 addr;
	int len = sizeof(struct sockaddr_in6);

	if ((sock=socket(AF_INET6, SOCK_DGRAM, 0))<0)	
	{
		return -1;
	}

	if (port!=0)
	{
		int on=1;
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  		memset(&addr, 0, sizeof(addr));
		addr.sin6_family = AF_INET6;
		//addr.sin_addr.s_addr = IN6ADDR_ANY;
		addr.sin6_port = htons(port);

		if (bind(sock, (struct sockaddr*)&addr, len)<0)
		{
			YIERROR("yiudpV6() bind error!\n");
			return -1;
		}	
	}

	//yisockblk(True);
	return sock;
}

// -------------------------------
int yisenduV6(int sock, const void* buffer, int size, address6_t address, int port)
{
	struct sockaddr_in6 sin6;
	memset(&sin6, 0, sizeof(sin6));

	sin6.sin6_family=AF_INET6;
	sin6.sin6_port=htons(port);
	memcpy(&sin6.sin6_addr, address, sizeof(address6_t));

	printf("yisenduV6 size %d\n", size);
	return sendto(sock, buffer, size, 0, (struct sockaddr*)&sin6, sizeof(sin6));
}

// -------------------------------
int yirecvuV6(int sock, void* buffer, int size, address6_t address, int* port, int _timeout)
{
	struct sockaddr_in6 sin6;
	struct sockaddr_in addr;
	int addr_size=sizeof(sin6);
	int res;
	int reads;
	int *timeout=&_timeout;
	
	memset(&sin6, 0, sizeof(sin6));
	res = yiwaitsock(sock, timeout);
	
	if (res<=0)
	{
		YIERROR("yirecvuV6 timeout!\n");
		return -1;
	}

	reads = recvfrom(sock, buffer, size, 0, (struct sockaddr*)&sin6, &addr_size);

	if (reads==0 ||
		reads<0)
	{
		close(sock);
		return -2;
	}

	memcpy(address, &sin6.sin6_addr, sizeof(address));
	*port=sin6.sin6_port;

	{	
		unsigned int* xx=(unsigned int*)&sin6.sin6_addr;
		YIDEBUG("packet from %x:%x:%x:%x\n", xx[0], xx[1], xx[2], xx[3]);
	}

	return reads;
}

// -------------------------------
int yiconnectudp(int sock, address_t address, int port)
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = address;

	if (connect(sock, (struct sockaddr *)&addr, sizeof(addr))!=0)
	{
		return -1;
	}

	return sock;
}

#define xLOCALHOST (address_t)0x0100007F

// -------------------------------
int yiconnect(address_t address, int port, int nRetry)
{
	int sock;
	struct sockaddr_in addr;

	if ((sock=  socket(AF_INET, SOCK_STREAM, 0))<0)	
	{
		return -1;
	}

	yisockblk(sock, 1);


	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = address==0?xLOCALHOST:address;

RETRY:
	if (connect(sock, (struct sockaddr *)&addr, sizeof(addr))!=0)
	{
		if (nRetry==0)
		{
			__close__(sock);
			return -1;
		}
#ifdef WIN32
		Sleep(10);
#else
		usleep(10000);
#endif
		nRetry--;
		goto RETRY;
	}

	return sock;
}


// -------------------------------
int yilisten(address_t address, int port, int nBuffCnt)
{
	int sock;
	//int on=1;
	struct sockaddr_in my_addr;
	int nAddrSize;

	if (port==0)
	{
		sock = address;
		goto LISTEN_ONLY;
	}
	

	if ((sock =  socket(AF_INET, SOCK_STREAM, 0))<0)	
	{
		return -1;
	}

	yisockblk(sock, 1);


	//setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));	
	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = address ? address:htonl(INADDR_ANY);
	nAddrSize = sizeof(my_addr);

	if (bind(sock, (struct sockaddr*)&my_addr, nAddrSize)==-1)
	{
		__close__(sock);
		return -1;
	}

LISTEN_ONLY:
	if (listen(sock, nBuffCnt)==-1)
	{
		__close__(sock);
		return -1;
	}

	return sock;
}

// -------------------------------
int yiaccept(int sock, address_t* address, int* port, int* timeout)
{
	struct sockaddr_in addr_accept_in;
	struct sockaddr*p = (struct sockaddr*)&addr_accept_in;
	int ret;
	socklen_t addr_size=sizeof(addr_accept_in);
	memset(&addr_accept_in, 0, sizeof(addr_accept_in));

	if (timeout!=0)
	{
		if (yiwaitsock(sock, timeout)<1)
		{
			return -1;
		}
	}
	ret = accept(sock, p, &addr_size);

	if (ret==-1)
	{
		return -1;
	}

	if (address)
		*address = addr_accept_in.sin_addr.s_addr;

	if (port)
		*port = ntohs(addr_accept_in.sin_port);

	return ret;
}

// --------------------------------
address_t yihtol(const char* pszHostName, int index)
{
	int i=0;
	struct hostent* host;

	if (pszHostName[0]>='0' && pszHostName[0]<='9')
	{
		return yiitol(pszHostName);
	}
	
	host = gethostbyname(pszHostName);
	if (host)
	{
		return *((long*)host->h_addr_list[index]);
	}
	return 0;
}

// --------------------------------
address_t yiitol(const char* pszIpAddress)
{
	return inet_addr(pszIpAddress);
}

// -------------------------
int yigetport(int sock, address_t* address)
{
	struct sockaddr_in addr_accept_in;
	int n = sizeof(addr_accept_in);

	if (getsockname(sock, (struct sockaddr*)&addr_accept_in, &n)==-1)
	{
		return -1;
	}

	if (address)
	{
		*address = addr_accept_in.sin_addr.s_addr;
	}	

	return ntohs(addr_accept_in.sin_port);
}

// -------------------------
int yigetportV6(int sock, address6_t address)
{
	struct sockaddr_in6 addr_accept_in;
	int n = sizeof(addr_accept_in);

	if (getsockname(sock, (struct sockaddr*)&addr_accept_in, &n)==-1)
	{
		return -1;
	}
	
	if (address)
	{
		memcpy(address, &addr_accept_in.sin6_addr, sizeof(address));
	}

	return ntohs(addr_accept_in.sin6_port);
}

// -------------------------
void yiclose(int sock)
{
	__close__(sock);
}

// -------------------------
static int memcat(void* buffer, ...)
{
	void** pArgs = (void**)&buffer;
	int i=0;
	int size=0;
	char* p = (char*)buffer;

	pArgs=(void**)&pArgs[1];

	while(pArgs[i])
	{
		int n=(int)pArgs[i+1];
		memcpy(&p[size], pArgs[i++], n);
		size+=n;
		i++;
	}

	return size;
}

#ifdef WIN32
#include <process.h>
typedef void (*thread_calllback)(void*);
typedef unsigned int yithread_t;
#define __end_thread__ _endthread()
#else
#include <pthread.h>
typedef void* (*thread_calllback)(void*);
typedef unsigned int yithread_t;
#define __end_thread__ return 0
#endif

// -------------------------
static BOOL yithread_create(thread_calllback callback, void* p)
{
#ifdef WIN32
	int ret = (int)_beginthread(callback, 0, p);
	return ret!=-1 ? TRUE:FALSE;
#else
	pthread_t id;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (pthread_create(&id, &attr, callback, p)!=0)
	{
		return FALSE;
	}
	return TRUE;
#endif
}


// -------------------------
#ifdef WIN32
static void __yisend__(void* p)
#else
static void* __yisend__(void* p)
#endif
{
	int* sock = (int*)p;
	int size = sock[1];
	void* buffer = (void*)&sock[2];


	//send(*sock, (const char*)buffer, size, MSG_NOSIGNAL);
	yisend(*sock, (const char*)buffer, size, False);

	__free__(p);
	
	__end_thread__;
}

// -------------------------
int yisend(int sock, void* buffer, int n, int bUseThread)
{
	if (bUseThread==0)
	{
		//(buffer, n, "yisend dump");
		//YIDEBUG("yisend() size:%d\n", n);
		if (n<=4096)
		{
			int res = send(sock, buffer, n, MSG_NOSIGNAL);

			if (res!=n)
			{
				YIERROR("yisend()...... %d!=%d, errno:%d\n", n, res, errno);
			}

			return res;
		}
		else
		{
			int nRemain=n;
			char* pIndex = (char*)buffer;//[4096];
			int sum=0;
			while(nRemain)
			{
				int sent = send(sock, &pIndex[sum], nRemain<4096?nRemain:4096, MSG_NOSIGNAL);
				if (sent<=0)
				{
					YIDEBUG("yisend()...... res:%d\n", sent);
					return sent;
				}
				nRemain-=sent;
			}
			//YIDEBUG("yisend()...... res:%d\n", n);
			return n;
		}
	}

	{
		char* p = (char*)__alloc__(n+sizeof(int)+sizeof(int));
		memcpy(p, &sock, sizeof(int));
		memcpy(&p[sizeof(int)],&n, sizeof(int));
		memcpy(&p[sizeof(int)+sizeof(int)],buffer, n);
		//memcat(p, &sock, sizeof(int), &n, sizeof(int), buffer, n, 0);
		if (yithread_create(__yisend__, p)==FALSE)
		{
			return eSE_THREAD_CREATE;
		}


	}

	return n;
}


// -------------------------
int yisendu(int sock, void* buffer, int n, unsigned int address, int port)
{
	struct sockaddr_in addr_in;
	int nResentCnt=0;
	int res;

	if (address==0 &&
		port==0)
	{
		res = send(sock, buffer, n, 0);
		goto SENDED;
	}

	memset(&addr_in, 0, sizeof(addr_in));
	addr_in.sin_addr.s_addr=address;
	addr_in.sin_port = htons(port);
	addr_in.sin_family=AF_INET;
	res = sendto(sock, buffer, n, 0, (struct sockaddr*)&addr_in, sizeof(addr_in));

SENDED:
	if (res==-1)
	{
		yiTrace("sendu() error -1\n");
		return eSE_SEND_ERROR;
	}
	else if (res!=n)
	{
		yiTrace("sendu() error %d!=%d\n", n, res);
		return eSE_DIFFERENT_SIZE;
	}

	return res;

/*	if (bIsConfirm==TRUE)
	{
		int n;
		int res2;
		unsigned int timeout = UDP_WAIT_TIMEOUT;
RETRY:
		n = sizeof(addr_in);
		if ((res2=yiwaitsock(sock, &timeout))>0)
		{
			syipacketu packet;

			int nRecieved = recvfrom(sockConfirm, &packet, sizeof(packet), 0, (struct sockaddr*)&addr_in, &n);

			if (addr_in.sin_port!=htons(port) ||
				addr_in.sin_addr.s_addr!=address)
			{
				goto RETRY;
			}

			yidecodex(&packet, sizeof(packet), security);
			if ((packet.flag16 & ePF16_UDP_REPLY)!=0)
			{
				if (packet.seq==seq)
				{
					return res;
				}
			}

			// これは問題、このケースをありとすべきか？
			return eSE_NO_REPLY;
		}
		else if (res2==0) // timeout
		{
			if (nResentCnt==0)
			{
				return eSE_TIMEOUT; // time out
			}

			nResentCnt--;
			goto RESEND;
		}
		else
		{
			return eSE_SELECT_ERROR;
		}
	}*/
	
	return res;
}

// -------------------------
int yisendx(int sock, int id, int security1, int security2, void* data, int n, int bUseThread)
{
	char buffer[MAX_PACKET_SIZE];
	char* pBuffer = buffer;
	syipacket packet;
	int nPacketSize;
	int res;

	if (n+sizeof(packet) > MAX_PACKET_SIZE)
	{
		pBuffer = __alloc__(n+sizeof(packet));
	}

	memset(&packet, 0, sizeof(packet));
	packet.security = security1;
	packet.id = id;
	packet.encode_key = security2;
	packet.sizex = n;
	packet.flag16=ePF16_PACKET_NORMAL;
	packet.check_sum=0;
	
	if (data!=0)
	{
		packet.check_sum = (unsigned short)yichecksum(data, n, 4);
	}

	if (security1!=0)
	{
		yiencodex(&packet, sizeof(packet), security1);	
	}

	if (data==0)
	{
		if (yisend(sock, &packet, sizeof(packet), bUseThread)==sizeof(packet))
		{
			return 0;
		}
		return eSE_SEND_ERROR;
	}


	memcpy(pBuffer, &packet, sizeof(packet));
	memcpy(&pBuffer[sizeof(packet)], data, n);

	//nPacketSize = memcat(pBuffer, &packet, sizeof(packet), data, n, 0);

	if (security2!=0)
	{
		yiencodex(&pBuffer[sizeof(packet)], n, security2);
	}


	res = yisend(sock, pBuffer, n+sizeof(packet), bUseThread);

	if (n+sizeof(packet) > MAX_PACKET_SIZE)
	{
		__free__(pBuffer);
	} 

	return (res-sizeof(packet));
}

// -------------------------
int yisendxu(int sock, int id, int security1, int security2, void* data, int n, unsigned int address, int port, BOOL bConfirm, int seq)
{
	char buffer[MAX_PACKET_SIZE];
	syipacketu packet;
	int nPacketSize;
	int res;

	if (n+sizeof(packet) > MAX_UDP_PACKET_SIZE)
	{
		return -1;
	}

	memset(&packet, 0, sizeof(packet));

	packet.security = security1;
	packet.encode_key = security2;
	packet.seq=seq;
	packet.flag16=ePF16_PACKET_NORMAL;

	__UF32_SET_ID__(packet.flag32, id);
	__UF32_SET_SIZE__(packet.flag32, n);
	__UF32_SET_CHECKSUM__(packet.flag32, (unsigned short)yichecksum(data, n, 4));

	if (bConfirm!=FALSE)
	{
		packet.flag16|=ePF16_REQ_UDP_CONFIRM;
		packet.seq = seq;
	}

	if (security1!=0)
	{
		yiencodex(&packet, sizeof(packet), security1);	
	}

	memcpy(buffer, &packet, sizeof(packet));
	memcpy(&buffer[sizeof(packet)], data, n);
	//nPacketSize = memcat(pBuffer, &packet, sizeof(packet), data, n, 0);

	if (security2!=0)
	{
		yiencodex(&buffer[sizeof(packet)], n, security2);
	}


	return (yisendu(sock, buffer, sizeof(packet)+n, address, port)-sizeof(syipacketu));
}

// -------------------------
int yirecv(int sock, void* buffer, int n, int* timeout)
{
	int res = yiwaitsock(sock, timeout);

	if (res<0)
		return eSE_SELECT_ERROR;

	if (res==0)
	{
		return eSE_TIMEOUT;
	}

	res = recv(sock, buffer, n, 0);

	//printf("recv:%d\n", res);

	return res;
}

// -------------------------
int yirecvu(int sock, void* buffer, int n, int* timeout, address_t* address, int* port)
{
	struct sockaddr_in addr;
	int addr_size=sizeof(addr);
	int res;
	int reads;

	res = yiwaitsock(sock, timeout);

	if (res<0)
	{
		YIDEBUG("yirecvu eSE_SELECT_ERROR\n");
		return eSE_SELECT_ERROR;
	}

	if (res==0&&
		*timeout==0)
	{
		YIDEBUG("yirecvu eSE_SELECT_TIMEOUT, res:%d, *timeout:%d\n", res, *timeout);
		return eSE_TIMEOUT;
	}

	if (address==0 &&
			port==0)
	{
		reads = recv(sock, buffer, n, 0);
		goto RECIEVED;
	}

	memset(&addr, 0, sizeof(addr));
	reads = recvfrom(sock, buffer, n, 0, (struct sockaddr*)&addr, &addr_size);

	if (address!=0 && port!=0)
	{
		if (*address==0 &&
				*port==0)
		{
			*address = addr.sin_addr.s_addr;
			*port = ntohs(addr.sin_port);
			return reads;
		}

		if (addr.sin_addr.s_addr!=*address ||
			addr.sin_port!=htons(*port))
		{
			yiTrace("%s!=%s\n", yiltostr(addr.sin_addr.s_addr), yiltostr(*address));
			yiTrace("%d!=%d\n", addr.sin_port, *port);
			return eSE_IGNORED_PACKET;
		}
	}

RECIEVED:
	if (reads==0 ||
		reads==-1)
	{
		YIDEBUG("yirecvu reads==0 or reads==-1\n");
		return eSE_CLOSED;
	}

	if (reads!=n)
	{
		YIDEBUG("yirecvu %d!=%d\n", reads, n);
		return reads;
		//return eSE_DIFFERENT_SIZE; // closed.
	}

	return reads;
}

// -------------------------
int yirecvx(int sock, syipacket* packet, void* data, unsigned int security, int timeout)
{
	int res = yirecv(sock, packet, sizeof(*packet), (timeout==-1)?0:&timeout);

	if (res<0)
	{
		YIERROR("yirecvx() select error timeout=%d\n", timeout);
		return res; // close.?
	}
	else if (res==0)
	{
		return eSE_CLOSED;
	}

	if (res!=sizeof(*packet))
	{
		YIERROR("yirecvx(%d) invalid packet\n", res);
		return eSE_INVALID_PACKET;
	}

	if (security!=0)
	{
		if (yidecodex(packet, sizeof(syipacket), security)==0)
		{
			return eSE_INVALID_PACKET;
		}
	}
	//yiTraceDump(packet, sizeof(syipacket), "yirecvx dump");

	if (packet->security!=security)
	{
		YIERROR("yirecvx() invalid packet (security not same, (%x:%x))\n", packet->security, security);
		//return eSE_INVALID_PACKET; // invalid packet.
	}

	//if (data==0)
	//{
	//	return 0;
	//}

	YIDEBUG("packet id:%d, sizex:%d\n", packet->id, packet->sizex);

	if (data==0 ||
		packet->sizex==0)
	{
		return 0;
	}

	res = yirecv(sock, data, packet->sizex, (timeout==-1)?0:&timeout);

	if (res<1)
	{
		YIERROR("yirecvx() invalid packet (res = %d), timeout=%d, read size:%d\n", res, timeout, packet->sizex);
		return res;
	}
	if (res!=packet->sizex)
	{
		YIERROR("yirecvx() invalid packet (%d != %d)\n", packet->sizex, res);
		return eSE_INVALID_PACKET;
	}

	if (yidecodex(data, packet->sizex, packet->encode_key)==0)
	{
		return eSE_INVALID_PACKET;
	}

	if (packet->check_sum==(unsigned short)yichecksum(data, packet->sizex, 4))
	{
		return packet->sizex;
	}
	YIERROR("yirecvx() invalid check sum\n");

	return res;
}

// -------------------------
int yirecvxu(int sock, syipacketu* packet, void* data,  unsigned int security, int timeout, unsigned int* addr, int* port)
{
	struct sockaddr_in addr_in;
	int addr_size=sizeof(addr_in);
	char buffer[MAX_UDP_PACKET_SIZE];
	int res = yirecvu(sock, buffer, MAX_UDP_PACKET_SIZE/*sizeof(*packet)*/, (timeout==-1)?0:&timeout, addr, port);

	//yiTrace("yirecvu() size %d\n", res);
	if (res<1)
	{
		return res;
	}

	if (res < sizeof(syipacketu))
	{
		yiTrace("yirecvxu() invalid size\n");
		return eSE_INVALID_PACKET;
	}

	memcpy(packet, buffer, sizeof(*packet));
	yidecodex(packet, sizeof(syipacketu), security);

	if (packet->security != security)
	{
		yiTrace("yirecvxu() invalid security\n");
		return eSE_INVALID_PACKET;
	}

	if (packet->flag16 & ePF16_REQ_UDP_FIN)
	{
		yiTrace("yirecvxu() get FIN\n");
		return 0;
	}


	{
		unsigned short checksum = __UF32_CHECKSUM__(packet->flag32);
		int size = __UF32_SIZE__(packet->flag32);
		int id = __UF32_ID__(packet->flag32);

		if (size>MAX_UDP_PACKET_SIZE+sizeof(syipacketu))
		{
			yiTrace("yirecvxu() too large size %d\n", size);
			return eSE_INVALID_PACKET;
		}

		if (size==0)
		{
			return 0;
		}

		if (res!=size+sizeof(*packet))
		{
			yiTrace("yirecvxu() size %d!=%d\n", size+sizeof(*packet), res);
			return eSE_INVALID_PACKET;
		}

		/*memset(&addr_in, 0, sizeof(addr_in));	
		res = recvfrom(sock, data, size, 0, (struct sockaddr*)&addr_in, &addr_size);
		if (res!=size)//+sizeof(*packet)))
		{
			yiTrace("yirecvxu() size %d!=%d\n", size, res);
			return eSE_INVALID_PACKET;
		}
	
		res = yirecvu(sock, data, size, (timeout==-1)?0:&timeout, addr, port);


		/*{
			int start = res;
			int remain = (size+sizeof(*packet))-start;
			while(remain)
			{
				int nread = yirecvu(sock, &buffer[start], remain, 0, 0, 0);
				if (nread<0)
				{
					yiTrace("yirecvxu() size %d!=%d\n", size, res);
					return eSE_INVALID_PACKET;
				}
				remain-=nread;
			}
		}*/
		

		memcpy(data, &buffer[sizeof(*packet)], size);

		if (packet->encode_key!=0)
		{
			if (yidecodex(data, size, packet->encode_key)==0)
			{
				return eSE_INVALID_PACKET;
			}
		}

		if (checksum!=(unsigned short)yichecksum(data, size, 4))
		{
			yiTrace("yirecvxu() invalid checksum\n");
			return eSE_INVALID_PACKET;
		}
		
		return (res-sizeof(*packet));

	}	
	return 0;
}

// -------------------------
int yiwaitsock(int sock, int* timeout)
{
	if (timeout==0)
	{
		return 1;
	}
	else if (timeout==WAIT_INFINITE)
	{
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(sock, &fds);
		return select(sock+1, &fds, 0, 0, 0);
	}
	else
	{
		int res=0;
		fd_set fds;
		long sec = *timeout/1000;
		long usec = (*timeout%1000)*1000;
		struct timeval tv = {sec, usec};


		FD_ZERO(&fds);
		FD_SET(sock, &fds);

		res = select(sock+1, &fds, 0, 0, &tv);

		*timeout = tv.tv_sec*1000+tv.tv_usec/1000;

#ifndef __OSX__
		if (tv.tv_sec!=0 ||
			tv.tv_usec!=0)
		{
			return 1;
		}
#endif
		return res;
	}

	return -1;
}

// -------------------------
static BOOL IsInbuffer(int sock)
{
	fd_set fds;
	struct timeval tv = {0, 0};

	FD_ZERO(&fds);
	FD_SET(sock, &fds);
	if (select(sock+1, &fds, 0, 0, &tv)!=0)
	{
		return TRUE;
	}

	return FALSE;
}

// -------------------------
int yisockclear(int sock)
{
	int timeout=0;
	int n=0;
	char buffer[MAX_UDP_PACKET_SIZE];

	while(1)
	{
		if (IsInbuffer(sock)==FALSE)
		{
			return n;
		}
		n+=yirecvu(sock, buffer, MAX_UDP_PACKET_SIZE, 0, 0, 0);
	}

	return n;
}

#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
// -------------------------
unsigned int yigetlocaladdress(char* pszAddress, int n)
{
	char ac[80];
	struct hostent* he;
	int i=0;
	struct addrinfo hint,* _info;
	unsigned int address;

	if (gethostname(ac, sizeof(ac))==-1)
	{
		return 0;
	}


	memset(&hint, 0, sizeof(hint));
	hint.ai_socktype=SOCK_STREAM;
	hint.ai_family=AF_INET;

	if (getaddrinfo(ac, 0, &hint, &_info)!=0)
	{
		return 0;
	}

	address = ((struct sockaddr_in*)_info->ai_addr)->sin_addr.s_addr;

	if (pszAddress!=0)
	{
		if (address)
		{
			unsigned char* p=(unsigned char*)&address;
			sprintf(pszAddress, "%d.%d.%d.%d", (int)p[0]&0xFF, (int)p[1]&0xFF, (int)p[2]&0xFF, (int)p[3]&0xFF);
		}
	}

	freeaddrinfo(_info);
	return address;
}


// -------------------------
int yigetlocaladdressV6(address6_t addr, const char* pszServiceName)
{
	int err;
	int sock;
	//unsigned int* p=0;
	struct addrinfo hints={0, };
	struct addrinfo* res = NULL;
	struct addrinfo* ai;
	char szHostName[64];

	memset(addr, 0, sizeof(address6_t));	

	hints.ai_family 	= AF_INET6;
	//hints.ai_socktype 	= SOCK_DGRM;
	hints.ai_flags		= /*AI_CANONNAME|AI_ADDRCONFIG|*/AI_PASSIVE;

	//gethostname(szHostName, 64);
	//printf("hostname:%s\n", szHostName);
	err = getaddrinfo(0, pszServiceName, &hints, &res);
	if (err!=0)
	{
		YIERROR("yigetlocaladdressV6() getaddrinfo error!(%s)\n", gai_strerror(err));
		return 0;
	}
	YIDEBUG("getaddrinfo ... success.... %p, sizeof(addr:%d), &res->ai_addr:%p\n", res, sizeof(address6_t), &res->ai_addr);


	memcpy(addr, &res->ai_addr, sizeof(address6_t));
	freeaddrinfo(res);
	
	return 1;
}

// -------------------------
const char* yiltostr(unsigned int addr)
{
	static char str[16];
	unsigned char* p=(unsigned char*)&addr;
	sprintf(str, "%u.%u.%u.%u", 0xFF&p[0], 0xFF&p[1], 0xFF&p[2], 0xFF&p[3]);

	return str;
}

#include <sys/types.h>
#include <sys/syscall.h>

// -------------------------
int yisocklock(int sock)
{
	struct flock fl;

	fl.l_type=F_WRLCK;
	fl.l_whence=0;
	fl.l_start=0;
	fl.l_len=0;
	fl.l_pid=syscall(SYS_gettid);//getpid();

	return fcntl(sock, F_SETLKW, &fl);
}

// -------------------------
int yisockunlock(int sock)
{
	struct flock fl;

	fl.l_type=F_UNLCK;
	fl.l_whence=0;
	fl.l_start=0;
	fl.l_len=0;
	fl.l_pid=0;//gettid();

	return fcntl(sock, F_SETLKW, &fl);
}

// -------------------------
void yisockblk(int sock, int bIsBlock)
{
#ifdef WIN32
        unsigned long val=(bIsBlock==1)?0:1;
        ioctlsocket(sock, FIONBIO, &val);
#endif
}

// -------------------------
int yisend_ssl(sockssl_t sock, const void* p, int size)
{
	if (sock->ssl)
	{
		int sum=0;
		int remain=size;
		int n;
		char* pIndex=(char*)p;
		//YIDEBUG("yisend_ssl(size:%d)", size);
RETRY:
		n = SSL_write(sock->ssl, pIndex, remain);

		//YIDEBUG("yisend_ssl() result=%d", n);

		if (n==SSL_ERROR_WANT_WRITE)
		{
			YIDEBUG("yisend_ssl() SSL_ERROR_WANT_WRITE", n);
			goto RETRY;
		}

		sum+=n;

		if (sum!=size)
		{
			remain-=n;
			pIndex+=n;
			//YIDEBUG("yisend_ssl() sum:%d != size", sum, size);
			goto RETRY;
		}

		return n;
	}

	return yisend(sock->sock, p, size, False);
}

// -------------------------
#define SSL_ANY_SIZE 256
#define MAX_RETRY_CONT 100
// -------------------------
int yirecv_ssl(sockssl_t sock, const void* p, int size, int nTimeOut)
{
	int nRetryCount=MAX_RETRY_CONT;
	if (sock->ssl)
	{
		char* pIndex=(char*)p;
		int remain=0;
		int sum=0;
		int _timeout=nTimeOut;

		//YIDEBUG("yirecv_ssl(size:%d, timeout:%d)", size, nTimeOut);

RETRY:
		//nTimeOut=_timeout;

		if (yiwaitsock(sock->sock, &nTimeOut)==True)
		{
			int n=0;
			int pending=0;
READ:			
			//yisockblk(sock->sock, False);
			n = SSL_read(sock->ssl, pIndex, size);
			//yisockblk(sock->sock, True);
			//YIDEBUG("yirecv_ssl() recved:%d", n);

			if (n==SSL_ERROR_WANT_READ)
			{
				if (nRetryCount--)
				{
#ifdef _WIN32
					Sleep(SSL_ERR_TIMEOUT);
#else
					usleep(SSL_ERR_TIMEOUT*1000);
#endif
					nTimeOut=SSL_DEFAULT_TIMEOUT;
					goto RETRY;
				}
				
				//YIDEBUG("yirecv_ssl() wait timeout(SSL_ERROR_WANT_READ)");
				return sum;
			}
			else if (n<0)
			{
				if (nRetryCount--)
				{
					//YIDEBUG("yirecv_ssl() wait recieved (%d) ret:%d", n, sum);
#ifdef _WIN32
					Sleep(SSL_ERR_TIMEOUT);
#else
					usleep(SSL_ERR_TIMEOUT*1000);
#endif
					nTimeOut=SSL_DEFAULT_TIMEOUT;
					goto RETRY;
				}
				//yisockblk(sock->sock, True);
				//return sum;//return 0;
				YIDEBUG("yirecv_ssl() wait timeout(SSL_ERROR_WANT_READ)");
				return sum;
			}

			sum+=n;
			pIndex+=n;

			if (size<SSL_ANY_SIZE)
			{
				if (sum!=0)
				{
					return sum;
				}
			}

			if ((pending=SSL_pending(sock->ssl))>0)
			{
				//YIDEBUG("yirecv_ssl() wait recieved (%d) pending: %d", sum, pending);
				//Sleep(SSL_DEFAULT_TIMEOUT);
				goto READ;
				//nTimeOut=100;
				//goto RETRY;
			}
			nTimeOut=SSL_DEFAULT_TIMEOUT;
			//Sleep(SSL_DEFAULT_TIMEOUT);
			goto RETRY;
		}
		else
		{
			int pending=0;
			//YIDEBUG("yirecv_ssl() wait timeout(%d)", nTimeOut);


			if ((pending=SSL_pending(sock->ssl))>0)
			{
				//YIDEBUG("yirecv_ssl() wait recieved (%d) pending: %d", sum, pending);
				goto READ;
			}

			if (sum==0)
			{
				if (nRetryCount--)
				{
					//YIDEBUG("yirecv_ssl() not pending and sum si zero");
#ifdef _WIN32
					Sleep(SSL_ERR_TIMEOUT);
#else
					usleep(SSL_ERR_TIMEOUT*1000);
#endif
					nTimeOut=SSL_DEFAULT_TIMEOUT;
					goto RETRY;
				}
				return sum;
			}

			// 内部で書き込み中であれば？
			if (yisockIsWritable(sock->sock)==False)
			{
				nTimeOut=SSL_DEFAULT_TIMEOUT;
				if (nRetryCount--)
				{
					//YIDEBUG("yirecv_ssl() yisockIsWritable()==False");
#ifdef _WIN32
					Sleep(SSL_ERR_TIMEOUT);
#else
					usleep(SSL_ERR_TIMEOUT*1000);
#endif
	
					nTimeOut=SSL_DEFAULT_TIMEOUT;
					goto RETRY;
				}
			}
		}
		
		//yisockblk(sock->sock, True);
		YIDEBUG("yirecv_ssl() wait recieved ret: %d", sum);
		return sum;
	}
	//snTimeOut*=10;
	return yirecv(sock->sock, p, size, &nTimeOut);
}

// -------------------------
void yiclose_ssl(sockssl_t sock)
{
    yiclose(sock->sock);//(sock);

	if (sock->ssl)
	{
		SSL_shutdown(sock->ssl);
		SSL_free(sock->ssl);
		SSL_CTX_free(sock->ctx);
		sock->ssl=0;
		sock->ctx=0;
	}
	yifree(sock);
}

// -------------------------
Bool yisockIsWritable(int sock)
{
	fd_set fds;
	struct timeval tv={0, 0};

	FD_ZERO(&fds);
	FD_SET(sock, &fds);

	if (select(0, 0, &fds, 0, 0)!=1)
	{
		return False;
	}
	return True;
}
