#include "yiShape.h"
#include "yiBitmapBlender.h"
#include "yistd.h"


cyiShapeMask::cyiShapeMask(int x, int y, Bitmap* bmp, Bool bIsReverse)
{
	m_bmp=bmp;
	m_bIsReverse=bIsReverse;
	m_bIsTypeBmp=True;
	m_rect.X = x;
	m_rect.Y = y;
	m_rect.Width=(bmp->GetWidth()+x)-1;
	m_rect.Height=(bmp->GetHeight()+y)-1;
	m_bmp->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &m_bmpdata);
}

cyiShapeMask::cyiShapeMask(Rect& rect, BOOL bIsReverse)
{
	m_bIsReverse=bIsReverse;
	m_bIsTypeBmp=False;
	m_rect=rect;
	m_rect.X = rect.X;
	m_rect.Y = rect.Y;
	m_rect.Width = (rect.X+rect.Width);
	m_rect.Height = (rect.Y+rect.Height);
}

cyiShapeMask::~cyiShapeMask()
{
	//if (m_bIsTypeBmp==True)
	//	m_bmp->UnlockBits(&m_bmpdata);
}

Bool cyiShapeMask::IsMasked(float x, float y)
{
	UINT _x = (UINT)x;
	UINT _y = (UINT)y;

	return IsMasked(_x, _y);
}

Bool cyiShapeMask::IsMasked(int x, int y)
{
	return IsMasked((UINT)x, (UINT)y);
}

Bool cyiShapeMask::IsMasked(UINT _x, UINT _y)
{
	int x = (int)_x;
	int y = (int)_y;

	if (m_bIsTypeBmp==True)
	{
		Bool res=True;
		if (x < m_rect.X)
		{
			res = False;
		}
		else if (x > m_rect.Width)
		{
			res = False;
		}
		else if (y < m_rect.Y)
		{
			res = False;
		}
		else if (y > m_rect.Height)
		{
			res = False;
		}

		res = m_bIsReverse ? (1-res):res;

		if (res==False)
			return False;

		int w = m_rect.Width-m_rect.X+1;
		int h = m_rect.Height-m_rect.Y;

		//if (x>w ||
		//		y>h)
		//{
		//	return False;
		//}

		color_t* color = (color_t*)m_bmpdata.Scan0;
		if (color[(x-m_rect.X)+w*(y-m_rect.Y)] & 0xFF000000)
		{
			return m_bIsReverse ? False:True;
		}
		return m_bIsReverse ? True:False;
	}
	else
	{
		Bool res=True;
		if (x < m_rect.X)
		{
			res = False;
		}
		else if (x > m_rect.Width)
		{
			res = False;
		}
		else if (y < m_rect.Y)
		{
			res = False;
		}
		else if (y > m_rect.Height)
		{
			res = False;
		}
		return m_bIsReverse ? (1-res):res;
	}

	return m_bIsReverse ? False:True;
}

//Bool cyiShapeMask::IsMasked(Rect& rect, Rect& out)
//{
//	return True;
//}

float cyiShapeMask::GetMaskedLevel(int x, int y)
{
	if (m_bIsTypeBmp==True)
	{
		Bool res=True;
		if (x < m_rect.X)
		{
			res = False;
		}
		else if (x > m_rect.Width)
		{
			res = False;
		}
		else if (y < m_rect.Y)
		{
			res = False;
		}
		else if (y > m_rect.Height)
		{
			res = False;
		}

		res = m_bIsReverse ? (1-res):res;

		if (res==False)
			return 0.0;

		if (m_bIsReverse)
		{
			return 1.0;
		}

		int w = m_rect.Width-m_rect.X+1;
		int h = m_rect.Height-m_rect.Y;

		color_t* color = (color_t*)m_bmpdata.Scan0;
		unsigned int a = __COLOR_Ap__(color[(x-m_rect.X)+w*(y-m_rect.Y)]);
		if (a==0)
		{
			return 0.0;
		}
		else if (a==255)
		{
			return 1.0;
		}

		return (float)a/255.0;
	}
	else
	{
		return IsMasked((UINT)x, (UINT)y) == True ? 1.0:0;
	}

	return 0;
}

Bitmap* yiUtilNewBitmap(int w, int h, PixelFormat format)
{
	return new Bitmap(w, h, format);
}

void yiUtilDeleteBitmap(Bitmap* bmp)
{
	if (bmp!=0)
		delete bmp;
}


cyiShape::cyiShape(void)
{
}

cyiShape::~cyiShape(void)
{
}

Bitmap* cyiShape::DrawRoundRect3D(color_t light, color_t dark, const RectF& rect, float radius, float level, GraphicsPath* path)
{
	GraphicsPath* _path = path;
	GraphicsPath __path;


	if (_path==NULL)
	{
		_path = &__path;
		
	}

	Bitmap* bmp = new Bitmap((UINT)rect.Width, (UINT)rect.Height, PixelFormat32bppARGB);
	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	_path->AddLine(rect.X, rect.Height-1-radius, rect.X, rect.Y+radius);
	_path->AddArc(rect.X, rect.Y, radius*2, radius*2, 180.0, 90.0);
	_path->AddLine(rect.X+radius, rect.Y, rect.Width-radius-1, rect.Y);

	g.DrawPath(&Pen(Color((ARGB)light), level), _path);

	GraphicsPath path2;
	_path=&path2;

	_path->AddArc(rect.Width-radius*2-1, rect.Y, radius*2, radius*2, -90.0, 90.0);
	//_path->AddBezier(PointF(rect.Width-radius, rect.Y), PointF(rect.Width-1,rect.Y), PointF(rect.Width-1,rect.Y), PointF(rect.Width-1, rect.Y+radius));
	_path->AddLine(rect.Width-1, rect.Y+radius, rect.Width-1, rect.Height-radius);
	_path->AddArc(rect.Width-radius*2-1, rect.Height-radius*2-1, radius*2, radius*2, 0.0, 90.0);
	_path->AddLine(rect.Width-radius-1, rect.Height-1, rect.X+radius, rect.Height-1);
	_path->AddArc(rect.X, rect.Height-1-radius*2, radius*2, radius*2, 90.0, 90.0);

	//g.FillPath(brush, _path);
	g.DrawPath(&Pen(Color((ARGB)dark), level), _path);

	return bmp;
}

Bitmap* cyiShape::DrawRoundRect(Pen* pen, const RectF& rect, float radius, GraphicsPath* path)
{
	GraphicsPath* _path = path;
	GraphicsPath __path;

	if (radius==0.0)
	{
		Bitmap* bmp = new Bitmap((UINT)rect.Width, (UINT)rect.Height, PixelFormat32bppARGB);
		Graphics g(bmp);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		g.DrawRectangle(pen, rect.X, rect.Y, rect.Width-1, rect.Height-1);
		return bmp;
	}

	if (_path==NULL)
	{
		_path = &__path;
		
	}

	_path->AddLine(rect.X+radius, rect.Y, rect.Width-radius-1, rect.Y);
	_path->AddArc(rect.Width-radius*2-1, rect.Y, radius*2, radius*2, -90.0, 90.0);
	//_path->AddBezier(PointF(rect.Width-radius, rect.Y), PointF(rect.Width-1,rect.Y), PointF(rect.Width-1,rect.Y), PointF(rect.Width-1, rect.Y+radius));
	_path->AddLine(rect.Width-1, rect.Y+radius, rect.Width-1, rect.Height-radius);
	_path->AddArc(rect.Width-radius*2-1, rect.Height-radius*2-1, radius*2, radius*2, 0.0, 90.0);
	_path->AddLine(rect.Width-radius-1, rect.Height-1, rect.X+radius, rect.Height-1);
	_path->AddArc(rect.X, rect.Height-1-radius*2, radius*2, radius*2, 90.0, 90.0);
	_path->AddLine(rect.X, rect.Height-1-radius, rect.X, rect.Y+radius);
	_path->AddArc(rect.X, rect.Y, radius*2, radius*2, 180.0, 90.0);

	Bitmap* bmp = new Bitmap((UINT)rect.Width, (UINT)rect.Height, PixelFormat32bppARGB);
	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	//g.FillPath(brush, _path);
	g.DrawPath(pen, _path);

	return bmp;
}

