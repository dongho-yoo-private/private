#include "StdAfx.h"
#include "AppGoYotsuya.h"
#include "yistd.h"


// -------------------------------
cAppGoYotsuya* cAppGoYotsuya::m_me=0;


cAppGoYotsuya::cAppGoYotsuya()
{
	//strcpy(m_szppName, pszApplicationName);
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

	//m_goban = new cglsGoban();

	return True;
}

#include "JosekiEditorApp.h"
	int nDefaultLineCount;
	Bool bIsPreviewGotoSame;
	Bool bIsJosekiEditorMode;
	Bool bIsDefaultLabelShow;
	eBranchViewMode nBranchViewMode;
// ---------------------------------
void cAppGoYotsuya::OnCreate(HWND _hWnd)
{
	RECT rect;
	cGlassInterface* obj = (cGlassInterface*)_hWnd;
	HWND hWnd = obj->m_hWnd;

	GetClientRect(hWnd, &rect);
	int nWidth=rect.right > rect.bottom ? rect.bottom:rect.right;

	m_hWnd = hWnd;

	sGobanDetail detail;
	cglsGoban::xDefaultDetail(&detail);

	CJosekiEditorApp* app = xGetMyApp();
	
	app->m_wnd=(CMainWindow*)_hWnd;

	// 碁盤生成
	m_goban=new cglsGoban();

	app->m_goban=m_goban;

	char buffer[128];
	if (app->GetPropertyInt(0, "nDefaultLineCount", detail.nDefaultLineCount)==False)
	{
		cglsGoban::xDefaultDetail(&detail);

		app->SetProperty(0, "bIsDefaultLabelShow", detail.bIsDefaultLabelShow?"True":"False");
		app->SetProperty(0, "bIsJosekiEditorMode", detail.bIsJosekiEditorMode?"True":"False");
		app->SetProperty(0, "bIsPreviewClikFocus", detail.bIsPreviewClikFocus?"True":"False");
		app->SetProperty(0, "bIsPreviewGotoSame", detail.bIsPreviewGotoSame?"True":"False");
		app->SetProperty(0, "bIsPreviewNewTab", detail.bIsPreviewNewTab?"True":"False");
		//app->SetPropertyInt(0, "nBranchViewMode", detail.nBranchViewMode);
		app->SetPropertyInt(0, "nDefaultLineCount", detail.nDefaultLineCount);
		app->SetProperty(0, "bIsPrevewShow", detail.bIsPrevewShow?"True":"False");
		app->SetProperty(0, "bIsPreviewPopup", detail.bIsPreviewPopup?"True":"False");
		app->SetProperty(0, "bIsNormalQuit", "False");

		detail.bIsOldInterface=True;
		m_goban->Create((HWND)obj, 0, 0, rect.right, rect.bottom, 0, &detail);
	}
	else
	{
		detail.bIsPreviewGotoSame=False;
		if (app->GetProperty(0, "bIsPreviewGotoSame", buffer)==True)
		{
			if (stricmp(buffer, "True")==0)
			{
				detail.bIsPreviewGotoSame=True;
			}
		}
		detail.bIsDefaultLabelShow=False;
		if (app->GetProperty(0, "bIsDefaultLabelShow", buffer)==True)
		{
			if (stricmp(buffer, "True")==0)
			{
				detail.bIsDefaultLabelShow=True;
			}
		}
		detail.bIsJosekiEditorMode=False;
		if (app->GetProperty(0, "bIsJosekiEditorMode", buffer)==True)
		{
			if (stricmp(buffer, "True")==0)
			{
				detail.bIsJosekiEditorMode=True;
			}
		}
		detail.bIsPreviewClikFocus=False;
		if (app->GetProperty(0, "bIsPreviewClikFocus", buffer)==True)
		{
			if (stricmp(buffer, "True")==0)
			{
				detail.bIsPreviewClikFocus=True;
			}
		}
		detail.bIsPreviewNewTab=False;
		if (app->GetProperty(0, "bIsPreviewNewTab", buffer)==True)
		{
			if (stricmp(buffer, "True")==0)
			{
				detail.bIsPreviewNewTab=True;
			}
		}
		detail.bIsPrevewShow=False;
		if (app->GetProperty(0, "bIsPreviewShow", buffer)==True)
		{
			if (stricmp(buffer, "True")==0)
			{
				detail.bIsPrevewShow=True;
			}
		}
		detail.bIsPreviewPopup=False;
		if (app->GetProperty(0, "bIsPreviewPopup", buffer)==True)
		{
			if (stricmp(buffer, "True")==0)
			{
				detail.bIsPreviewPopup=True;
			}
		}

		detail.bIsNormalQuit=False;
		if (app->GetProperty(0, "bIsNormalQuit", buffer)==True)
		{
			if (stricmp(buffer, "True")==0)
			{
				detail.bIsNormalQuit=True;
			}
		}

		// バックアップファイルの修復を行います。
#if 0
		if (detail.bIsNormalQuit==False)
		{
			time64_t stamp = cyiTime::GetFileTimeStamp(".\\data\\joseki.dat");

			WIN32_FIND_DATAA data;
			HANDLE hFind = FindFirstFileA(".\\data\\backups\\joseki.dat.*", &data);
			time64_t newestStamp=0;
			char szNewstFileName[64];

			do
			{
				char szFoundFileName[256];
				wsprintf(szFoundFileName, ".\\data\\backups\\%s",data.cFileName); 
				time64_t stamp2 = cyiTime::GetFileTimeStamp(szFoundFileName);//data.cFileName);

				if (newestStamp<stamp2)
				{
					strcpy(szNewstFileName, szFoundFileName);//data.cFileName);
					newestStamp=stamp2;
				}
				//newestStamp = newestStamp>stamp2?newestStamp:stamp2;

			} while(FindNextFileA(hFind, &data));

			if (newestStamp>stamp)
			{
				char szFileName[256];
				wsprintf(szFileName, ".\\data\\joseki.dat.%d.%d.%d.%d.%d.%d", xYEAR64(stamp), xMONTH64(stamp), xDAY64(stamp),
													xHOUR64(stamp), xMINUTE64(stamp), xSECOND64(stamp)); 
				MoveFileA(".\\data\\joseki.dat", szFileName);
				CopyFileA(szNewstFileName, ".\\data\\joseki.dat", FALSE);
			}
			
		}
#endif
		app->SetProperty(0, "bIsNormalQuit", "False");//==True)

		detail.bIsOldInterface=True;
		//detail.nBranchViewMode=(eBranchViewMode)0;
		//app->GetPropertyInt(0, "nBranchViewMode", (int&)detail.nBranchViewMode, 0);
		m_goban->Create((HWND)obj, 0, 0, rect.right, rect.bottom, 0, &detail);
	}

	////m_prev = new cglsPreviewGoban();
	////m_prev->Create(m_hWnd, 128, 5);
	////m_prev->UpdateTree(m_goban->GetCurrentNode());



	// コントロール生成

	// ビューアー生成

	// 各コントロール配置

	return;
}

