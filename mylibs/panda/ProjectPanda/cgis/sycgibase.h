#ifndef __sycgibase_h__
#define __sycgibase_h__

#include "yi.h"

// -----------------------
class csycgibase
{
public:
	csycgibase();
	~csycgibase();

	// -----------------------
	Bool start(char* buffer, int nBufferSize);

	// -----------------------
	Bool GetValue(const char* name, char* pszValue, Bool bIsConvert=False);

private:
	char* m_pBuffer;
	int m_nLen;
};

#endif

