#define __POSIX__ 1
#include "../yi.h"
#include "../yisock.h"
#include "../yiupnp.h"
#include "../yifile.h"
#include <stdio.h>


// ------------------------------
int main(int argc, char** argv)
{
	int size=0;
	HandleUpnp h = 0;//yiupnpCreatex();
	char addr[64];
	Bool bIsCreateMode=True;
	int nPort = atoi(argv[1]);
	void* p;
	
	if (argv[2] && strcmp("-d", argv[2])==0)
	{
		bIsCreateMode=False;
	}

	p = yiFileLoad("upnp.info", 0);	

	if (p)
	{
		h = yiupnpImport(p);
	}

	if (h==0)
	{
		const void* x=0;
		h = yiupnpCreatex();
		if (h==0)
		{
			printf("yiupnpCreatex() failure\n");
			return 0;
		}

		x = yiupnpExport(h, &size);
		yiFileWrite("upnp.info", x, 0, size);

	}

	if (yiupnpGetExternalAddressx(h, addr)==False)
	{
		printf("yiupnpGetExernalAddress() failure\n");
		return 0;
	}

	printf("external addr: %s\n", addr);


	if (bIsCreateMode==True)
	{
		if (yiupnpAddPortMapx(h, "192.168.1.7", nPort, True, "yotsuya")==False)
		{
			printf("yiupnpAddPortMapx() failure\n");
			return 0;
		}
	}
	else
	{
		if (yiupnpDeletePortMapx(h, nPort, True)==False)
		{
			printf("yiupnpAddPortMapx() failure\n");
			return 0;
		}
	}
	yiupnpDestroyx(h);

	return 0;	
}

