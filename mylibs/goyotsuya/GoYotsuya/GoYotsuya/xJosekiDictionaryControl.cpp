#include "yistd.h"
#include "AppGoYotsuya.h"
#include "resource.h"
#include <windows.h>
#include <process.h>

cyiTreeNode* g_currentNode=0;
unsigned short g_option;
unsigned int g_flag32;
unsigned char g_status;
unsigned char g_comment_code;
unsigned int  g_joseki_flag=0;
char* g_comment;
cyiList* g_comment_list;
cyiList* g_comment_list2;

HandleMutexx g_hMutex;

// ----------------------------------------
const char* g_pszJosekiNameHigh[] = {
	"選択なし",
	"小目",
	"目はずし",
	"高目",
	"星",
	"三々",
	0
};

// ----------------------------------------
const char* g_pszJosekiNameMiddle[] = {
	"なし",
	"小桂馬がかり",
	"一間高がかり",
	"二間高かかり",
	"大桂馬かかり",
	"高かかり",
	"隅がかり",
	"打ち込み",
	"その他",
	0
};

// ----------------------------------------
const char* g_pszJosekiNameDetail[] = {
	"なし",
	"一間ばさみ",
	"一間高ばさみ",
	"二間ばさみ",
	"二間高ばさみ",
	"三間ばさみ",
	"三間高ばさみ",
	"こすみ",
	"下つけ",
	"上つけ",
	"一間とび",
	"小桂馬",
	"大桂馬",
	"両がかり"
	"その他",
	0
};

int g_adjust_cnt=0;


// ----------------------------------
// 
static int xGetJosekiName(char* pszJosekiName, cyiTreeNode* node, int& _h, int& _l, int& _d)
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();
	cyiTreeNode* t = node;
	int joseki_code=0;
	int h=0, l=0, d=0;
	Bool bIsHasAName=False;
	_h=0;
	_l=0;
	_d=0;

	pszJosekiName[0]=0;
	if (info==0)
		return -1;

	while(t)
	{
		info = (cSgfStoneInfo*)t->GetData();

		if (info==0)
		{
			return -1;
		}

		if (info->exinfo!=0)
		{
			joseki_code = GET_JOSEKI_NAME(info->exinfo->flag32);


			if (joseki_code!=0)
			{
				h = GET_JOSEKI_HIGH(joseki_code);
				l = GET_JOSEKI_LOW(joseki_code);
				d = GET_JOSEKI_DETAIL(joseki_code);

				if (node==t)
				{
					bIsHasAName=True;
				}

				if (d!=0)
					_d=d;
				if (l!=0)
					_l=l;
				if (h!=0)
					_h=h;

				if (h!=0)
					break;
			}
		}

		t=t->GetParent();
	}

	if (_h==0)
		return -1;

	strcpy(pszJosekiName, g_pszJosekiNameHigh[_h]);

	if (_l==0)
	{
		if (bIsHasAName==True)
		{
			return 1;
		}
		return 0;
	}
	strcat(pszJosekiName, "-");
	strcat(pszJosekiName, g_pszJosekiNameMiddle[_l]);

	if (_d==0)
	{
		if (bIsHasAName==True)
		{
			return 1;
		}
		return 0;
	}

	strcat(pszJosekiName, "-");
	strcat(pszJosekiName, g_pszJosekiNameDetail[_d]);

	if (bIsHasAName==True)
		return 1;

	return 0;
}

