#include "StdAfx.h"
#include "MyMainFrame.h"

// ---------------------------------------
static ATOM xMyRegisterClass(HINSTANCE hInstance, const char* pszClassName);

// ---------------------------------------
static HWND xInitInstance(HINSTANCE hInstance, const char* pszClassName, const char* pszTitle, int nCmdShow, void* param);

// ---------------------------------------
cMyMainFrame::cMyMainFrame()
{
}

// ---------------------------------------
cMyMainFrame::~cMyMainFrame()
{
}

// ---------------------------------------
Bool cMyMainFrame::Create(HINSTANCE hInstance, const char* pszClassName, const char* pszTitle)
{
	if (xMyRegisterClass(hInstance, pszClassName)==0)
	{
		return False;
	}

	m_hWnd = xInitInstance(hInstance, pszClassName, pszTitle, SW_SHOW, this);
	m_bIsCreated=True;

	return m_hWnd ? True:False;
}



// ---------------------------------------
BOOL cMyMainFrame::OnPaint(HDC hdc, PAINTSTRUCT* ps/*=0*/)
{
	return False;
}

// ---------------------------------------------------------------
LRESULT cMyMainFrame::OnCreate(WPARAM wp, LPARAM lp)
{
	return 0;
}

// ------------------------------------
LRESULT cMyMainFrame::OnDestroy(WPARAM wp, LPARAM lp)
{
	PostQuitMessage(0);
	return 0;
}

// ---------------------------------------------------------------
LRESULT cMyMainFrame::OnAbout(HWND hWnd, int code)
{
	HINSTANCE hInst = (HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE);

	cyctrlbase m_dlg;
	m_dlg.CreateFromReosurce(m_hWnd, hInst, IDD_ABOUTBOX, True);
	return 0;
}

// ---------------------------------------------------------------
LRESULT cMyMainFrame::OnExit(HWND hWnd, int code)
{
	DestroyWindow(m_hWnd);
	return 0;
}


//
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
//  �R�����g:
//
//    ���̊֐�����юg�����́A'RegisterClassEx' �֐����ǉ����ꂽ
//    Windows 95 ���O�� Win32 �V�X�e���ƌ݊�������ꍇ�ɂ̂ݕK�v�ł��B
//    �A�v���P�[�V�������A�֘A�t����ꂽ
//    �������`���̏������A�C�R�����擾�ł���悤�ɂ���ɂ́A
//    ���̊֐����Ăяo���Ă��������B
//
static ATOM xMyRegisterClass(HINSTANCE hInstance, const char* pszClassName)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= cyctrlbase::__wndproc__;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, pszClassName);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_YIGEDITOR);
	wcex.lpszClassName	= pszClassName;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
static HWND xInitInstance(HINSTANCE hInstance, const char* pszClassName, const char* psztitle, int nCmdShow, void* param)
{
   HWND hWnd;

   hWnd = CreateWindow(pszClassName, psztitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, param);

   if (!hWnd)
   {
      return 0;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return hWnd;
}

