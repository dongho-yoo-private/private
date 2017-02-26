#include "yi.h"
#include "yistd.h"

#define __inline__  __attribute__((always_inline))

/**/
extern inline void* yimemcpy_sse2_asm(void* dest, const void* src, int size);


/**/
#define __ONCE_COPY_SMALL_COUNT_SHIFT__	4

/**/
#define __ONCE_COPY_LARGE_COUNT_SHIFT__	6

#define __long_cpy__(x, y) *x++=*y++

#ifdef __x64__
#	define __16_bytes_copy__(x, y) \
	__long_cpy__(x, y);\
	__long_cpy__(x, y);
#else
#	define __16_bytes_copy__(x, y) \
	__long_cpy__(x, y);\
	__long_cpy__(x, y);\
	__long_cpy__(x, y);\
	__long_cpy__(x, y)
#endif

#ifdef __x64__
#define __64_bytes_copy__(x, y) \
	__16_bytes_copy__(x,y) ;\
	__16_bytes_copy__(x,y) ;\
	__16_bytes_copy__(x,y) ;\
	__16_bytes_copy__(x,y) 
#else
#	define __64_bytes_copy__(x, y) \
	__16_bytes_copy__(x,y) ;\
	__16_bytes_copy__(x,y) ;\
	__16_bytes_copy__(x,y) ;\
	__16_bytes_copy__(x,y) 
#endif

/**/
inline void* yimemcpy_normal(void* dest, const void* src, int size)
{
	void* ret=dest;
	register long i;

	long cnt_high = size>>__ONCE_COPY_LARGE_COUNT_SHIFT__; /* 64 bytes block counts */
	long cnt_high_remain = __shift_mod__(__ONCE_COPY_LARGE_COUNT_SHIFT__, size); /* remain 1byte block count*/

	long cnt_middle = cnt_high_remain >>__ONCE_COPY_SMALL_COUNT_SHIFT__;/* 16 bytes block counts */
	register long cnt_middle_remain = __shift_mod__(__ONCE_COPY_SMALL_COUNT_SHIFT__, cnt_high_remain ); /* remain 1byte block count*/

	long long_cnt= cnt_middle_remain >>__long_shift__; /* 4(8)byte block count */
	register long long_cnt_remain = __shift_mod__(__long_shift__, cnt_middle_remain); /* 1byte block count */

	register long mask;

	long* p1 = (long*)dest;
	long* p2 = (long*)src;

	if (size<sizeof(long))
		goto BYTE_COPY;

	/* 64 byte copy*/
	for (i=0; i<cnt_high ; i++) 
	{
		__64_bytes_copy__(p1, p2) ;
	}

	/* 16 byte copy. */
	for (i=0; i<cnt_middle ; i++) 
	{
		__16_bytes_copy__(p1, p2) ;
	}

	/* 4(8) byte copy */
	for (i=0; i<long_cnt; i++) __long_cpy__(p1, p2);

	if (!long_cnt_remain) ret;

BYTE_COPY:
	mask =__LONG_FULL_BIT__<<bytetobit((sizeof(long)-long_cnt_remain));
	/*mask=~(mask);*/
	/**p1 = (*p2&mask);*/
	*p1 = ((*p1&~mask)|(*p2&mask));

	return ret;
}

/**/
void* FASTCALL yimemcpy(void* dest, const void* src, int size)
{
	void* ret=dest;

	if (__is_yi_aligned__(dest, src)==False)
	{
		int n =__shift_mod__(YI_ALIGN_SHIFT, dest);
		if (n==__shift_mod__(YI_ALIGN_SHIFT, src))
		{
			register long i=0;
			for (i=0; i<n; i++)
				((char*)dest)[i] = ((char*)src)[i];

			dest = &((char*)dest)[n];
			size-=n;
		}
		else
		{
			goto NORMAL_COPY;
		}
	}

#ifdef __use_sse2__
	//yimemcpy_sse2_asm(dest, src, size);
	//return ret;
#endif

NORMAL_COPY:
	yimemcpy_normal(dest, src, size);
	return ret;
}

