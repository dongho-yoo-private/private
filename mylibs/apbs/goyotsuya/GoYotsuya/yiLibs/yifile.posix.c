#include "yistd.h"
#include "yifile.h"

#ifdef __POSIX__
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthxread.h>
#include <unistd.h>
#include <errno.h>
#include <sys/file.h>
#include <string.h>

#define xopen open

#else
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <share.h>

static IntPtr xopen(const char* pszFileName, int flags, int mode)
{
	//int* handle = (int*)yialloc(sizeof(int)*2);
	HANDLE fd;

	//if (_sopen_s(&fd, pszFileName, flags, _SH_DENYWR, mode)!=0)
	//{
	//	return -1;
	//}

	int xmode = (flags&_O_RDONLY) ? GENERIC_READ:0;

	xmode |= GENERIC_READ;
	xmode |= (flags&_O_WRONLY) ? GENERIC_WRITE:0;


	fd = CreateFileA(pszFileName, xmode, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	//wsprintf(mutex_name, "%s.mutex", pszFileName);
	//handle[1]=(int)fd;
	//handle[0]=(int)CreateMutex(0, FALSE, mutex_name);

	return (IntPtr)(fd==INVALID_HANDLE_VALUE)?-1:(IntPtr)fd;//(int)&handle[1];
}

static void xclose(int fd)
{
	//int* handle = (int*)fd;
	
	//close(handle[0]);
	CloseHandle((HANDLE)fd);
	//yifree((void*)&handle[-1]);
}

static int xread(int fd, void* buffer, int n)
{
	DWORD nRead;
	if (ReadFile((HANDLE)fd, buffer, n, &nRead, 0)==FALSE)
	{
		return -1;
	}

	return nRead;
}

static int xwrite(int fd, const void* buffer, int n)
{
	DWORD nWrite;
	if (WriteFile((HANDLE)fd, buffer, n, &nWrite, 0)==FALSE)
	{
		return -1;
	}

	return nWrite;
}

#define LOCK_UN 0
#define LOCK_EX 1

static int flock(int fd, int operation)
{
	//int* handle = (int*)fd;

	//if (operation==LOCK_UN)
	//{
	//	ReleaseMutex((HANDLE)handle[-1]);
	//	return 0;
	//}

	//if (WaitForSingleObject((HANDLE)handle[-1], INFINITE)==WAIT_ABANDONED)
	//{
	//	return -1;
	//}
	//return 0;

	unsigned int n = GetFileSize((HANDLE)fd, 0);

	//if (operation==LOCK_UN)
	//{
	//	UnlockFileEx((HANDLE)fd, 0, n, 0, 0);
	//	return 0;
	//}

	if (operation==LOCK_EX)
	{
		OVERLAPPED ov;
		ZeroMemory(&ov, sizeof(OVERLAPPED));
		LockFileEx((HANDLE)fd, LOCKFILE_EXCLUSIVE_LOCK, 0, n, 0, &ov);
	}
	else
	{
		OVERLAPPED ov;
		ZeroMemory(&ov, sizeof(OVERLAPPED));
		UnlockFileEx((HANDLE)fd, 0, n, 0, &ov);
	}

	return 0;
}



static long xlseek(int fd, long pos, int from)
{
	int _from=FILE_BEGIN;

	if (from==SEEK_CUR)
		_from=FILE_CURRENT;
	else if (from==SEEK_END)
		_from=FILE_END;

	return (long)SetFilePointer((HANDLE)fd, pos, 0, _from);
	//int* handle = (int*)fd;
	//return _lseek(handle[0], pos, from);
}

#endif



extern int __buffer_size_once;


// --------------------------------------------------------------------
/*static int xwrite_continuos(int fd, ...)
{
	void** pArgv = (void**)&fd;
	pArgv++;

	int i=0;
	int sum=0;
	while(pArgv[i])
	{
		if (xwrite(fd, pArgv[i], pArgv[i+1])!=pArgv[i+1])
		{
			return -1;
		}
		sum+=pArgv[i+1];
		i++;i++;
	}

	return sum;
}*/

static __inline Int32 __get_check_sum__(const void* buffer, int size, int check_sum_interval)
{
	unsigned char* p = (unsigned char*)buffer;
	unsigned int check_sum=0;

	while(p<(((unsigned char*)buffer)+size))
	{
		check_sum+= (unsigned int)*p;
		p+=check_sum_interval;
	}

	return check_sum;
}

// --------------------------------------------------------------------
Bool yiFileRead(const char* pszFileName, void* buffer, int nIndex, int nSize)
{
	int n=0;
	int sum;
	int fd = xopen(pszFileName, O_RDONLY, MODE_666);
	char* pBuffer;
	
	//("%s xopen %d\n", pszFileName, fd);

	if (fd<0)
	{
		return False;
	}

	xlseek(fd, nIndex, SEEK_SET);	

	pBuffer = (char*)buffer;
	sum=0;
	while(nSize)
	{
		int nReadOnce = __buffer_size_once < nSize ? __buffer_size_once:nSize;
		int res = xread(fd, &pBuffer[sum], nReadOnce); 

		if (res<0)
		{
			return False;
		}
		else if (res==0)
		{
			return False;
		}
		
		nSize -= res;
		sum+=nReadOnce;
	}
	
	xclose(fd);

	return True;
}

//
// --------------------------------------------------------------------
void* yiFileLoad(const char* pszFileName, int* nSize)
{
	void* p=0;
	int n = yiFileSize(pszFileName);
	
	if (n<=0)
	{
		//("yiLoadFile(%s) Failure... size is %d\n", pszFileName, n);
		return 0;
	}

	if (nSize!=0)
	{
		*nSize=n;
	}

	p = yialloc(n+1);


	if (yiFileRead(pszFileName, p, 0, n)==True)
	{
		//("yiLoadFile(%s) Success... size is %d\n", pszFileName, n);
		return p;
	}

	//yistrn(p, n);
	
	return 0;
}

// --------------------------------------------------------------------
void yiFileUnload(void* p)
{
	yifree(p);
}

#include "yiencodex.h"
// --------------------------------------------------------------------
void* yiFileSafeLoad(const char* pszFileName, int* nSize)
{
	int size;
	void* p = yiFileLoad(pszFileName, &size);
	sSafeFileFormat* pFormat = (sSafeFileFormat*)p;
	int randKey = pFormat->reserved2;
	if (p==0)
		return 0;

	if (pFormat->n!=size)
	{
		yiFileUnload(p);
		*nSize=0;
		//YIERROR("yiFileSafeLoad(%s) Failure invalid size\n", pszFileName);
		return 0;
	}

	yidecodex(&pFormat[1], size-sizeof(sSafeFileFormat), randKey);
	if (pFormat->checksum!=__get_check_sum__(&pFormat[1], size-sizeof(sSafeFileFormat), 4))
	{
		yiFileUnload(p);
		*nSize=0;
		//YIERROR("yiFileSafeLoad(%s) Failure invalid checksum\n", pszFileName);
		return 0;
	}

	if (nSize)
		*nSize = size-sizeof(sSafeFileFormat);
	return (void*)&pFormat[1];
}

// --------------------------------------------------------------------
void yiFileSafeUnload(void* data)
{
	sSafeFileFormat* p = (sSafeFileFormat* )data;
	yiFileUnload(&p[-1]);	
}

#include <time.h>
// --------------------------------------------------------------------
Bool yiFileSafeWrite(const char* pszFileName, void* _buffer, int nSize, Bool bMakeShadow, const char* pszShadowRoot)
{
	char buffer[256];
	sSafeFileFormat* p = yiFileCreateSafeFormat(_buffer, nSize);
	int randKey =rand();
	
	strcpy(buffer, pszFileName);
	strcat(buffer, ".temp");
	rename(pszFileName, buffer);

	yiencodex(p, nSize, randKey);
	p=&p[-1];
	p->reserved2=randKey;
	//p->n = nSize+sizeof(sSafeFileFormat);
	//p->checksum = __get_check_sum__(&p[1], nSize, 4);
	//
	
	if (yiFileWrite(pszFileName, p, 0, nSize+sizeof(sSafeFileFormat))!=True)
	{
		//YIERROR("yiFileWrite(%s) Error!\n", pszFileName);
		rename(buffer, pszFileName);
		yiFileDestroySafeFormat(&p[1]);
		return False;
	}

	if (bMakeShadow==True)
	{
		char buffer[256];
		strcpy(buffer, pszShadowRoot);
		strcat(buffer, pszFileName);
		if (yiFileWrite(buffer, p, 0, nSize+sizeof(sSafeFileFormat))!=True)
		{
			//YIERROR("yiFileWrite(%s) Error!\n", buffer);
			yiFileDestroySafeFormat(&p[1]);
			return False;
		}
	}

	remove(buffer);
	yiFileDestroySafeFormat(&p[1]);
	return True;
}

// --------------------------------------------------------------------
SafeFormat yiFileCreateSafeFormat(void* data, int size)
{
	sSafeFileFormat* p = (sSafeFileFormat*)yialloc(size+sizeof(sSafeFileFormat));
	p->checksum = __get_check_sum__(data, size, 4);
	p->n = size+sizeof(sSafeFileFormat);
	memcpy(&p[1], data, size);

	return &p[1];
}

// --------------------------------------------------------------------
void yiFileDestroySafeFormat(SafeFormat p)
{
	sSafeFileFormat* fmp = (sSafeFileFormat*)p;
	yifree((void*)&fmp[-1]);
}

// --------------------------------------------------------------------
xEXTERN_C Bool yiFileSafeRestore(const char* pszBadFile, const char* pszBackupFile)
{
	int size_s, size_b;
	sSafeFileFormat* pSrc = (sSafeFileFormat*)yiFileLoad(pszBadFile, &size_s);
	sSafeFileFormat* pBackup = (sSafeFileFormat*)yiFileLoad(pszBackupFile,&size_b);
	Bool res;

	if (pSrc->n==size_s &&
		pSrc->checksum==__get_check_sum__(&pSrc[1], pBackup->n+size_b, 4))
	{
		yiFileUnload(pSrc);
		yiFileUnload(pBackup);
		return True;
	}

	if (pBackup->n!=size_b ||
		pBackup->checksum!=__get_check_sum__(&pBackup[1], pBackup->n+size_b, 4))
	{
		yiFileUnload(pSrc);
		yiFileUnload(pBackup);
		return False;
	}
	
	res = yiFileWrite(pszBadFile, pBackup, 0, pBackup->n+sizeof(sSafeFileFormat));
	yiFileUnload(pSrc);
	yiFileUnload(pBackup);

	return res;
}

// --------------------------------------------------------------------
Bool yiFileWrite(const char* pszFileName, void* buffer, int nIndex, int nSize)
{
	int n=0;
	int fd = xopen(pszFileName, O_WRONLY|O_CREAT|O_TRUNC, MODE_777);
	char* pIndex=(char*)buffer;


	//("yiWriteFile(%s) xopen %d\n", pszFileName, fd);
	
	if (fd<0)
	{
		//("yiWriteFile(%s) xopen Failure %d\n", pszFileName, fd);
		return False;
	
	}

	if (flock(fd, LOCK_EX)!=0)
	{
		yiTrace("lock failure....\n");
		return False;
	}

	xlseek(fd, nIndex, SEEK_SET);

	if (nIndex==-1)
		xlseek(fd, 0, SEEK_END);

	while(nSize)
	{
		int nWriteOnce = __buffer_size_once < nSize ? __buffer_size_once:nSize;
		nSize -= xwrite(fd, pIndex, nWriteOnce);
		pIndex=&pIndex[nWriteOnce];
	}

	if (flock(fd, LOCK_UN)!=0)
	{
		yiTrace("unlock failure....\n");
		return False;
	}
	
	xclose(fd);
	//("yiWriteFile(%s) xwrite success %d bytes\n", pszFileName, nSize);

	return True;
}

// --------------------------------------------------------------------
Bool yiFileWriteOnce(const char* pszFileName, void* buffer, int nIndex, int nSize)
{
	int n=0;
	int fd = xopen(pszFileName, O_WRONLY|O_CREAT|O_TRUNC, MODE_777);
	char* pIndex=(char*)buffer;


	//("yiWriteFile(%s) xopen %d\n", pszFileName, fd);
	
	if (fd<0)
	{
		//("yiWriteFile(%s) xopen Failure %d\n", pszFileName, fd);
		return False;
	
	}

	if (flock(fd, LOCK_EX)!=0)
	{
		yiTrace("lock failure....\n");
		return False;
	}

	xlseek(fd, nIndex, SEEK_SET);

	if (nIndex==-1)
		xlseek(fd, 0, SEEK_END);

	while(nSize)
	{
		int nWriteOnce = __buffer_size_once < nSize ? __buffer_size_once:nSize;
		nSize -= xwrite(fd, pIndex, nWriteOnce);
		pIndex=&pIndex[nWriteOnce];
	}

	if (flock(fd, LOCK_UN)!=0)
	{
		yiTrace("unlock failure....\n");
		return False;
	}
	
	SetEndOfFile((HANDLE)fd);
	xclose(fd);
	//("yiWriteFile(%s) xwrite success %d bytes\n", pszFileName, nSize);

	return True;
}

#include <sys/stat.h>

#define S_ISDIR(a) ((a&_S_IFDIR)!=0)
// --------------------------------------------------------------------
int yiFileSize(const char* pszFileName)
{
	struct stat info;

	if (stat(pszFileName, &info)!=-1)
	{
		//if ((info.st_mode&_S_IFDIR))==0)
		if (S_ISDIR(info.st_mode)==False)
		{
			return info.st_size;
		}

	}

	//("yiFileSize(%s) Failure.... errno=%d\n", pszFileName, errno);

	return -1;
}

// --------------------------------------------------------------------
Bool yiIsExist(const char* pszFileName, Bool* pIsDir)
{
	struct stat info;

	if (stat(pszFileName, &info)==0)
	{
		if (pIsDir!=0)
		{
			*pIsDir = S_ISDIR(info.st_mode)?True:False;
		}
		return True;
	}

	return False;
}

// --------------------------------------------------------------------
Bool yiIsDir(const char* pszName)
{
	struct stat info;

	if (stat(pszName, &info)==0)
	{
		if (S_ISDIR(info.st_mode))
		{
			return True;
		}
	}

	return False;
}

#include <stdio.h>

extern int rmdir(const char* name);
extern int remove(const char*);
extern int mkdir(const char*, int);

// --------------------------------------------------------------------
static Bool xRemoveForced(HANDLE hFind, const char* pszDir)
{

	WIN32_FIND_DATA data;
	char szFileName[256];
	
	xTRACE("xRemoveForced(%s)", pszDir);


	if (hFind==0)
	{

		strcpy(szFileName, pszDir);
		if (GetFileAttributesA(pszDir)&FILE_ATTRIBUTE_DIRECTORY)
		{
			strcat(szFileName, "\\*.*");
		}
		xTRACE("xRemoveForced(%s) FindFirst %s", pszDir, szFileName);
		hFind = FindFirstFile(szFileName, &data);
	}

	if (hFind==INVALID_HANDLE_VALUE)
		return False;

	wsprintf(szFileName, "%s\\%s", pszDir, data.cFileName);
	xTRACE("xRemoveForced(%s) sub is %s", pszDir, szFileName);

	if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		if (data.cFileName[0]!='.')
		{
			xRemoveForced(0, szFileName);
			rmdir(szFileName);
		}
	}
	else
	{
		remove(szFileName);
	}

	while(FindNextFile(hFind, &data))
	{
		xTRACE("xRemoveForced(%s) sub is %s", pszDir, data.cFileName);

		wsprintf(szFileName, "%s\\%s", pszDir, data.cFileName);
		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (data.cFileName[0]!='.')
			{
				xRemoveForced(0, szFileName);
				rmdir(szFileName);
			}
		}
		else
		{
			remove(szFileName);
		}
	}

	FindClose(hFind);

	return True;
}

