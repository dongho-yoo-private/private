#pragma once

//#include "YotsuyaNode.h"
#include "yctrlGoDefs.h"
#include "yiTreeNode.h"
#include "yiStack.h"
#include "yistd.h"


// ----------------------------------------
/*typedef struct xsGoHeaderW {

} sGoHeaderW;*/


// ----------------------------------------
class __declspec(dllexport) cSgfStoneInfo {
public:
	cSgfStoneInfo(stone_t s=STONE_EOF, char l=0, char* str=NULL)
	{
		stone=s;
		label=l;
		comment=str;
	};
	~cSgfStoneInfo()
	{
		if (comment)
			::GlobalFree(comment);
	};

	stone_t stone;
	char label;
	char* comment;
};


// ----------------------------------------
class __declspec(dllexport) cSgfManager
{
public:
	cSgfManager(void);
	~cSgfManager(void);

	// -----------------------------------
	void Init();

	// -----------------------------------
	Bool ParseSgfHeader(void* sgf, sGoHeader* header);

	// -----------------------------------
	Bool ParseSgf(void* sgf, int nSize);

	// -----------------------------------
	Bool SaveSgfFile(LPCSTR pszFileName);

	// -----------------------------------
	Bool LoadSgfFile(LPCSTR pszFileName);

	// -----------------------------------
	void UnloadSgfFile(void* data);

	// -----------------------------------
	int Add(stone_t stone, char label, LPCSTR comment, BOOL bIsCurrentNodeFixed=FALSE);

	// -----------------------------------
	void InitStone(stone_t stone);

	// -----------------------------------
	bool Delete(cyiTreeNode* node=NULL);

	// -----------------------------------
	bool HasBranch(int i);

	// -----------------------------------
	LPCSTR GetTitle();

	// -----------------------------------
	void SetTitle(LPCSTR pszTitle);

	// -----------------------------------
	cyiTreeNode* Back();

	// -----------------------------------
	cyiTreeNode* Play(int nBranchIndex=0);

	// -----------------------------------
	long GetBranchCount();

	// -----------------------------------
	bool GetBranches();

	// -----------------------------------
	bool ToTop();

	// -----------------------------------
	void Copy(cSgfManager& mgr);

	// -----------------------------------
	bool GetBranches(aStoneList list);


private:
	void NodeCopy(cSgfManager& mgr);
	//cYotsuyaNode* m_node;
	cyiTreeNode m_node;
	cyiTreeNode* m_currentNode;
	cyiList m_InitStone;
	long m_blocksize;
	void* m_sgfRaw;

	char m_szTitle[256];

	cyiStack m_stack;

	Bool m_bIsLoadSuccess;

	sGoHeader m_header;

};
