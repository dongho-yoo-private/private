#ifndef __yiTree_h__
#define __yiTree_h__

//#include "yiBase.h"
#include "yiList.h"

// ------------------------------
#ifdef _WIN32
class xEXPORT cyiTreeNode : public cyiBase
#else
class cyiTreeNode 
#endif
{
public:
	cyiTreeNode();
	cyiTreeNode(yiAnyValue data);
	virtual ~cyiTreeNode();

	// ----------------------------
	Bool add(cyiTreeNode* node);

	// ----------------------------
	Bool insert(cyiTreeNode* node, int index);

	// ----------------------------
	yiAnyValue GetData();

	// ----------------------------
	void SetData(yiAnyValue data);

	// ----------------------------
	//
	//FindNode(cyiTreeNode<SgfNode*>* sgfNode)
	//{
	//   cyiTreeNode<sgfNode*>* node = sgfNode;
	//
	//   cyiNode<cyiTreeNode<SgfNode*>>* pNode = node->GetChildList().GetTopFrom(0);
	//
	//   while(pNode)
	//   {
	// 	cyiTreeNode<SgfNode*>* tree = pNode->data; 
	//
	// 	FindNode(tree);
	// 	pNode = pNode->np;
	//   }
	//
	//
	// return ;
	//}
	cyiList& GetChildList() ;

	// ----------------------------
	cyiTreeNode* GetParent();

public:
	cyiTreeNode* m_parent;
	cyiList m_child;

private:

	yiAnyValue m_data;

	// ----------------------------
	static void __delete_node__(void* pNode);
};

#endif
