#pragma once

//#include "YotsuyaNode.h"
#include "yiTreeNode.h"
#include "yiStack.h"
#include "yistd.h"
#include "stonemap.h"
#include "yit.h"

#ifdef _WIN32
#include "yiBase.h"
#else
#include "yiw32.h"
#include "yiStream.h"
#endif



#ifdef _WIN32
extern void __declspec(dllexport) __xxSetRotateModeEnable();
#else
extern void __xxSetRotateModeEnable();
#endif


// ----------------------------------------
/*typedef struct xsGoHeaderW {

} sGoHeaderW;*/

#define __YGF_VERSION__ "YGFx"


#define GET_MASTER_LEVEL(flag) ((flag>>20)&0x0F)
#define SET_MASTER_LEVEL(flag, n) ((flag&0xFF0FFFFF)|(n<<20))

#define IS_PLAIED(flag) (((flag>>19)&0x01)==1)
#define SET_PLAIED(flag, b) ((flag&(~(1<<19)))|(b<<19))

#define IS_EDITED(flag) (((flag>>18)&0x01)==1)
#define SET_EDITED(flag, b) ((flag&(~(1<<18)))|(b<<18))

#define SET_JOSEKI_NAME(flag, a, b, c) (flag=((flag&(~(0x01FFF<<5)))|((a<<14)|(b<<10)|(c<<5))))
#define GET_JOSEKI_NAME(flag) ((flag>>5)&0x01FFF)

#define GET_JOSEKI_HIGH(joseki) (joseki>>9)
#define GET_JOSEKI_LOW(joseki) ((joseki>>5)&0x0F)
#define GET_JOSEKI_DETAIL(joseki) (joseki&0x1F)


typedef struct xsSgfExtendInfo
{
	// eGoResultStatus
	unsigned char status;
	unsigned char reserved;
	// eGoResultOption
	unsigned short option;
	unsigned short stonevalue;
	unsigned int flag32; // 31-28:�l�C�x, 27-24:��Փx, 23-20: �}�X�^�[�t���O, 19:�Đ��ς�, 18:�ҏW�ς�, 17-14:��Α啪�ށA13-10:�����ށA9-5:�ڍ�
	char* pszJosekiName;
	// using joseki dictionary
//	char** map;
} sSgfExtendInfo;

typedef unsigned char map_t[91];

#define REF_TOP ((cyiTreeNode*)1)


enum eSgfStoneInfoFlag {
	eSSIF_HIDE=1
};

