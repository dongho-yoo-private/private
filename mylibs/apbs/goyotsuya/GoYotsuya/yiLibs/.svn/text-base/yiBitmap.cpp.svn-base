#include "yiBitmap.h"

//#define CallOwnDraw(instance, function, bitmap) \
//__asm push ecx;\
//__asm mov ecx, instance;\
//__asm mov this, ecx;\
//function(bitmap)

#define lptr(data) LockBits(NULL, ImageLockModeRead, PixelFormat32bppARGB, data)

#define lptrw(data) LockBits(NULL, ImageLockModeWrite, PixelFormat32bppARGB, data)

#define uptr(data) UnlockBits(data)

//
//void __stdcall AlphaBlendingPixel(int Alpha, TARGB *Pos, TARGB *Color)
//{
//    asm{
//        movd mm6,Alpha          // mm6 = Alpha
//        punpcklwd mm6,mm6       // mm6 = 0000 0000 00Al 00Al
//        punpckldq mm6,mm6       // mm6 = 00Al 00Al 00Al 00Al
//
//        movq mm5,ALPHA_MAX_4    // mm5 = 00FF 00FF 00FF 00FF
//        psubusw mm5,mm6         // mm5 = 00FF-00Al 00FF-00Al 00FF-00Al 00FF-00Al
//
//        mov esi,Pos
//        mov edi,Color
//
//        pxor mm0,mm0            // mm0 = 0
//        movd mm1,[esi]          // mm1 = 0000 0000 PaPr PgPb
//        movd mm2,[edi]          // mm2 = 0000 0000 CaCr CgCb
//        punpcklbw mm1,mm0       // mm1 = 00Pa 00Pr 00Pg 00Pb
//        punpcklbw mm2,mm0       // mm2 = 00Ca 00Cr 00Cg 00Cb
//        pmullw mm1,mm5          // mm1 = Pa*Al Pr*Al Pg*Al Pb*Al
//        pmullw mm2,mm6          // mm2 = Ca*ZA Cr*ZA Cg*ZA Cb*ZA
//        paddusw mm2,mm1         // mm2 = mm2 - mm1
//        psrlw mm2,8                     // mm2 = mm2 >> 8
//        packuswb mm2,mm0        // mm2 = 0000 0000 BaBr BgBb
//
//        movd [esi],mm2          // Pos = mm2
//
//        emms
//    }
//}


#include "yiBitmapBlender.h"
// ------------------------------
static inline void __blt_mmx__(color_t* dest, int x, int y, int w, int h, color_t* src, int srcW, int srcH, float alpha=1.0)
{
	int _h = srcH > h ? h:srcH;
	int _w = srcW > w ? w:srcW;
	__int64 ALPHA_MAX_4 = 0x00FF00FF00FF00FF;


	if (alpha==1.0)
	{
		for (int j=0; j<_h; j++)
		{
			int wjd=(w*(j+y));
			int wjs=(j*srcW);
			for (int i=0; i<_w; i++)
			{
				UINT a = (0xFF000000&src[i+wjs])>>24;

				if (a==255)
				{
					dest[x+i+wjd] = src[i+wjs];
					continue;
				}
				else if (a==0)
				{
					continue;
				}

#ifdef _WIN64
				color_t s = src[i+wjs];
				color_t d = dest[x+i+wjd];
				
				int r = a*__COLOR_R__(s);
				int g = a*__COLOR_G__(s);
				int b = a*__COLOR_B__(s);

				int da = __COLOR_A__(d);
				int r2 = (255-a)*__COLOR_R__(d);
				int g2 = (255-a)*__COLOR_G__(d);
				int b2 = (255-a)*__COLOR_B__(d);

				dest[x+i+wjd] = __COLORp__(da, ((r+r2)>>8), ((g+g2)>>8), ((b+b2)>>8));
#endif
				

#ifndef _WIN64
				color_t* pSrc = &src[i+wjs];
				color_t* pDest = &dest[x+i+wjd];
				__asm{
					movd mm6,a				// mm6 = a
					punpcklwd mm6,mm6       // mm6 = 0000 0000 00Al 00Al
					punpckldq mm6,mm6       // mm6 = 00Al 00Al 00Al 00Al
					movq mm5,ALPHA_MAX_4    // mm5 = 00FF 00FF 00FF 00FF
					psubusw mm5,mm6				// mm5 = 00FF-00Al 00FF-00Al 00FF-00Al 00FF-00Al
					mov esi, pDest
					mov edi, pSrc
					pxor mm0,mm0            // mm0 = 0
					movd mm1,[esi]          // mm1 = 0000 0000 PaPr PgPb
					movd mm2,[edi]          // mm2 = 0000 0000 CaCr CgCb
					punpcklbw mm1,mm0       // mm1 = 00Pa 00Pr 00Pg 00Pb
					punpcklbw mm2,mm0       // mm2 = 00Ca 00Cr 00Cg 00Cb
					pmullw mm1,mm5          // mm1 = Pa*Al Pr*Al Pg*Al Pb*Al
					pmullw mm2,mm6          // mm2 = Ca*ZA Cr*ZA Cg*ZA Cb*ZA
					paddusw mm2,mm1         // mm2 = mm2 - mm1
					psrlw mm2,8                     // mm2 = mm2 >> 8
					packuswb mm2,mm0        // mm2 = 0000 0000 BaBr BgBb
					movd [esi],mm2          // pDest = mm2
					emms
				}
#endif
			}


		}
	}
}


