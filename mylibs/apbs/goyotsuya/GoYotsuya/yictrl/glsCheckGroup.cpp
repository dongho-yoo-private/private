#include "glsCheckGroup.h"

cglsCheckGroup* xcglsCheckGroup()
{
	return new cglsCheckGroup();
}

cglsCheckGroup::cglsCheckGroup(void)
{
}

cglsCheckGroup::~cglsCheckGroup(void)
{
}

// -----------------------------
void cglsCheckGroup::xDefaultDetail(sCheckGroupDetail* detail)
{
	detail->bIsAutoArrange=True;
	detail->colorCheck=Color::LightBlue;
	detail->colorCheckBorder=Color::Black;
	detail->colorText=Color::Black;
	detail->nDistance=3;
}

// -----------------------------
Bool cglsCheckGroup::Create(cGlassInterface* parent, int id, int x, int y, sCheckGroupDetail* detail)
{
	m_bIsTypeCheck=True;
	return cglsRadioGroup::Create(parent, id, x, y, (sRadioGroupDetail*)detail);
}

// -----------------------------
void cglsCheckGroup::SetChecked(int index)
{
	int n=0;
	int id=0;
	for (cyiNode* node=m_listItem->GetNodeTopFrom(0); node; node=node->np, n++)
	{
		sDrawItem* s = (sDrawItem*)node->ref;

		if (s && s->gls)
		{
			if (n==index)
			{
				xglsRadioButton* btn=(xglsRadioButton*)s->gls;
				
				Bool bIsChecked=btn->GetCheck();
				btn->SetCheck(True-bIsChecked);
				id=btn->m_id;
				SendMessage(m_hNotifyWnd, WM_COMMAND, (m_id|(id<<16)), (LPARAM)this);
				break;
			}
		}
	}
}

// -----------------------------
void cglsCheckGroup::SetChecked(int index, Bool bIsChecked)
{
	int n=0;
	int id=0;

	cyiNode* node=m_listItem->GetNodeTopFrom(index);

	if (node==0)
		return ;
	sDrawItem* s = (sDrawItem*)node->ref;
	xglsRadioButton* btn=(xglsRadioButton*)s->gls;
	
	Bool b=btn->GetCheck();

	//if (b==bIsChecked)
	//	return ;

	btn->SetCheck(bIsChecked);
	id=btn->m_id;
	SendMessage(m_hNotifyWnd, WM_COMMAND, (m_id|(id<<16)), (LPARAM)this);

	//for (cyiNode* node=m_listItem->GetNodeTopFrom(0); node; node=node->np, n++)
	//{
	//	sDrawItem* s = (sDrawItem*)node->ref;

	//	if (s && s->param)
	//	{
	//		if (n==index)
	//		{
	//			xglsRadioButton* btn=(xglsRadioButton*)s->param;
	//			
	//			Bool bIsChecked=btn->GetCheck();
	//			btn->SetCheck(True-bIsChecked);
	//			id=btn->m_id;
	//			SendMessage(m_hNotifyWnd, WM_COMMAND, (m_id|(id<<16)), (LPARAM)this);
	//			break;
	//		}
	//	}
	//}
}

// -----------------------------
Bool cglsCheckGroup::GetChecked(int index, int id)
{
	if (id!=-1)
	{
		index =GetIndex(id);
		//this->
	}
	cyiNode* node=m_listItem->GetNodeTopFrom(index);
	sDrawItem* s = (sDrawItem*)node->ref;

	if (s && s->gls)
	{
		xglsRadioButton* btn = (xglsRadioButton*)s->gls;
		return btn->GetCheck();
	}

	return False;
}
