#include "glsMyFriendView.h"
#include "glsListView.h"

#define xID 16

DEFINE_RESOURCE_BEGIN(_gRESOURCE)
	//DEFINE_SPILIT_BUTTON(xID+1, 0, 0, 0, 0, 0, 8, 0)
	//DEFINE_RESOURCE_PARAM(xID+1, eGIT_LISTBOX, L"", 0, 0, 10, 7, 0)
	DEFINE_RESOURCE(xID+16, eGIT_LABEL, L"Œ»Ý“o˜^‚³‚ê‚½—Fl‚ª‚ ‚è‚Ü‚¹‚ñB\r\n—Fl‚ÌID‚ð“ü—Í‚µ“o˜^‚ðs‚Á‚Ä‚­‚¾‚³‚¢B", 0, 0, 0)
	DEFINE_RESOURCE(xID+8, eGIT_PICTURE, 0, 0, 0, 0) //xID+1, eGP_BOTTOMSIDE|eGP_LEFT, eIM_SMALL_Y)
	DEFINE_RESOURCE_PARAM(xID+3, eGIT_BUTTON, L"“o˜^", 0, 0, 0, 0, 0)
	DEFINE_EDIT_BOX(xID+2, 8, False, False, 16, xID+3, eGP_LEFTSIDE|eGP_BOTTOM, eIM_SMALL_X)
	DEFINE_RESOURCE(xID+4, eGIT_LABEL, L"", 0, 0, 0)
	DEFINE_RESOURCE_PARAM(xID+7, eGIT_BUTTON, L"ŠÏí", 0, 0, 0, 0)
	DEFINE_RESOURCE_PARAM(xID+6, eGIT_BUTTON, L"‘Î‹Ç\‚µž‚Ý", xID+7, eGP_LEFTSIDE|eGP_TOP, 0, 0)

DEFINE_RESOURCE_END()

cglsMyFriendView::cglsMyFriendView(void)
: m_nGroupChangedCommand(0)
, m_nPlayCommand(0)
, m_nWatchCommand(0)
, m_mgr(0)
, m_nLastSelectedUserIndex(0)
, m_nRegisteredUserCnt(0)
, m_bmpLogin(0)
, m_bmpLogout(0)
, m_currentUserId(0)
, m_flagUpdate(0)

{
}

cglsMyFriendView::~cglsMyFriendView(void)
{
}


// ------------------------------------
Bool cglsMyFriendView::Create(cGlassInterface* parent, int x, int y, int w, int h, CUserManager* mgr, Font* _font)
{
	sGlsDialogDetail xdetail;
	cglsDialog::xDefaultDetail(&xdetail);

	float fFontSize = 12.0;
	Font font(L"Meiryo UI", fFontSize);

	m_mgr=mgr;

	xdetail.font=_font?_font:&font;
	xdetail.gauss.fBlureRad=5.0;
	xdetail.gauss.fShadowWidth=5.0;
	xdetail.gauss.colorTop=__COLORp__(128, 180,200,255);
	//xdetail.gauss.colorTop=;
	
	m_bIsSizeFixedMode=False;
	m_fMarginX=1.0;
	m_fMarginY=1.0;
	xdetail.bIsAnimationShow=True;
	xdetail.nShowAnimationTime=200;
	xdetail.nHideAnimationTime=300;

	Bool res = cglsDialog::Create((HWND)parent, 256 , 0, x, y, w, h, &xdetail);

	sGlsAnimationEx ani;
	glsInitAnimationEx(&ani, &m_item);
	glsSetTrAnimationEx(&ani, 0.0, 1.0, eGAP_FISO);
	glsSetSzAnimationEx(&ani, 0.0, 1.0, eGAP_FISO, (eGlsItemAlign)(eGIA_CENTER|eGIA_VCENTER));
	AnimationShow(True, &ani, 20, 1000);


	//if (_font)
	//	delete _font;

	return res;
}

