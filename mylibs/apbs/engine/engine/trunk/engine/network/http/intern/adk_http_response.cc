#define HTTP_USING_C_PLUS_PLUS

#include <network/http/adk_http_response.h>

#include <core/memory_allocation/guarded_alloc.h>
#include <core/library/lib_string.h>

#ifdef HTTP_USING_C_PLUS_PLUS


ADK_BEGIN_NAMESPACE

const Char* HttpResponse::GetContentType() const
{
	return response.contentType;
}

U32 HttpResponse::GetStatus() const
{
	return response.status;
}

Bool HttpResponse::IsContentTypeSame(const Char* contentType) const 
{
	return LIB_strcmpi(contentType, response.contentType)==0;
}


S32 HttpResponse::GetDataSize() const
{
	return response.size;
}

const Char* HttpResponse::GetData() const
{
	return response.data;
}

void HttpResponse::operator delete(void* p)
{
	ADK_Free(p);	
}

ADK_END_NAMESPACE
#endif
