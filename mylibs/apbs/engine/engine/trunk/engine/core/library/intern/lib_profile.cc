#include <core/library/lib_profile.h>

#include <core/memory_allocation/guarded_alloc.h>
#include <core/library/lib_file_operations.h>
#include <core/library/lib_list.h>
#include <core/library/lib_string.h>

// ------------------------------------------
#define xxTRACE printf

using namespace  adk;

//#define xxTRACE(...) 

#define MAX_SECTION_LEN 64
#define INDEX_CUR 0
#define INDEX_BEFORE 1
#define INDEX_NEXT 2

#define yistrAlloc(a) ADK_Malloc(a, "LIB_Profile")
#define yistrFree ADK_Free
#define yifree ADK_Free
#define yialloc(a) ADK_Malloc(a, "LIB_Profile")
#define yirealloc realloc

#define FASTCALL 

// ------------------------------------------
typedef struct {
  Bool bIsDeleteAtTime;
  S32 nMaxElementCnt;
  S32 nExtraIdSize;
  S32 nTotalSizeLimitted;
  S32 nUsedSize;
} sProfileBinaryHeader;

// ------------------------------------------
typedef struct {
  S32 pos;
  S32 id;
  S32 size;
} sProfileBinaryElement;

typedef struct {
  S32 nOffset;
  S32 nUsedSize;
  sProfileBinaryHeader bh;
} sProfileBinary;

// ------------------------------------------
typedef struct {
    LIB_TFileObject* fd;
#if 0
  S32 fd;
#endif
  S32  size;
  char* p;
  sProfileBinary* bin;
} sProfile;

#define __POSIX__

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
static char* FASTCALL yistrnheap(const char* str, S32 size)
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
static char* FASTCALL yistrheap(const char* str)
{
  return yistrnheap(str, 0);
}


// ------------------------------
static void* FASTCALL yitoheap(void* buffer, S32 n)
{
  void* p = yialloc(n);
  memcpy(p, buffer, n);
  return p;
}