// -------------------------------
Bitmap* cyiShape::DrawRoundRectHiLight(Pen* pen, const RectF& rect, float radius, GraphicsPath* path)
{
	GraphicsPath* _path = path;
	GraphicsPath __path;

	if (radius==0.0)
	{
		Bitmap* bmp = new Bitmap((UINT)rect.Width, (UINT)rect.Height, PixelFormat32bppARGB);
		Graphics g(bmp);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		g.DrawRectangle(pen, rect);
		return bmp;
	}

	if (_path==NULL)
	{
		_path = &__path;
		
	}

	//_path->AddArc(rect.Width-radius*2-1, rect.Y, radius*2, radius*2, -90.0, 90.0);
	//_path->AddBezier(PointF(rect.Width-radius, rect.Y), PointF(rect.Width-1,rect.Y), PointF(rect.Width-1,rect.Y), PointF(rect.Width-1, rect.Y+radius));
	//_path->AddLine(rect.Width-1, rect.Y+radius, rect.Width-1, rect.Height-radius);
	//_path->AddArc(rect.Width-radius*2-1, rect.Height-radius*2-1, radius*2, radius*2, 0.0, 90.0);
	//_path->AddLine(rect.Width-radius-1, rect.Height-1, rect.X+radius, rect.Height-1);
	//_path->AddArc(rect.X, rect.Height-1-radius*2, radius*2, radius*2, 90.0, 90.0);
	_path->AddLine(rect.X, rect.Height-1-radius, rect.X, rect.Y+radius);
	_path->AddArc(rect.X, rect.Y, radius*2, radius*2, 180.0, 90.0);
	_path->AddLine(rect.X+radius, rect.Y, rect.Width-radius-1, rect.Y);

	Bitmap* bmp = new Bitmap((UINT)rect.Width, (UINT)rect.Height, PixelFormat32bppARGB);
	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	//g.FillPath(brush, _path);
	g.DrawPath(pen, _path);

	//Bitmap* ret = cyiBitmapBlender::GaussBlureXX(bmp, 1.0, 127, 0);
	//delete bmp;
	return bmp;

}

// -------------------------------
Bitmap* cyiShape::SimpleShadow(Bitmap* bmp, int w, int h, float fTrans, float radius)
{
	GraphicsPath __path;
	GraphicsPath* _path = &__path;
	color_t color = ((color_t)(255.0*fTrans))<<24;

	RectF rect(0, 0, w, h);
	_path->AddArc(rect.Width-radius*2-1, rect.Y, radius*2, radius*2, -45.0, 45.0); 
	_path->AddLine(rect.Width-1, rect.Y+radius, rect.Width-1, rect.Height-radius); // |
	_path->AddArc(rect.Width-radius*2-1, rect.Height-radius*2-1, radius*2, radius*2, 0.0, 90.0); 
	_path->AddLine(rect.Width-radius-1, rect.Height-1, rect.X+radius, rect.Height-1); // _
	_path->AddArc(rect.X, rect.Height-1-radius*2, radius*2, radius*2, 45.0, 45.0); 

	if (bmp==0)
	{
		bmp = new Bitmap((UINT)rect.Width, (UINT)rect.Height, PixelFormat32bppARGB);
	}
	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	g.DrawPath(&Pen(Color(color)), _path);
	return bmp;
}


// -------------------------------
Bitmap* cyiShape::DrawRoundRectEx(Bitmap* bmp, Pen* pen, Brush* brush, const RectF& rect, float radius, GraphicsPath* path)
{
	GraphicsPath* _path = path;
	GraphicsPath __path;

	if (radius==0.0)
	{
		Bitmap* bmp = new Bitmap((UINT)rect.Width, (UINT)rect.Height, PixelFormat32bppARGB);
		Graphics g(bmp);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		g.FillRectangle(brush, rect);
		g.DrawRectangle(pen, rect);
		return bmp;
	}

	if (_path==NULL)
	{
		_path = &__path;
	}

	_path->AddLine(rect.X+radius, rect.Y, rect.Width-radius-1, rect.Y);
	_path->AddArc(rect.Width-radius*2-1, rect.Y, radius*2, radius*2, -90.0, 90.0);
	//_path->AddBezier(PointF(rect.Width-radius, rect.Y), PointF(rect.Width-1,rect.Y), PointF(rect.Width-1,rect.Y), PointF(rect.Width-1, rect.Y+radius));
	_path->AddLine(rect.Width-1, rect.Y+radius, rect.Width-1, rect.Height-radius);
	_path->AddArc(rect.Width-radius*2-1, rect.Height-radius*2-1, radius*2, radius*2, 0.0, 90.0);
	_path->AddLine(rect.Width-radius-1, rect.Height-1, rect.X+radius, rect.Height-1);
	_path->AddArc(rect.X, rect.Height-1-radius*2, radius*2, radius*2, 90.0, 90.0);
	_path->AddLine(rect.X, rect.Height-1-radius, rect.X, rect.Y+radius);
	_path->AddArc(rect.X, rect.Y, radius*2, radius*2, 180.0, 90.0);

	if (bmp==0)
	{
		bmp = new Bitmap((UINT)rect.Width, (UINT)rect.Height, PixelFormat32bppARGB);
	}

	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	g.FillPath(brush, _path);
	g.DrawPath(pen, _path);

	return bmp;
}

// -------------------------------
Rect& cyiShape::MarginRect(Rect& rect, int x, int y)
{
	rect.X = rect.X+x;
	rect.Y = rect.Y+y;
	rect.Width = rect.Width-(x<<1);
	rect.Height = rect.Height-(y<<1);
	return rect;
}

Bitmap* cyiShape::FillRoundRect(Brush* brush, const RectF& rect, float radius, GraphicsPath* path)
{
	GraphicsPath* _path = path;
	GraphicsPath __path;

	if (radius==0.0)
	{
		Bitmap* bmp = new Bitmap((UINT)rect.Width, (UINT)rect.Height, PixelFormat32bppARGB);
		Graphics g(bmp);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		g.FillRectangle(brush, rect);
		return bmp;
	}

	if (_path==NULL)
	{
		_path = &__path;
		
	}

	_path->AddLine(rect.X+radius, rect.Y, rect.Width-radius-1, rect.Y);
	_path->AddArc(rect.Width-radius*2-1, rect.Y, radius*2, radius*2, -90.0, 90.0);
	//_path->AddBezier(PointF(rect.Width-radius, rect.Y), PointF(rect.Width-1,rect.Y), PointF(rect.Width-1,rect.Y), PointF(rect.Width-1, rect.Y+radius));
	_path->AddLine(rect.Width-1, rect.Y+radius, rect.Width-1, rect.Height-radius);
	_path->AddArc(rect.Width-radius*2-1, rect.Height-radius*2-1, radius*2, radius*2, 0.0, 90.0);
	_path->AddLine(rect.Width-radius-1, rect.Height-1, rect.X+radius, rect.Height-1);
	_path->AddArc(rect.X, rect.Height-1-radius*2, radius*2, radius*2, 90.0, 90.0);
	_path->AddLine(rect.X, rect.Height-1-radius, rect.X, rect.Y+radius);
	_path->AddArc(rect.X, rect.Y, radius*2, radius*2, 180.0, 90.0);


	//_path->AddLine(rect.X, rect.Y+radius, rect.X, rect.GetBottom()-radius);
	

	//_path->AddArc(rect.X, rect.Y, radius*2, radius*2, 90.0, -90.0);
	//_path->AddArc(rect.X, rect.GetBottom()-radius*2, radius*2, radius*2, 180.0, -90.0);
	


	Bitmap* bmp = new Bitmap((UINT)rect.Width, (UINT)rect.Height, PixelFormat32bppARGB);
	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	g.FillPath(brush, _path);
	//g.DrawPath(&Pen(color, 1), _path);

	return bmp;
}

