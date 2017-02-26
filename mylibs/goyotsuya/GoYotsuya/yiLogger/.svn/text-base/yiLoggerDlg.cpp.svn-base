//#include "StdAfx.h"
#include "yiLoggerDlg.h"

cyiLoggerDlg::cyiLoggerDlg(void)
{
	m_bIsTeminateThread=False;
	m_mode=0xFFFFFFFF;
	m_nBufferSize=1024;
	m_nPortNo=514;
	m_szDir[0]=0;
}

cyiLoggerDlg::~cyiLoggerDlg(void)
{
}

// -----------------------
LRESULT cyiLoggerDlg::OnCreate(WPARAM wp, LPARAM lp)
{
	// 1. まず、iniファイルから情報を読み込む。
	
	// 1.1. デバッグモードの読み込み。
	::WritePrivateProfileString("yiLogger", "Debug", "On", ".\\x.ini");

	m_mode = ::GetPrivateProfileInt("yiLogger", "mode", 0xFFFFFFFF, ".\\x.ini");
	// 1.2. コンソールバッファーサイズの読み込み。
	m_nBufferSize = ::GetPrivateProfileInt("yiLogger", "ConsoleLine", 1024, ".\\x.ini");
	// 1.3. ポート番号の読み込み
	m_nPortNo = ::GetPrivateProfileInt("yiLogger", "port", 514, ".\\x.ini");
	// 1.4. ディレクトリを取得
	::GetPrivateProfileString("yiLogger", "dir", ".", m_szDir, 256, ".\\x.ini");


	// 3. ダイアログをシステムトレイへ
	if (yiUtilSetSystemTray(m_hWnd, 0, ::LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_YILOGGER)), WM_SYSTRAY_CLICKED, "yiLogger")==False)
	{
		EndDialog(m_hWnd, 0);
		return 0;
	}

	ShowWindow(m_hWnd, SW_HIDE);
	// 2. 待ち受けスレッド生成。
	m_bIsTeminateThread=False;
	StartThread(1);

	char buffer[128];
	wsprintf(buffer, "yiSysLog (port:%d, mode:%x, buffer:%d)", m_nPortNo, m_mode, m_nBufferSize);
	SetWindowText(m_hWnd, buffer);

	return 1;

}

// -----------------------
LRESULT cyiLoggerDlg::OnOk()
{
	ShowWindow(m_hWnd, SW_HIDE);
	return 1;
}

// -----------------------
LRESULT cyiLoggerDlg::OnCancel()
{
	m_bIsTeminateThread=True;
	WaitThread(1, True);

	EndDialog(m_hWnd, 0);
	return 0;
}

// -----------------------
LRESULT cyiLoggerDlg::OnClear()
{
	::SendMessage(GetDlgItem(m_hWnd, IDC_LISTLOG), LB_RESETCONTENT, 0, 0);
	return 0;
}

// -----------------------
LRESULT cyiLoggerDlg::OnAddConsole(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	int len=wParam;
	char* p = (char*)lParam;
	p[len]=0;

	::SendMessage(GetDlgItem(m_hWnd, IDC_LISTLOG), LB_ADDSTRING, 0, (LPARAM)p);
	yifree(p);

	int cnt = SendMessage(GetDlgItem(m_hWnd, IDC_LISTLOG), LB_GETCOUNT, 0, 0);

	if (m_nBufferSize!=0)
	{
		if (cnt==m_nBufferSize)
		{
			::SendMessage(GetDlgItem(m_hWnd, IDC_LISTLOG), LB_DELETESTRING, 0, 0);
		}
	}

	return 0;
}

// -----------------------
LRESULT cyiLoggerDlg::OnSystemTrayClicked(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	if (lParam==WM_LBUTTONDBLCLK)
	{
		ShowWindow(m_hWnd, SW_SHOWNORMAL);
	}

	return 0;
}

// -----------------------
LRESULT cyiLoggerDlg::OnClose(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	ShowWindow(m_hWnd, SW_MINIMIZE);
	ShowWindow(m_hWnd, SW_HIDE);
	handled=True;
	return 1;
}

#include "yisock.h"

// --------------------------------
Bool cyiLoggerDlg::IsOutput(unsigned int code)
{
	if ((m_mode&code)==0)
	{
		return False;
	}

	return True;
}

// --------------------------------
static char* xMakeFileName(char* m_szDir, char* szFileName)
{
	char buffer[32];
	unsigned int day=0;
	unsigned int time= ::cyiTime::GetLocalTime(day);
	cyiTime::TimeToString(buffer, day, time, False, "", "");

	sprintf(szFileName, "%s\\%s.log", m_szDir, buffer);
	return szFileName;
}

// --------------------------------
Bool cyiLoggerDlg::PutError(const char* p)
{
	::SendMessage(GetDlgItem(m_hWnd, IDC_LISTLOG), LB_ADDSTRING, 0, (LPARAM)p);
	return True;
}

// --------------------------------
void cyiLoggerDlg::OnThreadStart(int id, int param1, int param2)
{
	yiInitWinsock(2, 2);
	int sock = yiudp(0, m_nPortNo);

	if (sock<0)
	{
		PostMessage(m_hWnd, WM_THREAD_QUIT, -1, 0);
		return ;
	}

	char szFileName[128];
	xMakeFileName(m_szDir, szFileName);
	HANDLE hFile = ::CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ,  0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	SetFilePointer(hFile, 0, 0, FILE_END);

	while(m_bIsTeminateThread==0)
	{
		char out[1056];
		char buffer[1024];
		unsigned int* code=(unsigned int*)buffer;
		int  timeout=1000;
		int n = ::yirecvu(sock, buffer, 1024, &timeout, 0, 0);
		
		if (n>0)
		{
			if (IsOutput(*code)==True)
			{
				DWORD written=0;
				int nLength = strlen((char*)&code[1]);
				WriteFile(hFile, &code[1], nLength, &written,0);
				::PostMessage(m_hWnd, WM_ADD_CONSOLE, nLength, (LPARAM)yitoheap(&code[1], nLength+1)); 
			}
			continue;
		}
		
		if (n!=eSE_TIMEOUT)
		{
			PostMessage(m_hWnd, WM_THREAD_QUIT, -2, 0);
			CloseHandle(hFile);
			yiclose(sock);
			return ;
		}
	}
	
	yiclose(sock);
	CloseHandle(hFile);

	return ;
}


// -----------------------
LRESULT cyiLoggerDlg::OnThreadQuit(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	if (wParam==-1)
	{
		MessageBox(*this, "UDPポート作成失敗", "エラー", MB_OK|MB_ICONERROR);
		EndDialog(m_hWnd, 0);
		return 0;
	}
	else if (wParam==-2)
	{
		MessageBox(*this, "受信エラー", "エラー", MB_OK|MB_ICONERROR);
		EndDialog(m_hWnd, 0);
		return 0;
	}

	return 0;
}