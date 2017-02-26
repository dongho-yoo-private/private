#include "yiGoEngine.h"


// ----------------------------------------
static void xGetStoneList19(int n, POINT* pos, Bool bIsBlack)
{
	if (n>9)
		n=9;

	if (bIsBlack==False)
	{
		if (n==3)
		{
			pos[0].x=3;
			pos[0].y=3;
			pos[1].x=15;
			pos[1].y=3;
			pos[2].x=3;
			pos[2].y=15;
			return ;
		}
	}


	if (n==2)
	{
		pos[0].x=15;
		pos[0].y=3;
		pos[1].x=3;
		pos[1].y=15;
		return ;
	}
	if (n==3)
	{
		pos[0].x=15;
		pos[0].y=3;
		pos[1].x=15;
		pos[1].y=15;
		pos[2].x=3;
		pos[2].y=15;
		return ;
	}

	if (n==4)
	{
		pos[0].x=3;
		pos[0].y=3;
		pos[1].x=15;
		pos[1].y=15;
		pos[2].x=3;
		pos[2].y=15;
		pos[3].x=15;
		pos[3].y=3;
		return ;
	}

	if (n==5)
	{
		pos[0].x=3;
		pos[0].y=3;
		pos[1].x=15;
		pos[1].y=15;
		pos[2].x=3;
		pos[2].y=15;
		pos[3].x=15;
		pos[3].y=3;
		pos[4].x=9;
		pos[4].y=9;
		return ;
	}

	if (n==6)
	{
		pos[0].x=3;
		pos[0].y=3;
		pos[1].x=15;
		pos[1].y=15;
		pos[2].x=3;
		pos[2].y=15;
		pos[3].x=15;
		pos[3].y=3;
		pos[4].x=3;
		pos[4].y=9;
		pos[5].x=15;
		pos[5].y=9;
		return ;
	}

	if (n==7)
	{
		pos[0].x=3;
		pos[0].y=3;
		pos[1].x=15;
		pos[1].y=15;
		pos[2].x=3;
		pos[2].y=15;
		pos[3].x=15;
		pos[3].y=3;
		pos[4].x=3;
		pos[4].y=9;
		pos[5].x=15;
		pos[5].y=9;
		pos[6].x=9;
		pos[6].y=9;
		return ;
	}

	if (n==8)
	{
		pos[0].x=3;
		pos[0].y=3;
		pos[1].x=15;
		pos[1].y=15;
		pos[2].x=3;
		pos[2].y=15;
		pos[3].x=15;
		pos[3].y=3;
		pos[4].x=3;
		pos[4].y=9;
		pos[5].x=15;
		pos[5].y=9;
		pos[6].x=9;
		pos[6].y=3;
		pos[7].x=9;
		pos[7].y=15;
		return ;
	}
	if (n==9)
	{
		pos[0].x=3;
		pos[0].y=3;
		pos[1].x=15;
		pos[1].y=15;
		pos[2].x=3;
		pos[2].y=15;
		pos[3].x=15;
		pos[3].y=3;
		pos[4].x=3;
		pos[4].y=9;
		pos[5].x=15;
		pos[5].y=9;
		pos[6].x=9;
		pos[6].y=3;
		pos[7].x=9;
		pos[7].y=15;
		pos[8].x=9;
		pos[8].y=9;
		return ;
	}
}


// ----------------------------------------
static void xGetStoneList13(int n, POINT* pos, Bool bIsTrue)
{
	if (n>9)
		n=9;

	if (n==2)
	{
		pos[0].x=3;
		pos[0].y=3;
		pos[1].x=9;
		pos[1].y=9;
		return ;
	}
	if (n==3)
	{
		pos[0].x=3;
		pos[0].y=3;
		pos[1].x=9;
		pos[1].y=9;
		pos[2].x=3;
		pos[2].y=9;
		return ;
	}

	if (n==4)
	{
		pos[0].x=3;
		pos[0].y=3;
		pos[1].x=9;
		pos[1].y=9;
		pos[2].x=3;
		pos[2].y=9;
		pos[3].x=9;
		pos[3].y=3;
		return ;
	}

	if (n==5)
	{
		pos[0].x=3;
		pos[0].y=3;
		pos[1].x=9;
		pos[1].y=9;
		pos[2].x=3;
		pos[2].y=9;
		pos[3].x=9;
		pos[3].y=3;
		pos[4].x=7;
		pos[4].y=7;
		return ;
	}

	if (n==6)
	{
		pos[0].x=3;
		pos[0].y=3;
		pos[1].x=9;
		pos[1].y=9;
		pos[2].x=3;
		pos[2].y=9;
		pos[3].x=9;
		pos[3].y=3;
		pos[4].x=3;
		pos[4].y=7;
		pos[5].x=9;
		pos[5].y=7;
		return ;
	}

	if (n==7)
	{
		pos[0].x=3;
		pos[0].y=3;
		pos[1].x=9;
		pos[1].y=9;
		pos[2].x=3;
		pos[2].y=9;
		pos[3].x=9;
		pos[3].y=3;
		pos[4].x=3;
		pos[4].y=7;
		pos[5].x=9;
		pos[5].y=7;
		pos[6].x=7;
		pos[6].y=7;
		return ;
	}

	if (n==8)
	{
		pos[0].x=3;
		pos[0].y=3;
		pos[1].x=9;
		pos[1].y=9;
		pos[2].x=3;
		pos[2].y=9;
		pos[3].x=9;
		pos[3].y=3;
		pos[4].x=3;
		pos[4].y=7;
		pos[5].x=9;
		pos[5].y=7;
		pos[6].x=7;
		pos[6].y=3;
		pos[7].x=7;
		pos[7].y=9;
		return ;
	}
	if (n==9)
	{
		pos[0].x=3;
		pos[0].y=3;
		pos[1].x=9;
		pos[1].y=9;
		pos[2].x=3;
		pos[2].y=9;
		pos[3].x=9;
		pos[3].y=3;
		pos[4].x=3;
		pos[4].y=7;
		pos[5].x=9;
		pos[5].y=7;
		pos[6].x=7;
		pos[6].y=3;
		pos[7].x=7;
		pos[7].y=9;
		pos[8].x=7;
		pos[8].y=7;
		return ;
	}

}

// ----------------------------------------
static void xGetStoneList9(int n, POINT* pos, Bool bIsTrue)
{
	if (n>9)
		n=9;

	if (n==2)
	{
		pos[0].x=2;
		pos[0].y=2;
		pos[1].x=6;
		pos[1].y=6;
		return ;
	}
	if (n==3)
	{
		pos[0].x=2;
		pos[0].y=2;
		pos[1].x=6;
		pos[1].y=6;
		pos[2].x=2;
		pos[2].y=6;
		return ;
	}

	if (n==4)
	{
		pos[0].x=2;
		pos[0].y=2;
		pos[1].x=6;
		pos[1].y=6;
		pos[2].x=2;
		pos[2].y=6;
		pos[3].x=6;
		pos[3].y=2;
		return ;
	}

	if (n==5)
	{
		pos[0].x=2;
		pos[0].y=2;
		pos[1].x=6;
		pos[1].y=6;
		pos[2].x=2;
		pos[2].y=6;
		pos[3].x=6;
		pos[3].y=2;
		pos[4].x=5;
		pos[4].y=5;
		return ;
	}
	if (n==6)
	{
		pos[0].x=2;
		pos[0].y=2;
		pos[1].x=6;
		pos[1].y=6;
		pos[2].x=2;
		pos[2].y=6;
		pos[3].x=6;
		pos[3].y=2;
		pos[4].x=2;
		pos[4].y=5;
		pos[5].x=6;
		pos[5].y=5;
		return ;
	}

	if (n==7)
	{
		pos[0].x=2;
		pos[0].y=2;
		pos[1].x=6;
		pos[1].y=6;
		pos[2].x=2;
		pos[2].y=6;
		pos[3].x=6;
		pos[3].y=2;
		pos[4].x=2;
		pos[4].y=5;
		pos[5].x=6;
		pos[5].y=5;
		pos[6].x=5;
		pos[6].y=5;
		return ;
	}

	if (n==8)
	{
		pos[0].x=2;
		pos[0].y=2;
		pos[1].x=6;
		pos[1].y=6;
		pos[2].x=2;
		pos[2].y=6;
		pos[3].x=6;
		pos[3].y=2;
		pos[4].x=2;
		pos[4].y=5;
		pos[5].x=6;
		pos[5].y=5;
		pos[6].x=5;
		pos[6].y=3;
		pos[7].x=5;
		pos[7].y=6;
		return ;
	}
	if (n==9)
	{
		pos[0].x=2;
		pos[0].y=2;
		pos[1].x=6;
		pos[1].y=6;
		pos[2].x=2;
		pos[2].y=6;
		pos[3].x=6;
		pos[3].y=3;
		pos[4].x=3;
		pos[4].y=5;
		pos[5].x=6;
		pos[5].y=5;
		pos[6].x=5;
		pos[6].y=3;
		pos[7].x=5;
		pos[7].y=6;
		pos[8].x=5;
		pos[8].y=5;
		return ;
	}
}

