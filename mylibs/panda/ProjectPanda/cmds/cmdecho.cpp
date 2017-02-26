#include "yi.h"
#include "yistd.h"
#include "yisock.h"
#include "yiEncoder.h"
#include "svryenv.h"

#include <string.h>
#include <unistd.h>

main(int argc, char** argv)
{
	Bool bIsClearPacket=True;
	int packetId=ePID_ECHO;

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
	if (argv[2]!=0)
	{
		if (strcmp(argv[2], "-s")==0)
		{
			packetId=ePID_GET_STATUS;
		}
	}

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
	packet.id = packetId;

	unsigned long security=0;
	if (bIsClearPacket==False)
	{
		security = cyiEncoder::GetYotsuyaSecurityCode(argv[1]);

	}
	char buffer[256];
	sprintf(buffer, "%s", argv[3]?argv[3]:"Hellow!!");
	yisendx(sock, packetId, 0, 0, buffer, strlen(buffer)+1, False);

	buffer[0]=0;
	if (yirecvx(sock, &packet, buffer, security, 1000)>=0)
	{
		syiSecs sec = yiCompareSecs(secStart, yiGetCurrentSec());
		yiTrace(buffer);
		yiTrace("response %0d.%06d\n", sec.sec, sec.usec);
	}
	else
	{
		yiTrace("recive failure \n");
	}


	return 0;

}

