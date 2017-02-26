#include "cglsResultDialog.h"
#include "yiGoDefs.h"

cglsResultDialog::cglsResultDialog(void)
{
}

cglsResultDialog::~cglsResultDialog(void)
{
}

// ------------------------------
Bool cglsResultDialog::ShowResult(cGlassInterface* parent, goid_t id, sGoHeader* header, sResult* result, Font* font)
{
	sGlsDialogDetail xdetail;
	cglsDialog::xDefaultDetail(&xdetail);
	xdetail.font=font;
	xdetail.gauss.fBlureRad=5.0;
	xdetail.gauss.fShadowWidth=7.0;
	xdetail.bIsAnimationShow=True;
	xdetail.nShowAnimationTime=200;
	xdetail.nHideAnimationTime=300;

	int w=10;
	int h=10;
	int x=(parent->m_w-w)>>1;
	int y=(parent->m_h-h)>>1;

	m_bIsExitLoop=False;
	m_bIsSizeFixedMode=False;

	m_header=header;
	
	memset(&m_result, 0, sizeof(m_result));
	if (result)
		m_result=*result;

	Bool res = cglsDialog::Create((HWND)parent, 1024+id, 0, x, y, w, h, &xdetail);

	if (res==True)
	{
		sGlsAnimationEx ani;
		glsInitAnimationEx(&ani, &m_item);
		glsSetTrAnimationEx(&ani, 0.0, 1.0, eGAP_FISO);
		glsSetSzAnimationEx(&ani, 0.0, 1.0, eGAP_FISO, (eGlsItemAlign)(eGIA_CENTER|eGIA_VCENTER));
		AnimationShow(True, &ani, 20, 1000);
		//AnimationShow(True, 1000);
	}
	return False;
}

#define xID 16

// --------------------------------------------
LRESULT cglsResultDialog::OnCreate(WPARAM wParam, LPARAM lParam)
{
	int winer = GET_WINER(m_header->flag32);//condition
	int score = GET_SCORE(m_header->flag32);
	Bool bIsHalfScore= HALF_SCORE(m_header->condition);
	int komi = GET_KOMI(m_header->condition);
	int ba=0;
	int wa=0;
	float fkomi=komi;
	Bool bIsReverseKomi = REVERSE_KOMI(m_header->condition);
	// ここでテキスト作成
	wchar_t szTitle[32];
	wchar_t szDetail[128];

	fkomi += bIsHalfScore?0.5:0;
	fkomi = bIsReverseKomi?-fkomi:fkomi;


	cglsStatic* label = new cglsStatic();
	sGlsStaticDetail detail;
	cglsStatic::xDefaultDetail(&detail);
	detail.id=xID+1;

	detail.gls.font = new Font(L"ＭＳ Ｐ明朝", m_font->GetSize()*3.0);
	//detail.gls.font = (Font*)cyiShape::Clone(GetFont(), 3.0);



	if (winer==3)
	{
		wcscpy(szTitle, L"ジゴ");
	}
	else if (score==0xFF)
	{
		if (winer==1)
		{
			wcscpy(szTitle, L"黒 中押し勝ち!");
		}
		else
		{
			wcscpy(szTitle, L"白 中押し勝ち!");
		}
	}
	else if (score==0xFE)
	{
		if (winer==1)
		{
			wcscpy(szTitle, L"黒 時間勝ち!");
		}
		else
		{
			wcscpy(szTitle, L"白 時間勝ち!");
		}
	}
	else
	{
		ba = m_result.nBlackArea+m_result.nWhiteDeadStone;
		wa = m_result.nWhiteArea+m_result.nBlackDeadStone;
		float fScore;
		
		if (winer==1)
		{
			 fScore = (float)ba-(float)wa-fkomi;
		}
		else
		{
			fScore = (float)wa-(float)ba+fkomi;
		}

		if (bIsHalfScore==False)
		{
			wsprintfW(szTitle, L"%s %d目勝ち!", winer==1?L"黒":L"白", (int)fScore);
		}
		else
		{
			if ((int)fScore==0)
			{
				wsprintfW(szTitle, L"%s 半目勝ち!", winer==1?L"黒":L"白", (int)fScore);
			}
			else
			{
				wsprintfW(szTitle, L"%s %d目半勝ち!", winer==1?L"黒":L"白", (int)fScore);
			}
		}
		
	}

	label->Create((HWND)this, 0, 0, 0, 0, 0, szTitle, &detail);
	label->Show();
	delete detail.gls.font;
	detail.gls.font=0;


	if (m_result.nBlackArea==0 && m_result.nWhiteArea==0)
	{
		goto SKIP_DETAIL;
	}

	if (bIsReverseKomi==False)
	{
		wsprintfW(szDetail, L"黒  地(%d)+アゲハマ(%d)", m_result.nBlackArea, m_result.nWhiteDeadStone);
	}
	else
	{
		wsprintfW(szDetail, L"黒  地(%d)+アゲハマ(%d)+コミ(%d目%s)= %d", m_result.nBlackArea, m_result.nWhiteDeadStone, komi, bIsHalfScore?L"半":L"");
	}
	label = new cglsStatic();
	detail.id=xID+2;
	label->Create((HWND)this, 0, 0, 0, 0, 0, szDetail, &detail);
	label->Show();

	if (bIsReverseKomi==False)
	{
		wsprintfW(szDetail, L"白  地(%d)+アゲハマ(%d)+コミ(%d目%s)", m_result.nWhiteArea, m_result.nBlackDeadStone, komi, bIsHalfScore?L"半":L"");
	}
	else
	{
		wsprintfW(szDetail, L"白  地(%d)+アゲハマ(%d)", m_result.nWhiteArea, m_result.nBlackDeadStone);
	}
	label = new cglsStatic();
	detail.id=xID+3;
	label->Create((HWND)this, 0, 0, 0, 0, 0, szDetail, &detail);
	label->Show();

SKIP_DETAIL:
	cglsDialog::CreateButton(MB_OK);
	ReArrangeItemz();

	return 0;
}

// ------------------------------------
void cglsResultDialog::OnItemReArranged()
{
	int xOffset, yOffset;
	GetContentsOffset(xOffset, yOffset);
	int maxW=0, maxH=0;

	for (int i=xID+1; i<xID+4; i++)
	{
		cGlassInterface* child = GetChild(i);

		if (child==0)
			continue;
		
		maxW = maxW<child->m_w?child->m_w:maxW;
		maxH +=child->m_h;
	}
	SetContentsSize(maxW, maxH);

	GetChild(xID+1)->Move((m_w-GetChild(xID+1)->m_w)>>1, yOffset);

	if (GetChild(xID+2))
	{
		GetChild(xID+2)->Move(xOffset, GetChild(xID+1)->m_h+GetChild(xID+1)->m_y+GetFontHeight(True)/2);
		GetChild(xID+3)->Move(xOffset, GetChild(xID+2)->m_h+GetChild(xID+2)->m_y);
	}
	InvalidateRect(0, 0, True);
	Move((m_parent->m_w-m_w)>>1, (m_parent->m_h-m_h)>>1);
	return ;
}

// ------------------------------------
LRESULT cglsResultDialog::OnCommand(WPARAM wp, LPARAM lp, BOOL& handled)
{
	Hide();
	Destroy();
	return 0;
}
