#include "glsStoneInfoDlg.h"

cglsStoneInfoDlg::cglsStoneInfoDlg(void)
: m_node(0)
{
}

cglsStoneInfoDlg::~cglsStoneInfoDlg(void)
{
}

// ----------------------------------------
Bool cglsStoneInfoDlg::Create(HWND hwnd, int id, int x, int y, int w, int h, cyiTreeNode* t, Font* font)
{
	sGlsDialogDetail xdetail;

	cglsDialog::xDefaultDetail(&xdetail);

	xdetail.gauss.fShadowWidth=5.0;
	xdetail.gauss.fBlureRad=4.0;
	xdetail.font=0;
	xdetail.bIsAnimationShow=False;

	m_node=t;
	//m_header=header;

	//m_static.C
	Bool res = cglsDialog::Create(hwnd, id , 0, x, y, w, h, &xdetail);

	if (res==True)
	{
		SetFont(font);
		return True;
	}

	return False;
}

#include "sgfmanager.h"

typedef struct xStoneValueAndCode {
	const char* pszValue;
	int code;
} sStoneValueAndCode;

static sStoneValueAndCode g_pStoneValue[] = {
	{"普通の手(無難)", eSV_NORMAL},
	{"本手", eSV_HONTE},
	{"悪手（はまり）", eSV_BAD},
	{"イマイチな手", eSV_NOGOOD},
	{"疑問手", eSV_QUESTION},
	{"好手",eSV_GOOD},
	{"強手",eSV_KYOSYU},
	{"この一手(絶対)",eSV_ABSOLUTE},
	{"簡明策",eSV_SIMPLE},
	{"はめ手",eSV_TRICK},
	{"難解な手",eSV_DEFICULT},
	{"先手を取る手",eSV_SENTE},
	{"手筋",eSV_TESUZI},
	{"急所の一手",eSV_KYUSYO},
	{"場合の手",eSV_CASE},
	{"形",eSV_KATACHI},
	{"妙手",eSV_MYOSYU},
	{"筋悪の手",eSV_BADSUJI},
	{"黒の効き・狙い",eSV_BLACK_KIKI},
	{"白の効き・狙い",eSV_WHITE_KIKI},
	{"シチョウ有利",eSV_SICHO},
	{"シチョウ不利",eSV_NOSICHO},
	{"コウ材有利",eSV_KOU},
	{"コウ材不利",eSV_NOKOU},
	{0, 0}
};

static sStoneValueAndCode g_pszResult[] = {
	{"状況次第",eGRS_UNKNOWN},
	{"互角の別れ",eGRS_SAME},
	{"黒優勢",eGS_BLACK_GOOD},
	{"白優勢",eGS_WHITE_GOOD},
	{"黒やや良し",eGS_BLACK_LITTLE_GOOD},
	{"白やや良し",eGS_WHITE_LITTLE_GOOD},
	{"黒つぶれ",eGS_BLACK_TEREBLE},
	{"白つぶれ",eGS_WHITE_TEREBLE},
	{"黒状況次第",eGRS_BLACK_OTHER},
	{"白状況次第",eGRS_WHITE_OTHER},
	0
};

