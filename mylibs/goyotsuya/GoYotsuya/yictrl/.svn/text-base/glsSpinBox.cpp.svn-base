#include "glsSpinBox.h"

cglsSpinBox::cglsSpinBox(void)
: m_sumWidth(0)
, m_labelWidth(0)
, m_fMin(0.0)
, m_fMax(0.0)
, m_bmpCover(0)
, m_fNextMax(0.0)
, m_nAfterValue(0)
{
	__GLASSINTERFACE_NAME__;
}

cglsSpinBox::~cglsSpinBox(void)
{
	cGlassInterface::~cGlassInterface();
}

#define xLABEL_START_ID 10

//typedef struct {
//	int w;
//	int distance;
//} sSpinItemInfo;

// -----------------------------
void cglsSpinBox::xDefaultDetail(sGlsSpinBoxDetail* detail)
{
	detail->bgBrush=0;//new SolidBrush(Color::White);
	detail->colorBg=Color::White;
	detail->colorFont=Color::Black;
	detail->bIsShowButton=True;
	detail->fEdge=2.0;
	detail->eMode=eGPBSM_NONLINEAR;
	cGlassInterface::xDefaultDetail(&detail->gls);
	detail->gls.bIsNoMouseMove=True;
	detail->gls.nKeyLButtonDownTimer=500;
	detail->gls.nKeyRepeatInterval=300;
	detail->fScrollTerm=1.0;
}

#include "glsStatic.h"
// -----------------------------
Bitmap* cglsSpinBox::xCreateBmpCover(int width, int nFontHeight)
{
	int btnW=(m_btn?m_btn->m_w:0);
	int w = width-btnW;


	if (m_bmpCover==0 ||
		m_bmpCover->GetWidth()!=w ||
			m_bmpCover->GetHeight()!=nFontHeight)
	{
		if (m_bmpCover)
			delete m_bmpCover;
		m_bmpCover = BITMAP32(w, nFontHeight);
	}
	else
	{
		cyiBitmapBlender::FillColor(m_bmpCover, 0);
	}

	//return m_bmpCover;
	cGlassInterface* child=(cGlassInterface*)GetChild(xLABEL_START_ID-1);
	w-=child?child->m_w:0;

	Graphics g(m_bmpCover);
	{
		//g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		//g.DrawRectangle(&Pen(Color::Black), 0, 0, w-1, nFontHeight-1);
	
		int x=w-1;
		for (int i=0; i<m_nSpinCount; i++)
		{
			if (m_list[i]==0)
				continue;

			cglsStatic* p = (cglsStatic*)GetChild(xLABEL_START_ID+i);

			if (p)
			{
				//g.DrawLine(&Pen(Color::DarkGray, 0.5), (float)x+1/*nFontHeight*i*/, (float)1+1, (float)x/*nFontHeight*i*/, (float)nFontHeight-1);
				if (i!=0)
					g.DrawLine(&Pen(Color::Black, 0.5), (float)x+0.5, (float)1, (float)x+0.5, (float)nFontHeight-1);
				x-=p->m_w;

				//g.DrawLine(&Pen(Color::DarkGray, 0.5), (float)p->m_x/*nFontHeight*i*/, (float)1, (float)p->m_x/*nFontHeight*i*/, (float)nFontHeight-1);
				//g.DrawLine(&Pen(Color::Black, 0.5), (float)p->m_x/*nFontHeight*i*/+0.5, (float)1, (float)p->m_x/*nFontHeight*i*/+0.5, (float)nFontHeight-1);
			}

			int dx=m_list[i]->GetWidth();

			g.DrawLine(&Pen(Color::DarkGray, 0.5), (float)x/*nFontHeight*i*/, (float)1, (float)x/*nFontHeight*i*/, (float)nFontHeight-1);
			g.DrawLine(&Pen(Color::Black, 0.5), (float)x/*nFontHeight*i*/+0.5, (float)1, (float)x/*nFontHeight*i*/+0.5, (float)nFontHeight-1);
			g.DrawLine(&Pen(Color::Black, 0.5), (float)x, (float)0, (float)x-dx, (float)0);
			g.DrawLine(&Pen(Color::Black, 0.5), (float)x, (float)nFontHeight-1, (float)x-dx, (float)nFontHeight-1);
			g.FillRectangle(&LinearGradientBrush(Point(0, 0), Point(0, nFontHeight>>1), Color(0, 0, 0, 0), Color(32, 0, 0, 0)), x-dx, (nFontHeight>>1)+1, dx, (nFontHeight>>1)-1);
			g.FillRectangle(&LinearGradientBrush(Point(0, 0), Point(0, nFontHeight>>1), Color(16, 0, 0, 0), Color(0, 0, 0, 0)), x-dx, 0, dx, nFontHeight>>1);


			/*if ((nFontHeight<<1)%2==0)
			{
				int end=nFontHeight%2?nFontHeight-1:nFontHeight;
				g.FillRectangle(&LinearGradientBrush(Point(0, (nFontHeight>>1)), Point(0, end), Color(0, 0, 0, 0), Color(32, 0, 0, 0)), x-dx, (nFontHeight>>1), dx, (nFontHeight>>1));
			}
			else
			{
				int end=nFontHeight%2?nFontHeight-1:nFontHeight;
				g.FillRectangle(&LinearGradientBrush(Point(0, (nFontHeight>>1)+1), Point(0, end), Color(0, 0, 0, 0), Color(32, 0, 0, 0)), x-dx, (nFontHeight>>1)+1, dx, (nFontHeight>>1)-1);
			}*/
			x-=dx;

			if (i==m_nSpinCount-1)
			{
				g.DrawLine(&Pen(Color::Black, 0.5), (float)0, (float)1, (float)0, (float)nFontHeight-1);
			}
		}
		//x=1;
		//g.DrawLine(&Pen(Color::DarkGray, 0.5), (float)0/*nFontHeight*i*/, (float)1, (float)0/*nFontHeight*i*/, (float)nFontHeight-1);

	}

	return m_bmpCover;
}

