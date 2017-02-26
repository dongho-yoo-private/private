#include "yiQueue.h"
#include "yitrace.h"

// --------------------------------------
cyiQueue::cyiQueue(Bool bUseMutex)
	: cyiList(bUseMutex)
{
	m_nLimitation=0;
}

// --------------------------------------
cyiQueue::~cyiQueue()
{
}


// ---------------------
 bool cyiQueue::enq(yiAnyValue data)
{
	if (m_nLimitation!=0 && m_nLimitation==cyiList::m_cnt)
	{
		return false;	
	}

	return add(data);
}

// ---------------------
 bool cyiQueue::deq(yiAnyValue& data)
{
	if (cyiList::m_cnt==0)
	{
		return false;	
	}
	
	data = cyiList::m_pTop->data;
	remove(cyiList::m_pTop);

	return true;
}
