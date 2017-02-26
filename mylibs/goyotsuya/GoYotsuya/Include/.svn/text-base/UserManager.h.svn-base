#pragma once

#include "yistd.h"
#include"svry/svrydefs.h"

typedef struct {
	unsigned int id;
	sRegisterUserInfo* reg;
	sSimpleGoHeader* hd;
	unsigned short score[4]; /* wins, loses, draws, level*/
	Bitmap* bmp;
	unsigned int seq_bmp;
	int goHeaderSize;
	cyiList* listGo;
} sUserData;

typedef struct {
	unsigned int id;
	//unsigned int hash;
	unsigned int groupId; // 0: friend, 0x00000100: 一般対局室、0x00010000: 自由対局室
	char szName[16];
} sUserDictionary;

typedef struct {
	int n;
	sUserDictionary* user;
} sUserDictionaryFile;

// --------------------------------------
class CUserManager
{
public:
	CUserManager(void);
	~CUserManager(void);

	// --------------------------------------
	static CUserManager* GetInstance();

	// --------------------------------------
	static void SetInstance(CUserManager* instance);

	// --------------------------------------
	Bool Initailize(unsigned int myId, const char* pszMyName, const char* pszUserDir, 
		unsigned int server_addr, unsigned int udpPort, unsigned int nRoomNo=0);

	// --------------------------------------
	virtual Bool RegisterNotifyWindow(HWND hWndNotifyWnd, UINT msg);

	// --------------------------------------
	void LoadRegisterInfo(unsigned int userId);

	// --------------------------------------
	virtual sRegisterUserInfo* GetRegisterInfo(unsigned int userId);

	// --------------------------------------
	virtual void GetMyScore(unsigned int* pScore);

	// --------------------------------------
	void SetRegisterInfo(sRegisterUserInfo* info);

	// --------------------------------------
	int AddPlayingGoHeader(unsigned int id, sSimpleGoHeader* header);

	// --------------------------------------
	void DeletePlayingGoHeader(goid_t id, int index);

	// --------------------------------------
	void DeletePlayingGoHeader(goid_t id, ugoid_t uid);

	// --------------------------------------
	virtual cyiList* GetPlayingGoHeader(goid_t id, ugoid_t* uid=0);//, int* size);

	// --------------------------------------
	virtual sSimpleGoHeader* SearchPlayingGoHeader(ugoid_t& id);

	// --------------------------------------
	sUser* LoadMyFriends();

	// --------------------------------------
	virtual sUser* GetMyFriends();

	// --------------------------------------
	void SetMyFriends(sUser* info);

	// --------------------------------------
	virtual void GetVsScore(unsigned int id, int& wins, int& loses, int& draws);

	// --------------------------------------
	virtual int GetMyFriendsCount();

	// --------------------------------------
	//virtual Bool GetUserName(unsigned int id, char* szUserName);

	// --------------------------------------
	unsigned int GetMyFriendId(const char* pszUserName);

	// --------------------------------------
	int GetMyFriendIndex(unsigned int id);
	
	// --------------------------------------
	sMsg32* LoadMyMsg32();

	// --------------------------------------
	virtual sMsg32* GetMyMsg32(int* n=0);

	// --------------------------------------
	void SetMyMsg32(sMsg32* msg);

	// --------------------------------------
	int AddMsg32(sMsg32* msg);

	// --------------------------------------
	void DeleteMsg32(int index);

	// --------------------------------------
	Bool AppendGoHeader(unsigned int id, sSimpleGoHeader* header, int cnt);

	// --------------------------------------
	virtual sSimpleGoHeader* GetGoHeader(unsigned int id, int* size);

	// --------------------------------------
	void LoadGoHeader(unsigned int id, int* size);

	// --------------------------------------
	void LoadPicture(unsigned int userId);

	// --------------------------------------
	int GetPictureSequenceNo(unsigned int id);

	// --------------------------------------
	virtual Bool IsMyFriend(unsigned int userId);

	// --------------------------------------
	virtual Bool IsGroupUser(unsigned int userId);

	// --------------------------------------
	virtual Bitmap* GetPicture(unsigned int userId, unsigned int& seq);

	// --------------------------------------
	void* GetPictureFile(unsigned int userId, int* size);

	// --------------------------------------
	void SetPictureFile(unsigned int userId, void* data, int size, unsigned int seq);

	// --------------------------------------
	virtual void SetPicture(unsigned int userId, Bitmap* bmp, unsigned int seq);

	// --------------------------------------
	void LoadScore(unsigned int userId);

