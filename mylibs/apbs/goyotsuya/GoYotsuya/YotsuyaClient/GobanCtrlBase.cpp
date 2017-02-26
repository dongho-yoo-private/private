// GobanCtrlBase.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "YotsuyaClient.h"
#include "GobanCtrlBase.h"


// CGobanCtrlBase �_�C�A���O

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


// CGobanCtrlBase ���b�Z�[�W �n���h��

void CGobanCtrlBase::OnTcnSelchangeGobanselector(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
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

	// TODO:  �����ɏ�������ǉ����Ă�������


	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
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