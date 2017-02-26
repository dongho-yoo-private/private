#ifndef __YI_MEMORY_CASHE_H__
#include "yi.h"
#include "yistd.h"

// -------------------------------------
typedef void (*__distructor__)(void* );

// -------------------------------------
typedef struct xsMemoryCashe {
	UINT size;
	Bool bIsUsed;
	UINT id;
	UINT reserved;
} sMemoryCashe;

typedef void* MemoryBlock;

// -------------------------------------
class cyiMemoryCashe
{
public:
	cyiMemoryCash();
	~cyiMemoryCash();

	// ------------------------------
	Bool Create(int nMemorySize);

	// ------------------------------
	void Destroy();

	// ------------------------------
	int  Register(MemoryBlock memblock, int fullSize, int id);

	// ------------------------------
	int  Descard(MemoryBlock memblock, __distructor__ fn);

	// ------------------------------
	MemoryBlock Get(MemoryBlock memblock);

	// ------------------------------
	static MemoryBlock malloc(int n);

private:
	unsigned long m_nTotalMemorySize;
	unsigned long m_nTotalUsedMemorySize;
	unsigned long m_nTotalSleepingMemorySize;

	cyiList m_MemoryBlockList;
	cyiQueue m_MemorySleepingQueue;
};

#endif
