#include "MainWindow.h"
#include "JosekiEditorApp.h"
#include "resource.h"


static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

CMainWindow::CMainWindow(void)
{
}

CMainWindow::~CMainWindow(void)
{
}

// ---------------------------------------------------
LRESULT CMainWindow::OnSizeChanged(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	xTRACE("CMainWindow::OnSizeChanged()");
	cAppGoYotsuya& app = cAppGoYotsuya::GetInstance();
	RECT rect;
	::GetClientRect(m_hWnd, &rect);

	if (wParam==SIZE_MINIMIZED)
	{
		handled=False;
		return 0;
	}

	app.m_goban->Resize(rect.right, rect.bottom);
	int w;
	app.m_goban->GetSize(&w);//GetSize();
	SetWindowPos(app.m_hControlWnd, 0, w, 0, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	::InvalidateRect(app.m_hControlWnd, 0, FALSE);
	UpdateWindow(app.m_hControlWnd);

	__super::OnSizeChanged(wParam, lParam, handled);
	return 0;
}

// ---------------------------------------------------
LRESULT CMainWindow::OnPreviewSelected(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	cAppGoYotsuya::GetInstance().OnPreViewSelected(wParam, lParam);
	return 0;
}

// ---------------------------------------------------
LRESULT CMainWindow::OnPutStoned(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	cAppGoYotsuya::GetInstance().OnPutStoned(wParam, lParam);
	return 0;
}

// ---------------------------------------------------
LRESULT CMainWindow::OnQuitOptimize(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	char buffer[128];
	wsprintf(buffer, "%d件の同一型を統合しました。", wParam);
	cAppGoYotsuya::GetInstance().m_ProgressDlg.SetProgressOK(buffer);
	return 0;
}

// ---------------------------------------------------
LRESULT CMainWindow::OnSearchResult(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	//cAppGoYotsuya& app = cAppGoYotsuya::GetInstance();
	//app.m_prev->UpdateTree((cyiTreeNode*)lParam);
	return 0;
}

// ---------------------------------------------------
LRESULT CMainWindow::OnNodeChanged(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	cAppGoYotsuya::GetInstance().OnNodeChanged((cyiTreeNode*)lParam, wParam);
	return 0;
}

// ---------------------------------------------------
LRESULT CMainWindow::OnClose(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	if (cAppGoYotsuya::GetInstance().GetMode()==eAppMode::eAM_JOSEKI_EDITOR)
	{
		if (MessageBox(m_hWnd, "変更を保存しますか？", "お知らせ", MB_YESNO|MB_ICONINFORMATION)==IDYES)
		{
			cAppGoYotsuya::GetInstance().m_goban->Save(0, eGoFileType::eGFT_SGF);
		}
	}
	return 0;
}

#include "glsDialog.h"
// ---------------------------------------------------
LRESULT CMainWindow::OnCreate(WPARAM wParam, LPARAM lParam)
{
	xTRACE("CMainwindow::OnCreate(%x)", this);
	__super::OnCreate(wParam, lParam);

	m_TextureImg = BITMAP32(32, 32);//new Image(L".\\data\\131.jpg");
	Graphics gt(m_TextureImg);

	gt.FillRectangle(&SolidBrush(Color::Beige), 0, 0, 32, 32);
	gt.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	gt.FillEllipse(&SolidBrush(Color::YellowGreen), Rect(8, 8, 16, 16));

	m_app=&cAppGoYotsuya::GetInstance();


	m_app->OnCreate((HWND)this);

	//cglsDialog* dlg=new cglsDialog();

	//dlg->Create((HWND)this, 777, 0, 270, 100, 400, 200, 0);
	//dlg->Show();

	//AnimateWindow(m_hWnd, 1000, AW_CENTER);

	return 0;
}

// ---------------------------------------------------
LRESULT CMainWindow::OnDestroy(WPARAM wParam, LPARAM lParam)
{
	CJosekiEditorApp* app=xGetMyApp();

	{
		RECT rect;
		char szValue[32];
		GetWindowRect(m_hWnd, &rect);
		wsprintf(szValue, "(%d,%d,%d,%d)", rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top);
		app->SetProperty(0, "DISPLAY", szValue);
	}

	//cAppGoYotsuya::GetInstance().m_goban->Destroy();

	const sGobanDetail* detail = cAppGoYotsuya::GetInstance().m_goban->GetGobanDetail();

	app->SetProperty(0, "bIsDefaultLabelShow", detail->bIsDefaultLabelShow?"True":"False");
	app->SetProperty(0, "bIsJosekiEditorMode", detail->bIsJosekiEditorMode?"True":"False");
	app->SetProperty(0, "bIsPreviewGotoSame", detail->bIsPreviewGotoSame?"True":"False");
	app->SetProperty(0, "bIsPreviewClikFocus", detail->bIsPreviewClikFocus?"True":"False");
	app->SetProperty(0, "bIsPreviewNewTab", detail->bIsPreviewNewTab?"True":"False");
	app->SetPropertyInt(0, "nDefaultLineCount", detail->nDefaultLineCount);
	app->SetProperty(0, "bIsPreviewShow", detail->bIsPrevewShow?"True":"False");
	app->SetProperty(0, "bIsPreviewPopup", detail->bIsPreviewPopup?"True":"False");
	app->SetProperty(0, "bIsNormalQuit", "True");
	//app->SetPropertyInt(0, "nBranchViewMode", detail->nBranchViewMode);

	//delete cAppGoYotsuya::GetInstance().m_goban;

	//cAppGoYotsuya::GetInstance().m_prev->Destroy();
	//delete cAppGoYotsuya::GetInstance().m_prev;
	PostQuitMessage(0);
	return 0;
}

typedef struct {
	unsigned int flag;
	cyiTreeNode* orgNode;
	cyiTreeNode* newNode;
} sMergeData;
cyiList* g_listMerge=0;
static INT_PTR CALLBACK OnMergeProc(HWND hWnd, UINT iMsg, WPARAM wp, LPARAM lp)
{
	switch(iMsg)
	{
		case WM_INITDIALOG:
		{
			g_listMerge=(cyiList*)lp;
			HWND hListBox = GetDlgItem(hWnd, IDC_LIST_MERGE);

			int cnt=0;

			for (cyiNode* node = g_listMerge->GetNodeTopFrom(0); node; node=node->np)
			{
				char buffer[64];
				cyiTreeNode* xx = (cyiTreeNode*)node->ref;

				wsprintf(buffer, "%d:%08x", cnt++, xx);
				SendMessage(hListBox , LB_ADDSTRING , 0, (LPARAM)buffer);
			}

			char buffer[64];
			wsprintf(buffer, "全%d件", g_listMerge->count());
			SetWindowText(hWnd, buffer);

			return 1;
		}
		case WM_COMMAND:
		{
			int id = LOWORD(wp);

			if (id==IDOK || id==IDCANCEL)
			{
				//if (id==IDCANCEL)
				//{
					cAppGoYotsuya::GetInstance().m_goban->ReleaseMergeData(g_listMerge);
					g_listMerge=0;
					DestroyWindow(hWnd);
					break;
				//}
				
				// ここでマージ


			}
			else if (id==IDC_LIST_MERGE)
			{
				if (HIWORD(wp)==LBN_SELCHANGE)
				{
					HWND hListBox = (HWND)lp;
					CJosekiEditorApp* app=xGetMyApp();
					int index = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
					cyiTreeNode* data = (cyiTreeNode* )g_listMerge->GetTopFromRef(index);
					app->m_goban->PlayByNode(data);
				}
			}
			else if (id==IDC_NEW_NODE)
			{
				HWND hListBox = (HWND)GetDlgItem(hWnd, IDC_LIST_MERGE);
				int index = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
				sMergeData* data = (sMergeData* )g_listMerge->GetTopFromRef(index);

				CJosekiEditorApp* app = xGetMyApp();

				if (data->newNode)
				{
					app->m_goban->PlayByNode(data->newNode);
				}
				else
				{
					app->m_goban->Goto(0, MOVE_TOP, 0, 0);
					break;
				}
			}
			return 1;
		}
		default:
		{
			break;
		}
	}

	return 0;
}

sSoftwareLisenseCode g_license;
// ---------------------------------------------------
LRESULT CMainWindow::OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	int wmId    = LOWORD(wParam);
	int wmEvent = HIWORD(wParam);
	// 選択されたメニューの解析:
	switch (wmId)
	{
	case IDM_ABOUT:
		DialogBox(m_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), m_hWnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(m_hWnd);
		break;
	case IDM_EXPORT:
	{
		unsigned int n = 1;
		memset(&g_license, 0, sizeof(g_license));
		g_license.time = cyiTime::GetLocalTime(g_license.day);
		strcpy(g_license.szSoftwareName, "Yotsuya Joseki Special 1.0");
		g_license.flag32|=0x80000000;
		//m_app->m_goban->Export(".\\data\\joseki.bin", &g_license);

		HandleArchive hArch=yiArchiveCreate(0);
		
		yiArchiveAdd(hArch, &g_license, sizeof(g_license), "BGF\0", 4);

		int size=0;
		void* p = yiFileLoad(".\\data\\joseki.dat", &size);

		cyiMemoryStream* stream = yiUtilCompress(p, size);
		Binary data = stream->ExportToBinary();

		yiArchiveAdd(hArch, data, xBIN_SIZE(data), &n, sizeof(int));

		DeleteFile(".\\data\\joseki.bin");
		yiArchiveExport(hArch, ".\\data\\joseki.bin");

		delete stream;
		yifree(p);

		break;
	}
	case IDC_SAVE_NEW:
	{
		char filename[256];
		strcpy(filename, "untitled.sgf");
		if (::yiFileSaveDialog(m_hWnd, "Save As", ".\\", "*.sgf", "SGF(*.sgf)", filename)==True)
		{
			m_app->m_goban->Save(filename);
		}
		break;
	}
	case IDC_MERGE:
	{
		char filename[256];
		strcpy(filename, "*.sgf");
		if (::yiFileOpenDialog(m_hWnd, "開く", ".\\", "*.sgf", "SGF(*.sgf)", filename)==True)
		{
			cyiList* list = m_app->m_goban->GetMergeData(0, filename);

			if (list==0)
			{
				MessageBox(*this, "データは完全に一致します。", "お知らせ", MB_OK);
				break;
			}
			if (list)
			{
				::CreateDialogParamA((HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE), MAKEINTRESOURCEA(IDD_MERGE), m_hWnd, OnMergeProc, (LPARAM)list);
			}

		}
		break;
	}
	case IDC_NODE_COPY:
	{
		m_app->m_goban->CopyAndAdd(0, eIM_EDIT, 0, False);
		break;
	}
	default:
		return DefWindowProc(m_hWnd, WM_COMMAND, wParam, lParam);
	}
	//break;

	//bIsHandled = m_app.Command(wParam&0xFFFF, wParam>>16, lParam);
	return 0;
}

// ---------------------------------------
Bool CMainWindow::OnBgDraw()
{
	if (m_bmpCurrent==0)
	{
		m_bmpCurrent=BITMAP32(m_w, m_h);
	}
	if (m_bg==0)
	{
		m_bg=BITMAP32(m_w, m_h);
	}

	int w=m_bmpCurrent->GetWidth();
	int h=m_bmpCurrent->GetHeight();

	if (w!=m_w || h!=m_h)
	{
		delete m_bmpCurrent;
		delete m_bg;
		m_bmpCurrent=BITMAP32(m_w, m_h);
		m_bg=BITMAP32(m_w, m_h);
	}

	Graphics g(m_bg);

	TextureBrush br(m_TextureImg);

	g.FillRectangle(&br, 0, 0, m_w, m_h);

	cyiBitmapBlender::BitmapCopy(m_bmpCurrent, m_bg);

	Update(m_bmpCurrent);
	//InvalidateRect(0, False);
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}