#include "sycfile.h"
#include "yifile.h"

#define YOTUYA_FILE_HEADER_PREFIX 0x53544f59 // "YOTS"

// ------------------------------
enum eFileHeaderFlag16 {
	eFHF_ORIGINAL 	= 1<<31,
	eFHF_SHADOW 	= 1<<30,	
	eFHF_BACKUP 	= 1<<29,	
	eFHF_SIZE_FIXED	= 1<<28,
	eFHF_RECORD = 1<<27
};

// ---------------------------
typedef struct xsycFileHeader {
	Int32 prefix; // "YOTS":
	Int32 size;
	Int32 flag32; // 31-24: flags , 23-16: check_sum_valid, 15-0: record size
	unsigned short check_sum_valid; // HighWord : 0:not use, n:interval LowWord: record size
	Int32 check_sum; // if check_sum_valid is zero, it is record count. 
}sycFileHeader;


// --------------------------------------
inline static eFileError __check__(const char* pszFileName)
{
	Bool isDir=False;
	Bool isExsist = yiIsExist(pszFileName, &isDir);

	if (isExsist==True && isDir==True)
	{
		return eFE_PATH;
	}

	if (isExsist==True&&
		YIM_FLAGS(actions, eSA_NO_OVERWRITE)==True)
	{
		return eFE_EXSIST;	
	}

	if (isExsist==False &&
		YIM_FLAGS(actions, eSA_CREATE)==False)
	{
		return eFE_PATH;
	}

	return eFE_OK;	
}

// --------------------------------------
static inline unsigned int __GetCheckSum__(void* data, int size, int check_sum_valid)
{
}

// --------------------------------------
static inline eFileError __write_fixed_size__(const char* pszFileName, sycFileHeader* hd, void* data, int size, eSubAction actions)
{
	int fd = __openfile__(pszFileName);

	if (fd<0)
	{
		return eFE_UNKNOWN;
	}

	flock(fd, LOCK_EX); 

	hd->size = size+sizeof(*hd);
	int check_sum_valid = __highbyte__(__highword__(hd->flag32));
	if (check_sum_valid!=0)
	{
		hd->check_sum=__GetCheckSum__(data, size, check_sum_valid);
	}
	hd->flag32|=eFHF_ORIGINAL;

	if (write(fd, hd, sizeof(*hd))!=sizeof(*hd))
	{
		flock(fd, LOCK_UN);
		close(fd);
		return eFE_READWRITE;
	}

	if (write(fd, data, size)!=size)
	{
		flock(fd, LOCK_UN);
		close(fd);
		return eFE_READWRITE;
	}

	flock(fd, LOCK_UN);
	ftruncate(fd, hd->size);
	close(fd);

	char szFullPath[256];

	hd->flag32&=~(eFHF_ORIGINAL);
	if ((actions&eSA_WITH_SHADOW)!=0)
	{
		const char* pszShadowRoot = cycEnv::GetShadowRoot();
		sprintf(szFullPath, "%s/%s", pszShadowRoot, pszFileName);

		if (yiMakePath(szFullpath, True)==False)
		{
			SYSLOG(,"make dir error %s\n", szFullPath);
			return eFE_UNKNOWN;
		}
		hd->flag32|=eFHF_SHADOW;
	}
	else if ((actions&eSA_WITH_BACKUP)!=0)
	{
		sprintf(szFullPath, "%s.backup", pszFileName);
		hd->flag32|=eFHF_BACKUP;
	}

	fd = __openfile__(szFullPath);

	if (fd<0)
	{
		SYSLOG(,"open error %s\n", szFullPath);
		return eFE_UNKNOWN;
	}

	flock(fd, LOCK_EX);

	if (write(fd, hd, sizeof(*hd))!=sizeof(*hd))
	{
		SYSLOG(,"write error %s\n", szFullPath);
		flock(fd, LOCK_UN);
		close(fd);
		return eFE_READWRITE;
	}

	if (write(fd, data, size)!=size)
	{
		SYSLOG(,"write error %s\n", szFullPath);
		flock(fd, LOCK_UN);
		close(fd);
		return eFE_READWRITE;
	}

	ftruncate(fd, hd->size);
	flock(fd, LOCK_UN);
	close(fd);
	return eFE_OK;
}


