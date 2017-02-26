#ifndef __sycfile_h__
#define __sycfile_h__
#include "yi.h"


// ---------------------------
enum eFileError {
	eFE_OK=0,
	eFE_EXSIST=-1,
	eFE_PATH=-2,
	eFE_CHECKSUM=-3,
	eFE_CHECKSUMEX=-4,
	eFE_READWRITE=-5,
	eFE_READWRITEEX=-6,
	eFE_DISK_FULL=-7,
	eFE_INVALID_HEADER=-8,
	eFE_UNKNOWN=-9
};

// ---------------------------
enum eSubAction {
	eSA_NO_OVERWRITE=1,
	eSA_CREATE=2,
	eSA_WITH_SHADOW=4,
	eSA_WITH_BACKUP=8,
};

// --------------------------------------
eFileError sycFileCreditWriteFixed(const char* pszFileName, void* data, int size, eSubAction actions);

// --------------------------------------
eFileError sycFileCreditWriteRecord(const char* pszFileName, void* data, int size, int index, eSubAction actions);

// --------------------------------------
eFileError sycFileCreditAppendRecord(const char* pszFileName, void* data, int size, eSubAction actions);

// --------------------------------------
eFileError sycFileCreditWriteFreeType(const char* pszFileName, void* data, int size, int index, eSubAction actions);

// --------------------------------------
eFileError sycFileCreditAppendFreeType(const char* pszFileName, void* data, int size, eSubAction actions);

// --------------------------------------
eFileError sycFileCreditRead(const char* pszFileName, void* data, int index, int size);

// --------------------------------------
eFileError sycFileCreditReadFreeType(const char* pszFileName, void* data, int index, int size);

// --------------------------------------
void* sycFileCreditLoad(const char* pszFileName, void* data, int* size=0, eFileError* error=0);

// --------------------------------------
void sycFileCreditUnload(void* p);

#endif
