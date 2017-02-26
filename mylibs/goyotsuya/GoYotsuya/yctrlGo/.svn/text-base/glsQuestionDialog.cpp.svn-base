#include "glsQuestionDialog.h"

cglsQuestionDialog::cglsQuestionDialog(void)
{
}

cglsQuestionDialog::~cglsQuestionDialog(void)
{
}



// ------------------------------
int cglsQuestionDialog::Show(cGlassInterface* parent, int nQuestion, Font* font)
{
	sGlsDialogDetail xdetail;

	cglsDialog::xDefaultDetail(&xdetail);
	xdetail.font=font;
	xdetail.gauss.fBlureRad=5.0;
	xdetail.gauss.fShadowWidth=7.0;

	int w=10;
	int h=10;
	int x=(parent->m_w-w)>>1;
	int y=(parent->m_h-h)>>1;

	m_bIsExitLoop=False;
	m_bIsSizeFixedMode=False;

	m_nQuestion=nQuestion;
	return cglsDialog::DoModal((HWND)parent, GetBlankId(), L"Ž¿–â", x, y, w, h, &xdetail);
}

#define xID 16
// --------------------------------------------
LRESULT cglsQuestionDialog::OnCreate(WPARAM wParam, LPARAM lParam)
{
	int nButtonHeight=cglsButton::GetMeasureHeight(GetFont());

	sGlsStaticDetail detail;
	cglsStatic* label = new cglsStatic();
	const wchar_t* question;

	if (m_nQuestion==0)
	{
		question=GO_QUESTION_1;
	}
	else if (m_nQuestion==1)
	{
		question=GO_QUESTION_2;
	}
	else
	{
		question=GO_QUESTION_3;
	}

	cglsStatic::xDefaultDetail(&detail);
	detail.id=xID+2;
	label->Create((HWND)this, 0, 0, 0, 0, 0, question, &detail);

	label->Show();
	
	cglsSplitButton* btn = new cglsSplitButton();

	btn->Create((HWND)this, xID+1, 0, 0, GetFontHeight()*8/*(Bitmap*)eGRID_NOT_SHOW_NO*/, nButtonHeight, L"•ÔŽ–");

	if (m_nQuestion==0)
	{
		btn->AddItem(-1, 0, L"•ÔŽ–", L"–³Œ¾", True);
		btn->AddItem(1, 0/*(Bitmap*)eGRID_SHOW_NO*/, GO_ANSER_1_1, GO_ANSER_1_1);
		btn->AddItem(2, 0/*(Bitmap*)eGRID_SHOW_NO*/, GO_ANSER_1_2, GO_ANSER_1_2);
		btn->AddItem(3, 0/*(Bitmap*)eGRID_SHOW_NO*/, GO_ANSER_1_3, GO_ANSER_1_3);
		btn->AddItem(4, 0/*(Bitmap*)eGRID_SHOW_NO*/, GO_ANSER_1_4, GO_ANSER_1_4);
		btn->AddItem(5, 0/*(Bitmap*)eGRID_SHOW_NO*/, GO_ANSER_1_5, GO_ANSER_1_5);
	}
	else if (m_nQuestion==1)
	{
		btn->AddItem(-1, 0, L"•ÔŽ–", L"–³Œ¾", True);
		btn->AddItem(1, 0/*(Bitmap*)eGRID_SHOW_NO*/, GO_ANSER_2_1, GO_ANSER_2_1);
		btn->AddItem(2, 0/*(Bitmap*)eGRID_SHOW_NO*/, GO_ANSER_2_2, GO_ANSER_2_2);
		btn->AddItem(3, 0/*(Bitmap*)eGRID_SHOW_NO*/, GO_ANSER_2_3, GO_ANSER_2_3);
		btn->AddItem(4, 0/*(Bitmap*)eGRID_SHOW_NO*/, GO_ANSER_2_4, GO_ANSER_2_4);
		btn->AddItem(5, 0/*(Bitmap*)eGRID_SHOW_NO*/, GO_ANSER_2_5, GO_ANSER_2_5);
	}
	else if (m_nQuestion==2)
	{
		btn->AddItem(-1, 0, L"•ÔŽ–", L"–³Œ¾", True);
		btn->AddItem(1, 0/*(Bitmap*)eGRID_SHOW_NO*/, GO_ANSER_3_1, GO_ANSER_3_1);
		btn->AddItem(2, 0/*(Bitmap*)eGRID_SHOW_NO*/, GO_ANSER_3_2, GO_ANSER_3_2);
		btn->AddItem(3, 0/*(Bitmap*)eGRID_SHOW_NO*/, GO_ANSER_3_3, GO_ANSER_3_3);
	}

	//btn->Update(eGUM_FORCED);
	btn->Show();

	cGlassInterface::Update(eGUM_FORCED);
	cglsDialog::CreateButton(MB_OK);
	ReArrangeItemz();
	AnimationShow(True, this->m_nShowAnimationTime);
	return 0;
}

// ------------------------------------
void cglsQuestionDialog::OnItemReArranged()
{
	int xOffset, yOffset;
	GetContentsOffset(xOffset, yOffset);

	GetChild(xID+2)->Move(xOffset, yOffset);
	int maxW = GetChild(xID+2)->m_w;
	int maxH = GetChild(xID+2)->m_h+GetChild(xID+1)->m_h;

	SetContentsSize(maxW, maxH);

	GetChild(xID+1)->Move((m_w-GetChild(xID+1)->m_w)>>1, GetChild(xID+2)->m_h+GetChild(xID+2)->m_y+GetFontHeight()/2);

	InvalidateRect(0, 0, True);

	Move((m_parent->m_w-m_w)>>1, (m_parent->m_h-m_h)>>1);
	return ;
}

// ------------------------------------
void cglsQuestionDialog::OnExitLoop()
{
	cglsSplitButton* btn = (cglsSplitButton*)GetChild(xID+1);
	m_returnValue=btn->GetSelectIndex();
}