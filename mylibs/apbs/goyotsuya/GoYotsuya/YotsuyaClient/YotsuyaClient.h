// YotsuyaClient.h : YotsuyaClient �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��
#include "ypnGoNetwork.h"

// CYotsuyaClientApp:
// ���̃N���X�̎����ɂ��ẮAYotsuyaClient.cpp ���Q�Ƃ��Ă��������B
//

class CYotsuyaClientApp : public CWinApp
{
public:
	CYotsuyaClientApp();


// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

// ����
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	cypnGoNetwork m_GoNetworkInerface;
};

extern CYotsuyaClientApp theApp;