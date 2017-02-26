#include "yctrlTab.h"


cyctrlTab::cyctrlTab(void)
: cyctrlbase()

{
	m_hNotifyWnd=0;
	m_prevselectedIndex=-1;
	m_btnId=0;
}

cyctrlTab::~cyctrlTab(void)
{
}

// ---------------------------------------
UINT cyctrlTab::Add(cyctrlbase* ctrl, const char* pszTitle, UINT bgColor)
{
	//::ShowWindow(*ctrl, SW_HIDE);
	int n = ctrllist.addref(new cyTabData(ctrl, pszTitle, bgColor));

	m_currentIndex=n-1;
	Update();


	return 0;
}

// ---------------------------------------
void cyctrlTab::Remove(UINT index)
{

	cyiNode* p=ctrllist.GetNodeTopFrom(index); 

	if (p!=0)
	{
		cyTabData* data = (cyTabData*)p->ref;

		if (data->btn!=0)
		{
			data->btn->Destroy();
			data->btn=0;
			//delete data->btn;
		}

		delete data;
	}

	ctrllist.remove(index);
	Update();
}

// ---------------------------------------
BOOL cyctrlTab::Create(HWND hWndParent, UINT id, int x, int y, int w, int h, int maxlen, int minlen, eTabDirection dir, BOOL bHasCloseButton)
{
	m_maxlen=maxlen;
	m_minlen=minlen;
	m_dir=dir;
	m_currentIndex=0;

	m_bHasCloseButton = bHasCloseButton;

	if (dir==eTabDirection::eTD_HORIZENTAL)
	{
		m_bufferBitmap = new Bitmap(w, h);
	}
	else
	{
		m_bufferBitmap = new Bitmap(h, w);
	}
	Graphics g(m_bufferBitmap);
	g.Clear(Color(0,0,0,0));

	m_bCreated=TRUE;
	if (cyctrlbase::Create(hWndParent, NULL, id, x, y, w, h, FALSE)==TRUE)
	{
		m_ToolTip.Create(*this);
		m_currentIndex=0;
		if (ctrllist.count()!=0)
		{
			m_current = ((cyTabData*)ctrllist.GetNodeTopFrom(0)->ref)->ctrl;
			flip(m_currentIndex);
		}
		return TRUE;
	}

	return FALSE;
}



// ---------------------------------------
BOOL cyctrlTab::flip(int index)
{
	if (index>=ctrllist.count())
	{
		return FALSE;
	}

	if (m_current!=0)
	{
		::ShowWindow(*m_current, SW_HIDE);
	}

	cyTabData* data = (cyTabData*)(ctrllist.GetNodeTopFrom(index)->ref);
	

	if (m_hNotifyWnd==0)
	{
		if ((data->ctrl)!=0)
		{
			::ShowWindow(*(data->ctrl), SW_SHOW);
		}
	}
	else
	{
		::SendMessage(m_hNotifyWnd, m_message, m_currentIndex, index);
	}

	m_current=data->ctrl;
	m_currentIndex=index;
	Update();

	return TRUE;
}

#define DEFAULT_BTN_SIZE 16

// ---------------------------------------
void cyctrlTab::Update()
{
	if (m_bCreated==FALSE)
	{
		return ;
	}
	int n = ctrllist.count();
	float length = (float)this->m_maxlen;
	float length2 = (float)m_w/(float)n;
	length = (length > length2) ? length2:length;

	if (this->m_dir==eTabDirection::eTD_VERTICAL)
	{
		length = (float)this->m_maxlen;
		length2 = (float)m_h/(float)n;
		length = (length > length2) ? length2:length;
	}

	Graphics g(m_bufferBitmap);

	g.Clear(Color::White);// Color(0,0,0,0));

	int i=0;
	float length_sum=0.0;
	for (cyiNode* p=ctrllist.GetNodeTopFrom(0); p!=0; p=p->np)
	{
		cyTabData* data = (cyTabData*)p->ref;
		
		SolidBrush brush(Color(0,0,0));

		if (i==m_currentIndex)
		{
			brush.SetColor(Color((ARGB)data->bgColor));
		}
		else
		{
			brush.SetColor(Color(((ARGB)data->bgColor)&0x1FFFFFFF));
		}

		float x = length_sum;//length*i++;
		float y=0;
		float h = m_h;
		
		if (this->m_dir==eTabDirection::eTD_VERTICAL)
		{
			h = (float)m_w;
		}

		RectF bound;
		int len = wcslen(data->title);
		Status ret = g.MeasureString(data->title, len, &m_font, RectF(x, (float)y, (float)1024, (float)h), &bound);

		float adjust_length=length;
		if (this->m_bHasCloseButton==TRUE)
		{
			adjust_length= length-DEFAULT_BTN_SIZE;
		}

		float rlength=length;
		if (bound.Width < adjust_length)
		{
			rlength = bound.Width;

		}

		adjust_length = rlength;

		if (this->m_bHasCloseButton==TRUE)
		{
			rlength+=DEFAULT_BTN_SIZE;
			if (data->btn==NULL)
			{
				data->btn = new cyctrlButton();
				data->btn->Create(*this, ++m_btnId, 0, 0, DEFAULT_BTN_SIZE, DEFAULT_BTN_SIZE, eBS_CLOSE, data->bgColor|0xFF000000);
				data->btnId=m_btnId;
			}

			if (this->m_dir==eTabDirection::eTD_VERTICAL)
			{
				UINT bh = (((UINT)h - DEFAULT_BTN_SIZE)>>1);
				data->btn->Move(bh, (UINT)(x+rlength-DEFAULT_BTN_SIZE)-bh);
			}
			else
			{
				UINT bh = (((UINT)h - DEFAULT_BTN_SIZE)>>1);
				data->btn->Move((UINT)(x+rlength-DEFAULT_BTN_SIZE)-bh, (UINT)bh);
			}
		}

		length_sum+=rlength;

		g.FillRectangle(&brush, x, (float)y, rlength, (float)h);
		g.DrawRectangle(&Pen(Color::Black, 0.5), (float)x, (float)y, rlength, (float)h);

		if (bound.Width < adjust_length)
		{
			g.DrawString(data->title, len, &m_font, RectF(x, (float)y, (float)adjust_length, (float)h), NULL, &SolidBrush(Color(0,0,0)));
		}
		else
		{
			wchar_t buffer[256];
			float f = adjust_length/bound.Width;
			int adjustLen = (int)((float)len*f);

			if (adjustLen<3)
			{
				return ;
			}
			else
			{
				wcsncpy(buffer, data->title, adjustLen-3);
				wcsncpy(&buffer[adjustLen-3], L"...", 3);
				g.DrawString(buffer, adjustLen, &m_font, RectF(x, (float)y, (float)adjust_length, (float)h), NULL, &SolidBrush(Color(0,0,0)));
			}
		}

	}

	::InvalidateRect(m_hWnd, NULL, TRUE);
	
	return ;
}


