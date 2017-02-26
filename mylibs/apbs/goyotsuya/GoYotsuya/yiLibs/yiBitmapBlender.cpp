#include "yistd.h"

#include "yiBitmapBlender.h"
#include "yiShape.h"
#include <math.h>


// -------------------------
typedef struct xsGdTable {
	UINT rad;
	float mask[16][16];

} sBlurMask;

// -------------------------
typedef struct xsGdTablex {
	UINT rad;
	unsigned int mask[16][16];

} sBlurMaskx;

// -------------------------
typedef struct xsGdTablexx {
	UINT rad;
	unsigned int mask[16];

} sBlurMaskxx;


// -------------------------
typedef sBlurMaskxx sBlurMaskInt;


typedef struct xcolorf_t {
	color_t color;
	float mask;
	float full_mask;
	float alpha_mask;
} colorf_t;

typedef struct xcolorx_t {
	color_t color;
	unsigned int mask;
	unsigned int full_mask;
	unsigned int alpha_mask;
} colorx_t;

typedef sBlurMask sGdTable;

typedef sBlurMaskx sGdTablex;

typedef sBlurMaskxx sGdTablexx;

// ----------------
extern int GetGaussDistributionTable(float rad, sGdTable* table);

// ----------------
extern int GetGaussDistributionTablex(float rad, sGdTablex* table);

// ----------------
extern int GetGaussDistributionTablexx(float rad, sGdTablexx* table);

cyiBitmapBlender::cyiBitmapBlender(void)
{
}

cyiBitmapBlender::cyiBitmapBlender(HDC hdc, UINT w, UINT h)
{
	HBITMAP hBitmap = ::CreateCompatibleBitmap(hdc, w, h);
	m_hBitmap = (HBITMAP)::SelectObject(hdc, hBitmap);
	::GetObject(m_hBitmap, sizeof(BITMAP), &m_bitmap);
	m_hdc = hdc;
}

cyiBitmapBlender::cyiBitmapBlender(HBITMAP hBitmap)
{
	//HBITMAP hBitmap = ::CreateCompatibleBitmap(hdc, w, h);
	m_hBitmap = hBitmap; //(HBITMAP)::SelectObject(hdc, hBitmap);
	::GetObject(m_hBitmap, sizeof(BITMAP), &m_bitmap);
	//m_hdc = hdc;
}


cyiBitmapBlender::~cyiBitmapBlender(void)
{
}



// ------------------------------------------------------
static color_t GetAveragleColor(color_t* colors, int n)
{
	UINT sum_a=0;
	UINT sum_r=0;
	UINT sum_g=0;
	UINT sum_b=0;

	int cnt=0;

	for (int i=0; i<n; i++)
	{
		UINT a = __COLOR_Ap__(colors[i]);

		if (a==0)
		{
			continue;
		}

		sum_a+=a;
		sum_r+=__COLOR_Rp__(colors[i]);
		sum_g+=__COLOR_Gp__(colors[i]);
		sum_b+=__COLOR_Bp__(colors[i]);
		cnt++;
	}

	if (cnt==0)
		return 0;

	return __COLORp__(sum_a/n, sum_r/cnt, sum_g/cnt, sum_b/cnt);
}

#define NEAR_ZERO(n) (n<0.5)

// ------------------------------------------------------
static color_t GetAveragleColorf(colorf_t* colors, int n)
{
	float sum_a=0.0;
	float sum_r=0.0;
	float sum_g=0.0;
	float sum_b=0.0;

	for (int i=0; i<n; i++)
	{
		if (__COLOR_Ap__(colors[i].color)==0)
			continue;

		sum_a+=((float)__COLOR_Ap__(colors[i].color))*colors[i].alpha_mask;
		sum_r+=((float)__COLOR_Rp__(colors[i].color))*colors[i].full_mask;
		sum_g+=((float)__COLOR_Gp__(colors[i].color))*colors[i].full_mask;
		sum_b+=((float)__COLOR_Bp__(colors[i].color))*colors[i].full_mask;
	}

	return __COLORp__((UINT)(sum_a), (UINT)(sum_r), (UINT)(sum_g), (UINT)(sum_b));
}


// ------------------------------------------------------
static color_t GetAveragleColorxx(colorx_t* colors, int n, Bool bIsNoTransparent )
{
	unsigned int sum_a=0;
	unsigned int sum_r=0;
	unsigned int sum_g=0;
	unsigned int sum_b=0;

	if (n==0)
		return 0;

	if (bIsNoTransparent==True)
	{
		for (int i=0; i<n; i++)
		{
			sum_a+=(__COLOR_Ap__(colors[i].color))*colors[i].alpha_mask;
			//sum_r+=(__COLOR_Rp__(colors[i].color))*colors[i].full_mask;
			//sum_g+=(__COLOR_Gp__(colors[i].color))*colors[i].full_mask;
			//sum_b+=(__COLOR_Bp__(colors[i].color))*colors[i].full_mask;		
		}
		return ((sum_a>>12)<<24);//0xFF000000;//__COLORp__(255, (sum_r>>12), (sum_g>>12), (sum_b>>12));
	}

	for (int i=0; i<n; i++)
	{
		if (__COLOR_Ap__(colors[i].color)==0)
			continue;

		sum_a+=(__COLOR_Ap__(colors[i].color))*colors[i].alpha_mask;
		//sum_r+=(__COLOR_Rp__(colors[i].color))*colors[i].full_mask;
		//sum_g+=(__COLOR_Gp__(colors[i].color))*colors[i].full_mask;
		//sum_b+=(__COLOR_Bp__(colors[i].color))*colors[i].full_mask;
	}

	return ((sum_a>>12)<<24);//__COLORp__((UINT)(sum_a), (UINT)(sum_r), (UINT)(sum_g), (UINT)(sum_b));
}

// ------------------------------------------------------
static color_t GetAveragleColorx(colorx_t* colors, int n, Bool bIsNoTransparent, Bool bIsEdge=False )
{
	unsigned int sum_a=0;
	unsigned int sum_r=0;
	unsigned int sum_g=0;
	unsigned int sum_b=0;

	if (bIsEdge==True)
	{
		for (int i=0; i<n; i++)
		{
			if (__COLOR_Ap__(colors[i].color)==0)
				continue;

			register unsigned int nFullMask = colors[i].full_mask;
			register unsigned int xColor=colors[i].color;

			sum_a+=(__COLOR_Ap__(xColor))*nFullMask;
			sum_r+=(__COLOR_Rp__(xColor))*nFullMask;
			sum_g+=(__COLOR_Gp__(xColor))*nFullMask;
			sum_b+=(__COLOR_Bp__(xColor))*nFullMask;
		}

		return __COLORp__((UINT)(sum_a>>12), (UINT)(sum_r>>12), (UINT)(sum_g>>12), (UINT)(sum_b>>12));
	}

	//if (bIsNoTransparent==True)
	//{
	//	for (int i=0; i<n; i++)
	//	{
	//		//sum_a+=(__COLOR_Ap__(colors[i].color))*colors[i].alpha_mask;
	//		sum_r+=(__COLOR_Rp__(colors[i].color))*colors[i].full_mask;
	//		sum_g+=(__COLOR_Gp__(colors[i].color))*colors[i].full_mask;
	//		sum_b+=(__COLOR_Bp__(colors[i].color))*colors[i].full_mask;		
	//	}
	//	return __COLORp__(255, (sum_r>>12), (sum_g>>12), (sum_b>>12));
	//}

	for (int i=0; i<n; i++)
	{
		if (__COLOR_Ap__(colors[i].color)==0)
			continue;
		register unsigned int nFullMask = colors[i].full_mask;
		register unsigned int xColor=colors[i].color;
		sum_a+=(__COLOR_Ap__(xColor))*colors[i].alpha_mask;
		sum_r+=(__COLOR_Rp__(xColor))*nFullMask;
		sum_g+=(__COLOR_Gp__(xColor))*nFullMask;
		sum_b+=(__COLOR_Bp__(xColor))*nFullMask;
	}

	return __COLORp__((UINT)(sum_a>>12), (UINT)(sum_r>>12), (UINT)(sum_g>>12), (UINT)(sum_b>>12));
}

#define BITMAP32_COLOR(p, i, j, w) (p[i+w*j])

// ------------------------------------------------------
static int GetColorz(color_t* colors, UINT rad, int x, int y, int w, int h, color_t* pSrc)
{
	int cnt=0;
	//float factor;
	for (int i=x-rad; i<(int)(x+rad+1); i++)
	{
		if (i<0 || i>=w)
		{
			colors[cnt++]=0;
			continue;
		}

		for (int j=y-rad; j<(int)(y+rad+1); j++)
		{
			if (j<0 || j>=h)
			{
				colors[cnt++]=0;
				continue;
			}
			colors[cnt++] = BITMAP32_COLOR(pSrc, i, j, w);
		}
	}

	return cnt;
}



//typedef float (**fmask[6][6]);
//
// ------------------------------------------------------
static void MakeBlurMask(UINT rad, sBlurMask* out_mask )
{
	const float _mask[6][6] = {
		{100, 84, 47,  17, 3,  0.39},
		{84, 65,  32,  11, 2,  0.1},
		{47,  32,  20,  6, 1,  0.05},
		{17,   11,  6,  3.7,  0.7,  0.025},
		{3,   2,    1,  0.7,     0.35,  0.0125},
		{0.39,    0.1,   0.05,  0.025, 0.0125,  0}
	};

	out_mask->rad = rad;

	int irad=(int)rad;
	float mask_sum=0.0;

	for (int i=-irad; i<(irad+1); i++)
		for (int j=-irad; j<(irad+1); j++)
			mask_sum+=_mask[::abs(i)][::abs(j)];

	for (int i=0; i<(irad+1); i++)
		for (int j=0; j<(irad+1); j++)
			out_mask->mask[i][j] = _mask[i][j]/mask_sum;
}

#define MAX_BLOCK_CNT 32
// ------------------------------------------------------
static int GetColorzf(colorf_t* colors, sBlurMask* mask, int x, int y, int w, int h, color_t* pSrc, int mode=0)
{
	//color_t buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];

	float mask_sum=0.0;
	float alpha_mask_sum=0.0;
	int rad = (int)mask->rad;
	int cnt=0;
	//float factor;
	BOOL bHasTransparentColor=FALSE;
	for (int i=x-rad; i<(int)(x+rad+1); i++)
	{
		if (i<0 || i>=w)
		{
			continue;
		}

		for (int j=y-rad; j<(int)(y+rad+1); j++)
		{
			if (j<0 || j>=h)
			{
				continue;
			}

			color_t color = BITMAP32_COLOR(pSrc, i, j, w);

			int _i = abs(i-(int)x);
			int _j = abs(j-(int)y);

			//buffer[cnt]=color;

			if (__COLOR_Ap__(color)==0)
			{
				bHasTransparentColor=TRUE;
				//continue;
				//color=0x00FFFFFF;
			}
			else
			{
				mask_sum+=mask->mask[_i][_j];
			}

			colors[cnt].color=color;
			colors[cnt].mask=mask->mask[_i][_j];
			colors[cnt].alpha_mask = mask->mask[_i][_j];
			colors[cnt].full_mask=mask->mask[_i][_j];
			alpha_mask_sum+=mask->mask[_i][_j];
			cnt++;
		}
	}

	if (bHasTransparentColor==TRUE ||
				cnt!=((mask->rad<<1)+1)*((mask->rad<<1)+1))
	{
		for (int i=0; i<cnt; i++)
		{
			if (__COLOR_Ap__(colors[i].color)!=0)
			{
				colors[i].full_mask = colors[i].mask/mask_sum;
			}
			colors[i].alpha_mask = colors[i].mask/alpha_mask_sum;
		}
	}

	return cnt;
}

// ------------------------------------------------------
static int GetColorzxFast(colorx_t* colors, sBlurMaskx* mask, int x, int y, int w, int h, color_t* pSrc, Bool* bIsNotTransparent=0)
{
	//color_t buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];

	unsigned int mask_sum=0;
	unsigned int alpha_mask_sum=0;
	int rad = (int)mask->rad;
	int cnt=0;
	int xUntil=x+rad+1;
	int yUntil=y+rad+1;

	BOOL bHasTransparentColor=FALSE;


	for (int j=y-rad; j<(int)(yUntil); j++)	
	{

		cnt++;
		if (j<0 || j>=h)
		{
			continue;
		}

		for (int i=x-rad; i<(int)xUntil; i++)
		{
			if (i<0 || i>=w)
			{
				continue;
			}

			color_t color = BITMAP32_COLOR(pSrc, i, j, w);

			int _i = abs(i-(int)x);
			int _j = abs(j-(int)y);

			//buffer[cnt]=color;

			if (__COLOR_Ap__(color)==0)
			{
				bHasTransparentColor=TRUE;
				//continue;
				//color=0x00FFFFFF;
			}
			else
			{
				mask_sum+=mask->mask[_i][_j];
			}

			colors[cnt].color=color;
			colors[cnt].mask=mask->mask[_i][_j];
			colors[cnt].alpha_mask = mask->mask[_i][_j];
			colors[cnt].full_mask=mask->mask[_i][_j];
			alpha_mask_sum+=mask->mask[_i][_j];
			cnt++;
		}
	}

	*bIsNotTransparent = bHasTransparentColor?False:True;

	if (bHasTransparentColor==TRUE ||
				cnt!=((mask->rad<<1)+1)*((mask->rad<<1)+1))
	{
		for (int i=0; i<cnt; i++)
		{
			if (__COLOR_Ap__(colors[i].color)!=0)
			{
				colors[i].full_mask = colors[i].mask/mask_sum;
			}
			colors[i].alpha_mask = colors[i].mask/alpha_mask_sum;
		}
	}

	return cnt;

}

// ------------------------------------------------------
static int GetColorzx(colorx_t* colors, sBlurMaskx* mask, int x, int y, int w, int h, color_t* pSrc, Bool* bIsNotTransparent=0, Bool* bHasEdge=0)
{
	//color_t buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];

	unsigned int mask_sum=0;
	unsigned int alpha_mask_sum=0;
	int rad = (int)mask->rad;
	int cnt=0;
	int xUntil=x+rad+1;
	int yUntil=y+rad+1;

	BOOL bHasTransparentColor=FALSE;

	for (int j=y-rad; j<(int)(yUntil); j++)	
	{
		if (j<0 || j>=h)
		{
			continue;
		}


		for (int i=x-rad; i<(int)xUntil; i++)
		{
			if (i<0 || i>=w)
			{
				continue;
			}

			color_t color = BITMAP32_COLOR(pSrc, i, j, w);

			int _i = abs(i-(int)x);
			int _j = abs(j-(int)y);

			//buffer[cnt]=color;

			if (__COLOR_Ap__(color)==0)
			{
				bHasTransparentColor=TRUE;
				//continue;
				//color=0x00FFFFFF;
			}
			else
			{
				mask_sum+=mask->mask[_i][_j];
			}

			colors[cnt].color=color;
			colors[cnt].mask=mask->mask[_i][_j];
			colors[cnt].alpha_mask = mask->mask[_i][_j];
			colors[cnt].full_mask=mask->mask[_i][_j];
			alpha_mask_sum+=mask->mask[_i][_j];
			cnt++;
		}
	}

	*bIsNotTransparent = bHasTransparentColor?False:True;

	if (bHasTransparentColor==TRUE ||
				cnt!=((mask->rad<<1)+1)*((mask->rad<<1)+1))
	{
		for (int i=0; i<cnt; i++)
		{
			if (__COLOR_Ap__(colors[i].color)!=0)
			{
				colors[i].full_mask = colors[i].mask/mask_sum;
			}
			colors[i].alpha_mask = colors[i].mask/alpha_mask_sum;
		}

	}

	return cnt;
}

// ------------------------------------------------------
static int GetColorzxH_old(colorx_t* colors, sBlurMaskxx* mask, int x, int y, int w, int h, color_t* pSrc, Bool* bIsNotTransparent=0, Bool* bHasEdge=0)
{
	//color_t buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];

	unsigned int mask_sum=0;
	unsigned int alpha_mask_sum=0;
	int rad = (int)mask->rad;
	int cnt=0;
	int xUntil=x+rad+1;
	int yw = y*w;
	BOOL bHasTransparentColor=FALSE;

	if (y<0 ||
			y>=h)
	{
		return 0;
	}

	for (int i=x-rad; i<(int)xUntil; i++)
	{
		if (i<0 || i>=w)
		{
			continue;
		}

		color_t color = pSrc[i+yw];//BITMAP32_COLOR(pSrc, i, y, w);

		int _i = abs(i-(int)x);

		//buffer[cnt]=color;

		if (__COLOR_Ap__(color)==0)
		{
			bHasTransparentColor=TRUE;
			//continue;
			//color=0x00FFFFFF;
		}
		else
		{
			mask_sum+=mask->mask[_i];
		}

		// かなり無駄な処理
		colors[cnt].color=color;
		colors[cnt].mask=mask->mask[_i];
		colors[cnt].alpha_mask = mask->mask[_i];
		colors[cnt].full_mask=mask->mask[_i];
		alpha_mask_sum+=mask->mask[_i];
		cnt++;
	}

	*bIsNotTransparent = bHasTransparentColor?False:True;

	if (bHasTransparentColor==TRUE ||
				cnt!=((mask->rad<<1)+1))
	{
		for (int i=0; i<cnt; i++)
		{
			if (__COLOR_Ap__(colors[i].color)!=0)
			{
				colors[i].full_mask = (colors[i].mask<<12)/mask_sum;
			}
			colors[i].alpha_mask = (colors[i].mask<<12)/alpha_mask_sum;
		}
		if (bHasTransparentColor==FALSE)
		{
			if (bHasEdge)
				*bHasEdge=True;
		}
	}

	return cnt;
}

