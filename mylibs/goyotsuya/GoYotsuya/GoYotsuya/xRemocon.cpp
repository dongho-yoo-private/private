#include "AppGoYotsuya.h"
#include "resource.h"
#include <windows.h>
#include <process.h>

HWND hNotifyWindow;


// ----------------------------------
extern Bool xShowCommentDialog(HWND hWnd);


// ----------------------------------
void xOnRemoconCommand(WPARAM wp, LPARAM lp)
{
	int ctrlId = (0xFFFF&wp);

	switch(ctrlId)
	{
		case IDC_R_PLAY:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_PLAY);
			break;
		}
		case IDC_R_OPEN:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_OPEN);
			break;
		}
		case IDC_R_OPEN_YGF:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_OPEN_YGF);
			break;
		}
		case IDC_R_SAVE_SGF:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_SAVE_SGF, 1);
			break;
		}
		case IDC_R_SAVE_YGF:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_SAVE_YGF);
			break;
		}
		case IDC_R_SHOW_NO:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_SHOW_NO);
			break;
		}
		case IDC_R_BACK:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_BACK);
			break;
		}
		case IDC_R_FF:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_NNEXT);
			break;
		}
		case IDC_R_RV:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_PPREV);
			break;
		}
		case IDC_R_TOP:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_TOP);
			break;
		}
		case IDC_R_END:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_END);
			break;
		}
		case IDC_R_NEXT_BRANCH:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_NEXT_BRANCH);
			break;
		}
		case IDC_R_PREV_BRANCH:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_PREV_BRANCH);
			break;
		}
		case IDC_R_ADDSTONE_MODE:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_CHANGE_INPUT_MODE, (int)eIM_EDIT_ADDSTONE);
			break;
		}
		case IDC_R_NORMAL_MODE:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_CHANGE_INPUT_MODE, (int)eIM_EDIT);
			break;
		}
		case IDC_R_START_COMMENT:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_FIND_COMMENT_START);
			break;
		}
		case IDC_R_END_COMMENT:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_FIND_COMMENT_END);
			break;
		}
		case IDC_R_NEXT_COMMENT:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_NEXT_COMMENT);
			break;
		}
		case IDC_R_PREV_COMMENT:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_PREV_COMMENT);
			break;
		}
		case IDC_R_FREE_INITSTONE:
		{
			HWND hWndInitStone = GetDlgItem(GetParent((HWND)lp), IDC_INIT_STONE_COUNT);
			int index = SendMessage(hWndInitStone, CB_GETCURSEL, 0, 0);
			
			if (index==0)
				break;

			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_INIT_STONE, index+1);
			break;
		}
		case IDC_R_TEXT_OK:
		{
			char buffer[4096];
			HWND hParent = ::GetParent((HWND)lp);

			int n = GetWindowText(GetDlgItem(hParent, IDC_COMMENT), buffer, 4096);

			if (n==0)
				break;

			buffer[n]=0;

			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_SET_COMMENT, (int)buffer);

			break;
		}
		case IDC_R_SELECT_DEAD_STONE:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_START_CALCULATE, 0);
			break;
		}
		case IDC_R_RESULT:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_END_CALCULATE, 0);
			break;
		}
		case IDC_COMMENT_REGISTER:
		{
			HWND hWnd = GetParent((HWND)lp);
			xShowCommentDialog(GetParent(hWnd));
			break;
		}
		case IDC_LABEL_SELECT:
		{
			if (HIWORD(wp)==CBN_SELCHANGE)
			{
				HWND hWndLabel = (HWND)lp;
				int index = SendMessage(hWndLabel, CB_GETCURSEL, 0, 0);

				if (index==0)
				{
					cAppGoYotsuya::GetInstance().Command(eAGY_CMD_SET_LABEL, eLabelType::eLT_LABLE_CLOSE);
				}
				else if (index==1)
				{
					cAppGoYotsuya::GetInstance().Command(eAGY_CMD_SET_LABEL, eLabelType::eLT_ROMA_LARGE);
				}
				else if (index==2)
				{
					cAppGoYotsuya::GetInstance().Command(eAGY_CMD_SET_LABEL, eLabelType::eLT_ROMA_SMALL);
				}
				else if (index==3)
				{
					cAppGoYotsuya::GetInstance().Command(eAGY_CMD_SET_LABEL, eLabelType::eLT_NUMERIC);
				}
				else if (index>3)
				{
					unsigned short code = (0xFE00|(index-3));
					cAppGoYotsuya::GetInstance().Command(eAGY_CMD_SET_LABEL, code);
				}

			}

			break;
		}
	}
}
// ----------------------------------
INT_PTR CALLBACK xRemoconProc(HWND hWnd, UINT iMsg, WPARAM wp, LPARAM lp)
{
	switch(iMsg)
	{
		case WM_INITDIALOG:
		{
			HWND hWndLabel = GetDlgItem(hWnd, IDC_LABEL_SELECT);

			SendMessage(hWndLabel, CB_ADDSTRING, 0, (LPARAM)"none");
			SendMessage(hWndLabel, CB_ADDSTRING, 1, (LPARAM)"A,B,C...");
			SendMessage(hWndLabel, CB_ADDSTRING, 2, (LPARAM)"a,b,c...");
			SendMessage(hWndLabel, CB_ADDSTRING, 3, (LPARAM)"1,2,3...");
			SendMessage(hWndLabel, CB_ADDSTRING, 4, (LPARAM)"□");
			SendMessage(hWndLabel, CB_ADDSTRING, 5, (LPARAM)"○");
			SendMessage(hWndLabel, CB_ADDSTRING, 6, (LPARAM)"△");
			SendMessage(hWndLabel, CB_ADDSTRING, 7, (LPARAM)"×");
			SendMessage(hWndLabel, CB_ADDSTRING, 8, (LPARAM)"好手");
			SendMessage(hWndLabel, CB_ADDSTRING, 9, (LPARAM)"悪手");
			SendMessage(hWndLabel, CB_ADDSTRING, 10, (LPARAM)"いまいちな手");
			SendMessage(hWndLabel, CB_ADDSTRING, 11, (LPARAM)"黒の効き・狙い");
			SendMessage(hWndLabel, CB_ADDSTRING, 12, (LPARAM)"白の効き・狙い");
			SendMessage(hWndLabel, CB_ADDSTRING, 13, (LPARAM)"本手");
			SendMessage(hWndLabel, CB_ADDSTRING, 14, (LPARAM)"はめ手");
			SendMessage(hWndLabel, CB_ADDSTRING, 15, (LPARAM)"難解な手");
			SendMessage(hWndLabel, CB_ADDSTRING, 16, (LPARAM)"先手を取る手");
			SendMessage(hWndLabel, CB_ADDSTRING, 17, (LPARAM)"手筋");

			SendMessage(hWndLabel, CB_SETCURSEL, 0, 0);

			HWND hWndInitStone = GetDlgItem(hWnd, IDC_INIT_STONE_COUNT);

			for (int i=1; i<10; i++)
			{
				char buffer[16];
				wsprintf(buffer, "%d", i);
				SendMessage(hWndInitStone, CB_ADDSTRING, i-1, (LPARAM)buffer);
				SendMessage(hWndInitStone, CB_SETCURSEL, 0, 0);
			}

			return 1;
		}
		case WM_COMMAND:
		{
			int ctrlId = (0xFFFF&wp);

			if (ctrlId==IDOK ||
					ctrlId==IDCANCEL)
			{
				DestroyWindow(hWnd);
				return 1;
			}
			xOnRemoconCommand(wp, lp);
			return 1;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 1;
		}
		default:
			break;
	}

	return 0;
}

