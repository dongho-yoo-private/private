#include <core/library/lib_zlib.h>

#include <zlib.h>

#include <core/memory_allocation/guarded_alloc.h>
#include <core/library/lib_list.h>

#include "intern/unzip.h"

// 
#define MAX_MEMORY_SIZE (1024*1024*20)  // -> 20MBÜÅÍãÅ¼ÚWJ
using namespace cocos2d;

ADK_BEGIN_NAMESPACE

// -------------------------------
void* LIBZ_Compress(void* p, S32 size, S32* compressedSize, float* fProcess)
{
	char* pRet=0;
    int flush, ret;
    z_stream strm;
    unsigned char out[_ZLIB_CHUNK_];
	unsigned char* pIndex = (unsigned char*)p;
	int all=size;
	int sum=0, remain=0, nCompressedSize=0;
	int status=0;
  adk::HandleList hList=0;

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, Z_BEST_COMPRESSION);
    if (ret != Z_OK)
        return 0;

	strm.avail_in=0;
    strm.next_out = out; /* oÍobt@cÊð³Éß· */
    strm.avail_out = _ZLIB_CHUNK_; /* oÍ|C^ð³Éß· */
	flush = Z_NO_FLUSH;

	hList = adk::LIB_ListCreate();

	if (hList==0)
		return 0;

	while(1)
	{
		if (strm.avail_in==0)
		{
			strm.next_in=&pIndex[sum];
			strm.avail_in = _ZLIB_CHUNK_>all?all:_ZLIB_CHUNK_;
			flush = _ZLIB_CHUNK_>all?Z_FINISH:Z_NO_FLUSH;
			all-=strm.avail_in;
			sum+=strm.avail_in;

			if (fProcess)
				*fProcess=100.0*(float)sum/(float)size;
		}

		status = deflate(&strm, flush); /* ³k·é */
		if (status == Z_STREAM_END) break; /* ®¹ */

        if (status != Z_OK) {   /* G[ */
			deflateEnd(&strm);
			goto PROC_END;
		}

        if (strm.avail_out == 0) { /* oÍobt@ªs«êÎ */
			char* p = (char*)ADK_Malloc(_ZLIB_CHUNK_, "ZLIB_COMPRESS_PARTS");
			memcpy(p, out, _ZLIB_CHUNK_);
			adk::LIB_ListAdd(hList, p);
            strm.next_out = out; /* oÍobt@cÊð³Éß· */
            strm.avail_out = _ZLIB_CHUNK_; /* oÍ|C^ð³Éß· */
        }
	}

	remain = _ZLIB_CHUNK_ - strm.avail_out;
	nCompressedSize = adk::LIB_ListCount(hList)*_ZLIB_CHUNK_+ remain;	
	pRet = (char*)ADK_Malloc(nCompressedSize +1, "ZLIB_COMPRESS_COMPLETE");

	{
		adk::TListNode* node;
		int index=0;
		for (node=adk::LIB_ListGetTopFrom(hList, 0); node; node=node->np)
		{
			memcpy(&pRet[index], node->ref, _ZLIB_CHUNK_);
			index+=_ZLIB_CHUNK_;
		}

		if (remain)
			memcpy(&pRet[index], out, remain);

		pRet[nCompressedSize]=0; // 多分、この場合は意味ない。
		if (compressedSize)
			*compressedSize=nCompressedSize;
	}

    if (deflateEnd(&strm) != Z_OK) {
		goto PROC_END;
    }

	return pRet;

PROC_END:
	if (hList)
	{
		adk::TListNode* node;
		for (node=adk::LIB_ListGetTopFrom(hList, 0); node; node=node->np)
		{
			if (node->ref)
				ADK_Free(node->ref);
		}
		adk::LIB_ListClear(hList);
		adk::LIB_ListDestroy(hList);
	}

	if (pRet)
		ADK_Free(pRet);

	return 0;
}

