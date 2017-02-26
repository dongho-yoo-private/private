#include "yiStack.h"

// --------------------------------------
cyiStack::cyiStack()
	: cyiList()
{
	m_nLimitation=0;
}

// --------------------------------------
cyiStack::~cyiStack()
{
}

// ---------------------
 bool cyiStack::push(yiAnyValue data)
{
	if (m_nLimitation!=0 && m_nLimitation==cyiList::m_cnt)
	{
		return false;	
	}

	return insert(0, data);
}

// ---------------------
 bool cyiStack::pop(yiAnyValue & data)
{
	if (cyiList::m_cnt==0)
	{
		return false;	
	}
	
	data = cyiList::m_pTop->data;
	remove(cyiList::m_pTop);

	return true;
}
