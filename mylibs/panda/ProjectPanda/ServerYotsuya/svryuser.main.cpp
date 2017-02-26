#include "yi.h"
#include "yitrace.h"
#include "svryuser.h"

#define SERVER_NAME "UserServerYotsuya"

int main(int argc, char** argv)
{
	if (argv[1])
	{
		printf("%s!!!!!\n", argv[1]);
	}
	csvryuser* svr = new csvryuser(argv[1]==0?SERVER_NAME:argv[1]);
	svr->Start();
}

