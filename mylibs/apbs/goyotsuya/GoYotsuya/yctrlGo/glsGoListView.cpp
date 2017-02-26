#include "glsGoListView.h"

cglsGoListView::cglsGoListView(void)
:m_goz(0)
{
}

cglsGoListView::~cglsGoListView(void)
{
}

void cglsGoListView::xDefaultDetail(sGlsGoListDetail* detail)
{
	detail->bShowDelete=True;
	detail->bShowInfoView=True;
	detail->bShowScrollBar=True;
	detail->bShowSearchCtrl=True;
	detail->bShowSelect=True;
	detail->colorBg=0;
	detail->colorText=0;
	detail->nPreviewHeight=128;
	detail->font=0;
}

enum eGlsGoListFlag {
	eGGLF_SHOW_SEARCH=0x01,
	eGGLF_SHOW_SCROLL=0x02,
	eGGLF_SHOW_INFO=0x04,
	eGGLF_SHOW_SELECT=0x08,
	eGGLF_SHOW_DELETE=0x10
};


// ------------------------------
Bool cglsGoListView::Create(cGlassInterface* gls, int id, int x, int y, int w, int h, sGlsGoListDetail* detail)
{
	sGlsGoListDetail xdetail;
	

	if (detail)
		xDefaultDetail(&xdetail);
	else
		xdetail=*detail;

	xSET_GLS_FLAG(m_xflag, eGGLF_SHOW_SEARCH, xdetail.bShowSearchCtrl);
	xSET_GLS_FLAG(m_xflag, eGGLF_SHOW_SCROLL, xdetail.bShowScrollBar);
	xSET_GLS_FLAG(m_xflag, eGGLF_SHOW_INFO, xdetail.bShowInfoView);
	xSET_GLS_FLAG(m_xflag, eGGLF_SHOW_SELECT, xdetail.bShowSelect);
	xSET_GLS_FLAG(m_xflag, eGGLF_SHOW_DELETE, xdetail.bShowDelete);

	m_color=xdetail.colorText;
	m_colorBg=xdetail.colorBg;

	sGlsDetail xgls;
	cGlassInterface::xDefaultDetail(&xgls);
	xgls.font=xdetail.font;
	RECT rect={x, y, w, h};
	return cGlassInterface::Create((HWND)gls, 0, id, &rect, &xgls);
}

#define xIDSEARCH 32

