#ifdef WIN32
#include <windows.h>
#endif

#define __sizeofbit__(a) (sizeof(a)<<3)

#define __round_shift__(a, n) ((a<<n) | (a>> (__sizeofbit__(a)-n)))

#define __round_rshift__(a, n) ((a>>n) | (a<< (__sizeofbit__(a)-n)))

#ifndef __shift_mod__
#define __shift_mod__(a, n) ((~(0xFFFFFFFF<<n))&a)
#endif

#ifdef __POSIX__
#define BOOL int
#define TRUE 1
#define FALSE 0
#endif

//#include "yistd.h"
#include "yiencodex.h"

#include <stdlib.h>

#define yialloc malloc



// --------------------------------
static void* yitoheap_s(void* buffer, int size, unsigned long security)
{
	void* p = (void*)yitoheap(buffer, size);
	yiencodex(p, size, (unsigned long)security);
	return p;
}

// --------------------------------
unsigned int yichecksum(void* buffer, int size, int check_sum_interval)
{
	unsigned char* p = (unsigned char*)buffer;
	unsigned int check_sum=0;

	if (p==0)
		return 0;

	while(p<(((unsigned char*)buffer)+size))
	{
		check_sum+= (unsigned int)*p;
		p+=check_sum_interval;
	}

	return check_sum;
}

// --------------------------------
void* yiencodex(void* buffer, int size, unsigned long security)
{
	return buffer;
}

// --------------------------------
void* yiencode(void* buffer, int size, unsigned long security)
{
	register int i;
	register int cnt=0;
	register int n;
	int nShiftStart=__shift_mod__(security, 5);
	BOOL bShiftReverse=__shift_mod__(security, 1);

	unsigned long* ulptr = (unsigned long*)buffer;
	unsigned short* usptr = (unsigned short*)buffer;
	unsigned char* uptr=(unsigned char*)buffer;


	// xxxx
	// エラーチェック
	{
		// チェック
		if (security==0)
			return buffer;
		if (size==0)
			return 0;
		if (buffer==0)
			return 0;
	}

	if (size<8)
		goto EN_8;
	
	n = (size>>3); // 8で割る。

	if (bShiftReverse==TRUE)
	{
		for (i=0; i<n; i++)
		{
			unsigned long temp=0;

			ulptr[cnt]+=__round_rshift__(security, __shift_mod__(nShiftStart++, 5));
			ulptr[cnt+1]+=__round_rshift__(security, __shift_mod__(nShiftStart++, 5));
			temp = ((ulptr[cnt] & security) | (ulptr[cnt+1] & (~security)));
			ulptr[cnt]=((ulptr[cnt+1] & security) | (ulptr[cnt] & (~security)));
			ulptr[++cnt]=temp;
			cnt++;
			size-=8;
		}
	}
	else
	{
		for (i=0; i<n; i++)
		{
			unsigned long temp=0;

			ulptr[cnt]+=__round_shift__(security, __shift_mod__(nShiftStart++, 5));
			ulptr[cnt+1]+=__round_shift__(security, __shift_mod__(nShiftStart++, 5));
			temp = ((ulptr[cnt] & security) | (ulptr[cnt+1] & (~security)));
			ulptr[cnt]=((ulptr[cnt+1] & security) | (ulptr[cnt] & (~security)));
			ulptr[++cnt]=temp;
			cnt++;
			size-=8;
		}
	}
	usptr = (unsigned short*)&ulptr[cnt];
	uptr = (unsigned char*)&ulptr[cnt];
EN_8:
	if (size<4)
		goto EN_4;

	if (bShiftReverse==TRUE)
	{
		unsigned short temp=0;
		unsigned short ssecurity = *((unsigned short*)&(((unsigned char*)&security)[security%3]));
		nShiftStart=__shift_mod__((unsigned long)ssecurity, 4);

		usptr[0]+=__round_rshift__(ssecurity, nShiftStart++);
		usptr[1]+=__round_rshift__(ssecurity, __shift_mod__(nShiftStart, 4));
		temp = ((usptr[0] & ssecurity) | (usptr[1] & (~ssecurity)));
		usptr[0]=((usptr[1] & ssecurity) | (usptr[0] & (~ssecurity)));
		usptr[1]=temp;
		size-=sizeof(unsigned long);
	}
	else
	{
		unsigned short temp=0;
		unsigned short ssecurity = *((unsigned short*)&(((unsigned char*)&security)[security%3]));
		nShiftStart=__shift_mod__((unsigned long)ssecurity, 4);

		usptr[0]+=__round_shift__(ssecurity, nShiftStart++);
		usptr[1]+=__round_shift__(ssecurity, __shift_mod__(nShiftStart, 4));
		temp = ((usptr[0] & ssecurity) | (usptr[1] & (~ssecurity)));
		usptr[0]=((usptr[1] & ssecurity) | (usptr[0] & (~ssecurity)));
		usptr[1]=temp;
		size-=sizeof(unsigned long);
	}
	uptr = (unsigned char*)&usptr[2];
EN_4:
	if (size<2)
		goto EN_2;

	if (bShiftReverse==TRUE)
	{
		unsigned char temp=0;
		unsigned char csecurity = (((unsigned char*)&security)[security%4]);
		nShiftStart=__shift_mod__((unsigned long)csecurity, 3);

		uptr[0]+=__round_rshift__(csecurity, nShiftStart++);
		uptr[1]+=__round_rshift__(csecurity, __shift_mod__(nShiftStart, 3));
		temp = ((uptr[0] & csecurity) | (uptr[1] & (~csecurity)));
		uptr[0]=((uptr[1] & csecurity) | (uptr[0] & (~csecurity)));
		uptr[1]=temp;
		size-=sizeof(unsigned short);
	}
	else
	{
		unsigned char temp=0;
		unsigned char csecurity = (((unsigned char*)&security)[security%4]);
		nShiftStart=__shift_mod__((unsigned long)csecurity, 3);

		uptr[0]+=__round_shift__(csecurity, nShiftStart++);
		uptr[1]+=__round_shift__(csecurity, __shift_mod__(nShiftStart, 3));
		temp = ((uptr[0] & csecurity) | (uptr[1] & (~csecurity)));
		uptr[0]=((uptr[1] & csecurity) | (uptr[0] & (~csecurity)));
		uptr[1]=temp;
		size-=sizeof(unsigned short);
	}

	uptr=&uptr[2];
EN_2:
	if (size==0)
	{
		return buffer;
	}

	if (bShiftReverse==TRUE)
	{
		unsigned char temp=0;
		unsigned char csecurity = (((unsigned char*)&security)[security%4]);
		nShiftStart=__shift_mod__((unsigned long)csecurity, 3);

		uptr[0]+=__round_rshift__(csecurity, nShiftStart);
		//uptr[0]+=((unsigned char*)buffer)[0];
	}
	else
	{
		unsigned char temp=0;
		unsigned char csecurity = (((unsigned char*)&security)[security%4]);
		nShiftStart=__shift_mod__((unsigned long)csecurity, 3);

		uptr[0]+=__round_shift__(csecurity, nShiftStart);
		//uptr[0]+=((unsigned char*)buffer)[0];
	}
	
	return buffer;

}