// --------------------------------
unsigned int cyiGoEngine::DefaultCondition(unsigned int& flag)
{
	unsigned int condition=0;

	flag=0;

	SET_LINE_COUNT(flag, 19);
	SET_KOMI(condition, 6);
	SET_HALF_SCORE(condition);

	return condition;
}

// --------------------------------
void cyiGoEngine::xDefaultDetail(sGobanContentsDetail* detail)
{
	detail->eCharSet=eCS_JAPANESE;
	detail->eFormat=eGFT_SGF;
	detail->eInputModeValue=eIM_EDIT;
	detail->eMouseModeValue=eMM_RBUTTON_RETURN;
	detail->eContentsMode=eGCM_NORMAL;
	detail->nLineCount=19;
	detail->eBranchViewModeValue=eBVM_NORMAL;
	detail->extra_info=0;
}

// --------------------------------
char** cyiGoEngine::CreateMap(cyiTreeNode* node)
{
	cStoneMap map;

	map.Init();
	cyiTreeNode* nodeTop=node;
	cyiStack stack;

	while(nodeTop->m_parent!=0){
		stack.push((yiAnyValue)nodeTop);
		nodeTop=nodeTop->GetParent();
	}
	
	yiAnyValue val;

	while(stack.pop(val)==True)
	{
		nodeTop=(cyiTreeNode*)val;
		cSgfStoneInfo* info = (cSgfStoneInfo* )nodeTop->GetData();

		if (info==0)
			continue;

		if (info->m_AddStoneList)
		{
			for (cyiNode* node=info->m_AddStoneList->GetNodeTopFrom(); node; node=node->np)
			{
				stone_t stone = (stone_t)node->data;
				aStoneList list;
				map.SetMap(stone, list, True);
			}
		}

		eStoneType type = STONE_TYPE(info->stone);
		if (type==eST_BLACK ||
				type==eST_WHITE)
		{
			aStoneList list;
			map.SetMap(info->stone, list);
		}
	}

	return (char**)map.GetMap(True);
}

// --------------------------------
cyiTreeNode* cyiGoEngine::GoLastNode(cyiTreeNode* node)
{
	cyiTreeNode* child = node;

RETRY:
	if (child->GetChildList().count()==0)
	{
		cSgfStoneInfo* info = (cSgfStoneInfo* )child->GetData();

		if (info==0)
		{
			return child;
		}

		if (info->ref==0)
		{
			return child;
		}
		child=info->ref;
		goto RETRY;
	}

	child = (cyiTreeNode*)child->GetChildList().GetTopFromRef(0);

	return GoLastNode(child);
}

// --------------------------------
cyiTreeNode* cyiGoEngine::GoTopNode(cyiTreeNode* node)
{
	cyiTreeNode* parent = node;
	while(parent->m_parent)
	{
		parent=parent->m_parent;
	}

	return parent;
}

// --------------------------------
ubit8_t* cyiGoEngine::CreateMapBitz(cyiTreeNode* node)
{
	cStoneMap map;

	map.Init();
	map.Clear();

	cyiStack stack;
	cyiTreeNode*parent=node;
	int n=0;

	while(parent)
	{
		stack.push((yiAnyValue)parent);
		parent=parent->m_parent;
	}

	n=stack.count();
	for (int i=0; i<n; i++)
	{
		cyiTreeNode* node;
		yiAnyValue val;
		stack.pop(val);
		node=(cyiTreeNode*)val;

		cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();

		if (info==0)
			continue;

		if (info->m_AddStoneList)
		{
			xNODELOOP(xx, info->m_AddStoneList)
			{
				stone_t stone=(stone_t)xx->ref;
				map.SetMap(stone, 0, True);
			}
		}

		if (info->stone==eST_INVALID)
		{
			continue;
		}

		map.SetMap(info->stone, 0);
	}

	return map.ExportBitz();
}

// --------------------------------
stone_t* cyiGoEngine::CreateBitzMap(stonemap_t map)
{
	stone_t* stonez = (stone_t*)yialloci(sizeof(stone_t)*361);

	int nByteIndex=0;
	int nBitShift=6;
	int index=0;
	for (int i=0; i<19; i++)
	{
		for (int j=0; j<19; j++)
		{
			stonez[j+index]=(map[nByteIndex]>>nBitShift)&0x03;
			nBitShift-=2;

			if (nBitShift<0)
			{
				nBitShift=6;
				nByteIndex++;
			}
		}
		index+=19;
	}

	return stonez;
}



cyiGoEngine::cyiGoEngine(void)
: m_nReferenceFromId(0),
  m_input_mode(eIM_EDIT),
  m_prev_input_mode(eIM_EDIT),
  m_mouse_mode(eMM_DEFAULT),
  m_id(0),
  m_pLimitNode(0),
  m_nRemainInitStoneCnt(0),
  m_remoteId(0),
  m_laststone(eST_INVALID),
  m_hCommentList(0),
  m_nBlackDeadStone(0),
  m_nWhiteDeadStone(0),
  m_flag(0),
  m_nShowNo(-1),
  m_parent(0),
  m_ugoid(0, 0),
  m_pUserData(0),
  m_bIsLocked(False)
{
	m_szFileName[0]=0;
	m_szTitle[0]=0;
	//m_ugoid = ugoid_t(0, 0);
	
}

cyiGoEngine::~cyiGoEngine(void)
{
	//if (m_pUserData)
	//	yifree(m_pUserData);::
}

// --------------------------------
cSgfManager* cyiGoEngine::GetSgfManager()
{
	return &m_sgfMgr;
}

// --------------------------------
void cyiGoEngine::InitMember(sGobanContentsDetail* detail)
{
	xSET_GO_FORMAT(m_flag, detail->eFormat);
	xSET_GOBAN_MODE(m_flag, detail->eContentsMode);
	xSET_GOBAN_STATUS(m_flag, eCS_READY);
	xSET_BRANCH_VIEW_MODE(m_flag, detail->eBranchViewModeValue);

	//xSET_BIT(m_flag, eGEF_HIDE_CURSOR, True);
	xSET_BIT(m_flag, eGEF_CALCULATE_MODE, False);
	xSET_BIT(m_flag, eGEF_NOW_SEARCH_RESULT, False);
	

	m_pExtraInfo=detail->extra_info;
	m_eLocale=detail->eCharSet;

	SetInputMode(detail->eInputModeValue);
	SetMouseMode(detail->eMouseModeValue);
}

// -----------------------------------
Bool cyiGoEngine::MakeTitle(char* title)
{
	// 優先順位
	// 1. Event名
	// 2. 対局者 VS 対局者
	// 3．ファイル名
	// 4. un
	sGoHeader* m_header = m_sgfMgr.m_header;
	if (m_header->pszEventName!=0 &&
			m_header->pszEventName[0]!=0)
	{
		strcpy(title, m_header->pszEventName);
		return True;
	}

	if (m_header->pszBlackName!=0 && m_header->pszBlackName[0]!=0
			&& m_header->pszWhiteName!=0 && m_header->pszWhiteName[0]!=0)
	{
		sprintf(title, "%s VS %s", m_header->pszBlackName, m_header->pszWhiteName);
		return True;
	}

	if (m_szFileName[0]!=0)
	{
		yiFileGetNameAndExt(m_szFileName, 0, title, 0);
		return True;
	}

	sprintf(title, "タイトルなし-%d", m_id);
	return False;
}


// --------------------------------
Bool cyiGoEngine::Create(sGoHeader* header/*=0*/, sGobanContentsDetail* detail/*=0*/)//eGoNodeType eType/*=eGNT_NORMAL*/, eCharacterSet eLocale/*=eCS_JAPANESE*/)
{
	int nInitStone=0;
	sGobanContentsDetail xdetail;

	if (detail==0)
	{
		detail=&xdetail;
		xDefaultDetail(detail);
	}

	InitMember(detail);

	if (header==0)
	{
		header=(sGoHeader*)yialloci(sizeof(sGoHeader));
		SET_LINE_COUNT(header->flag32, 19);
	}
	else
	{
		header = (sGoHeader*)yitoheap(header, sizeof(*header));
	}


	m_sgfMgr.Init(header);
	m_map.Init((eGoLineCount)GET_LINE_COUNT(m_sgfMgr.m_header->flag32));

	if ((nInitStone=GET_USER_INIT_STONE(m_sgfMgr.m_header->flag32))==0)
	{
		xSET_GOBAN_STATUS(m_flag, eCS_PLAY);
		return True;
	}

	m_szFileName[0]=0;

	char title[256];
	MakeTitle(title);
	m_sgfMgr.SetTitle(title);
	/*
	if (header->pszEventName)
	{
		m_sgfMgr.SetTitle(header->pszEventName);
	}
	else if (header->pszBlackName && header->pszWhiteName)
	{
		char buffer[256];
		sprintf(buffer, "%s VS %s", header->pszBlackName, header->pszWhiteName);
		m_sgfMgr.SetTitle(buffer);
	}
	else
	{
		m_sgfMgr.SetTitle("no named");
	}
*/
	m_nRemainInitStoneCnt=nInitStone;
	xSET_GOBAN_STATUS(m_flag, eCS_READY);

	InitStone();

	return True;
}