// ----------------------------------
static void xOnCommand(WPARAM wp, LPARAM lp)
{
	int ctrlId = (0xFFFF&wp);
	int excode = HIWORD(wp);
	HWND hControl = (HWND)lp;

	switch(ctrlId)
	{
		case IDC_R_PLAY:
		{
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_PLAY);
			break;
		}
		case IDC_R_SAVE_SGF:
		{
			yiMutexLockEx(g_hMutex);

			if (lp!=-1)
				cAppGoYotsuya::GetInstance().Command(eAGY_CMD_SAVE_SGF, 0);
			else
			{
				::CopyFile(".\\data\\joseki.dat", ".\\data\\joseki.dat.copy", FALSE);
				cAppGoYotsuya::GetInstance().Command(eAGY_CMD_SAVE_SGF, (int)".\\data\\Joseki.dat.bak");
			}
			yiMutexUnlockEx(g_hMutex);
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
		case IDC_R_PASS:
		{
			cAppGoYotsuya::GetInstance().m_goban->PutStone(0, STONE_PASS);
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
		case IDC_R_DELETE_BRANCH:
		{
			HWND hwnd = (HWND)lp;
			if (MessageBox(GetParent(hwnd), "以降の枝も全て削除されるよ。\r\n大丈夫？", "警告", MB_YESNO|MB_ICONWARNING)==IDYES)
			{
				cAppGoYotsuya::GetInstance().m_goban->DeleteNode(0);
			}
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
		case IDC_R_HIDE_LABEL:
		{
			Bool bIsShow = cAppGoYotsuya::GetInstance().m_goban->ShowHideLabel();

			if (bIsShow==True)
			{
				SetWindowText((HWND)lp, "ラベル表示");
			}
			else
			{
				SetWindowText((HWND)lp, "ラベルを隠す");
			}

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
				else if (index==4)
				{
					cAppGoYotsuya::GetInstance().Command(eAGY_CMD_SET_LABEL, eLabelType::eLS_SQURE);
				}
				else if (index==5)
				{
					cAppGoYotsuya::GetInstance().Command(eAGY_CMD_SET_LABEL, eLabelType::eLS_CIRCLE);
				}
				else if (index==6)
				{
					cAppGoYotsuya::GetInstance().Command(eAGY_CMD_SET_LABEL, eLabelType::eLS_TRIANGLE);
				}
				else if (index==7)
				{
					cAppGoYotsuya::GetInstance().Command(eAGY_CMD_SET_LABEL, eLabelType::eLS_MA);
				}

			}

			break;
		}
		case IDC_CMB_LABEL_SPETIAL:
		{
			if (HIWORD(wp)==CBN_SELCHANGE)
			{
				HWND hWndLabel = (HWND)lp;
				int index = SendMessage(hWndLabel, CB_GETCURSEL, 0, 0);
				unsigned short code = (0xFE00|(index+4));

				if (index==0)
				{
					cAppGoYotsuya::GetInstance().Command(eAGY_CMD_SET_LABEL, eLabelType::eLT_LABLE_CLOSE);
					break;
				}
				cAppGoYotsuya::GetInstance().Command(eAGY_CMD_SET_LABEL, code);
				break;
			}
		}
		case IDC_COMMENT_CLEAR:
		{
			HWND hComment = GetDlgItem(GetParent((HWND)lp), IDC_COMMENT);
			SetWindowText(hComment, 0);
			break;
		}
		case IDC_CMB_TEXT:
		{
			if (HIWORD(wp)==CBN_SELCHANGE)
			{
				char result[1024];
				char buffer[1024];
				HWND hWndText = (HWND)lp;
				int index = SendMessage(hWndText, CB_GETCURSEL, 0, 0);

				if (index==-1)
					break;
				SendMessage(hWndText, CB_GETLBTEXT, (WORD)index, (LONG)buffer);

				HWND hParent=GetParent(hWndText);
				HWND hSubject=GetDlgItem(hParent, IDC_CMB_SUBJECT);
				index = SendMessage(hSubject, CB_GETCURSEL, 0, 0);
				if (index!=-1)
				{
					char subject[32];
					SendMessage(hSubject, CB_GETLBTEXT, (WORD)index, (LONG)subject);

					wsprintf(result, buffer, subject, buffer);
				}
				else
				{
					strcpy(result, buffer);
				}

				HWND hComment = GetDlgItem(hParent, IDC_COMMENT);
				char bufferx[4096];
				bufferx[0];
				
				GetWindowText(hComment, bufferx, 4096);
				if (bufferx[0]!=0)
				{
					strcat(bufferx, "\r\n");
				}

				strcat(bufferx, result);

				SetWindowText(hComment, bufferx);

			}
			break;
		}
		case IDC_CMB_JOSEKI_NAME_HIGH:
		case IDC_CMB_JOSEKI_NAME_LOW:
		case IDC_CMB_JOSEKI_NAME_DETAIL:
		{
			if (HIWORD(wp)==CBN_SELCHANGE)
			{
				HWND hParent = GetParent((HWND)lp);
				int h, l, d;
				char buffer[128];

				h = SendMessage(GetDlgItem(hParent, IDC_CMB_JOSEKI_NAME_HIGH), CB_GETCURSEL, 0, 0);

				if (h==0 || h==-1)
					return ;

				strcpy(buffer, g_pszJosekiNameHigh[h]);
				SetWindowText(GetDlgItem(hParent, IDC_LBL_JOSEKI_NAME), buffer);

				l = SendMessage(GetDlgItem(hParent, IDC_CMB_JOSEKI_NAME_LOW), CB_GETCURSEL, 0, 0);
				if (l==0 || l==-1)
					return ;

				strcat(buffer, "-");
				strcat(buffer, g_pszJosekiNameMiddle[l]);
				SetWindowText(GetDlgItem(hParent, IDC_LBL_JOSEKI_NAME), buffer);

				d = SendMessage(GetDlgItem(hParent, IDC_CMB_JOSEKI_NAME_DETAIL), CB_GETCURSEL, 0, 0);

				if (d==0 || d==-1)
					return ;

				strcat(buffer, "-");
				strcat(buffer, g_pszJosekiNameDetail[d]);
				SetWindowText(GetDlgItem(hParent, IDC_LBL_JOSEKI_NAME), buffer);

			}
			break;
		}

		case IDC_CMB_TEXT2:
		{
			if (HIWORD(wp)==CBN_SELCHANGE)
			{
				char buffer[1024];
				HWND hWndText = (HWND)lp;
				int index = SendMessage(hWndText, CB_GETCURSEL, 0, 0);

				if (index==-1)
					break;
				SendMessage(hWndText, CB_GETLBTEXT, (WORD)index, (LONG)buffer);
				
				HWND hComment = GetDlgItem(GetParent(hWndText), IDC_COMMENT);
				char bufferx[4096];
				bufferx[0];
				
				GetWindowText(hComment, bufferx, 4096);

				if (bufferx[0]!=0)
				{
					strcat(bufferx, "\r\n");
				}
				strcat(bufferx, buffer);

				SetWindowText(hComment, bufferx);

			}
			break;
		}
		case IDC_CMB_NANIDO:
		case IDC_CMB_HINDO:
		{
			if (HIWORD(wp)==CBN_SELCHANGE)
			{
				HWND hWndCmb = (HWND)lp;
				int index = SendMessage(hWndCmb, CB_GETCURSEL, 0, 0);

				//cAppGoYotsuya::GetInstance().Command(eAGY_CMD_SET_FLAG, );
			}
			break;
		}
		case IDC_COMMENT_REGISTER:
		{
			char buffer[1024];
			HWND hWnd = GetParent((HWND)lp);
			GetWindowText(GetDlgItem(hWnd, IDC_TXT_REGISTER), buffer, 1024);
			cAppGoYotsuya::GetInstance().RegisterCommentToDictionary(g_comment_list, buffer);
			cAppGoYotsuya::GetInstance().SaveCommentToDictionary(".\\data\\comment.dat", g_comment_list);

			SendMessage(GetDlgItem(hWnd, IDC_CMB_TEXT), CB_RESETCONTENT, 0, 0);
			for (cyiNode* node = g_comment_list->GetNodeTopFrom(0); node; node=node->np)
			{
				SendMessage(GetDlgItem(hWnd, IDC_CMB_TEXT), CB_ADDSTRING, 0, (LONG)node->ref);
			}

			SetWindowText(GetDlgItem(hWnd, IDC_TXT_REGISTER), 0);
			break;
		}
		case IDC_COMMENT_REGISTER2:
		{
			char buffer[1024];
			HWND hWnd = GetParent((HWND)lp);
			GetWindowText(GetDlgItem(hWnd, IDC_TXT_REGISTER2), buffer, 1024);
			cAppGoYotsuya::GetInstance().RegisterCommentToDictionary(g_comment_list2, buffer);
			cAppGoYotsuya::GetInstance().SaveCommentToDictionary(".\\data\\comment2.dat", g_comment_list2);

			SendMessage(GetDlgItem(hWnd, IDC_CMB_TEXT2), CB_RESETCONTENT, 0, 0);
			for (cyiNode* node = g_comment_list2->GetNodeTopFrom(0); node; node=node->np)
			{
				SendMessage(GetDlgItem(hWnd, IDC_CMB_TEXT2), CB_ADDSTRING, 0, (LONG)node->ref);
			}

			SetWindowText(GetDlgItem(hWnd, IDC_TXT_REGISTER2), 0);
			break;
		}
		case IDC_R_QUIT_LABEL:
		{
			HWND hParent = GetParent((HWND)lp);
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_SET_LABEL, eLabelType::eLT_LABLE_CLOSE);
			SendMessage(GetDlgItem(hParent, IDC_CMB_LABEL_SPETIAL), CB_SETCURSEL, 0, 0);
			SendMessage(GetDlgItem(hParent, IDC_LABEL_SELECT), CB_SETCURSEL, 0, 0);
			break;
		}
		case IDC_COMMENT_DELETE:
		{
			HWND hParent = GetParent((HWND)lp);
			int n = SendMessage(GetDlgItem(hParent, IDC_CMB_TEXT), CB_GETCURSEL, 0, 0);
			if (n!=-1)
				SendMessage(GetDlgItem(hParent, IDC_CMB_TEXT), CB_DELETESTRING, n, 0);

			cAppGoYotsuya::GetInstance().DeleteCommentToDictionary(g_comment_list, n);
			cAppGoYotsuya::GetInstance().SaveCommentToDictionary(".\\data\\comment.dat", g_comment_list);
			break;
		}
		case IDC_COMMENT_DELETE2:
		{
			HWND hParent = GetParent((HWND)lp);
			int n = SendMessage(GetDlgItem(hParent, IDC_CMB_TEXT2), CB_GETCURSEL, 0, 0);
			if (n!=-1)
				SendMessage(GetDlgItem(hParent, IDC_CMB_TEXT2), CB_DELETESTRING, n, 0);

			cAppGoYotsuya::GetInstance().DeleteCommentToDictionary(g_comment_list2, n);
			cAppGoYotsuya::GetInstance().SaveCommentToDictionary(".\\data\\comment2.dat", g_comment_list2);
			break;
		}
		case IDC_ADJUST:
		{
			if (g_currentNode==0)
				break;

			cSgfStoneInfo* info  = (cSgfStoneInfo*)g_currentNode->GetData();

			if (info==0)
			{
				break;
			}

			if (info->exinfo==0)
			{
				info->exinfo = (sSgfExtendInfo*)yialloci(sizeof(sSgfExtendInfo));
				cAppGoYotsuya::GetInstance().CreateMapData(g_currentNode);
			}

			char buffer[4096];

			if (info->comment)
			{
				yifree(info->comment);
			}
			GetWindowText(GetDlgItem(GetParent((HWND)lp), IDC_COMMENT), buffer, 4096);
			info->comment = yistrheap(buffer);
			info->exinfo->flag32=g_flag32;
			info->exinfo->option=g_option;
			info->exinfo->status=g_status;

			if (info->exinfo->option!=0)
			{
				// 再生済みへ
				info->flag32|=0x80000000;
			}

			g_adjust_cnt++;


			break;
		}
		case IDC_R_OPTIMIZE:
		{					
			cAppGoYotsuya::GetInstance().Command(eAGY_CMD_OPTIMIZE);
			break;
		}
		case IDC_CMB_STONE_ATTR:
		{
			if (HIWORD(wp)==CBN_SELCHANGE)
			{
				HWND hWndText = (HWND)lp;
				int index = SendMessage(hWndText, CB_GETCURSEL, 0, 0);

				if (index!=-1)
				{
					if (g_currentNode==0)
						break;

					cSgfStoneInfo* info  = (cSgfStoneInfo*)g_currentNode->GetData();

					if (info)
					{
						eStoneType srcType = STONE_TYPE(info->stone);
						eStoneType type;

						if (index==0)
							type = STONE_BLACK;
						else if (index==1)
							type= STONE_WHITE;
						else
						{
							if (srcType==STONE_BLACK)
								type=STONE_BLACK_PASS;
							else if (srcType==STONE_WHITE)
								type=STONE_WHITE_PASS;
						}

						int x = STONE_POS_X(info->stone);
						int y = STONE_POS_Y(info->stone);
						int order=STONE_ORDER(info->stone);

						info->stone=MAKESTONE(type, order, x, y);
					}
					
				}
			}
			break;
		}
		case IDC_CHK_OKINIIRI:
		{
			if (g_currentNode==0)
				break;

			cSgfStoneInfo* info  = (cSgfStoneInfo*)g_currentNode->GetData();

			if (info==0)
				break;

			int n = SendMessage((HWND)lp, BM_GETCHECK, 0, 0);

			if (n==0)
			{
				info->flag32&=~(0x40000000);
				break;
			}
			info->flag32|=0x40000000;
			cAppGoYotsuya::GetInstance().CreateMapData(g_currentNode);
			break;
		}
	}
}