#include <math.h>


// ---------------------------
Bitmap* cyiShape::DrawRoundTriangle(Bitmap* bmp, Pen* pen, const RectF& rect, float radius, float rotate, GraphicsPath* _path)
{
	GraphicsPath* path = _path;
	GraphicsPath __path;

	if (_path==NULL)
	{
		path = &__path;
		
	}

	float w = rect.Width;
	float h = rect.Height;
	float sw = w/2;
	float l = sqrtf(sw*sw+h*h);
	float _cos = sw/l;
	float _sin = h/l;
	float x=rect.X;
	float y=rect.Y;

	PointF pos1(x, y);
	PointF pos2(sw+x, h+y);
	PointF pos3(w+x, y);
	PointF posEdge1[2];
	PointF posEdge2[2];
	PointF posEdge3[2];

	posEdge1[0].X=pos1.X+radius;
	posEdge1[0].Y=pos1.Y;
	posEdge1[1].X=posEdge1[0].X*_cos;
	posEdge1[1].Y=posEdge1[0].X*_sin;

	posEdge2[0].X=sw-(_cos*radius);
	posEdge2[0].Y=h-(_sin*radius);
	posEdge2[1].X=sw+(sw-posEdge2[0].X);
	posEdge2[1].Y=posEdge2[0].Y;

	posEdge3[0].X=pos3.X-radius;
	posEdge3[0].Y=pos3.Y;
	posEdge3[1].X=(sw-posEdge1[1].X)+sw;
	posEdge3[1].Y=posEdge1[1].Y;

	//RotatePos(PointF(sw, h/2), rotate,  &pos2,  &pos3, &pos1,&posEdge1[0], &posEdge1[1], 
	//											&posEdge2[0], &posEdge2[1], 
	//												&posEdge3[0], &posEdge3[1], NULL); 

	path->AddLine(posEdge1[0], posEdge3[0]);
	path->AddBezier(posEdge3[0], pos3, pos3, posEdge3[1]);
	path->AddLine(posEdge3[1], posEdge2[1]);
	path->AddBezier(posEdge2[1], pos2, pos2, posEdge2[0]);
	path->AddLine(posEdge2[0], posEdge1[1]);
	path->AddBezier(posEdge1[1], pos1, pos1, posEdge1[0]);

	if (bmp==NULL)
		bmp = new Bitmap((UINT)rect.Width, (UINT)rect.Height, PixelFormat32bppARGB);

	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	g.DrawPath(pen, path);

	return bmp;

}

// -------------------------------
Bitmap* cyiShape::DrawCross(Bitmap* bmp, Pen* pen, const RectF& rect)
{
	int w, h;

	if (bmp==0)
	{
		bmp = new Bitmap(rect.Width, rect.Height, PixelFormat32bppARGB);
	}
	w=bmp->GetWidth();
	h=bmp->GetHeight();

	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	g.DrawLine(pen, rect.X, rect.Y, rect.X+rect.Width-1, rect.Y+rect.Height-1);
	g.DrawLine(pen, rect.X+rect.Width-1, rect.Y, rect.X, rect.Y+rect.Height-1);

	//Bitmap* cir = new Bitmap(w, h, PixelFormat32bppARGB);
	//Graphics g2(cir);
	//g2.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	//g2.FillEllipse(&SolidBrush(Color::Black),rect.X+1, rect.Y+1, rect.Width-2, rect.Height-2);
	//Bitmap* bmp2 = cyiBitmapBlender::SetMask(bmp, cir,0, 0);

	//delete cir;

	return bmp;
}


// -------------------------------
static color_t xGetBetweenColor(color_t color1, color_t color2, float rate)
{
	if (rate<=0.0)
		return color1;
	if (rate>=1.0)
		return color2;

	int a1 = __COLOR_Ap__(color1);
	int a2 = __COLOR_Ap__(color2);
	int r1 = __COLOR_Rp__(color1);
	int r2 = __COLOR_Rp__(color2);
	int g1 = __COLOR_Gp__(color1);
	int g2 = __COLOR_Gp__(color2);
	int b1 = __COLOR_Bp__(color1);
	int b2 = __COLOR_Bp__(color2);

	int a = (int)((float)(color1>>24)*(1.0-rate)+(color2>>24)*(rate));
	int r = (int)((float)((color1>>16)&0xFF)*(1.0-rate)+((color2>>16)&0xFF)*(rate));
	int g = (int)((float)((color1>>8)&0xFF)*(1.0-rate)+((color2>>8)&0xFF)*(rate));
	int b = (int)((float)(color1&0xFF)*(1.0-rate)+(color2&0xFF)*(rate));

	return __COLORp__(a, r, g, b);
}

// -------------------------------
Bitmap* cyiShape::FillRadialGradiation(Bitmap* dest, int xCenter, int yCenter, float rad, color_t from, color_t to)
{
	int w, h;

	if (dest==0)
	{
		w=rad*2;
		h=rad*2;

		dest = BITMAP32(w, h);
	}

	Graphics g(dest);
	g.SetSmoothingMode(SmoothingModeHighQuality);

	float fx=(float)xCenter;
	float fy=(float)yCenter;
	float fradDelta=rad/100;
	float frad=rad;

	for (int i=0; i<100; i++)
	{
		color_t colorAvg=xGetBetweenColor(to, from, (float)i/100.0);
		g.FillEllipse(&SolidBrush(colorAvg), fx-frad, fy-frad, frad+frad, frad+frad);
		frad-=fradDelta;
	}

	return dest;
}

// -------------------------------
Bitmap* cyiShape::DrawGlassCover(int w, int h, float edge, color_t colorThone)
{
	Bitmap* bmp=0;

	::LinearGradientBrush brush(Point(0,0), Point(0, h-1), Color(0x0FFFFFFF&Color::White), Color(0x7FFFFFFF&Color::White));
	bmp = cyiShape::DrawRoundRectEx(0, &Pen(0x7FFFFFFF&Color::LightCyan), &brush,RectF(0, 0, w-1, h-1), edge);
	//Bitmap* bmp2 = cyiShape::DrawRoundRect(&Pen(0x7FFFFFFF&Color::LightGray), RectF(0, 0, w-2, h-2), edge);

	Graphics g(bmp);
	//g.DrawImage(bmp2, 1, 1);

	return bmp;
}


// -------------------------------
Bitmap* cyiShape::DrawTriAngleEx(Bitmap* bmp, int nDirection, Pen* pen, Brush* brush, const RectF& rect, GraphicsPath* _path)
{
	GraphicsPath* path = _path;
	GraphicsPath __path;

	if (_path==NULL)
	{
		path = &__path;
		
	}

	PointF pos1, pos2, pos3;

	if (nDirection==0) // down
	{
		pos1.X = rect.X;
		pos1.Y = rect.Y;
		pos2.X = (rect.X+rect.GetRight())/2.0;
		pos2.Y = rect.GetBottom();
		pos3.X = rect.GetRight();
		pos3.Y = rect.Y;
	}
	else if (nDirection==3) // left
	{
		pos1.X = rect.GetRight();
		pos1.Y = rect.Y;
		pos2.X = rect.X;
		pos2.Y = (rect.GetBottom()+rect.Y)/2.0;
		pos3.X = rect.GetRight();
		pos3.Y = rect.GetBottom();
	}
	else if (nDirection==2) // up
	{
		pos1.X = rect.X;
		pos1.Y = rect.GetBottom();
		pos2.X = (rect.X+rect.GetRight())/2.0;
		pos2.Y = rect.Y;
		pos3.X = rect.GetRight();
		pos3.Y = rect.GetBottom();
	}
	else if (nDirection==1) // right
	{
		pos1.X = rect.X;
		pos1.Y = rect.Y;
		pos2.X = rect.GetRight();
		pos2.Y = (rect.GetBottom()+rect.Y)/2.0;
		pos3.X = rect.X;
		pos3.Y = rect.GetBottom();
	}

	//PointF pos1(x, y);
	//PointF pos2(sw, h);
	//PointF pos3(w, y);

	path->AddLine(pos1, pos3);
	path->AddLine(pos3, pos2);
	path->AddLine(pos2, pos1);

	if (bmp==NULL)
		bmp = new Bitmap((UINT)rect.Width, (UINT)rect.Height, PixelFormat32bppARGB);

	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	if (brush)
		g.FillPath(brush, path);
	if (pen)
		g.DrawPath(pen, path);

	return bmp;

}