// --------------------------------
Bool cyiGoEngine::Create(sSimpleGoData* data, int size, const char* pszBlackName, const char* pszWhiteName, eInputMode input, unsigned int myId)
{
	sGobanContentsDetail xdetail;

	xDefaultDetail(&xdetail);
	xdetail.eInputModeValue=input;
	//if (detail==0)
	//{
	//	detail=&xdetail;
	//	xDefaultDetail(detail);
	//}

	InitMember(&xdetail);

	if (m_sgfMgr.ImportSimpleGoData(data, size, pszBlackName, pszWhiteName)==False)
	{
		return False;
	}
	//{
	//	return False;
	//}

	m_map.Init((eGoLineCount)GET_LINE_COUNT(m_sgfMgr.m_header->flag32));
	m_map.Clear();

	int nInitStone=0;
	if ((nInitStone=GET_INIT_STONE(m_sgfMgr.m_header->condition))!=0)
	{
		cyiList* list = m_sgfMgr.GetInitStoneList();

		for (cyiNode* node=list->GetNodeTopFrom(0); node; node=node->np)
		{
			stone_t stone = (stone_t)node->ref;
			m_map.SetMap(stone, 0, True);
		}
	}
	m_szFileName[0]=0;

	char title[128];
	MakeTitle(title);
	m_sgfMgr.SetTitle(title);

	m_ugoid.b_id = IAM_BLACK(m_sgfMgr.m_header->condition)?myId:data->hd.remoteId;
	m_ugoid.w_id = IAM_BLACK(m_sgfMgr.m_header->condition)?data->hd.remoteId:myId;
	m_ugoid.day	 = data->hd.day;
	m_ugoid.time = data->hd.time;

//		if (m_sgfMgr.GetTitle()==0)
	//{
	//	char name[256];
	//	yiFileGetNameAndExt(m_szFileName, 0, name, 0);
	//	
	//	m_sgfMgr.SetTitle(name);
	//}

	xSET_GOBAN_STATUS(m_flag, eCS_PLAY);
	return True;
}

// --------------------------------
Bool cyiGoEngine::Create(const void* pBlock, int size, sGobanContentsDetail* detail)
{
	sGobanContentsDetail xdetail;

	if (detail==0)
	{
		detail=&xdetail;
		xDefaultDetail(detail);
	}

	m_pExtraInfo=detail->extra_info;

	InitMember(detail);

	int nInitStone;
	Bool bIsRepair=(detail->eContentsMode==eGCM_JOSEKI_EDITOR || detail->eContentsMode==eGCM_JOSEKI_PLAYER)?True:False;

	if (bIsRepair)
	{
		bIsRepair = detail->eInputModeValue==eIM_EDIT?False:True;
	}

	if (m_sgfMgr.ImportBlock(pBlock, size, bIsRepair)==False)//.LoadSgfFile(pszFileName)==False)
	{
		return False;
	}

	m_map.Init((eGoLineCount)GET_LINE_COUNT(m_sgfMgr.m_header->flag32));
	m_map.Clear();

	if ((nInitStone=GET_INIT_STONE(m_sgfMgr.m_header->condition))!=0)
	{
		cyiList* list = m_sgfMgr.GetInitStoneList();

		for (cyiNode* node=list->GetNodeTopFrom(0); node; node=node->np)
		{
			stone_t stone = (stone_t)node->ref;
			m_map.SetMap(stone, 0, True);
		}
	}

	//strcpy(m_szFileName, pszFileName);

	char title[256];
	MakeTitle(title);
	m_sgfMgr.SetTitle(title);

	//Goto(MOVE_END, 0, 0);
//		if (m_sgfMgr.GetTitle()==0)
	//{
	//	char name[256];
	//	yiFileGetNameAndExt(m_szFileName, 0, name, 0);
	//	
	//	m_sgfMgr.SetTitle(name);
	//}

	xSET_GOBAN_STATUS(m_flag, eCS_PLAY);
	return True;
}


// --------------------------------
Bool cyiGoEngine::Create(const char* pszFileName, sGobanContentsDetail* detail)//eGoFileType format/*=eGF_SGF*/, eGoNodeType eType/*=eGNT_NORMAL*/, eCharacterSet eLocale/*=eCS_JAPANESE*/)
{
	sGobanContentsDetail xdetail;

	if (detail==0)
	{
		detail=&xdetail;
		xDefaultDetail(detail);
	}

	m_pExtraInfo = detail->extra_info;
	InitMember(detail);

	int nInitStone;

	if (xGET_GO_FORMAT(m_flag)==eGFT_SGF)
	{
		if (m_sgfMgr.LoadSgfFile(pszFileName, (detail->eContentsMode==eGCM_JOSEKI_EDITOR || detail->eContentsMode==eGCM_JOSEKI_PLAYER)?True:False)==False)
		{
			return False;
		}
	}
	else if (xGET_GO_FORMAT(m_flag)==eGFT_BIN)
	{
		if (m_sgfMgr.LoadBinaryGoFormat((LPCSTR)pszFileName, (void**)0, (int*)0)==False)
		{
			return False;
		}
	}
	else
	{
		if (m_sgfMgr.LoadYgfFile(pszFileName)==False)
		{
			return False;
		}
	}

	m_map.Init((eGoLineCount)GET_LINE_COUNT(m_sgfMgr.m_header->flag32));
	m_map.Clear();

	if ((nInitStone=GET_INIT_STONE(m_sgfMgr.m_header->condition))!=0)
	{
		cyiList* list = m_sgfMgr.GetInitStoneList();

		for (cyiNode* node=list->GetNodeTopFrom(0); node; node=node->np)
		{
			stone_t stone = (stone_t)node->ref;
			m_map.SetMap(stone, 0, True);
		}
	}

	strcpy(m_szFileName, pszFileName);

	char title[256];
	MakeTitle(title);
	m_sgfMgr.SetTitle(title);
//		if (m_sgfMgr.GetTitle()==0)
	//{
	//	char name[256];
	//	yiFileGetNameAndExt(m_szFileName, 0, name, 0);
	//	
	//	m_sgfMgr.SetTitle(name);
	//}

	xSET_GOBAN_STATUS(m_flag, eCS_PLAY);
	return True;
}


// --------------------------------
Bool cyiGoEngine::Destroy()
{
	m_sgfMgr.Destroy();
	return True;
}

// --------------------------------
stone_t** cyiGoEngine::SetDeadStone(stone_t stone, int* nBlackDeadStone, int* nWhiteDeadStone)
{
	return m_map.SetDeadStone(stone, nBlackDeadStone, nWhiteDeadStone);
}

// -----------------------------------
Bool cyiGoEngine::IsChanged()
{
	return m_sgfMgr.IsChanged();
}

// -----------------------------------
void cyiGoEngine::SetChangedFlag(Bool bIsChanged)
{
	m_sgfMgr.SetChangedFlag(bIsChanged);
}

// -----------------------------------
sGoHeader* cyiGoEngine::GetSgfHeader()
{
	return m_sgfMgr.m_header;
}

// --------------------------------
int cyiGoEngine::GetCurrentOrder()
{
	return m_history.count();
}

