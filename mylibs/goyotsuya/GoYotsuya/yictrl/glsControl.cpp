#include "glsControl.h"

cglsControl::cglsControl(void)
{
}

cglsControl::~cglsControl(void)
{
}

// ---------------------------------------------
void cglsControl::xDefaultDetail(sGlsControlDetail* detail)
{
	detail->align=(eGlsItemAlign)(eGIA_LEFT|eGIA_TOP);
	detail->mode=eGCIM_SIZE_FIXED;
	detail->bgColor=0;
	detail->bCommandNotify=False;
}

// ---------------------------------------------
Bool cglsControl::Create(cGlassInterface* parent, HINSTANCE hInstance, int id, RECT* lpRect, sGlsControlDetail* detail)
{
	if (detail)
		m_detail = *detail;
	else
		xDefaultDetail(&m_detail);

	RECT rect = {0, 0, 10, 10};

	if (lpRect)
		rect=*lpRect;

	OnUpdatePosition();

	sGlsDetail gls;

	cGlassInterface::xDefaultDetail(&gls);

	if (cGlassInterface::Create((HWND)parent, hInstance, id, &rect)==True)
		return True;

	return False;
}

// ---------------------------------------------
void cglsControl::Destroy()
{
	__super::Destroy();
}

// ---------------------------------------------
// この関数が呼び出されると、
// 1. OnUpdateFontが呼び出され、
// 2. OnItemUpdateが呼び出され、
// 3. UpdateForcedが呼び出され、
// 4. OnUpdatePositionが呼び出される。
Bool cglsControl::ReArrangeItemz()
{
	OnItemReArranged();

	return True;
}


// ---------------------------------------------
Bool cglsControl::ReSize(UINT x, UINT y, Bool bIsRepaint)
{
	if (__super::ReSize(x, y, bIsRepaint)==True)
	{
		if (m_detail.mode!=eGCIM_SIZE_DYNAMIC)
		{
			ReArrangeItemz();
		}

		return True;
	}

	return False;
}

// ------------------------------------
void cglsControl::OnItemReArranged()
{
	// ここでアイテムの再配置を行う。
	if (m_children==0)
		return ;

	Update(eGUM_FORCED);

	xNODELOOP(node, m_children)
	{
		cGlassInterface* obj = (cGlassInterface*)node->ref;
		obj->MoveByPosition(0, 0);
	}

	if (m_detail.mode==eGCIM_SIZE_DYNAMIC)
	{
		int maxWidth=0;
		int maxHeight=0;
		xNODELOOP(node, m_children)
		{
			cGlassInterface* obj = (cGlassInterface*)node->ref;
			
			maxWidth=(maxWidth<(obj->m_w+obj->m_x))?obj->m_w+obj->m_x:maxWidth;
			maxHeight=(maxHeight<(obj->m_h+obj->m_y))?obj->m_h+obj->m_y:maxHeight;
		}

		ReSize(maxWidth, maxHeight, False);
	}

	if (m_detail.align==0)
		return ;

	int x, y;
	RECT rect;
	m_parent->GetMargin(rect);

	if (m_detail.align&eGIA_LEFT)
	{
		x=rect.left;
	}
	else if (m_detail.align==eGIA_CENTER)
	{
		x=(m_parent->m_w-m_w)>>1;
	}
	else if (m_detail.align==eGIA_RIGHT)
	{
		x=(m_parent->m_w-m_w)-rect.right;
	}

	if (m_detail.align&eGIA_TOP)
	{
		y=rect.top;
	}
	else if (m_detail.align==eGIA_CENTER)
	{
		y=(m_parent->m_h-m_h)>>1;
	}
	else if (m_detail.align==eGIA_RIGHT)
	{
		x=(m_parent->m_h-m_h)-rect.bottom;
	}

	Move(x, y);

	return ;
}

// ------------------------------------
Bool cglsControl::OnCreate()
{
	OnDefItemArrange();

	return True;
}

// ------------------------------------
LRESULT cglsControl::OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	if (m_detail.bCommandNotify==True)
	{
		if (m_parent)
			return m_parent->OnCommand(wParam, lParam, bIsHandled);
	}

	return 0;
}
