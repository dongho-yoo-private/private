// yLoginDialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "YotsuyaClient.h"
#include "yLoginDialog.h"


// cyLoginDialog ダイアログ

IMPLEMENT_DYNAMIC(cyLoginDialog, CDialog)

cyLoginDialog::cyLoginDialog(CWnd* pParent /*=NULL*/)
	: CDialog(cyLoginDialog::IDD, pParent)
	, m_bIsNormalLogin(FALSE)
	, m_txtId(_T(""))
	, m_txtPassword(_T(""))
	, m_lblId(_T(""))
	, m_strSelectedId(_T(""))
	, m_strRoomPassword(_T(""))
{

}

cyLoginDialog::~cyLoginDialog()
{
}

void cyLoginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_NORMAL, m_bIsNormalLogin);
	DDX_Text(pDX, IDC_EDIT_ID, m_txtId);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_txtPassword);
	/*DDX_Text(pDX, IDC_LABEL_ID, m_lblId);*/
	DDX_CBString(pDX, IDC_CMB_IDLIST, m_strSelectedId);
	DDX_Control(pDX, IDC_CMB_IDLIST, m_cmbIdList);
	DDX_Control(pDX, IDC_EDIT_ID, m_edit_id);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_edit_password);
	DDX_Control(pDX, IDC_LABEL_ID2, m_lblName);
	//DDX_Control(pDX, IDC_LABEL_ID, m_lblID);
	DDX_Control(pDX, IDC_EDIT_ROOM_PASSWORD, m_txtRoomPassword);
	DDX_Text(pDX, IDC_EDIT_ROOM_PASSWORD, m_strRoomPassword);
	DDX_Control(pDX, IDC_STATIC_ID, m_lblIDx);
}


BEGIN_MESSAGE_MAP(cyLoginDialog, CDialog)
	ON_BN_CLICKED(IDOK, &cyLoginDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_NORMAL, &cyLoginDialog::OnBnClickedRadioNormal)
	ON_BN_CLICKED(IDC_RADIO_NO_ID, &cyLoginDialog::OnBnClickedRadioNoId)
	ON_BN_CLICKED(IDCANCEL, &cyLoginDialog::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT_PASSWORD, &cyLoginDialog::OnEnChangeEditPassword)
END_MESSAGE_MAP()


// cyLoginDialog メッセージ ハンドラ

void cyLoginDialog::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	UpdateData();
	if (m_bIsNormalLogin==1)
	{
		strcpy(&m_szName[1], m_txtId);
		m_szName[0]=0xFF;
		strcpy(m_szPassword, m_strRoomPassword);
	}
	else
	{
		strcpy(m_szName, m_strSelectedId);

		strcpy(m_szPassword, m_txtPassword);
	}

	OnOK();
}

void cyLoginDialog::OnBnClickedRadioNormal()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	ShowMode(FALSE);
}

void cyLoginDialog::OnBnClickedRadioNoId()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	//m_bIsNormalLogin=TRUE;
	ShowMode(TRUE);
}

void cyLoginDialog::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OnCancel();
}

void cyLoginDialog::ShowMode(BOOL bIsNoId) 
{
	int sw = bIsNoId ? SW_SHOW:SW_HIDE;
	int hd = bIsNoId ? SW_HIDE:SW_SHOW;

	m_bIsNormalLogin=bIsNoId?1:0;
	m_cmbIdList.ShowWindow(hd);
	m_lblIDx.ShowWindow(hd);
	m_lblName.ShowWindow(sw);
	m_edit_id.ShowWindow(sw);

	m_edit_password.ShowWindow(hd);
	m_txtRoomPassword.ShowWindow(sw);

	if (bIsNoId)
	{
		m_edit_id.SetFocus();
		m_edit_id.ShowCaret();

	}
	else
	{
		m_cmbIdList.SetFocus();
		m_cmbIdList.ShowCaret();
	}
}

BOOL cyLoginDialog::OnInitDialog() 
{
   CDialog::OnInitDialog();
   
   if (m_bFoundUser==FALSE)
   {
		ShowMode(TRUE);
   }
   else
   {
	   ShowMode(FALSE);
   }

   UpdateData(FALSE);


   return TRUE;   // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
void cyLoginDialog::OnEnChangeEditPassword()
{
	// TODO:  これが RICHEDIT コントロールの場合、
	// まず、CDialog::OnInitDialog() 関数をオーバーライドして、OR 状態の ENM_CHANGE
	// フラグをマスクに入れて、CRichEditCtrl().SetEventMask() を呼び出さない限り、
	// コントロールは、この通知を送信しません。

	// TODO:  ここにコントロール通知ハンドラ コードを追加してください。

}
