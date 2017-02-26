#ifndef _yiw32_h__
#define _yiw32_h__

#ifndef _WIN32

#include "yi.h"

#define __RPC__deref_out_opt 
#define STDMETHODCALLTYPE 
#define HRESULT long
#define S_OK 1
#define S_FAIL 0
#define S_FALSE 0
#define STDMETHODCALLTYPE 

typedef unsigned long ULONG;
typedef long LONG;

typedef ubit32_t DWORD;


typedef IntPtr DWORD_PTR;
typedef short WORD;
typedef char  BYTE;

#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))


typedef struct {
	int x;
	int y;
} POINT,*LPPOINT;

//typedef int BOOL;

//#define TRUE (1)
//#define FALSE (0)

typedef char* 	LPSTR;
typedef const char* 	LPCSTR;
typedef const char* 	LPCTSTR;
typedef void* 		HGLOBAL;

typedef ubit64_t UINT64;
typedef bit64_t INT64;
typedef ubit32_t UINT32;

typedef UINT64 ULONGLONG;
typedef INT64 LONGLONG;

typedef void* HWND;

typedef union _LARGE_INTEGER {
	struct {
		DWORD LowPart;
		LONG  HighPart;
    	};
	struct {
		DWORD LowPart;
		LONG  HighPart;
	} u;
	bit64_t QuadPart;
} LARGE_INTEGER;

typedef union _ULARGE_INTEGER {
	struct {
		DWORD LowPart;
		DWORD HighPart;
    	};
	struct {
		DWORD LowPart;
		DWORD HighPart;
	} u;
	ubit64_t QuadPart;
} ULARGE_INTEGER;

typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} GUID;

typedef GUID REFIID;
typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

typedef GUID CLSID;
typedef wchar_t* LPOLESTR;

extern const CLSID CLSID_NULL;

#define __RPC__deref_out 
#define __RPC_FAR 

typedef
enum tagSTATFLAG
{	STATFLAG_DEFAULT= 0,
	STATFLAG_NONAME	= 1,
	STATFLAG_NOOPEN	= 2
} STATFLAG;

typedef 
enum tagSTREAM_SEEK
    {	STREAM_SEEK_SET	= 0,
	STREAM_SEEK_CUR	= 1,
	STREAM_SEEK_END	= 2
    } 	STREAM_SEEK;

typedef struct tagSTATSTG
{
    LPOLESTR pwcsName;
    DWORD type;
    ULARGE_INTEGER cbSize;
    FILETIME mtime;
    FILETIME ctime;
    FILETIME atime;
    DWORD grfMode;
    DWORD grfLocksSupported;
    CLSID clsid;
    DWORD grfStateBits;
    DWORD reserved;
}STATSTG;


#define __RPC__out    

#define wsprintf sprintf

#ifdef __cplusplus
class IStream 
{
public:

virtual /* [local] */ HRESULT STDMETHODCALLTYPE Read( 
            /* [length_is][size_is][out] */ void *pv,
            /* [in] */ ULONG cb,
            /* [out] */ ULONG *pcbRead) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Write( 
            /* [size_is][in] */ const void *pv,
            /* [in] */ ULONG cb,
            /* [out] */ ULONG *pcbWritten) = 0;
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Seek( 
            /* [in] */ LARGE_INTEGER dlibMove,
            /* [in] */ DWORD dwOrigin,
            /* [out] */ ULARGE_INTEGER *plibNewPosition) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSize( 
            /* [in] */ ULARGE_INTEGER libNewSize) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE CopyTo( 
            /* [unique][in] */ IStream *pstm,
            /* [in] */ ULARGE_INTEGER cb,
            /* [out] */ ULARGE_INTEGER *pcbRead,
            /* [out] */ ULARGE_INTEGER *pcbWritten) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Commit( 
            /* [in] */ DWORD grfCommitFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Revert( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LockRegion( 
            /* [in] */ ULARGE_INTEGER libOffset,
            /* [in] */ ULARGE_INTEGER cb,
            /* [in] */ DWORD dwLockType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnlockRegion( 
            /* [in] */ ULARGE_INTEGER libOffset,
            /* [in] */ ULARGE_INTEGER cb,
            /* [in] */ DWORD dwLockType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Stat( 
            /* [out] */ __RPC__out STATSTG *pstatstg,
            /* [in] */ DWORD grfStatFlag) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ __RPC__deref_out_opt IStream **ppstm) = 0;
        
    	virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

    	virtual ULONG STDMETHODCALLTYPE Release( void) = 0;
    };
#endif // __cplusplus
#endif

#endif
