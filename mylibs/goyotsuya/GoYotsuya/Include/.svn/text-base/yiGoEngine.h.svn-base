#pragma once

#include "yistd.h"
#include "yiGoDefs.h"
#include "stonemap.h"
#include "sgfmanager.h"

enum eCurrentState {
	eCS_READY,		// 初期状態
	eCS_EDIT,		// 編集状態
	eCS_PLAY,		// 対局状態
	eCS_CALCULATE,	// 地合い計算状態
	eCS_END			// 終局状態
};

// --------------------------------
enum eGoFormat {
	eGF_SGF,
	eGF_BINARY
};



//enum eGoNodeType {
//	eGNT_NORMAL=0,
//	eGNT_JOSEKI_DICTIONARY_EDITOR,
//	eGNT_REFERENCE, 
//	eGNT_REFERENCE2,
//	eGNT_PREVIEW
//};

// --------------------------------
typedef struct xsResult {
	int nBlackDeadStone;
	int nWhiteDeadStone;
	int nKomi;
	int nBlackArea;
	int nWhiteArea;
}sResult;

// --------------------------------
// 31-30: branch view mode. 
// 29-27: mode
// 26-24: status
// 23-22: format
enum eGoEngineFlag {
	eGEF_HIDE_CURSOR=0,
	eGEF_CALCULATE_MODE,
	eGEF_NOW_SEARCH_RESULT, 
	eGEF_NOW_SHOW_RESULT,
};

#define xBITZ_MASK(flag, to, from) (((((0xFFFFFFFF)>>from)<<from)<<(31-to))>>(31-to))
#define xBITZ_VALUE(flag, to, from) ((xBITZ_MASK(flag, to, from)&flag)>>from)
#define xCLEAR_BITZ(flag, to, from) (flag&=(~(xBITZ_MASK(flag, to, from))))

#define xGET_BRANCH_VIEW_MODE(flag) ((eBranchViewMode)xBITZ_VALUE(flag, 31, 30))
#define xSET_BRANCH_VIEW_MODE(flag, value) (flag=((xCLEAR_BITZ(flag, 31, 30)|((bit32_t)value<<30))))

#define xGET_GOBAN_MODE(flag) ((eGoContentsMode)xBITZ_VALUE(flag, 29, 27))
#define xSET_GOBAN_MODE(flag, value) (flag=((xCLEAR_BITZ(flag, 29, 27)|((bit32_t)value<<27))))

#define xGET_GOBAN_STATUS(flag) ((eCurrentState)xBITZ_VALUE(flag, 26, 24))
#define xSET_GOBAN_STATUS(flag, value) (flag=((xCLEAR_BITZ(flag, 26, 24)|((bit32_t)value<<24))))

#define xGET_GO_FORMAT(flag) ((eGoFileType)xBITZ_VALUE(flag, 23, 22))
#define xSET_GO_FORMAT(flag, value) (flag=((xCLEAR_BITZ(flag, 23, 22)|((bit32_t)value<<22))))

#define xIS_BIT(flag, n) ((flag&(1<<n))!=0)
#define xON_BIT(flag, n) (flag|=(1<<n))
#define xOFF_BIT(flag, n) (flag&=~(1<<n))
#define xSET_BIT(flag, n, bIsTrue) (flag=(bIsTrue?xON_BIT(flag, n):xOFF_BIT(flag,n)))




#define MOVE_TOP 0x1FFFFFFF
#define MOVE_END 0x0FFFFFFF

typedef Handle HandleCommentList;

class cglsGoban;

// --------------------------------
// 修正箇所
// コウの時の挙動
// とられるところに石をいれない。

class xEXPORT cyiGoEngine
{
public:
	cyiGoEngine(void);
	//cyiGoEngine(unsigned long id, sGoHeader* header=0);
	//cyiGoEngine(const char* pszFileName, eGoFormat format=eGF_SGF);

	~cyiGoEngine(void);

	// --------------------------------
	static unsigned int DefaultCondition(unsigned int& flag);


	// --------------------------------
	static void xDefaultDetail(sGobanContentsDetail* detail);

	// --------------------------------
	static goid_t GenerateGoId();

	// --------------------------------
	static char** CreateMap(cyiTreeNode* node);

	// --------------------------------
	Bool Create(sSimpleGoData* data, int size, const char* pszBlackName=0, const char* pszWhiteName=0, eInputMode=eIM_EDIT, unsigned int myId=0);

