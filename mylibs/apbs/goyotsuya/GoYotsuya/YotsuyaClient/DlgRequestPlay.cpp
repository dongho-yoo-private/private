// DlgRequestPlay.cpp : 実装ファイル
//

#include "stdafx.h"
#include "YotsuyaClient.h"
#include "DlgRequestPlay.h"


// CDlgRequestPlay ダイアログ

IMPLEMENT_DYNAMIC(CDlgRequestPlay, CDialog)

CDlgRequestPlay::CDlgRequestPlay(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRequestPlay::IDD, pParent)
	, m_WhatStone(0)
	, m_bReverseKomi(FALSE)
	, m_bLimitTime(FALSE)
	, m_bSecondRead(FALSE)
	, m_bIsWatable(FALSE)
	, m_bAbortable(FALSE)
	, m_InputUserName(FALSE)
	, m_strRoomNo(_T(""))
	, bIsRecvMode(FALSE)
	, m_strRoomPassword(_T(""))
	, m_strRoomPassword2(_T(""))
	, m_strSelectedUser(_T(""))
	, m_RadioRoomNo(0)
{

}

CDlgRequestPlay::~CDlgRequestPlay()
{
}

void CDlgRequestPlay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_IAM_WHITE, m_WhatStone);
	DDX_Control(pDX, IDC_HANDICAPS, m_HandicapList);
	DDX_Control(pDX, IDC_KOMI_LIST, m_Komi);
	DDX_Check(pDX, IDC_BOOL_REVERSE_KOMI, m_bReverseKomi);
	DDX_Check(pDX, IDC_BOOL_LIMIT_TIME, m_bLimitTime);
	DDX_Check(pDX, IDC_CHECK5, m_bSecondRead);
	DDX_Check(pDX, IDC_BOOL_WAITABLE, m_bIsWatable);
	DDX_Check(pDX, IDC_BOOL_ABORTABLE, m_bAbortable);
	DDX_Control(pDX, IDC_LIMIT_TIME_LIST, m_bLimitedTimeList);
	DDX_Control(pDX, IDC_LIMITED_MINITE_LIST, m_LimitedTimeMinute);
	DDX_Control(pDX, IDC_MINITE_READ, m_SecondReadList);
	DDX_Control(pDX, IDC_SECOND_READ_COUNT, m_SecondReadCount);
	DDX_Control(pDX, IDC_ROOM_NUMBER, m_txtRoomNo);
	DDX_Control(pDX, IDC_PLAYER_NAME, m_UserNameList);
	DDX_Radio(pDX, IDC_INPUT_USER_NAME, m_InputUserName);
	DDX_Control(pDX, IDC_ROOM_PASSWORD, m_txtRoomPassword);
	DDX_Control(pDX, IDC_STATIC_ROOM_NO, m_static_RoomNo);
	//DDX_Control(pDX, IDC_STATIC_USER_NAME, m_static_UserName);
	//DDX_Control(pDX, IDC_MY_NAME_LIST, m_MyNameList);
	DDX_Control(pDX, IDC_STATIC_PASSWORD, m_static_password);
	DDX_Text(pDX, IDC_ROOM_NUMBER, m_strRoomNo);
	DDX_Text(pDX, IDC_ROOM_PASSWORD, m_strRoomPassword);
	//DDX_Control(pDX, IDC_STATIC_PASSWORD2, m_lbl_room_password);
	//DDX_Control(pDX, IDC_ROOM_PASSWORD2, m_txtRoomPassword2);
	//DDX_Text(pDX, IDC_ROOM_PASSWORD2, m_strRoomPassword2);
	DDX_CBString(pDX, IDC_PLAYER_NAME, m_strSelectedUser);
	DDX_Control(pDX, IDC_CHANGE_CONDITION, m_ChangeCondition);
	DDX_Control(pDX, IDC_STATIC_USER_NAME, m_static_username);
	DDX_Control(pDX, IDC_INPUT_USER_NAME, m_RadioName);
}


