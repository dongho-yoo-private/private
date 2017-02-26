#ifdef __POSIX__
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <sys/file.h>
#include <string.h>
#define xopen open
#define xclose close
#define xlseek lseek
#define xread read
#define xwrite write

#else
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <share.h>

static int xopen(const char* pszFileName, int flags, mode_t mode)
{
	int fd;
	if (_sopen_s(&fd, flags, _SH_DENYWR, mode)!=0)
	{
		return -1;
	}

	return fd;
}

#endif

#include "yitrace.h"
#include "yistd.h"
#include "yifile.h"

extern int __buffer_size_once;


// --------------------------------------------------------------------
/*static int write_continuos(int fd, ...)
{
	void** pArgv = (void**)&fd;
	pArgv++;

	int i=0;
	int sum=0;
	while(pArgv[i])
	{
		if (write(fd, pArgv[i], pArgv[i+1])!=pArgv[i+1])
		{
			return -1;
		}
		sum+=pArgv[i+1];
		i++;i++;
	}

	return sum;
}*/

inline static Int32 __get_check_sum__(const void* buffer, int size, int check_sum_interval)
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
	
	yiTraceLib("%s xopen %d\n", pszFileName, fd);

	if (fd<0)
	{
		return False;
	}

	lseek(fd, nIndex, SEEK_SET);	

	pBuffer = (char*)buffer;
	sum=0;
	while(nSize)
	{
		int nReadOnce = __buffer_size_once < nSize ? __buffer_size_once:nSize;
		int res = read(fd, &pBuffer[sum], nReadOnce); 

		if (res<0)
		{
			return False;
		}
		
		nSize -= res;
		sum+=nReadOnce;
	}
	
	close(fd);

	return True;
}

//
// --------------------------------------------------------------------
void* yiFileLoad(const char* pszFileName, int* nSize)
{
	void* p=0;
	int n = yiFileSize(pszFileName);
	
	if (n<0)
	{
		yiTraceLib("yiLoadFile(%s) Failure... size is %d\n", pszFileName, n);
		return 0;
	}

	if (nSize!=0)
	{
		*nSize=n;
	}

	p = yialloc(n+1);


	if (yiFileRead(pszFileName, p, 0, n)==True)
	{
		yiTraceLib("yiLoadFile(%s) Success... size is %d\n", pszFileName, n);
		return p;
	}

	//yistrn(p, n);
	
	return 0;
}

// -------------------------------------------------
const void* yiFileLoadLock(const char* pszFileName, int* nSize)
{
	unsigned char* p=0;
	int n = yiFileSize(pszFileName);
	int fd;
	
	if (n<0)
	{
		yiTraceLib("yiLoadFile(%s) Failure... size is %d\n", pszFileName, n);
		return 0;
	}

	if (nSize!=0)
	{
		*nSize=n;
	}

	p = (unsigned char*)yialloc(n+1+sizeof(int)+sizeof(int));


	//if (yiFileRead(pszFileName, &p[sizeof(int)+sizeof(int)], 0, n)==True)
	{
		int fd = xopen(pszFileName, O_RDONLY, MODE_666);
	
		if (fd<0)
		{
			yifree(p);
			close(fd);
			return 0;
		}	
		flock(fd, LOCK_EX);	
		
		if (read(fd, &p[sizeof(int)+sizeof(int)], n)!=n)
		{
			flock(fd, LOCK_UN);
			yifree(p);
			close(fd);
			return 0;
		}
	
		*(unsigned int*)p = n;
		*(unsigned int*)&p[sizeof(int)]=fd;
			
		return (const void*)&p[sizeof(int)+sizeof(int)];
	}

	//yistrn(p, n);
	
	return 0;
}

// --------------------------------------------------------------------
void yiFileUnload(void* p)
{
	yifree(p);
}

// --------------------------------------------------------------------
void yiFileUnloadUnlock(void* p)
{
	void* pTop = &((unsigned char*)p)[-8];
	int fd = ((int*)p)[1];

	flock(fd, LOCK_UN);
	close(fd);
	yifree(pTop);
}

// --------------------------------------------------------------------
void* yiFileSafeLoad(const char* pszFileName, int* nSize)
{
	int size;
	void* p = yiFileLoad(pszFileName, &size);
	sSafeFileFormat* pFormat = (sSafeFileFormat*)p;
	int record_size;
	int record_cnt;
	if (p==0)
		return 0;

	if (pFormat->n!=size)
	{
		yiFileUnload(p);
		*nSize=0;
		YIERROR("yiFileSafeLoad(%s) Failure invalid size\n", pszFileName);
		return 0;
	}

	if (pFormat->checksum!=__get_check_sum__(&pFormat[1], size-sizeof(sSafeFileFormat), 4))
	{
		yiFileUnload(p);
		*nSize=0;
		YIERROR("yiFileSafeLoad(%s) Failure invalid checksum\n", pszFileName);
		return 0;
	}

	*nSize = size-sizeof(sSafeFileFormat);
	return (void*)&pFormat[1];
}

// --------------------------------------------------------------------
void yiFileSafeUnload(void* data)
{
	sSafeFileFormat* p = (sSafeFileFormat* )data;
	yiFileUnload(&p[-1]);	
}


