#include "glsTitlePannel.h"

cglsTitlePannel::cglsTitlePannel(void)
{
	__GLASSINTERFACE_NAME__;
}

cglsTitlePannel::~cglsTitlePannel(void)
{
}

// ----------------------------------------
Bool cglsTitlePannel::Create(HWND hwnd, int id, int x, int y, int w, int h, const wchar_t* pszTitle, Font* font)
{
	sGlsDialogDetail xdetail;
	//int nMaxLabelWidth = CreateLabels();
	wcscpy(m_szCurrentTitle, pszTitle);

	cglsDialog::xDefaultDetail(&xdetail);
	xdetail.gauss.colorTop=__COLORp__(127, 0, 0, 0);
	xdetail.gauss.colorBottom=__COLORp__(200, 0, 0, 0);
	xdetail.font=font;
	xdetail.gauss.fBlureRad=4.0;
	xdetail.gauss.fShadowWidth=5.0;
	xdetail.bIsAnimationShow=False;

	if (h==0)
	{
		m_font=(Font*)cyiShape::Clone(font);//font->Clone();
		unsigned int pos = (unsigned int)cyiShape::BitmapStringEx(0, L"1", m_font, 0, 0);
		m_nFontHeightGDI = (pos&0xFFFF);
		h=m_nFontHeightGDI+2+(xdetail.gauss.fShadowWidth)*2;
	}

	//m_static.C
	return cglsDialog::Create(hwnd, id , 0, x, y, w, h, &xdetail);
}

// ----------------------------------------
Bool cglsTitlePannel::SetTitle(const wchar_t* pszTitle)
{
	wcscpy(m_szCurrentTitle, pszTitle);
	m_editor.SetText(m_szCurrentTitle);
	InvalidateRect(0);
	return True;
}

// ------------------------------------
int cglsTitlePannel::SetFont(Font* font)
{
	__super::SetFont(font);

	{
		int wMargin = cyiShape::GetShadowSize(m_hShadow);
		ReSize(m_w, GetFontHeight(True)+(wMargin<<1)+2);//m_editor.m_h+(wMargin<<1));
		//m_editor.ReSize(m_w-2, m_editor.m_h);
	}

	return 0;
}

// ----------------------------------------
wchar_t* cglsTitlePannel::GetTitle()
{
	return m_szCurrentTitle;
}


// ----------------------------------------
LRESULT cglsTitlePannel::OnTitleEdited(HWND hWnd, int code)
{
	if (code==EN_CHANGE)
	{
		m_editor.GetText(m_szCurrentTitle, 128);
		SendMessage(m_hNotifyWnd, WM_COMMAND, (EN_CHANGE<<16)|m_id, (LPARAM)m_hWnd);
	}
	return True;
}


// ----------------------------------------
LRESULT cglsTitlePannel::OnCreate(WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	sGlsEditBoxDetail detail;

	cglsEditBox::xDefaultDetail(&detail);

	detail.bIsFontSizeVariable=False;
	detail.bIsStaticEditorMode=True;
	detail.colorStaicModeFont=Color::White;
	detail.eAlign=eGIA_CENTER;
	detail.bIsMouseDownShow=True;

	GetClientRect(&rect);
	m_editor.Create((HWND)this, 1, rect.left+1, rect.top+1, rect.right-2, rect.bottom-2, &detail);
	m_editor.SetText(m_szCurrentTitle);
	m_editor.Show();

	InvalidateRect(0);

	return 0;
}

// ----------------------------------------
LRESULT cglsTitlePannel::OnSizeChanged(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	RECT rect;
	GetClientRect(&rect);

	//if (ReSize(m_w, GetFontHeight(True)+2+

	m_editor.ReSize(rect.right-2, rect.bottom-2);

	return 0;
}

// ----------------------------------------
static char* g_pszJosekiNameHigh[] = {
	"選択なし",
	"小目",
	"目はずし",
	"高目",
	"星",
	"三々",
	0
};

// ----------------------------------------
static char* g_pszJosekiNameMiddle[] = {
	"なし",
	"小桂馬がかり",
	"一間高がかり",
	"二間高かかり",
	"大桂馬かかり",
	"高かかり",
	"隅がかり",
	"打ち込み",
	"その他",
	0
};

// ----------------------------------------
static char* g_pszJosekiNameDetail[] = {
	"なし",
	"一間ばさみ",
	"一間高ばさみ",
	"二間ばさみ",
	"二間高ばさみ",
	"三間ばさみ",
	"三間高ばさみ",
	"こすみ",
	"下つけ",
	"上つけ",
	"一間とび",
	"小桂馬",
	"大桂馬",
	"両がかり"
	"その他",
	0
};

#include "sgfManager.h"

static int xGetJosekiName(char* pszJosekiName, cyiTreeNode* node, int& _h, int& _l, int& _d)
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();
	cyiTreeNode* t = node;
	int joseki_code=0;
	int h=0, l=0, d=0;
	Bool bIsHasAName=False;
	_h=0;
	_l=0;
	_d=0;

	pszJosekiName[0]=0;
	if (info==0)
		return -1;

	while(t)
	{
		info = (cSgfStoneInfo*)t->GetData();

		if (info==0)
		{
			return -1;
		}

		if (info->exinfo!=0)
		{
			joseki_code = GET_JOSEKI_NAME(info->exinfo->flag32);


			if (joseki_code!=0)
			{
				h = GET_JOSEKI_HIGH(joseki_code);
				l = GET_JOSEKI_LOW(joseki_code);
				d = GET_JOSEKI_DETAIL(joseki_code);

				if (node==t)
				{
					bIsHasAName=True;
				}

				if (d!=0)
					_d=d;
				if (l!=0)
					_l=l;
				if (h!=0)
					_h=h;

				if (h!=0)
					break;
			}
		}

		t=t->GetParent();
	}

	if (_h==0)
		return -1;

	strcpy(pszJosekiName, g_pszJosekiNameHigh[_h]);

	if (_l==0)
	{
		if (bIsHasAName==True)
		{
			return 1;
		}
		return 0;
	}
	strcat(pszJosekiName, "-");
	strcat(pszJosekiName, g_pszJosekiNameMiddle[_l]);

	if (_d==0)
	{
		if (bIsHasAName==True)
		{
			return 1;
		}
		return 0;
	}

	strcat(pszJosekiName, "-");
	strcat(pszJosekiName, g_pszJosekiNameDetail[_d]);

	if (bIsHasAName==True)
		return 1;

	return 0;
}

// ----------------------------------------
LRESULT cglsTitlePannel::OnNotifyNodeChanged(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	//yiTraceM2("c:\\yotsuya\\x.log", "cglsTitlePannel::OnNotifyNodeChanged() Enter");
	// 定石辞典モードのみとみなしてよい？
	if (wParam==0) 
	{
		cyiTreeNode* t = (cyiTreeNode*)lParam;
		int h, l, d;
		char szJosekiName[128];
		szJosekiName[0]=0;
		xGetJosekiName(szJosekiName, t, h, l, d);

		wchar_t swzJosekiName[128];
		yiToUnicode(swzJosekiName, szJosekiName, 0);

		if (swzJosekiName[0]==0)
		{
			SetTitle(L"よつや定石データベース");
		}
		else
		{
			SetTitle(swzJosekiName);
		}

		//InvalidateRect(0, True, True);
	}

	return 0;
}

