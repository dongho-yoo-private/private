#include "../yistd.h"
#include <unistd.h>

void* OnEventThread(void* param)
{
	unsigned int eventId = (unsigned int)param;
	int n = rand()%1000;

	yiEventWait(eventId, INFINITE);

	usleep(n);
	yiEventReset(eventId);
	printf("reset and wait event...(%d)\n", eventId);
	yiEventWait(eventId, INFINITE);
	printf("exit thread (%d)\n", eventId);

	return 0;
}


main()
{
	int i=0;
	unsigned int eventId[10];

	for (i=0; i<10; i++)
	{
		eventId[i]=yiEventInit();
		if (eventId[i]==0)
		{
			printf("event init failure\n");
		}
		printf("event(%d) id:%d\n", i, eventId[i]);
		yiThreadStartx(OnEventThread, (void*)eventId[i], 0);
	}

	getchar();

	for (i=0; i<10; i++)
	{
		printf("set event:%d\n", eventId[9-i]);
		yiEventSet(eventId[9-i]);
		yiEventSet(eventId[9-i]);
	}
	getchar();

	for (i=0; i<10; i++)
	{
		printf("set event:%d\n", eventId[9-i]);
		yiEventSet(eventId[9-i]);
	}
	getchar();
}

