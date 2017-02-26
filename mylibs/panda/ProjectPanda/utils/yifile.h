#ifndef __yi_file_h__
#define __yi_file_h__

#include "yi.h"

#define __BUFFERSIZE_PER_ONCE__ 8192


#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

// ---------------------------
typedef struct xsSafeFileFormat {
	int n; // total size.
	int checksum; // checksum.
	int reserved;
	int reserved2;
} sSafeFileFormat;

// ---------------------------
enum eFileError {
	eFE_OK=0,
	eFE_EXSIST=-1,
	eFE_PATH=-2,
	eFE_CHECKSUM=-3,
	eFE_OPEN=-3,
	eFE_CHECKSUMEX=-4,
	eFE_READWRITE=-5,
	eFE_INVALID_FORMAT=-8,
	eFE_MEMORY=-9,
	eFE_INVALID_PARAM=-10,
	eFE_UNKNOWN=-11
};

// ---------------------------
enum eSubAction {
	eSA_NO_OVERWRITE=1,
	eSA_CREATE=2,
	eSA_WITH_SHADOW=4,
	eSA_WITH_BACKUP=8,
};

#ifndef __WIN32__
enum eyiFileMode
{
	MODE_777 = S_IRWXU|S_IRWXG|S_IRWXO,
	MODE_755 = (MODE_777&~S_IWOTH)&~S_IWGRP,
	MODE_666 = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH,
	MODE_444 = S_IRUSR|S_IRGRP|S_IROTH, /* ReadOnly*/
	MODE_400 = S_IRUSR, 
	MODE_644 = S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH, 
	MODE_664 = S_IRUSR|S_IWUSR|S_IWGRP|S_IRGRP|S_IROTH|S_IWOTH,
	MODE_600 = S_IRUSR|S_IWUSR
};
#else
enum eyiFileMode
{
	MODE_777 = _S_IREAD|_S_IWRITE,
	MODE_755 = MODE_777, 
	MODE_666 = MODE_777, 
	MODE_444 = _S_IREAD,
	MODE_400 = _S_IREAD, 
	MODE_644 = MODE_777,
	MODE_664 = MODE_777,
	MODE_600 = MODE_777
};
#endif /*__POSIX__*/

// ---------------------------
#define MAX_FILE_NAME 128

typedef struct {
	char name[MAX_FILE_NAME];
	unsigned int size;
	unsigned short type;
	unsigned short checksum;
	char data[16];
} sFileBinaryFormat;

typedef void* SafeFormat;

// --------------------------------------------------------------------
EXTERN_C Bool yiFileRead(const char* pszFileName, void* buffer, int nIndex, int nSize);

// --------------------------------------------------------------------
EXTERN_C void* yiFileSafeLoad(const char* pszFileName, int* nSize);

// --------------------------------------------------------------------
EXTERN_C void yiFileSafeUnload(SafeFormat p);

// --------------------------------------------------------------------
EXTERN_C Bool yiFileSafeWrite(const char* pszFileName, SafeFormat p, int size, Bool bMakeShadow, const char* pszShadowFileName);

// --------------------------------------------------------------------
EXTERN_C SafeFormat yiFileCreateSafeFormat(void* p, int size);

// --------------------------------------------------------------------
EXTERN_C void yiFileDestroySafeFormat(SafeFormat p);

// --------------------------------------------------------------------
EXTERN_C Bool yiFileSafeRestore(const char* pszBadFile, const char* pszBackupFile);

// --------------------------------------------------------------------
EXTERN_C void* yiFileLoad(const char* pszFileName, int* nSize);


// --------------------------------------------------------------------
EXTERN_C const void* yiFileLoadLock(const char* pszFileName, int* nSize);

//
// --------------------------------------------------------------------
EXTERN_C void yiFileUnload(void* p);

// --------------------------------------------------------------------
EXTERN_C void yiFileUnloadUnlock(void* p);

// --------------------------------------------------------------------
EXTERN_C Bool yiFileWrite(const char* pszFileName, void* buffer, int nIndex, int nSize);

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
EXTERN_C Bool yiMakePath(const char* path, Bool bPathContainsFile);

// --------------------------------------------------------------------
EXTERN_C void yiSetBuffer(int n);

// --------------------------------------------------------------------
EXTERN_C int yiFileCreditWrite(const char* pszFile, const void* buffer, int size, int check_sum_interval);


// --------------------------------------------------------------------
#define yiFileCreditAppendRecord(pszFile, record, size, check_sum_interval) yiFileCreditWriteRecord(pszFile, record, -1, size, check_sum_interval)

// --------------------------------------------------------------------
EXTERN_C int yiFileCreditWriteRecord(const char* pszFile, const void* record, int n, int size, int check_sum_interval);

// --------------------------------------------------------------------
EXTERN_C int yiFileCreditRead(const char* pszFile, void* buffer);

// --------------------------------------------------------------------
EXTERN_C void* yiFileCreditLoad(const char* pszFile, int* reason);

// --------------------------------------------------------------------
EXTERN_C void* yiFileCreditLoadEx(const char* pszFile, int* reason);

// --------------------------------------------------------------------
EXTERN_C void yiFileCreditUnloadEx(void* p);

// --------------------------------------------------------------------
EXTERN_C int yiFileCreditReadRecord(const char* pszFile, void* record, int n);

