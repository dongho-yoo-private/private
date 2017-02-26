#include "yiQueue.h"

// --------------------------------------
 cyiQueue::cyiQueue(Bool bMutexCreate)
{
	cyiList::cyiList(bMutexCreate);
	m_nLimitation=0;
}

 // --------------------------------------
 cyiQueue::~cyiQueue()
{
	cyiList::~cyiList();
}

// ---------------------
 Bool cyiQueue::enq(yiAnyValue data)
{
	if (m_nLimitation!=0 && m_nLimitation==cyiList::m_cnt)
	{
		return false;	
	}

	return add(data);
}

// ---------------------
 Bool cyiQueue::deq(yiAnyValue& data)
{
	if (cyiList::m_cnt==0)
	{
		return false;	
	}
	
	data = cyiList::m_pTop->data;
	remove(cyiList::m_pTop);

	return true;
}