// ---------------------------
Bitmap* cyiShape::FillRoundTriangle(Bitmap* bmp, Brush* brush, const RectF& rect, float radius, float rotate, GraphicsPath* _path)
{
	GraphicsPath* path = _path;
	GraphicsPath __path;

	if (_path==NULL)
	{
		path = &__path;
		
	}

	float x = rect.X;
	float y = rect.Y;
	float w = rect.GetRight();
	float h = rect.GetBottom();
	float sw = (x+w)/2;
	float l = sqrtf(sw*sw+h*h);
	float _cos = sw/l;
	float _sin = h/l;

	PointF pos1(x, y);
	PointF pos2(sw, h);
	PointF pos3(w, y);
	PointF posEdge1[2];
	PointF posEdge2[2];
	PointF posEdge3[2];

	posEdge1[0].X=pos1.X+radius;
	posEdge1[0].Y=pos1.Y;
	posEdge1[1].X=posEdge1[0].X*_cos;
	posEdge1[1].Y=posEdge1[0].X*_sin;

	posEdge2[0].X=sw-(_cos*radius);
	posEdge2[0].Y=h-(_sin*radius);
	posEdge2[1].X=sw+(sw-posEdge2[0].X);
	posEdge2[1].Y=posEdge2[0].Y;

	posEdge3[0].X=pos3.X-radius;
	posEdge3[0].Y=pos3.Y;
	posEdge3[1].X=(sw-posEdge1[1].X)+sw;
	posEdge3[1].Y=posEdge1[1].Y;

	//RotatePos(PointF(sw, h/2), rotate,  &pos2,  &pos3, &pos1,&posEdge1[0], &posEdge1[1], 
	//											&posEdge2[0], &posEdge2[1], 
	//												&posEdge3[0], &posEdge3[1], NULL); 

	path->AddLine(posEdge1[0], posEdge3[0]);
	path->AddBezier(posEdge3[0], pos3, pos3, posEdge3[1]);
	path->AddLine(posEdge3[1], posEdge2[1]);
	path->AddBezier(posEdge2[1], pos2, pos2, posEdge2[0]);
	path->AddLine(posEdge2[0], posEdge1[1]);
	path->AddBezier(posEdge1[1], pos1, pos1, posEdge1[0]);

	if (bmp==NULL)
		bmp = new Bitmap((UINT)rect.Width, (UINT)rect.Height, PixelFormat32bppARGB);

	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	g.FillPath(brush, path);
	//g.DrawPath(&Pen(color, 1), path);

	return bmp;

}

#define PIE ((float)3.141592)

// ---------------------------
static float GetRadian(float x, float y, float r)
{
	float t1 = acosf(x/r);
	float t2 = asinf(y/r);

	if (x==0.0 ||
		y==0.0)
	{
		return t1;
	}

	if (x>0)
	{
		if (y>0)
		{
			return t1;
		}
		else
		{
			return t2;
		}
	}
	else
	{
		if (y>0)
		{
			return 3*PIE/2-t2;
		}
		else
		{
			return 2*PIE-t1;//-(PIE/2-t1);
		}
	}

	return  t1;
}

// -------------------------------
int cyiShape::RotatePos(PointF& center, float radian, PointF* pos1, ...)
{
	va_list ap; 
	float _radian = PIE*(radian/180.0);
	float centerX = center.X;
	float centerY = center.Y;


	va_start(ap,pos1); 
	PointF** pPointers = (PointF**)&pos1;

	int i=0;
	while(pPointers[i])
	{
		float x1 = pPointers[i]->X-centerX;
		float y1 = pPointers[i]->Y-centerY;
		float r = sqrtf(x1*x1+y1*y1);
		float theata = GetRadian(x1, y1, r);
		float theata2 = (theata+_radian);

		pPointers[i]->X = r*cosf(theata2);
		pPointers[i]->Y = r*sinf(theata2);
		pPointers[i]->X += centerX;
		pPointers[i]->Y += centerY;
		i++;
	}

	va_end(ap); 
	return i;
}

#ifndef PIE
#define PIE ((float)3.141592))
#endif

#define ROOT2 ((float)1.4142)

// -------------------------------
BOOL cyiShape::FillX(Graphics& g, float x, float y, float w, Color& color, float LineWidth, BOOL IsRound/*=FALSE*/)
{
	//float x = ::cos((float)PIE/4)*((w>>1)>>1);
	//float y = ::sin((float)PIE/4)*((h>>1)>>1);

	Pen pen(color, LineWidth);

	Bitmap* bmp = new Bitmap(w, w, PixelFormat32bppARGB);

	if (IsRound==TRUE)
	{
		pen.SetStartCap(LineCap::LineCapRound);
		pen.SetEndCap(LineCap::LineCapRound);
	}

	float delta = LineWidth==1.0 ? 0.0 :(LineWidth/(::sqrtf(4.0*ROOT2)));
	float _x = delta;
	float _y = delta;
	float _w = ((float)w-delta);
	float _h = ((float)w-delta);

	g.DrawLine(&pen, _x, _y, _w+_x, _h+_y);
	g.DrawLine(&pen, _w, _y, _x, _h+_y);

	return TRUE;
}


// -------------------------------
Bitmap* cyiShape::FillMikatsuki(Bitmap* bmp, int w, int h, Brush* brush, BOOL IsHorizental)
{
	if (bmp==NULL)
	{
		bmp = new Bitmap(w, h, PixelFormat32bppARGB);
	}

	PointF pt[4];

	if (IsHorizental==TRUE)
	{
		pt[0].X = w;
		pt[0].Y = 0;
		pt[1].X = 0;
		pt[1].Y = h/2;
		pt[2].X = w;
		pt[2].Y = h;
		pt[3].X = w/2;
		pt[3].Y = h/2;
	}
	else
	{
		pt[0].X = 0;
		pt[0].Y = h;
		pt[1].X = w/2;
		pt[1].Y = 0;
		pt[2].X = w;
		pt[2].Y = h;
		pt[3].X = w/2;
		pt[3].Y = h/2;
	}

	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	g.FillClosedCurve(brush, pt, 4);

	return bmp;
}


