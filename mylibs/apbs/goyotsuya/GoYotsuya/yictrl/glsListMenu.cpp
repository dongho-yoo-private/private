#include "glsListMenu.h"


typedef struct {
	int id;
	wchar_t* pszText;
	Bitmap* icon;
	cGlsAssistant* assistant;
} sGlsListMenuItem;

cglsListMenu::cglsListMenu(Font* font, Bool bIsNoMouseDown, Bool bIsMoveNotify)
: m_nTotalHeight(0)
, m_nTotalWidth(0)
, m_nResultCode(0)
, m_nInitSelect(0)
, m_nInitSelectIndex(-1)
, m_nMaxWidth(0)
, m_nMaxHeight(0)
, m_listAssistant(0)
, m_bIsNoMouseDown(bIsNoMouseDown)
, m_bIsMoveNotify(bIsMoveNotify)
, m_bIsSelectionCheck(False)
, m_bIsAutoScroll(True)
, m_pListBoxDetail(0)
{
	if (font)
		m_font=(Font*)cyiShape::Clone(font);
	else
	{
		m_font=new Font(L"Meiryo", 11.0);
	}
	m_bIsCreated=False;
}

cglsListMenu::~cglsListMenu(void)
{
}

#include "glsListBox.h"
// ------------------------------------
Bool cglsListMenu::TrackPopup(HWND hWndParent, int x, int y, int w, int h, Bool bIsSelectCheck, Font* font, Bool bIsAutoScroll, sGlsListBoxDetail* detail)
{
	//if (font)
	//	m_fontMenu=cyiShape::Clone(font);
	int res=0;
	//int m_nResultCode=0;

	if (GetChild(1)!=0 &&
			GetItemCount()==0)
		return False;

	m_bIsExitCode=0;
	m_bIsExitLoop=False;
	if (m_bIsCreated==True)
	{
		Move(x, y);


		Show();
		InvalidateRect(0);
		res = cglsWindow::MessageLoop(True);

		if (res==0)
		{
			m_bIsChanged=False;
			Hide();
			return 0;
		}

		cglsListBox* list = (cglsListBox* )GetChild(1);

		int nResultCode=list->GetSelectedItem();

		if (nResultCode==m_nResultCode)
		{
			this->m_bIsChanged=False;
		}
		else
		{
			m_bIsChanged=True;
		}

		m_nResultCode=nResultCode;//list->GetSelectedItem();
		Hide();
		return 1;
	}

	m_bIsAutoScroll=bIsAutoScroll;
	m_bIsSelectionCheck=bIsSelectCheck;

	m_bIsCreated=True;

	m_nMaxWidth=w;

	m_nMaxHeight=h;

	m_pListBoxDetail=detail;

	if (detail)
	{
		detail->bIsMsgLikeMenu=True;
		detail->bIsMouseEnterNotify=m_bIsMoveNotify;
		detail->bIsNoClicked=m_bIsNoMouseDown;
		detail->stack.gls.font=GetFont();

		detail->bIsSelectionCheck=m_bIsSelectionCheck;
		detail->bIsNoClicked=m_bIsSelectionCheck==False?True:False;
	}
	res = CreatePopup(hWndParent, x, y, 10, 10, &Font(L"Meiryo", 11.0)/*font*/, 0);

	if (res==False)
	{
		m_bIsChanged=False;
		Hide();
		return 0;
	}

	cglsListBox* list = (cglsListBox* )GetChild(1);
	
	int nResultCode=list->GetSelectedItem();

	if (nResultCode==m_nResultCode)
	{
		this->m_bIsChanged=False;
	}
	else
	{
		m_bIsChanged=True;
	}

	m_nResultCode=nResultCode;
	//m_nResultCode=list->GetSelectedItem();
	//list->Destroy();
	//delete list;
	//DestroyWindow(m_hWnd);
	//m_children->clear();
	Hide();



	return 1;
}

// ------------------------------------
int cglsListMenu::GetResultCode()
{
	return m_nResultCode;
}

