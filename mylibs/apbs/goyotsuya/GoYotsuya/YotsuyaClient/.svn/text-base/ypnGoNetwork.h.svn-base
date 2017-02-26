#ifndef __ypnGoNetwork_h__
#define __ypnGoNetwork_h__
#include "ypn.h"

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
	eGNPMSG_REQ_CALCULATE_END_OK

};

// -------------------------
enum eGoUserStatus 
{
	eGUS_REQ_PLAY,
	eGUS_REQ_PLAY_END,
	eGUS_REQ_CAL_END
};

// -------------------------
typedef struct xsGoUserInfo {
	socket_t sock;
	int index;
	unsigned int id;
	eGoUserStatus status;
}sGoUserInfo;

#define MAX_USER_CNT 128

// -------------------------
typedef Handle HandleUser;

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
	//Bool SendTestMessage(HandleUser hUser, char* buffer, int size);	

	// ---------------------------
	Bool SendTextMessage(HandleUser hUser, const char* pszMsg);

	// ---------------------------
	Bool SendRequestPlay(HandleUser hUser, unsigned int con1, unsigned int con2);

	// ---------------------------
	Bool SendReplyPlay(HandleUser hUser, unsigned int con1, unsigned int con2);

	// ---------------------------
	unsigned int SendRequestPlayStart(HandleUser hUser, unsigned int* con1, unsigned int con2);

	// ---------------------------
	Bool SendRequestPlayEnd(HandleUser hUser, unsigned int id, ePlayEndStatus status);

	// ---------------------------
	Bool SendRequestPlayEndOk(HandleUser hUser, unsigned int id, ePlayEndStatus status, Bool bIsOK);

	// ---------------------------
	Bool SendRequestCalculateEnd(HandleUser hUser, unsigned int id);

	// ---------------------------
	Bool SendRequestCalculateEndOk(HandleUser hUser, unsigned int id, Bool bIsOk);

	// ---------------------------
	Bool SendStone(HandleUser hUser, unsigned int id, stone_t stone);

	// ---------------------------
	HandleUser IndexToUser(int i);

	// ---------------------------
	HandleUser FindUser(const char* pszUserName, unsigned int id=0, unsigned int go_id=0);

	// ---------------------------
	static void SetGoId(HandleUser hUser, unsigned int id);

	// ---------------------------
	static unsigned int GetUserId(HandleUser hUser);

	// ---------------------------
	static const char* GetUserName(HandleUser hUser);

protected:
	// ---------------------------
	virtual Bool OnMessage(socket_t sock, int index, int id, void* buffer);

	// ---------------------------
	virtual void OnAddUser(socket_t sock, int index);

	// ---------------------------
	virtual void OnRemoveUser(socket_t sock, int index);

	// ---------------------------
	HandleUser m_hUserList[MAX_USER_CNT];

private:
	HandleMutex m_hUserMutex;
	
};

#endif
