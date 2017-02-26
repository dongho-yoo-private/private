#include "glsPlayerPannel.h"
#include "glsGobanCommon.h"
cglsPlayerPannel::cglsPlayerPannel(void)
:m_nDeadStone(0)
,m_nSelectedTimerId(0)
,m_listTimer(0)
{
	__GLASSINTERFACE_NAME__;
}

cglsPlayerPannel::~cglsPlayerPannel(void)
{
}

void cglsPlayerPannel::xDefaultDetail(sGlsPlayerPannelDetail* detail)
{
	detail->font1=0;
	detail->font2=0;
	detail->style=eGPPS_EDIT;
}


#include "yctrlGobanResource.h"
// --------------------------------------------
Bool cglsPlayerPannel::Create(HWND hWnd, int id, int x, int y, int w, int h, Bool bIsBlack, sGoHeader* header, eGlsPlayerPannelStyle style, sGlsPlayerPannelDetail* detail)
{
	sGlsDialogDetail xdetail;
	//int nMaxLabelWidth = CreateLabels();

	//yiToUnicode(m_szPlayerName, pszPlayerName, 0);
	////wcscpy(m_szPlayerName, pszPlayerName);
	//cyctrlGobanResource::GetInstance().GetLevelString(nLevel, m_szLevel);
	m_header=header;
	m_eStyle=style;

	xdetail.font=detail->font1;
	m_font2=(Font*)cyiShape::Clone(detail->font2);//->Clone();

	m_eStyle=style;

	cglsDialog::xDefaultDetail(&xdetail);

	if (bIsBlack==True)
	{
		xdetail.gauss.colorTop=__COLORp__(127, 0, 0, 0);
		xdetail.gauss.colorBottom=__COLORp__(200, 0, 0, 0);
	}
	m_bIsBlack=bIsBlack;
	xdetail.font=detail->font1;
	xdetail.gauss.fBlureRad=5.0;
	xdetail.gauss.fShadowWidth=5.0;
	
	m_font=detail->font1;

	//m_eStyle=detail->style;
	return cglsDialog::Create(hWnd, id , 0, x, y, w, h, &xdetail);
}

// ------------------------------------
void cglsPlayerPannel::DrawStaticLabel()
{

}
#include "glsNameLevel.h"
#include "yiGoCommandz.h"

// --------------------------------------------
void cglsPlayerPannel::SetPlayerInfo(const char* pszPlayerName, int nlevel, const char* pszLevel)
{
	wchar_t szPlayer[256];
	cglsEditBox* edit = (cglsEditBox* )GetChild(1);

	cglsNameLevel* level=(cglsNameLevel*)edit->GetAssistant();

	level->SetNameLevel(pszPlayerName, nlevel, pszLevel);
	level->GetResult(szPlayer);

	edit->SetText(szPlayer);

	InvalidateRect(0);

}

// ------------------------------------
int cglsPlayerPannel::GetSecondReadCount(int id)
{
	for (cyiNode* node=this->m_listTimer->GetNodeTopFrom(); node; node=node->np)
	{
		sPlayTimerInfo* info = (sPlayTimerInfo*)node->ref;

		if (info->id==id)
		{
			return info->m_nReadSecondCount;
		}
	}

	return -1;
}

// ------------------------------------
int cglsPlayerPannel::xGetCurrentTime(int id, int* cnt)
{
	sPlayTimerInfo* info = this->GetTimer(id);

	if (info==0)
	{
		if (cnt)
			*cnt=0;
		return -1;
	}

	if (cnt)
		*cnt = info->m_nReadSecondCount;

	return info->m_spinValue;
}



// ------------------------------------
void cglsPlayerPannel::SetSecondReadCount(int id, int cnt)
{
	for (cyiNode* node=this->m_listTimer->GetNodeTopFrom(); node; node=node->np)
	{
		sPlayTimerInfo* info = (sPlayTimerInfo*)node->ref;

		if (info->id==id)
		{
			info->m_nReadSecondCount=cnt;
			break;
		}
	}
}

