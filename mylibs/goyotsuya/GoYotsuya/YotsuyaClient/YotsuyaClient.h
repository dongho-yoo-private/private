// YotsuyaClient.h : YotsuyaClient アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル
#include "ypnGoNetwork.h"

// CYotsuyaClientApp:
// このクラスの実装については、YotsuyaClient.cpp を参照してください。
//

class CYotsuyaClientApp : public CWinApp
{
public:
	CYotsuyaClientApp();


// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	cypnGoNetwork m_GoNetworkInerface;
};

extern CYotsuyaClientApp theApp;