#include "ypnGoNetwork.h"

// ---------------------------
cypnGoNetwork::cypnGoNetwork(void)
	: cyPrivateNetwork()
{
}

// ---------------------------
cypnGoNetwork::~cypnGoNetwork(void)
{
}

// ---------------------------
Bool cypnGoNetwork::SendTestMessage(int n, char* buffer, int size)
{
	return SendToUser(n, (int)eGNPMSG_TEST, buffer, size);	
}

// ---------------------------
Bool cypnGoNetwork::OnMessage(socket_t sock, int id, void* buffer)
{
	if (cyPrivateNetwork::OnMessage(sock, id, buffer)==False)
	{	
		return False;
	}

	switch(id)
	{
		case eGNPMSG_TEST :
			printf("recved: %s\n", buffer);
			break;
	
	}

	return True;
		
}	

