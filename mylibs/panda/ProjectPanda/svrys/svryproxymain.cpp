#include "yi.h"
#include "yitrace.h"
#include "svryproxy.h"

#define SERVER_NAME "ProxyServerYotsuya"

main()
{
	csvryproxy* svr = new csvryproxy(SERVER_NAME);
	svr->Start();
}