// -----------------------------
void cglsSpinBox::xReSize(int h)
{
	ReSize(m_sumWidth+m_labelWidth+(m_btn?m_btn->m_w:0), h);

	cGlassInterface* endLabel=GetChild(xLABEL_START_ID-1);
	if (endLabel)
	{
		endLabel->Move(m_sumWidth+(m_btn?m_btn->m_w:0), m_h-endLabel->m_h);
	}

	if (m_btn)
	{
		if (endLabel)
		{
			endLabel->AddPositionLink(m_btn, eGP_LEFTSIDE|eGP_TOP);
		}
		else
			m_btn->Move(m_sumWidth+m_labelWidth, m_btn->m_y);
	}



	//if (m_bmpCover)
	//{
	//	delete m_bmpCover;
	//}
	m_bmpCover=xCreateBmpCover(m_w, m_h);

	if (m_bmpCurrent)
		delete m_bmpCurrent;

	m_bmpCurrent=BITMAP32(m_w, m_h);
	m_item.pCurrentBmp=m_bmpCurrent;

	//ReSize(width, nFontHeight);
	//m_bmpCurrent = BITMAP32(width, nFontHeight);
	//

	//m_bmpCover = xCreateBmpCover(m_w, nFontHeight);
}

// -----------------------------
Bool cglsSpinBox::Create(cGlassInterface* parent, int id, int x, int y, int nSpinCount, sGlsSpinBoxDetail* detail)
{
	sGlsSpinBoxDetail xdetail;

	if (detail)
		xdetail=*detail;
	else
		xDefaultDetail(&xdetail);

	RECT rect = {x, y, 0, 0};

	m_nSpinCount=nSpinCount;
	m_bIsShowButton=xdetail.bIsShowButton;
	m_brush=xdetail.bgBrush;
	m_colorFont=xdetail.colorFont;
	m_colorBg=xdetail.colorBg;
	m_eMode=xdetail.eMode;
	Bool res = cGlassInterface::Create((HWND)parent, 0, id, &rect, &xdetail.gls);

	if (res==False)
	{
		goto CREATE_ERROR;
	}

	m_btn=0;

	int nFontHeight = GetFontHeight(True);//m_font->GetHeight(__DEFAULTGRAPHICS__);
#if 0
	if (m_bIsShowButton)
	{
		m_btn = new cglsToolButton();
		sGlsToolButtonDetail toolDetail;
		cglsToolButton::xDefaultDetail(&toolDetail);
		toolDetail.stack.bIsVertical=True;
		toolDetail.stack.bIsFixedMode=True;
		toolDetail.stack.gls.nKeyLButtonDownTimer=500;
		toolDetail.stack.gls.nKeyRepeatInterval=300;
		toolDetail.fScale=0.6;
		toolDetail.stack.gauss.fEdge=0.0;
		toolDetail.stack.gls.nKeyLButtonDownTimer=xdetail.gls.nKeyLButtonDownTimer;

		res = m_btn->Create(this, 1, 0, 0, nFontHeight/3, nFontHeight, &toolDetail);
		m_btn->Add(eGTBT_AUP, eGTBSS_FILL);
		m_btn->Add(eGTBT_ADOWN, eGTBSS_FILL);
		
		if (res==False)
			goto CREATE_ERROR;

		m_btn->Show();
		m_btn->Update(eGUM_FORCED);
	}
#endif


	int width = nFontHeight*nSpinCount+(m_btn?m_btn->m_w:0);

	m_list=(cglsImageList**)yialloci(sizeof(cglsImageList*)*nSpinCount);
	m_scrinfo=(sScrollAnitInfo**)yialloci(sizeof(sScrollAnitInfo*)*nSpinCount);

	ReSize(width, nFontHeight);

	m_bmpCurrent = BITMAP32(width, nFontHeight);
	m_item.pCurrentBmp=m_bmpCurrent;

	m_bmpCover = xCreateBmpCover(m_w, nFontHeight);

	

	//if (m_btn)
	//	m_btn->Move(width-m_btn->m_w-1, 0);

	Show();
	InvalidateRect(0);

	return True;

CREATE_ERROR:
	if (m_brush)
		delete m_brush;

	return False;

}

