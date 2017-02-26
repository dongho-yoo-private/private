#pragma once

#include "glsDialog.h"
#include "glsEditBox.h"
#include "glsStatic.h"


enum eGlsPlayerPannelStyle  {
	eGPPS_AGEHAMA=0x01,
	eGPPS_TIME=0x02,
	eGPPS_SEISEKI=0x04,
	eGPPS_SENSEKI2=0x08,
	eGPPS_TIME2=0x10,

	eGPPS_PLAY=(eGPPS_AGEHAMA|eGPPS_TIME|eGPPS_TIME2|eGPPS_SEISEKI|eGPPS_SENSEKI2),
	eGPPS_PLAY2=(eGPPS_AGEHAMA|eGPPS_TIME|eGPPS_TIME2|eGPPS_SEISEKI),
	eGPPS_EDIT=(eGPPS_AGEHAMA),
	eGPPS_PLAY3=(eGPPS_AGEHAMA|eGPPS_TIME|eGPPS_TIME2)
};

typedef struct {
	eGlsPlayerPannelStyle style;
	Font* font1;
	Font* font2;

}sGlsPlayerPannelDetail;

typedef struct {
	// ------------------------------------
	goid_t id;

	// ------------------------------------
	unsigned int m_nTotalTime;

	// ------------------------------------
	unsigned short m_nReadSecondCount;

	// ------------------------------------
	unsigned short m_nNormalReadSecond;

	// ------------------------------------
	unsigned short m_nReadSecond;

	// ------------------------------------
	unsigned int m_nUsedTime;

	// ------------------------------------
	unsigned int m_nStartTime;

	// ------------------------------------
	Bool m_bIsSecondRead;

	// ------------------------------------
	Bool m_bIsTimerStopped;

	// ------------------------------------
	unsigned int m_minus;

	// ------------------------------------
	unsigned int m_spinValue;

} sPlayTimerInfo;


class cglsPlayerPannel : public cglsDialog
{
public:
	cglsPlayerPannel(void);
	~cglsPlayerPannel(void);

	static void xDefaultDetail(sGlsPlayerPannelDetail* detail);

	// --------------------------------------------
	//Bool Create(HWND m_hWnd, int id, int x, int y, int w, int h, Bool bIsBlack, sGlsPlayerPannelDetail* detail);
	Bool Create(HWND hWnd, int id, int x, int y, int w, int h, Bool bIsBlack, sGoHeader* header, eGlsPlayerPannelStyle style, sGlsPlayerPannelDetail* detail);

	// --------------------------------------------
	//void SetPlayerInfo(const char* pszPlayerName, int nLevel);

	// --------------------------------------------
	void SetPlayerInfo(const char* pszPlayerName, int level, const char* pszLevel=0);

	// --------------------------------------------
	Bool IsRegistered(goid_t id);

	// --------------------------------------------
	Bool ShowTimer(goid_t id);

	// --------------------------------------------
	void StartTimer(goid_t id, Bool bIsRemainValue=False);

	// --------------------------------------------
	void StopTimer(goid_t id, unsigned int usedTime);

	// --------------------------------------------
	void SetUsedTime(goid_t id, unsigned int usedTime);

	// --------------------------------------------
	void GobanChanged(goid_t id);

	// --------------------------------------------
	Bool AddTimer(goid_t id, int nTotalTime, int normalReadSec, int readSec, int readSecCount);

	// ----------------------------------------------
	void RemoveTimer(goid_t id);

	// --------------------------------------------
	void SetDeadStone(int n, Bool bAnimation=True);

	// ------------------------------------
	int SetFont(Font* font1, Font* font2);

	// ------------------------------------
	void SetPannelStyle(eGlsPlayerPannelStyle style);

	// ------------------------------------
	void SetTotalRemainTime(int id, int time);

	// ------------------------------------
	int GetTotalUsedTime(int id);

	// ------------------------------------
	unsigned int GetRemainTime(int id);

	// ------------------------------------
	void SetRemainTime(int id, unsigned int time, Bool bIsMinus=True);

	// ------------------------------------
	LRESULT OnTimer(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ------------------------------------
	int GetSecondReadCount(int id);

	// ------------------------------------
	void SetSecondReadCount(int id, int cnt);

	// ------------------------------------
	int xGetCurrentTime(int id, int* cnt);

protected:
	// --------------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	virtual void OnItemReArranged();

	// ------------------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& handled);

	// ------------------------------------
	sPlayTimerInfo* GetTimer(goid_t id);

DEFINE_MESSAGES_BEGIN
	MESSAGE(OnTimer, WM_TIMER)
DEFINE_MESSAGES_END


private:
	// ------------------------------------
	void DrawStaticLabel();

	Font* m_font2;

	Bool m_bIsBlack;
	eGlsPlayerPannelStyle style;

	wchar_t m_szLevel[16];
	wchar_t m_szPlayerName[128];

	eGlsPlayerPannelStyle m_eStyle;

	sGoHeader* m_header;

	int m_nDeadStone;

	int m_nSelectedTimerId;

	// ------------------------------------
	cyiList* m_listTimer;

	// ------------------------------------
	Bool xCreateChild();

	//// ------------------------------------
	//Bool m_bIsSecondRead;
};