#include "glsNameLevel.h"
#include "glsSpinbox.h"

// ------------------------------------
sPlayTimerInfo* cglsPlayerPannel::GetTimer(goid_t id)
{
	if (m_listTimer==0)
		return 0;

	for (cyiNode* node=this->m_listTimer->GetNodeTopFrom(); node; node=node->np)
	{
		sPlayTimerInfo* info = (sPlayTimerInfo*)node->ref;

		if (info->id==id)
		{
			return info;
		}
	}

	return 0;
}

// --------------------------------------------
void cglsPlayerPannel::StartTimer(goid_t id, Bool bIsRemainValue)
{

	if (m_listTimer==0)
		return ;

	sPlayTimerInfo* info = GetTimer(id);

	if (info==0)
		return ;

	if (info->m_bIsTimerStopped==False)
		return ;

	info->m_nStartTime = cyiTime::CurrentmSec();
	//info->m_bIsTimerStopped=False;

	if (bIsRemainValue==False &&
		info->m_bIsSecondRead==True)
	{
		info->m_bIsSecondRead=False;
		cglsSpinBox* sin=(cglsSpinBox*)GetChild(4);
		info->m_spinValue=0;

		if (m_nSelectedTimerId==info->id)
		{
			sin->SetValue(0, eSBVF_ZERO, False);
		}
	}

	Bool bIsAlived=False;
	for (cyiNode* node=m_listTimer->GetNodeTopFrom(); node; node=node->np)
	{
		sPlayTimerInfo* info = (sPlayTimerInfo*)node->ref;
		if (info->m_bIsTimerStopped==False)
		{
			bIsAlived=True;
			break;
		}
	}

	if (bIsAlived==False)
		SetTimer(*this, 3, 100, NULL);
	info->m_bIsTimerStopped=False;

	//xTRACE("cglsPlayerPannel::StartTimer! %s, start:%d", m_bIsBlack?"black":"white", info->m_nStartTime);
}

// --------------------------------------------
void cglsPlayerPannel::StopTimer(goid_t id, unsigned int usedTime)
{
	if (m_listTimer==0)
		return ;

	xTRACE("cglsPlayerPannel::StopTimer (%x): %d", GetProcessId(GetCurrentProcess()), usedTime);
	sPlayTimerInfo* info = GetTimer(id);

	if (info==0)
		return ;

	//KillTimer(*this, 3);
	if (info->m_bIsTimerStopped==True)
		return ;

	info->m_bIsTimerStopped=True;

	if (info->m_bIsSecondRead==True)
	{
		info->m_nUsedTime=0;
	}
	else
	{
		info->m_nUsedTime=usedTime+info->m_minus*1000;
		xTRACE("cglsPlayerPannel::StopTimer! %s usedTime:%d minus:%d", m_bIsBlack?"black":"white", usedTime, info->m_minus);
	}
}

// --------------------------------------------
void cglsPlayerPannel::GobanChanged(goid_t id)//, unsigned int usedTime)
{
	sPlayTimerInfo* info = GetTimer(id);

	if (info==0)
		return ;

	if (m_nSelectedTimerId==id)
		return ;

	m_nSelectedTimerId=id;
	cglsSpinBox* spin =(cglsSpinBox*)GetChild(4);

	if (spin)
	{
		spin->SetValue(info->m_spinValue, eSBVF_ZERO, False);

	}

	cglsSpinBox* spin2 = (cglsSpinBox*)GetChild(10);

	if (spin2)
		spin2->SetValue(info->m_nReadSecondCount, eSBVF_ZERO, False);
}

// --------------------------------------------
void cglsPlayerPannel::SetUsedTime(goid_t id, unsigned int usedTime)
{
	sPlayTimerInfo* info = GetTimer(id);

	if (info==0)
		return ;

	info->m_nUsedTime=usedTime;
	cglsSpinBox* spin = (cglsSpinBox*)GetChild(4);

	if (spin==0)
		return ;

	spin->InvalidateRect(0);

	int n =info->m_nTotalTime-(info->m_nUsedTime/1000);

	n=n<0?0:n;
	spin->SetValue(n, eSBVF_ZERO, False);
	
	spin=(cglsSpinBox*)GetChild(10);
	spin->SetValue(info->m_nReadSecondCount, eSBVF_ZERO, False);
	spin->InvalidateRect(0);
}


