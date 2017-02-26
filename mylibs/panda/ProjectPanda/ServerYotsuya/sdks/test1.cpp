#include <stdio.h>
#include "ypnGoNetwork.h"
#include "yistd.h"
#include "yithread.h"
#include "yiEncoder.h"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int userindex=-1;

void* xOnRandomMessage(void* param)
{
	srand(time(0));
	int i=0;
	while(1)
	{
		char buffer[128];
		int n = (rand()%2000);
		usleep(n*100);

		if (userindex==-1)
		{
			continue;
		}

		sprintf(buffer, "Hellow......(%d)", i++);
		int len = strlen(buffer)+1;
		len = (len%16) ? (16-(len%16))+len:len;
		if (((cypnGoNetwork*)cypnGoNetwork::GetInstance())->SendTestMessage(userindex, buffer, len)==False)
		{
			cypnGoNetwork::GetInstance()->DisConnectUser(userindex);
			break;
		}
		//printf("%s, size:%d\n", buffer, len);
	}
}

// -------------------------------
int main(int argc, char** argv)
{
	unsigned int event = yiEventInit();
	cypnGoNetwork network;

	network.InitNetwork(yiitol("192.168.1.7"));

	if (network.Start(event, yiitol("192.168.1.7"), 10003, cyiEncoder::GetYotsuyaSecurityCode("UserServerYotsuya"), 0, 0, 0)==False)
	{
		printf("network start failure\n");
		return 0;
	}

	sNetworkInfo* net = (sNetworkInfo*)network.GetNetworkInfo();
	printf("------ mynetwork info\n");
	printf("type:%d \n", net->type);
	printf("address:%s \n", yiltostr(net->address));
	printf("port:%d \n", net->port);
	printf("index:%d \n", net->index);

	yiThreadStartx(xOnRandomMessage, 0, 0);

	while(1)
	{
		int id;
		void* data;
		network.GetEvent(event, &id, &data);

		if (id==eNRMSG_ADD_USER)
		{
			userindex=(int)data;
			printf("User Added..... %d\n", userindex);
		}
		else if (id==eNRMSG_CONNECTION_CLOSED)
		{
			printf("User Connection Closed.....\n");
			break;
		}
	}
	network.Terminate();

	return 0;		
}