// --------------------------------
stone_t** cyiGoEngine::PutStone(stone_t stone, aStoneList branches, Bool bIsAdded/*=False*/, Bool bIsPlayMode/*=False*/)
{
	int n=0;
	int nCatchedStone=0;
	aStoneList list;
	branches[0]=STONE_EOF;

	//bIsPlayMode=(m_input_mode==eIM_PLAY);

	// 順番関係ない石の追加
	if (bIsAdded==True)
		goto ADD_STONE;

	// 地合い計算時
	if (xIS_BIT(m_flag, eGEF_CALCULATE_MODE)==True)
	{
		return (stone_t**)m_map.SetDeadStone(stone, 0, 0);
	}

	// これどういう意味？
	//if (bIsPlayMode==True)
	//{
	//	stone_t next_stone=m_sgfMgr.GetNextStone(stone);

	//	if (next_stone!=eST_INVALID)
	//	{
	//		stone=next_stone;
	//	}
	//}


	// まず、入力できるかどうかを見る。

	if ((nCatchedStone=m_map.SetMap(stone, list, bIsAdded))==-1)
	{
		if (m_input_mode==eIM_PLAY)
			return 0;
		// 入力できない場合、入力したのが最後の石であり、かつ、
		// 次のノードにパスノードがあれば、移動する。

		if (STONE_POS_X(stone)==STONE_POS_X(m_laststone) &&
			STONE_POS_Y(stone)==STONE_POS_Y(m_laststone))
		{
			if (m_sgfMgr.IsNextHasPass()==False)
			{
				return 0;
			}

			eStoneType type = STONE_TYPE(m_laststone);

			if (type==eST_BLACK ||
					type==eST_BPASS)
			{
				type=eST_WPASS;
			}
			else if (type==eST_WHITE||
						type==eST_BPASS)
			{
				type=eST_BPASS;
			}
			else
			{
				return 0;
			}
			stone = MAKESTONE(type, 0, 0, 0);
		}
		else 
			return 0;
	}

	n = m_sgfMgr.Add(stone, nCatchedStone, bIsAdded);

	m_history.push((yiAnyValue)m_sgfMgr.m_currentNode);
	SetAddedStoneToMap();
	m_laststone=stone;
	SumDeadStone(nCatchedStone);

	if (m_input_mode!=eIM_PLAY)
	{
		SetBranches(branches);
	}

	return m_map.GetMap();

ADD_STONE:

	// まず、入力できるかどうかを見る。
	if ((nCatchedStone=m_map.SetMap(stone, list, bIsAdded))==-1)
	{
		// 入れられないか、既にある石を選んだか。
		//cyiList* ret = m_sgfMgr.GetNodeHistory(0);
		Bool bIsRemove=False;

		//if (ret==0)
		//	return 0;

		// この場合は、入力できないところに入力しようとした場合。
		if (m_AddedStoneList.count()==0)
			return 0;

		// 追加された石を検索する。
		for (cyiNode* node=m_AddedStoneList.GetNodeTopFrom(0); node; node=node->np)
		{
			cyiList* list = (cyiList*)node->ref;
			for (cyiNode* n2 = list->GetNodeTopFrom(0); n2; n2=n2->np)
			{
				stone_t s = (stone_t)n2->data;
				if ((s&0xFFFF)==(stone&0xFFFF))
				{
					// この場合は、削除対象。
					bIsRemove=True;
					list->remove(n2);
					break;
				}

			}
			if (bIsRemove==True)
				break;
		}

		// この場合は、PutStone失敗で良し。
		if (bIsRemove==False)
		{
			return 0;
		}

		// 削除に値する場合マップも書き換える。
		return PlayByHistory();
	}

	{
		Bool bIsNew=False;
		if (m_sgfMgr.m_currentNode)
		{
			cSgfStoneInfo* info = (cSgfStoneInfo*)m_sgfMgr.m_currentNode->GetData();
			if (info==0)
			{
				bIsNew=True;
			}
			else if (info->m_AddStoneList==0 || info->m_AddStoneList->count()==0)
			{
				bIsNew=True;
			}
		}
		m_sgfMgr.Add(stone, -1, True, bIsNew);
		SetAddedStoneToMap();
	}
	return m_map.GetMap();
}

// ほぼ定石辞典用

// -----------------------------------
void cyiGoEngine::AddCommentCode(int nCommentCode)
{
	m_sgfMgr.AddCommentCode(nCommentCode);
}

// -----------------------------------
int cyiGoEngine::GetCommentCode()
{
	return m_sgfMgr.GetCommentCode();
}


// -----------------------------------
void cyiGoEngine::SetOptionCode(int Option)
{
	m_sgfMgr.SetOptionCode(Option);
}

// -----------------------------------
int cyiGoEngine::GetOptionCode()
{
	return m_sgfMgr.GetOptionCode();
}

// -----------------------------------
void cyiGoEngine::SetStatus(eGoResultStatus status)
{
	m_sgfMgr.SetStatus(status);
}

// -----------------------------------
eGoResultStatus cyiGoEngine::GetStatus()
{
	return m_sgfMgr.GetStatus();
}

// -----------------------------------
void cyiGoEngine::SetFlag(int flag)
{
	m_sgfMgr.SetFlag(flag);
}

// -----------------------------------
int cyiGoEngine::GetFlag()
{
	return m_sgfMgr.GetFlag();
}


// ----------------------------------
void cyiGoEngine::GetDeadStone(int& nBlack, int& nWhite)
{
	//m_sgfMgr.GetDeadStone(nBlack, nWhite);
	nBlack=m_nBlackDeadStone;
	nWhite=m_nWhiteDeadStone;
}

// --------------------------------
Bool cyiGoEngine::PutLabel(int i, int j, label_t label)
{
	SetChangedFlag(True);
	return m_sgfMgr.AddLabel(i, j, label);
}

// --------------------------------
Bool cyiGoEngine::RemoveLabel(int i, int j)
{
	SetChangedFlag(True);
	return m_sgfMgr.RemoveLabel(i, j);
}

// --------------------------------
Bool cyiGoEngine::PutComment(const char* pszComment, Bool bIsOverwrite)
{
	SetChangedFlag(True);
	return m_sgfMgr.AddComment(pszComment, bIsOverwrite);
}

// --------------------------------
Bool cyiGoEngine::GetNextStone()
{
	int type =  STONE_TYPE(m_laststone);

	if (type==eST_BLACK ||
		type==eST_BPASS)
	{
		return False;
	}
	else if (m_laststone==eST_INVALID)
	{
		if (GET_INIT_STONE(m_sgfMgr.m_header->condition)==0)
		{
			return True;
		}

		// 置き石を全て置いた時
		if (m_nRemainInitStoneCnt==0)
		{
			return False;
		}
		//if (m_state==eCS_PLAY)
		//{
		//	return False;
		//}
	}

	return True;
}

// -----------------------------------
int cyiGoEngine::GetHideBranches(cyiList& list)
{
	return m_sgfMgr.GetHideBranches(list);
}


// --------------------------------
void cyiGoEngine::SetBranches(aStoneList branches)
{
	int nBranchCnt  = m_sgfMgr.GetBranchCount(branches);
}

// --------------------------------
stone_t** cyiGoEngine::Play(int nIndex)
{
	int nAddedStoneCnt=0;
	m_laststone = m_sgfMgr.Play(nIndex);
	aStoneList list;
	int nCatchedStone = m_map.SetMap(m_laststone, list);
	SetAddedStoneToMap();
	m_history.push((yiAnyValue)m_sgfMgr.m_currentNode); // nIndex
	SumDeadStone(nCatchedStone);

		//int x=STONE_POS_X(m_laststone);
		//int y=STONE_POS_Y(m_laststone);
		//::yiTraceM2("c:\\yotsuya\\history.log","push:%d, cnt:%d (%d, %d) :%08x, child:%d", nIndex, m_history.count(), x, y, m_sgfMgr.m_currentNode, m_sgfMgr.m_currentNode->GetChildList().count());

	return m_map.GetMap();
}