// --------------------------------------------
void cglsPlayerPannel::SetDeadStone(int n, Bool bAnimation)
{
	cglsSpinBox* spin = (cglsSpinBox*)GetChild(3);
	int nDelay = abs(m_nDeadStone-n)*100;
	spin->SetValue(n, eSBVF_ZERO, bAnimation, nDelay>1000?1000:nDelay);
	m_nDeadStone=n;
}

// ------------------------------------
void cglsPlayerPannel::SetPannelStyle(eGlsPlayerPannelStyle style)
{
	if (m_eStyle==style)
		return ;
	m_eStyle=style;
	ReArrangeItemz();
	
}

// ------------------------------------
void cglsPlayerPannel::SetTotalRemainTime(int id, int time)
{
	sPlayTimerInfo* info = this->GetTimer(id);
	if (info==0)
		return ;

	info->m_nUsedTime=time;
}


// ------------------------------------
int cglsPlayerPannel::GetTotalUsedTime(int id)
{
	sPlayTimerInfo* info = this->GetTimer(id);
	if (info==0)
		return 0;

	return info->m_nUsedTime/1000+info->m_minus*1000;
}


// ------------------------------------
unsigned int cglsPlayerPannel::GetRemainTime(int id)
{
	id = id==0?m_nSelectedTimerId:id;
	sPlayTimerInfo* info =0;
	for (cyiNode* node=m_listTimer->GetNodeTopFrom(); node; node=node->np)
	{
		info = (sPlayTimerInfo*)node->ref;
		if (info->id==id)
			break;
	}
	if (info==0)
		return 0;

	//if (m_nSelectedTimerId==id)
	//{
	//	cglsSpinBox* spin =(cglsSpinBox*)GetChild(4);
	//	return spin->GetValue();
	//}

	//int n= info->m_nTotalTime-(info->m_nUsedTime/1000);

	//if (n<0)
	//	n=0;

	return info->m_spinValue;
}

// ------------------------------------
void cglsPlayerPannel::SetRemainTime(int id, unsigned int time, Bool bIsMinus)
{

	id = id==0?m_nSelectedTimerId:id;
	sPlayTimerInfo* info =0;
	for (cyiNode* node=m_listTimer->GetNodeTopFrom(); node; node=node->np)
	{
		info = (sPlayTimerInfo*)node->ref;
		if (info->id==id)
			break;
	}
	if (info==0)
		return ;

	//info->m_minus+= time;

	if (bIsMinus==True)
	{
		info->m_nTotalTime-=time;
	}
	else
	{
		info->m_nTotalTime=time;
	}

	int n = info->m_nTotalTime-(info->m_nUsedTime/1000)-info->m_minus;
	info->m_spinValue=n;

	xTRACE("cglsPlayerPannel::SetRemainTime() %d, minus:%d, remain:%d", time, info->m_minus, n);
	
	if (n<0)
	{
		info->m_nUsedTime=(info->m_nTotalTime)*1000;//info->m_minus;//(info->m_minus-info->m_nTotalTime)*1000;
		n=0;
		info->m_spinValue=0;
	}
	else
	{
		info->m_nUsedTime+=(info->m_minus)*1000;//n+(info->m_minus-info->m_nTotalTime)*1000;
	}

	if (m_nSelectedTimerId==id)
	{
		cglsSpinBox* spin =(cglsSpinBox*)GetChild(4);
		spin->SetValue(n, eSBVF_ZERO, False);
		return ;
	}
}