	// --------------------------------
	Bool Create(sGoHeader* header=0, sGobanContentsDetail* detail=0);

	// --------------------------------
	Bool Create(const char* pszFileName, sGobanContentsDetail* detail=0);//eGoNodeType eType=eGNT_NORMAL, eCharacterSet eLocal=eCS_JAPANESE);

	// --------------------------------
	Bool Create(const void* pBlock, int size, sGobanContentsDetail* detail=0);

	// --------------------------------
	Bool Destroy();

	// --------------------------------
	Bool Destroy(unsigned long id, sGoHeader* header=0);

	// -----------------------------------
	Bool IsChanged();

	// -----------------------------------
	Bool MakeTitle(char* title);

	// -----------------------------------
	void SetChangedFlag(Bool bIsChanged);

	// -----------------------------------
	sGoHeader* GetSgfHeader();

	// --------------------------------
	int GetCurrentOrder();

	// --------------------------------
	// 石のマップを返す。
	stone_t** PutStone(stone_t stone, aStoneList list, Bool bIsAdd=False, Bool bIsPlayMode=False);

	// ----------------------------------
	void GetDeadStone(int& nBlack, int& nWhite);

	// --------------------------------
	Bool PutLabel(int i, int j,label_t label);

	// --------------------------------
	Bool RemoveLabel(int i, int j);

	// --------------------------------
	Bool PutComment(const char* pszComment, Bool bIsOverwrite=True);

	// --------------------------------
	Bool GetNextStone();

	// --------------------------------
	stone_t** Goto(int nIndex, aStoneList branches, Bool bIgnoreBranch=False);

	// --------------------------------
	stone_t** Play(int nIndex);

	// --------------------------------
	stone_t** PlayByNode(cyiTreeNode* node);

	// --------------------------------
	cyiTreeNode* GetTopNode();

	// --------------------------------
	HandleCommentList FindCommentList();

	// --------------------------------
	stone_t** GotoNextComment(int index=-1);

	// --------------------------------
	stone_t** GotoPrevComment();

	// --------------------------------
	void CloseCommentList();

	// --------------------------------
	stone_t** GotoNextBranch(aStoneList list);

	// --------------------------------
	stone_t** GotoPrevBranch(aStoneList list);

	// --------------------------------
	stone_t** StartCalculate();

	// --------------------------------
	stone_t** EndCalculate(sResult* res=0);

	// --------------------------------
	cyiTreeNode* CreateReference(const char* refName, int& index);

	// --------------------------------
	cyiList* GetReference();

	// --------------------------------
	Bool AddNode(cyiTreeNode* node);

	//// --------------------------------
	//cyiTreeNode* GetTopNode();

	// --------------------------------
	void InitStone(int i=-1, int j=-1);

	// --------------------------------
	void StartInitStone(int count);

	// -----------------------------------
	void AddCommentCode(int nCommentCode);

	// -----------------------------------
	int GetCommentCode();

	// ほぼ定石辞典用
	// -----------------------------------
	void SetOptionCode(int Option);

	// -----------------------------------
	int GetOptionCode();

	// -----------------------------------
	void SetStatus(eGoResultStatus status);

	// -----------------------------------
	eGoResultStatus GetStatus();

	// -----------------------------------
	void SetFlag(int flag);

	// -----------------------------------
	int GetFlag();

	// --------------------------------
	//int** SelectBranch(int nIndex);

	// --------------------------------
	// 配列形式であり、最大64個、ある意味枝は64個以上は作れない。
	int GetBranchList(aStoneList list);

	// --------------------------------
	const char* GetTitle();

	// --------------------------------
	void SetTitle(const char* pszTitle);

	// --------------------------------
	char* GetComment();

	// --------------------------------
	cyiList* GetLabel(Bool* bIsReqFree=0);

	// --------------------------------
	void DeleteLabel(cyiList*list);

	// --------------------------------
	stone_t GetLastStone();

	// --------------------------------
	Bool IsCurrentOrderBlack();

	// --------------------------------
	Bool IsCurrentOrderMine();


	// --------------------------------
	int GetInitStoneCount();

	// --------------------------------
	Bool Load(const char* pszFileName);

	// --------------------------------
	Bool Save(const char* pszFileName, eGoFileType type, void* header=0, int nHeaderSize=0);