// ------------------------------------------------------
static int GetColorzxV_old(colorx_t* colors, sBlurMaskxx* mask, int x, int y, int w, int h, color_t* pSrc, Bool* bIsNotTransparent=0, Bool* bHasEdge=0)
{
	//color_t buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];

	unsigned int mask_sum=0;
	unsigned int alpha_mask_sum=0;
	int rad = (int)mask->rad;
	int cnt=0;
	int yUntil=y+rad+1;
	int wj = (y-rad)*w+x;
	BOOL bHasTransparentColor=FALSE;

	if (x<0 ||
			x>=w)
		return 0;

	for (int j=y-rad; j<(int)yUntil; j++)
	{
		if (j<0 || j>=h)
		{
			wj+=w;
			continue;
		}

		color_t color = pSrc[wj];//BITMAP32_COLOR(pSrc, x, j, w);
		wj+=w;

		int _j = abs(j-(int)y);

		//buffer[cnt]=color;

		if (__COLOR_Ap__(color)==0)
		{
			bHasTransparentColor=TRUE;
			//continue;
			//color=0x00FFFFFF;
		}
		else
		{
			mask_sum+=mask->mask[_j];
		}

		// かなり無駄な処理
		colors[cnt].color=color;
		colors[cnt].mask=mask->mask[_j];
		colors[cnt].alpha_mask = mask->mask[_j];
		colors[cnt].full_mask=mask->mask[_j];
		alpha_mask_sum+=mask->mask[_j];
		cnt++;
	}

	*bIsNotTransparent = bHasTransparentColor?False:True;

	if (bHasTransparentColor==TRUE ||
				cnt!=((mask->rad<<1)+1))
	{
		for (int i=0; i<cnt; i++)
		{
			if (__COLOR_Ap__(colors[i].color)!=0)
			{
				colors[i].full_mask = (colors[i].mask<<12)/mask_sum;
			}
			colors[i].alpha_mask = (colors[i].mask<<12)/alpha_mask_sum;
		}
		if (bHasTransparentColor==FALSE)
		{
			if (bHasEdge)
				*bHasEdge=True;
		}
	}

	return cnt;
}

// ------------------------------------------------------
static int GetColorzxH(colorx_t* colors, sBlurMaskxx* mask, int x, int y, int w, int h, color_t* pSrc, Bool* bIsNotTransparent=0, Bool* bHasEdge=0, Bool* bIsSame=0)
{
	//color_t buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];

	register unsigned int mask_sum=0;
	register unsigned int alpha_mask_sum=0;
	int rad = (int)mask->rad;
	int cnt=0;
	int xUntil=x+rad+1;
	int yw = y*w;
	BOOL bHasTransparentColor=FALSE;

	if (y<0 ||
			y>=h)
	{
		return 0;
	}

	color_t orgColor=pSrc[yw+x];
	*bIsSame=True;
	for (int i=x-rad; i<(int)xUntil; i++)
	{
		if (i<0 || i>=w)
		{
			continue;
		}

		if (orgColor!= pSrc[i+yw])
		{
			*bIsSame=False;
			break;
		}
	}

	if (*bIsSame==True)
	{
		return 0;
	}

	for (int i=x-rad; i<(int)xUntil; i++)
	{
		if (i<0 || i>=w)
		{
			continue;
		}

		color_t color = pSrc[i+yw];//BITMAP32_COLOR(pSrc, i, y, w);

		int _i = abs(i-(int)x);

		//buffer[cnt]=color;

		if (__COLOR_Ap__(color)==0)
		{
			bHasTransparentColor=TRUE;
			//continue;
			//color=0x00FFFFFF;
		}
		else
		{
			mask_sum+=mask->mask[_i];
		}

		// かなり無駄な処理
		colors[cnt].color=color;
		colors[cnt].mask=mask->mask[_i];
		colors[cnt].alpha_mask = mask->mask[_i];
		colors[cnt].full_mask=mask->mask[_i];
		alpha_mask_sum+=mask->mask[_i];
		cnt++;
	}

	*bIsNotTransparent = bHasTransparentColor?False:True;

	if (bHasTransparentColor==TRUE ||
				cnt!=((mask->rad<<1)+1))
	{
		for (int i=0; i<cnt; i++)
		{
			if (__COLOR_Ap__(colors[i].color)!=0)
			{
				colors[i].full_mask = (colors[i].mask<<12)/mask_sum;
			}
			colors[i].alpha_mask = (colors[i].mask<<12)/alpha_mask_sum;
		}
		if (bHasTransparentColor==FALSE)
		{
			if (bHasEdge)
				*bHasEdge=True;
		}
	}

	return cnt;
}


// ------------------------------------------------------
static int GetColorzxV(colorx_t* colors, sBlurMaskxx* mask, int x, int y, int w, int h, color_t* pSrc, Bool* bIsNotTransparent=0, Bool* bHasEdge=0, Bool* bIsSame=0)
{
	//color_t buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];

	unsigned int mask_sum=0;
	unsigned int alpha_mask_sum=0;
	int rad = (int)mask->rad;
	int cnt=0;
	int yUntil=y+rad+1;
	int wj = (y-rad)*w+x;
	BOOL bHasTransparentColor=FALSE;

	if (x<0 ||
			x>=w)
		return 0;

	color_t orgColor=pSrc[w*y+x];
	*bIsSame=True;
	int n=wj;
	for (int j=y-rad; j<(int)yUntil; j++)
	{
		if (j<0 || j>=h)
		{
			n+=w;
			continue;
		}

		if (orgColor!= pSrc[n])
		{
			*bIsSame=False;
			break;
		}
		n+=w;
	}

	if (*bIsSame==True)
	{
		return 0;
	}

	for (int j=y-rad; j<(int)yUntil; j++)
	{
		if (j<0 || j>=h)
		{
			wj+=w;
			continue;
		}

		color_t color = pSrc[wj];//BITMAP32_COLOR(pSrc, x, j, w);
		wj+=w;

		int _j = abs(j-(int)y);

		//buffer[cnt]=color;

		if (__COLOR_Ap__(color)==0)
		{
			bHasTransparentColor=TRUE;
			//continue;
			//color=0x00FFFFFF;
		}
		else
		{
			mask_sum+=mask->mask[_j];
		}

		// かなり無駄な処理
		colors[cnt].color=color;
		colors[cnt].mask=mask->mask[_j];
		colors[cnt].alpha_mask = mask->mask[_j];
		colors[cnt].full_mask=mask->mask[_j];
		alpha_mask_sum+=mask->mask[_j];
		cnt++;
	}

	*bIsNotTransparent = bHasTransparentColor?False:True;

	if (bHasTransparentColor==TRUE ||
				cnt!=((mask->rad<<1)+1))
	{
		for (int i=0; i<cnt; i++)
		{
			if (__COLOR_Ap__(colors[i].color)!=0)
			{
				colors[i].full_mask = (colors[i].mask<<12)/mask_sum;
			}
			colors[i].alpha_mask = (colors[i].mask<<12)/alpha_mask_sum;
		}
		if (bHasTransparentColor==FALSE)
		{
			if (bHasEdge)
				*bHasEdge=True;
		}
	}

	return cnt;
}

// ------------------------------------------------------
static int GetColorzxH_i(colorx_t* colors, sBlurMaskxx* mask, int x, int y, int w, int h, color_t* pSrc, Bool* bIsNotTransparent=0, Bool* bHasEdge=0)
{
	//color_t buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];

	unsigned int mask_sum=0;
	unsigned int alpha_mask_sum=0;
	int rad = (int)mask->rad;
	int cnt=0;
	int xUntil=x+rad+1;
	int yw = y*w;
	BOOL bHasTransparentColor=FALSE;

	if (y<0 ||
			y>=h)
	{
		return 0;
	}

	for (int i=x-rad; i<(int)xUntil; i++)
	{
		if (i<0 || i>=w)
		{
			int _i = abs(i-(int)x);
			bHasTransparentColor=TRUE;
			colors[cnt].color=0;
			colors[cnt].mask=mask->mask[_i];
			colors[cnt].alpha_mask = mask->mask[_i];
			colors[cnt].full_mask=mask->mask[_i];
			alpha_mask_sum+=mask->mask[_i];
			cnt++;
			continue;
		}

		color_t color = pSrc[i+yw];//BITMAP32_COLOR(pSrc, i, y, w);

		int _i = abs(i-(int)x);

		//buffer[cnt]=color;

		if (__COLOR_Ap__(color)==0)
		{
			bHasTransparentColor=TRUE;
			//continue;
			//color=0x00FFFFFF;
		}
		else
		{
			mask_sum+=mask->mask[_i];
		}

		// かなり無駄な処理
		colors[cnt].color=color;
		colors[cnt].mask=mask->mask[_i];
		colors[cnt].alpha_mask = mask->mask[_i];
		colors[cnt].full_mask=mask->mask[_i];
		alpha_mask_sum+=mask->mask[_i];
		cnt++;
	}

	*bIsNotTransparent = bHasTransparentColor?False:True;

	if (bHasTransparentColor==TRUE ||
				cnt!=((mask->rad<<1)+1))
	{
		for (int i=0; i<cnt; i++)
		{
			if (__COLOR_Ap__(colors[i].color)!=0)
			{
				colors[i].full_mask = (colors[i].mask<<12)/mask_sum;
			}
			colors[i].alpha_mask = (colors[i].mask<<12)/alpha_mask_sum;
		}
		//if (bHasTransparentColor==FALSE)
		//{
		//	if (bHasEdge)
		//		*bHasEdge=True;
		//}
	}

	return cnt;
}


// ------------------------------------------------------
static int GetColorzxV_i(colorx_t* colors, sBlurMaskxx* mask, int x, int y, int w, int h, color_t* pSrc, Bool* bIsNotTransparent=0, Bool* bHasEdge=0)
{
	//color_t buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];

	unsigned int mask_sum=0;
	unsigned int alpha_mask_sum=0;
	int rad = (int)mask->rad;
	int cnt=0;
	int yUntil=y+rad+1;
	int wj = (y-rad)*w+x;
	BOOL bHasTransparentColor=FALSE;

	if (x<0 ||
			x>=w)
	{
		return 0;
	}

	for (int j=y-rad; j<(int)yUntil; j++)
	{
		if (j<0 || j>=h)
		{
			wj+=w;
			int _j = abs(j-(int)y);
			bHasTransparentColor=TRUE;
			colors[cnt].color=0;
			colors[cnt].mask=mask->mask[_j];
			colors[cnt].alpha_mask = mask->mask[_j];
			colors[cnt].full_mask=mask->mask[_j];
			alpha_mask_sum+=mask->mask[_j];
			cnt++;
			continue;
		}

		color_t color = pSrc[wj];//BITMAP32_COLOR(pSrc, x, j, w);
		wj+=w;

		int _j = abs(j-(int)y);

		//buffer[cnt]=color;

		if (__COLOR_Ap__(color)==0)
		{
			bHasTransparentColor=TRUE;
			//continue;
			//color=0x00FFFFFF;
		}
		else
		{
			mask_sum+=mask->mask[_j];
		}

		// かなり無駄な処理
		colors[cnt].color=color;
		colors[cnt].mask=mask->mask[_j];
		colors[cnt].alpha_mask = mask->mask[_j];
		colors[cnt].full_mask=mask->mask[_j];
		alpha_mask_sum+=mask->mask[_j];
		cnt++;
	}

	*bIsNotTransparent = bHasTransparentColor?False:True;

	if (bHasTransparentColor==TRUE ||
				cnt!=((mask->rad<<1)+1))
	{
		for (int i=0; i<cnt; i++)
		{
			if (__COLOR_Ap__(colors[i].color)!=0)
			{
				colors[i].full_mask = (colors[i].mask<<12)/mask_sum;
			}
			colors[i].alpha_mask = (colors[i].mask<<12)/alpha_mask_sum;
		}
		//if (bHasTransparentColor==FALSE)
		//{
		//	if (bHasEdge)
		//		*bHasEdge=True;
		//}
	}

	return cnt;
}




// ------------------------------------------------------
static int GetColorzf_d(colorf_t* colors, sBlurMask* mask, int x, int y, int w, int h, color_t* pSrc, int mode=0)
{
	//color_t buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];

	float mask_sum=0.0;
	float alpha_mask_sum=0.0;
	int rad = (int)mask->rad;
	int cnt=0;
	//float factor;
	BOOL bHasTransparentColor=FALSE;
	for (int i=x-rad; i<(int)(x+rad+1); i++)
	{
		for (int j=y-rad; j<(int)(y+rad+1); j++)
		{
			color_t color = 0;
			if (i<0 || i>=w ||
					j<0 || j>=h)
			{
			}
			else
			{
				color = BITMAP32_COLOR(pSrc, i, j, w);
			}

			int _i = abs(i-(int)x);
			int _j = abs(j-(int)y);

			//buffer[cnt]=color;

			if (__COLOR_Ap__(color)==0)
			{
				bHasTransparentColor=TRUE;
				//continue;
				//color=0x00FFFFFF;
			}
			else
			{
				mask_sum+=mask->mask[_i][_j];
			}

			colors[cnt].color=color;
			colors[cnt].mask=mask->mask[_i][_j];
			colors[cnt].alpha_mask = mask->mask[_i][_j];
			colors[cnt].full_mask=mask->mask[_i][_j];
			alpha_mask_sum+=mask->mask[_i][_j];
			cnt++;
		}
	}

	if (bHasTransparentColor==TRUE ||
				cnt!=((mask->rad<<1)+1)*((mask->rad<<1)+1))
	{
		for (int i=0; i<cnt; i++)
		{
			if (__COLOR_Ap__(colors[i].color)!=0)
			{
				colors[i].full_mask = colors[i].mask/mask_sum;
			}
			colors[i].alpha_mask = colors[i].mask/alpha_mask_sum;
		}
	}

	return cnt;
}

// ------------------------------------------------------
static int GetColorzx_d(colorx_t* colors, sBlurMaskx* mask, int x, int y, int w, int h, color_t* pSrc, Bool*  IsTrans)
{
	//color_t buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];

	unsigned int  mask_sum=0.0;
	unsigned int alpha_mask_sum=0.0;
	int rad = (int)mask->rad;
	int cnt=0;

	BOOL bHasTransparentColor=FALSE;
	for (int i=x-rad; i<(int)(x+rad+1); i++)
	{
		for (int j=y-rad; j<(int)(y+rad+1); j++)
		{
			color_t color = 0;
			if (i<0 || i>=w ||
					j<0 || j>=h)
			{
			}
			else
			{
				color = BITMAP32_COLOR(pSrc, i, j, w);
			}

			int _i = abs(i-(int)x);
			int _j = abs(j-(int)y);

			//buffer[cnt]=color;

			if (__COLOR_Ap__(color)==0)
			{
				bHasTransparentColor=TRUE;
				//continue;
				//color=0x00FFFFFF;
			}
			else
			{
				mask_sum+=mask->mask[_i][_j];
			}

			colors[cnt].color=color;
			colors[cnt].mask=mask->mask[_i][_j];
			colors[cnt].alpha_mask = mask->mask[_i][_j];
			colors[cnt].full_mask=mask->mask[_i][_j];
			alpha_mask_sum+=mask->mask[_i][_j];
			cnt++;
		}
	}

	*IsTrans = 1-bHasTransparentColor;

	if (bHasTransparentColor==TRUE ||
				cnt!=((mask->rad<<1)+1)*((mask->rad<<1)+1))
	{
		for (int i=0; i<cnt; i++)
		{
			if (__COLOR_Ap__(colors[i].color)!=0)
			{
				colors[i].full_mask = colors[i].mask/mask_sum;
			}
			colors[i].alpha_mask = colors[i].mask/alpha_mask_sum;
		}
	}

	return cnt;
}


// ------------------------------------------------------
Bitmap* cyiBitmapBlender::Blure(Bitmap* src, UINT rad, UINT transparent, cyiShapeMask* mask)
{
		UINT w = src->GetWidth();
		UINT h = src->GetHeight();

		if (rad>MAX_BLOCK_CNT)
		{
			rad=MAX_BLOCK_CNT;
		}

		UINT nBlockCnt = (rad<<1)+1;

		Bitmap* dest = new Bitmap(w, h, PixelFormat32bppARGB);

		BitmapData srcdata, destdata;
		src->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &srcdata);
		dest->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &destdata);

		color_t* pSrc = (color_t*)srcdata.Scan0;
		color_t* pDest = (color_t*)destdata.Scan0;
		color_t  buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];

		if (mask!=0)
		{
			for (UINT i=0; i<w; i++)
			{
				for (UINT j=0; j<h; j++)
				{
					if (mask->IsMasked(i, j)==False)
					{
						pDest[i+w*j] = 0;
						continue;
					}

					int n = GetColorz(buffer, rad, i, j, w, h, pSrc);
					color_t color = GetAveragleColor(buffer, n);

					pDest[i+w*j] = color;
				}
			}
		}
		else
		{
			for (UINT i=0; i<w; i++)
			{
				for (UINT j=0; j<h; j++)
				{
					int n = GetColorz(buffer, rad, i, j, w, h, pSrc);
					color_t color = GetAveragleColor(buffer, n);

					pDest[i+w*j] = color;
				}
			}
		}

		src->UnlockBits(&srcdata);
		dest->UnlockBits(&destdata);

		return dest;
}

