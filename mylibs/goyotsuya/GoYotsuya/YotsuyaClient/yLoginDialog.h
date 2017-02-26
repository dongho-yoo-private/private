#pragma once
#include "afxwin.h"


// cyLoginDialog ダイアログ

class cyLoginDialog : public CDialog
{
	DECLARE_DYNAMIC(cyLoginDialog)

public:
	cyLoginDialog(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~cyLoginDialog();

	BOOL m_bFoundUser;

// ダイアログ データ
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	virtual BOOL OnInitDialog();


	void ShowMode(BOOL bIsNoId);

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bIsNormalLogin;
	CString m_txtId;
	CString m_txtPassword;
	CString m_lblId;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioNormal();
	afx_msg void OnBnClickedRadioNoId();
	afx_msg void OnBnClickedCancel();
	CString m_strSelectedId;
	CComboBox m_cmbIdList;
	CEdit m_edit_id;
	CEdit m_edit_password;
	CStatic m_lblName;
	//CStatic m_lblID;
	CEdit m_txtRoomPassword;
	CString m_strRoomPassword;

	char m_szPassword[8];
	char m_szName[16];
	CStatic m_lblIDx;
	afx_msg void OnEnChangeEditPassword();
};
