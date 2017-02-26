#ifndef __yi_file_h__
#define __yi_file_h__

#include "yi.h"

#define __BUFFERSIZE_PER_ONCE__ 8192

#ifndef __WIN32__

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
enum eyiFileMode
{
	MODE_777 = S_IRWXU|S_IRWXG|S_IRWXO,
	MODE_755 = (MODE_777&~S_IWOTH)&~S_IWGRP,
	MODE_666 = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH,
	MODE_444 = S_IRUSR|S_IRGRP|S_IROTH, /* ReadOnly*/
	MODE_400 = S_IRUSR, 
	MODE_644 = S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH, 
	MODE_664 = S_IRUSR|S_IWUSR|S_IWGRP|S_IRGRP|S_IROTH|S_IWOTH 
};
#endif /*__POSIX__*/

// --------------------------------------------------------------------
EXTERN_C Bool yiReadFile(const char* pszFileName, void* buffer, int nIndex, int nSize);

// --------------------------------------------------------------------
EXTERN_C void* yiFileLoad(const char* pszFileName, int* nSize);
//
// --------------------------------------------------------------------
EXTERN_C void yiFileUnload(void* p);

// --------------------------------------------------------------------
EXTERN_C Bool yiFileWrite(const char* pszFileName, void* buffer, int nIndex, int nSize);
//
// --------------------------------------------------------------------
EXTERN_C int yiFileSize(const char* pszFileName);

// --------------------------------------------------------------------
EXTERN_C Bool yiIsExist(const char* path, Bool* pIsDir);
//
// --------------------------------------------------------------------
EXTERN_C Bool yiIsDir(const char* path);

// --------------------------------------------------------------------
EXTERN_C Bool yiMoveFile(const char* src, const char* dest);

// --------------------------------------------------------------------
EXTERN_C Bool yiRemove(const char* filename);

// --------------------------------------------------------------------
EXTERN_C Bool yiCopyFile(const char* src, const char* dest);
//
// --------------------------------------------------------------------
EXTERN_C Bool yiMakeDir(const char* path, Bool bIsSubDir);

// --------------------------------------------------------------------
EXTERN_C void yiSetBuffer(int n);

#ifdef __POSIX__
// --------------------------------------------------------------------
EXTERN_C Bool yiFileMode(const char* pszFilename, int nMode);
#endif /*__POSIX__*/

#endif
