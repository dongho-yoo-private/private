#pragma once

#include "glsStackPannel.h"


enum eGobanControlSet {
	eGCS_BASE,		// |< << < > >> >| b> b< n auto pass
	eGCS_EDITOR,	// delete, ラベル, 置き石, 地合い計算、コメント入力
	eGCS_PLAY_INFO, // 白：名前 あげいし：　残り時間：　などのコントロール
	eGCS_PLAY,		// パス、投了、待った、キャンセル、うちかけ
	eGCS_CALCULATE, // 地合い計算終了、地合い計算キャンセル
	eGCS_GOMSG,
	eGCS_MAX
};

enum eGobanControlCommand {
	eGCC_TOP = 0x1000,
	eGCC_PP,
	eGCC_PREV,
	eGCC_NEXT,
	eGCC_NN,
	eGCC_END,
	eGCC_SHOW_NO,
	eGCC_NEXT_BRANCH,
	eGCC_PREV_BRANCH,
	eGCC_LABEL,
	eGCC_ADDSTONE,
	eGCC_AUTOPLAY,
	eGCC_DELETE_NODE,
	eCGS_CALCULATE_END,
	eCGS_CALCULATE_STOP,
	eGCC_PASS,
	eGCC_WAIT,
	eGCC_HUU,
	eGCC_PAUSE,
	eGCC_CANCEL,
	eGCC_CLOSE,
	eGCC_GIVEUP,
	eGCC_STUDY_END,
	eGCC_ONE_MORE_TIME,
	eGCC_COPY
};




// -------------------------------------------
class cglsGobanControl : public cglsStackPannel
{
public:
	cglsGobanControl(void);
	~cglsGobanControl(void);

	// -------------------------------------------------
	static cglsGobanControl* GetInstance(eGobanControlSet set);

	// -------------------------------------------------
	Bool Create(cGlassInterface* parent, int id, int w, int h, eGobanControlSet set, Font* font=0);

protected:
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);


private:
	// -----------------------------------------
	void xAddButton(int id, int bitmapId, wchar_t* pszText, wchar_t* comment, int systemId, Bool bIsOnOff=False, int style=eGGS_NEON);

	// -----------------------------------------
	void xAddSplitButton(int id, int systemId, void* p, int width, const wchar_t* pszToolTip);

	// -----------------------------------------
	Bool xCreateBase();

	// -----------------------------------------
	Bool xCreateEditor();

	eGobanControlSet m_set;
};
