#include "yi.h"
#include "yistd.h"
#include "yiProfile.h"

// ------------------------------------------
#define xxTRACE printf

//#define xxTRACE(...) 

#define INDEX_CUR 0
#define INDEX_BEFORE 1
#define INDEX_NEXT 2

// ------------------------------------------
typedef struct {
	Bool bIsDeleteAtTime;
	int nMaxElementCnt;
	int nExtraIdSize;
	int nTotalSizeLimitted;
	int nUsedSize;
} sProfileBinaryHeader;

// ------------------------------------------
typedef struct {
	int pos;
	int id;
	int size;
} sProfileBinaryElement;

typedef struct {
	int nOffset;
	int nUsedSize;
	sProfileBinaryHeader bh;
} sProfileBinary;

// ------------------------------------------
typedef struct {
	int fd;
	int  size;
	char* p;
	sProfileBinary* bin;
} sProfile;


#ifdef __POSIX__
#define xopen open
#define xlseek lseek
#define xread read
#define xwrite write 
#define xclose close
#define xtruncate ftruncate
#endif


// ------------------------------------------
syipDetailForBinaryMode* yipSetDefaultParam(syipDetailForBinaryMode* mode)
{
	mode->nElementCount=0;
	mode->nExtraIdSize=0;
	mode->nTotalSizeLimitted=0;
	mode->bIsDeleteAtTime=False;
	return mode;
}