// ------------------------------------
void cglsMyFriendView::UpdateRegisterData(unsigned id)
{
	char text[128];
	int win, loses, draws;
	char* pszStatus=(char*)"Ú‘±’†";
	
	
	if (m_mgr->GetUserHandle(id)==0)
	{
		pszStatus=(char*)"ƒIƒtƒ‰ƒCƒ“";
	}

	sRegisterUserInfo* reg = m_mgr->GetRegisterInfo(id);

	if (reg==0)
	{
		cglsStatic* label = (cglsStatic*)GetChild(xID+16);
		label->SetText(0, "");
		return ;
	}
	m_mgr->GetScore(id, win, loses, draws);

	int vwin, vloses, vdraws;
	m_mgr->GetVsScore(id, vwin, vloses, vdraws);
	wsprintf(text, "%s %dË %s\r\níÑF%dí%dŸ%d”s%d•ª\r\n‘ÎííÑF%dí%dŸ%d”s%d•ª\r\n%s", GetAreaName(reg->nAreaCode, 0), 
		yiUtilGetAge(reg->birthday, 0), reg->flag&0x80000000?"’j«":"—«", win+loses+draws, win, loses, draws,
				vwin+vloses+vdraws, vwin, vloses, vdraws, pszStatus);

	cglsStatic* label = (cglsStatic*)GetChild(xID+16);
	label->SetText(0, text);

	label->InvalidateRect(0);
	//InvalidateRect(0);
}

// ------------------------------------
void cglsMyFriendView::UpdatePicture(unsigned id)
{
	cglsImageBox* img = (cglsImageBox*)GetChild(xID+8);
	unsigned int seq;

	Bitmap* bmp = m_mgr->GetPicture(id, seq);
	img->SetImage(bmp?cyiBitmapBlender::BitmapCopy(0, bmp):0, eGIBVS_TRIM_BORDER_FIXED);
}

// ------------------------------------
void cglsMyFriendView::AddGoHeader(int index, sSimpleGoHeader* header, Bool bIsLastUpdate, color_t color)
{
	cglsListView* listView = (cglsListView*)GetChild(xID+5);

	if (listView==0)
		return ;

	char szBlackName[16];
	char szWhiteName[16];
	wchar_t* pwszWiner;
	wchar_t szResult[8]={0,};
	wchar_t szDay[32];
	wchar_t szTeai[5];
	Bool bIsIamBlack=(header->result&0x80000000)!=0;
	const char* pszSelectedUserName = m_mgr->GetUserName(m_currentUserId);
	
	const char* pszUserName = m_mgr->GetUserName(header->remoteId);
	if (bIsIamBlack)
	{
		strcpy(szBlackName, pszSelectedUserName);//m_mgr->GetMyName());

		if (pszUserName)
			strcpy(szWhiteName, pszUserName);
		else
			strcpy(szWhiteName, "?????");
	}
	else
	{
		strcpy(szWhiteName, pszSelectedUserName);//, m_mgr->GetMyName());

		if (pszUserName)
			strcpy(szBlackName, pszUserName);
		else
			strcpy(szBlackName, "?????");
	}

	int score = (header->result>>19)&0xFF;
	int n = (header->result>>15)&0x0F;
	Bool bIsHalfScore = (header->result&(1<<28))!=0; //HALF_SCORE(header->condition);

	int winer = (header->result>>29)&0x03;
	
	if (winer==1)
		pwszWiner=L"•";
	else if (winer==2)
		pwszWiner=L"”’";
	else if (winer==3)
		pwszWiner=L"ƒW";
	else
		pwszWiner=L"--";

	if (n==0)
	{
		if (bIsHalfScore==True)
		{
			wcscpy(szTeai, L"ŒÝæ");
		}
		else
		{
			wcscpy(szTeai, L"’èæ");
		}
	}
	else
	{
		wsprintfW(szTeai, L"%dŽq", n);
	}


	if (winer==2 || winer==1)
	{
		if (score==0xFF)
		{
			wsprintfW(szResult, L"’†");
		}
		else if (score==0xFE)
		{
			wsprintfW(szResult, L"Žž");
		}
		else if (bIsHalfScore==True)
		{
			wsprintfW(szResult, L"%d.5", score);
		}
		else
		{
			wsprintfW(szResult, L"%d", score);
		}
	}
	else if (winer==0)
	{
		wsprintfW(szResult, L"i");
	}

	wsprintfW(szDay, L"%d/%02d/%02d", DAY_YEAR(header->day), DAY_MON(header->day), DAY_DAY(header->day));

	//cglsListView* listView = (cglsListView*)GetChild(xID+8);
	listView->InsertItem(index, 0,
		color,
		0, (wchar_t*)szBlackName,
			0, szWhiteName,
				0, szTeai, 
					0, (header->result>>6)&0x1FF,
						0, pwszWiner,
							0, szResult,
								0, szDay);


	if (bIsLastUpdate==True)
	{
		listView->Update(eGUM_FORCED);
		if (IsWindowVisible(*this)==True)
		{
			listView->InvalidateRect(0);
		}
	}
}