// ----------------------------------
static void EnableDisableButtons(HWND hWnd, BOOL bIsForcedTrue=FALSE)
{
	HWND hWndType = GetDlgItem(hWnd, IDC_CMB_TYPE);
	int id = SendMessage(hWndType, CB_GETCURSEL, 0, 0);

	// すべて非活性化
	if (bIsForcedTrue==FALSE &&
			id==0)
	{
		EnableWindow(GetDlgItem(hWnd, IDC_CMB_HINDO), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_CMB_STATUS), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_CMB_NANIDO), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_CHK_EDIT_COMPLETE), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_CHK_KIKI), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_CHK_OKIGO), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_CHK_SICHO), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_CHK_SENTE), FALSE);
	}
	// すべて活性化
	else
	{
		EnableWindow(GetDlgItem(hWnd, IDC_CMB_HINDO), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_CMB_STATUS), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_CMB_NANIDO), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_CHK_EDIT_COMPLETE), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_CHK_KIKI), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_CHK_OKIGO), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_CHK_SICHO), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_CHK_SENTE), TRUE);
	}

}


// ----------------------------------
static void InitControlz(HWND hWnd)
{
	// コントロールの初期化

	// コンボボックスの初期化

	SendMessage(GetDlgItem(hWnd, IDC_CMB_HINDO), CB_SETCURSEL, 0, 0);
	SendMessage(GetDlgItem(hWnd, IDC_CMB_STATUS), CB_SETCURSEL, 0, 0);
	SendMessage(GetDlgItem(hWnd, IDC_CMB_NANIDO), CB_SETCURSEL, 0, 0);
	SendMessage(GetDlgItem(hWnd, IDC_CMB_TYPE), CB_SETCURSEL, 0, 0);

	// テキストボックスの初期化

	// チェックボックスの初期化
	SendMessage(GetDlgItem(hWnd, IDC_CHK_EDIT_COMPLETE), BM_SETCHECK, (WPARAM)0, 0L);
	SendMessage(GetDlgItem(hWnd, IDC_CHK_KIKI), BM_SETCHECK, (WPARAM)0, 0L);
	SendMessage(GetDlgItem(hWnd, IDC_CHK_OKIGO), BM_SETCHECK, (WPARAM)0, 0L);
	SendMessage(GetDlgItem(hWnd, IDC_CHK_SICHO), BM_SETCHECK, (WPARAM)0, 0);
	SendMessage(GetDlgItem(hWnd, IDC_CHK_SENTE), BM_SETCHECK, (WPARAM)0, 0);

	g_comment_list = cAppGoYotsuya::GetInstance().LoadCommentDictionary(".\\data\\comment.dat");
	g_comment_list2 = cAppGoYotsuya::GetInstance().LoadCommentDictionary(".\\data\\comment2.dat");

	SendMessage(GetDlgItem(hWnd, IDC_CMB_TEXT), CB_RESETCONTENT, 0, 0);
	for (cyiNode* node = g_comment_list->GetNodeTopFrom(0); node; node=node->np)
	{
		SendMessage(GetDlgItem(hWnd, IDC_CMB_TEXT), CB_ADDSTRING, 0, (LONG)node->ref);
	}
	SendMessage(GetDlgItem(hWnd, IDC_CMB_TEXT2), CB_RESETCONTENT, 0, 0);
	for (cyiNode* node = g_comment_list2->GetNodeTopFrom(0); node; node=node->np)
	{
		SendMessage(GetDlgItem(hWnd, IDC_CMB_TEXT2), CB_ADDSTRING, 0, (LONG)node->ref);
	}

	SendMessage(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_HIGH), CB_SETCURSEL, 0, 0);
	SendMessage(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_LOW), CB_SETCURSEL, 0, 0);
	SendMessage(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_DETAIL), CB_SETCURSEL, 0, 0);

		EnableWindow(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_HIGH), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_LOW), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_DETAIL), FALSE);

	SetWindowText(GetDlgItem(hWnd, IDC_LBL_JOSEKI_NAME), 0);

}