// ------------------------------------
LRESULT cglsPlayerPannel::OnTimer(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	if (m_listTimer==0 || m_listTimer->count()==0)
	{
		KillTimer(*this, wParam);
		return 0;
	}

	cglsSpinBox* spin = (cglsSpinBox*)GetChild(4);

	if (spin==0)
	{
		return 0;
	}

	//if (IsWindowVisible(*spin)==FALSE)
	//	return 0;

	Bool bIsAllStop=True;
	for (cyiNode* node=m_listTimer->GetNodeTopFrom(); node; node=node->np)
	{
		sPlayTimerInfo* info = (sPlayTimerInfo*)node->ref;

		if (info->m_bIsTimerStopped==True)
			continue;

		bIsAllStop=False;

		unsigned int curr=0;
		unsigned int delta=0;
		unsigned int used=0;
		
		//if (info->id!=m_nSelectedTimerId)
		//{
		//	delta = cyiTime::CurrentmSec(info->m_nStartTime);
		//	//used  = info->m_nTotalTime-(info->m_nUsedTime+delta)/1000;
		//	curr   = (info->m_nTotalTime-((info->m_nUsedTime+delta)/1000));
		//}
		//else
		//{
		curr  = info->m_spinValue;//spin->GetValue();
		delta = cyiTime::CurrentmSec(info->m_nStartTime);
		used  = info->m_nTotalTime-(info->m_nUsedTime+delta)/1000;
		
			//xTRACE("cglsPlayerPannel::OnTimer (%x): %d, %d, %d", GetProcessId(GetCurrentProcess()), curr, delta, used);

		//}

		//xTRACE("cglsPlayerPannel::OnTimer() %s total:%d, delta:%d, cur:%d, used:%d", m_bIsBlack?"black":"white", m_nTotalTime, delta, curr, used);

		// 既にゼロの場合、まだ秒読みに入ってない場合に、
		if (curr==0)
		{
			// 打ち切り
			if (info->m_nNormalReadSecond==0)
			{
				GetSystem()->GetCommandCenter()->PostCommand(eGCID_TIME_LOSE, this, m_bIsBlack, info->id);
				continue;
			}

			if (info->m_bIsSecondRead==False)
			{
				info->m_bIsSecondRead=True;
				info->m_nStartTime=cyiTime::CurrentmSec();
				info->m_nTotalTime=info->m_nNormalReadSecond;
				info->m_nUsedTime=0;
				//xTRACE("Start ReadSecondCount:+%d", info->m_nNormalReadSecond);
				info->m_spinValue=info->m_nNormalReadSecond;

				if (info->id==m_nSelectedTimerId)
				{
					spin->SetValue(info->m_nNormalReadSecond, eSBVF_ZERO, False);
				}
				else
				{
					GetSystem()->GetCommandCenter()->PostCommand(eGCID_SECOND_READ, this, info->id, 0);
				}
				continue;
			}
			else
			{
				if (info->m_nReadSecondCount==0)
				{
					GetSystem()->GetCommandCenter()->PostCommand(eGCID_TIME_LOSE, this, m_bIsBlack, info->id);
					continue;
				}
				else
				{
					xTRACE("Start ReadSecond:+%d", info->m_nReadSecond);
					info->m_nReadSecondCount--;
					info->m_nTotalTime=info->m_nReadSecond;
					info->m_nUsedTime=0;
					info->m_spinValue=info->m_nReadSecond;

					if (info->id==m_nSelectedTimerId)
					{
						spin->SetValue(info->m_nReadSecond, eSBVF_ZERO, False);
						cglsSpinBox* spin2 = (cglsSpinBox*)GetChild(10);
						spin2->SetValue(-1);
					}
					info->m_nStartTime=cyiTime::CurrentmSec();
					continue;
				}
			}
		}

		//if (info->id!=m_nSelectedTimerId)
		//{
		//	continue;
		//}

		if (curr>used)
		{
			xTRACE("cglsPlayerPannel::OnTimer() SetValue!", curr, used);

			if (info->m_bIsSecondRead==True &&
					curr<=10)
			{
				if (curr==10 || curr<6)
					yiBeep(MB_OK);//0x07FF, 20);
				GetSystem()->GetCommandCenter()->PostCommand(eGCID_SECOND_READ ,this, info->id, curr-1);
			}

			if (spin->IsNowAnimation()==False)
			{
				if (curr-used>2)
				{
					info->m_spinValue=used;

					if (this->m_nSelectedTimerId==info->id)
					{
						spin->SetValue(used, eSBVF_ZERO, False);
					}
				}
				else
				{
					info->m_spinValue--;
					if (info->m_spinValue<0)
					{
						info->m_spinValue=0;
					}
					else
					{
						if (m_nSelectedTimerId==info->id)
						{
							spin->SetValue(-1);
						}
					}
				}
			}
		}

	}

	if (bIsAllStop==True)
	{
		KillTimer(*this, wParam);
		return 0;
	}

	//if (m_bIsTimerStopped==True)
	//{
	//	return 0;
	//}

	//cglsSpinBox* spin = (cglsSpinBox*)GetChild(4);
	//if (spin==0)
	//{
	//	KillTimer(*this, wParam);
	//	return 0;
	//}



	return 0;

}

