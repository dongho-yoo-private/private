#include "glsDrawObject.h"



// ---------------------------------------
int cglsResourceObject::GetFontHeight(Font* font)
{
	//static Bitmap* bmp = BITMAP32(48, 48);
	//static Graphics* g = new Graphics(bmp);
	if (font==0)
	{
		Font xFont(L"ƒƒCƒŠƒI", 11.0);	
		return xFont.GetHeight(__DEFAULTGRAPHICS__);
		//return xFont.GetHeight(g);
	}
	return font->GetHeight(__DEFAULTGRAPHICS__);

	//return font->GetHeight(g);
}

// ---------------------------------------
int cglsResourceObject::xGetPos(eItemAlign align, int wAll, int margin, int w)
{
	if (align==eIA_CENTER)
	{
		int _w = (wAll-(margin<<1));
		return ((_w-w)>>1)+margin;
	}
	else if (align==eSA_LEFT)
	{
		return margin;
	}
	else
	{
		return wAll-w-margin;
	}

	return 0;
}



// ---------------------------------------
typedef struct xsResourceItem {
	Bitmap* bmp;
	int subId;
	color_t color;
	int w;
	int h;
} sResourceItem;

// ---------------------------------------
cglsResourceObject::cglsResourceObject()

{
	memset(m_resources, 0, sizeof(m_resources));
}

// ---------------------------------------
cglsResourceObject::~cglsResourceObject()
{
}

// ---------------------------------------
Bitmap* cglsResourceObject::xGetSameObject(int id, int subId, color_t color, int w, int h)
{
	if (id>=MAX_RESOURCE_COUNT)
	{
		return 0;
	}

	if (m_resources[id]==0)
	{
		return 0;
	}

	for (cyiNode* node=m_resources[id]->GetNodeTopFrom(); node; node=node->np)
	{
		sResourceItem* item = (sResourceItem*)node->data;

		if (item->subId==subId &&
				item->w==w &&
					item->h==h)
		{
			return item->bmp;
		}
	}

	return 0;
}

#define MAX_QUEUE_CNT 32

// ---------------------------------------
void cglsResourceObject::xSetObject(Bitmap* bmp, int id, int subId, color_t color, int w, int h)
{
	if (id>=MAX_RESOURCE_COUNT)
	{
		return ;
	}

	if (m_resources[id]==0)
	{
		m_resources[id] = new cyiQueue();
	}

	if (m_resources[id]->count() > MAX_QUEUE_CNT)
	{
		yiAnyValue val;
		sResourceItem* item;
		m_resources[id]->deq(val);

		item=(sResourceItem*)val;
		delete item->bmp;
		yifree(item);
	}

	sResourceItem item={bmp, subId, w, h};
	m_resources[id]->enq((yiAnyValue)yitoheap(&item, sizeof(item)));

	return ;
}


cglsDrawObject::cglsDrawObject(void)
: m_bIsTypeUserDraw(False),
m_xresource(0),
m_xfont(0),
m_nDrawObjectId(0)
{

}

cglsDrawObject::cglsDrawObject(int id)
: m_xfont(0)
{
	cglsDrawObject::cglsDrawObject();
	m_nDrawObjectId=id;
}

cglsDrawObject::~cglsDrawObject(void)
{
}

// --------------------------
//Bool cglsDrawObject::CreateBitmap(int bmpId, const wchar_t* pszText, float scale, sDrawItem* item, void* pExtra)
//{
//	return False;
//}

// ------------------------
void cglsDrawObject::Initialize(cglsResourceObject* resource, Font* font)
{
	m_xresource=resource;
	m_xfont=font;
}

// --------------------------
Bool cglsDrawObject::CreateBitmap(sDrawItem* item, void* pExtra)
{
	for (int i=0; i<eDIS_MAX_STATUS; i++)
	{
		if (item->pBmp[i])
			delete item->pBmp[i];
		item->pBmp[i]=0;
	}
	return False;
}

// --------------------------
Bool cglsDrawObject::OnNullBitmapDraw(sDrawItem* item, void* pExtra)
{
	return False;
}


// --------------------------
void cglsDrawObject::DestroyBitmap(sDrawItem* item)
{
	return ;
}

// --------------------------
cglsUserDrawObject::cglsUserDrawObject(void)
:	cglsDrawObject()
{
	m_bIsTypeUserDraw=True;
}

// --------------------------
cglsUserDrawObject::~cglsUserDrawObject(void)
{
}