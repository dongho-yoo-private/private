#include <network/http/adk_np_http_response.h>

#include <core/library/lib_string.h>

ADK_BEGIN_NAMESPACE

// --------------------------------
NpHttpResponse::NpHttpResponse()
: mData(0),
  mStatus(0),
  mDataSize(0),
  mContentType(0)
{
}

// --------------------------------
NpHttpResponse::~NpHttpResponse()
{
}

// --------------------------------
S32 NpHttpResponse::GetResult()
{
	return mStatus;
}
// --------------------------------
const void* NpHttpResponse::GetData()
{
	return mData;
}

// --------------------------------
U32 NpHttpResponse::GetDataSize()
{
	return mDataSize;
}
// --------------------------------
const Char* NpHttpResponse::GetContentType()
{
	return mContentType;
}
// --------------------------------
Bool NpHttpResponse::IsContentTypeSame(const Char* contentType)
{
	return LIB_strcmpi(contentType, mContentType)?false:true;
}


// --------------------------------
// this method called by NpHttpMethod only.
void NpHttpResponse::Set(S32 status, const void* data, U32 size, const Char* contentType)
{
	mStatus=status;
	mData=(const Char*)data;
	mDataSize=size;
	mContentType=contentType;
}

ADK_END_NAMESPACE
