#include "yi.h"
#include "yitrace.h"
#include "samplesvr.h"

#define SERVER_NAME "samplesvr"

main()
{
	csamplesvr* svr = new csamplesvr(SERVER_NAME);
	svr->Start();
}