// ------------------------------------------
static int xSearchId(sProfile*s, int id, void* eid)
{
	const int nExtraIdSize = s->bin->bh.nExtraIdSize;
	const int nElementSize=sizeof(sProfileBinaryElement)+nExtraIdSize;
	char* pIndex=(char*)s->p;

	int i=0;

	xxTRACE("xSearchId() extraIdSize:%d, %d, %s\n", nExtraIdSize, id, eid);

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

	if (nExtraIdSize==sizeof(int))
	{
		for (i=0; i<s->bin->bh.nMaxElementCnt; i++)
		{
			sProfileBinaryElement* el = (sProfileBinaryElement*)pIndex;

			if (el->size!=0 &&
				el->id==id)
			{
				int* k = (int*)&el[1];
				if (*k==*((int*)eid))
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
				xxTRACE("xSearchId() it's same (%s) index:%d\n", eid, i);
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
static int xGetBlankIndex(sProfile*s, int size)
{
	const int nElementSize=sizeof(sProfileBinaryElement)+s->bin->bh.nExtraIdSize;
	int i=0;
	int offset;
	int normalIndex=-1;
	char* pIndex=(char*)s->p;
	sProfileBinaryElement* before=0;
	sProfileBinaryElement* next=0;


	if (s->bin->bh.nTotalSizeLimitted!=0 &&
		(size+s->bin->bh.nUsedSize>s->bin->bh.nTotalSizeLimitted))
	{
		xxTRACE("size is full \n");
		return -1;
	}

	if (s->bin->bh.bIsDeleteAtTime==True)
	{
		int i=0;
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
			int prevpos=before?(before->pos+before->size):0;
			int nextpos=next?(next->pos):0;
			
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
static int xReloadFile(sProfile* s)
{
	int nReads=0;
	int size = xlseek(s->fd, 0, SEEK_END)-xlseek(s->fd, 0, SEEK_SET);


	if (s->p)
		yifree(s->p);

	if (size==0)
	{
		s->p=0;
		s->size=0;
		return 0;
	}

	if (size>sizeof(int) &&
		xread(s->fd, &nReads, sizeof(int))==sizeof(int))
	{
		if (nReads==0 ||
			nReads==1)
		{
			yifree(s->p);
			s->p=0;
			xlseek(s->fd, 0, SEEK_SET);
			return -2; // it is binary format.
		}
	}

	xlseek(s->fd, 0, SEEK_SET);

	s->size=size;
	s->p = (char*)yialloc(size+1);

	if ((nReads=xread(s->fd, s->p, size))!=size)
	{
		xxTRACE("xread %d:%d..... error\n", size, nReads);
		yifree(s->p);
		s->p=0;
		return -1; // error.
	}
	s->p[size]=0;
	return size;
}

// ------------------------------------------
static void xPrintHeader(sProfile*s)//, int index, int size, int next, void* id)
{
	int elementSize = sizeof(sProfileBinaryElement)+s->bin->bh.nExtraIdSize;
	int i=0;
	int nStartOffset=0;
	char* pIndex=(char*)s->p;

	if (pIndex==0)
		return ;

	xxTRACE("xPrintHeader() cnt:%d, limited:%d, used:%d, deleteflag:%d\n", 
		s->bin->bh.nMaxElementCnt, s->bin->bh.nTotalSizeLimitted, s->bin->bh.nUsedSize, s->bin->bh.bIsDeleteAtTime);

	for (i=0; i<s->bin->bh.nMaxElementCnt; i++)
	{
		sProfileBinaryElement* sx = (sProfileBinaryElement*)&pIndex[nStartOffset];

		if (sx->size!=0)
		{
			xxTRACE("xPrintHeader() index:%d, size:%d, id:%d, pos:%d, exid:%s\n", i, sx->size, sx->id, sx->pos, &sx[1]);
		}
		nStartOffset+=elementSize;
	}
}

// ------------------------------------------
static Bool xReadHeader(sProfile* s)
{
	int headerSize; 
	void* p;
	xlseek(s->fd, 0, SEEK_SET);


	if (xread(s->fd, &s->bin->bh, sizeof(s->bin->bh))!=sizeof(s->bin->bh))
	{
		yifree(p);
		return False;
	}
	headerSize = s->bin->bh.nMaxElementCnt*(s->bin->bh.nExtraIdSize+sizeof(sProfileBinaryElement));
	p = yialloci(headerSize);
	xxTRACE("xReadHeader.... headerSize:%d\n", headerSize);
	if (xread(s->fd, p, headerSize)!=headerSize)//sizeof(s->bin->bh))
	{
		yifree(p);
		return False;
	}
	s->p=p;	
	s->size = xlseek(s->fd, 0, SEEK_END)-headerSize-sizeof(sProfileBinaryHeader);
	s->bin->nOffset = headerSize+sizeof(sProfileBinaryHeader);

	xxTRACE("xReadHeader.... bodysize:%d offset:%d SUCCESS\n", s->size, s->bin->nOffset);
	xPrintHeader(s);
	return True;
}

// ------------------------------------------
static Bool xCreateHeader(sProfile* s)
{
	int headerSize = s->bin->bh.nMaxElementCnt*(s->bin->bh.nExtraIdSize+sizeof(sProfileBinaryElement));
	void* p = yialloci(headerSize);
	xlseek(s->fd, 0, SEEK_SET);

	xxTRACE("xCreateHeader.... enter\n");

	if (xwrite(s->fd, &s->bin->bh, sizeof(s->bin->bh))!=sizeof(s->bin->bh))
	{
		yifree(p);
		return False;
	}
	if (xwrite(s->fd, &p, headerSize)!=headerSize)//sizeof(s->bin->bh))
	{
		yifree(p);
		return False;
	}

	s->p=p;	
	s->size = xlseek(s->fd, 0, SEEK_END)-headerSize-sizeof(sProfileBinaryHeader);
	s->bin->nOffset=headerSize+sizeof(sProfileBinaryHeader);
	
	return True;
}



// ------------------------------------------
static Bool xUpdateHeader(sProfile* s)
{
	int headerSize = s->bin->bh.nMaxElementCnt*(s->bin->bh.nExtraIdSize+sizeof(sProfileBinaryElement));
	//xlseek(s->fd, sizeof(sProfileBinaryHeader), SEEK_SET);
	xlseek(s->fd, 0, SEEK_SET);

	if (xwrite(s->fd, &s->bin->bh, sizeof(sProfileBinaryHeader))!=sizeof(sProfileBinaryHeader))//sizeof(s->bin->bh))
	{
		yifree(s->p);
		return False;
	}

	xxTRACE("xUpdateHeader() size %d\n", headerSize);

	if (xwrite(s->fd, s->p, headerSize)!=headerSize)//sizeof(s->bin->bh))
	{
		xxTRACE("header write error....\n");
		yifree(s->p);
		return False;
	}
	xPrintHeader(s);

	return True;
}
//
// ------------------------------------------
static Bool xShiftHeaderPos(sProfile* s, int from, int value)
{
	//int headerSize = s->bin->bh.nMaxElementCnt*(s->bin->bh.nExtraIdSize+sizeof(sProfileBinaryElement));
	int elementSize = sizeof(sProfileBinaryElement)+s->bin->bh.nExtraIdSize;
	int nStartOffset=elementSize*from;
	char* pIndex=(char*)s->p;
	int i=0;

	for (i=from; i<s->bin->bh.nMaxElementCnt; i++)
	{
		sProfileBinaryElement* sx = (sProfileBinaryElement*)&pIndex[nStartOffset];
		sx->pos+=value;
		nStartOffset+=elementSize;
	}
	xPrintHeader(s);

	return True;
}

// ------------------------------------------
static Bool xSetHeader(sProfile*s, int index, int size, int next, void* id)
{
	int n = s->bin->bh.nExtraIdSize+sizeof(sProfileBinaryElement);
	char* pIndex = (char*)s->p;
	sProfileBinaryElement* eh = (sProfileBinaryElement* )&pIndex[n*index];

	//eg->index=index;
	eh->size=size;

	if (s->bin->bh.nExtraIdSize==0)
	{
		return True;
	}

	if (s->bin->bh.nExtraIdSize==sizeof(int))
	{
		int* pInt = (int*)&eh[1];
		*pInt = *((int*)id);
		return True;
	}
	memcpy(&eh[1], id, s->bin->bh.nExtraIdSize);
	return True;
}

// ------------------------------------------
sProfileBinaryElement* xGetIndexHeader(sProfile* s, int index, int act)//INDEX_CUR)
{
	int elementSize = sizeof(sProfileBinaryElement)+s->bin->bh.nExtraIdSize;
	char* pIndex = (char*)s->p;

	xxTRACE("xGetIndexHeader(%d), %d\n", index, act);

	if (act==INDEX_CUR)
	{
		char* pIndex = (char*)s->p;
		return (sProfileBinaryElement*)&pIndex[elementSize*index];
	}
	else if (act==INDEX_BEFORE)
	{
		int i=0;
		int nStartOffset;

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
		int i=0;
		int nStartOffset;

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
HandleProfile yipOpen(const char* pszFileName, syipDetailForBinaryMode* detail)
{
	sProfile s = {0, };
	int nFileSize;
	int fd = xopen(pszFileName, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH); // 755?

	if (fd<0)
		return 0;

	flock(fd, LOCK_EX);
	s.fd=fd;
	s.bin=detail==0?0:yialloci(sizeof(sProfileBinary));

	//s.size = nFileSize = xlseek(fd, 0, SEEK_END);
	nFileSize = xlseek(fd, 0, SEEK_END);
OPEN_FOR_BINARY:
	if (s.bin!=0)
	{
		if (nFileSize>sizeof(sProfileBinaryHeader))
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
		xPrintHeader(&s);
	}
	else
	{
		int res=0;
		if ((res = xReloadFile(&s))==-1)
		{
			// case size is zero.
			xclose(fd);
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
Bool yipReadString(HandleProfile hProfile, const char* key, char* value)
{
	sProfile* s = (sProfile*)hProfile;
	char* pIndex=s->p;

	if (s->bin!=0 ||
		s->p==0)
	{
		return False;
	}

	xxTRACE("yipReadString: Search(%s, %s)\n", key, value);
RETRY_SEARCH:
	while(pIndex=strstr(pIndex, key))
	{
		if (pIndex==s->p)
			break;	
		
		if (pIndex[-1]=='\n')
		{
			break;
		}
		//xxTRACE("yipReadString: Search(%s, %s) Hitted but ???? %s\n", key, value, pIndex);
		pIndex++;
	}
	
	if (pIndex==0)
	{
		return False;
	}
	
	{
		char* pNextLineTop=0;
		char* pLine = yistrGetLine(pIndex, 0, &pNextLineTop);
		int   n;
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

		pKey = yistrTrim(pKeyAndValue[0], False);

		if (n==2)
		{
			pValue = yistrTrim(pKeyAndValue[1], False);
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

	return True;	
}

// ------------------------------------------
int yipReadInt(HandleProfile hProfile, const char* key, int nDefaultNum)
{
	char buffer[256];
	int ret;

	if (yipReadString(hProfile, key, buffer)==False)
	{
		return False;
	}

	if (yiatoi(buffer, &ret)==False)
	{
		return nDefaultNum;
	}
	return ret;
}

// ------------------------------------------
Bool yipWriteInt(HandleProfile hProfile, const char* key, int n)
{
	char buffer[256];
	sprintf(buffer, "%d", n);
	return yipWriteString(hProfile, key, buffer);
}

// ------------------------------------------
Bool yipWriteString(HandleProfile hProfile, const char* key, const char* value)
{
	sProfile* s = (sProfile*)hProfile;
	char* pIndex=s->p;
	char* pFoundKeyLine=0;
	char* pNextLine=0;

	xxTRACE("yipWriteString called (%s, %s)\n", key, value);

	if (s->bin!=0)
	{
		return False;
	}

	if (s->p==0)
	{
		xxTRACE("s->p is zero, so goto append\n");
		goto CASE_APPEND;
	}

RETRY_SEARCH:
	xxTRACE("yipWriteString() before loop\n");
	while(pIndex=strstr(pIndex, key))
	{
		if (pIndex==s->p)
			break;	
		
		if (pIndex[-1]=='\n')
		{
			break;
		}
		pIndex++;
	}
	xxTRACE("yipWriteString() after loop\n");
	
	if (pIndex==0)
	{
		xxTRACE("Key is not found.... goto append\n");
		goto CASE_APPEND;
	}
	
	{
		char* pNextLineTop=0;
		char* pLine = yistrGetLine(pIndex, 0, &pNextLineTop);
		int   n;
		char** pKeyAndValue = yistrSplit(pLine, "=", &n);
		char* pKey=0, *pValue=0;

		xxTRACE("yipWriteString() Trace.....1\n");
		yistrFree(pLine);
		
		if (n==0)
		{
			yistrSplitFree(pKeyAndValue);
			pIndex=pNextLineTop;
			xxTRACE("yipWriteString() split is zero\n");
			goto RETRY_SEARCH;
		}
		xxTRACE("yipWriteString() split(%s, %s)\n", pKeyAndValue[0], pKeyAndValue[1]);

		pKey = yistrTrim(pKeyAndValue[0], False);

		if (strcmp(pKey, key)!=0)
		{
			yistrFree(pKey);
			yistrFree(pValue);
			yistrSplitFree(pKeyAndValue);
			pIndex=pNextLineTop;
			goto RETRY_SEARCH;
		}
		xxTRACE("yipWriteString() same key found! (%s, %s)\n", pKeyAndValue[0], pKeyAndValue[1]);

		yistrFree(pKey);
		yistrFree(pValue);
		yistrSplitFree(pKeyAndValue);

		pFoundKeyLine=pIndex;
		pNextLine = strstr(pFoundKeyLine, "\n");

		xlseek(s->fd, 0, SEEK_SET);
		if (pFoundKeyLine!=s->p)
		{
			xwrite(s->fd, s->p, pFoundKeyLine-s->p);
		}

		if (pNextLine)
		{
			pNextLine++;
			xxTRACE("yipWriteString() pNextLine:%s\n", pNextLine);//pKeyAndValue[0], pKeyAndValue[1]);
		}
	}

	{
		char buffer[1024];
		int len;
		int upLen=pFoundKeyLine-s->p;
	
		//if (pFoundKeyLine!=s->p)
		//{	
		//	sprintf(buffer, "\n%s=%s", key, value);
		//}
		//else
		//{
			sprintf(buffer, "%s=%s", key, value);
		//}

		len = strlen(buffer);
		xwrite(s->fd, buffer, len);

		if (pNextLine)
		{
			xwrite(s->fd, "\n", 1);
			xwrite(s->fd, pNextLine, s->size - (pNextLine-s->p));
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

		xlseek(s->fd, 0, SEEK_END);
		xwrite(s->fd, buffer, strlen(buffer));
	}

TRUNCATE:
	{
		int pos = xlseek(s->fd, 0, SEEK_CUR);
		xtruncate(s->fd, pos);
	}	
	xReloadFile(s);
	return True;	
}

// ------------------------------------------
const char** yipGetEnumKeys(HandleProfile hProfile, int* cnt)
{
	return 0;
}

// ------------------------------------------
const char** yipReleaseEnumKeys(HandleProfile hProfile, int* cnt)
{
	return 0;
}

// ------------------------------------------
const void* yipReadBinary(HandleProfile hProfile, int index, int* size)
{
	sProfile* s = (sProfile*)hProfile;
	sProfileBinaryElement*sCur=xGetIndexHeader(s, index, INDEX_CUR);
	void* p;
	
	if (sCur==0)
		return 0;

	xxTRACE("yipReadBinary(%d), sCur->size:%d, sCur->pos:%d, sCur->id:%d\n", index, sCur->size, sCur->pos, sCur->id);	

	p=yialloc(sCur->size+sizeof(int));
	xlseek(s->fd, s->bin->nOffset+sCur->pos, SEEK_SET);
	xread(s->fd, p, sCur->size);
	
	if (size)
		*size=sCur->size;
	
	return p;
}

// ------------------------------------------
const void* yipReadBinaryById(HandleProfile hProfile, int id, void* exid, int* size)
{
	sProfile* s = (sProfile*)hProfile;

	int index=xSearchId(s, id, exid);
	
	if (index!=-1)
	{
		return yipReadBinary(hProfile, index, size);
	}

	return 0;
}



// ------------------------------------------
Bool yipWriteBinaryByIndex(HandleProfile hProfile, int index, void* data, int size, void* extraId)
{
	sProfile* s = (sProfile*)hProfile;
	sProfileBinaryElement*sCur;// = xGetIndexHeader(s, index);
	sProfileBinaryElement*sBefore;// = xGetIndexHeader(s, index-1);
	sProfileBinaryElement*sNext;// = xGetIndexHeader(s, index+1);	

	xxTRACE("yipWriteBinaryByIndex(), index:%d, size:%d\n", index, size);

	sCur	= xGetIndexHeader(s, index, INDEX_CUR);
	sBefore = xGetIndexHeader(s, index, INDEX_BEFORE);
	sNext   = xGetIndexHeader(s, index, INDEX_NEXT);

	if (data==0)
	{
		s->bin->bh.nUsedSize-=sCur->size;
		sCur->id=-1;
		sCur->size=0;
		if (s->bin->bh.bIsDeleteAtTime==False)
		{
			xUpdateHeader(s);
			return True;
		}
		xPrintHeader(s);
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
		xxTRACE("yipWriteBinaryByIndex(), sNext is null (append)\n");
		xlseek(s->fd, 0, SEEK_END);
		xwrite(s->fd, data, size);
		goto SUCCESS;
	}
	
	if (s->bin->bh.bIsDeleteAtTime==True)
	{
		int remain;// = s->size-sNext->pos;
		void* pRemain;// = yialloc(remain);
NORMAL_WRITE:
		remain = s->size-sNext->pos;

		// append.
		if (sNext->pos!=0)
		{
			pRemain = yialloc(remain);
			if (pRemain==0)
			{
				return False;
			}
		}
		else
		{
			remain=0;
			pRemain=0;
		}

		xxTRACE("yipWriteBinaryByIndex(), NORMAL_WRITE, remain:%d\n", remain);
		
		
		xlseek(s->fd, s->bin->nOffset+sNext->pos, SEEK_SET);
		if (xread(s->fd, pRemain, remain)!=remain)
		{
			return False;
		}
	
		if (data!=0)
		{	
			xxTRACE("yipWriteBinaryByIndex(), write pos:%d, size:%d\n", sCur->pos, sCur->size);
			xlseek(s->fd, s->bin->nOffset+sCur->pos, SEEK_SET);
			if (xwrite(s->fd, data, size)!=size)
			{
				return False;
			}
		}
		
		if (remain)
		{
			sProfileBinaryElement* remainTop = xGetIndexHeader(s, index+1, INDEX_CUR);	
			int pos = xlseek(s->fd, 0, SEEK_CUR);
			int delta = (pos-s->bin->nOffset)-remainTop->pos;

			if (xwrite(s->fd, pRemain, remain)!=remain)
			{
				return False;
			}
			xxTRACE("form:%d shifte:%d\n", index+1, delta);
			xShiftHeaderPos(s, index+1, delta);
		}
	}
	else
	{
		int prevPos=0;
		int nextPos=sNext->pos;

		xxTRACE("yipWriteBinaryByIndex(), smart write\n");
		if (sBefore!=0)
		{
			prevPos=sBefore->pos+sBefore->size;
		}

		if (nextPos-prevPos>=size)
		{
			xxTRACE("yipWriteBinaryByIndex(), write pos:%d, size:%d\n", sCur->pos, sCur->size);
			xlseek(s->fd, s->bin->nOffset+sCur->pos, SEEK_SET);
			if (xwrite(s->fd, data, size)!=size)
			{
				return False;
			}
			xUpdateHeader(s);
			return True;
		}
		else
		{
			goto NORMAL_WRITE;
		}	
	}

SUCCESS:
	xtruncate(s->fd, xlseek(s->fd, 0, SEEK_CUR));
	xUpdateHeader(s);
	
	xxTRACE("yipWriteBinaryByIndex(), SUCCESS\n");
	//xPrintHeader(&s);
	return True;
}


// ------------------------------------------
Bool yipWriteBinary(HandleProfile hProfile, int id, void* data, int size, void* extraId)
{
	sProfile* s = (sProfile*)hProfile;
	sProfileBinaryElement* sCur=0;
	int index=xSearchId(s, id, extraId);

	int nNextIndex=0;
	int nStartIndex=0;
	
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
	return yipWriteBinaryByIndex(hProfile, index, data, size, extraId);
}

// ------------------------------------------
Bool yipWriteBinaryById(HandleProfile hProfile, int id, void* exid, void* data, int size)
{
	sProfile* s = (sProfile*)hProfile;
	int nIndex = xSearchId(s, id, exid);
	sProfileBinaryElement* sCur=0;


	if (nIndex==-1)
	{
		xxTRACE("not found id....\n");

		nIndex=xGetBlankIndex(s, size);
		if (nIndex==-1)
		{
			xxTRACE("element is full....!!\n");
			return False;
		}
	}

	sCur	= xGetIndexHeader(s, nIndex, INDEX_CUR);
	sCur->id=id;

	xxTRACE("index: %d....\n", nIndex);
	if (yipWriteBinaryByIndex(hProfile, nIndex, data, size, exid)==True)
	{
		return nIndex;
	}
	
	return -1;
}


// ------------------------------------------
void yipClose(HandleProfile hProfile)
{
	sProfile* s = (sProfile*)hProfile;
	
	if (s->p)
	{
		yifree(s->p);
	}

	if (s->fd)
	{
		flock(s->fd, LOCK_UN);
		xclose(s->fd);
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
	HandleProfile hProfile = yipOpen(pszFileName, 0);
	Bool ret=False;

	if (hProfile==0)
	{
		xxTRACE("yiProfileWriteString(%s, %s, %s): yipOpen() error\n", pszTag, value, pszFileName);
		return False;
	}

	ret = yipWriteString(hProfile, pszTag, value);
	yipClose(hProfile);
	return ret;
/*
	void* p = yiFileLoadLock(pszFileName, &size);

	if (p==0)
	{
		return False;
	}

	xGetLineString(

	return True;*/
}

// ------------------------------------------
Bool yiProfileWriteInt(const char* pszTag, int value, const char* pszFileName)
{
	HandleProfile hProfile = yipOpen(pszFileName, 0);//False, 0, 0);
	Bool ret=False;

	if (hProfile==0)
		return False;

	ret = yipWriteInt(hProfile, pszTag, value);
	yipClose(hProfile);
	return ret;
}

// ------------------------------------------
Bool yiProfileReadString(const char* pszTag, char* value, const char* pszFileName)
{
	HandleProfile hProfile = yipOpen(pszFileName, 0);//False, 0, 0);
	Bool ret=False;

	if (hProfile==0)
		return False;

	ret = yipReadString(hProfile, pszTag, value);
	yipClose(hProfile);
	return ret;
}

// ------------------------------------------
int yiProfileReadInt(const char* pszTag, int defaultValue, const char* pszFileName)
{
	int ret=0;
	HandleProfile hProfile = yipOpen(pszFileName, 0);//False, 0, 0);

	if (hProfile==0)
		return False;

	ret = yipReadInt(hProfile, pszTag, defaultValue);

	yipClose(hProfile);
	return ret;
}

// ------------------------------------------
void yipGetDetail(HandleProfile hProfile, syipDetailForBinaryMode* mode)
{
	sProfile* s = (sProfile*)hProfile;
	mode->bIsDeleteAtTime=s->bin->bh.bIsDeleteAtTime;
	mode->nElementCount=s->bin->bh.nMaxElementCnt;
	mode->nExtraIdSize=s->bin->bh.nExtraIdSize;
	mode->nTotalSizeLimitted=s->bin->bh.nTotalSizeLimitted;
}

// ------------------------------------------
void yipUpdateDetail(HandleProfile hProfile, syipDetailForBinaryMode* mode)
{
	sProfile* s = (sProfile*)hProfile;
	s->bin->bh.bIsDeleteAtTime = mode->bIsDeleteAtTime;
	//s->bin->bh.nMaxElementCnt = mode->nElementCount;
	//mode->nExtraIdSize=s->bin->bh.nExtraIdSize;
	s->bin->bh.nTotalSizeLimitted=mode->nTotalSizeLimitted;
	
}

