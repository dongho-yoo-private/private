#include "glsMessageBox.h"

cglsMessageBox::cglsMessageBox(void)
: m_pszMessage(0)
, m_pszCaption(0)
, m_nTimer(0)
, m_nNotifyCode(0)
{
	__GLASSINTERFACE_NAME__;
}

cglsMessageBox::~cglsMessageBox(void)
{
//	if (m_pszMessage)
//		yifree(m_pszMessage);

//	if (m_pszCaption)
//		yifree(m_pszCaption);

	//cglsDialog::~cglsDialog();
}
// ---------------------------------------
cglsMessageBox* cglsMessageBox::xNEW()
{
	return new cglsMessageBox();
}


// ----------------------------------------------
// For modal
int cglsMessageBox::Show(cGlassInterface* parent, const wchar_t* pszMessage, const wchar_t* pszCaption, int btn, Font* font)
{
	cglsMessageBox box;
	return box.Create(-1, parent, pszMessage, pszCaption, btn, True, font);
}

// ----------------------------------------------
int cglsMessageBox::ShowByTimer(cGlassInterface* parent, const wchar_t* pszMessage, const wchar_t* pszCaption, int btn, Bool bIsModal, int ntime, int nShowAnimationtime, int nHideAnimationTime, Font* font, Point* pt)
{
	int res = Create(-1, parent, pszMessage, pszCaption, btn, bIsModal, font, ntime, nShowAnimationtime, nHideAnimationTime, pt);

	if (bIsModal==False)
		SetTimer(*this, 1, ntime, 0);

	return res;
}

// ---------------------------------------------------
void cglsMessageBox::SetNotifyCode(int code)
{
	m_nNotifyCode=code;
}


// ----------------------------------------------
// For modless
int cglsMessageBox::Show(int id, cGlassInterface* parent, const wchar_t* pszMessage, const wchar_t* pszCaption, int btn, Font* font, Point* pt)
{


	return Create(id, parent, pszMessage, pszCaption, btn, False, font, 0, 0, 0, pt);
}
#include "glsImageBox.h"
#include "glsButton.h"
#include "glsStackPannel.h"

// --------------------------------
void cglsMessageBox::Destroy()
{
	if (m_pszMessage)
		yifree(m_pszMessage);

	if (m_pszCaption)
		yifree(m_pszCaption);

	m_pszMessage=0;
	m_pszCaption=0;
	__super::Destroy();
}

// ---------------------------------------------------
int cglsMessageBox::Create(int id, cGlassInterface* parent, const wchar_t* pszMessage, const wchar_t* pszCaption, int btn, Bool bIsModal, Font* font, int time, int nShowAnimationTime, int nHideAnimationTime, Point* pt)
{
	sGlsDialogDetail xdetail;

	cglsDialog::xDefaultDetail(&xdetail);
	//xdetail.gauss.colorTop=__COLORp__(127, 0, 0, 0);
	//xdetail.gauss.colorBottom=__COLORp__(200, 0, 0, 0);
	xdetail.font=font;
	xdetail.gauss.fBlureRad=4.0;
	xdetail.gauss.fShadowWidth=5.0;
	xdetail.nHideAnimationTime=nHideAnimationTime;
	xdetail.nShowAnimationTime=nShowAnimationTime;
	xdetail.bIsAnimationShow=True;

	m_nButtonzId=btn;
	cyiShape::MeasureTextSize(pszMessage, font?font:parent->GetFont(), (parent->m_w<<1)/2-20, m_ptMessage); 

	int w=m_ptMessage.x+20;
	int h=m_ptMessage.y+20+(cglsButton::GetMeasureHeight(font?font:parent->GetFont()))*2;

	w+=((float)w*0.3);

	m_pszMessage=yistrheapW(pszMessage);
	m_pszCaption=yistrheapW(pszCaption);
	m_btnId=btn;
	m_nTimer=time;
	xdetail.bIsAnimationShow=True;

	if (pt)
	{
		m_x=pt->X;
		m_y=pt->Y;
	}

	m_bIsModal=bIsModal;

	if (bIsModal)
	{
		m_bIsExitLoop=False;
		return cglsDialog::DoModal((HWND)parent, id, (wchar_t*)pszCaption, (parent->m_w-w)>>1, (parent->m_h-h)>>1, w, h, &xdetail);
	}

	m_bIsExitLoop=True;
	return cglsDialog::Create((HWND)parent, id, (wchar_t*)pszCaption, pt==0?(parent->m_w-w)>>1:pt->X, pt==0?(parent->m_h-h)>>1:pt->Y, w, h, &xdetail);
}