// --------------------------------------------------------------------
Bool yiRemove(const char* pszFileName, Bool bIsForced)
{
	Bool res;

	if (yiIsDir(pszFileName)==True)
	{
		res = rmdir(pszFileName);

		if (res!=0)
		{
			if (bIsForced==True)
			{
				xRemoveForced(0, pszFileName);
				rmdir(pszFileName);
			}
		}
	}
	else
	{
		res = remove(pszFileName);
	}

	return res==0?True:False;
}

// --------------------------------------------------------------------
Bool yiMakeDir(const char* path/*, Bool bIsSubCreate*/)
{
//	if (bIsSubCreate==False)
//	{
		return mkdir(path, MODE_755)==0?True:False;
//	}

	/*char** list = yiStrSplitz(path, "\\", 0);

	if (list==0)
		return False;

	char path[256];
	path[0]=0;
	for (cyiNode* node=list->GetNodeTopFrom(); node; node=node->np)
	{
		char* str = (char*)node->ref;
		strcat(str);
		mkdir(str, MODE_755);
		strcat("\\");

		yifree(str);
	}

	delete list;*/
	return True;
}

#include <string.h>
// --------------------------------------------------------------------
static char* __getprevpath__(char* buffer, const char* path)
{
	int n = strlen(path);

	while(n!=0&&path[n--]!='\\');

	if (n==0)
		return 0;

	memcpy(buffer, path, n+1);
	buffer[n+1]=0;
	return buffer;
}

