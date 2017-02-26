#include "glsMonthCalender.h"

//MONTHCALDLG DIALOG DISCARDABLE  0, 0, 121, 69
//STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
//CAPTION "日付"
//FONT 11, "メイリオ"
//BEGIN
//    DEFPUSHBUTTON   "OK",IDOK,7,38,50,14
//    PUSHBUTTON      "キャンセル",IDCANCEL,61,37,50,14
//END

cglsMonthCalender::cglsMonthCalender(time64_t initTime)
:m_initTime(initTime)
{
	if (m_initTime==0)
	{
		SYSTEMTIME t;
		GetSystemTime(&t);
		m_initTime=MAKETIME64(t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
	}
}

cglsMonthCalender::~cglsMonthCalender(void)
{
}

// ------------------------------------
void cglsMonthCalender::SetDay(time64_t time)
{
	m_initTime=time;
	m_time=time;
}



// ------------------------------------
Bool cglsMonthCalender::DoModal(HWND hWndParent, char* pszTitle, int x, int y, 
								time64_t day, eGlsMonthCalendarMode mode, Font* font)
{
	m_eMode=mode;
	m_initTime=day;
	return cglsWindow::DoModal(hWndParent, 0, pszTitle, x, y, 100, 100, font);
}

#include "glsButton.h"

// ------------------------------------
Bool cglsMonthCalender::TrackPopup(HWND hWndParent, int x, int y, 
								   time64_t day, eGlsMonthCalendarMode mode, Font* font)
{
	m_eMode=mode;
	
	if (day)
	{
		m_initTime=day;
	}
	Bool res = CreatePopup(hWndParent, x, y, 100, 100, font, 0);

	SYSTEMTIME t;
	MonthCal_GetCurSel(m_hwndMonthCal, &t);
	m_time=MAKETIME64(t.wYear, t.wMonth, t.wDay, 0, 0, 0);



	if (m_children)
	{
		cglsButton* btn = (cglsButton* )m_children->removeEx(0);
		delete btn;
	}

	DestroyWindow(m_hWnd);
	m_hWnd=0;

	return res;
}

// ------------------------------------
Bool cglsMonthCalender::OnOk(HWND hWnd, int code)
{
	SYSTEMTIME t;
	MonthCal_GetCurSel(m_hwndMonthCal, &t);
	m_time=MAKETIME64(t.wYear, t.wMonth, t.wDay, 0, 0, 0);
	m_bIsExitLoop=True;
	m_bIsChanged=True;

	//EndDialog(m_hWnd, 1);
	return True;
}

// ------------------------------------
time64_t cglsMonthCalender::GetSelectedTime()
{
	return m_time;
}

// ------------------------------------
Bool cglsMonthCalender::OnCancel(HWND hWnd, int code)
{
	EndDialog(m_hWnd, 0);
	return True;
}

#include "glsButton.h"

// 以下を参照
// http://homepage2.nifty.com/c_lang/sdk2/sdk_178.htm
// ------------------------------------
Bool cglsMonthCalender::OnInitDialog(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	RECT rc;
    INITCOMMONCONTROLSEX icex;
    HWND hwndMonthCal = NULL;

    // Load the window class.
    icex.dwSize = sizeof(icex);
    icex.dwICC  = ICC_DATE_CLASSES;
    InitCommonControlsEx(&icex);

    // Create a modeless dialog box to hold the control.
                        
    // Create the month calendar.
    hwndMonthCal  = CreateWindowEx(0,
                    MONTHCAL_CLASS,
                    "",
					WS_BORDER | WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN| MCS_DAYSTATE,
                    0,0,0,0, // resize it later
                    m_hWnd,
                    (HMENU) 10/*IDC_MONTHCAL*/,
                    (HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE),
                    NULL);

    // Return if creating the month calendar failed. 
    if (hwndMonthCal == NULL)
        return False; 

	m_hwndMonthCal=hwndMonthCal;
                     
    // Get the size required to show an entire month.
    MonthCal_GetMinReqRect(hwndMonthCal, &rc);

	cglsButton* btnOk = new cglsButton();
	btnOk->Create((HWND)this, IDOK, 0, rc.bottom, 0, L"OK");

	cglsButton* btnCancel = new cglsButton();
	btnCancel->Create((HWND)this, IDCANCEL, btnOk->m_w, rc.bottom, 0, L"取り消し");

	int widthOk, widthCancel, h;

	btnCancel->GetMeasureSize(widthCancel, h);
	btnOk->GetMeasureSize(widthOk, h);

	btnCancel->Move(rc.right-widthCancel, btnCancel->m_y);
	btnOk->Move(btnCancel->m_x-widthOk, btnOk->m_y);

	btnOk->Show();
	btnOk->Update(eGUM_FORCED);
	btnCancel->Show();
	btnCancel->Update(eGUM_FORCED);
	ReSizeClientRect(rc.right, rc.bottom+h);

    // Resize the control now that the size values have been obtained.
    SetWindowPos(hwndMonthCal, NULL, 0, 0, 
        rc.right, rc.bottom, SWP_NOZORDER);

    // Set the calendar to the annual view.
    MonthCal_SetCurrentView(hwndMonthCal, MCMV_YEAR);

	::InvalidateRect(m_hWnd, 0, FALSE);
	InvalidateRect(0);

	//ReSize(rc.right, rc.bottom);

    // Make the window visible.
    //ShowWindow(hwndDlg, SW_SHOW);
	return True;
}

// ---------------------------------------------------
static HWND xCreateCalendarWindow(HWND m_hWnd)
{
    // Create the month calendar.
    return CreateWindowEx(0,
                    MONTHCAL_CLASS,
                    "",
					WS_BORDER | WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN| MCS_DAYSTATE,
                    0,0,0,0, // resize it later
                    m_hWnd,
                    (HMENU) 10/*IDC_MONTHCAL*/,
                    (HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE),
                    NULL);
}

// ---------------------------------------------------
static HWND xCreateTimeWindow(HWND m_hWnd)
{
    // Create the month calendar.
    LPSTR lpstrTime = "tthh'時'mm'分'ss'秒'";


   HWND hTime =CreateWindowEx(0,
				DATETIMEPICK_CLASS, NULL, WS_BORDER | WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN| DTS_TIMEFORMAT,
				160, 10, 120, 25, m_hWnd, (HMENU)11, (HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE), NULL);

   if (hTime)
   {
		SendMessage(hTime, DTM_SETFORMAT, 0, (LPARAM)lpstrTime);
   }

   return hTime;
}
#include "glsSpinBox.h"

// ---------------------------------------------------
LRESULT cglsMonthCalender::OnCreate(WPARAM wParam, LPARAM lParam)
{
	RECT rc;
    INITCOMMONCONTROLSEX icex;
    HWND hwndMonthCal = NULL;

    // Load the window class.
    icex.dwSize = sizeof(icex);
    icex.dwICC  = ICC_DATE_CLASSES;
    InitCommonControlsEx(&icex);

    // Create a modeless dialog box to hold the control.
	if (m_eMode==::eGMCM_CAL)
	{
		hwndMonthCal  = xCreateCalendarWindow(m_hWnd);
	}
	else
	{
		hwndMonthCal=xCreateTimeWindow(m_hWnd);
	}


    // Return if creating the month calendar failed. 
    if (hwndMonthCal == NULL)
        return False; 

	m_hwndMonthCal=hwndMonthCal;
    

	if (m_eMode==eGMCM_CAL)
	{
		int year, month;
		// Get the size required to show an entire month.
		MonthCal_GetMinReqRect(hwndMonthCal, &rc);
		
		// Set the calendar to the annual view.
		SetWindowPos(hwndMonthCal, NULL, 0, 0, 
			rc.right, rc.bottom, SWP_NOZORDER);
		MonthCal_SetCurrentView(hwndMonthCal, MCMV_YEAR);
		MonthCal_SetCurrentView(hwndMonthCal, MCMV_MONTH);

		if (m_initTime!=0)
		{
			SYSTEMTIME t;
			memset(&t, 0, sizeof(t));
			t.wYear=xYEAR64(m_initTime);
			t.wMonth=xMONTH64(m_initTime);
			t.wDay=xDAY64(m_initTime);
			MonthCal_SetCurSel(hwndMonthCal, &t);
		}
	}
	else
	{
		//cglsSpinBox spinHour = new cglsSpinBox();
		//spinHour->Create(this, 2, 0, 0, 2);
		//spinHour->SetNumer(0, 0, 9, 0);
		//spinHour->SetNumer(1, 0, 2, 0);

		//cglsSpinBox spinMin = new cglsSpinBox();
		//spinMin->Create(this, 2, 0, 0, 2);
		//spinMin->SetNumer(0, 0, 9, 0);
		//spinMin->SetNumer(1, 0, 5, 0);

		//cglsSpinBox spinSec = new cglsSpinBox();
		//spinSec->Create(this, 2, 0, 0, 2);
		//spinSec->SetNumer(0, 0, 9, 0);
		//spinSec->SetNumer(1, 0, 5, 0);


		::GetClientRect(hwndMonthCal, &rc);
	}
	
	cglsButton* btn = new cglsButton();
	sGlsButtonDetail x;
	cglsButton::xDefaultDetail(&x);

	x.rect.right=rc.right;
	x.rect.bottom=0;

	//wchar_t buffer[128];

	//cyiTime::TimeToString(m_initTime, buffer, "YYYY-MM-DD");

	btn->Create((HWND)this, IDOK, 0, rc.bottom, 0, L"OK", 0, &x);
	btn->Update(eGUM_FORCED);
	btn->Show();

	ReSizeClientRect(rc.right, rc.bottom+btn->m_h);

    // Resize the control now that the size values have been obtained.


	//MonthCal_SetCurrentView(hwndMonthCal, MCMV_YEAR);
	//MonthCal_SetCurrentView(hwndMonthCal, MCMV_MONTH);


	::InvalidateRect(m_hWnd, 0, FALSE);
	//InvalidateRect(0);

	return 0;
}

// ------------------------------------
Bool cglsMonthCalender::OnClose(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	EndDialog(m_hWnd, 0);
	return True;
}

#define BOLDDAY(ds, iDay)  \
    if (iDay > 0 && iDay < 32)(ds) |= (0x00000001 << (iDay - 1))


// ------------------------------------
LRESULT cglsMonthCalender::OnNotify(WPARAM wParam, LPARAM lParam)
{
    //if (((LPNMHDR)lParam)->code == MCN_GETDAYSTATE)
    //{
    //    MONTHDAYSTATE rgMonths[12] = { 0 };
    //    int cMonths = ((NMDAYSTATE*)lParam)->cDayState;
    //    for (int i = 0; i < cMonths; i++)
    //    {
    //        BOLDDAY(rgMonths[i], 1);
    //        BOLDDAY(rgMonths[i], 15);
    //    }
    //    ((NMDAYSTATE*)lParam)->prgDayState = rgMonths;
    //    return TRUE;
    //}


	return False;
}

// assitant interface
// -----------------------------------------------------
Bool cglsMonthCalender::ShowAssistant(HWND hWnd, int x, int y)
{
	return TrackPopup(hWnd, x, y, m_initTime);
	//return True;
}

// -----------------------------------------------------
Bool cglsMonthCalender::GetResult(wchar_t* buffer)
{
	wsprintfW(buffer, L"%d/%02d/%02d", xYEAR64(m_time), xMONTH64(m_time), xDAY64(m_time));
	return True;
}

// -----------------------------------------------------
Bool cglsMonthCalender::GetResult(unsigned int& result1, unsigned int& result2)
{
	return False;
}

// -----------------------------------------------------
Bool cglsMonthCalender::GetDefaultValue(wchar_t* buffer)
{
	wsprintfW(buffer, L"%d/%02d/%02d", xYEAR64(m_initTime), xMONTH64(m_initTime), xDAY64(m_initTime));
	return True;
}

// -----------------------------------------------------
Bool cglsMonthCalender::GetDefaultValue(unsigned int& result1, unsigned int& result2)
{
	return False;
}

// -----------------------------------------------------
Bool cglsMonthCalender::SetDefaultValue(wchar_t* value)
{
	return True;
}

// -----------------------------------------------------
void cglsMonthCalender::Update(Bitmap* bmp, RECT* rect, RECT* _childRect, int offsetX, int offsetY, Bool bIsLocalBufferUpdate)
{
	if (m_nRequestUpdateOrder==0)
	{
		::InvalidateRect(m_hwndMonthCal, NULL, FALSE);
	}
	__super::Update(bmp, rect, _childRect, offsetX, offsetY, bIsLocalBufferUpdate);
}