// --------------------------------
// 現在の位置からの移動
// nIndex==MOVE_TOP トップ
// nIndex==MOVE_END 最後に移動
stone_t** cyiGoEngine::Goto(int nIndex, aStoneList branches, Bool bIgnoreBranch)
{
	stone_t** stoneRet;
	int nBranchCnt=0;
	aStoneList list;

	if (branches)
		branches[0]=STONE_EOF;

	if (nIndex==MOVE_TOP)
		goto GOTO_TOP;

	if (nIndex<0)
		goto BACK;

	// ツリーの先頭のみを再生する場合
	// 観戦・棋譜再生時
	if (bIgnoreBranch==True ||
		/*m_input_mode==eIM_ENJOY ||*/
			m_input_mode==eIM_WATCH)
	{
		for (int i=0; i<nIndex; i++)
		{
			int nAddedStoneCnt=0;
			m_laststone = m_sgfMgr.Play(0);

			if (m_laststone==STONE_EOF)
			{
				SetAddedStoneToMap();
				return m_map.GetMap();
			}

			int nCatchedStone = m_map.SetMap(m_laststone, list);
			SetAddedStoneToMap();
			m_history.push((yiAnyValue)m_sgfMgr.m_currentNode); //0
			SumDeadStone(nCatchedStone);
		}
		return m_map.GetMap();
	}

	// ツリーの選択を行う場合
	for (int i=0; i<nIndex; i++)
	{
		aStoneList branches;
		cyiList otherList;
		nBranchCnt  = m_sgfMgr.GetBranchCount(branches);
		
		// あらかじめやっておく
		SetAddedStoneToMap();

		if (nBranchCnt==1)
		{
			m_laststone = m_sgfMgr.Play(0);

			if (m_laststone==STONE_EOF)
			{
				//SetAddedStoneToMap();
				return m_map.GetMap();
			}

			int nCatchedStone = m_map.SetMap(m_laststone, list);
			m_history.push((yiAnyValue)m_sgfMgr.m_currentNode); // 0

			if (nCatchedStone!=-1 &&
					nCatchedStone!=0)
			{
				SumDeadStone(nCatchedStone);
			}

		}
		else 
		{
			//SetAddedStoneToMap();
			return m_map.GetMap();
		}
	}

	return m_map.GetMap();

BACK:

	if (m_pLimitNode==m_sgfMgr.GetCurrentNode())
		return 0;

	// すでに先頭なので失敗
	if (m_history.count()==0)
	{
		return 0;
	}

	// ここにはマイナス値しか入ってこないので。
	{
		cyiTreeNode* currentNode=m_sgfMgr.GetCurrentNode();
		for (int i=0; i<-nIndex; i++)
		{
			// 現在の位置から移動する履歴分を削除する。
			if (currentNode==m_pLimitNode)
			{
				break;
			}			
			yiAnyValue val;
			m_history.pop(val);
			currentNode=currentNode->GetParent();


		}
	}

	if (m_history.count()==0)
	{
		goto GOTO_TOP;
	}

	// 履歴再生
	stoneRet = PlayByHistory();

	// ノードが変わった時点でもう一度検索する。
	//SetBranches(branches);
	m_sgfMgr.GetBranchCount(branches);

	return stoneRet;

// トップへ移動
GOTO_TOP:
	{
		if (m_pLimitNode)
		{
			if (m_pLimitNode==GetCurrentNode())
			{
				return 0;
			}
			return PlayByNode(m_pLimitNode);
		}

		m_laststone=eST_INVALID;
		m_history.clear();
		m_sgfMgr.ToTop();
		m_map.Clear();
		
		SetInitStoneToMap();
		SetAddedStoneToMap();
	}

	return m_map.GetMap();
}

// --------------------------------
stone_t** cyiGoEngine::MoveEnd()
{
	// ツリーの選択を行う場合
	while(1)
	{
		aStoneList branches;
		cyiList otherList;
		int nBranchCnt  = m_sgfMgr.GetBranchCount(branches);
		
		// あらかじめやっておく
		SetAddedStoneToMap();

		m_laststone = m_sgfMgr.Play(0);

		if (m_laststone==STONE_EOF)
		{
			break;
		}

		int nCatchedStone = m_map.SetMap(m_laststone, 0);
		m_history.push((yiAnyValue)m_sgfMgr.m_currentNode); // 0

		if (nCatchedStone!=-1 &&
				nCatchedStone!=0)
		{
			SumDeadStone(nCatchedStone);
		}

	}

	return m_map.GetMap();
}


// --------------------------------
//cyiTreeNode* cyiGoEngine::GetTopNode()
//{
//	return m_sgfMgr.m_pNodeTop==0?&m_sgfMgr.m_node:m_sgfMgr.m_pNodeTop;
//}


// --------------------------------
stone_t** cyiGoEngine::PlayByNode(cyiTreeNode* node)
{
	cyiStack history;
	stone_t** ret=0;
	aStoneList branches;
	for (cyiTreeNode* n = node; n; n=n->GetParent())
	{
		history.push((yiAnyValue)n);
	}

	m_sgfMgr.ToTop();
	m_map.Clear();
	SetInitStoneToMap();
	m_AddedStoneList.clear();
	SetAddedStoneToMap();
	m_history.clear();
	m_nWhiteDeadStone=0;
	m_nBlackDeadStone=0;

	cyiTreeNode* x;

	while(history.pop((yiAnyValue&)x)==True)
	{
		cSgfStoneInfo* info = (cSgfStoneInfo*)x->GetData();
		
		if (info==0)
			continue;

		m_sgfMgr.m_currentNode=x;
		m_history.push((yiAnyValue)x);//m_sgfMgr.GetNodeIndex(x));
		SetAddedStoneToMap();

		if (info->stone!=eST_INVALID)
		{
			m_laststone=info->stone;
			aStoneList list;
			int nCatchedStone=m_map.SetMap(info->stone, list);
			if (nCatchedStone!=-1)
			{
				SumDeadStone(nCatchedStone);
			}
		}
	}
	
	SetBranches(branches);
	
	return m_map.GetMap();
}

// --------------------------------
stone_t** cyiGoEngine::GotoNextBranch(aStoneList list)
{
	list[0]=STONE_EOF;

	if (m_sgfMgr.SearchNextBranch()==False)
	{
		return 0;
	}


	while(1)
	{
		int cnt=0;
		stone_t** stone = Goto(1, list);

		// 保険
		if (m_sgfMgr.IsNodeEnd()==True)
		{
			return 0;
		}

		if ((cnt = m_sgfMgr.GetBranchCount(list))==0 ||
				cnt==1)
		{
			continue;
		}

		return m_map.GetMap();
	}

	return 0;
}

// --------------------------------
stone_t** cyiGoEngine::GotoPrevBranch(aStoneList list)
{

	if (m_sgfMgr.SearchPrevBranch()==False)
	{
		return 0;
	}

	list[0]=STONE_EOF;

	while(1)
	{
		int cnt=0;
		stone_t** stone = Goto(-1, list);

		if (stone==0)
		{
			return 0;
		}

		if (m_sgfMgr.IsNodeTop()==True)
		{
			return 0;
		}


		if ((cnt = m_sgfMgr.GetBranchCount(list))==0 ||
				cnt==1)
		{
			continue;
		}

		return m_map.GetMap();
	}
	return 0;
}

// --------------------------------
void cyiGoEngine::SumDeadStone(int n, stone_t stone/*=eST_INVALID*/)
{
	stone_t x = stone==eST_INVALID?m_laststone:stone;

	if (STONE_TYPE(x)==eST_BLACK)
	{
		m_nWhiteDeadStone+=n;
	}
	else if (STONE_TYPE(x)==eST_WHITE)
	{
		m_nBlackDeadStone+=n;
	}
}

// --------------------------------
void cyiGoEngine::SetInitStoneToMap()
{
	int nInitStoneCnt = m_sgfMgr.m_InitStone.count();

	// 置石があれば、設定する。
	if (nInitStoneCnt)
	{
		for (cyiNode* node = m_sgfMgr.m_InitStone.GetNodeTopFrom(0); node; node=node->np)
		{
			aStoneList list;
			stone_t stone = (stone_t)node->ref;
			m_map.SetMap(stone, list, True);				
		}
	}

	m_nBlackDeadStone=0;
	m_nWhiteDeadStone=0;
}

// --------------------------------
int cyiGoEngine::SetAddedStoneToMap()
{
	int nCatchedStone=0;
	cyiList* list = m_sgfMgr.GetAddedStone();
	if (list && list->count())
	{
		m_AddedStoneList.addref(list);
		
		for (cyiNode* node=list->GetNodeTopFrom(0); node!=0; node=node->np)
		{
			aStoneList buff;
			stone_t stone = (stone_t)node->data;
			
			// 失敗しても無視して良い。
			int n = m_map.SetMap(stone, buff, True);

			if (n!=-1)
			{
				nCatchedStone+=n;
			}
		}
	}

	return nCatchedStone;
}

// --------------------------------
stone_t** cyiGoEngine::PlayByHistory()
{
	cyiStack tempStack;
	while(1)
	{
/*		int index=0;
		if (m_history.pop((yiAnyValue&)index)==True)
		{
			tempStack.push((yiAnyValue)index);
			continue;
		}*/
		cyiTreeNode* node=0;
		if (m_history.pop((yiAnyValue&)node)==True)
		{
			tempStack.push((yiAnyValue)node);
			continue;
		}
		break;
	}

	m_map.Clear();
	m_AddedStoneList.clear();
	m_sgfMgr.ToTop();

	// 置き石がある場合置き石セット
	SetInitStoneToMap();

	while(1)
	{
		cyiTreeNode* node=0;
		SetAddedStoneToMap();
		if (tempStack.pop((yiAnyValue&)node)==False)
		{
			break;
		}
		//SetAddedStoneToMap();
		//int n = m_sgfMgr.GetNodeIndex(node);
		stone_t stone = m_sgfMgr.PlayByNode(node);//.Play(n);
		m_laststone=stone;

		int nCatchedStone = m_map.SetMap(stone, 0);
		SetAddedStoneToMap();
		m_history.push((yiAnyValue)m_sgfMgr.m_currentNode);

		//int x=STONE_POS_X(stone);
		//int y=STONE_POS_Y(stone);
		//::yiTraceM2("c:\\yotsuya\\history.log", "PlayByHistory, push:%d, cnt:%d (%d, %d) :%08x, child:%d", n, m_history.count(), x, y,  m_sgfMgr.m_currentNode, m_sgfMgr.m_currentNode->GetChildList().count());


		SumDeadStone(nCatchedStone);

		// もし、追加された石があれば、マップに書き換えたり、
		// リストに保存する。

	}

	return (stone_t**)m_map.GetMap();
}

