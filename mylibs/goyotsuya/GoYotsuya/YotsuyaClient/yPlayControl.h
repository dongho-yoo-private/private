#pragma once
#include "afxwin.h"
#include "ypnGoNetwork.h"


// cyPlayControl ダイアログ

class cyPlayControl : public CDialog
{
	DECLARE_DYNAMIC(cyPlayControl)

public:
	cyPlayControl(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~cyPlayControl();

// ダイアログ データ
	enum { IDD = IDD_PLAY_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CButton m_btnCalculateEnd;
	CButton m_btnCancelPlay;
	CButton m_btnAbortPlay;
	CButton m_btnWaitPlay;
	CButton m_btnReqQuit;
	CButton m_btnPass;
	CButton m_btnLost;
	afx_msg void OnBnClickedBtnGiveup();
	afx_msg void OnBnClickedBtnPass();
	afx_msg void OnBnClickedBtnPlayEnd();
	afx_msg void OnBnClickedBtnPlayWait();
	afx_msg void OnBnClickedBtnPlayAbort();
	afx_msg void OnBnClickedBtnPlayCancel2();
	afx_msg void OnBnClickedBtnCalculateEnd();

	cypnGoNetwork* m_network;
	afx_msg void OnPaint(void);
};
