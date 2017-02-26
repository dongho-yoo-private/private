#ifndef __yi_list_h__
#define __yi_list_h__

typedef unsigned long yiAnyValue;

// -------------------------------

class cyiNode
{
public:
	cyiNode* np;
	cyiNode* pp;

	yiAnyValue data;
	void*	   ref;
};

// -----------------------------------------------
typedef void (*fyiDeleteCallback)(void* val);

// -------------------------------
class cyiList
{
public:
	// -----------------------
	cyiList();

	// -----------------------
	cyiList(fyiDeleteCallback);

	// -----------------------
	~cyiList();

	// -----------------------
	bool add(yiAnyValue data);

	// -----------------------
	bool addref(void* ref);

	// -----------------------
	bool insert(int nTopFrom, yiAnyValue data);

	// -----------------------
	bool insertref(int nTopFrom, void* data);

	// -----------------------
	void remove(int nTopFrom, fyiDeleteCallback f=0);

	// -----------------------
	void remove(cyiNode* node, fyiDeleteCallback f=0);

	// -----------------------
	void clear(fyiDeleteCallback f=0);

	// -----------------------
	unsigned long count();

	// -----------------------
	cyiNode* GetNodeTopFrom(int n);

	// -----------------------
	cyiNode* GetNodeTailFrom(int n);

	// -----------------------
	void destroyer(fyiDeleteCallback callback);

protected:
	unsigned long m_cnt;
	cyiNode* m_pTop;
	cyiNode* m_pTail;
	fyiDeleteCallback m_deletef;

private: // member functions.
	// -----------------------------
	inline cyiNode* __get__(int n);

	// -----------------------
	inline cyiNode* __getr__(int n);
	
	// -----------------------------
	bool __create_node__(int n, yiAnyValue data, void* ref=0);

};

#endif
