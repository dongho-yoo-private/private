// usage
//  IO::Stream stream;

//  stream.CreateOnResource((HINSTANCE)::GetWindowLong(m_hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDB_PNG1), L"PNG");

//  Image img(stream);
//  g.DrawImage(&img, 0, 0);

#ifdef _WIN32
#pragma comment(lib, "zlib.lib")
#endif
 
// Stream.cpp
#define _WRITE_ONCE_SIZE_ (256*1024)
//#include "stdafx.h"

#include "yiStream.h"
#include "yistd.h"

#ifndef _WIN32
#define STDMETHODIMP_(a) a
#define STDMETHODIMP HRESULT
#endif

#ifndef _WIN32
const CLSID CLSID_NULL = {0, 0, 0, {0, 0, 0, 0}};
#endif

namespace IO
{
 namespace Private
 {
  ////////////////////////////////////////////////////////////
  // MemoryStream
  // 渡されたメモリの解放などは行わない
  // TODO: ポリシーベースの解放処理？
  class xEXPORT MemoryStream : public IStream
  {
  private:
   const BYTE *m_pBegin, *m_pEnd, *m_pCurrent;
	Bool m_bIsMemorySelf;
   volatile LONG m_ReferenceCount;
  public:
   MemoryStream(const void* data, IntPtr size) : m_ReferenceCount(1)
   {
    m_pBegin = m_pCurrent = (BYTE*)data;
    m_pEnd = m_pBegin + size;
	m_bIsMemorySelf=False;
   }
   //MemoryStream(const char* pszFileName) : m_ReferenceCount(1)
   //{
	  // IntPtr size;
	  // m_pBegin=m_pCurrent=(BYTE*)yiFileLoad(pszFileName, &size);

	  // if (m_pBegin==0)
	  // {
		 //  m_pEnd=0;
	  // }
	  // else
	  // {
		 //  m_pEnd = m_pBegin + size;
	  // }
	  // m_bIsMemorySelf=True;
   //}
  protected:
   ~MemoryStream()
   {
    ASSERT(m_ReferenceCount == 0);

	//if (m_bIsMemorySelf==True)
	//	yifree(m_pBegin);
		
	m_pBegin=0;
   }
  public:
   // メモリ空間の問題に対処するために operator new/delete を実装したほうが良いのかも
   // void* operator new (size_t size);
   // void operator delete (void* ptr);
   STDMETHODIMP_(ULONG) AddRef()
   {
#ifdef _WIN32
    return ::InterlockedIncrement(&m_ReferenceCount);
#else
	return 0;
#endif
   }
   STDMETHODIMP_(ULONG) Release()
   {
#ifdef _WIN32
    ULONG refs = ::InterlockedDecrement(&m_ReferenceCount);
    if(refs == 0)
    {
     delete this;
    }
    return refs;
#else
	return 0;
#endif
   }
   STDMETHODIMP QueryInterface(REFIID iid, void** ppObj)
   {
#ifdef _WIN32
    ASSERT(*ppObj == NULL);
    if(iid == __uuidof(IStream) || iid == __uuidof(ISequentialStream) || iid == __uuidof(IUnknown))
    {
     *ppObj = static_cast<IStream*>(this);
     AddRef();
     return S_OK;
    }
    *ppObj = NULL;
    return E_NOINTERFACE;
#endif
	return S_OK;
   }
   STDMETHODIMP Read(void* buffer, ULONG size, ULONG* done)
   {
    ASSERT(m_pEnd >= m_pCurrent);
    uIntPtr remain = (uIntPtr)(m_pEnd - m_pCurrent);
    if(size > remain) size = remain;
    memcpy(buffer, m_pCurrent, size);
    m_pCurrent += size;
    if(done) *done = size;
    return size > 0 ? S_OK : S_FALSE;
   }
   STDMETHODIMP Seek(LARGE_INTEGER move, DWORD method, ULARGE_INTEGER* newpos)
   {
    switch(method)
    {
    case STREAM_SEEK_SET:
     if(move.QuadPart < 0 || (ULONGLONG)move.QuadPart > GetSize())
      return STG_E_INVALIDPOINTER;
     m_pCurrent = m_pBegin + move.QuadPart;
     break;
    case STREAM_SEEK_CUR:
    {
     const BYTE* pos = m_pCurrent + move.QuadPart;
     if(pos < m_pBegin || pos > m_pEnd)
      return STG_E_INVALIDPOINTER;
     m_pCurrent = pos;
     break;
    }
    case STREAM_SEEK_END:
     if(move.QuadPart > 0 || (ULONGLONG)(-move.QuadPart) > GetSize())
      return STG_E_INVALIDPOINTER;
     m_pCurrent = m_pEnd + move.QuadPart;
     break;
    default:
     //TRESPASS();
     return E_INVALIDARG;
    }
    if(newpos) newpos->QuadPart = m_pCurrent - m_pBegin;
    return S_OK;
   }
   STDMETHODIMP Clone(IStream** ppStream)
   {
    *ppStream = new MemoryStream(m_pBegin, (size_t)(m_pEnd - m_pBegin));
    return S_OK;
   }
   STDMETHODIMP Stat(STATSTG* pStat, DWORD flag)
   {
    if(flag != STATFLAG_NONAME)
    { // とりあえず名前なし
     pStat->pwcsName[0] = L'\0';
    }
    else
    { // NULL にしておかないと未初期化ポインタ解放で死ぬアプリケーションあり
     //（GDI+でPNGを読み込んだ場合など；ちなみに、PNG以外はなぜか大丈夫）
     pStat->pwcsName = NULL;
    }
	ULONGLONG size = GetSize();
    pStat->type     = STGTY_LOCKBYTES;
    pStat->cbSize.QuadPart  = size;//(m_pEnd-m_pBegin);//(ULONGLONG)GetSize();

	ULONGLONG* psize=&pStat->cbSize.QuadPart;

	*psize=size;
    pStat->grfMode    = STGM_READ | STGM_SHARE_DENY_WRITE;
    pStat->grfLocksSupported = 0;
    pStat->clsid    = CLSID_NULL;
    pStat->grfStateBits   = 0;
    return S_OK;
   }
   // 未実装のメソッド
   STDMETHODIMP Commit(DWORD flag) { return E_NOTIMPL; }
   STDMETHODIMP LockRegion(ULARGE_INTEGER offset, ULARGE_INTEGER size, DWORD locktype)  { return E_NOTIMPL; }
   STDMETHODIMP UnlockRegion(ULARGE_INTEGER offset, ULARGE_INTEGER size, DWORD locktype) { return E_NOTIMPL; }
   STDMETHODIMP CopyTo(IStream* pStream, ULARGE_INTEGER size, ULARGE_INTEGER* read, ULARGE_INTEGER* written) { return E_NOTIMPL; }
   // 無効なメソッド
   STDMETHODIMP Write(const void* buffer, ULONG size, ULONG* done) { return STG_E_ACCESSDENIED; }
   STDMETHODIMP SetSize(ULARGE_INTEGER size) { return STG_E_ACCESSDENIED; }
   STDMETHODIMP Revert()   { return STG_E_ACCESSDENIED; }
   // 
   ULONGLONG  GetSize() { 
	   ULONGLONG size = m_pEnd - m_pBegin;
	   return size; 
   }
  };
 }