// --------------------------------
stone_t** cyiGoEngine::StartCalculate()
{
	xON_BIT(m_flag, eGEF_CALCULATE_MODE);
	SetInputMode(eIM_CALCULATE);
	return m_map.StartCalculate();
}

// --------------------------------
stone_t** cyiGoEngine::EndCalculate(sResult* res/*=0*/)
{
	int nDeadBlackStone, nDeadWhiteStone, nBlackArea, nWhiteArea;
	stone_t** stone = m_map.EndCalculate(&nBlackArea, &nWhiteArea, &nDeadBlackStone, &nDeadWhiteStone);

	SetInputMode(eIM_CALCULATE_END);

	if (res)
	{
		//int b=0, w=0;
		//m_sgfMgr.GetDeadStone(b, w);

		res->nBlackDeadStone = nDeadBlackStone + m_nBlackDeadStone; // 終局後のあげ石+対局時のあげ石
		res->nWhiteDeadStone = nDeadWhiteStone + m_nWhiteDeadStone; // 終局後のあげ石+対局時のあげ石
		res->nBlackArea = nBlackArea;
		res->nWhiteArea = nWhiteArea;
		res->nKomi = MAKELONG(HALF_SCORE(m_sgfMgr.m_header->condition) , GET_KOMI(m_sgfMgr.m_header->condition)) ;
	}
	return stone;
}


// --------------------------------
Bool cyiGoEngine::Load(const char* pszFileName)
{
	return m_sgfMgr.LoadSgfFile(pszFileName);
}

// --------------------------------
Bool cyiGoEngine::Save(const char* pszFileName, eGoFileType type, void* p, int n)
{
	if (type==eGFT_SGF)
		return m_sgfMgr.SaveSgfFile(pszFileName, (xFLAG_JOSEKI_CLR&m_flag)!=0);
	else if (type==eGFT_BIN)
		return m_sgfMgr.SaveBinaryGoFormat(pszFileName, p, n);

#if 0
	return m_sgfMgr.SaveYgfFile(pszFileName);
#endif
	return False;
}

// --------------------------------
sSimpleGoData* cyiGoEngine::Export(int* size)
{
	sSimpleGoData* data = m_sgfMgr.ExportSimpleGoData(size);

	data->hd.day=m_ugoid.day;
	data->hd.time=m_ugoid.time;

	data->hd.remoteId = IAM_BLACK(m_sgfMgr.m_header->condition)?m_ugoid.w_id:m_ugoid.b_id;

	return data;
}


// --------------------------------
Bool cyiGoEngine::DeleteAt(int index/*=-1*/)
{
	return m_sgfMgr.Delete();
}

// --------------------------------
stone_t cyiGoEngine::GetStoneEx(int i, int j)
{
	return m_map.GetStoneInfo(i,j);
}

// --------------------------------
int cyiGoEngine::GetNextStonez(stone_t* buffer)
{
	int cnt =0;
	cyiTreeNode* cur = m_sgfMgr.m_currentNode;

	if (cur->GetChildList().count()==0)
		return 0;

	for (cyiNode* node = cur->GetChildList().GetNodeTopFrom(); node; node=node->np)
	{
		cyiTreeNode* t = (cyiTreeNode*)node->ref;
		cSgfStoneInfo* info = (cSgfStoneInfo*)t->GetData();

		if (info!=0 && info->stone!=eST_INVALID &&
				(info->flag32&1)!=1)
		{
			buffer[cnt++]=info->stone;
		}
	}

	return cnt;
}


// --------------------------------
stone_t cyiGoEngine::GetStone(int i, int j)
{
	return (m_map.GetMap())[i][j];
}

// --------------------------------
stone_t** cyiGoEngine::GetStoneMap()
{
	return m_map.GetMap();
}

// --------------------------------
stone_t** cyiGoEngine::GetStoneOrderMap()
{
	return m_map.GetMapExtra();
}

int cyiGoEngine::GetBranchList(aStoneList list)
{
	return m_sgfMgr.GetBranchCount(list);
}

// --------------------------------
void cyiGoEngine::UpdateHeader(sGoHeader* header)
{

}

// --------------------------------
sGoHeader* cyiGoEngine::GetHeader()
{
	return m_sgfMgr.m_header;
}

// --------------------------------
eCurrentState cyiGoEngine::GetCurrentState()
{
	return xGET_GOBAN_STATUS(m_flag);//m_state;
}


// --------------------------------
void cyiGoEngine::InitStone(int i, int j)
{
	int nInitStoneCnt=GET_INIT_STONE(m_sgfMgr.m_header->condition);
	POINT pt[9];

	if (nInitStoneCnt==0)
	{
		xSET_GOBAN_STATUS(m_flag, eCS_PLAY);
		return ;
	}

	if (i!=-1 && j!=-1)
	{
		if (m_map.SetMap(i, j, eST_BLACK, 0, True)==-1)
		{
			//return ;
		}

		if (m_sgfMgr.InitStone(MAKESTONE(eST_BLACK, 0, i, j))==True)
		{
			m_nRemainInitStoneCnt--;
		}
		else
		{
			aStoneList list;
			m_nRemainInitStoneCnt++;
			Goto(MOVE_TOP, list);
		}

		if (m_nRemainInitStoneCnt==0)
		{
			xSET_GOBAN_STATUS(m_flag, eCS_PLAY);

			SetInputMode(eIM_PREV);
			m_laststone=eST_INVALID;
			return ;
		}

		return;
	}

	int nLineCnt = GET_LINE_COUNT(m_sgfMgr.m_header->flag32);


	Bool bIsBlack=True;

	if (m_input_mode==eIM_PLAY ||
			m_input_mode==eIM_WATCH)
	{
		if (IAM_BLACK(m_sgfMgr.m_header->condition)==False)
		{
			bIsBlack=False;
		}
	}

	if (nLineCnt==19)
	{
		xGetStoneList19(nInitStoneCnt, pt, bIsBlack);
	}
	else if (nLineCnt==9)
	{
		xGetStoneList9(nInitStoneCnt, pt, bIsBlack);	
	}
	else if (nLineCnt==13)
	{
		xGetStoneList13(nInitStoneCnt, pt, bIsBlack);	
	}
	else
	{
		m_nRemainInitStoneCnt=nInitStoneCnt;
		return ;
	}

	if (nInitStoneCnt>9)
	{
		m_nRemainInitStoneCnt = nInitStoneCnt-9;
	}

	for (int i=0; i<nInitStoneCnt; i++)
	{
		m_sgfMgr.InitStone(MAKESTONE(eST_BLACK, 0, pt[i].x, pt[i].y));
		m_map.SetMap(pt[i].x, pt[j].y, eST_BLACK, 0, True);
	}

	xSET_GOBAN_STATUS(m_flag, eCS_PLAY);

	return ;
}

// --------------------------------
void cyiGoEngine::StartInitStone(int count)
{
	m_nRemainInitStoneCnt=count;
	SetInputMode(eIM_INITSTONE_MODE);
	SET_INIT_STONE(m_sgfMgr.m_header->condition, count);
}

#define xSTRING_VALID(str) (str!=0&&str[0]!=0)
// --------------------------------
const char* cyiGoEngine::GetTitle()
{
	if (m_szTitle[0])
		return m_szTitle;

	if (xSTRING_VALID(m_sgfMgr.m_header->pszEventName))
	{
		strcpy(m_szTitle, m_sgfMgr.m_header->pszEventName);
		return m_szTitle;
	}
	if (xSTRING_VALID(m_sgfMgr.m_header->pszBlackName) && 
			xSTRING_VALID(m_sgfMgr.m_header->pszWhiteName))
	{
		sprintf(m_szTitle, "%s VS %s", m_sgfMgr.m_header->pszBlackName, m_sgfMgr.m_header->pszWhiteName);
		return m_szTitle;
	}
	if (m_szFileName[0])
	{
		yiFileGetNameAndExt(m_szFileName, 0, m_szTitle,0);
		return m_szTitle;
	}

	//return 0;
#ifdef _WIN32
	sprintf(m_szTitle, "名前なし-%d", GetGoId());
#else
	sprintf(m_szTitle, "new file - %d", GetGoId());
#endif

	return m_szTitle;
}

