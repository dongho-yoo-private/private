#pragma once

#include "yi.h"
#include "afxwin.h"

// CDlgRequestPlay ダイアログ

class CDlgRequestPlay : public CDialog
{
	DECLARE_DYNAMIC(CDlgRequestPlay)

public:
	CDlgRequestPlay(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDlgRequestPlay();

// ダイアログ データ
	enum { IDD = IDD_DLG_REQUEST_PLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangePlayerName();
	afx_msg void OnEnChangeRoomNumber();
	afx_msg void OnEnChangeRoomPassword();
	afx_msg void OnBnClickedIamWhite();
	afx_msg void OnBnClickedIamBlack();
	afx_msg void OnBnClickedIamUnknown();
	afx_msg void OnBnClickedBoolReverseKomi();
	afx_msg void OnBnClickedBoolLimitTime();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedBoolWaitable();
	afx_msg void OnBnClickedBoolAbortable();
	afx_msg void OnCbnSelchangeLimitTimeList();
	afx_msg void OnCbnSelchangeMiniteRead();
	afx_msg void OnCbnSelchangeLimitedMiniteList();
	afx_msg void OnCbnSelchangeSecondReadCount();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
public:
	void SetDefaultValue(stone_t n1, stone_t n2, const char* pszUserName=0);
	stone_t GetCondition(stone_t* conditionex=0);
public:
	int m_WhatStone;
	stone_t m_condition1;
	stone_t m_condition2;

private:
	stone_t m_condition1_org;
	stone_t m_condition2_org;
	void DisableControls();
public:
	LRESULT OnNotifyFromNetwork(WPARAM wp, LPARAM lp);

public:
	CComboBox m_HandicapList;
	CComboBox m_Komi;
	BOOL bIsRecvMode;
	BOOL m_bReverseKomi;
	BOOL m_bLimitTime;
	BOOL m_bSecondRead;
	BOOL m_bIsWatable;
	BOOL m_bAbortable;
	BOOL m_bChangedCondition;
	CComboBox m_bLimitedTimeList;
	CComboBox m_LimitedTimeMinute;
	CComboBox m_SecondReadList;
	CComboBox m_SecondReadCount;
	CEdit m_txtRoomNo;
	CComboBox m_UserNameList;
	BOOL m_InputUserName;
	CEdit m_txtRoomPassword;
	CStatic m_static_RoomNo;
	CStatic m_static_UserName;
	afx_msg void OnBnClickedInputUserName();
	afx_msg void OnBnClickedInputRoomNo();
	CComboBox m_MyNameList;
	CStatic m_static_password;
	CString m_strRoomNo;
	int m_nRoomNo;
	afx_msg void OnBnClickedChangeCondition();
	CString m_strRoomPassword;
	CStatic m_lbl_room_password;
	CEdit m_txtRoomPassword2;
	CString m_strRoomPassword2;
	afx_msg void OnCbnEditchangePlayerName();
	CString m_strSelectedUser;

private:
	void UpdateCondition();
	BOOL m_bIsFirstRequest;
	char m_szTitle[128];
public:
	CButton m_ChangeCondition;
	afx_msg void OnCbnSelchangeHandicaps();
	afx_msg void OnCbnSelchangeKomiList();
	CStatic m_static_username;
	CButton m_RadioName;
	int m_RadioRoomNo;
};