 HRESULT Stream::CreateOnFile(LPCTSTR filename, DWORD mode)
 {
#ifdef _WIN32
  ASSERT(m_pStream == NULL);
  HRESULT hr = ::SHCreateStreamOnFile(filename, mode, &m_pStream);
  if FAILED(hr)
  {
   //TRACE("@Stream::CreateOnFile(filename=", filename, ") : HRESULT=", hr);
   return hr;
  }
#endif
  return S_OK;
 }
 HRESULT Stream::CreateOnHGlobal(HGLOBAL hGlobal, BOOL deleteOnRelease)
 {
#ifdef _WIN32
  ASSERT(m_pStream == NULL);
  return ::CreateStreamOnHGlobal(hGlobal, deleteOnRelease, &m_pStream);
#endif
	return S_OK;
 }
#ifdef _WIN32
 HRESULT Stream::CreateOnResource(HINSTANCE hInstance, LPCTSTR name, LPCTSTR type)
 {
  ASSERT(m_pStream == NULL);
  HRSRC hResInfo = ::FindResource(hInstance, name, type);
  void* data = ::LockResource(::LoadResource(hInstance, hResInfo));
  if(data == NULL)
  {
   //TRACE("@Stream::CreateOnResource(name=", name, ", type=", type, ") : Resource not found.");
   //::MessageBox(NULL, L"load error", L"ERROR", MB_OK);
   return E_FAIL;
  }
  m_pStream = new Private::MemoryStream(data, ::SizeofResource(hInstance, hResInfo));
  return S_OK;
 }
#endif
 HRESULT Stream::CreateOnMemory(void* data, int size)
 {
	 m_pStream = new Private::MemoryStream(data, size);
	 return S_OK;
 }

}

 
void* memcpy(void* p, cyiBaseStream* stream, int n)
{
	if (stream->Read(p, n, 0)==S_OK)
	{
		return p;
	}
	return 0;
}

void* memcpy(cyiBaseStream* stream, void* p, int n)
{
	if (stream->Write(p, n, 0)==S_OK)
	{
		return p;
	}
	return 0;
}

void* memcpy(cyiBaseStream* dest, cyiBaseStream* src, int n)
{
	ULARGE_INTEGER ri;

	ri.LowPart=n;
	ri.HighPart=0;
	if (src->CopyTo(dest, ri, 0, 0)==S_OK)
	{
		return (void*)dest;
	}
	return 0;
}

int memcmp(cyiBaseStream* stream, void* p, int n)
{
	ULONG nRead=0;
	void* pDest = yialloc(n);
	stream->Read(pDest, n, &nRead);

	if (nRead<n)
	{
		yifree(pDest);
		return 0;
	}

	int res = memcmp(pDest, p, n);

	yifree(pDest);
	return res;
}

int memcmp(void* p, cyiBaseStream* stream, int n)
{
	ULONG nRead=0;
	void* pDest = yialloc(n);
	stream->Read(pDest, n, &nRead);

	if (nRead<n)
	{
		yifree(pDest);
		return 0;
	}

	int res = memcmp(pDest, p, n);

	yifree(pDest);
	return res;
}

int memcmp(cyiBaseStream* dest, cyiBaseStream* src, int n)
{
	ULONG nRead=0;
	ULONG nRead2=0;
	void* pDest = yialloc(n);
	void* pSrc  = yialloc(n);

	dest->Read(pDest, n, &nRead);
	src->Read(pSrc, n, &nRead2);

	if (nRead!=nRead2)
	{
		yifree(pDest);
		yifree(pSrc);
		return -1;
	}

	int res = memcmp(pDest, pSrc, nRead);

	yifree(pDest);
	yifree(pSrc);
	return res;
}



cyiMemoryStream::cyiMemoryStream(int nMinumBufferSize, unsigned int nMaxMemorySize)
: m_list(0)
, m_nTotalSize(0)
, m_nCurrentPos(0)
, m_nMinumPageSize(nMinumBufferSize)
, m_pszFileName(0)
, m_nMaxMemorySize(nMaxMemorySize)
, m_ptr(0)
, m_nStreamOffset(0)
, m_pReference(0)
{
	m_list = new cyiList(True);


}

cyiMemoryStream::~cyiMemoryStream()
{
	if (m_list)
	{
		int cnt = m_list->count();
		for (int i=0; i<cnt; i++)
			yifree(m_list->removeEx(0));

		delete m_list;
	}



	if (m_pszFileName)
		yifree(m_pszFileName);
}

HRESULT STDMETHODCALLTYPE cyiMemoryStream::Seek( 
        /* [in] */ LARGE_INTEGER dlibMove,
        /* [in] */ DWORD dwOrigin,
        /* [out] */ ULARGE_INTEGER *plibNewPosition)
{
	
	if (dwOrigin==STREAM_SEEK_CUR)
	{
		m_nCurrentPos+=dlibMove.LowPart;
	}
	else if (dwOrigin==STREAM_SEEK_END)
	{
		m_nCurrentPos=m_nTotalSize-dlibMove.LowPart;
	}
	else
	{
		m_nCurrentPos=dlibMove.LowPart;
	}

	if (m_nCurrentPos<0)
		m_nCurrentPos=0;
	if (m_nCurrentPos>m_nTotalSize)
		m_nCurrentPos=m_nTotalSize;

	if (plibNewPosition)
	{
		plibNewPosition->HighPart=0;
		plibNewPosition->LowPart=m_nCurrentPos;
	}

	return S_OK;
}

unsigned int cyiMemoryStream::lseek(unsigned int pos, int from)
{
	
	if (from==SEEK_CUR)
	{
		m_nCurrentPos+=pos;
	}
	else if (from==SEEK_END)
	{
		m_nCurrentPos=m_nTotalSize-pos;
	}
	else
	{
		m_nCurrentPos=pos;
	}

	if (m_nCurrentPos<0)
		m_nCurrentPos=0;

	if (m_nCurrentPos>m_nTotalSize)
		m_nCurrentPos=m_nTotalSize;

	return S_OK;
}

#define _MAX_CHECK_SUM_SIZE (10*1024*1024) // -> 10MByte以上ならば、全体のサイズから回数で行う。
//#define _MAX_CHECK_SUM_CNT  (_MAX_CHECK_SUM_SIZE/_DEFAULT_CHECK_SUM)

// ----------------------
#ifdef _WIN32
unsigned int cyiBaseStream::GetCheckSum(int nInterval, HWND hWnd, int curPos, float factor)
#else
unsigned int cyiBaseStream::GetCheckSum(int nInterval)//, HWND hWnd, int curPos, float factor)
#endif
{
	unsigned int size = GetSize();
	unsigned int check_sum=0;
	unsigned int temp = ftell();

	lseek(0, SEEK_SET);

	if (size<=nInterval)
	{
		nInterval=1;
	}

	if (size>_MAX_CHECK_SUM_SIZE)
	{
		int cnt = _MAX_CHECK_SUM_SIZE/128;
		nInterval = size/cnt;
	}

#ifdef _WIN32
	if (hWnd!=0 && curPos==-1)
	{
		curPos = SendMessage(hWnd, PBM_GETPOS, 0, 0);
	}
#endif

	while(1)//p<(((unsigned char*)buffer)+size))
	{
		unsigned int buffer;
		unsigned long ret=0;
		unsigned int cur = ftell();

		if (Read(&buffer, 4, &ret)!=S_OK)
			break;

		check_sum+=buffer;
		lseek(cur+nInterval, SEEK_SET);
		//check_sum+= (unsigned int)*p;
		//p+=n;

#ifdef _WIN32
		if (hWnd)
		{
			SendMessage(hWnd, PBM_SETPOS, (WPARAM)(curPos+(cur)*factor), 0);
		}
#endif
	}

	lseek(temp, SEEK_SET);
	return check_sum;
}
    
