// usage
//  IO::Stream stream;

//  stream.CreateOnResource((HINSTANCE)::GetWindowLong(m_hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDB_PNG1), L"PNG");

//  Image img(stream);
//  g.DrawImage(&img, 0, 0);

#pragma comment(lib, "zlib.lib")
 
// Stream.cpp

//#include "stdafx.h"

#include "yiStream.h"

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
    return ::InterlockedIncrement(&m_ReferenceCount);
   }
   STDMETHODIMP_(ULONG) Release()
   {
    ULONG refs = ::InterlockedDecrement(&m_ReferenceCount);
    if(refs == 0)
    {
     delete this;
    }
    return refs;
   }
   STDMETHODIMP QueryInterface(REFIID iid, void** ppObj)
   {
    ASSERT(*ppObj == NULL);
    if(iid == __uuidof(IStream) || iid == __uuidof(ISequentialStream) || iid == __uuidof(IUnknown))
    {
     *ppObj = static_cast<IStream*>(this);
     AddRef();
     return S_OK;
    }
    *ppObj = NULL;
    return E_NOINTERFACE;
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
  ASSERT(m_pStream == NULL);
  HRESULT hr = ::SHCreateStreamOnFile(filename, mode, &m_pStream);
  if FAILED(hr)
  {
   //TRACE("@Stream::CreateOnFile(filename=", filename, ") : HRESULT=", hr);
   return hr;
  }
  return S_OK;
 }
 HRESULT Stream::CreateOnHGlobal(HGLOBAL hGlobal, BOOL deleteOnRelease)
 {
  ASSERT(m_pStream == NULL);
  return ::CreateStreamOnHGlobal(hGlobal, deleteOnRelease, &m_pStream);
 }
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
 HRESULT Stream::CreateOnMemory(void* data, int size)
 {
	 m_pStream = new Private::MemoryStream(data, size);
	 return S_OK;
 }

}

 


cyiMemoryStream::cyiMemoryStream(int nMinumBufferSize)
: m_list(0)
, m_nTotalSize(0)
, m_nCurrentPos(0)
, m_nMinumPageSize(nMinumBufferSize)
, m_pszFileName(0)
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

	STATSTG stat;
	pstm->Stat(&stat, STATFLAG_NONAME|STATFLAG_NOOPEN);

	unsigned long written=0;
	void* p		= yialloc(stat.cbSize.LowPart);
	Bool res = Write(p, stat.cbSize.LowPart, &written);
	yifree(p);

	return res;
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


Bool cyiMemoryStream::ExportToFile(const char* pszFileName)
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

	while(sum!=cb)
	{
		int n = xRead(&pIndex[sum], m_nCurrentPos, cb-sum);

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

#include "zlib/zlib.h"

#define _ZLIB_CHUNK_ 16384


// -------------------------------
cyiMemoryStream* yiUtilCompress(void* p, int size)
{
	cyiMemoryStream* stream=0;
    int flush, ret;
    z_stream strm;
    unsigned char in[_ZLIB_CHUNK_];
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

	stream = new cyiMemoryStream(_ZLIB_CHUNK_);

	while(1)
	{
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
	
	return stream;

PROC_END:
	if (stream)
		delete stream;

	return 0;
}

// -------------------------------
cyiMemoryStream* yiUtilDeCompress(void* p, int size)
{
	cyiMemoryStream* stream=0;
	int all=size, sum=0;
    unsigned char inbuf[_ZLIB_CHUNK_];
    unsigned char outbuf[_ZLIB_CHUNK_];
    int count, status;
    z_stream z;
	unsigned char* pIndex=(unsigned char*)p;

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

	stream = new cyiMemoryStream(_ZLIB_CHUNK_);

    while (status != Z_STREAM_END) 
	{
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
	return stream;

END_PROC:
	if(stream)
		delete stream;
	return 0;
}

typedef struct {
	int size;
	int n;
	cyiList* list;
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
Bool yiArchiveRemove(HandleArchive hArchive, int index)
{
	sArchive* ar = (sArchive*)hArchive;
	Binary p = ar->list->removeEx(index);
	yiBinaryDelete(p);

	return True;
}

// -------------------------------
Binary yiArchiveEnum(HandleArchive hArchive, int index)
{
	sArchive* ar = (sArchive*)hArchive;

	cyiNode* node = ar->list->GetNodeTopFrom(index);

	if (node==0)
		return 0;

	return node->ref;
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
	return ar->list->Export(pszFileName, GetArchiveSize, ARCHCHAR, 6);
}

// -------------------------------
void yiArchiveClose(HandleArchive hArchive)
{
	sArchive* ar = (sArchive*)hArchive;
	int cnt = ar->list->count();

	for (int i=0; i<cnt; i++)
	{
		yiBinaryDelete(ar->list->removeEx(0));
	}
	delete ar->list;

	yifree(ar);
}
