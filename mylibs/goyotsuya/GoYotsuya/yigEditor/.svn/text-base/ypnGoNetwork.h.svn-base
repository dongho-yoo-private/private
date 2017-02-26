#ifdef _GOYOTSUYA

#ifndef __ypnGoNetwork_h__
#define __ypnGoNetwork_h__
#include "ypn.h"
#include "yitList.h"
#include "svry/svrydefs.h"
#include "svry/svrydefs.register.h"
#include "svry/svrydefs.login.h"

#define MAX_CONNECT_USER_CNT 128

enum eypnGoNetworkPacketMessage {
	eGNPMSG_SEND_TEXT_MESSAGE = 1,
	eGNPMSG_REQ_PLAY,
	eGNPMSG_REPLY_PLAY,
	eGNPMSG_REQ_PLAY_START,
	eGNPMSG_SEND_STONE,
	eGNPMSG_REQ_PLAY_END,
	eGNPMSG_REQ_PLAY_END_OK,
	eGNPMSG_REQ_CALCULATE_END,
	eGNPMSG_REQ_CALCULATE_END_OK,
	eGNPMSG_REQ_INFO_DETAIL,
	eGNPMSG_RPY_INFO_DETAIL,
	eGNPMSG_GIVEUP,
	eGNPMSG_TIME_LOSE,
	eGNPMSG_WAIT,
	eGNPMSG_STOP_TIMER,
	eGNPMSG_WAIT_RESULT,
	eGNPMSG_REQ_PICTURE,
	eGNPMSG_RPY_PICTURE,
	eGNPMSG_REQ_GODATA,
	eGNPMSG_RPY_GODATA,
	eGNPMSG_REQ_DATA,
	eGNPMSG_RPY_DATA,
	eGNPMSG_SEND_CMD,
	eGNPMSG_STUDY_END,
	//eGNPMSG_STUDY_ANSWER,
	//eGNPMSG_STUDY_QUESTION,
	eGNPMSG_CURRENT_GO_LIST,
	eGNPMSG_HAGASHI,
	eGNPMSG_NOTIFY_PUTSTONE,
	eGNPMSG_START_PLAY,
	eGNPMSG_CANCEL_FORCED,
	eGNPMSG_CANCEL,
	eGNPMSG_NO_CANCEL,
	eGNPMSG_WATCH_GO_RESULT,
	eGNPMSG_WATCH_GO,
	eGNPMSG_GO_RESULT,
	eGNPMSG_WATCH_REMAIN_TIME,
	eGNPMSG_CALCULATE_CONFIRM,
	eGNPMSG_CALCULATE_NO_CONFIRM,
	eGNPMSG_CALCULATE_END,
	eGNPMSG_STONE_MESSAGE,
	eGNPMSG_REQ_REPLAY,
	eGNPMSG_REQ_REPLAY_OK,
	eGNPMSG_PAUSE,


	eGNPMSG_COUNT


};

// -------------------------
enum eGoUserStatus 
{
	eGUS_REQ_PLAY,
	eGUS_REQ_PLAY_END,
	eGUS_REQ_CAL_END
};

// -------------------------
typedef struct {
	unsigned short wins;
	unsigned short loses;
	unsigned short draws;
	unsigned short level;
	unsigned int	score;
} sGoUserInfoDetail;

// -------------------------
enum eGroupInUserState {
	eGIUS_WAIT		= 1,
	eGIUS_PLAYING,
	eGIUS_RESEARCHING,
	eGIUS_DENINED	=0x8000,
	eGIUS_PATIAL_DENINED = 0x4000, // 友達のみ引き受ける。
	eGIUS_MYFRIEND=0x00010000,		// 友人
	eGIUS_GROUP_FRIEND=0x00020000,	// グループ仲間
	eGIUS_NO_NAMED_USER=0x00040000

};

typedef struct {
	unsigned int id;
	unsigned int room;
	char szName[16];
	unsigned int state;
}sSimpleUserInfo;

// -------------------------
typedef struct {
	unsigned int id;				// サーバ上のID

	union {
		unsigned int room;			// サーバ上のソケットインデックス, IDなしログイン時
		unsigned int udpPort;		// udpPort; 
	};
	socket_t sock;					// ローカルのソケット
	int index;						// ローカル上のソケットインデックス
	goid_t	goid;					// ローカルの関連付け
	char szName[16];				// ID
}sGoUserInfo;

typedef struct {
	sGoUserInfo base;
	eGroupInUserState state;		// 状態
	sRegisterUserInfo reg;
	unsigned int score[4];
	unsigned int seq_pic;
}sGoUserInfoEx;

typedef class cyitNode<sSimpleUserInfo> CSimpleUser;
typedef class cyitList<sSimpleUserInfo> CSimpleUserList;

typedef struct {
	unsigned int id;
	char szName[32];
	CSimpleUserList* users;
} sGoGroupInfo;

typedef class cyitNode<sGoUserInfo> CUserInfo;
typedef class cyitList<sGoUserInfo> CUserList;

enum eGoPlayState {
	eGPS_START=1,
	eGPS_PLAYING,
	eGPS_PAUSED,
	eGPS_HUU,
	eGPS_CALCULATE_START,
	eGPS_CALCULATE_END,
	eGPS_RESEARCHING,
	eGPS_END=0
};

