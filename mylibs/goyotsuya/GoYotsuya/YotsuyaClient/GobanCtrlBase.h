#pragma once

#include "../yctrlGo/yctrlGoban.h"
#include "ypnGoNetwork.h"
#include "afxcmn.h"


// CGobanCtrlBase �_�C�A���O

class CGobanCtrlBase : public CDialog
{
	DECLARE_DYNAMIC(CGobanCtrlBase)

public:
	CGobanCtrlBase(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CGobanCtrlBase();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_GOBANCTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

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