	// --------------------------------------
	virtual Bool GetScore(unsigned int userId, int& wins, int& loses, int& draw);

	// --------------------------------------
	void  SetScore(unsigned int userId, int win, int loses, int draw);

	// --------------------------------------
	void  SetScore(unsigned int userId, unsigned int* score);

	// --------------------------------------
	Bool UpdateFriend();

	// --------------------------------------
	Bool LoadSequenceNo();

	// --------------------------------------
	Bool UpdateSequenceNo(int nFileNo);


	// --------------------------------------
	virtual unsigned int GetMyId();

	// --------------------------------------
	virtual const char* GetMyName();

	// --------------------------------------
	virtual const char* GetGoFilePath(int index);
	
	// --------------------------------------
	void SetGoFilePath(int index, const char* pszFilePath);

	// --------------------------------------
	void SetMyRoomNo(int nRoomNo);

	// --------------------------------------
	virtual sBaseUserInfo* GetMyBaseInfo();

	// --------------------------------------
	Bool SaveMyBaseInfo();

	// --------------------------------------
	// ログイン時に一度のみ設定される。
	void SetMyBaseInfo(sBaseUserInfo* info);

	// --------------------------------------
	sBaseUserInfo* LoadBaseInfo();

	// --------------------------------------
	virtual Bool AddUserHandle(Handle hUser);

	// --------------------------------------
	void RemoveUserHandle(unsigned int id);

	// --------------------------------------
	virtual Handle GetUserHandle(unsigned int id);

	// --------------------------------
	Bool QueryUserName(unsigned int id, char* pszOutUserName);

	// --------------------------------
	Bool LoadDictionary();

	// --------------------------------
	Bool AddDictionary(unsigned int id, const char* pszUserName, int flag);

	// --------------------------------
	virtual const char* GetUserName(unsigned int id);

	// --------------------------------
	unsigned int GetUserId(const char* pszUserName, Bool bSearchNetwork);

	// --------------------------------
	// 接続待ち・予定のあるデータ
	int AddWaitGoHeader(sSimpleGoHeader* data);

	// --------------------------------
	// 接続待ち・予定のあるデータ
	sSimpleGoHeader* GetWaitGoHeader(ubit32_t id, int index=0);

	// --------------------------------
	// 接続待ち・予定のあるデータ
	sSimpleGoHeader* GetWaitGoHeader(ubit32_t id, ugoid_t& uid);

	// --------------------------------
	void RemoveWaitGoHeader(ubit32_t id, ugoid_t& uid);

	// 待ち碁をロード
	void SaveWaitGoHeader();

	// --------------------------------
	void LoadWaitGoHeader();

	// --------------------------------
	sSimpleGoData* GetGoData(ugoid_t& uid, int& size, Bool bIsWaitGo=False);

private:
	// --------------------------------------
	sUserData* GetUserData(unsigned int id);

	// --------------------------------------
	Bool NotifyMessage(WPARAM wParam, LPARAM lParam);

	static CUserManager* me;

	//// --------------------------------------
	unsigned int m_myId;

	// --------------------------------------
	char m_szUserDir[256];
	// --------------------------------------
	char m_szMyName[16];
	// --------------------------------------
	char m_szMyDir[256];

	//// --------------------------------------
	//HWND m_hNotifyWnd;

	//// --------------------------------------
	//UINT m_msg;

	// --------------------------------------
	cyiList* m_listFriend;

	// --------------------------------------
	cyiList* m_listGroupUser;

	// --------------------------------------
	cyiList* m_listNotify;

	// --------------------------------------
	cyiList* m_listUserHandle;

	// --------------------------------------
	cyiList* m_listWaitGo;

	// --------------------------------------
	sRegisterUserInfo* m_myRegisterInfo;

	// --------------------------------------
	sBaseUserInfo* m_base;

	// --------------------------------------
	sUser* m_myFriend;

	// --------------------------------------
	sMsg32* m_myMsg32;

	// --------------------------------------
	sSimpleGoHeader* m_pGoHeader;

	// --------------------------------------
	Bitmap* m_myBmp;

	// --------------------------------------
	unsigned int m_nBmpSeqNo;

	// --------------------------------------
	char m_szGoFilePath[2][128];

	// --------------------------------------
	unsigned int m_nRoomNo;

	unsigned int m_nGoHeaderSize;

	unsigned int m_udpPort;

	unsigned int m_server_addr;

	sUserDictionaryFile m_dictionary;

	friend class CGoEditorApp;
};
