#include "igx_profile.h"

// ------------------------------------------
#define xxTRACE printf

//#define xxTRACE(...) 

#define MAX_SECTION_LEN 64
#define INDEX_CUR 0
#define INDEX_BEFORE 1
#define INDEX_NEXT 2

#define IGX_PROFILE_MALLOC malloc
#define IGX_PROFILE_FREE free

#define FASTCALL 

// ------------------------------------------
typedef struct {
	bit32_t fd;
	bit32_t size;
	char* p;
	profile_tBinary* bin;
} profile_t;

#ifdef __POSIX__
#define xopen open
#define xlseek lseek
#define xread read
#define xwrite write 
#define xclose close
#define xtruncate ftruncate

#include <fcntl.h>
#endif

// ------------------------------
static char* FASTCALL strnheap(const char* str, bit32_t size)
{
	char* p;

    size = (size==0)?(strlen(str)+1):size;
	p = (char*)IGX_PROFILE_MALLOC(size);
	memcpy(p, str, size);

	return p;
}

// ------------------------------
static char* FASTCALL yistrheap(const char* str)
{
	return strnheap(str, 0);
}


// ------------------------------
static void* FASTCALL yitoheap(void* buffer, bit32_t n)
{
	void* p = IGX_PROFILE_MALLOC(n);
	memcpy(p, buffer, n);
	return p;
}

// ------------------------------
static Bool yiatoi(const char* buffer, bit32_t* n)
{
	bit32_t cnt=0;
	char temp[16];
	char* _buff=(char*)buffer;
	bit32_t len=0;

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
				return false;

			if (*_buff!='-')
			{
				return false;
			}
		}
		temp[cnt++]=*_buff++;
	}

	temp[cnt]=0;
	*n= atoi(temp);

	return true;
}



// --------------------------------------
static char* IGX_PROFILE_MALLOCi(bit32_t size)
{
	char* p = (char*)IGX_PROFILE_MALLOC(size);

	if (p)
		return (char*)memset(p, 0, size);
	return 0;
}

// ------------------------------
static char* FASTCALL yistrSub(char* dest, const char* str, bit32_t start, int end)
{
	char* ret = (dest==0)?(char*)IGX_PROFILE_MALLOC(end-start+1):dest;
	memcpy(ret, &str[start], (end-start));
	ret[end-start]=0;

	return ret;
}

#define xIS_SPACE(a) (a==' ')
#define xIS_TRIMCHARZ(a) (a==' '||a=='\t'||a=='\r'||a=='\n')

