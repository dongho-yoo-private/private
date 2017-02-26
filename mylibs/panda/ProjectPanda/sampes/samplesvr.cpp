#include "samplesvr.h"
#include "sycTypes.h"

#include <sys/types.h>
#include <sys/socket.h>

// --------------------------
csamplesvr::csamplesvr(const char* servername)
	: csvrybase(servername)
{
}

// --------------------------
csamplesvr::~csamplesvr()
{
}

// --------------------------
Bool csamplesvr::OnMessage(int sock, ssyNormalPacket& packet, void** pExtraDataRef)
{
	char buffer[4096];

	while(1)
	{
/*
		int flag=fcntl(sock, F_GETFL, 0);
		if(flag<0) yiTrace("fcntl(GET) error");
		if (fcntl(sock, F_SETFL, flag|O_NONBLOCK)<0) yiTrace("fcntl(NONBLOCK) error");

		int n;
		if ((n=::send(sock, buffer, 4096, MSG_NOSIGNAL))!=4096)
		{

			if (n==-1)
			{
				if (errno==EPIPE)
				{
					yiTrace("csamplesvr::OnMessage() already closed\n");
					break;
				}
			}
			yiTrace("csamplesvr::OnMessage() write failure (%d)\n", n);
			//return False;
		}

		if(fcntl(sock, F_SETFL, flag)<0) yiTrace("fcntl(END) error");
*/
		int n;
		if ((n=::read(sock, buffer, 4096))!=4096)
		{
			yiTrace("csamplesvr::OnMessage() read failure (%d)\n", n);
			//return False;
		}

		if (n==0 ||
			n==-1)
		{
			yiTrace("csamplesvr::OnMessage() connection closed (%d)\n", n);
			break;
		}
	}
	return True;		
}

//
Bool csamplesvr::OnMessageLocal(int sock, ssyNormalPacket& packet, void** pExtraDataRef)
{
	csvrybase::OnMessageLocal(sock, packet, pExtraDataRef);
	yiTrace("in> csamplesvr::OnMessageLocal() \n");
	return True;
}

// --------------------------
Bool csamplesvr::OnServerStart()
{
	return True;
}

Bool csamplesvr::OnServerEnd()
{
	return True;
}



