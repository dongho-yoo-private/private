#ifndef __yiQueue_h__
#define __yiQueue_h__

#include "yiList.h"

// ------------------------------
class cyiQueue : public cyiList
{
public:
	// ---------------------
	cyiQueue();

	// ---------------------
	~cyiQueue();

	// ---------------------
	void SetLimitation(int n);

	// ---------------------
	bool enq(yiAnyValue data);

	// ---------------------
	bool deq(yiAnyValue& data);
	

private:
	unsigned long m_nLimitation;
};

#endif

