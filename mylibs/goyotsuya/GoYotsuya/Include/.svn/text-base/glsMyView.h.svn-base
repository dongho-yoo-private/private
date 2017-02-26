#pragma once

#include "glsDialog.h"
#include "UserManager.h"

enum eUserStatus {
	eUS_PLAYING,
	eUS_NOTHING,
	eUS_ENJOY,
	eUS_NO_PLAY=0x80
};

enum eMyViewStyle {
	eMVS_NORMAL,
	eMVS_NOID
};

#define WM_ADD_GO (WM_USER + 1)

// --------------------------------
typedef struct {
	wchar_t szMyId[16];
	eUserStatus status;
	unsigned long lastLoginDay;
	unsigned long lastLoginTime;
	unsigned short win;
	unsigned short loses;
	unsigned short draws;
	unsigned int point;
	wchar_t szMyImagePath[256];
	char szGoFilePath[2][256];
	//cyiList* playList;
	//cyiList* friendList;
	//cyiList* groupList;
}sMyViewInfo;


typedef struct {
	char szUserName[16];
	unsigned int filter[5];
} sGoListFilter;

class xEXPORT cglsMyView : public cglsDialog
{
public:
	cglsMyView(void);
	~cglsMyView(void);

	Bool Create(cGlassInterface* parent, int x, int y, int w, int h, CUserManager* mgr, Font* font=0, eMyViewStyle style=eMVS_NORMAL);

	// ------------------------------------
	void SetGroupChangedId(int id);

	// ------------------------------------
	void SetPlayCommandId(int id);

	// ------------------------------------
	void SetWatchCommandId(int id);

	// ------------------------------------
	void UpdateMyView(eMyViewStyle style=eMVS_NORMAL);

	// ------------------------------------
	void UpdateGo(int index, int i, sSimpleYgfHeader* header, Bool bIsUpdate=True);

	// ------------------------------------
	void AddGo(int index, sSimpleYgfHeader* header, Bool bIsUpdate=True);

	// ------------------------------------
	void AddGo(int index, sSimpleGoHeader* header, Bool bIsUpdate=True);

	// ------------------------------------
	virtual int SetFont(Font* font, Bool bIsReference=True);


protected:
	// ------------------------------------
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

	// ------------------------------------
	virtual void OnItemReArranged();

	// ------------------------------------
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ------------------------------------
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

	// ------------------------------------
	virtual void OnThreadStart(int id, int param1, int param2);

	// ------------------------------------
	void OnLoadMyGoData();

	// ------------------------------------
	//void OnAdjustFilter();

	// ------------------------------------
	LRESULT OnAddGo(WPARAM wParam, LPARAM lParam, Bool& bIsHandled);

private:
	int m_nGroupChangedCommand;
	int m_nPlayCommand;
	int m_nWatchCommand;

	CUserManager* m_mgr;
	//sMyViewInfo m_myinfo;

	cyiList* m_listGo[2];

	// ------------------------------------
	sGoListFilter m_filter[2];

	// ------------------------------------
	Bool m_bIsQuitThread[2];

	sSimpleYgfHeader* m_ygf_header;

	eMyViewStyle m_eStyle;

	DEFINE_MESSAGES_BEGIN
		MESSAGE(OnAddGo, WM_ADD_GO)
	DEFINE_MESSAGES_END

};
