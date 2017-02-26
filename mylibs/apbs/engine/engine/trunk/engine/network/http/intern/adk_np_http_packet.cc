#include <network/http/adk_np_http_packet.h>

#include <core/debug/debug_assert.h>
#include <core/debug/logging.h>
#include <core/library/lib_string.h>
#include <network/http/intern/http.h>
#include <network/http/intern/np_cmn.h>

ADK_BEGIN_NAMESPACE

NpHttpPacket::NpHttpPacket(const Char* url, S32 keepAlivetime/*=0*/, 
			const Char* contentType/*="application/x-wwww-form-urlencoded"*/, 
							const Char* characterSet/*="utf-8"*/)
: mData(0),
mDataSize(0),
mHeaderSize(0)
{
	mResponse=0;
	mHttpHeader[0]=0;
	Set(url, keepAlivetime, contentType, characterSet);
}

NpHttpPacket::NpHttpPacket(const Char* url, const void* data, S32 dataSize, S32 keepAliveTime/*=0*/,
				const Char* contentType/*="application/x-wwww-form-urlencoded"*/, 
							const Char* characterSet/*="utf-8"*/)
: mData(0),
mDataSize(0),
mHeaderSize(0)
{
	mResponse=0;
	mHttpHeader[0]=0;
	Set(url, data, dataSize, keepAliveTime, contentType, characterSet);
}

NpHttpPacket::NpHttpPacket(const Char* url, const void* data, S32 dataSize, const TRequestHttpHeader* headers, int headerCount)
: mData(0),
  mDataSize(0),
  mHeaderSize(0)
{
	mResponse=0;
	mHttpHeader[0]=0;

	Set(url, data, dataSize, headers, headerCount);
}


NpHttpPacket::~NpHttpPacket()
{
}

void NpHttpPacket::Set(const Char* url, const void* data, S32 dataSize, const TRequestHttpHeader* headers, int headerCount)
{
	char hostname[MAX_HOSTNAME_LEN+1];
	const char* path = NPCmnUrlParse(url, mProtocol, hostname, &mPort);
	int len = strlen(hostname);
	ADK_ASSERT(len<MAX_HOSTNAME_LEN);

	HttpMakeGeneralHeader(mHttpHeader, sizeof(mHttpHeader), hostname, dataSize, mPort, path, headers, headerCount);

	mHeaderSize=strlen(mHttpHeader);
	strncpy(mHostName, hostname, MAX_HOSTNAME_LEN);

	mData=(void*)data;
	mDataSize=dataSize;

	mUseSSL=false;
	if (LIB_strcmpi(mProtocol, "https")==0)
	{
		DEBUG_LOG("protocol is ssl\n");
		mUseSSL=true;
	}
	mPort=(mPort==0)?((mUseSSL==true)?443:80):mPort;

	return ;
}


void NpHttpPacket::Set(const Char* url, S32 keepAlivetime/*=0*/, 
			const Char* contentType/*="application/x-www-form-urlencoded"*/, 
							const Char* characterSet/*="utf-8"*/)
{
	char hostname[MAX_HOSTNAME_LEN+1];
	
	const char* path = NPCmnUrlParse(url, mProtocol, hostname, &mPort);
	//printf("NPCmnUrlParse(%s, %s, %d, %s)\n", hostname, mProtocol, mPort, path);
	//DEBUG_LOG("NPCmnUrlParse(%s, %s, %d, %s)\n", hostname, mProtocol, mPort, path);

	mUseSSL=false;
	if (LIB_strcmpi(mProtocol, "https")==0)
	{
		DEBUG_LOG("protocol is ssl\n");
		mUseSSL=true;
	}
	mPort=(mPort==0)?((mUseSSL==true)?443:80):mPort;

	contentType  = (contentType==0)?"application/x-www-form-urlencoded":contentType;
	characterSet = (characterSet==0)?"utf-8":characterSet;

	char* header = HttpMakeHeaderInBuffer(mHttpHeader, MAX_HEADER_LEN+1, hostname, 0, mPort, path, keepAlivetime, contentType, characterSet);

	mHeaderSize=strlen(header);
	int len = strlen(hostname);
	ADK_ASSERT(len<MAX_HOSTNAME_LEN);
	strncpy(mHostName, hostname, MAX_HOSTNAME_LEN);
}

void NpHttpPacket::Set(const Char* url, const void* data, S32 dataSize, S32 keepAliveTime/*=0*/,
				const Char* contentType/*="application/x-www-form-urlencoded"*/, 
							const Char* characterSet/*="utf-8"*/)
{
	if (data==0)
	{
		Set(url, keepAliveTime, contentType, characterSet);
		return ;
	}

	char hostname[MAX_HOSTNAME_LEN+1];
	const char* path = (const char*)NPCmnUrlParse(url, mProtocol, hostname, &mPort);

	DEBUG_LOG("NPCmnUrlParse(%s, %s, %d, %s)\n", hostname, mProtocol, mPort, path);

	int len = strlen(hostname);
	ADK_ASSERT(len<MAX_HOSTNAME_LEN);
	strncpy(mHostName, hostname, MAX_HOSTNAME_LEN);

	mUseSSL=false;

	if (LIB_strcmpi(mProtocol, "https")==0)
	{
		mUseSSL=true;
	}
	mPort=(mPort==0)?((mUseSSL==true)?443:80):mPort;

	contentType  = (contentType==0)?"application/x-www-form-urlencoded":contentType;
	characterSet = (characterSet ==0)?"utf-8":characterSet ;

	//mHeader = HttpMakeHeader(hostname, dataSize, mPort, path, keepAliveTime, contentType, characterSet);
	char* header = HttpMakeHeaderInBuffer(mHttpHeader, MAX_HEADER_LEN+1, hostname, dataSize, mPort, path, keepAliveTime, contentType, characterSet);
	mHeaderSize=strlen(header);

	mData=(void*)data;
	mDataSize=dataSize;
}


const void* NpHttpPacket::GetHeader()
{
	return mHttpHeader;
}

U32 NpHttpPacket::GetHeaderSize()
{
	return mHeaderSize;
}

const void* NpHttpPacket::GetData()
{
	return (const void*)mData;
}

U32 NpHttpPacket::GetDataSize()
{
	return mDataSize;
}
const Char* NpHttpPacket::GetProtocol()
{
	/* "http" or "https" */
	return mProtocol; 
}

const HttpResponse* NpHttpPacket::GetHttpResponse()
{
	return (const HttpResponse*)mResponse;
}

void NpHttpPacket::DeleteResponse(const void* p)
{
	const HttpResponse* x = (const HttpResponse*)p;
	delete x;
}

ADK_END_NAMESPACE
