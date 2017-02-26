#include <unistd.h>

#include <core/debug/debug_assert.h>
#include <core/debug/logging.h>
#include <core/library/lib_string.h>
#include <network/http/intern/http.h>
#include <network/http/adk_np_http_method.h>
#include <network/http/adk_np_http_packet.h>
#include <network/http/intern/np_cmn.h>

ADK_BEGIN_NAMESPACE

// --------------------------------------
NpHttpMethod::NpHttpMethod()
:
mhHttp(0),
mhMutex(0),
mResponse()
{
	ADK_ASSERT(mhMutex = (HandleMutex)Malloc(LIB_MutexGetHandleSize()));
	ADK_ASSERT(LIB_MutexCreateInBuffer(mhMutex));
}

// --------------------------------------
NpHttpMethod::~NpHttpMethod()
{
	if (mhMutex)
	{
		LIB_MutexDestroy(mhMutex);
	}
	mhMutex=0;
	mhHttp=0;

}

// --------------------------------------
Bool NpHttpMethod::IsTypeRest() 
{
	return true;
}

// --------------------------------------
Bool NpHttpMethod::IsMyProtocol(const Char* protocol)
{
	if (LIB_strcmpi(protocol, "http")==0)
		return true;
	if (LIB_strcmpi(protocol, "https")==0)
		return true;
	return false;
}

TNPError NpHttpMethod::Request(INpPacket* _packet, S32 timeout/*=NO_TIMEOUT*/)
{
	NpHttpPacket* packet= (NpHttpPacket*)_packet;
	THttpError error;
	THttpDetail detail;
	
	LIB_MutexLock(mhMutex, false);

	// disconnect.
	if (mhHttp!=0 &&
			HttpIsConnected(mhHttp)==true)
	{
		HttpClose(mhHttp);
	}

	HttpSetDefaultDetail(&detail);

	detail.useSSL			= packet->mUseSSL;
	detail.connectTimeout		= timeout;
	detail.readTimeout		= timeout;
	detail.port			= packet->mPort;
	detail.charset			= 0;
	detail.contentType		= 0;
	detail.keepAliveTimeout		= 0;

	address_t addr = SearchHostAddr(packet->mHostName);
	if (addr==0)
	{
		addr = NPCmnGetHostAdr(packet->mHostName);
		if (addr==0)
		{
			FATAL_LOG("%s unknown host\n", packet->mHostName);
			LIB_MutexUnlock(mhMutex);
			return kNPE_UNKNOWN_HOST;
		}
		AddHostAddr(addr, packet->mHostName);//LIB_strdup(packet->mHostName));
	}

	//DEBUG_LOG("NpHttpMethod::Request() addr:%x, hostname:%s\n", addr, packet->mHostName);

	if (mhHttp==0)
	{
		mhHttp = (HandleHttp)Malloc(HttpGetHandleSize());
	}
	HandleHttp hHttp = HttpCreateInBuffer(mhHttp, addr, packet->mHostName, &detail, &error);

	if (hHttp==0)
	{
		FATAL_LOG("NpHttpMethod::Request() HttpCreate Failure!\n");
		LIB_MutexUnlock(mhMutex);
		return error;//xConvertNpError(error);
	}

	//DEBUG_LOG("NpHttpMethod::Request() HttpMessage\n");
	error = HttpMessage(mhHttp, (const char*)packet->GetHeader(), (const void*)packet->GetData(), packet->GetDataSize());
	//HttpPost(path, data, dataSize, contentType, characterSet);

	if (error!=kNPE_SUCCESS)
	{
		FATAL_LOG("NpHttpMethod::Request() HttpMessage Failure!\n");
		LIB_MutexUnlock(mhMutex);
		HttpClose(mhHttp);
		return error;
	}

	_packet->SetResponse(HttpGetResponse(mhHttp));
	HttpClose(mhHttp);

	LIB_MutexUnlock(mhMutex);
	return kNPE_SUCCESS;
}

void NpHttpMethod::Close(Bool bIsForced)
{
	int retryCnt=MAX_HTTP_CLOSE_WAIT_RETRY_COUNT;

	if (bIsForced)
	{
		HttpClose(mhHttp);
		mhHttp=0;
	}

__RETRY:
	// ここで通信待ちにする。
	if (retryCnt!=0 && mhMutex!=0 &&
			LIB_MutexLock(mhMutex, true)==false)
	{
		DEBUG_LOG("NpHttpMethod... wait\n");
		sleep(1);
		retryCnt--;
		goto __RETRY;
	}

	if (mhMutex)
	{
		LIB_MutexUnlock(mhMutex);
	}

	if (mhHttp)
	{
		HttpClose(mhHttp);
		mhHttp=0;
	}


}

ADK_END_NAMESPACE