// --------------------------------
void* yidecodex(void* buffer, int size, unsigned long security)
{
	return buffer;
}

// --------------------------------
void* yidecode(void* buffer, int size, unsigned long security)
{
	register int i;
	register int cnt=0;
	register int n;
	int nShiftStart=__shift_mod__(security, 5);
	BOOL bShiftReverse=__shift_mod__(security, 1);

	unsigned long* ulptr = (unsigned long*)buffer;
	unsigned short* usptr = (unsigned short*)buffer;
	unsigned char* uptr=(unsigned char*)buffer;

// 	xxxx
	// エラーチェック
	{
		// チェック
		if (security==0)
			return buffer;
		if (size==0)
			return 0;
		if (buffer==0)
			return 0;
	}

	if (size<8)
		goto EN_8;
	
	n = (size>>3); // 8で割る。

	if (bShiftReverse==TRUE)
	{
		for (i=0; i<n; i++)
		{
			unsigned long temp=0;

			temp = ((ulptr[cnt] & security) | (ulptr[cnt+1] & (~security)));
			ulptr[cnt]=((ulptr[cnt+1] & security) | (ulptr[cnt] & (~security)));
			ulptr[cnt+1]=temp;

			ulptr[cnt]-=__round_rshift__(security, __shift_mod__(nShiftStart++, 5));
			ulptr[cnt+1]-=__round_rshift__(security, __shift_mod__(nShiftStart++, 5));
			size-=8;
			cnt++;
			cnt++;
		}
	}
	else
	{
		for (i=0; i<n; i++)
		{
			unsigned long temp=0;

			temp = ((ulptr[cnt] & security) | (ulptr[cnt+1] & (~security)));
			ulptr[cnt]=((ulptr[cnt+1] & security) | (ulptr[cnt] & (~security)));
			ulptr[cnt+1]=temp;

			ulptr[cnt]-=__round_shift__(security, __shift_mod__(nShiftStart++, 5));
			ulptr[cnt+1]-=__round_shift__(security, __shift_mod__(nShiftStart++, 5));
			size-=8;
			cnt++;
			cnt++;

		}
	}
	usptr = (unsigned short*)&ulptr[cnt];
	uptr = (unsigned char*)&ulptr[cnt];
EN_8:
	if (size<4)
		goto EN_4;

	if (bShiftReverse==TRUE)
	{
		unsigned short temp=0;
		unsigned short ssecurity = *((unsigned short*)&(((unsigned char*)&security)[security%3]));
		nShiftStart=__shift_mod__((unsigned long)ssecurity, 4);

		temp = ((usptr[0] & ssecurity) | (usptr[1] & (~ssecurity)));
		usptr[0]=((usptr[1] & ssecurity) | (usptr[0] & (~ssecurity)));
		usptr[1]=temp;
		usptr[0]-=__round_rshift__(ssecurity, nShiftStart++);
		usptr[1]-=__round_rshift__(ssecurity, __shift_mod__(nShiftStart, 4));
		size-=sizeof(unsigned long);
	}
	else
	{
		unsigned short temp=0;
		unsigned short ssecurity = *((unsigned short*)&(((unsigned char*)&security)[security%3]));
		nShiftStart=__shift_mod__((unsigned long)ssecurity, 4);

		temp = ((usptr[0] & ssecurity) | (usptr[1] & (~ssecurity)));
		usptr[0]=((usptr[1] & ssecurity) | (usptr[0] & (~ssecurity)));
		usptr[1]=temp;
		usptr[0]-=__round_shift__(ssecurity, nShiftStart++);
		usptr[1]-=__round_shift__(ssecurity, __shift_mod__(nShiftStart, 4));
		size-=sizeof(unsigned long);
	}
	uptr = (unsigned char*)&usptr[2];
EN_4:
	if (size<2)
		goto EN_2;

	if (bShiftReverse==TRUE)
	{
		unsigned char temp=0;
		unsigned char csecurity = (((unsigned char*)&security)[security%4]);
		nShiftStart=__shift_mod__((unsigned long)csecurity, 3);

		temp = ((uptr[0] & csecurity) | (uptr[1] & (~csecurity)));
		uptr[0]=((uptr[1] & csecurity) | (uptr[0] & (~csecurity)));
		uptr[1]=temp;

		uptr[0]-=__round_rshift__(csecurity, nShiftStart++);
		uptr[1]-=__round_rshift__(csecurity, __shift_mod__(nShiftStart, 3));

		size-=sizeof(unsigned short);
	}
	else
	{
		unsigned char temp=0;
		unsigned char csecurity = (((unsigned char*)&security)[security%4]);
		nShiftStart=__shift_mod__((unsigned long)csecurity, 3);

		temp = ((uptr[0] & csecurity) | (uptr[1] & (~csecurity)));
		uptr[0]=((uptr[1] & csecurity) | (uptr[0] & (~csecurity)));
		uptr[1]=temp;

		uptr[0]-=__round_shift__(csecurity, nShiftStart++);
		uptr[1]-=__round_shift__(csecurity, __shift_mod__(nShiftStart, 3));

		size-=sizeof(unsigned short);
	}

	uptr=&uptr[2];
EN_2:
	if (size==0)
	{
		return buffer;
	}

	if (bShiftReverse==TRUE)
	{
		unsigned char temp=0;
		unsigned char csecurity = (((unsigned char*)&security)[security%4]);
		nShiftStart=__shift_mod__((unsigned long)csecurity, 3);

		//uptr[0]-=((unsigned char*)buffer)[0];
		uptr[0]-=__round_rshift__(csecurity, nShiftStart);

	}
	else 
	{
		unsigned char temp=0;
		unsigned char csecurity = (((unsigned char*)&security)[security%4]);
		nShiftStart=__shift_mod__((unsigned long)csecurity, 3);

		//uptr[0]-=((unsigned char*)buffer)[0];
		uptr[0]-=__round_shift__(csecurity, nShiftStart);
	}
	
	return buffer;
}