// ------------------------------------
void cglsListMenu::SetCheck(int index, int id, Bool bIsChecked)
{
	if (m_bIsCreated==False)
	{
		m_nInitSelect=id;
		m_nInitSelectIndex=index;
		m_nResultCode=id;
	}
	else
	{
		cglsListBox* list=(cglsListBox* )GetChild(1);
		
		list->SelectItem(index, id, bIsChecked);
	}
}

// ------------------------------------
void cglsListMenu::Close()
{
	m_bIsExitLoop=True;
	return ;
}

// ------------------------------------
void cglsListMenu::Clear()
{
	cglsListBox* list = (cglsListBox*)GetChild(1);
	list->Clear();
}


// ------------------------------------
void cglsListMenu::SetExceptWindow(cGlassInterface* obj)
{
	m_exceptionObject=obj;
}

// ------------------------------------
Bool cglsListMenu::RemoveItem(int index)
{
	sGlsListMenuItem* item = (sGlsListMenuItem*)m_TextList.removeEx(index);

	if (GetChild(1)!=0)
	{
		cglsListBox* listbox=(cglsListBox*)GetChild(1);
		listbox->Remove(item->id);
		listbox->Update(eGUM_FORCED);

		listbox->ReSizeByItemz();
		ReSize(listbox->m_w, listbox->m_h);
	}
	return True;
}

// ------------------------------------
int cglsListMenu::GetItemCount()
{
	if (GetChild(1)==0)
		return 0;
	cglsListBox* listbox = (cglsListBox*)GetChild(1);
	return listbox->GetItemCount();
}

// ------------------------------------
int cglsListMenu::GetIndex(int id)
{
	cglsListBox* listbox = (cglsListBox*)GetChild(1);

	if (listbox)
	{
		return listbox->GetIndex(id);
	}

	return -1;
}

// ------------------------------------
Bool cglsListMenu::AddItem(int id, const wchar_t* text, cGlsAssistant* assistant)
{
	return AddItem(id, 0, text, assistant);
}

// ------------------------------------
Bool cglsListMenu::AddItem(int id, Bitmap* icon, const wchar_t* text, cGlsAssistant* assistant)
{
	sGlsListMenuItem item ={id, yistrheapW(text), icon, assistant};
	m_TextList.addref(yitoheap(&item, sizeof(item)));

	if (GetChild(1)!=0)
	{
		cglsListBox* listbox = (cglsListBox*)GetChild(1);
		listbox->InsertItem(item.id, 0, icon, item.pszText);
	}

	int height;
	int width=cglsListBox::MeasureItemWidth((wchar_t*)text, GetFont(), True, &height);

	if (icon!=0)
		width+=icon->GetWidth()+GetFontHeight(True);

	item.icon=icon;

	m_nTotalHeight+=height;
	m_nTotalWidth=m_nTotalWidth<width?width:m_nTotalWidth;

	if (GetChild(1)!=0)
	{
		cglsListBox* listbox=(cglsListBox*)GetChild(1);
		
		if (m_bIsAutoScroll==False)
		{
			listbox->ReSizeByItemz();
			ReSize(listbox->m_w, listbox->m_h);
		}
		else
		{
			listbox->ReSize(width, m_nTotalHeight);
			listbox->Update(eGUM_FORCED);
			ReSize(width, m_nTotalHeight);
		}
	}

	if (assistant)
	{
		if (m_listAssistant==0)
			m_listAssistant=new cyiList();
		m_listAssistant->addref(assistant);
	}
	return True;
}