// ------------------------------
static Bool yiatoi(const char* buffer, S32* n)
{
  S32 cnt=0;
  char temp[16];
  char* _buff=(char*)buffer;
  S32 len=0;

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
static char* yialloci(S32 size)
{
  char* p = (char*)yialloc(size);

  if (p)
    return (char*)memset(p, 0, size);
  return 0;
}

// ------------------------------
static char* FASTCALL yistrSub(const char* str, S32 start, int end)
{
  char* ret = (char*)yialloc(end-start+1);
  memcpy(ret, &str[start], (end-start));
  ret[end-start]=0;

  return ret;
}

#define xIS_SPACE(a) (a==' ')
#define xIS_TRIMCHARZ(a) (a==' '||a=='\t'||a=='\r'||a=='\n')

// ------------------------------
// space, tab
static char* FASTCALL yistrTrim(const char* str, Bool bSpaceOnly)
{
  S32 i=0, start=0, end=strlen(str);
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
  return yistrSub(str, start, end);

FULL_TRIM:
  for (i=0; i<end; i++)
  {
    if (xIS_TRIMCHARZ(pIndex[i]))
    {
      start++;
      continue;
    }
    break;  
  }

  for (i=end-1; i>=0; i--)
  {
    if (xIS_TRIMCHARZ(pIndex[i]))
    {
      end--;
      continue;
    }
    break;
  }

  return yistrSub(str, start, end);
}



// ------------------------------
static char* FASTCALL yistrGetLine(const char* str, S32 n, char** next)
{
  char* pIndex = (char*)str;
  S32 len=0;

  while(*pIndex&&*pIndex!='\n') pIndex++;

  len=(S32)(pIndex-str);

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
static void yistrSplitFree(char** str)
{
  S32 cnt=0;
  if (str==0)
    return ;

  while(str[cnt])
    yistrFree(str[cnt++]);
  
  yifree(str);
}

// ------------------------------
static char** FASTCALL yistrSplit(const char* str, const char* spliter, S32* _cnt)
{
  char* pIndex=(char*)str;
  char* pLastIndex=0;
  char* pNext;
  S32 start=0;
  S32 cnt=0;
  S32 nSplitCnt=0;
  S32 limitCnt=16;
  S32 spliterlength=(S32)strlen(spliter);

  char** ret = (char**)yialloci(sizeof(char*)*limitCnt);

  xxTRACE("yistrSplit before loop\n");
  while((pNext=strstr(pIndex,spliter))!=0)
  {
    S32 len=0;
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
      S32 i=0;
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


// ------------------------------------------
sLIB_ProfileDetailForBinaryMode* LIB_ProfileSetDefaultParam(sLIB_ProfileDetailForBinaryMode* mode)
{
  mode->nElementCount=0;
  mode->nExtraIdSize=0;
  mode->nTotalSizeLimitted=0;
  mode->bIsDeleteAtTime=false;
  return mode;
}

// ------------------------------------------
static S32 xSearchId(sProfile*s, int id, void* eid)
{
  const S32 nExtraIdSize = s->bin->bh.nExtraIdSize;
  const S32 nElementSize=sizeof(sProfileBinaryElement)+nExtraIdSize;
  char* pIndex=(char*)s->p;

  S32 i=0;

  xxTRACE("xSearchId() extraIdSize:%d, %d, %s\n", nExtraIdSize, id, (char*)eid);

  if (eid==0 ||
    nExtraIdSize==0)
  {
    for (i=0; i<s->bin->bh.nMaxElementCnt; i++)
    {
      sProfileBinaryElement* el = (sProfileBinaryElement*)pIndex;

      if (el->size!=0 &&
        el->id==id)
      {
        return i;
      }
  
      pIndex+=nElementSize;
    }
    return -1;
  }

  if (nExtraIdSize==sizeof(S32))
  {
    for (i=0; i<s->bin->bh.nMaxElementCnt; i++)
    {
      sProfileBinaryElement* el = (sProfileBinaryElement*)pIndex;

      if (el->size!=0 &&
        el->id==id)
      {
        S32* k = (int*)&el[1];
        if (*k==*((S32*)eid))
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
    sProfileBinaryElement* el = (sProfileBinaryElement*)pIndex;
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
static void xSetExtraId(sProfile*s, sProfileBinaryElement* el, void* id)
{
  void* dest = (void*)&el[1];
  memcpy(dest, id, s->bin->bh.nExtraIdSize);
}

// ------------------------------------------
static S32 xGetBlankIndex(sProfile*s, int size)
{
  const S32 nElementSize=sizeof(sProfileBinaryElement)+s->bin->bh.nExtraIdSize;
  S32 i=0;
  S32 normalIndex=-1;
  char* pIndex=(char*)s->p;
  sProfileBinaryElement* before=0;
  sProfileBinaryElement* next=0;


  if (s->bin->bh.nTotalSizeLimitted!=0 &&
    (size+s->bin->bh.nUsedSize>s->bin->bh.nTotalSizeLimitted))
  {
    xxTRACE("size is full \n");
    return -1;
  }

  if (s->bin->bh.bIsDeleteAtTime==true)
  {
    S32 i=0;
    for (i=0; i<s->bin->bh.nMaxElementCnt; i++)
    {
      sProfileBinaryElement* el = (sProfileBinaryElement*)pIndex;

      if (el->size==0)
        return i;

      pIndex+=nElementSize;
    }

    xxTRACE("element is full !\n");
    return -1;
  }

  for (i=0; i<s->bin->bh.nMaxElementCnt; i++)
  {
    sProfileBinaryElement* el = (sProfileBinaryElement*)pIndex;

    if (el->size==0)
    {
      S32 prevpos=before?(before->pos+before->size):0;
      S32 nextpos=next?(next->pos):0;
      
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
    before = (sProfileBinaryElement* )pIndex;
    pIndex+=nElementSize;

    next = i<(s->bin->bh.nMaxElementCnt-1) ? (sProfileBinaryElement* )(pIndex+nElementSize):0;
  }

  xxTRACE("element is full !\n");
  return normalIndex;
}

// ------------------------------------------
static S32 xReloadFile(sProfile* s)
{
  S32 nReads=0;
  S32 size = LIB_FileSize(s->fd);//LIB_FileSeek(s->fd, 0, SEEK_END)-LIB_FileSeek(s->fd, 0, LIB_kFileSet);

  if (s->p)
    yifree(s->p);

  if (size==0)
  {
    s->p=0;
    s->size=0;
    return 0;
  }
  xxTRACE("s->fd:%x, size:%d\n", (intptr_t)s->fd, size);

//  if (size>sizeof(S32) &&
//    xread(s->fd, &nReads, sizeof(S32))==sizeof(S32))
#if 0
  if (size>sizeof(S32) &&
    LIB_FileRead(&nReads, s->fd, sizeof(S32))==sizeof(S32))
  {
    if (nReads==0 ||
      nReads==1)
    {
      yifree(s->p);
      s->p=0;
      LIB_FileSeek(s->fd, 0, LIB_kFileSet);
      return -2; // it is binary format.
    }
  }
#endif
  LIB_FileSeek(s->fd, 0, LIB_kFileSet);
  //LIB_FileSeek(s->fd, 0, LIB_kFileSet);

  s->size=size;
  s->p = (char*)yialloc(size+1);

  xxTRACE("before xread ..... \n");
  if ((nReads=LIB_FileRead(s->p, s->fd, (S64)size))!=size)
  {
    xxTRACE("after xread success..... \n");
    yifree(s->p);
    s->p=0;
    return -1; // error.
  }
  xxTRACE("xread success..... \n");
  s->p[size]=0;
  return size;
}

// ------------------------------------------
static void xPrS32Header(sProfile*s)//, int index, int size, int next, void* id)
{
  S32 elementSize = sizeof(sProfileBinaryElement)+s->bin->bh.nExtraIdSize;
  S32 i=0;
  S32 nStartOffset=0;
  char* pIndex=(char*)s->p;

  if (pIndex==0)
    return ;

  xxTRACE("xPrS32Header() cnt:%d, limited:%d, used:%d, deleteflag:%d\n", 
    s->bin->bh.nMaxElementCnt, s->bin->bh.nTotalSizeLimitted, s->bin->bh.nUsedSize, s->bin->bh.bIsDeleteAtTime);

  for (i=0; i<s->bin->bh.nMaxElementCnt; i++)
  {
    sProfileBinaryElement* sx = (sProfileBinaryElement*)&pIndex[nStartOffset];

    if (sx->size!=0)
    {
      xxTRACE("xPrS32Header() index:%d, size:%d, id:%d, pos:%d, exid:%s\n", i, sx->size, sx->id, sx->pos, (char*)&sx[1]);
    }
    nStartOffset+=elementSize;
  }
}

// ------------------------------------------
static Bool xReadHeader(sProfile* s)
{
  S32 headerSize; 
  void* p;

  LIB_FileSeek(s->fd, 0, LIB_kFileSet);
  //LIB_FileSeek(s->fd, 0, LIB_kFileSet);


  if (LIB_FileRead(&s->bin->bh, s->fd, sizeof(s->bin->bh))!=sizeof(s->bin->bh))
  {
    return false;
  }
  headerSize = s->bin->bh.nMaxElementCnt*(s->bin->bh.nExtraIdSize+sizeof(sProfileBinaryElement));
  p = yialloci(headerSize);
  xxTRACE("xReadHeader.... headerSize:%d\n", headerSize);
  if (LIB_FileRead(p, s->fd, headerSize)!=headerSize)//sizeof(s->bin->bh))
  {
    yifree(p);
    return false;
  }
  s->p=(char*)p;  
  s->size = LIB_FileSize(s->fd)/*LIB_FileSeek(s->fd, 0, SEEK_END)*/-headerSize-sizeof(sProfileBinaryHeader);
  s->bin->nOffset = headerSize+sizeof(sProfileBinaryHeader);
  LIB_FileSeek(s->fd, 0, LIB_kFileEnd);

  xxTRACE("xReadHeader.... bodysize:%d offset:%d SUCCESS\n", s->size, s->bin->nOffset);
  xPrS32Header(s);
  return true;
}

// ------------------------------------------
static Bool xCreateHeader(sProfile* s)
{
  S32 headerSize = s->bin->bh.nMaxElementCnt*(s->bin->bh.nExtraIdSize+sizeof(sProfileBinaryElement));
  void* p = yialloci(headerSize);
  LIB_FileSeek(s->fd, 0, LIB_kFileSet);
  //LIB_FileSeek(s->fd, 0, LIB_kFileSet);

  xxTRACE("xCreateHeader.... enter\n");

  if (LIB_FileWrite(s->fd, &s->bin->bh, (S64)sizeof(s->bin->bh))!=true/*sizeof(s->bin->bh)*/)
  {
    yifree(p);
    return false;
  }
  if (LIB_FileWrite(s->fd, &p, (S64)headerSize)!=headerSize)//sizeof(s->bin->bh))
  {
    yifree(p);
    return false;
  }

  s->p=(char*)p;
  s->size = (U32)LIB_FileSize(s->fd)/*LIB_FileSeek(s->fd, 0, SEEK_END)*/-headerSize-sizeof(sProfileBinaryHeader);
  s->bin->nOffset=headerSize+sizeof(sProfileBinaryHeader);
  LIB_FileSeek(s->fd, 0, LIB_kFileEnd);
  
  return true;
}



// ------------------------------------------
static Bool xUpdateHeader(sProfile* s)
{
  S32 headerSize = s->bin->bh.nMaxElementCnt*(s->bin->bh.nExtraIdSize+sizeof(sProfileBinaryElement));
  //LIB_FileSeek(s->fd, sizeof(sProfileBinaryHeader), LIB_kFileSet);
  LIB_FileSeek(s->fd, 0, LIB_kFileSet);

  if (LIB_FileWrite(s->fd, &s->bin->bh, (S64)sizeof(sProfileBinaryHeader))!=true/*sizeof(sProfileBinaryHeader)*/)//sizeof(s->bin->bh))
  {
    yifree(s->p);
    return false;
  }

  xxTRACE("xUpdateHeader() size %d\n", headerSize);

  if (LIB_FileWrite(s->fd, s->p, (S64)headerSize)!=headerSize)//sizeof(s->bin->bh))
  {
    xxTRACE("header write error....\n");
    yifree(s->p);
    return false;
  }
  xPrS32Header(s);

  return true;
}
//
// ------------------------------------------
static Bool xShiftHeaderPos(sProfile* s, S32 from, int value)
{
  //S32 headerSize = s->bin->bh.nMaxElementCnt*(s->bin->bh.nExtraIdSize+sizeof(sProfileBinaryElement));
  S32 elementSize = sizeof(sProfileBinaryElement)+s->bin->bh.nExtraIdSize;
  S32 nStartOffset=elementSize*from;
  char* pIndex=(char*)s->p;
  S32 i=0;

  for (i=from; i<s->bin->bh.nMaxElementCnt; i++)
  {
    sProfileBinaryElement* sx = (sProfileBinaryElement*)&pIndex[nStartOffset];
    sx->pos+=value;
    nStartOffset+=elementSize;
  }
  xPrS32Header(s);

  return true;
}

// ------------------------------------------
static Bool xSetHeader(sProfile*s, S32 index, int size, int next, void* id)
{
  S32 n = s->bin->bh.nExtraIdSize+sizeof(sProfileBinaryElement);
  char* pIndex = (char*)s->p;
  sProfileBinaryElement* eh = (sProfileBinaryElement* )&pIndex[n*index];

  //eg->index=index;
  eh->size=size;

  if (s->bin->bh.nExtraIdSize==0)
  {
    return true;
  }

  if (s->bin->bh.nExtraIdSize==sizeof(S32))
  {
    S32* pInt = (int*)&eh[1];
    *pInt = *((S32*)id);
    return true;
  }
  memcpy(&eh[1], id, s->bin->bh.nExtraIdSize);
  return true;
}

// ------------------------------------------
sProfileBinaryElement* xGetIndexHeader(sProfile* s, S32 index, int act)//INDEX_CUR)
{
  S32 elementSize = sizeof(sProfileBinaryElement)+s->bin->bh.nExtraIdSize;
  char* pIndex = (char*)s->p;

  xxTRACE("xGetIndexHeader(%d), %d\n", index, act);

  if (act==INDEX_CUR)
  {
    char* pIndex = (char*)s->p;
    return (sProfileBinaryElement*)&pIndex[elementSize*index];
  }
  else if (act==INDEX_BEFORE)
  {
    S32 i=0;
    S32 nStartOffset;

    if (index-1<0)
    {
      return 0;
    }

    nStartOffset=elementSize*(index-1);
    for (i=index-1; i>=0; i--)
    {
      sProfileBinaryElement* sx = (sProfileBinaryElement*)&pIndex[nStartOffset];
      if (sx->size!=0)
      {
        return sx;
      }
      nStartOffset-=elementSize;
    }
  }
  else
  {
    S32 i=0;
    S32 nStartOffset;

    if ((index+1)>=s->bin->bh.nMaxElementCnt)
    {
      return 0;
    }

    nStartOffset=elementSize*(index+1);
    for (i=index+1; i<s->bin->bh.nMaxElementCnt; i++)
    {
      sProfileBinaryElement* sx = (sProfileBinaryElement*)&pIndex[nStartOffset];//elementSize*i];
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
HandleProfile LIB_ProfileOpen(const char* pszFileName, sLIB_ProfileDetailForBinaryMode* detail)
#endif 
HandleProfile LIB_ProfileOpen(const char* pszFileName)
{
  sProfile s = {0, };
  S32 nFileSize;
#if 0
  S32 fd = xopen(pszFileName, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH); // 755?
#endif
  LIB_TFileObject* fd = LIB_FileOpen(pszFileName, LIB_kFileTypeRead);

  if (fd==0)
  {
    
    LIB_TFileObject* fd = LIB_FileOpen(pszFileName, LIB_kFileTypeWrite);
    if (fd==0)
    {
      return 0;
    }

    LIB_FileClose(fd);
    fd = LIB_FileOpen(pszFileName, LIB_kFileTypeRead);
  }

  s.fd=fd;
#if 0
  flock(fd, LOCK_EX);
  s.bin=detail==0?0:yialloci(sizeof(sProfileBinary));
#else
  s.bin=0;
#endif
  xxTRACE("trace......1\n");

  //s.size = nFileSize = LIB_FileSeek(fd, 0, SEEK_END);
  s.size=nFileSize=(S32)LIB_FileSize(fd);
  LIB_FileSeek(fd, 0, LIB_kFileEnd);
OPEN_FOR_BINARY:
  if (s.bin!=0)
  {
#if 0
    if (nFileSize>sizeof(sProfileBinaryHeader))
    {
      xReadHeader(&s);//fd, &s.header);
    }
    else
    {
      s.bin->bh.nMaxElementCnt  = detail->nElementCount;
      s.bin->bh.nExtraIdSize     = detail->nExtraIdSize;
      s.bin->bh.nTotalSizeLimitted  = detail->nTotalSizeLimitted;
      s.bin->bh.bIsDeleteAtTime  = detail->bIsDeleteAtTime;
      xCreateHeader(&s);//fd, &s.header);
    }
    xPrS32Header(&s);
#endif
  }
  else
  {
    S32 res=0;
    if ((res = xReloadFile(&s))==-1)
    {
      // case size is zero.
            LIB_FileClose(fd);
      //xclose(fd);
      return 0;
    }
    else if (res==-2)
    {
      s.bin = (sProfileBinary*)yialloc(sizeof(sProfileBinary));
      goto OPEN_FOR_BINARY;
    }
  }
  return (HandleProfile)yitoheap(&s, sizeof(s));
  
}

// ------------------------------------------
Bool LIB_ProfileReadStringEx(HandleProfile hProfile, const char* section, const char* key, char* value)
{
  sProfile* s = (sProfile*)hProfile;
  char* pIndex=s->p;
  char _section[64];
  char* pSectionStart, *pSectionEnd;
  S32 sectionLen;
  xxTRACE("LIB_ProfileReadString: Search(%x, %s, %s)\n", (intptr_t)hProfile, section, key);

  if (s->bin!=0 ||
    s->p==0)
  {
    xxTRACE("file is not found\n");
    return false;
  }

  sprintf(_section, "[%s]", section);
  sectionLen = (S32)strlen(_section);

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
    S32   n;
    char** pKeyAndValue;// = yistrSplit(pLine, "=", &n);
    char* pKey, *pValue, *strTemp;

    // it is comment.
    if (pLine[0]=='#' || pLine[0]=='\t' || pLine[0]==' ')
    {
      yistrFree(pLine);
      pIndex=pNextLineTop;
      goto RETRY_SEARCH;
    }

    pKeyAndValue = yistrSplit(pLine, "=", &n);
    yistrFree(pLine);

    if (n!=2)
    {
      yistrSplitFree(pKeyAndValue);
      pIndex=pNextLineTop;
      goto RETRY_SEARCH;
    }

    // key=value
    pKey = yistrTrim(pKeyAndValue[0], false);

    if (LIB_strcmpi(pKey, key)!=0)
    {
      yistrFree(pKey);
      yistrSplitFree(pKeyAndValue);
      pIndex=pNextLineTop;
      goto RETRY_SEARCH;
    }


    // case
    // key = value # comment ...... 
    strTemp=strstr(pKeyAndValue[1], " #");
    if (strTemp)
      *strTemp=0; 

    // key = value  # comment ...... 
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

    yistrFree(pKey);
    yistrFree(pValue);
    yistrSplitFree(pKeyAndValue);
  }

  return true;  
}

// ------------------------------------------
Bool LIB_ProfileReadString(HandleProfile hProfile, const char* key, char* value)
{
  sProfile* s = (sProfile*)hProfile;
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
    S32   n;
    char** pKeyAndValue;// = yistrSplit(pLine, "=", &n);
    char* pKey, *pValue;

    // it is comment.
    if (pLine[0]=='#')
    {
      yistrFree(pLine);
      pIndex=pNextLineTop;
      goto RETRY_SEARCH;
    }

    pKeyAndValue = yistrSplit(pLine, "=", &n);
    yistrFree(pLine);

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
      yistrFree(pKey);
      yistrFree(pValue);
      yistrSplitFree(pKeyAndValue);
      pIndex=pNextLineTop;
      goto RETRY_SEARCH;
    }


    if (n==1)
      value[0]=0;
    else
      strcpy(value, pValue);

    yistrFree(pKey);
    yistrFree(pValue);
    yistrSplitFree(pKeyAndValue);
  }

  return true;  
}

// ------------------------------------------
S32 LIB_ProfileReadInt(HandleProfile hProfile, const char* key, int nDefaultNum)
{
  char buffer[256];
  S32 ret;

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
Bool LIB_ProfileReadIntEx(HandleProfile hProfile, const char* section, const char* key, S32* val)
{
  char buffer[256];
  S32 ret;

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
Bool LIB_ProfileWriteInt(HandleProfile hProfile, const char* key, S32 n)
{
  char buffer[256];
  sprintf(buffer, "%d", n);
  return LIB_ProfileWriteString(hProfile, key, buffer);
}

// ------------------------------------------
Bool LIB_ProfileWriteIntEx(HandleProfile hProfile, const char* section, const char* key, S32 n)
{
  char buffer[256];
  sprintf(buffer, "%d", n);
  return LIB_ProfileWriteStringEx(hProfile, section, key, buffer);
}



// ------------------------------------------
Bool LIB_ProfileWriteStringEx(HandleProfile hProfile, const char* section, const char* key, const char* value)
{
  sProfile* s = (sProfile*)hProfile;
  char* pIndex=s->p;
  char* pFoundKeyLine=0;
  char* pNextLine=0;
  char _section[MAX_SECTION_LEN+2];
  char* pSectionStart=pIndex, *pSectionEnd=0;
  S32 sectionLen ;
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
  sectionLen = (S32)strlen(_section);

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
    S32   n;
    char** pKeyAndValue = yistrSplit(pLine, "=", &n);
    char* pKey=0, *pValue=0;

    xxTRACE("LIB_ProfileWriteString() Trace.....1\n");
    yistrFree(pLine);
    
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
      yistrFree(pKey);
      yistrFree(pValue);
      yistrSplitFree(pKeyAndValue);
      pIndex=pNextLineTop;
      goto RETRY_SEARCH;
    }
    xxTRACE("LIB_ProfileWriteString() same key found! (%s, %s)\n", pKeyAndValue[0], pKeyAndValue[1]);

    yistrFree(pKey);
    yistrFree(pValue);
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
    S32 len;
    //S32 upLen=(S32)pFoundKeyLine-s->p;
  
    //if (pFoundKeyLine!=s->p)
    //{  
    //  sprintf(buffer, "\n%s=%s", key, value);
    //}
    //else
    //{
      sprintf(buffer, "%s=%s", key, value);
    //}

    len = (S32)strlen(buffer);
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
      S32 end = pSectionEnd-s->p;
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
    //S32 pos = LIB_FileSeek(s->fd, 0, LIB_kFileCur);
    LIB_FileTruncate(s->fd, LIB_FileTell(s->fd));
    //xtruncate(s->fd, pos);
  }  
  xReloadFile(s);
  return true;  
}



// ------------------------------------------
Bool LIB_ProfileWriteString(HandleProfile hProfile, const char* key, const char* value)
{
  sProfile* s = (sProfile*)hProfile;
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
    S32   n;
    char** pKeyAndValue = yistrSplit(pLine, "=", &n);
    char* pKey=0;

    xxTRACE("LIB_ProfileWriteString() Trace.....1\n");
    yistrFree(pLine);
    
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
      yistrFree(pKey);
      //yistrFree(pValue);
      yistrSplitFree(pKeyAndValue);
      pIndex=pNextLineTop;
      goto RETRY_SEARCH;
    }
    xxTRACE("LIB_ProfileWriteString() same key found! (%s, %s)\n", pKeyAndValue[0], pKeyAndValue[1]);

    yistrFree(pKey);
    //yistrFree(pValue);
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
    S32 len;

  
    //if (pFoundKeyLine!=s->p)
    //{  
    //  sprintf(buffer, "\n%s=%s", key, value);
    //}
    //else
    //{
      sprintf(buffer, "%s=%s", key, value);
    //}

    len = (S32)strlen(buffer);
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
const char** LIB_ProfileEnumKeysEx(HandleProfile hProfile, const char* section, S32* _cnt)
{
  sProfile* s = (sProfile*)hProfile;
  char* pIndex=s->p;
  char* pSectionStart;
  char* pSectionEnd=0;
  S32 sectionLen;
  char _section[MAX_SECTION_LEN+2];
  char** pRet;
  HandleList hList;

  sprintf(_section, "[%s]", section);

  if (s==0 ||
      section==0)
    return 0;

  sectionLen=(S32)strlen(_section);
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
    S32   n;
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
      yistrFree(pLine);
      pIndex=pNextLineTop;
      continue;
    }

    yistrFree(pLine);
    
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
      S32 n = (((cnt&0xF0)>>4)+1)*0x10;// *sizeof(char*);
      S32 size = (n+1)*sizeof(char*);
      char** temp  = (char**)yistrAlloc((size+1)*sizeof(char*));
      
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
const char** LIB_ProfileGetEnumKeys(HandleProfile hProfile, S32* cnt)
{
  return 0;
}

// ------------------------------------------
void LIB_ProfileReleaseEnumKeys(const char** p, S32 cnt)
{
  S32 i;

  if (p==0)
    return ;

  for (i=0; i<cnt; i++)
  {
    yistrFree((void*)p[i]);
  }
  LIB_ListExportDelete((void**)p);
}

// ------------------------------------------
const void* LIB_ProfileReadBinary(HandleProfile hProfile, S32 index, int* size)
{
  sProfile* s = (sProfile*)hProfile;
  sProfileBinaryElement*sCur=xGetIndexHeader(s, index, INDEX_CUR);
  void* p;
  
  if (sCur==0)
    return 0;

  xxTRACE("LIB_ProfileReadBinary(%d), sCur->size:%d, sCur->pos:%d, sCur->id:%d\n", index, sCur->size, sCur->pos, sCur->id);  

  p=yialloc(sCur->size+sizeof(S32));
  LIB_FileSeek(s->fd, s->bin->nOffset+sCur->pos, LIB_kFileSet);
  LIB_FileRead(p, s->fd, (S64)sCur->size);
  
  if (size)
    *size=sCur->size;
  
  return p;
}

// ------------------------------------------
const void* LIB_ProfileReadBinaryById(HandleProfile hProfile, S32 id, void* exid, int* size)
{
  sProfile* s = (sProfile*)hProfile;

  S32 index=xSearchId(s, id, exid);
  
  if (index!=-1)
  {
    return LIB_ProfileReadBinary(hProfile, index, size);
  }

  return 0;
}



// ------------------------------------------
Bool LIB_ProfileWriteBinaryByIndex(HandleProfile hProfile, S32 index, void* data, int size, void* extraId)
{
  sProfile* s = (sProfile*)hProfile;
  sProfileBinaryElement*sCur;// = xGetIndexHeader(s, index);
  sProfileBinaryElement*sBefore;// = xGetIndexHeader(s, index-1);
  sProfileBinaryElement*sNext;// = xGetIndexHeader(s, index+1);  

  xxTRACE("LIB_ProfileWriteBinaryByIndex(), index:%d, size:%d\n", index, size);

  sCur  = xGetIndexHeader(s, index, INDEX_CUR);
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
    xPrS32Header(s);
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
    S32 remain;// = s->size-sNext->pos;
    void* pRemain;// = yialloc(remain);
NORMAL_WRITE:
    remain = s->size-sNext->pos;

    // append.
    if (sNext->pos!=0)
    {
      pRemain = yialloc(remain);
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
      sProfileBinaryElement* remainTop = xGetIndexHeader(s, index+1, INDEX_CUR);  
      S32 pos = LIB_FileSeek(s->fd, 0, LIB_kFileCur);
      S32 delta = (pos-s->bin->nOffset)-remainTop->pos;

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
    S32 prevPos=0;
    S32 nextPos=sNext->pos;

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
  //xPrS32Header(&s);
  return true;
}


// ------------------------------------------
Bool LIB_ProfileWriteBinary(HandleProfile hProfile, S32 id, void* data, int size, void* extraId)
{
  sProfile* s = (sProfile*)hProfile;
  sProfileBinaryElement* sCur=0;
  S32 index=xSearchId(s, id, extraId);

  
  if (index==-1)
  {
    index = xGetBlankIndex(s, size);
    if (index==-1)
    {
      return -1;
    }
  }
  
  sCur  = xGetIndexHeader(s, index, INDEX_CUR);
  sCur->id=id;
  return LIB_ProfileWriteBinaryByIndex(hProfile, index, data, size, extraId);
}

// ------------------------------------------
Bool LIB_ProfileWriteBinaryById(HandleProfile hProfile, S32 id, void* exid, void* data, int size)
{
  sProfile* s = (sProfile*)hProfile;
  S32 nIndex = xSearchId(s, id, exid);
  sProfileBinaryElement* sCur=0;


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

  sCur  = xGetIndexHeader(s, nIndex, INDEX_CUR);
  sCur->id=id;

  xxTRACE("index: %d....\n", nIndex);
  if (LIB_ProfileWriteBinaryByIndex(hProfile, nIndex, data, size, exid)==true)
  {
    return nIndex;
  }
  
  return -1;
}


// ------------------------------------------
void LIB_ProfileClose(HandleProfile hProfile)
{
  sProfile* s = (sProfile*)hProfile;
  
  if (s->p)
  {
    yifree(s->p);
  }

  if (s->fd)
  {
    //flock(s->fd, LOCK_UN);
    //xclose(s->fd);
    LIB_FileClose(s->fd);
  }

  if (s->bin)
  {
    yifree(s->bin);
  }
  
  yifree(s);
}

// ------------------------------------------
Bool yiProfileWriteString(const char* pszTag, const char* value, const char* pszFileName)
{
  HandleProfile hProfile = LIB_ProfileOpen(pszFileName);//, 0);
  Bool ret=false;

  if (hProfile==0)
  {
    xxTRACE("yiProfileWriteString(%s, %s, %s): LIB_ProfileOpen() error\n", pszTag, value, pszFileName);
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
Bool yiProfileWriteInt(const char* pszTag, S32 value, const char* pszFileName)
{
  HandleProfile hProfile = LIB_ProfileOpen(pszFileName);//, 0);//false, 0, 0);
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
  HandleProfile hProfile = LIB_ProfileOpen(pszFileName);//, 0);//false, 0, 0);
  Bool ret=false;

  if (hProfile==0)
    return false;

  ret = LIB_ProfileReadString(hProfile, pszTag, value);
  LIB_ProfileClose(hProfile);
  return ret;
}

// ------------------------------------------
S32 yiProfileReadInt(const char* pszTag, int defaultValue, const char* pszFileName)
{
  S32 ret=0;
  HandleProfile hProfile = LIB_ProfileOpen(pszFileName);//, 0);//false, 0, 0);

  if (hProfile==0)
    return false;

  ret = LIB_ProfileReadInt(hProfile, pszTag, defaultValue);

  LIB_ProfileClose(hProfile);
  return ret;
}

// ------------------------------------------
void LIB_ProfileGetDetail(HandleProfile hProfile, sLIB_ProfileDetailForBinaryMode* mode)
{
  sProfile* s = (sProfile*)hProfile;
  mode->bIsDeleteAtTime=s->bin->bh.bIsDeleteAtTime;
  mode->nElementCount=s->bin->bh.nMaxElementCnt;
  mode->nExtraIdSize=s->bin->bh.nExtraIdSize;
  mode->nTotalSizeLimitted=s->bin->bh.nTotalSizeLimitted;
}

// ------------------------------------------
void LIB_ProfileUpdateDetail(HandleProfile hProfile, sLIB_ProfileDetailForBinaryMode* mode)
{
  sProfile* s = (sProfile*)hProfile;
  s->bin->bh.bIsDeleteAtTime = mode->bIsDeleteAtTime;
  //s->bin->bh.nMaxElementCnt = mode->nElementCount;
  //mode->nExtraIdSize=s->bin->bh.nExtraIdSize;
  s->bin->bh.nTotalSizeLimitted=mode->nTotalSizeLimitted;
  
}

#ifdef __UNIT_TEST__

S32 main(int argc, char** argv)
{
  HandleProfile hProfile = LIB_ProfileOpen("StartupInfo.inf.txt");
  S32 n, i;
  char** p;
  char value[32];
  Bool ret = LIB_ProfileReadStringEx(hProfile, "Debug", "Debug", value);

  printf("ret=%d, [Debug] Debug=%s\n", ret, value);

#if 0
  LIB_ProfileWriteIntEx(hProfile, "Thread", "count", 1000);
  LIB_ProfileWriteStringEx(hProfile, "Thread", "use", "false");
  LIB_ProfileWriteIntEx(hProfile, "Network", "KeepAliveTime", 30000);
  LIB_ProfileWriteIntEx(hProfile, "Network", "ConnectTimeout", 0);
  LIB_ProfileClose(hProfile);

  printf("write... ok\n");

  hProfile = LIB_ProfileOpen("a.inf");
  LIB_ProfileReadIntEx(hProfile, "Network", "KeepAliveTime",   &n);

  printf("KeepAliveTime:%d\n", n);
  LIB_ProfileReadIntEx(hProfile, "Network", "ConnectTimeout",   &n);
  printf("ConnectTimeout:%d\n", n);

  p = (char**)LIB_ProfileGetEnumKeysEx(hProfile, "Thread", &n);

  for (i=0; i<n; i++)
    printf("enum Key(%d):%s\n", i, p[i]);

  LIB_ProfileReleaseEnumKeys(p, n);
  LIB_ProfileClose(hProfile);
#endif
}

#endif
