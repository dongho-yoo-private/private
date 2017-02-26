#include "glsComboBox.h"

cglsComboBox::cglsComboBox(void)
{
}

cglsComboBox::~cglsComboBox(void)
{
}

// ------------------------------------
void cglsComboBox::xDefaultDetail(sGlsComboBoxDetail* detail)
{
	cglsEditBox::xDefaultDetail(&detail->edit);
	cglsButton::xDefaultDetail(&detail->btn);
}

// ------------------------------------
Bool cglsComboBox::Create(HWND hWndParent, int id, int x, int y, int w, int h)
{
	m_hWnd = CreateWindow(
    "COMBOBOX", "", 
    WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS| WS_TABSTOP | CBS_HASSTRINGS|CBS_AUTOHSCROLL|CBS_DROPDOWN,
	x, y, w, h, hWndParent, (HMENU)id, ::GetModuleHandleA(0),NULL);

	return m_hWnd ? True:False;
}

// ------------------------------------
Bool cglsComboBox::Create(HWND hWndParent, int id, int x, int y, int w, int h, sGlsComboBoxDetail* detail)
{
	sGlsComboBoxDetail xdetail;

	if (detail)
		xdetail=*detail;
	else
	{
		xDefaultDetail(&xdetail);
	}

	RECT rect = {x, y, w, h};
	sGlsStackPannelDetail stack;
	cglsStackPannel::xDefaultDetail(&stack);
	//stack.bUseGaussPannel=True;
	//stack.gauss.fBlureRad=0.0;
	//stack.gauss.fShadowWidth=2.0;
	//stack.gauss.colorTop=0;
	//stack.gauss.colorBottom=0;
	if (cglsStackPannel::Create(hWndParent, id, &rect, 0, this, &stack)==False)
	{
		return False;
	}


	//Bitmap temp(1, 1, PixelFormat32bppARGB);
	//Graphics g(&temp);
	//h=xdetail.gls.font->GetHeight(&g)+(xTEXT_MARGINE<<1);
	
	m_colorBtn=xdetail.btn.colorHilight;

	// edit box‚ðì¬
	m_edit = new cglsEditBox();
	m_edit->Create((HWND)this, 1, 0, 0, w, 0, &xdetail.edit);
	m_edit->Show();
	Add(m_edit, 0, eSA_CENTER);

	Add(2, 0, 0, 0, 0);

	Update(eGUM_FORCED);
	//Show();
	return True;
}

// ------------------------------------
Bool cglsComboBox::AddString(const char* pszStr)
{
	SendMessage(m_hWnd, CB_ADDSTRING, 0, (LPARAM)pszStr);
	return True;
}

// ------------------------------------
Bool cglsComboBox::SetSelectIndex(int n)
{
	SendMessage(m_hWnd, CB_SETCURSEL, n, 0);
	return True;
}

// ------------------------------------
int cglsComboBox::GetSelectIndex()
{
	return SendMessage(m_hWnd, CB_GETCURSEL, 0, 0);
}

#include "glsResourceBitmap.h"
// ------------------------------------
Bool cglsComboBox::CreateBitmap(sDrawItem* item, void* param/*=0*/)
{
	if (item->id==-1)
	{
		//item->pBmp[0]=BITMAP32(0, 0, m_w, m_h);
		//item->pBmp[1]=cyiShape::DrawRoundRect(&Pen(Color::Orange), RectF(0, 0, m_w-1, m_h-1), 2.0);
		//item->pCurrentBmp=0;
		return False;
	}
	if (item->id==2)
	{
		cglsResourceBitmap* resource=(cglsResourceBitmap*)cGlassInterface::GetDefaultResource();
		int w = (int)((float)m_edit->m_h*0.5);
		Bitmap* bmp = resource->GetBitmap(eGRID_ARROW_DOWN, (int)Color::Black, w, w);
		
		sGlsButtonDetail detail;
		cglsButton::xDefaultDetail(&detail);
		detail.colorHilight=m_colorBtn;
		detail.rect.right=m_edit->m_h;
		detail.rect.bottom=m_edit->m_h;
		item->pBmp[0] = resource->Draw(eGDID_GLASS_BTN_PANNEL, 0, 0, m_edit->m_h, bmp, 0, &detail);
		item->pBmp[1] = resource->Draw(eGDID_GLASS_BTN_PANNEL, 1, 0, m_edit->m_h, bmp, 0, &detail);
		item->pBmp[2] = resource->Draw(eGDID_GLASS_BTN_PANNEL, 2, 0, m_edit->m_h, bmp, 0, &detail);

		item->pCurrentBmp=item->pBmp[0];

		item->rect.w=item->pBmp[0]->GetWidth();
		item->rect.h=item->pBmp[1]->GetWidth();
		//delete bmp;
		return True;
	}

	return False;
}