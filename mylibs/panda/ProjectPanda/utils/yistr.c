#include "yi.h"
#include "yistd.h"
#include <string.h>

//#define xxTRACE printf
#define xxTRACE(...) 

// ------------------------------
static Bool xIsContain(const char* p, int n, char ch)
{
	int i;
	n = n==0?strlen(p):n;
	
	for (i=0; i<n; i++)
	{
		if (ch==p[i])
			return True;
	}
	return False;
}

// ------------------------------
int yistrlenUntil(const char* src, const char* untilz)
{
	char* pIndex=src;
	int n = strlen(untilz);

	while(*pIndex!=0 &&
			xIsContain(untilz, n, *pIndex)==False)
	{
		pIndex++;
	}

	return pIndex-src;
}

// ------------------------------
char* yistrcpyUntil(char* dest, const char* src, const char* untilz)
{
	int n = yistrlenUntil(src, untilz);

	if (n==0)
		return 0;

	if (dest==0)
	{
		dest = yialloc(n+1);
	}
	memcpy((void*)dest, src, n);
	dest[n]=0;

	return dest;
}


// ----------------------------------------
string_t yitostrx(const char* pszStr)
{
	int n = strlen(pszStr);
	char* p = (char*)yialloc(n+2);
	unsigned short* size = (unsigned short*)p;
	*size = (unsigned short)n;
	memcpy(&size[1], pszStr, n);
	return p;//(string_t)&size[1];
}

// ----------------------------------------
ubit32_t yitohash(const char* pszStr, int len)
{
	if (len==0)
		len = strlen(pszStr);

	if (len<5)
	{
		ubit32_t ret = 0;
		memcpy(&ret, pszStr, len);
		return ret;
	}

	if (len<8)
	{
		ubit32_t ret = 0;
		memcpy(&ret, pszStr, len);
		return ret;
	}	
}


// ----------------------------------------
char16_t* yiAsciiToUnicode(const char* pszString)
{
	int i=0;
	int n = strlen(pszString);
	char16_t* dest = (char16_t*)yialloc(((n+1)<<1));

	for (i=0; i<n; i++)
	{
		dest[i]=(wchar_t)pszString[i];
	}

	dest[n]=0;

	return dest;
	
}

// ----------------------------------------
void FASTCALL yistr(const char* str)
{
	return yistrn(str, strlen(str));
}

// ----------------------------------------
void FASTCALL yistrn(const char* str, int n)
{
	register int i;
	char* p = (char*)str;
	int remain = __shift_mod__(__long_shift__, n);

	remain = remain ? sizeof(long)-remain : remain;	

	p=&p[n];
	for (i=0; i<remain; i++)
	{
		*p=0;
		p++;
	}

	*((long*)p)=0;	

	return ;	
}

// ------------------------------
char* FASTCALL yistricpy(char* dest, const char* from, const char* end)
{
	long len = (long)end-(long)from;

	//yimemcpy(dest, from, len);
	memcpy(dest, from, len);
	yistrn(dest, len);

	return dest;
}

// ------------------------------
char* FASTCALL yistrcpych(char* dest, const char* src, char ch)
{
	char* ret=dest;
	while(*src && *src!=ch) *dest++=*src++;	
	*dest=0;
	return ret;
}

// ------------------------------
EXTERN_C int FASTCALL yiatoh(char* HexsaCode)
{
	char *stopString;
	int result = strtol( HexsaCode, &stopString, 16 );
	return result;
}

// ------------------------------
char* FASTCALL yistrheap(const char* str)
{
	return yistrnheap(str, 0);
}

// ------------------------------
char* FASTCALL yistrnheap(const char* str, int size)
{
	char* p=0;
	if (size==0)
	{
		size = strlen(str)+1;
	}

	p = (char*)yialloc(size);

	memcpy(p, str, size);

	return p;
}

// ------------------------------
char* FASTCALL yistrSub(const char* str, int start, int end)
{
	char* ret = (char*)yialloc(end-start+1);
	memcpy(ret, &str[start], (end-start));
	ret[end-start]=0;

	return ret;
}

// ------------------------------
char* FASTCALL yistrlist(const char* str, ...)
{
	va_list ap;
	char** pArgs = (char**)&str;
	int i=0;
	int cnt=0;
	int nTotalLength=0;
	char* ret=0;
	char* retIndex;

	while(pArgs[i])
	{
		nTotalLength+=strlen(pArgs[i++]);
		cnt++;
	}
	
	if (cnt==0 ||
		nTotalLength==0)
	{
		va_end(ap);
		return 0;
	}
	
	ret = (char*)yialloc(cnt+nTotalLength+1);	
	retIndex=ret;
	
	while(pArgs[i])
	{
		int len = strlen(pArgs[i]);
		strcpy(retIndex, pArgs[i]);
		retIndex=&retIndex[len+1];
		*retIndex=0;
	}

	va_end(ap);

	return ret;
}