// --------------------------------------------------------------------
Bool yiMakePathx(const char* path, Bool bPathContainsFile)
{
	char buffer[256];
	Bool bIsDir=True;

	if (yiIsExist(path, &bIsDir)==True)
	{
		if (bIsDir==False && bPathContainsFile==False)
		{
			return False;
		}
		return False;
	}

	if (__getprevpath__(buffer, path)==0)
	{
		return False;
	}

	if (yiMakePath(buffer, False)==True)
	{
		return mkdir(buffer, MODE_755)==0?True:False;
	}

	return True;
}

// --------------------------------------------------------------------
Bool yiMakePath(const char* path, Bool bPathContainsFile)
{
	Bool res = yiMakePathx(path, bPathContainsFile);

	if (bPathContainsFile==False)
	{
		return mkdir(path, MODE_755)==0?True:False;
	}

	return res;
}

//
// --------------------------------------------------------------------
xEXTERN_C Bool yiFileMode(const char* pszFilename, int nMode)
{
	return True;
}

// -------------------------------
typedef struct __syiFileCreditHeader__
{
	Int32 prefix;
	Int32 size;
	Int32 check_sum_interval;
	Int32 check_sum;	
} syiFileCreditHeader;

// -------------------------------
typedef struct __syiFileCreditHeaderRecord__
{
	Int32 prefix;
	Int32 size;
	unsigned short record_size;
	unsigned short check_sum_interval;
	Int32 cnt;
} syiFileCreditHeaderRecord;



// --------------------------------------------------------------------
int yiFileCreditWrite(const char* pszFileName, const void* buffer, int size, int check_sum_interval)
{
	syiFileCreditHeader header;
	int fd;

	header.prefix = FILE_YOTSUYA_PREFIX;
	header.size = size;
	header.check_sum_interval = check_sum_interval;
	
	fd = xopen(pszFileName, O_WRONLY|O_CREAT, MODE_600);

	if (fd<0)
	{
		yiTrace("xopen error (%s)\n", pszFileName);
		return eFE_OPEN;
	}

	flock(fd, LOCK_EX);

	if (check_sum_interval!=0)
	{
		header.check_sum = __get_check_sum__(buffer, size, check_sum_interval);
	}

	if (xwrite(fd, &header, sizeof(header))!=sizeof(header))
	{
		yiTrace("xwrite error (%s)\n", pszFileName);
		goto READWRITE_ERROR;
	}

	if (xwrite(fd, buffer, size)!=size)
	{
		yiTrace("xwrite error (%s)\n", pszFileName);
		goto READWRITE_ERROR;
	}


	//ftruncate(fd, size+sizeof(header));	
	flock(fd, LOCK_UN);
	xclose(fd);

	return eFE_OK;

READWRITE_ERROR:
	flock(fd, LOCK_UN);
	xclose(fd);
	return eFE_READWRITE;
}

