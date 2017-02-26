#include "ButtonSet.h"
#include "yistd.h"

cButtonSet::cButtonSet(void)
{
	cglsDrawObject::cglsDrawObject();
}

cButtonSet::cButtonSet(int id)
{
	cglsDrawObject::cglsDrawObject(id);
	m_nDrawObjectId=id;
}

cButtonSet::~cButtonSet(void)
{
}

// --------------------------
void cButtonSet::SetBgColor(color_t color1, color_t color2)
{
	m_bgColor=color1;
	m_fgColor=color2;
}

// --------------------------
Bool cButtonSet::CreateBitmap(sDrawItem* item, void* pExtra)
{
	int w = 32;
	int h = 32;
	Bitmap* bmpBaseNormal;
	Bitmap* bmpBaseHiLight;
	Bitmap* bmpBaseMouseDown;

	Bitmap* str;
	Bitmap* str2;

	Font font(L"ƒƒCƒŠƒI", 12.0);


	if (item->id==0)
	{
		if (m_nDrawObjectId==1)
		{
			return False;
		}
		sDrawItem* xItem=(sDrawItem*)item;

		if (item->pBmp[0])
			delete item->pBmp[0];

		if (m_nDrawObjectId!=1 &&
			item->pBmp[1])
			delete item->pBmp[1];

		bmpBaseNormal = cyiShape::DrawRoundRectEx(0, &Pen(Color::Black), &SolidBrush(m_bgColor), RectF(0, 0, item->rect.w, item->rect.h), 4.0);
		item->pBmp[0] = bmpBaseNormal;

		if (m_nDrawObjectId!=1)
		{
			item->pBmp[1] = cyiShape::DrawRoundRectEx(0, &Pen(Color::Black), &SolidBrush(m_fgColor), RectF(0, 0, item->rect.w, item->rect.h), 4.0);
		}
		else 
		{
			item->pBmp[1]=item->pBmp[0];
		}
		item->pBmp[2] = item->pBmp[1];
		item->pCurrentBmp=item->pBmp[0];

		return True;
	}
	else if (item->id==-1)
	{
		if (m_nDrawObjectId==1)
		{
			item->pBmp[0]=BITMAP32(item->rect.w, item->rect.h);//cyiShape::DrawRoundRectEx(&Pen(Color::Black), &SolidBrush(Color(0, 255,255,255)), RectF(2, 2, item->rect.w-4, item->rect.h-4), 4.0);
			item->pBmp[1]=cyiShape::DrawRoundRectEx(0, &Pen(Color::Black), &SolidBrush(Color(127, 255,255,255)), RectF(2, 2, item->rect.w, item->rect.h), 4.0);
			item->pBmp[2]=cyiShape::DrawRoundRectEx(0, &Pen(Color::Black), &SolidBrush(Color(64, 255,255,255)), RectF(2, 2, item->rect.w, item->rect.h), 4.0);
			item->pCurrentBmp=item->pBmp[0];
			return True;
		}

		return False;
	}

	cglsDrawObject::CreateBitmap(item);

	//w=(int)((float)w);
	//h=(int)((float)h);

	item->rect.w=w;
	item->rect.h=h;
	
	if (item->txt!=0)
	{
		str = cyiShape::BitmapString(0, item->txt, &font, &SolidBrush(Color::Black));
		str2 = cyiShape::BitmapString(0, item->txt, &font, &SolidBrush(Color::White));
		w=item->rect.w=str->GetWidth();
	}

	bmpBaseNormal = cyiShape::DrawRoundRectEx(0, &Pen(Color::Black), &SolidBrush(Color::White), RectF(0, 0, w, h), 2.0);
	bmpBaseHiLight = cyiShape::DrawRoundRectEx(0, &Pen(Color::Black), &SolidBrush(Color::Orange), RectF(0, 0, w, h), 2.0);
	bmpBaseMouseDown = cyiShape::DrawRoundRectEx(0, &Pen(Color::Black), &SolidBrush(Color(64, 0, 0, 0)), RectF(0, 0, w, h), 2.0);



	if (item->id==1024)
	{

		if (item->txt==0)
		{
			str = cyiShape::BitmapString(0, L"1", &font, &SolidBrush(Color::Black));
			str2 = cyiShape::BitmapString(0, L"1", &font, &SolidBrush(Color::White));
		}
	}
	else if (item->id==1025)
	{
		if (item->txt==0)
		{
			str = cyiShape::BitmapString(0, L"2", &font, &SolidBrush(Color::Black));
			str2 = cyiShape::BitmapString(0, L"2", &font, &SolidBrush(Color::White));
		}
	}
	else if (item->id==1026)
	{
		if (item->txt==0)
		{
			str = cyiShape::BitmapString(0, L"3", &font, &SolidBrush(Color::Black));
			str2 = cyiShape::BitmapString(0, L"2", &font, &SolidBrush(Color::White));
		}
	}
	else
	{
		if (item->txt==0)
		{
			str = cyiShape::BitmapString(0, L"0", &font, &SolidBrush(Color::Black));
			str2 = cyiShape::BitmapString(0, L"1", &font, &SolidBrush(Color::White));
		}
	}


	Graphics g(bmpBaseNormal);
	g.DrawImage(str, (INT)xCENTER((bmpBaseNormal->GetWidth()>>1), str->GetWidth()), (INT)xCENTER((bmpBaseNormal->GetHeight()>>1), str->GetHeight()));
	item->pBmp[0]=bmpBaseNormal;

	Graphics g2(bmpBaseHiLight);
	g2.DrawImage(str2, (INT)xCENTER((bmpBaseHiLight->GetWidth()>>1), str->GetWidth()), (INT)xCENTER((bmpBaseHiLight->GetHeight()>>1), str->GetHeight()));

	Graphics g3(bmpBaseMouseDown);
	g3.DrawImage(str, (INT)xCENTER((bmpBaseMouseDown->GetWidth()>>1), str->GetWidth()), (INT)xCENTER((bmpBaseMouseDown->GetHeight()>>1), str->GetHeight()));

	item->pBmp[1]=bmpBaseHiLight;
	item->pBmp[2]=bmpBaseMouseDown;

	item->pCurrentBmp=item->pBmp[0];
	return True;
}

// --------------------------
void cButtonSet::DestroyBitmap(sDrawItem* item)
{
	return ;
}