#include "yiTime.h"
// ------------------------------------------------------
Bitmap* cyiBitmapBlender::GaussBlure(Bitmap* src, float rad, UINT transparent, cyiShapeMask* pBound)
{
		UINT w = src->GetWidth();
		UINT h = src->GetHeight();

		if (rad>MAX_BLOCK_CNT)
		{
			rad=MAX_BLOCK_CNT;
		}

		Bitmap* dest = new Bitmap(w, h, PixelFormat32bppARGB);

		BitmapData srcdata, destdata;
		src->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &srcdata);
		dest->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &destdata);

		color_t* pSrc = (color_t*)srcdata.Scan0;
		color_t* pDest = (color_t*)destdata.Scan0;
		colorf_t  buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];

		sBlurMask mask;
		//MakeBlurMask(rad, &mask);

		if (GetGaussDistributionTable(rad, &mask)!=0)
		{
			delete dest;
			return 0;
		}

		if (pBound)
		{
			for (UINT i=0; i<w; i++)
			{
				for (UINT j=0; j<h; j++)
				{
					if (pBound->IsMasked(i, j)==FALSE)
						continue;

					int n = GetColorzf(buffer, &mask, i, j, w, h, pSrc);
					color_t color = GetAveragleColorf(buffer, n);

					pDest[i+w*j] = color;
				}
			}
		}
		else
		{
			for (UINT i=0; i<w; i++)
			{
				for (UINT j=0; j<h; j++)
				{
					int n=GetColorzf(buffer, &mask, i, j, w, h, pSrc);
					color_t color = GetAveragleColorf(buffer, n);

					pDest[i+w*j] = color;
				}
			}
		}
		src->UnlockBits(&srcdata);
		dest->UnlockBits(&destdata);

		return dest;
}

// ------------------------------------------------------
Bitmap* cyiBitmapBlender::GaussBlureX(Bitmap* src, float rad, UINT transparent, cyiShapeMask* pBound)
{
		UINT w = src->GetWidth();
		UINT h = src->GetHeight();

		if (rad>MAX_BLOCK_CNT)
		{
			rad=MAX_BLOCK_CNT;
		}

		Bitmap* dest = new Bitmap(w, h, PixelFormat32bppARGB);

		BitmapData srcdata, destdata;
		src->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &srcdata);
		dest->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &destdata);

		color_t* pSrc = (color_t*)srcdata.Scan0;
		color_t* pDest = (color_t*)destdata.Scan0;
		colorx_t  buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];

		sBlurMaskx mask;
		//MakeBlurMask(rad, &mask);

		if (GetGaussDistributionTablex(rad, &mask)!=0)
		{
			delete dest;
			return 0;
		}

		if (pBound)
		{
			for (UINT j=0; j<h; j++)
			{
				UINT wj=j*w;
				for (UINT i=0; i<w; i++)
				{
					if (pBound->IsMasked(i, j)==FALSE)
					{
						//pDest[i+w*j]=pSrc[i+w*j];
						continue;
					}

					Bool bIsTrue=False;
					Bool bHasEdge=False;
					int n = GetColorzx(buffer, &mask, i, j, w, h, pSrc, &bIsTrue, &bHasEdge);
					color_t color = GetAveragleColorx(buffer, n, bIsTrue, bHasEdge);

					pDest[i+wj] = color;
				}
			}
		}
		else
		{
			for (UINT j=0; j<h; j++)
			{
				UINT wj=j*w;
				for (UINT i=0; i<w; i++)
				{
					Bool bIsTrue=False;
					int n = GetColorzx(buffer, &mask, i, j, w, h, pSrc, &bIsTrue);

					color_t color = GetAveragleColorx(buffer, n, bIsTrue);


					pDest[i+wj] = color;
				}
			}
		}
		src->UnlockBits(&srcdata);
		dest->UnlockBits(&destdata);

		return dest;
}

// ------------------------------------------------------
Bitmap* cyiBitmapBlender::GaussBlureXX(Bitmap* src, float rad, UINT transparent, cyiShapeMask* pBound)
{
	UINT w = src->GetWidth();
	UINT h = src->GetHeight();


	if (rad==0.0)
	{
		return cyiBitmapBlender::BitmapCopy(0, src);
	}

	if (rad>MAX_BLOCK_CNT)
	{
		rad=MAX_BLOCK_CNT;
	}

	Bitmap* dest = new Bitmap(w, h, PixelFormat32bppARGB);
	Bitmap* dest2 = new Bitmap(w, h, PixelFormat32bppARGB);

	BitmapData srcdata, destdata, destdata2;
	src->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &srcdata);
	dest->LockBits(0,ImageLockModeRead|ImageLockModeWrite, PixelFormat32bppARGB, &destdata);
	dest2->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &destdata2);

	color_t* pSrc = (color_t*)srcdata.Scan0;
	color_t* pDest = (color_t*)destdata.Scan0;
	color_t* pDest2 = (color_t*)destdata2.Scan0;

	colorx_t  buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];

	sBlurMaskxx mask;
	//MakeBlurMask(rad, &mask);

	if (GetGaussDistributionTablexx(rad, &mask)!=0)
	{
		delete dest;
		return 0;
	}

	if (pBound)
	{
		UINT wj=0;
		for (UINT j=0; j<h; j++)
		{
			for (UINT i=0; i<w; i++)
			{
				if (pBound->IsMasked(i, j)==FALSE)
				{
					pDest[i+wj]=pSrc[i+wj];
					continue;
				}

				Bool bIsTrue=False;
				Bool bHasEdge=False;
				Bool bIsSame;

				int n = GetColorzxH(buffer, &mask, i, j, w, h, pSrc, &bIsTrue, &bHasEdge, &bIsSame);
				if (bIsSame==False)
				{
					color_t color = GetAveragleColorx(buffer, n, bIsTrue, bHasEdge);
					pDest[i+wj] = color;
				}
				else
					pDest[i+wj] = pSrc[i+wj];
			}
			wj+=w;
		}


		wj=0;
		for (UINT j=0; j<h; j++)
		{
			for (UINT i=0; i<w; i++)
			{
				if (pBound->IsMasked(i, j)==FALSE)
				{
					pDest2[i+wj]=pDest[i+wj];
					continue;
				}

				Bool bIsTrue=False;
				Bool bHasEdge=False;
				Bool bIsSame;

				int n = GetColorzxV(buffer, &mask, i, j, w, h, pDest, &bIsTrue,  &bHasEdge, &bIsSame);
				if (bIsSame==False)
				{
					color_t color = GetAveragleColorx(buffer, n, bIsTrue, bHasEdge);
					pDest2[i+wj] = color;
				}
				else
					pDest2[i+wj]=pDest[i+wj];
				
			}
			wj+=w;
		}
	}
	else
	{
		UINT wj=0;
		for (UINT j=0; j<h; j++)
		{

			for (UINT i=0; i<w; i++)
			{
				Bool bIsTrue=False;
				Bool bHasEdge=False;
				Bool bIsSame;

				int n = GetColorzxV(buffer, &mask, i, j, w, h, pSrc, &bIsTrue, &bHasEdge, &bIsSame);

				if (bIsSame==False)
				{
					color_t color = GetAveragleColorx(buffer, n, bIsTrue, bHasEdge);
					pDest[i+wj] = color;
				}
				else
					pDest[i+wj]=pSrc[i+wj];
			}
			wj+=w;
		}

		wj=0;
		for (UINT j=0; j<h; j++)
		{
			for (UINT i=0; i<w; i++)
			{
				Bool bIsTrue=False;
				Bool bHasEdge=False;
				Bool bIsSame=False;
				
				int n = GetColorzxH(buffer, &mask, i, j, w, h, pDest, &bIsTrue, &bHasEdge, &bIsSame);

				if (bIsSame==False)
				{
					color_t color = GetAveragleColorx(buffer, n, bIsTrue, bHasEdge);
					pDest2[i+wj] = color;
				}
				else
					pDest2[i+wj]=pDest[i+wj];
			}
			wj+=w;
		}
	}
	src->UnlockBits(&srcdata);
	dest->UnlockBits(&destdata);
	dest2->UnlockBits(&destdata2);
	delete dest;

	return dest2;
}

// ------------------------------------------------------
Bitmap* cyiBitmapBlender::GaussBlureEx(Bitmap* src, float rad, Bool* stopped)
{
	UINT w = src->GetWidth();
	UINT h = src->GetHeight();
	BitmapData srcdata;
	src->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &srcdata);
	color_t* pSrc = (color_t*)srcdata.Scan0;

	Bitmap* ret = GaussBlureEx(pSrc, w, h, rad, stopped);

	src->UnlockBits(&srcdata);

	return ret;
}

typedef struct {
	uIntPtr param[16];
}sThreadParam;

// ------------------------------------------------------
unsigned int __stdcall xOnGaussBlureH(void* param)
{
	sThreadParam*s = (sThreadParam*)param;

	color_t* dest	=(color_t*)s->param[0];
	color_t* src	=(color_t*)s->param[1];
	int w			= s->param[2];
	int from		= s->param[3];
	int to			= s->param[4];
	sBlurMaskxx* mask = (sBlurMaskxx*)s->param[5];
	Bool* stopped		= (Bool*)s->param[6];
	int h			= s->param[7];

	colorx_t  buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];

	UINT wj=from*w;
	for (UINT j=from; j<to; j++)
	{
		for (UINT i=0; i<w; i++)
		{
			Bool bIsTrue=False;
			Bool bHasEdge=False;
			Bool bIsSame=False;
			
			if (stopped!=0 &&
					*stopped==True)
			{
				yifree(s);
				_endthreadex(0);
				return 0;
			}

			int n = GetColorzxH(buffer, mask, i, j, w, h, src, &bIsTrue, &bHasEdge, &bIsSame);

			if (bIsSame==False)
			{
				color_t color = GetAveragleColorx(buffer, n, bIsTrue, bHasEdge);
				dest[i+wj] = color;
			}
			else
				dest[i+wj]=src[i+wj];
		}
		wj+=w;
	}

	yifree(s);
	_endthreadex(0);

	return 0;
}

// ------------------------------------------------------
unsigned int __stdcall xOnGaussBlureV(void* param)
{
	sThreadParam*s = (sThreadParam*)param;

	color_t* dest	=(color_t*)s->param[0];
	color_t* src	=(color_t*)s->param[1];
	int w			= s->param[2];
	int from		= s->param[3];
	int to			= s->param[4];
	sBlurMaskxx* mask = (sBlurMaskxx*)s->param[5];
	Bool* stopped		= (Bool*)s->param[6];
	int h			= s->param[7];

	colorx_t  buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];

	UINT wj=0;
	for (UINT j=0; j<h; j++)
	{
		for (UINT i=from; i<to; i++)
		{
			Bool bIsTrue=False;
			Bool bHasEdge=False;
			Bool bIsSame=False;
			
			if (stopped!=0 &&
					*stopped==True)
			{
				yifree(s);
				_endthreadex(0);
				return 0;
			}

			int n = GetColorzxV(buffer, mask, i, j, w, h, src, &bIsTrue, &bHasEdge, &bIsSame);

			if (bIsSame==False)
			{
				color_t color = GetAveragleColorx(buffer, n, bIsTrue, bHasEdge);
				dest[i+wj] = color;
			}
			else
				dest[i+wj]=src[i+wj];
		}
		wj+=w;
	}

	yifree(s);
	_endthreadex(0);

	return 0;
}

// ------------------------------------------------------
static HANDLE xGaussBlureH(color_t* dest, color_t* src, int width, int height, int from, int to, sBlurMaskxx* mask, Bool* stopped)
{
	sThreadParam param = {(uIntPtr)dest, (uIntPtr)src, (uIntPtr)width, (uIntPtr)from, (uIntPtr)to, (uIntPtr)mask, (uIntPtr)stopped, (uIntPtr)height};
	unsigned int id;
	return (HANDLE)_beginthreadex(0, 0, xOnGaussBlureH, yitoheap(&param, sizeof(param)), 0, &id);
}

// ------------------------------------------------------
static HANDLE xGaussBlureV(color_t* dest, color_t* src, int width, int height, int from, int to, sBlurMaskxx* mask, Bool* stopped)
{
	sThreadParam param = {(uIntPtr)dest, (uIntPtr)src, (uIntPtr)width, (uIntPtr)from, (uIntPtr)to, (uIntPtr)mask, (uIntPtr)stopped, (uIntPtr)height};
	unsigned int id;
	return (HANDLE)_beginthreadex(0, 0, xOnGaussBlureV, yitoheap(&param, sizeof(param)), 0, &id);
}

// ------------------------------------------------------
Bitmap* cyiBitmapBlender::GaussBlureEx(void* raw, int w, int h, float rad, Bool* stopped)
{
	int nCpuCnt=yiUtilGetCpuCount();
	Bitmap* dest = new Bitmap(w, h, PixelFormat32bppARGB);
	Bitmap* dest2 = new Bitmap(w, h, PixelFormat32bppARGB);

	BitmapData destdata, destdata2;
	dest->LockBits(0,ImageLockModeRead|ImageLockModeWrite, PixelFormat32bppARGB, &destdata);
	dest2->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &destdata2);

	color_t* pSrc = (color_t*)raw;
	color_t* pDest = (color_t*)destdata.Scan0;
	color_t* pDest2 = (color_t*)destdata2.Scan0;

	colorx_t  buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];
	UINT wj=0;

	sBlurMaskxx mask;
	//MakeBlurMask(rad, &mask);

	if (rad>MAX_BLOCK_CNT)
	{
		rad=MAX_BLOCK_CNT;
	}

	if (GetGaussDistributionTablexx(rad, &mask)!=0)
	{
		delete dest;
		return 0;
	}

	if (nCpuCnt==0 ||
			nCpuCnt==1)
		goto DIRECT;

	if (w*h<10000)
		goto DIRECT;

	if (w<nCpuCnt ||
			h<nCpuCnt)
		goto DIRECT;

	if (nCpuCnt>16)
		nCpuCnt=16;

	HANDLE hThread[16]={0,};
	int sum=0;
	int val = h/nCpuCnt;
	int nRemainCnt=h%nCpuCnt;
	
	for (int i=0; i<nCpuCnt; i++)
	{
		hThread[i]=xGaussBlureH(pDest, pSrc, w, h, sum, sum+val+nRemainCnt, &mask, stopped);
		sum+=val+nRemainCnt;
		if (i==0)
			nRemainCnt=0;
	}

	for (int i=0; i<nCpuCnt; i++)
	{
		WaitForSingleObject(hThread[i], INFINITE);
		CloseHandle(hThread[i]);
	}

	if (stopped!=0 &&
			*stopped==True)
	{
		dest->UnlockBits(&destdata);
		dest2->UnlockBits(&destdata2);
		delete dest;
		return 0;
	}

	sum=0;
	nRemainCnt=w%nCpuCnt;
	val = w/nCpuCnt;

	for (int i=0; i<nCpuCnt; i++)
	{
		hThread[i]=xGaussBlureV(pDest2, pDest, w, h, sum, sum+nRemainCnt+val, &mask, stopped);
		sum+=val+nRemainCnt;
		if (i==0)
			nRemainCnt=0;
	}

	for (int i=0; i<nCpuCnt; i++)
	{
		WaitForSingleObject(hThread[i], INFINITE);
		CloseHandle(hThread[i]);
	}

	goto RET;

DIRECT:
	for (UINT j=0; j<h; j++)
	{
		for (UINT i=0; i<w; i++)
		{
			Bool bIsTrue=False;
			Bool bHasEdge=False;
			Bool bIsSame;

			if (stopped)
			{
				if (*stopped==True)
				{
					dest->UnlockBits(&destdata);
					dest2->UnlockBits(&destdata2);
					delete dest;
					delete dest2;
					return 0;
				}
			}

			int n = GetColorzxV(buffer, &mask, i, j, w, h, pSrc, &bIsTrue, &bHasEdge, &bIsSame);

			if (bIsSame==False)
			{
				color_t color = GetAveragleColorx(buffer, n, bIsTrue, bHasEdge);
				pDest[i+wj] = color;
			}
			else
				pDest[i+wj]=pSrc[i+wj];

		}
		wj+=w;
	}

	wj=0;
	for (UINT j=0; j<h; j++)
	{
		for (UINT i=0; i<w; i++)
		{
			Bool bIsTrue=False;
			Bool bHasEdge=False;
			Bool bIsSame=False;

			if (stopped)
			{
				if (*stopped==True)
				{
					dest->UnlockBits(&destdata);
					dest2->UnlockBits(&destdata2);
					delete dest;
					delete dest2;
					return 0;
				}
			}

			int n = GetColorzxH(buffer, &mask, i, j, w, h, pDest, &bIsTrue, &bHasEdge, &bIsSame);

			if (bIsSame==False)
			{
				color_t color = GetAveragleColorx(buffer, n, bIsTrue, bHasEdge);
				pDest2[i+wj] = color;
			}
			else
				pDest2[i+wj]=pDest[i+wj];
		}
		wj+=w;
	}