// --------------------------------------------------------------------
void* yiFileCreditLoad(const char* pszFileName, int* reason)
{
	syiFileCreditHeader header;
	int fd = xopen(pszFileName, O_RDONLY, MODE_600);
	int size;
	void* p;
	
	if (fd<0)
	{
		if (reason)
			*reason=eFE_OPEN;
		yiTrace("xopen error (%s)\n", pszFileName);
		return 0;
	}

	flock(fd, LOCK_EX);


	if (xread(fd, &header, sizeof(header))!=sizeof(header))
	{
		yiTrace("xread error (%s)\n", pszFileName);
		goto READWRITE_ERROR;
	}

	size = header.size;
	p=yialloc(size);

	if (xread(fd, p, size)!=size)
	{
		yiTrace("xread error (%s)\n", pszFileName);
		goto READWRITE_ERROR;
	}

	{
		syiFileCreditHeader* hd = &header;
		Int32 check_sum;

		if (hd->prefix!=FILE_YOTSUYA_PREFIX)
		{
			yiTrace("invalid format (%s)\n", pszFileName);
			goto INVALID_FORMAT;
		}	

		if (hd->check_sum_interval!=0)
		{		
			check_sum = __get_check_sum__(p, hd->size, hd->check_sum_interval);
		}

		if (hd->check_sum != check_sum)
		{
			yiTrace("invalid checksum(%d!=%d) (%s)\n", hd->check_sum, check_sum, pszFileName);
			goto INVALID_FORMAT;
		}
		
		flock(fd, LOCK_UN);
		xclose(fd);
		if (reason!=0) *reason=size;
		return (void*)p;
	}
	
INVALID_FORMAT:
	flock(fd, LOCK_UN);
	xclose(fd);
	if (reason!=0) *reason=eFE_INVALID_FORMAT;
	return 0;

READWRITE_ERROR:
	flock(fd, LOCK_UN);
	xclose(fd);
	if (reason!=0) *reason=eFE_READWRITE;
	return 0;
}
// --------------------------------------------------------------------
void* yiFileCreditLoadEx(const char* pszFileName, int* reason)
{
	int fd = xopen(pszFileName, O_RDONLY, MODE_600);
	int size;
	void* p;
	
	if (fd<0)
	{
		if (reason)
			*reason=eFE_OPEN;
		yiTrace("xopen error (%s)\n", pszFileName);
		return 0;
	}

	flock(fd, LOCK_EX);
	size = xlseek(fd, 0, SEEK_END)-xlseek(fd, 0, SEEK_SET);
	p=yialloc(size);	

	if (xread(fd, p, size)!=size)
	{
		yiTrace("xread error (%s)\n", pszFileName);
		goto READWRITE_ERROR;
	}

	{
		syiFileCreditHeader* hd = (syiFileCreditHeader* )p;
		Int32 check_sum;

		if (hd->prefix!=FILE_YOTSUYA_PREFIX)
		{
			yiTrace("invalid format (%s)\n", pszFileName);
			goto INVALID_FORMAT;
		}	

		if (hd->size!=size-sizeof(*hd))
		{
			yiTrace("invalid size (%d!=%d)\n", hd->size, size-sizeof(*hd));
			goto INVALID_FORMAT;
		}

		if (hd->check_sum_interval!=0)
		{		
			check_sum = __get_check_sum__(&hd[1], hd->size, hd->check_sum_interval);
		}

		if (hd->check_sum != check_sum)
		{
			yiTrace("invalid checksum(%d!=%d) (%s)\n", hd->check_sum, check_sum, pszFileName);
			goto INVALID_FORMAT;
		}
		
		flock(fd, LOCK_UN);
		xclose(fd);
		if (reason!=0) *reason=(size-sizeof(hd));
		return (void*)&hd[1];
	}
	
INVALID_FORMAT:
	flock(fd, LOCK_UN);
	xclose(fd);
	if (reason!=0) *reason=eFE_INVALID_FORMAT;
	return 0;

READWRITE_ERROR:
	flock(fd, LOCK_UN);
	xclose(fd);
	if (reason!=0) *reason=eFE_READWRITE;
	return 0;
}

// --------------------------------------------------------------------
int yiFileCreditRead(const char* pszFileName, void* buffer)
{
	int fd = xopen(pszFileName, O_RDONLY, MODE_600);
	syiFileCreditHeader hd; 
	
	if (fd<0)
	{
		yiTrace("xopen error (%s)\n", pszFileName);
		return eFE_OPEN;
	}

	flock(fd, LOCK_EX);

	if (xread(fd, &hd, sizeof(hd))!=sizeof(hd))
	{
		yiTrace("xread error (%s)\n", pszFileName);
		goto READWRITE_ERROR;
	}

	{
		Int32 check_sum;

		if (hd.prefix!=FILE_YOTSUYA_PREFIX)
		{
			yiTrace("invalid format (%s)\n", pszFileName);
			goto INVALID_FORMAT;
		}

		if (xread(fd, buffer, hd.size)!=hd.size)
		{
			yiTrace("invalid size \n");
			goto READWRITE_ERROR;
		}

		if (hd.check_sum_interval!=0)
		{		
			check_sum = __get_check_sum__(buffer, hd.size, hd.check_sum_interval);
		}

		if (hd.check_sum != check_sum)
		{
			yiTrace("invalid checksum(%d!=%d) (%s)\n", hd.check_sum, check_sum, pszFileName);
			goto INVALID_FORMAT;
		}
		
		flock(fd, LOCK_UN);
		xclose(fd);

		return eFE_OK;
	}
	
INVALID_FORMAT:
	flock(fd, LOCK_UN);
	xclose(fd);
	return eFE_INVALID_FORMAT;

READWRITE_ERROR:
	flock(fd, LOCK_UN);
	xclose(fd);
	return eFE_READWRITE;
}

// --------------------------------------------------------------------
void yiFileCreditUnloadEx(void* p)
{
	yifree(&((syiFileCreditHeader*)p)[-1]);
}