// --------------------------------------
static inline eFileError __write_record__(const char* pszFileName, sycFileHeader* hd, void* data, int index, int size, eSubAction actions )
{
	int fd = __openfile__(pszFileName);

	if (fd<0)
	{
		return eFE_UNKNOWN;
	}
	flock(fd, LOCK_EX);

	Bool bIsNew = lseek(fd, 0, SEEK_END)==0?True:False;

	int _size = 0;

	if (bIsNew==False)
	{
		lseek(fd, 0, SEEK_SET);
		if (read(fd, hd, sizeof(*hd))!=sizeof(*hd))
		{
			flock(fd, LOCK_UN);
			close(fd);
			return eFE_READWRITE;
		}

		if (hd->check_sum!=size+sizeof(Int32))
		{
			flock(fd, LOCK_UN);
			close(fd);
			return eFE_INVALID_HEADER;
		}
	}
	else
	{
		hd->check_sum = size+sizeof(Int32);
	}

	hd->flag16 |= eFHF_ORIGINAL;

	int _index;
	
	if (index!=-1)
	{
		_index = sizeof(*hd)+index*(hd->check_sum);
	}
	else
	{
		_index = hd->size ? hd->size : sizeof(*hd);
	}

	if (_index=>hd->size)
	{
		hd->size+=hd->check_sum;
	}

	lseek(fd, 0, SEEK_SET);
	if (write(fd, hd, sizeof(*hd))!=sizeof(*hd))
	{
		flock(fd, LOCK_UN);
		close(fd);
		return eFE_READWRITE;
	}
	
	lseek(fd, _index, SEEK_SET);	

	if (write(fd, data, hd->check_sum)!=hd->check_sum)
	{
		flock(fd, LOCK_UN);
		close(fd);
		return eFE_READWRITE;
	}

	ftruncate(fd, hd->size);
	flock(fd, LOCK_UN);
	close(fd);

	char szFullPath[256];

	if ((actions&eSA_WITH_SHADOW)!=0)
	{
		const char* pszShadowRoot = cycEnv::GetShadowRoot();
		sprintf(szFullPath, "%s/%s", pszShadowRoot, pszFileName);

		if (yiMakePath(szFullpath, True)==False)
		{
			SYSLOG(,"make dir error %s\n", szFullPath);
			return eFE_UNKNOWN;
		}

		hd->flag16 &= ~(eFHF_ORIGINAL);
		hd->flag16 |= eFHF_SHADOW;

	}
	else if (actions&eSA_WITH_BACKUP!=0)
	{
		sprintf(szFullPath, "%s.backup", pszFileName);
		hd->flag16 &= ~(eFHF_ORIGINAL);
		hd->flag16 |= eFHF_BACKUP;
	}

	fd = __openfile__(szFullPath);

	flock(fd, LOCK_EX);

	lseek(fd, 0, SEEK_SET);

	if (write(fd, hd, sizeof(*hd))!=sizeof(*hd))
	{
		flock(fd, LOCK_UN);
		close(fd);
		return eFE_READWRITEEX;
	}

	lseek(fd, _index, SEEK_SET);
	if (write(fd, data, size+sizeof(Int32))!=sizeof(Int32))
	{
		flock(fd, LOCK_UN);
		close(fd);
		return eFE_READWRITEEX;
	}

	flock(fd, LOCK_UN);
	close(fd);

	return eEF_OK;
}

// --------------------------------------
eFileError sycFileCreditWriteFixed(const char* pszFileName, void* data, int size, eSubAction actions)
{
	eFileError err = __check__(pszfileName);

	if (err!=eFE_OK)
	{
		return err;
	}

	sycFileHeader header;

	header.flag16=0;
	header.flag16|=eFHF_SIZE_FIXED;
	header.prefix = YOTUYA_FILE_HEADER_PREFIX;

	return __write_fixed_size__(pszFileName, &header, data, size, actions, fd);
}