// ------------------------------------
void cglsMyFriendView::UpdateGoData(unsigned int id)
{
	xTRACE("cglsMyFriendView::UpdateGoData(%x)", id);
	int size;
	int n;
	int cnt=0;
	sSimpleGoHeader* header = m_mgr->GetGoHeader(id, &size);
	cyiList* list = m_mgr->GetPlayingGoHeader(id);

	//list=0;
	if (header==0 && list==0)
	{
		cglsListView* listView = (cglsListView*)GetChild(xID+5);
		listView->Clear();
		listView->Update(eGUM_FORCED);
		listView->InvalidateRect(0);
		return ;
	}

	cglsListView* listView = (cglsListView*)GetChild(xID+5);
	listView->Clear();

	if (header==0)
		goto SKIP_GODATA;

	n = size>>4;
	

	int xx=list?list->count():0;
	for (int i=0; i<n; i++)
	{
		AddGoHeader(i+xx+1, &header[i], False);//i==(n-1));
	}

SKIP_GODATA:
	if (list)
	{
		for (cyiNode* node=list->GetNodeTopFrom(); node; node=node->np)
		{
			sSimpleGoHeader* hd = (sSimpleGoHeader*)node->ref;
			AddGoHeader(cnt+1, hd, False/*(cnt==(list->count()-1))*/, Color::Red);
			cnt++;
		}
	}

	listView->Update(eGUM_FORCED);
	listView->InvalidateRect(0);

	xTRACE("cglsMyFriendView::UpdateGoData(%x) exit", id);
}