BEGIN_MESSAGE_MAP(CDlgRequestPlay, CDialog)
	ON_CBN_SELCHANGE(IDC_PLAYER_NAME, &CDlgRequestPlay::OnCbnSelchangePlayerName)
	ON_EN_CHANGE(IDC_ROOM_NUMBER, &CDlgRequestPlay::OnEnChangeRoomNumber)
	ON_EN_CHANGE(IDC_ROOM_PASSWORD, &CDlgRequestPlay::OnEnChangeRoomPassword)
	ON_BN_CLICKED(IDC_IAM_WHITE, &CDlgRequestPlay::OnBnClickedIamWhite)
	ON_BN_CLICKED(IDC_IAM_BLACK, &CDlgRequestPlay::OnBnClickedIamBlack)
	ON_BN_CLICKED(IDC_IAM_UNKNOWN, &CDlgRequestPlay::OnBnClickedIamUnknown)
	ON_BN_CLICKED(IDC_BOOL_REVERSE_KOMI, &CDlgRequestPlay::OnBnClickedBoolReverseKomi)
	ON_BN_CLICKED(IDC_BOOL_LIMIT_TIME, &CDlgRequestPlay::OnBnClickedBoolLimitTime)
	ON_BN_CLICKED(IDC_CHECK5, &CDlgRequestPlay::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_BOOL_WAITABLE, &CDlgRequestPlay::OnBnClickedBoolWaitable)
	ON_BN_CLICKED(IDC_BOOL_ABORTABLE, &CDlgRequestPlay::OnBnClickedBoolAbortable)
	ON_CBN_SELCHANGE(IDC_LIMIT_TIME_LIST, &CDlgRequestPlay::OnCbnSelchangeLimitTimeList)
	ON_CBN_SELCHANGE(IDC_MINITE_READ, &CDlgRequestPlay::OnCbnSelchangeMiniteRead)
	ON_CBN_SELCHANGE(IDC_LIMITED_MINITE_LIST, &CDlgRequestPlay::OnCbnSelchangeLimitedMiniteList)
	ON_CBN_SELCHANGE(IDC_SECOND_READ_COUNT, &CDlgRequestPlay::OnCbnSelchangeSecondReadCount)
	ON_BN_CLICKED(IDOK, &CDlgRequestPlay::OnBnClickedOk)
	ON_BN_CLICKED(IDC_INPUT_USER_NAME, &CDlgRequestPlay::OnBnClickedInputUserName)
	ON_BN_CLICKED(IDC_INPUT_ROOM_NO, &CDlgRequestPlay::OnBnClickedInputRoomNo)
	ON_BN_CLICKED(IDC_CHANGE_CONDITION, &CDlgRequestPlay::OnBnClickedChangeCondition)
	ON_CBN_EDITCHANGE(IDC_PLAYER_NAME, &CDlgRequestPlay::OnCbnEditchangePlayerName)
	ON_CBN_SELCHANGE(IDC_HANDICAPS, &CDlgRequestPlay::OnCbnSelchangeHandicaps)
	ON_CBN_SELCHANGE(IDC_KOMI_LIST, &CDlgRequestPlay::OnCbnSelchangeKomiList)
END_MESSAGE_MAP()


// CDlgRequestPlay メッセージ ハンドラ

void CDlgRequestPlay::OnCbnSelchangePlayerName()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	//m_lbl_room_password.EnableWindow(FALSE);
	//m_txtRoomPassword.EnableWindow(FALSE);
}

void CDlgRequestPlay::OnEnChangeRoomNumber()
{
	// TODO:  これが RICHEDIT コントロールの場合、
	// まず、CDialog::OnInitDialog() 関数をオーバーライドして、OR 状態の ENM_CHANGE
	// フラグをマスクに入れて、CRichEditCtrl().SetEventMask() を呼び出さない限り、
	// コントロールは、この通知を送信しません。

	// TODO:  ここにコントロール通知ハンドラ コードを追加してください。
}

void CDlgRequestPlay::OnEnChangeRoomPassword()
{
	// TODO:  これが RICHEDIT コントロールの場合、
	// まず、CDialog::OnInitDialog() 関数をオーバーライドして、OR 状態の ENM_CHANGE
	// フラグをマスクに入れて、CRichEditCtrl().SetEventMask() を呼び出さない限り、
	// コントロールは、この通知を送信しません。

	// TODO:  ここにコントロール通知ハンドラ コードを追加してください。
}