// -------------------------------
Bitmap* cyiShape::BitmapString(Bitmap* bmp, const wchar_t* str, Font* fnt, Brush* brush, eShapeTextStyle style, color_t glowcolor/*=0*/, float rad/*=2.0*/, float level/*=3.0*/)
{
	Bitmap xbmp(1, 1, PixelFormat32bppARGB);
	Graphics g(&xbmp);
	RectF bound;
	int len=0;
	Bitmap* mask=0;
	int maskLen=0;
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	g.SetTextRenderingHint(TextRenderingHint::TextRenderingHintClearTypeGridFit);

	len =  wcslen(str);
	Status ret = g.MeasureString(str, len, fnt, RectF(0, 0, 1024, 100), 0, &bound);

	//if (len==1)
	//{
	//	wchar_t xxxx[4]={str[0], str[0], str[0], 0};
	//	
	//	ret=g.MeasureString(xxxx, 3, fnt, RectF(0, 0, 1024, 100), 0, &bound);

	//	RectF bound2;
	//	ret=g.MeasureString(xxxx, 2, fnt, RectF(0, 0, 1024, 100), 0, &bound2);
	//	bound.Width-=bound2.Width;
	//}

	Bool bIsDelete=False;
	if (bmp==0)
	{
		UINT xlen = (UINT)rad;
		bIsDelete=True;

		if (len && 
			(style==eSTS_FIXED || style==eSTS_FIXED_NORMAL))
		{
			UINT w= (UINT)rad > (INT)(bound.Width+1) ? (INT)(bound.Width+1):(UINT)xlen;
			bmp = new Bitmap(xlen, (INT)(bound.Height+1), PixelFormat32bppARGB);

			if ((INT)(bound.Width+1)>xlen)
			{
				int i;
				mask = BitmapString(0, L"...", fnt, brush);
				maskLen=mask->GetWidth();
				style=eSTS_FIXED;
				for (i=len-1; i>-1; i--)
				{
					Status ret = g.MeasureString(str, i, fnt, RectF(0, 0, 1024, 100), 0, &bound);

					if ((INT)(bound.Width+1+maskLen)<=xlen)
					{
						len=i;
						break;
					}
				}



				if (glowcolor)
				{
					Bool* temp = (Bool*)glowcolor;
					*temp=1;
				}

			}
			else
			{
				if (glowcolor)
				{
					Bool* temp = (Bool*)glowcolor;
					*temp=0;
				}
				if (style==eSTS_FIXED_NORMAL)
				{
					delete bmp;
					bmp = new Bitmap((INT)(bound.Width)+1, (INT)(bound.Height)+1, PixelFormat32bppARGB);

					Color col;

					((SolidBrush*)brush)->GetColor(&col);
					cBB::FillColor(bmp, col.GetValue()& 0x00FFFFFF);
					style=eSTS_NONE;
				}
			}
		}
		else
		{
			if (style==eSTS_FIXED)
			{
				style=eSTS_NONE;
				if (glowcolor)
				{
					Bool* temp = (Bool*)glowcolor;
					*temp=0;
				}
			}

			bmp = new Bitmap((INT)(bound.Width)+1, (INT)(bound.Height)+1, PixelFormat32bppARGB);
			Color col;

			((SolidBrush*)brush)->GetColor(&col);
			cBB::FillColor(bmp,  col.GetValue() & 0x00FFFFFF);
		}
	}

	Graphics g2(bmp);
	g2.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	g2.SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAlias);

	StringFormat format;

	g2.DrawString(str, len, fnt, RectF((bmp->GetWidth()-bound.Width)/2, (bmp->GetHeight()-bound.Height)/2, bmp->GetWidth()-maskLen, bmp->GetHeight()), 0, brush);

	if (style==eSTS_NONE)
	{
		return bmp;
	}

	if (style==eSTS_FIXED)
	{
		int len = (int)rad;

		Graphics gx(bmp);
		//gx.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		gx.SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAlias);
		//gx.DrawImage(mask, (UINT)((UINT)(bound.Width+1+len-maskLen))>>1, 0);

		if (mask)
		{
			gx.DrawImage(mask, (UINT)(bound.Width), 0);
			delete mask;
		}
		return bmp;
	}

	if (style==eSTS_GROW)
	{
		////Bitmap* bmpGlow = cyiBitmapBlender::DropShadow(bmp, 0, 0, rad, 1.0, glowcolor);
		////delete bmp;
		////return bmpGlow;
		//Bitmap* bmpGlow = cyiBitmapBlender::GlowBitmapEx(bmp, rad, level, glowcolor);//ShadowBitmapx(bmp, rad, glowcolor, level);

		//Graphics gg(bmpGlow);
		//
		////gg.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		//gg.DrawImage(bmp, (INT)rad, (INT)rad);

		//if (bIsDelete==True)
		//{
		//	delete bmp;
		//}
		//return bmpGlow;
		//HFONT hFont=FontToHFont(fnt, FW_EXTRABOLD);
		//Font xFont(

		FontFamily fam;
		fnt->GetFamily(&fam);
		StringFormat sf;
		wchar_t famliyName[128];
		int nHeight = fam.GetEmHeight(FontStyleBold);
		fam.GetFamilyName(famliyName);
		Font fontb(famliyName, fnt->GetSize(), FontStyleBold);
		
		Bitmap* bString = BitmapString(0, str, &fontb, &SolidBrush(glowcolor), eSTS_NONE);

		Graphics gret(bString);
		gret.DrawImage(bString, 1, 0);
		gret.DrawImage(bString, 0, 1);
		gret.DrawImage(bString, 1, 1);
		Bitmap* blured=cyiBitmapBlender::GaussBlureXX(bString,rad==0.0?3.0:rad, 0, 0);

		if (level!=0)
		{
			Bitmap* tra = cyiBitmapBlender::Transparent(blured, level);
			delete blured;
			blured=tra;
		}

		Graphics gtemp(blured);
		//gtemp.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		//gtemp.SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAliasGridFit);
		//gtemp.DrawString(str, len, fnt, RectF(1, 1, blured->GetWidth(), blured->GetHeight()), 0, brush);
		gtemp.DrawImage(bmp, 1, 1);

		

		delete bString;
		delete bmp;


		return blured;

	}

	if (style==eSTS_DROP_SHADOW)
	{
		Bitmap* bmpDropShadow = cyiBitmapBlender::DropShadowEx(bmp, 2, 2, 1.0, 0.3);
		delete bmp;
		return bmpDropShadow;
	}

	//if (style==eSTS_GROW)
	//{
	//	int radi=(int)rad;
	//	Bitmap* bmpTemp = new Bitmap((UINT)(bound.Width+1), (UINT)(bound.Height)+1, PixelFormat32bppARGB);
	//	Graphics gTemp(bmpTemp);
	//	gTemp.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	//	gTemp.SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAlias);
	//	gTemp.DrawString(str, len, fnt, RectF(0, 0, bmp->GetWidth(), bmp->GetHeight()), 0, &SolidBrush(Color(glowcolor)));

	//	radi=radi<<1;
	//	Bitmap* bmpRet = new Bitmap((UINT)(bound.Width+1)+radi, (UINT)(bound.Height)+1+radi, PixelFormat32bppARGB);

	//	Graphics gRet(bmpRet);

	//	radi=radi>>1;
	//	gRet.DrawImage(bmpTemp, 1, 0);
	//	gRet.DrawImage(bmpTemp, 1, 2);
	//	gRet.DrawImage(bmpTemp, 0, 1);
	//	gRet.DrawImage(bmpTemp, 2, 1);

	//	gRet.DrawImage(bmp, radi, radi);

	//	delete bmp;
	//	delete bmpTemp;
	//	
	//	return bmpRet;
	//}

	return 0;


	return 0;
}

// -------------------------------
HFONT cyiShape::FontToHFont(Font* font, int weight)
{
	LOGFONTW logFont;
	wchar_t szFontName[128];
	FontFamily fam;

	font->GetFamily(&fam);
	font->GetLogFontW(__DEFAULTGRAPHICS__, &logFont);

	fam.GetFamilyName(szFontName);

	return CreateFontW(logFont.lfHeight, 0, 0, 0, weight==0?logFont.lfWeight:weight/*FW_REGULAR*/, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                ANTIALIASED_QUALITY | PROOF_QUALITY,FIXED_PITCH | FF_MODERN,
                szFontName );

}

// -------------------------------
void* cyiShape::Clone(Font* font, float fscale, wchar_t* name)
{
	wchar_t szFontName[128];
	FontFamily fam;
	font->GetFamily(&fam);
	fam.GetFamilyName(szFontName);
	return (void*) new Font(name?name:szFontName, (font->GetSize())*fscale);
}