// ------------------------------
cyiDrawObject::cyiDrawObject(eObjectType type)
{
	m_type = type;

	m_w=0;
	m_h=0;
	m_scale=1.0;
	m_bResourceDiscardable=FALSE;
}

// ------------------------------
cyiDrawObject::~cyiDrawObject(void)
{
}

// ------------------------------
eObjectType cyiDrawObject::GetType()
{
	return m_type;
}

// ------------------------------
void cyiDrawObject::GetAlignPos(float& x, float&y, eDrawAlign align)
{
	if (align==eDA_LEFT_TOP)
		return ;

	// Horizental
	if ((align & eDA_CENTER))
	{
		x = x - (GetScaledWidth()/2);
	}
	else if ((align & eDA_RIGHT))
	{
		x = x - (GetScaledWidth());
	}
	
	if ((align & eDA_VCENTER))
	{
		y = y - (GetScaledHeight()/2);
	}
	else if ((align & eDA_BOTTOM))
	{
		y = y-(GetScaledHeight());
	}

	return ;
}

// ------------------------------
void cyiDrawObject::GetAlignPos(UINT& x, UINT&y, eDrawAlign align)
{
	if (align==eDA_LEFT_TOP)
		return ;

	// Horizental
	if ((align & eDA_CENTER))
	{
		x = x - (GetWidth()/2);
	}
	else if ((align & eDA_RIGHT))
	{
		x = x - (GetWidth());
	}
	
	if ((align & eDA_VCENTER))
	{
		y = y - (GetHeight()/2);
	}
	else if ((align & eDA_BOTTOM))
	{
		y = y-(GetHeight());
	}

	return ;
}

// -----------------------------------
cyiBitmap::~cyiBitmap(void)
{
}

// -----------------------------------
cyiBitmap::cyiBitmap(const wchar_t* pszFileName)
: cyiDrawObject(eOT_BITMAP)
{
	cyiBitmap(&Image(pszFileName));
}

// -----------------------------------
cyiBitmap::cyiBitmap(Image* pImage)
: cyiDrawObject(eOT_BITMAP)
{
	m_w = pImage->GetWidth();
	m_h = pImage->GetHeight();
	m_bmp = new Bitmap(m_w, m_h, PixelFormat32bppARGB);

	Graphics g(m_bmp);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	g.DrawImage(pImage, 0, 0);
	m_class_instance=NULL;
	m_draw_fnc=NULL;
	m_scaled_bmp=NULL;
	
}

