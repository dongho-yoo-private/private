// InformationCtrl.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "YotsuyaClient.h"
#include "InformationCtrl.h"


// cInformationCtrl �_�C�A���O

IMPLEMENT_DYNAMIC(cInformationCtrl, CDialog)

cInformationCtrl::cInformationCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(cInformationCtrl::IDD, pParent)
{

}

cInformationCtrl::~cInformationCtrl()
{
}

void cInformationCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cInformationCtrl, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// cInformationCtrl ���b�Z�[�W �n���h��

void cInformationCtrl::OnPaint()
{
}