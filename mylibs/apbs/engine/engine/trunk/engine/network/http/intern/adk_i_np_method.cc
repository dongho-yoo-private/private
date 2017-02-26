#include <network/http/adk_i_np_method.h>

#include <core/debug/debug_assert.h>
#include <core/debug/logging.h>
#include <core/library/lib_string.h>
#include <core/memory_allocation/guarded_alloc.h>
#include <network/http/adk_np_types.h>


ADK_BEGIN_NAMESPACE

char* 	INpMethod::mpStaticMemoryTop=0;
U32	INpMethod::mStaticMemoryOffset=0;

typedef struct {
	U32 hashKey;    /** \brief */
	address_t addr; /** \brief */
	int len;
	char *hostname;
} THostnameAndAddr;

static THostnameAndAddr g_hosts[xMAX_HOST_COUNT] = {0, };
static S32 g_hostsCount=0;

INpMethod::INpMethod()
: mNext(0)
{
	if (mpStaticMemoryTop==0)
	{
		mpStaticMemoryTop = (char*)ADK_Malloc(NP_COMMON_GLOBAL_MEMORY_SIZE, "INpMethod's Heap");
		ADK_ASSERT(mpStaticMemoryTop);
	}
}

INpMethod::~INpMethod()
{
	if (mNext)
	{
		delete mNext;
	}

	if (mpStaticMemoryTop)
	{
		ADK_Free(mpStaticMemoryTop);
		//free(mpStaticMemoryTop);
		mpStaticMemoryTop=0;
	}
}


TNPError INpMethod::Connect(const char* hostname, const Char* id, const Char* password, S32 timeout)
{
	// not supported.
	return kNPE_UNKNOWN;
}

#if 0
void INpMethod::Close()
{
	// not supported.
}
#endif

INpMethod* INpMethod::SetNext(INpMethod* _next)
{
	mNext=_next;
	return mNext;
}

static S32 xGetHashKey(const Char* str)
{
	char* p = (char*)str;
	int sum=0;
	while(*p)
	{
		sum+=(S32)*p++;
	}
	return sum;
}


address_t INpMethod::SearchHostAddr(const Char* hostName)
{
	S32 hashKey = xGetHashKey(hostName);
	for (int i=0; i<g_hostsCount; i++)
	{
		if (g_hosts[i].hashKey==hashKey)
		{
			if (strncmp(hostName, g_hosts[i].hostname, g_hosts[i].len)==0)
			{
				return g_hosts[i].addr;
			}
		}
	}

	return 0;
}

address_t INpMethod::AddHostAddr(address_t addr, const char* hostname)
{
	if (g_hostsCount==xMAX_HOST_COUNT)
	{
		return 0;
	}

	int len = strlen(hostname);

	g_hosts[g_hostsCount].addr = addr;
	g_hosts[g_hostsCount].hostname = (char*)Malloc(len+1);
	memcpy(g_hosts[g_hostsCount].hostname, hostname, len+1);
	g_hosts[g_hostsCount].hashKey = xGetHashKey(hostname);
	g_hosts[g_hostsCount++].len = len;

	return addr;	
}

void* INpMethod::Malloc(U32 size)
{
#if 1
	U32 top = mStaticMemoryOffset;
	ADK_ASSERT(mpStaticMemoryTop);

  size = ADK_ALIGN(size,16);
	//DEBUG_LOG("INpMethod::Malloc(%d)\n", size);
	if ((mStaticMemoryOffset+size)<NP_COMMON_GLOBAL_MEMORY_SIZE)
	{
		mStaticMemoryOffset+=size;
		return mpStaticMemoryTop+top;
	}
#else
	U32 top = mStaticMemoryOffset;
	ADK_ASSERT(mpStaticMemoryTop);

	//DEBUG_LOG("INpMethod::Malloc(%d)\n", size);
	if ((mStaticMemoryOffset+size)<NP_COMMON_GLOBAL_MEMORY_SIZE)
	{
		mStaticMemoryOffset+=size;
		return mpStaticMemoryTop+top;
	}
#endif

	return 0;
}

ADK_END_NAMESPACE