HRESULT STDMETHODCALLTYPE cyiMemoryStream::SetSize( 
        /* [in] */ ULARGE_INTEGER libNewSize)
{
	m_nTotalSize=libNewSize.LowPart;
	if (m_nCurrentPos>m_nTotalSize)
		m_nCurrentPos=m_nTotalSize;
	return S_OK;
}
    


HRESULT STDMETHODCALLTYPE cyiMemoryStream::CopyTo( 
    /* [unique][in] */ IStream *pstm,
    /* [in] */ ULARGE_INTEGER cb,
    /* [out] */ ULARGE_INTEGER *pcbRead,
    /* [out] */ ULARGE_INTEGER *pcbWritten)
{
	if (pstm==0)
		return S_FALSE;

	//STATSTG stat;
	//pstm->Stat(&stat, STATFLAG_NONAME|STATFLAG_NOOPEN);

	unsigned long written=0;
	//void* p		= yialloc(cb.LowPart);

	ULONG nRead=0;
	int   nRemain = cb.LowPart;
	char* buffer = (char*)yialloc(_WRITE_ONCE_SIZE_);
	while(nRemain)
	{
		int xxxx = nRemain<_WRITE_ONCE_SIZE_?nRemain:_WRITE_ONCE_SIZE_;

		if (Read(buffer, xxxx, &nRead)!=S_OK)
			break;

		if (pcbRead)
		{
			pcbRead->LowPart+=nRead;
			pcbRead->HighPart=0;
		}

		if (pstm->Write(buffer, nRead, &nRead)!=S_OK)
			break;

		if (pcbWritten)
		{
			pcbWritten->LowPart+=nRead;
			pcbWritten->HighPart=0;
		}

		nRemain-=nRead;
	}
	yifree(buffer);
	return S_OK;
}
    
HRESULT STDMETHODCALLTYPE cyiMemoryStream::Commit( 
        /* [in] */ DWORD grfCommitFlags)
{
	return S_OK;
}
    
HRESULT STDMETHODCALLTYPE cyiMemoryStream::Revert( void)
{
	return S_OK;
}
    
HRESULT STDMETHODCALLTYPE cyiMemoryStream::LockRegion( 
        /* [in] */ ULARGE_INTEGER libOffset,
        /* [in] */ ULARGE_INTEGER cb,
        /* [in] */ DWORD dwLockType)
{
	m_list->lock();
	return S_OK;
}
    
HRESULT STDMETHODCALLTYPE cyiMemoryStream::UnlockRegion( 
        /* [in] */ ULARGE_INTEGER libOffset,
        /* [in] */ ULARGE_INTEGER cb,
        /* [in] */ DWORD dwLockType)
{
	m_list->unlock();
	return S_OK;
}
    
HRESULT STDMETHODCALLTYPE cyiMemoryStream::Stat( 
        /* [out] */ __RPC__out STATSTG *pstatstg,
        /* [in] */ DWORD grfStatFlag)
{
	pstatstg->cbSize.HighPart=0;
	pstatstg->cbSize.LowPart=m_nTotalSize;
	pstatstg->pwcsName=0;
	return S_OK;
}
    
HRESULT STDMETHODCALLTYPE cyiMemoryStream::Clone( 
        /* [out] */ __RPC__deref_out_opt IStream **ppstm)
{
	return S_OK;
}

void cyiMemoryStream::SetPointer(void* p, int size)
{
	m_ptr=(unsigned char*)p;
	m_nTotalSize=size;
}

void cyiMemoryStream::SetStream(cyiBaseStream* stream, int cur, int size)
{
	m_nTotalSize=size;
	m_nStreamOffset=cur;

	m_pReference=stream;
}

unsigned int cyiMemoryStream::ftell()
{
	return m_nCurrentPos;
}

unsigned int cyiMemoryStream::GetSize()
{
	return m_nTotalSize;
}

Binary cyiMemoryStream::ExportToBinary()
{
	void* p = yialloc(m_nTotalSize+sizeof(unsigned int));
	unsigned int* pIndex = (unsigned int*)p;

	unsigned long   readz;
	unsigned int nBeforePos=m_nCurrentPos;
	
	m_nCurrentPos=0;
	if (Read(&pIndex[1], m_nTotalSize, &readz)!=S_OK)
	{
		yifree(p);
		m_nCurrentPos=nBeforePos;
		return 0;
	}
	pIndex[0]=m_nTotalSize;

	return (Binary)&pIndex[1];

}


#ifdef _WIN32
Bool cyiMemoryStream::ExportToFile(const char* pszFileName, HWND hWndProgress, int cur, float factor)
#else
Bool cyiMemoryStream::ExportToFile(const char* pszFileName)//, HWND hWndProgress, int cur, float factor)
#endif
{
	void* p = yialloc(m_nTotalSize);
	unsigned long   readz;
	unsigned int nBeforePos=m_nCurrentPos;
	
	m_nCurrentPos=0;
	if (Read(p, m_nTotalSize, &readz)!=S_OK)
	{
		yifree(p);
		m_nCurrentPos=nBeforePos;
		return False;
	}

	Bool res = yiFileWrite(pszFileName, p, 0, readz);
	yifree(p);
	m_nCurrentPos=nBeforePos;
	return res;
}

Bool cyiMemoryStream::ImportFromFile(const char* pszFileName)
{
	int size=0;
	void* p = yiFileLoad(pszFileName, &size);

	if (p)
	{
		unsigned long written=0;
		if (Write(p, size, &written)!=S_OK)
		{
			yifree(p);
			return False;
		}

		yifree(p);
		
		if (m_pszFileName)
			yifree(m_pszFileName);

		m_pszFileName=yistrheap(pszFileName);
		m_nCurrentPos=0;
		return True;
	}


	return False;
}

