// GobanCtrlBase.cpp : 実装ファイル
//

#include "stdafx.h"
#include "YotsuyaClient.h"
#include "GobanCtrlBase.h"


// CGobanCtrlBase ダイアログ

IMPLEMENT_DYNAMIC(CGobanCtrlBase, CDialog)

CGobanCtrlBase::CGobanCtrlBase(CWnd* pParent /*=NULL*/)
	: CDialog(CGobanCtrlBase::IDD, pParent)
{

}

CGobanCtrlBase::~CGobanCtrlBase()
{
}

void CGobanCtrlBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GOBANSELECTOR, m_tabCtrl);
}


BEGIN_MESSAGE_MAP(CGobanCtrlBase, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_GOBANSELECTOR, &CGobanCtrlBase::OnTcnSelchangeGobanselector)
	ON_MESSAGE(WM_GOBAN_ADDED, &CGobanCtrlBase::OnAddGoban)
END_MESSAGE_MAP()


// CGobanCtrlBase メッセージ ハンドラ

void CGobanCtrlBase::OnTcnSelchangeGobanselector(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	*pResult = 0;

	m_gobanCtrl->ChangeGoban(TabCtrl_GetCurSel(pNMHDR->hwndFrom));
}

//
BOOL CGobanCtrlBase::AddTab(const char* pszTabName, int index)
{
	TC_ITEM tc;
	tc.mask = TCIF_TEXT;
	tc.pszText = (LPSTR)pszTabName;

	TabCtrl_InsertItem(m_hGobanSelector, index, &tc);

	RECT rt;
	LPPOINT pt=(LPPOINT)&rt;
	::GetClientRect(m_hGobanSelector, &rt);
	TabCtrl_AdjustRect(m_hGobanSelector, FALSE, &rt);
	::MapWindowPoints(m_hGobanSelector, *this, pt, 2);	

	m_gobanCtrl->Resize(rt.left, rt.top, rt.right-rt.left, rt.bottom-rt.top);
	return TRUE;
}

#include "yiEncoder.h"
//
BOOL CGobanCtrlBase::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

LRESULT CGobanCtrlBase::OnAddGoban(WPARAM wp, LPARAM lp)
{
	sGoDatas* p = (sGoDatas*)lp;
	AddTab(p->sgfMgr.GetTitle(), p->nIndex);
	return 0;
}

cyctrlGoban* CGobanCtrlBase::GetGobanCtrl()
{
	return m_gobanCtrl;
}