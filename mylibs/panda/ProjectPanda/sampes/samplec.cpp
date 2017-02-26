#include "yi.h"
#include "yistd.h"
#include "yiclient.h"
#include "sycenv.h"

#include <string.h>
#include <pthread.h>

void* __write__(void* data)
{
	cyiClient* pClient = (cyiClient*)data;

	char buffer[4096];
	int n;
	if ((n=pClient->write(buffer, 4096))!=4096)
	{
		yiTrace("write failure %d bytes write (%d)\n", n);
		//errcnt++;
	}
}

main(int argc, char** argv)
{
	Bool bIsClearPacket=False;

	if (argv[1]==0)
	{
		yiTrace("no argument...\n");
	}

	if (argv[2]!=0)
	{
		if (strcmp(argv[2], "-c")==0)
		{
			bIsClearPacket=True;
		}
	}

	cyiClient* client  = new cyiClient(cyiClient::itol("192.168.1.7"), 10006, "samplesvr");
	//cyiClient* client  = new cyiClient(cyiClient::htol("www.yahoo.co.jp"), 10005, "samplesvr");
	//

	if (client->connect(5)==False)
	{
		yiTrace("connection failure!\n");
		delete client;
		return 0;
	}
	
	ssyNormalPacket packet;
	packet.id = 300;

	if (client->write(packet)==False)
	{
		yiTrace("write failure! \n");
		delete client;
		return 0;
	}

	/*int sock = client->GetSock();
	int flag=fcntl(sock, F_GETFL, 0);
	if(flag<0) yiTrace("fcntl(GET) error");
	if (fcntl(sock, F_SETFL, flag|O_NONBLOCK)<0) yiTrace("fcntl(NONBLOCK) error");*/

	syiSecs secStart = yiGetCurrentSec();
	int cnt=0;
	int errcnt=0;
	while(cnt!=1000)
	{
		char buffer[4096];
		int n;

/*		if ((n=client->read(buffer, 4096, 5000))!=4096)
		{
			yiTrace("read failure %d bytes read, (%d)\n", n, cnt);
		}
*/
		/*pthread_t th;
		if (pthread_create(&th, 0, __write__, client)==0)
		{
			pthread_detach(th);
		}*/

		if ((n=client->write(buffer, 4096))!=4096)
		{
			yiTrace("write failure %d bytes write (%d)\n", n, cnt);
			errcnt++;
		}

	//if(fcntl(sock, F_SETFL, flag)<0) yiTrace("fcntl(END) error");

		cnt++;
	}

	syiSecs sec = yiCompareSecs(secStart, yiGetCurrentSec());
	yiTrace("all %d read/write (err:%d), response %0d.%06d\n", cnt, errcnt, sec.sec, sec.usec);

	/*char buffer[4096];
	int n;	
	if ((n=client->read(buffer, 4096, 5000))!=4096)
	{
		yiTrace("read failure %d bytes read, (%d)\n", n, cnt);
	}*/
	client->disconnect();

}

