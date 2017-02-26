#include "yi.h"
#include "yistd.h"
#include <string.h>


/*
 * */
extern inline int __yistrlen__(const char* src);

/*
 * */
extern inline void* yimemcpy_sse2_asm(void* dest, const void* src, int n);

/*
 * */
char* /*__attribute__((fastcall))*/ yistrcpy(char* dest, const char* src)
{
	char* ret = (char*)dest;
	register long n;

#ifdef __use_sse2__
	n = __shift_mod__(__sse2_bit_shift__, dest);
	if (n==__shift_mod__(__sse2_bit_shift__, src))
	{
		int i;
		int remain;
		unsigned long n = __yistrlen__(src);
		yimemcpy(dest, src, n);
		yistrn(dest, n);
		return ret;
	}
#endif
	n = __shift_mod__( __long_shift__, dest);
	if (n==__shift_mod__(__long_shift__, src))
	{
		long* pDest=(long*)dest;
		long* pSrc =(long*)src;
		if (n==0)
		{	
COPY_LONG:
			while(*pSrc)
			{
				*pDest++=*pSrc++;
			}
		}
		else
		{
			n=sizeof(long)-n;
			int i;
			for (i=0; i<n&&*src!=0; i++)
				*dest++=*src++;

			if (*src==0)
			{
				yistrn(ret, i);
				return ret;
			}

			pDest=(long*)dest;
			pSrc=(long*)src;
			goto COPY_LONG;
		}	
		*pDest=0;
		return ret;
	}

BYTE_COPY:
	{
		int cnt=0;
		while(*dest++=*src++) cnt++;

		yistrn(ret, cnt);
	}


MAKE_WSXSTR:

	return ret;
}

/**/
char* /*__attribute__((fastcall))*/ yistrncpy(char* dest, const char* src, int cnt)
{
	register int i=0;
	char* ret = dest;
	long* pDest = (long*)dest;
	long* pSrc = (long*)src;
	register int n = __shift_mod__( __long_shift__, dest);
	if (n==__shift_mod__(__long_shift__, src))
	{
		if (n==0)
		{	
COPY_LONG:
			while(*pSrc)
			{
				if (i>=cnt)
				{
					yistrn(ret, cnt);
					return ret;
				}
				*pDest++=*pSrc++;
				//*((unsigned long*)dest++)=*((unsigned long*)src++);
				i+=sizeof(long);
			}

			*pDest=0;
		}
		else
		{
			n=sizeof(long)-n;
			for (i=0; i<n &&*src!=0; i++)
				*dest++=*src++;

			if (cnt<=i) 
			{
				yistrn(ret, cnt);
				return ret;
			}
			else if (*src==0)
			{
				yistrn(ret, i);
				return ret;
			}

			goto COPY_LONG;
		}	
		return ret;
	}

	strncpy(dest, src, cnt);
	yistrn(dest, cnt);

	return dest;
}


/**/
char* /*__attribute__((fastcall))*/ yistrcat(char* dest, const char* src)
{
	unsigned long n = __yistrlen__(src);
	return yistrcpy(dest, &src[n]);
}
