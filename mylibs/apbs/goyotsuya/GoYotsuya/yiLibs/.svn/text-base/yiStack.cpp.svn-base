#include "yiStack.h"

// --------------------------------------
 cyiStack::cyiStack()
{
	cyiList::cyiList();
	m_nLimitation=0;
}

 // --------------------------------------
cyiStack::~cyiStack()
{
	cyiList::~cyiList();
}

// ---------------------
 Bool cyiStack::push(yiAnyValue data)
{
	if (m_nLimitation!=0 && m_nLimitation==cyiList::m_cnt)
	{
		return False;	
	}

	return insert(0, data);
}

// ---------------------
 Bool cyiStack::pop(yiAnyValue& data)
{
	if (cyiList::m_cnt==0)
	{
		return False;	
	}
	
	data = cyiList::m_pTop->data;
	remove(cyiList::m_pTop);

	return True;
}
