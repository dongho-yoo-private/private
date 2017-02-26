#pragma once
#include "yctrlbase.h"
#include "cyctrlxII.h"

enum eTextxType {
	eTXT_TYPE_NORMAL,
	eTXT_TYPE_PASSWORD
};

class cyTextx :
	public cyctrlbase
{
public:
	cyTextx(void);
	~cyTextx(void);

	// -------------------------------
	Bool Create(cyctrlbase* ctrl, int id, int x, int y, int w, int nMaxLen, eTextxType type, char* buffer, Bool bIsShow=False, Bool bIsReadOnly=False);

	// -------------------------------
	Bool Create(cyctrlbase* ctrl, int id, const wchar_t* pszLabel, int w, int nMaxLen, eTextxType type, char* buffer, Bool bIsShow=False, Bool bIsReadOnly=False, int distance=0);

	// -------------------------------
	void SetText(const char* pszText);

	// -------------------------------
	char* GetText(char* pszText);

	// -------------------------------
	void SelectAll();

	// -------------------------------
	virtual LRESULT OnCreate(WPARAM wp, LPARAM lp);

	// -------------------------------
	virtual Bool OnCtrlxDraw(int id, int subId, Bitmap* srf, Bitmap* bg, Font* fnt, sControlxII* s, eControlxEventII eventId);

	// ------------------------------------
	virtual void Move(UINT x, UINT y);

	// -------------------------------
	LRESULT OnTxtCommand(HWND hWnd, int cmdId);

	// ---------------------------------------
	void SetFocus();
	

	DEFINE_COMMANDS_BEGIN
		COMMAND(OnTxtCommand, 1024);
	DEFINE_COMMANDS_END

private:
	HWND m_hText;
	int m_nMaxLen;
	char* m_buffer;
	eTextxType m_type;
	Bool m_bIsReadOnly;
	cyctrlxII m_ctrlx;
	wchar_t m_szLabel[256];
	int m_distance;

};
