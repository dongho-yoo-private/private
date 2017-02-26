#include "glsLayoutObject.h"

cglsLayoutObject::cglsLayoutObject(void)
: m_listLayout(False),
m_item(False)
{
}

cglsLayoutObject::~cglsLayoutObject(void)
{
}

// -----------------------------------
Bool cglsLayoutObject::CreateLayout(cGlassInterface* main)
{
	m_w=main->m_w;
	m_h=main->m_h;
	m_main=main;
}

// -----------------------------------
Bool cglsLayoutObject::ReSize(int w, int h)
{
	if (m_main->ReSize(w, h)==True)
	{
		m_w=w;
		m_h=h;

		m_main->OnReSize(w, h);
		m_main->InvalidateRect(0);
		return True;
	}
}

typedef struct {
	int id;
	RECT rect;
}sLayoutObject;

// -----------------------------------
int cglsLayoutObject::AddLayout(int id, Rect* rect)
{
	for (cyiNode* node=m_listLayout.GetNodeTopFrom(); node; node=node->np)
	{
		sLayoutObject* layout = (sLayoutObject*)node->ref;

		if (layout->id==id)
		{
			return False;
		}
	}

	sLayoutObject layout={id, *rect};
	m_listLayout.addref(yitoheap(&layout, sizeof(layout)));

	return m_listLayout.count();
}

// -----------------------------------
int cglsLayoutObject::AddLayout(int id, int x, int y)
{
	RECT rect={x, y, 0, 0};
	return AddLayout(id, &rect);
}

// -----------------------------------
int cglsLayoutObject::AddLayout(int id, RECT* rect, eLayout layout)
{
	int x=0, y=0;
	x = xGetPos(rect->left, rect->right, (layout&0x0F));
	y = xGetPos(rect->top, rect->bottom, (layout&0xF0));

	return AddLayout(id, x, y);
}

// -----------------------------------
int cglsLayoutObject::SetLayout(int id, Rect* rect)
{
	for (cyiNode* node=m_listLayout.GetNodeTopFrom(); node; node=node->np)
	{
		sLayoutObject* layout = (sLayoutObject*)node->ref;

		if (layout->id==id)
		{
			layout->rect=*rect;
			return True;
		}
	}

	return False;
}

typedef struct {
	int id;
	eLayout layout;
	cGlassInterface* item;
}sLayoutObjectItem;

// -----------------------------------
Bool cglsLayoutObject::SetItem(cGlassInterface* item, int nLayoutId, eLayout layout)
{
	for (cyiNode* node=m_item.GetNodeTopFrom(); node; node=node->np)
	{
		sLayoutObjectItem* layout = (sLayoutObjectItem*)node->ref;

		if (layout->item==item)
		{
			layout->id=nLayoutId;
			layout->layout=layout
			return True;
		}
	}

	sLayoutObjectItem layout = {nLayoutId, layout, item};
	m_item.addref(yitoheap(&layout, sizeof(layout));

	return True;
}

// -----------------------------------
Bool cglsLayoutObject::xGetPos(int x, int y, int w, int h, RECT* allRect, eLayout elayout)
{
	return True;
}

// -----------------------------------
Bool cglsLayoutObject::xMoveItem(cGlassInterface* item, int id, eLayout elayout)
{
	sLayoutObject* selected=0;
	for (cyiNode* node=m_listLayout.GetNodeTopFrom(); node; node=node->np)
	{
		sLayoutObject* layout = (sLayoutObject*)node->ref;

		if (layout->id==id)
		{
			selected=layout;
			break;
		}
	}

	if (selected==0)
		return False;

	unsigned int pos = xGetPos(item->m_x, item->m_y, item->m_w, item->m_h, 
								selected->rect, elayout);

	int x = pos>>16;
	int y = pos&0xFFFF;

	item->Move(x, y);

	return 0;

}


// -----------------------------------
Bool cglsLayoutObject::Update()
{
	for (cyiNode* node=m_item.GetNodeTopFrom(); node; node=node->np)
	{
		sLayoutObjectItem* layout = (sLayoutObjectItem*)node->ref;

		if (layout->item)
		{
			xMoveItem(layout->item, layout->id=nLayoutId, layout->layout);
		}
	}

	return True;
}

