#include "svrybase.h"

//
Bool csvrybase::OnTeminate(int sock, cConnectionManager& mgr)
{
	return True;
}

// -------------------------------------
Bool csvrybase::OnPreAcceptLocal(int sock)
{
	return True;
}

// -------------------------------------
Bool csvrybase::OnPreAccept(int sock, unsigned int from)
{
	return True;
}

// -------------------------------------
Bool csvrybase::OnAccepted(int sock, int index, void** pExtra)
{
	return True;
}

// ---------------------------------
Bool csvrybase::OnPreMessage(int sock, void** pExtraDataRef, Bool& bIsHandled)
{
	bIsHandled=False;
	return True;
}

// ---------------------------------
Bool csvrybase::OnMessage(int sock, ssyNormalPacket& packet, void** pExtraDataRef)
{
	return True;
}

// ---------------------------------
Bool csvrybase::OnMessageLocal(int sock, ssyNormalPacket& packet, void** pExtraDataRef)
{
	yiTraceWithTime("csvrybase::OnMessageLocal(%x) called\n", (int)packet.id);

	if ((packet.id&ePID_TERMINATE)!=0)
	{
		QuitMessage(packet.id);
		return True;
	}

	switch (packet.id)
	{
		case ePID_ECHO:
		{
			char buffer[256];
			//int  timeout=100;
			if (yirecv(sock, buffer, packet.sizex, 0)!=packet.sizex)
			{
				return True;
			}

			yidecodex(buffer, packet.sizex, packet.encode_key);	

			yiTraceWithTime(" recieved: say %s\n", buffer);

			if (yisendx(sock, ePID_ECHO, m_LocalInfo.bIsPacketClear ? 0:m_security, packet.encode_key, 0, 0, True)!=0);
			{
				return True;
			}
			
			break;
		}
		case ePID_GET_STATUS:
		{
			break;
		}
	}

	return True;
}

// -----------------------------------
void csvrybase::OnClientClose(int sock, void** pExtraDataRef)
{
	yiTrace("csvrybase::OnClientClose() called\n");
	return ;
}
