#include "glsSlide.h"

cglsSlide::cglsSlide(void)
: m_ctrlz(0)
, m_flagSlide(0)
, m_nSelectedControlId(0)
, m_fromId(0)
, m_toId(0)
, m_eDir(eSBSD_TOLEFT)
, m_ePattern(eGAP_FISO)
, m_nSlideTerm(15)
, m_nSlideInterval(500)
, m_hSlideAnit(0)
, m_bmpParentBg(0)
, m_bmpFrom(0)
, m_bmpTo(0)
{
	__GLASSINTERFACE_NAME__;
}

cglsSlide::~cglsSlide(void)
{
}

enum eSlideFlag {
	eSF_RINGBUFFER=1<<1,
	eSF_SHOWBUTTON=1<<2,
	eSF_USEGAUSS=1<<3
};

// -------------------------------------------------------
void cglsSlide::xDefaultDetail(sGlsSlideDetail* detail)
{
	detail->bRingBuffer=True;
	detail->bShowButton=True;
	detail->nMillisecInterval=300;
	detail->nMillSecTerm=15;
	detail->style=eSBMS_NONE;

}

// -------------------------------------------------------
// 当然ながらFixedモードで動く。
Bool cglsSlide::Create(cGlassInterface* parent, int id, int x, int y, int w, int h, sGlsSlideDetail* detail)
{
	sGlsDetail gls;
	cGlassInterface::xDefaultDetail(&gls);

	RECT rect = {x, y, w, h};

	Bool res = __super::Create((HWND)parent, 0, id, &rect, &gls);
	return res;
}

// -------------------------------------------------------
void cglsSlide::Destroy()
{

	if (m_hSlideAnit)
		yiAnitDestroy(m_hSlideAnit);
	m_hSlideAnit=0;
	__super::Destroy();
}

#include "glsButton.h"
// -------------------------------------------------------
int cglsSlide::AddControl(cGlassInterface* ctrl, eGlsItemAlign align)
{
	if (m_ctrlz==0)
		m_ctrlz = new cyiList();

	ctrl->SetParent(this);
	ctrl->Hide();
	xSET_ITEM_HIDE((&(ctrl->m_item)));
	m_ctrlz->addref(ctrl);

	int n = m_ctrlz->count();

	if (m_h > ctrl->m_h)
	{
		ReSize(m_w, ctrl->m_h);
	}

	ctrl->Move((m_w-ctrl->m_w)>>1, (m_h-ctrl->m_h)>>1);

	return 0;
}

// -------------------------------------------------------
void cglsSlide::RemoveControl(int id)
{
	xNODELOOP(node, m_ctrlz)
	{
		cGlassInterface* i = (cGlassInterface*)node->ref;

		if (i->m_id==id)
		{
			m_ctrlz->remove(node);
			UnRegisterChild(i);
			return ;
		}
	}

	
}

// -----------------------------
cGlassInterface* cglsSlide::GetCurrentControl()
{
	return GetChild(m_toId);
}

// -----------------------------
cGlassInterface* cglsSlide::GetControl(int id)
{
	return GetChild(id);
}

typedef struct {
	int nAnimationCounter;
} sSlideAnimation;

// -------------------------------------------------------
int cglsSlide::Scroll(int to, eSlideBarScrollDirection dir, eGlsAnimationPattern pattern, int nMillSecTer, int nMillisecInterval)
{
	if (m_hSlideAnit==0)
		m_hSlideAnit = yiAnitCreate();

	if (yiAnitIsAlived(m_hSlideAnit)==True)
	{
		yiAnitStop(m_hSlideAnit, True);
	}

	m_fromId=m_toId;
	m_toId=to;
	m_eDir=dir;
	m_ePattern=pattern;
	m_nSlideTerm=nMillSecTer;
	m_nSlideInterval=nMillisecInterval;

	sSlideAnimation s = {0};
	yiAnitStart(m_hSlideAnit, m_hWnd, WM_SLIDE_ANIMATION, m_nSlideInterval, m_nSlideTerm, m_ePattern, yitoheap(&s, sizeof(s)));

	return 0;

}

// -----------------------------
Bitmap* cglsSlide::xMakeSlideImage(Bitmap* _bmp, Bitmap* bmpFrom, Bitmap* bmpTo, float factor, eSlideBarScrollDirection dir)
{
	Bitmap* bmp = cBB::BitmapCopy(0, _bmp);
	Graphics g(bmp);

	int xw = factor*(float)bmp->GetWidth();
	int rw = bmp->GetWidth()-xw;
	if (dir==eSBSD_TOLEFT)
	{
		g.DrawImage(bmpFrom, 0, 0, xw, 0, rw, bmpFrom->GetHeight(), Unit::UnitPixel);
		g.DrawImage(bmpTo, rw, 0, 0, 0, xw, bmpTo->GetHeight(), Unit::UnitPixel);
	}
	else
	{
		g.DrawImage(bmpTo, 0, 0, rw, 0, xw, bmpTo->GetHeight(), Unit::UnitPixel);
		g.DrawImage(bmpFrom, xw, 0, 0, 0, rw, bmpFrom->GetHeight(), Unit::UnitPixel);
	}

	return bmp;
}


