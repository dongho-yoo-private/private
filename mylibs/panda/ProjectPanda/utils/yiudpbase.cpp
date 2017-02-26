#include "yiudpbase.h"

// --------------------------------
Bool cyiUdpBase::VerifyFlag(syiUdpPacket* packet, void* buffer)
{
	if ((packet->flag & ePF_YOTSUYA_FLAG )!=0)
	{
		return True;
	}

	if (packet->security != m_security_code)
	{
		return True;
	}

	if ((packet->flag & REQ_CON)!=0)
	{
		AddAcception(buffer);	
	}
}

// --------------------------------
void cyiUdpBase::__recieve__()
{
	char buffer[__MAX_UDP_BUFFER__<<1];
	fdset fds;

	FD_ZERO(&fds);
	while(m_bQuit)
	{
		int res;
		FD_SET(&fds, m_sock);
		if ((res = select(m_scok+1, 0, 0, 0, &tv))<0)
		{
			break;
		}
		
		int n = yirecv(m_sock, buffer, __MAX_UDP_BUFFER__, &addr, &port);

		if (n==__KEEP_ALIVE_PACKET_SIZE__)
		{
			continue;
		}

		xDecode(buffer, m_security_code);
		syiUdpPacket* packet = (syiUdpPacket*)buffer;

		if (packet->size!=n)
		{
			continue;
		}

		res = VerifyFlag(packet->flag16);

		if (res==-1)
		{
			continue;
		}

		// noral message.
		if (SetPacket(packet->id&0xFFFF, &buffer[sizeof(syiUdpPacket)], n-sizeof(syiUdpPacket))==False)
		{
			yisend(m_sock, &b, );
			continue;
		}

	}
}