// --------------------------------------------------------------------
Bool yiFileSafeWrite(const char* pszFileName, void* _buffer, int nSize, Bool bMakeShadow, const char* pszShadowRoot)
{
	char buffer[256];
	sSafeFileFormat* p = yiFileCreateSafeFormat(_buffer, nSize);

	strcpy(buffer, pszFileName);
	strcat(buffer, ".temp");
	rename(pszFileName, buffer);

	p=&p[-1];
	//p->n = nSize+sizeof(sSafeFileFormat);
	//p->checksum = __get_check_sum__(&p[1], nSize, 4);
	//
	
	if (yiFileWrite(pszFileName, p, 0, nSize+sizeof(sSafeFileFormat))!=True)
	{
		YIERROR("yiFileWrite(%s) Error!\n", pszFileName);
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
			YIERROR("yiFileWrite(%s) Error!\n", buffer);
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
EXTERN_C Bool yiFileSafeRestore(const char* pszBadFile, const char* pszBackupFile)
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
	int fd = xopen(pszFileName, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);


	yiTraceLib("yiWriteFile(%s) xopen %d, write (%d)\n", pszFileName, fd, nSize);
	
	if (fd<0)
	{
		yiTraceLib("yiWriteFile(%s) xopen Failure %d\n", pszFileName, fd);
		return False;
	}

	if (flock(fd, LOCK_EX)!=0)
	{
		yiTrace("lock failure....\n");
		close(fd);
		return False;
	}

	lseek(fd, nIndex, SEEK_SET);

	while(nSize)
	{
		int nWriteOnce = __buffer_size_once < nSize ? __buffer_size_once:nSize;
		nSize -= write(fd, buffer, nWriteOnce);
	}

	if (flock(fd, LOCK_UN)!=0)
	{
		yiTrace("unlock failure....\n");
		return False;
	}
	
	close(fd);
	yiTraceLib("yiWriteFile(%s) write success %d bytes\n", pszFileName, nSize);

	return True;
}

// --------------------------------------------------------------------
int yiFileSize(const char* pszFileName)
{
	struct stat info;

	if (stat(pszFileName, &info)!=-1)
	{
		if (S_ISDIR(info.st_mode)==False)
		{
			return info.st_size;
		}

	}

	yiTraceLib("yiFileSize(%s) Failure.... errno=%d\n", pszFileName, errno);

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


// --------------------------------------------------------------------
Bool yiRemove(const char* pszFileName)
{
	Bool res;

	if (yiIsDir(pszFileName)==True)
	{
		res = rmdir(pszFileName);
	}
	else
	{
		res = remove(pszFileName);
	}

	return res==0?True:False;
}

#include <stdio.h>
// --------------------------------------------------------------------
Bool yiMakeDir(const char* path, Bool bIsSubDir)
{
	printf("yiMakeDir(%s, %d)\n", path, bIsSubDir);
	if (bIsSubDir==False)
	{
		return mkdir(path, MODE_755)==0?True:False;
	}

	if (mkdir(path, MODE_755)!=0)
	{
		int i=0;
		int n=strlen(path);
		char buffer[256]={0,};

		if (errno==EEXIST)
		{
			return True;
		}
		
		for (i=n-1; i>-1; i--)
		{
			if (path[i]=='/')
			{
				strncpy(buffer, path, i);
				if (yiMakeDir(buffer, True)==False)
					return False;

				break;
			}
		}

		return yiMakeDir(path, False);

	}

	return True;
/*
	char szTemp[1024];
	char** pDirz=0;
	register Bool res;
	register cnt=0;

	pDirz = (char**)yiStrSplitz(path, "/", 0);

	if (pDirz==0) return False;

	while(pDirz[cnt]) {
		res = (mkdir(pDirz[cnt++], MODE_755)==0)?True:False;
	}
*/
}

#include <string.h>
// --------------------------------------------------------------------
static char* __getprevpath__(char* buffer, const char* path)
{
	int n = strlen(path);

	while(n!=0&&path[n--]!='/');

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
EXTERN_C Bool yiFileMode(const char* pszFilename, int nMode)
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

	if (write(fd, &header, sizeof(header))!=sizeof(header))
	{
		yiTrace("write error (%s)\n", pszFileName);
		goto READWRITE_ERROR;
	}

	if (write(fd, buffer, size)!=size)
	{
		yiTrace("write error (%s)\n", pszFileName);
		goto READWRITE_ERROR;
	}


	ftruncate(fd, size+sizeof(header));	
	flock(fd, LOCK_UN);
	close(fd);

	return eFE_OK;

READWRITE_ERROR:
	flock(fd, LOCK_UN);
	close(fd);
	return eFE_READWRITE;
}

// --------------------------------------------------------------------
void* yiFileCreditLoad(const char* pszFileName, int* reason)
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

	syiFileCreditHeader header;

	if (read(fd, &header, sizeof(header))!=sizeof(header))
	{
		yiTrace("read error (%s)\n", pszFileName);
		goto READWRITE_ERROR;
	}

	size = header.size;
	p=yialloc(size);

	if (read(fd, p, size)!=size)
	{
		yiTrace("read error (%s)\n", pszFileName);
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
		close(fd);
		if (reason!=0) *reason=size;
		return (void*)p;
	}
	
INVALID_FORMAT:
	flock(fd, LOCK_UN);
	close(fd);
	if (reason!=0) *reason=eFE_INVALID_FORMAT;
	return 0;

READWRITE_ERROR:
	flock(fd, LOCK_UN);
	close(fd);
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
	size = lseek(fd, 0, SEEK_END)-lseek(fd, 0, SEEK_SET);
	p=yialloc(size);	

	if (read(fd, p, size)!=size)
	{
		yiTrace("read error (%s)\n", pszFileName);
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
		close(fd);
		if (reason!=0) *reason=(size-sizeof(hd));
		return (void*)&hd[1];
	}
	
INVALID_FORMAT:
	flock(fd, LOCK_UN);
	close(fd);
	if (reason!=0) *reason=eFE_INVALID_FORMAT;
	return 0;

READWRITE_ERROR:
	flock(fd, LOCK_UN);
	close(fd);
	if (reason!=0) *reason=eFE_READWRITE;
	return 0;
}

// --------------------------------------------------------------------
int yiFileCreditRead(const char* pszFileName, void* buffer)
{
	int size;
	int fd = xopen(pszFileName, O_RDONLY, MODE_600);
	syiFileCreditHeader hd; 
	
	if (fd<0)
	{
		yiTrace("xopen error (%s)\n", pszFileName);
		return eFE_OPEN;
	}

	flock(fd, LOCK_EX);

	if (read(fd, &hd, sizeof(hd))!=sizeof(hd))
	{
		yiTrace("read error (%s)\n", pszFileName);
		goto READWRITE_ERROR;
	}

	{
		Int32 check_sum;

		if (hd.prefix!=FILE_YOTSUYA_PREFIX)
		{
			yiTrace("invalid format (%s)\n", pszFileName);
			goto INVALID_FORMAT;
		}

		if (read(fd, buffer, hd.size)!=hd.size)
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
		close(fd);

		return eFE_OK;
	}
	
INVALID_FORMAT:
	flock(fd, LOCK_UN);
	close(fd);
	return eFE_INVALID_FORMAT;

READWRITE_ERROR:
	flock(fd, LOCK_UN);
	close(fd);
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
	
		if (write(fd, &header, sizeof(header))!=sizeof(header))
		{
			yiTrace("write error (%s)\n", pszFileName);
			goto READWRITE_ERROR;
		}

		if (check_sum_interval!=0)
		{
			*((Int32*)&((char*)record)[size])=__get_check_sum__(record, size, check_sum_interval);
		}

		if (write(fd, record, header.record_size)!=header.record_size)
		{
			yiTrace("write error (%s)\n", pszFileName);
			goto READWRITE_ERROR;
		}

		ftruncate(fd, header.size);
		flock(fd, LOCK_UN);
		close(fd);

		return eFE_OK;
	}

	fd = xopen(pszFileName, O_RDWR, MODE_600);

	if (fd<0)
	{
		yiTrace("xopen error (%s)\n", pszFileName);
		return eFE_OPEN;
	}

	if (read(fd, &header, sizeof(header))!=sizeof(header))
	{
		yiTrace("read error (%s)\n", pszFileName);
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

		lseek(fd, 0, SEEK_END);
		if (write(fd, record, header.record_size)!=header.record_size)
		{
			yiTrace("write error (%s)\n", pszFileName);
			goto READWRITE_ERROR;
		}

		lseek(fd, 0, SEEK_SET);
		if (write(fd, &header, sizeof(header))!=sizeof(header))
		{
			yiTrace("write error (%s)\n", pszFileName);
			goto READWRITE_ERROR;
		}

		ftruncate(fd, header.size);
		flock(fd, LOCK_UN);
		close(fd);
		return eFE_OK;
	}

	if (header.check_sum_interval!=0)
	{
		if (header.check_sum_interval!=0)
		{
			*((Int32*)&((char*)record)[size])=__get_check_sum__(record, size, header.check_sum_interval);
		}
	}

	lseek(fd, sizeof(header)+n*header.size, SEEK_SET);
	if (write(fd, &header, sizeof(header))!=sizeof(header))
	{
		yiTrace("write error (%s)\n", pszFileName);
		goto READWRITE_ERROR;
	}

	flock(fd, LOCK_UN);
	close(fd);

	return eFE_OK;


READWRITE_ERROR:
	flock(fd, LOCK_UN);
	close(fd);
	return eFE_READWRITE;
}