// -------------------------
//typedef struct {
//	ugoid_t id;
//	sGoUserInfo* black;
//	sGoUserInfo* white;
//	unsigned int condition1;
//	unsigned int condition2;
//	unsigned short result;
//	unsigned short order;
//	eGoPlayState eState;
//}sGoPlayInfo;

#define MAX_USER_CNT 128

// -------------------------
typedef void* HandleUser;

enum ePlayEndStatus {
	ePES_NORMAL,
	ePES_GIVEUP,
	ePES_CANCEL
};



// -------------------------
class cypnGoNetwork : public cyPrivateNetwork
{
public:
	// ---------------------------
	cypnGoNetwork(void);

	// ---------------------------
	~cypnGoNetwork(void);

	// ---------------------------
	void SetMyGoInfo(sGoUserInfoEx info);

	// ---------------------------
	//Bool SendTestMessage(HandleUser hUser, char* buffer, int size);

	// ---------------------------
	int CreateUdpSock(address_t& addr, int& port, address_t& addr_local, int& local_port);

	// ---------------------------
	BOOL SendToUserEx(HandleUser hUser, int id, const void* data, int size, Bool bIsConfirm=True);

	// ---------------------------
	Bool BroadCastSend(HandleUser* hUsers, int n, int id, const void* data, int size);

	// ---------------------------
	Bool BroadCastSend(cyiList* listUser, int id, const void* data, int size);

	// ---------------------------
	Bool SendTextMessage(HandleUser hUser, const char* pszMsg);

	// ---------------------------
	Bool SendRequestPlay(HandleUser hUser, unsigned int con1, unsigned int con2);

	// ---------------------------
	Bool SendReplyPlay(HandleUser hUser, unsigned int con1, unsigned int con2);

	// ---------------------------
	Bool Login(HWND hNotifyWnd, UINT msg, address_t server, int port, unsigned int security, 
						unsigned int id, const char* pszUserName, const char* pszPassword);

	// ---------------------------
	Bool CheckMyNetwork(sNetworkInfo* info=0, address_t localaddr=0);

	// ---------------------------
	Bool StartLoop();

	// ---------------------------
	unsigned int SendRequestPlayStart(HandleUser hUser, unsigned int* con1, unsigned int con2, ugoid_t& uid);

	// ---------------------------
	Bool SendRequestPlayEnd(HandleUser hUser, unsigned int id, ePlayEndStatus status);

	// ---------------------------
	Bool SendRequestPlayEndOk(HandleUser hUser, unsigned int id, ePlayEndStatus status, Bool bIsOK);

	// ---------------------------
	Bool SendRequestCalculateEnd(HandleUser hUser, unsigned int id);

	// ---------------------------
	Bool SendRequestCalculateEndOk(HandleUser hUser, unsigned int id, Bool bIsOk);

	// ---------------------------
	Bool SendStone(HandleUser hUser, stone_t stone, unsigned int millisec);

	// -------------------------------
	Bool SendGiveup(HandleUser user);

	// -------------------------------
	Bool SendTimeout(HandleUser user, Bool bIsBlack, unsigned int goid);

	// -------------------------------
	void DestroyUser(HandleUser hUser);

	// ---------------------------
	HandleUser IndexToUser(int i);

	// ---------------------------
	HandleUser FindUser(const char* pszUserName, unsigned int id=0, unsigned int go_id=0);

	// ---------------------------
	HandleUser FindUserByRommNo(int nRoomNo);

	// ---------------------------
	static void SetGoId(HandleUser hUser, unsigned int id);

	// ---------------------------
	static sGoUserInfo* GetGoUserInfo(HandleUser hUser);

	//// ---------------------------
	//static unsigned int GetUserId(HandleUser hUser);

	//// ---------------------------
	//static const char* GetUserName(HandleUser hUser);

	//// ---------------------------
	//static const int GetUserRoomNo(HandleUser hUser);

	// ---------------------------
	static void SetUserDetail(HandleUser hUser, sGoUserInfoDetail& detail);

	// ---------------------------
	static Bool IsUserValid(HandleUser hUser);

	// -------------------------------------------
	sGoUserInfoEx* GetMyGoInfo();

public:
	//// ---------------------------
	//sRegisterUserInfo m_register;
	//// ---------------------------
	//sBaseUserInfo m_base;

	// ---------------------------
	sMsg32* msg;

	// ---------------------------
	cyiList* m_msg;


protected:
	// ---------------------------
	virtual Bool OnMessage(socket_t sock, int index, int id, void* buffer, int size);

	// ---------------------------
	virtual void OnAddUser(socket_t sock, int index, void* param);

	// ---------------------------
	virtual void OnRemoveUser(socket_t sock, int index);

	// ---------------------------
	virtual Bool OnLogin(void* buffer, int size);

	// ---------------------------
	HandleUser m_hUserList[MAX_USER_CNT];


private:
	HandleMutex m_hUserMutex;

	// ---------------------------
	sGoUserInfoEx m_myGoInfo;

	friend class CGoEditorApp;

	friend class cMyMainFrame;

	
};

#endif

#endif // _GOYOTSUYA