// ------------------------------------
void cglsMyFriendView::UpdateFriends(unsigned int id, int eFlag)
{
	if (m_mgr->GetMyId()==0)
		return ;

	if (m_mgr->GetMyId()==id)
		return ;

	if (id==m_currentUserId ||
			id==0)
	{
		if (IsWindowVisible(*this)==False)
		{
			m_flagUpdate=1;
			return ;
		}
	}

	cglsListBox* listBox =(cglsListBox*)GetChild(xID+1);
	int cnt=0;
	int n;
	sUser* user=m_mgr->GetMyFriends();
	unsigned int userId = listBox->GetSelectedItem();
	//cglsSplitButton* btn=0;
	cglsListBox* listbox =0;

	if (id==0)
		goto ALL;

	if (userId==-1)
		userId=0;

	userId=user[userId-1].id;


	//if (eFlag==0xFF)
	//	goto ALL;


	if (id!=userId)
	{
		return ;
	}
	m_currentUserId=id;

	if (m_mgr->GetUserHandle(id)==0)
	{
		GetChild(xID+6)->Enable(False);
		GetChild(xID+7)->Enable(False);
	}
	else
	{
		GetChild(xID+6)->Enable(True);
		//GetChild(xID+7)->Enable(True);
	}

	// ŒÂlî•ñ
	if (eFlag&eMFVU_REG)
	{
		UpdateRegisterData(userId);
	}
	if (eFlag&eMFVU_GODATA)
	{
		UpdateGoData(userId);
	}
	if (eFlag&eMFVU_PICTURE)
	{
		unsigned int seq;
		UpdatePicture(userId);
	}

	return ;
ALL:
	n=m_mgr->GetMyFriendsCount();


	listbox = (cglsListBox*)GetChild(xID+1);
	//btn = (cglsSplitButton*)GetChild(xID+1);

	listbox->Clear();
	//btn->Clear();//->Clear();

	if (n==0)
	{
		return ;
	}

	wchar_t wszUserName[16];



	for (int i=0; i<n; i++)
	{
		if (user[i].id==0)
			continue;

		cnt++;
		yiToUnicode(wszUserName, user[i].szUserName, 0);
		sRegisterUserInfo* info = m_mgr->GetRegisterInfo(user[i].id);

		if (m_mgr->GetUserHandle(user[i].id)!=0)
		{
			//sGlsListBoxItemDetail detail;
			listbox->AddItem(i+1, 0, wszUserName);
			//btn->AddItem(i+1, m_bmpLogin, wszUserName, 0, False);//(m_nLastSelectedUserIndex==i));
		}
	}

	for (int i=0; i<n; i++)
	{
		if (user[i].id==0)
			continue;

		yiToUnicode(wszUserName, user[i].szUserName, 0);
		sRegisterUserInfo* info = m_mgr->GetRegisterInfo(user[i].id);

		if (m_mgr->GetUserHandle(user[i].id)==0)
		{
			//sGlsListBoxItemDetail detail;
			//listbox->AddItemDetail(i+1, 0, wszUserName);
			listbox->AddItem(i+1, 0, wszUserName, Color::Gray);
			//listbox->EnableItem(i+1, False);
			//btn->AddItem(i+1, m_bmpLogout, wszUserName, 0, False);//(m_nLastSelectedUserIndex==i));
		}
	}

	listbox->Update(eGUM_FORCED);

	int index = listbox->GetSelectedIndex();//->GetSelectIndex();
	if (index!=m_nLastSelectedUserIndex)
	{
		listbox->SelectItem(m_nLastSelectedUserIndex);//->SelectIndex(m_nLastSelectedUserIndex);
		int id = listbox->GetSelectedItem();
		userId=user[id-1].id;
	}


	if (cnt==0)
	{
		ReArrangeItemz();
		return ;
	}

	//btn->Update(eGUM_FORCED);
	//btn->InvalidateRect(0);

	m_nRegisteredUserCnt=cnt;
	Bool bIsFullMode=IsWindowVisible(*GetChild(xID+6));

	if ((bIsFullMode==False &&cnt!=0) ||
		(bIsFullMode==True && cnt==0))
	{
		ReArrangeItemz();
	}

	if (userId!=0)
	{
		m_currentUserId=userId;
		UpdateRegisterData(userId);
		UpdateGoData(userId);
		UpdatePicture(userId);

	}
}

// ------------------------------------
void cglsMyFriendView::SetGroupChangedId(int id)
{
	m_nGroupChangedCommand=id;
	return ;
}

// ------------------------------------
void cglsMyFriendView::SetPlayCommandId(int id)
{
	m_nPlayCommand=id;
	return;
}

// ------------------------------------
void cglsMyFriendView::SetWatchCommandId(int id)
{
	m_nWatchCommand=id;
	return;
}
#include "glsListView.h"

