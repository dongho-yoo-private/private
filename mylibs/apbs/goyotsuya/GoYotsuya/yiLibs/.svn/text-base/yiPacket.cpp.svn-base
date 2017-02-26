#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "yi.h"
#include "yiPacket.h"
#include "yiEncoder.h"
#include "yistd.h"

#ifdef _WIN32
#include <io.h>
#include <process.h>
#endif

#ifdef __POSIX__
#include <unistd.h>
#include <pthread.h>
#endif


// -------------------------------------------
ePacketResult cyiPacket::ReadNormalPacket(int sock, ssyNormalPacket& packet, sSecurityInfo& info)
{
	char buffer[__YOTSUYA_NORMAL_PACKET_SIZE__];
	int n = recv(sock, buffer, __YOTSUYA_NORMAL_PACKET_SIZE__, 0);

	if (n!=__YOTSUYA_NORMAL_PACKET_SIZE__)
	{
		if (n==0 ||
			n==1 ||
				n==-1)
		{
			yiTrace("connection closed\n");
			return ePR_INVALID_SOCKET;
		}
		yiTrace("read failure %d bytes read\n", n);
		return ePR_INVALID_PACKET;
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
Bool cyiPacket::WriteNormalPacket(int sock, ssyNormalPacket& packet, sSecurityInfo& info, Bool bIsSync)
{
	char buffer[__YOTSUYA_NORMAL_PACKET_SIZE__];

	packet.reserved=info.nSecurity;
	cyiEncoder en(info.nRandNo, info.nMask);

	if (en.Encode(buffer, &packet, sizeof(packet))==NULL)
	{
		return False;
	}

	if (bIsSync==False)
	{
		return cyiPacket::write(sock, buffer, __YOTSUYA_NORMAL_PACKET_SIZE__);
	}

	int n;	
	if ((n=::send(sock, buffer, __YOTSUYA_NORMAL_PACKET_SIZE__, 0))!=__YOTSUYA_NORMAL_PACKET_SIZE__)
	{
		yiTrace("cyiPacket::WriteNormalPacket() write error (%d bytes written)\n", n);
		return False;
	}

	return True;
}

// ------------------------------
Bool cyiPacket::WriteNormalPacket(int sock, ssyNormalPacket& packet, const void* extData, int extSize, sSecurityInfo& info, Bool bIsSync)
{
	if (cyiPacket::WriteNormalPacket(sock, packet, info, bIsSync)==False)
	{
		return False;
	}

	if (bIsSync==False)
	{
		return cyiPacket::write(sock, extData, extSize);
	}

	if (write(sock, extData, extSize)!=extSize)
	{
		return False;
	}

	return True;
}

void __write__(void* data)
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

#ifdef __POSIX__
	pthread_t th;
	if (pthread_create(&th, 0, __write__, p)==0)
	{
		pthread_detach(th);
		return True;
	}
#else
	if (::_beginthread(__write__, 0, p)!=-1)
	{
		return True;
	}
#endif

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
		return True;
	}
	if (res==0)
	{
		return True;
	}

	return False;
}

// ------------------------------
char* cyiPacket::makebuffer(char* buffer, ...)
{
	void** pArgvs = (void**)&buffer;
	int i=0;
	int n=0;

	pArgvs = &pArgvs[1];
	
	while(pArgvs[i])
	{
		void* data = pArgvs[i++];
		int size = (int)pArgvs[i++];

		memcpy(&buffer[n], data, size);
		n+=size;
	}
	return buffer;
}