// --------------------------------
eGoResultStatus g_status_code[10] = {
	eGRS_UNKNOWN, 
	eGRS_SAME, 
	eGS_BLACK_GOOD, 
	eGS_WHITE_GOOD, 
	eGS_BLACK_LITTLE_GOOD, 
	eGS_WHITE_LITTLE_GOOD,
	eGS_BLACK_TEREBLE,
	eGS_WHITE_TEREBLE,
	eGRS_BLACK_OTHER, // 状況次第
	eGRS_WHITE_OTHER  // 状況次第
};

// ----------------------------------
static wchar_t xGetLabelCode(cyiTreeNode* node, int x, int y)
{
	if (node==0)
	{
		return 0;
	}

	cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();

	if (info==0)
		return 0;

	if (info->label==0)
		return 0;

	int n = info->label->count();

	if (n==0)
		return 0;

	for (cyiNode* xnode= info->label->GetNodeTopFrom(); xnode; xnode=xnode->np)
	{
		label_t label = (label_t)xnode->ref;
		int _x = label>>24;
		int _y = (label>>16)&0xFF;
		wchar_t code = label&0xFFFF;

		if (x==_x && y==_y)
			return code;

	}

	return 0;
}

// ----------------------------------
static void xSetLabelCode(cyiTreeNode* node, int x, int y, int code)
{
	if (node==0)
	{
		return ;
	}

	cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();

	if (info==0)
		return ;

	if (info->label==0)
	{
		info->label = new cyiList();
		info->label->addref((void*)((x<<24)|(y<<16)|code));
		return ;
	}

	int n = info->label->count();

	if (n==0)
		return ;

	for (cyiNode* xnode= info->label->GetNodeTopFrom(); xnode; xnode=xnode->np)
	{
		label_t label = (label_t)xnode->ref;
		int _x = label>>24;
		int _y = (label>>16)&0xFF;
	
		if (x==_x && y==_y)
		{
			xnode->ref=(void*)((x<<24)|(y<<16)|code);
		}

	}

	return ;
}