// ---------------------------------
void cAppGoYotsuya::OnDestroy()
{
	//m_prev->Destroy();
}	

// ---------------------------------
void cAppGoYotsuya::OnPutStoned(WPARAM wp,LPARAM lp)
{
	////m_prev->UpdateTree((cyiTreeNode*)lp);
}

// ---------------------------------
void cAppGoYotsuya::OnPreViewSelected(WPARAM wp,LPARAM lp)
{
	//m_goban->PlayByNode((cyiTreeNode*)lp);
	CJosekiEditorApp* app = xGetMyApp();
	char buffer[16];

	if (wp==0)
	{
		m_goban->PlayByNode((cyiTreeNode*)lp);
	}
	else
	{
		m_goban->AddByNode((cyiTreeNode*)lp, "プレビュー",False, True);
	}
/*
	if (app->GetProperty(0, "bIsPreviewGotoSame", buffer)==True)
	{
		if (stricmp(buffer, "True")==0)
		{
			m_goban->PlayByNode((cyiTreeNode*)lp);
		}
	}

	if (app->GetProperty(0, "bIsPreviewNewTab", buffer)==True)
	{
		if (stricmp(buffer, "True")==0)
		{
			m_goban->AddByNode((cyiTreeNode*)lp, "");
		}
	}
*/
	//if (wp!=-1)
	//{
	//	//m_prev->UpdateTree(m_goban->GetCurrentNode());
	//}
}

