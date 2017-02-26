#include "yi.h"
#include "yitime.h"
#include "yiTimeEx.h"
#include "yiUtil.h"
//#include <windows.h>



#define MINUTE_MILLISEC (60*1000)
#define HOUR_MILLISEC (60*60*1000)

// ------------------------------------
unsigned long cyiTime::CurrentmSec(unsigned int before)
{
	syiSecs sec = yiGetCurrentSec();
	ubit32_t t = sec.sec*1000+sec.usec/1000;

	if (before==0)
		return t;

	return GetSecDiff(before, t);
	/*unsigned long t2;
	SYSTEMTIME time;
	::GetLocalTime(&time);
	

	t2 = (unsigned long)((ULONG)time.wHour*HOUR_MILLISEC + (ULONG)time.wMinute*MINUTE_MILLISEC+(ULONG)time.wSecond*1000+(ULONG)time.wMilliseconds);

	if (before==0)
		return t2;

	return GetSecDiff(before, t2);*/
}

// ------------------------------------
unsigned long cyiTime::GetSecDiff(unsigned long before, unsigned long after)
{
	if (before>after)
	{
		after+=25*HOUR_MILLISEC;
	}

	return after-before;
}

// ------------------------------------
unsigned int cyiTime::GetLocalTime()
{
	return yiLocalTime();
/*	SYSTEMTIME curr;
	::GetLocalTime(&curr);

	return MAKETIME(curr.wYear, curr.wMonth, curr.wDay, curr.wHour, curr.wMinute, curr.wSecond);*/
}

// ------------------------------------
unsigned int cyiTime::GetLocalTime(unsigned int &day)
{
	time_t t=time(0);
	struct tm* curr = localtime(&t);
	//SYSTEMTIME curr;
	//::GetLocalTime(&curr);

	day = MAKEDAY(curr->tm_year+1900, curr->tm_mon, curr->tm_mday);//curr.wYear, curr.wMonth, curr.wDay);
	return MAKETIME(curr->tm_year+1900, curr->tm_mon, curr->tm_mday, curr->tm_hour, curr->tm_min, curr->tm_sec);//wHour, curr.wMinute, curr.wSecond);
}

// ------------------------------------
char* cyiTime::TimeToString(char* szTime, unsigned int day, unsigned int time, Bool bSpace, char* pTimeSpliter, char* pDaySpliter)
{
	if (pTimeSpliter==0)
		pTimeSpliter="/";
	if (pDaySpliter==0)
		pDaySpliter=":";

	int y, m, d, hh, mm, ss;

	y=DAY_YEAR(day);
	m=DAY_MON(day);
	d=DAY_DAY(day);

	hh = xHOUR(time);
	mm = xMINUTE(time);
	ss = xSECOND(time);

	if (time!=0 &&
			day!=0)
	{
		if (bSpace==True)
		{
			wsprintf(szTime, "%04d%s%02d%s%02d %02d%s%02d%s%02d", y, pDaySpliter, m, pDaySpliter, d, 
							hh, pTimeSpliter, mm, pTimeSpliter, ss);
		}
		else
		{
			wsprintf(szTime, "%04d%s%02d%s%02d%02d%s%02d%s%02d", y, pDaySpliter, m, pDaySpliter, d, hh, pTimeSpliter, mm,  pTimeSpliter, ss);
		}
	}
	else if (day==0)
	{
		wsprintf(szTime, "%02d%s%02d%s%02d", hh, pTimeSpliter, mm, pTimeSpliter, ss);
	}
	else if (time==0)
	{
		wsprintf(szTime, "%04d%s%02d%s%02d", y, pDaySpliter, m, pDaySpliter, d);
	}

	return szTime;
}

// ------------------------------------
time64_t cyiTime::GetLocalTime64()
{
/*
	SYSTEMTIME curr;
	::GetLocalTime(&curr);

	unsigned long t = MAKETIME64(curr.wYear, curr.wMonth, curr.wDay, curr.wHour, curr.wMinute, curr.wSecond);
	//unsigned __int64 ret = curr.wYear;

	//ret=(ret<<26);

	return t;//(ret | t);*/
}

// ------------------------------------
Bool cyiTime::IsLeapYear(unsigned int timex)
{
	int y = xYEAR(timex);

	if (y%4!=0)
	{
		return False;
	}

	if (y%100!=0)
	{
		return True;
	}

	if (y%400==0)
	{
		return True;
	}
	return False;
}