// ----------------------------------
unsigned int __stdcall xOnRemoconLoop(void* param)
{
	HWND hNotifyWnd=(HWND)param;

	HWND hRemocon = CreateDialogParamA((HINSTANCE)GetWindowLong(hNotifyWnd, GWL_HINSTANCE), MAKEINTRESOURCEA(IDD_REMOCON),hNotifyWnd, xRemoconProc, (LPARAM)hNotifyWnd);

	RECT rect;
	RECT rect2;
	GetClientRect(hRemocon, &rect);
	GetClientRect(hNotifyWnd, &rect2);
	SetWindowPos(hRemocon, 0, rect2.right-rect.right, rect2.bottom-rect.bottom,0,0, SWP_NOSIZE|SWP_NOZORDER);


	SetFocus(hNotifyWnd);
	if (hRemocon==0)
		return FALSE; 

	//ShowWindow(hRemocon, SW_SHOWNORMAL);
	//UpdateWindow(hRemocon);


	// メイン メッセージ ループ:
	//MSG msg;
	//while (GetMessage(&msg, hRemocon, 0, 0))
	//{
	//	if (TranslateMessage(&msg))
	//	{
	//		DispatchMessage(&msg);
	//	}
	//}

	return True;
}

// ----------------------------------
HWND xCreateRemocon(HWND hWnd)
{
	//unsigned int dwThreadId;

	//xOnRemoconLoop((void*)hWnd);
	//HANDLE hThread = (HANDLE)_beginthreadex(0, 0, xOnRemoconLoop, 	(void*)hWnd, 0, &dwThreadId);
	//CloseHandle(hThread);

	HWND hNotifyWnd=hWnd;

	HWND hRemocon = CreateDialogParamA((HINSTANCE)GetWindowLong(hNotifyWnd, GWL_HINSTANCE), MAKEINTRESOURCEA(IDD_REMOCON),hNotifyWnd, xRemoconProc, (LPARAM)hNotifyWnd);

	RECT rect;
	RECT rect2;
	GetClientRect(hRemocon, &rect);
	GetClientRect(hNotifyWnd, &rect2);
	SetWindowPos(hRemocon, 0, rect2.right-rect.right, rect2.bottom-rect.bottom,0,0, SWP_NOSIZE|SWP_NOZORDER);


	SetFocus(hNotifyWnd);

	return hRemocon;
}