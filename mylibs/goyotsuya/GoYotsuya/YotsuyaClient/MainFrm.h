// MainFrm.h : CMainFrame �N���X�̃C���^�[�t�F�C�X
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
	
protected: // �V���A��������̂ݍ쐬���܂��B
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:

// �I�[�o�[���C�h
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����
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


protected:  // �R���g���[�� �o�[�p�����o
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
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