static sScrollAnitInfo* xCreateScrollAnitInfo()
{
	sScrollAnitInfo* s= (sScrollAnitInfo* )yialloci(sizeof(sScrollAnitInfo));
	s->fCurrentPos=0.0;
	return s;
}

// -----------------------------
Bool cglsSpinBox::SetNumer(int n, int min, int max, int nDefaultValue, int shift)
{
	if (n>=m_nSpinCount)
	{
		return False;
	}

	if (min>max)
		return False;

	int nFontHeight=GetFontHeight(True);

	cglsImageList* list = m_list[n]=new cglsImageList(max-min+1, True);
	
	m_scrinfo[n] = xCreateScrollAnitInfo();
	m_scrinfo[n]->hScrollAnimation=yiAnitCreate();
	m_scrinfo[n]->nShift=shift;

	int width=0;
	int height=0;
	for (int i=min; i<max+1; i++)
	{
		wchar_t str[32];
		wsprintfW(str, L"%d", i%10);
		//Bitmap* bmp = cyiShape::BitmapStringEx(0, str, GetFont(), m_colorFont, m_colorBg, eSTS_NONE, DT_CENTER|DT_VCENTER);
		list->Add(str, GetFont(), m_colorFont, m_colorBg);
		//width=width<bmp->GetWidth()?bmp->GetWidth():width;
	}
	width=list->GetWidth();
	height=list->GetHeight();

	width+=4;
	width = width>nFontHeight?width:nFontHeight;
	list->SetBitmapSize(width, list->GetHeight(), m_colorBg);
	m_sumWidth+=width;

	xReSize(height);

	//ReSize(m_sumWidth+(m_btn?m_btn->m_w:0), m_h);
	//ReSize(m_sumWidth+m_labelWidth+(m_btn?m_btn->m_w:0) , m_h);

	m_scrinfo[n]->fBeforePos=m_scrinfo[n]->fCurrentPos=nDefaultValue;

	InvalidateRect(0);

	return True;
}
#include "glsStatic.h"
// -----------------------------
void cglsSpinBox::InsertLabel(const wchar_t* label, int index, color_t fntColor)
{
	cglsStatic* staticLabel = new cglsStatic();
	sGlsStaticDetail detail;

	cglsStatic::xDefaultDetail(&detail);
	detail.colorFont=fntColor;
	detail.id=xLABEL_START_ID+index;
	staticLabel->Create((HWND)this, 0, 0, 0, 0, 0, label, &detail);
	m_labelWidth+=staticLabel->m_w;
	staticLabel->Show();
	xReSize(m_h);

	//ReSize(m_sumWidth+m_labelWidth+(m_btn?m_btn->m_w:0) , m_h);
	InvalidateRect(0);
}


// -----------------------------
void cglsSpinBox::SetLimited(int n, int no)
{
	m_scrinfo[n]->fLimited=(float)no;

	m_list[n]->SetLimited(no);
}

// -----------------------------
void cglsSpinBox::SetTotalLimited(float from, float to)
{
	m_fMax=to;
	m_fMin=from;

	m_fNextMax=m_fMax+xGetFactorIndex(m_nSpinCount-1);
}

// ------------------------------
Bool cglsSpinBox::AddString(int n, wchar_t* str, int number, Bool bIsDefault)
{
	int nPrevWidth=0;
	if (n>m_nSpinCount)
	{
		return False;
	}

	int nFontHeight=GetFontHeight(True);

	cglsImageList* list=m_list[n];
	
	if (list==0)
	{
		list = m_list[n] = new cglsImageList(0, True);
	}

	if (m_scrinfo[n]==0)
	{
		m_scrinfo[n] = xCreateScrollAnitInfo();
		m_scrinfo[n]->hScrollAnimation=yiAnitCreate();
	}

	//int pos = (int)cyiShape::BitmapStringEx(0, str, m_font, 0, 0, eSTS_NONE, DT_CENTER|DT_VCENTER);
	//int width = (pos>>16)+4;


	//Bitmap* bmp = cyiShape::BitmapStringEx(0, str, GetFont(), m_colorFont, m_colorBg, eSTS_NONE, DT_CENTER|DT_VCENTER);
	//list->Add(bmp);
	nPrevWidth=list->GetWidth();
	list->Add(str, GetFont(), m_colorFont, m_colorBg);
	
	if (bIsDefault==True)
	{
		if (n==0)
		{
			m_scrinfo[0]->fCurrentPos=number;
		}
		else {
			m_scrinfo[0]->fCurrentPos=(number*m_list[n-1]->count());
		}
	}

	int height=m_h>list->GetHeight()?m_h:list->GetHeight();
	int width = list->GetWidth()+4;
	width = width>nFontHeight?width:nFontHeight;
	list->SetBitmapSize(width, list->GetHeight(), m_colorBg);
	
	m_sumWidth+=(list->GetWidth()-nPrevWidth);

	//ReSize(m_sumWidth+(m_btn?m_btn->m_w:0), m_h);

	xReSize(height);
	

	return True;
}

// ------------------------------
Bool cglsSpinBox::Clear(int n)
{
	return True;
}

// ------------------------------
//Bool cglsSpinBox::ScrollTo(float fIndex, int nTerm)
//{
//	m_scrinfo[0]->fTerm=fIndex;
//	xScroll(0,nTerm);
//	return True;
//}