#define CallOwnDraw(instance, drawfunc, bitmap) \
{\
	void* p = instance;\
	yifBmpOwnerDraw fnc = drawfunc;\
	Bitmap* bmp=bitmap;\
	__asm {\
		push ecx\
		mov  ecx, bmp\
		push ecx\
		mov  ecx, p\
		push edx\
		mov  edx, fnc\
		call edx\
		pop  ecx\
	};\
}

#define CallDIBOwnDraw(instance, drawfunc, bitmap, w, h) \
{\
	void* p = instance;\
	yifDIBmpOwnerDraw fnc = drawfunc;\
	color_t* bmp=bitmap;\
	UINT _w = w;\
	UINT _h = h;\
	__asm push ecx; \
	__asm mov  ecx, bmp; \
	__asm push ecx; \
	__asm mov  ecx, _w; \
	__asm push ecx; \
	__asm mov  ecx, _h; \
	__asm push ecx; \
	__asm mov  ecx, p; \
	__asm push edx; \
	__asm mov  edx, fnc; \
	__asm call edx; \
	__asm pop  edx; \
	__asm pop  ecx; \
	__asm pop  ecx; \
	__asm pop  ecx; \
	};\
}

// -----------------------------------
// new ccyiBitmap(w, h, class pointer or NULL, yifBmpOwnerDraw);
cyiBitmap::cyiBitmap(UINT w, UINT h, ...)
: cyiDrawObject(eOT_BITMAP)
{
	void** pArgv = (void**)&h;
	m_class_instance=pArgv[1];
	m_draw_fnc = (yifBmpOwnerDraw)pArgv[2];
	m_bmp = new Bitmap(w, h, PixelFormat32bppARGB);

	m_w=w;
	m_h=h;
	m_scaled_bmp=NULL;

	//CallOwnDraw(m_class_instance, m_draw_fnc, m_bmp);

	if (m_class_instance)
	{
		void* p = m_class_instance;
		yifBmpOwnerDraw fnc = m_draw_fnc;
		Bitmap* bmp=m_bmp;
#ifndef _WIN64
		__asm {
			push ecx
			mov  ecx, bmp
			push ecx
			mov  ecx, p
			mov  edx, fnc
			call edx
			pop  ecx
		};
#endif
	}
}

// -----------------------------------
void cyiBitmap::SetScale(float scale, BOOL bRedraw)
{
	if (bRedraw==TRUE 
		&& m_class_instance)
	{
		delete m_bmp;

		UINT w = (UINT)__INTEGER__(m_w*scale);
		UINT h = (UINT)__INTEGER__(m_h*scale);

		m_bmp = new Bitmap(w, h, PixelFormat32bppARGB);
		m_scaled_bmp = m_bmp;
		m_scale=scale;

		//CallOwnDraw(m_class_instance, m_draw_fnc, m_bmp);
		if (m_class_instance)
		{
			void* p = m_class_instance;
			yifBmpOwnerDraw fnc = m_draw_fnc;
			Bitmap* bmp=m_bmp;
#ifndef _WIN64
			__asm {
				push ecx
				mov  ecx, bmp
				push ecx
				mov  ecx, p
				mov  edx, fnc
				call edx
				pop  ecx
			};
#endif
		}
		return ;
	}

	if (m_scaled_bmp!=NULL)
		delete m_scaled_bmp;

	m_scale = scale;
	float w = scale*(float)m_w;
	float h = scale*(float)m_h;

	m_scaled_bmp = new Bitmap((UINT)__INTEGER__(w), (UINT)__INTEGER__(h));

	Graphics g(m_scaled_bmp);
	g.SetSmoothingMode(SmoothingModeHighQuality);

	g.DrawImage(m_bmp, (float)0.0, (float)0.0, w, h);
}

// ------------------------------
BOOL cyiBitmap::Draw(Graphics* pGr, float x, float y, eDrawAlign align)
{
	GetAlignPos(x, y, align);
	pGr->DrawImage(m_scaled_bmp?m_scaled_bmp:m_bmp, (float)(UINT)x, (float)(UINT)y);

	return TRUE;
}