// --------------------------------------------------------------------
int yiFileCreditWriteRecord(const char* pszFileName, const void* record, int n, int size, int check_sum_interval)
{
	syiFileCreditHeaderRecord header;
	int fd;

	if (yiIsExist(pszFileName, 0)==False)
	{
		header.prefix = FILE_YOTSUYA_PREFIX;
		header.record_size = size+sizeof(Int32);
		header.size= size+sizeof(Int32)+sizeof(header);
		header.check_sum_interval = check_sum_interval;
		header.cnt=1;

		fd = xopen(pszFileName, O_WRONLY|O_CREAT, MODE_600);

		if (fd<0)
		{
			yiTrace("xopen error (%s)\n", pszFileName);
			return eFE_OPEN;
		}

		flock(fd, LOCK_EX);
	
		if (xwrite(fd, &header, sizeof(header))!=sizeof(header))
		{
			yiTrace("xwrite error (%s)\n", pszFileName);
			goto READWRITE_ERROR;
		}

		if (check_sum_interval!=0)
		{
			*((Int32*)&((char*)record)[size])=__get_check_sum__(record, size, check_sum_interval);
		}

		if (xwrite(fd, record, header.record_size)!=header.record_size)
		{
			yiTrace("xwrite error (%s)\n", pszFileName);
			goto READWRITE_ERROR;
		}

		//ftruncate(fd, header.size);
		flock(fd, LOCK_UN);
		xclose(fd);

		return eFE_OK;
	}

	fd = xopen(pszFileName, O_RDWR, MODE_600);

	if (fd<0)
	{
		yiTrace("xopen error (%s)\n", pszFileName);
		return eFE_OPEN;
	}

	if (xread(fd, &header, sizeof(header))!=sizeof(header))
	{
		yiTrace("xread error (%s)\n", pszFileName);
		goto READWRITE_ERROR;
	}

	if (header.prefix!=FILE_YOTSUYA_PREFIX )
	{
		yiTrace("invalid format\n");
		return eFE_INVALID_FORMAT;
	}

	if (n==-1 ||
		n>= header.cnt)
	{
		header.cnt++;
		header.size+=header.record_size;

		if (header.check_sum_interval!=0)
		{
			*((Int32*)&((char*)record)[size])=__get_check_sum__(record, size, header.check_sum_interval);
		}

		xlseek(fd, 0, SEEK_END);
		if (xwrite(fd, record, header.record_size)!=header.record_size)
		{
			yiTrace("xwrite error (%s)\n", pszFileName);
			goto READWRITE_ERROR;
		}

		xlseek(fd, 0, SEEK_SET);
		if (xwrite(fd, &header, sizeof(header))!=sizeof(header))
		{
			yiTrace("xwrite error (%s)\n", pszFileName);
			goto READWRITE_ERROR;
		}

		//ftruncate(fd, header.size);
		flock(fd, LOCK_UN);
		xclose(fd);
		return eFE_OK;
	}

	if (header.check_sum_interval!=0)
	{
		if (header.check_sum_interval!=0)
		{
			*((Int32*)&((char*)record)[size])=__get_check_sum__(record, size, header.check_sum_interval);
		}
	}

	xlseek(fd, sizeof(header)+n*header.size, SEEK_SET);
	if (xwrite(fd, &header, sizeof(header))!=sizeof(header))
	{
		yiTrace("xwrite error (%s)\n", pszFileName);
		goto READWRITE_ERROR;
	}

	flock(fd, LOCK_UN);
	xclose(fd);

	return eFE_OK;


READWRITE_ERROR:
	flock(fd, LOCK_UN);
	xclose(fd);
	return eFE_READWRITE;
}

// --------------------------------------------------------------------
static __inline int _yiFileCreditReadRecord(const char* pszFileName, void* record, int n, void** p)
{
	syiFileCreditHeaderRecord hd;
	int fd = xopen(pszFileName, O_RDONLY, MODE_600);

	if (p) *p=0;
			
	if (fd<0)
	{
		yiTrace("xopen error %s\n", pszFileName);
		return eFE_OPEN;
	}

	if (xread(fd, &hd, sizeof(hd))!=sizeof(hd))
	{
		yiTrace("xread error %s\n", pszFileName);
		goto READ_WRITE_ERROR;
	}

	if (hd.prefix!=FILE_YOTSUYA_PREFIX)
	{
		yiTrace("invalid format(prefix) %s\n", pszFileName);
		goto INVALID_FORMAT;
	}

	if (hd.cnt<=n)
	{
		yiTrace("invalid format cnt over(%d:%d) (%s)\n", hd.cnt, n, pszFileName);
		goto INVALID_FORMAT;
	}

	if (record==0)
	{
		*p = yialloc(hd.record_size);
		record=*p;
	}

	xlseek(fd, hd.record_size*n, SEEK_CUR);
	
	if (xread(fd, record, hd.record_size)!=hd.record_size)
	{
		yiTrace("xread error %s\n", pszFileName);
		goto READ_WRITE_ERROR;
	}

	flock(fd, LOCK_UN);
	xclose(fd);

	if (hd.check_sum_interval!=0)
	{
		int record_size = hd.record_size-sizeof(Int32);
		Int32 check_sum = __get_check_sum__(record, record_size, hd.check_sum_interval);

		if (check_sum != *(Int32*)(&((char*)record)[record_size]))
		{
			flock(fd, LOCK_UN);
			xclose(fd);
			yiTrace("invalid format checksum %s\n", pszFileName);
			goto INVALID_FORMAT;
		}
	}

	return eFE_OK;

INVALID_FORMAT:
	if (p && *p) yifree(*p);
	return eFE_INVALID_FORMAT;

	
READ_WRITE_ERROR:
	if (p && *p) yifree(*p);
	flock(fd, LOCK_UN);
	xclose(fd);

	return eFE_READWRITE;
}

// --------------------------------------------------------------------
int yiFileCreditReadRecord(const char* pszFileName, void* record, int n)
{
	return _yiFileCreditReadRecord(pszFileName, record, n, 0);
}

// --------------------------------------------------------------------
void* yiFileCreditLoadRecord(const char* pszFileName, int n, int* reason)
{
	void* p=0;
	*reason = _yiFileCreditReadRecord(pszFileName, 0, n, &p);

	return *reason == eFE_OK ? p : 0;
}

// --------------------------------------------------------------------
static __inline int _yiFileCreditReadRecords(const char* pszFileName, void* record, int from, int cnt, void** p)
{
	syiFileCreditHeaderRecord hd;
	int xreads;
	int fd;

	if (p) *p=0;

	fd = xopen(pszFileName, O_RDONLY, MODE_600);


	if (fd<0)
	{
		return eFE_OPEN;
	}


	if (xread(fd, &hd, sizeof(hd))!=sizeof(hd))
	{
		goto READWRITE_ERROR;
	}



	if (hd.prefix!=FILE_YOTSUYA_PREFIX)
	{
		goto INVALID_FORMAT;
	}

	if (hd.cnt<=(from+cnt-1))
	{
		goto INVALID_FORMAT;
	}

	if (record==0)
	{
		*p = yialloc(hd.record_size*cnt);
		record=*p;
	}

	xlseek(fd, hd.record_size*from, SEEK_CUR);

	xreads=hd.record_size*cnt;
	if (xread(fd, record, xreads)!=xreads)
	{
		goto READWRITE_ERROR;
	}

	flock(fd, LOCK_UN);
	xclose(fd);	

	if (hd.check_sum_interval!=0)
	{
		int i;
		char* pIndex=(char*)record;

		for (i=0; i<cnt; i++)
		{
			int record_size = hd.record_size-sizeof(Int32);
			Int32 check_sum = *((Int32*)(&pIndex[record_size]));
			if (check_sum != __get_check_sum__(pIndex, record_size, hd.check_sum_interval))
			{
				goto INVALID_FORMAT;
			}

			pIndex+=hd.record_size;
		}
	}

	return eFE_OK;

READWRITE_ERROR:
	if (p && *p) yifree(*p);
	flock(fd, LOCK_UN);
	xclose(fd);
	return eFE_READWRITE;

INVALID_FORMAT:
	if (p && *p) yifree(*p);
	flock(fd, LOCK_UN);
	xclose(fd);
	return eFE_INVALID_FORMAT;
}

// --------------------------------------------------------------------
int yiFileCreditReadRecords(const char* pszFileName, void* record, int from, int cnt)
{
	return _yiFileCreditReadRecords(pszFileName, record, from, cnt, 0);
}