// ------------------------------
// space, tab
static char* FASTCALL yistrTrim(char* dest, const char* str, Bool bSpaceOnly)
{
	bit32_t i=0, start=0, end=strlen(str);
	char* pIndex=(char*)str;
	//char sztrimCharz[] = {' ', '\t', '\r', '\n', 0};

	if (bSpaceOnly==false)
		goto FULL_TRIM;

	for (i=0; i<end; i++)
	{
		if (pIndex[i]==' ')
		{
			start++;
			continue;
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
	return yistrSub(dest, str, start, end);

FULL_TRIM:
	for (i=0; i<end; i++)
	{
		bit32_t k=0;

		if (xIS_TRIMCHARZ(pIndex[i]))
		{
			start++;
			continue;
		}
		break;	
	}

	for (i=end-1; i>=0; i--)
	{
		bit32_t k=0;
		if (xIS_TRIMCHARZ(pIndex[i]))
		{
			end--;
			continue;
		}
		break;
	}

	return yistrSub(dest, str, start, end);
}



// ------------------------------
static char* FASTCALL yistrGetLine(char* dest, const char* str, bit32_t n, char** next)
{
	char* pIndex = (char*)str;
	bit32_t len=0;

	while(*pIndex&&*pIndex!='\n') pIndex++;

	len=(bit32_t)(pIndex-str);

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

	return yistrSub(dest, str, 0, len);
}


// ------------------------------
static void yistrSplitFree(char** str)
{
	bit32_t cnt=0;
	if (str==0)
		return ;

	while(str[cnt])
		IGX_PROFILE_FREE(str[cnt++]);
	
	IGX_PROFILE_FREE(str);
}

// ------------------------------
static char** FASTCALL yistrSplit(const char* str, const char* spliter, bit32_t* _cnt)
{
	char* pIndex=(char*)str;
	char* pLastIndex=0;
	char* pNext;
	bit32_t start=0;
	bit32_t cnt=0;
	bit32_t nSplitCnt=0;
	bit32_t limitCnt=16;
	bit32_t spliterlength=(bit32_t)strlen(spliter);

	char** ret = (char**)IGX_PROFILE_MALLOC(sizeof(char*)*limitCnt);

	xxTRACE("yistrSplit before loop\n");
	while((pNext=strstr(pIndex,spliter))!=0)
	{
		bit32_t len=0;
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
			bit32_t i=0;
			limitCnt+=16;
			ret = (char**)IGX_PROFILE_REALLOC(ret, sizeof(char*)*(limitCnt+1));
			
			for (i=0; i<16+1; i++)
			{
				ret[limitCnt-16+i-1]=0;
			}
		}

		xxTRACE("yistrSplit Set pIndex(pIndex:%s, pNext:%s), %d, %d\n", pIndex, pNext, start, start+len);
		ret[nSplitCnt++]=yistrSub(0, pIndex, start, start+len);
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
		ret[nSplitCnt++]=strnheap(pLastIndex, 0);
	}

	if (_cnt)
		*_cnt=nSplitCnt;

	return ret;
}

// ------------------------------------------
static bit32_t xSearchId(profile_t*s, int id, void* eid)
{
	const bit32_t nExtraIdSize = s->bin->bh.nExtraIdSize;
	const bit32_t nElementSize=sizeof(profile_tBinaryElement)+nExtraIdSize;
	char* pIndex=(char*)s->p;

	bit32_t i=0;

	xxTRACE("xSearchId() extraIdSize:%d, %d, %s\n", nExtraIdSize, id, (char*)eid);

	if (eid==0 ||
		nExtraIdSize==0)
	{
		for (i=0; i<s->bin->bh.nMaxElementCnt; i++)
		{
			profile_tBinaryElement* el = (profile_tBinaryElement*)pIndex;

			if (el->size!=0 &&
				el->id==id)
			{
				return i;
			}
	
			pIndex+=nElementSize;
		}
		return -1;
	}

	if (nExtraIdSize==sizeof(bit32_t))
	{
		for (i=0; i<s->bin->bh.nMaxElementCnt; i++)
		{
			profile_tBinaryElement* el = (profile_tBinaryElement*)pIndex;

			if (el->size!=0 &&
				el->id==id)
			{
				bit32_t* k = (int*)&el[1];
				if (*k==*((bit32_t*)eid))
				{
					return i;
				}
			}
	
			pIndex+=nElementSize;
		}

		return -1;
	}

	for (i=0; i<s->bin->bh.nMaxElementCnt; i++)
	{
		profile_tBinaryElement* el = (profile_tBinaryElement*)pIndex;
		//xxTRACE("xSearchId() element(%d) size:%d, pos:%d, id:%d, exid: \n", i, el->size, el->pos, el->id/*, &el[1]*/);

		if (el->size!=0 &&
			el->id==id)
		{
			//xxTRACE("xSearchId() compare %s, %s\n", &el[1], eid);
			if (memcmp(&el[1], eid, nExtraIdSize)==0)
			{
				xxTRACE("xSearchId() it's same (%s) index:%d\n", (char*)eid, i);
				return i;
			}
		}
	
		pIndex+=nElementSize;
	}
	
	
	return -1;
}

// ------------------------------------------
static void xSetExtraId(profile_t*s, profile_tBinaryElement* el, void* id)
{
	void* dest = (void*)&el[1];
	memcpy(dest, id, s->bin->bh.nExtraIdSize);
}

// ------------------------------------------
static bit32_t xGetBlankIndex(profile_t*s, int size)
{
	const bit32_t nElementSize=sizeof(profile_tBinaryElement)+s->bin->bh.nExtraIdSize;
	bit32_t i=0;
	bit32_t normalIndex=-1;
	char* pIndex=(char*)s->p;
	profile_tBinaryElement* before=0;
	profile_tBinaryElement* next=0;


	if (s->bin->bh.nTotalSizeLimitted!=0 &&
		(size+s->bin->bh.nUsedSize>s->bin->bh.nTotalSizeLimitted))
	{
		xxTRACE("size is full \n");
		return -1;
	}

	if (s->bin->bh.bIsDeleteAtTime==true)
	{
		bit32_t i=0;
		for (i=0; i<s->bin->bh.nMaxElementCnt; i++)
		{
			profile_tBinaryElement* el = (profile_tBinaryElement*)pIndex;

			if (el->size==0)
				return i;

			pIndex+=nElementSize;
		}

		xxTRACE("element is full !\n");
		return -1;
	}

	for (i=0; i<s->bin->bh.nMaxElementCnt; i++)
	{
		profile_tBinaryElement* el = (profile_tBinaryElement*)pIndex;

		if (el->size==0)
		{
			bit32_t prevpos=before?(before->pos+before->size):0;
			bit32_t nextpos=next?(next->pos):0;
			
			xxTRACE("index:%d, prevpos:%d, nextpos:%d\n", i, prevpos, nextpos);

			if (normalIndex==-1)
			{
				normalIndex=i;
			}

			if (nextpos==0)
				return i;

			if (nextpos-prevpos>=size)
			{
				return i;
			}

		}
		before = (profile_tBinaryElement* )pIndex;
		pIndex+=nElementSize;

		next = i<(s->bin->bh.nMaxElementCnt-1) ? (profile_tBinaryElement* )(pIndex+nElementSize):0;
	}

	xxTRACE("element is full !\n");
	return normalIndex;
}

// ------------------------------------------
static bit32_t xReloadFile(profile_t* s)
{
	bit32_t nReads=0;
	bit32_t size = LIB_FileSize(s->fd);//LIB_FileSeek(s->fd, 0, SEEK_END)-LIB_FileSeek(s->fd, 0, LIB_kFileSet);

	if (s->p)
		IGX_PROFILE_FREE(s->p);

	if (size==0)
	{
		s->p=0;
		s->size=0;
		return 0;
	}
	xxTRACE("s->fd:%x, size:%d\n", s->fd, size);

//	if (size>sizeof(bit32_t) &&
//		xread(s->fd, &nReads, sizeof(bit32_t))==sizeof(bit32_t))
#if 0
	if (size>sizeof(bit32_t) &&
		LIB_FileRead(&nReads, s->fd, sizeof(bit32_t))==sizeof(bit32_t))
	{
		if (nReads==0 ||
			nReads==1)
		{
			IGX_PROFILE_FREE(s->p);
			s->p=0;
			LIB_FileSeek(s->fd, 0, LIB_kFileSet);
			return -2; // it is binary format.
		}
	}
#endif
	LIB_FileSeek(s->fd, 0, LIB_kFileSet);
	//LIB_FileSeek(s->fd, 0, LIB_kFileSet);

	s->size=size;
	s->p = (char*)IGX_PROFILE_MALLOC(size+1);

	xxTRACE("before xread ..... \n");
	if ((nReads=LIB_FileRead(s->p, s->fd, (S64)size))!=size)
	{
		xxTRACE("after xread success..... \n");
		IGX_PROFILE_FREE(s->p);
		s->p=0;
		return -1; // error.
	}
	xxTRACE("xread success..... \n");
	s->p[size]=0;
	return size;
}

// ------------------------------------------
static void xPrbit32_tHeader(profile_t*s)//, int index, int size, int next, void* id)
{
	bit32_t elementSize = sizeof(profile_tBinaryElement)+s->bin->bh.nExtraIdSize;
	bit32_t i=0;
	bit32_t nStartOffset=0;
	char* pIndex=(char*)s->p;

	if (pIndex==0)
		return ;

	xxTRACE("xPrbit32_tHeader() cnt:%d, limited:%d, used:%d, deleteflag:%d\n", 
		s->bin->bh.nMaxElementCnt, s->bin->bh.nTotalSizeLimitted, s->bin->bh.nUsedSize, s->bin->bh.bIsDeleteAtTime);

	for (i=0; i<s->bin->bh.nMaxElementCnt; i++)
	{
		profile_tBinaryElement* sx = (profile_tBinaryElement*)&pIndex[nStartOffset];

		if (sx->size!=0)
		{
			xxTRACE("xPrbit32_tHeader() index:%d, size:%d, id:%d, pos:%d, exid:%s\n", i, sx->size, sx->id, sx->pos, (char*)&sx[1]);
		}
		nStartOffset+=elementSize;
	}
}

// ------------------------------------------
static Bool xReadHeader(profile_t* s)
{
	bit32_t headerSize; 
	void* p;

	LIB_FileSeek(s->fd, 0, LIB_kFileSet);
	//LIB_FileSeek(s->fd, 0, LIB_kFileSet);


	if (LIB_FileRead(&s->bin->bh, s->fd, sizeof(s->bin->bh))!=sizeof(s->bin->bh))
	{
		IGX_PROFILE_FREE(p);
		return false;
	}
	headerSize = s->bin->bh.nMaxElementCnt*(s->bin->bh.nExtraIdSize+sizeof(profile_tBinaryElement));
	p = IGX_PROFILE_MALLOCi(headerSize);
	xxTRACE("xReadHeader.... headerSize:%d\n", headerSize);
	if (LIB_FileRead(p, s->fd, headerSize)!=headerSize)//sizeof(s->bin->bh))
	{
		IGX_PROFILE_FREE(p);
		return false;
	}
	s->p=(char*)p;	
	s->size = LIB_FileSize(s->fd)/*LIB_FileSeek(s->fd, 0, SEEK_END)*/-headerSize-sizeof(profile_tBinaryHeader);
	s->bin->nOffset = headerSize+sizeof(profile_tBinaryHeader);
	LIB_FileSeek(s->fd, 0, LIB_kFileEnd);

	xxTRACE("xReadHeader.... bodysize:%d offset:%d SUCCESS\n", s->size, s->bin->nOffset);
	xPrbit32_tHeader(s);
	return true;
}

// ------------------------------------------
static Bool xCreateHeader(profile_t* s)
{
	bit32_t headerSize = s->bin->bh.nMaxElementCnt*(s->bin->bh.nExtraIdSize+sizeof(profile_tBinaryElement));
	void* p = IGX_PROFILE_MALLOCi(headerSize);
	LIB_FileSeek(s->fd, 0, LIB_kFileSet);
	//LIB_FileSeek(s->fd, 0, LIB_kFileSet);

	xxTRACE("xCreateHeader.... enter\n");

	if (LIB_FileWrite(s->fd, &s->bin->bh, (S64)sizeof(s->bin->bh))!=true/*sizeof(s->bin->bh)*/)
	{
		IGX_PROFILE_FREE(p);
		return false;
	}
	if (LIB_FileWrite(s->fd, &p, (S64)headerSize)!=headerSize)//sizeof(s->bin->bh))
	{
		IGX_PROFILE_FREE(p);
		return false;
	}

	s->p=(char*)p;
	s->size = (U32)LIB_FileSize(s->fd)/*LIB_FileSeek(s->fd, 0, SEEK_END)*/-headerSize-sizeof(profile_tBinaryHeader);
	s->bin->nOffset=headerSize+sizeof(profile_tBinaryHeader);
	LIB_FileSeek(s->fd, 0, LIB_kFileEnd);
	
	return true;
}



// ------------------------------------------
static Bool xUpdateHeader(profile_t* s)
{
	bit32_t headerSize = s->bin->bh.nMaxElementCnt*(s->bin->bh.nExtraIdSize+sizeof(profile_tBinaryElement));
	//LIB_FileSeek(s->fd, sizeof(profile_tBinaryHeader), LIB_kFileSet);
	LIB_FileSeek(s->fd, 0, LIB_kFileSet);

	if (LIB_FileWrite(s->fd, &s->bin->bh, (S64)sizeof(profile_tBinaryHeader))!=true/*sizeof(profile_tBinaryHeader)*/)//sizeof(s->bin->bh))
	{
		IGX_PROFILE_FREE(s->p);
		return false;
	}

	xxTRACE("xUpdateHeader() size %d\n", headerSize);

	if (LIB_FileWrite(s->fd, s->p, (S64)headerSize)!=headerSize)//sizeof(s->bin->bh))
	{
		xxTRACE("header write error....\n");
		IGX_PROFILE_FREE(s->p);
		return false;
	}
	xPrbit32_tHeader(s);

	return true;
}
//
// ------------------------------------------
static Bool xShiftHeaderPos(profile_t* s, bit32_t from, int value)
{
	//bit32_t headerSize = s->bin->bh.nMaxElementCnt*(s->bin->bh.nExtraIdSize+sizeof(profile_tBinaryElement));
	bit32_t elementSize = sizeof(profile_tBinaryElement)+s->bin->bh.nExtraIdSize;
	bit32_t nStartOffset=elementSize*from;
	char* pIndex=(char*)s->p;
	bit32_t i=0;

	for (i=from; i<s->bin->bh.nMaxElementCnt; i++)
	{
		profile_tBinaryElement* sx = (profile_tBinaryElement*)&pIndex[nStartOffset];
		sx->pos+=value;
		nStartOffset+=elementSize;
	}
	xPrbit32_tHeader(s);

	return true;
}

// ------------------------------------------
static Bool xSetHeader(profile_t*s, bit32_t index, int size, int next, void* id)
{
	bit32_t n = s->bin->bh.nExtraIdSize+sizeof(profile_tBinaryElement);
	char* pIndex = (char*)s->p;
	profile_tBinaryElement* eh = (profile_tBinaryElement* )&pIndex[n*index];

	//eg->index=index;
	eh->size=size;

	if (s->bin->bh.nExtraIdSize==0)
	{
		return true;
	}

	if (s->bin->bh.nExtraIdSize==sizeof(bit32_t))
	{
		bit32_t* pInt = (int*)&eh[1];
		*pInt = *((bit32_t*)id);
		return true;
	}
	memcpy(&eh[1], id, s->bin->bh.nExtraIdSize);
	return true;
}

// ------------------------------------------
profile_tBinaryElement* xGetIndexHeader(profile_t* s, bit32_t index, int act)//INDEX_CUR)
{
	bit32_t elementSize = sizeof(profile_tBinaryElement)+s->bin->bh.nExtraIdSize;
	char* pIndex = (char*)s->p;

	xxTRACE("xGetIndexHeader(%d), %d\n", index, act);

	if (act==INDEX_CUR)
	{
		char* pIndex = (char*)s->p;
		return (profile_tBinaryElement*)&pIndex[elementSize*index];
	}
	else if (act==INDEX_BEFORE)
	{
		bit32_t i=0;
		bit32_t nStartOffset;

		if (index-1<0)
		{
			return 0;
		}

		nStartOffset=elementSize*(index-1);
		for (i=index-1; i>=0; i--)
		{
			profile_tBinaryElement* sx = (profile_tBinaryElement*)&pIndex[nStartOffset];
			if (sx->size!=0)
			{
				return sx;
			}
			nStartOffset-=elementSize;
		}
	}
	else
	{
		bit32_t i=0;
		bit32_t nStartOffset;

		if ((index+1)>=s->bin->bh.nMaxElementCnt)
		{
			return 0;
		}

		nStartOffset=elementSize*(index+1);
		for (i=index+1; i<s->bin->bh.nMaxElementCnt; i++)
		{
			profile_tBinaryElement* sx = (profile_tBinaryElement*)&pIndex[nStartOffset];//elementSize*i];
			if (sx->size!=0)
			{
				return sx;
			}
			nStartOffset+=elementSize;
		}
	}
	return 0;
}

// ------------------------------------------
#if 0
HIGX_PROFILE igx_profile_open(const char* pszFileName, sLIB_ProfileDetailForBinaryMode* detail)
#endif 
HIGX_PROFILE igx_profile_open(const char* pszFileName)
{
	profile_t s = {0, };
	bit32_t nFileSize;
#if 0
	bit32_t fd = xopen(pszFileName, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH); // 755?
#endif
	LIB_TFileObject* fd = LIB_FileOpen(pszFileName, LIB_kFileTypeAppend);

	if (fd==0)
	{
		
		LIB_TFileObject* fd = LIB_FileOpen(pszFileName, LIB_kFileTypeWrite);
		if (fd==0)
		{
			return 0;
		}

		LIB_FileClose(fd);
		fd = LIB_FileOpen(pszFileName, LIB_kFileTypeAppend);
	}

	s.fd=fd;
#if 0
	flock(fd, LOCK_EX);
	s.bin=detail==0?0:IGX_PROFILE_MALLOCi(sizeof(profile_tBinary));
#else
	s.bin=0;
#endif
	xxTRACE("trace......1\n");

	//s.size = nFileSize = LIB_FileSeek(fd, 0, SEEK_END);
	s.size=nFileSize=(bit32_t)LIB_FileSize(fd);
	LIB_FileSeek(fd, 0, LIB_kFileEnd);
OPEN_FOR_BINARY:
	if (s.bin!=0)
	{
#if 0
		if (nFileSize>sizeof(profile_tBinaryHeader))
		{
			xReadHeader(&s);//fd, &s.header);
		}
		else
		{
			s.bin->bh.nMaxElementCnt	= detail->nElementCount;
			s.bin->bh.nExtraIdSize 		= detail->nExtraIdSize;
			s.bin->bh.nTotalSizeLimitted	= detail->nTotalSizeLimitted;
			s.bin->bh.bIsDeleteAtTime	= detail->bIsDeleteAtTime;
			xCreateHeader(&s);//fd, &s.header);
		}
		xPrbit32_tHeader(&s);
#endif
	}
	else
	{
		bit32_t res=0;
		if ((res = xReloadFile(&s))==-1)
		{
			// case size is zero.
            LIB_FileClose(fd);
			//xclose(fd);
			return 0;
		}
		else if (res==-2)
		{
			s.bin = (profile_tBinary*)IGX_PROFILE_MALLOC(sizeof(profile_tBinary));
			goto OPEN_FOR_BINARY;
		}
	}
	return (HIGX_PROFILE)yitoheap(&s, sizeof(s));
	
}

// ------------------------------------------
Bool LIB_ProfileReadStringEx(HIGX_PROFILE hProfile, const char* section, const char* key, char* value)
{
	profile_t* s = (profile_t*)hProfile;
	char* pIndex=s->p;
	char _section[64];
	char* pSectionStart, *pSectionEnd;
	bit32_t sectionLen;
	xxTRACE("LIB_ProfileReadString: Search(%x, %s, %s)\n", hProfile, section, key);

	if (s->bin!=0 ||
		s->p==0)
	{
		xxTRACE("file is not found\n");
		return false;
	}

	sprintf(_section, "[%s]", section);
	sectionLen = (bit32_t)strlen(_section);

	if ((pSectionStart=LIB_strstri(pIndex, _section))==0)
	{
		return 0;
	}

	pSectionEnd=pSectionStart+sectionLen;
	while(1)
	{
		pSectionEnd=strstr(pSectionEnd, "[");

		if (pSectionEnd)
		{
			if (pSectionEnd[-1]!='\n')
			{
				pSectionEnd++;
				continue;	
			}
		}
		break;
	}

	pIndex=pSectionStart+sectionLen;
	
	xxTRACE("LIB_ProfileReadString: Search(%s, %s)\n", key, value);
RETRY_SEARCH:
	//while((pIndex=strstr(pIndex, key))!=0)
	while((pIndex=LIB_strstri(pIndex, key))!=0)
	{
		if (pSectionEnd!=0 &&
				pIndex>pSectionEnd)
			break;

		if (pIndex==s->p)
			break;	
		
		if (pIndex[-1]=='\n')
		{
			break;
		}
		xxTRACE("LIB_ProfileReadString: Search(%s, %s) Hitted but ???? %s\n", key, value, pIndex);
		pIndex++;
	}
	
	if (pIndex==0)
	{
		return false;
	}
	
	{
		char* pNextLineTop=0;
		char* pLine = yistrGetLine(pIndex, 0, &pNextLineTop);
		bit32_t   n;
		char** pKeyAndValue;// = yistrSplit(pLine, "=", &n);
		char* pKey, *pValue, *strTemp;

		// it is comment.
		if (pLine[0]=='#' || pLine[0]=='\t' || pLine[0]==' ')
		{
			IGX_PROFILE_FREE(pLine);
			pIndex=pNextLineTop;
			goto RETRY_SEARCH;
		}

		pKeyAndValue = yistrSplit(pLine, "=", &n);
		IGX_PROFILE_FREE(pLine);

		if (n!=2)
		{
			yistrSplitFree(pKeyAndValue);
			pIndex=pNextLineTop;
			goto RETRY_SEARCH;
		}

		// key=value
		pKey = yistrTrim(pKeyAndValue[0], false);

		if (strcmp(pKey, key)!=0)
		{
			IGX_PROFILE_FREE(pKey);
			IGX_PROFILE_FREE(pValue);
			yistrSplitFree(pKeyAndValue);
			pIndex=pNextLineTop;
			goto RETRY_SEARCH;
		}


		// case
		// key = value # comment ...... 
		strTemp=strstr(pKeyAndValue[1], " #");
		if (strTemp)
			*strTemp=0; 

		// key = value	# comment ...... 
		strTemp=strstr(pKeyAndValue[1], "\t#");
		if (strTemp)
			*strTemp=0; 

		pValue = yistrTrim(pKeyAndValue[1], false);

#if 0 // 仕様変更 値がちゃんと設定されていない場合のキーは無効。
		if (n==1)
			value[0]=0;
		else
			strcpy(value, pValue);
#endif
		xxTRACE("value is %s\n", pValue);
		strcpy(value, pValue);

		IGX_PROFILE_FREE(pKey);
		IGX_PROFILE_FREE(pValue);
		yistrSplitFree(pKeyAndValue);
	}

	return true;	
}

// ------------------------------------------
Bool LIB_ProfileReadString(HIGX_PROFILE hProfile, const char* key, char* value)
{
	profile_t* s = (profile_t*)hProfile;
	char* pIndex=s->p;

	if (s->bin!=0 ||
		s->p==0)
	{
		return false;
	}

	xxTRACE("LIB_ProfileReadString: Search(%s, %s)\n", key, value);
RETRY_SEARCH:
	while((pIndex=LIB_strstri(pIndex, key))!=0)
	{
		if (pIndex==s->p)
			break;	
		
		if (pIndex[-1]=='\n')
		{
			break;
		}
		//xxTRACE("LIB_ProfileReadString: Search(%s, %s) Hitted but ???? %s\n", key, value, pIndex);
		pIndex++;
	}
	
	if (pIndex==0)
	{
		return false;
	}
	
	{
		char* pNextLineTop=0;
		char* pLine = yistrGetLine(pIndex, 0, &pNextLineTop);
		bit32_t   n;
		char** pKeyAndValue;// = yistrSplit(pLine, "=", &n);
		char* pKey, *pValue;

		// it is comment.
		if (pLine[0]=='#')
		{
			IGX_PROFILE_FREE(pLine);
			pIndex=pNextLineTop;
			goto RETRY_SEARCH;
		}

		pKeyAndValue = yistrSplit(pLine, "=", &n);
		IGX_PROFILE_FREE(pLine);

		if (n==0 ||
			n>2)
		{
			yistrSplitFree(pKeyAndValue);
			pIndex=pNextLineTop;
			goto RETRY_SEARCH;
		}

		pKey = yistrTrim(pKeyAndValue[0], false);

		if (n==2)
		{
			pValue = yistrTrim(pKeyAndValue[1], false);
		}

		if (strcmp(pKey, key)!=0)
		{
			IGX_PROFILE_FREE(pKey);
			IGX_PROFILE_FREE(pValue);
			yistrSplitFree(pKeyAndValue);
			pIndex=pNextLineTop;
			goto RETRY_SEARCH;
		}


		if (n==1)
			value[0]=0;
		else
			strcpy(value, pValue);

		IGX_PROFILE_FREE(pKey);
		IGX_PROFILE_FREE(pValue);
		yistrSplitFree(pKeyAndValue);
	}

	return true;	
}

// ------------------------------------------
bit32_t LIB_ProfileReadInt(HIGX_PROFILE hProfile, const char* key, int nDefaultNum)
{
	char buffer[256];
	bit32_t ret;

	if (LIB_ProfileReadString(hProfile, key, buffer)==false)
	{
		return false;
	}

	if (yiatoi(buffer, &ret)==false)
	{
		return nDefaultNum;
	}
	return ret;
}

// ------------------------------------------
Bool LIB_ProfileReadIntEx(HIGX_PROFILE hProfile, const char* section, const char* key, bit32_t* val)
{
	char buffer[256];
	bit32_t ret;

	if (LIB_ProfileReadStringEx(hProfile, section, key, buffer)==false)
	{
		return false;
	}

	if (yiatoi(buffer, &ret)==false)
	{
		return false;//nDefaultNum;
	}
	*val=ret;
	return true;
}

// ------------------------------------------
Bool LIB_ProfileWriteInt(HIGX_PROFILE hProfile, const char* key, bit32_t n)
{
	char buffer[256];
	sprintf(buffer, "%d", n);
	return LIB_ProfileWriteString(hProfile, key, buffer);
}

// ------------------------------------------
Bool LIB_ProfileWriteIntEx(HIGX_PROFILE hProfile, const char* section, const char* key, bit32_t n)
{
	char buffer[256];
	sprintf(buffer, "%d", n);
	return LIB_ProfileWriteStringEx(hProfile, section, key, buffer);
}



// ------------------------------------------
Bool LIB_ProfileWriteStringEx(HIGX_PROFILE hProfile, const char* section, const char* key, const char* value)
{
	profile_t* s = (profile_t*)hProfile;
	char* pIndex=s->p;
	char* pFoundKeyLine=0;
	char* pNextLine=0;
	char _section[MAX_SECTION_LEN+2];
	char* pSectionStart=pIndex, *pSectionEnd=0;
	bit32_t sectionLen ;
	Bool bIsInsert=false;

	xxTRACE("LIB_ProfileWriteStringEx called (%s, %s, %s)\n", section, key, value);

	if (section==0)
		return false;

	if (s->bin!=0)
	{
		return false;
	}

	if (s->p==0)
	{
		pSectionStart=0;
		xxTRACE("s->p is zero, so goto append(%s=%s)\n", key, value);
		goto CASE_APPEND;
	}

	sprintf(_section, "[%s]", section);
	sectionLen = (bit32_t)strlen(_section);

	if ((pSectionStart=LIB_strstri(pIndex, _section))==0)
	{
		goto CASE_APPEND;
	}

	pSectionEnd=pSectionStart+sectionLen;
	while(1)
	{
		pSectionEnd=strstr(pSectionEnd, "[");

		if (pSectionEnd)
		{
			if (pSectionEnd[-1]!='\n')
			{
				pSectionEnd++;
				continue;	
			}

		}
		break;
	}

	pIndex=pSectionStart+sectionLen;

	xxTRACE("trace............1\n");


RETRY_SEARCH:
	xxTRACE("LIB_ProfileWriteString() before loop\n");
	while((pIndex=LIB_strstri(pIndex, key))!=0)
	{

		if (pSectionEnd!=0 &&
				pIndex>pSectionEnd)
		{
			bIsInsert=true;
			goto CASE_APPEND;
		}

		if (pIndex==s->p)
			break;	
		
		if (pIndex[-1]=='\n')
		{
			break;
		}
		pIndex++;
	}
	xxTRACE("LIB_ProfileWriteString() after loop\n");
	
	if (pIndex==0)
	{
		xxTRACE("Key is not found.... goto append\n");
		goto CASE_APPEND;
	}
	
	{
		char* pNextLineTop=0;
		char* pLine = yistrGetLine(pIndex, 0, &pNextLineTop);
		bit32_t   n;
		char** pKeyAndValue = yistrSplit(pLine, "=", &n);
		char* pKey=0, *pValue=0;

		xxTRACE("LIB_ProfileWriteString() Trace.....1\n");
		IGX_PROFILE_FREE(pLine);
		
		if (n==0)
		{
			yistrSplitFree(pKeyAndValue);
			pIndex=pNextLineTop;
			xxTRACE("LIB_ProfileWriteString() split is zero\n");
			goto RETRY_SEARCH;
		}
		xxTRACE("LIB_ProfileWriteString() split(%s, %s)\n", pKeyAndValue[0], pKeyAndValue[1]);

		pKey = yistrTrim(pKeyAndValue[0], false);

		if (strcmp(pKey, key)!=0)
		{
			IGX_PROFILE_FREE(pKey);
			IGX_PROFILE_FREE(pValue);
			yistrSplitFree(pKeyAndValue);
			pIndex=pNextLineTop;
			goto RETRY_SEARCH;
		}
		xxTRACE("LIB_ProfileWriteString() same key found! (%s, %s)\n", pKeyAndValue[0], pKeyAndValue[1]);

		IGX_PROFILE_FREE(pKey);
		IGX_PROFILE_FREE(pValue);
		yistrSplitFree(pKeyAndValue);

		pFoundKeyLine=pIndex;
		pNextLine = strstr(pFoundKeyLine, "\n");

		//LIB_FileSeek(s->fd, 0, LIB_kFileSet);
		LIB_FileSeek(s->fd, 0, LIB_kFileSet);
		if (pFoundKeyLine!=s->p)
		{
			LIB_FileWrite(s->fd, s->p, (S64)(pFoundKeyLine-s->p));
		}

		if (pNextLine)
		{
			pNextLine++;
			xxTRACE("LIB_ProfileWriteString() pNextLine:%s\n", pNextLine);//pKeyAndValue[0], pKeyAndValue[1]);
		}
	}

	{
		char buffer[1024];
		bit32_t len;
		//bit32_t upLen=(bit32_t)pFoundKeyLine-s->p;
	
		//if (pFoundKeyLine!=s->p)
		//{	
		//	sprintf(buffer, "\n%s=%s", key, value);
		//}
		//else
		//{
			sprintf(buffer, "%s=%s", key, value);
		//}

		len = (bit32_t)strlen(buffer);
		LIB_FileWrite(s->fd, buffer, (S64)len);

		if (pNextLine)
		{
			LIB_FileWrite(s->fd, "\n", 1);
			LIB_FileWrite(s->fd, pNextLine, (S64)(s->size - (pNextLine-s->p)));
		}
	}

	goto TRUNCATE;

CASE_APPEND:
	{
		char buffer[1024];
		char strSection[MAX_SECTION_LEN+3];
		strSection[0]=0;

		if (pSectionStart==0)
		{	
			xxTRACE("create section %s\n", section);
			sprintf(strSection, "[%s]\n", section);
		}

		if (s->p)
		{
			if (s->p[s->size-1]=='\n')
			{
				sprintf(buffer, "%s%s=%s", strSection, key, value);
			}
			else
			{
				sprintf(buffer, "\n%s%s=%s", strSection, key, value);
			}
		}
		else
			sprintf(buffer, "%s%s=%s", strSection, key, value);

		if (bIsInsert)
		{
			bit32_t end = pSectionEnd-s->p;
			LIB_FileSeek(s->fd, end, LIB_kFileSet);
			//LIB_FileSeek(s->fd, end, LIB_kFileSet);
		}
		else
		{
			LIB_FileSeek(s->fd, 0, LIB_kFileEnd);
			//LIB_FileSeek(s->fd, 0, SEEK_END);
		}

		xxTRACE("write %s\n", buffer);
		LIB_FileWrite(s->fd, buffer, (S64)strlen(buffer));

		if (bIsInsert)
		{
			LIB_FileWrite(s->fd, pSectionEnd, (S64)strlen(pSectionEnd));
		}
	}

TRUNCATE:
	{
		//bit32_t pos = LIB_FileSeek(s->fd, 0, LIB_kFileCur);
		LIB_FileTruncate(s->fd, LIB_FileTell(s->fd));
		//xtruncate(s->fd, pos);
	}	
	xReloadFile(s);
	return true;	
}



// ------------------------------------------
Bool LIB_ProfileWriteString(HIGX_PROFILE hProfile, const char* key, const char* value)
{
	profile_t* s = (profile_t*)hProfile;
	char* pIndex=s->p;
	char* pFoundKeyLine=0;
	char* pNextLine=0;

	xxTRACE("LIB_ProfileWriteString called (%s, %s)\n", key, value);

	if (s->bin!=0)
	{
		return false;
	}

	if (s->p==0)
	{
		xxTRACE("s->p is zero, so goto append\n");
		goto CASE_APPEND;
	}

RETRY_SEARCH:
	xxTRACE("LIB_ProfileWriteString() before loop\n");
	while((pIndex=LIB_strstri(pIndex, key))!=0)
	{
		if (pIndex==s->p)
			break;	
		
		if (pIndex[-1]=='\n')
		{
			break;
		}
		pIndex++;
	}
	xxTRACE("LIB_ProfileWriteString() after loop\n");
	
	if (pIndex==0)
	{
		xxTRACE("Key is not found.... goto append\n");
		goto CASE_APPEND;
	}
	
	{
		char* pNextLineTop=0;
		char* pLine = yistrGetLine(pIndex, 0, &pNextLineTop);
		bit32_t   n;
		char** pKeyAndValue = yistrSplit(pLine, "=", &n);
		char* pKey=0;

		xxTRACE("LIB_ProfileWriteString() Trace.....1\n");
		IGX_PROFILE_FREE(pLine);
		
		if (n==0)
		{
			yistrSplitFree(pKeyAndValue);
			pIndex=pNextLineTop;
			xxTRACE("LIB_ProfileWriteString() split is zero\n");
			goto RETRY_SEARCH;
		}
		xxTRACE("LIB_ProfileWriteString() split(%s, %s)\n", pKeyAndValue[0], pKeyAndValue[1]);

		pKey = yistrTrim(pKeyAndValue[0], false);

		if (strcmp(pKey, key)!=0)
		{
			IGX_PROFILE_FREE(pKey);
			//IGX_PROFILE_FREE(pValue);
			yistrSplitFree(pKeyAndValue);
			pIndex=pNextLineTop;
			goto RETRY_SEARCH;
		}
		xxTRACE("LIB_ProfileWriteString() same key found! (%s, %s)\n", pKeyAndValue[0], pKeyAndValue[1]);

		IGX_PROFILE_FREE(pKey);
		//IGX_PROFILE_FREE(pValue);
		yistrSplitFree(pKeyAndValue);

		pFoundKeyLine=pIndex;
		pNextLine = strstr(pFoundKeyLine, "\n");

		LIB_FileSeek(s->fd, 0, LIB_kFileSet);
		if (pFoundKeyLine!=s->p)
		{
			LIB_FileWrite(s->fd, s->p, pFoundKeyLine-s->p);
		}

		if (pNextLine)
		{
			pNextLine++;
			xxTRACE("LIB_ProfileWriteString() pNextLine:%s\n", pNextLine);//pKeyAndValue[0], pKeyAndValue[1]);
		}
	}

	{
		char buffer[1024];
		bit32_t len;

	
		//if (pFoundKeyLine!=s->p)
		//{	
		//	sprintf(buffer, "\n%s=%s", key, value);
		//}
		//else
		//{
			sprintf(buffer, "%s=%s", key, value);
		//}

		len = (bit32_t)strlen(buffer);
		LIB_FileWrite(s->fd, buffer, len);

		if (pNextLine)
		{
			LIB_FileWrite(s->fd, "\n", 1);
			LIB_FileWrite(s->fd, pNextLine, s->size - (U32)(pNextLine-s->p));
		}
	}

	goto TRUNCATE;

CASE_APPEND:
	{
		char buffer[1024];

		if (s->p)
		{
			if (s->p[s->size-1]=='\n')
			{
				sprintf(buffer, "%s=%s", key, value);
			}
			else
			{
				sprintf(buffer, "\n%s=%s", key, value);
			}
		}
		else
			sprintf(buffer, "%s=%s", key, value);

		LIB_FileSeek(s->fd, 0, LIB_kFileEnd);
		LIB_FileWrite(s->fd, buffer, strlen(buffer));
	}

TRUNCATE:
	{
        LIB_FileTruncate(s->fd, LIB_FileTell(s->fd));
		//xtruncate(s->fd, pos);
	}	
	xReloadFile(s);
	return true;	
}

// ------------------------------------------
const char** LIB_ProfileEnumKeysEx(HIGX_PROFILE hProfile, const char* section, bit32_t* _cnt)
{
	profile_t* s = (profile_t*)hProfile;
	char* pIndex=s->p;
	char* pSectionStart;
	char* pSectionEnd=0;
	bit32_t sectionLen;
	char _section[MAX_SECTION_LEN+2];
	char** pRet;
	HandleList hList;

	sprintf(_section, "[%s]", section);

	if (s==0 ||
			section==0)
		return 0;

	sectionLen=(bit32_t)strlen(_section);
	pSectionStart=LIB_strstri(pIndex, _section);

	if (pSectionStart==0)
		return 0;

	pSectionStart+=sectionLen;
	pSectionEnd=pSectionStart;
	while(1)
	{
		pSectionEnd=strstr(pSectionEnd, "[");

		if (pSectionEnd)
		{
			if (pSectionEnd[-1]!='\n')
			{
				pSectionEnd++;
				continue;	
			}

		}
		break;
	}

	pIndex=pSectionStart;
	xxTRACE("LIB_ProfileEnumKeysEx(%s)\n", pIndex);

	hList = LIB_ListCreate();
	while(pIndex)
	{
		char* pNextLineTop=0;
		bit32_t   n;
		char** pKeyAndValue;
		char* pLine;
		char* pKey=0;

		pLine = yistrGetLine(pIndex, 0, &pNextLineTop);

		if (pLine==0&&pNextLineTop==0)
			break;

		if (pLine==0)
		{
			pIndex=pNextLineTop;
			continue;
		}

		pKeyAndValue = yistrSplit(pLine, "=", &n);


		if (*pLine=='#')
		{
			IGX_PROFILE_FREE(pLine);
			pIndex=pNextLineTop;
			continue;
		}

		IGX_PROFILE_FREE(pLine);
		
		if (n!=2)
		{
			yistrSplitFree(pKeyAndValue);
			pIndex=pNextLineTop;

			
			if (pSectionEnd!=0 &&
					pIndex>pSectionEnd)
				break;
			xxTRACE("LIB_ProfileWriteString() split is zero\n");
			continue;
		}
		xxTRACE("LIB_ProfileWriteString() split(%s, %s)\n", pKeyAndValue[0], pKeyAndValue[1]);

		pKey = yistrTrim(pKeyAndValue[0], false);
		yistrSplitFree(pKeyAndValue);

		if (pKey==0)
		{
			pIndex=pNextLineTop;
			continue;
		}

		LIB_ListAdd(hList, pKey);

		/*if ((cnt&0x0F)==0)
		{
			bit32_t n = (((cnt&0xF0)>>4)+1)*0x10;//*sizeof(char*);
			bit32_t size = (n+1)*sizeof(char*);
			char** temp  = (char**)IGX_PROFILE_MALLOC((size+1)*sizeof(char*));
			
			if (temp==0)
				return 0;
			
			memset(temp, 0, size);
			memcpy(temp, pRet, (n-0x10)*sizeof(char*));
			free(pRet); 
			pRet=temp;
		}

		pRet[cnt++]=pKey;*/
		pIndex=pNextLineTop;

		if (pSectionEnd!=0 &&
				pIndex>pSectionEnd)
			break;
	}

	if (_cnt)
		*_cnt=LIB_ListCount(hList);

	pRet=(char**)LIB_ListExport(hList);

	LIB_ListClear(hList);
	LIB_ListDestroy(hList);
	
	return (const char**)pRet;
}

// ------------------------------------------
const char** LIB_ProfileGetEnumKeys(HIGX_PROFILE hProfile, bit32_t* cnt)
{
	return 0;
}

// ------------------------------------------
void LIB_ProfileReleaseEnumKeys(const char** p, bit32_t cnt)
{
	bit32_t i;

	if (p==0)
		return ;

	for (i=0; i<cnt; i++)
	{
		IGX_PROFILE_FREE((void*)p[i]);
	}
	LIB_ListExportDelete((void**)p);
}

// ------------------------------------------
const void* LIB_ProfileReadBinary(HIGX_PROFILE hProfile, bit32_t index, int* size)
{
	profile_t* s = (profile_t*)hProfile;
	profile_tBinaryElement*sCur=xGetIndexHeader(s, index, INDEX_CUR);
	void* p;
	
	if (sCur==0)
		return 0;

	xxTRACE("LIB_ProfileReadBinary(%d), sCur->size:%d, sCur->pos:%d, sCur->id:%d\n", index, sCur->size, sCur->pos, sCur->id);	

	p=IGX_PROFILE_MALLOC(sCur->size+sizeof(bit32_t));
	LIB_FileSeek(s->fd, s->bin->nOffset+sCur->pos, LIB_kFileSet);
	LIB_FileRead(p, s->fd, (S64)sCur->size);
	
	if (size)
		*size=sCur->size;
	
	return p;
}

// ------------------------------------------
const void* LIB_ProfileReadBinaryById(HIGX_PROFILE hProfile, bit32_t id, void* exid, int* size)
{
	profile_t* s = (profile_t*)hProfile;

	bit32_t index=xSearchId(s, id, exid);
	
	if (index!=-1)
	{
		return LIB_ProfileReadBinary(hProfile, index, size);
	}

	return 0;
}



// ------------------------------------------
Bool LIB_ProfileWriteBinaryByIndex(HIGX_PROFILE hProfile, bit32_t index, void* data, int size, void* extraId)
{
	profile_t* s = (profile_t*)hProfile;
	profile_tBinaryElement*sCur;// = xGetIndexHeader(s, index);
	profile_tBinaryElement*sBefore;// = xGetIndexHeader(s, index-1);
	profile_tBinaryElement*sNext;// = xGetIndexHeader(s, index+1);	

	xxTRACE("LIB_ProfileWriteBinaryByIndex(), index:%d, size:%d\n", index, size);

	sCur	= xGetIndexHeader(s, index, INDEX_CUR);
	sBefore = xGetIndexHeader(s, index, INDEX_BEFORE);
	sNext   = xGetIndexHeader(s, index, INDEX_NEXT);

	if (data==0)
	{
		s->bin->bh.nUsedSize-=sCur->size;
		sCur->id=-1;
		sCur->size=0;
		if (s->bin->bh.bIsDeleteAtTime==false)
		{
			xUpdateHeader(s);
			return true;
		}
		xPrbit32_tHeader(s);
	}
	
	if (sBefore==0)
	{
		sCur->pos=0;
		xxTRACE("sBefore is null!!\n");
	}
	else
	{
		sCur->pos=sBefore->pos+sBefore->size;
		xxTRACE("sCur pos is %d!!\n", sCur->pos);
	}

	if (sCur->size==0)
	{
		s->bin->bh.nUsedSize+=size;
	}
	else
	{
		s->bin->bh.nUsedSize+=(size-sCur->size);
	}

	sCur->size=size;
	
	xSetExtraId(s, sCur, extraId);//s, index, extraId);
	
	// append.
	if (sNext==0)
	{
		xxTRACE("LIB_ProfileWriteBinaryByIndex(), sNext is null (append)\n");
		LIB_FileSeek(s->fd, 0, LIB_kFileEnd);
		LIB_FileWrite(s->fd, data, size);
		goto SUCCESS;
	}
	
	if (s->bin->bh.bIsDeleteAtTime==true)
	{
		bit32_t remain;// = s->size-sNext->pos;
		void* pRemain;// = IGX_PROFILE_MALLOC(remain);
NORMAL_WRITE:
		remain = s->size-sNext->pos;

		// append.
		if (sNext->pos!=0)
		{
			pRemain = IGX_PROFILE_MALLOC(remain);
			if (pRemain==0)
			{
				return false;
			}
		}
		else
		{
			remain=0;
			pRemain=0;
		}

		xxTRACE("LIB_ProfileWriteBinaryByIndex(), NORMAL_WRITE, remain:%d\n", remain);
		
		
		LIB_FileSeek(s->fd, s->bin->nOffset+sNext->pos, LIB_kFileSet);
		//LIB_FileSeek(s->fd, s->bin->nOffset+sNext->pos, LIB_kFileSet);
		if (LIB_FileRead(pRemain, s->fd, (S64)remain)!=(S64)remain)
		{
			return false;
		}
	
		if (data!=0)
		{	
			xxTRACE("LIB_ProfileWriteBinaryByIndex(), write pos:%d, size:%d\n", sCur->pos, sCur->size);
			LIB_FileSeek(s->fd, s->bin->nOffset+sCur->pos, LIB_kFileSet);
			//if (LIB_FileWrite(s->fd, data, size)!=size)
			if (LIB_FileWrite(s->fd, data, (S64)size)!=size)
			{
				return false;
			}
		}
		
		if (remain)
		{
			profile_tBinaryElement* remainTop = xGetIndexHeader(s, index+1, INDEX_CUR);	
			bit32_t pos = LIB_FileSeek(s->fd, 0, LIB_kFileCur);
			bit32_t delta = (pos-s->bin->nOffset)-remainTop->pos;

			if (LIB_FileWrite(s->fd, pRemain, (S64)remain)!=(S64)remain)
			{
				return false;
			}
			xxTRACE("form:%d shifte:%d\n", index+1, delta);
			xShiftHeaderPos(s, index+1, delta);
		}
	}
	else
	{
		bit32_t prevPos=0;
		bit32_t nextPos=sNext->pos;

		xxTRACE("LIB_ProfileWriteBinaryByIndex(), smart write\n");
		if (sBefore!=0)
		{
			prevPos=sBefore->pos+sBefore->size;
		}

		if (nextPos-prevPos>=size)
		{
			xxTRACE("LIB_ProfileWriteBinaryByIndex(), write pos:%d, size:%d\n", sCur->pos, sCur->size);
			LIB_FileSeek(s->fd, s->bin->nOffset+sCur->pos, LIB_kFileSet);
			if (LIB_FileWrite(s->fd, data, size)!=size)
			{
				return false;
			}
			xUpdateHeader(s);
			return true;
		}
		else
		{
			goto NORMAL_WRITE;
		}	
	}

SUCCESS:
	LIB_FileTruncate(s->fd, LIB_FileTell(s->fd));
	xUpdateHeader(s);
	
	xxTRACE("LIB_ProfileWriteBinaryByIndex(), SUCCESS\n");
	//xPrbit32_tHeader(&s);
	return true;
}


// ------------------------------------------
Bool LIB_ProfileWriteBinary(HIGX_PROFILE hProfile, bit32_t id, void* data, int size, void* extraId)
{
	profile_t* s = (profile_t*)hProfile;
	profile_tBinaryElement* sCur=0;
	bit32_t index=xSearchId(s, id, extraId);

	
	if (index==-1)
	{
		index = xGetBlankIndex(s, size);
		if (index==-1)
		{
			return -1;
		}
	}
	
	sCur	= xGetIndexHeader(s, index, INDEX_CUR);
	sCur->id=id;
	return LIB_ProfileWriteBinaryByIndex(hProfile, index, data, size, extraId);
}

// ------------------------------------------
Bool LIB_ProfileWriteBinaryById(HIGX_PROFILE hProfile, bit32_t id, void* exid, void* data, int size)
{
	profile_t* s = (profile_t*)hProfile;
	bit32_t nIndex = xSearchId(s, id, exid);
	profile_tBinaryElement* sCur=0;


	if (nIndex==-1)
	{
		xxTRACE("not found id....\n");

		nIndex=xGetBlankIndex(s, size);
		if (nIndex==-1)
		{
			xxTRACE("element is full....!!\n");
			return false;
		}
	}

	sCur	= xGetIndexHeader(s, nIndex, INDEX_CUR);
	sCur->id=id;

	xxTRACE("index: %d....\n", nIndex);
	if (LIB_ProfileWriteBinaryByIndex(hProfile, nIndex, data, size, exid)==true)
	{
		return nIndex;
	}
	
	return -1;
}


// ------------------------------------------
void LIB_ProfileClose(HIGX_PROFILE hProfile)
{
	profile_t* s = (profile_t*)hProfile;
	
	if (s->p)
	{
		IGX_PROFILE_FREE(s->p);
	}

	if (s->fd)
	{
		//flock(s->fd, LOCK_UN);
		//xclose(s->fd);
		LIB_FileClose(s->fd);
	}

	if (s->bin)
	{
		IGX_PROFILE_FREE(s->bin);
	}
	
	IGX_PROFILE_FREE(s);
}

// ------------------------------------------
Bool yiProfileWriteString(const char* pszTag, const char* value, const char* pszFileName)
{
	HIGX_PROFILE hProfile = igx_profile_open(pszFileName);//, 0);
	Bool ret=false;

	if (hProfile==0)
	{
		xxTRACE("yiProfileWriteString(%s, %s, %s): igx_profile_open() error\n", pszTag, value, pszFileName);
		return false;
	}

	ret = LIB_ProfileWriteString(hProfile, pszTag, value);
	LIB_ProfileClose(hProfile);
	return ret;
/*
	void* p = yiFileLoadLock(pszFileName, &size);

	if (p==0)
	{
		return false;
	}

	xGetLineString(

	return true;*/
}

// ------------------------------------------
Bool yiProfileWriteInt(const char* pszTag, bit32_t value, const char* pszFileName)
{
	HIGX_PROFILE hProfile = igx_profile_open(pszFileName);//, 0);//false, 0, 0);
	Bool ret=false;

	if (hProfile==0)
		return false;

	ret = LIB_ProfileWriteInt(hProfile, pszTag, value);
	LIB_ProfileClose(hProfile);
	return ret;
}

// ------------------------------------------
Bool yiProfileReadString(const char* pszTag, char* value, const char* pszFileName)
{
	HIGX_PROFILE hProfile = igx_profile_open(pszFileName);//, 0);//false, 0, 0);
	Bool ret=false;

	if (hProfile==0)
		return false;

	ret = LIB_ProfileReadString(hProfile, pszTag, value);
	LIB_ProfileClose(hProfile);
	return ret;
}

// ------------------------------------------
bit32_t yiProfileReadInt(const char* pszTag, int defaultValue, const char* pszFileName)
{
	bit32_t ret=0;
	HIGX_PROFILE hProfile = igx_profile_open(pszFileName);//, 0);//false, 0, 0);

	if (hProfile==0)
		return false;

	ret = LIB_ProfileReadInt(hProfile, pszTag, defaultValue);

	LIB_ProfileClose(hProfile);
	return ret;
}

#ifdef __UNIT_TEST__

bit32_t main(int argc, char** argv)
{
	HIGX_PROFILE hProfile = igx_profile_open("StartupInfo.inf.txt");
	bit32_t n, i;
	char** p;
	char value[32];
	Bool ret = LIB_ProfileReadStringEx(hProfile, "Debug", "debug", value);

	printf("ret=%d, [Debug] Debug=%s\n", ret, value);

#if 0
	LIB_ProfileWriteIntEx(hProfile, "Thread", "count", 1000);
	LIB_ProfileWriteStringEx(hProfile, "Thread", "use", "false");
	LIB_ProfileWriteIntEx(hProfile, "Network", "KeepAliveTime", 30000);
	LIB_ProfileWriteIntEx(hProfile, "Network", "ConnectTimeout", 0);
	LIB_ProfileClose(hProfile);

	printf("write... ok\n");

	hProfile = igx_profile_open("a.inf");
	LIB_ProfileReadIntEx(hProfile, "Network", "KeepAliveTime", 	&n);

	printf("KeepAliveTime:%d\n", n);
	LIB_ProfileReadIntEx(hProfile, "Network", "ConnectTimeout", 	&n);
	printf("ConnectTimeout:%d\n", n);

	p = (char**)LIB_ProfileGetEnumKeysEx(hProfile, "Thread", &n);

	for (i=0; i<n; i++)
		printf("enum Key(%d):%s\n", i, p[i]);

	LIB_ProfileReleaseEnumKeys(p, n);
	LIB_ProfileClose(hProfile);
#endif
}

#endif