// ------------------------------
BOOL cyiBitmap::Draw(cyiDIBitmap* bmp, float x, float y, eDrawAlign align)
{
	GetAlignPos(x, y, align);

	Bitmap* p = m_scaled_bmp ? m_scaled_bmp : m_bmp;
	BitmapData data;
	p->lptr(&data);
	__blt_mmx__((color_t*)*bmp, (int)x, (int)y, bmp->GetWidth(), bmp->GetHeight(), (color_t*)data.Scan0, p->GetWidth(), p->GetHeight());
	p->uptr(&data);
	return TRUE;
}


// -------------------------------------
static void __call_dib_own_draw__(void* p, yifDIBmpOwnerDraw fnc, color_t* bmp, UINT w, UINT h)
{
#ifndef _WIN64
	__asm push ecx;	
	__asm push edx;
	__asm mov  ecx, bmp;
	__asm push ecx; 
	__asm mov  ecx, w; 
	__asm push ecx; 
	__asm mov  ecx, h; 
	__asm push ecx; 
	__asm mov  ecx, p;	// this
	__asm mov  edx, fnc; 
	__asm call edx; 
	__asm pop  edx;		// any value
	__asm pop  ecx;		// _h
	__asm pop  ecx;		// _w
	__asm pop  ecx;		// _bmp
	__asm pop  edx;		// restore edx
	__asm pop  ecx;		// restore ecx
#endif
}

// -----------------------------------
cyiDIBitmap::cyiDIBitmap(UINT w, UINT h, ...)
: cyiDrawObject(eOT_DIBITMAP)
{
	if (Create(w, h)==TRUE)
	{
		void** pArgv = (void**)&h;
		m_class_instance=pArgv[1];
		m_func = (yifDIBmpOwnerDraw)pArgv[2];
		__call_dib_own_draw__(m_class_instance, m_func, m_bmp, w, h);
	}

}


cyiDIBitmap::cyiDIBitmap(UINT w, UINT h, color_t* data)
: cyiDrawObject(eOT_DIBITMAP)
{
	if (Create(w, h)==TRUE)
	{
		if (data)
		{
			memcpy(m_bmp, data, (w*h)<<2);
		}
	}
}

// -----------------------------------
cyiDIBitmap::cyiDIBitmap(cyiBitmap* pBmp)
: cyiDrawObject(eOT_DIBITMAP)
{
	Import(*pBmp);
	m_func=0;
}

// -----------------------------------
cyiDIBitmap::cyiDIBitmap(Bitmap* pBmp)
: cyiDrawObject(eOT_DIBITMAP)
{
	Import(pBmp);
	m_func=0;
}

// -----------------------------------
cyiDIBitmap::~cyiDIBitmap()
{
}


// -----------------------------------
HBITMAP cyiDIBitmap::GetBitmap()
{
	return m_hBmp;
}

// -----------------------------------
BOOL cyiDIBitmap::Draw(HDC hdc, UINT x, UINT y, eDrawAlign align)
{
	GetAlignPos(x, y, align);
	UINT w = GetWidth();
	UINT h = GetHeight();
	::BitBlt(hdc, x, y, w, h, m_hdc, 0, 0, SRCCOPY);
	return TRUE;
}

// -----------------------------------
BOOL cyiDIBitmap::Draw(HDC hdc, UINT x, UINT y, UINT w, UINT h, eDrawAlign align)
{
	GetAlignPos(x, y, align);
	::BitBlt(hdc, x, y, w, h, m_hdc, 0, 0, SRCCOPY);
	return TRUE;
}