// ---------------------------------
Bool cAppGoYotsuya::Command(int id, int param1, int param2)
{
	switch(id)
	{
		case eAGY_CMD_PLAY:
		{
			m_goban->Play();
			////m_prev->UpdateTree(m_goban->GetCurrentNode());
			break;
		}
		case eAGY_CMD_BACK:
		{
			aStoneList branches;
			m_goban->Goto(m_goban->GetCurrentGoId(), -1, branches);
			////m_prev->UpdateTree(m_goban->GetCurrentNode());
			break;
		}
		case eAGY_CMD_TOP:
		{
			aStoneList branches;
			m_goban->Goto(m_goban->GetCurrentGoId(), INDEX_TOP, branches);
			////m_prev->UpdateTree(m_goban->GetCurrentNode());
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
			////m_prev->UpdateTree(m_goban->GetCurrentNode());
			break;
		}
		case eAGY_CMD_PPREV:
		{
			aStoneList branches;
			m_goban->Goto(m_goban->GetCurrentGoId(), -10, branches);
			////m_prev->UpdateTree(m_goban->GetCurrentNode());
			break;
		}
		case eAGY_CMD_NEXT_BRANCH:
		{
			aStoneList branches;
			m_goban->GotoNextBranch(m_goban->GetCurrentGoId(), branches);
			//m_prev->UpdateTree(m_goban->GetCurrentNode());
			break;
		}
		case eAGY_CMD_PREV_BRANCH:
		{
			aStoneList branches;
			m_goban->GotoPrevBranch(m_goban->GetCurrentGoId(), branches);
			//m_prev->UpdateTree(m_goban->GetCurrentNode());
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
				sGobanContentsDetail detail;
				cyiGoEngine::xDefaultDetail(&detail);
				m_goban->AddForEditByFile(szFileName, &detail);
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

				if (m_goban->AddForEditByFile(szFileName))
				{
					char szBackupFileName[256];
					SYSTEMTIME time;
					GetLocalTime(&time);
					wsprintf(szBackupFileName, "%s.%04d.%02d.%02d.%02d.%02d.%02d", szFileName, 
						(int)time.wYear, (int)time.wMonth, (int)time.wDay,
							(int)time.wHour, (int)time.wMinute, (int)time.wSecond);
					CopyFile(szFileName, szBackupFileName, False);
				}
				
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
				m_goban->Save(".\\data\\joseki.dat");
			}
			else if (param1==-2)
			{
				char szBackupFileName[256];
				SYSTEMTIME time;
				GetLocalTime(&time);
				wsprintf(szBackupFileName, "%s.%04d.%02d.%02d.%02d.%02d.%02d", szFileName, 
					(int)time.wYear, (int)time.wMonth, (int)time.wDay,
						(int)time.wHour, (int)time.wMinute, (int)time.wSecond);
				m_goban->Save(szBackupFileName);
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
			m_goban->StartCalculateMode(m_goban->GetCurrentGoId());
			break;
		}
		//case eAGY_CMD_END_CALCULATE:
		//{
		//	int nBlack, nWhite;
		//	m_goban->CalculateEnd(m_goban->GetCurrentGoId(), &nBlack, &nWhite);
		//	break;
		//}

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
		default :
			return False;
	}

	return True;
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
	unsigned int t1=cyiTime::CurrentmSec();
	SendMessage(m_hControlWnd, WM_NOTIFY_NODE_CHANGED, wParam, (LPARAM)node);
	unsigned int t2=cyiTime::CurrentmSec(t1);
	//m_prev->UpdateTree((cyiTreeNode*)node);
	unsigned int t3=cyiTime::CurrentmSec(t1);

	return ;
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
	//int size;
	//char* p = (char*)yiFileLoad(name, &size);
	//cyiList* list = new cyiList();

	//if (p==0)
	//{
	//	return list;
	//}

	//char buffer[256];
	//char* pIndex = p;
	//char* pDest=buffer;

	//while(1)
	//{
	//	if (*pIndex==0)
	//	{
	//		pIndex++;
	//		*pDest=0;
	//		if (strlen(buffer))
	//			list->addref(yistrheap(buffer));

	//		if (*pIndex==0)
	//			break;
	//		pDest=buffer;
	//	}

	//	*pDest++=*pIndex++;
	//}

	//yiFileUnload(p);
	//return list;
	FILE* fp = fopen(name, "r");
	char szBuffer[4096];
	cyiList* list = new cyiList();

	if (fp==0)
	{
		return list;
	}


	while(fgets(szBuffer, 4096, fp)!=0)
	{
		if (strlen(szBuffer)<4)
		{
			continue;
		}
		int n = strlen(szBuffer);
		//szBuffer[n-2]=0;
		list->addref(yistrheap(szBuffer));
		//SendMessage(GetDlgItem(m_hWnd, IDC_CMB_HISTORY), CB_INSERTSTRING, 0, (LPARAM)szBuffer);
	}

	fclose(fp);

	return list;
}

