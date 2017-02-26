// MainFrm.h : CMainFrame クラスのインターフェイス
//


#pragma once


#include "yi.h"
#include "DlgRequestPlay.h"
#include "yLoginDialog.h"
#include "ypn.h"
#include "ypnGoNetwork.h"
#include "yPlayControl.h"
#include "../yctrlGo/yctrlGoban.h"
//#include "../yictrl/cyctrlButtons.h"
#include "cyctrlx.h"


#define WM_NOTIFY_FROM_NETWORK (WM_USER+128)

class CMainFrame : public CFrameWnd
{
	
protected: // シリアル化からのみ作成します。
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

// 操作
public:

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 実装
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	BOOL CreateYotsuya();
	LRESULT OnGobanAdded(WPARAM wp, LPARAM lp);
	LRESULT OnNotifyFromNetwork(WPARAM wp, LPARAM lp);
	LRESULT OnPutStone(WPARAM wp, LPARAM lp);
	// ----------------------------------------
	LRESULT OnGobanChanged(WPARAM wp, LPARAM lp);

	void UpdateControlLayout();

	int MessageBox(const char* pszText, const char* pszCaption, UINT type);

	void OnRequestPlay();
	void AdjustLayout(CDialog* control);

	Bool CreatePlayControl();

	Bool CreateEditControl();

	Bool CreateEnjoyControl();

	cyLoginDialog m_LoginDlg;
	CDlgRequestPlay m_RequestPlayDlg;
	cyPlayControl m_PlayControl;

	cypnGoNetwork* m_network;
	cyctrlGoban* m_goban;
	CMenu* m_pGobanListMenu;
	cyctrlx* m_currentCtrl;
	cyctrlx m_ctrlPlay;
	cyctrlx m_ctrlEdit;
	cyctrlx m_ctrlEnjoy;


protected:  // コントロール バー用メンバ
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// 生成された、メッセージ割り当て関数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCmdGiveup();
	afx_msg void OnCmdPlayCancel();
	afx_msg void OnCmdQuitPlay();
	afx_msg void OnCmdCalculate();
	afx_msg void OnCmdCalculateEnd();
	afx_msg void OnCmdWait();
	afx_msg void OnCmdAbort();
	afx_msg void OnCmdPlayEnd();
	afx_msg void OnCmdNext();
	afx_msg void OnCmdTop();
	afx_msg void OnCmdPrev();
	afx_msg void OnAppExit();
};