// ------------------------------------
void cglsMyFriendView::CreateLoginBitmap(int nHeight)
{
	return ;
	if (m_bmpLogin!=0)
	{
		if (m_bmpLogin->GetWidth()==nHeight)
		{
			return ;
		}
		delete m_bmpLogin;
	}

	if (m_bmpLogout!=0)
		delete m_bmpLogout;

	m_bmpLogin=BITMAP32(nHeight, nHeight);
	m_bmpLogout=BITMAP32(nHeight, nHeight);


	{
		Graphics g(m_bmpLogin);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		g.FillEllipse(&SolidBrush(Color::LightGreen), 0, 0, nHeight-1, nHeight-1);
		g.DrawEllipse(&Pen(Color::Black), 0, 0, nHeight-1, nHeight-1);
	}

	{
		Graphics g(m_bmpLogout);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		g.FillEllipse(&SolidBrush(Color::Gray), 0, 0, nHeight-1, nHeight-1);
		g.DrawEllipse(&Pen(Color::DarkGray), 0, 0, nHeight-1, nHeight-1);
	}

}


// ------------------------------------
int cglsMyFriendView::SetFont(Font* font, Bool bIsReference)
{
	if (m_font)
		delete m_font;


	m_font=(Font*)cyiShape::Clone(font);//font->Clone();
	m_nFontHeight = cGlassInterface::CalculateFontHeight(m_font, False);//m_font->GetHeight(__DEFAULTGRAPHICS__);
	m_nFontHeightGDI = cGlassInterface::CalculateFontHeight(m_font, True);

	Font* fontSmall = (Font*)cyiShape::Clone(GetFont(), 0.9);
	int nSmallFontHeight=cGlassInterface::CalculateFontHeight(fontSmall, True);;

	CreateLoginBitmap(m_nFontHeightGDI*0.8);

	GetChild(xID+8)->SetFont(fontSmall);//ReSize();
	GetChild(xID+9)->SetFont(fontSmall);

	delete fontSmall;

	UpdateFriends();
	ReArrangeItemz();


	return 0;
}


// ------------------------------------
LRESULT cglsMyFriendView::OnCreate(WPARAM wParam, LPARAM lParam)
{
	if (cglsDialog::CreateChildFromResource(_gRESOURCE)==False)
		return False;


	/*cglsSplitButton* btn = (cglsSplitButton* )GetChild(xID+1);
	btn->AddItem(1, 0, L"—Fl", L"‘I‘ð‚È‚µ", True);
	btn->Show();*/


	

	Font* fontSmall = (Font*)cyiShape::Clone(GetFont(), 0.9);

	int nSmallFontHeight;
	
	m_nFontHeight	 = GetFont()->GetHeight(__DEFAULTGRAPHICS__);
	m_nFontHeightGDI = ((unsigned int)cyiShape::BitmapStringEx(0, L"1", m_font, 0, 0) & 0xFFFF);

	nSmallFontHeight = fontSmall->GetHeight(__DEFAULTGRAPHICS__);
	unsigned int pos = (unsigned int)cyiShape::BitmapStringEx(0, L"1", fontSmall, 0, 0);
	nSmallFontHeight = (pos&0xFFFF);

	cglsListBox* listBox = new cglsListBox();
	sGlsListBoxDetail listDetail;
	cglsListBox::xDefaultDetail(&listDetail);

	listBox->Create((HWND)this, xID+1, 0, 0, m_nFontHeight*7, m_nFontHeight*6, &listDetail);
	listBox->Hide();

	cglsListView* listView = new cglsListView();
	listView->Create(this, xID+5, 0, 0, 10, nSmallFontHeight*10, fontSmall);
	listView->CreateColumn(7, &sListViewColumn(12, L"•", eGIA_LEFT, eGCT_STRING), 
									&sListViewColumn(12, L"”’", eGIA_LEFT, eGCT_STRING), 
									 &sListViewColumn(5, L"Žè‡", eGIA_LEFT),
										&sListViewColumn(5, L"Žè”", eGIA_LEFT, eGCT_NUMERIC), 
											&sListViewColumn(3, L"Ÿ"),
												&sListViewColumn(5, L""),
													&sListViewColumn(11, L"“úŽž"));
	listView->Update(eGUM_FORCED);
	listView->Hide();

	CreateLoginBitmap(m_nFontHeight*0.8);
	UpdateFriends();
	cGlassInterface::Update(eGUM_FORCED);

	Bool res = ReArrangeItemz();
	UpdateFriends();

	if (::IsWindowVisible(GetChild(xID+1)->m_hWnd)==TRUE)
	{
		cglsSplitButton* btn = (cglsSplitButton*)GetChild(xID+1);
		btn->SelectIndex(0);
	}

	return True;
}