// ---------------------------------
// 定石辞典のみ
int cAppGoYotsuya::RegisterCommentToDictionary(cyiList* list, char* pszComment)
{
	return list->insertref(0, yistrheap(pszComment));
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
int cAppGoYotsuya::CommentToTop(cyiList* list, int index)
{
	cyiNode* node= list->GetNodeTopFrom(index);
	char* pszComment = (char*)node->ref;

	list->remove(index, 0);

	list->insertref(0, pszComment);
	return 0;
}

// ---------------------------------
// 定石辞典のみ
int cAppGoYotsuya::SaveCommentToDictionary(const char* name, cyiList* list)
{
	//int size=4096;
	//int nWrittenByte=0;
	//char* buffer=(char*)yialloc(size);
	//char* pIndex=buffer;

	//for (cyiNode* node=list->GetNodeTopFrom(0); node; node=node->np)
	//{
	//	char* str = (char*)node->ref;
	//	int len = strlen(str);

	//	if (len+nWrittenByte>size)
	//	{
	//		size+=4096;
	//		buffer = (char*)yirealloc(buffer, size);
	//		pIndex=buffer;
	//		pIndex=&pIndex[nWrittenByte];
	//	}

	//	memcpy(pIndex, str, len);
	//	pIndex[len]=0;
	//	pIndex=&pIndex[len+1];
	//	nWrittenByte+=len+1;
	//}

	//*pIndex=0;
	//pIndex[1]=0;

	//char xxxx[256];
	//wsprintf(xxxx, "%s.bak", name);
	//MoveFile(name, xxxx);
	//yiFileWrite(name, buffer, 0, nWrittenByte+2);

	FILE* fp = fopen(name, "w");

	if (fp==0)
		return 0;


	if (fp)
	{
		for (cyiNode* node=list->GetNodeTopFrom();node;node=node->np)
		{
			char szComment[256];
			strcpy(szComment, (char*)node->ref);
			strcat(szComment, "\r\n");
			fputs(szComment, fp);
		}
		fclose(fp);
	}
	return 0;
}