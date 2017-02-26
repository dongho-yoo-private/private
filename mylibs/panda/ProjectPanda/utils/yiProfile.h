#ifndef __yi_profile_h__
#define __yi_profile_h__

#include "yifile.h"

typedef Handle HandleProfile;

typedef struct {
	Bool bIsDeleteAtTime;
	int nElementCount;
	int nExtraIdSize;
	int nTotalSizeLimitted;
} syipDetailForBinaryMode;

// ------------------------------------------
EXTERN_C syipDetailForBinaryMode* yipSetDefaultParam(syipDetailForBinaryMode* mode);

// ------------------------------------------
EXTERN_C HandleProfile yipOpen(const char* pszFileName, syipDetailForBinaryMode* detail);//Bool bIsBinaryMode, int nMaxElementSize, int nIdSize);

// ------------------------------------------
EXTERN_C Bool yipReadString(HandleProfile hProfile, const char* key, char* value);

// ------------------------------------------
EXTERN_C int yipReadInt(HandleProfile hProfile, const char* key, int nDefaultNum);

// ------------------------------------------
EXTERN_C Bool yipWriteInt(HandleProfile hProfile, const char* key, int nDefaultNum);

// ------------------------------------------
EXTERN_C Bool yipWriteString(HandleProfile hProfile, const char* key, const char* value);

// ------------------------------------------
EXTERN_C const char** yipGetEnumKeys(HandleProfile hProfile, int* cnt);

// ------------------------------------------
EXTERN_C const char** yipReleaseEnumKeys(HandleProfile hProfile, int* cnt);

// ------------------------------------------
EXTERN_C const void* yipReadBinary(HandleProfile hProfile, int index, int* size);

// ------------------------------------------
EXTERN_C const void* yipReadBinaryById(HandleProfile hProfile, int id, void* exid, int* size);

// ------------------------------------------
EXTERN_C Bool yipWriteBinary(HandleProfile hProfile, int index, void* data, int size, void* id);

// ------------------------------------------
EXTERN_C Bool yipWriteBinaryById(HandleProfile hProfile, int id, void* exid, void* data, int size);

// ------------------------------------------
EXTERN_C void yipGetDetail(HandleProfile hProfile, syipDetailForBinaryMode* mode);

// ------------------------------------------
EXTERN_C void yipUpdateDetail(HandleProfile hProfile, syipDetailForBinaryMode* mode);


// ------------------------------------------
EXTERN_C void yipClose(HandleProfile h);

// ------------------------------------------
EXTERN_C Bool yiProfileWriteString(const char* pszTag, const char* value, const char* pszFileName);

// ------------------------------------------
EXTERN_C Bool yiProfileWriteInt(const char* pszTag, int value, const char* pszFileName);

// ------------------------------------------
EXTERN_C Bool yiProfileReadString(const char* pszTag, char* value, const char* pszFileName);

// ------------------------------------------
EXTERN_C int yiProfileReadInt(const char* pszTag, int defaultValue, const char* pszFileName);


#endif

