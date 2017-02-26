#pragma once

#include "glsGaussPannel.h"

enum eSlideBarScrollDirection
{
	eSBSD_TOLEFT,
	eSBSD_TORIGHT,
	eSBSD_TOUP,
	eSBSD_TODOWN
};

enum eSlideBarMoveStyle
{
	eSBMS_NONE,
	eSBMS_UP,
	eSBMS_DOWN,
	eSBMS_LEFT,
	eSBMS_RIGHT
};

typedef struct {
	Bool bShowButton;
	Bool bRingBuffer;
	Bool bShowAutoScroll;
	int  nMillSecTerm;
	int nMillisecInterval;
	eSlideBarMoveStyle style;
} sGlsSlideDetail;

#define SLIDE_SCROLL_MOVE_NONE 0
#define SLIDE_SCROLL_MOVE_CUR  0xFFFFFFFF

#define WM_SLIDE_ANIMATION (WM_USER+1)

//class cglsControl;

class xEXPORT cglsSlide : public cGlassInterface
{
public:
	cglsSlide(void);
	~cglsSlide(void);

	// -------------------------------------------------------
	void xDefaultDetail(sGlsSlideDetail* detail);

	// -------------------------------------------------------
	// 当然ながらFixedモードで動く。
	Bool Create(cGlassInterface* parent, int id, int x, int y, int w, int h, sGlsSlideDetail* detail=0);//cglsDrawObject* obj/*必須*/, sGlsSlide* detail=0);
	
	// -------------------------------------------------------
	virtual void Destroy();

	// -------------------------------------------------------
	int AddControl(cGlassInterface* ctrl, eGlsItemAlign align);

	//// -------------------------------------------------------
	//int InsertControl(cGlassInterface* ctrl, int index, eGlsItemAlign align);

	// -------------------------------------------------------
	void RemoveControl(int id);

	// -------------------------------------------------------
	//virtual Bool ReSize(unsigned int w, unsigned int y);

	// -----------------------------
	cGlassInterface* GetCurrentControl();

	// -----------------------------
	cGlassInterface* GetControl(int id);

	// -------------------------------------------------------
	int Scroll(int to, eSlideBarScrollDirection dir=eSBSD_TOLEFT, eGlsAnimationPattern pattern=eGAP_FISO, 
		int nMillSecTer=15, int nMillisecInterval=300);

	// -----------------------------
	LRESULT OnSlideAnimation(WPARAM wParam, LPARAM lParam, Bool& handled);

	// -----------------------------
	Bitmap* xMakeSlideImage(Bitmap* bmp, Bitmap* bmpFrom, Bitmap* bmpTo, float factor, eSlideBarScrollDirection dir);

	// -----------------------------
	virtual void Update(eGlsUpdateMode mode, eDrawItemStatus status=eDIS_NORMAL);
	
	// -----------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

#ifdef _DEBUG
	virtual void Update(Bitmap* bmp, RECT* rect, RECT* _childRect, int offsetX, int offsetY, Bool bIsLocalBufferUpdate);
#endif

protected:
	//// -----------------------------
	//Bitmap* GetBgBitmap();

	DEFINE_MESSAGES_BEGIN
		MESSAGE(OnSlideAnimation, WM_SLIDE_ANIMATION) 
	DEFINE_MESSAGES_END
private:
	cyiList*	m_ctrlz;
	ubit32_t	m_xflag;

	ubit32_t  m_flagSlide;

	int m_nSelectedControlId;

	int   m_fromId;
	int   m_toId;

	eSlideBarScrollDirection m_eDir;
	eGlsAnimationPattern m_ePattern;
	int m_nSlideTerm;
	int m_nSlideInterval;

	Bitmap* m_bmpParentBg;
	Bitmap* m_bmpFrom;
	Bitmap* m_bmpTo;

	HandleAnit m_hSlideAnit;

};