// ----------------------------------------------
void cglsPlayerPannel::RemoveTimer(goid_t id)
{
	xTRACE("cglsPlayerPannel::RemoveTimer(%d)", id);
	if (m_listTimer==0)
		return ;
	for (cyiNode* node=this->m_listTimer->GetNodeTopFrom(); node; node=node->np)
	{
		sPlayTimerInfo* info = (sPlayTimerInfo*)node->ref;

		if (info->id==id)
		{
			m_listTimer->remove(node);
			yifree(info);
			xTRACE("cglsPlayerPannel::RemoveTimer(%d) remove success", id);
			break;
		}
	}

}

//// --------------------------------------------
Bool cglsPlayerPannel::AddTimer(goid_t id, int nTotalTime, int normalReadSec, int readSec, int readSecCount)
{
	xTRACE("cglsPlayerPannel::AddTimer(%d) ", id);
	
	if (m_listTimer==0)
		m_listTimer = new cyiList();

	sPlayTimerInfo* info = GetTimer(id);

	if (info)
	{
		if (nTotalTime==0xFFFF)
		{
			return False;
		}

		info->m_spinValue=nTotalTime;
		info->m_nNormalReadSecond=normalReadSec;
		info->m_nReadSecond=readSec;
		info->m_nReadSecondCount=readSecCount;
		info->m_nTotalTime=nTotalTime;
		info->m_nUsedTime=0;

		cglsSpinBox* spin =(cglsSpinBox*)GetChild(4);
		spin->SetValue(info->m_spinValue/*-info->m_minus*/, eSBVF_ZERO, False);
		spin->InvalidateRect(0);
		
		cglsSpinBox* spinRemainTime2 = (cglsSpinBox*)GetChild(10);
		spinRemainTime2->InvalidateRect(0);
		return False;
	}

	if (info==0)
	{
		info = (sPlayTimerInfo*)yialloci(sizeof(sPlayTimerInfo));
		info->m_nTotalTime=nTotalTime;
		info->m_nReadSecond=readSec;
		info->m_nNormalReadSecond=normalReadSec;
		info->m_nReadSecondCount=readSecCount;
		info->id=id;
		info->m_bIsTimerStopped=True;
		m_listTimer->addref(info);
	}


	m_nSelectedTimerId=id;
	cglsSpinBox* spinRemainTime2 = (cglsSpinBox*)GetChild(10);
	spinRemainTime2->SetValue(info->m_nReadSecondCount, eSBVF_ZERO, False);
	spinRemainTime2->InvalidateRect(0);

	cglsSpinBox* spin =(cglsSpinBox*)GetChild(4);

	unsigned int delta = 0;
	
	if (info->m_bIsTimerStopped==False)
	{
		delta=cyiTime::CurrentmSec(info->m_nStartTime);
	}

	info->m_spinValue=info->m_nTotalTime-((info->m_nUsedTime+delta)/1000);
	spin->SetValue(info->m_spinValue/*-info->m_minus*/, eSBVF_ZERO, False);
	spin->InvalidateRect(0);

	return True;
}

