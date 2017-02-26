#include "yistd.h"
#include "glsBaloon.h"

// --------------------------------
cglsBaloon* xcglsBaloon()
{
	return new cglsBaloon();
}

cglsBaloon::cglsBaloon(void)
: m_bg(0)
, m_comment(0)
{
	m_pt.x=0;
	m_pt.y=0;
}

cglsBaloon::~cglsBaloon(void)
{
}

void cglsBaloon::xDefaultDetail(sGlsBaloonDetail* detail)
{
	cGlassInterface::xDefaultDetail(&detail->gls);
	detail->fShadowDepth=3.0;
	detail->bg=0;
}

// -----------------------------
Bool cglsBaloon::Create(cGlassInterface* parent, int id, int x, int y, const wchar_t* pszContext, Bool bIsUp, sGlsBaloonDetail* detail)
{
	sGlsBaloonDetail xdetail;

	if (detail)
	{
		xdetail=*detail;
	}
	else
	{
		xDefaultDetail(&xdetail);
	}

	wcscpy(m_wszContext, pszContext);
	
	//if (xdetail.bg==0)
	//	m_bg = new SolidBrush(Color::Beige);

	m_fShadowDepth=xdetail.fShadowDepth;

	m_bIsArrowUp=bIsUp;

	m_pt.x=x;
	m_pt.y=y;
	
	RECT rect = {x, y, 10, 10};
	if (cGlassInterface::Create((HWND)parent, 0, id, &rect, &xdetail.gls)==False)
	{
		return False;
	}
	return True;
}

// -----------------------------
Bool cglsBaloon::Create(cGlassInterface* parent, int id, int x, int y, const char* pszContext, Bool bIsUp, sGlsBaloonDetail* detail)
{
	wchar_t* p = ::yiToUnicode(0, pszContext, 0);
	Bool res = Create(parent, id, x, y, p, bIsUp, detail);
	yifree(p);
	return res;
}

// -----------------------------
void cglsBaloon::GetBaloonPos(POINT& pt)
{
	pt.x=m_pt.x;
	pt.y=m_pt.y;
}


// -----------------------------
void cglsBaloon::Destroy()
{
	DELETEBMP(m_bg);
	m_bg=0;

	__super::Destroy();
}

// -----------------------------
LRESULT cglsBaloon::OnMouseEnter(WPARAM wParam, LPARAM lParam)
{
	SendMessage(m_hNotifyWnd, WM_COMMAND, (1<<16)|m_id, (LPARAM)this);
	return 0;
}


// -----------------------------
LRESULT cglsBaloon::OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	PostMessage(m_hNotifyWnd, WM_COMMAND, m_id, (LPARAM)this);
	return 0;
}

// -----------------------------
Bool cglsBaloon::SetText(const wchar_t* pszText, Bool bIsUp)
{
	m_bIsArrowUp=bIsUp;
	if (pszText==0)
	{
		m_wszContext[0]=0;
	}
	else
	{
		wcscpy(m_wszContext, pszText);
	}

	m_comment->SetText(0, m_wszContext);
	m_comment->Update(eGUM_FORCED);

	DELETEBMP(m_bg);
	m_bg=0;
	return InvalidateRect(0);
}

// -----------------------------
int cglsBaloon::GetText(wchar_t* pszText, int bufferSize)
{
	if (pszText==0)
	{
		return wcslen(m_wszContext);
	}

	wcsncpy(pszText, m_wszContext, bufferSize);
	int n = wcslen(m_wszContext);
	pszText[n]=0;
	return n;
}


// -----------------------------
void cglsBaloon::Show()
{
	if (m_wszContext[0]==0)
	{
		Hide();
		return ;
	}

	__super::Show();
}



// -----------------------------
LRESULT cglsBaloon::OnCreate(WPARAM wParam, LPARAM lParam)
{
	m_comment = xcglsStatic();
	int nMaxWidth = GetFontHeight(True)*10;

	sGlsStaticDetail detail;
	cglsStatic::xDefaultDetail(&detail);
	detail.nFixedWidth=nMaxWidth;
	m_comment->Create((HWND)this, m_fShadowDepth, m_fShadowDepth, 0, 0, 0, m_wszContext, &detail);
	m_comment->Update(eGUM_FORCED);
	m_comment->Show();
	return 0;
}

