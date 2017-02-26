#include "yisock.h"
#include "yiencodex.h"
#include "yistd.h"

#define SSL_DEFAULT_TIMEOUT 100
#define SSL_ERR_TIMEOUT 10

#ifdef WIN32

#define __close__ closesocket

#ifndef __shift_mod__
#define __shift_mod__(a, n) ((~(0xFFFFFFFF<<n))&a)
#endif


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
#else
#define __close__ close
#endif

//// -------------------------------
//unsigned short check_sum(void* data, int n)
//{
//	unsigned int sum=0;
//	unsigned int* p=(unsigned int*)data;
//	register int i=0;
//	int r=n;
//	int cnt=0;
//
//RET:
//	if (r==0)
//		return (unsigned short)sum;
//
//	if (r==1)
//	{
//		unsigned char* p = (unsigned char*)data;
//		return (unsigned short)*p;
//	}
//	else if (r==2)
//	{
//		return *((unsigned short*)data);
//	}
//	else if (r==3)
//	{
//		return (*((unsigned short*)data)+(unsigned short)((unsigned char*)data)[3]);
//	}
//
//	cnt=n>>2;
//
//	for (i=0; i<cnt; i++)
//	{
//		sum+=p[i];
//	}
//
//	r = __shift_mod__(n, 2);
//
//	goto RET;
//
//	return 0;
//
//}

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
	//}	
	return sock;
}

//// -------------------------------
//// udp.
//int yiudp_unix(const char* port, int port)
//{
//	int sock;
//	struct sockaddr_un addr;
//
//	if ((sock=socket(AF_UNIX, SOCK_DGRAM, 0))<0)	
//	{
//		return -1;
//	}
//
//	//if (port!=0)
//	//{
//		//int on=1;
//	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));	
//	if (port==0)
//	{
//		int size;
//		addr.sun_family = AF_UNIX;
//		strncpy(addr.sun_path, port, sizeof(addr.sun_path));
//		size = sizeof(addr.sun_family)+strlen(addr.sun_path);
//	
//		if (bind(sock, (struct sockaddr*)&addr, size)<0)
//		{
//			return -1;
//		}
//	}
//
//	//}	
//	return sock;
//}
//
//// -------------------------------
//HandleUdpUnix yiupdUnixHandle(const char* to)
//{
//	int* p;
//	struct sockaddr_un addr_un;
//    memset(&addr_un, 0, sizeof(addr_un));
//	addr.sun_family = AF_UNIX;
//	strncpy(addr_un.sun_path, to, sizeof(addr.sun_path));
//	size = sizeof(addr_un.sun_family)+strlen(addr_un.sun_path);
//
//	p = (int*)yialloc(size+sizeof(int));
//	*p=size;
//	memcpy(&p[1], addr_un, size);
//
//	return (HandleUdpUnix)p;
//}
//
//// -------------------------------
//HandleUdpUnix yiupdUnixHandleClose(HandleUdpUnix h)
//{
//	yifree(h);
//}
//
//// -------------------------------
//int yiudp_send(int sock, HandleUdpUnix h, const void* buffer, int n)
//{
//	int* p = (int*)h;
//    res = sendto(sock, buffer, n, 0, (struct sockaddr*)&p[1], *p);
//	return res;
//}
//
//// -------------------------------
//int yiudp_recv(int sock, const void* buffer, int n)
//{
//	struct sockaddr_un addr;
//	int addr_size=sizeof(addr);
//	int res;
//	memset(&addr, 0, sizeof(addr));
//	res = recvfrom(sock, (char*)buffer, n, 0, (struct sockaddr*)&addr, &addr_size);
//
//	return res;
//}

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
		usleep(100000);
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
int yigetport(int sock, unsigned int* address)
{
	struct sockaddr_in addr_accept_in;
	int n = sizeof(addr_accept_in);

	if (getsockname(sock, (struct sockaddr*)&addr_accept_in, &n)==-1)
	{
		return -1;
	}

	if (address)
		*address=addr_accept_in.sin_addr.s_addr;

	return ntohs(addr_accept_in.sin_port);
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


	send(*sock, (const char*)buffer, size, 0);

	__free__(buffer);
	
	__end_thread__;
}