// ------------------------------------
unsigned int cyiTime::IncreaseSec(unsigned int timex)
{
	int month[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int monthy[12]={31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int* mon=month;
	int y = xYEAR(timex);
	int m = xMONTH(timex);
	int d = xDAY(timex);
	int hh = xHOUR(timex);
	int mm = xMINUTE(timex);
	int ss = xSECOND(timex);

	if (ss!=59)
	{
		return MAKETIME(y, m, d, hh, mm, ss);
	}

	if (mm!=59)
	{
		return MAKETIME(y, m, d, hh, mm+1, 0);
	}

	if (hh!=23)
	{
		return MAKETIME(y, m, d, hh+1, 0, 0);
	}

	if (IsLeapYear(timex)==True)
	{
		mon=monthy;
	}
	if (mon[m-1]!=d)
	{
		return MAKETIME(y, m, d+1, 1, 0, 0);
	}

	if (m!=12)
	{
		return MAKETIME(y, m+1, 1, 0, 0, 0);
	}
	return MAKETIME(y+1, 1, 0, 0, 0, 0);
}

#ifdef _WIN32
#include <commctrl.h>
#define IDC_MONTHCAL 10

// ------------------------------------
Bool cyiTime::ShowMonthCalender(HWND hwndOwner, time64_t& date)
{
/*    RECT rc;
    INITCOMMONCONTROLSEX icex;
    HWND hwndDlg = NULL;
    HWND hwndMonthCal = NULL;

    // Return an error code if the owner handle is invalid.
    if (hwndOwner == NULL)
        return E_INVALIDARG;

    // Load the window class.
    icex.dwSize = sizeof(icex);
    icex.dwICC  = ICC_DATE_CLASSES;
    InitCommonControlsEx(&icex);

    // Create a modeless dialog box to hold the control.
    hwndDlg = CreateDialog((HINSTANCE)GetWindowLong(hwndOwner, GWL_HINSTANCE),
                    MAKEINTRESOURCE(IDD_DATE_PICKER),
                    hwndOwner,
                    MonthCalDlgProc);
    // Return if creating the dialog box failed. 
    if (hwndDlg == NULL)
        return False;//HRESULT_FROM_WIN32(GetLastError()); 
                        
    // Create the month calendar.
    hwndMonthCal  = CreateWindowEx(0,
                    MONTHCAL_CLASS,
                    L"",
                    WS_BORDER | WS_CHILD | WS_VISIBLE | MCS_DAYSTATE,
                    0,0,0,0, // resize it later
                    hwndDlg,
                    (HMENU) IDC_MONTHCAL,
                    g_hInst,
                    NULL);

    // Return if creating the month calendar failed. 
    if (hwndMonthCal == NULL)
        return HRESULT_FROM_WIN32(GetLastError()); 
                     
    // Get the size required to show an entire month.
    MonthCal_GetMinReqRect(hwndMonthCal, &rc);

    // Resize the control now that the size values have been obtained.
    SetWindowPos(hwndMonthCal, NULL, LEFT, TOP, 
        rc.right, rc.bottom, SWP_NOZORDER);

    // Set the calendar to the annual view.
    MonthCal_SetCurrentView(hwndMonthCal, MCMV_YEAR);

    // Make the window visible.
    ShowWindow(hwndDlg, SW_SHOW);

*/	
    return True;

}

#endif
#define BOLDDAY(ds, iDay)  \
    if (iDay > 0 && iDay < 32)(ds) |= (0x00000001 << (iDay - 1))


#ifdef _WIN32
// -----------------------------
INT_PTR CALLBACK MonthCalDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
/*	switch(msg)
	{
		case WM_NOTIFY:
		{
				if (((LPNMHDR)lParam)->code == MCN_GETDAYSTATE)
				{
					MONTHDAYSTATE rgMonths[12] = { 0 };
					int cMonths = ((NMDAYSTATE*)lParam)->cDayState;
					for (int i = 0; i < cMonths; i++)
					{
						BOLDDAY(rgMonths[i], 1);
						BOLDDAY(rgMonths[i], 15);
					}
					((NMDAYSTATE*)lParam)->prgDayState = rgMonths;
					return TRUE;
				}
				break;
		}
	}
*/
	return TRUE;
}

#endif
// ------------------------------------
time64_t cyiTime::GetFileTimeStamp(const char* pszFileName)
{
#ifdef _WIN32
	HANDLE hFile = CreateFileA(pszFileName, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile==INVALID_HANDLE_VALUE)
		return 0;

	FILETIME writeTime;
	
	if (::GetFileTime(hFile, 0, 0, &writeTime)==TRUE)
	{
		CloseHandle(hFile);
		SYSTEMTIME sys;
		if (FileTimeToSystemTime(&writeTime, &sys)==TRUE)
		{
			return MAKETIME64(sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
		}
	}
	
	CloseHandle(hFile);
#endif
	return 0;
}

// ------------------------------------
wchar_t* cyiTime::DateToTime(time64_t t, wchar_t* buffer, const char* pszFormat)
{
	return buffer;
}