// --------------------------------------------------------------------
EXTERN_C int yiFileCreditReadRecords(const char* pszFile, void* record, int from, int cnt);

// --------------------------------------------------------------------
EXTERN_C void* yiFileCreditLoadRecord(const char* pszFile, int n, int* reason);

// --------------------------------------------------------------------
EXTERN_C void* yiFileCreditLoadRecords(const char* pszFile, int from, int cnt, int* reason);

// --------------------------------------------------------------------
EXTERN_C int yiFileCreditAppendFreeRecord(const char* path, const char* pszDataName, void* data, int size);

// --------------------------------------------------------------------
EXTERN_C void* yiFileCreditLoadFreeRecord(const char* path, const char* pszDataName, int index, int* out_size, int* reason);

// --------------------------------------------------------------------
EXTERN_C void* yiFileCreditUnloadFreeRecord(void* p);

// --------------------------------------------------------------------
EXTERN_C void* yiFileLoadEx(const char* pszFilePath, int* size);

// --------------------------------------------------------------------
EXTERN_C void yiFileUnLoadEx(void* data);

// --------------------------------------------------------------------
EXTERN_C Bool yiFileWriteEx(const char* pszFilePath, int offset, void* data, int size, Bool bBackup);

// --------------------------------------------------------------------
EXTERN_C Bool yiFileCreateEx(const char* pszFilePath, int offset, void* data, int size, Bool bBackup);

// --------------------------------------------------------------------
EXTERN_C Bool yiFileReadEx(const char* pszFilePath, int offset, void* data, int size, Bool bRestore);

// --------------------------------------------------------------------
EXTERN_C Bool yiFileCreateQ(const char* pszFilePath, int count, int size, Bool bOverWritable);//void* data, int size, Bool bRestore);

// --------------------------------------------------------------------
EXTERN_C Bool yiFileEnQ(const char* pszFilePath, void* data);

// --------------------------------------------------------------------
EXTERN_C  Bool yiFileDeQ(const char* pszFilePath, void* data);//void* data, int size, Bool bRestore);

// --------------------------------------------------------------------
EXTERN_C void* yiFileLoadQ(const char* pszFileName, int* n, Bool bIsRemove);//void* data, int size, Bool bRestore);

// --------------------------------------------------------------------
EXTERN_C Bool yiFileWriteEx(const char* pszFilePath, int offset, void* data, int size, Bool bBackup);

// --------------------------------------------------------------------
EXTERN_C Bool yiFileCreateEx(const char* pszFilePath, int offset, void* data, int size, Bool bBackup);

// --------------------------------------------------------------------
EXTERN_C Bool yiFileWriteOnceEx(const char* pszFilePath, const void* buffer, int index, int nSize, Bool bBackup);

// --------------------------------------------------------------------
EXTERN_C Bool yiFileWriteOnce(const char* pszFileName, void* buffer, int nIndex, int nSize);

// --------------------------------------------------------------------
EXTERN_C void yiFileGetNameAndExt(LPCSTR fullpath, LPSTR path, LPSTR name, LPSTR ext);

#ifdef __POSIX__
// --------------------------------------------------------------------
EXTERN_C Bool yiFileMode(const char* pszFilename, int nMode);
#endif /*__POSIX__*/


// --------------------------------------------------------------------
EXTERN_C Handle _yiFileOpen(const char* pszFileName);//void* data, int size, Bool bRestore);

// --------------------------------------------------------------------
EXTERN_C int _yiFileRead(Handle h, void* buffer, int size);//void* data, int size, Bool bRestore);

// --------------------------------------------------------------------
EXTERN_C int _yiFileWrite(Handle h, const void* buffer, int size);//void* data, int size, Bool bRestore);

// --------------------------------------------------------------------
EXTERN_C unsigned int _yiFileSize(Handle h);

// --------------------------------------------------------------------
EXTERN_C unsigned int _yiFileCurpos(Handle h);

// --------------------------------------------------------------------
EXTERN_C void _yiFileClose(Handle h);

// --------------------------------------------------------------------
#ifdef _WIN32
EXTERN_C Bool yiFileCopy(const char* pszOriginalFile, const char* pszNewFile, HWND hWndProgress, int cur, float factor);
#else
EXTERN_C Bool yiFileCopy(const char* pszOriginalFile, const char* pszNewFile);//, HWND hWndProgress, int cur, float factor);
#endif

// --------------------------------------------------------------------
EXTERN_C char* yiMakeTempFileName(char* name);


// --------------------------------------------------------------------
EXTERN_C unsigned int _yiFileSeek(Handle h, int pos, int from);

// --------------------------------------------------------------------
EXTERN_C void _yiFileTrucate(Handle h);

// --------------------------------------------------------------------
EXTERN_C char* yiMakeTempFileName(char* name);

//#ifdef __IOS__
// --------------------------------------------------------------------
EXTERN_C void iosSetHomeDir(const char* pszHomeDir);

// --------------------------------------------------------------------
EXTERN_C const char* iosGetDocumentDir();

// --------------------------------------------------------------------
EXTERN_C const char* iosGetTempDir();

// --------------------------------------------------------------------
EXTERN_C const char* iosGetLibDir();

// --------------------------------------------------------------------
EXTERN_C const char* iosGetHomeDir();
//#endif

#endif
