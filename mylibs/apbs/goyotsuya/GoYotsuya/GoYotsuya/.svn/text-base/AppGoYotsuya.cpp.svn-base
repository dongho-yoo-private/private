#include "StdAfx.h"
#include "AppGoYotsuya.h"
#include "yistd.h"
#include "yctrlGoban.h"


// -------------------------------
cAppGoYotsuya* cAppGoYotsuya::m_me=0;


cAppGoYotsuya::cAppGoYotsuya(void)
{
}

cAppGoYotsuya::~cAppGoYotsuya(void)
{
}

// ---------------------------------
cAppGoYotsuya& cAppGoYotsuya::GetInstance()
{
	if (m_me==0)
		m_me=new cAppGoYotsuya();
	return *m_me;
}

// ---------------------------------
// Global Mutexで多重起動を防ぐなど。
Bool cAppGoYotsuya::Initialize()
{
	// TODO. mutex.

	// mutex.

	// 多重起動時には、ファイル名などを送ること。

	m_goban = new cyctrlGoban();

	return True;
}

// ---------------------------------
void cAppGoYotsuya::OnCreate(HWND hWnd)
{
	RECT rect;

	GetClientRect(hWnd, &rect);
	int nWidth=rect.right > rect.bottom ? rect.bottom:rect.right;

	m_hWnd = hWnd;

	// 碁盤生成
	m_goban->Create(0, 0, rect.right, rect.bottom, 19, hWnd);

	m_prev = new cyctrlPreviewGoban();
	m_prev->Create(m_hWnd, 128, 5);
	m_prev->Update(m_goban->GetCurrentNode());



	// コントロール生成

	// ビューアー生成

	// 各コントロール配置

	return;
}

// ---------------------------------
void cAppGoYotsuya::OnDestroy()
{
}	

// ---------------------------------
void cAppGoYotsuya::OnPutStoned(WPARAM wp,LPARAM lp)
{
	m_prev->Update((cyiTreeNode*)lp);
}

// ---------------------------------
void cAppGoYotsuya::OnPreViewSelected(WPARAM wp,LPARAM lp)
{
	m_goban->PlayByNode((cyiTreeNode*)lp);

	//if (wp!=-1)
	//{
	//	m_prev->Update(m_goban->GetCurrentNode());
	//}
}

// ---------------------------------
void cAppGoYotsuya::Command(int id, int param1, int param2)
{
	switch(id)
	{
		case eAGY_CMD_PLAY:
		{
			m_goban->Play();
			m_prev->Update(m_goban->GetCurrentNode());
			break;
		}
		case eAGY_CMD_BACK:
		{
			aStoneList branches;
			m_goban->Goto(m_goban->GetCurrentGoId(), -1, branches);
			m_prev->Update(m_goban->GetCurrentNode());
			break;
		}
		case eAGY_CMD_TOP:
		{
			aStoneList branches;
			m_goban->Goto(m_goban->GetCurrentGoId(), INDEX_TOP, branches);
			m_prev->Update(m_goban->GetCurrentNode());
			break;
		}
		case eAGY_CMD_END:
		{
			aStoneList branches;
			m_goban->Goto(m_goban->GetCurrentGoId(), INDEX_END, branches);
			break;
		}
		case eAGY_CMD_NNEXT:
		{
			aStoneList branches;
			m_goban->Goto(m_goban->GetCurrentGoId(), 10, branches);
			m_prev->Update(m_goban->GetCurrentNode());
			break;
		}
		case eAGY_CMD_PPREV:
		{
			aStoneList branches;
			m_goban->Goto(m_goban->GetCurrentGoId(), -10, branches);
			m_prev->Update(m_goban->GetCurrentNode());
			break;
		}
		case eAGY_CMD_NEXT_BRANCH:
		{
			aStoneList branches;
			m_goban->GotoNextBranch(m_goban->GetCurrentGoId(), branches);
			m_prev->Update(m_goban->GetCurrentNode());
			break;
		}
		case eAGY_CMD_PREV_BRANCH:
		{
			aStoneList branches;
			m_goban->GotoPrevBranch(m_goban->GetCurrentGoId(), branches);
			m_prev->Update(m_goban->GetCurrentNode());
			break;
		}
		case eAGY_CMD_FIND_COMMENT_START:
		{
			m_goban->StartFindComment();
			break;
		}
		case eAGY_CMD_FIND_COMMENT_END:
		{
			m_goban->StartFindComment();
			break;
		}
		case eAGY_CMD_NEXT_COMMENT:
		{
			m_goban->GotoNextComment();

			break;
		}
		case eAGY_CMD_PREV_COMMENT:
		{
			m_goban->GotoPrevComment();

			break;
		}
		case eAGY_CMD_SET_COMMENT:
		{
			char* comment = (char*)param1;
			m_goban->PutComment(m_goban->GetCurrentGoId(), comment);
			break;
		}
		case eAGY_CMD_INIT_STONE:
		{
			int count = param1;
			m_goban->FreeInitStoneMode(count);
			break;
		}
		case eAGY_CMD_DELETE_BRANCH:
		{
			m_goban->DeleteNode((cyiTreeNode*)param1);
		}

		case eAGY_CMD_OPEN:
		{
			char szFileName[256]={0,};
			strcpy(szFileName, "*.sgf");
			if (yiFileOpenDialog(m_hWnd, "開く", "*.sgf", "sgf(*.sgf)\0ygf(*.ygf)", "", szFileName)==True)
			{
				m_goban->AddForEditByFile(szFileName, eGFT_SGF);
				m_goban->SearchResult();
			}
			break;
		}
		case eAGY_CMD_OPEN_YGF:
		{
			char szFileName[256]={0,};
			strcpy(szFileName, "*.ygf");
			if (yiFileOpenDialog(m_hWnd, "開く", "*.ygf", "ygf(*.ygf)\0ygf(*.ygf)", "", szFileName)==True)
			{
				m_goban->AddForEditByFile(szFileName);

			}
			break;
		}
		case eAGY_CMD_SAVE_SGF:
		{
			char szFileName[256];
			
			if (param1==1)
			{
				wsprintf(szFileName, "%s.sgf", m_goban->GetTitle());

				if (yiFileSaveDialog(m_hWnd, "保存", "*.sgf", "sgf(*.sgf)\0", "", szFileName)==True)
				{
					m_goban->Save(szFileName);
				}
			}
			else if (param1==0)
			{
				m_goban->Save(0);
			}
			else
			{
				m_goban->Save((const char*)param1);
			}

			break;
		}
		case eAGY_CMD_SAVE_YGF:
		{
			char szFileName[256];
			
			wsprintf(szFileName, "%s.ygf", m_goban->GetTitle());

			if (yiFileSaveDialog(m_hWnd, "保存", "*.ygf", "ygf(*.ygf)\0", "", szFileName)==True)
			{
				m_goban->Save(szFileName, eGoFileType::eGFT_YGF);
			}
			break;
		}
		case eAGY_CMD_START_CALCULATE:
		{
			m_goban->CalculateMode(m_goban->GetCurrentGoId());
			break;
		}
		case eAGY_CMD_END_CALCULATE:
		{
			int nBlack, nWhite;
			m_goban->CalculateEnd(m_goban->GetCurrentGoId(), &nBlack, &nWhite);
			break;
		}

		case eAGY_CMD_SHOW_NO:
		{
			m_goban->ShowNo(True);
			break;
		}
		case eAGY_CMD_CHANGE_INPUT_MODE:
		{
			m_goban->ChangeInputMode((eInputMode)param1);
			break;
		}
		case eAGY_CMD_SET_LABEL:
		{
			m_goban->SetLabelMode((eLabelType)param1);
			break;
		}
		case eAGY_CMD_OPTIMIZE:
		{
			int Progress=0;
			Bool bIsCanceled=False;
			int max = m_goban->GetFullNodeCnt();
			m_goban->OptimizeNode(&Progress, &bIsCanceled);

			m_ProgressDlg.Create(m_hWnd, "最適化", "最適化中です。中止するにはキャンセルを押して下さい。", 0, max, &Progress, True, &bIsCanceled);
			break;
		}
	}


}

