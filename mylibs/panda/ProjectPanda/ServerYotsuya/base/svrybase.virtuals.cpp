#include "svrybase.h"

// -------------------------------------
Bool csvrybase::OnError(int error, unsigned int userId, void* p)
{
	return True;
}

//
Bool csvrybase::OnTeminate(int sock, csvryConnMgr& mgr)
{
	return True;
}

// -------------------------------------
Bool csvrybase::OnPreAcceptLocal(int sock)
{
	return True;
}

// -------------------------------------
Bool csvrybase::OnStatus(char* buffer)
{
	*buffer=0;
	return True;
}

// -------------------------------------
Bool csvrybase::OnPacket(int sock, Bool bIsLocal, Bool& bIsConnectionClosed)
{
	return False;
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

			if (yisendx(sock, ePID_ECHO, m_LocalInfo.security, packet.encode_key, 0, 0, False)!=0);
			{
				return True;
			}
			
			break;
		}
		case ePID_GET_STATUS:
		{
			char buffer[256];
			buffer[0]=0;
			OnStatus(buffer);
			yiTraceWithTime("get status received %s\n", buffer);
			if (yisendx(sock, ePID_GET_STATUS, m_LocalInfo.security, packet.encode_key, buffer, strlen(buffer), False)!=0);
			{
				return True;
			}
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
