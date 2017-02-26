#ifndef __sycHashL2_h__
#define __sycHashL2_h__
#include "yi.h"
#include "yistd.h"
#include "yiList.h"

#define YOTSUYA_HASH_SIZE 128
#define YOTSUYA_HASH_SIZE_SHIFT 7

// ---------------------------------
typedef struct xssycHashL2
{
	int nArrayCnt;
	int nLayerCnt;
	
	int nTotalUsedMemory;
	int nTotalDataCnt;
	int nFreeLayerCnt;
	char szRootPath[256];

	cyiList*** pHashTableEntryPoint;
			
}ssycHashL2;


// ---------------------------------
class csycHashL2
{
public:
	// ---------------------------------
	ssycHashL2* Load(const char* root_path, int array_shift);

	// ---------------------------------
	void Delete();

	// ---------------------------------
	int AddRefData(Bit32 layer, void* pRef);


	// ---------------------------------
	cyiList* GetList(Bit32 layer); 

	// ---------------------------------
	void ReleaseList(cyiList* list);

	// ---------------------------------
	const void* GetData(Bit32 layer, int dataindex); 


private:
	HandleMutex m_hMutex;
	ssycHashL2* m_p;
};

#endif