RET:
	dest->UnlockBits(&destdata);
	dest2->UnlockBits(&destdata2);
	delete dest;

	return dest2;
}


// ------------------------------------------------------
Bitmap* cyiBitmapBlender::MakeMaskImage(Bitmap* src, color_t color)
{
	BitmapData data;
	src->LockBits(0, ImageLockModeRead|ImageLockModeWrite, PixelFormat32bppARGB, &data);
	color_t* pSrc = (color_t*)data.Scan0;

	if (pSrc==0)
		return 0;
	
	int w = src->GetWidth();
	int h = src->GetHeight();
	int line = 0;

	for (int j=0; j<h; j++)
	{
		for (int i=0; i<w; i++)
		{
			pSrc[i+line]= ((pSrc[i+line]&0xFF000000)|(color&0x00FFFFFF));
		}
		line+=w;
	}
	src->UnlockBits(&data);

	return src;

}


// ------------------------------------------------------
Bitmap* cyiBitmapBlender::NeonSign(Bitmap* src, color_t color, Bool bIsStrong)
{
	int w = src->GetWidth();
	int h = src->GetHeight();

	Bitmap* ret=BITMAP32(w, h);
	Bitmap* bmpBg = BITMAP32(w, h);
	Bitmap* bmpBg2 = BITMAP32(w, h);


	FillColor(bmpBg, 0x00FFFFFF);
	FillColor(bmpBg2, 0x00FFFFFF);

	bmpBg = BitmapCopy(bmpBg, src, 0, 0);
	MakeMaskImage(bmpBg, color);

	bmpBg2 = BitmapCopy(bmpBg2, src, 0, 0);

	Bitmap* bg		= GaussBlureXX(bmpBg, 10.0, 0, 0);
	Bitmap* mask	= GaussBlureXX(bmpBg2, 5.0, 0, 0);

	Graphics g(ret);
	g.DrawImage(bg, 0, 0);

	if (bIsStrong)
		g.DrawImage(bg, 0, 0);

	g.DrawImage(mask, 0, 0);

	if (bIsStrong)
		g.DrawImage(mask, 0, 0);

	g.DrawImage(src, 0, 0);

	delete bg;
	delete mask;
	delete bmpBg;
	delete bmpBg2;

	return ret;

	//cyiBitmapBlender::FillColor(bmp, 0x00FFFFFFFF);

}

#include "yistd.h"

// ------------------------------------------------------
Bitmap* cyiBitmapBlender::GaussBlureXX(color_t* dest, color_t* src, int w, int h, float rad, UINT transparent, cyiShapeMask* pBound)
{
	if (rad>MAX_BLOCK_CNT)
	{
		rad=MAX_BLOCK_CNT;
	}

	//Bitmap* dest = new Bitmap(w, h, PixelFormat32bppARGB);
	//Bitmap* dest2 = new Bitmap(w, h, PixelFormat32bppARGB);

	//BitmapData srcdata, destdata, destdata2;
	//src->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &srcdata);
	//dest->LockBits(0,ImageLockModeRead|ImageLockModeWrite, PixelFormat32bppARGB, &destdata);
	//dest2->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &destdata2);

	int nMemSize=w*h<<2;
	color_t* pSrc = src;//(color_t*)srcdata.Scan0;
	color_t* pDest = (color_t*)yialloc(nMemSize);
	color_t* pDest2 = dest?dest:(color_t*)yialloc(nMemSize);

	colorx_t  buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];

	sBlurMaskxx mask;
	//MakeBlurMask(rad, &mask);

	if (GetGaussDistributionTablexx(rad, &mask)!=0)
	{
		delete dest;
		return 0;
	}

	if (pBound)
	{
		UINT wj=0;
		for (UINT j=0; j<h; j++)
		{
			for (UINT i=0; i<w; i++)
			{
				if (pBound->IsMasked(i, j)==FALSE)
				{
					pDest[i+wj]=pSrc[i+wj];
					continue;
				}

				Bool bIsTrue=False;
				Bool bHasEdge=False;
				Bool bIsSame;

				int n = GetColorzxH(buffer, &mask, i, j, w, h, pSrc, &bIsTrue, &bHasEdge, &bIsSame);
				if (bIsSame==False)
				{
					color_t color = GetAveragleColorx(buffer, n, bIsTrue, bHasEdge);
					pDest[i+wj] = color;
				}
				else
					pDest[i+wj] = pSrc[i+wj];
			}
			wj+=w;
		}


		wj=0;
		for (UINT j=0; j<h; j++)
		{
			for (UINT i=0; i<w; i++)
			{
				if (pBound->IsMasked(i, j)==FALSE)
				{
					pDest2[i+wj]=pDest[i+wj];
					continue;
				}

				Bool bIsTrue=False;
				Bool bHasEdge=False;
				Bool bIsSame;

				int n = GetColorzxV(buffer, &mask, i, j, w, h, pDest, &bIsTrue,  &bHasEdge, &bIsSame);
				if (bIsSame==False)
				{
					color_t color = GetAveragleColorx(buffer, n, bIsTrue, bHasEdge);
					pDest2[i+wj] = color;
				}
				else
					pDest2[i+wj]=pDest[i+wj];
				
			}
			wj+=w;
		}
	}
	else
	{
		UINT wj=0;
		for (UINT j=0; j<h; j++)
		{

			for (UINT i=0; i<w; i++)
			{
				Bool bIsTrue=False;
				Bool bHasEdge=False;
				Bool bIsSame;

				int n = GetColorzxV(buffer, &mask, i, j, w, h, pSrc, &bIsTrue, &bHasEdge, &bIsSame);

				if (bIsSame==False)
				{
					color_t color = GetAveragleColorx(buffer, n, bIsTrue, bHasEdge);
					pDest[i+wj] = color;
				}
				else
					pDest[i+wj]=pSrc[i+wj];
			}
			wj+=w;
		}

		wj=0;
		for (UINT j=0; j<h; j++)
		{
			for (UINT i=0; i<w; i++)
			{
				Bool bIsTrue=False;
				Bool bHasEdge=False;
				Bool bIsSame=False;
				
				int n = GetColorzxH(buffer, &mask, i, j, w, h, pDest, &bIsTrue, &bHasEdge, &bIsSame);

				if (bIsSame==False)
				{
					color_t color = GetAveragleColorx(buffer, n, bIsTrue, bHasEdge);
					pDest2[i+wj] = color;
				}
				else
					pDest2[i+wj]=pDest[i+wj];
			}
			wj+=w;
		}
	}
	//src->UnlockBits(&srcdata);
	//dest->UnlockBits(&destdata);
	//dest2->UnlockBits(&destdata2);

	yifree(pDest);

	return (Bitmap*)pDest2;
	//delete dest;

	//return dest2;
}


// ------------------------------------------------------
typedef struct xsGaussBlureEx {
	unsigned int nMultipleResult_a[16];
	unsigned int nMultipleResult_r[16];
	unsigned int nMultipleResult_g[16];
	unsigned int nMultipleResult_b[16];
	unsigned int nSumResult[4];
	unsigned int nSum;
	unsigned int nTotalSum;
	Bool bIsNoMultiple;
} sGaussBlureEx;

#include "yistd.h"

// ------------------------------------------------------
sGaussBlureEx* xCreateMultipleResult(color_t* pSrc, int w, int h, sBlurMaskInt* mask, int irad, Rect* rect)
{
	int ih=0;
	int nTotalSum=0;
	unsigned int nGerneralMask[16];

	for (int i=1; i<irad; i++)
	{
		nTotalSum+=(mask->mask[i]<<1);
	}

	nTotalSum+=mask->mask[0];

	for (int i=0; i<irad; i++)
	{
		nGerneralMask[i]=(mask->mask[i]<<12)/nTotalSum;
	}

	sGaussBlureEx* ret = (sGaussBlureEx*)yialloc(w*h*sizeof(sGaussBlureEx));
	for (int i=0; i<h; i++)
	{
		for (int j=0; j<w; j++)
		{
			int x=j+ih;
			color_t color=pSrc[x];
			unsigned char a=__COLOR_Ap__(color);

			if (a==0)
			{
				ret[x].bIsNoMultiple=True;
				ret[x].nTotalSum=nTotalSum;
				continue;
			}

			unsigned char r=__COLOR_Rp__(color);
			unsigned char g=__COLOR_Gp__(color);
			unsigned char b=__COLOR_Bp__(color);

			ret[x].bIsNoMultiple=False;
			ret[x].nTotalSum=nTotalSum;

			for (int k=0; k<irad; k++)
			{
				ret[x].nMultipleResult_a[k]= nGerneralMask[i]*a;
				ret[x].nMultipleResult_r[k]= nGerneralMask[i]*r;
				ret[x].nMultipleResult_g[k]= nGerneralMask[i]*g;
				ret[x].nMultipleResult_b[k]= nGerneralMask[i]*b;
			}
		}
		ih+=w;
	}

	return ret;
}

// -------------------------------------------
static void xMakeSumH(int rad, int w, int h, color_t* pSrc, sGaussBlureEx* pixelz, sBlurMaskInt* mask)
{
	int ih=0;
	int max=w*h;

	unsigned int nTotalSum=0;
	unsigned int nGerneralMask[16];

	for (int i=1; i<rad; i++)
	{
		nTotalSum+=(mask->mask[i]<<1);
	}

	nTotalSum+=mask->mask[0];

	for (int i=0; i<rad; i++)
	{
		nGerneralMask[i]=(mask->mask[i]<<12)/nTotalSum;
	}

	for (int i=0; i<h; i++)
	{
		for (int j=0; j<w; j++)
		{
			int x = ih+j;
			color_t color=pSrc[x];
			Bool bIsSame=True;

			for (int k=(1-rad); k<0; k++)
			{
				int xk=x+k;


				if (xk<0 ||
					pixelz[xk].bIsNoMultiple==True)
				{
					pixelz[x].nTotalSum-=nGerneralMask[-k];//mask->mask[-k];
					bIsSame=False;
					continue;
				}
				if (color!=pSrc[xk])
					bIsSame=False;

				pixelz[x].nSumResult[0]+=pixelz[xk].nMultipleResult_a[-k];
				pixelz[x].nSumResult[1]+=pixelz[xk].nMultipleResult_r[-k];
				pixelz[x].nSumResult[2]+=pixelz[xk].nMultipleResult_g[-k];
				pixelz[x].nSumResult[3]+=pixelz[xk].nMultipleResult_b[-k];
			}
			for (int k=0; k<rad; k++)
			{
				int xk=x+k;


				if (xk>max ||
						pixelz[xk].bIsNoMultiple==True)
				{
					pixelz[x].nTotalSum-=nGerneralMask[k];//mask->mask[k];
					bIsSame=False;
					continue;
				}
				
				if (color!=pSrc[xk])
					bIsSame=False;

				pixelz[x].nSumResult[0]+=pixelz[xk].nMultipleResult_a[k];
				pixelz[x].nSumResult[1]+=pixelz[xk].nMultipleResult_r[k];
				pixelz[x].nSumResult[2]+=pixelz[xk].nMultipleResult_g[k];
				pixelz[x].nSumResult[3]+=pixelz[xk].nMultipleResult_b[k];
			}
			if (bIsSame==True)
			{
				pixelz[x].nTotalSum=0xFFFFFFFF;
			}
		}
		ih+=w;
	}
}

// -------------------------------------------
static void xMakeSumV(int rad, int w, int h, color_t* pSrc, sGaussBlureEx* pixelz, sBlurMaskInt* mask)
{
	int ih=0;
	int max=w*h;

	unsigned int nTotalSum=0;
	unsigned int nGerneralMask[16];

	for (int i=1; i<rad; i++)
	{
		nTotalSum+=(mask->mask[i]<<1);
	}

	nTotalSum+=mask->mask[0];

	for (int i=0; i<rad; i++)
	{
		nGerneralMask[i]=(mask->mask[i]<<12)/nTotalSum;
	}

	for (int i=0; i<h; i++)
	{
		for (int j=0; j<w; j++)
		{
			int x = ih+j;
			int xw=x+(1-rad)*w;
			color_t color=pSrc[x];
			Bool bIsSame=True;

			for (int k=(1-rad); k<0; k++)
			{

				//int xkw=x+kw;
				if (xw<0 ||
					pixelz[xw].bIsNoMultiple==True)
				{
					pixelz[x].nTotalSum-=nGerneralMask[-k];//mask->mask[-k];
					xw+=w;
					bIsSame=False;
					continue;
				}
				if (color!=pSrc[xw])
					bIsSame=False;


				pixelz[x].nSumResult[0]+=pixelz[xw].nMultipleResult_a[-k];
				pixelz[x].nSumResult[1]+=pixelz[xw].nMultipleResult_r[-k];
				pixelz[x].nSumResult[2]+=pixelz[xw].nMultipleResult_g[-k];
				pixelz[x].nSumResult[3]+=pixelz[xw].nMultipleResult_b[-k];
				xw+=w;
			}
			for (int k=0; k<rad; k++)
			{
				//int xkw=x+kw;
				if (xw>max ||
						pixelz[xw].bIsNoMultiple==True)
				{
					pixelz[x].nTotalSum-=nGerneralMask[k];//mask->mask[k];
					xw+=w;
					bIsSame=False;
					continue;
				}
				
				if (color!=pSrc[xw])
					bIsSame=False;

				pixelz[x].nSumResult[0]+=pixelz[xw].nMultipleResult_a[k];
				pixelz[x].nSumResult[1]+=pixelz[xw].nMultipleResult_r[k];
				pixelz[x].nSumResult[2]+=pixelz[xw].nMultipleResult_g[k];
				pixelz[x].nSumResult[3]+=pixelz[xw].nMultipleResult_b[k];
				xw+=w;
			}

			if (bIsSame==True)
			{
				pixelz[x].nTotalSum=0xFFFFFFFF;
			}
		}
		ih+=w;
	}
}


// ------------------------------------------------------
static Bitmap* xMakeAverage(Bitmap* dest, color_t* pSrc, int w, int h, int rad, 
							sGaussBlureEx* pResult, sBlurMaskInt* mask, Rect* rect, Bool bIsHorizental=True)
{
	int ih=0;


	if (bIsHorizental==True)
	{
		xMakeSumH(rad, w, h, pSrc, pResult, mask);
	}
	else
	{
		xMakeSumV(rad, w, h, pSrc, pResult, mask);
	}

	if (dest==0)
	{
		dest = BITMAP32(w, h);
	}

	BitmapData destdata;
	dest->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &destdata);
	color_t* pDest = (color_t*)destdata.Scan0;

	for (int i=0; i<h; i++)
	{
		for (int j=0; j<w; j++)
		{
			int x=ih+j;

			if (pResult[x].nTotalSum==0)
				continue;

			if (pResult[x].nTotalSum==0xFFFFFFFF)
			{
				pDest[x]=pSrc[x];
				continue;
			}

			unsigned int a = (pResult[x].nSumResult[0]>>12);///pResult[x].nTotalSum;
			unsigned int r = (pResult[x].nSumResult[1]>>12);//pResult[x].nTotalSum;
			unsigned int g = (pResult[x].nSumResult[2]>>12);//pResult[x].nTotalSum;
			unsigned int b = (pResult[x].nSumResult[3]>>12);//pResult[x].nTotalSum;

			pDest[x]=__COLORp__(a, r, g, b);
		}
		ih+=w;
	}

	dest->UnlockBits(&destdata);

	return dest;
}

// ------------------------------------------------------
Bitmap* cyiBitmapBlender::GaussBlureEx(Bitmap* dest, Bitmap* src, float rad, Rect* rect)
{
	int w=src->GetWidth();
	int h=src->GetHeight();
	int irad = __INTEGER__(rad);

	BitmapData srcdata;
	src->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &srcdata);

	color_t* pSrc = (color_t*)srcdata.Scan0;

	sBlurMaskInt mask;

	if (GetGaussDistributionTablexx(rad, &mask)!=0)
	{
		return 0;
	}

	// まずは水平方向に?け算を行う
	sGaussBlureEx* pResult = xCreateMultipleResult(pSrc, w, h, &mask, irad+1, rect);

	// 平均値を求める
	Bitmap* bmpTemp = xMakeAverage(0, pSrc, w, h, irad+1, pResult, &mask, rect);
	src->UnlockBits(&srcdata);

	bmpTemp->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &srcdata);
	pSrc = (color_t*)srcdata.Scan0;

	if (dest==0)
	{
		dest = BITMAP32(w, h);
	}

	yifree(pResult);

	// 垂直方向の?け算を行う
	pResult = xCreateMultipleResult(pSrc, w, h, &mask, irad+1, rect);

	// 平均値を求める
	Bitmap* ret = xMakeAverage(dest, pSrc, w, h, irad+1, pResult, &mask, rect, False);
	src->UnlockBits(&srcdata);

	delete bmpTemp;
	yifree(pResult);

	return ret;
}

