#include "yiMemoryCashe.h"

// ----------------------------------
cyiMemoryCashe::cyiMemoryCashe()
	: m_MemoryBlockList(True),
	m_MemorySleepingQueue(True)
{
	m_nTotalMemorySize = 0;
	m_nTotalUsedMemorySize =0;
	m_nTotalSleepingMemorySize =0;
}

// ----------------------------------
cyiMemoryCashe::~cyiMemoryCashe()
{
}

// ----------------------------------
Bool cyiMemoryCashe::Create(int nMemorySize)
{
	m_nTotalMemorySize = nMemorySize;
}

// ------------------------------
void cyiMemoryCashe::Destroy()
{
}