// ------------------------------------
int cglsPlayerPannel::SetFont(Font* font1, Font* font2)
{
	
	if (m_font2)
		delete m_font2;
	m_font2=(Font*)cyiShape::Clone(font2);

	__super::SetFont(font1);

	//cglsEditBox* edit = (cglsEditBox*)GetChild(1);
	//edit->SetFont(m_font2, True);
	//edit->Update(

	return 0;
}

// ------------------------------------
LRESULT cglsPlayerPannel::OnCommand(WPARAM wParam, LPARAM lParam, Bool& handled)
{
	int id = wParam&0xFFFF;

	if (id==1)
	{
		cglsEditBox* edit = (cglsEditBox*)GetChild(1);
		cglsNameLevel* name = (cglsNameLevel*)edit->GetAssistant();

		if (m_bIsBlack)
		{
			if (m_header->pszBlackName)
				yifree(m_header->pszBlackName);

			m_header->pszBlackName = yistrheap(name->GetName());
			
			unsigned int ret;
			char* pszLevel = name->GetLevel(ret);

			if (pszLevel)
			{
				if (m_header->pszBlackLevel)
					yifree(m_header->pszBlackLevel);
				m_header->pszBlackLevel=pszLevel;
			}
			else
			{
				if (m_header->pszBlackLevel)
					yifree(m_header->pszBlackLevel);
				m_header->pszBlackLevel=0;

				Bool bIsAmature = IS_AMATURE(ret);
				int  level		= GET_LEVEL_ONLY(ret);

				//if (bIsAmature)
				//	SET_BLACK_AMA(m_header->flag32);
				//else
				//	SET_BLACK_PRO(m_header->flag32);
				SET_BLACK_LEVEL_OFF(m_header->flag32);
				SET_BLACK_LEVEL(m_header->flag32, level);
			}

		}
		else
		{
			if (m_header->pszWhiteName)
				yifree(m_header->pszWhiteName);

			m_header->pszWhiteName = yistrheap(name->GetName());
			
			unsigned int ret;
			char* pszLevel = name->GetLevel(ret);

			if (pszLevel)
			{
				if (m_header->pszWhiteLevel)
					yifree(m_header->pszWhiteLevel);
				m_header->pszWhiteLevel=pszLevel;
			}
			else
			{
				if (m_header->pszWhiteLevel)
					yifree(m_header->pszWhiteLevel);
				m_header->pszWhiteLevel=0;

				Bool bIsAmature = IS_AMATURE(ret);
				int  level		= GET_LEVEL_ONLY(ret);

				if (bIsAmature)
					SET_WHITE_AMA(m_header->flag32);
				else
					SET_WHITE_PRO(m_header->flag32);

				SET_WHITE_LEVEL_OFF(m_header->flag32);
				SET_WHITE_LEVEL(m_header->flag32, level);
			}

		}
		
		PostMessage(m_hNotifyWnd, WM_GO_HEADER_CHANGED, 0, 0);
		
	}
	return 0;
}