HRESULT STDMETHODCALLTYPE cyiMemoryStream::Read( 
    /* [length_is][size_is][out] */ void *pv,
    /* [in] */ ULONG cb,
    /* [out] */ ULONG *pcbRead)
{
	char* pIndex=(char*)pv;
	int sum=0;

	if (m_nCurrentPos==m_nTotalSize)
	{
		// EOF
		return S_FALSE;
	}

	if (m_pReference)
	{
		unsigned long readz=0;
		unsigned int temp = m_pReference->ftell();
		m_pReference->lseek(m_nStreamOffset+m_nCurrentPos, SEEK_SET);

		if (m_nCurrentPos+cb>m_nTotalSize)
		{
			cb = m_nTotalSize-m_nCurrentPos;
		}

		HRESULT result = m_pReference->Read(pv, cb, &readz);
		m_nCurrentPos+=readz;
		m_pReference->lseek(temp, SEEK_SET);

		if (pcbRead)
			*pcbRead=readz;

		return result;
	}

	if (m_ptr)
	{
		int readable	= m_nTotalSize-m_nCurrentPos;
		readable		= cb>readable?readable:cb;

		memcpy(pv, &m_ptr[m_nCurrentPos], readable);

		if (pcbRead)
			*pcbRead=readable;

		m_nCurrentPos+=readable;
		return S_OK;
	}

	while(sum!=cb)
	{
		int nReadCount=cb-sum;
		if (nReadCount+m_nCurrentPos>m_nTotalSize)
		{
			nReadCount=m_nTotalSize-m_nCurrentPos;
		}

		int n = xRead(&pIndex[sum], m_nCurrentPos, nReadCount);

		if (n==0)
			break;

		m_nCurrentPos+=n;
		sum+=n;
	}	

	if (pcbRead!=0)
		*pcbRead=sum;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE cyiMemoryStream::Write( 
    /* [size_is][in] */ const void *pv,
    /* [in] */ ULONG cb,
    /* [out] */ ULONG *pcbWritten)
{
	char* pIndex=(char*)pv;
	int sum=0;

	if (m_ptr)
	{
		return S_FALSE;
	}

	while(sum!=cb)
	{
		int n = xWrite(&pIndex[sum], m_nCurrentPos, cb-sum);
		m_nCurrentPos+=n;
		sum+=n;
	}

	if (pcbWritten!=0)
		*pcbWritten=sum;

	m_nTotalSize=m_nCurrentPos>m_nTotalSize?m_nCurrentPos:m_nTotalSize;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE cyiMemoryStream::QueryInterface( 
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject)
{
	return S_OK;
}

ULONG STDMETHODCALLTYPE cyiMemoryStream::AddRef( void)
{
	return 1;
}

ULONG STDMETHODCALLTYPE cyiMemoryStream::Release( void)
{
	return 0;
}

int cyiMemoryStream::xGetPage(unsigned int pos)
{
	int nTotalPageCnt = m_list->count();
	int nCurrentPage = -1;//m_nTotalSize;//m_list->count()-1;
	int sum=0;

	for (int i=0; i<nTotalPageCnt; i++)
	{
		sum+=m_nMinumPageSize;
		if (m_nCurrentPos<sum)
		{
			nCurrentPage=i;
			break;
		}
	}

	return nCurrentPage;
}

int cyiMemoryStream::xGetWritableSize(unsigned int pos, unsigned int size)
{
	int n = xGetPage(pos);

	if (n<0)
	{
		return 0;
	}

	int nPageInPos = pos-n*(m_nMinumPageSize);


	if (size<=m_nMinumPageSize-nPageInPos)
	{
		return size;
	}

	return (m_nMinumPageSize-nPageInPos);
}

int cyiMemoryStream::xWrite(void* buffer, unsigned int pos, unsigned int size)
{
	char* p=0;

	int n = xGetPage(pos);

	if (n<0)
	{
		p=(char*)yialloc(m_nMinumPageSize);
		m_list->addref(p);

		n=xGetPage(pos);

		if (n<0)
			return 0;
	}

	int nPageInPos = pos-n*(m_nMinumPageSize);

	cyiNode* node = m_list->GetNodeTopFrom(n);

	if (node==0)
	{
		p=(char*)yialloc(m_nMinumPageSize);
		m_list->addref(p);
	}
	else
	{
		p = (char*)node->ref;
	}

	int x = xGetWritableSize(pos, size);

	memcpy(&p[nPageInPos], buffer, x);

	return x;
}

int cyiMemoryStream::xRead(void* buffer, unsigned int pos, int size)
{
	int n = xGetPage(pos);
	int nPageInPos = pos-n*(m_nMinumPageSize);
	int maxReadSize = m_nMinumPageSize-nPageInPos;

	if (maxReadSize<=0)
	{
		return 0;
	}

	cyiNode* node = m_list->GetNodeTopFrom(n);

	if (node==0)
	{
		// EOF
		return 0;
	}

	if (size>maxReadSize)
	{
		size = maxReadSize;
	}

	char* p = (char*)node->ref;
	memcpy(buffer, &p[nPageInPos], size);

	return size;
}

#ifdef _WIN32
#include "zlib/zlib.h"
#else
#include "zlib.h"
#endif

#define _ZLIB_CHUNK_ 16384

#define MAX_MEMORY_SIZE (1024*1024*20)  // -> 20MBまではメモリ上で直接展開

// -------------------------------
cyiBaseStream* yiUtilCompress(void* p, int size, int* progress)
{
	cyiBaseStream* stream=0;
    int flush, ret;
    z_stream strm;
    //unsigned char in[_ZLIB_CHUNK_];
    unsigned char out[_ZLIB_CHUNK_];
	unsigned char* pIndex = (unsigned char*)p;
	int all=size;
	int sum=0;
	int status=0;
	unsigned int* pCompressed=0;

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, Z_BEST_COMPRESSION);
    if (ret != Z_OK)
        return 0;

	strm.avail_in=0;
    strm.next_out = out; /* 出力バッファ残量を元に戻す */
    strm.avail_out = _ZLIB_CHUNK_; /* 出力ポインタを元に戻す */
	flush = Z_NO_FLUSH;

	if (size>MAX_MEMORY_SIZE)
	{
		cyiFileStream* _stream = new cyiFileStream(0, True);
		stream=_stream;
	}
	else
	{
		cyiMemoryStream* _stream = new cyiMemoryStream(_ZLIB_CHUNK_);
		stream=_stream;
	}

	while(1)
	{
		if (progress)
			*progress=sum;

		if (strm.avail_in==0)
		{
			strm.next_in=&pIndex[sum];
			strm.avail_in = _ZLIB_CHUNK_>all?all:_ZLIB_CHUNK_;
			flush = _ZLIB_CHUNK_>all?Z_FINISH:Z_NO_FLUSH;
			all-=strm.avail_in;
			sum+=strm.avail_in;
		}
		status = deflate(&strm, flush); /* 圧縮する */
		if (status == Z_STREAM_END) break; /* 完了 */

        if (status != Z_OK) {   /* エラー */
			deflateEnd(&strm);
			goto PROC_END;
		}

        if (strm.avail_out == 0) { /* 出力バッファが尽きれば */
			stream->Write(out, _ZLIB_CHUNK_, 0);
            strm.next_out = out; /* 出力バッファ残量を元に戻す */
            strm.avail_out = _ZLIB_CHUNK_; /* 出力ポインタを元に戻す */
        }
	}

    /* 残りを吐き出す */
    if ((sum=_ZLIB_CHUNK_ - strm.avail_out) != 0) 
	{
		stream->Write(out, sum, 0);
    }

    if (deflateEnd(&strm) != Z_OK) {
		goto PROC_END;
    }
	stream->lseek(0, SEEK_SET);
	if (progress)
		*progress=size;	
	return stream;

PROC_END:
	if (stream)
		delete stream;

	return 0;
}

// -------------------------------
#ifdef _WIN32
cyiBaseStream* yiUtilCompressEx(cyiBaseStream* st, HWND hWndProgress, int curPos, float factor)
#else
cyiBaseStream* yiUtilCompressEx(cyiBaseStream* st)//, HWND hWndProgress, int curPos, float factor)
#endif
{
	cyiBaseStream* stream=0;
    int flush, ret;
    z_stream strm;
    unsigned char in[_ZLIB_CHUNK_];
    unsigned char out[_ZLIB_CHUNK_];
	unsigned int size = st->GetSize();
	unsigned int all=size;
	int sum=0;
	int status=0;
	unsigned int* pCompressed=0;

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, Z_BEST_COMPRESSION);
    if (ret != Z_OK)
        return 0;

	strm.avail_in=0;
    strm.next_out = out; /* 出力バッファ残量を元に戻す */
    strm.avail_out = _ZLIB_CHUNK_; /* 出力ポインタを元に戻す */
	flush = Z_NO_FLUSH;

	if (size>MAX_MEMORY_SIZE)
	{
		cyiFileStream* _stream = new cyiFileStream(0, True);
		stream=_stream;
	}
	else
	{
		cyiMemoryStream* _stream = new cyiMemoryStream(_ZLIB_CHUNK_);
		stream=_stream;
	}

	while(1)
	{
		if (strm.avail_in==0)
		{
			st->Read(in, _ZLIB_CHUNK_>all?all:_ZLIB_CHUNK_, 0);
			strm.next_in=in;//&pIndex[sum];
			strm.avail_in = _ZLIB_CHUNK_>all?all:_ZLIB_CHUNK_;
			flush = _ZLIB_CHUNK_>all?Z_FINISH:Z_NO_FLUSH;
			all-=strm.avail_in;
			sum+=strm.avail_in;
		}
		status = deflate(&strm, flush); /* 圧縮する */
		if (status == Z_STREAM_END) break; /* 完了 */

        if (status != Z_OK) {   /* エラー */
			deflateEnd(&strm);
			goto PROC_END;
		}

        if (strm.avail_out == 0) { /* 出力バッファが尽きれば */
			stream->Write(out, _ZLIB_CHUNK_, 0);
            strm.next_out = out; /* 出力バッファ残量を元に戻す */
            strm.avail_out = _ZLIB_CHUNK_; /* 出力ポインタを元に戻す */

#ifdef _WIN32
			if (hWndProgress)
			{
				SendMessage(hWndProgress, PBM_SETPOS, (WPARAM)(curPos+(sum)*factor), 0);
			}
#endif
        }
	}

    /* 残りを吐き出す */
    if ((sum=_ZLIB_CHUNK_ - strm.avail_out) != 0) 
	{
		stream->Write(out, sum, 0);
    }

    if (deflateEnd(&strm) != Z_OK) {
		goto PROC_END;
    }
	stream->lseek(0, SEEK_SET);

	return stream;

PROC_END:
	if (stream)
		delete stream;

	return 0;
}