// ----------------------------------------
#ifdef _WIN32
class xEXPORT cSgfStoneInfo : public cyiBase
#else
class xEXPORT cSgfStoneInfo 
#endif
{
public:
	cSgfStoneInfo(stone_t s=eST_INVALID)
	{
		stone=s;
		label=0;
		comment=0;
		nBlackCatchedStone=0;
		nWhiteCatchedStone=0;
		nOrder=0;
		m_AddStoneList=0;
		// using joseki dictionary	
		exinfo=0;
		map=0;
		ref=0;
		refNo=0;
		flag32=0;
		link=0;
		linkNo=0;
		reference=0;
		referenceFrom=0;
		referenceChild=0;

	};

	cSgfStoneInfo(cSgfStoneInfo* info)
	{
		stone=info->stone;
		label=info->label!=0?new cyiList(info->label):0;
		comment=yistrheap(info->comment);
		m_AddStoneList=info->m_AddStoneList? new cyiList(info->m_AddStoneList):0;
		nBlackCatchedStone=info->nBlackCatchedStone;
		nWhiteCatchedStone=info->nWhiteCatchedStone;
		nOrder=info->nOrder;
		flag32=info->flag32;
		
		if (info->exinfo)
		{
			exinfo=(sSgfExtendInfo*)yitoheap(info->exinfo, sizeof(sSgfExtendInfo));
		}
		map=0;
		ref=0;
		refNo=info->refNo;
		link=0;
		linkNo=info->linkNo;
		reference=0;
		referenceFrom=0;
		referenceChild=0;
	}

	virtual ~cSgfStoneInfo()
	{
		if (comment)
			yifree(comment);
		if (label)
			delete label;

		if (m_AddStoneList)
			delete m_AddStoneList;

		// TODO. Memory Leak.
		if (map)
		{

		}

		if (exinfo)
			yifree(exinfo);

		if (link)
			delete link;

		if (reference)
		{
			while(reference->count())
			{
				cyiTreeNode* node = (cyiTreeNode*)reference->removeEx(0);
				cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();
				delete info;
				delete node;
			}
		}

		if (referenceChild)
		{
			delete referenceChild;
		}
	};

	void set(cSgfStoneInfo& info)
	{
		// �R�����g
		if (info.comment)
		{
			if (comment)
				yifree(comment);
			comment = yistrheap(info.comment);
		}
		else
		{
			if (comment)
				yifree(comment);
			comment=0;
		}

		if (info.label)
		{
			if (label)
			{
				label->clear();
				delete label;
			}
			label = new cyiList(info.label);
		}

		if (info.m_AddStoneList)
		{
			if (m_AddStoneList)
			{
				m_AddStoneList->clear();
				delete m_AddStoneList;
			}
			m_AddStoneList = new cyiList(info.m_AddStoneList);
		}

		nBlackCatchedStone=info.nBlackCatchedStone;
		nWhiteCatchedStone=info.nWhiteCatchedStone;
		nOrder=info.nOrder;
		flag32=info.flag32;
		
		if (info.exinfo)
		{
			if (exinfo)
				yifree(exinfo);
			exinfo=(sSgfExtendInfo*)yitoheap(info.exinfo, sizeof(sSgfExtendInfo));
		}

	}

	Bool isSameStone(cSgfStoneInfo& info)
	{
		if (info.stone!=stone)
			return False;
		
		if (info.m_AddStoneList==0 && m_AddStoneList==0)
		{
			return True;
		}
		
		if (info.m_AddStoneList!=0 && m_AddStoneList!=0)
		{
			if (info.m_AddStoneList->count()!=m_AddStoneList->count())
			{
				return False;
			}

			int cnt=0;
			for (cyiNode* node = m_AddStoneList->GetNodeTopFrom();node; node=node->np)
			{
				stone_t stone = node->n;
				stone_t stone2 = (stone_t)info.m_AddStoneList->GetTopFromRef(cnt++);

				if (stone!=stone2)
				{
					return False;
				}
			}
			return True;
		}
		return False;
	}

	Bool isSame(cSgfStoneInfo& info)
	{
		if (info.comment!=0 &&
				comment!=0)
		{
			if (strcmp(info.comment, comment)!=0)
			{
				return False;
			}
		}
		else
		{
			if (info.comment!=comment)
			{
				return False;
			}
		}

		if (info.exinfo!=0 && exinfo!=0)
		{
			if (info.exinfo->flag32!=exinfo->flag32)
			{
				return False;
			}

			if (info.exinfo->option!=exinfo->option)
			{
				return False;
			}

			if (info.exinfo->status!=exinfo->status)
			{
				return False;
			}

			if (info.exinfo->stonevalue!=exinfo->stonevalue)
			{
				return False;
			}
		}
		else
		{
			if (info.exinfo!=exinfo)
			{
				return False;
			}
		}

		// �Ƃ肠�����́B
		return True;
	}

	Bool operator == (cSgfStoneInfo& info)
	{
		return isSame(info);
	}

	Bool operator!=(cSgfStoneInfo& info)
	{
		return 1-isSame(info);
	}

	stone_t stone;
	char* comment;

	// 31: 1 (��ΏI��)
	//
	bit32_t flag32;
	cyiList* label;
	cyiList* m_AddStoneList;
	unsigned short nBlackCatchedStone;
	unsigned short nWhiteCatchedStone;
	unsigned short nOrder;

	// �d���֘A
	cyiTreeNode* ref;
	int refNo;
	cyiList* referenceChild;

	cyiList* link;
	int linkNo;
	char** map;

	// �Q�l�}
	cyiList* reference;
	cyiTreeNode* referenceFrom;
	sSgfExtendInfo* exinfo;
};



//xEXPORT cSgfStoneInfo* xSgfStoneInfo()
//{
//	return new cSgfStoneInfo();
//}


template class cyitTreeNode<cSgfStoneInfo>;
typedef cyitTreeNode<cSgfStoneInfo> cSgfTreeNode;

template class cyitList<cSgfTreeNode>;
typedef cyitList<cSgfTreeNode> cSgfList;

template class cyitNode<cSgfStoneInfo>;
typedef cyitNode<cSgfStoneInfo> cSgfNode;