// ------------------------------------
void cglsGoListView::xCreateSearchControl()
{
	sGlsEditBoxDetail editDetail;
	cglsEditBox::xDefaultDetail(&editDetail);
	editDetail.gls.font=fontSmall;
	cglsEditBox* edit = new cglsEditBox();
	edit->Create((HWND)this, xIDSEARCH+1, 0, 0, GetFontHeight(True)*10, GetFontHeight(True), &editDetail);
	//tab->AddPositionLink(edit, eGP_BOTTOMSIDE|eGP_LEFT, 0, 2);

	sGlsSplitButtonDetail splitDetail;
	cglsSplitButton::xDefaultDetail(&splitDetail);

	splitDetail.gls.font=fontSmall;

	cglsSplitButton* s1 = new cglsSplitButton();
	s1->Create((HWND)this, xIDSEARCH+2, 0, 0, GetFontHeight(True)*3, GetFontHeight(True), 0, &splitDetail);
	s1->AddItem(1, 0, L"�荇", L"�S�ĕ\��", True);
	s1->AddItem(2, 0, L"�ݐ�", L"�݂���", False);
	s1->AddItem(3, 0, L"���", L"���", False);
	
	edit->AddPositionLink(s1, eGP_RIGHTSIDE|eGP_BOTTOM);
	for (int i=2; i<10; i++)
	{
		wchar_t xx[5];
		wchar_t xxx[5];
		
		//wsprintfW(xx, L"%d", i);
		wsprintfW(xxx, L"%d�q", i);
		
		s1->AddItem(2+i, 0, xxx, xxx, False);
	}

	cglsSplitButton* s2 = new cglsSplitButton();
	s2->Create((HWND)this, xIDSEARCH+3, 0, 0, GetFontHeight(True)*4, GetFontHeight(True), 0, &splitDetail);
	s2->AddItem(1, 0, L"�萔", L"�S�ĕ\��", True);
	s2->AddItem(2, 0, L"30��", L"30�薢���̂ݕ\��", False);
	s2->AddItem(3, 0, L"150��", L"30��ȏ�150�薢���̂ݕ\��", False);
	s2->AddItem(4, 0, L"250��", L"150��ȏ�250�薢���̂ݕ\��", False);
	s2->AddItem(5, 0, L"250��", L"250��ȏ�̂ݕ\��", False);
	s2->AddItem(6, 0, L"300��", L"300��ȏ�̂ݕ\��", False);

	s1->AddPositionLink(s2, eGP_RIGHTSIDE|eGP_TOP);

	cglsSplitButton* s3 = new cglsSplitButton();
	s3->Create((HWND)this, xIDSEARCH+4, 0, 0, GetFontHeight(True)*3, GetFontHeight(True), 0, &splitDetail);
	s3->AddItem(1, 0, L"����", L"�S�ĕ\��", True);
	s3->AddItem(2, 0, L"����", L"�����̏����邷�ׂ�", False);
	s3->AddItem(3, 0, L"����", L"�����̕����邷�ׂ�", False);
	s3->AddItem(4, 0, L"�W�S", L"�W�S�̑S��", False);
	s2->AddPositionLink(s3, eGP_RIGHTSIDE|eGP_BOTTOM);

	cglsSplitButton* s4 = new cglsSplitButton();
	s4->Create((HWND)this, xIDSEARCH+5, 0, 0, GetFontHeight(True)*3, GetFontHeight(True), 0, &splitDetail);
	s4->AddItem(1, 0, L"�ڍ�", L"�S�ĕ\��", True);
	s4->AddItem(2, 0, L"����", L"������", False);
	s4->AddItem(3, 0, L"����", L"���Ԑ؂�", False);
	s4->AddItem(4, 0, L"1��", L"1�ڈȓ�", False);
	s4->AddItem(5, 0, L"3��", L"3�ڈȓ�", False);
	s4->AddItem(6, 0, L"10��", L"10�ڈȓ�", False);
	s4->AddItem(7, 0, L"30��", L"30�ڈȏ�", False);
	s3->AddPositionLink(s4, eGP_RIGHTSIDE|eGP_BOTTOM);
}

#include "glsImageBox.h"

// ------------------------------------
void cglsGoListView::xCreateInfoControl()
{
	// 1. �v���r���[���
	cglsImageBox* img = new cglsImageBox();
	sGlsImageBoxDetail detail;
	cglsImageBox::xDefaultDetail(&detail);
	detail.bAutoResize=False;
	detail.bHasBorder=True;
	detail.bIsBorderDropShadow=True;
	detail.bIsDragAndDrop=False;
	img->Create(this, 16, 0, 0, m_nImageSize, m_nImageSize, 0, &detail);


	// 2. 
	sGlsStaticDetail x;
	cglsStatic::xDefaultDetail(&x);
	x.id=17;
	cglsStatc* comment = new cglsStatic();

	comment->Create();




	if (xIS_GLS_FLAG(m_xflag, eGGLF_SHOW_SELECT)==True)
	{
		sGlsButtonDetail btn;
		cglsButton::xDefaultDetail(&btn);
		cglsButton* button = new cglsButton();	
	}

	if (xIS_GLS_FLAG(m_xflag, eGGLF_SHOW_DELETE)==True)
	{
		sGlsButtonDetail btn;
		cglsButton::xDefaultDetail(&btn);
		cglsButton* button = new cglsButton();	
	}

}

#include "glsListview.h"