// -------------------------------
#ifdef _WIN32
cyiBaseStream* yiUtilDeCompressEx(cyiBaseStream* st, HWND hWndProgress, int curPos, float factor)
#else
cyiBaseStream* yiUtilDeCompressEx(cyiBaseStream* st)//, HWND hWndProgress, int curPos, float factor)
#endif
{
	cyiBaseStream* stream=0;

    unsigned char inbuf[_ZLIB_CHUNK_];
    unsigned char outbuf[_ZLIB_CHUNK_];
    int count, status;
    z_stream z;
	unsigned int size = st->GetSize();
	int all=size, sum=0;

	memset(&z, 0, sizeof(z));
    /* すべてのメモリ管理をライブラリに任せる */
    z.zalloc = Z_NULL;
    z.zfree = Z_NULL;
    z.opaque = Z_NULL;

    /* 初期化 */
    z.next_in = Z_NULL;
    z.avail_in = 0;
    if (inflateInit(&z) != Z_OK) {
		goto END_PROC;
    }

    z.next_out = outbuf;        /* 出力ポインタ */
    z.avail_out = _ZLIB_CHUNK_;    /* 出力バッファ残量 */
    status = Z_OK;

	if (size>(MAX_MEMORY_SIZE>>1)) // 大体圧縮率を５割とみている。
	{
		cyiFileStream* _stream = new cyiFileStream(0, True);
		stream=_stream;
	}
	else
	{
		cyiMemoryStream* _stream = new cyiMemoryStream(_ZLIB_CHUNK_);
		stream=_stream;
	}

	//stream = new cyiMemoryStream(_ZLIB_CHUNK_);

    while (status != Z_STREAM_END) 
	{
        if (z.avail_in == 0) /* 入力残量がゼロになれば */
		{  
			st->Read(inbuf, _ZLIB_CHUNK_>all?all:_ZLIB_CHUNK_, 0);
            z.next_in = inbuf;//&pIndex[sum];  /* 入力ポインタを元に戻す */
			z.avail_in = _ZLIB_CHUNK_>all?all:_ZLIB_CHUNK_;//fread(inbuf, 1, INBUFSIZ, fin); /* データを読む */
			sum+=z.avail_in;
			all-=z.avail_in;
        }

        status = inflate(&z, Z_NO_FLUSH); /* 展開 */
        
		if (status == Z_STREAM_END) break; /* 完了 */
        
		if (status != Z_OK) 
		{   /* エラー */
			goto END_PROC;
        }

        if (z.avail_out == 0) { /* 出力バッファが尽きれば */
			stream->Write(outbuf, _ZLIB_CHUNK_, 0);
            z.next_out = outbuf; /* 出力ポインタを元に戻す */
            z.avail_out = _ZLIB_CHUNK_; /* 出力バッファ残量を元に戻す */

#ifdef _WIN32
			if (hWndProgress)
			{
				SendMessage(hWndProgress, PBM_SETPOS, (WPARAM)(curPos+(sum)*factor), 0);
			}
#endif
        }
    }

    /* 残りを吐き出す */
    if ((count = _ZLIB_CHUNK_ - z.avail_out) != 0) 
	{
		stream->Write(outbuf, count, 0);
    }

    /* 後始末 */
    if (inflateEnd(&z) != Z_OK) 
	{
		goto END_PROC;
    }

	stream->lseek(0, SEEK_SET);

	return stream;

END_PROC:
	if(stream)
		delete stream;
	return 0;
}