// ------------------------------------
Bool cglsSpinBox::IsNowAnimation()
{
	for (int i=0; i<m_nSpinCount; i++)
	{
		if (m_scrinfo[i]->hScrollAnimation)
		{
			if (yiAnitIsAlived(m_scrinfo[i]->hScrollAnimation)==True)
			{
				return True;
			}
		}
	}
	return False;
}


// ------------------------------
IntPtr cglsSpinBox::GetValue(int nIndex)
{
	int n = (int)m_scrinfo[0]->fCurrentPos;
	if (nIndex==-1)
	{
		int sum=0;
		for (int i=0; i<m_nSpinCount; i++)
		{
			sum+=(IntPtr)m_list[i]->Get(m_scrinfo[i]->fCurrentPos, -1)*xGetFactorIndex(i);
		}

		return sum;
	}

	return (IntPtr)m_list[nIndex]->Get(m_scrinfo[nIndex]->fCurrentPos, -1);

	//eturn xGetValue(nIndex);
	//int nMax=(m_list[nIndex]->count())*xGetFactorIndex(nIndex);

	//if (nIndex==-1 ||
	//		nIndex==0)
	//{
	//	return n%((int)nMax);
	//}
	//int result = xGetFactor(m_scrinfo[0]->fCurrentPos, nIndex);
	//return result%nMax;

}

// ------------------------------
//static float xMode(float value, int n)
//{
//	if (value==0.0)
//		return 0.0;
//	if (value<0)
//	{
//
//	}
//
//	if (value<(float)n)
//	{
//		return value;
//	}
//
//	value/(float)n;
//}
#include <math.h>

// ------------------------------
float cglsSpinBox::xGetFactor(float f, int index)
{
	for (int i=index-1; i>=0; i--)
	{
		f=f/m_list[i]->count();
	}
	int n = (int)f/m_list[index]->count();
	f = f-(float)(n*m_list[index]->count());

	//if (f<0.0)
	//{
	//	f+=-1;
	//}

	return f;
}

// ------------------------------
float cglsSpinBox::xGetFactorIndex(int index)
{
	float factor=1.0;
	for (int i=index-1; i>=0; i--)
	{
		factor=factor*(float)m_list[i]->count();
	}
	return factor;
}

// ------------------------------
void cglsSpinBox::Update(Bitmap* bmp, RECT* rect, RECT* _childRect, int offsetX, int offsetY, Bool bIsLocalBufferUpdate)
{
	//return ;
	int nFontHeight=GetFontHeight(True);
	Bool bIsNextAnimation=False;

	RECT rectSpin = {0, 0, m_nSpinCount*nFontHeight, nFontHeight};
	RECT retRect;
	if (xIsInRect(&rectSpin, _childRect, retRect)==False)
	{
		__super::Update(bmp, rect, _childRect, offsetX, offsetY, bIsLocalBufferUpdate);
		return ;
	}

	int x=0;
	int btnWidth=m_btn?m_btn->m_w:0;
	int labelWidth=0;
		
	cGlassInterface* child=GetChild(xLABEL_START_ID-1);

	x+=child?child->m_w:0;

	for (int i=0; i<m_nSpinCount; i++)
	{
		//int x; = (m_nSpinCount-i-1)*nFontHeight;

		if (m_list[i]==0)
			continue;

		if (m_list[i]->count()==0)
			continue;

		int max = m_list[i]->count();
		
		float factor;

		factor = m_scrinfo[i]->fCurrentPos;//xGetFactor(m_scrinfo[i]->fCurrentPos, i);

		m_scrinfo[i]->bIsInit=True;

		Bitmap* bmpElement = m_list[i]->Get(factor, nFontHeight);
		m_scrinfo[i]->value=m_list[i]->GetIndex(factor);
		//cyiBitmapBlender::FillColor(m_bmpCurrent, Color::Red, &Rect(x, 0, m_list[i]->GetWidth(), nFontHeight));
		
		cglsStatic* obj = (cglsStatic*)GetChild(xLABEL_START_ID+i);

		if (obj)
		{
			obj->Move(m_w-btnWidth-x-obj->m_w, 0);
			x+=obj->m_w;
		}
		cyiBitmapBlender::BitmapCopy(m_bmpCurrent, bmpElement, m_w-bmpElement->GetWidth()-btnWidth-x/*+((nFontHeight-bmpElement->GetWidth())>>1)*/, 0, 0, 0, bmpElement->GetWidth(), bmpElement->GetHeight());

		x+=bmpElement->GetWidth();
		delete bmpElement;

	}

	//cglsStatic* obj = (cglsStatic*)GetChild(xLABEL_START_ID-1);

	//if (obj)
	//{
	//	obj->Move(m_w-obj->m_w, 0);
	//}

	Graphics g(m_bmpCurrent);
	g.DrawImage(m_bmpCover, 0, 0);
	__super::Update(bmp, rect, _childRect, offsetX, offsetY, bIsLocalBufferUpdate);
	return ;
}

// -----------------------------------------------------
int cglsSpinBox::GetValueAfterAnimation()
{
	return m_nAfterValue;
}

