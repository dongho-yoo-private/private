#ifndef __yiStack_h__
#define __yiStack_h__

#include "yiList.h"

// ------------------------------
class xEXPORT cyiStack : public cyiList
{
public:
	// ---------------------
	cyiStack();

	// ---------------------
	~cyiStack();

	// ---------------------
	void SetLimitation(int n);

	// ---------------------
	Bool push(yiAnyValue data);

	// ---------------------
	Bool pop(yiAnyValue& data);

private:
	unsigned long m_nLimitation;
};

#endif

