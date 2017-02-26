#ifndef __yiQueue_h__
#define __yiQueue_h__

#include "yiList.h"

// ------------------------------
class xEXPORT cyiQueue : public cyiList
{
public:
	// ---------------------
	cyiQueue(Bool bIsMutexCreate=False);

	// ---------------------
	~cyiQueue();

	// ---------------------
	void SetLimitation(int n);

	// ---------------------
	Bool enq(yiAnyValue data);

	// ---------------------
	Bool deq(yiAnyValue& data);

private:
	unsigned long m_nLimitation;
};

#endif