// --------------------------------------------------------------------
void* yiFileCreditLoadRecords(const char* pszFileName, int from, int cnt, int* reason)
{
	void* p=0;
	*reason = _yiFileCreditReadRecords(pszFileName, 0, from, cnt, &p);

	return *reason == eFE_OK ? p : 0;
}

// --------------------------------------------------------------------
typedef struct _syiFreeRecordHeader {
	Int32 prefix;
	Int32 reserved[3];
} syiFreeRecordHeader;

// --------------------------------------------------------------------
typedef struct _syiFreeRecordHeaderData
{
	Int32 offset;
	Int32 size;
} syiFreeRecordHeaderData;

// --------------------------------------------------------------------
int yiFileCreditAppendFreeRecord(const char* path, const char* pszDataName, void* data, int size)
{
	char szFileName[256];
	int  fd=0;
	int  nDataCnt=0;
	int  nOffset;
	Bool IsDir=False;
	syiFreeRecordHeader header;
	syiFreeRecordHeaderData headerRecordOffset;

	sprintf(szFileName, "%s/%s", path, pszDataName);

	fd = xopen(szFileName, O_WRONLY|O_CREAT, MODE_600);
	if (fd<0)
	{
		return eFE_OPEN;
	}
	flock(fd, LOCK_EX);
	nOffset = xlseek(fd, 0, SEEK_END);

	if (xwrite(fd, data, size)!=size)
	{
		goto READWRITE_ERROR;
	}

	//ftruncate(fd, xlseek(fd, 0, SEEK_END));
	flock(fd, LOCK_UN);
	xclose(fd);

	sprintf(szFileName, "%s/.%s", path, pszDataName);

	if (yiIsExist(szFileName, &IsDir)==False)
	{
		fd = xopen(szFileName, O_WRONLY|O_CREAT, MODE_600);

		if (fd<0)
		{
			return eFE_OPEN;
		}
		header.prefix = FILE_YOTSUYA_PREFIX;

		if (xwrite(fd, &header, sizeof(header))!=sizeof(header))
		{
			goto READWRITE_ERROR;
		}

		//ftruncate(fd, sizeof(header));
	}
	else
	{
		if (IsDir==True)
		{
			return eFE_PATH;
		}

		fd = xopen(szFileName, O_WRONLY, MODE_600);

		if (fd<0)
		{
			return eFE_OPEN;
		}
	}

	flock(fd, LOCK_EX);

	headerRecordOffset.offset = nOffset;
	headerRecordOffset.size = size;
	
	xlseek(fd, 0, SEEK_END);

	if (xwrite(fd, &headerRecordOffset, sizeof(headerRecordOffset))!=sizeof(headerRecordOffset))
	{
		goto READWRITE_ERROR;
	}

	//ftruncate(fd, xlseek(fd, 0, SEEK_END));
	flock(fd, LOCK_UN);
	xclose(fd);

	return eFE_OK;

READWRITE_ERROR:
	flock(fd, LOCK_UN);
	xclose(fd);

	return eFE_READWRITE;
}

// --------------------------------------------------------------------
void* yiFileCreditLoadFreeRecord(const char* path, const char* pszDataName, int index, int* out_size, int* reason)
{
	char szFileName[256];
	int fd;
	int offset;
	int offset_e;
	char* buffer=0;
	syiFreeRecordHeaderData data;
	
	sprintf(szFileName, "%s/.%s", path, pszDataName);

	fd = xopen(szFileName, O_RDONLY, MODE_600);

	if (fd <0)
	{
		if (reason) *reason = eFE_OPEN;
		return 0;
	}

	flock(fd, LOCK_EX);

	offset_e = xlseek(fd, 0, SEEK_END);
	offset = xlseek(fd, sizeof(syiFreeRecordHeader)+index*sizeof(syiFreeRecordHeaderData), SEEK_SET);

	if (offset_e < offset)
	{
		flock(fd, LOCK_UN);
		xclose(fd);
		if (reason) *reason = eFE_INVALID_PARAM;
		return 0;
	}

	if (xread(fd, &data, sizeof(data))!=sizeof(data))
	{
		goto READWRITE_ERROR;
	}

	flock(fd, LOCK_UN);
	xclose(fd);

	sprintf(szFileName, "%s/%s", path, pszDataName);

	fd = xopen(szFileName, O_RDONLY, MODE_600);

	if (fd <0)
	{
		if (reason) *reason = eFE_OPEN;
		return 0;
	}
		
	flock(fd , LOCK_EX);

	buffer = (char*) yialloc(data.size);

	if (buffer==0)
	{
		flock(fd, LOCK_UN);
		xclose(fd);
		if (reason) *reason = eFE_MEMORY;
		return 0;
	}

	xlseek(fd, data.offset, SEEK_SET);

	if (xread(fd, buffer, data.size)!=data.size)
	{
		goto READWRITE_ERROR;
	}

	flock(fd, LOCK_UN);
	xclose(fd);

	if (reason) *reason = eFE_OK;
	if (out_size) *out_size= data.size;

	return buffer;

READWRITE_ERROR:
	flock(fd, LOCK_UN);
	xclose(fd);
	if (reason) *reason = eFE_READWRITE;
	return 0;
}

// --------------------------------------------------------------------
void yiFileCreditUnloadFreeRecord(void* p)
{
	yifree(p);
}

typedef struct xsFileStream {
	HandleMutex hMutex;
	int fd;
}sFileStream;

// --------------------------------------------------------------------
HandleFileStream yiFileStreamOpen(const char* pszFileName)
{
	int fd = xopen(pszFileName, _O_WRONLY|_O_CREAT, MODE_777);
	sFileStream* p;
	if (fd<0)
		return 0;

	xlseek(fd, 0, SEEK_END);

	p=(sFileStream*)yialloc(sizeof(sFileStream));
	p->fd=fd;
	p->hMutex = yiMutexCreate(0);
	return (HandleFileStream)p;
}

//#include "yiThread.h"
#include "yistd.h"

// --------------------------------------------------------------------
static void OnFileStreamWrite(void* param)
{
	unsigned int* p = (unsigned int*)param;
	sFileStream* h = (sFileStream*)p[0];

	if (h==0)
	{
		yifree(p);
		return ;
	}
	yiMutexLock(h->hMutex);
	xwrite(h->fd, &p[2], p[1]);
	yiMutexUnlock(h->hMutex);

	yifree(p);
}

// --------------------------------------------------------------------
Bool yiFileStreamWrite(HandleFileStream hStream, const void* data, int size)
{
	unsigned int param_size = size+sizeof(HandleFileStream)+sizeof(int);
	unsigned int* param = (unsigned int*)yialloc(param_size);

	if (hStream==0)
		return False;

	param[0]=hStream;
	param[1]=size;
	memcpy(&param[2], data, size);

	return yiStartThread(OnFileStreamWrite, param, param_size);
}

