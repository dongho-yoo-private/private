#include "yi.h"
#include "yistd.h"


inline long yimemcmp_normal(const void* p1, const void* p2, int size)
{
	register unsigned long k=0;
	register unsigned long mask; 
	register unsigned long cnt_8 = size>>3;
	int remain_8 = __shift_mod__(3, size);
	int cnt = remain_8 >> __long_shift__;
	int remain = __shift_mod__(__long_shift__, remain_8 ); /* remain 1byte block count*/

	unsigned long* ip1 = (unsigned long*)p1;
	unsigned long* ip2 = (unsigned long*)p2;

__CMP_4_BYTES__:
	if (!cnt) goto __CMP_8_BYTES__;

	k = *ip1-*ip2;

	if (k) return k;

	ip1++;ip2++;

	if (cnt_8) goto __CMP_8_BYTES__;

	goto __CMP_BYTE__;

__CMP_8_BYTES__:
	//if (!cnt_8) goto __CMP_BYTE__;

	while(!k&&cnt_8--)
	{	
		k|=ip1[0]-ip2[0];
		k|=ip1[1]-ip2[1];

		ip1 = &ip1[2];
		ip2 = &ip2[2];
	}

	if (k) {
		ip1 = &ip1[-2];
		ip2 = &ip2[-2];
		k=*ip1-*ip2;
		return k ? k:ip1[1]-ip2[1];
	}

	goto __CMP_BYTE__;


__CMP_BYTE__:

	if (!remain) return 0;

	mask=~(0);
	mask<<=bytetobit(remain);
	mask=~(mask);

	k = (*ip1-*ip2)&mask;

	if (k) return k;

	return 0;
}


/**/
long FASTCALL  yimemcmp(const void* mem1, const void* mem2, int n)
{
#ifdef __use_sse2__
#endif

#ifdef __x64__
#endif

#ifdef __use_mmx__
#endif
	int remain = __shift_mod__(sizeof(long), mem1);

	if (__shift_mod__(sizeof(long), mem2)!=remain)
	{
		int i;
		char* p1=(char*)mem1;
		char* p2=(char*)mem2;
		register long dif;

		for (i=0; i<n; i++)
		{
			dif= (long)*p1-(long)*p2;
			if (dif!=0)
			{
				return dif;
			}
			p1++;
			p2++;
		}

		return dif;
	}

	{	
		int i;
		char* p1=(char*)mem1;
		char* p2=(char*)mem2;
		register long dif;

		remain=remain?sizeof(long)-remain:remain;
		for (i=0; i<remain;i++)
		{
			dif= (long)*p1-(long)*p2;
			if (dif!=0)
			{
				return dif;
			}
			p1++;
			p2++;
		}
		mem1 = (void*)p1;
		mem2 = (void*)p2;
	}
	return yimemcmp_normal(mem1, mem2, n);
}