// -----------------------------------
BOOL cyiDIBitmap::DrawAlpha(HDC hdc, UINT x, UINT y,  eDrawAlign align/*=eDA_LEFT_TOP*/,  UINT alpha/*=255*/)
{
	GetAlignPos(x, y, align);
	UINT w = GetWidth();
	UINT h = GetHeight();

	BLENDFUNCTION fnc;
	fnc.AlphaFormat=AC_SRC_ALPHA;
	fnc.BlendFlags=0;
	fnc.BlendOp=AC_SRC_OVER;
	fnc.SourceConstantAlpha=alpha;

	//::AlphaBlend(hdc, x, y, w, h, m_hdc, 0, 0, w, h,  fnc);

	return TRUE;
}

// -----------------------------------
void cyiDIBitmap::SetScale(float scale, BOOL bReduced/*=FALSE unusedd*/)
{
	m_scale=scale;
	m_w=(UINT)__INTEGER__((scale*(float)m_w));
	m_h=(UINT)__INTEGER__((scale*(float)m_h));

	__call_dib_own_draw__(m_class_instance, m_func, m_bmp, m_w, m_h);
}

// -----------------------------------
BOOL cyiDIBitmap::Import(Bitmap* bmp)
{
	m_w = bmp->GetWidth();
	m_h = bmp->GetHeight();

	if (Create(m_w, m_h)==FALSE)
	{
		return FALSE;
	}

	UINT w=bmp->GetWidth();
	UINT h=bmp->GetHeight();

	BitmapData data;
	bmp->LockBits(NULL, ImageLockModeRead, PixelFormat32bppARGB, &data);
	color_t* colors = (color_t*)data.Scan0;

	if (colors==0)
	{
		return  FALSE;
	}
	
	memcpy(m_bmp, colors, (m_w*m_h)<<2);

	bmp->UnlockBits(&data);
	return TRUE;
}

// -----------------------------------
Bitmap* cyiDIBitmap::Export(Bitmap* bmp)
{
	if (bmp==NULL)
	{
		bmp=new Bitmap(m_w, m_h, PixelFormat32bppARGB);
	}

	UINT w=bmp->GetWidth();
	UINT h=bmp->GetHeight();

	BitmapData data;
	bmp->LockBits(NULL, ImageLockModeWrite, PixelFormat32bppARGB, &data);
	color_t* colors = (color_t*)data.Scan0;

	if (colors==0)
	{
		return FALSE;
	}

	memcpy(colors, m_bmp, (m_w*m_h)<<2);
	bmp->UnlockBits(&data);
	return bmp;
}

// -----------------------------------
Bitmap* cyiDIBitmap::Export(Bitmap* bmp, int x, int y, int xSrc, int ySrc, int w, int h)
{
	//if (bmp==NULL)
	//{
	//	return 0;
	//}

	if (bmp==0)
	{
		bmp=BITMAP32(w!=0?w:m_w,h!=0?h:m_h);
	}

	UINT dw=bmp->GetWidth();
	UINT dh=bmp->GetHeight();

	//h=dh<h?dh:h;
	//w=dw<w?dw:w;

	h=m_h<h?m_h:h;
	w=m_w<w?m_w:w;

	BitmapData data;
	bmp->LockBits(NULL, ImageLockModeWrite, PixelFormat32bppARGB, &data);
	color_t* colors = (color_t*)data.Scan0;

	int iw=ySrc*m_w;
	int diw=dw*y;
	int di=0;

	int yUntil=(ySrc+h)>dh?dh:ySrc+h;
	int xUntil=(xSrc+w)>dw?dw:xSrc+w;
	for (int j=ySrc; j<yUntil; j++)
	{
		for (int i=xSrc; i<xSrc+w; i++)
		{
			colors[x+di+diw] = m_bmp[i+iw];
			di++;
		}
		di=0;
		iw+=m_w;
		diw+=dw;
	}

	bmp->UnlockBits(&data);
	return bmp;
}