// --------------------------------------------------------------------
void yiFileStreamClose(HandleFileStream hStream)
{
	sFileStream* p = (sFileStream*)hStream;

	yiMutexLock(p->hMutex);
	yiMutexUnlock(p->hMutex);

	yiMutexDestroy(p->hMutex);

	xclose(p->fd);

	yifree(p);
}

typedef struct {
	char  path[256];
	int   size;
	void* p;
} sBackupThreadParam;

#include <process.h>

void OnBackupThread(void* p)
{
	char path[256];
	sBackupThreadParam* param = (sBackupThreadParam*)p;

	wsprintf(path, "%s.bak", param->path);
	yiFileWrite(path, param->p, 0, param->size);

	yifree(param);
	_endthread();
}

// --------------------------------------------------------------------
void* yiFileLoadEx(const char* pszFilePath, int* size)
{
	void* p=0;
	int n = yiFileSize(pszFilePath);

	if (n==0 ||
			n==-1)
	{
		return 0;
	}

	p=yialloc(n+1);
	if (p==0)
		return 0;

	if (yiFileReadEx(pszFilePath, 0, p, n-sizeof(unsigned int), True)==True)
	{
		if (size)
			*size=n;
		return p;
	}
	yifree(p);
	return 0;
}

// --------------------------------------------------------------------
void yiFileUnLoadEx(void* data)
{
	if (data)
		yifree(data);
}

// --------------------------------------------------------------------
unsigned int xGetCheckSum(int fd)
{
	unsigned int check_sum=0;
	int filesize=xlseek(fd, 0, SEEK_END);
	
	if (filesize<128)
	{
		int i=0;
		unsigned char read_buffer[256];
		unsigned int check_sum=0;
		xlseek(fd, sizeof(unsigned int), SEEK_SET);
		xread(fd, &read_buffer, filesize-sizeof(unsigned int));

		for (i=0; i<(filesize-sizeof(unsigned int)); i++)
		{
			check_sum+=(unsigned int)read_buffer[i];	
		}
		//lseek(fd, 0, SEEK_SET);
		//write(fd, &check_sum, sizeof(unsigned int));
	}	
	else
	{	
		int i=0;
		int filesize = xlseek(fd, 0, SEEK_END)-sizeof(unsigned int);
		const int n=16;
		const int delta = filesize/n;
		int sum=sizeof(unsigned int);

		for (i=0; i<n; i++)
		{
			unsigned int value=0;
			xlseek(fd, sum, SEEK_SET);
			xread(fd, &value, sizeof(value));
			check_sum+=value;
			sum+=delta;
			if (sum>filesize)
				break;
		}
		//lseek(fd, 0, SEEK_SET);
		//write(fd, &check_sum, sizeof(unsigned int));
	}
	return check_sum;
}