// -------------------------------
void cyiShape::MeasureTextSize(const wchar_t* str, Font* font, int nMaxWidth, POINT& pt)
{
	RECT rect ={0, 0, nMaxWidth, 1024};
	color_t* bmp;
	HFONT hFont = FontToHFont(font);

	HDC hdc = cyiBitmapBlender::CreateMemoryDC(rect.right, rect.bottom, &bmp);
	SelectObject(hdc, hFont);

	pt.x=0;
	pt.y=0;
	if (DrawTextW(hdc, str, -1, &rect, DT_LEFT|DT_TOP|DT_CALCRECT)==0)
	{
		cyiBitmapBlender::DestroyMemoryDC(hdc);
		DeleteObject(hFont);
		return ;
	}

	pt.x=rect.right;
	pt.y=rect.bottom;
	cyiBitmapBlender::DestroyMemoryDC(hdc);
	DeleteObject(hFont);
}



// -------------------------------
Bitmap* cyiShape::BitmapStringEx(Bitmap* dest, const wchar_t* str, Font* fnt, color_t colorFont, color_t colorBg, eShapeTextStyle style, int align, int nFixedWidth, int margin)
{
	int w=1, h=1;
	color_t* bmp;
	HDC hdc;  
	RECT rect={0, 0, nFixedWidth?nFixedWidth:1024, nFixedWidth==0?(int)fnt->GetHeight(__DEFAULTGRAPHICS__):1024};
	HFONT hFont = FontToHFont(fnt);
	Bitmap* ret=0;
	BitmapData bmpdata;
	color_t* pDest;
	
	rect.left+=margin;

	if (hFont==0)
		return 0;

	if (dest!=0)
	{
		rect.right=dest->GetWidth();
		rect.bottom=dest->GetHeight();
		goto SKIP_CALCULATE;
	}


	hdc = cyiBitmapBlender::CreateMemoryDC(w, h, &bmp);
	SelectObject(hdc, hFont);

	if (hdc==0)
	{
		if (hdc)
			cyiBitmapBlender::DestroyMemoryDC(hdc);
		if (hFont)
			DeleteObject(hFont);

		return 0;
	}

	if (DrawTextW(hdc, str, -1, &rect, /*DT_LEFT|DT_TOP|*/DT_LEFT|DT_TOP|DT_CALCRECT|DT_EDITCONTROL|DT_WORDBREAK/*|align*/)==0)
	{
		goto xERROR;
	}

	cyiBitmapBlender::DestroyMemoryDC(hdc);

	if (dest==0 && colorBg==0)
	{
		return (Bitmap*)(rect.right<<16 | rect.bottom);
	}

	if (dest==0)
	{
		dest = BITMAP32(rect.right, rect.bottom);
		cyiBitmapBlender::FillColor(dest, colorBg);
	}

SKIP_CALCULATE:

	w=dest->GetWidth();
	h=dest->GetHeight();
	hdc=cyiBitmapBlender::CreateMemoryDC(w, h, &bmp);//rect.right, rect.bottom, &bmp);
	SelectObject(hdc, hFont);

	SetTextColor(hdc, (0x00FFFFFF&colorFont));
	SetBkMode( hdc, TRANSPARENT);
	SetBkColor(hdc, (0x00FFFFFF&colorBg));

	if (style==eSTS_DROP_SHADOW)
	{
		Bitmap* bmp=cyiShape::BitmapString(0, str, fnt, &SolidBrush(Color::Black));
		Bitmap* shadow=cyiBitmapBlender::GaussBlureXX(bmp, 1.0, 0, 0);
		delete bmp;
		Bitmap* dropshdow=cyiBitmapBlender::Transparent(shadow, 0.3);
		delete shadow;

		Graphics g(dest);
		g.DrawImage(dropshdow, 0, 0);
		delete dropshdow;
	}

	//if (colorBg==0)
	//{
	//}
	//else
	//{
	//	SetBkColor(hdc, (0x00FFFFFF&colorBg));
	//}

	//ret=cyiBitmapBlender::Bitmapx(rect.right, rect.bottom, bmp);

	dest->LockBits(0,ImageLockModeRead|ImageLockModeWrite, PixelFormat32bppARGB, &bmpdata);
	memcpy(bmp, bmpdata.Scan0, (w*h)<<2);

	if (DrawTextW(hdc, str, -1, &rect, align|DT_EDITCONTROL|DT_WORDBREAK)==0)
	{
		dest->UnlockBits(&bmpdata);
		goto xERROR;
	}

	pDest=(color_t*)bmpdata.Scan0;
	int iw=0;
	for (int i=0; i<h; i++)
	{
		for (int j=margin; j<w-margin; j++)
		{
			if ((bmp[j+iw]>>24)!=0)
				continue;
			
			pDest[j+iw]=0xFF000000|bmp[j+iw];
		}
		iw+=w;
	}

	dest->UnlockBits(&bmpdata);
	cyiBitmapBlender::DestroyMemoryDC(hdc);
	DeleteObject(hFont);


	return dest;

xERROR:
	if (hdc)
		cyiBitmapBlender::DestroyMemoryDC(hdc);
	if (hFont)
		DeleteObject(hFont);

	return 0;

}

// -------------------------------
Bitmap* cyiShape::BitmapStringEx(Bitmap* dest, int x, int y, const wchar_t* str, Font* fnt, color_t colorFont, eShapeTextStyle style)
{
	if (dest==0)
		return 0;

	color_t* bmp;
	HDC hdc;  
	RECT rect={x, y, dest->GetWidth(), dest->GetHeight()};
	HFONT hFont = FontToHFont(fnt);
	Bitmap* ret=0;
	BitmapData bmpdata;
	color_t* pDest;
	int w, h;
	
	if (hFont==0)
		return 0;


	w=dest->GetWidth();
	h=dest->GetHeight();
	hdc=cyiBitmapBlender::CreateMemoryDC(w, h, &bmp);//rect.right, rect.bottom, &bmp);
	SelectObject(hdc, hFont);

	SetTextColor(hdc, (0x00FFFFFF&colorFont));
	SetBkMode( hdc, TRANSPARENT);
	SetBkColor(hdc, 0x00FFFFFFFF);

	dest->LockBits(0,ImageLockModeRead|ImageLockModeWrite, PixelFormat32bppARGB, &bmpdata);
	memcpy(bmp, bmpdata.Scan0, (w*h)<<2);

	if (DrawTextW(hdc, str, -1, &rect, DT_LEFT|DT_TOP)==0)
	{
		dest->UnlockBits(&bmpdata);
		goto xERROR;
	}

	pDest=(color_t*)bmpdata.Scan0;
	int iw=0;
	for (int i=0; i<h; i++)
	{
		for (int j=x; j<w-x; j++)
		{
			if ((bmp[j+iw]>>24)!=0)
				continue;
			
			pDest[j+iw]=0xFF000000|bmp[j+iw];
		}
		iw+=w;
	}

	dest->UnlockBits(&bmpdata);
	cyiBitmapBlender::DestroyMemoryDC(hdc);
	DeleteObject(hFont);


	return dest;

xERROR:
	if (hdc)
		cyiBitmapBlender::DestroyMemoryDC(hdc);
	if (hFont)
		DeleteObject(hFont);

	return 0;
}




// -------------------------------------------------------
Bool cyiShape::GetTextSize(Font* fnt, const wchar_t* txt, float& max_w, float& max_h)
{
	static Bitmap* bmp = new Bitmap(1, 1, PixelFormat32bppARGB);
	static Graphics* g = new Graphics(bmp);
	static Bool   bIsFirst=True;

	RectF bound;

	if (bIsFirst==True)
	{
		g->SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		g->SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAlias);
		bIsFirst=False;
	}

	max_w = max_w!=0.0 ? max_w:1920.0;
	max_h = max_h!=0.0 ? max_h:1200.0;

	int len =  wcslen(txt);
	Status ret = g->MeasureString(txt, len, fnt, RectF(0, 0, max_w, max_h), 0, &bound);

	max_w = (int)(bound.Width+1);
	max_h = (int)(bound.Height+1);

	return True;
}

// -------------------------------------------------------
Bool cyiShape::GetTextSize(Font* fnt, const wchar_t* txt, int& max_w, int& max_h)
{
	float w=(float)max_w, h=(float)max_h;

	GetTextSize(fnt, txt, w, h);
	max_w=(int)w;
	max_h=(int)h;
	return True;
}


