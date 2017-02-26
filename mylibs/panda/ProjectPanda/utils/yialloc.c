#include "yistd.h"
#include "yitrace.h"
#include <stdlib.h>
#include <string.h>


// ------------------------------
void* FASTCALL yitoheap(void* buffer, int n)
{
	void* p = yialloc(n);
	memcpy(p, buffer, n);
	return p;
}

// ------------------------------
void* FASTCALL yitoheapi(void* buffer, int n)
{
	void* p = yialloci(n);
	memcpy(p, buffer, n);
	return p;
}


// ----------------------------------
int FASTCALL  yiallocSize(void* p)
{
	unsigned int* pIndex=(unsigned int*)p;
	if (p==0)
		return ;

	return (int)pIndex[-1];
}

// ----------------------------------
void* FASTCALL  yiallocex(int n)
{
	unsigned int* p = (unsigned int*)malloc(n+4);
	p[0]=n;
	return (void*)&p[1];	
}

// ----------------------------------
void* FASTCALL  yiallocexi(int n)
{
	void* ret = yiallocex(n);
	memset(ret, 0, n);
	return ret;
}

// ----------------------------------
void FASTCALL  yifreeex(void* p)
{
	unsigned int* pIndex=(unsigned int*)p;
	if (p==0)
		return ;

	free((void*)&p[-1]);
}

// ----------------------------------
/*void* yialloc(int n)
{
	return malloc(n);
}*/

// ----------------------------------
void* FASTCALL  yialloci(int n)
{
	return memset(yialloc(n), 0, n);
/*	int size = n+sizeof(int32)+YI_DEF_STRLEN+YI_ALIGNMENT;
	register char* p = (char*)malloc(size);
	register long remain;
	register char* pAligned;

	if (p==0)
	{
		return 0;
	}

	memset(p, 0, size);
#ifdef __x64__
	remain = ((p+sizeof(int32)) & 0x000000000000000F);
#else
	remain = (long)((long)(p+sizeof(int32)) & 0x0000000F);
#endif
	
	remain = (remain==0)?YI_ALIGNMENT+sizeof(int32):remain;

	pAligned = &p[YI_ALIGNMENT-remain];
	((int*)pAligned)[-1] = (int)p;


	return (void*)pAligned;*/
}

// ----------------------------------
void* FASTCALL  yirealloc(void* p, int n)
{
	return realloc(p, n);
}

// ----------------------
void yifree(void* p)
{
	free(p);
	/*int* pr = (int*)p;
	free((void*)pr[-1]);*/
}

// ------------------------------------
char* yistrcpy(char* dest, const char* src, const char* until)
{
	char* pEnd = strstr(src, until);
	if (pEnd==0)
		return 0;
	else
	{
		int n = pEnd-src;
		memcpy(dest, src, n);
		dest[n]=0;
	}

	return dest;
}

// -------------------------------
void yiBinaryDelete(Binary p)
{
	char* pIndex = (char*)p;

	if (p==0)
		return ;

	pIndex=&pIndex[-4];
	yifree(pIndex);
}

// -------------------------------
Binary yiBinary(void* p, int size, ...)
{
	int i=0;
	int all=0;
	va_list ap;
	int** pIndex = (int**)&p;
	char* dest=0;
	void* ret=0;

	va_start(ap, size);

	while(pIndex[i]!=0)
	{
		all+=(int)pIndex[i+1];
		i++;
		i++;
	}

	ret = yialloc(all+sizeof(unsigned int)+sizeof(unsigned int));

	dest=(char*)ret;
	dest+=4;

	pIndex=(int**)&p;
	i=0;
	while(pIndex[i]!=0)
	{
		memcpy(dest, pIndex[i], (int)pIndex[i+1]);
		dest+=(int)pIndex[i+1];
		i++;
		i++;
	}

	*((int*)ret)=all;
	((char*)ret)[all+sizeof(unsigned int)]=0;
	return (Binary)&((int*)ret)[1];
}

// ----------------------------
ubit32_t yirand32(Bool bIsInitial)
{
#ifdef _WIN32
	if (bIsInitial)
	{
		srand(time(0));
	}
	return (rand()|(rand()<<16));
#else
	if (bIsInitial)
	{
		srand(time(0));
	}
	return rand();
	
#endif
}

// ------------------------------
Bool yiatoi(const char* buffer, int* n)
{
	int cnt=0;
	char temp[16];
	char* _buff=(char*)buffer;
	int len=0;

	while(*_buff!=0 && cnt-15!=0)
	{
		if (*_buff==' ')
		{
			if (cnt==0)
			{
				continue;
			}
		}

		if (*_buff>'9' ||
				*_buff<'0')
		{
			if (len!=0)
				return False;

			if (*_buff!='-')
			{
				return False;
			}
		}
		temp[cnt++]=*_buff++;
	}

	temp[cnt]=0;
	*n= atoi(temp);

	return True;
}

// ------------------------------
Bool yiatof(void* buffer, float* n)
{
	int cnt=0;
	char temp[16];
	char* _buff=(char*)buffer;
	int len=0;

	while(*_buff!=0 && cnt-15!=0)
	{
		if (*_buff==' ')
		{
			if (cnt==0)
			{
				continue;
			}
		}

		if (*_buff>'9' ||
				*_buff<'0')
		{
			if (cnt!=0)
			{
				if (*_buff!='.')
				{
					return False;
				}
			}
			else if (*_buff!='-')
			{
				return False;
			}
		}
		temp[cnt++]=*_buff++;
	}

	temp[cnt]=0;
	*n= atof(temp);

	return True;
}