// -----------------------------------
BOOL cyiDIBitmap::Create(UINT w, UINT h)
{
	BITMAPV4HEADER hdv4;

	memset(&hdv4, 0, sizeof(hdv4));
	hdv4.bV4Size = sizeof(BITMAPV4HEADER);
	hdv4.bV4Width = w;
	hdv4.bV4Height = -((int)h);
	hdv4.bV4Planes = 1;
	hdv4.bV4BitCount = 32;
	hdv4.bV4V4Compression = BI_RGB;
	hdv4.bV4SizeImage = 0;
	hdv4.bV4XPelsPerMeter = 0;
	hdv4.bV4YPelsPerMeter = 0;
	hdv4.bV4ClrUsed = 0;
	hdv4.bV4ClrImportant = 0;
	hdv4.bV4RedMask = 0x00FF0000;
	hdv4.bV4GreenMask = 0x0000FF00;
	hdv4.bV4BlueMask = 0x000000FF;
	hdv4.bV4AlphaMask = 0xFF000000;

	m_hBmp = ::CreateDIBSection(NULL, (BITMAPINFO*)&hdv4, DIB_RGB_COLORS, (void**)&m_bmp,NULL, 0);

#ifdef __WINDOWS200__
	GdiFlush();
#endif

	if (m_hBmp==NULL)
	{
		return FALSE;
	}

	m_hdc = ::CreateCompatibleDC(NULL);
	if (m_hdc==NULL)
	{
		return FALSE;
	}

	::SelectObject(m_hdc, m_hBmp);

	m_w=w;
	m_h=h;
	m_scale=1.0;

	return TRUE;
}

// -----------------------------------
void cyiDIBitmap::Destroy()
{
	if (m_hBmp)
		DeleteObject(m_hBmp);
	if (m_hdc)
		DeleteDC(m_hdc);
	m_hBmp=NULL;
	m_hdc=NULL;
}

// -----------------------------------
cyiSurface::cyiSurface(UINT w, UINT h)
:cyiDIBitmap(w, h, NULL)
{

}

// -----------------------------------
cyiSurface::~cyiSurface()
{
	Destroy();
}

// -----------------------------------
//BOOL cyiSurface::Create(UINT w, UINT h)
//{
//	return cyiDIBitmap::Create(w, h);
//}


// -----------------------------------
void cyiSurface::SetScale(float scale, BOOL bReduced)
{
	m_w=(UINT)__INTEGER__((scale*(float)m_w));
	m_h=(UINT)__INTEGER__((scale*(float)m_h));
	m_scale=1.0;
	Create(m_w, m_h);
}

// -----------------------------------
void cyiSurface::Resize(UINT w, UINT h)
{
	Destroy();
	Create(w, h);
}


// ------------------------------
void cyiSurface::blt(int x, int y, Bitmap* bmp, eDrawAlign align)
{
	GetAlignPos((UINT&)x, (UINT&)y, align);

	BitmapData data;
	bmp->lptrw(&data);

//#ifndef _WIN64
	__blt_mmx__((color_t*)m_bmp, (int)x, (int)y, m_w, m_h, (color_t*)data.Scan0, bmp->GetWidth(), bmp->GetHeight());
//#else
//#endif

	bmp->uptr(&data);

}

//
//// -----------------------------------
void cyiSurface::blt(UINT x, UINT y, Bitmap* bmp, UINT xSrc, UINT ySrc, UINT w, UINT h)
{
	BitmapData data;


	bmp->lptr(&data);

	if (x==0 && y==0 && xSrc==0 && ySrc==0 && w==0 && h==0)
	{
		memcpy(m_bmp, data.Scan0, (m_w*m_h)<<2);
	}
	else
	{
		UINT _srcWidth=bmp->GetWidth();
		color_t* src = (color_t*)data.Scan0;

		for (UINT j=0; j<h; j++)
		{
			int jw = (y+j)*m_w;
			int jws = (j+ySrc)*_srcWidth;
			for (UINT i=0; i<w; i++)
			{
				m_bmp[x+i+jw] = src[xSrc+i+jws];
			}
		}

	}

	bmp->uptr(&data);
}

// -----------------------------------
void cyiSurface::Copy(int x, int y, cyiSurface* sf, int destx, int desty, int w, int h)
{
	color_t* dest = (color_t*)sf->m_bmp;

	for (int j=0; j<h; j++)
	{
		int jw=m_w*(j+y);
		int jwd=sf->m_w*(j+desty);
		for (int i=0; i<w; i++)
		{
			dest[jwd+i+destx] = m_bmp[x+i+jw];
		}
	}
}