// ------------------------------------
void cglsMyFriendView::OnItemReArranged()
{
	int xOffset, yOffset;
	int w, w2;
	int maxWidth=0;
	int maxHeight=0;
	Font* fontLarge = (Font*)cyiShape::Clone(GetFont(), 2.0);

	GetContentsOffset(xOffset, yOffset);
	maxWidth=GetChild(xID+5)->m_w;


	GetChild(xID+1)->Move(xOffset, yOffset);

	GetChild(xID+8)->ReSize(GetFontHeight(True)*4, GetFontHeight(True)*4);

	GetChild(xID+16)->Move(GetChild(xID+1)->m_w+xOffset+(GetFontHeight(True)>>1), yOffset);//GetChild(xID+8)->m_y);

	GetChild(xID+3)->Move(xOffset+maxWidth-GetChild(xID+3)->m_w, GetChild(xID+1)->m_y+GetChild(xID+1)->m_h-GetChild(xID+3)->m_h);	

	GetChild(xID+5)->Move(xOffset, GetChild(xID+1)->m_y+GetChild(xID+1)->m_h+(GetFontHeight(True)>>2));
	
	GetChild(xID+7)->Move(xOffset+maxWidth-GetChild(xID+7)->m_w, GetChild(xID+5)->m_y+GetChild(xID+5)->m_h);
	
	//GetMaxWidthHeight(xID+1, xID+16, maxWidth, maxHeight);

	CreateLoginBitmap(m_nFontHeightGDI*0.8);


	SetContentsSize(maxWidth, GetChild(xID+7)->m_h+GetChild(xID+7)->m_y-yOffset);

	GetChild(xID+8)->Move(m_w-xOffset-GetChild(xID+8)->m_w, xOffset);

	if (m_nRegisteredUserCnt==0)
	{
		int h = GetChild(xID+16)->m_h+GetChild(xID+3)->m_h+(GetFontHeight(True)>>1);
		int w=0;
		int x;
		int y = (m_h-(h))>>1;
		GetChild(xID+16)->Move((m_w-GetChild(xID+16)->m_w)>>1, y);
		w=GetChild(xID+3)->m_w+GetChild(xID+2)->m_w;
		x = (m_w-w)>>1;

		GetChild(xID+3)->Move(x+GetChild(xID+2)->m_w, GetChild(xID+16)->m_y+GetChild(xID+16)->m_h+(GetFontHeight(True)>>1));

		for (int i=xID+1; i<=xID+16; i++)
		{
			cGlassInterface* child = GetChild(i);
			if (child==0)
				continue;
			child->Hide();
		}

		GetChild(xID+2)->Show();
		GetChild(xID+3)->Show();
		GetChild(xID+16)->Show();
		SetFocus(*GetChild(xID+2));
	}
	else
	{
		for (int i=xID+1; i<=xID+16; i++)
		{
			cGlassInterface* child = GetChild(i);
			if (child==0)
				continue;
			child->Show();
		}
	}


	Move((m_parent->m_w-m_w)>>1, (m_parent->m_h-m_h)>>1);
	InvalidateRect(0, 0, True);

	return ;
}

// ------------------------------------
unsigned int cglsMyFriendView::GetCurrentFriendId()
{
	return this->m_currentUserId;
}