char base64table[64] = {0, };// = {

// --------------------------------
void yiInitBase64()
{
	int i=0;
	int cnt=0;

	for (i=0; i<('Z'-'A')+1; i++)
		base64table[i]=i+'A';

	cnt=i;
	for (i=0; i<('z'-'a')+1; i++)
	{
		base64table[cnt++]=i+'a';
	}

	for (i=0; i<('9'-'0')+1; i++)
	{
		base64table[cnt++]=i+'0';
	}

	base64table[62]='+';
	base64table[63]='/';

/*	printf("yiInitBase64!!\n");

	for (i=0; i<64; i++)
	{
		printf("%02x : ", i);
		putchar(base64table[i]);
		putchar('\n');
	}
*/
}

unsigned char xGet6BitCode(unsigned char ch)
{
	int i=0;
	//printf("xGet6BitCode: %c, %x\n", ch, (int)ch);
	for (i=0; i<64; i++)
	{
		if (ch==base64table[i])
		{
			//printf("xGet6BitCode: same %c=%c, ret=%d\n", ch, base64table[i], i);
			return (unsigned char)i;
		}	
	}

	return 0xFF;
}

const unsigned char firstMaskBits[8]  = { 0xFC, 0x7E, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01};
const unsigned char secondMaskBits[8] = { 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8};
const char shiftBits[8] = { 2, 1, 0, -1, -2, -3, -4, -5 };

