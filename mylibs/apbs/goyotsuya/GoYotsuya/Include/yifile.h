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

#ifndef WIN32
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

typedef void* SafeFormat;

// --------------------------------------------------------------------
xEXTERN_C unsigned int yiGetCheckSum(void* buffer, int size, int n);

// --------------------------------------------------------------------
xEXTERN_C Bool yiFileRead(const char* pszFileName, void* buffer, int nIndex, int nSize);

// --------------------------------------------------------------------
xEXTERN_C void* yiFileSafeLoad(const char* pszFileName, int* nSize);

// --------------------------------------------------------------------
xEXTERN_C void yiFileSafeUnload(SafeFormat p);

// --------------------------------------------------------------------
xEXTERN_C Bool yiFileSafeWrite(const char* pszFileName, SafeFormat p, int size, Bool bMakeShadow, const char* pszShadowFileName);

// --------------------------------------------------------------------
xEXTERN_C SafeFormat yiFileCreateSafeFormat(void* p, int size);

// --------------------------------------------------------------------
xEXTERN_C void yiFileDestroySafeFormat(SafeFormat p);

// --------------------------------------------------------------------
xEXTERN_C Bool yiFileSafeRestore(const char* pszBadFile, const char* pszBackupFile);

// --------------------------------------------------------------------
xEXTERN_C void* yiFileLoad(const char* pszFileName, int* nSize);
//
// --------------------------------------------------------------------
xEXTERN_C void yiFileUnload(void* p);

// --------------------------------------------------------------------
xEXTERN_C Bool yiFileWrite(const char* pszFileName, void* buffer, int nIndex, int nSize);

// --------------------------------------------------------------------
xEXTERN_C Bool yiFileWriteOnce(const char* pszFileName, void* buffer, int nIndex, int nSize);


// --------------------------------------------------------------------
xEXTERN_C int yiFileSize(const char* pszFileName);

// --------------------------------------------------------------------
xEXTERN_C Bool yiIsExist(const char* path, Bool* pIsDir);
//
// --------------------------------------------------------------------
xEXTERN_C Bool yiIsDir(const char* path);

// --------------------------------------------------------------------
xEXTERN_C Bool yiMoveFile(const char* src, const char* dest);

// --------------------------------------------------------------------
xEXTERN_C Bool yiRemove(const char* filename, Bool bIsForced);

// --------------------------------------------------------------------
xEXTERN_C Bool yiCopyFile(const char* src, const char* dest);
//
// --------------------------------------------------------------------
xEXTERN_C Bool yiMakeDir(const char* path);

// --------------------------------------------------------------------
xEXTERN_C Bool yiMakePath(const char* path, Bool bPathContainsFile);

// --------------------------------------------------------------------
xEXTERN_C void yiSetBuffer(int n);

/*
// --------------------------------------------------------------------
xEXTERN_C int yiFileCreditWrite(const char* pszFile, const void* buffer, int size, int check_sum_interval);


// --------------------------------------------------------------------
#define yiFileCreditAppendRecord(pszFile, record, size, check_sum_interval) yiFileCreditWriteRecord(pszFile, record, -1, size, check_sum_interval)

// --------------------------------------------------------------------
xEXTERN_C int yiFileCreditWriteRecord(const char* pszFile, const void* record, int n, int size, int check_sum_interval);

// --------------------------------------------------------------------
xEXTERN_C int yiFileCreditRead(const char* pszFile, void* buffer);

// --------------------------------------------------------------------
xEXTERN_C void* yiFileCreditLoad(const char* pszFile, int* reason);

// --------------------------------------------------------------------
xEXTERN_C void* yiFileCreditLoadEx(const char* pszFile, int* reason);

// --------------------------------------------------------------------
xEXTERN_C void yiFileCreditUnloadEx(void* p);

// --------------------------------------------------------------------
xEXTERN_C int yiFileCreditReadRecord(const char* pszFile, void* record, int n);

// --------------------------------------------------------------------
xEXTERN_C int yiFileCreditReadRecords(const char* pszFile, void* record, int from, int cnt);

// --------------------------------------------------------------------
xEXTERN_C void* yiFileCreditLoadRecord(const char* pszFile, int n, int* reason);

// --------------------------------------------------------------------
xEXTERN_C void* yiFileCreditLoadRecords(const char* pszFile, int from, int cnt, int* reason);

// --------------------------------------------------------------------
xEXTERN_C int yiFileCreditAppendFreeRecord(const char* path, const char* pszDataName, void* data, int size);

// --------------------------------------------------------------------
xEXTERN_C void* yiFileCreditLoadFreeRecord(const char* path, const char* pszDataName, int index, int* out_size, int* reason);

// --------------------------------------------------------------------
xEXTERN_C void yiFileCreditUnloadFreeRecord(void* p);
*/
// --------------------------------------------------------------------
xEXTERN_C const char* yiPathGetFileName(const char* pszFullPath);