// ------------------------------------
LRESULT cglsGoListView::OnCreate(WPARAM wParam, LPARAM lParam)
{
	if (xIS_GLS_FLAG(m_xflag, eGGLF_SHOW_SEARCH)==True)
	{
		xCreateSearchControl();
	}

	cglsListView* listView = new cglsListView();
	listView->Create(this, 1, 0, 0, 10, nSmallFontHeight*10, fontSmall);
	listView->CreateColumn(7, &sListViewColumn(12, L"��", eGIA_LEFT, eGCT_STRING), 
									&sListViewColumn(12, L"��", eGIA_LEFT, eGCT_STRING), 
										&sListViewColumn(5, L"�荇", eGIA_LEFT),
											&sListViewColumn(5, L"�萔", eGIA_LEFT, eGCT_NUMERIC), 
												&sListViewColumn(3, L"��"),
													&sListViewColumn(6, L""),
														&sListViewColumn(11, L"����"));

	//listView->Update(eGUM_FORCED);
	listView->Show();

	if (xIS_GLS_FLAG(m_xflag, eGGLF_SHOW_SEARCH)==True)
	{
		cGlassInterface* child = GetChild(xIDSEARCH);

		child->AddPositionLink(listView, eGP_BOTTOMSIDE|eGP_LEFT, 0, 2);
	}

	if (xIS_GLS_FLAG(m_xflag, eGGLF_SHOW_INFO)==True)
	{
		xCreateInfoControl();
	}

	Update(eGUM_FORCED);
	return 0;
}

// ------------------------------
Bool cglsGoListView::AddItem(int id, const char* pszBlack, const char* pszWhite, int teai, int cnt, unsigned int result, unsigned int day, unsigned int time, color_t color=0)
{
	cglsListView* listView = (cglsListView*)GetChild(1);

	listView->AddItem(id, this->m_color, 0, pszBlack, 
	return 0;
}

// ------------------------------
Bool cglsGoListView::InsertItem(int id, int index, const char* pszBlack, const char* pszWhite, int teai, int cnt, unsigned int result, unsigned int day, unsigned int time, color_t color)
{
	cglsListView* listView = (cglsListView*)GetChild(1);
	return 0;
}

// ------------------------------
int cglsGoListView::UpdateItem(int id, int index, const char* pszBlack, const char* pszWhite, int teai, int cnt, unsigned int result, unsigned int day, unsigned int time, color_t color)
{
	cglsListView* listView = (cglsListView*)GetChild(1);
	return 0;
}

// ------------------------------
int cglsGoListView::GetCurrentIndex()
{
	return 0;
}

// ------------------------------
int cglsGoListView::IndexToId(int index)
{
	return 0;
}

// ------------------------------
void cglsGoListView::Destroy()
{
	return 0;
}

// ------------------------------------
void cglsGoListView::OnThreadStart(int id, int param1, int param2)
{
	if (id==1)
	{
	}
}


// ------------------------------------
LRESULT cglsGoListView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	int id = (wParam&0xFFFF);

	switch(id)
	{
		case (xIDSEARCH+1):
		case (xIDSEARCH+2):
		case (xIDSEARCH+3):
		case (xIDSEARCH+4):
		case (xIDSEARCH+5):
		{
			cglsTabCtrl* tab = (cglsTabCtrl*)GetChild(xID+7);
			char szText[16]={0, };
			int tabIndex = tab->GetCurSel()-1;

			cglsEditBox* edit = (cglsEditBox*)GetChild(xIDSEARCH+1);

			edit->GetText(m_filter[tabIndex].szUserName, 16);

			for (int i=0; i<4; i++)
			{
				cglsSplitButton* btn = (cglsSplitButton*)GetChild(i+xIDSEARCH+2);
				m_filter[tabIndex].filter[i]=btn->GetSelectIndex();
			}
			

			m_bIsQuitThread[tabIndex]=True;
			if (WaitThread(1, 1000)==False)
				break;

			m_bIsQuitThread=False;
			cglsListView* listView=(cglsListView*)GetChild(1);
			listView->Clear();

			StartThread(1, 0);

			break;
		}
	}
	return 0;
}
