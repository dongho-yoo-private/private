#include <gameplay/http_data_interface/startup/adk_startup_info.h>
#include <gameplay/http_data_interface/system/system.h>
#include <network/http/adk_i_np_packet.h>
#include <network/http/adk_network_protocol.h>
#include <network/http/adk_np_http_method.h>
#include <network/http/intern/np_cmn.h>

ADK_BEGIN_NAMESPACE

// ---------------------------------------
NetworkProtocol::NetworkProtocol()
: mMethodz(0),
mCurrentMethod(0)
{
}

// ---------------------------------------
NetworkProtocol::~NetworkProtocol()
{
	if (mMethodz)
	{
		Terminate(false);
	}
}


// ---------------------------------------
Result NetworkProtocol::Initialize()
{
#if 0
	System* system = System::GetSystem();
	ADK_ASSERT(system);

	ADK_StartupInfo* info 	= system->GetStartupInfo();
	mReadTimeout		= info->GetReadTimeout();
#endif
	mMethodz = new NpHttpMethod();

	if (mMethodz==0)
	{
		return ADK_RESULT(ResultOutOfRange, true, true, kNetworkProtocolComponent, 0, kNPE_NO_MEMORY);
	}

	return ResultSuccess;
}

// ---------------------------------------
Result NetworkProtocol::Connect(const char* hostname, S32 port, const Char* name, const Char* password)
{
	// 未対応
	return ResultUnknown;
}

// ---------------------------------------
void NetworkProtocol::Close()
{
	// 未対応
	return ;
}

// ---------------------------------------
Result NetworkProtocol::Request(INpPacket* packet, S32 timeout)
{
	ADK_ASSERT(packet);
	ADK_ASSERT(mMethodz);

	if (mCurrentMethod==0)
	{
		for (mCurrentMethod=mMethodz;mCurrentMethod;mCurrentMethod=mCurrentMethod->mNext)
		{
			if (mCurrentMethod->IsMyProtocol(packet->GetProtocol())==true)
			{
				break;
			} /* if */
		} /* for */
	} /* if */

	if (mCurrentMethod==0)
	{
		// TODO. not support protocol.
		return ResultUnknown;
	}

	TNPError error = mCurrentMethod->Request(packet, timeout/*mReadTimeout*/);

	if (error!=kNPE_SUCCESS)
	{
		return ADK_RESULT(ResultUnknown, true, false, kNetworkProtocolComponent, 0, error);
	}

	return ResultSuccess;
}

// ---------------------------------------
Result NetworkProtocol::Terminate(Bool bIsForced)
{
  if (mCurrentMethod) {
    mCurrentMethod->Close(bIsForced);
  }
	delete mMethodz;
	mMethodz=0;
	return ResultSuccess;		
}


ADK_END_NAMESPACE

#ifdef __UNIT_TEST__
#include <../ADK_NpHttpReqPacket.h>
#include <../ADK_NpHttpResponse.h>
#include <../../error/result.h>
int main(int argc, char** argv)
{
	adk::NetworkProtocol np;
	adk::NpHttpReqPacket data("https://www.yahoo.co.jp/", 0,
					"urlencode-xxx-applitcation", "utf-8");
	adk::NpHttpResponse* response;

	adk::Result res = np.Request(&data, 1000);
	
	if (res!=adk::ResultSuccess)
	{
		printf("request error\n");
		return 0;
	}
	
	response = (adk::NpHttpResponse*)np.GetResponse();

	printf("status:%d\n", response->GetResult());
	printf("contenttype:%s\n", response->GetContentType());
	printf("%s\n", response->GetData());

	return 0;
}

#endif


