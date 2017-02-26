#pragma once

#include "yistd.h"

// --------------------------------------------
enum eDrawItemStatus {
	eDIS_NORMAL=0,
	eDIS_MOUSE_MOVE,
	eDIS_MOUSE_DOWN,
	eDIS_MOUSE_UP,
	eDIS_MAX_STATUS,
	eDIS_MOUSE_LEAVE,
	eDIS_MOUSE_ENTER,
	eDIS_HILIGHT,
	eDIS_FREE,
	eDIS_FREE2,
	eDIS_PREV=0x0F,
	eDIS_FORCED=0xFF,
	eDIS_INVALID=-1
};

// --------------------------------------------
enum eItemAlign {
	eIA_LEFT,
	eIA_CENTER,
	eIA_RIGHT,
	eIA_TOP=eIA_LEFT,
	eIA_VCENTER=eIA_CENTER,
	eIA_BOTTOM=eIA_RIGHT
};

// --------------------------------------------
enum eStackAlign {
	eSA_LEFT,
	eSA_CENTER,
	eSA_RIGHT,
	eSA_CENTER2,
	eSA_TOP=eSA_LEFT,
	eSA_VCENTER=eSA_CENTER,
	eSA_BOTTOM=eSA_RIGHT
};

// --------------------------------------------
enum eStackPannelType {
	eSPT_NORMAL,
	eSPT_CLICKED,
	eSPT_STATIC,
	eSPT_GLASSINTERFACE,
	eSPT_SCROLLBAR,
	eSPT_LF,
	eSPT_SPACE
};

// ------------------------------------
enum eGlsItemAlign {
	eGIA_LEFT=0,
	eGIA_CENTER=1,
	eGIA_RIGHT=2,
	eGIA_TOP=0x00,
	eGIA_VCENTER=0x10,
	eGIA_BOTTOM=0x20
};

// --------------------------------------------
typedef struct xsGlsItemDetail {
	eStackPannelType eType;
	eStackAlign eAlign;
	RECT rect;
	void* param;
	Bool bIsDragAble;
	Bool bIsTransparant;
	Bool bInitVisible;
	float fEnterTransparency;
	float fLeaveTransparency;
	color_t colorFont;
	color_t colorBmp;
} sGlsItemDetail;

#define xIS_ITEM_DISABLE(item) (item->flag&0x80000000)
#define xITEM_DISABLE(item) (item->flag|=0x80000000)
#define xITEM_ENABLE(item) (item->flag&=(~0x80000000))

#define xITEM_PUSHED(item) (item->flag|=(1<<26))
#define xITEM_RELEASED(item) (item->flag&=(~(1<<26)))
#define xIS_ITEM_PUSHED(item) (item->flag&(1<<26))

#define xITEM_SET_DRAWING(item) (item->flag|=(1<<25))
#define xITEM_RELEASE_DRAWING(item) (item->flag&=(~(1<<25)))
#define xIS_ITEM_DRAWING(item) (item->flag&(1<<25))

#define xIS_ITEM_FREE(item) (item->flag&(1<<23))
#define xSET_ITEM_FREE(item) (item->flag|=(1<<23))
#define xRELEASE_ITEM_FREE(item) (item->flag&=(~(1<<23)))

#define xIS_ITEM_HIDE(item) (item->flag&(1<<22))
#define xSET_ITEM_HIDE(item) (item->flag|=(1<<22))
#define xSET_ITEM_SHOW(item) (item->flag&=(~(1<<22)))

#define xSET_ITEM_DRAG_ON(item) (item->flag|=(1<<21))
#define xSET_ITEM_DRAG_OFF(item) (item->flag&=(~(1<<21)))
#define xIS_ITEM_DRAG_ON(item) (item->flag&(1<<21))

#define xIS_ITEM_ANIMATION_NOTIFY_ONLY(item) (item->flag&(1<<30))
#define xSET_ITEM_ANIMATION_NOTIFY_ONLY_ON(item) (item->flag|=(1<<30))
#define xSET_ITEM_ANIMATION_NOTIFY_ONLY_OFF(item) (item->flag&=(~(1<<30)))

