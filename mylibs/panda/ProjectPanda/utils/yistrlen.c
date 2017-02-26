#include "yi.h"
#include "yistd.h"

/**/
int inline __yistrlen__(const char* str)
{
	int i;
	long* p=(long*)str;
	char* pchar = (char*)str;

	if (*pchar==0) return ;

	if ((i=__shift_mod__(__long_shift__, (long)str))!=0)
	{
		while(i)
		{
			pchar++;
			i--;
		}
		p=(long*)pchar;
	}

	while(*p) ++p;

	pchar = (char*)p;
	while(*pchar == 0) pchar--;

	return (long)pchar-(long)str+1;
}

/**/
int /*__attribute__((fastcall))*/ yistrlen(const char* str)
{
	return __yistrlen__(str);
}