// ---------------------------------------
BOOL cyctrlTab::OnPaint(HDC hdc)
{
	Graphics g(hdc);

	if (this->m_dir==eTabDirection::eTD_VERTICAL)
	{
		g.ResetTransform();
		g.TranslateTransform(m_w, 0.0);
		g.RotateTransform(90.0);
	}
	g.DrawImage(m_bufferBitmap, 0, 0);


	return TRUE;
}


// ---------------------------------------
LRESULT cyctrlTab::OnClicked(WPARAM wp, LPARAM lParam)
{
	int xPos   =  lParam & 0xFFFF;         // カーソルx座標
	int yPos   = (lParam >> 16) & 0xFFFF;  // カーソルy座標
	int n = ctrllist.count();

	if (this->m_dir==eTabDirection::eTD_HORIZENTAL)
	{
		float length = (float)this->m_maxlen;
		float length2 = (float)m_w/(float)n;
		
		length = (length > length2) ? length2:length;

		int selectIndex = (int)(xPos/length);

		flip(selectIndex);

	}
	else
	{
		float length = (float)this->m_maxlen;
		float length2 = (float)m_h/(float)n;
		
		length = (length > length2) ? length2:length;

		int selectIndex = (int)(yPos/length);

		flip(selectIndex);

	}

	m_ToolTip.Hide();
	return 0;
}

// ---------------------------------------
LRESULT cyctrlTab::OnMouseMove(WPARAM wp, LPARAM lp, BOOL& handled)
{
	int n= ctrllist.count();
	int selectedIndex=0;
	POINT pos;
	int xPos   =  lp & 0xFFFF;         // カーソルx座標
	int yPos   = (lp >> 16) & 0xFFFF;  // カーソルy座標
	
	if (this->m_dir==eTabDirection::eTD_HORIZENTAL)
	{
		float length = (float)this->m_maxlen;
		float length2 = (float)m_w/(float)n;
		
		length = (length > length2) ? length2:length;

		selectedIndex = (int)(xPos/length);
	}
	else
	{
		float length = (float)this->m_maxlen;
		float length2 = (float)m_h/(float)n;
		
		length = (length > length2) ? length2:length;

		selectedIndex = (int)(yPos/length);
	}

	if (m_prevselectedIndex==selectedIndex)
	{
		return 0;
	}

	cyiNode* p = ctrllist.GetNodeTopFrom(selectedIndex);

	if (p==0)
		return 0;

	cyTabData* data = (cyTabData*)p->ref;
	if(data!=NULL)
	{
		m_ToolTip.Show(data->title);
		m_prevselectedIndex=selectedIndex;
	}

	return 0;
}

// ---------------------------------------
LRESULT cyctrlTab::OnMouseLeave(WPARAM wp, LPARAM lp)
{
	m_prevselectedIndex=-1;
	m_ToolTip.Hide();
	cyctrlbase::OnMouseLeave(wp, lp);
	return 0;
}

// ---------------------------------------
LRESULT cyctrlTab::OnCommand(WPARAM wp, LPARAM lp)
{
	int cnt=0;
	for (cyiNode* p = ctrllist.GetNodeTopFrom(0); p!=0; p=p->np)
	{
		cyTabData* data = (cyTabData*)p->ref;

		if (data->btn!=0)
		{
			if (data->btnId==wp)
			{
				Remove(cnt);
				break;
			}
		}
		cnt++;
	}

	::InvalidateRect(*this, NULL, TRUE);

	return 0;
}