// ------------------------------------
void cglsMyFriendView::Show()
{
	Bool bIsVisible=IsWindowVisible(*this);
	__super::Show();

	if (bIsVisible==False)
	{
		if (m_flagUpdate!=0)
		{
			m_flagUpdate=0;
			UpdateFriends();
		}
	}
}



#include "glsPopupEdit.h"
#include "glsMessageBox.h"
#include "yiGoCommandz.h"
// ------------------------------------
LRESULT cglsMyFriendView::OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	int id = (wParam&0xFFFF);

	switch(id)
	{
		case (xID+3):
		{
			char* pszUserId = (char*)yialloci(16);
			cglsEditBox* edit = (cglsEditBox*)GetChild(xID+2);

			edit->GetText(pszUserId, 16);
			GetSystem()->GetCommandCenter()->OnCommand(eGCID_ADD_FRIEND, this, (IntPtr)pszUserId);
			edit->SetText((const wchar_t*)0);
			break;
		}
		case (xID+5):
		{
			if (lParam==1)
			{
				int size;
				unsigned int id = wParam>>16;

				// ƒXƒNƒ[ƒ‹ƒo[H
				if (id==0xFFFF)
				{
					break;
				}

				if (m_currentUserId==0)
					break;

				sSimpleGoHeader* hd = m_mgr->GetGoHeader(m_currentUserId, &size);
				
				if (hd==0)
					return 0;
				
				cyiList* list = m_mgr->GetPlayingGoHeader(m_currentUserId);

				int from = list?list->count():0;

				if (id-1<from)
				{
					cglsSystem* system=GetSystem();
					if (system)
					{
						sSimpleGoHeader* hd = (sSimpleGoHeader*)list->GetTopFromRef(id-1);
						system->GetCommandCenter()->OnCommand(eGCID_WATCH_GO, this, m_currentUserId, (unsigned int)hd);
					}
					return 0;
				}

				sSimpleGoHeader*header = &hd[id-1-from];
				//if (node)
				{
					char szFilePath[256];
					//sSimpleGoHeader* header = (sSimpleGoHeader*)node->ref;

					Bool bIsImBlack=((header->result&0x80000000)!=0);

					wsprintf(szFilePath, "%08x%08x%08x%08x.sgf", bIsImBlack?m_currentUserId:header->remoteId, bIsImBlack?header->remoteId:m_currentUserId,
																			header->day, header->time);
					GetSystem()->GetCommandCenter()->OnCommand(eGCID_ADD_GOBAN, this, (IntPtr)szFilePath, (IntPtr)header);
				}
			}
			break;
		}
		case (xID+6):
		{
			cglsListBox* btn=(cglsListBox*)GetChild(xID+1);
			int id = btn->GetSelectedItem();//GetSelectIndex();
			sUser* u=m_mgr->GetMyFriends();

			GetSystem()->GetCommandCenter()->OnCommand(eGCID_REQ_PLAY_DIALOG, this, u[id-1].id, 0);

			break;
		}
		// —Fl‚ÌØ‚è‘Ö‚¦Žž
		case (xID+1):
		{
			if (m_mgr->GetMyId()==0)
				break;
			cglsListBox* btn=(cglsListBox*)GetChild(xID+1);

			if (m_nLastSelectedUserIndex==btn->GetSelectedIndex())
			{
				break;
			}
			m_nLastSelectedUserIndex=btn->GetSelectedIndex();

			sUser* u=m_mgr->GetMyFriends();
			m_currentUserId=u[btn->GetSelectedItem()-1].id;
			UpdateFriends(u[btn->GetSelectedItem()-1].id);
			break;
		}
		default:
			break;
	}

	return 0;
}

// ------------------------------------
LRESULT cglsMyFriendView::OnNotify(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	NMHDR* hdr=(NMHDR*)lParam;
	return 0;
}


// ------------------------------------
void cglsMyFriendView::OnThreadStart(int id, int param1, int param2)
{
	return ;
}