#define __TRANSPARENT__(t, c) ((((UINT)(t*(float)(c>>24)))<<24) | (c&0x00FFFFFF))

// ------------------------------------------------------
Bitmap* cyiBitmapBlender::GaussBlureInnerGrow(Bitmap* src, float rad, float trans, BOOL bIsCopy)
{
		UINT w = src->GetWidth();
		UINT h = src->GetHeight();

		if (rad>MAX_BLOCK_CNT)
		{
			rad=MAX_BLOCK_CNT;
		}

		Bitmap* dest = new Bitmap(w, h, PixelFormat32bppARGB);

		BitmapData srcdata, destdata;
		src->LockBits(0,ImageLockModeRead|((bIsCopy==TRUE)?0:ImageLockModeWrite), PixelFormat32bppARGB, &srcdata);
		dest->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &destdata);

		color_t* pSrc = (color_t*)srcdata.Scan0;
		color_t* pDest = (color_t*)destdata.Scan0;
		colorf_t  buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];

		sBlurMask mask;
		//MakeBlurMask(rad, &mask);

		if (GetGaussDistributionTable(rad, &mask)!=0)
		{
			delete dest;
			return 0;
		}

		if (trans==1.0)
		{
			for (UINT i=0; i<w; i++)
			{
				for (UINT j=0; j<h; j++)
				{
					int n = GetColorzf_d(buffer, &mask, i, j, w, h, pSrc);
					color_t color = GetAveragleColorf(buffer, n);

					pDest[i+w*j] = color;
				}
			}
		}
		else
		{
			for (UINT i=0; i<w; i++)
			{
				for (UINT j=0; j<h; j++)
				{
					int n = GetColorzf_d(buffer, &mask, i, j, w, h, pSrc);
					color_t color = GetAveragleColorf(buffer, n);

					pDest[i+w*j] = __TRANSPARENT__(trans, color);
				}
			}
		}

		if (bIsCopy==TRUE)
		{
			src->UnlockBits(&srcdata);
			dest->UnlockBits(&destdata);
			return dest;
		}

		memcpy(pSrc, pDest, (w*h)<<2);
		src->UnlockBits(&srcdata);
		dest->UnlockBits(&destdata);

		delete dest;
		return src;
}

// ------------------------------------------------------
Bitmap* cyiBitmapBlender::ShadowBitmap(Bitmap* src,  float rad, color_t xcolor, float transparent, cyiShapeMask* shapemask)
{
	UINT irad = __INTEGERF__(rad);
	UINT d = irad<<1;
	UINT w = src->GetWidth();
	UINT h = src->GetHeight();
	UINT _w = w+d;//__INTEGERF__(x);
	UINT _h = h+d;//__INTEGERF__(y);

	Bitmap* dest = new Bitmap(_w, _h, PixelFormat32bppARGB);

	BitmapData srcdata, destdata;
	src->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &srcdata);
	dest->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &destdata);

	color_t* pSrc = (color_t*)srcdata.Scan0;
	color_t* pDest = (color_t*)destdata.Scan0;
	colorf_t  buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];
	
	sBlurMask mask;

	if (GetGaussDistributionTable(rad, &mask)!=0)
	{
		delete dest;
		return 0;
	}


	if (transparent==1.0)
	{
		if (shapemask)
		{
			for (UINT i=0; i<_w; i++)
			{
				for (UINT j=0; j<_h; j++)
				{
					if (shapemask->IsMasked((UINT)(i-irad), (UINT)(j-irad))==FALSE)
					{
						pDest[i+_w*j] = 0;
						continue;
					}

					int n = GetColorzf_d(buffer, &mask, i-irad, j-irad, w, h, pSrc);
					color_t color = GetAveragleColorf(buffer, n);

					//color_t col = 0.299*__COLOR_Rp__(color) + 0.587*__COLOR_Gp__(color)+ 0.114*__COLOR_Bp__(color);
					pDest[i+_w*j] = (color&0xFF000000)|(xcolor&0x00FFFFFF);
				}
			}
		}
		else
		{
			for (UINT i=0; i<_w; i++)
			{
				for (UINT j=0; j<_h; j++)
				{
					int n = GetColorzf_d(buffer, &mask, i-irad, j-irad, w, h, pSrc);
					color_t color = GetAveragleColorf(buffer, n);

					//color_t col = 0.299*__COLOR_Rp__(color) + 0.587*__COLOR_Gp__(color)+ 0.114*__COLOR_Bp__(color);
					//pDest[i+_w*j] = xcolor;
					pDest[i+_w*j] = (color&0xFF000000)|(xcolor&0x00FFFFFF);

				}
			}
		}
	}
	else
	{				
		if (shapemask)
		{
			for (UINT i=0; i<_w; i++)
			{
				for (UINT j=0; j<_h; j++)
				{
					if (shapemask->IsMasked((UINT)(i-irad), (UINT)(j-irad))==FALSE)
					{
						pDest[i+_w*j] = 0;
						continue;
					}

					int n = GetColorzf_d(buffer, &mask, i-irad, j-irad, w, h, pSrc);
					color_t color = GetAveragleColorf(buffer, n);
					UINT a = ((float)(__COLOR_Ap__(color))*transparent);
					a = a>255? 255:a;

					//color_t col = 0.299*__COLOR_Rp__(color) + 0.587*__COLOR_Gp__(color)+ 0.114*__COLOR_Bp__(color);
					pDest[i+_w*j] = __COLORt__(a, xcolor);
				}
			}
		}
		else
		{
			for (UINT i=0; i<_w; i++)
			{
				for (UINT j=0; j<_h; j++)
				{
					int n = GetColorzf_d(buffer, &mask, i-irad, j-irad, w, h, pSrc);
					color_t color = GetAveragleColorf(buffer, n);
					UINT a = ((float)(__COLOR_Ap__(color))*transparent);
					a = a>255? 255:a;

					//color_t col = 0.299*__COLOR_Rp__(color) + 0.587*__COLOR_Gp__(color)+ 0.114*__COLOR_Bp__(color);
					pDest[i+_w*j] = __COLORt__(a, xcolor);
				}
			}
		}
	}
	src->UnlockBits(&srcdata);
	dest->UnlockBits(&destdata);

	return dest;
}


// ------------------------------------------------------
Bitmap* cyiBitmapBlender::ShadowBitmapx(Bitmap* src,  float rad, color_t xcolor, float transparent, cyiShapeMask* shapemask)
{
	UINT irad = __INTEGERF__(rad);
	UINT d = irad<<1;
	UINT w = src->GetWidth();
	UINT h = src->GetHeight();
	UINT _w = w+d;//__INTEGERF__(x);
	UINT _h = h+d;//__INTEGERF__(y);

	Bitmap* dest = new Bitmap(_w, _h, PixelFormat32bppARGB);
	Bitmap* dest2 = new Bitmap(_w, _h, PixelFormat32bppARGB);

	BitmapData srcdata, destdata, destdata2;
	src->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &srcdata);
	dest->LockBits(0,ImageLockModeRead|ImageLockModeWrite, PixelFormat32bppARGB, &destdata);
	dest2->LockBits(0,ImageLockModeRead|ImageLockModeWrite, PixelFormat32bppARGB, &destdata2);

	color_t* pSrc = (color_t*)srcdata.Scan0;
	color_t* pDest = (color_t*)destdata.Scan0;
	color_t* pDest2 = (color_t*)destdata2.Scan0;
	colorx_t  buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];
	
	sBlurMaskxx mask;

	if (GetGaussDistributionTablexx(rad, &mask)!=0)
	{
		delete dest;
		return 0;
	}


	if (transparent==1.0)
	{
		if (shapemask)
		{
			for (UINT j=0; j<_h; j++)
			{
				int _wj = _w*j;
				for (UINT i=0; i<_w; i++)
				{
					if (shapemask->IsMasked(i-(int)irad, j-(int)irad)==FALSE)
					{
						pDest[i+_wj] = 0;//pSrc[i+w*j];
						continue;
					}

					Bool bIsTras=False;
					int n = GetColorzxH_i(buffer, &mask, i-irad, j-irad, w, h, pSrc, &bIsTras);
					color_t color = GetAveragleColorxx(buffer, n, bIsTras);

					pDest[i+_wj] = (color&0xFF000000)|(xcolor&0x00FFFFFF);
				}
			}
			for (UINT j=0; j<_h; j++)
			{
				int _wj = _w*j;
				for (UINT i=0; i<_w; i++)
				{
					if (shapemask->IsMasked(i-(int)irad, j-(int)irad)==FALSE)
					{
						//pDest2[i+_wj] = 0;
						continue;
					}

					Bool bIsTras=False;
					int n = GetColorzxV_i(buffer, &mask, i, j, _w, _h, pDest, &bIsTras);
					color_t color = GetAveragleColorxx(buffer, n, bIsTras);

					pDest2[i+_wj] = (color&0xFF000000)|(xcolor&0x00FFFFFF);
				}
			}
		}
		else
		{
			for (UINT j=0; j<_h; j++)
			{
				int _wj = _w*j;
				for (UINT i=0; i<_w; i++)
				{
					Bool bIsTras=False;
					int n = GetColorzxH_i(buffer, &mask, i-irad, j-irad, w, h, pSrc, &bIsTras);
					color_t color = GetAveragleColorxx(buffer, n, bIsTras);

					//color_t col = 0.299*__COLOR_Rp__(color) + 0.587*__COLOR_Gp__(color)+ 0.114*__COLOR_Bp__(color);
					pDest[i+_wj] = (color&0xFF000000)|(xcolor&0x00FFFFFF);
				}
			}
			for (UINT j=0; j<_h; j++)
			{
				int _wj = _w*j;
				for (UINT i=0; i<_w; i++)
				{
					Bool bIsTras=False;
					int n = GetColorzxV_i(buffer, &mask, i, j, _w, _h, pDest, &bIsTras);
					color_t color = GetAveragleColorxx(buffer, n, bIsTras);

					pDest2[i+_wj] = (color&0xFF000000)|(xcolor&0x00FFFFFF);
				}
			}
		}
	}
	else
	{				
		if (shapemask)
		{
			for (UINT j=0; j<_h; j++)
			{
				int _wj = _w*j;
				for (UINT i=0; i<_w; i++)
				{
					if (shapemask->IsMasked(i-(int)irad, j-(int)irad)==FALSE)
					{
						pDest[i+_wj] = 0;//pSrc[i+w*j];
						continue;
					}

					Bool bIsTras=False;
					int n = GetColorzxH_i(buffer, &mask, i-irad, j-irad, w, h, pSrc, &bIsTras);
					color_t color = GetAveragleColorxx(buffer, n, bIsTras);

					pDest[i+_wj] = (color&0xFF000000)|(xcolor&0x00FFFFFF);
				}
			}
			for (UINT j=0; j<_h; j++)
			{
				int _wj = _w*j;
				for (UINT i=0; i<_w; i++)
				{
					if (shapemask->IsMasked(i-(int)irad, j-(int)irad)==FALSE)
					{
						pDest[i+_wj] = 0;
						continue;
					}

					Bool bIsTras=False;
					int n = GetColorzxV_i(buffer, &mask, i, j, _w, _h, pDest, &bIsTras);
					color_t color = GetAveragleColorxx(buffer, n, bIsTras);

					UINT a = ((float)(__COLOR_Ap__(color))*transparent);
					a = a>255? 255:a;
					pDest2[i+_wj] = ((a<<24)&0xFF000000)|(xcolor&0x00FFFFFF);
				}
			}
		}
		else
		{
			for (UINT j=0; j<_h; j++)
			{
				int _wj = _w*j;
				for (UINT i=0; i<_w; i++)
				{
					Bool bIsTras=False;
					int n = GetColorzxH_i(buffer, &mask, i-irad, j-irad, w, h, pSrc, &bIsTras);
					color_t color = GetAveragleColorxx(buffer, n, bIsTras);

					pDest[i+_wj] = (color&0xFF000000)|(xcolor&0x00FFFFFF);
				}
			}
			for (UINT j=0; j<_h; j++)
			{
				int _wj = _w*j;
				for (UINT i=0; i<_w; i++)
				{
					Bool bIsTras=False;
					int n = GetColorzxV_i(buffer, &mask, i, j, _w, _h, pDest, &bIsTras);
					color_t color = GetAveragleColorxx(buffer, n, bIsTras);

					UINT a = ((float)(__COLOR_Ap__(color))*transparent);
					a = a>255? 255:a;
					pDest2[i+_wj] = ((a<<24)&0xFF000000)|(xcolor&0x00FFFFFF);
				}
			}
		}
	}
	src->UnlockBits(&srcdata);
	dest->UnlockBits(&destdata);
	dest2->UnlockBits(&destdata2);

	delete dest;


	return dest2;
}

// ------------------------------------------------------
Bitmap* cyiBitmapBlender::GlowBitmapEx(Bitmap* src, float rad, float level/*=1.5*/, color_t color/*=0xFF000000*/)
{
	Bitmap* bmp = DropShadowEx(src, 0, 0, rad, 1.0, color);
	Bitmap* ret = Transparent(bmp, level);

	delete bmp;
	return ret;
}

// ------------------------------------------------------
Bitmap* cyiBitmapBlender::GlowBitmap(Bitmap* src,  float rad, color_t xcolor, cyiShapeMask* shapemask)
{
	UINT irad = __INTEGERF__(rad);
	UINT d = irad<<1;
	UINT w = src->GetWidth();
	UINT h = src->GetHeight();
	UINT _w = w+d;//__INTEGERF__(x);
	UINT _h = h+d;//__INTEGERF__(y);

	Bitmap* dest = new Bitmap(_w, _h, PixelFormat32bppARGB);

	BitmapData srcdata, destdata;
	src->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &srcdata);
	dest->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &destdata);

	color_t* pSrc = (color_t*)srcdata.Scan0;
	color_t* pDest = (color_t*)destdata.Scan0;
	colorf_t  buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];
	
	sBlurMask mask;

	if (GetGaussDistributionTable(rad, &mask)!=0)
	{
		delete dest;
		return 0;
	}



	if (shapemask)
	{
		for (UINT i=0; i<_w; i++)
		{
			for (UINT j=0; j<_h; j++)
			{
				if (shapemask->IsMasked(i, j)==FALSE)
				{
					pDest[i+_w*j] = 0;
					continue;
				}

				int n = GetColorzf_d(buffer, &mask, i-irad, j-irad, w, h, pSrc);
				color_t color = GetAveragleColorf(buffer, n);

				//color_t col = 0.299*__COLOR_Rp__(color) + 0.587*__COLOR_Gp__(color)+ 0.114*__COLOR_Bp__(color);
				pDest[i+_w*j] = (color&0xFF000000) ? xcolor:0;
			}
		}
	}
	else
	{
		for (UINT i=0; i<_w; i++)
		{
			for (UINT j=0; j<_h; j++)
			{
				int n = GetColorzf_d(buffer, &mask, i-irad, j-irad, w, h, pSrc);
				color_t color = GetAveragleColorf(buffer, n);

				//color_t col = 0.299*__COLOR_Rp__(color) + 0.587*__COLOR_Gp__(color)+ 0.114*__COLOR_Bp__(color);
				//pDest[i+_w*j] = xcolor;
				pDest[i+_w*j] = (color&0xFF000000) ? xcolor:0;

			}
		}
	}
	src->UnlockBits(&srcdata);
	dest->UnlockBits(&destdata);

	return dest;
}

//#define INTEGER(f) (((f - (float)((INT)f)) < 0.5) ? (INT)f : ((INT)f+1))
// ------------------------------------------------------
Bitmap* cyiBitmapBlender::DropShadow(Bitmap* src, UINT x, UINT y, float rad, float transparent, color_t _color, cyiShapeMask* shadowmask)
{
		UINT irad = __INTEGERF__(rad);
		UINT d = irad<<1;
		color_t xcolor = 0x00FFFFFF&_color;

		//Bitmap* src = new Bitmap(_src->GetWidth()+d, _src->GetHeight()+d, PixelFormat32bppARGB);
		//Graphics gx(src);
		//gx.DrawImage(_src, (float)(d>>1), (float)(d>>1));

		UINT w = src->GetWidth();
		UINT h = src->GetHeight();
		UINT _w = w+__INTEGERF__(x);
		UINT _h = h+__INTEGERF__(y);

		if (rad>MAX_BLOCK_CNT)
		{
			rad=MAX_BLOCK_CNT;
		}

		Bitmap* dest = new Bitmap(w+d, h+d, PixelFormat32bppARGB);

		BitmapData srcdata, destdata;
		src->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &srcdata);
		dest->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &destdata);

		color_t* pSrc = (color_t*)srcdata.Scan0;
		color_t* pDest = (color_t*)destdata.Scan0;
		colorf_t  buffer[MAX_BLOCK_CNT*MAX_BLOCK_CNT];
		
		sBlurMask mask;

		if (GetGaussDistributionTable(rad, &mask)!=0)
		{
			delete dest;
			return 0;
		}

		int b_width = w+d;
		int b_height = h+d;
		for (int i=0; i<b_width; i++)
		{
			for (int j=0; j<b_height; j++)
			{
				if (shadowmask &&
						shadowmask->IsMasked((UINT)i, (UINT)j)==FALSE)
				{
					continue;
				}

				int n = GetColorzf_d(buffer, &mask, i-irad, j-irad, w, h, pSrc);
				color_t color = GetAveragleColorf(buffer, n);

				UINT a = ((float)(__COLOR_Ap__(color))*transparent);
				//color_t col = 0.299*__COLOR_Rp__(color) + 0.587*__COLOR_Gp__(color)+ 0.114*__COLOR_Bp__(color);
				//pDest[i+b_width*j] = (__COLORp__(a, 0, 0, 0)|xcolor);
				pDest[i+b_width*j] = ((a<<24)|xcolor);
			}
		}

		src->UnlockBits(&srcdata);
		dest->UnlockBits(&destdata);



		Bitmap* pret = new Bitmap(b_width+__INTEGERF__(x), b_height+__INTEGERF__(y), PixelFormat32bppARGB);
		Graphics g(pret);

		//g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		g.DrawImage(dest, (float)x, (float)y);
		g.DrawImage(src, 0, 0);

		delete dest;
		return pret;
}

