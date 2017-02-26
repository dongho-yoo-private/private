#include "yiTreeNode.h"

// ------------------------------
 cyiTreeNode::cyiTreeNode()
{
	m_child.clear();
	m_parent=0;
	m_data=0;
}

// ------------------------------
 cyiTreeNode::cyiTreeNode(yiAnyValue data)
{
	m_child.clear();
	m_parent=0;
	m_data = data;
}

// ------------------------------
 cyiTreeNode::~cyiTreeNode()
{
	m_child.clear(cyiTreeNode::__delete_node__);

	//if (m_data)
	//	delete (void*)m_data;
}

// ----------------------------
 Bool cyiTreeNode::add(cyiTreeNode* node)
{
	node->m_parent=this;
	return m_child.add((yiAnyValue)node);	
}

// ----------------------------
Bool cyiTreeNode::insert(cyiTreeNode* node, int index)
{
	node->m_parent=this;
	return m_child.insert(index, (yiAnyValue)node);	
}


// ----------------------------
 yiAnyValue cyiTreeNode::GetData()
{
	return m_data;
}

// ----------------------------
 void cyiTreeNode::SetData(yiAnyValue data)
{
	m_data = data;
}

// ----------------------------
cyiList& cyiTreeNode::GetChildList() 
{
	return m_child;
}


// ----------------------------
 cyiTreeNode* cyiTreeNode::GetParent()
{
	return m_parent;
}


// ----------------------------
 void cyiTreeNode::__delete_node__(void* pNode)
{
	delete pNode;
}