void CDlgRequestPlay::OnBnClickedIamWhite()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	DisableControls();
}

void CDlgRequestPlay::OnBnClickedIamBlack()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	DisableControls();
}

void CDlgRequestPlay::OnBnClickedIamUnknown()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	DisableControls();
}

void CDlgRequestPlay::OnBnClickedBoolReverseKomi()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	DisableControls();
}

void CDlgRequestPlay::OnBnClickedBoolLimitTime()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	DisableControls();
}

void CDlgRequestPlay::OnBnClickedCheck5()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	DisableControls();
}

void CDlgRequestPlay::OnBnClickedBoolWaitable()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	DisableControls();
}

void CDlgRequestPlay::OnBnClickedBoolAbortable()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	DisableControls();
}

void CDlgRequestPlay::OnCbnSelchangeLimitTimeList()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	DisableControls();
}

void CDlgRequestPlay::OnCbnSelchangeMiniteRead()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	DisableControls();
}

void CDlgRequestPlay::OnCbnSelchangeLimitedMiniteList()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	DisableControls();
}

void CDlgRequestPlay::OnCbnSelchangeSecondReadCount()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	DisableControls();
}

#include "ypn.h"
#include "ypnGoNetwork.h"

// -----------------------------------
void CDlgRequestPlay::UpdateCondition()
{

	unsigned int condition=0;

	if (m_WhatStone==0)
	{
		SET_IAM_BLACK(condition);
		SET_HANDICAP(condition);
	}
	else if (m_WhatStone==1)
	{
		SET_HANDICAP(condition);
	}

	if (m_WhatStone!=2)
	{
		SET_INIT_STONE(condition, m_HandicapList.GetCurSel());
	}

	int n = m_Komi.GetCurSel();

	SET_KOMI(condition, n>>1);

	if (n%2)
		SET_HALF_SCORE(condition);
	
	if (m_WhatStone!=2 && 
		m_bReverseKomi==TRUE)
	{
		SET_REVERSE_KOMI(condition);
	}

	if (m_bLimitTime==TRUE)
	{
		int n = (m_bLimitedTimeList.GetCurSel()*60);
		n+=m_LimitedTimeMinute.GetCurSel()*5;
		SET_PLAY_TIME(condition, n);
	}
	else 
	{
		SET_PLAY_TIME(condition, 0x7F);
	}

	if (m_bSecondRead==TRUE)
	{
		SET_SECOND(condition, (m_SecondReadList.GetCurSel()+1));
		SET_SECOND_COUNT(condition, m_SecondReadCount.GetCurSel()+1);
	}

	m_condition1=condition;
}

void CDlgRequestPlay::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	m_condition1=0;
	UpdateData();
	UpdateCondition();

	const char* p = m_strRoomNo;
	m_nRoomNo = ::atoi(p);



	//SET_INIT_STONE(m_condition1, 5);
	//SET_KOMI(m_condition1, 0);
	//SET_HALF_SCORE(m_condition1);
	//SET_REVERSE_KOMI(m_condition1);
	//SET_PLAY_TIME(m_condition1, 5*18);
	//SET_SECOND(m_condition1, 1);
	//SET_SECOND_COUNT(m_condition1, 5);
	OnOK();
}

static void SetChecked(HWND hWnd, UINT id, BOOL bIsChecked)
{
	//Button_SetCheck(GetDlgItem(hWnd, id), bIsChecked ? BST_CHECKED:BST_UNCHECKED);
	((void)::SendMessage(GetDlgItem(hWnd, id), BM_SETCHECK, (WPARAM)(int)( bIsChecked ? BST_CHECKED:BST_UNCHECKED), 0L));

}


static BOOL GetChecked(HWND hWnd, UINT id)
{
	return (((int)(DWORD)::SendMessage(GetDlgItem(hWnd, id), BM_GETCHECK, 0L, 0L)))==BST_CHECKED ? TRUE:FALSE;
}

