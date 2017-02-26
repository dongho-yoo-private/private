#include "glsGobanContainer.h"

cglsGobanContainer::cglsGobanContainer(void)
{
	m_goban=0;
}

cglsGobanContainer::~cglsGobanContainer(void)
{
}

Bool cglsGobanContainer::SetStream(IStream* stream)
{
	if (m_goban==0)
		return False;
	if (m_goban->AddForStream(stream)==0)
		return False;

	InvalidateRect(0);
	return True;
}

// -------------------------------------
Bool cglsGobanContainer::ReSize(UINT x, UINT y, Bool bIsRepaint)
{
	//__super::ReSize(x, y, bIsRepaint);
	if (__super::ReSize(x, y, bIsRepaint)==True)
	{
		return True;//m_goban->Resize(x, y);
	}

	return False;
}


#include "glsButton.h"
// -------------------------------------
LRESULT cglsGobanContainer::OnCreate(WPARAM wParam, LPARAM lParam)
{

	if (m_w < 300 ||
			m_h < 300)
	{
		ReSize(300, 500);
	}

	m_goban= new cglsGoban();
	
	if (m_goban->CreateForSimple(this, m_hInstance, 1, 0, 0, m_w, m_h)==False)
	{
		Destroy();
		//DestroyWindow(*this);
		delete m_goban;
		return 0;
	}

	//cglsButton* btn = new cglsButton();
	//btn->Create((HWND)this, 2, 0, 0, 0, L"OK");
	//btn->Update(eGUM_FORCED);
	//btn->Show();
	return 0;
}

// -------------------------------------
LRESULT cglsGobanContainer::OnDestroy(WPARAM wParam, LPARAM lParam)
{
	delete m_font;
	m_font=0;

	if (m_goban)
	{
		m_goban->Destroy();
	}
	return 0;
}

// -------------------------------------
Bool cglsGobanContainer::OnBgDraw()
{
	if (xIS_GLS_FLAG(m_flag, eGF_NOT_OWNDRAW)==True)
	{
		return False;
	}

	if (m_bmpCurrent==0)
	{
		m_bmpCurrent=BITMAP32(m_w, m_h);
	}
	if (m_bg==0)
	{
		m_bg=BITMAP32(m_w, m_h);
	}

	int w=m_bmpCurrent->GetWidth();
	int h=m_bmpCurrent->GetHeight();

	if (w!=m_w || h!=m_h)
	{
		delete m_bmpCurrent;
		delete m_bg;
		m_bmpCurrent=BITMAP32(m_w, m_h);
		m_bg=BITMAP32(m_w, m_h);
	}

	Graphics g(m_bg);
	g.FillRectangle(&SolidBrush(Color::Beige), 0, 0, m_w, m_h);
	//g.DrawRectangle(&Pen(Color::Black), 0, 0, m_w-1, m_h-1);

	cyiBitmapBlender::BitmapCopy(m_bmpCurrent, m_bg);

	//NotifyBgChanged();

	//m_eUpdateReason=eGUR_BG_CHANGED;
	//Update(m_bmpCurrent, 0);
	InvalidateRect(0);
	return True;
}

// -------------------------------------
LRESULT cglsGobanContainer::OnCommand(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	handled=False;
	int id = (0xFFFF&wParam);
	switch(id)
	{
	}
	return 0;
}