#include "glsToolButton.h"
// ---------------------------------------------------
LRESULT cglsMessageBox::OnCreate(WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	cglsStackPannel* pannel;
	GetClientRect(&rect);

	// caption.

	cglsToolButton* btn = new cglsToolButton();
	btn->Create(this, 64, m_w-rect.left, rect.top, 0, 0);
	btn->Add(eGTBT_CROSS, eGTBSS_FILL);//eGTBT_ALEFT, eGTBSS_FILL);
	//btn->Add(eGTBT_ARIGHT, eGTBSS_FILL);
	btn->Show();
	btn->Update(eGUM_FORCED);
	btn->Move(m_w-rect.left-btn->m_w-3, btn->m_y+3);


	// message.
	{
		sGlsStaticDetail detail;
		cglsStatic::xDefaultDetail(&detail);
		//detail.colorFont=Color::White;
		cglsStatic* label = new cglsStatic();
		label->Create((HWND)this, (m_w-m_ptMessage.x)>>1, m_h-rect.top-(float)cglsButton::GetMeasureHeight(GetFont())*1.5-m_ptMessage.y, m_ptMessage.x, m_ptMessage.y,
						0, m_pszMessage, &detail);
		label->Show();
	}

	if (m_btnId==-1)
	{
		__super::OnCreate(wParam, lParam);

		sGlsAnimationEx ani;
		glsInitAnimationEx(&ani, &m_item);
		glsSetTrAnimationEx(&ani, 0.0, 1.0, eGAP_FISO);
		glsSetSzAnimationEx(&ani, 0.0, 1.0, eGAP_FISO, (eGlsItemAlign)(eGIA_CENTER|eGIA_VCENTER));
		AnimationShow(True, &ani, 15, m_nShowAnimationTime);//this->m_nShowAnimationTime);
		ZOrder(-1);
		InvalidateRect(0, 0, True);
		return 0;
	}

	//return 0;
	pannel = new cglsStackPannel();
	pannel->Create((HWND)this, 32, 0, 0, 0);

	// button
	if (m_btnId==MB_OK ||
			m_btnId==MB_OKCANCEL)
	{
		cglsButton* btnOk = new cglsButton();
		btnOk->Create((HWND)pannel, IDOK, 0, 0, 0, L"OK");
		btnOk->Show();
		pannel->Add(btnOk, 0);
	}

	if (m_btnId==MB_OKCANCEL)
	{
		cglsButton* btnCancel = new cglsButton();
		btnCancel->Create((HWND)pannel, IDCANCEL, 0, 0, 0, L"Cancel");
		btnCancel->Show();
		pannel->Add(btnCancel, 0);
		goto EXIT;
	}

	if (m_btnId==MB_YESNO)
	{
		cglsButton* btnOk = new cglsButton();
		btnOk->Create((HWND)pannel, IDOK, 0, 0, 0, L"‚Í‚¢");
		btnOk->Show();
		pannel->Add(btnOk, 0);

		cglsButton* btnCancel = new cglsButton();
		btnCancel->Create((HWND)pannel, IDCANCEL, 0, 0, 0, L"‚¢‚¢‚¦");
		btnCancel->Show();
		pannel->Add(btnCancel, 0);
	}

EXIT:
	pannel->Show();
	pannel->Update(eGUM_FORCED);

	//if (m_bIsExitLoop==False)
	//{
	pannel->Move((m_w-pannel->m_w)>>1, m_h-rect.top-pannel->m_h-(rect.top<<1));
	//}


	__super::OnCreate(wParam, lParam);

	ZOrder(-1);

	//sGlsAnimationEx ani;
	//glsInitAnimationEx(&ani, &m_item);
	//glsSetTrAnimationEx(&ani, 0.0, 1.0, eGAP_FISO);
	//m_item.fSizeRate=0.0;
	//glsSetSzAnimationEx(&ani, 0.0, 1.0, eGAP_FISO, (eGlsItemAlign)(eGIA_CENTER|eGIA_VCENTER));
	//AnimationShow(True, &ani, 15, m_nShowAnimationTime);
	InvalidateRect(0, 0, True);
	return 0;
}


