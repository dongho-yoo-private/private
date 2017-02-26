#ifndef __yiStack_h__
#define __yiStack_h__

#include "yiList.h"

// ------------------------------
class cyiStack : protected cyiList
{
public:
	// ---------------------
	cyiStack();

	// ---------------------
	~cyiStack();

	// ---------------------
	void SetLimitation(int n);

	// ---------------------
	bool push(yiAnyValue data);

	// ---------------------
	bool pop(yiAnyValue& data);

private:
	unsigned long m_nLimitation;
};

#endif