// ----------------------------------------
Bool cglsStoneInfoDlg::UpdateTree(cyiTreeNode* t)
{
	cglsStatic* label = (cglsStatic*)GetChild(16);
	cSgfStoneInfo* info = (cSgfStoneInfo*)t->GetData();
	char* pszTitle=0;
	char* pszResult=0;
	char buffer[64];

	buffer[0]=0;

	if (info==0)
	{
		label->SetText(0, L"");
		label = (cglsStatic*)GetChild(17);
		label->SetText(0, L"");
		return True;
	}


	if (info->exinfo)
	{
		int option=info->exinfo->option;

		if (option&eGRO_HAMETE)
		{
			//nIndex=2;
			pszTitle="ハメ手";
		}
		else if (option&eGRO_JOSEKI)
		{
			pszTitle="定石";
		}
		else if (option&eGRO_JOSEKI_OTHER)
		{
			pszTitle="変化";
		}
		else if (option&eGRO_JOSEKI_AFTER)
		{
			pszTitle="定石後の変化";
		}
		//else if (option&eGRO_JOSEKI_TENUKI)
		//{
		//	SendMessage(GetDlgItem(hWnd, IDC_CMB_TYPE), CB_SETCURSEL, 5, 0);
		//}
		else if (option&eGRO_JOSEKI_CASE)
		{
			pszTitle="場合の手";
		}

		if (pszTitle)
			strcpy(buffer, pszTitle);
		else
			strcpy(buffer, "変化");

		if (option&eGRO_NERAI)
		{
			strcat(buffer, "/ネライあり");
			//SendMessage(GetDlgItem(hWnd, IDC_CHK_KIKI), BM_SETCHECK, (WPARAM)1, 0L);
		}
		if (option&eGRO_SICHOU)
		{
			strcat(buffer, "/シチョウ関係");
		}
		if (option&eGRO_SENTE)
		{
			char xxx[16];
			eStoneType type = STONE_TYPE(info->stone);
			wsprintf(xxx, "/%s先手", type==eST_BLACK?"白":"黒");
			strcat(buffer, xxx);
		}
		if (option&eGRO_OKIGO)
		{
			strcat(buffer, "/置き碁定石");
			//SendMessage(GetDlgItem(hWnd, IDC_CHK_OKIGO), BM_SETCHECK, (WPARAM)1, 0L);
		}
		//if (option&eGRO_EDIT_COMPRETE)
		//{
		//	SendMessage(GetDlgItem(hWnd, IDC_CHK_EDIT_COMPLETE), BM_SETCHECK, (WPARAM)1, 0L);
		//}
	}

	if (pszTitle!=0 ||
			t->GetChildList().count()==0)
	{
		if (info->exinfo)
		{
			int cnt=0;
			while(g_pszResult[cnt].pszValue)
			{
				if (info->exinfo->status==g_pszResult[cnt].code)
				{
					pszResult=(char*)g_pszResult[cnt].pszValue;
					break;
				}
				cnt++;
			}
		}
		else
		{
			pszResult=(char*)g_pszResult[0].pszValue;
		}
	}

	if (pszTitle!=0 ||
			t->GetChildList().count()==0)
	{
		label->SetText(0, pszResult);
		label = (cglsStatic*)GetChild(17);

		wchar_t xx[64];

		label->SetText(0, yiToUnicode(xx, buffer, 0));
		label->InvalidateRect(0, True);
		/*if (g_pszResult)
		{
			label->SetText(0, g_pszResult);
		}
		else
		{
			label->SetText(0, "変化");
		}*/
	}
	else
	{
		if (info->exinfo)
		{
			for (int i=0; g_pStoneValue[i].pszValue!=0; i++)
			{
				if (g_pStoneValue[i].code==info->exinfo->stonevalue)
				{
					label->SetText(0, g_pStoneValue[i].pszValue);
					break;
				}
			}
		}
		else
		{
			label->SetText(0, g_pStoneValue[0].pszValue );
		}
		label->InvalidateRect(0, True);

		label = (cglsStatic*)GetChild(17);
		label->SetText(0, L"");
		label->InvalidateRect(0, True);
	}
	
	InvalidateRect(0, True, True);
	return True;

}




// ----------------------------------------
LRESULT cglsStoneInfoDlg::OnCreate(WPARAM wParam, LPARAM lParam)
{
	sGlsStaticDetail detail;

	cglsStatic::xDefaultDetail(&detail);
	cglsStatic* label = new cglsStatic();

	detail.id=16;
	label->Create((HWND)this, 0, 0, 0, 0, 0, L"", &detail);
	label->Show();

	label = new cglsStatic();
	detail.id=17;
	label->Create((HWND)this, 0, 0, 0, 0, 0, L"", &detail);
	label->Show();

	UpdateTree(m_node);
	return ReArrangeItemz();
}

// ------------------------------------
void cglsStoneInfoDlg::OnItemReArranged()
{
	int xOffset;
	int yOffset;
	int width=0, height=0;
	Font* fontLarge = (Font*)cyiShape::Clone(GetFont(), 2.0);

	cglsStatic* label = (cglsStatic*)GetChild(16);
	label->SetFont(fontLarge);

	delete fontLarge;

	GetContentsOffset(xOffset, yOffset);

	label->Move(xOffset, yOffset);
	width+=label->m_w;
	height+=label->m_h;

	label = (cglsStatic*)GetChild(17);
	label->Move(xOffset, yOffset+height);

	cGlassInterface::Update(eGUM_FORCED);

	ReSize(m_w, height+(yOffset<<1));

	InvalidateRect(0, 0, True);
	return ;
}

// ----------------------------------------
LRESULT cglsStoneInfoDlg::OnNotifyNodeChanged(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	// 定石辞典モードのみとみなしてよい？
	if (wParam==0) 
	{
		cyiTreeNode* t = (cyiTreeNode*)lParam;
		UpdateTree(t);
	}

	return 0;
}