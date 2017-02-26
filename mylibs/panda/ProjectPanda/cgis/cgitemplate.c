#include <stdio.h>
#include <stdlib.h>

#define MAX_POST_STRING 1024



int main(int argc, char** argv)
{
	char buffer[MAX_POST_STRING];
	const char* pszLen = getenv("CONTENT_LENGTH");
	int n;

	printf("Content-type: text/html\n\n");

	if (pszLen==0)
	{
		printf("CONTENT_LENGTH is NULL\n");
		return 0;
	}

	n = atoi(pszLen);

	if (n>MAX_POST_STRING -1)
	{
		printf("string too long %d\n", n);
		return 0;
	}

	if (read(0, buffer, n)!=n)
	{
		printf("read error \n");
		return 0;
	}

	buffer[n]=0;

	return 0;
}