// --------------------------------
enum eYgfHeaderChunkId {
	eYHCID_TITLE=1, // (�T�C�Y16�r�b�g+�T�C�Y���̕�����)
	eYHCID_BLACK_NAME, //PB�@(�T�C�Y16�r�b�g+�T�C�Y���̕�����)
	eYHCID_WHITE_NAME, //PW�@(�T�C�Y16�r�b�g+�T�C�Y���̕�����)
	eYHCID_INITSTONE_COUNT, // HA�@(�u���΂̐��A1�o�C�g(0x80���t���Ă���ꍇ�́A�u���Ε\��(AB)�A������16�r�b�g)
	eYHCID_KOMI,//KO float�ŁA�}�C�i�X�͋t����
	eYHCID_PLAY_TIME, //TM �b�P��32�r�b�g
	eYHCID_PLAY_DATE, //DT 32�r�b�g
	eYHCID_BLACK_LEVEL, //BR 1�o�C�g
	eYHCID_WHITE_LEVEL, //WR�@1�o�C�g
	eYHCID_PLACE, //PC�@(�T�C�Y16�r�b�g+�T�C�Y���̕�����)
	eYHCID_PLAY_COMMENT, // GC�@(�T�C�Y16�r�b�g+�T�C�Y���̕�����)
	eYHCID_RESULT, //RE 16�r�b�g (15:0 W, 15:1 B, 14:1 half score, 13-0:score �����A14-0���[���Ȃ��������)
	eYHCID_RULE, //RU�@1�o�C�g
	eYHCID_COPYRIGHT, //CP�@(�T�C�Y16�r�b�g+�T�C�Y���̕�����)
	eYHCID_APPNAME, //AP�@(�T�C�Y16�r�b�g+�T�C�Y���̕�����)
	eYHCID_INIT_STONE, // 1�o�C�g���A������16�r�b�g�iAB, AW)
	eYHCID_LOCALE, // ���P�[��1�o�C�g
	eYHCID_LINE_COUNT,
	eYHCID_BLACK_TEAM,
	eYHCID_WHITE_TEAM,
	eYHCID_RO, // ��킢�i������j
	eYHCID_EOF = 0xFF // �w�b�_�[�I��
};

#define MAKE_SCMD(cmd, x, y) ((cmd<<10)|(x<<5)|y)
#define MAKE_CMD(cmd) (char)((cmd<<2))

// --------------------------------
enum eYgfCommandId {
	eYCID_B=1, // Black
	eYCID_W, // White
	eYCID_B_PASS, // BlackPass
	eYCID_W_PASS, // BlackPass
	eYCID_AB,
	eYCID_AW,
	eYCID_LABEL,
	eYCID_LABEL_SHAPE,
	eYCID_LABEL16,
	eYCID_COMMENT,
	eYCID_COMMENT_CODE,
	eYCID_ENTER_BRANCH,
	eYCID_LEAVE_BRANCH,
	eYCID_END
};

// ���x��Shape�ꗗ
// ��
// ��
// ��
// ��
// �~

// �ԁ~�F����
// �΁��F�D��
// �΁��F�ǂ���i�{��j
// �����F���܂�ǂ��Ȃ���
// �ԁH�F�͂܂�
// ���F����

//
enum eYgfLabelCode {
	eYLC_CR = 1,
	eYLC_TR,
	eYLC_RC,
	eYLC_MA,
	eYLC_CR2,
	eYLC_BAD2,
	eYLC_GOOD2,
	eYLC_GOOD,
	eYLC_BAD,
	eYLC_TRIC,
	eYLC_KIKI
};

// �R�����g�R�[�h�ꗗ

// ���+���
// ���ǂ�

// ��̃��X�g
// ��
// ��
// �Ȃ�

// ��ԃ��X�g
// ���Ȃ�悢
// �悢
// �����悢
// �݊p
// ��������
// ����
// ���Ȃ舫��
// �Ԃ�
// ���
// ����
// �͂܂�
// �}���Ƃ���



class cyiGoEngine;
class cglsGoban;