// ----------------------------------
static void SyncToControl(HWND hWnd)
{
	// ノードから動機処理
	char szJosekiName[128];
	int joseki_code=0;
	cSgfStoneInfo* info  = (cSgfStoneInfo*)g_currentNode->GetData();

	InitControlz(hWnd);
	//EnableDisableButtons(hWnd, TRUE);

	if (info==0)
	{
		SendMessage(GetDlgItem(hWnd, IDC_CMB_STONE_ATTR), CB_SETCURSEL, -1, 0);
		SendMessage(GetDlgItem(hWnd, IDC_CHK_OKINIIRI), BM_SETCHECK, 0, 0);
		return ;
	}

	{
		eStoneType type = STONE_TYPE(info->stone);
		if (type==STONE_BLACK)
			SendMessage(GetDlgItem(hWnd, IDC_CMB_STONE_ATTR), CB_SETCURSEL, 0, 0);
		else if (type==STONE_WHITE)
			SendMessage(GetDlgItem(hWnd, IDC_CMB_STONE_ATTR), CB_SETCURSEL, 1, 0);
		else if (type==STONE_BLACK_PASS ||
					type==STONE_WHITE_PASS)
		{
			SendMessage(GetDlgItem(hWnd, IDC_CMB_STONE_ATTR), CB_SETCURSEL, 2, 0);
		}

		if (info->flag32&0x40000000)
		{
			SendMessage(GetDlgItem(hWnd, IDC_CHK_OKINIIRI), BM_SETCHECK, 1, 0);
		}
		else
		{
			SendMessage(GetDlgItem(hWnd, IDC_CHK_OKINIIRI), BM_SETCHECK, 0, 0);
		}
	}

	{
		wchar_t code = xGetLabelCode(g_currentNode->GetParent(), STONE_POS_X(info->stone), STONE_POS_Y(info->stone));

		if (code==0)
		{
			SendMessage(GetDlgItem(hWnd, IDC_CMB_STONE_VALUE), CB_SETCURSEL, -1, 0);
		}
		else
		{
			if ((code>>8)==0xFE)
			{
				int index = (code&0x00FF)-5;
				SendMessage(GetDlgItem(hWnd, IDC_CMB_STONE_VALUE), CB_SETCURSEL, index, 0);
			}
			else
			{
				SendMessage(GetDlgItem(hWnd, IDC_CMB_STONE_VALUE), CB_SETCURSEL, -1, 0);
			}
		}

	}

	

	if (info->comment)
	{
		SetWindowText(GetDlgItem(hWnd, IDC_COMMENT), info->comment);
	}
	else
	{
		SetWindowText(GetDlgItem(hWnd, IDC_COMMENT), 0);
	}

	int h, l, d;
	g_joseki_flag=xGetJosekiName(szJosekiName, g_currentNode, h, l, d);

	SetWindowText(GetDlgItem(hWnd, IDC_LBL_JOSEKI_NAME), szJosekiName);

	if (g_joseki_flag==0)
	{
		if (h==0)
			EnableWindow(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_HIGH), TRUE);

		if (l==0)
			EnableWindow(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_LOW), TRUE);

		if (d==0)
			EnableWindow(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_DETAIL), TRUE);
	}
	else if (g_joseki_flag==-1)
	{
		EnableWindow(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_HIGH), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_LOW), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_DETAIL), TRUE);
	}
	else
	{
		if (d)
			EnableWindow(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_DETAIL), TRUE);
		else if (l)
		{
			EnableWindow(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_DETAIL), TRUE);
			EnableWindow(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_LOW), TRUE);
		}
		else if (h)
		{
			EnableWindow(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_DETAIL), TRUE);
			EnableWindow(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_LOW), TRUE);
			EnableWindow(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_HIGH), TRUE);
		}
	}

	if (g_joseki_flag!=-1)
	{
		SendMessage(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_HIGH), CB_SETCURSEL, h, 0);
		SendMessage(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_LOW), CB_SETCURSEL, l, 0);
		SendMessage(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_DETAIL), CB_SETCURSEL, d, 0);
	}

	if (info->exinfo==0)
	{
		return ;
	}

	g_option=info->exinfo->option;
	g_flag32=info->exinfo->flag32;
	g_status=info->exinfo->status;
	g_comment_code=info->exinfo->comment_code;




	for (int i=0; i<10; i++)
	{
		if (g_status_code[i]==info->exinfo->status)
		{
			SendMessage(GetDlgItem(hWnd, IDC_CMB_STATUS), CB_SETCURSEL, i, 0);
			break;
		}
	}

	if (info->exinfo->option)
	{
		int option=info->exinfo->option;

		if (option&eGRO_HAMETE)
		{
			SendMessage(GetDlgItem(hWnd, IDC_CMB_TYPE), CB_SETCURSEL, 2, 0);
		}
		else if (option&eGRO_JOSEKI)
		{
			SendMessage(GetDlgItem(hWnd, IDC_CMB_TYPE), CB_SETCURSEL, 1, 0);
		}
		else if (option&eGRO_JOSEKI_OTHER)
		{
			SendMessage(GetDlgItem(hWnd, IDC_CMB_TYPE), CB_SETCURSEL, 3, 0);
		}
		else if (option&eGRO_JOSEKI_AFTER)
		{
			SendMessage(GetDlgItem(hWnd, IDC_CMB_TYPE), CB_SETCURSEL, 4, 0);
		}
		else if (option&eGRO_JOSEKI_TENUKI)
		{
			SendMessage(GetDlgItem(hWnd, IDC_CMB_TYPE), CB_SETCURSEL, 5, 0);
		}
		else if (option&eGRO_JOSEKI_CASE)
		{
			SendMessage(GetDlgItem(hWnd, IDC_CMB_TYPE), CB_SETCURSEL, 6, 0);
		}

		if (option&eGRO_NERAI)
		{
			SendMessage(GetDlgItem(hWnd, IDC_CHK_KIKI), BM_SETCHECK, (WPARAM)1, 0L);
		}
		if (option&eGRO_SICHOU)
		{
			SendMessage(GetDlgItem(hWnd, IDC_CHK_SICHO), BM_SETCHECK, (WPARAM)1, 0L);
		}
		if (option&eGRO_SENTE)
		{
			SendMessage(GetDlgItem(hWnd, IDC_CHK_SENTE), BM_SETCHECK, (WPARAM)1, 0L);
		}
		if (option&eGRO_OKIGO)
		{
			SendMessage(GetDlgItem(hWnd, IDC_CHK_OKIGO), BM_SETCHECK, (WPARAM)1, 0L);
		}
		if (option&eGRO_EDIT_COMPRETE)
		{
			SendMessage(GetDlgItem(hWnd, IDC_CHK_EDIT_COMPLETE), BM_SETCHECK, (WPARAM)1, 0L);
		}
	}

	if (info->exinfo->flag32)
	{
		unsigned int flag32 = info->exinfo->flag32;
		int n;

		if ((n=(flag32>>28)))
		{
			SendMessage(GetDlgItem(hWnd, IDC_CMB_HINDO), CB_SETCURSEL, n, 0);			
		}
		if ((n=(flag32>>24)&0x0F))
		{
			SendMessage(GetDlgItem(hWnd, IDC_CMB_NANIDO), CB_SETCURSEL, n, 0);			
		}

		//int joseki_code;
		//n=xGetJosekiName(buffer, g_currentNode, joseki_code);
		//g_joseki_flag=n;

		//SetWindowText(GetDlgItem(hWnd, IDC_LBL_JOSEKI_NAME), szJosekiName);


	}



	InvalidateRect(hWnd, 0, FALSE);
}