	// --------------------------------
	sSimpleGoData* Export(int* size);

	// --------------------------------
	Bool DeleteAt(int index/*=-1*/);

	// --------------------------------
	void GetAt(POINT& pos);

	// --------------------------------
	stone_t GetStone(int i, int j);

	// --------------------------------
	stone_t GetStoneEx(int i, int j);

	// --------------------------------
	stone_t** GetStoneMap();

	// --------------------------------
	stone_t** GetStoneOrderMap();

	// --------------------------------
	void UpdateHeader(sGoHeader* header);

	// --------------------------------
	sGoHeader* GetHeader();

	// --------------------------------
	unsigned long GetId();

	// --------------------------------
	eCurrentState GetCurrentState();

	// --------------------------------
	eInputMode GetInputMode();

	// --------------------------------
	void SetInputMode(eInputMode mode);

	// --------------------------------
	eMouseMode GetMouseMode();

	// --------------------------------
	void SetMouseMode(eMouseMode mode);

	// --------------------------------
	eCurrentState GetState();

	// --------------------------------
	void SetState(eCurrentState state);

	// --------------------------------
	goid_t GetGoId();

	// --------------------------------
	void SetShowNo(int n);

	// --------------------------------
	int GetShowNo();

	// --------------------------------
	unsigned int GetLineCount();

	// --------------------------------
	Bool IsValidHit(stone_t stone);

	// --------------------------------
	void SetRemoteId(int n);

	// --------------------------------
	int GetRemoteId();

	// --------------------------------
	stone_t** SetDeadStone(stone_t stone, int* nBlackDeadStone=0, int* nWhiteDeadStone=0);

	// --------------------------------
	void SetLocale(eCharacterSet locale=eCS_JAPANESE);

	// --------------------------------
	eCharacterSet GetLocale();

	// --------------------------------
	Bool IsYgfFormat();

	// --------------------------------
	Bool AddNewBranch();

	// --------------------------------
	Bool DeleteNode(cyiTreeNode* node=0);

	// -----------------------------------
	cyiTreeNode* GetCurrentNode();

	// -----------------------------------
	Bool SearchResult();

	// -----------------------------------
	Bool CreateMapImage(cyiTreeNode* node);


	// -----------------------------------
	int OptimizeNode(int* nProgress, Bool* canceled);

	// -----------------------------------
	int GetFullNodeCnt();

	// --------------------------------
	int GetBlockId(int i, int j);

	// --------------------------------
	void SetExtraData(void* p);

	// --------------------------------
	void* GetExtraData();

	// Universal GoId
	ugoid_t m_ugoid;

	// --------------------------------
	Bool ConvertGobanDirection();

private:
	eInputMode m_input_mode;
	eInputMode m_prev_input_mode;
	eMouseMode m_mouse_mode;



	goid_t m_nReferenceFromId;
	cyiTreeNode* m_pLimitNode;

	cyiGoEngine* m_parent;


	// --------------------------------
	stone_t** PlayByHistory();


	// --------------------------------
	int SetAddedStoneToMap();

	// --------------------------------
	void SetInitStoneToMap();

	// -----------------------------------
	int GetHideBranches(cyiList& list);

	// --------------------------------
	void SetBranches(aStoneList branches);

	// --------------------------------
	void SumDeadStone(int n, stone_t stone=eST_INVALID);

	// --------------------------------
	void InitMember(sGobanContentsDetail* detail);

	// --------------------------------
	cSgfManager* GetSgfManager();


private:
	cSgfManager m_sgfMgr;
	cStoneMap m_map;
	cyiStack m_history;
	stone_t m_laststone;
	int m_nRemainInitStoneCnt;
	int m_nShowNo;
	int m_remoteId;

	eCharacterSet m_eLocale;
	char m_szFileName[256];
	HandleCommentList m_hCommentList;
	//eGoNodeType m_eNodeType;
	bit32_t m_flag;

	int m_nFreeInitStoneCount;

	// ----------------------------
	int m_nBlackDeadStone;

	// ----------------------------
	int m_nWhiteDeadStone;

	// -------------------------
	cyiList m_AddedStoneList;

	// -------------------------
	char m_szTitle[256];

	void* m_pUserData;

	

public:
	goid_t m_id;
	Bool m_bIsLocked;

friend class cglsGoban;

};