// -----------------------------------
Bitmap* cyiSurface::ToBitmap(Bitmap* dest, UINT x, UINT y)
{
	int w = dest->GetWidth();
	int h = dest->GetHeight();

	BitmapData data;


	dest->lptrw(&data);

	if (y==0 && x==0 && w==m_w && h==m_h)
	{
		memcpy(data.Scan0, m_bmp, (m_w*m_h)<<2);
	}
	else
	{
		UINT _srcWidth=dest->GetWidth();
		color_t* dest = (color_t*)data.Scan0;

		for (int j=0; j<h; j++)
		{
			int jw = (y+j)*m_w;
			int jwd = j*w;
			for (int i=0; i<w; i++)
			{
				dest[i+jwd] = m_bmp[x+i+jw];
			}
		}

	}

	dest->uptr(&data);

	return dest;
}

// -----------------------------------
void cyiSurface::blt(UINT x, UINT y, cyiDIBitmap* bmp, UINT xSrc, UINT ySrc, UINT w, UINT h)
{
	if (w==0)
		w = bmp->GetWidth();

	if (h==0)
		h=bmp->GetHeight();

	::BitBlt(m_hdc, x, y, w, h, bmp->m_hdc, xSrc, ySrc, SRCCOPY);
}


// -----------------------------------
static inline  color_t blend(color_t src, color_t dest)
{
	return dest;
}

// -----------------------------------
void cyiSurface::blt_alpha(UINT x, UINT y, Bitmap* bmp, UINT xSrc, UINT ySrc, UINT w, UINT h)
{
	BitmapData data;

	bmp->lptr(&data);

	UINT _x = (UINT)x;
	UINT _y = (UINT)y;
	UINT _srcWidth=bmp->GetWidth();
	color_t* src = (color_t*)data.Scan0;

	if (w==0)
		w = bmp->GetWidth();

	if (h==0)
		h=bmp->GetHeight();

	for (UINT i=0; i<w; i++)
	{
		for (UINT j=0; j<h; j++)
		{
			m_bmp[x+i+(y+j)*m_w] = blend(m_bmp[x+i+(y+j)*m_w], src[xSrc+i+(j+ySrc)*_srcWidth]);
		}
	}

	bmp->uptr(&data);
}

// -----------------------------------
void cyiSurface::blt_alpha(UINT x, UINT y, cyiDIBitmap* bmp, UINT xSrc, UINT ySrc, UINT w, UINT h)
{
	BLENDFUNCTION fnc;
	fnc.AlphaFormat=AC_SRC_ALPHA;
	fnc.BlendFlags=0;
	fnc.BlendOp=AC_SRC_OVER;
	fnc.SourceConstantAlpha=255;

	if (w==0)
		w = bmp->GetWidth();

	if (h==0)
		h=bmp->GetHeight();


	//::AlphaBlend(m_hdc, x, y, m_w, m_h, bmp->m_hdc, xSrc, ySrc, w, h,  fnc);
}

// -----------------------------------
color_t* cyiSurface::GetRaw(int&x, int&y)
{
	x=m_w;
	y=m_h;
	return m_bmp;
}


Bitmap* cyiSurface::export_bmp(UINT x, UINT y, UINT w, UINT h)
{
	Graphics g(m_hdc);
	Bitmap* bmp = new Bitmap(w, h, PixelFormat32bppARGB);

	BitmapData data;
	bmp->lptrw(&data);
	color_t* dest = (color_t*)data.Scan0;

	for (UINT j=0; j<h; j++)
	{
		int jw=m_w*(j+y);
		int jwd=w*j;
		for (UINT i=0; i<w; i++)
		{
			dest[jwd+i] = m_bmp[x+i+jw];
		}
	}
	bmp->uptr(&data);
	return bmp;

}