// --------------------------------
static char xGetBitz(unsigned char* p, int sumBits, int size)
{
	int s8 = sumBits/8;
	int s = sumBits%8;
	unsigned char ret=0;
	unsigned char v1, v2; //=p[s8], v2=p[s8+1];

	if (sumBits >= (size<<3))
	{
		return 0xFF;
	}

	v1=p[s8];

	if (shiftBits[s]>=0)
	{
		ret = ((firstMaskBits[s]&v1)>>shiftBits[s]);
		return ret;
	}

	v2=p[s8+1];
	ret = ((firstMaskBits[s]&v1)<<(-shiftBits[s]));

	if (sumBits+6 > (size<<3))
	{
		return ret;	
	}
	ret|=((secondMaskBits[s]&v2)>>(10-s));

	return ret;

}

// --------------------------------
char* yiToBase64(const void* buffer, int size, int* changedSize)
{
	int i;
	char* ret = 0;
	unsigned char* pIndex=(unsigned char*)buffer;
	int newSize = (size<<3)/6;
	int remain = (size<<3)%6;
	int cnt=0;
	int sumBits=0;

	newSize = (remain==0)?newSize:newSize+1;

	if ((newSize&0x03)!=0)
	{
		newSize+=(4-(newSize&0x03));
	}

	ret = yialloc(newSize+1);
	ret[newSize]=0;

	//printf("new size -----> %d->%d\n", size, newSize);


	if (base64table[0]==0)
	{
		yiInitBase64();
	}

	printf("start encode......\n");
	memset(ret, '=', newSize);
	for (i=0; i<newSize; i++)
	{
		unsigned char n = xGetBitz(pIndex, sumBits, size);//sBit, eBit);

		if (n==0xFF)
			break;

		ret[cnt]=base64table[n];
		//putchar(ret[cnt]);
		//printf("%x:%c ", (int)ret[cnt], ret[cnt]);
		cnt++;
		sumBits+=6;
	}
	
	if (changedSize)
	{
		*changedSize=newSize;
	}

	return ret;
}

unsigned char restoreMask[9]={0, 0x20, 0x30, 0x38, 0x3C, 0x3E, 0x3F, 0x3F, 0x3F};
int restoreShiftBits[9] = {0, 5, 4, 3, 2, 1, 0, -1, -2};