// ------------------------------
Bool cglsSpinBox::SetValue(int value, eSplitBoxValueFrom eFrom, Bool bAnimation, int nAnimationTerm)
{
	if (bAnimation==False)
	{
		int sum=1;
		int factor=1;
		for (int i=0; i<m_nSpinCount; i++)
		{
			sum*=m_list[i]->count();
			m_scrinfo[i]->fBeforePos=m_scrinfo[i]->fCurrentPos=(value%sum)/factor;
			factor=sum;
		}

		m_nAfterValue=value;
		PostMessage(m_hNotifyWnd, WM_COMMAND, m_id, m_nAfterValue);
		InvalidateRect(0);
		return True;
	}

	if (eFrom==eSBVF_CURRENT)
	{
		int n = GetValue();
		m_scrinfo[0]->fTerm=(float)value;
		xScroll(0, nAnimationTerm);

		m_nAfterValue=n+value;
		PostMessage(m_hNotifyWnd, WM_COMMAND, m_id, m_nAfterValue);

		return True;
	}

	int n=GetValue(-1);

	if (n==value)
		return False;

	m_scrinfo[0]->fTerm=(float)(value-n);

	xScroll(0, nAnimationTerm, eGAP_FISO);
	m_nAfterValue=value;
	PostMessage(m_hNotifyWnd, WM_COMMAND, m_id, m_nAfterValue);

	return True;

}

// ------------------------------
LRESULT cglsSpinBox::xScroll(int nIndex, int nTerm, int nAnimationType)
{
TOP:
	int n=m_list[nIndex]->count();

	if (n==0)
		return 0;

	if (n==1)
	{
		float fTerm=m_scrinfo[nIndex++]->fTerm;
		m_scrinfo[nIndex]->fTerm=fTerm;
		goto TOP;
	}

	if (yiAnitIsAlived(m_scrinfo[nIndex]->hScrollAnimation)==True)
	{
		//xTRACE("Animation is stopped!");
		yiAnitStop(m_scrinfo[nIndex]->hScrollAnimation, True);
	}

	m_scrinfo[nIndex]->fBeforePos=m_scrinfo[nIndex]->fCurrentPos;

	//float remain=m_scrinfo[nIndex]->fCurrentPos-(float)(int)m_scrinfo[nIndex]->fCurrentPos;
	//m_scrinfo[nIndex]->fTerm+=(1.0-remain);
	if (m_scrinfo[nIndex]->fCurrentPos!=(float)(int)(m_scrinfo[nIndex]->fCurrentPos))
	{
		if (m_scrinfo[nIndex]->fCurrentPos<0)
				m_scrinfo[nIndex]->fCurrentPos=(int)m_scrinfo[nIndex]->fCurrentPos-1.0;
		else
			m_scrinfo[nIndex]->fCurrentPos=(int)m_scrinfo[nIndex]->fCurrentPos+1.0;

		m_scrinfo[nIndex]->fBeforePos=m_scrinfo[nIndex]->fCurrentPos;
	}


	yiAnitStart(m_scrinfo[nIndex]->hScrollAnimation, m_hWnd, WM_SPINBOX_SCROLL, nTerm, 10, (eGlsAnimationPattern)nAnimationType, (void*)(nIndex+1));

	return 0;
}


// ------------------------------
LRESULT cglsSpinBox::OnScrollBtn(HWND hWnd, int code)
{
	int type = eGAP_SIFO;
	int nTerm=100;
	int nKeyRepeat = (0x8000&code);
	code = 0x7FFF&code;
	if (yiAnitIsAlived(m_scrinfo[0]->hScrollAnimation)==True)
		return 0;

	if (code==eGTBT_AUP)
	{
		m_scrinfo[0]->fTerm=(nKeyRepeat==0)?-m_fScrollTerm:-10.0;
	}
	else
	{
		m_scrinfo[0]->fTerm=(nKeyRepeat==0)?m_fScrollTerm:10.0;
	}

	if (nKeyRepeat!=0)
	{
		nTerm=300;
		type = eGAP_LINEAR;
	}

	xScroll(0,nTerm, type);


	//InvalidateRect(0);
	
	return 0;
}

// ------------------------------
float cglsSpinBox::xIsNextAnimation(int index, float cur, float before)
{
	/*if ((float)(cur-(int)cur)!=0.0)
	{
		return 0.0;
	}

	int n = (int)cur;
	int max = m_list[index]->count();
	int min = 0;
	n = n%max;

	if (n==min)
	{
		if (m_scrinfo[index]->fSelectedIndex<m_scrinfo[index]->fSelectedIndexBefore)
		{
		}
	}
	else if (n==max)
	{
	}*/

	return 0.0;
}