void CDlgRequestPlay::SetDefaultValue(stone_t param1, stone_t param2, const char* pszUserName)
{
	m_condition1=param1;
	m_condition2=param2;
	m_condition1_org=param1;
	m_condition2_org=param2;

	if (pszUserName!=0)
	{
		//char buffer[256];
		wsprintf(m_szTitle, "%sさんからの対局申し込み", pszUserName);
		m_bIsFirstRequest=FALSE;
		//SetWindowText(buffer);
		//m_ChangeCondition.ShowWindow(SW_SHOW);
	}
	else
	{
		strcpy(m_szTitle, "対局申し込み");
		m_bIsFirstRequest=TRUE;
	}
	m_bChangedCondition=FALSE;
}



BOOL CDlgRequestPlay::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->m_Komi.AddString("なし");
	this->m_Komi.AddString("半目");

	for (int i=1; i<127; i++)
	{
		char str[16];
		wsprintfA(str, "%d目", i);
		this->m_Komi.AddString(str);
		wsprintfA(str, "%d目半", i);
		this->m_Komi.AddString(str);
	}

	// TODO:  ここに初期化を追加してください
	//SetDefaultValue();

	if (m_condition1==0 && m_condition2==0)
	{
		m_WhatStone= 2;

		m_bLimitTime=TRUE;
		m_HandicapList.SetCurSel(0);
		m_Komi.SetCurSel((6<<1)+1);
		m_bLimitedTimeList.SetCurSel(0);
		m_LimitedTimeMinute.SetCurSel(20/5);
		m_SecondReadList.SetCurSel(0);
		m_SecondReadCount.SetCurSel(3);
		m_bSecondRead=TRUE;

	}
	else
	{
		int nHandi = 0;
		if (IS_NIGIRI(m_condition1)==TRUE)
		{
			m_WhatStone=2;
		}
		else
		{
			if (IAM_BLACK(m_condition1)==TRUE)
			{
				m_WhatStone=0;
			}
			else
			{
				m_WhatStone=1;
			}
			nHandi = GET_INIT_STONE(m_condition1);
		}
		m_HandicapList.SetCurSel(nHandi);

		int nKomi = GET_KOMI(m_condition1);
		BOOL bIsHalfScore = HALF_SCORE(m_condition1);

		nKomi = (nKomi<<1)+ bIsHalfScore;

		m_Komi.SetCurSel(nKomi);


		this->m_bReverseKomi = REVERSE_KOMI(m_condition1);

		int n = GET_PLAY_TIME(m_condition1);

		if (n==0x7F)
		{
			this->m_bLimitTime=FALSE;
		}
		else
		{
			n = n*5;
			this->m_bLimitTime=TRUE;
			this->m_bLimitedTimeList.SetCurSel(n/60);
			this->m_LimitedTimeMinute.SetCurSel((n%60)/5);
		}

		n = GET_SECOND(m_condition1);

		if (n==0)
		{
			this->m_bSecondRead=FALSE;
		}
		else
		{
			this->m_bSecondRead=TRUE;
			m_SecondReadList.SetCurSel(n-1);
			m_SecondReadCount.SetCurSel(GET_SECOND_COUNT((m_condition1-1)));
		}


	}
	
	m_InputUserName=TRUE;

	if (m_bIsFirstRequest==TRUE)
	{
		m_ChangeCondition.ShowWindow(SW_HIDE);
		m_RadioName.EnableWindow(TRUE);
		GetDlgItem(IDC_INPUT_ROOM_NO)->EnableWindow(TRUE);

		//m_RadioRoomNo.EnableWindow(TRUE);

	}
	else
	{
		m_ChangeCondition.ShowWindow(SW_SHOW);
		m_RadioName.EnableWindow(FALSE);
		GetDlgItem(IDC_INPUT_ROOM_NO)->EnableWindow(FALSE);
		//m_RadioRoomNo.EnableWindow(FALSE);
	}
	SetWindowText(m_szTitle);

	UpdateData(FALSE);
	m_txtRoomNo.SetFocus();
	m_txtRoomNo.ShowCaret();
	//m_lbl_room_password.EnableWindow(FALSE);
	//m_txtRoomPassword.EnableWindow(FALSE);

	if (bIsRecvMode==TRUE)
	{
		m_txtRoomNo.EnableWindow(FALSE);
		m_txtRoomPassword.EnableWindow(FALSE);
	}

	DisableControls();

	//this->m_MyNameList.SetFocus();
	//this->m_MyNameList.ShowCaret();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//