// ---------------------------------------------------
LRESULT cglsListMenu::OnCreate(WPARAM wParam, LPARAM lParam)
{

	cglsListBox* listbox = new cglsListBox();
	sGlsListBoxDetail detail;
	cglsListBox::xDefaultDetail(&detail);

	detail.bIsMsgLikeMenu=True;
	detail.bIsMouseEnterNotify=m_bIsMoveNotify;
	detail.bIsNoClicked=m_bIsNoMouseDown;
	detail.stack.gls.font=GetFont();
	//detail.colorBg2=__COLORp__(255, 255, 245, 245);
	detail.colorBg2=__COLORp__(255, 243, 243, 253);

	detail.bIsSelectionCheck=m_bIsSelectionCheck;
	detail.bIsNoClicked=m_bIsSelectionCheck==False?True:False;

	int w=m_nTotalWidth+2+GetFontHeight(True);
	int h=m_nTotalHeight+2;
	int hMaxScreen = GetSystemMetrics(SM_CYSCREEN); 
	POINT pt={0, 0};
	ClientToScreen(*this, &pt);

	if (m_nMaxWidth)
	{
		w=m_nMaxWidth>w?m_nMaxWidth:w;
	}

	if (m_nMaxHeight)
	{
		h=m_nMaxHeight<h?m_nMaxHeight:h;
	}

	if ((pt.y+h)>hMaxScreen)
	{
		h=hMaxScreen-pt.y-GetFontHeight(True);

	}

	detail.stack.bIsShowScrollBar=m_bIsAutoScroll;

	if (m_pListBoxDetail==0)
		m_pListBoxDetail=&detail;

	listbox->Create((HWND)this, 1, 0, 0, w, h, m_pListBoxDetail);
	//listbox->SetNotifyWindow(m_hParentWnd);

	m_hNotifyWnd=m_hParentWnd;

	for (cyiNode* node=m_TextList.GetNodeTopFrom(); node; node=node->np)
	{
		sGlsListMenuItem* item=(sGlsListMenuItem*)node->ref;
		listbox->AddItem(item->id, item->icon, item->pszText);
	}
	listbox->Show();
	listbox->Update(eGUM_FORCED);

	if (m_bIsSelectionCheck==True)
	{
		listbox->SelectItem(m_nInitSelectIndex, m_nInitSelect, True);
	}
	//if (m_nInitSelect==0)
	//{
	//	if (m_nInitSelectIndex!=-1)

	//}
	//else
	//{
	//	listbox->SelectItem(listbox->GetIndex(m_nInitSelectIndex));
	//}

	//ReSize(w, h);
	listbox->ReSizeByItemz();
	ReSize(listbox->m_w, listbox->m_h);
	InvalidateRect(0);

	return 0;
}

// -----------------------------------------------------
Bool cglsListMenu::ShowAssistant(HWND hWnd, int x, int y)
{
	return TrackPopup(hWnd, x, y, 0, 0);
	//return True;
}
// -----------------------------------------------------
Bool cglsListMenu::GetResult(wchar_t* buffer)
{
	buffer[0]=0;
	for (cyiNode* node = m_TextList.GetNodeTopFrom(); node; node=node->np)
	{
		sGlsListMenuItem* item = (sGlsListMenuItem*)node->ref;

		if (item->id==this->m_nResultCode)
		{
			wcscpy(buffer, item->pszText);
			break;
		}
	}

	return True;
}
// -----------------------------------------------------
Bool cglsListMenu::GetResult(unsigned int& result1, unsigned int& result2)
{
	result1=this->m_nResultCode;
	return True;
}
// -----------------------------------------------------
Bool cglsListMenu::GetDefaultValue(wchar_t* buffer)
{
	return GetResult(buffer);
}
// -----------------------------------------------------
Bool cglsListMenu::GetDefaultValue(unsigned int& result1, unsigned int& result2)
{
	return True;
}
// -----------------------------------------------------
Bool cglsListMenu::SetDefaultValue(wchar_t* str)
{
	return True;
}

// -----------------------------------------------------
LRESULT cglsListMenu::OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	bIsHandled=True;
	m_bIsExitLoop=True;
	m_bIsExitCode=1;
	PostMessage(m_hNotifyWnd, WM_COMMAND, wParam, lParam);
	return 0;
}
// ---------------------------------------
LRESULT cglsListMenu::OnActivate(WPARAM wParam, LPARAM lParam, Bool&bIshandled)
{
	//bIshandled=True;

	//if (lParam)
	//	::SetActiveWindow((HWND)lParam);
	return 0;
}

// ---------------------------------------
LRESULT cglsListMenu::OnFocus(WPARAM wParam, LPARAM lParam, Bool&bIshandled)
{
	//if (wParam)
	//	::SetFocus((HWND)wParam);
	//bIshandled=True;
	return 0;
}