// ----------------------------------------
#ifdef _WIN32
class xEXPORT cSgfManager : public cyiBase
#else
class cSgfManager 
#endif
{
public:
	cSgfManager(void);
	virtual ~cSgfManager(void);

	// -----------------------------------
	// �n���Ƃ��̓q�[�v��̃�������n���B
	// �q�[�v�̍폜�́AcSgfManager�̒��ōs���B
	void Init(sGoHeader* header=0, int locale = eCS_SYSTEM);

	// -----------------------------------
	Bool ParseSgfHeader(void* sgf, sGoHeader* header);

	// -----------------------------------
	Bool ParseSgf(void* sgf, int nSize);

	// -----------------------------------
	Bool IsChanged();

	// -----------------------------------
	void SetChangedFlag(Bool bIsChanged);


	// -----------------------------------
#ifdef _WIN32
	Bool WriteSgfBody(HANDLE hFile, cyiTreeNode* t, Bool xxxFlag=False);
#else
	Bool WriteSgfBody(int fd, cyiTreeNode* t, Bool xxxFlag=False);
#endif


	// -----------------------------------
	Bool SaveSgfFile(LPCSTR pszFileName, Bool xxxFlag=False);

	// -----------------------------------
	static Bool WriteBinaryGoFormat(cyiMemoryStream* stream, cyiTreeNode* t, cyiTreeNode* pTopNode);

	// -----------------------------------
	Bool SaveBinaryGoFormat(LPCSTR pszFileName, void* header, int nHeaderSize);

	// -----------------------------------
	static cyiMemoryStream* ExportBinaryGoFormat(sGoHeader* goheader, cyiTreeNode* pTopNode, void* header=0, int nHeaderSize=0);

#if 0
	// -----------------------------------
	Bool SaveYgfFile(LPCSTR pszFileName);

	// -----------------------------------
	//Bool SaveYgfHeader(cyiMemoryIO* io);

	// -----------------------------------
	//Bool WriteYgfBody(cyiMemoryIO* io, cyiTreeNode* node);
#endif

	// -----------------------------------
	Bool LoadSgfFile(LPCSTR pszFileName, Bool bIsRepair=False, int locale = eCS_SYSTEM);

	// -----------------------------------
	Bool LoadBinaryGoFormat(LPCSTR pszFileName, void** header, int* size, int locale = eCS_SYSTEM);

	// -----------------------------------
	Bool LoadBinaryGoFormat(const void* p, int size, int locale = eCS_SYSTEM);

	// -----------------------------------
	Bool LoadBinaryGoFormat(IStream& stream, int locale = eCS_SYSTEM);

	// -----------------------------------
	Bool ImportBlock(const void* block, int size, Bool bRepair=False);

	// -----------------------------------
	Bool LoadYgfFile(LPCSTR pszFileName, int locale = eCS_SYSTEM);
#if 0
	// -----------------------------------
	Bool ParseYgfHeader(cyiMemoryIO* io, sGoHeader* header);

	// -----------------------------------
	Bool ParseYgf(cyiMemoryIO* io);
#endif

	// -----------------------------------
	void UnloadSgfFile(void* data);

	// -----------------------------------
	int GetNodeIndex(cyiTreeNode* t);

	// -----------------------------------
	int Add(stone_t stone, int nCatchedStone, Bool bIsAdded=False, Bool bIsNodeCreate=False);

	// -----------------------------------
	int AddLabel(int i, int j, wchar_t label);

	// -----------------------------------
	int RemoveLabel(int i, int j);

	// -----------------------------------
	int AddComment(const char* pszComment, Bool bIsOverwrite=True);

	// -----------------------------------
	void AddCommentCode(int nCommentCode);

	// -----------------------------------
	cyiTreeNode* AddReference(const char* pszRefName, int& nIndex);

	// -----------------------------------
	cyiTreeNode* CreateReference(const char* pszRefName, int& nIndex);

	// -----------------------------------
	Bool AddNode(cyiTreeNode* node);

	// -----------------------------------
	int GetCommentCode();

	// -----------------------------------
	sSimpleGoData* ExportSimpleGoData(int* size, int ver=2);

	// -----------------------------------
	Bool ImportSimpleGoData(sSimpleGoData* data, int size, const char* pszBlackName=0, const char* pszWhiteName=0);


	// -----------------------------------
	sStonePath* CreatePath();

// �قڒ�Ύ��T�p
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


	// -----------------------------------
	Bool InitStone(stone_t stone);

	// -----------------------------------
	cyiList* GetInitStoneList();

	// -----------------------------------
	int GetDeadStone(int& nBlack, int& nWhite);

	// -----------------------------------
	Bool Delete(cyiTreeNode* node=NULL);

	// -----------------------------------
	Bool IsNextHasPass();

	// -----------------------------------
	Bool HasBranch(int i);

	// -----------------------------------
	LPCSTR GetTitle();

	// -----------------------------------
	void SetTitle(LPCSTR pszTitle);

	// -----------------------------------
	stone_t GetNextStone(stone_t stone);

	// -----------------------------------
	Bool PrevBranch(int n=-1);

	// -----------------------------------
	stone_t Play(int nBranchIndex=0);

	// -----------------------------------
	stone_t PlayByNode(cyiTreeNode* node);

	// -----------------------------------
	stone_t Back();

	// -----------------------------------
	char* GetComment();

	// -----------------------------------
	cyiList* GetLabel();

	// --------------------------------
	cyiList* GetAddedStone();

	// -----------------------------------
	long GetBranchCount(aStoneList list=0);//, cyiList& otherNodeList);

	// -----------------------------------
	int GetHideBranches(cyiList& list);

	// -----------------------------------
	Bool ToTop();

	// -----------------------------------
	void Copy(cSgfManager& mgr);

	// -----------------------------------
	Bool IsNodeTop();

	// -----------------------------------
	Bool IsNodeEnd();

	// -----------------------------------
	Bool SearchNextBranch();

	// -----------------------------------
	Bool SearchPrevBranch();

	// -----------------------------------
	cyiList* GetCommentNodeList();

	// -----------------------------------
	void ReleaseCommentNodeList(cyiList* list);

	// -----------------------------------
	cyiList* GetNodeHistory(cyiTreeNode* node);

	// -----------------------------------
	void ReleaseNodeHistory(cyiList* list);

	// -----------------------------------
	Bool AddNewBranch(const char* brancheTag);

	// -----------------------------------
	Bool DeleteNode(cyiTreeNode* node);

	// -----------------------------------
	Bool Destroy();

	// -----------------------------------
	Bool DeleteChildren(cyiTreeNode* node);


	// -----------------------------------
	Bool SearchResult();

	// -----------------------------------
	cyiTreeNode* GetCurrentNode();

	// -----------------------------------
	int OptimizeNode(int* nProgress, Bool* canceled);

	// -----------------------------------
	int GetFullNodeCnt();

	// -----------------------------------
	int CreateLink();

	// -----------------------------------
	int DeleteLink();

	// -----------------------------------
	int AddLink(int no);

	// -----------------------------------
	int RemoveLink(int no);

	// -----------------------------------
	cyiTreeNode* SearchLink(int no);

private:

	// -----------------------------------
	Bool IsDuplicatedReferenceNo(int n);

	// -----------------------------------
	void NodeCopy(cSgfManager& mgr);

	// -----------------------------------
	Bool xSearchResult(cyiTreeNode* node);

	// -----------------------------------
	int xOptimizeNode(cyiTreeNode* node, int level, int* nProgress, Bool* canceled);

	// -----------------------------------
	cyiList* xSearchSameStoneMap(cyiTreeNode* node, int level);


	// -----------------------------------
	void CreateMapImage(cyiTreeNode* node, Bool bIsNoremainResult=False, Bool* bHasStoneMap=0);

	// -----------------------------------
	void CreateMapImage2(cyiTreeNode* node, cStoneMap& map);

	// -----------------------------------
	int CreateMapImageEx(cyiTreeNode* _node, int nOrder, map_t _map);


	//cYotsuyaNode* m_node;
	cyiTreeNode m_node;
	cyiTreeNode* m_currentNode;
	cyiTreeNode* m_pNodeTop;

	cyiList m_InitStone;
	long m_blocksize;
	void* m_sgfRaw;
	Bool m_bIsLoading;

	Bool m_bIsRepair;

	Bool m_bIschanged;

	Bool m_bsNgf;

	char m_szTitle[256];

	cyiStack m_stack;

	Bool m_bIsLoadSuccess;

	sGoHeader* m_header;

	char* m_pszRootComment;

	char m_szFileName[256];

	int m_Locale;

	int m_nTotalNodeCnt;

	// --------------------------------
	cyiList m_RefList;

	// --------------------------------
	cyiList m_LinkList;

	// --------------------------------
	int m_nLinkNo;

	//int m_nLineCnt;

	int m_nMaxRefNo;

#ifdef JOSEKI_DICTIONARY_MODE	
	cyiList m_ResultList;
#endif


	friend	class cyiGoEngine;
	friend	class cglsGoban;

};
