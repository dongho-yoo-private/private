#include <stdio.h>
#include "ypnGoNetwork.h"
#include "yistd.h"
#include "yiEncoder.h"

// -------------------------------
int main(int argc, char** argv)
{
	unsigned int event = yiEventInit();
	cypnGoNetwork network;

	//network.InitNetwork(yiitol("192.168.1.7"), 30111);

	if (network.Start(event, yiitol("192.168.1.7"), 10003, cyiEncoder::GetYotsuyaSecurityCode("UserServerYotsuya"), 0, 0, 0, 0)==False)
	{
		printf("network start failure\n");
		return 0;
	}

	sNetworkInfo* net = (sNetworkInfo*)network.GetNetworkInfo();
	printf("------ mynetwork info\n");
	printf("type:%d \n", net->type);
	printf("address:%s \n", yiltostr(net->address));
	printf("port:%d \n", net->port);


	int nRoomNo;
	printf("connect room no> ");
	scanf("%d", &nRoomNo);

	if (network.Connect(0, 0, nRoomNo, 0)==False)
	{
		printf("connect error\n");
		return False;
	}

	while(1)
	{
		int id;
		void* data;
		network.GetEvent(event, &id, &data);
		if (id==eNRMSG_ADD_USER)
		{
			printf("User Added.....\n");
		}
		else if (id==eNRMSG_CONNECTION_CLOSED)
		{
			printf("User Connection Closed.....\n");
		}
	}
	network.Terminate();

	return 0;		
}