// ------------------------------
int cglsSpinBox::xIsBetween(int index, float from, float to)
{
	if (from==to)
		return 0;


	float factorIndex = xGetFactorIndex(index);
	float fCurrentPos = m_scrinfo[index]->fCurrentPos;

	// 正方向
	if (from<to)
	{
		int n = m_list[index-1]->count();
		int next = ((int)(fCurrentPos+n)/n)*n;
		

		if (from<0.0 && to>=0.0)
		{
			float fTerm = to-m_scrinfo[index]->fCurrentPos;
			return (int)(fTerm/factorIndex+1);
		}

		if ((float)next<from)
		{
			return 0;
		}

		if ((float)next<=to)
		{
			float fTerm = to-m_scrinfo[index]->fCurrentPos;
			return (int)fTerm/factorIndex;
		}

	}
	// 逆方向
	else
	{
		int n = m_list[index-1]->count();
		int prev = ((int)(m_scrinfo[index]->fCurrentPos)/n)*n - 1;

		if (to>=0.0 && from<0.0)
		{
			float fTerm = to-m_scrinfo[index]->fCurrentPos;
			return (int)(fTerm/factorIndex-1);
		}

		if ((float)prev>from)
		{
			return 0;
		}

		to = (float)((int)to/n);
		if (to>=(float)prev )
		{
			float fTerm = to-m_scrinfo[index]->fCurrentPos;
			return (int)fTerm/factorIndex;
		}
	}

	return 0;
}


// ------------------------------
void cglsSpinBox::RunNextAnimation(int nIndex, float before, float after, Bool bIsForced, float fTerm)
{


	if (nIndex>=m_nSpinCount-1)
		return ;

	if (bIsForced==True)
	{
		m_scrinfo[nIndex+1]->fTerm=fTerm;
		xScroll(nIndex+1, (nIndex+1)*100);
		return ;
	}

	if (before==after)
	{
		return ;
	}

	float _before=before;
	float _after=after;

	int n = m_list[nIndex]->count();

	if (before<after)
	{
		if (m_scrinfo[nIndex]->fLimited==0.0)
		{

			if (before<0 && after<0)
			{
				int dumy = (int)after/n;

				dumy=-dumy;
				dumy++;

				before+=(float)(dumy*n);
				after+=(float)(dumy*n);

			}

			//if (((int)before%n)==(n-1+m_scrinfo[nIndex]->nShift) &&
			//		((int)after%n)==m_scrinfo[nIndex]->nShift)
			if (((int)before%n)==(n-1-m_scrinfo[nIndex]->nShift) &&
				((int)after%n)==(m_scrinfo[nIndex]->nShift?n-m_scrinfo[nIndex]->nShift:0))
			{
				m_scrinfo[nIndex+1]->fTerm=1.0;
				xScroll(nIndex+1, (nIndex+1)*100);
			}
		}
		else
		//if (m_scrinfo[nIndex]->fLimited!=0.0)
		{
			n=(int)m_scrinfo[nIndex]->fLimited;

			if (before<0 && after<0)
			{
				int dumy = (int)after/n;

				dumy=-dumy;
				dumy++;

				before+=(float)(dumy*n);
				after+=(float)(dumy*n);

			}

			if (((int)before%n)==(n-1) &&
					((int)after%n)==0)
			{
				m_scrinfo[nIndex+1]->fTerm=(float)(int)(after-before);//1.0;
				xScroll(nIndex+1, (nIndex+1)*100);
			}

			n = m_list[nIndex]->count();

			if (((int)before%n)==(n-1-m_scrinfo[nIndex]->nShift) &&
				((int)after%n)==(m_scrinfo[nIndex]->nShift?n-m_scrinfo[nIndex]->nShift:0))
			{
				m_scrinfo[nIndex+1]->fTerm=1.0;
				xScroll(nIndex+1, (nIndex+1)*100);
			}
		}


		if (before<0.0 &&
				after>=0.0)
		{
			m_scrinfo[nIndex+1]->fTerm=(float)(int)(after-before)+1.0;
			xScroll(nIndex+1, (nIndex+1)*100);
		}


	}
	else // 逆方向
	{
		if (before<0 && after<0)
		{
			int dumy = (int)after/n;

			dumy=-dumy;
			dumy++;

			before+=(float)(dumy*n);
			after+=(float)(dumy*n);

		}

		if (m_scrinfo[nIndex]->fLimited!=0.0)
		{
			n=(int)m_scrinfo[nIndex]->fLimited;

			if (_before<0 && _after<0)
			{
				int dumy = (int)_after/n;

				dumy=-dumy;
				dumy++;

				before=_before+(float)(dumy*n);
				after=_after+(float)(dumy*n);

			}

			if (((int)before%n)==0 &&
					((int)after%n)==(n-1))
			{
				m_scrinfo[nIndex+1]->fTerm=-1.0;
				xScroll(nIndex+1, (nIndex+1)*100);
				return ;
			}

			n=m_list[nIndex]->count();

			//if (((int)before%n)==0 &&
			//		((int)after%n)==(n-1))
			if (((int)before%n)==(m_scrinfo[nIndex]->nShift?n-m_scrinfo[nIndex]->nShift:0) &&
					((int)after%n)==(n-1)-m_scrinfo[nIndex]->nShift)
			{
				m_scrinfo[nIndex+1]->fTerm=-1.0;
				xScroll(nIndex+1, (nIndex+1)*100);
				return ;
			}
		}
		else
		{
			if (((int)before%n)==(m_scrinfo[nIndex]->nShift?n-m_scrinfo[nIndex]->nShift:0) &&
					((int)after%n)==(n-1)-m_scrinfo[nIndex]->nShift)
			{
				m_scrinfo[nIndex+1]->fTerm=-1.0;
				xScroll(nIndex+1, (nIndex+1)*100);
				return ;
			}
		}

		if (before>=0.0 &&
				after<0.0)
		{
			m_scrinfo[nIndex+1]->fTerm=-1.0;
			xScroll(nIndex+1, (nIndex+1)*100);
			return ;
		}


	}
	

}