// -------------------------------
void* LIBZ_DeCompress(const void* st, U32 size, U32* extractedSize, float* fProcess)
{
	char* p = (char*)st;
	char* pRet=0;
	S32 nExtractSize=0;
  adk::HandleList hList = 0;
    unsigned char outbuf[_ZLIB_CHUNK_];
    int count, status;
    z_stream z={0, };
	int all=size, sum=0;

	if (extractedSize)
		*extractedSize=0;

	/** 今のところコメントアウト
		必要のある時には、以下の値を設定*/
#if 0
    z.zalloc = Z_NULL;
    z.zfree = Z_NULL;
    z.opaque = Z_NULL;
    z.next_in = Z_NULL;
    z.avail_in = 0;
#endif

    if (inflateInit(&z) != Z_OK) 
	{
		goto END_PROC;
    }

    z.next_out = outbuf;        /* 出力バッファー設定*/
    z.avail_out = _ZLIB_CHUNK_;    /* デフォルトの値。。。*/
    status = Z_OK;

	hList = adk::LIB_ListCreate();
    while (status != Z_STREAM_END) 
	{
        if (z.avail_in == 0) /* üÍcÊª[ÉÈêÎ */
		{  
			int readSize = _ZLIB_CHUNK_>all?all:_ZLIB_CHUNK_;
            z.next_in = (Bytef*)&p[sum];
			z.avail_in = readSize;
			sum+=z.avail_in;
			all-=z.avail_in;
        }

        status = inflate(&z, Z_NO_FLUSH); /* WJ */
        
		if (status == Z_STREAM_END) 
		{
			break; /* ®¹ */
		}
        
		if (status != Z_OK) 
		{   /* G[ */
			goto END_PROC;
        }

        if (z.avail_out == 0) { /* oÍobt@ªs«êÎ */
			char* pOut = (char*)ADK_Malloc(_ZLIB_CHUNK_, "LIB_ZLIB_DECOMPRESS");
			adk::LIB_ListAdd(hList, pOut);
			memcpy(pOut, outbuf, _ZLIB_CHUNK_);
            z.next_out = outbuf; /* oÍ|C^ð³Éß· */
            z.avail_out = _ZLIB_CHUNK_; /* oÍobt@cÊð³Éß· */

			if (fProcess)
				*fProcess = 100.0*(float)sum/(float)size;
        }
    }

	count = _ZLIB_CHUNK_ - z.avail_out;
	nExtractSize = count+adk::LIB_ListCount(hList)*_ZLIB_CHUNK_;
	
	pRet = (char*)ADK_Malloc(nExtractSize+1, "LIB_ZLIB_DECOMPRESS_DATA");
	{
		adk::TListNode* node;
		S32 index=0;
		for (node=adk::LIB_ListGetTopFrom(hList, 0); node; node=node->np)
		{
			memcpy(&pRet[index], node->ref, _ZLIB_CHUNK_);
			index+=_ZLIB_CHUNK_;
			ADK_Free(node->ref);
		}

		if (count!=0)
		{
			memcpy(&pRet[index], outbuf, count);
		}

		// もし、回答したデータが文字列ならば、このまま出力してみれる。
		pRet[nExtractSize]=0;
	}

	if (extractedSize)
		*extractedSize=nExtractSize;

	if (hList)
	{
		adk::LIB_ListClear(hList);
		adk::LIB_ListDestroy(hList);
	}
	
    /* ãn */
    if (inflateEnd(&z) != Z_OK) 
	{
		goto END_PROC;
    }

	return pRet;

END_PROC:
	if (hList)
	{
		adk::TListNode* node;
		for (node=adk::LIB_ListGetTopFrom(hList, 0); node; node=node->np)
		{
			if (node->ref)
				ADK_Free(node->ref);
		}
		adk::LIB_ListClear(hList);
		adk::LIB_ListDestroy(hList);
	}

	if (pRet)
		ADK_Free(pRet);
	return 0;
}

static int xGetGzipSize(const void* data, int size, int* zipOffset)
{
#define MAX_STACK_BUFFER (1024) 
    
    unsigned char* p = (unsigned char*)data;
    int offset=0;
	int ret=0;
    
    if( p[0] != 0x1f || p[1] != 0x8b )
    {
        return 0;
    }
    
    offset+=2;
    
    unsigned char method = p[offset];
    
    if (method != Z_DEFLATED)
    {
        return 0;
    }

    offset++; // flg
    
    
    unsigned char flags = p[offset];
    offset++; // MTIME
    

    offset+=6; // XFL, OS
    
    if ((flags & 0xE0) != 0)
    {
        return 0;
    }
    
    
    // CRC
    if (flags & 0x02)
    {
        // この場合はサイズが入っている。
        ret = *((int*)&p[size-sizeof(int)]);
    }
    
    // Extra
    if (flags & 0x04)
    {
        unsigned short extraSize = *((unsigned short*)&p[offset]);
        offset+=2;
        offset+=extraSize;
    }
    
    // name
    if (flags & 0x08)
    {
        while(p[offset++]);
    }
    
    // comments
    if (flags & 0x10)
    {
        while(p[offset++]);
    }

	if (zipOffset)
	{
		*zipOffset=offset;
	}

	if (ret!=0)
	{
		return ret;
	}

    // ここから解凍
    unsigned char outbuf[MAX_STACK_BUFFER];
    int count=0, status;
    z_stream z = {0, };
    int all=size, sum=0;
    
    
    if(inflateInit2(&z, -MAX_WBITS) != Z_OK)
    {
        return 0;
    }
    
    z.next_out = outbuf;        
    z.avail_out = MAX_STACK_BUFFER; 
    status = Z_OK;
    
    p=&p[offset];

    while (status != Z_STREAM_END)
    {
        if (z.avail_in == 0)
        {            
            z.next_in = &p[sum];
            z.avail_in = MAX_STACK_BUFFER>all?all:MAX_STACK_BUFFER;
            sum+=z.avail_in;
            all-=z.avail_in;
        }
        
        status = inflate(&z, Z_NO_FLUSH); 
        
        if (status == Z_STREAM_END)
        {
            break;
        }
            
        if (status != Z_OK)
        {
            return 0;
        }
        
        if (z.avail_out == 0)
        {
            z.next_out = outbuf; 
            z.avail_out = MAX_STACK_BUFFER;
        }
        count++;
    }
    
    ret = MAX_STACK_BUFFER-z.avail_out + count*MAX_STACK_BUFFER;
    inflateEnd(&z);
    return ret;
}