// -------------------------------
cyiBaseStream* yiUtilDeCompress(void* p, int size, int* progress)
{
	cyiBaseStream* stream=0;
	int all=size, sum=0;
    //unsigned char inbuf[_ZLIB_CHUNK_];
    unsigned char outbuf[_ZLIB_CHUNK_];
    int count, status;
    z_stream z;
	unsigned char* pIndex=(unsigned char*)p;

	memset(&z, 0, sizeof(z));
    /* すべてのメモリ管理をライブラリに任せる */
    z.zalloc = Z_NULL;
    z.zfree = Z_NULL;
    z.opaque = Z_NULL;

    /* 初期化 */
    z.next_in = Z_NULL;
    z.avail_in = 0;
    if (inflateInit(&z) != Z_OK) {
		goto END_PROC;
    }

    z.next_out = outbuf;        /* 出力ポインタ */
    z.avail_out = _ZLIB_CHUNK_;    /* 出力バッファ残量 */
    status = Z_OK;

	if (size>(MAX_MEMORY_SIZE>>1)) // 大体圧縮率を５割とみている。
	{
		cyiFileStream* _stream = new cyiFileStream(0, True);
		stream=_stream;
	}
	else
	{
		cyiMemoryStream* _stream = new cyiMemoryStream(_ZLIB_CHUNK_);
		stream=_stream;
	}

	//stream = new cyiMemoryStream(_ZLIB_CHUNK_);

    while (status != Z_STREAM_END) 
	{
		if (progress)
			*progress=sum;

        if (z.avail_in == 0) /* 入力残量がゼロになれば */
		{  
            z.next_in = &pIndex[sum];  /* 入力ポインタを元に戻す */
			z.avail_in = _ZLIB_CHUNK_>all?all:_ZLIB_CHUNK_;//fread(inbuf, 1, INBUFSIZ, fin); /* データを読む */
			sum+=z.avail_in;
			all-=z.avail_in;
        }

        status = inflate(&z, Z_NO_FLUSH); /* 展開 */
        
		if (status == Z_STREAM_END) break; /* 完了 */
        
		if (status != Z_OK) 
		{   /* エラー */
			goto END_PROC;
        }

        if (z.avail_out == 0) { /* 出力バッファが尽きれば */
			stream->Write(outbuf, _ZLIB_CHUNK_, 0);
            z.next_out = outbuf; /* 出力ポインタを元に戻す */
            z.avail_out = _ZLIB_CHUNK_; /* 出力バッファ残量を元に戻す */
        }
    }

    /* 残りを吐き出す */
    if ((count = _ZLIB_CHUNK_ - z.avail_out) != 0) 
	{
		stream->Write(outbuf, count, 0);
    }

    /* 後始末 */
    if (inflateEnd(&z) != Z_OK) 
	{
			goto END_PROC;
    }

	stream->lseek(0, SEEK_SET);
	if (progress)
		*progress=size;	
	return stream;

END_PROC:
	if(stream)
		delete stream;
	return 0;
}

cyiFileStream::cyiFileStream(const char* pszFileName, Bool bIsNew)
: m_hFile(0)
, m_bIsTempFile(False)
{
	if (pszFileName)
		strcpy(m_szFileName, pszFileName);
	else
	{
		yiMakeTempFileName(m_szFileName);
		m_bIsTempFile=True;
	}

	m_hFile=_yiFileOpen(m_szFileName);

	if (m_hFile)
	{
		if (bIsNew==True)
			_yiFileTrucate(m_hFile);

	}

}

cyiFileStream::~cyiFileStream()
{
	if (m_hFile)
	{
		_yiFileClose(m_hFile);
	}

	if (m_bIsTempFile)
	{
#ifdef _WIN32
		DeleteFile(m_szFileName);
#else
		unlink(m_szFileName);
#endif
	}
}

HRESULT STDMETHODCALLTYPE cyiFileStream::Seek( 
        /* [in] */ LARGE_INTEGER dlibMove,
        /* [in] */ DWORD dwOrigin,
        /* [out] */ ULARGE_INTEGER *plibNewPosition)
{
	int from = SEEK_SET;

	if (dwOrigin==STREAM_SEEK_CUR)
	{
		from = SEEK_CUR;
		//m_nCurrentPos+=dlibMove.LowPart;
	}
	else if (dwOrigin==STREAM_SEEK_END)
	{
		from=SEEK_END;
		//m_nCurrentPos=m_nTotalSize-dlibMove.LowPart;
	}
	//else
	//{
	//	from=SE
	//	m_nCurrentPos=dlibMove.LowPart;
	//}

	//if (m_nCurrentPos<0)
	//	m_nCurrentPos=0;
	//if (m_nCurrentPos>m_nTotalSize)
	//	m_nCurrentPos=m_nTotalSize;

	unsigned int ret =_yiFileSeek(m_hFile, dlibMove.LowPart, from);

	if (plibNewPosition)
	{
		plibNewPosition->HighPart=0;
		plibNewPosition->LowPart=ret;
	}

	return S_OK;
}

HRESULT STDMETHODCALLTYPE cyiFileStream::SetSize( 
        /* [in] */ ULARGE_INTEGER libNewSize)
{
	unsigned int pos = _yiFileCurpos(m_hFile);
	_yiFileSeek(m_hFile, libNewSize.LowPart,SEEK_SET);
	_yiFileTrucate(m_hFile);
	_yiFileSeek(m_hFile, pos, SEEK_SET);

	return S_OK;
}

unsigned int cyiFileStream::lseek(unsigned int pos, int from)
{
	
	_yiFileSeek(m_hFile, pos, from);

	return S_OK;
}
    


HRESULT STDMETHODCALLTYPE cyiFileStream::CopyTo( 
    /* [unique][in] */ IStream *pstm,
    /* [in] */ ULARGE_INTEGER cb,
    /* [out] */ ULARGE_INTEGER *pcbRead,
    /* [out] */ ULARGE_INTEGER *pcbWritten)
{
	if (pstm==0)
		return S_FALSE;

	//STATSTG stat;
	//pstm->Stat(&stat, STATFLAG_NONAME|STATFLAG_NOOPEN);

	unsigned long written=0;
	int size = cb.LowPart;
	int remain=size;
	char* buffer = (char*)yialloc(_WRITE_ONCE_SIZE_);
	while(remain)
	{
		
		ULONG nRead=0;
		if (Read(buffer, remain>_WRITE_ONCE_SIZE_?_WRITE_ONCE_SIZE_:remain, &nRead)!=S_OK)
		{
			return S_FALSE;
		}

		if (pstm->Write(buffer, nRead, 0)!=S_OK)
		{
			return S_FALSE;
		}

		remain-=nRead;
	}

	yifree(buffer);

	if (pcbRead)
	{
		pcbRead->LowPart=cb.LowPart;
		pcbRead->HighPart=0;
	}
	if (pcbWritten)
	{
		pcbWritten->LowPart=cb.LowPart;
		pcbWritten->HighPart=0;
	}

	return S_OK;
}
    
HRESULT STDMETHODCALLTYPE cyiFileStream::Commit( 
        /* [in] */ DWORD grfCommitFlags)
{
	return S_FALSE;
}
    
HRESULT STDMETHODCALLTYPE cyiFileStream::Revert( void)
{
	return S_FALSE;
}
    
HRESULT STDMETHODCALLTYPE cyiFileStream::LockRegion( 
        /* [in] */ ULARGE_INTEGER libOffset,
        /* [in] */ ULARGE_INTEGER cb,
        /* [in] */ DWORD dwLockType)
{
	//m_list->lock();
	return S_OK;
}
    
HRESULT STDMETHODCALLTYPE cyiFileStream::UnlockRegion( 
        /* [in] */ ULARGE_INTEGER libOffset,
        /* [in] */ ULARGE_INTEGER cb,
        /* [in] */ DWORD dwLockType)
{
	//m_list->unlock();
	return S_OK;
}
    
HRESULT STDMETHODCALLTYPE cyiFileStream::Stat( 
        /* [out] */ __RPC__out STATSTG *pstatstg,
        /* [in] */ DWORD grfStatFlag)
{
	pstatstg->cbSize.HighPart=0;
	pstatstg->cbSize.LowPart=::_yiFileSize(m_hFile);
#ifdef _WIN32
	pstatstg->pwcsName=yiToUnicode(0, m_szFileName, 0);
#else
	strcpy((char*)pstatstg->pwcsName, m_szFileName);//=yiToUnicode(0, m_szFileName, 0);
#endif
	return S_OK;
}
    