// --------------------------------
void cyiGoEngine::SetTitle(const char* pszTitle)
{
	if (pszTitle==0)
	{
		m_szTitle[0]=0;
	}
	else
	{
		strcpy(m_szTitle, pszTitle);
	}
}


// --------------------------------
char* cyiGoEngine::GetComment()
{
	return m_sgfMgr.GetComment();
}

// --------------------------------
goid_t cyiGoEngine::GetGoId()
{
	return m_id;
}

// --------------------------------
label_t xStoneValueToLabel(int x,int y, eStoneValue val)
{
	int code=0;
	switch(val)
	{
		case eSV_NORMAL:
		case eSV_SIMPLE:
			code=eLS_GOOD;
			break;
		case eSV_HONTE:
		case eSV_GOOD:
		case eSV_KATACHI:
			code=eLS_GOOD2;
			break;
		case eSV_CASE:
			code=eLS_CASE;
			break;
		case eSV_BAD:
		case eSV_QUESTION:
		case eSV_BADSUJI:
		case eSV_NOSICHO:
			code=eLS_BAD;
			break;
		case eSV_NOGOOD:
		case eSV_NOKOU:
			code=eLS_NOGOOD;
			break;
		case eSV_SICHO:
		case eSV_TESUZI:
		case eSV_MYOSYU:
			code=eLS_INFO;
			break;
		default :
			code=eLS_CAREFULL;
			break;
		//case eSV_TRICK:
		//case eSV_DEFICULT:
		//case eSV_SENTE:
		//case eSV_TESUZI:
		//case eSV_KYOSYU:
		//case eSV_CASE:
	}

	return ((x<<24)|(y<<16)|((0x0000FFFF)&code));
}

// --------------------------------
void cyiGoEngine::DeleteLabel(cyiList*list)
{
	delete list;
}

// --------------------------------
cyiList* cyiGoEngine::GetLabel(Bool* bIsReqFree)
{
	if (bIsReqFree)
		*bIsReqFree=False;

	Bool bIsJoseki = (xGET_GOBAN_MODE(m_flag)==eGCM_JOSEKI_EDITOR) || (xGET_GOBAN_MODE(m_flag)==eGCM_JOSEKI_PLAYER);


	if (bIsJoseki==False)
	{
		if (m_parent==0)
		{
			return m_sgfMgr.GetLabel();
		}
	}

	cyiTreeNode* t=GetCurrentNode();

	if (t==0)
		return 0;

	cSgfStoneInfo* info = (cSgfStoneInfo*)t->GetData();

	if (info==0)
	{
		return 0;
	}

	cyiList* list = new cyiList();


	for (cyiNode* node=t->GetChildList().GetNodeTopFrom(); node; node=node->np)
	{
		cyiTreeNode* child=(cyiTreeNode*)node->data;

		cSgfStoneInfo* childInfo=(cSgfStoneInfo*)child->GetData();

		if (childInfo==0)
			continue;
		if (childInfo->exinfo==0)
			continue;
		
		int x, y;
		eStoneType type = STONE_TYPE(childInfo->stone);

		if (type!=eST_BLACK &&
				type!=eST_WHITE)
				continue;
		x=STONE_POS_X(childInfo->stone);
		y=STONE_POS_Y(childInfo->stone);

		if (childInfo->exinfo->stonevalue)
		{
			label_t label=xStoneValueToLabel(x, y, (eStoneValue)childInfo->exinfo->stonevalue);
			list->addref((void*)label);
		}
	}

	if (list->count()==0)
	{
		delete list;
		list=0;
		if (bIsReqFree)
			*bIsReqFree=False;
	}

	if (m_sgfMgr.GetLabel()==0)
	{
		if (list)
			*bIsReqFree=True;
		return list;
	}

	if (list)
	{
		int intCnt=list->count();
		for (cyiNode* node=m_sgfMgr.GetLabel()->GetNodeTopFrom(); node; node=node->np)
		{
			label_t label = (label_t)node->ref;
			Bool bNoAdd=False;
			int i=0;
			for (cyiNode* xx=list->GetNodeTopFrom(); i<intCnt; xx=xx->np)
			{
				label_t xlabel = (label_t)xx->ref;

				if ((0xFFFF0000&label)==(0xFFFF0000&xlabel))
				{
					bNoAdd=True;
					break;
				}
				i++;
			}

			if (bNoAdd==False)
			{
				list->addref((void*)label);
			}

		}
	}
	else
	{
		if (bIsReqFree)
			*bIsReqFree=False;
		return m_sgfMgr.GetLabel();
	}

	if (list)
	{
		if (bIsReqFree)
		{
			*bIsReqFree=True;
		}
	}


	return list;
}

// --------------------------------
stone_t cyiGoEngine::GetLastStone()
{

	return m_laststone;
}

// --------------------------------
int cyiGoEngine::GetInitStoneCount()
{
	return m_sgfMgr.m_InitStone.count();
}

// --------------------------------
Bool cyiGoEngine::IsCurrentOrderMine()
{
	Bool bIsBlack=IAM_BLACK(m_sgfMgr.m_header->condition);

	if (bIsBlack==IsCurrentOrderBlack())
	{
		return True;
	}

	return False;
}

// --------------------------------
Bool cyiGoEngine::IsCurrentOrderBlack()
{
	// この場合が厄介
	if (m_laststone==eST_INVALID)
	{
		int n = GetInitStoneCount();

		// 黒番
		if (n<2)
			return True;
		else
			return False;
	}

	int type = STONE_TYPE(m_laststone);
	
	if (type==eST_BLACK||
		type==eST_BPASS)
	{
		return False;
	}

	return True;
}

// --------------------------------
unsigned int cyiGoEngine::GetLineCount()
{
	return GET_LINE_COUNT(m_sgfMgr.m_header->flag32);
}

// --------------------------------
Bool cyiGoEngine::IsValidHit(stone_t stone)
{
	int i=STONE_POS_X(stone);
	int j=STONE_POS_Y(stone);

	if (stone==eST_INVALID)
	{
		return False;
	}


	// 打てない条件
	// 1. 碁石がすでにある場合。
	// 2. コウの取り番でない場合。

	stone_t** stonez = (stone_t**)m_map.GetMap();


	if (stonez[i][j]==eST_INVALID)
	{
		return False;
	}

	stone_t kou;
	if ((kou=m_map.GetKou())!=STONE_EOF)
	{
		if (i==STONE_POS_X(kou) &&
				j==STONE_POS_Y(kou))
		{
			return False;
		}
	}

	if (stonez[i][j]!=eST_NONE)
	{
		return False;
	}

	return True;
}

// --------------------------------
eInputMode cyiGoEngine::GetInputMode()
{
	return m_input_mode;
}

// --------------------------------
void cyiGoEngine::SetInputMode(eInputMode mode)
{
	if (mode==eIM_PREV)
	{
		eInputMode temp=m_input_mode;
		m_input_mode=m_prev_input_mode;
		m_prev_input_mode=temp;

		return ;
	}

	if (m_input_mode!=mode)
	{
		m_prev_input_mode = m_input_mode;		
	}
	m_input_mode=mode;


	//xSET_BIT(m_flag, eGEF_HIDE_CURSOR, ((mode==eIM_ENJOY)||(mode==eIM_WATCH)));
	xSET_BIT(m_flag, eGEF_HIDE_CURSOR, (mode==eIM_WATCH));
}

// --------------------------------
eMouseMode cyiGoEngine::GetMouseMode()
{
	return m_mouse_mode;
}

// --------------------------------
void cyiGoEngine::SetMouseMode(eMouseMode mode)
{
	m_mouse_mode=mode;
}

// --------------------------------
void cyiGoEngine::SetRemoteId(int n)
{
	m_remoteId=n;
}

// --------------------------------
int cyiGoEngine::GetRemoteId()
{
	return m_remoteId;
}

// --------------------------------
void cyiGoEngine::SetLocale(eCharacterSet locale/*=eCS_JAPANESE*/)
{
	m_eLocale=locale;
}

// --------------------------------
eCharacterSet cyiGoEngine::GetLocale()
{
	return m_eLocale;
}
// --------------------------------
Bool cyiGoEngine::IsYgfFormat()
{
	return (xGET_GO_FORMAT(m_flag)!=eGFT_SGF);
}

// --------------------------------
Bool cyiGoEngine::AddNewBranch()
{
	return True;
}

// --------------------------------
Bool cyiGoEngine::DeleteNode(cyiTreeNode* _node)
{
	cyiTreeNode* node = _node==0?m_sgfMgr.GetCurrentNode():_node;
	cyiTreeNode* parent = node->GetParent();

	m_sgfMgr.DeleteNode(node);

	PlayByNode(parent);
	return True;
}

// --------------------------------
void cyiGoEngine::SetShowNo(int n)
{
	m_nShowNo=n;
}

