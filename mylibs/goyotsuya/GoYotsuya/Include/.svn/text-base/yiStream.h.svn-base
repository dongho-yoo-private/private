
/// @file Stream.h
/// 入出力関連.
#pragma once

#include "yi.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

//#include "Debug.h"
// TODO: ASSERT() を定義してください
#ifndef ASSERT
#define ASSERT
#endif

const DWORD STGM_STANDARD_READ  = STGM_READ  | STGM_SHARE_DENY_WRITE;
const DWORD STGM_STANDARD_WRITE = STGM_WRITE | STGM_SHARE_DENY_WRITE;

/// 入出力関連.
namespace IO
{
 ////////////////////////////////////////////////////////////
 /// IStreamラッパー.
 /// スマートポインタの機能も兼ねる.
 class xEXPORT Stream
 {
 private:
  IStream* m_pStream;
 public:
  ////////////////////////////////////////////////////////////
  /// 既存のIStreamに対して、もしくは空のStreamの作成.
  /// @param pStream 参照カウントはインクリメントされる.
  explicit Stream(IStream* pStream = NULL) : m_pStream(pStream)
  {
  }

  ////////////////////////////////////////////////////////////
  /// コピーコンストラクタ.
  Stream(const Stream& rhs)
  {
   if(m_pStream = rhs.m_pStream)
    m_pStream->AddRef();
  }

  ////////////////////////////////////////////////////////////
  /// 代入演算子.
  Stream& operator = (const Stream& rhs)
  {
   if(rhs.m_pStream)
    rhs.m_pStream->AddRef();
   if(m_pStream)
    m_pStream->Release();
   m_pStream = rhs.m_pStream;
   return *this;
  }

  ////////////////////////////////////////////////////////////
  /// デストラクタ.
  ~Stream()
  {
   if(m_pStream)
    m_pStream->Release();
  }

  ////////////////////////////////////////////////////////////
  /// IStreamへの自動変換.
  operator IStream* () { return m_pStream; }

  ////////////////////////////////////////////////////////////
  /// 既存のストリームへ結びつける.
  /// @param pStream 参照カウントはインクリメントされない
  void Attach(IStream* pStream)
  {
   if(m_pStream)
    m_pStream->Release();
   m_pStream = pStream;
  }

  ////////////////////////////////////////////////////////////
  /// 内部ストリームを切り離す.
  /// @return 参照カウントはデクリメントされない
  IStream* Detach()
  {
   IStream* pStream = m_pStream;
   m_pStream = NULL;
   return pStream;
  }

 ////////////////////////////////////////////////////////////
 /// @name 作成・破棄
 //@{

  ////////////////////////////////////////////////////////////
  /// ファイルに対するStreamの作成.
  /// @param filename ファイル名
  /// @param mode 作成モード
  ///
  /// 作成モード一覧
  /// - 読み込み : STGM_STANDARD_READ  = STGM_READ | STGM_SHARE_DENY_WRITE
  /// - 書き込み : STGM_STANDARD_WRITE = STGM_WRITE | STGM_SHARE_DENY_WRITE
  /// - Access  : STGM_READ | STGM_WRITE | STGM_READWRITE.
  /// - Sharing : STGM_SHARE_DENY_NONE | STGM_SHARE_DENY_READ | STGM_SHARE_DENY_WRITE | STGM_SHARE_EXCLUSIVE | STGM_PRIORITY
  /// - Creation STGM_CREATE | STGM_CONVERT | STGM_FAILIFTHERE
  /// - Transactioning : STGM_DIRECT | STGM_TRANSACTED
  /// - Transactioning Performance : STGM_NOSCRATCH | STGM_NOSNAPSHOT
  /// - Direct SWMR and Simple : STGM_SIMPLE | STGM_DIRECT_SWMR
  /// - Delete On Release : STGM_DELETEONRELEASE
  HRESULT CreateOnFile(LPCTSTR filename, DWORD mode);

  HRESULT CreateOnMemory(void* p, int size);