typedef Handle HandleFileStream;

// --------------------------------------------------------------------
xEXTERN_C void yiFileStreamClose(HandleFileStream hStream);

// --------------------------------------------------------------------
xEXTERN_C Bool yiFileStreamWrite(HandleFileStream hStream, const void* data, int size);

// --------------------------------------------------------------------
xEXTERN_C HandleFileStream yiFileStreamOpen(const char* pszFileName);

// --------------------------------------------------------------------
// 2010.9.11 ’Ç‰Á
xEXTERN_C Bool yiGetExtens(const char* pszFileName, char* buffer);

// --------------------------------------------------------------------
xEXTERN_C Bool yiFileOpenDialog(HWND hWndParent, LPCSTR title, LPCSTR initPath, LPCSTR Filters, LPCSTR FilterLabel, LPSTR out_filename);

// --------------------------------------------------------------------
xEXTERN_C Bool yiFileSaveDialog(HWND hWndParent, LPCSTR title, LPCSTR initPath, LPCSTR Filters, LPCSTR FilterLabel, LPSTR out_filename);

// --------------------------------------------------------------------
xEXTERN_C LPCSTR yiPathGetFileName(LPCSTR fullpath);

// --------------------------------------------------------------------
xEXTERN_C void yiFileGetNameAndExt(LPCSTR fullpath, LPSTR path, LPSTR name, LPSTR ext);

// --------------------------------------------------------------------
xEXTERN_C void yiFileGetNameAndExtW(const wchar_t* fullpath, wchar_t* path, wchar_t* name, wchar_t* ext);

// --------------------------------------------------------------------
xEXTERN_C Bool yiRemoveForced(const char* dir, Bool bIsMoveTrush);


// --------------------------------------------------------------------
xEXTERN_C Bool yiFileOpenPath(HWND hWndParent, LPCSTR title, LPSTR path);

#ifdef __POSIX__
// --------------------------------------------------------------------
xEXTERN_C Bool yiFileMode(const char* pszFilename, int nMode);
#endif /*__POSIX__*/

// --------------------------------------------------------------------
xEXTERN_C void* yiFileLoadEx(const char* pszFilePath, int* size);

// --------------------------------------------------------------------
xEXTERN_C void yiFileUnLoadEx(void* data);

// --------------------------------------------------------------------
xEXTERN_C Bool yiFileWriteEx(const char* pszFilePath, int offset, void* data, int size, Bool bBackup);

// --------------------------------------------------------------------
xEXTERN_C Bool yiFileCreateEx(const char* pszFilePath, int offset, void* data, int size, Bool bBackup);

// --------------------------------------------------------------------
xEXTERN_C Bool yiFileReadEx(const char* pszFilePath, int offset, void* data, int size, Bool bRestore);

// --------------------------------------------------------------------
xEXTERN_C Bool yiFileCreateQ(const char* pszFilePath, int count, int size, Bool bOverWritable);//void* data, int size, Bool bRestore);

// --------------------------------------------------------------------
xEXTERN_C Bool yiFileEnQ(const char* pszFilePath, void* data);

// --------------------------------------------------------------------
xEXTERN_C  Bool yiFileDeQ(const char* pszFilePath, void* data);//void* data, int size, Bool bRestore);

// --------------------------------------------------------------------
xEXTERN_C void* yiFileLoadQ(const char* pszFileName, int* n, Bool bIsRemove);//void* data, int size, Bool bRestore);


#endif
