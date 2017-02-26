#include "yistd.h"
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#define __malloci__(n) HeapAlloc(GetProcessHeap(), /*HEAP_NO_SERIALIZE|*/HEAP_ZERO_MEMORY, n)
#define __malloc__(n) HeapAlloc(GetProcessHeap(), /*HEAP_NO_SERIALIZE*/0, n)
#define __free__(p) HeapFree(GetProcessHeap(), /*HEAP_NO_SERIALIZE*/0, p)
#else
#define __malloci__(n) memset(malloc(n), 0, n)
#define __malloc__(n) malloc(n)
#endif

// ----------------------------------
void* FASTCALL  yiallocx(int n)
{
	register char* p = (char*)__malloc__(n+sizeof(IntPtr)+YI_DEF_STRLEN+YI_ALIGNMENT);
	register long remain;
	register char* pAligned;
#ifdef __x64__
	remain = ((p+sizeof(int32)) & 0x000000000000000F);
#else
	remain = (long)((long)(p+sizeof(int32)) & 0x0000000F);
#endif
	
	remain = (remain==0)?YI_ALIGNMENT+sizeof(int32):remain;

	pAligned = &p[YI_ALIGNMENT-remain];
	((IntPtr*)pAligned)[-1] = (IntPtr)p;	

	return (void*)pAligned;
}

// ----------------------------------
void* FASTCALL  yialloci(int n)
{
	return __malloci__(n);
}


// ----------------------------------
void* FASTCALL  yialloc(int n)
{
	return __malloc__(n);
}

// ----------------------------------
void* FASTCALL  yirealloc(void* p, int n)
{
	return HeapReAlloc(GetProcessHeap(), 0,p, n);
}

// ----------------------------------
void* FASTCALL  yitoh(const void* buffer, int n)
{
	void* p = __malloc__(n);

	if (p==0)
		return 0;

	memcpy(p, buffer, n);
	return p;
}

// ----------------------------------
void yifree(void* p)
{
	if (p)
		__free__(p);
}

// ----------------------------------
void* FASTCALL  yiallocix(int n)
{
	int size = n+sizeof(int32)+YI_DEF_STRLEN+YI_ALIGNMENT;
	register char* p = (char*)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY, size);
	register long remain;
	register char* pAligned;

	if (p==0)
	{
		return 0;
	}

	ZeroMemory(p, size);
#ifdef __x64__
	remain = ((p+sizeof(int32)) & 0x000000000000000F);
#else
	remain = (long)((long)(p+sizeof(int32)) & 0x0000000F);
#endif
	
	remain = (remain==0)?YI_ALIGNMENT+sizeof(IntPtr):remain;

	pAligned = &p[YI_ALIGNMENT-remain];
	((IntPtr*)pAligned)[-1] = (IntPtr)p;


	return (void*)pAligned;
}

// ----------------------
void yifreex(void* p)
{
	int* pr = (int*)p;
	yifree((void*)pr[-1]);
}

// ----------------------
void* FASTCALL yitoheap(void* buffer, int size)
{
	return memcpy(yialloc(size), buffer, size);
}




// ----------------------
wchar_t* FASTCALL yistrheapW(const wchar_t* str)
{
	int len;
	if (str==0)
		return 0;

	len = wcslen(str);
	return (wchar_t*)yitoheap((void*)str, (len+1)<<1);
}

// ----------------------
char* FASTCALL yistrheap(const char* str)
{
	int len;
	if (str==0)
		return 0;
	len = strlen(str);
	return (char*)yitoheap((void*)str, len+1);
}

// ----------------------
char* FASTCALL yistrheapex(const char* str, char ch)
{
	int len=0;
	char* pIndex=(char*)str;
	char* ret;
	while(*pIndex!=0 &&
			*pIndex!=ch) len++;

	ret = (char*)yitoheap((void*)str, len+1);
	ret[len]=0;
	return ret;
}

// ------------------------------
Bool FASTCALL yiatoi(const char* buffer, int* n)
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
Bool FASTCALL yiatof(void* buffer, float* n)
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

// ------------------------------
Bool yiIsOverXP()
{
	OSVERSIONINFO osVer;
	osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx( &osVer );

	if (osVer.dwMajorVersion<6)
		return False;

	return True;
}