  ////////////////////////////////////////
  /// HGLOBALメモリに対するStreamの作成.
  /// @param hGlobal 対象のHGLOBALメモリ
  /// @param deleteOnRelease Streamが閉じられたときにhGlobalを解放するか否か
  /// @see GetHGlobal
  HRESULT CreateOnHGlobal(HGLOBAL hGlobal, BOOL deleteOnRelease);

  ////////////////////////////////////////
  /// リソースに対するStreamの作成.
  /// ストリームは読み込み専用.
  HRESULT CreateOnResource(HINSTANCE hInstance, LPCTSTR name, LPCTSTR type);

  ////////////////////////////////////////////////////////////
  /// 内部ストリームを解放する.
  void Close()
  {
   if(m_pStream)
   {
    m_pStream->Release();
    m_pStream = NULL;
   }
  }
 //@}

 ////////////////////////////////////////////////////////////
 /// @name 操作
 //@{

  ////////////////////////////////////////
  /// Streamからの読み込み.
  /// @param buffer バッファ
  /// @param size   読み込むバイト数
  /// @param done   読み込まれたバイト数
  HRESULT Read(void* buffer, UINT32 size, UINT32* done = NULL)
  {
   ASSERT(m_pStream != NULL);
   return m_pStream->Read(buffer, size, (ULONG*)done);
  }

  ////////////////////////////////////////
  /// Streamへの書き込み.
  /// @param buffer バッファ
  /// @param size   書き込むバイト数
  /// @param done   書き込まれたバイト数
  HRESULT Write(const void* buffer, UINT32 size, UINT32* done = NULL)
  {
   ASSERT(m_pStream != NULL);
   return m_pStream->Write(buffer, size, (ULONG*)done);
  }

  ////////////////////////////////////////
  /// Streamのシーク.
  /// @param move   移動バイト数
  /// @param method シーク方法 (STREAM_SEEK_CUR, STREAM_SEEK_SET, STREAM_SEEK_END)
  /// @param newpos シーク後のポインタ位置
  HRESULT Seek(INT64 move, DWORD method, UINT64* newpos = NULL)
  {
   ASSERT(m_pStream != NULL);
   return m_pStream->Seek((LARGE_INTEGER&)move, method, (ULARGE_INTEGER*)newpos);
  }

  ////////////////////////////////////////
  /// データサイズの取得.
  UINT64 GetSize()
  {
   STATSTG stat;
   m_pStream->Stat(&stat, STATFLAG_NONAME);
   return stat.cbSize.QuadPart;
  }

  ////////////////////////////////////////
  /// 現在のポインタ位置の取得.
  UINT64 GetPosition()
  {
   UINT64 pos;
   Seek(0, STREAM_SEEK_CUR, &pos);
   return pos;
  }

  ////////////////////////////////////////
  /// Streamに関連付けられたHGLOBALメモリハンドルの取得.
  /// CreateOnHGlobal()で作成されたStreamにのみ有効.
  /// @see CreateOnHGlobal
  HGLOBAL GetHGlobal()
  {
   HGLOBAL hGlobal = NULL;
   ::GetHGlobalFromStream(m_pStream, &hGlobal);
   return hGlobal;
  }
 //@}
 };
} // IO


#include "yistd.h"

class xEXPORT cyiMemoryStream : public IStream
{
public:
	cyiMemoryStream(int nMinumBufferSize=4096);

	virtual ~cyiMemoryStream();

    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Seek( 
        /* [in] */ LARGE_INTEGER dlibMove,
        /* [in] */ DWORD dwOrigin,
        /* [out] */ ULARGE_INTEGER *plibNewPosition);
    
    virtual HRESULT STDMETHODCALLTYPE SetSize( 
        /* [in] */ ULARGE_INTEGER libNewSize);
    
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE CopyTo( 
        /* [unique][in] */ IStream *pstm,
        /* [in] */ ULARGE_INTEGER cb,
        /* [out] */ ULARGE_INTEGER *pcbRead,
        /* [out] */ ULARGE_INTEGER *pcbWritten);
    
