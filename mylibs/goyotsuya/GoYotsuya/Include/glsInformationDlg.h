#pragma once

#include "yit.h"
#include "glsDialog.h"
#include "glsEditBox.h"
#include "glsStatic.h"
#include "sgfManager.h"
#include "glsGobanCommon.h"


enum eGlsInformationDlgItemz {
	eGIDID_TXT_GAME=1<<4,
	eGIDID_TXT_GAMETIME=1<<5,
	eGIDID_TXT_PLACE=1<<6,
	eGIDID_TXT_LIMITTIME=1<<7,
	eGTRID_TXT_TEAI=1<<8,
	eGIDID_TXT_HANDICAP=1<<9,
	eGIDID_TXT_RESULT=1<<10,
	eGIDID_COUNT=7/*(eGIDID_TXT_HANDICAP2>>4)*/,
	eGIDID_ALL= (eGIDID_TXT_GAMETIME|eGIDID_TXT_PLACE|eGIDID_TXT_LIMITTIME|eGTRID_TXT_TEAI|eGIDID_TXT_HANDICAP|eGIDID_TXT_RESULT)
};

class cglsInformationDlg : public cglsDialog
{
public:
	cglsInformationDlg(void);
	~cglsInformationDlg(void);

	// ----------------------------------------
	Bool Create(HWND hwnd, int id, int x, int y, int w, int h, sGoHeader* header, eGlsInformationDlgItemz eItemzFlag=eGIDID_ALL, Font* font=0);

	// ----------------------------------------
	Bool UpdateHeader(sGoHeader* header);

	// ----------------------------------------
	void SetItemzFlag(eGlsInformationDlgItemz eItemzFlag);

	// ----------------------------------------
	virtual void Update(Bitmap* bmp, RECT* rect=0, RECT* _childRect=0, int offsetX=0, int offsetY=0, Bool bIsLocalBufferUpdate=False);
	

private:

	// ----------------------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ----------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	virtual void OnItemReArranged();

	// ------------------------------------
	void SetHeaderValue();

	// ------------------------------------
	Bool xCreateChild(sGoHeader* header);

	sGoHeader* m_header;
	int m_nLabelLayer;

	cyitrb<cglsStatic> m_listLabel;
	cyitrb<cGlassInterface> m_listValue;
	//cyitList<cglsStatic> m_listLabel;
	//cyitList<cGlassInterface> m_listValue;

	eGlsInformationDlgItemz m_eItemzFlag;
	unsigned char m_fItemzFlagz[eGIDID_COUNT];

};