// ------------------------------------
void cglsPlayerPannel::OnItemReArranged()
{
	RECT rect;
	int y=0;
	int x;// = 
	int x2;
	cGlassInterface* child;

	GetClientRect(&rect);
	x=rect.left+m_fEdge+2;

	cglsEditBox* edit = (cglsEditBox*)GetChild(1);

	edit->Move(x, rect.top+1);//, rect.right-2);
	edit->ReSize(rect.right-2, edit->m_h);

	y=edit->m_y+edit->m_h;



	// あげはま
	cglsStatic* label = (cglsStatic*)GetChild(7);
	label->Move(x, y);
	
	cglsSpinBox* spin = (cglsSpinBox*)GetChild(3);
	spin->Move(label->m_x+label->m_w+3, label->m_y);

	y+=label->m_h;
	//spin->Show();

	{
		child=GetChild(8);
		child->Move(x, y);
		x2=child->m_x+child->m_w+3;
	}
	//x2=spin->m_x;

	y=label->m_y+label->m_h+1;

	// 残り時間
	if (m_eStyle & eGPPS_TIME)
	{
		int delta;
		child=GetChild(8);
		child->Move(x, y);
		child->Show();
		
		delta=child->m_w;
		child=GetChild(4);

		//if (m_eStyle==eGPPS_PLAY3)
		//{
		//	child->Move(x, y);//x+delta+3, y);
		//	GetChild(8)->Hide();
		//}
		//else
		{
			child->Move(x2, y);//x+delta+3, y);
		}
		child->Show();

		if ((m_eStyle & eGPPS_TIME2)==0)
			y+=child->m_h;
		//y+=child->m_h;
		//y+=child->m_h;
	}
	else
	{
		GetChild(8)->Hide();
		GetChild(4)->Hide();
	}



	// 残り秒読み
	if (m_eStyle & eGPPS_TIME2)
	{
		int w;
		int x=GetChild(4)->m_x+GetChild(4)->m_w+GetFontHeight();
		child=GetChild(9);
		child->Move(x, y);
		child->Show();
		w=child->m_w;

		child=GetChild(10);

		//if (m_eStyle==eGPPS_PLAY3)
		//{
		//	child->Move(x, y);//+w+3, y);
		//}
		//else
			child->Move(x+w+3, y);//+w+3, y);
		child->Show();

		y+=child->m_h;

	}
	else
	{
		GetChild(9)->Hide();
		GetChild(10)->Hide();
	}

	//if (m_eStyle==eGPPS_PLAY3)
	//{
	//	GetChild(9)->Hide();
	//}

	// 戦績
	if (m_eStyle & eGPPS_SEISEKI)
	{
		child=GetChild(5);
		child->Move(x, y);
		child->Show();

		//if (m_eStyle&eGPPS_SENSEKI2==0)
		//{
			y+=child->m_h;
		//}
	}
	else
	{
		GetChild(5)->Hide();
	}

	// 対戦成績
	if (m_eStyle & eGPPS_SENSEKI2)
	{
		child=GetChild(6);
		//if (m_eStyle&eGPPS_SEISEKI)
		//{

		//	child->Move(GetChild(5)->m_w+x+GetFontHeight(), y);
		//}
		//else
		//{
			child->Move(x, y);
		//}
		child->Show();
		y+=child->m_h;
	}
	else
	{
		GetChild(6)->Hide();
	}

	cGlassInterface::Update(eGUM_FORCED);
	ReSize(m_w, y+(rect.top<<1));
	InvalidateRect(0);

}

