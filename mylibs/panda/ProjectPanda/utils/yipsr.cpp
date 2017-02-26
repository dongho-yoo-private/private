#include "yipsr.h"
#include "yistd.h"
#include "yitrace.h"
#include <string.h>

// -------------------------------
Bool FASTCALL yiPsrSplit(const char* p, const char* spliter, cyiList& list)
{
	char* pIndex = (char*)p;
	int cnt=0;
	int spliterlen = strlen(spliter);

	list.destroyer(yifree);

	while(1)
	{
		int len;
		char* buffer;
		char* pTop=pIndex;
		pIndex = strstr(pIndex, spliter);

		if (pIndex==0)
		{
			break;
		}

		len = pIndex-pTop;

		if (len==0)
		{
			pIndex+=spliterlen;
			continue;
		}

		buffer = (char*)yialloc(len);
		strncpy(buffer, pTop, len);
		list.addref(buffer);
		pIndex+=spliterlen;
		cnt++;
	}

	return cnt?True:False;
}

// -------------------------------
char* FASTCALL yiPsrUntilCpy(char* dest, const char* src, const char* until)
{
	char* pIndex = strstr(src, until);
	int n=0;
	if (pIndex==0)
		return 0;

	//printf("psr: %s\n", pIndex);
	n = pIndex-src;
	memcpy(dest, src, n);
	dest[n]=0;
	

	return pIndex+1;
}

// -------------------------------
char* yiPsrFromCpy(char* dest, const char* src, const char* from)
{
	char* pIndex = strstr(src, from);
	if (pIndex==0)
		return 0;
	pIndex=&pIndex[strlen(from)];
	return strcpy(dest, pIndex);
}

// -------------------------------
char* FASTCALL yiPsrBetweenCpy(char* dest, const char* src, const char* from, const char* until)
{
	char* pStartIndex = strstr(src, from);

	if (pStartIndex==0)
		return 0;

	char* pEndIndex = strstr(pStartIndex, until);	

	if (pEndIndex==0)
		return 0;

	pStartIndex = &pStartIndex[strlen(from)];

	yistricpy(dest, pStartIndex, pEndIndex);

	return pEndIndex;
}

