#include "../yi.h"
#include "../yisock.h"
#include "../yiupnp.h"
#include <stdio.h>


// ------------------------------
main()
{
	//int sock = yiudp(0, 0);	
/*
	int port=0;
	char url[1024];
	char szAddress[128];
	char controlUrl[64];
	char szExternalAddress[128];
	Bool bIsPPPConnection;


	if (yiupnpGetRouterControlUrl(szAddress, &port, controlUrl, &bIsPPPConnection)==False)
	{
		printf("no router\n");
		return 0;
	}

	if (yiupnpGetExternalAddress(szAddress, port, controlUrl, bIsPPPConnection, szExternalAddress)==False)
	{
		printf("yiupnpGetExternalAddress failure\n");
		return False;
	}

	if (yiupnpAddPortMap(szAddress, port, controlUrl, bIsPPPConnection, "192.168.1.7", 32333, True, "TestPort")==False)
	{
		return 0;
	}

	if (yiupnpDeletePortMap(szAddress, port, controlUrl, bIsPPPConnection, 32333, True)==False)
	{
		return 0;
	}*/

	HandleUpnp h = yiupnpCreatex();
	char addr[64];

	if (h==0)
	{
		printf("yiupnpCreatex() failure\n");
		return 0;
	}

	if (yiupnpGetExternalAddressx(h, addr)==False)
	{
		printf("yiupnpGetExernalAddress() failure\n");
		return 0;
	}

	printf("external addr: %s\n", addr);

	if (yiupnpAddPortMapx(h, "192.168.1.7", 32333, True, "yotsuya")==False)
	{
		printf("yiupnpAddPortMapx() failure\n");
		return 0;
	}

	if (yiupnpAddPortMapx(h, "192.168.1.7", 32333, True, "yotsuya")==False)
	{
		printf("yiupnpAddPortMapx() failure\n");
		return 0;
	}

	return 0;	
}