// -------------------------------------------------------
color_t* cyiShape::BitmapToBlock(Bitmap* bmp, color_t* buffer)
{
	BitmapData data;
	int w=bmp->GetWidth();
	int h=bmp->GetHeight();
	bmp->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &data);

	color_t* color=(color_t*)data.Scan0;

	int size = (w*h)<<2;
	
	color_t* dest = (color_t*)buffer;

	if (dest==0)
	{
		dest = (color_t*)yiallocx(size);
	}

	memcpy(dest, color, size);
	bmp->UnlockBits(&data);
	return dest;

}

// -------------------------------------------------------
void cyiShape::DeleteBlock(color_t* p)
{
	yifreex(p);
}


// -------------------------------------------------------
Bitmap* cyiShape::BlockToBitmap(color_t* mem, int w, int h)
{
	Bitmap* bmp = new Bitmap(w, h, PixelFormat32bppARGB);
	BitmapData data;
	bmp->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &data);
	color_t* color=(color_t*)data.Scan0;

	memcpy(color, mem, (w*h)<<2);

	bmp->UnlockBits(&data);
	return bmp;
}

// -------------------------------------------------------
Bitmap* cyiShape::Clear(Bitmap* dest, cyiShapeMask* mask)
{
	BitmapData data;
	int w=dest->GetWidth(), h=dest->GetHeight();

	dest->LockBits(0,ImageLockModeWrite|ImageLockModeRead, PixelFormat32bppARGB, &data);
	color_t* color=(color_t*)data.Scan0;

	for (int j=0; j<h; j++)
	{
		int jw=j*w;
		for (int i=0; i<w; i++)
		{
			float level = mask->GetMaskedLevel(i, j);
			if (level==1.0)
				continue;
			else if (level==0.0)
			{
				color[i+jw]=0;
				continue;
			}
			int a =(int)((float)__COLOR_Ap__(color[i+jw])*level);
			color[i+jw]=((color[i+jw]&0x00FFFFFF)|(a<<24));
		}
	}
	dest->UnlockBits(&data);
	return dest;
}

// --------------------------------
typedef struct xsHandleShadow {
	int width;
	Bitmap* bmp[8];
} sHandleShadow;

// -------------------------------------------------------
HandleShadow cyiShape::CreateShadow(int nShadowWidth, float edge, float level, color_t color)
{
	sHandleShadow* shadow = (sHandleShadow*)yialloc(sizeof(sHandleShadow));
	int nShadowWidthx=nShadowWidth<<1;
	int w=nShadowWidthx*3;

	shadow->width=nShadowWidth;

	RectF rectFull(0, 0, w, w);
	Bitmap* pMask=cyiShape::FillRoundRect(&SolidBrush(Color(color)), rectFull, edge);
	Bitmap* pShadow = cyiBitmapBlender::ShadowBitmapx(pMask, nShadowWidth, color, level, &cyiShapeMask(cyiShape::MarginRect(Rect(0, 0, w, w), nShadowWidth, nShadowWidth), True));

	w=pShadow->GetWidth();

	Bitmap* bmpShadow;
	bmpShadow = cyiBitmapBlender::RotateCopy(0, pShadow, 0, 0, 0, 0, 0, nShadowWidthx, nShadowWidthx);
	shadow->bmp[0] = bmpShadow;
	
	bmpShadow = cyiBitmapBlender::RotateCopy(0, pShadow, 0, 0, 0, nShadowWidthx, 0, nShadowWidthx, nShadowWidthx);
	shadow->bmp[1] = bmpShadow;
	
	bmpShadow = cyiBitmapBlender::RotateCopy(0, pShadow, 0, 0, 0, w-nShadowWidthx, 0, nShadowWidthx, nShadowWidthx);
	shadow->bmp[2] = bmpShadow;

	bmpShadow = cyiBitmapBlender::RotateCopy(0, pShadow, 0, 0, 0, w-nShadowWidthx, nShadowWidthx, nShadowWidthx, nShadowWidthx);
	shadow->bmp[3] = bmpShadow;

	bmpShadow = cyiBitmapBlender::RotateCopy(0, pShadow, 0, 0, 0, w-nShadowWidthx, w-nShadowWidthx, nShadowWidthx, nShadowWidthx);
	shadow->bmp[4] = bmpShadow;

	bmpShadow = cyiBitmapBlender::RotateCopy(0, pShadow, 0, 0, 0, nShadowWidthx, w-nShadowWidthx, nShadowWidthx, nShadowWidthx);
	shadow->bmp[5] = bmpShadow;

	bmpShadow = cyiBitmapBlender::RotateCopy(0, pShadow, 0, 0, 0, 0, w-nShadowWidthx, nShadowWidthx, nShadowWidthx);
	shadow->bmp[6] = bmpShadow;

	bmpShadow = cyiBitmapBlender::RotateCopy(0, pShadow, 0, 0, 0, 0, nShadowWidthx, nShadowWidthx, nShadowWidthx);
	shadow->bmp[7] = bmpShadow;

	return (HandleShadow)shadow;
}

// -------------------------------------------------------
int cyiShape::GetShadowSize(HandleShadow h)
{
	sHandleShadow* shadow = (sHandleShadow*)h;

	if (shadow)
		return shadow->width;

	return 0;
}

// -------------------------------------------------------
void cyiShape::DestroyShadow(HandleShadow h)
{
	sHandleShadow* shadow = (sHandleShadow*)h;

	if (shadow==0)
		return ;

	for (int i=0; i<8; i++)
		delete shadow->bmp[i];
}

// -------------------------------------------------------
Bitmap* cyiShape::MakeShadowBitmap(HandleShadow hShadow,  int w, int h, Bitmap* mask)
{
	sHandleShadow* bmpz = (sHandleShadow*)hShadow;
	Bitmap* bmp = new Bitmap(w, h, PixelFormat32bppARGB);
	Graphics g(bmp);

	//g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	int nShadowSizeX = bmpz->bmp[0]->GetWidth();
	int nShadowSize	 = nShadowSizeX>>1;

	int wLen = (w-((nShadowSize)<<2));
	int hLen = (h-((nShadowSize)<<2));

	g.DrawImage(bmpz->bmp[0], (INT)0, (INT)0);
	g.DrawImage(bmpz->bmp[2], w-nShadowSizeX, 0);
	g.DrawImage(bmpz->bmp[4], w-nShadowSizeX, h-nShadowSizeX);
	g.DrawImage(bmpz->bmp[6], 0, (h-nShadowSizeX));

	Bitmap bmpTemp(nShadowSizeX, hLen);
	Graphics gTmp(&bmpTemp);
	gTmp.FillRectangle(&TextureBrush(bmpz->bmp[3]), 0, 0, nShadowSizeX, hLen);
	g.DrawImage(&bmpTemp, w-nShadowSizeX, nShadowSizeX);

	gTmp.Clear(Color(0, 0, 0, 0));
	gTmp.FillRectangle(&TextureBrush(bmpz->bmp[7]), 0, 0, nShadowSizeX, hLen);
	g.DrawImage(&bmpTemp, 0, nShadowSizeX);

	Bitmap bmpTemp2(wLen, nShadowSizeX);
	Graphics gTmp2(&bmpTemp2);
	gTmp2.FillRectangle(&TextureBrush(bmpz->bmp[5]), 0, 0, wLen, nShadowSizeX);
	g.DrawImage(&bmpTemp2, nShadowSizeX, h-nShadowSizeX);

	gTmp2.Clear(Color(0, 0, 0, 0));
	gTmp2.FillRectangle(&TextureBrush(bmpz->bmp[1]), 0, 0, wLen, nShadowSizeX);
	g.DrawImage(&bmpTemp2, nShadowSizeX, 0);

	if (mask)
	{
		cyiBitmapBlender::SetMask(bmp, mask, 0, 0, True);
	}
	return bmp;
}

