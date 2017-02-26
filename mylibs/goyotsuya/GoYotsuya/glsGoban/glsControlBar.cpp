#include "glsControlBar.h"
#include "yistd.h"

cglsControlBar::cglsControlBar(void)
: m_slide(0)
, m_scroll(0)
{
	__GLASSINTERFACE_NAME__;
}

cglsControlBar::~cglsControlBar(void)
{
}


// -------------------------------------------
Bool cglsControlBar::Create(cGlassInterface* parent, int id, Font* font, eGlsItemAlign align, eGlsGuiStyle style, sGlsStackPannelDetail* detail)
{
	sGlsStackPannelDetail stack;

	cglsStackPannel::xDefaultDetail(&stack);

	if (detail==0)
	{
		stack.bIsFixedMode=True;
		stack.bIsShowScrollBar=False;
		stack.gls.font=font;
		stack.gauss.fBlureRad=5.0;
		stack.gauss.fShadowWidth=0.0;
		stack.gauss.colorTop=__COLORp__(127, 0, 0, 0);
		stack.gauss.colorBottom=__COLORp__(200, 0, 0, 0);
		stack.bUseGaussPannel=True;
	}
	else
	{
		stack = *detail;
	}
	
	stack.bIsFixedMode=True;
	stack.bIsReSizeManual=True;
	stack.gls.style=style;

	m_font=font;

	stack.gls.bIsAnimationShow=False;
	m_align = align;
	return cglsStackPannel::Create((HWND)parent, id, 0, 0, 0, &stack);
}

// -------------------------------------------
LRESULT cglsControlBar::OnCreate(WPARAM wParam, LPARAM lParam)
{
	m_slide = new cglsSlide();
	m_slide->Create(this, 1, 0, 0, m_w, m_h);
	m_slide->Show();

	Add(m_slide, 0, eSA_CENTER);

	return 0;
}

// -------------------------------------------
int cglsControlBar::AddPage(cGlassInterface* obj)
{
	m_slide->AddControl(obj, eGIA_CENTER);
	return True;
}

// -------------------------------------------
int cglsControlBar::SetPageCount(int n)
{
	if (n>1)
	{
		if (m_scroll==0)
		{
			m_scroll = new cglsPageScroll();
			Font* font = (Font*)cyiShape::Clone(GetFont(), 0.6);
			m_scroll->Create(this, 2, 0, 0, font, n, True, 0);
			m_scroll->Show();
		}

		if (GetChild(2)==0)
		{
			AddLF(0);
			Add(m_scroll, 0, eSA_CENTER);
		}
	}
	else
	{
		if (m_scroll)
		{
			Remove(m_scroll->m_id, False);
		}
	}

	return 0;
}


// -------------------------------------------
cGlassInterface* cglsControlBar::GetPage(int id)
{
	return m_slide->GetControl(id);
}

// -------------------------------------------
Bool cglsControlBar::ShowPage(int id, eSlideBarScrollDirection dir)
{
	return m_slide->Scroll(id, dir, eGAP_FISO);//, 10, 500);
}


// -------------------------------------------
Bool cglsControlBar::ReSize(uInt32 x, uInt32 y, Bool bIsRepaint)
{
	__super::ReSize(x, y, bIsRepaint);

	int xPos=0, yPos=0;

	if (m_align==eGIA_BOTTOM)
	{
		yPos=m_parent->m_h-m_h;
	}
	else if (m_align==eGIA_CENTER)
	{
		yPos=(m_parent->m_h-m_h)>>1;
	}

	xPos=(m_parent->m_w-m_w)>>1;

	RECT rect;
	GetClientRect(&rect);
	m_slide->ReSize(rect.right, rect.bottom);

	return Move(xPos, yPos);
}

#include "yiShape.h"

// ------------------------------------
Bool cglsControlBar::OnDrawMyself(Bitmap* bmp, RECT* rect, RECT* childRect, int offsetX, int offsetY)
{

	if (m_bg==0 ||
			(m_bg->GetWidth()!=m_w || m_bg->GetHeight()!=0))
	{
		goto FORCED;
	}

	return False;

FORCED:
	if (m_bg)
		DELETEBMP(m_bg);

	m_bg = BITMAP32(m_w, m_h);

	Bitmap* bmpFrame;// = cyiShape::DrawRoundRectEx(0, &Pen(
	float m_fRadius=m_fEdge;

	if (m_guiStyle==eGGS_NEON_WHITE)
	{
		LinearGradientBrush brush(Point(0, m_align==eGIA_TOP?m_h:0), Point(0, m_align==eGIA_TOP?0:m_h), Color(180, 255, 255, 255), Color::White); 

		bmpFrame = cyiShape::DrawRoundRectEx(0, &Pen(Color::Gray, 1.0), &brush, RectF(0, 0, m_w, m_h+m_fRadius), m_fRadius);

		Graphics g(m_bg);

		if (m_align==eGIA_TOP)
		{
			g.DrawImage(bmpFrame, (INT)0, (INT)0, 0, (INT)m_fRadius, m_bg->GetWidth(), m_bg->GetHeight(), Unit::UnitPixel);
		}
		else
		{
			g.DrawImage(bmpFrame, 0, 0);
		}

		delete bmpFrame;
	}
	else
	{
		LinearGradientBrush brush(Point(0, m_align==eGIA_TOP?m_h:0), Point(0, m_align==eGIA_TOP?0:m_h), Color(128, 0, 0, 0), Color::Black); 
		bmpFrame = cyiShape::DrawRoundRectEx(0, &Pen(Color::DarkGray, 1.0), &brush, RectF(0, 0, m_w, m_h+m_fRadius), m_fRadius);
		Bitmap* bmpSubFrame = cyiShape::DrawRoundRectEx(0, 0, &brush, RectF(0, 0, m_rectMargin.right, m_rectMargin.bottom), m_fRadius);

		Graphics g(m_bg);

		if (m_align==eGIA_TOP)
		{
			g.DrawImage(bmpFrame, (INT)0, (INT)0, 0, (INT)m_fRadius, m_bg->GetWidth(), m_bg->GetHeight(), Unit::UnitPixel);
		}
		else
		{
			g.DrawImage(bmpFrame, 0, 0);
		}

		delete bmpFrame;
	}

	return False;
}

// -----------------------------
Bitmap* cglsControlBar::GetBgBitmap()
{
	return m_bg;
}