// -----------------------------------
cyiTreeNode* cyiGoEngine::GetCurrentNode()
{
	return m_sgfMgr.GetCurrentNode();
}

// -----------------------------------
cyiTreeNode* cyiGoEngine::GetNode(stone_t* stone)
{
	cyiTreeNode* pNode = &m_sgfMgr.m_node;

	int cnt=0;
	while(stone[cnt]!=STONE_EOF)
	{
		for (cyiNode* node = pNode->GetChildList().GetNodeTopFrom(); node; node=node->np)
		{
			cyiTreeNode* t = (cyiTreeNode*)node->ref;
			cSgfStoneInfo* info = (cSgfStoneInfo*)t->GetData();

			if ((info->stone&0xFF00FFFF)==(stone[cnt]&0xFF00FFFF))
			{
				pNode=t;
				cnt++;
				goto NEXT;
			}
		}

		return 0;
NEXT:
		continue;
	}

	return pNode;
}


// --------------------------------
int cyiGoEngine::GetShowNo()
{
	return m_nShowNo;
}

typedef struct xsCommentList {
	cyiList* list;
	cyiNode* curr;
} sCommentList;

// --------------------------------
HandleCommentList cyiGoEngine::FindCommentList()
{
	cyiList* list = m_sgfMgr.GetCommentNodeList();

	if (list==0)
		return 0;

	sCommentList* s = (sCommentList*)yialloc(sizeof(sCommentList));

	s->list=list;
	s->curr=0;

	SetInputMode(eIM_FIND_COMMENT);

	m_hCommentList= (HandleCommentList)s;

	return m_hCommentList;
}

// --------------------------------
stone_t** cyiGoEngine::GotoNextComment(int index/*=-1*/)
{
	sCommentList* s = (sCommentList*) m_hCommentList;

	if (s==0)
		return 0;

	if (s->curr ==0)
	{
		s->curr = s->list->GetNodeTopFrom(0);
	}
	else
	{
		s->curr = s->curr->np;
	}

	if (index!=-1)
		s->curr = s->list->GetNodeTopFrom(index);

	// EOF
	if (s->curr==0)
		return 0;

	cyiList* ret = m_sgfMgr.GetNodeHistory((cyiTreeNode*)s->curr->ref);

	if (ret==0)
		return 0;

	aStoneList branches;
	Goto(MOVE_TOP, branches);

	for (cyiNode* xnode=ret->GetNodeTopFrom(0); xnode!=0; xnode=xnode->np)
	{
		aStoneList list;
		int nIndex = (int)xnode->data;
		stone_t stone = m_sgfMgr.Play(nIndex);
		int nCatchedStone = m_map.SetMap(stone, list);
		m_laststone=stone;
		SumDeadStone(nCatchedStone);


	}

	return m_map.GetMap();
}

// --------------------------------
cyiTreeNode* cyiGoEngine::CreateReference(const char* refName,int& index)
{
	return m_sgfMgr.CreateReference(refName, index);
}

// --------------------------------
cyiList* cyiGoEngine::GetReference()
{
	if (m_sgfMgr.m_currentNode)
	{
		cSgfStoneInfo* info = (cSgfStoneInfo*)m_sgfMgr.m_currentNode->GetData();

		if (info)
		{
			return (info->reference);

		}
	}

	return 0;
}


// --------------------------------
Bool cyiGoEngine::AddNode(cyiTreeNode* node)
{
	return m_sgfMgr.AddNode(node);
}

// --------------------------------
cyiTreeNode* cyiGoEngine::GetTopNode()
{
	return m_sgfMgr.m_pNodeTop;
	//return m_sgfMgr.AddNode(node);
}


// -----------------------------------
Bool cyiGoEngine::SearchResult()
{
	//xSET_BIT(m_flag, eGEF_NOW_SEARCH_RESULT, True);
	Bool res= m_sgfMgr.SearchResult();
	//xSET_BIT(m_flag, eGEF_NOW_SEARCH_RESULT, False);
	YITRACE(6, "cyiGoEngine::SearchResult() quit!");

	return True;
}

// -----------------------------------
Bool cyiGoEngine::CreateMapImage(cyiTreeNode* node)
{
	m_sgfMgr.CreateMapImage(node);
	return True;
}

// -----------------------------------
int cyiGoEngine::OptimizeNode(int* nProgress, Bool* canceled)
{
	return m_sgfMgr.OptimizeNode(nProgress, canceled);
}

// -----------------------------------
int cyiGoEngine::GetFullNodeCnt()
{
	return m_sgfMgr.GetFullNodeCnt();
}

// --------------------------------
int cyiGoEngine::GetBlockId(int i, int j)
{
	return m_map.GetBlockId(i, j);
}

// --------------------------------
void cyiGoEngine::SetExtraData(void* p)
{
	m_pUserData=p;
}

// --------------------------------
void* cyiGoEngine::GetExtraData()
{
	return m_pUserData;
}

// --------------------------------
Bool xConvertGobanDirection(cyiTreeNode* node, int n)
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();

	if (info)
	{
		int x = STONE_POS_X(info->stone);
		int y = STONE_POS_Y(info->stone);
		eStoneType type = STONE_TYPE(info->stone);
		int order = STONE_ORDER(info->stone);

		x=n-x-1;
		y=n-y-1;

		info->stone=MAKESTONE(type, order, x, y);

		// 置き石
		if (info->m_AddStoneList)
		{
			for (cyiNode* p=info->m_AddStoneList->GetNodeTopFrom(); p; p=p->np)
			{
				stone_t stone = (stone_t)p->ref;
				int x = STONE_POS_X(stone);
				int y = STONE_POS_Y(stone);
				eStoneType type = STONE_TYPE(stone);
				int order = STONE_ORDER(stone);

				x=n-x-1;
				y=n-y-1;
				p->ref = (void*)MAKESTONE(type, order, x, y);
			}
		}
	}

	if (node->GetChildList().count()==0)
	{
		return True;
	}

	for (cyiNode* x = node->GetChildList().GetNodeTopFrom(); x; x=x->np)
	{
		cyiTreeNode* child = (cyiTreeNode*)x->ref;
		xConvertGobanDirection(child, n);
	}

	return True;
}

// --------------------------------
Bool cyiGoEngine::ConvertGobanDirection()
{
	cyiTreeNode* node = m_sgfMgr.m_pNodeTop;
	int nLineCount = GET_LINE_COUNT(m_sgfMgr.m_header->flag32);

	// 置き石があれば？
	if (m_sgfMgr.m_InitStone.count()!=0)
	{
		for (cyiNode* node = m_sgfMgr.m_InitStone.GetNodeTopFrom(0); node; node=node->np)
		{
			stone_t stone = (stone_t)node->ref;
			int x = STONE_POS_X(stone);
			int y = STONE_POS_Y(stone);
			eStoneType type = STONE_TYPE(stone);
			int order = STONE_ORDER(stone);

			x=nLineCount-x-1;
			y=nLineCount-y-1;
			node->ref = (void*)MAKESTONE(type, order, x, y);
		}
	}

	if (node)
	{
		Bool res = xConvertGobanDirection(node, nLineCount);

		//if (res==True)
		//{
		//	PlayByNode(m_sgfMgr.GetCurrentNode());
		//	return True;
		//}
	}

	return False;
}


// --------------------------------
stone_t** cyiGoEngine::GotoPrevComment()
{
	sCommentList* s = (sCommentList*) m_hCommentList;

	if (s==0)
		return 0;

	// TOP
	if (s->curr ==0)
	{
		return 0;
	}

	s->curr = s->curr->pp;
	
	// TOP
	if (s->curr==0)
	{
		return 0;
	}

	cyiList* ret = m_sgfMgr.GetNodeHistory((cyiTreeNode*)s->curr->ref);

	if (ret==0)
		return 0;

	aStoneList branches;
	Goto(MOVE_TOP, branches);

	for (cyiNode* xnode=ret->GetNodeTopFrom(0); xnode!=0; xnode=xnode->np)
	{
		aStoneList list;
		int nIndex = (int)xnode->data;
		stone_t stone = m_sgfMgr.Play(nIndex);
		int nCatchedStone=m_map.SetMap(stone, list);
		m_laststone=stone;
		SumDeadStone(nCatchedStone);
	}

	return m_map.GetMap();
}

// --------------------------------
void cyiGoEngine::CloseCommentList()
{
	sCommentList* s = (sCommentList*)m_hCommentList;

	if (s->list!=0)
	{
		m_sgfMgr.ReleaseCommentNodeList(s->list);
	}

	yifree(s);
	SetInputMode(eIM_PREV);
	m_hCommentList=0;
}

// -----------------------------------
sStonePath* cyiGoEngine::CreatePath()
{
	return m_sgfMgr.CreatePath();
}
