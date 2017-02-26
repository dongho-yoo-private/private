#include "glsDlgProgress.h"
#include "resource.h"



cglsDlgProgress::cglsDlgProgress(void)
{
}

cglsDlgProgress::~cglsDlgProgress(void)
{
}


// ------------------------------------------------------------------------
Bool cglsDlgProgress::Create(HWND hParent, const char* pszCaption, const char* pszContents, int min, int max, int* progress, Bool bIsModal, Bool* bIsCalceledPointer)
{
	m_progress_pointer=progress;
	m_bIsCalceledPointer=bIsCalceledPointer;

	if (m_progress_pointer)
		*m_progress_pointer=min;

	if (m_bIsCalceledPointer)
		*m_bIsCalceledPointer=False;

	m_nMinCnt=min;
	m_nMaxCnt=max;

	m_szCaption[0]=0;
	m_szInitString[0]=0;

	if (pszContents)
	{
		strncpy(m_szInitString, pszContents, 255);
	}

	if (pszCaption)
	{
		strncpy(m_szCaption, pszCaption, 255);
	}

	return cyctrlbase::CreateFromReosurce(hParent, (HINSTANCE)cyctrlbase::GetModuleHandle(), IDD_PROGRESS, bIsModal);
}

// ------------------------------------------------------------------------
void cglsDlgProgress::Destroy()
{
	if (m_bIsModal)
		EndDialog(m_hWnd, 0);
	else
		DestroyWindow(m_hWnd);
}

// ------------------------------------------------------------------------
void cglsDlgProgress::SetProgress(int n, char* pszText/*=0*/)
{
	if (pszText)
	{
		SetWindowText(m_hProgressText, pszText);
	}
	SendMessage(m_hProgress, PBM_STEPIT, 0, 0);
}

// ------------------------------------------------------------------------
void cglsDlgProgress::SetProgressOK(char* pszText)
{
	KillTimer(m_hWnd, 1);
	SetWindowText(GetDlgItem(m_hWnd, IDCANCEL), "確認");
	SendMessage(m_hProgress, PBM_SETPOS, m_nMaxCnt, 0);

	SetWindowText(m_hProgressText, pszText);
}

// ------------------------------------------------------------------------
void cglsDlgProgress::Cancel()
{
	Destroy();
}

// ------------------------------------------------------------------------
LRESULT cglsDlgProgress::OnInitDialog(WPARAM wp, LPARAM lp, Bool& bIsHandled)
{
	if (m_progress_pointer)
	{
		SetTimer(m_hWnd, 1, 100, 0);
	}

	m_hProgress = GetDlgItem(m_hWnd, IDC_PROGRESS);
	m_hProgressText = GetDlgItem(m_hWnd, IDC_LBL_PROGRESS);

	if (m_nMaxCnt>0xFFFF)
	{
		SendMessage(m_hProgress, PBM_SETRANGE, 0, MAKELPARAM(m_nMinCnt, m_nMaxCnt/10));
	}
	else
	{
		SendMessage(m_hProgress, PBM_SETRANGE, 0, MAKELPARAM(m_nMinCnt, m_nMaxCnt));
	}
	SendMessage(m_hProgress, PBM_SETSTEP, 1, 0);
	SetWindowText(m_hProgressText, m_szInitString);
	SetWindowText(m_hWnd, m_szCaption);
	SetWindowText(GetDlgItem(m_hWnd, IDCANCEL), "キャンセル");

	bIsHandled=True;
	return True;
}

// ------------------------------------------------------------------------
LRESULT cglsDlgProgress::OnCancel(HWND hWnd, int code)
{
	if (m_bIsCalceledPointer)
	{
		*m_bIsCalceledPointer=True;
	}

	KillTimer(m_hWnd, 1);
	Destroy();
	return 0;
}

// ------------------------------------------------------------------------
LRESULT cglsDlgProgress::OnTimer(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	if (m_bIsCalceledPointer && *m_bIsCalceledPointer==True)
	{
		KillTimer(m_hWnd, 1);
		Destroy();
		return 0;
	}

	if (m_nMaxCnt>0xFFFF)
	{
		SendMessage(m_hProgress, PBM_SETPOS, *m_progress_pointer/10, 0);
	}
	else
	{
		SendMessage(m_hProgress, PBM_SETPOS, *m_progress_pointer, 0);
	}

	if (*m_progress_pointer==m_nMaxCnt)
	{
		KillTimer(m_hWnd, 1);
		Destroy();
		return 0;
	}
	return 0;
}