HRESULT STDMETHODCALLTYPE cyiFileStream::Clone( 
        /* [out] */ __RPC__deref_out_opt IStream **ppstm)
{
	return S_FALSE;
}

unsigned int cyiFileStream::ftell()
{
	return _yiFileCurpos(m_hFile);
}

// -------------------------------
Bool cyiFileStream::Move(const char* pszFileName)
{
	int cur=0;
	if (m_hFile)
	{
		cur=ftell();
		_yiFileClose(m_hFile);
	}

#ifdef _WIN32
	if (MoveFileEx(m_szFileName, pszFileName, MOVEFILE_REPLACE_EXISTING|MOVEFILE_COPY_ALLOWED)==FALSE)
		return False;
#else
	if (rename(m_szFileName, pszFileName)!=0)//, MOVEFILE_REPLACE_EXISTING|MOVEFILE_COPY_ALLOWED)==FALSE)
		return False;
#endif

	strcpy(m_szFileName, pszFileName);
	m_bIsTempFile=False;

	m_hFile=_yiFileOpen(m_szFileName);

	if (m_hFile!=0)
	{
		lseek(m_hFile, cur);
		return True;
	}
	return False;
}

Binary cyiFileStream::ExportToBinary()
{
	int nTotalSize = _yiFileSize(m_hFile);
	void* p = yialloc(nTotalSize+sizeof(unsigned int)+sizeof(unsigned int));
	unsigned int* pIndex = (unsigned int*)p;

	unsigned long   readz;
	unsigned int nBeforePos=_yiFileCurpos(m_hFile);

	lseek(0, SEEK_SET);
	
	if (Read(&pIndex[1], nTotalSize, &readz)!=S_OK)
	{
		yifree(p);
		_yiFileSeek(m_hFile, nBeforePos, SEEK_SET);
		return 0;
	}
	pIndex[0]=nTotalSize;

	return (Binary)&pIndex[1];

}

#ifdef _WIN32
Bool cyiFileStream::ExportToFile(const char* pszFileName, HWND hWndProgress, int cur, float factor)
#else
Bool cyiFileStream::ExportToFile(const char* pszFileName)//, HWND hWndProgress, int cur, float factor)
#endif
{
	return yiFileCopy(m_szFileName, pszFileName);//, hWndProgress, cur, factor);
}

Bool cyiFileStream::ImportFromFile(const char* pszFileName)
{
	_yiFileClose(m_hFile);
	
	m_hFile=_yiFileOpen(pszFileName);
	if (m_hFile>0)
	{
		strcpy(m_szFileName, pszFileName);
		return True;
	}

	return False;
}

