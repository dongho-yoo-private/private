#include "../../utils/yistd.h"
#include "../../utils/yitime.h"
#include "../../utils/yisock.h"
#include "../../utils/yimutex.h"
#include "../../utils/yiHashTable.h"
#include "../base/svryenv.h"
#include "../svrydefs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// ----------------------------------
static void xCreateTemporaryName(char* buffer)
{
	int n = (rand()%12) + 4;
	int interval = 'Z'-'A';

	for (int i=0; i<n; i++)
	{
		int k = rand()%2;
		*buffer = (rand()%interval)+(k ?'A':'a'); 
		buffer++;
	}
	*buffer=0;
}

main(int argc, char** argv)
{
	syiUserRegisterData data;
	int sock;
	char localport[128];

	memset(&data, 0, sizeof(data));

	if (argv[1]!=0)
	{
		strcpy(data.szUserName, argv[1]);
		strcpy(data.szEmailAddress, argv[2]);
		data.id = cyiHashTable::GetHashLayer(argv[1], 3);
		sock = yiconnect_unix(csvryEnv::GetLocalPortName("UserServerYotsuya", localport), 1);

		if (sock<0)
		{
			printf("connect error\n");
			return 0;
		}

		if (yisendx(sock, eUSPID_REQ_REGISTER_USER, 0, 0, &data, sizeof(data), False)!=sizeof(data))
		{
			printf("send error\n");
			yiclose(sock);
			return 0;
		}

		syipacket packet;
		int result;
		if (yirecvx(sock, &packet, (void*)&result, 0, 500)!=sizeof(result))
		{
			printf("recv error \n");
			yiclose(sock);
			return 0;
		}

		if (packet.id==eUSPID_RPY_REGISTER_USER &&
							result==0)
		{
			printf("register user success\n");
			yiclose(sock);
			return 0;
		}
		printf("register user failure\n");
		yiclose(sock);
		return 0;
	}

	srand(time(0));
	syiSecs sec = yiGetCurrentSec();

for(int i=0; i<100000; i++)
{
	xCreateTemporaryName(data.szUserName);
	data.id = cyiHashTable::GetHashLayer(data.szUserName, 3);

	sock = yiconnect_unix(csvryEnv::GetLocalPortName("UserServerYotsuya", localport), 1);

	if (sock<0)
	{
		printf("connect error\n");
		continue;
	}

	if (yisendx(sock, eUSPID_REQ_REGISTER_USER, 0, 0, &data, sizeof(data), False)!=sizeof(data))
	{
		printf("send error\n");
		yiclose(sock);
		continue;
	}

	syipacket packet;
	int result;
	if (yirecvx(sock, &packet, (void*)&result, 0, 500)!=sizeof(result))
	{
		printf("recv error (%d)\n", i);
		yiclose(sock);
		continue;
	}

	if (packet.id==eUSPID_RPY_REGISTER_USER &&
		result==0)
	{
		//printf("register user success\n");
		yiclose(sock);
		continue;
	}
	//printf("register user failure\n");
	yiclose(sock);
	continue;
}
	syiSecs sec2 = yiGetCurrentSec();
	syiSecs sec3 = yiCompareSecs(sec, sec2);

	yiTrace("register end %d.%06d secs.....\n", sec3.sec, sec3.usec);
	return 0;
}

