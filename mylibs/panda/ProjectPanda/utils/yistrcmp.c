#include "yi.h"
#include "yistd.h"

// ------------------------------
long FASTCALL yistrcmp(const char* str1, const char* str2)
{
	long* p1 = (long*)str1;
	long* p2 = (long*)str2;
	long def;

	while((def = *p1-*p2)==0)
	{
		if (*p1==0||*p2==0)
		{
			break;
		}
		p1++;
		p2++;
	}

	return def;
}

// ------------------------------
long FASTCALL yistrncmp(const char* str1, const char* str2, int n)
{
	long* p1 = (long*)str1;
	long* p2 = (long*)str2;
	long def;
	int i;
	int n_shift = n>>__long_shift__;
	int remain = __shift_mod__(__long_shift__, n);

	for (i=0; i<n_shift; i++)
	{
		if ((def = *p1-*p2)!=0)
		{
			return def;
		}
		if (*p1==0&&*p2==0)
		{
			return 0;
		}
		p1++;
		p2++;
	}

	if (remain)
	{
		char* pStr1=(char*)p1;
		char* pStr2=(char*)p2;

		for (i=0; i<remain; i++)
		{
			if ((def = (long)*pStr1-(long)*pStr2)!=0)
			{
				return def;
			}
			if (*pStr1==0&&*pStr2==0)
			{
				return 0;
			}
			pStr1++;
			pStr2++;
		}
	}

	return def;
}