// --------------------------------
static int xRestoreBits(unsigned char* pNewBuffer, unsigned char code, int i)
{
	int startBit 	 = i*6;
	int nStartOffset = (startBit>>3);
	int nRemainBits  = 8-(startBit&0x07);
	unsigned char x = restoreMask[nRemainBits]&code;

	//printf("restore bits.... startBit:%d, nStartOffset:%d, nRemainBits:%d, mask:%x, code:%x, x is %x\n", startBit, nStartOffset, nRemainBits, (int)restoreMask[nRemainBits], (int)code, (unsigned int)x);

	//printf("shiftbits:%d\n", restoreShiftBits[nRemainBits]);
	if (restoreShiftBits[nRemainBits]>0)
	{
		x=(x>>restoreShiftBits[nRemainBits]);
	}
	else
	{
		x=(x<<(-restoreShiftBits[nRemainBits]));
	}

	//printf("restore bits.... x:%x\n", (unsigned int)x);

	pNewBuffer[nStartOffset]|=x;
	//printf("restore bits.... new buffer x:%x\n", (unsigned int)pNewBuffer[nStartOffset]);
	
	if (restoreMask[nRemainBits]!=0x3F)
	{
		nStartOffset++;
		x = (~restoreMask[nRemainBits])&code;
		nRemainBits = 6-nRemainBits;
		nRemainBits = 8-nRemainBits;
		pNewBuffer[nStartOffset]|=(x<<nRemainBits);
	}	

	//printf("restore bits.... new buffer x:%x\n", (unsigned int)pNewBuffer[nStartOffset]);
	return nStartOffset;
}

#if 0
// --------------------------------
void* yiToPlain(const char* buffer, int* changedSize)
{
	int i;
	int cnt=0;
	int size=strlen(buffer);
	char* pIndex=(char*)buffer;
	char* pNewBuffer=0;

	if (base64table[0]==0)
	{
		yiInitBase64();
	}

	// 1. remove '='
	/*while(1)
	{
		putchar(pIndex[size-cnt-1]);
		if (pIndex[size-cnt-1]=='=')
		{
			pIndex[size-cnt-1]=0;
			cnt++;
			continue;
		}
		break;
	}*/

	cnt = (size-cnt);	

	pNewBuffer = yialloci(cnt+1);

	if (changedSize)
		*changedSize=0;

	for (i=0; i<cnt; i++)
	{
		if (pIndex[i]=='=')
		{
			break;
		}
		if (pIndex[i]==0)
		{
			break;
		}

		unsigned char code = xGet6BitCode(pIndex[i]);	

		if (code==0xFF)
		{
			yifree(pNewBuffer);
			//YIERROR("invalid code!, pIndex[%d]:%x\n", i, (int)pIndex[i]);
			return 0;
		}

		size = xRestoreBits(pNewBuffer, code, i);
		//printf(pNewBuffer);
		//putchar('\n');
	}

	if (changedSize)
		*changedSize=size+1;//(i*6)>>3;

	return pNewBuffer;
}
#endif

// --------------------------------
static Binary _yiToPlain(const char* buffer, Bool bIsForced)
{
	int i, k;
	int cnt=0;
	int size=strlen(buffer);
	char* pIndex=(char*)buffer;
	char* pNewBuffer=0;
	int dummy=(buffer[size-1]!='=')?1:0;

	if (base64table[0]==0)
	{
		yiInitBase64();
	}

	// 1. remove '='
	/*while(1)
	{
		putchar(pIndex[size-cnt-1]);
		if (pIndex[size-cnt-1]=='=')
		{
			pIndex[size-cnt-1]=0;
			cnt++;
			continue;
		}
		break;
	}*/

	cnt = (size-cnt);	

	pNewBuffer = (char*)yialloci(cnt+sizeof(int)+sizeof(int));

	//if (changedSize)
	//	*changedSize=0;

	for (i=0,k=0; i<cnt; i++)
	{
		unsigned char code = 0;
		if (pIndex[i]=='=')
		{
			break;
		}
		if (pIndex[i]==0)
		{
			break;
		}

		code = xGet6BitCode(pIndex[i]);	

		if (code==0xFF)
		{
			if (bIsForced==True)
			{
				// ｿｿｿｿｿ
				continue;
			}

			yifree(pNewBuffer);
			//YIERROR("_yiToPlain() invalid code!, pIndex[%d]:%x\n", i, (int)pIndex[i]);
			return 0;
		}

		size = xRestoreBits(&pNewBuffer[4], code, k++);
		//printf(pNewBuffer);
		//putchar('\n');
	}

	*((int*)pNewBuffer)=size+dummy;//+1;
	pNewBuffer[4+size+1]=0;

	return &pNewBuffer[4];
}

// --------------------------------
Binary yiToPlain(const char* buffer)
{
	return _yiToPlain(buffer, False);
}

// --------------------------------
Binary yiToPlainForced(const char* buffer)
{
	return _yiToPlain(buffer, True);
}