// -------------------------------
void* LIBZ_DeCompress4Raw(const void* source, U32 size, U32* extractedSize, float* fProcess)
{
	int offset;
    int status;
    z_stream z = {0, };
	int n = xGetGzipSize(source, size, &offset);
	char* p;
	char* ret;
  
	if (n==0)
	{
		// invalid format.
		return 0;
	}
	p = (char*)source+offset;

	if (extractedSize)
	{
		*extractedSize=n;
	}

	ret = (char*)ADK_Malloc(n+1, "LIB_ZLIB");
  memset(ret, 0, n+1);

	if (ret==0)
	{
		return 0;
	}
  
    if(inflateInit2(&z, -MAX_WBITS) != Z_OK)
    {
        return 0;
    }
    
    z.next_out = (Bytef*)ret;        
    z.avail_out = n;
    status = Z_OK;
    
    while (status != Z_STREAM_END)
    {
        if (z.avail_in == 0)
        {            
            z.next_in = (Bytef*)p;
            z.avail_in = size;
        }
        
        status = inflate(&z, Z_NO_FLUSH); 
        
        if (status == Z_STREAM_END)
        {
            break;
        }
            
        if (status != Z_OK)
        {
    		inflateEnd(&z);
            return 0;
      }
  }
    
  inflateEnd(&z);
  
	ret[n]=0;
  return ret;

}

// -------------------------------
void LIBZ_Delete(void* p)
{
	ADK_Free(p);
}

// cocos2d内部で利用しているunzipをラップ
//TODO ファイル保存ではなく、メモリ上への確保に変更する
HZIP LIBZ_UnzOpen(const char *path)
{
  return (HZIP)cocos2d::unzOpen(path);
}

void LIBZ_UnzClose(HZIP hZip)
{
  cocos2d::unzClose(hZip);
}

Bool LIBZ_UnzGoToFirstFile(HZIP hZip) {
  return (cocos2d::unzGoToFirstFile (hZip)==UNZ_OK);
}

//TODO
Bool LIBZ_UnzGetCurrentFileInfo(HZIP hZip, Char* filename, S32 len, S32* file_size)
{
  unz_file_info info;
  S32 ret = unzGetCurrentFileInfo((unzFile)hZip, &info, filename, (uLong)len, 0, 0, 0, 0);
  *file_size = info.uncompressed_size;
  
  return (ret==UNZ_OK);
}


Bool LIBZ_UnzGetCurrentFileInfo64(HZIP hZip,
                                 cocos2d::unz_file_info64 * pfile_info,
                                 Char *szFileName, U32 fileNameBufferSize,
                                 void *extraField, U32 extraFieldBufferSize,
                                 Char *szComment,  U32 commentBufferSize)
{
  return (unzGetCurrentFileInfo64(hZip,
                                 pfile_info,
                                 szFileName, fileNameBufferSize,
                                 extraField, extraFieldBufferSize,
                                 szComment,  commentBufferSize)==UNZ_OK);
}



Bool LIBZ_UnzLocateFile(HZIP hZip, const char* szFileName, S32 iCaseSensitivity)
{
  return (cocos2d::unzLocateFile(hZip, szFileName, iCaseSensitivity)==UNZ_OK);
}

Bool LIBZ_UnzOpenCurrentFile (HZIP hZip)
{
  return (cocos2d::unzOpenCurrentFile (hZip)==UNZ_OK);
}

Bool LIBZ_ReadCurrentFile(HZIP hZip, void* buffer, U32 len)
{
  return (cocos2d::unzReadCurrentFile  (hZip, buffer, len)==UNZ_OK);
}

Bool LIBZ_UnzReadCurrentFile  (HZIP hZip, voidp buffer, U32 len)
{
  return (cocos2d::unzReadCurrentFile(hZip, buffer, len)==UNZ_OK);
}

Bool LIBZ_UnzCloseCurrentFile (HZIP hZip)
{
  return (cocos2d::unzCloseCurrentFile(hZip)==UNZ_OK);
}

Bool LIBZ_UnzGoToNextFile (HZIP hZip){
  return (cocos2d::unzGoToNextFile(hZip)==UNZ_OK);
}

ADK_END_NAMESPACE