// ---------------------------------
void cAppGoYotsuya::SetMode(eAppMode mode)
{
	m_mode=mode;
}

// ---------------------------------
eAppMode cAppGoYotsuya::GetMode()
{
	return m_mode;
}

// ---------------------------------
void cAppGoYotsuya::OnNodeChanged(cyiTreeNode* node, int wParam)
{
	SendMessage(m_hControlWnd, WM_NOTIFY_NODE_CHANGED, wParam, (LPARAM)node);
}

// ---------------------------------
void cAppGoYotsuya::CreateMapData(cyiTreeNode* node)
{
	m_goban->CreateMapIamge(node);
}


// ---------------------------------
// 定石辞典のみ
cyiList* cAppGoYotsuya::LoadCommentDictionary(const char* name)
{
	int size;
	char* p = (char*)yiFileLoad(name, &size);
	cyiList* list = new cyiList();

	if (p==0)
	{
		return list;
	}

	char buffer[256];
	char* pIndex = p;
	char* pDest=buffer;

	while(1)
	{
		if (*pIndex==0)
		{
			pIndex++;
			*pDest=0;
			if (strlen(buffer))
				list->addref(yistrheap(buffer));

			if (*pIndex==0)
				break;
			pDest=buffer;
		}

		*pDest++=*pIndex++;
	}

	yiFileUnload(p);
	return list;
}

// ---------------------------------
// 定石辞典のみ
int cAppGoYotsuya::RegisterCommentToDictionary(cyiList* list, char* pszComment)
{
	return list->addref(yistrheap(pszComment));
}

// ---------------------------------
// 定石辞典のみ
int cAppGoYotsuya::DeleteCommentToDictionary(cyiList* list, int index)
{
	list->remove(index, yifree);
	return 0;
}

// ---------------------------------
// 定石辞典のみ
int cAppGoYotsuya::SaveCommentToDictionary(const char* name, cyiList* list)
{
	int size=4096;
	int nWrittenByte=0;
	char* buffer=(char*)yialloc(size);
	char* pIndex=buffer;

	for (cyiNode* node=list->GetNodeTopFrom(0); node; node=node->np)
	{
		char* str = (char*)node->ref;
		int len = strlen(str);

		if (len+nWrittenByte>size)
		{
			size+=4096;
			buffer = (char*)yirealloc(buffer, size);
			pIndex=buffer;
			pIndex=&pIndex[nWrittenByte];
		}

		memcpy(pIndex, str, len);
		pIndex[len]=0;
		pIndex=&pIndex[len+1];
		nWrittenByte+=len+1;
	}

	*pIndex=0;
	pIndex[1]=0;

	char xxxx[256];
	wsprintf(xxxx, "%s.bak", name);
	MoveFile(name, xxxx);
	yiFileWrite(name, buffer, 0, nWrittenByte+2);

	return 0;
}