// ------------------------------
char* FASTCALL yistrGetLine(const char* str, int n, char** next)
{
	char* pIndex = (char*)str;
	int len=0;

	while(*pIndex&&*pIndex!='\n') pIndex++;

	len=(int)(pIndex-str);

LENGTH_IS_ZERO:	
	if (len==0)
	{
		if (next!=0)
		{
			if (*pIndex==0)
			{
				*next=0;
			}
			else
			{
				*next=++pIndex;
			}
		}

		return 0;
	}

	if (pIndex[-1]=='\r')
	{
		pIndex--;
		len--;
		
		if (len==0)
		{
			goto LENGTH_IS_ZERO;
		}
	}

	if (next)
	{
		if (*pIndex=='\r')
		{
			*next=(pIndex+2);
		}
		else if (*pIndex==0)
		{
			*next=0;
		}
		else
		{
			*next=(pIndex+1);
		}
		
	}

	return yistrSub(str, 0, len);
}

// ------------------------------
// space, tab
char* FASTCALL yistrTrim(const char* str, Bool bSpaceOnly)
{
	int i=0, start=0, end=strlen(str);
	char* pIndex=(char*)str;
	char sztrimCharz[] = {' ', '\t', '\r', '\n', 0};

	if (bSpaceOnly==False)
		goto FULL_TRIM;

	for (i=0; i<end; i++)
	{
		if (pIndex[i]==' ')
		{
			start++;
		}
		break;	
	}

	for (i=end-1; i>=0; i--)
	{
		if (pIndex[i]==' ')
		{
			end--;
		}
	}

	return yistrSub(str, start, end);

FULL_TRIM:
	for (i=0; i<end; i++)
	{
		int k=0;
		for (k=0; k<4; k++)
		{
			if (pIndex[i]==sztrimCharz[k])
			{
				start++;
				break;
			}
		}
		break;	
	}

	for (i=end-1; i>=0; i--)
	{
		int k=0;
		for (k=0; k<4; k++)
		{
			if (pIndex[i]==sztrimCharz[k])
			{
				end--;
				break;
			}
		}
		break;
	}

	return yistrSub(str, start, end);
}


// ------------------------------
char* yistrTrimEx(char* dest, const char* src, const char* trimz)
{
	char* pStart=src;
	char* pEnd=src+strlen(src)-1;
	int n = strlen(trimz);

	while(*pStart!=0 &&
			xIsContain(trimz, n, *pStart)==True) pStart++;



	while((pEnd!=pStart) &&
			xIsContain(trimz, n, *pEnd)==True) pEnd--;

	n = pEnd-pStart+1;

	if (dest==src)
	{
		pEnd[1]=0;
		return pStart;
	}

	if (n<0)
	{
		return 0;
	}

	if (dest==0)
	{
		dest = yialloc(n+1);
	}

	strncpy(dest, pStart, n);
	dest[n]=0;

	return dest;
}


#include "yiListx.h"
// ------------------------------
strz yistrSplitEx(const char* src, const char* untilz)
{
	HandleList hList = yiListCreate(0, 0);
	char* p, *pIndex=(char*)src;
	int len = strlen(untilz);
	syiNode* node=0;
	int cnt=0;
	int total=0;
	char* pRet, *pRetIndex;

	while(p=yistrcpyUntil(0, pIndex, untilz))
	{
		int n = strlen(p);
		pIndex+=n;
		
		// この場合は文字列の最後で抜けるケース
		if (*pIndex==0)
		{
			yiListAdd(hList, (IntPtr)p);
			total+=(n+1);
			break;
		}

		yiListAdd(hList, (IntPtr)p);
		pIndex+=len;
		total+=(n+1);
	}

	if (yiListCount(hList)==0)
		return 0;

	pRetIndex = pRet = yialloc(total+sizeof(int));
	for (node=yiListGetTopFrom(hList, 0); node; node=node->np)
	{
		char* p = node->ref;
		char* p2 = yistrTrimEx(p, p, " ");
		int n = strlen(p2);
		memcpy(pRetIndex, p2, n);
		pRetIndex[n]=0;
		pRetIndex+=(n+1);

		yifree(p);
	}
	*pRetIndex=0;

	yiListDestroy(hList);
	return pRet;
}