// ----------------------------------------
char* yistrchcpy(char*dest, const char* src, char ch)
{
	char* p=(char*)src;

	while(*p && *p!=ch)
		*dest++=*p++;

	*dest=0;
	return p;
}

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
		
		dest = pIndex = yialloci(len);
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
const char* yistrzCount(const strz dest)
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
	strz* p = (strz*)yialloci(sizeof(strz)*n+2);
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
			if (str[n])
				yifree(str[n]);
		}
		yifree(p);
	}
}

// ------------------------------
char* yistrstrheap(const char* str, const char* eos)
{
	char* pIndex = strstr(str, eos);
	int len = pIndex-str;
	char* ret=0;

	if (len==0)
		return 0;

	ret = yitoheap(pIndex, len+1);
	ret[len]=0;
	return ret;
}

// ------------------------------
char* yistrBetween(const char* str, const char* start, const char* end, Bool bContainEof)
{
	char* pStart = strstr(str, start);
	char* pEnd = pStart;
	int len=0;
	char* pRet;

	if (pStart==0)
		return 0;

	len = strlen(start);

	if (strlen(pStart)<=len)
	{
		return 0;
	}

	pStart+=strlen(start);
	pEnd = strstr(pStart, end);

	if (pEnd==0)
	{
		if (bContainEof)
			return yistrheap(pStart);
		return 0;
	}

	len = pEnd-pStart;
	pRet = yitoheap(pStart, len+1);
	pRet[len]=0;
	return pRet;
}

// ------------------------------
char* yistrBetweenEx(const char* str, const char* start, const char* end)
{
	return 0;
}

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
char* yistrTrimIndex(const char* src, const char* trimz)
{
	char* pStart=src;
	int n = strlen(trimz);
	while(*pStart!=0 &&
			xIsContain(trimz, n, *pStart)==True) pStart++;

	return pStart;
}

// ------------------------------
char* yistrTrim(char* dest, const char* src, const char* trimz)
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

	if (dest==0)
	{
		dest = yialloc(n+1);
	}

	strncpy(dest, pStart, n);
	dest[n]=0;

	return dest;
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

#include "yiListx.h"
// ------------------------------
strz yistrSplit(const char* src, const char* untilz)
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
		char* p2 = yistrTrim(p, p, " ");
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
int yistrReplaceChar(char* str, char before, char after)
{
	int cnt=0;
	while(*str)
	{
		if (*str==before)
		{
			cnt++;
			*str=after;
		}
		str++;
	}
	return cnt;
}

// ------------------------------
char* yistrReplaceStr(const char* str, const char* before, const char* after)
{
	char* pIndex = (char*)str;
	int   cnt=0;
	char* pRet = 0;
	int n=0;
	int bn=strlen(before);
	int an=strlen(after);
	char* pIndex2;

	while((pIndex=strstr(pIndex, before))!=0) {
		cnt++;
		pIndex+=bn;
	}

	pIndex=(char*)str;
	if (cnt==0)
		return 0;

	n = (strlen(str)+(strlen(after)-bn)*cnt);
	pRet = yialloc(n+1);
	pRet[n]=0;
	pIndex2=pRet;

	while(*pIndex)
	{
		if (strncmp(pIndex, before, bn)==0)
		{
			// replace.
			memcpy(pIndex2, after, an);
			pIndex2+=an;
			pIndex+=bn;
			continue;
		}
		*pIndex2=*pIndex;
		pIndex2++;
		pIndex++;
	}
	
	return pRet;
}

extern Bool g_yilib_IsDebug;
int g_yilib_locale_code = 0;
const char* g_yilib_moji_code="sjis";

// ----------------------------------
int yiGetSystemLocale(char* mozi)
{
	if (mozi)
		strcpy(mozi, g_yilib_moji_code);

	return g_yilib_locale_code;
}

#ifndef STATIC_COMPILE
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		char buffer[32];
		GetPrivateProfileString("yiLogger", "Debug", "", buffer, 32, ".\\x.ini");
		if (stricmp(buffer, "On")==0)
		{
			int port=0;
			g_yilib_IsDebug=True;
			port = GetPrivateProfileInt("yiLogger", "port", 514, ".\\x.ini");
			yiTraceInit(port);
		}

		// ここで文字コードなどを取得し、システムロケールを取得する。
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#endif