// -----------------------------
Bool cglsBaloon::OnDrawMyself(Bitmap* bmp, RECT* rect, RECT* childRect, int offsetX, int offsetY)
{
	if (m_bg!=0 &&
		m_bg->GetWidth()==m_w && m_bg->GetHeight()==m_h)
	{
		return False;
	}

	if (m_bg)
	{
		DELETEBMP(m_bg);
	}

	m_pt.x=m_x;
	m_pt.y=m_y;


	color_t colorPen = Color::Black;
	color_t colorBg  = Color::Beige;

	int w = m_comment->m_w+GetFontHeight(True);
	int h = m_comment->m_h+GetFontHeight(True);//+FontHeight(True)>>2;

	int xOffset=0;
	int yOffset=0;

	if (w<(GetFontHeight(True)<<2))
	{
		w = GetFontHeight(True)<<2;
	}

	Bitmap* bg=cyiShape::DrawRoundRectEx(0, &Pen(Color(colorPen)), &SolidBrush(Color(colorBg)), RectF(0, 0, w, h), 2.0);

	Bitmap* tri=cyiShape::DrawTriAngleEx(0, m_bIsArrowUp?2:0, &Pen(Color(colorPen)), &SolidBrush(Color(colorBg)), RectF(0, 0, GetFontHeight(True), GetFontHeight(True)));

	Bitmap* ret = BITMAP32(w, h+tri->GetHeight()-1);

	RECT rectParent;
	m_parent->GetClientRect(&rectParent);

	int bgWidth=ret->GetWidth()+1;
	int bgHeight=ret->GetHeight()+1;
	int xBaloonPos = m_x-(bgWidth>>1);

	int xTry=(w-tri->GetWidth())>>1;

	if (rectParent.right<(xBaloonPos+bgWidth))
	{
		int prevBaloonPos=xBaloonPos;
		xBaloonPos = rectParent.right-bgWidth;
		xTry+=(prevBaloonPos-xBaloonPos);
	}
	else if (xBaloonPos<0)
	{
		int prevBaloonPos=xBaloonPos;
		xBaloonPos=0;
		xTry+=prevBaloonPos;
	}


	//cyiBitmapBlender::FillColor(ret, 0);
	if (m_bIsArrowUp==False)
	{
		cyiBitmapBlender::BitmapCopy(ret, bg, 0, 0, 0, 0, w, h);
		cyiBitmapBlender::BitmapCopy(ret, tri, xTry, h-1, 0, 1, tri->GetWidth(), tri->GetHeight()-1);
	}
	else
	{
		cyiBitmapBlender::BitmapCopy(ret, bg, 0, ret->GetHeight()-h, 0, 0, w, h);
		cyiBitmapBlender::BitmapCopy(ret, tri, xTry, 1, 0, 0, tri->GetWidth(), tri->GetHeight()-1);
	}

	m_bg = cyiBitmapBlender::DropShadowEx(ret, 1, 1, m_fShadowDepth, 0.3);



	if (m_bIsArrowUp==False)
	{
		m_comment->Move(m_fShadowDepth+2.0+(GetFontHeight(True)>>1), m_fShadowDepth+2.0);
	}
	else
	{
		m_comment->Move(m_fShadowDepth+2.0+(GetFontHeight(True)>>1), m_fShadowDepth+2.0+tri->GetHeight()+(GetFontHeight()>>1));
	}
	DELETEBMP(bg);
	DELETEBMP(tri);
	DELETEBMP(ret);

	ReSize(m_bg->GetWidth(), m_bg->GetHeight());

	Move(xBaloonPos/*m_x-(m_bg->GetWidth()>>1)*/, m_bIsArrowUp?m_y:m_y-m_bg->GetHeight());

	return False;
}

// -----------------------------
Bitmap* cglsBaloon::GetBgBitmap()
{
	return m_bg;
}