#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __POSIX__
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#endif
#include "yiPacket.h"
#include "yiEncoder.h"
#include "yistd.h"

// -------------------------------------------
ePacketResult cyiPacket::ReadNormalPacket(int sock, ssyNormalPacket& packet, sSecurityInfo& info)
{
	char buffer[__YOTSUYA_NORMAL_PACKET_SIZE__];
	int n = read(sock, buffer, __YOTSUYA_NORMAL_PACKET_SIZE__);

	if (n!=__YOTSUYA_NORMAL_PACKET_SIZE__)
	{
		if (n==0 ||
			n==1 ||
				n==-1)
		{
			yiTrace("connection closed\n");
			return ePR_ALREADY_CLOSED;
		}
		yiTrace("read failure %d bytes read\n", n);
		return ePR_PARTIAL_RDWR;
	}

	cyiEncoder en(info.nRandNo, info.nMask);

	if (en.Decode(&packet, buffer, sizeof(packet))==NULL)
	{
		yiTrace("decode error (%x, %x)\n", info.nRandNo, info.nMask);
		return ePR_INVALID_PACKET;
	}

	if (packet.reserved!=info.nSecurity)
	{
		yiTrace("decode error not eq security (%x!=%x)\n", packet.reserved, info.nSecurity);
		return ePR_INVALID_PACKET;
	}

	return  ePR_SUCCESS;
}

// ------------------------------
ePacketResult cyiPacket::WriteNormalPacket(int sock, ssyNormalPacket& packet, sSecurityInfo& info, Bool bIsSync)
{
	char buffer[__YOTSUYA_NORMAL_PACKET_SIZE__];

	packet.reserved=info.nSecurity;
	cyiEncoder en(info.nRandNo, info.nMask);

	if (en.Encode(buffer, &packet, sizeof(packet))==NULL)
	{
		return ePR_INVALID_PACKET;
	}

	if (bIsSync==False)
	{
		return cyiPacket::write(sock, buffer, __YOTSUYA_NORMAL_PACKET_SIZE__) ? ePR_SUCCESS:ePR_THREAD;
	}

	int n;	
	if ((n=::send(sock, buffer, __YOTSUYA_NORMAL_PACKET_SIZE__, MSG_NOSIGNAL))!=__YOTSUYA_NORMAL_PACKET_SIZE__)
	{
		if (n==-1)
		{
			if (errno==EPIPE)
			{
				SYSLOG( ,"cyiPacket::WriteNormalPacket() client alreay closed\n");
				return ePR_ALREADY_CLOSED;
			}
		}
		yiTrace("cyiPacket::WriteNormalPacket() write error (%d bytes written)\n", n);
		return ePR_PARTIAL_RDWR;
	}

	return ePR_OK;
}

// ------------------------------
ePacketResult cyiPacket::WriteNormalPacket(int sock, ssyNormalPacket& packet, const void* extData, int extSize, sSecurityInfo& info, Bool bIsSync)
{
	ePacketResult res;
	if ((res = cyiPacket::WriteNormalPacket(sock, packet, info, bIsSync))!=ePR_SUCCESS)
	{
		return res;
	}

	if (bIsSync==False)
	{
		return cyiPacket::write(sock, extData, extSize) ? ePR_SUCCESS:ePR_THREAD;
	}

	if (write(sock, extData, extSize)!=extSize)
	{
		return ePR_RDWR_SIZE;
	}

	return ePR_SUCCESS;
}

static void* __write__(void* data)
{
	int* p = (int*)data;
	write(p[0], (void*)&p[4], p[1]);

	yifree(data);
}

// ------------------------------
Bool cyiPacket::write(int sock, const void* buffer, int size)
{
	int* p = (int*)yialloc(size+16);	
	p[0]=sock;
	p[1]=size;
	yimemcpy(&p[4], buffer, size);

	pthread_t th;
	if (pthread_create(&th, 0, __write__, p)==0)
	{
		pthread_detach(th);
		return True;
	}

	assert(0);
	return False;
}


// ------------------------------
Bool cyiPacket::InitPacket(ssyNormalPacket& packet)
{
	ssyNormalPacket p;
	memcpy(&packet, &p, sizeof(p));
	return True;
}

// ------------------------------
Bool cyiPacket::timeout(int sock, int nMilliSecond)
{
	struct timeval tv = {nMilliSecond/1000, (nMilliSecond%1000)*1000};
	fd_set fdset;

	FD_ZERO(&fdset);
	FD_SET(sock, &fdset);
	int res = select(sock+1, &fdset, 0, 0, &tv);

	if (res<0)
	{
		SYSLOG(LogLevel::ERROR, "select error\n");
		return True;
	}
	if (res==0)
	{
		SYSLOG(LogLevel::ERROR, "Timeout %d msecs\n", nMilliSecond);
		return True;
	}

	return False;
}

// ------------------------------
void cyiPacket::close(int sock)
{
	::shutdown(sock, 2);
	::close(sock);
}

// ------------------------------
void cyiPacket::blocking(int sock, Bool bIsBlock)
{
	if (bIsBlock)
	{
		int flag=fcntl(sock, F_GETFL, 0);
		if (flag<0)
			return ;
		fcntl(sock, F_SETFL, flag|O_NONBLOCK);
		return ;
	}

	int flag=fcntl(sock, F_GETFL, 0);
	if (flag<0)
		return ;
	fcntl(sock, F_SETFL, flag|O_ASYNC);
}
