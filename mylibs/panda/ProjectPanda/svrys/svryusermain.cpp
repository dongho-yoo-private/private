#include "yi.h"
#include "yitrace.h"
#include "svryuser.h"

#define SERVER_NAME "UserServerYotsuya"

main()
{
	csvryuser* svr = new csvryuser(SERVER_NAME);
	svr->Start();
}