// ------------------------------------
Bool cglsPlayerPannel::xCreateChild()
{
	RECT rect;
	sGlsEditBoxDetail detail;
	const char* pszBlack="黒";
	const char* pszWhite="白";

	int h=0;

	// プレイヤー名
	cglsEditBox::xDefaultDetail(&detail);

	detail.bIsFontSizeVariable=False;
	detail.bIsStaticEditorMode=True;
	detail.colorStaicModeFont=m_bIsBlack?Color::White:Color::Black;
	detail.eAlign = eGIA_CENTER;
	detail.assistant= new cglsNameLevel(m_bIsBlack?((m_header->pszBlackName&&*m_header->pszBlackName)?m_header->pszBlackName:pszBlack):((m_header->pszWhiteName&&*m_header->pszWhiteName)?m_header->pszWhiteName:pszWhite), 
		m_bIsBlack?GET_BLACK_LEVEL(m_header->flag32):GET_WHITE_LEVEL(m_header->flag32), m_bIsBlack?m_header->pszBlackLevel:m_header->pszWhiteLevel);
	detail.bIsMouseDownShow=True;
	//detail.gls.font=GetFont();


	GetClientRect(&rect);

	detail.gls.font=m_font2;//->Clone();
	cglsEditBox* edit = new cglsEditBox();
	edit->Create((HWND)this, 1, rect.left+1, rect.top+1, rect.right-2, 0, &detail);

	// あげはま
	wchar_t buffer[256];
	detail.assistant->GetDefaultValue(buffer);
	edit->SetText(buffer);
	edit->Show();
	h+=edit->m_h;

	sGlsStaticDetail staticDetail;
	cglsStatic::xDefaultDetail(&staticDetail);
	staticDetail.id=7;

	staticDetail.colorFont=m_bIsBlack?Color::White:Color::Black;
	cglsStatic* staticAgeStone = CreateLabel(L"アゲハマ", &staticDetail);

	staticAgeStone->Move(rect.left+1, edit->m_h+rect.top+1);
	h+=staticAgeStone->m_h;
	staticAgeStone->Show();

	sGlsSpinBoxDetail spinDetail;

	cglsSpinBox::xDefaultDetail(&spinDetail);
	spinDetail.gls.bIsNoInput=True;
	spinDetail.bIsShowButton=False;
	cglsSpinBox* spin = new cglsSpinBox();
	spin->Create(this, 3, staticAgeStone->m_x+staticAgeStone->m_w+3, staticAgeStone->m_y, 3, &spinDetail);
	spin->SetNumer(0, 0, 9, 0);
	spin->SetNumer(1, 0, 9, 0);
	spin->SetNumer(2, 0, 9, 0);


	// 残り時間
	cglsSpinBox* spinTime = new cglsSpinBox();
	spinTime->Create(this, 4, 0, 0, 6, &spinDetail);
	spinTime->SetNumer(0, 0, 9, 0);
	spinTime->SetNumer(1, 0, 5, 0);
	spinTime->SetNumer(2, 0, 9, 0);
	spinTime->SetNumer(3, 0, 5, 0);
	spinTime->SetNumer(4, 0, 9, 1);
	spinTime->SetNumer(5, 0, 1, 0);
	spinTime->SetLimited(4, 16);

	spinTime->InsertLabel(L":", 2, staticDetail.colorFont);
	spinTime->InsertLabel(L":", 4, staticDetail.colorFont);

	spinTime->Hide();

	// 戦績（トータル）
	//sGlsStaticDetail staticDetail;
	cglsStatic::xDefaultDetail(&staticDetail);
	staticDetail.id=5;

	staticDetail.colorFont=m_bIsBlack?Color::White:Color::Black;
	cglsStatic* staticScore =  CreateLabel(L"戦績：1000勝280敗12分け", &staticDetail);
	staticScore->Hide();



	// 対戦戦績
	//sGlsStaticDetail staticDetail;
	cglsStatic::xDefaultDetail(&staticDetail);
	staticDetail.id=6;

	staticDetail.colorFont=m_bIsBlack?Color::White:Color::Black;
	staticScore =  CreateLabel(L"対戦戦績：1000勝280敗12分け", &staticDetail);
	staticScore->Hide();

	staticDetail.id=8;
	cglsStatic* remainTime= CreateLabel(L"持ち時間", &staticDetail);
	remainTime->Hide();

	staticDetail.id=9;
	cglsStatic* remainTime2= CreateLabel(L"秒読み", &staticDetail);
	remainTime2->Hide();

	cglsSpinBox* spinRemainTime2 = new cglsSpinBox();
	spinRemainTime2->Create(this, 10, 0, 0, 2, &spinDetail);
	spinRemainTime2->SetNumer(0, 0, 9, 0);
	spinRemainTime2->SetNumer(1, 0, 9, 0);
	spinRemainTime2->InsertLabel(L"回", 0, staticDetail.colorFont);
	spinRemainTime2->Hide();

	//InvalidateRect(0);

	return True;
}

// ----------------------------------------
LRESULT cglsPlayerPannel::OnCreate(WPARAM wParam, LPARAM lParam)
{
	if (xCreateChild()==True)
	{
		//return 0;
		return ReArrangeItemz();
	}

	return 0;
}