// ------------------------------
LRESULT cglsSpinBox::OnScroll(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	float factor=0.0;
	float fBefore=0;
	float fAfter=0;
	int nIndex=(lParam-1);
	int nValue=GetValue();

	// 中止
	if (lParam==0)
	{
		return 0;
	}

	if (yiAnitIsAlived(m_scrinfo[nIndex]->hScrollAnimation)==False)
	{
		// 既に中止されている。
		return 0;
	}

	if ((factor=yiAnitGetFactor(m_scrinfo[nIndex]->hScrollAnimation, wParam))<0.0 ||
		factor>1.0)
	{
		// 終了
		//if (m_fMax!=0.0)
		//{
		//	float fAnimationValue=nValue+m_scrinfo[nIndex]->fTerm*xGetFactorIndex(nIndex);

		//	if (fAnimationValue>m_fMax)
		//	{
		//		// 何もしない。
		//		yiAnitStop(m_scrinfo[nIndex]->hScrollAnimation, False);
		//		RunNextAnimation(nIndex, 0, 0, True, 1.0);
		//		//SetValue(0, eSBVF_ZERO, True);
		//		return 0;
		//	}
		//}

		fBefore=m_scrinfo[nIndex]->fCurrentPos;

		
		m_scrinfo[nIndex]->fCurrentPos=m_scrinfo[nIndex]->fBeforePos+m_scrinfo[nIndex]->fTerm;//(fTerm*factor);
		
		fAfter=m_scrinfo[nIndex]->fCurrentPos=(m_scrinfo[nIndex]->fBeforePos+m_scrinfo[nIndex]->fTerm);
		m_scrinfo[nIndex]->fBeforePos=m_scrinfo[nIndex]->fCurrentPos;

		//xTRACE("End: m_scrinfo[%d] fCurrentPos:%f, fBeforePos:%f, fTerm:%f", nIndex, m_scrinfo[nIndex]->fCurrentPos, m_scrinfo[nIndex]->fBeforePos, m_scrinfo[nIndex]->fTerm);

		if (m_scrinfo[nIndex]->fLimited!=0.0)
		{
			if ((int)fAfter%(int)m_scrinfo[nIndex]->fLimited==0)
			{
				m_scrinfo[nIndex]->fBeforePos=m_scrinfo[nIndex]->fCurrentPos=0.0;
			}
		}

		yiAnitStop(m_scrinfo[nIndex]->hScrollAnimation, False);
	}
	else
	{
		float fNextAnit=m_scrinfo[nIndex]->fTerm*factor;

		//YIDEBUG("glsSpinBox.... OnScroll()");
		
		// 一度動き出したのは止めない。
		if (wParam!=0)
		{
			goto XXXXX_SKIP;
		}

		// 終了
		if (m_fMax!=0.0)
		{
			float factorIndex=xGetFactorIndex(nIndex);
			float fAnimationValue=nValue+m_scrinfo[nIndex]->fTerm*factorIndex; // Max+1に向かう
			Bool bIsSkip=False;

			// この場合は最大値を意味するので、
			// 最大値にさせられるもの以外は動かないようにする。
			if (nValue==0 && m_scrinfo[nIndex]->fTerm<0)
			{
				float factor=m_list[nIndex]->count();
				fAnimationValue=m_fMax+(factor+m_scrinfo[nIndex]->fTerm)*factorIndex;//nValue+(factor+m_scrinfo[nIndex]->fTerm);
			}
			else if (nValue>=m_fMax && m_scrinfo[nIndex]->fTerm<0)
			{
				bIsSkip=True;
				goto XXXXX_SKIP;
			}

			if (/*fNextAnit>0 &&*/
				fAnimationValue>m_fMax)
			{

				if (m_scrinfo[nIndex]->fTerm>0)
				{
					int k=(int)fAnimationValue%(int)m_fNextMax;
					if ((float)k>m_fMax &&
							(float)k<m_fNextMax)
					{
						//何もしない。
						yiAnitStop(m_scrinfo[nIndex]->hScrollAnimation, False);
						//xTRACE("yiAnitStop (%d)...........1", nIndex);
						RunNextAnimation(nIndex, 0, 0, True, 1.0);//fNextAnit>0?1.0:-1.0);
						return 0;
					}
				}
				else
				{
					int k=(int)fAnimationValue%(int)m_fNextMax;
					if ((float)k>m_fMax &&
							(float)k<m_fNextMax)
					{
						//何もしない。
						yiAnitStop(m_scrinfo[nIndex]->hScrollAnimation, False);
						//xTRACE("yiAnitStop (%d)...........2", nIndex);
						RunNextAnimation(nIndex, 0, 0, True, -1.0);//fNextAnit>0?1.0:-1.0);
						return 0;
					}
				}
			}
		}
XXXXX_SKIP:
		fBefore=m_scrinfo[nIndex]->fCurrentPos;
		fAfter=m_scrinfo[nIndex]->fCurrentPos=m_scrinfo[nIndex]->fBeforePos+fNextAnit;

		//if (wParam==0)
		//{
			//xTRACE("m_scrinfo[%d] fCurrentPos:%f, fBeforePos:%f, fTerm:%f", nIndex, m_scrinfo[nIndex]->fCurrentPos, m_scrinfo[nIndex]->fBeforePos, m_scrinfo[nIndex]->fTerm);
		//}
	}

	RunNextAnimation(nIndex, fBefore, fAfter);

	m_scrinfo[nIndex]->bIsReqAnimationUpdte=True;
	
	if (factor!=0.0)
		InvalidateRect(0);

	//float fNextAnit;
	//if ((fNextAnit=xIsNextAnimation(m_fSelectedIndex, m_fSelectedIndexBefore))!=0)
	//{
	//	yiAnitStop(m_hScrollAnimation, True);
	//	m_fTerm=fNextAnit;
	//	xScroll(++m_nAnimationIndex);
	//}
	return 0;
}


