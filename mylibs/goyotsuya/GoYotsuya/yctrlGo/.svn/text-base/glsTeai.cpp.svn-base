#include "glsTeai.h"

cglsTeai::cglsTeai(int n)
:m_n(n)
,m_bIsAssistantMode(False)
{

}

cglsTeai::~cglsTeai(void)
{
}
#include "glsSplitButton.h"
#include "glsSpinBox.h"


// ------------------------------------
Bool cglsTeai::TrackPopup(HWND hWndParent, int x, int y, int n, Font* font)
{
	if (n!=-1)
		m_n=n;

	Bool res = CreatePopup(hWndParent, x, y, 100, 100, font, 0);

	cglsSplitButton* sbtn = (cglsSplitButton* )GetChild(2);

	if (sbtn->m_bIsBeforeMenuShow==True)
	{
		MSG msg;
		while(GetMessage(&msg, NULL, 0, 0))
		{
			if (msg.message==WM_MOUSEMOVE)
			{
				RECT rect;
				POINT pt={msg.wParam&0xFFFF, (msg.wParam>>16)&0xFFFF};
				ClientToScreen(msg.hwnd, &pt);
				GetWindowRect(m_hWnd, &rect);

				if (yiUtilPointIsInRect(&rect, pt.x, pt.y)==False)
				{
					continue;
				}
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (m_bIsExitLoop==True)
			{
				if (sbtn->m_bIsBeforeMenuShow==True)
					continue;

				Sleep(100);
				while (PeekMessage(&msg,NULL, 0, 0, PM_REMOVE)==True)
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				break;
			}
		}
	}

	OnOk(0, 0);

	DestroyWindow(m_hWnd);

	m_children->clear();
	return res;
}

// ------------------------------------
Bool cglsTeai::Create(cGlassInterface* parent, int id, int x, int y, int n, Font* font)
{
	m_bIsAssistantMode=False;

	if (n!=-1)
		m_n=n;
	m_DrawObject=this;

	sGlsDetail detail;
	cGlassInterface::xDefaultDetail(&detail);
	detail.font=font;

	Bool res = cGlassInterface::Create((HWND)parent, 0, id, 0, &detail);

	if (res==False)
	{
		return False;
	}

	Move(x, y);

	return True;
}


// ------------------------------------
Bool cglsTeai::OnOk(HWND hWnd, int code)
{
	cglsSplitButton* btn = (cglsSplitButton*)GetChild(2);
	int n = btn->GetSelectIndex();

	//if (n==2)
	//{
	//	cglsSpinBox* box = (cglsSpinBox*)GetChild(3);
	//	n = box->GetValue();
	//}

	m_bIsChanged=True;

	if (m_n==n)
	{
		m_bIsChanged=False;
	}

	m_n=n;

	m_bIsExitLoop=True;
	return True;
}

// ---------------------------------------------------
LRESULT cglsTeai::OnCreate(WPARAM wParam, LPARAM lParam)
{

	cglsSplitButton* sBtn = new cglsSplitButton();
	int h = GetFontHeight(True);

	if (h==0)
		h=23;

	sBtn->Create((HWND)this, 2, 0, 0, h*5, 0, 0);

	sBtn->AddItem(0, 0, L"ŒÝ‚¢æ", L"ŒÝ‚¢æ", True);
	sBtn->AddItem(1, 0, L"’èæ", L"’èæ");
	sBtn->AddItem(2, 0, L"’u‚«Œé", L"’u‚«Œé");
	sBtn->Show();
	sBtn->Update(eGUM_FORCED);

	cglsSpinBox* spinMin = new cglsSpinBox();
	spinMin->Create(this, 4, sBtn->m_w, 0, 2);
	spinMin->SetNumer(0, 2, 9, xMINUTE(m_n)%10);
	spinMin->SetNumer(1, 0, 1, xMINUTE(m_n)/10);

	cglsButton* btn = new cglsButton();
	sGlsButtonDetail s;
	cglsButton::xDefaultDetail(&s);

	s.rect.right=spinMin->m_x+spinMin->m_w;
	s.rect.bottom=0;

	//wchar_t buffer[128];

	//cyiTime::TimeToString(m_initTime, buffer, "YYYY-MM-DD");

	btn->Create((HWND)this, IDOK, 0, spinMin->m_h, 0, L"OK", 0, &s);
	btn->Update(eGUM_FORCED);
	btn->Show();

	ReSizeClientRect(s.rect.right, spinMin->m_h+btn->m_h);
	
	return 0;
}

// ------------------------------------
Bool cglsTeai::OnClose(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	EndDialog(m_hWnd, 0);
	return True;
}

#define BOLDDAY(ds, iDay)  \
    if (iDay > 0 && iDay < 32)(ds) |= (0x00000001 << (iDay - 1))


// ------------------------------------
LRESULT cglsTeai::OnNotify(WPARAM wParam, LPARAM lParam)
{
	return True;
}

// assitant interface
// -----------------------------------------------------
Bool cglsTeai::ShowAssistant(HWND hWnd, int x, int y)
{
	return TrackPopup(hWnd, x, y, -1);
	//return True;
}

// -----------------------------------------------------
Bool cglsTeai::GetResult(wchar_t* buffer)
{
	if (m_n==0)
	{
		wsprintfW(buffer, L"ŒÝ‚¢æ");
	}
	else if (m_n==1)
	{
		wsprintfW(buffer, L"’èæ");
	}
	else
	{
		wsprintfW(buffer, L"%dŽq‹Ç", m_n);
	}
	
	return True;
}

// -----------------------------------------------------
Bool cglsTeai::GetResult(unsigned int& result1, unsigned int& result2)
{
	result1 = m_n;
	return True;
}

// -----------------------------------------------------
Bool cglsTeai::GetDefaultValue(wchar_t* buffer)
{
	//wsprintfW(buffer, L"%02d:%02d:%02d", xHOUR(m_initTime), xMINUTE(m_initTime), xSECOND(m_initTime));//xYEAR64(m_initTime), xMONTH64(m_initTime), xDAY64(m_initTime));
	return GetResult(buffer);
}

// -----------------------------------------------------
Bool cglsTeai::GetDefaultValue(unsigned int& result1, unsigned int& result2)
{
	return GetResult(result1, result2);
}

// --------------------------
Bool cglsTeai::CreateBitmap(sDrawItem* item, void* pExtra)
{
	item->pBmp[1]=item->pBmp[2]=item->pBmp[0]=BITMAP32(m_w, m_h);
	item->rect.w=m_w;
	item->rect.h=m_h;
	return True;
}