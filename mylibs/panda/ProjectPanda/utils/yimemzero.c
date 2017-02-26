#include "yi.h"
#include "yistd.h"
#include <string.h>

/**/
extern inline void* yimemzero_sse2_asm(void*, unsigned long);


/**/
#define __ONCE_COPY_SMALL_COUNT_SHIFT__	4

/**/
#define __ONCE_COPY_LARGE_COUNT_SHIFT__	6

#define __long_zero__(x) *x++=0;

#ifdef __x64__
#	define __16_bytes_zero__(x) \
	__long_zero__(x);\
	__long_zero__(x);
#else
#	define __16_bytes_zero__(x) \
	__long_zero__(x);\
	__long_zero__(x);\
	__long_zero__(x);\
	__long_zero__(x)
#endif

#ifdef __x64__
#	define __64_bytes_zero__(x) \
	__16_bytes_zero__(x) ;\
	__16_bytes_zero__(x) ;\
	__16_bytes_zero__(x) ;\
	__16_bytes_zero__(x) 
#else
#	define __64_bytes_zero__(x) \
	__long_zero__(x) ;\
	__long_zero__(x) ;\
	__long_zero__(x) ;\
	__long_zero__(x) 
#endif


/**/
inline void* yimemzero_normal(void* dest, int size)
{
	void* ret=dest;
	register long i;

	long cnt_high = size>>__ONCE_COPY_LARGE_COUNT_SHIFT__; /* 64 bytes block counts */
	long cnt_high_remain = __shift_mod__(__ONCE_COPY_LARGE_COUNT_SHIFT__, size); /* remain 1byte block count*/

	long cnt_middle = cnt_high_remain >>__ONCE_COPY_SMALL_COUNT_SHIFT__;/* 16 bytes block counts */
	register long cnt_middle_remain = __shift_mod__(__ONCE_COPY_SMALL_COUNT_SHIFT__, cnt_high_remain ); /* remain 1byte block count*/

	long long_cnt = cnt_middle_remain >>__long_shift__; /* 4(8)byte block count */
	register long long_cnt_remain = __shift_mod__(__long_shift__, cnt_middle_remain); /* 1byte block count */

	register long mask;

	long* p1 = (long*)dest;

	if (size<sizeof(long))
		goto BYTE_COPY;

	/* 64 byte copy*/
	for (i=0; i<cnt_high ; i++) 
	{
		__64_bytes_zero__(p1) ;
	}

	/* 16 byte copy. */
	for (i=0; i<cnt_middle ; i++) 
	{
		__16_bytes_zero__(p1) ;
	}

	/* 4(8) byte copy */
	for (i=0; i<long_cnt; i++) __long_zero__(p1);

	if (!long_cnt_remain ) ret;

BYTE_COPY:
	mask =__LONG_FULL_BIT__<<bytetobit((sizeof(long)-long_cnt_remain));
	mask=~(mask);
	*p1 &= mask;

	return ret;
} 

/**/
void* FASTCALL yimemzero(void* dest, int size)
{
	void* org = dest;
	unsigned long n;

	if (size<YI_ALIGNMENT )
	{
		goto ZERO_BY_BYTE;
	}

	if ((n = __shift_mod__(YI_ALIGN_SHIFT , dest))!=0)
	{
		int i;

		register unsigned long cnt = (n=16-n)>>__long_shift__;
		register unsigned long cnt_mod = __shift_mod__(__long_shift__, n);

		for (i=0; i<cnt; i++) ((unsigned long*)dest)[i] = 0;
		dest = &((unsigned long*)dest)[i];

		for (i=0; i<cnt_mod; i++) ((char*)dest)[i] = 0;
		dest = &((char*)dest)[i];

		size-=(cnt_mod+(cnt<<__long_shift__));
	}

#ifdef __use_sse2__
	if (size<YI_ALIGNMENT)
	{
		return memset(dest, 0,size) ? org: 0;
	}

	dest = yimemzero_sse2_asm(dest, size);

ZERO_BY_BYTE:	
{
	register int i;
	register int remain = (0x0000000F&size);
	for (i=0; i<remain; i++)
	{
		((char*)dest)[i]=0;
	}
}
	return org;
#endif

#ifdef __x64__
	yimemzero_normal(dest, size);
	return org;
#endif

NORMAL:
	yimemzero_normal(dest, size);
	return org;
}