// --------------------------------------------------------------------
static inline int _yiFileCreditReadRecord(const char* pszFileName, void* record, int n, void** p)
{
	syiFileCreditHeaderRecord hd;
	int fd = xopen(pszFileName, O_RDONLY, MODE_600);

	if (p) *p=0;
			
	if (fd<0)
	{
		yiTrace("xopen error %s\n", pszFileName);
		return eFE_OPEN;
	}

	if (read(fd, &hd, sizeof(hd))!=sizeof(hd))
	{
		yiTrace("read error %s\n", pszFileName);
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

	lseek(fd, hd.record_size*n, SEEK_CUR);
	
	if (read(fd, record, hd.record_size)!=hd.record_size)
	{
		yiTrace("read error %s\n", pszFileName);
		goto READ_WRITE_ERROR;
	}

	flock(fd, LOCK_UN);
	close(fd);

	if (hd.check_sum_interval!=0)
	{
		int record_size = hd.record_size-sizeof(Int32);
		Int32 check_sum = __get_check_sum__(record, record_size, hd.check_sum_interval);

		if (check_sum != *(Int32*)(&((char*)record)[record_size]))
		{
			flock(fd, LOCK_UN);
			close(fd);
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
	close(fd);

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
static inline int _yiFileCreditReadRecords(const char* pszFileName, void* record, int from, int cnt, void** p)
{
	syiFileCreditHeaderRecord hd;
	int reads;

	if (p) *p=0;

	int fd = xopen(pszFileName, O_RDONLY, MODE_600);


	if (fd<0)
	{
		return eFE_OPEN;
	}


	if (read(fd, &hd, sizeof(hd))!=sizeof(hd))
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

	lseek(fd, hd.record_size*from, SEEK_CUR);

	reads=hd.record_size*cnt;
	if (read(fd, record, reads)!=reads)
	{
		goto READWRITE_ERROR;
	}

	flock(fd, LOCK_UN);
	close(fd);	

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
	close(fd);
	return eFE_READWRITE;

INVALID_FORMAT:
	if (p && *p) yifree(*p);
	flock(fd, LOCK_UN);
	close(fd);
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
	int  header_size;
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
	nOffset = lseek(fd, 0, SEEK_END);

	if (write(fd, data, size)!=size)
	{
		goto READWRITE_ERROR;
	}

	ftruncate(fd, lseek(fd, 0, SEEK_END));
	flock(fd, LOCK_UN);
	close(fd);

	sprintf(szFileName, "%s/.%s", path, pszDataName);

	if (yiIsExist(szFileName, &IsDir)==False)
	{
		fd = xopen(szFileName, O_WRONLY|O_CREAT, MODE_600);

		if (fd<0)
		{
			return eFE_OPEN;
		}
		header.prefix = FILE_YOTSUYA_PREFIX;

		if (write(fd, &header, sizeof(header))!=sizeof(header))
		{
			goto READWRITE_ERROR;
		}

		ftruncate(fd, sizeof(header));
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
	
	lseek(fd, 0, SEEK_END);

	if (write(fd, &headerRecordOffset, sizeof(headerRecordOffset))!=sizeof(headerRecordOffset))
	{
		goto READWRITE_ERROR;
	}

	ftruncate(fd, lseek(fd, 0, SEEK_END));
	flock(fd, LOCK_UN);
	close(fd);

	return eFE_OK;

READWRITE_ERROR:
	flock(fd, LOCK_UN);
	close(fd);

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

	offset_e = lseek(fd, 0, SEEK_END);
	offset = lseek(fd, sizeof(syiFreeRecordHeader)+index*sizeof(syiFreeRecordHeaderData), SEEK_SET);

	if (offset_e < offset)
	{
		flock(fd, LOCK_UN);
		close(fd);
		if (reason) *reason = eFE_INVALID_PARAM;
		return 0;
	}

	if (read(fd, &data, sizeof(data))!=sizeof(data))
	{
		goto READWRITE_ERROR;
	}

	flock(fd, LOCK_UN);
	close(fd);

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
		close(fd);
		if (reason) *reason = eFE_MEMORY;
		return 0;
	}

	lseek(fd, data.offset, SEEK_SET);

	if (read(fd, buffer, data.size)!=data.size)
	{
		goto READWRITE_ERROR;
	}

	flock(fd, LOCK_UN);
	close(fd);

	if (reason) *reason = eFE_OK;
	if (out_size) *out_size= data.size;

	return buffer;

READWRITE_ERROR:
	flock(fd, LOCK_UN);
	close(fd);
	if (reason) *reason = eFE_READWRITE;
	return 0;
}

// --------------------------------------------------------------------
void* yiFileCreditUnloadFreeRecord(void* p)
{
	yifree(p);
}


// --------------------------------------------------------------------
void* yiFileLoadEx(const char* pszFilePath, int* size)
{
	void* p=0;
	int n = yiFileSize(pszFilePath);

	p=yialloc(n+1);
	if (p==0)
		return 0;

	if (yiFileReadEx(pszFilePath, 0, p, n-sizeof(unsigned int), True)==True)
	{
		if (size)
			*size=n-sizeof(unsigned int);
		return p;
	}
	yifree(p);
	if (size)
		*size=0;
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
	int filesize=lseek(fd, 0, SEEK_END);
	
	if (filesize<128)
	{
		int i=0;
		unsigned char read_buffer[256];
		unsigned int check_sum=0;
		lseek(fd, sizeof(unsigned int), SEEK_SET);
		read(fd, &read_buffer, filesize-sizeof(unsigned int));

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
		int filesize = lseek(fd, 0, SEEK_END)-sizeof(unsigned int);
		const int n=16;
		const int delta = filesize/n;
		int sum=sizeof(unsigned int);

		for (i=0; i<n; i++)
		{
			unsigned int value=0;
			lseek(fd, sum, SEEK_SET);
			read(fd, &value, sizeof(value));
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

typedef struct {
	char  path[256];
	int   size;
	void* p;
} sBackupThreadParam;

// --------------------------------------------------------------------
static void* xOnBackupThread(void* p)
{
	char path[256];
	sBackupThreadParam* param = (sBackupThreadParam*)p;

	sprintf(path, "%s.bak", param->path);
	yiFileWrite(path, param->p, 0, param->size);

	yifree(param);
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
		int n = xwrite(fd, pIndex, nWriteOnce);
		if (n<0 || n==0)
		{
			if (flock(fd, LOCK_UN)!=0)
			{
				yiTrace("unlock failure....\n");
				return False;
			}
			xclose(fd);

			return False;
		}

		nSize -= n;


		pIndex=&pIndex[nWriteOnce];
	}

	if (flock(fd, LOCK_UN)!=0)
	{
		yiTrace("unlock failure....\n");
		return False;
	}

#ifdef _WIN32	
	SetEndOfFile((HANDLE)fd);
#else
	ftruncate(fd, lseek(fd, 0, SEEK_CUR));
#endif
	xclose(fd);
	//("yiWriteFile(%s) xwrite success %d bytes\n", pszFileName, nSize);

	return True;
}

// --------------------------------------------------------------------
static Bool _yiFileWriteEx(const char* pszFilePath, int flag, int nIndex, void* buffer, int nSize, Bool bBackup, Bool bWriteOnce)
{
	int n=0;
	unsigned int check_sum=0;
	int fd;
	int sum=0;
	char* pIndex=(char*)buffer;
	Bool bIsDir;
	if (yiIsExist(pszFilePath, &bIsDir)==False)
	{
#ifdef _WIN32
		fd = (IntPtr)CreateFileA(pszFilePath, GENERIC_READ|GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
#else
		fd = xopen(pszFilePath, O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
#endif
	}
	else
	{
#ifdef _WIN32
		fd = (IntPtr)CreateFileA(pszFilePath, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
#else
		fd = xopen(pszFilePath, O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
#endif
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

		int n = xwrite(fd, pIndex, nWriteOnce);
		if (n<0 || n==0)
		{
			if (flock(fd, LOCK_UN)!=0)
			{
				yiTrace("unlock failure....\n");
				return False;
			}
			xclose(fd);

			return False;
		}

		nSize -= n;

		//nSize -= xwrite(fd, &pIndex[sum], nWriteOnce);
		sum+=nWriteOnce;
	}

	if (bWriteOnce)
	{
#ifdef _WIN32
		SetEndOfFile(fd);
#else
		ftruncate(fd, lseek(fd, 0, SEEK_CUR));
#endif
	}

	// here calculate checksum... 
	check_sum = xGetCheckSum(fd);
	xlseek(fd, 0, SEEK_SET);
	xwrite(fd, &check_sum, sizeof(unsigned int));

	if (bBackup==True)
	{
		int size = xlseek(fd, 0, SEEK_END);
		void* p = yialloc(size+1);
#ifndef _WIN32
		pthread_t th=0;
#endif
		sBackupThreadParam* param=(sBackupThreadParam*)yialloc(sizeof(sBackupThreadParam));

		xlseek(fd, 0, SEEK_SET);
		xread(fd, p, size);
		strcpy(param->path, pszFilePath);
		param->p=p;
		param->size=size;

#ifdef _WIN32
		_beginthread(OnBackupThread, 0, (void*)param);
#else
		pthread_create(&th, 0, xOnBackupThread, (void*)param);
#endif
	}



	if (flock(fd, LOCK_UN)!=0)
	{
		return False;
	}
	
	xclose(fd);

	return True;	
}

// --------------------------------------------------------------------
/*static Bool _yiFileWriteEx(const char* pszFilePath, int flag, int nIndex, void* buffer, int nSize, Bool bBackup)
{
	int n=0;
	unsigned int check_sum=0;
	int fd;
	Bool bIsDir;
	if (yiIsExist(pszFilePath, &bIsDir)==False)
	{
		fd = xopen(pszFilePath, O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
	}
	else
	{
		fd = xopen(pszFilePath, O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
	}

	YIDEBUG("yiWriteFile(%s) xopen %d\n", pszFilePath, fd);
	
	if (fd<0)
	{
		YIERROR("yiWriteFile(%s) xopen Failure %d, err:%d\n", pszFilePath, fd, errno);
		return False;
	}

	if (flock(fd, LOCK_EX)!=0)
	{
		YIERROR("lock failure.... errno:%d\n", errno);
		close(fd);
		return False;
	}
	
	if (lseek(fd, 0, SEEK_END)==0)
	{
		unsigned int checksum=0;
		write(fd, &checksum, sizeof(unsigned int));
	}


	if (nIndex!=-1)
	{
		lseek(fd, nIndex+sizeof(unsigned int), SEEK_SET);
	}

	char* pIndex=(char*)buffer;
	while(nSize)
	{
		int nWriteOnce = __buffer_size_once < nSize ? __buffer_size_once:nSize;

		int nWritten=write(fd, pIndex, nWriteOnce);

		if (nWritten!=nWriteOnce)
		{
			YIERROR("yiWriteFileEx() write error! %d, %d\n", nWritten, errno);
			flock(fd, LOCK_UN);
			close(fd);
			return False;
		}

		nSize -= nWritten;//write(fd, buffer, nWriteOnce);
		pIndex+=nWritten;
	}

	// here calculate checksum... 
	check_sum = xGetCheckSum(fd);
	lseek(fd, 0, SEEK_SET);
	write(fd, &check_sum, sizeof(unsigned int));

	if (bBackup==True)
	{
		pthread_t th;
		int size = lseek(fd, 0, SEEK_END);
		void* p = yialloc(size+1);
		sBackupThreadParam* param=(sBackupThreadParam*)yialloc(sizeof(sBackupThreadParam));

		lseek(fd, 0, SEEK_SET);
		read(fd, p, size);
		strcpy(param->path, pszFilePath);
		param->p=p;
		param->size=size;
		pthread_create(&th, 0, xOnBackupThread, (void*)param);
		pthread_detach(th);
	}

	if (flock(fd, LOCK_UN)!=0)
	{
		YIERROR("unlock failure....\n");
		close(fd);
		return False;
	}
	
	close(fd);
	YIDEBUG("yiWriteFileEx(%s) write success \n", pszFilePath);

	return True;	
}*/

// --------------------------------------------------------------------
/*Bool yiFileCreateEx(const char* pszFilePath, int nIndex, void* buffer, int nSize, Bool bBackup)
{
	Bool res;
	Bool bIsFree=False;
	if (buffer==0)
	{
		bIsFree=True;
		buffer=yialloci(nSize);
	}
	res = _yiFileWriteEx(pszFilePath, O_RDWR|O_CREAT|O_TRUNC, nIndex, buffer, nSize, bBackup, True);

	if (bIsFree==True)
		yifree(buffer);

	return res;
}

// --------------------------------------------------------------------
Bool yiFileWriteEx(const char* pszFilePath, int nIndex, void* buffer, int nSize, Bool bBackup)
{
	return _yiFileWriteEx(pszFilePath, O_RDWR, nIndex, buffer, nSize, bBackup);
}
*/

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
		YIERROR("%s xopen %d failure! errno%d\n", pszFilePath, fd, errno);
		return False;
	}

	if (flock(fd, LOCK_EX)!=0)
	{
		YIERROR("%s lock failure! errno:%d\n", pszFilePath, errno);
		return False;
	}

	check_sum = xGetCheckSum(fd);

	lseek(fd, 0, SEEK_SET);

	if (read(fd, &header_check_sum, sizeof(unsigned int))!=sizeof(unsigned int))
	{
		flock(fd, LOCK_UN);
		close(fd);
		return False;
	}

	if (check_sum!=header_check_sum)
	{
		char path[256];
		int  size;
		void* p;

		yiTraceLib("checksum failure! %u:%u\n", check_sum, header_check_sum);
		if (bRestore==False)
		{
			flock(fd, LOCK_UN);
			close(fd);
			return False;
		}

		if (bIsRestored==True)
		{
			flock(fd, LOCK_UN);
			close(fd);
			return False;
		}
		sprintf(path, "%s.bak", pszFilePath);
		p = yiFileLoad(path, &size);

		flock(fd, LOCK_UN);
		close(fd);
		if (p)
		{
			yiFileWrite(pszFilePath, p, 0, size);
		}

		bIsRestored=True;
		goto TOP;
	}

	if (nIndex==-1)
	{
		int filesize = lseek(fd, 0, SEEK_END)-sizeof(unsigned int);
		int x = nSize;

		if (filesize==0)
		{
			flock(fd, LOCK_UN);
			close(fd);
			return False;
		}

		if (filesize<nSize)
		{
			lseek(fd, sizeof(unsigned int), SEEK_SET);
			nSize=filesize;
		}
		else
			lseek(fd, -x, SEEK_END);
	}
	else
	{
		lseek(fd, nIndex+sizeof(unsigned int), SEEK_SET);
	}

	pBuffer = (char*)buffer;
	sum=0;
	while(nSize)
	{
		int nReadOnce = __buffer_size_once < nSize ? __buffer_size_once:nSize;
		int res = read(fd, &pBuffer[sum], nReadOnce); 
		
		if (res<0 ||
			res==0)
		{
			flock(fd, LOCK_UN);
			close(fd);
			return False;
		}

		
		nSize -= res;
		sum+=res;
	}

	
	flock(fd, LOCK_UN);
	close(fd);

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
	sCreateQ* s;

	if (fd<0)
		return False;

	flock(fd, LOCK_EX);
	s = (sCreateQ*)yialloc(fileSize);

	Bool res=False;
	s->max_count=count;
	s->size=size;
	s->top=0;
	s->bottom=0;
	s->count=0;

	if (bOverWritable)
		s->max_count|=0x8000;

	res = write(fd, s, fileSize);
	flock(fd, LOCK_UN);
	close(fd);
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
	{
		return False;
	}

	flock(fd, LOCK_EX);

	lseek(fd, 0, SEEK_SET);
	if (read(fd, &s, sizeof(s))!=sizeof(s))
	{
		goto ERROR;
	}

	max_count=s.max_count&0x7FFF;
	bIsOverwritable=((s.max_count&0x8000)!=0);

	if (bIsOverwritable!=True &&
		s.count==max_count)
	{
		goto ERROR;
	}


	lseek(fd, sizeof(s)+s.bottom*s.size, SEEK_SET);
	if (write(fd, data, s.size)!=s.size)
	{
		goto ERROR;
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
		
	lseek(fd, 0, SEEK_SET);
	if (write(fd, &s, sizeof(s))!=sizeof(s))
	{
		goto ERROR;
	}
	flock(fd, LOCK_UN);
	close(fd);

	YIDEBUG("yiFileEnQ(%s).... success top:%d, bottom:%d, cnt:%d\n", pszFilepath, s.top, s.bottom, s.count);

	return True;
ERROR:
	YIERROR("yiFileEnQ(%s), WriteError!\n", pszFilepath);
	flock(fd, LOCK_UN);
	close(fd);

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

	lseek(fd, 0, SEEK_SET);
	if (read(fd, &s, sizeof(s))!=sizeof(s))
	{
		goto ERROR;
	}

	if (s.count==0)
	{
		flock(fd, LOCK_UN);
		close(fd);
		return False;
	}

	max_count=s.max_count&0x7FFF;

	lseek(fd, sizeof(s)+s.top*s.size, SEEK_SET);
	if (read(fd, data, s.size)!=s.size)
	{
		goto ERROR;
	}
	s.count--;
	s.top++;
	if (s.top==max_count)
		s.top=0;
	
	lseek(fd, 0, SEEK_SET);
	if (write(fd, &s, sizeof(s))!=sizeof(s))
	{
		goto ERROR;
	}
	flock(fd, LOCK_UN);
	close(fd);

	return True;
ERROR:
	YIERROR("yiFileDeQ(%s) read error!\n", pszFilepath);
	flock(fd, LOCK_UN);
	close(fd);
	return False;
}

// --------------------------------------------------------------------
void* yiFileLoadQ(const char* pszFileName, int* n, Bool bIsRemove)
{
	sCreateQ s;
	int fd = xopen(pszFileName, O_RDWR, MODE_666);
	unsigned short max_count;
	void* ret=0;
	int size;

	if (fd<0)
	{
		*n=0;
		return 0;
	}

	flock(fd, LOCK_EX);

	lseek(fd, 0, SEEK_SET);
	if (read(fd, &s, sizeof(s))!=sizeof(s))
	{
		goto ERROR;
	}
	YIDEBUG("yiFileLoadQ(%s).... top:%d, bottom:%d, cnt:%d\n", pszFileName, s.top, s.bottom, s.count);

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

	lseek(fd, sizeof(s)+s.top*s.size, SEEK_SET);

	//printf("top:%d, bottom:%d, count:%d, size:%d\n", s.top, s.bottom, s.count, size);
	if (s.top<s.bottom)
	{
		if (read(fd, ret, size)!=size)
		{
			yifree(ret);
			goto ERROR;
		}

		if (bIsRemove==False)
		{
			flock(fd, LOCK_UN);
			close(fd);
			return ret;
		}
	}
	else
	{
		unsigned char* pIndex=(unsigned char*)ret;
		lseek(fd, sizeof(s)+s.top*s.size, SEEK_SET);
		int xSize=(s.count-s.top)*s.size;
		int reads=0;
		if ((reads=read(fd, ret, xSize))!=xSize)
		{
			//printf("reads:%d, xSize:%d\n", reads, xSize);
			yifree(ret);
			goto ERROR;
		}
		//printf("trace...........1\n");
		lseek(fd, sizeof(s), SEEK_SET);
		xSize=(s.bottom)*s.size;
		if (xSize!=0 &&
			read(fd, &pIndex[(s.count-s.top)*s.size], xSize)!=xSize)//(s.bottom+1)*size)
		{
			yifree(ret);
			goto ERROR;
		}
		//printf("trace...........2\n");
		if (bIsRemove==False)
		{
			flock(fd, LOCK_UN);
			close(fd);
			return ret;
		}
	}

	s.count=0;
	s.top=0;
	s.bottom=0;
	lseek(fd, 0, SEEK_SET);
	if (write(fd, &s, sizeof(s))!=sizeof(s))
	{
		yifree(ret);
		goto ERROR;
	}
	
	flock(fd, LOCK_UN);
	close(fd);
	return ret;

ERROR:
	YIERROR("yiFileLoadQ(%s) read error!\n", pszFileName);
	flock(fd, LOCK_UN);
	close(fd);
	return 0;
}


// --------------------------------------------------------------------
Bool yiFileCreateEx(const char* pszFilePath, int nIndex, void* buffer, int nSize, Bool bBackup)
{
	return _yiFileWriteEx(pszFilePath, O_RDWR|O_CREAT|O_TRUNC, nIndex, buffer, nSize, bBackup, False);
}

// --------------------------------------------------------------------
Bool yiFileWriteEx(const char* pszFilePath, int nIndex, void* buffer, int nSize, Bool bBackup)
{
	return _yiFileWriteEx(pszFilePath, O_RDWR, nIndex, buffer, nSize, bBackup, False);
}

// --------------------------------------------------------------------
Bool yiFileWriteOnceEx(const char* pszFilePath, const void* buffer, int index, int nSize, Bool bBackup)
{
	return _yiFileWriteEx(pszFilePath, O_RDWR, index, buffer, nSize, bBackup, True);
}


// --------------------------------------------------------------------
void yiFileGetNameAndExt(LPCSTR fullpath, LPSTR path, LPSTR name, LPSTR ext)
{
	char* pIndex=(char*)fullpath;
	int n = strlen(fullpath);
	int i=0;

	// まずはパス検索
	for ( i=n-1; i>=0; i--)
	{
		if (pIndex[i]=='\\')
		{
			if (path)
			{
				strncpy(path,fullpath, i);
				path[i]=0;
			}
			break;
		}
	}

	// 名前検索
	{
		int nNameIndex = i?++i:i;
		for ( i=n-1; i>=0; i--)
		{
			if (pIndex[i]=='.')
			{
				if (name)
				{
					strncpy(name, &pIndex[nNameIndex], i-nNameIndex);
					name[i-nNameIndex]=0;
				}
				break;
			}

		}
	}

	// 拡張子検索
	if (ext)
	{
		if (pIndex[i]!='.')
		{
			ext[0]=0;
			return ;
		}

		strcpy(ext, &pIndex[i+1]);
	}

	return ;
}

// --------------------------------------------------------------------
Handle _yiFileOpen(const char* pszFileName)
{
	int fd = 0;
	
	fd = xopen(pszFileName, O_WRONLY|O_CREAT, MODE_777);

	if (fd<0)
	{
		//("yiWriteFile(%s) xopen Failure %d\n", pszFileName, fd);
		return 0;
	}

	return (Handle)fd;
}

// --------------------------------------------------------------------
int _yiFileRead(Handle h, void* buffer, int size)//void* data, int size, Bool bRestore);
{
	return xread((int)h, buffer, size);
}

// --------------------------------------------------------------------
int _yiFileWrite(Handle h, const void* buffer, int size)//void* data, int size, Bool bRestore);
{
	return xwrite((int)h, buffer, size);
}

// --------------------------------------------------------------------
#ifdef _WIN32
Bool yiFileCopy(const char* pszOriginalFile, const char* pszNewFile, HWND hWndProgress, int cur, float factor)
#else
Bool yiFileCopy(const char* pszOriginalFile, const char* pszNewFile)//, HWND hWndProgress, int cur, float factor)
#endif
{
	int fdOrg = _yiFileOpen(pszOriginalFile);
	int fdNew = _yiFileOpen(pszNewFile);
	int all  = _yiFileSize(fdOrg);
	int remain=all;
	char* buffer;

#ifdef _WIN32
	if (fdOrg==INVALID_HANDLE_VALUE)
		return False;
	if (fdNew==INVALID_HANDLE_VALUE)
		return False;
#else
	if ((int)fdOrg<0)
		return False;
	if ((int)fdNew<0)
		return False;
#endif
	
	buffer = (char*)yialloc(__BUFFERSIZE_PER_ONCE__);

	_yiFileTrucate(fdNew);

#ifdef _WIN32
	if (hWndProgress)
	{
		if (cur==-1)
		{
			cur = SendMessage(hWndProgress, PBM_GETPOS, 0, 0);
		}
	}
#endif	
	while(remain)
	{
		int size = __BUFFERSIZE_PER_ONCE__<remain?__BUFFERSIZE_PER_ONCE__:remain;
		if (_yiFileRead(fdOrg, buffer, size)!=size)
		{
			yifree(buffer);
			return False;
		}

		if (_yiFileWrite(fdNew, buffer, size)!=size)
		{
			yifree(buffer);
			return False;
		}
		
		remain-=size;
#ifdef _WIN32
		SendMessage(hWndProgress, PBM_SETPOS, cur+(all-remain), 0);
#endif
	}

	yifree(buffer);

	// ファイルのタイムスタンプも複製。
#ifdef _WIN32
	{
		FILETIME ct;
		FILETIME lat;
		FILETIME lwt;
		
		GetFileTime(fdOrg, &ct, &lat, &lwt);
		SetFileTime(fdNew, &ct, &lat, &lwt);
	}
#endif

	_yiFileClose(fdOrg);
	_yiFileClose(fdNew);

	// ファイルの属性を合わせる。
#ifdef _WIN32
	SetFileAttributes(pszNewFile, GetFileAttributesA(pszOriginalFile));
#endif

	return True;
}

// --------------------------------------------------------------------
unsigned int _yiFileSize(Handle h)
{
#ifdef _WIN32
	return GetFileSize((HANDLE)h, 0);
#else
	int pos = lseek((int)h, 0, SEEK_CUR);
	int size = lseek((int)h, 0, SEEK_END)-lseek((int)h, 0, SEEK_SET);
	lseek((int)h, pos, SEEK_SET);

	return size;
#endif

}

// --------------------------------------------------------------------
unsigned int _yiFileCurpos(Handle h)
{
	return xlseek((int)h, 0, SEEK_CUR);
}

// --------------------------------------------------------------------
unsigned int _yiFileSeek(Handle h, int pos, int from)
{
	return xlseek((int)h, pos, from);
}

// --------------------------------------------------------------------
void _yiFileClose(Handle h)
{
	xclose((int)h);
}

// --------------------------------------------------------------------
void _yiFileTrucate(Handle h)
{
#ifdef _WIN32
	SetEndOfFile(h);
#else
	int cur = lseek((int)h, 0, SEEK_CUR);
	truncate((int)h, cur);
#endif
	
}

char* yiMakeTempFileName(char* name)
{
	//char buffer[256];
	char filename[256];
	//GetTempPathA(256, buffer);

	if (tmpnam(filename)==0)
		return 0;
	//GetTempFileName(buffer, 0, yirand32(True), filename);

	if (name==0)
	{
		name = yistrheap(filename);
	}
	else
	{
		strcpy(name, filename);
	}

	return name;
}

#ifdef __IOS__
char g_szHomeDir[512];

// --------------------------------------------------------------------
void iosSetHomeDir(const char* pszHomeDir)
{
	strcpy(g_szHomeDir, pszHomeDir);
}

// --------------------------------------------------------------------
const char* iosGetHomeDir()
{
	return (const char*)g_szHomeDir;
}

// --------------------------------------------------------------------
const char* iosGetDocumentDir()
{
	static char document[512];
	
	if (document[0]==0)
		sprintf(document, "%s/Documents", g_szHomeDir);
	return (const char*)document;
}

// --------------------------------------------------------------------
const char* iosGetTempDir()
{
	static char tempdir[512];
	if (tempdir[0]==0)
		sprintf(tempdir, "%s/Documents", g_szHomeDir);
	return (const char*)tempdir;
}

// --------------------------------------------------------------------
const char* iosGetLibDir()
{
	static char libdir[512];
	if (libdir[0]==0)
		sprintf(libdir, "%s/Documents", g_szHomeDir);
	return (const char*)libdir;
}

#endif
