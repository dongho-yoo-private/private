#ifndef __yitTree_h__
#define __yitTree_h__

#include "yitList.h"

// ------------------------------
template <class T> class cyitTreeNode
{
public:
	cyitTreeNode();
	cyitTreeNode(T* data);
	~cyitTreeNode();

	// ----------------------------
	Bool add(cyitTreeNode* node);

	// ----------------------------
	T* GetData();

	// ----------------------------
	void SetData(T* data);

	cyitList<cyitTreeNode>& GetChildList() ;

	// ----------------------------
	cyitTreeNode<T>* GetParent();

public:
	cyitTreeNode<T>* m_parent;
	cyitList<cyitTreeNode<T>> m_child;

private:

	T* m_data;

	// ----------------------------
	static void __delete_node__(void* pNode);
};


// ------------------------------
 template <class T> cyitTreeNode<T>::cyitTreeNode()
{
	m_child.clear();
	m_parent=0;
	m_data=0;
}

// ------------------------------
template <class T>  cyitTreeNode<T>::cyitTreeNode(T* data)
{
	m_child.clear();
	m_parent=0;
	m_data = data;
}

// ------------------------------
template <class T>  cyitTreeNode<T>::~cyitTreeNode()
{
	m_child.clear(cyitTreeNode<T>::__delete_node__);

	//if (m_data)
	//	delete (void*)m_data;
}

// ----------------------------
 template <class T> Bool cyitTreeNode<T>::add(cyitTreeNode* node)
{
	node->m_parent=this;
	return m_child.add(node);	
}


// ----------------------------
template <class T>  T* cyitTreeNode<T>::GetData()
{
	return m_data;
}

// ----------------------------
template <class T> void cyitTreeNode<T>::SetData(T* data)
{
	m_data = data;
}

// ----------------------------
template <class T> cyitList<cyitTreeNode<T>>& cyitTreeNode<T>::GetChildList() 
{
	return m_child;
}


// ----------------------------
 template <class T> cyitTreeNode<T>* cyitTreeNode<T>::GetParent()
{
	return m_parent;
}


// ----------------------------
 template <class T> void cyitTreeNode<T>::__delete_node__(void* pNode)
{
	delete pNode;
}

#endif
