#ifndef __yi_list_h__
#define __yi_list_h__

#include "yi.h"
#ifdef _WIN32
#include "yibase.h"
#endif
#include "yimutex.h"

typedef IntPtr yiAnyValue;

#ifndef xNODELOOP
#define xNODELOOP(node, list) \
	for (cyiNode* node=list->GetNodeTopFrom(); node; node=node->np)
#endif

#ifndef xNODELOOP_R
#define xNODELOOP_R(node, list) \
	for (cyiNode* node=list->GetNodeTailFrom(0); node; node=node->pp)
#endif


// -------------------------------
#ifdef _WIN32
class xEXPORT cyiNode : public cyiBase
#else
class cyiNode 
#endif
{
public:
	cyiNode* np;
	cyiNode* pp;

	union {
		yiAnyValue data;
		void*	   ref;
		IntPtr n;
	};
};

// -----------------------------------------------
typedef void (*fyiDeleteCallback)(void* val);

// -----------------------------------------------
typedef int (*fyiGetSizeCallback)(int index, void* val);

// -------------------------------
#ifdef _WIN32
class xEXPORT cyiList : public cyiBase
#else
class cyiList
#endif
{
public:
	// -----------------------
	cyiList(Bool bIsCreateMutex=False);

	// -----------------------
	cyiList(fyiDeleteCallback callback, Bool bIsCreateMutex=False);

	// -----------------------
	cyiList(cyiList* list);

	// -----------------------
	virtual ~cyiList();

	// -----------------------
	Bool add(yiAnyValue data);

	// -----------------------
	Bool addref(void* ref);

	// -----------------------
	Bool insert(int nTopFrom, yiAnyValue data);

	// -----------------------
	Bool insertref(int nTopFrom, void* data);

	// -----------------------
	void remove(int nTopFrom, fyiDeleteCallback f=0);

	// -----------------------
	void* removeEx(int nTopFrom);

	// -----------------------
	cyiNode* removenode(cyiNode* node);

	// -----------------------
	Bool insertnode(int nTopFrom, cyiNode* node);

	// -----------------------
	void remove(cyiNode* node, fyiDeleteCallback f=0);

	// -----------------------
	void clear(fyiDeleteCallback f=0);

	// -----------------------
	void SetIdOffset(int offset, int size);

	// -----------------------
	unsigned long count();

	// -----------------------
	cyiNode* GetNodeTopFrom(int n=0);

	// -----------------------
	//void* GetFromId(int id);

	// -----------------------
	void* GetFromId(void* pId);



	// -----------------------
	void* GetTopFromRef(int n);

	// -----------------------
	cyiNode* GetNodeTailFrom(int n);

	// -----------------------
	void destroyer(fyiDeleteCallback callback);

	// -----------------------
	void lock();

	// -----------------------
	Bool islocked();
	
	// -----------------------
	void unlock();

	// -----------------------
	void** ExportArray();

	// -----------------------
	Binary Export(const char* pszFileName, int nElementSize, void* header, int headerSize, unsigned int security=0, Bool bBackup=False);

	// -----------------------
	Binary Export(const char* pszFileName, fyiGetSizeCallback callback, void* header, int headerSize, unsigned int security=0, Bool bBackup=False);

	// -----------------------
	int	Import(const char* pszFileName, void* header, int headerSize, Bool bIsEncoded=False, Bool bBackup=False);

	// -----------------------
	int	Import(void* p, int n, void* header, int headerSize, Bool bIsEncoded=False, Bool bBackup=False);

	// -----------------------
	void DeleteArray(void** pArray);

	// -----------------------
	int GetReferenceIndex(void* ref);

	// -----------------------
	void SetElementSize(int size);

	// -----------------------
	int GetElementSize();

protected:
	unsigned long m_cnt;
	cyiNode* m_pTop;
	cyiNode* m_pTail;
	fyiDeleteCallback m_deletef;
	Handle m_hMutex;
	int m_nElementSize;
	int m_nIdOffset;
	int m_nIdSize;

private: // member functions.
	// -----------------------------
	inline cyiNode* __get__(int n);

	// -----------------------
	inline cyiNode* __getr__(int n);
	
	// -----------------------------
	Bool __create_node__(int n, void* ref);

};

#endif