HRESULT STDMETHODCALLTYPE cyiFileStream::Read( 
    /* [length_is][size_is][out] */ void *pv,
    /* [in] */ ULONG cb,
    /* [out] */ ULONG *pcbRead)
{
	char* pIndex=(char*)pv;
	int sum=0;
	unsigned int nFileSize = _yiFileSize(m_hFile);
	unsigned int nCurrentPos = _yiFileCurpos(m_hFile);
	if (nCurrentPos>=nFileSize)
	{
		// EOF
		return S_FALSE;
	}

	while(sum!=cb)
	{
		//int n = xRead(&pIndex[sum], m_nCurrentPos, cb-sum);
		int n = _yiFileRead(m_hFile, &pIndex[sum], cb-sum);

		if (n==0)
			break;

		sum+=n;
	}	

	if (pcbRead!=0)
		*pcbRead=sum;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE cyiFileStream::Write( 
    /* [size_is][in] */ const void *pv,
    /* [in] */ ULONG cb,
    /* [out] */ ULONG *pcbWritten)
{
	char* pIndex=(char*)pv;
	int sum=0;

	while(sum!=cb)
	{
		int n = _yiFileWrite(m_hFile, &pIndex[sum], cb-sum);

		if (n==-1)
		{
			return S_FALSE;
		}

		sum+=n;
	}

	if (pcbWritten!=0)
		*pcbWritten=sum;

	return S_OK;
}

unsigned int cyiFileStream::GetSize()
{
	return _yiFileSize(m_hFile);
}

HRESULT STDMETHODCALLTYPE cyiFileStream::QueryInterface( 
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject)
{
	return S_OK;
}

ULONG STDMETHODCALLTYPE cyiFileStream::AddRef( void)
{
	return 1;
}

ULONG STDMETHODCALLTYPE cyiFileStream::Release( void)
{
	return 0;
}

typedef struct {
	int size;
	int n;
	cyiList* list;
	cyiBaseStream* stream;
	Bool bStreamRemove;
} sArchive;

#define ARCHCHAR "#$AX!"


// -------------------------------
HandleArchive yiArchiveCreate(void* pMemoryBlock, int n)
{
	int all=0;
	sArchive* ar = (sArchive*)yialloci(sizeof(sArchive));
	ar->list = new cyiList();
	
	if (pMemoryBlock)
	{
		if (memcmp(pMemoryBlock, ARCHCHAR, 6)!=0)
		{
			delete ar->list;
			yifree(ar);
			return 0;
		}
		
		char* pIndex = (char*)pMemoryBlock;
		pIndex+=6;

		if (ar->list->Import(pIndex, n-6, 0, 0)==0)
		{
			delete ar->list;
			yifree(ar);
			return 0;
		}

		//if (ar->list->GetElementSize()==0)
		//{
		//	ar->size=ar->list->GetElementSize()*ar->list->count();
		//}
		//else
		//{
		//	ar->size=ar->list->GetElementSize()*ar->list->count();
		//}
		//unsigned int* size = (unsigned int*)pIndex;
		//all=*size;

		//pIndex+=4;
/*		while(1)
		{
			unsigned int* blockSize = (unsigned int*)pIndex;
			pIndex+=4;
			
			ar->list->addref(yiBinary(pIndex, *blockSize));
			all-=(*blockSize);
			ar->n++;
			ar->size+=*blockSize;
		}*/
	}

	return (Handle)ar;
}

HandleArchive yiArchiveCreateFromStream(cyiBaseStream* stream, int nElementHeaderSize)
{
	int all=0;
	sArchive* ar = (sArchive*)yialloci(sizeof(sArchive));
	ar->stream=stream;

	if (ar->stream==0)
	{
		ar->stream = new cyiFileStream(0, True);
		ar->bStreamRemove=True;
	}

	char buffer[6]={0, };
	stream->Read(buffer, 6, 0);

	if (memcmp(buffer, ARCHCHAR, 6)!=0)
	{
		yifree(ar);
		return 0;
	}

	unsigned int nElementSize=0;
	if (stream->Read(&nElementSize, sizeof(nElementSize), 0)!=S_OK)
	{
		yifree(ar);
		return 0;
	}

	if (nElementSize)
	{
		yifree(ar);
		return 0;
	}

	ar->list = new cyiList(True);
	while(stream->IsEOF()==False)
	{
		unsigned int* pElement = (unsigned int*)yialloci(sizeof(unsigned int)*3+nElementHeaderSize);

		int size=0;
		unsigned int n = stream->ftell();

		if (stream->Read(&size, 4, 0)!=S_OK)
		{
			break;
		}
		cyiMemoryStream* xx = new cyiMemoryStream(0);

		pElement[0]=0xFFFFFFFF;
		pElement[2]=nElementHeaderSize;
		pElement[1]=(unsigned int)xx;

		xx->SetStream(stream, n+4+nElementHeaderSize, size-nElementHeaderSize);

		if (nElementHeaderSize!=0)
		{
			if (stream->Read(&pElement[3], nElementHeaderSize, 0)!=S_OK)
			{
				break;
			}
		}

		stream->lseek(n+sizeof(unsigned int)+size, SEEK_SET);
		ar->list->addref(&pElement[1]);
	}

	return (Handle)ar;
}

// -------------------------------
Bool yiArchiveInsert(HandleArchive hArchive, int index, void* data, int size, void* header, int nHeaderSize)
{
	sArchive* ar = (sArchive*)hArchive;

	if (index==-1)
	{
		if (header)
		{
			ar->list->addref(yiBinary(header, nHeaderSize, data, size, 0));
		}
		else
		{
			ar->list->addref(yiBinary(data, size, 0));
		}
	}
	else
	{
		if (header)
		{
			ar->list->insertref(index, yiBinary(header, nHeaderSize, data, size, 0));
		}
		else
		{
			ar->list->insertref(index, yiBinary(data, size, 0));
		}
	}

	return True;
}

// -------------------------------
Bool yiArchiveAdd(HandleArchive hArchive, void* data, int size, void* header, int nHeaderSize)
{
	return yiArchiveInsert(hArchive, -1, data, size, header, nHeaderSize);
}

// -------------------------------
Bool yiArchiveAddStream(HandleArchive hArchive, cyiBaseStream* stream, void* header, int nHeaderSize)
{
	return yiArchiveInsertStream(hArchive, -1, stream, header, nHeaderSize);
}

// -------------------------------
Bool yiArchiveInsertStream(HandleArchive hArchive, int index, cyiBaseStream* stream, void* header, int nHeaderSize)
{
	sArchive* ar = (sArchive*)hArchive;

	unsigned int* p = (unsigned int*)yialloci(sizeof(unsigned int)*3+nHeaderSize);
	p[0]=0xFFFFFFFF;
	p[1]=(unsigned int)stream;
	p[2]=nHeaderSize;

	if (nHeaderSize)
	{
		memcpy(&p[3], header, nHeaderSize);
	}

	if (index==-1)
	{
		ar->list->addref(&p[1]);
	}
	else
	{
		ar->list->insertref(index, &p[1]);
	}

	return True;
}

// -------------------------------
Bool yiArchiveRemove(HandleArchive hArchive, int index)
{
	sArchive* ar = (sArchive*)hArchive;
	Binary p = (Binary)ar->list->removeEx(index);
	yiBinaryDelete(p);

	return True;
}

// -------------------------------
int yiArchiveCount(HandleArchive hArchive)
{
	sArchive* ar = (sArchive*)hArchive;

	if (ar)
		if (ar->list)
			return ar->list->count();

	return 0;
	/*cyiNode* node = ar->list->GetNodeTopFrom(index);

	if (node==0)
		return 0;

	return node->ref;*/
}


// -------------------------------
Binary yiArchiveEnum(HandleArchive hArchive, int index)
{
	sArchive* ar = (sArchive*)hArchive;

	cyiNode* node = ar->list->GetNodeTopFrom(index);

	if (node==0)
		return 0;

	return (Binary)node->ref;
}

// -------------------------------
Bool yiArchiveEnumEx(HandleArchive hArchive, int index, void* buffer, int size)
{
	sArchive* ar = (sArchive*)hArchive;

	cyiNode* node = ar->list->GetNodeTopFrom(index);

	if (node==0)
		return 0;

	unsigned int* p = (unsigned int*)node->ref;

	if (p[-1]==0xFFFFFFFF)
	{
		int n =0;
		int remain=size;
		char* pBuffer=(char*)buffer;
		cyiBaseStream* st = (cyiBaseStream*)p[0];
		if (p[1]!=0)
		{
			n =  p[1]<size?p[1]:size;
			memcpy(pBuffer, &p[2],n);
			remain-=n;
		}

		if (remain!=0)
		{
			st->Read(&pBuffer[n], remain, 0);
		}
		return True;
	}

	memcpy(buffer, p, size);
	return True;
}

// -------------------------------
static int GetArchiveSize(int index, void* data)
{
	return xBIN_SIZE(data);
}

// -------------------------------
Binary yiArchiveExport(HandleArchive hArchive, const char* pszFileName)
{
	sArchive* ar = (sArchive*)hArchive;
	return (Binary)ar->list->Export(pszFileName, GetArchiveSize, (void*)ARCHCHAR, 6);
}

// -------------------------------
cyiBaseStream* yiArchiveExportStream(HandleArchive hArchive, const char* pszFileName)
{
	sArchive* ar = (sArchive*)hArchive;

	cyiFileStream* st = new cyiFileStream(pszFileName, True);

	unsigned int temp=0;
	st->Write(ARCHCHAR ,6, 0);
	st->Write(&temp, 4, 0);
	
	xNODELOOP(node, ar->list)
	{
		unsigned int* x = (unsigned int*)node->ref;

		if (x[-1]==0xFFFFFFFF)
		{
			cyiBaseStream* stream = (cyiBaseStream*)x[0];
			int size = stream->GetSize()+x[1];

			st->Write(&size, sizeof(unsigned int), 0);

			if (x[1]!=0)
			{
				st->Write(&x[2], x[1], 0);
			}

			ULARGE_INTEGER x;
			x.HighPart=0;
			x.LowPart=stream->GetSize();
			if (stream->CopyTo(st, x, 0, 0)!=S_OK)
			{
				delete st;
				return 0;
			}
		}
		else
		{
			st->Write(&x[-1], sizeof(unsigned int), 0); 
			st->Write(node->ref, xBIN_SIZE(node->ref), 0);
		}
	}

	return st;
	//if (pszFileName==0)
	//{
	//	cyiMemoryStream* st = new cyiMemoryStream();
	//	Binary bin = ar->list->Export(0, GetArchiveSize, ARCHCHAR, 6);
	//	st->Write(bin, xBIN_SIZE(bin), 0);
	//	yiBinaryDelete(bin);
	//	return st;
	//}
	//else
	//{
	//	cyiFileStream* st = new cyiFileStream(pszFileName, True);
	//	Binary bin = ar->list->Export(0, GetArchiveSize, ARCHCHAR, 6);
	//	st->Write(bin, xBIN_SIZE(bin), 0);
	//	yiBinaryDelete(bin);
	//	return st;
	//}
	return 0;
}

// -------------------------------
void yiArchiveClose(HandleArchive hArchive)
{
	sArchive* ar = (sArchive*)hArchive;
	int cnt = ar->list->count();

	for (int i=0; i<cnt; i++)
	{
		Binary bin = (Binary)ar->list->removeEx(0);
		unsigned int size = xBIN_SIZE(bin);

		if (size==0xFFFFFFFF)
		{
			unsigned int* p = (unsigned int*)bin;
			cyiBaseStream* st =  (cyiBaseStream*)*p;
			delete st;
		}

		yiBinaryDelete(bin);
	}
	delete ar->list;

	if (ar->bStreamRemove)
		delete ar->stream;

	yifree(ar);
}