// -------------------------------------------------------
Bool cyiShape::SaveBitmap(wchar_t* pwszImagePath, Bitmap* bmp, wchar_t* pwszMimeType)
{
	ImageCodecInfo* m_pEncoders;
	ImageCodecInfo* m_pSelectedCodec=0;
	UINT m_NumEncoders;

	UINT size = 0;
	Gdiplus::GetImageEncodersSize(&m_NumEncoders, &size);
	m_pEncoders = (Gdiplus::ImageCodecInfo*)yialloc(size);
	GetImageEncoders(m_NumEncoders, size, m_pEncoders);

	if (size==0)
		return False;

	if (pwszMimeType)
	{
		for(UINT i = 0; i < m_NumEncoders; ++i)
		{
			if(wcscmp(m_pEncoders[i].MimeType, pwszMimeType) == 0)
			{
				m_pSelectedCodec= &m_pEncoders[i];	// Found.
				break;
			}
		}
	}

	if (m_pSelectedCodec==0)
	{
		wchar_t ext[16];
		yiFileGetNameAndExtW(pwszImagePath, 0, 0, ext);
		
		for(UINT i = 0; i < m_NumEncoders; ++i)
		{
			if (wcsicmp(m_pEncoders[i].FilenameExtension, ext)==0)
			{
				m_pSelectedCodec= &m_pEncoders[i];	// Found.
				break;
			}
		}

	}

	if (m_pSelectedCodec==0)
	{
		yifree(m_pSelectedCodec);
		return False; // Not Found.
	}

	if (bmp->Save(pwszImagePath, &m_pSelectedCodec->Clsid)==Status::Ok)
	{
		yifree(m_pEncoders);
		return True;
	}

	yifree(m_pEncoders);
	return False;
}


// ----------------------------------
Font* cyiShape::GetFontByHeight(int height, wchar_t* name)
{
	Font font(name, height/2);
	Bitmap* bmp = BitmapString(0, L"1", &font, &SolidBrush(Color::Black));
	int n = bmp->GetHeight();
	delete bmp;

	float factor = (float)height/(float)n;
	float newFontSize = factor*(font.GetSize());

	return new Font(name, newFontSize);
}

// ----------------------------------
Bitmap* cyiShape::MakeCircleInChar(int rad, wchar_t* ch, wchar_t* fontname, color_t color, color_t color_txt)
{
	int rad_hf=rad>>1;
	Bitmap* bmp = new Bitmap(rad, rad, PixelFormat32bppARGB);

	Font* font = GetFontByHeight(rad, fontname);
	Graphics g(bmp);

	Bitmap* bmpString = BitmapString(0, ch, font, &SolidBrush(Color(color)));

	int x = bmpString->GetWidth();
	int y = bmpString->GetHeight();

	if (x%2)
		x = (x>>1)+1;
	else
		x=x>>1;

	if (y%2)
		y=(y>>1)+1;
	else
		y=y>>1;

	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	g.DrawImage(bmpString, (rad>>1)-x, (rad>>1)-y+1);
	g.DrawEllipse(&Pen(Color(color)), 0, 0, rad-1, rad-1);

	delete font;
	delete bmpString;

	return bmp;
}

/*
// GdiplusUtil.cpp

#include <comdef.h>
#include <gdiplus.h>
#include <shlwapi.h>	// for Path APIs, requires IE4.0 or later

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "shlwapi.lib")

class GdiplusUtil
{
private:
	ULONG_PTR m_Token;
	Gdiplus::GdiplusStartupInput m_StartupInput;
	UINT m_NumEncoders;
	Gdiplus::ImageCodecInfo* m_pEncoders;
public:
	// コンストラクタ.
	GdiplusUtil()
	{
		// GDI+の初期化
		Gdiplus::GdiplusStartup(&m_Token, &m_StartupInput, NULL);
		// エンコーダ情報の初期化
		UINT size = 0;
		Gdiplus::GetImageEncodersSize(&m_NumEncoders, &size);
		m_pEncoders = (Gdiplus::ImageCodecInfo*)malloc(size);
		Gdiplus::GetImageEncoders(m_NumEncoders, size, m_pEncoders);
	}
	// デストラクタ.
	~GdiplusUtil()
	{
		free(m_pEncoders);
		Gdiplus::GdiplusShutdown(m_Token);
	}
	// 名前を指定してエンコーダを取得する.
	// name : "BMP", "JPEG", ...
	Gdiplus::ImageCodecInfo*	GetEncoderByName(LPCWSTR name)
	{
		for(UINT i = 0; i < m_NumEncoders; ++i)
		{
			if(wcscmp(m_pEncoders[i].FormatDescription, name) == 0)
				return &m_pEncoders[i];	// Found.
		}
		return NULL; // Not Found.
	}
	// MIMEタイプを指定してエンコーダを取得する.
	// mimetype : "image/bmp", "image/jpeg", ...
	Gdiplus::ImageCodecInfo*	GetEncoderByMimeType(LPCWSTR mimetype)
	{
		for(UINT i = 0; i < m_NumEncoders; ++i)
		{
			if(wcscmp(m_pEncoders[i].MimeType, mimetype) == 0)
				return &m_pEncoders[i];	// Found.
		}
		return NULL; // Not Found.
	}
	// 拡張子（または拡張子付きファイル名）を指定してエンコーダを取得する.
	// extension : "bmp", "hoge.jpeg", ....
	Gdiplus::ImageCodecInfo*	GetEncoderByExtension(LPCWSTR extension)
	{
		for(UINT i = 0; i < m_NumEncoders; ++i)
		{
			// PathMatchSpecW() ユニコードバージョンはNTにしかないので、
			// 9xに対応させる場合はいったんASCIIコードに変換するか、
			// "Microsoft Layer for Unicode on Windows 95/98/Me Systems" を使用すること。
			// MSDN : http://msdn.microsoft.com/library/default.asp?url=/library/en-us/win32/unilayer_4wj7.asp
			// DL   : http://www.microsoft.com/downloads/release.asp?releaseid=30039
			if(PathMatchSpecW(extension, m_pEncoders[i].FilenameExtension))
			{
				return &m_pEncoders[i];	// Found.
			}
		}
		return NULL; // Not Found.
	}
	// てきとーな表示関数.
	void	DumpEncoders()
	{
		TCHAR buffer[1024];
		for(UINT i = 0; i < m_NumEncoders; ++i)
		{
			OutputDebugString("========================================================\n");
			wsprintf(buffer, "CodecName : %ls\n", m_pEncoders[i].CodecName);
			OutputDebugString(buffer);
			wsprintf(buffer, "DllName : %ls\n", m_pEncoders[i].DllName);
			OutputDebugString(buffer);
			wsprintf(buffer, "FormatDescription : %ls\n", m_pEncoders[i].FormatDescription);
			OutputDebugString(buffer);
			wsprintf(buffer, "FilenameExtension : %ls\n", m_pEncoders[i].FilenameExtension);
			OutputDebugString(buffer);
			wsprintf(buffer, "MimeType : %ls\n", m_pEncoders[i].MimeType);
			OutputDebugString(buffer);
			wsprintf(buffer, "Version : %d\n", m_pEncoders[i].Version);
			OutputDebugString(buffer);
		}
	}
};

// SAMPLE CODE

// GdiplusUtil グローバルインスタンスを作っておく
GdiplusUtil theGdiplus;

int main()
{
	theGdiplus.DumpEncoders();

	// 読み込みは簡単
	Gdiplus::Image image(L"sample.bmp");

	// MIMEタイプからエンコーダを取得
	image.Save(L"sample.png", &theGdiplus.GetEncoderByMimeType(L"image/png")->Clsid);

	// ファイルの拡張子にあわせたエンコードの取得
	// 見つからない場合もあるので、本当はちゃんとエラー処理もしてください
	LPCWSTR savename = L"hoge.jpeg";
	image.Save(savename, &theGdiplus.GetEncoderByExtension(savename)->Clsid);

	return 0;
}

*/