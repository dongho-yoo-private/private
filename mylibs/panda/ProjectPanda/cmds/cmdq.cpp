#include "yi.h"
#include "yistd.h"
#include "yisock.h"
#include "yiEncoder.h"
#include "svryenv.h"

#include <string.h>
#include <unistd.h>

main(int argc, char** argv)
{
	Bool bIsClearPacket=False;
	int packetId=ePID_REQUEST_TERMINATE;

	if (argv[1]==0)
	{
		yiTrace("no argument...\n");
		return 0;
	}

	/*if (argv[2]!=0)
	{
		if (strcmp(argv[2], "-c")==0)
		{
			bIsClearPacket=True;
		}
	}*/
	bIsClearPacket=True;


	char localport[256];
	int i;
	int cnt=0;
	int loopcnt=10;

	int sock;

	syiSecs secStart = yiGetCurrentSec();

	sock = yiconnect_unix(csvryEnv::GetLocalPortName(argv[1], localport), 1);

	if (sock<0)
	{
		yiTrace("connect error\n");
		return 0;
	}
	
	syipacket packet;
	packet.id = packetId;//ePID_ECHO;

	unsigned long security=0;
	if (bIsClearPacket==False)
	{
		security = cyiEncoder::GetYotsuyaSecurityCode(argv[1]);

	}
	yisendx(sock, ePID_REQUEST_TERMINATE, security, security, 0, 0, False);

	char recved[1024]={0, };
	if (yirecvx(sock, &packet, recved, security, 1000)>=0)
	{
		//yiTrace("recive failure \n");
		yiTrace(recved);
	}
	else
	{
		yiTrace("recive failure \n");
	}

	syiSecs sec = yiCompareSecs(secStart, yiGetCurrentSec());
	yiTrace("response %0d.%06d\n", sec.sec, sec.usec);
}