// ---------------------------------------------------
LRESULT cglsMessageBox::OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	m_returnValue=(wParam&0xFFFF);

	if (m_returnValue==64)
	{
		m_returnValue=IDCANCEL;
	}

	if (m_bIsModal==False)
	{
		PostMessage(m_hNotifyWnd, WM_COMMAND, ((m_returnValue&0xFFFF)<<16)|m_id, m_nNotifyCode);
		m_bIsRequestDestroy=True;
		sGlsAnimationEx ani;
		glsInitAnimationEx(&ani, &m_item);
		glsSetTrAnimationEx(&ani, 0.0, 1.0, eGAP_FISO);
		glsSetSzAnimationEx(&ani, 0.0, 1.0, eGAP_FISO, (eGlsItemAlign)(eGIA_CENTER|eGIA_VCENTER));
		AnimationShow(True, &ani, 15, m_nShowAnimationTime);

		return 0;
	}
	
	if (m_bIsExitLoop==False)
	{
		if (m_returnValue==IDCANCEL)
			m_returnValue=0;

		sGlsAnimationEx ani;
		glsInitAnimationEx(&ani, &m_item);
		glsSetTrAnimationEx(&ani, 1.0, 0.0, eGAP_FISO);
		glsSetSzAnimationEx(&ani, 1.0, 0.0, eGAP_FISO, (eGlsItemAlign)(eGIA_CENTER|eGIA_VCENTER));

		if (AnimationShow(False, &ani, 15, m_nShowAnimationTime*2)==False)//m_nShowAnimationTime)==False)
		{
			m_bIsExitLoop=True;
		}
		return 0;
	}

	if (m_returnValue==IDCANCEL)
		m_returnValue=0;
	//Destroy();
	return 0;
}

// ----------------------------------------------
LRESULT cglsMessageBox::OnTimer(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	sGlsAnimationEx ani;
	
	glsInitAnimationEx(&ani, &m_item);
	glsSetTrAnimationEx(&ani, 1.0, 0.0, eGAP_SIFO);
	glsSetSzAnimationEx(&ani, 1.0, 0.0, eGAP_SIFO, (eGlsItemAlign)(eGIA_CENTER|eGIA_VCENTER));
	AnimationShow(False, &ani, 15, m_nShowAnimationTime);

	KillTimer(m_hWnd, wParam);
	
	if (m_bIsModal==True)
	{
		m_returnValue=IDOK;
		m_bIsExitLoop=True;
	}
	else
	{
		m_bIsRequestDestroy=True;
		glsInitAnimationEx(&ani, &m_item);
		glsSetTrAnimationEx(&ani, 1.0, 0.0, eGAP_SIFO);
		glsSetSzAnimationEx(&ani, 1.0, 0.0, eGAP_SIFO, (eGlsItemAlign)(eGIA_CENTER|eGIA_VCENTER));
		AnimationShow(False, &ani, 15, m_nShowAnimationTime);

	}
	return 0;
}

// ------------------------------------
void cglsMessageBox::OnReqDestroy()
{
	__super::OnReqDestroy();
	Destroy();
}