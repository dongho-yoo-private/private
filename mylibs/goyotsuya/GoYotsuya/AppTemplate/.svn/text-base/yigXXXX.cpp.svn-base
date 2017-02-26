// yigXXXX.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "yigXXXX.h"
#include "MyApp.h"

#define MAX_LOADSTRING 100

GdiplusStartupInput __g_gdiSI__;
ULONG_PTR           __g_gdiToken__;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	GdiplusStartup(&__g_gdiToken__, &__g_gdiSI__, NULL);

 	// TODO: ここにコードを挿入してください。

	cMyApp* app = new cMyApp(hInstance, "CLASS_MY_APP", "MyApplication");
	
	if (app->CreateMainFrame()==False)
		return 0;

	app->OnLoop();
	GdiplusShutdown(__g_gdiToken__);

	return 0;
}