// ------------------------------------------------------
Bitmap* cyiBitmapBlender::DropShadowEx(Bitmap* src, int x, int y, float _rad, float trans, color_t _color/*=0*/)
{
	color_t color=_color==0?Color::Black:_color;
	int rad = __INTEGER__(_rad);
	int srcw = src->GetWidth();
	int srch = src->GetHeight();
	int w=0, h=0;
	Bitmap* bmp;

	w+=(rad<<1);
	h+=(rad<<1);
	w+=abs(x);
	h+=abs(y);
	w+=srcw;
	h+=srch;

	bmp = BITMAP32(w, h);

// まずは、元デ??を単色にしてコピ?する。
	BitmapData srcdata, destdata;
	color_t* pSrc, *pDest;
	src->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &srcdata);
	bmp->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &destdata);

	pSrc=(color_t*)srcdata.Scan0;
	pDest=(color_t*)destdata.Scan0;

	int jw=0;
	int yjw=(y+rad)*w;

	// まずは、モノクロのイメージを作成する。

	if (trans==1.0)
	{
		for (int j=0; j<srch; j++)
		{
			for (int i=0; i<srcw; i++)
			{
				int a = __COLOR_Ap__(pSrc[jw+i]);
				pDest[yjw+i+x+rad]=__COLORt__(a, color);
			}
			jw+=srcw;
			yjw+=w;
		}
	}
	else
	{
		for (int j=0; j<srch; j++)
		{
			for (int i=0; i<srcw; i++)
			{
				int a = __COLOR_Ap__(pSrc[jw+i]);
				a = (int)((float)a*trans);
				//a = a>255?255:a;
				pDest[yjw+i+x+rad]=__COLORt__(a, color);
			}
			jw+=srcw;
			yjw+=w;
		}
	}

	src->UnlockBits(&srcdata);
	bmp->UnlockBits(&destdata);

	// Blure
	Bitmap* bmpBlured=GaussBlureXX(bmp, _rad, 0, 0);

	delete bmp;

	Graphics g(bmpBlured);

	g.DrawImage(src, rad, rad, 0, 0, srcw, srch, Unit::UnitPixel);

	return bmpBlured;


}

// ------------------------------------------------------
Bitmap* cyiBitmapBlender::MakeShadow(Bitmap* src, color_t color, float _rad, float fTransparency)
{
	//color_t color=_color==0?Color::Black:_color;
	int rad = __INTEGER__(_rad);
	int srcw = src->GetWidth();
	int srch = src->GetHeight();
	int w=0, h=0;
	Bitmap* bmp;

	w+=(rad<<1);
	h+=(rad<<1);
	w+=srcw;
	h+=srch;

	bmp = BITMAP32(w, h);

// まずは、元データを単色にしてコピ?する。
	BitmapData srcdata, destdata;
	color_t* pSrc, *pDest;
	src->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &srcdata);
	bmp->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &destdata);

	pSrc=(color_t*)srcdata.Scan0;
	pDest=(color_t*)destdata.Scan0;

	int jw=0;
	int yjw=(rad)*w;

	// まずは、モノクロのイメージを作成する。

	if (fTransparency==1.0)
	{
		for (int j=0; j<srch; j++)
		{
			for (int i=0; i<srcw; i++)
			{
				int a = __COLOR_Ap__(pSrc[jw+i]);
				pDest[yjw+i+rad]=__COLORt__(a, color);
			}
			jw+=srcw;
			yjw+=w;
		}
	}
	else
	{
		for (int j=0; j<srch; j++)
		{
			for (int i=0; i<srcw; i++)
			{
				int a = __COLOR_Ap__(pSrc[jw+i]);
				a = (int)((float)a*fTransparency);
				//a = a>255?255:a;
				pDest[yjw+i+rad]=__COLORt__(a, color);
			}
			jw+=srcw;
			yjw+=w;
		}
	}

	src->UnlockBits(&srcdata);
	bmp->UnlockBits(&destdata);

	// Blure
	Bitmap* bmpBlured=GaussBlureXX(bmp, _rad, 0, 0);

	delete bmp;

	return bmpBlured;
}



// ------------------------------------------------------
//Bitmap* cyiBitmapBlender::DropShadowEx(Bitmap* src, UINT x, UINT y, float rad, float transparent, color_t _color, cyiShapeMask* shadowmask)
//{
//	Bitmap* blure = GaussBlureXX(src, rad, transparent, pBound);
//}

// ------------------------------------------------------
color_t cyiBitmapBlender::GetGrayScaleColor(color_t color, float level)
{
	UINT r = __COLOR_Rp__(color);
	UINT g = __COLOR_Gp__(color);
	UINT b = __COLOR_Bp__(color);

	color_t glayscale = 0.299*__COLOR_Rp__(color) + 0.587*__COLOR_Gp__(color)+ 0.114*__COLOR_Bp__(color);
	glayscale*=level;
	return __COLORp__(__COLOR_Ap__(color), (UINT)(r*(1.0-level)+level*glayscale), (UINT)(g*(1.0-level)+level*glayscale), (UINT)(b*(1.0-level)+level*glayscale));
}


// ------------------------------------------------------
Bitmap* cyiBitmapBlender::GrayScale(Bitmap* src, float level)
{
	UINT w = src->GetWidth();
	UINT h = src->GetHeight();

	Bitmap* dest = new Bitmap(w, h, PixelFormat32bppARGB);

	BitmapData srcdata, destdata;
	src->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &srcdata);
	dest->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &destdata);

	color_t* pSrc = (color_t*)srcdata.Scan0;
	color_t* pDest = (color_t*)destdata.Scan0;

	if (level==1.0)
	{
		for (UINT i=0; i<w; i++)
		{
			for (UINT j=0; j<h; j++)
			{
				color_t color = pSrc[i+w*j];
				color_t glayscale = 0.299*__COLOR_Rp__(color) + 0.587*__COLOR_Gp__(color)+ 0.114*__COLOR_Bp__(color);
				pDest[i+w*j] = __COLORp__(__COLOR_Ap__(color), glayscale, glayscale, glayscale);
			}
		}
	}
	else
	{
		for (UINT i=0; i<w; i++)
		{
			for (UINT j=0; j<h; j++)
			{
				color_t color = pSrc[i+w*j];
				UINT r = __COLOR_Rp__(color);
				UINT g = __COLOR_Gp__(color);
				UINT b = __COLOR_Bp__(color);

				color_t glayscale = 0.299*__COLOR_Rp__(color) + 0.587*__COLOR_Gp__(color)+ 0.114*__COLOR_Bp__(color);
				glayscale*=level;
				pDest[i+w*j] = __COLORp__(__COLOR_Ap__(color), (UINT)(r*(1.0-level)+level*glayscale), (UINT)(g*(1.0-level)+level*glayscale), (UINT)(b*(1.0-level)+level*glayscale));
			}
		}
	}
	src->UnlockBits(&srcdata);
	dest->UnlockBits(&destdata);

	return dest;
}

// ------------------------------------------------------
Bitmap* cyiBitmapBlender::ArgbShift(Bitmap* src, int da, int dr, int dg, int db)
{
	UINT w = src->GetWidth();
	UINT h = src->GetHeight();

	Bitmap* dest = new Bitmap(w, h, PixelFormat32bppARGB);

	BitmapData srcdata, destdata;
	
	if (src->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &srcdata)!=Status::Ok)
	{
		return 0;
	}

	dest->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &destdata);

	color_t* pSrc = (color_t*)srcdata.Scan0;
	color_t* pDest = (color_t*)destdata.Scan0;

	for (UINT i=0; i<w; i++)
	{
		for (UINT j=0; j<h; j++)
		{
			color_t color = pSrc[i+w*j];
			int a= (int)__COLOR_Ap__(color)+da;
			int r= (int)__COLOR_Rp__(color)+dr;
			int g= (int)__COLOR_Gp__(color)+dg;
			int b= (int)__COLOR_Bp__(color)+db;

			r=r<0?0:r;
			g=g<0?0:g;
			b=b<0?0:b;
			a=a<0?0:a;

			pDest[i+w*j]=__COLORp__(((a>255)?255:a), ((r>255)?255:r), ((g>255)?255:g), ((b>255)?255:b));
		}
	}
	src->UnlockBits(&srcdata);
	dest->UnlockBits(&destdata);

	return dest;
}

// ------------------------------------------------------
Bitmap* cyiBitmapBlender::Transparent(Bitmap* src, float level)
{
	UINT w = src->GetWidth();
	UINT h = src->GetHeight();

	if (level==1.0)
		return BitmapCopy(0, src);

	Bitmap* dest = new Bitmap(w, h, PixelFormat32bppARGB);


	BitmapData srcdata, destdata;
	src->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &srcdata);
	dest->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &destdata);

	color_t* pSrc = (color_t*)srcdata.Scan0;
	color_t* pDest = (color_t*)destdata.Scan0;

	int wj=0;
	for (UINT j=0; j<h; j++)
	{
		for (UINT i=0; i<w; i++)
		{
			color_t color = pSrc[i+wj];
			int a= (int)((float)(__COLOR_Ap__(color))*level);
			//int r= (int)__COLOR_Rp__(color);
			//int g= (int)__COLOR_Gp__(color);
			//int b= (int)__COLOR_Bp__(color);

			pDest[i+wj]= __COLORt__((a>255)?255:a, color);//__COLORp__(((a>255)?255:a), ((r>255)?255:r), ((g>255)?255:g), ((b>255)?255:b));
		}
		wj+=w;
	}
	src->UnlockBits(&srcdata);
	dest->UnlockBits(&destdata);

	return dest;
}

// ------------------------------------------------------
static float GetDistance(float x1, float y1, float x2, float y2)
{
	float x_diff=x1-x2;
	float y_diff=y1-y2;

	return ::sqrtf((x_diff*x_diff)+(y_diff*y_diff));
}


// ------------------------------------------------------
static BOOL IsInRect(float x, float y, xRECTF* pRect)
{
	if (x<pRect->x ||
			y<pRect->y ||
				x>(pRect->w+pRect->x) ||
					y>(pRect->h+pRect->y))
	{
		return FALSE;
	}

	return TRUE;
}


// ------------------------------------------------------
static BOOL NearZero(float f)
{
	if (f<(float)0.0001 &&
			f>(float)-0.0001)
	{
		return TRUE;
	}

	return FALSE;
}

// ------------------------------------------------------
BOOL cyiBitmapBlender::GetAvPixel(float _x, float _y, color_t& color, xRECTF* pRect)
{
	if (IsInRect(_x, _y, pRect)==FALSE)
	{
		color=0;
		return FALSE;
	}

	UINT* colors = (UINT*)m_bitmap.bmBits;
	UINT w = m_bitmap.bmWidth;

	UINT x[4];
	UINT y[4];
	float distance[4];
	float distance_sum=0;

	x[0]=(UINT)_x;
	y[0]=(UINT)_y;
	x[1]=x[0]+1;
	y[1]=y[0];
	x[2]=x[0];
	y[2]=y[0]+1;
	x[3]=x[0]+1;
	y[3]=y[0]+1;

	int cnt=0;
	for (UINT i=0; i<4; i++)
	{
		if (IsInRect(x[i], y[i], pRect)==FALSE)
		{
			distance[i]=-1.0;
			continue;
		}

		UINT  col = colors[(y[i]*w)+x[i]];

		if (__COLOR_A__(col)==0)
		{
			distance[i]=-1.0;
			continue;
		}
		
		distance[i] = GetDistance((float)x[i], (float)y[i], _x, _y);

		if (NearZero(distance[i])==TRUE)
		{
			color = col;
			return TRUE;
		}
		distance[i]=1/distance[i];
		distance_sum+= distance[i];
	}

	if (distance_sum==0.0)
	{
		color=0;
		return FALSE;
	}

	UINT a_sum=0;
	UINT r_sum=0;
	UINT g_sum=0;
	UINT b_sum=0;

	for (UINT i=0; i<4; i++)
	{
		float f = (distance[i]/distance_sum);
		UINT  col = colors[w*y[i]+x[i]];

		if (f==0.0)
		{
			color=col;
			return TRUE;
		}


		if (distance[i]<0)
		{
			continue;
		}
		else
		{
			a_sum += (UINT)(f*(float)__COLOR_A__(col));
			r_sum += (UINT)(f*(float)__COLOR_R__(col));
			g_sum += (UINT)(f*(float)__COLOR_G__(col));
			b_sum += (UINT)(f*(float)__COLOR_B__(col));
		}
	}

	color = __COLOR__(a_sum,r_sum, g_sum, b_sum);

	return TRUE;
}

#ifndef PI
#define PI (3.141592)
#endif

// ------------------------------------------------------
BOOL cyiBitmapBlender::GetCirclePixel(float x, float y, float rad, UINT n, xPOINTF* pt)
{
	UINT	_n = n>>2;
	float	delta = (PI/4)/(float)_n;
	float   delta_sum=0.0;

	for (UINT i=0; i<_n+1; i++)
	{
		pt[i].y = ::sinf(delta_sum)*rad;
		pt[i].x = ::cosf(delta_sum)*rad;

		delta_sum+=delta;
	}

	for (UINT i=1; i<_n+1; i++)
	{
		pt[_n+i].x = -pt[_n-i].x;
		pt[_n+i].y = pt[_n-i].y;
	}

	for (UINT i=1; i<_n+1; i++)
	{
		pt[_n+i].x = -pt[_n-i].x;
		pt[_n+i].y = pt[_n-i].y;
	}

	int cnt=(_n<<1)-1;
	for (UINT i=(_n<<1)+1; i<n; i++, cnt--)
	{
		pt[i].x = pt[cnt].x ;
		pt[i].y = pt[cnt].y ;
	}

	for (UINT i=0; i<n; i++)
	{
		pt[i].x+=x;
		pt[i].y+=y;
	}

	return TRUE;
}

// ------------------------------------------------------
BOOL cyiBitmapBlender::GetAvPixelz(xPOINTF* pt, UINT* color, int num, xRECTF* pBoundary)
{
	for (int i=0; i<num; i++)
	{
		GetAvPixel(pt[i].x, pt[i].y, color[i], pBoundary);
	}

	return TRUE;
}

// ------------------------------------------------------
color_t cyiBitmapBlender::GetAverageColor(const color_t* colors, int n)
{
	int cnt=0;
	color_t a=0;
	color_t r=0;
	color_t g=0;
	color_t b=0;

	for (int i=0; i<n; i++)
	{
		if (__COLOR_A__(colors[i])==0)
		{
			continue;
		}

		a += __COLOR_A__(colors[i]);
		r += __COLOR_R__(colors[i]);
		g += __COLOR_G__(colors[i]);
		b += __COLOR_B__(colors[i]);
		cnt++;
	}

	if (cnt==0)
		return 0;

	a = a/cnt;
	g = b/cnt;
	b = b/cnt;
	r = r/cnt;

	return __COLOR__(a, r, g, b);

}


// -------------------------------
Bitmap* cyiBitmapBlender::SetMask(Bitmap* src, Bitmap* mask, int x, int y, BOOL bIsReverse)
{
	BitmapData srcdata, maskdata;
	src->LockBits(0,ImageLockModeRead|ImageLockModeWrite, PixelFormat32bppARGB, &srcdata);
	mask->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &maskdata);

	color_t* pSrc = (color_t*)srcdata.Scan0;
	color_t* pMask =(color_t*)maskdata.Scan0;

	int w = src->GetWidth();
	int h = src->GetHeight();
	int dw = mask->GetWidth();
	int dh=mask->GetHeight();
	int jdw=-y*dw;
	int jw=0;

	if (bIsReverse==TRUE)
		goto REVERSE;


	for (int j=0; j<h; j++)
	{
		for (int i=0; i<w; i++)
		{
			int _x = i-x;
			int _y = j-y;

			if (_x<0 || _y<0)
			{
				continue;
			}
			if (_x>dw || _y>dh)
			{
				continue;
			}
			
			color_t a = __COLOR_Ap__(pMask[_x+jdw]);
			if (a==0)
			{
				pSrc[i+jw]=0;
				continue;
			}

			if (a==255)
			{
				continue;
			}
			else
			{
				int tmp=i+jw;
				color_t color_src=pSrc[tmp];
				if ((color_src&0xFF000000)==0)
					continue;

				int _a = (a*__COLOR_Ap__(color_src))>>8; //
				 pSrc[tmp]= ((0x00FFFFFF&color_src)|(_a<<24));
			}
		}
		jw+=w;
		jdw+=dw;
	}

	src->UnlockBits(&srcdata);
	mask->UnlockBits(&maskdata);

	return src;

