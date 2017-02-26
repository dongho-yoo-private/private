#pragma once
#include "yi.h"
#include "GlassInterface.h"

enum eGlsImageBoxViewStyle {
	eGIBVS_NONE,	// ����ɕ\���i�X�N���[���@�\�H�j
	eGIBVS_CENTER,	// �^���ɒu���B
	eGIBVS_FIX,		// �L�΂�
	eGIBVS_TILE,	// �^�C��
	eGIBVS_TRIM,	// �g����
	eGIBVS_TRIM_BORDER_FIXED,	// �g���C���[�W�ɍ��킷�i�T�C�Y�Œ�)
	eGIBVS_TRIM2,				// �C���[�W�Řg�ɍ��킷�B
	eGIBVS_FULL					// �C���[�W���t���ɕ\���i�T�C�Y�ύX�j

};

typedef struct {
	sGlsDetail gls;
	eGlsImageBoxViewStyle eInitialStyle;
	Bool bIsClickEvent;
	Bool bIsDragAndDrop;
	Bool bIsBorderDropShadow;
	Bool bHasBorder;
	Bool bAutoResize;
	Pen* pen;
	Bool bIsImageListMode;
	Bool bIsMouseParent;
} sGlsImageBoxDetail;

class xEXPORT cglsImageBox : public cGlassInterface
{
public:
	cglsImageBox(void);
	virtual ~cglsImageBox(void);

	// ----------------------------------
	static void xDefaultDetail(sGlsImageBoxDetail* detail);

	// ----------------------------------
	Bool Create(cGlassInterface* parent, int id, int x, int y, int w, int h, Bitmap* bmp, sGlsImageBoxDetail* detail=0);

	// ----------------------------------
	void SetImage(const char* pszImagePath, eGlsImageBoxViewStyle style);

	// ----------------------------------
	void SetImage(wchar_t* pwszImagePath, eGlsImageBoxViewStyle style);

	// ----------------------------------
	void SetImage(Bitmap* bmp, eGlsImageBoxViewStyle style);

	// ----------------------------------
	void SetImage(void* raw, int size, eGlsImageBoxViewStyle style);

	// ----------------------------------
	void Clear();

	// ----------------------------------
	virtual Bool ReSize(UINT x, UINT y, Bool bIsRepaint=0);

	// ----------------------------------
	virtual void Destroy();

	// ----------------------------------
	void AppendBitmap(int id, Bitmap* bmp, Bool bDelete=False);

	// ----------------------------------
	Bitmap* GetBitmap(int id);

	// ----------------------------------
	Bitmap* SetBitmap(int id, Bitmap* bmp, Bool bDelete=False);

	// ----------------------------------
	void SelectBitmap(int id, Bool bDrawDirect=True, Bool bIsForced=False);

	// ----------------------------------
	int GetSelectedBitmap();

	// ----------------------------------
	void ClearBitmaps();

	// ----------------------------------
	Bitmap* ExportBitmap(int w, int h);
protected:
	// ----------------------------------
	LRESULT OnDropFiles(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ----------------------------------
	Bitmap* CreateImage(Bitmap* dest, Bitmap* bmp, int _w, int _h, eGlsImageBoxViewStyle style);

	// ----------------------------------
	LRESULT OnMouse(int id, WPARAM wParam, LPARAM lParam, Bool& handled);

	// ----------------------------------
	virtual LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
private:
	// ----------------------------------
	void xDrawDragAndDrop();

DEFINE_MESSAGES_BEGIN
	MESSAGE(OnDropFiles, WM_DROPFILES)
	MESSAGEEX(OnMouse, WM_MOUSEMOVE)
	MESSAGEEX(OnMouse, WM_LBUTTONDOWN)
	MESSAGEEX(OnMouse, WM_LBUTTONUP)
	MESSAGEEX(OnMouse, WM_RBUTTONDOWN)
	MESSAGEEX(OnMouse, WM_RBUTTONUP)
DEFINE_MESSAGES_END

	wchar_t m_szImagePath[256];

	unsigned int m_myFlag;

	eGlsImageBoxViewStyle m_eStyle;
	Pen* m_pen;

	cyiList* m_listBitmaps;

	int m_nCurrentSelectedId;

};
