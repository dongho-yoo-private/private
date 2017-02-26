#pragma once

#include "../yctrlGo/yctrlGoban.h"
#include "ypnGoNetwork.h"
#include "afxcmn.h"


// CGobanCtrlBase ダイアログ

class CGobanCtrlBase : public CDialog
{
	DECLARE_DYNAMIC(CGobanCtrlBase)

public:
	CGobanCtrlBase(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CGobanCtrlBase();

// ダイアログ データ
	enum { IDD = IDD_GOBANCTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeGobanselector(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();

	cyctrlGoban* GetGobanCtrl();

private:
	HWND m_hGobanSelector;
	cyctrlGoban* m_gobanCtrl;
	cypnGoNetwork* m_network;

	BOOL AddTab(const char* pszTabName, int index);
	LRESULT OnAddGoban(WPARAM wp, LPARAM lp);
public:
	CTabCtrl m_tabCtrl;
};