    virtual HRESULT STDMETHODCALLTYPE Commit( 
        /* [in] */ DWORD grfCommitFlags) ;
    
    virtual HRESULT STDMETHODCALLTYPE Revert( void);
    
    virtual HRESULT STDMETHODCALLTYPE LockRegion( 
        /* [in] */ ULARGE_INTEGER libOffset,
        /* [in] */ ULARGE_INTEGER cb,
        /* [in] */ DWORD dwLockType);
    
    virtual HRESULT STDMETHODCALLTYPE UnlockRegion( 
        /* [in] */ ULARGE_INTEGER libOffset,
        /* [in] */ ULARGE_INTEGER cb,
        /* [in] */ DWORD dwLockType);
    
    virtual HRESULT STDMETHODCALLTYPE Stat( 
        /* [out] */ __RPC__out STATSTG *pstatstg,
        /* [in] */ DWORD grfStatFlag);
    
    virtual HRESULT STDMETHODCALLTYPE Clone( 
        /* [out] */ __RPC__deref_out_opt IStream **ppstm);

    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Read( 
        /* [length_is][size_is][out] */ void *pv,
        /* [in] */ ULONG cb,
        /* [out] */ ULONG *pcbRead);
    
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Write( 
        /* [size_is][in] */ const void *pv,
        /* [in] */ ULONG cb,
        /* [out] */ ULONG *pcbWritten);

    virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);

    virtual ULONG STDMETHODCALLTYPE AddRef( void);

    virtual ULONG STDMETHODCALLTYPE Release( void);

	//Bool Read(void* buffer, int n, int* readz=0);

	//Bool Write(const void* buffer, int n, int* writez=0);

	Binary ExportToBinary();

	Bool ExportToFile(const char* pszFileName);

	Bool ImportFromFile(const char* pszFileName);

	unsigned int lseek(unsigned int pos, int from);

private:
	cyiList* m_list;
	unsigned int m_nTotalSize;
	unsigned int m_nCurrentPos;
	unsigned int m_nMinumPageSize;

	int xGetPage(unsigned int pos);

	int xGetWritableSize(unsigned int pos, unsigned int size);

	int xWrite(void* buffer, unsigned int pos, unsigned int size);

	int xRead(void* buffer, unsigned int pos, int size);

	char* m_pszFileName;
};

// -------------------------------
extern xEXPORT cyiMemoryStream* yiUtilCompress(void* p, int size);

// -------------------------------
extern xEXPORT cyiMemoryStream* yiUtilDeCompress(void* p, int size);

typedef Handle HandleArchive;

// -------------------------------
extern xEXPORT  HandleArchive yiArchiveCreate(void* pMemoryBlock=0, int n=0);

// -------------------------------
extern xEXPORT  Bool yiArchiveAdd(HandleArchive hArchive, void* data, int size, void* header, int nHeaderSize);

// -------------------------------
extern xEXPORT  Bool yiArchiveInsert(HandleArchive hArchive, int index, void* data, int size, void* header, int nHeaderSize);

// -------------------------------
extern xEXPORT  Bool yiArchiveRemove(HandleArchive hArchive, int index);

// -------------------------------
extern xEXPORT  Binary yiArchiveEnum(HandleArchive hArchive, int index);

// -------------------------------
extern xEXPORT  Binary yiArchiveExport(HandleArchive hArchive, const char* pszFileName);


// -------------------------------
extern xEXPORT  void yiArchiveClose(HandleArchive hArchive);

typedef struct {
	char szPath[64];
	char szFileName[128];
	unsigned int flag32;
	unsigned int size;
	unsigned int check_sum;
} syiTarFormat;


typedef struct {
	char szPath[64];
	char szFileName[128];
	unsigned int flag32;
	unsigned int size;
	unsigned int check_sum;
	char szShortCutName[64];
} syiInstallFormat;