#define xIS_ITEM_ABS_COORD(item) (item->flag&(1<<29))
#define xSET_xIS_ITEM_ABS_COORD_ON(item) (item->flag|=(1<<29))
#define xSET_xIS_ITEM_ABS_COORD_OFF(item) (item->flag&=(~(1<<29)))

#define xSET_ITEM_TYPE(item, type) (item->flag|=((bit32_t)type<<18))
#define xGET_ITEM_TYPE(item) (eStackPannelType)((item->flag>>18)&0x07)
#define xITEM_TYPE_CLEAR(item) (item->flag&=~(0x07<<18))

#define xSET_ITEM_ALIGN(item, type) (item->flag|=(type<<27))
#define xGET_ITEM_ALIGN(item) (eStackAlign)(((item->flag>>27)&0x03))
#define xITEM_ALIGN_CLEAR(item) (item->flag&=~(0x03<<27))

#define xSET_ITEM_STATUS(item, status) (item->flag = (item->flag&~(0x0000000F)) | status)
#define xGET_ITEM_STATUS(item) (eDrawItemStatus)(item->flag &0x0F)

#define xSET_ITEM_PRE_STATUS(item, status) (item->flag = (item->flag&~(0x000000F0)) | (status<<4))
#define xGET_ITEM_PRE_STATUS(item) (eDrawItemStatus)((item->flag>>4) &0x0F)



// ----------------------------------------------
enum eGlsAnimationType {
	eGAT_MOVE = 0x01,
	eGAT_CROSS = 0x02,
	eGAT_BOTH = eGAT_MOVE|eGAT_CROSS
};



// ----------------------------------------------
typedef struct xsGlsAnimation {
	eGlsAnimationType		eType;
	unsigned long			nAnimationStartTime;
	Bitmap*					pStartBmp;
	Bitmap*					pEndBmp;
	Bitmap*					pAnimationBmp;
}sGlsAnimation;

enum eGlsAnimationFlag {
	eGAF_TRANSPARENT=0x01,
	eGAF_SIZE=0x02,
	eGAF_POS=0x04,
	eGAF_DESOLVE=0x08
};

typedef struct {
	eGlsAnimationFlag flag;
	unsigned int nAnimationStartTime;

	float fToTransparency;
	float fFromTransparency;
	eGlsAnimationPattern eTrPattern;

	float fToSizeRate;
	float fFromSizeRate;
	int					align;
	eGlsAnimationPattern eSzPattern;
	
	POINT ptStart;
	POINT ptEnd;
	eGlsAnimationPattern eMvPatternX;
	eGlsAnimationPattern eMvPatternY;

	int   nBmpStartIndex;
	int	  nBmpEndIndex;
	eGlsAnimationPattern eDvPattern;

	unsigned int AnimationLoopInterval; // -> Animation Once ÇÃèÍçáÇÕ0xFFFFFFFF
	Bitmap* pBmpAnimation;
	Bitmap* pBmpAborted;
	int nLastIndex;
}sGlsAnimationEx;


class cGlassInterface;

// ----------------------------------------------
typedef struct {
	int id;
	wchar_t* txt;
	int distance;
	IntPtr bitmapId;
	yiRect rect;
	bit32_t flag; // 31: disabled, 30: AnimationNotifyOnly, 29: Coordinate is absolute (for fixed mode) 28-27: eStackAlign, 26: pushed, 25:drawing, 24: bg hitted ok. 23:free object, 22:visible 21:dragable 20-18:eStackPannelType 15:selected 0-3:eDrawItemStatus, 4-7: eDrawItemStatus
	Bitmap* pBmp[eDIS_MAX_STATUS];
	Bitmap** pBmpEx;
	Bitmap* pCurrentBmp;
	int nLastItemIndex;
	color_t colorBmp;
	color_t colorFont;

	float fTransparency;
	float fSizeRate;
	float fEnterTransparency;
	float fLeaveTransparency;

	HandleAnit hAnit;
	sGlsAnimationEx* animation;

	void* param;
	cGlassInterface* gls;
} sDrawItem;

xEXPORT void glsInitAnimationEx(sGlsAnimationEx* ani, sDrawItem* item);