REVERSE:
	for (int i=0; i<w; i++)
	{
		for (int j=0; j<h; j++)
		{
			int _x = i-x;
			int _y = jdw;

			if (_x<0 || _y<0)
			{
				continue;
			}
			if (_x>dw || _y>dh)
			{
				continue;
			}
			
			color_t a = __COLOR_Ap__(pMask[_x+_y]);
			if (a==0)
			{
				continue;
			}
			if (a==255)
			{
				pSrc[i+jw]=0;
				continue;
			}
			else
			{
				int tmp=i+jw;
				color_t color_src=pSrc[tmp];
				if ((color_src&0xFF000000)==0)
					continue;
				int _a = ((255-a)*__COLOR_Ap__(color_src))>>8; //
				pSrc[tmp]= ((0x00FFFFFF&color_src)|(_a<<24));

				pSrc[i+jw] = ((0x00FFFFFF&pSrc[i+j*w])|(_a<<24));
			}
		}
		jw+=w;
		jdw+=dw;
	}
	src->UnlockBits(&srcdata);
	mask->UnlockBits(&maskdata);

	return src;
}

// -------------------------------------------------------------
//Bitmap* cyiBitmapBlender::BitmapCopy(Bitmap* bmp, const Bitmap* _src)
//{
//	Bitmap* src=(Bitmap*)_src;
//	int w = src->GetWidth();
//	int h = src->GetHeight();
//
//	if (bmp==NULL)
//		bmp = new Bitmap(src->GetWidth(), src->GetHeight(), PixelFormat32bppARGB);
//
//	BitmapData srcdata, maskdata;
//	src->LockBits(0,ImageLockModeRead|ImageLockModeWrite, PixelFormat32bppARGB, &srcdata);
//	bmp->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &maskdata);
//
//	color_t* pSrc = (color_t*)srcdata.Scan0;
//	color_t* pDest =(color_t*)maskdata.Scan0;
//
//	memcpy(pDest, pSrc, (w*h)<<2);
//
//	src->UnlockBits(&srcdata);
//	bmp->UnlockBits(&maskdata);
//
//	return bmp;
//}

// ------------------------------------------------------
color_t cyiBitmapBlender::ColorShift(color_t color, int a, int r, int g, int b)
{
	int _a = __COLOR_A__(color);
	int _r = __COLOR_R__(color);
	int _g = __COLOR_G__(color);
	int _b = __COLOR_B__(color);

	_a+=a;
	_r+=r;
	_g+=g;
	_b+=b;

	_a = _a > 255? 255:_a;
	_r = _r > 255? 255:_r;
	_g = _g > 255? 255:_g;
	_b = _b > 255? 255:_b;

	return __COLORp__(_a, _r, _g, _b);
}

// ------------------------------------------------------
color_t cyiBitmapBlender::ColorContrast(color_t color, float level)
{
	return 0;
	//color_t glayscale = 0.299*__COLOR_Rp__(color) + 0.587*__COLOR_Gp__(color)+ 0.114*__COLOR_Bp__(color);
}


// ------------------------------------------------------
Bitmap* cyiBitmapBlender::FillColor(Bitmap* bmp, color_t color, Rect* rect)
{
	int w = bmp->GetWidth();
	int h = bmp->GetHeight();

	BitmapData bmpdata;
	bmp->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &bmpdata);
	color_t* pSrc = (color_t*)bmpdata.Scan0;

	if (rect==0 && color==0)
	{
		memset(pSrc, color, (w*h)<<2);
		bmp->UnlockBits(&bmpdata);

		return bmp;
	}

	Rect xrect(0, 0, w, h);// = {0, 0, w, h};

	if (rect==0)
		rect=&xrect;
	int wj=rect->Y*w;
	for (int j=rect->Y; j<(rect->Height+rect->Y); j++)
	{
		for (int i=rect->X; i<(rect->Width+rect->X); i++)
		{
			pSrc[i+wj]=color;
		}
		wj+=w;
	}
	
	bmp->UnlockBits(&bmpdata);	

	return bmp;
}


//---------------------------------------------------
void cyiBitmapBlender::Reverse(Bitmap* bmp, BOOL bIsHorizental)
{
	int w = bmp->GetWidth();
	int h = bmp->GetHeight();
	if (bIsHorizental)
	{
		BitmapData bmpdata;
		bmp->LockBits(0,ImageLockModeRead|ImageLockModeWrite, PixelFormat32bppARGB, &bmpdata);
		color_t* pSrc = (color_t*)bmpdata.Scan0;

		int _w = w/2;

		for (int i=0; i<_w; i++)
		{
			for (int j=0; j<h; j++)
			{
				int n = i+w*j;
				int n2 = (w-i-1)+w*j;
				color_t temp = pSrc[n];
				pSrc[n]=pSrc[n2];
				pSrc[n2]=temp;
			}
		}

		bmp->UnlockBits(&bmpdata);
		return ;
	}

	BitmapData bmpdata;
	bmp->LockBits(0,ImageLockModeRead|ImageLockModeWrite, PixelFormat32bppARGB, &bmpdata);
	color_t* pSrc = (color_t*)bmpdata.Scan0;

	int _h = h/2;
	color_t buffer[4096];

	for (int j=0; j<_h; j++)
	{
		int n1 = w*j;
		int n2 = w*(h-j-1);
		memcpy(buffer, &pSrc[n1], w<<2);
		memcpy(&pSrc[n1], &pSrc[n2], w<<2);
		memcpy(&pSrc[n2], buffer, w<<2);
	}

	bmp->UnlockBits(&bmpdata);
	return ;
}

//---------------------------------------------------
static Bool xGetMaxRange(Bitmap* bmp, int x, int y, int w, int h, int& w_out, int& h_out)
{
	int _w=bmp->GetWidth();
	int _h=bmp->GetHeight();
	Bool result=True;

	w_out=0;
	h_out=0;

	if (x<0)
	{
		result=False;
		w_out=w+x;
		x=0;
	}

	if (y<0)
	{
		result=False;
		h_out=h+y;
		y=0;
	}

	if (x+w>_w)
	{
		result=False;
		w_out=_w-x;
	}

	if (y+h>_h)
	{
		result=False;
		h_out=_h-y;
	}

	return result;
}

//---------------------------------------------------
void cyiBitmapBlender::DeleteBitmap(Bitmap* bmp)
{
	if (bmp!=0)
		delete bmp;
}

//---------------------------------------------------
Bitmap* cyiBitmapBlender::BitmapCopy(Bitmap* dest, Bitmap* src, int xDest, int yDest,  int xSrc, int ySrc, int wSrc, int hSrc)
{
	if (src==0)
	{
		return dest;
	}
	if (dest==0)
	{
		if (src==0)
		{
			return 0;
		}
		unsigned int w = wSrc==0 ? src->GetWidth():wSrc;
		unsigned int h = hSrc==0 ? src->GetHeight():hSrc;
		dest = new Bitmap(w, h, PixelFormat32bppARGB);
	}

	if (wSrc>src->GetWidth())
		wSrc=src->GetWidth();

	if (hSrc>src->GetHeight())
		hSrc=src->GetHeight();

	if (xSrc+wSrc>src->GetWidth())
	{
		wSrc=src->GetWidth()-xSrc;
	}

	if (ySrc+hSrc>src->GetHeight())
	{
		hSrc=src->GetHeight()-ySrc;
	}

	//if (xSrc>=wSrc ||
	//		ySrc>=hSrc)
	//{
	//	return 0;
	//}

	//RECT rectDest = {xDest, yDest, dest->GetWidth()+xDest, dest->GetHeight()+yDest};
	RECT rectDest = {xDest, yDest, dest->GetWidth(), dest->GetHeight()};
	RECT rectSrc = {xDest, yDest, (wSrc?wSrc:src->GetWidth())+xDest, (hSrc?hSrc:src->GetHeight())+yDest};
	RECT rectResult;
	{
		if (yiUtilIsInRect(&rectDest, &rectSrc, rectResult)==False)
		{
			return False;
		}

		wSrc=rectResult.right-rectResult.left;
		hSrc=rectResult.bottom-rectResult.top;
	}

	if (wSrc<0 ||
			hSrc<0)
	{
		return False;
	}


	unsigned int xw=src->GetWidth();
	unsigned int w_src = wSrc ? wSrc:xw;
	unsigned int h_src = hSrc ? hSrc:src->GetHeight();
	unsigned int w_dest = dest->GetWidth();
	unsigned int h_dest = dest->GetHeight();

	if (src->GetWidth()==dest->GetWidth() &&
		src->GetWidth()==wSrc &&
		xDest==xSrc &&
		yDest==ySrc)
	{
		BitmapData bmpdata, bmpdata2;
		src->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &bmpdata);
		color_t* pSrc = (color_t*)bmpdata.Scan0;
		dest->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &bmpdata2);
		color_t* pDest = (color_t*)bmpdata2.Scan0;

		int index = w_src*ySrc+xSrc;
		memcpy(&pDest[index], &pSrc[index], (w_src*h_src)<<2);

		src->UnlockBits(&bmpdata);
		dest->UnlockBits(&bmpdata2);
		return dest;
	}
			
/*
	int fw, fh;

	if (xGetMaxRange(dest, xDest, yDest, w_src, h_src, fw, fh)==False)
	{
		if (fw<=0 || fh <=0)
		{
			return 0;
		}

		if (fw)
			w_src=fw;
		if (fh)
			h_src=fh;

	}

	if (xGetMaxRange(src, xSrc, ySrc, w_src, h_src, fw, fh)==False)
	{
		if (fw<=0 || fh <=0)
		{
			return 0;
		}

		if (fw)
			w_src=fw;
		if (fh)
			h_src=fh;
	}
	*/

	BitmapData bmpdata, bmpdata2;
	src->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &bmpdata);
	color_t* pSrc = (color_t*)bmpdata.Scan0;
	dest->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &bmpdata2);
	color_t* pDest = (color_t*)bmpdata2.Scan0;

	//if (w_src==w_dest &&
	//		h_src==h_dest)
	//{
	//	int len = w_src*h_src-(w_src*ySrc+xSrc);
	//	memcpy(&pDest[w_dest*yDest+xDest], &pSrc[w_src*ySrc+xSrc], sizeof(color_t)*len);
	//	src->UnlockBits(&bmpdata);
	//	dest->UnlockBits(&bmpdata2);

	//	return dest;
	//}

	int k=ySrc*xw;
	int k2=yDest*w_dest;
	for (UINT j=ySrc, ij=yDest; j<(ySrc+h_src); j++, ij++)
	{
		//int k = xw*j;
		//int k2 =w_dest*ij;
		for (UINT i=xSrc, ix=xDest; i<(xSrc+w_src); i++, ix++)
		{
			pDest[k2+ix]=pSrc[k+i];
		}
		k+=xw;
		k2+=w_dest;
		//k+=xw;
		//k2+=w_dest;
	}


	src->UnlockBits(&bmpdata);
	dest->UnlockBits(&bmpdata2);

	return dest;
}

//---------------------------------------------------
//Bitmap* BitmapMerge(Bitmap* dest, Bitmap* src, eBitmapMergeOption option)//, int xDest, int yDest,  int xSrc, int ySrc, int wSrc, int hSrc, eBitmapMergeOption option)
//{
//	if (dest==0 ||
//			src==0)
//		return 0;
//
//	if (dest->GetWidth()!=src->GetWidth())
//		return 0;
//
//	if (dest->GetHBITMAP()!=src->GetHeight())
//		return 0;
//
//	//wSrc = wSrc==0?src->GetWidth():wSrc;
//	//hSrc = hSrc==0?src->GetHeight():hSrc;
//
//	//wSrc = src->GetWidth()<(wSrc+xSrc)?(src->GetWidth()-xSrc):(wSrc-xSrc);
//	//hSrc = src->GetHeight()<(hSrc+ySrc)?(src->GetHeight()-ySrc):(hSrc-ySrc);
//
//	//RECT rectSrc	= {xSrc, ySrc, wSrc, hSrc};
//	//RECT rectDest	= {xDest, yDest, wSrc, hSrc};
//
//	//rectDest.right	= dest->GetWidth()<(rectDest.right+xDest)?(dest->GetWidth()-xDest):(rectDest.right-xDest);
//	//rectDest.bottom = dest->GetHeight()<(rectDest.bottom+yDest)?(dest->GetHeight()-yDest):(rectDest.bottom-yDest);
//	////hSrc = src->GetHeight()<(hSrc+ySrc)?(src->GetHeight()-ySrc):(hSrc-ySrc);
//	//RECT rectResult;
//	//
//	//if yiUtilIsInRect(&rectSrc, &rectDest, &rectResult)==False)
//	//	return False;
//
//	// ビットマップ作成。
//
//	int w = src->GetWidth();
//	int h = src->GetHeight();
//	Bitmap* ret = sBITMAP32(w, h);
//
//	BitmapData data1, data2, destData;
//
//	src->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &data1);
//	dest->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &data2);
//	ret->LockBits(0, ImageLockModeWrite, PixelFormat32bppARGB, &destData);
//	color_t* pSrc = (color_t*)bmpdata.Scan0;
//	color_t* pDest = (color_t*)bmpdata2.Scan0;
//
//
//	if (option==eBMO_AVER)
//	{
//		for (int i=0; i<h; i++)
//		{
//			for (int j=0; j<w; j++)
//			{
//				unsigned int a = (((((pSrc[j+line]&0xFF000000)>>24)+((pDest[j+line]&0xFF000000)>>24))>>1)<<24);
//				unsigned int r = (((((pSrc[j+line]&0x00FF0000)>>16)+((pDest[j+line]&0x00FF0000)>>16))>>1)<<16);
//				unsigned int g = (((((pSrc[j+line]&0x0000FF00)>>8)+((pDest[j+line]&0x0000FF00)>>8))>>1)<<8);
//				unsigned int b = (((((pSrc[j+line]&0xFF))+((pDest[j+line]&0xFF)))>>1));
//				pRet[j+line]=xARGB(a, r, g, b);
//			}
//		}
//	}
//	else if (option==eBMO_LINEAR)
//	{
//		for (int i=0; i<h; i++)
//		{
//			for (int j=0; j<w; j++)
//			{
//				unsigned int a = (((((pSrc[j+line]&0xFF000000)>>24)+((pDest[j+line]&0xFF000000)>>24))>>1)<<24)&0xFF;
//				unsigned int r = (((((pSrc[j+line]&0x00FF0000)>>16)+((pDest[j+line]&0x00FF0000)>>16))>>1)<<16)&0xFF;
//				unsigned int g = (((((pSrc[j+line]&0x0000FF00)>>8)+((pDest[j+line]&0x0000FF00)>>8))>>1)<<8)&0xFF;
//				unsigned int b = (((((pSrc[j+line]&0xFF))+((pDest[j+line]&0xFF)))))&0xFF;
//				pRet[j+line]=xARGB(a, r, g, b);
//			}
//		}
//	}
//
//	src->UnlockBits(&bmpdata);
//	dest->UnlockBits(&bmpdata2);
//	ret->UnlockBits(&destData);
//
//	return ret;
//
//}