// ------------------------------
char** FASTCALL yistrSplit(const char* str, const char* spliter, int* _cnt)
{
	char* pIndex=(char*)str;
	char* pLastIndex=0;
	char* pNext;
	int start=0;
	int end=0;
	int cnt=0;
	int nSplitCnt=0;
	int limitCnt=16;
	int spliterlength=strlen(spliter);

	char** ret = (char**)yialloci(sizeof(char*)*limitCnt);

	xxTRACE("yistrSplit before loop\n");
	while(pNext=strstr(pIndex,spliter))
	{
		int len=0;
		xxTRACE("yistrSplit (pNext:%s)\n", pNext);
		if (pNext==pIndex)
		{
			pIndex++;
			start=cnt++;
			xxTRACE("yistrSplit pIndex==pNext (pIndex:%s)\n", pIndex);
			continue;
		}
		
		len = pNext-pIndex;

		if (nSplitCnt==limitCnt)
		{
			int i=0;
			limitCnt+=16;
			ret = (char**)yirealloc(ret, sizeof(char*)*(limitCnt+1));
			
			for (i=0; i<16+1; i++)
			{
				ret[limitCnt-16+i-1]=0;
			}
		}

		xxTRACE("yistrSplit Set pIndex(pIndex:%s, pNext:%s), %d, %d\n", pIndex, pNext, start, start+len);
		ret[nSplitCnt++]=yistrSub(pIndex, start, start+len);
		pLastIndex=pIndex=(pNext+spliterlength);
		start=0;
	}
	xxTRACE("yistrSplit after loop\n");

	if (nSplitCnt==0)
	{
		nSplitCnt++;
		ret[0]=yistrheap(str);
	}

	if (pLastIndex)
	{
		xxTRACE("yistrSplit Set pIndexLast(pIndexLast:%s)\n", pLastIndex);
		ret[nSplitCnt++]=yistrnheap(pLastIndex, 0);
	}

	if (_cnt)
		*_cnt=nSplitCnt;

	return ret;
}

// ------------------------------
char* FASTCALL yistrGetBetween(const char* str, const char* start, const char* end, char** next)
{
	return 0;
/*
	int startlen 	= strlen(start);
	int endlen 	= strlen(end);
	char* pStartIndex;// = (char*)str; 
	char* pTop=(char*)str;
	char* pNextStartIndex;
	char* pEndIndex;
	int startCnt=0;

SEARCH_TOP:

	pStartIndex = strstr(pTop, end);
	if (pStartIndex==0)
		return 0;

	pEndIndex   = strstr(&pStartIndex[strlen], end);
	if (pEndIndex==0)
		return 0;

	pNextStartIndex=pStartIndex=pTop;
	while(pNextStartIndex=strstr(pNextStartIndex, start)
	{
		if (pNextStartIndex>pEndIndex)
		{
			break;
		}
		pStartIndex=pNextStartIndex;
		pNextStartIndex+=startlen;
		startCnt++;
	}

	if (startCnt==0)
	{
		return 0;
	}

	if (startCnt==1)
	{
		return yiStrSub(str, (pStartIndex-str)+startlen, (pEndIndex-str));
	}

	goto SEARCH_END;
*/
}

// ------------------------------
void yistrFree(char* str)
{
	if (str)
		yifree((void*)str);
}

// ------------------------------
void yistrSplitFree(char** str)
{
	int cnt=0;
	if (str==0)
		return ;

	while(str[cnt])
		yistrFree(str[cnt++]);
	
	yifree(str);
}

// ------------------------------
//void yistrFree(char* str);

// ------------------------------
strz yistrz(strz dest, ...)
{
	va_list ap;
	char** p = (char**)&dest;
	char* pIndex=dest;

	va_start(ap, dest);
	p++;

	if (dest==0)
	{
		char** x = p;
		int len = 0;
		while(*x)
		{
			len+=strlen(*x);
			len++;
			x++;
		}
		
		dest = pIndex = yialloci(len+1);
	}

	*pIndex=0;
	while(*p)
	{
		strcpy(pIndex, *p);
		pIndex+=strlen(*p);
		pIndex++;
		
		p++;
	}
	*pIndex=0;

	return dest;
}

// ------------------------------
const char* yistrzGet(const strz dest, int index, char splt)
{
	int i;
	char* p = (char*)dest;

	for (i=0; i<index; i++)
	{
		while(*p!=splt) p++;
		p++;

		if (*p==0)
			return 0;
	}
	return p;
}

// ------------------------------
int yistrzCount(const strz dest)
{
	char* p = (char*)dest;
	int cnt=0;

	while(1)
	{
		while(*p) p++;
		p++;

		if (*p==0)
			break;
		cnt++;
	}

	return cnt+1;
}

// ------------------------------
strz* yistrzp(int n)
{
	strz* p = (strz*)yialloci(sizeof(strz)*(n+2));
	p[0]=(strz)n;
	return &p[1];
}

// ------------------------------
void yistrzp_del(strz* str)
{
	if (str)
	{
		strz* p = &str[-1];
		int n = (int)str[-1];
		int i;

		for (i=0; i<n; i++)
		{
			if (str[i])
				yifree(str[i]);
		}
		yifree(p);
	}
}


// ------------------------------
char* yistrchcpy(char* dest, const char* src, char ch)
{
	while(*src!=ch &&
		*src!=0)
	{
		*dest=*src;
		dest++;src++;
	}
	*dest=0;
	return dest;
}