// --------------------------------------
eFileError sycFileCreditWriteRecord(const char* pszFileName, void* data, int size, eSubAction actions)
{
	eFileError err = __check__(pszfileName);
	if (err!=eFE_OK)
	{
		return err;
	}

	sycFileHeader header;

	header.flag16=0;
	header.flag16|=eFHF_RECORD;
	header.prefix = YOTUYA_FILE_HEADER_PREFIX;
	header.check_sum = size+sizeof(unsigned int);

	return __write_record__(pszFileName, &header, data, size, actions, fd);
}



// --------------------------------------
eFileError sycFileCreditWrite(const char* pszFileName, sycFileHeader* hd, void* data, int index, int size, eSubAction actions)
{
	Bool isDir=False;
	Bool isExsist = yiIsExist(pszFileName, &isDir);

	if (isExsist==True && isDir==True)
	{
		return eFE_PATH;
	}

	if (isExsist==True&&
		YIM_FLAGS(actions, eSA_NO_OVERWRITE)==True)
	{
		return eFE_EXSIST;	
	}

	if (isExsist==False &&
		YIM_FLAGS(actions, eSA_CREATE)==False)
	{
		return eFE_PATH;
	}

	
	int fd = __openfile__(pszFileName);

	if (fd<0)
	{
		return eFE_UNKNOWN;
	}

	flock(fd, LOCK_EX); 


	// file size fixed.
	if ((hd->flag16&eFHF_SIZE_FIXED)!=0)
	{
		hd->prefix = YOTUYA_FILE_HEADER_PREFIX;
		return __write_fixed_size__(pszFileName, hd, data, index, size, actions, fd);
	}
	else if ((hd->flag16&eFHF_FIXED_RECORD)!=0)
	{
		return __write_fixed_record__(pszFileName, hd, data, index, size, actions, fd);
	} 

	// size is not fixed.
	//
	sycFileHeader header;
	int _filesize = leek(fd, 0, SEEK_END);
	
	memset(&header, 0, sizeof(header));

	if (hd==0 && _filesize==0)
	{
		flock(fd, LOCK_UN);
		close(fd);
		return eFE_UNKNOWN;
	}

	if (hd==0)
	{
		hd = &header;
	}

	lseek(fd, 0, SEEK_SET);
	if (_filesize && 
		read(fd, hd, sizeof(*hd))!=sizeof(*hd))
	{
		flock(fd, LOCK_UN);
		close(fd);
		return eFE_READWRITE;
	}

	int record_size=size;
	if (hd->check_sum_valid!=0)
	{
		unsigned int check_sum = __GetCheckSum__(data, size, hd->check_sum_valid);
		*(unsigned int*)(&((char*)data)[size])=check_sum;
		record_size+=sizeof(unsigned int);
	}

	if (_filesize==0)
	{
		hd->size = sizeof(*hd)+size;
	}

	if (write(fd, hd, sizeof(*hd))!=sizeof(*hd))
	{
		flock(fd, LOCK_UN);
		close(fd);
		close(fd);
		return eFE_READWRITE;
	}
			
	lseek(fd, hd->check_sum*index, SEEK_SET);

	if (write(fd, data, size+sizeof(unsigned int));

	//　ファイルのサイズが固定ではない場合、
	hd->check_sum;

	if (write(fd, hd, sizeof(*hd))!=sizeof(*hd))
	{
		flock(fd, LOCK_UN);
		close(fd);
		return eFE_READWRITE;
	}

	lseek(fd, _index, SEEK_SET);

	CHECK_SUM(data, _size, check_sum);

	if (write(fd, data, _size)!=_size)
	{
		flock(fd, LOCK_UN);
		close(fd);
		return eFE_READWRITE;
	}

	if (hd-> )
	{
		ftruncate(fd, hd->size);
	}

	flock(fd, LOCK_UN);
	close(fd);
	return eFE_OK;
}