// -------------------------
int yisend(int sock, void* buffer, int n, int bUseThread)
{
	if (bUseThread==0)
	{
		//fd_set fds;
		//FD_ZERO(&fds);
		//FD_SET(sock, &fds);

		//if (select(0, 0, &fds, 0, 0)<0)
		//{
		//	return -1;
		//}

		return send(sock, (const char*)buffer, n, 0);
	}
	{
		void* p = __alloc__(n+sizeof(int)+sizeof(int));
		memcat(p, &sock, &n, sizeof(int), sizeof(int), buffer, n, 0);
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


	memset(&addr_in, 0, sizeof(addr_in));
	addr_in.sin_addr.s_addr=address==0?xLOCALHOST:address;
	addr_in.sin_port = htons(port);
	addr_in.sin_family=AF_INET;

	xTRACE("%x> yisendu() %d bytes", GetProcessId(GetCurrentProcess()), n);
	res = sendto(sock, (const char*)buffer, n, 0, (struct sockaddr*)&addr_in, sizeof(addr_in));

	if (res==-1)
	{
		return eSE_SEND_ERROR;
	}
	else if (res!=n)
	{
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
int yisendx(int sock, int id, int security1, int security2, const void* data, int n, int bUseThread)
{
	char buffer[MAX_PACKET_SIZE];
	char* pBuffer = buffer;
	syipacket packet;

	xTRACE("yisendx packet size:%d (%x, %x, %x, %x, %x, %x)", sizeof(packet), &packet.encode_key, &packet.security, &packet.sizex, &packet.check_sum, &packet.id, &packet.flag16);

	if (n+sizeof(packet) > MAX_PACKET_SIZE)
	{
		pBuffer = (char*)__alloc__(n+sizeof(packet));
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
		packet.check_sum = check_sum((void*)data, n);
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
		xTRACE("yisendx() send error...!!");
		return eSE_SEND_ERROR;
	}

	{
		//unsigned int flag = security1==0?0:0xFFFFFFFF;

		//memcpy(pBuffer, &flag, sizeof(unsigned int));

		//memcpy(&pBuffer[sizeof(flag)], &packet, sizeof(packet));
		//memcpy(&pBuffer[sizeof(flag)+sizeof(packet)], data, n);
		memcpy(pBuffer, &packet, sizeof(packet));
		memcpy(&pBuffer[sizeof(packet)], data, n);
	}

	if (security2!=0)
	{
		yiencodex(&pBuffer[sizeof(packet)], n, security2);
	}

	return (yisend(sock, pBuffer, n+sizeof(packet), bUseThread)-sizeof(packet));
}

// -------------------------
int yisendxu(int sock, int id, int security1, int security2, const void* data, int n, unsigned int address, int port, BOOL bConfirm, int seq)
{
	char buffer[MAX_PACKET_SIZE];
	char* pBuffer = buffer;
	syipacketu packet;
	char* px=0;

	if (n+sizeof(packet) > MAX_UDP_PACKET_SIZE)
	{
		return yisendxul(sock, id, security1, security2, data, n, address, port, seq);
	}
	xTRACE("%x> yisendxu() id:%d, size:%d bytes", GetProcessId(GetCurrentProcess()), id, n);
	memset(&packet, 0, sizeof(packet));

	packet.security = security1;
	packet.encode_key = security2;
	packet.seq=seq;
	packet.flag16=ePF16_PACKET_NORMAL;

	__UF32_SET_ID__(packet.flag32, id);
	__UF32_SET_SIZE__(packet.flag32, n);
	__UF32_SET_CHECKSUM__(packet.flag32, check_sum((void*)data, n));

	if (bConfirm!=FALSE)
	{
		packet.flag16|=ePF16_REQ_UDP_CONFIRM;
		packet.seq = seq;
	}


	if (security1!=0)
	{
		yiencodex(&packet, sizeof(packet), security1);	
	}

	//px = (char*)yialloc(sizeof(packet)+n);
	//memcpy(px, &packet, sizeof(packet));

	//if (data!=0 &&
	//		n!=0)
	//{
	//	memcpy(&px[sizeof(packet)], data, n);

	//	if (security2!=0)
	//	{
	//		yiencodex(&px[sizeof(packet)], n, security2);
	//	}
	//}

	if (yisendu(sock, &packet, sizeof(packet), address, port)!=(sizeof(packet)))
	{
		return -1;
	}

	if (n==0)
	{
		return 0;
	}


	//{
	//	unsigned int flag = security1==0?0:0xFFFFFFFF;

	//	memcpy(pBuffer, &flag, sizeof(unsigned int));
	//	memcpy(&pBuffer[sizeof(int)], &packet, sizeof(packet));

	//	if (n!=0)
	//	{
	//		memcpy(&pBuffer[sizeof(packet)], data, n);
	//	}
	//}


	if (security2!=0)
	{
		memcpy(buffer, data, n);
		yiencodex(buffer, n, security2);
	}

	return yisendu(sock, security2!=0?buffer:data, n, address, port);
}

// -------------------------
int yisendxul(int sock, int id, int security1, int security2, const void* data, int n, address_t address, int port, int seq)
{
	char buffer[MAX_PACKET_SIZE];
	char* pBuffer = buffer;
	syipacketu packet;
	int nRemainSize = n;
	int nSendSize=0;

	memset(&packet, 0, sizeof(packet));

	packet.security = security1;
	packet.encode_key = security2;
	packet.seq=seq;
	packet.flag16=(ePF16_PACKET_NORMAL|ePF16_UDP_LARGE);

	__UF32_SET_ID__(packet.flag32, id);
	__UF32_SET_SIZE__(packet.flag32, 4); // full size.
	__UF32_SET_CHECKSUM__(packet.flag32, check_sum((void*)data, n));

	if (security1!=0)
	{
		yiencodex(&packet, sizeof(packet), security1);	
	}

	// 1. まず、パケットのみ転送する。
	if (yisendu(sock, pBuffer, sizeof(packet), address, port)!=sizeof(packet))
	{
		return -1;
	}

	// 2. 次にサイズを転送する。
	if (yisendu(sock, &n, sizeof(int), address, port)!=sizeof(int))
	{
		return -1;
	}


	//if (yisendu(sock, pBuffer, nSendSize+sizeof(packet)+sizeof(int), address, port)!=nSendSize+sizeof(packet)+sizeof(int))
	//{
	//	return -1;
	//}

	nRemainSize=n;
	pBuffer = (char*)data;

	// -------------------------
	// 送る側は責任を持たない。
	while(nRemainSize)
	{
		int dummy;
		int nTimeOut=500;
		nSendSize = nRemainSize>(MAX_UDP_PACKET_SIZE)?(MAX_UDP_PACKET_SIZE):nRemainSize;
		
		if (security2!=0)
		{
			memcpy(buffer, pBuffer, nSendSize);
			yiencodex(buffer, nSendSize, security2);
			
			if (yisendu(sock, buffer, nSendSize, address, port)!=nSendSize)
			{
				return -1;
			}
		}
		else
		{
			if (yisendu(sock, pBuffer, nSendSize, address, port)!=nSendSize)
			{
				return -1;
			}
		}

		if (yirecvu(sock, &dummy, sizeof(int), &nTimeOut, 0, 0)!=sizeof(int))
		{
			// リトライ？
			return -2;
		}

		pBuffer+=nSendSize;
		nRemainSize-=nSendSize;
	}

	return n;
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

	if (n<4096)
	{
		int reced = recv(sock, (char*)buffer, n, 0);

		return reced;
		//if (reced<0)
		//	return reced;

		//if (reced<n)
		//{
		//	int size=n-reced;
		//	int top=reced;
		//	char* pIndex=(char*)buffer;
		//	int nRetry=0;
		//	

		//	while(size)
		//	{
		//		int timeout=1;
		//		int res=0;
		//		if ((res=yiwaitsock(sock, &timeout))<0)
		//		{
		//			return n-size;
		//		}
		//		if (res==0)
		//		{
		//			nRetry++;

		//			if (nRetry==100)
		//			{
		//				return n-size;
		//			}
		//			continue;
		//		}

		//		reced=recv(sock, (char*)&pIndex[top], size, 0);

		//		if (reced==0)
		//		{
		//			return n-size;
		//		}

		//		if (reced==size)
		//		{
		//			return n;
		//		}
		//		top+=reced;
		//		size-=reced;
		//	}
		//}

		return n;
	}
	else
	{
		int remain=n;
		int sum=0;
		char* p = (char*)buffer;
		while(remain)
		{
			int n = remain<4096?remain:4096;
			int reads = recv(sock, &p[sum], n, 0);

			if (reads==0 ||
					reads==-1)
			{
				return sum;
			}

			if (reads<n)
			{
				int timeout=50;
				if (yiwaitsock(sock, &timeout)==False)
				{
					sum+=reads;
					return sum;
				}
			}

			sum+=reads;
			remain-=reads;
		}
	}

	return n;
}

// -------------------------
int yirecvu(int sock, void* buffer, int n, int* timeout, address_t* address, int* port)
{
	struct sockaddr_in addr;
	int addr_size = sizeof(addr);
	int res;
	int reads;
	res = yiwaitsock(sock, timeout);

	xTRACE("%x> yirecvu() %d bytes", GetProcessId(GetCurrentProcess()), n);

	if (res==0)
	{
		xTRACE("yirecvu.... timeout");
		return eSE_TIMEOUT;
	}

	if (res<0)
	{
		xTRACE("yirecvu.... select error");
		return eSE_SELECT_ERROR;
	}

	
	memset(&addr, 0, sizeof(addr));


	reads = recvfrom(sock, (char*)buffer, n, 0, (struct sockaddr*)&addr, &addr_size);

	if (reads<1)
	{
		//int dwError = WSAGetLastError();
		//int dwError2 = GetLastError();

		xTRACE("yirecvu.... read(%d) error(%d, %d)", reads, WSAGetLastError(), GetLastError());

		return eSE_DIFFERENT_SIZE; // closed.
	}

	if (address!=0 && port!=0)
	{
		if (*address==0 &&
				*port==0)
		{
			*address = addr.sin_addr.s_addr;
			*port = ntohs(addr.sin_port);
			xTRACE("%x> yirecvu() %d bytes success", GetProcessId(GetCurrentProcess()), n);
			return reads;
		}

		if (addr.sin_addr.s_addr!=*address ||
			addr.sin_port!=htons(*port))
		{
			xTRACE("yirecvu.... IGNORE PACKET(%u.%u.%u.%u:%d!=%u.%u.%u.%u:%d)",
									addr.sin_addr.s_addr&0xFF, (addr.sin_addr.s_addr>>8)&0xFF, (addr.sin_addr.s_addr>>16)&0xFF, (addr.sin_addr.s_addr>>24)&0xFF, addr.sin_port&0xFFFF,
										*address&0xFF, (*address>>8)&0xFF, (*address>>16)&0xFF, (*address>>24)&0xFF, *port&0xFFFF);
			return eSE_IGNORED_PACKET;
		}
	}

	//if (reads!=n)
	//{
	//	return eSE_DIFFERENT_SIZE; // closed.
	//}
	xTRACE("%x> yirecvu() %d bytes success", GetProcessId(GetCurrentProcess()), n);

	return reads;
}

// -------------------------
int yirecvx(int sock, syipacket* packet, void* data, unsigned int security, int timeout)
{
	char buffer[sizeof(syipacket)];
	Bool bIsEncoded=True;

	//int res = yirecv(sock, packet, sizeof(syipacket), (timeout==-1)?0:&timeout);
	int res = yirecv(sock, buffer, sizeof(buffer), (timeout==-1)?0:&timeout);

	if (res<1)
	{
		return res; // close.?
	}

	if (res!=sizeof(buffer))
	{
		return eSE_INVALID_PACKET;
	}


	//if (*((unsigned int*)buffer)==0)
	//{
	//	bIsEncoded=False;
	//}

	memcpy(packet, buffer, sizeof(syipacket));

	if (yidecodex(packet, sizeof(syipacket), security)==0)
	{
		return eSE_INVALID_PACKET;
	}

	//if (packet->security!=security)
	//{
	//	return eSE_INVALID_PACKET; // invalid packet.
	//}

	if (data==0)
	{
		return 0;
	}

	if (packet->sizex==0)
		return 0;

	res = yirecv(sock, data, packet->sizex, (timeout==-1)?0:&timeout);

	if (res<1)
	{
		return res;
	}

	if (res!=packet->sizex)
	{
		return eSE_INVALID_PACKET;
	}

	if (packet->encode_key)
	{
		if (yidecodex(data, packet->sizex, packet->encode_key)==0)
		{
			return eSE_INVALID_PACKET;
		}
	}

	if (packet->check_sum!=0 &&
			packet->check_sum==check_sum(data, packet->sizex))
	{
		return packet->sizex;
	}

	return packet->sizex;
}

// -------------------------
xIntPtr yirecvxu(int sock, syipacketu* packet, void* data,  unsigned int security, int timeout, unsigned int* addr, int* port)
{
	char buffer[sizeof(syipacketu)];
	int res = 0;
	Bool bIsEncoded=True;
	int size=0;
	int dataSize=0;


	res = yirecvu(sock, buffer, sizeof(buffer)/*MAX_UDP_PACKET_SIZE*//*sizeof(packet)*/, (timeout==-1)?0:&timeout, addr, port);

	if (res<(sizeof(syipacketu)))
	{
		xTRACE("yirecvxu() eSE_INVALID_PACKET (%d<%d)", res, sizeof(syipacketu));
		return eSE_INVALID_PACKET;
	}
	xTRACE("%x> yirecvxu() id:%d, size:%d bytes", GetProcessId(GetCurrentProcess()), __UF32_ID__(packet->flag32), __UF32_SIZE__(packet->flag32));

	//if (res != sizeof(syipacketu))
	//{
	//	return eSE_INVALID_PACKET;
	//}

	memcpy(packet, buffer, sizeof(syipacketu));

	yidecodex(packet, sizeof(syipacketu), security);

	if (packet->security != security)
	{
		return eSE_INVALID_PACKET;
	}

	if (packet->flag16 & ePF16_REQ_UDP_FIN)
	{
		return 0;
	}

	if (packet->flag16&ePF16_UDP_LARGE)
	{
		char* recvBuffer;// = (char*)__alloc__(dataSize);
		int remain=dataSize;
		int sum=0;
		int cnt=0;

		if (yirecvu(sock, &dataSize, sizeof(unsigned int), 0, addr, port)!=sizeof(unsigned int))
		{
			return 0;
		}

		recvBuffer = (char*)__alloc__(dataSize);

		while(remain)
		{
			int nTimeOut=timeout;
			int nRecved=yirecvu(sock, &recvBuffer[sum], remain<512?remain:512, &nTimeOut, addr, port);

			if (nRecved==0 ||
					nRecved==-1)
			{
				free(recvBuffer);
				return nRecved;
			}

			if (yisendu(sock, &cnt, sizeof(int), *addr, *port)!=sizeof(int))
			{
				return 0;
			}
			cnt++;
			sum+=nRecved;
			remain-=nRecved;
		}
		
		memcpy(data, &dataSize, sizeof(unsigned int));
		return (int)recvBuffer;
	}

	size = __UF32_SIZE__(packet->flag32);

	if (size==0)
	{
		return 0;
	}
	

	res = yirecvu(sock, data, size/*MAX_UDP_PACKET_SIZE*//*sizeof(packet)*/, (timeout==-1)?0:&timeout, addr, port);

	{
		unsigned short checksum = __UF32_CHECKSUM__(packet->flag32);
		int id = __UF32_ID__(packet->flag32);

		if (size!=res)
		{
			xTRACE("yirecvxu() eSE_INVALID_PACKET (%d!=%d)\n", size, res);
			return eSE_INVALID_PACKET;
		}

		//memcpy(data, &buffer[sizeof(*packet)+sizeof(int)], size);

		if (bIsEncoded==True)
		{
			if (packet->encode_key!=0)
			{
				if (yidecodex(data, size, packet->encode_key)==0)
				{
					xTRACE("yirecvxu() encode error!\n", size, res);
					return eSE_INVALID_PACKET;
				}
			}
		}

		if (checksum!=0 &&
			checksum!=yichecksum(data, size, 4))
		{
			xTRACE("yirecvxu() Invalid checksum");
			return eSE_INVALID_PACKET;
		}
		
		//return res;

	}	
	return res;
}


// -------------------------
int yirecvxul(int sock, syipacketu* packet, void* data,  unsigned int security, int timeout, unsigned int* addr, int* port, void** extened)
{
	char buffer[sizeof(syipacketu)+sizeof(int)];
	int res = yirecvu(sock, buffer, sizeof(buffer)/*MAX_UDP_PACKET_SIZE*//*sizeof(packet)*/, (timeout==-1)?0:&timeout, addr, port);
	Bool bIsEncoded=True;
	int size = 0;

	if (extened!=0)
	{
		*extened=0;
	}

	if (res<(int)((sizeof(syipacketu)+sizeof(int))))
	{
		return eSE_INVALID_PACKET;
	}

	memcpy(packet, &buffer[sizeof(int)], sizeof(syipacketu));

	if (*((unsigned int*)buffer)==0)
	{
		bIsEncoded=False;
	}

	if (bIsEncoded==True)
	{
		yidecodex(packet, sizeof(syipacketu), security);

		if (packet->security != security)
		{
			return eSE_INVALID_PACKET;
		}
	}

	if (packet->flag16 & ePF16_REQ_UDP_FIN)
	{
		return 0;
	}

	if (packet->flag16 & ePF16_UDP_HAS_NEXT)
	{
		int res = yirecvu(sock, &size, sizeof(int)/*MAX_UDP_PACKET_SIZE*//*sizeof(packet)*/, (timeout==-1)?0:&timeout, addr, port);
		if (res<0)
		{
			return eSE_INVALID_PACKET;
		}
	}
	else
	{
		 size = __UF32_SIZE__(packet->flag32);
	}

	if (size==0)
	{
		return 0;
	}


	if (size>(MAX_UDP_PACKET_SIZE-sizeof(int)-sizeof(syipacketu)))
	{
		int nRemainSize = size;
		char* pIndex;
		int dummy=0;

		*extened = __alloc__(size);
		pIndex = (char*)*extened;
		
		while(nRemainSize)
		{
			int nReceiveSize = nRemainSize>(MAX_UDP_PACKET_SIZE)?(MAX_UDP_PACKET_SIZE):nRemainSize;
			
			if (yirecvu(sock, &pIndex[size-nRemainSize], nReceiveSize, (timeout==-1)?0:&timeout, addr, port)!=nReceiveSize)
			{
				return eSE_RECV_ERROR;
			}

			//printf("recieved.... %d/%d\n", nRemainSize, size);
			yisendu(sock, &dummy, sizeof(int), *addr, *port);



			if (bIsEncoded==True)
			{
				if (packet->encode_key!=0)
				{
					if (yidecodex(&pIndex[size-nRemainSize], nReceiveSize-sizeof(int), packet->encode_key)==0)
					{
						return eSE_INVALID_PACKET;
					}
				}
			}

			nRemainSize-=nReceiveSize;
		}

		return size;
	}
	
	res = yirecvu(sock, data, size/*MAX_UDP_PACKET_SIZE*//*sizeof(packet)*/, (timeout==-1)?0:&timeout, addr, port);

	{
		unsigned short checksum = __UF32_CHECKSUM__(packet->flag32);
		int id = __UF32_ID__(packet->flag32);

		if (size!=res)
		{
			return eSE_INVALID_PACKET;
		}

		//memcpy(data, &buffer[sizeof(*packet)+sizeof(int)], size);

		if (bIsEncoded==True)
		{
			if (packet->encode_key!=0)
			{
				if (yidecodex(data, size, packet->encode_key)==0)
				{
					return eSE_INVALID_PACKET;
				}
			}
		}

		if (checksum!=0 &&
			checksum!=yichecksum(data, size, 4))
		{
			return eSE_INVALID_PACKET;
		}
	}	
	return res;
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

// -------------------------
int yiwaitsock(int sock, int* timeout)
{
	if (timeout==0)
	{
		return 1;
	}
	else
	{
		long sec=0, usec=0;
		int res=0;
		fd_set fds;
		struct timeval tv;

		FD_ZERO(&fds);
		FD_SET(sock, &fds);

		if (*timeout==-1)
		{
			return select(0, &fds, 0, 0, 0);
		}

		sec = *timeout/1000;
		usec = (*timeout%1000)*1000;
		tv.tv_sec=sec;
		tv.tv_usec=usec;

		//yisockblk(sock, 1);
		res = select(0, &fds, 0, 0, &tv);


		if (FD_ISSET(sock, &fds)!=0)
		{
			return 1;
		}
		//if (tv.tv_sec==0 &&
		//		tv.tv_usec==0)
		//{
		//	*timeout=0;
		//	return 0;
		//}
		*timeout = tv.tv_sec*1000+tv.tv_usec/1000;
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
	return select(sock+1, &fds, 0, 0, &tv);
}

// -------------------------
int yisockclear(int sock)
{
	int timeout=0;
	int n=0;
	int res;
	char buffer[MAX_UDP_PACKET_SIZE];

	while(1)
	{
		if ((res=IsInbuffer(sock))<0)
		{
			return -1;
		}
		if (res==0)
		{
			return n;
		}
		n+=yirecvu(sock, buffer, MAX_UDP_PACKET_SIZE, 0, 0, 0);
	}

	return n;
}

#include <ws2tcpip.h>
// -------------------------
unsigned int yigetlocaladdress(char* pszAddress, const char* pszPort)
{
    char ac[80];
    int i=0;
    struct addrinfo hint,* _info;
    unsigned int address;
	struct addrinfo* pNext=0;

    if (gethostname(ac, sizeof(ac))==-1)
    {
            return 0;
    }

    memset(&hint, 0, sizeof(hint));
    hint.ai_socktype=SOCK_STREAM;
    hint.ai_family=AF_INET;

    if (getaddrinfo(ac, pszPort, &hint, &_info)!=0)
    {
            return 0;
    }

//	for (pNext=_info; pNext; pNext=pNext->ai_next)
//	{
		address = ((struct sockaddr_in*)_info->ai_addr)->sin_addr.s_addr;

		if (pszAddress!=0)
		{
			strcpy(pszAddress, yiltostr(address));
		}
//	}

    freeaddrinfo(_info);
    return address;
}

#include <MSTcpIp.h>
// -------------------------
int yiTcpSetKeepAlive(int sock, Bool bIsEnable, int nTcpTimeout, int nRetryInterval)
{
	BOOL bOptVal	= TRUE; 
	int bOptLen		= sizeof(BOOL); 
	int iOptVal		= 0; 
	int iOptLen		= sizeof(int); 
	DWORD dwBytes	= 0;
	struct tcp_keepalive se={0, }, sReturns={0,};


	if (getsockopt(sock, SOL_SOCKET, SO_KEEPALIVE,  (char*)&iOptVal, &iOptLen) == SOCKET_ERROR) { 
		return -1;
	} 

	if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&bOptVal, iOptLen)==SOCKET_ERROR) {
		return -1;
	}

	se.onoff=1;
	se.keepaliveinterval=nRetryInterval;//1000*60*10; // 10分
	se.keepalivetime=nTcpTimeout; // 10分
	if (WSAIoctl(sock, SIO_KEEPALIVE_VALS, &se, sizeof(se), &sReturns, sizeof(sReturns), &dwBytes, 0, 0)==0)
	{
		return -1;
	}

	return 0;
}


// -------------------------
const char* yiltostr(unsigned int addr)
{
	static char str[16];
	unsigned char* p=(unsigned char*)&addr;
	sprintf(str, "%u.%u.%u.%u", 0xFF&p[0], 0xFF&p[1], 0xFF&p[2], 0xFF&p[3]);

	return str;
}

// -------------------------
void yisockblk(int sock, int bIsBlock)
{
#ifdef WIN32
        unsigned long val=(bIsBlock==1)?0:1;
        ioctlsocket(sock, FIONBIO, &val);
#endif
}
#ifndef STATIC_COMPILE
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")

// -----------------------------
// for ssl
// -------------------------
sockssl_t yiconnect_ssl(address_t address, int port, int nRetry)
{
	_sockssl_t ssl;
    SSL_CTX     *ctx;
    SSL         *sslx;
    SSL_METHOD  *method;
    const char  *seed = "DHJSHD2HSDopHSFewqAJKAJKytFrJKAFuKytJAFJKAS4DFJ";


	int sock = yiconnect(address, port, nRetry);

	if (sock<0)
	{
		return 0;
	}
    RAND_seed(seed, strlen(seed));
    SSLeay_add_ssl_algorithms();
    method = (SSL_METHOD*)SSLv23_client_method();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(method);
    SSL_CTX_set_mode(ctx, SSL_MODE_AUTO_RETRY);

    sslx = SSL_new(ctx);
    SSL_set_fd(sslx, sock);
    SSL_connect(sslx);

	ssl.sock=sock;
	ssl.ssl=sslx;
	ssl.ctx=ctx;

	return (sockssl_t)yitoheap(&ssl, sizeof(ssl));
	
}

// ここらへん、
// http://d.hatena.ne.jp/f99aq/20110710/1310296402


// -------------------------
int yisend_ssl(sockssl_t sock, const void* p, int size)
{
	if (sock->ssl)
	{
		int sum=0;
		int remain=size;
		int n;
		char* pIndex=(char*)p;
		YIDEBUG("yisend_ssl(size:%d)", size);
RETRY:
		n = SSL_write(sock->ssl, pIndex, remain);

		YIDEBUG("yisend_ssl() result=%d", n);

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
			YIDEBUG("yisend_ssl() sum:%d != size", sum, size);
			goto RETRY;
		}

		return n;
	}

	return yisend(sock->sock, p, size, False);
}

// -------------------------
#define SSL_ANY_SIZE 256

// -------------------------
int yirecv_ssl(sockssl_t sock, const void* p, int size, int nTimeOut)
{
	if (sock->ssl)
	{
		char* pIndex=(char*)p;
		int remain=0;
		int sum=0;
		int _timeout=nTimeOut;

		YIDEBUG("yirecv_ssl(size:%d, timeout:%d)", size, nTimeOut);

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
			YIDEBUG("yirecv_ssl() recved:%d", n);

			if (n==SSL_ERROR_WANT_READ)
			{
				Sleep(SSL_ERR_TIMEOUT);
				YIDEBUG("yirecv_ssl() wait timeout(SSL_ERROR_WANT_READ)");
				goto RETRY;
			}
			else if (n<0)
			{
				YIDEBUG("yirecv_ssl() wait recieved (%d) ret:%d", n, sum);
				Sleep(SSL_ERR_TIMEOUT);
				goto RETRY;
				//yisockblk(sock->sock, True);
				//return sum;//return 0;
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
				YIDEBUG("yirecv_ssl() wait recieved (%d) pending: %d", sum, pending);
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
			YIDEBUG("yirecv_ssl() wait timeout(%d)", nTimeOut);


			if ((pending=SSL_pending(sock->ssl))>0)
			{
				YIDEBUG("yirecv_ssl() wait recieved (%d) pending: %d", sum, pending);
				goto READ;
			}

			if (sum==0)
			{
				nTimeOut=SSL_DEFAULT_TIMEOUT;
				goto RETRY;
			}

			// 内部で書き込み中であれば？
			if (yisockIsWritable(sock->sock)==False)
			{
				nTimeOut=SSL_DEFAULT_TIMEOUT;
				goto RETRY;
			}
		}
		
		//yisockblk(sock->sock, True);
		YIDEBUG("yirecv_ssl() wait recieved ret: %d", sum);
		return sum;
	}
	nTimeOut*=10;
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

#endif