// ----------------------------------
static Bool SyncToVariable(HWND hWnd)
{
	if (g_currentNode==0)
		return False;

	g_option=0;
	g_flag32=0;
	g_status=0;
	g_comment_code=0;

	// ステータス設定
	int n = SendMessage(GetDlgItem(hWnd, IDC_CMB_STATUS), CB_GETCURSEL, 0, 0);
	g_status = g_status_code[n];

	n = SendMessage(GetDlgItem(hWnd, IDC_CMB_TYPE), CB_GETCURSEL, 0, 0);

	if (n==1)
		g_option|=eGRO_JOSEKI;
	else if (n==2)
		g_option|=eGRO_HAMETE;
	else if (n==3)
		g_option|=eGRO_JOSEKI_OTHER;
	else if (n==4)
		g_option|=eGRO_JOSEKI_AFTER;
	else if (n==5)
		g_option|=eGRO_JOSEKI_TENUKI;
	else if (n==6)
		g_option|=eGRO_JOSEKI_CASE;



	// ラベル設定
	{
		int n = SendMessage(GetDlgItem(hWnd, IDC_CMB_STONE_VALUE), CB_GETCURSEL, 0, 0);

		cSgfStoneInfo* info = (cSgfStoneInfo* )g_currentNode->GetData();

		if (info)
		{
			eStoneType type = STONE_TYPE(info->stone);

			if (type==STONE_BLACK ||
					type==STONE_WHITE)
			{
				int x=STONE_POS_X(info->stone);
				int y=STONE_POS_Y(info->stone);
				if (n==-1)
				{
					n=0;
				}
				n=(n+5)|0xFE00;
				xSetLabelCode(g_currentNode->GetParent(), x, y, n);
			}
		}
	}


	if(BST_CHECKED == SendMessage(GetDlgItem(hWnd, IDC_CHK_KIKI) , BM_GETCHECK , 0 , 0))
	{
		g_option|=eGRO_NERAI;
	}
	if(BST_CHECKED == SendMessage(GetDlgItem(hWnd, IDC_CHK_SICHO) , BM_GETCHECK , 0 , 0))
	{
		g_option|=eGRO_SICHOU;
	}
	if(BST_CHECKED == SendMessage(GetDlgItem(hWnd, IDC_CHK_SENTE) , BM_GETCHECK , 0 , 0))
	{
		g_option|=eGRO_SENTE;
	}
	if(BST_CHECKED == SendMessage(GetDlgItem(hWnd, IDC_CHK_OKIGO) , BM_GETCHECK , 0 , 0))
	{
		g_option|=eGRO_OKIGO;
	}
	if(BST_CHECKED == SendMessage(GetDlgItem(hWnd, IDC_CHK_EDIT_COMPLETE) , BM_GETCHECK , 0 , 0))
	{
		g_option|=eGRO_EDIT_COMPRETE;
	}

	// 頻度
	if ((n = SendMessage(GetDlgItem(hWnd, IDC_CMB_HINDO), CB_GETCURSEL, 0, 0)))
	{
		g_flag32|=(n<<28);
	}

	// レベル
	if ((n = SendMessage(GetDlgItem(hWnd, IDC_CMB_NANIDO), CB_GETCURSEL, 0, 0)))
	{
		g_flag32|=(n<<24);
	}

	// 定石名
	//if (g_joseki_flag!=0)
	{
		int h=0, l=0, d=0;

		if (IsWindowEnabled(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_HIGH))==TRUE)
			h = SendMessage(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_HIGH), CB_GETCURSEL, 0, 0);

		if (IsWindowEnabled(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_LOW))==TRUE)
			l = SendMessage(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_LOW), CB_GETCURSEL, 0, 0);

		if (IsWindowEnabled(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_DETAIL))==TRUE)
			d = SendMessage(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_DETAIL), CB_GETCURSEL, 0, 0);

		SET_JOSEKI_NAME(g_flag32, h, l, d);
	}


	cSgfStoneInfo* info  = (cSgfStoneInfo*)g_currentNode->GetData();

	if (info==0)
	{
		EnableWindow(GetDlgItem(hWnd, IDC_ADJUST), FALSE);
		return False;
	}


	char szCurrentComment[4096];

	szCurrentComment[0]=0;
	GetWindowText(GetDlgItem(hWnd, IDC_COMMENT), szCurrentComment, 4096);

	if (info->comment)
	{
		if (strcmp(szCurrentComment, info->comment)!=0)
		{
			EnableWindow(GetDlgItem(hWnd, IDC_ADJUST), TRUE);
			return True;
		}
	}
	else
	{
		if (strlen(szCurrentComment)!=0)
		{
			EnableWindow(GetDlgItem(hWnd, IDC_ADJUST), TRUE);
			return True;
		}
	}

	if (info->exinfo==0)
	{
		if (g_flag32 || 
			g_status || 
			g_option || 
			g_comment)
		{
			EnableWindow(GetDlgItem(hWnd, IDC_ADJUST), TRUE);
			return True;
		}

		return False;
	}

	if (info->exinfo->flag32==g_flag32 &&
			info->exinfo->comment_code==g_comment_code &&
				info->exinfo->option==g_option &&
					info->exinfo->status==g_status)
	{
		EnableWindow(GetDlgItem(hWnd, IDC_ADJUST), FALSE);
		return False;
	}

	EnableWindow(GetDlgItem(hWnd, IDC_ADJUST), TRUE);

	return True;
}

// ----------------------------------
static void xOnBackup(void* param)
{
	xOnCommand(IDC_R_SAVE_SGF, -1);
}