void CDlgRequestPlay::DisableControls()
{
	UpdateData();
	if (m_WhatStone==2)
	{
		m_HandicapList.SetCurSel(0);
		m_HandicapList.EnableWindow(FALSE);
		this->m_bReverseKomi=FALSE;

	}
	else
	{
		m_HandicapList.EnableWindow(TRUE);
	}

	if (m_bLimitTime==FALSE)
	{
		m_bLimitedTimeList.EnableWindow(FALSE);
		m_LimitedTimeMinute.EnableWindow(FALSE);
	}
	else
	{
		m_bLimitedTimeList.EnableWindow();
		m_LimitedTimeMinute.EnableWindow();
	}

	if (m_bSecondRead==FALSE)
	{
		m_SecondReadList.EnableWindow(FALSE);
		m_SecondReadCount.EnableWindow(FALSE);
	}
	else
	{
		m_SecondReadList.EnableWindow();
		m_SecondReadCount.EnableWindow();
	}

	if (m_InputUserName==FALSE)
	{
		m_static_RoomNo.ShowWindow(SW_HIDE);
		m_txtRoomNo.ShowWindow(SW_HIDE);
		m_static_password.ShowWindow(SW_HIDE);
		m_txtRoomPassword.ShowWindow(SW_HIDE);
		m_static_username.ShowWindow(SW_SHOW);
		//m_static_UserName.ShowWindow(SW_SHOW);
		m_UserNameList.ShowWindow(SW_SHOW);
		//m_txtRoomPassword2.ShowWindow(SW_SHOW);
		//m_lbl_room_password.ShowWindow(SW_SHOW);
		//m_UserNameList.SetFocus();
		//m_UserNameList.ShowCaret();
	}
	else
	{
		m_static_RoomNo.ShowWindow(SW_SHOW);
		m_txtRoomNo.ShowWindow(SW_SHOW);
		//m_static_UserName.ShowWindow(SW_HIDE);
		m_static_username.ShowWindow(SW_HIDE);
		m_UserNameList.ShowWindow(SW_HIDE);
		//m_txtRoomPassword2.ShowWindow(SW_HIDE);
		m_static_password.ShowWindow(SW_SHOW);
		m_txtRoomPassword.ShowWindow(SW_SHOW);
		//m_lbl_room_password.ShowWindow(SW_HIDE);
		//m_txtRoomNo.SetFocus();
		//m_txtRoomNo.ShowCaret();
		m_txtRoomPassword.EnableWindow(TRUE);
	}

	UpdateCondition();
	if (m_condition1==m_condition1_org)
	{
		m_ChangeCondition.EnableWindow(FALSE);
		//m_ChangeCondition.ShowWindow(SW_SHOW);
	}
	else
	{
		m_ChangeCondition.EnableWindow(TRUE);
		//m_ChangeCondition.ShowWindow(SW_SHOW);
	}
	//UpdateData(FALSE);
	UpdateWindow();
}

void CDlgRequestPlay::OnBnClickedInputUserName()
{
	DisableControls();
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
}

void CDlgRequestPlay::OnBnClickedInputRoomNo()
{
	DisableControls();
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
}

void CDlgRequestPlay::OnBnClickedChangeCondition()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_bChangedCondition=TRUE;
	OnBnClickedOk();

}

void CDlgRequestPlay::OnCbnEditchangePlayerName()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	//m_lbl_room_password.EnableWindow(TRUE);
	m_txtRoomPassword.EnableWindow(TRUE);
}

void CDlgRequestPlay::OnCbnSelchangeHandicaps()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	DisableControls();
}

void CDlgRequestPlay::OnCbnSelchangeKomiList()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	DisableControls();
}