//---------------------------------------------------
Bitmap* cyiBitmapBlender::RotateCopy(Bitmap* dest, Bitmap* src, int n, int xDest, int yDest,  int xSrc, int ySrc, int wSrc, int hSrc, cyiShapeMask* mask)
{
	if (dest==0)
	{
		unsigned int w = wSrc==0 ? src->GetWidth():wSrc;
		unsigned int h = hSrc==0 ? src->GetHeight():hSrc;

		if (n==0 || n==2)
			dest = new Bitmap(w, h, PixelFormat32bppARGB);
		else
			dest = new Bitmap(h, w, PixelFormat32bppARGB);
	}

	BitmapData bmpdata, bmpdata2;
	src->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &bmpdata);
	color_t* pSrc = (color_t*)bmpdata.Scan0;
	dest->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &bmpdata2);
	color_t* pDest = (color_t*)bmpdata2.Scan0;

	if (n==0)
	{
		unsigned int xw=src->GetWidth();
		unsigned int w_src = wSrc ? wSrc:xw;
		unsigned int h_src = hSrc ? hSrc:src->GetHeight();
		unsigned int w_dest = dest->GetWidth();
		unsigned int h_dest = dest->GetHeight();

		if (mask==0)
		{
			//if (w_src==w_dest &&
			//	ySrc==0)
			//{
			//	memcpy(pDest, pSrc, (w_src*h_src)<<2);
			//}
			//else
			//{
				for (UINT j=ySrc, ij=yDest; j<(ySrc+h_src); j++, ij++)
				{
					int k = xw*j;
					int k2 =w_dest*ij;
					for (UINT i=xSrc, ix=xDest; i<(xSrc+w_src); i++, ix++)
					{
						pDest[k2+ix]=pSrc[k+i];
					}
				}
			/*}*/
		}
		else
		{
			for (UINT i=xSrc, ix=xDest; i<(xSrc+w_src); i++, ix++)
			{
				for (UINT j=ySrc, ij=yDest; j<(ySrc+h_src); j++, ij++)
				{
					float level;
					if ((level=mask->GetMaskedLevel(i, j))==0.0)
						continue;
					int k = i+xw*j;
					int k2 =ix+w_dest*ij;

					//if (level!=1.0)
					//{
					//	int a = __COLOR_A__(pSrc[k]);
					//	a = (int)a*level;
					//	pDest[k2]=__COLORt__(a, pSrc[k]);
					//	continue;
					//}
					pDest[k2]=pSrc[k];
				}
			}
		}

		src->UnlockBits(&bmpdata);
		dest->UnlockBits(&bmpdata2);
		return dest;
	}
	else if (n==3)
	{
		unsigned int xw=src->GetWidth();
		unsigned int w_src = wSrc ? wSrc:src->GetWidth();
		unsigned int h_src = hSrc ? hSrc:src->GetHeight();
		unsigned int w_dest = dest->GetWidth();
		unsigned int h_dest = dest->GetHeight();

		if (mask==0)
		{
			for (UINT i=xSrc, ix=xDest; i<w_src; i++, ix++)
			{
				for (UINT j=ySrc, ij=yDest; j<h_src; j++, ij++)
				{
					int k = i+xw*j;
					int k2 = (h_src-1-ij-xDest)+w_dest*(ix);
					pDest[k2]=pSrc[k];
				}
			}
		}
		else
		{
			for (UINT i=xSrc, ix=xDest; i<w_src; i++, ix++)
			{
				for (UINT j=ySrc, ij=yDest; j<h_src; j++, ij++)
				{
					float level;
					if ((level=mask->GetMaskedLevel(i, j))==0.0)
						continue;
					int k = i+w_src*j;
					int k2 = (h_src-1-j-xDest)+w_dest*(ix);

					if (level!=1.0)
					{
						int a = __COLOR_A__(pSrc[k]);
						a = (int)a*level;
						pDest[k2]=__COLORt__(a, pSrc[k]);
						continue;
					}
					pDest[k2]=pSrc[k];
				}
			}
		}
		src->UnlockBits(&bmpdata);
		dest->UnlockBits(&bmpdata2);
		return dest;
	}
	else if (n==2)
	{
		unsigned int w_src = wSrc ? wSrc:src->GetWidth();
		unsigned int h_src = hSrc ? hSrc:src->GetHeight();
		unsigned int w_dest = dest->GetWidth();
		unsigned int h_dest = dest->GetHeight();

		if (mask==0)
		{
			for (UINT i=xSrc, ix=xDest; i<w_src; i++, ix++)
			{
				for (UINT j=ySrc; j<h_src; j++)
				{
					int k = i+w_src*j;
					int k2 =(w_src-ix-1)+w_dest*(h_src-j-1);
					pDest[k2]=pSrc[k];
				}
			}
		}
		else
		{
			for (UINT i=xSrc, ix=xDest; i<w_src; i++, ix++)
			{
				for (UINT j=ySrc; j<h_src; j++)
				{
					float level;
					if ((level=mask->GetMaskedLevel(i, j))==0.0)
						continue;

					int k = i+w_src*j;
					int k2 =(w_src-ix-1)+w_dest*(h_src-j-1);

					if (level!=1.0)
					{
						int a = __COLOR_A__(pSrc[k]);
						a = (int)a*level;
						pDest[k2]=__COLORt__(a, pSrc[k]);
						continue;
					}

					pDest[k2]=pSrc[k];
				}
			}

		}

		src->UnlockBits(&bmpdata);
		dest->UnlockBits(&bmpdata2);
		return dest;
	}
	else if (n==1)
	{
		unsigned int w_srcx=src->GetWidth();
		unsigned int w_src = wSrc ? wSrc:w_srcx;
		unsigned int h_src = hSrc ? hSrc:src->GetHeight();
		unsigned int w_dest = dest->GetWidth();
		unsigned int h_dest = dest->GetHeight();

		if (mask==0)
		{
			for (UINT i=xSrc, ix=xDest; i<w_src; i++, ix++)
			{
				for (UINT j=ySrc; j<h_src; j++)
				{
					int k = i+w_srcx*j;
					int k2 = (xDest+j)+w_dest*(ix);
					pDest[k2]=pSrc[k];
				}
			}
		}
		else
		{
			for (UINT i=xSrc, ix=xDest; i<w_src; i++, ix++)
			{
				for (UINT j=ySrc; j<h_src; j++)
				{
					float level;
					if ((level=mask->GetMaskedLevel(i, j))==0.0)
						continue;
					int k = i+w_srcx*j;
					int k2 = (xDest+j)+w_dest*(ix);

					if (level!=1.0)
					{
						int a = __COLOR_A__(pSrc[k]);
						a = (int)a*level;
						pDest[k2]=__COLORt__(a, pSrc[k]);
						continue;
					}
					
					pDest[k2]=pSrc[k];
				}
			}
		}
		src->UnlockBits(&bmpdata);
		dest->UnlockBits(&bmpdata2);
		return dest;
	}
	src->UnlockBits(&bmpdata);
	dest->UnlockBits(&bmpdata2);

	return 0;
}

//---------------------------------------------------
HBITMAP cyiBitmapBlender::CreateHBitmap(int w, int h, color_t** data, int nBitCount/*=32*/)
{
	BITMAPV4HEADER m_BMInfHeader;

	memset(&m_BMInfHeader, 0, sizeof(m_BMInfHeader));
	m_BMInfHeader.bV4Size = sizeof(BITMAPV4HEADER);
	m_BMInfHeader.bV4Width = w;
	m_BMInfHeader.bV4Height = -h;
	m_BMInfHeader.bV4Planes = 1;
	m_BMInfHeader.bV4BitCount = nBitCount;
	m_BMInfHeader.bV4V4Compression = BI_RGB;
	m_BMInfHeader.bV4SizeImage = 0;
	m_BMInfHeader.bV4XPelsPerMeter = 0;
	m_BMInfHeader.bV4YPelsPerMeter = 0;
	m_BMInfHeader.bV4ClrUsed = 0;
	m_BMInfHeader.bV4ClrImportant = 0;
	m_BMInfHeader.bV4RedMask = 0x00FF0000;
	m_BMInfHeader.bV4GreenMask = 0x0000FF00;
	m_BMInfHeader.bV4BlueMask = 0x000000FF;
	m_BMInfHeader.bV4AlphaMask = 0xFF000000;

	return CreateDIBSection(NULL, (BITMAPINFO*)&m_BMInfHeader, DIB_RGB_COLORS, (void**)data, NULL, 0);
}


//---------------------------------------------------
HBITMAP cyiBitmapBlender::CreateHBitmapV5(int w, int h, Bitmap* bmp)
{
    // ビットマップハンドルの作成
	color_t* data;
    BITMAPV5HEADER bi;

    ZeroMemory(&bi,sizeof(BITMAPV5HEADER));
    bi.bV5Size        = sizeof(BITMAPV5HEADER);
    bi.bV5Width       = w;
    bi.bV5Height      = -h;
    bi.bV5Planes      = 1;
    bi.bV5BitCount    = 32;
    bi.bV5Compression = BI_BITFIELDS;
    bi.bV5RedMask     = 0x00FF0000;
    bi.bV5GreenMask   = 0x0000FF00;
    bi.bV5BlueMask    = 0x000000FF;
    bi.bV5AlphaMask   = 0xFF000000; 

    HBITMAP hBmp =  CreateDIBSection(0, (BITMAPINFO *)&bi, DIB_RGB_COLORS, (void **)&data, NULL, (DWORD)0);

	if (hBmp==0)
		return 0;

	if (bmp)
	{
		BitmapData bmpdata;

		bmp->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &bmpdata);
		color_t* pSrc = (color_t*)bmpdata.Scan0;
		memcpy(data, pSrc, (w*h)<<2);
		bmp->UnlockBits(&bmpdata);
	}
	
	return hBmp;
}


//---------------------------------------------------
HDC cyiBitmapBlender::CreateMemoryDC(int w, int h, color_t** data, int nBitCount)
{
	HDC hdc = ::CreateCompatibleDC(NULL);
	HBITMAP hBmp = data?CreateHBitmap(w, h, data, nBitCount):CreateCompatibleBitmap(hdc, w, h);
	
	if (hBmp==NULL)
	{
		return 0;
	}

	if (hdc==NULL)
	{
		return 0;
	}

	SelectObject(hdc, hBmp);

	return hdc;
}

//---------------------------------------------------
void cyiBitmapBlender::DestroyMemoryDC(HDC hdc)
{
	HBITMAP hBitmap=(HBITMAP)::GetCurrentObject(hdc, OBJ_BITMAP);
	::DeleteObject(hBitmap);
	DeleteDC(hdc);
}

//---------------------------------------------------
Bitmap* cyiBitmapBlender::Bitmapx(int w, int h, color_t* data, int depth)
{
	PixelFormat format;

	if (depth==32)
		format=PixelFormat32bppARGB;
	else if (depth=24)
		format=PixelFormat24bppRGB;
	else if (depth=16)
		format=PixelFormat16bppARGB1555;
	else
		return 0;

	Bitmap* bmp = new Bitmap(w, h, format);
	BitmapData bmpdata;

	bmp->LockBits(0,ImageLockModeWrite, format, &bmpdata);
	color_t* pSrc = (color_t*)bmpdata.Scan0;

	if (depth==32)
		memcpy(pSrc, data, (w*h)<<2);
	else if (depth==24) // ダミー領域の計算は？（てきとー）
		memcpy(pSrc, data, (w*h)*3);
	else if (depth==16)
		memcpy(pSrc, data, (w*h)<<1);

	bmp->UnlockBits(&bmpdata);
	return bmp;
}

int xg_nBitDepth=-1;

//---------------------------------------------------
Bitmap* cyiBitmapBlender::CreateBitmap(HDC hdcsrc, int x, int y, int w, int h)
{
	color_t* colors;
	HDC hdc = ::CreateCompatibleDC(NULL);

	if (hdc==0)
		return 0;

	if (xg_nBitDepth==-1)
	{
		DEVMODE mode;
		if (EnumDisplaySettings(0, ENUM_CURRENT_SETTINGS, &mode)==True)
		{
			xg_nBitDepth=mode.dmBitsPerPel;
		}
	}
	
	HBITMAP hBmp = cyiBitmapBlender::CreateHBitmap(w, h, &colors, xg_nBitDepth!=-1?xg_nBitDepth:32);

	if (hBmp==0)
		return 0;

	SelectObject(hdc, hBmp);

	if (BitBlt(hdc, 0, 0, w, h, hdcsrc, x, y, SRCCOPY)==FALSE)
	{
		DeleteObject(hBmp);
		DeleteDC(hdc);
		return 0;
	}

	Bitmap* ret = cyiBitmapBlender::Bitmapx(w, h, colors, xg_nBitDepth!=-1?xg_nBitDepth:32);

	DeleteObject(hBmp);
	DeleteDC(hdc);

	return ret;

}

//---------------------------------------------------
Bitmap* cyiBitmapBlender::CopyBmpFromHDC(HDC hdc, int nSrcX, int nSrcY, int nDestX, int nDestY, int nDestW, int nDestH)
{
	Graphics g(hdc);
	return 0;
}

//---------------------------------------------------
Bitmap* cyiBitmapBlender::Desolve(Bitmap* bmp, Bitmap* bmp2, float factor, eDesolveType type)
{
	int w, h;

	if (bmp==0&&
		bmp2==0)
		return 0;

	if (bmp==0)
	{
		return cyiBitmapBlender::Transparent(bmp2, factor);
	}

	if (bmp2==0)
	{
		return cyiBitmapBlender::Transparent(bmp, 1.0-factor);
	}

	if ((w=bmp->GetWidth())!=bmp2->GetWidth())
		return 0;

	if ((h=bmp->GetHeight())!=bmp2->GetHeight())
		return 0;

	if (factor==0.0)
	{
		return BitmapCopy(0, bmp);
	}

	if (factor==1.0)
	{
		return BitmapCopy(0, bmp2);
	}


	Bitmap* ret = BITMAP32(w, h);

	float factor2=1.0-factor;
	BitmapData bmpdata1;
	BitmapData bmpdata2;
	BitmapData retdata;

	bmp->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &bmpdata1);
	color_t* pSrc1 = (color_t*)bmpdata1.Scan0;
	bmp2->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &bmpdata2);
	color_t* pSrc2 = (color_t*)bmpdata2.Scan0;
	ret->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &retdata);
	color_t* pRet = (color_t*)retdata.Scan0;


	int wx=0;
	for (int j=0; j<h; j++)
	{
		for (int i=0; i<w; i++)
		{
			int iwx=wx+i;
			int as=__COLOR_Ap__(pSrc1[iwx]);
			int rs=__COLOR_Rp__(pSrc1[iwx]);
			int gs=__COLOR_Gp__(pSrc1[iwx]);
			int bs=__COLOR_Bp__(pSrc1[iwx]);
			int as2=__COLOR_Ap__(pSrc2[iwx]);
			int rs2=__COLOR_Rp__(pSrc2[iwx]);
			int gs2=__COLOR_Gp__(pSrc2[iwx]);
			int bs2=__COLOR_Bp__(pSrc2[iwx]);

			if (as==0)
			{
				as = ((float)as*factor2+(float)as2*factor);
				rs = rs2;
				gs = gs2;
				bs = bs2;
			}
			else if (as2==0)
			{
				as = ((float)as*factor2+(float)as2*factor);
			}
			else
			{
				as = ((float)as*factor2+(float)as2*factor);
				rs = ((float)rs*factor2+(float)rs2*factor);
				gs = ((float)gs*factor2+(float)gs2*factor);
				bs = ((float)bs*factor2+(float)bs2*factor);
			}

			pRet[iwx]=__COLORp__(as, rs, gs, bs);
		}
		wx+=w;
	}
	bmp->UnlockBits(&bmpdata1);
	bmp2->UnlockBits(&bmpdata2);
	ret->UnlockBits(&retdata);

	return ret;

	//Bitmap* ret = BITMAP32(w, h);
	//Bitmap* bmpd1 = cyiBitmapBlender::Transparent(bmp, 1.0-factor);
	//Bitmap* bmpd2 = cyiBitmapBlender::Transparent(bmp2, factor);

	//BitmapCopy(ret, bmpd1, 0, 0);

	//Graphics g(ret);
	//g.DrawImage(bmpd2, 0, 0);

	return ret;

}

//---------------------------------------------------
Bitmap* cyiBitmapBlender::Average(Bitmap* bmp, Bitmap* bmp2, float factor)
{
	int w, h;

	if (bmp==0&&
		bmp2==0)
		return 0;

	if (bmp==0)
	{
		return cyiBitmapBlender::Transparent(bmp2, factor);
	}

	if (bmp2==0)
	{
		return cyiBitmapBlender::Transparent(bmp, 1.0-factor);
	}

	if ((w=bmp->GetWidth())!=bmp2->GetWidth())
		return 0;

	if ((h=bmp->GetHeight())!=bmp2->GetHeight())
		return 0;



	if (factor==0.0)
	{
		return BitmapCopy(0, bmp);
	}

	if (factor==1.0)
	{
		return BitmapCopy(0, bmp2);
	}

	Bitmap* ret = BITMAP32(w, h);

	BitmapData bmpdata, bmpdata2, destdata;
	bmp->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &bmpdata);
	color_t* pBmp = (color_t*)bmpdata.Scan0;

	bmp2->LockBits(0,ImageLockModeRead, PixelFormat32bppARGB, &bmpdata2);
	color_t* pBmp2 = (color_t*)bmpdata2.Scan0;

	ret->LockBits(0,ImageLockModeWrite, PixelFormat32bppARGB, &destdata);
	color_t* pDest = (color_t*)destdata.Scan0;

	int ih=0;
	for (int i=0; i<h; i++)
	{
		for (int j=0; j<w; j++)
		{
			float a, r, g, b;
			float a2, r2, g2, b2;

			a = (__COLOR_A__(pBmp[j+ih]))*factor;
			a2 = (__COLOR_A__(pBmp2[j+ih]))*(1.0-factor);
			r = (__COLOR_R__(pBmp[j+ih]))*factor;
			r2 = (__COLOR_R__(pBmp2[j+ih]))*(1.0-factor);
			g = (__COLOR_G__(pBmp[j+ih]))*factor;
			g2 = (__COLOR_G__(pBmp2[j+ih]))*(1.0-factor);
			b = (__COLOR_B__(pBmp[j+ih]))*factor;
			b2 = (__COLOR_B__(pBmp2[j+ih]))*(1.0-factor);

			pDest[j+ih]=__COLORp__((int)(a+a2), (int)(r+r2), (int)(g+g2), (int)(b+b2));
		}
		ih += w;
	}

	bmp->UnlockBits(&bmpdata);
	bmp2->UnlockBits(&bmpdata2);
	ret->UnlockBits(&destdata);

	//Graphics g(ret);
	//g.DrawImage(bmpd2, 0, 0);

	return ret;
}