xEXPORT void glsSetTrAnimationEx(sGlsAnimationEx* ani, float fFrom, float fTo, eGlsAnimationPattern pattern);

xEXPORT void glsSetSzAnimationEx(sGlsAnimationEx* ani, float fFrom, float fTo, eGlsAnimationPattern pattern, eGlsItemAlign align);

xEXPORT void glsSetDvAnimationEx(sGlsAnimationEx* ani, int from, int to, eGlsAnimationPattern pattern);

xEXPORT void glsSetPtAnimationEx(sGlsAnimationEx* ani, POINT* from, POINT* to, eGlsAnimationPattern patternX, eGlsAnimationPattern patternY);


// ----------------------------------------------
//typedef struct xsDrawItemEx {
//	int id;
//	wchar_t* txt;
//	int distance;
//	int bitmapId;
//	yiRect rect;
//	void* param;
//	float fTransparency;
//	bit32_t flag; // 31: disabled, 30-29: eStackPannelType, 28-27: eStackAlign, 26: pushed, 25:drawing, 24: bg hitted ok. 0-3:eDrawItemStatus, 4-7: eDrawItemStatus
//	Bitmap* pBmp[eDIS_MAX_STATUS];
//	Bitmap* pCurrentBmp;
//
//	 for animation.
//	HandleAnit hAnit;
//	Bitmap* pStartBmp;
//	Bitmap* pEndBmp;
//	Bitmap* pAnimationBmp;
//	unsigned int nAnimationId;
//	float from;
//	float to;
//
//	Bitmap* pBmpMasked[eDIS_MAX_STATUS];
//	Bitmap* pCurrentMaskBmp;
//	Bitmap* pMaskStartBmp;
//	Bitmap* pMaskEndBmp;
//	Bitmap* pMaskAnimationBmp;
//
//} sDrawItemEx;

#define MAX_RESOURCE_COUNT 512

// ---------------------------------------
class xEXPORT cglsResourceObject
{
public:
	cglsResourceObject();
	~cglsResourceObject();

	// ---------------------------------------
	static int GetFontHeight(Font* font);

	// ---------------------------------------
	static int xGetPos(eItemAlign align, int wAll, int margin, int w);

	// ---------------------------------------
	virtual Bool Initialize()=0;

	// ---------------------------------------
	virtual Bitmap* GetBitmap(int id, int subId, IntPtr w, IntPtr h, color_t color=0, void* extra=0)=0;

	// ---------------------------------------
	virtual Bitmap* Draw(int id, int subId, int w, int h, Bitmap* bmp, Bitmap* bmpStr, void* extra)=0;

protected:
	// ---------------------------------------
	Bitmap* xGetSameObject(int id, int subId, color_t color, int w, int h);

	// ---------------------------------------
	void xSetObject(Bitmap* bmp, int id, int subId, color_t color, int w, int h);

private:
	// ---------------------------------------
	cyiQueue* m_resources[MAX_RESOURCE_COUNT];

};

// ---------------------------------------
class xEXPORT cglsDrawObject
{
public:
	cglsDrawObject(void);
	cglsDrawObject(int id);
	virtual ~cglsDrawObject(void);

	// ------------------------
	void Initialize(cglsResourceObject* resource=0, Font* font=0);

	// ------------------------
	static Bitmap* CreateGlassPannel(int w, int h, color_t color);

	// --------------------------
	virtual Bool CreateBitmap(sDrawItem* item, void* pExtra=0);

	// --------------------------
	virtual Bool OnNullBitmapDraw(sDrawItem* item, void* pExtra=0);

	// --------------------------
	virtual void DestroyBitmap(sDrawItem* item);

	// --------------------------
	Bool m_bIsTypeUserDraw;

	cglsResourceObject* m_xresource;
	Font* m_xfont;

	int m_nDrawObjectId;


};

// ---------------------------------------
class xEXPORT cglsUserDrawObject : public cglsDrawObject
{
public:
	cglsUserDrawObject(void);
	~cglsUserDrawObject(void);

//protected:
	virtual void OnDrawItem(sDrawItem* item, eDrawItemStatus status)=0;
	cyiList* m_Itemz;
};


