#ifndef __yiTree_h__
#define __yiTree_h__

#include "yiList.h"

// ------------------------------
class cyiTreeNode
{
public:
	cyiTreeNode();
	cyiTreeNode(yiAnyValue data);
	~cyiTreeNode();

	// ----------------------------
	bool add(yiAnyValue data);

	// ----------------------------
	yiAnyValue GetData();

	// ----------------------------
	void SetData(yiAnyValue data);

	// ----------------------------
	const cyiList<cyiTreeNode*>& GetChildList() ;

	// ----------------------------
	const cyiTreeNode* GetParent();

private:
	cyiList m_child;
	cyiTreeNode* m_parent;
	yiAnyValue m_data;

	// ----------------------------
	static void __delete_node__(void* pNode);
};

#endif