// ----------------------------------
static INT_PTR CALLBACK xDlgProc(HWND hWnd, UINT iMsg, WPARAM wp, LPARAM lp)
{
	switch(iMsg)
	{
		// 自動バックアップ&保存 (保存はせず、バックアップファイルを生成する）
		case WM_TIMER:
		{
			_beginthread(xOnBackup, 0, 0);
			break;
		}
		case WM_INITDIALOG:
		{
			g_hMutex = yiMutexCreateEx(0);

			SetTimer(hWnd, 1, 1000*60*5, 0);

			HWND hWndLabel = GetDlgItem(hWnd, IDC_LABEL_SELECT);

			SendMessage(hWndLabel, CB_ADDSTRING, 0, (LPARAM)"ラベル選択なし");
			SendMessage(hWndLabel, CB_ADDSTRING, 1, (LPARAM)"A,B,C...");
			SendMessage(hWndLabel, CB_ADDSTRING, 2, (LPARAM)"a,b,c...");
			SendMessage(hWndLabel, CB_ADDSTRING, 3, (LPARAM)"1,2,3...");
			SendMessage(hWndLabel, CB_ADDSTRING, 4, (LPARAM)"□");
			SendMessage(hWndLabel, CB_ADDSTRING, 5, (LPARAM)"○");
			SendMessage(hWndLabel, CB_ADDSTRING, 6, (LPARAM)"△");
			SendMessage(hWndLabel, CB_ADDSTRING, 7, (LPARAM)"×");


			HWND hWndLabelS = GetDlgItem(hWnd, IDC_CMB_LABEL_SPETIAL);
			SendMessage(hWndLabelS, CB_ADDSTRING, 0, (LPARAM)"ラベル選択なし");
			SendMessage(hWndLabelS, CB_ADDSTRING, 6, (LPARAM)"本手(良く見かける手)");
			SendMessage(hWndLabelS, CB_ADDSTRING, 3, (LPARAM)"いまいちな手（あまり見ない手）");
			SendMessage(hWndLabelS, CB_ADDSTRING, 2, (LPARAM)"悪手（はまり）");
			SendMessage(hWndLabelS, CB_ADDSTRING, 1, (LPARAM)"好手");
			SendMessage(hWndLabelS, CB_ADDSTRING, 4, (LPARAM)"黒の効き・狙い");
			SendMessage(hWndLabelS, CB_ADDSTRING, 5, (LPARAM)"白の効き・狙い");
			SendMessage(hWndLabelS, CB_ADDSTRING, 7, (LPARAM)"はめ手");
			SendMessage(hWndLabelS, CB_ADDSTRING, 8, (LPARAM)"難解な手");
			SendMessage(hWndLabelS, CB_ADDSTRING, 9, (LPARAM)"先手を取る手");
			SendMessage(hWndLabelS, CB_ADDSTRING, 10, (LPARAM)"手筋");
			SendMessage(hWndLabelS, CB_ADDSTRING, 11, (LPARAM)"強手");
			SendMessage(hWndLabelS, CB_ADDSTRING, 12, (LPARAM)"ウソ手");



			SendMessage(hWndLabel, CB_SETCURSEL, 0, 0);
			SendMessage(hWndLabelS, CB_SETCURSEL, 0, 0);

			HWND hWndStoneValue = GetDlgItem(hWnd, IDC_CMB_STONE_VALUE);
			SendMessage(hWndStoneValue, CB_ADDSTRING, 6, (LPARAM)"本手(良く見かける手)");
			SendMessage(hWndStoneValue, CB_ADDSTRING, 3, (LPARAM)"いまいちな手（あまり見ない手）");
			SendMessage(hWndStoneValue, CB_ADDSTRING, 2, (LPARAM)"悪手（はまり）");
			SendMessage(hWndStoneValue, CB_ADDSTRING, 1, (LPARAM)"好手");
			SendMessage(hWndStoneValue, CB_ADDSTRING, 4, (LPARAM)"黒の効き・狙い");
			SendMessage(hWndStoneValue, CB_ADDSTRING, 5, (LPARAM)"白の効き・狙い");
			SendMessage(hWndStoneValue, CB_ADDSTRING, 7, (LPARAM)"はめ手");
			SendMessage(hWndStoneValue, CB_ADDSTRING, 8, (LPARAM)"難解な手");
			SendMessage(hWndStoneValue, CB_ADDSTRING, 9, (LPARAM)"先手を取る手");
			SendMessage(hWndStoneValue, CB_ADDSTRING, 10, (LPARAM)"手筋");
			SendMessage(hWndStoneValue, CB_ADDSTRING, 11, (LPARAM)"強手");
			SendMessage(hWndStoneValue, CB_ADDSTRING, 12, (LPARAM)"ウソ手");

			SendMessage(hWndStoneValue, CB_SETCURSEL, -1, 0);

			

			HWND hWndHindo = GetDlgItem(hWnd, IDC_CMB_HINDO);
			SendMessage(hWndHindo, CB_ADDSTRING, 0, (LPARAM)"頻度");
			SendMessage(hWndHindo, CB_ADDSTRING, 1, (LPARAM)"★");
			SendMessage(hWndHindo, CB_ADDSTRING, 2, (LPARAM)"★★");
			SendMessage(hWndHindo, CB_ADDSTRING, 3, (LPARAM)"★★★");
			SendMessage(hWndHindo, CB_ADDSTRING, 4, (LPARAM)"★★★★");
			SendMessage(hWndHindo, CB_ADDSTRING, 5, (LPARAM)"★★★★★");
			SendMessage(hWndHindo, CB_SETCURSEL, 0, 0);

			HWND hWndLevel = GetDlgItem(hWnd, IDC_CMB_NANIDO);
			SendMessage(hWndLevel, CB_ADDSTRING, 0, (LPARAM)"難易度");
			SendMessage(hWndLevel, CB_ADDSTRING, 1, (LPARAM)"★");
			SendMessage(hWndLevel, CB_ADDSTRING, 2, (LPARAM)"★★");
			SendMessage(hWndLevel, CB_ADDSTRING, 3, (LPARAM)"★★★");
			SendMessage(hWndLevel, CB_ADDSTRING, 4, (LPARAM)"★★★★");
			SendMessage(hWndLevel, CB_ADDSTRING, 5, (LPARAM)"★★★★★");
			SendMessage(hWndLevel, CB_SETCURSEL, 0, 0);

			HWND hWndStatus = GetDlgItem(hWnd, IDC_CMB_STATUS);
			SendMessage(hWndStatus, CB_ADDSTRING, 0, (LPARAM)"評価なし");
			SendMessage(hWndStatus, CB_ADDSTRING, 1, (LPARAM)"互角");
			SendMessage(hWndStatus, CB_ADDSTRING, 2, (LPARAM)"黒優勢");
			SendMessage(hWndStatus, CB_ADDSTRING, 3, (LPARAM)"白優勢");
			SendMessage(hWndStatus, CB_ADDSTRING, 4, (LPARAM)"黒やや有利");
			SendMessage(hWndStatus, CB_ADDSTRING, 6, (LPARAM)"白やや有利");
			SendMessage(hWndStatus, CB_ADDSTRING, 7, (LPARAM)"黒つぶれ");
			SendMessage(hWndStatus, CB_ADDSTRING, 8, (LPARAM)"白つぶれ");
			SendMessage(hWndStatus, CB_ADDSTRING, 9, (LPARAM)"状況次第");
			SendMessage(hWndStatus, CB_SETCURSEL, 0, 0);


			HWND hWndType = GetDlgItem(hWnd, IDC_CMB_TYPE);
			SendMessage(hWndType, CB_ADDSTRING, 0, (LPARAM)"選択なし");
			SendMessage(hWndType, CB_ADDSTRING, 1, (LPARAM)"定石");
			SendMessage(hWndType, CB_ADDSTRING, 2, (LPARAM)"はめ手");
			SendMessage(hWndType, CB_ADDSTRING, 3, (LPARAM)"変化");
			SendMessage(hWndType, CB_ADDSTRING, 4, (LPARAM)"定石後の変化");
			SendMessage(hWndType, CB_ADDSTRING, 5, (LPARAM)"手抜き定石");
			SendMessage(hWndType, CB_ADDSTRING, 6, (LPARAM)"場合の手");



			SendMessage(hWndType, CB_SETCURSEL, 0, 0);

			HWND hWndSubJect = GetDlgItem(hWnd, IDC_CMB_SUBJECT);
			SendMessage(hWndSubJect, CB_ADDSTRING, 0, (LPARAM)"黒");
			SendMessage(hWndSubJect, CB_ADDSTRING, 1, (LPARAM)"白");
			SendMessage(hWndSubJect, CB_ADDSTRING, 2, (LPARAM)"A");
			SendMessage(hWndSubJect, CB_ADDSTRING, 3, (LPARAM)"B");

			SendMessage(hWndSubJect, CB_SETCURSEL, -1, 0);

			HWND hAttribute = GetDlgItem(hWnd, IDC_CMB_STONE_ATTR);

			SendMessage(hAttribute, CB_ADDSTRING, 0, (LPARAM)"黒");
			SendMessage(hAttribute, CB_ADDSTRING, 0, (LPARAM)"白");
			SendMessage(hAttribute, CB_ADDSTRING, 0, (LPARAM)"パス");

			SendMessage(hAttribute, CB_SETCURSEL, -1, 0);

			int cnt=0;
			while(g_pszJosekiNameHigh[cnt])
			{
				SendMessage(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_HIGH), CB_ADDSTRING, 0, (LONG)g_pszJosekiNameHigh[cnt++]);
			}
			SendMessage(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_HIGH), CB_SETCURSEL, 0, 0);

			cnt=0;
			while(g_pszJosekiNameHigh[cnt])
			{
				SendMessage(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_LOW), CB_ADDSTRING, 0, (LONG)g_pszJosekiNameMiddle[cnt++]);
			}
			SendMessage(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_LOW), CB_SETCURSEL, 0, 0);

			cnt=0;
			while(g_pszJosekiNameHigh[cnt])
			{
				SendMessage(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_DETAIL), CB_ADDSTRING, 0, (LONG)g_pszJosekiNameDetail[cnt++]);
			}
			SendMessage(GetDlgItem(hWnd, IDC_CMB_JOSEKI_NAME_DETAIL), CB_SETCURSEL, 0, 0);

			InitControlz(hWnd);
			EnableDisableButtons(hWnd);
			return 1;
		}
		case WM_NOTIFY_NODE_CHANGED:
		{
			if (wp==0)
			{
				if (SyncToVariable(hWnd)==True)
				{
					SendMessage(hWnd, WM_COMMAND, IDC_ADJUST, (LPARAM)GetDlgItem(hWnd, IDC_ADJUST));
				}
				
			}

			g_currentNode = (cyiTreeNode*)lp;
			SyncToControl(hWnd);
			EnableDisableButtons(hWnd);

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
			xOnCommand(wp, lp);
			EnableDisableButtons(hWnd);
			SyncToVariable(hWnd);
			return 1;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 1;
		}
		case WM_CANCELMODE:
		case WM_ENABLE:
			return 0;
		default:
			break;
	}
	//EnableDisableButtons(hWnd);
	//SyncToVariable(hWnd);

	return 0;
}

// ----------------------------------
HWND xCreateJosekiControl(HWND hWnd, int x, int y)
{
	//unsigned int dwThreadId;

	//xOnRemoconLoop((void*)hWnd);
	//HANDLE hThread = (HANDLE)_beginthreadex(0, 0, xOnRemoconLoop, 	(void*)hWnd, 0, &dwThreadId);
	//CloseHandle(hThread);

	HWND hNotifyWnd=hWnd;

	HWND hRemocon = CreateDialogParamA((HINSTANCE)GetWindowLong(hNotifyWnd, GWL_HINSTANCE), MAKEINTRESOURCEA(IDD_JOSEKI_DIALOG), hWnd, xDlgProc, (LPARAM)hNotifyWnd);

	RECT rect;
	RECT rect2;
	GetClientRect(hRemocon, &rect);
	GetClientRect(hNotifyWnd, &rect2);
	SetWindowPos(hRemocon, 0, x, y,0,0, SWP_NOSIZE|SWP_NOZORDER);
	ShowWindow(hRemocon, SW_SHOW);


	SetFocus(hNotifyWnd);

	return hRemocon;
}