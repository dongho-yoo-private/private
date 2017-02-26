#pragma once

#include "glsDialog.h"
#include "UserManager.h"

enum eMyFriendViewUpdate {
	eMFVU_ALL=0xFF,
	eMFVU_ADD_FRIEND=0x01,
	eMFVU_DEL_FRIEND=0x02,
	eMFVU_PICTURE=0x04,
	eMFVU_GODATA=0x08,
	eMFVU_REG=0x10
};

class xEXPORT cglsMyFriendView : public cglsDialog
{
public:
	cglsMyFriendView(void);
	~cglsMyFriendView(void);

	// ------------------------------------
	Bool Create(cGlassInterface* parent, int x, int y, int w, int h, CUserManager* mgr, Font* font=0);

	// ------------------------------------
	void UpdateFriends(unsigned int id=0, int eFlag=eMFVU_ALL);

	// ------------------------------------
	void SetGroupChangedId(int id);

	// ------------------------------------
	void SetPlayCommandId(int id);

	// ------------------------------------
	void SetWatchCommandId(int id);

	// ------------------------------------
	virtual int SetFont(Font* font, Bool bIsReference=True);

	// ------------------------------------
	void AddGoHeader(int index, sSimpleGoHeader* header, Bool bIsLastUpdate=0, color_t color=Color::Black);

	// ------------------------------------
	unsigned int GetCurrentFriendId();

	// ------------------------------------
	virtual void Show();

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
	void CreateLoginBitmap(int nHeight);

private:
	// ------------------------------------
	void UpdateRegisterData(unsigned id);

	// ------------------------------------
	void UpdatePicture(unsigned id);

	// ------------------------------------
	void UpdateGoData(unsigned id);

	int m_nGroupChangedCommand;
	int m_nPlayCommand;
	int m_nWatchCommand;
	unsigned int m_currentUserId;

	CUserManager* m_mgr;

	int m_nLastSelectedUserIndex;
	int m_nRegisteredUserCnt;

	// ------------------------------------
	Bitmap* m_bmpLogin;

	// ------------------------------------
	Bitmap* m_bmpLogout;

	int m_flagUpdate;


};