// --------------------------------------------------------------------
static Bool _yiFileWriteEx(const char* pszFilePath, int flag, int nIndex, void* buffer, int nSize, Bool bBackup)
{
	int n=0;
	unsigned int check_sum=0;
	int fd;
	Bool bIsDir;
	if (yiIsExist(pszFilePath, &bIsDir)==False)
	{
		fd = (IntPtr)CreateFileA(pszFilePath, GENERIC_READ|GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
		//fd = xopen(pszFilePath, O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
	}
	else
	{
		fd = (IntPtr)CreateFileA(pszFilePath, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		//fd = xopen(pszFilePath, O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
	}

	if (fd<0)
	{
		return False;
	}

	if (flock(fd, LOCK_EX)!=0)
	{
		xclose(fd);
		return False;
	}
	
	if (xlseek(fd, 0, SEEK_END)==0)
	{
		unsigned int checksum=0;
		xwrite(fd, &checksum, sizeof(unsigned int));
	}

	if (nIndex==-1)
	{
		xlseek(fd, 0, SEEK_END);
	}
	else
		xlseek(fd, nIndex+sizeof(unsigned int), SEEK_SET);

	while(nSize)
	{
		int nWriteOnce = __buffer_size_once < nSize ? __buffer_size_once:nSize;
		nSize -= xwrite(fd, buffer, nWriteOnce);
	}

	// here calculate checksum... 
	check_sum = xGetCheckSum(fd);
	xlseek(fd, 0, SEEK_SET);
	xwrite(fd, &check_sum, sizeof(unsigned int));

	if (bBackup==True)
	{
		int size = xlseek(fd, 0, SEEK_END);
		void* p = yialloc(size+1);
		sBackupThreadParam* param=(sBackupThreadParam*)yialloc(sizeof(sBackupThreadParam));

		xlseek(fd, 0, SEEK_SET);
		xread(fd, p, size);
		strcpy(param->path, pszFilePath);
		param->p=p;
		param->size=size;

		_beginthread(OnBackupThread, 0, (void*)param);
		//pthread_create(&th, 0, xOnBackupThread, (void*)param);
	}

	if (flock(fd, LOCK_UN)!=0)
	{
		return False;
	}
	
	xclose(fd);

	return True;	
}

// --------------------------------------------------------------------
Bool yiFileCreateEx(const char* pszFilePath, int nIndex, void* buffer, int nSize, Bool bBackup)
{
	return _yiFileWriteEx(pszFilePath, O_RDWR|O_CREAT|O_TRUNC, nIndex, buffer, nSize, bBackup);
}

// --------------------------------------------------------------------
Bool yiFileWriteEx(const char* pszFilePath, int nIndex, void* buffer, int nSize, Bool bBackup)
{
	return _yiFileWriteEx(pszFilePath, O_RDWR, nIndex, buffer, nSize, bBackup);
}

// --------------------------------------------------------------------
Bool yiFileReadEx(const char* pszFilePath, int nIndex, void* buffer, int nSize, Bool bRestore)
{
	unsigned int check_sum;
	unsigned int header_check_sum=0;
	Bool bIsRestored=False;
	int n=0;
	int sum;
	int fd=0;
	char* pBuffer;
TOP:
	fd = xopen(pszFilePath, O_RDONLY, MODE_666);
	
	if (fd<0)
	{
		return False;
	}

	if (flock(fd, LOCK_EX)!=0)
	{
		return False;
	}

	check_sum = xGetCheckSum(fd);

	xlseek(fd, 0, SEEK_SET);

	if ( xread(fd, &header_check_sum, sizeof(unsigned int))!=sizeof(unsigned int))
	{
		xclose(fd);
		return False;
	}

	if (check_sum!=header_check_sum)
	{
		char path[256];
		int  size;
		void* p;

		if (bRestore==False)
		{
			flock(fd, LOCK_UN);
			xclose(fd);
			return False;
		}

		if (bIsRestored==True)
		{
			flock(fd, LOCK_UN);
			xclose(fd);
			return False;
		}
		sprintf(path, "%s.bak", pszFilePath);
		p = yiFileLoad(path, &size);

		flock(fd, LOCK_UN);
		xclose(fd);
		if (p)
		{
			yiFileWrite(pszFilePath, p, 0, size);
		}

		bIsRestored=True;
		goto TOP;
	}
	xlseek(fd, nIndex+sizeof(unsigned int), SEEK_SET);

	pBuffer = (char*)buffer;
	sum=0;
	while(nSize)
	{
		int nReadOnce = __buffer_size_once < nSize ? __buffer_size_once:nSize;
		int res = xread(fd, &pBuffer[sum], nReadOnce); 
		
		if (res<0)
		{
			flock(fd, LOCK_UN);
			xclose(fd);
			return False;
		}
		
		nSize -= res;
		sum+=nReadOnce;
	}

	
	flock(fd, LOCK_UN);
	xclose(fd);

	return True;

}

typedef struct {
	unsigned short max_count;
	unsigned short size; 
	unsigned short top;
	unsigned short bottom;
	unsigned short count;
} sCreateQ;
// --------------------------------------------------------------------
Bool yiFileCreateQ(const char* pszFilePath, int count, int size, Bool bOverWritable)
{
	int fd = xopen(pszFilePath, O_WRONLY|O_CREAT|O_TRUNC, MODE_666);
	int fileSize = count*size+sizeof(sCreateQ);
	int res=0;
	sCreateQ* s;

	if (fd<0)
		return False;

	flock(fd, LOCK_EX);
	s = (sCreateQ*)yialloc(fileSize);

	s->max_count=count;
	s->size=size;
	s->top=0;
	s->bottom=0;
	s->count=0;

	if (bOverWritable)
		s->max_count|=0x8000;

	res = xwrite(fd, s, fileSize);
	flock(fd, LOCK_UN);
	xclose(fd);
	yifree(s);
	return (res==fileSize)?True:False;
}

// --------------------------------------------------------------------
Bool yiFileEnQ(const char* pszFilepath, void* data)
{
	sCreateQ s;
	int fd = xopen(pszFilepath, O_RDWR, MODE_666);
	unsigned short max_count;//=s.max_count&0x7FFF;
	Bool bIsOverwritable=False;

	if (fd<0)
		return False;

	flock(fd, LOCK_EX);

	xlseek(fd, 0, SEEK_SET);
	if (xread(fd, &s, sizeof(s))!=sizeof(s))
	{
		goto xERROR;
	}

	max_count=s.max_count&0x7FFF;
	bIsOverwritable=((s.max_count&0x8000)!=0);

	if (bIsOverwritable!=True &&
		s.count==max_count)
	{
		goto xERROR;
	}


	xlseek(fd, sizeof(s)+s.bottom*s.size, SEEK_SET);
	if (xwrite(fd, data, s.size)!=s.size)
	{
		goto xERROR;
	}

	s.bottom++;
		
	if (s.bottom==max_count)
		s.bottom=0;

	s.count++;
	if (s.count>max_count)
	{
		s.count=max_count;
		s.top=s.bottom;
	}
		
	xlseek(fd, 0, SEEK_SET);
	if (xwrite(fd, &s, sizeof(s))!=sizeof(s))
	{
		goto xERROR;
	}
	flock(fd, LOCK_UN);
	xclose(fd);

	return True;
xERROR:
	flock(fd, LOCK_UN);
	xclose(fd);

	return False;
}


// --------------------------------------------------------------------
Bool yiFileDeQ(const char* pszFilepath, void* data)
{
	sCreateQ s;
	int fd = xopen(pszFilepath, O_RDWR, MODE_666);
	unsigned short max_count;

	if (fd<0)
		return False;

	flock(fd, LOCK_EX);

	xlseek(fd, 0, SEEK_SET);
	if (xread(fd, &s, sizeof(s))!=sizeof(s))
	{
		goto xERROR;
	}

	if (s.count==0)
	{
		flock(fd, LOCK_UN);
		xclose(fd);
		return False;
	}

	max_count=s.max_count&0x7FFF;

	xlseek(fd, sizeof(s)+s.top*s.size, SEEK_SET);
	if (xread(fd, data, s.size)!=s.size)
	{
		goto xERROR;
	}
	s.count--;
	s.top++;
	if (s.top==max_count)
		s.top=0;
	
	xlseek(fd, 0, SEEK_SET);
	if (xwrite(fd, &s, sizeof(s))!=sizeof(s))
	{
		goto xERROR;
	}
	flock(fd, LOCK_UN);
	xclose(fd);

	return True;
xERROR:
	flock(fd, LOCK_UN);
	xclose(fd);
	return False;
}

// --------------------------------------------------------------------
void* yiFileLoadQ(const char* pszFileName, int* n, Bool bIsRemove)
{
	sCreateQ s;
	int fd = xopen(pszFileName, O_RDWR, MODE_666);
	void* ret=0;
	int size;

	if (fd<0)
	{
		*n=0;
		return 0;
	}

	flock(fd, LOCK_EX);

	xlseek(fd, 0, SEEK_SET);
	if (xread(fd, &s, sizeof(s))!=sizeof(s))
	{
		goto xERROR;
	}

	if (s.count==0)
	{
		*n=0;
		flock(fd, LOCK_UN);
		close(fd);
		return 0;
	}

	*n=s.count;
	size=s.count*s.size;
	ret = yialloc(size);//size = s.count*s.size);

	xlseek(fd, sizeof(s)+s.top*s.size, SEEK_SET);

	//printf("top:%d, bottom:%d, count:%d, size:%d\n", s.top, s.bottom, s.count, size);
	if (s.top<s.bottom)
	{
		if (xread(fd, ret, size)!=size)
		{
			yifree(ret);
			goto xERROR;
		}

		if (bIsRemove==False)
		{
			flock(fd, LOCK_UN);
			xclose(fd);
			return ret;
		}
	}
	else
	{
		unsigned char* pIndex=(unsigned char*)ret;
		int xSize=(s.count-s.top)*s.size;
		int reads=0;

		xlseek(fd, sizeof(s)+s.top*s.size, SEEK_SET);
		if ((reads=xread(fd, ret, xSize))!=xSize)
		{
			//printf("reads:%d, xSize:%d\n", reads, xSize);
			yifree(ret);
			goto xERROR;
		}
		//printf("trace...........1\n");
		lseek(fd, sizeof(s), SEEK_SET);
		xSize=(s.bottom)*s.size;
		if (xSize!=0 &&
			read(fd, &pIndex[(s.count-s.top)*s.size], xSize)!=xSize)//(s.bottom+1)*size)
		{
			yifree(ret);
			goto xERROR;
		}
		//printf("trace...........2\n");
		if (bIsRemove==False)
		{
			flock(fd, LOCK_UN);
			xclose(fd);
			return ret;
		}
	}

	s.count=0;
	s.top=0;
	s.bottom=0;
	xlseek(fd, 0, SEEK_SET);
	if (xwrite(fd, &s, sizeof(s))!=sizeof(s))
	{
		yifree(ret);
		goto xERROR;
	}
	
	flock(fd, LOCK_UN);
	xclose(fd);
	return ret;

xERROR:
	flock(fd, LOCK_UN);
	xclose(fd);
	return 0;
}