// ---------------------------------------
LRESULT cglsSpinBox::OnMouseLeave(WPARAM wp, LPARAM lp)
{
	if (xIS_GLS_FLAG(m_flag, eGF_NO_INPUT)==True)
	{
		return 0;
	}

	SetCursor(LoadCursorA(NULL, IDC_ARROW));
	return 0;
}

// ---------------------------------------
LRESULT cglsSpinBox::OnMouseEnter(WPARAM wp, LPARAM lp)
{
	if (xIS_GLS_FLAG(m_flag, eGF_NO_INPUT)==True)
	{
		return 0;
	}

	HCURSOR hCursor=LoadCursorA(NULL, IDC_HAND);
	SetCursor(hCursor);
	//HCURSOR hPrevCursor=SetCursor(hCursor);
	return 0;
}

// ---------------------------------------
LRESULT cglsSpinBox::OnMouseLButtonDown(WPARAM wp, LPARAM lp, Bool bIsHandled)
{
	if (xIS_GLS_FLAG(m_flag, eGF_NO_INPUT)==True)
	{
		return 0;
	}

	int x   =  lp & 0xFFFF;         // カーソルx座標
	int y   = (lp >> 16) & 0xFFFF;  // カーソルy座標
	int half=m_h>>1;

	int n = xPosToIndex(x, y);

	if (n==-1)
		return 0;

	if (y<half)
		m_scrinfo[n]->fTerm=1.0;
	else
		m_scrinfo[n]->fTerm=-1.0;

	xScroll(n);

	return 0;

}

// -----------------------------
void cglsSpinBox::Update(eGlsUpdateMode mode, eDrawItemStatus status)
{
	//return ;
	if (mode==eGUM_FORCED)
	{
		// まずラベルを更新してから。
		if (m_children)
		{
			for (cyiNode* node=m_children->GetNodeTopFrom(); node; node=node->np)
			{
				cGlassInterface* obj = (cGlassInterface*)node->ref;
				obj->Update(mode, status);
			}
		}

		m_sumWidth=0;
		m_labelWidth=GetChild(xLABEL_START_ID-1)?GetChild(xLABEL_START_ID-1)->m_w:0;
		int nFontHeight=GetFontHeight(True);
		for (int i=0; i<this->m_nSpinCount; i++)
		{
			m_list[i]->Update(GetFont());
			//m_sumWidth+=m_list[i]->GetWidth();
			nFontHeight=nFontHeight>m_list[i]->GetHeight()?m_list[i]->GetHeight():nFontHeight;
			int width=m_list[i]->GetWidth();
			width+=4;
			width = width>nFontHeight?width:nFontHeight;
			m_list[i]->SetBitmapSize(width, nFontHeight, m_colorBg);
			m_sumWidth+=m_list[i]->GetWidth();

			cGlassInterface*child=GetChild(xLABEL_START_ID+i);
			if (child)
			{
				m_labelWidth+=child->m_w;
			}
		}
		
		//ReSize(m_sumWidth+m_labelWidth, nFontHeight);
		xReSize(nFontHeight);
		m_bmpCover=xCreateBmpCover(m_w, nFontHeight);
		InvalidateRect(0);
	}
}


// ---------------------------------------
LRESULT cglsSpinBox::xPosToIndex(int x, int y)
{
	int sum_x=m_w;
	cGlassInterface* child=(cGlassInterface*)GetChild(xLABEL_START_ID-1);
	sum_x-=child?child->m_w:0;

	for (int i=0; i<m_nSpinCount; i++)
	{
		if (m_list[i]==0)
			continue;

		cglsStatic* p = (cglsStatic*)GetChild(xLABEL_START_ID+i);

		if (p)
		{
			sum_x-=p->m_w;
		}

		int dx=m_list[i]->GetWidth();

		if ((sum_x-dx)<=x &&
			sum_x>x)
		{
			return i;
		}
		sum_x-=dx;
	}

	return -1;
}