// -----------------------------
LRESULT cglsSlide::OnSlideAnimation(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	int cnt = wParam;
	int* nLastIndex=(int*)lParam;
	Bool bSizeChanged=False;

	//YIDEBUG("OnSlideAnimation().... start");

	if (m_bIsCreated==False)
		return 0;

	if (cnt==-1)
	{
		yifree((void*)lParam);
		return 0;
	}

	if (cnt==-2)
	{
		yifree((void*)lParam);
		return 0;
	}

	if (cnt!=*nLastIndex)
	{
		return 0;
	}

	if (yiAnitIsCleared(m_hSlideAnit, 0)==True)
	{
		return 0;
	}

	(*nLastIndex)++;

	float factor = yiAnitGetFactorEx(m_hSlideAnit, cnt, m_ePattern);

	if (factor==-4.0)
		return 0;

	if (factor==1.0 ||
			factor<0.0)
	{
		// アニメーション終了
		//yifree((void*)lParam);
		cGlassInterface* child = GetChild(m_toId);

		if (child!=0)
		{
			xSET_ITEM_SHOW((&(child->m_item)));
			child->Show();
		}
		InvalidateRect(0);
		//YIDEBUG("OnSlideAnimation().... end");

		return 0;
	}

	if (cnt!=0)
	{
		if(m_bmpParentBg->GetWidth()!=m_w ||
				m_bmpParentBg->GetHeight()!=m_h)
		{
			bSizeChanged=True;
		}
	}

	if (cnt==0 ||
			bSizeChanged==True)
	{
		cGlassInterface* childTo = GetChild(m_toId);
		cGlassInterface* childFrom = GetChild(m_fromId);

		if (childTo!=0)
		{
			xSET_ITEM_HIDE((&(childTo->m_item)));
			childTo->Hide();
		}


		if (childFrom!=0)
		{
			xSET_ITEM_HIDE((&(childFrom->m_item)));
			childFrom->Hide();
		}

		// 1. ここで、バックグラウンドのイメージを取ってくる。

		if (m_bmpParentBg)
			delete m_bmpParentBg;

		m_bmpParentBg=GetParentBg();



		// 2. 目的地のビットマップ作成
		if (m_bmpTo)
		{
			if (m_bmpFrom)
				delete m_bmpFrom;
			m_bmpFrom=m_bmpTo;
			//delete m_bmpTo;
		}
		m_bmpTo = BITMAP32(m_w, m_h);

		if (childTo!=0)
		{
			RECT rect={childTo->m_x, childTo->m_y, childTo->m_w, childTo->m_h};
			RECT rectChild={0, 0, childTo->m_w, childTo->m_h};

			xSET_ITEM_SHOW((&childTo->m_item));
			childTo->Update(m_bmpTo, &rect, &rectChild, -m_nAbsX, -m_nAbsY);
			xSET_ITEM_HIDE((&childTo->m_item));
		}
		// 3. 開始ｲﾒｰｼﾞのビットマップ作成
		if (m_bmpFrom==0)
		{
			m_bmpFrom = BITMAP32(m_w, m_h);

			if (childFrom!=0)
			{
				RECT rect={childFrom->m_x, childFrom->m_y, childFrom->m_w, childFrom->m_h};
				RECT rectChild={0, 0, childFrom->m_w, childFrom->m_h};
				xSET_ITEM_SHOW((&childFrom->m_item));
				childFrom->Update(m_bmpFrom, &rect, &rectChild, -m_nAbsX, -m_nAbsY);
				xSET_ITEM_HIDE((&childFrom->m_item));
			}
		}
	}
	
	// 2. 合成したイメージを作成。
	Bitmap* ret = xMakeSlideImage(m_bmpParentBg, m_bmpFrom, m_bmpTo, factor, m_eDir);

	//YIDEBUG("OnSlideAnimation().... %.3f", factor);

	// 3. ダイレクト描画
	HDC hdc = ::GetDC(*m_glsDrawInterface);
	Graphics g(hdc);
	//g.FillRectangle(&SolidBrush(Color::Red), m_nAbsX, m_nAbsY, m_w, m_h);
	g.DrawImage(ret, m_nAbsX, m_nAbsY);

	delete ret;

	return 0;

}

// -----------------------------
void cglsSlide::Update(eGlsUpdateMode mode, eDrawItemStatus status)
{
	int nHeight=0;
	xNODELOOP(node, m_ctrlz)
	{
		cGlassInterface* child = (cGlassInterface*)node->ref;
		child->Update(mode);
		nHeight=nHeight<child->m_h?child->m_h:nHeight;
	}

	ReSize(m_w, nHeight);

	xNODELOOP(node, m_ctrlz)
	{
		cGlassInterface* child = (cGlassInterface*)node->ref;
		child->Move((m_w-child->m_w)>>1, (m_h-child->m_h)>>1);
	}

}

#ifdef _DEBUG
void cglsSlide::Update(Bitmap* bmp, RECT* rect, RECT* _childRect, int offsetX, int offsetY, Bool bIsLocalBufferUpdate)
{
	__super::Update(bmp, rect, _childRect, offsetX, offsetY, bIsLocalBufferUpdate);
	return ;
}
#endif

// -----------------------------
LRESULT cglsSlide::OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	bIsHandled=True;
	return SendMessage(m_hNotifyWnd, WM_COMMAND, wParam, lParam);
}
