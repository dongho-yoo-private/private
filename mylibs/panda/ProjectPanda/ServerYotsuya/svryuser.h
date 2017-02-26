#ifndef __svryuser_h__ 
#define __svryuser_h__ 

#include "yi.h"
#include "yisock.h"
#include "svrybase.h"
#include "svrydefs.h"
#include "yiHashTable.h"

// ------------------------------
#define MAX_UDP_TABLE_CNT 100

// ------------------------------
#define sGeneralParam syiGeneralParam

typedef void** HandleUserFile;

// ------------------------------
typedef struct xsUdpTables {
	Bool bIsUsed;
	int  port;	
} sUdpTables;

enum eErrorSvryUser {
	eESU_USERFILE_WRITE_ERROR=-1
};

typedef struct {
	unsigned int remoteId;
	int type;
} sMsg8;

typedef struct {
	unsigned int remoteId;
	unsigned int day;
	unsigned int time;
	unsigned short type;
	unsigned short reserved;
	char szUserName[16];
} sMsg32;

// ---------------------------
// contents format.
// <TEXT> </TEXT> : text
// <SGF></SGF> : sgf body.
// 
typedef struct {
	sMsg32 msg;
	unsigned short offset;
	unsigned short length;
} sMsgEx;

typedef struct {
	sMsg32 msg;
	char szText[128-32];
} sMsg128;

// ------------------------------
enum eMessageType {
	eWMT_PUT_STONE = 1,
	eWMT_ADD_FRIEND,
	eWMT_ADD_FRIEND_NG,
	eWMT_ADD_FRIEND_OK,
	eWMT_ADD_FRIEND_WAIT,
};

// ------------------------------
class csvryuser : public csvrybase
{
public:
	// --------------------------
	csvryuser(const char* servername);

	// --------------------------
	~csvryuser();
	
	// --------------------------
	static csvryuser* GetInstance();

	// --------------------------
	void GetUserFilePath(char* path, unsigned int id, const char* pszFileName);
	//
	virtual Bool OnServerStart();

	//
	virtual Bool OnServerEnd();

	//
	int GetFreeUdpPort();

	//
	void ReleaseUdpPort(int port);

	// operations
protected:
	// --------------------------
	Bool IsEmailValid(unsigned int key, const char* pszEmailAddress);

	// --------------------------
	Bool OnPreRegister(int sock, unsigned int id, const char* pszEmailAddress);

	// --------------------------
	Bool OnReleaseEmailAddress(int sock, const char* pszEmailAddress);

	// --------------------------
	Bool OnReconizedData(int sock, unsigned int r1, unsigned int r2);

	// --------------------------
	Bool OnQueryUser(int sock, unsigned int id, const char* pszId);

	// --------------------------
	Bool OnRegiterUser(int sock, unsigned int id, sRegisterUserInfo* s);

	// -----------------------------------
	Bool OnNoIdLogin(int sock, int index, syiLoginData* data, void** pExtra);

	// -----------------------------------
	Bool OnLogin(int sock, int index, syiLoginData* data, void** pExtra);

	// -----------------------------------
	Bool OnLoginOK(int sock, HandleUserFile hFile);

	// -----------------------------------
	Bool OnReqFiles(int sock, void* data, int size, HandleUserFile hFile);

	// -----------------------------------
	Bool OnReqGoHeader(int sock, unsigned int sequenceNo, HandleUserFile hFile);

	// -----------------------------------
	void* OnReqGoData(int sock, unsigned int black, unsigned int white, unsigned int time, unsigned int day, HandleUserFile hFile, int* size=0);

	// -----------------------------------
	void* OnReqGoData2(int sock, unsigned int black, unsigned int white, unsigned int time, unsigned int day, HandleUserFile hFile, int* size=0);

	// -----------------------------------
	Bool OnUploadGoData(int sock, sSimpleGoData* data, int size, HandleUserFile hFile);

	// -----------------------------------
	Bool OnUploadGoData2(int sock, sSimpleGoData* data, int size, HandleUserFile hFile);

	// -----------------------------------
	Bool OnDeleteGoData(int sock, sSimpleGoHeader* hd, HandleUserFile hFile);

	// -----------------------------------
	Bool OnDeleteGoHeader(int sock, sSimpleGoHeader* hd, HandleUserFile hFile);

	// -----------------------------------
	Bool AppendGoHeader(HandleUserFile hFile, sSimpleGoHeader* header);

	// -----------------------------------
	Bool AppendGoHeader2(HandleUserFile hFile, sSimpleGoHeader* header, Bool bIsDelete=False);

	// -----------------------------------
	Bool OnReqConnectionMe(int sock, unsigned int remoteId, HandleUserFile hFile);

	// -----------------------------------
	Bool OnReqConnectionUdp(int sock, int myIndex, int remote, HandleUserFile hFile, const char* pszPassword=0);

	// -----------------------------------
	Bool OnReqConnectionUdp(int sock, unsigned int remote, HandleUserFile hUserFile);//, const char* pszName, void** pExtraDataRef);

	// --------------------------
	Bool OnReqConnectionUdpOk(int sock, int myIndex, int remote, void** pExtraDataRef);

	// --------------------------
	Bool OnReqCheckMyNetwork(int sock, unsigned int address, int port, void** pExtraDataRef);

	// --------------------------
	Bool OnReqRegisterMyNetwork(int sock, eNatType type, unsigned int address, int port, void** pExtraDataRef);

	// --------------------------
	Bool OnReqDelMsg32(int sock, HandleUserFile hFile, int index);

	// --------------------------
	Bool OnReqBuyPoint(void* data, HandleUserFile hFile);

	// --------------------------
	Bool OnReqBuyPoint2(void* data, HandleUserFile hFile);

	// --------------------------
	Bool OnReqBuyPointSpecial(void* data, HandleUserFile hFile);
protected:
	// -----------------------------------
	virtual void OnClientClose(int sock, void** pExtraDataRef);

	// --------------------------
	virtual Bool OnMessageLocal(int sock, syipacket& packet, void** pExtraDataRef);

	// --------------------------
	virtual Bool OnMessagex(int sock, int id, void* data, int size, void** pExtraDataRef);

	// --------------------------
	virtual Bool OnAccepted(int sock, int index, void** pExtra);

	// -------------------------------------
	virtual Bool OnStatus(char* buffer);
private:
	// --------------------------
	Bool OpenUdpPort();

	// --------------------------
	void OnUdpLoop(unsigned long param1, unsigned long parm2, unsigned long param3);
friend void* xOnUdpLoop(void* param);

	// --------------------------
	HandleUserFile GetUserExtraData(unsigned int id, const char* pszUserName);

	// --------------------------
	HandleUserFile GetUserExtraData(unsigned int index);//, const char* pszUserName);

	// --------------------------
	void ReleaseUserExtraData(HandleUserFile h);

	// -------------------------------------
	Bool xCreateDefaultUserFiles(unsigned int id, sRegisterUserInfo* info);

#define SendResult SendError

	// -------------------------------------
	Bool SendError(int sock, int id, int result, const void* p, int size);

	// -------------------------------------
	Bool SendError(int sock, int id, int result, int no=0, ...);

	// -------------------------------------
	Bool SendError(HandleUserFile hFile, int id, int result, int no=0, ...);

	// --------------------------
	Bit32 GetUserId(const char* pszUserName);

	// --------------------------
	//syiLoginUserInfo* GetUserInfo(const char* pszUserName);

	// --------------------------
	//syiLoginUserInfo* GetUserLoginInfo(unsigned int id, const char* pszUserName);

	// --------------------------
	//syiLoginUserInfo* GetUserLoginInfo(int index);

	// --------------------------
	int sendx(syiLoginUserInfo* user, int id, void* data, int n, int bBackground);	

	// --------------------------
	Bool LoadUserData();

	// --------------------------
	Bool UpdateUserFile(HandleUserFile hUserFile, int FileNo, void* p=0, int size=0, int flag=0);

	// --------------------------
	Bool OnLoadUserData(const char* path, unsigned int id, Bool bIsReLoad=False);

	// --------------------------
	Bool LoadGroupData();
	
	// --------------------------
	Bool OnLoadGroupData(const char* path, unsigned int id);
	
	// --------------------------
	Bool LoadEmailAddress();

	// --------------------------
	Bool OnLoadEmailAddress(const char* path, unsigned int id, Bool bIsReLoad=False);

	// -------------------------------------
	Bool OnRegisterUser(int sock, unsigned int id, sRegisterUserInfo* info);

	// -------------------------------------
	Bool OnReqAddFriend(int sock, unsigned int myId, Bool bIsSave, unsigned int remoteId, const char* pszFriendName, void** pExtraDataRef);

	// -------------------------------------
	Bool OnReqDeleteFriend(int sock, unsigned int myId, unsigned int remoteId, void** pExtraDataRef);

	// ---------------------------------------
	Bool GetPassportFromProxy(int* passport, int* id, int* port);
	// -------------------------- database.
	//csycHashL3 m_UserDataHash;
	//ssycHashL3* m_pUserHash;
	
	// -------------------------- database.
	//csycHashL3 m_GroupDataHash;
	//ssycHashL3* m_pGroupHash;
	//
	// -------------------------------------
	Bool WriteUserFile(unsigned int id, const char* path, void* data, int n);

	// -------------------------------------
	//Bool RegisterUser(syiUserRegisterData* data, Bool* bIsDuplicate);

	// -------------------------------------
	void* GetValue(unsigned int userId, char** szUserName);

	// -------------------------------------
	Bool OnReqFriend(int sock, unsigned int myId, unsigned int myFriendId, void** pExtra);

	// -------------------------------------
	Bool OnReqFriendOK(int sock, unsigned int myId, unsigned int myFriendId, void** pExtra);

	// -------------------------------------
	Bool OnGetMsg32(int sock, unsigned int myId, void** pExtra);
	
	// -------------------------------------
	Bool OnGetFriendList(int sock, unsigned int myId, void** pExtra);

	// -------------------------------------
	Bool xSendMail(const char* pszEmailAddress, unsigned int r1, unsigned int r2);

	// -------------------------------------
	cyiList* LoadMsgEx(HandleUserFile hFile);

	// -------------------------------------
	Bool SaveMsgEx(cyiList* msg);

	// -------------------------------------
	Bool AddMsgEx(HandleUserFile hFile, eMessageType type, HandleUserFile pRemoteFileList, void* pMsg, int size, Bool bIsReception=False);

	// -------------------------------------
	Bool DeleteMsgEx(HandleUserFile hFile, unsigned int id);

	// -------------------------------------
	Bool AddMsg32(HandleUserFile hFile, eMessageType type, HandleUserFile pRemoteFileList, Bool bIsReception=True, unsigned short data=0);

	// -------------------------------------
	Bool DeleteMsg32(HandleUserFile hUser, unsigned int remoteId, int type);

	// -------------------------------------
	Bool DeleteMsg32(HandleUserFile hUser, int index);

	// -------------------------------------
	int FindMsg32(HandleUserFile hFile, unsigned int from, eMessageType type);

	// -------------------------------------
	Bool AddWaitMsg8(HandleUserFile hUserFile, unsigned int remoteId, eMessageType type);

	// -------------------------------------
	Bool FindWaitMsg8(HandleUserFile hUserFile, unsigned int remoteId, eMessageType type, Bool bIsRemove=False);

	// -------------------------------------
	Bool AddFriend(HandleUserFile, unsigned int remoteId, const char* pszFriendName);
	
	// -------------------------------------
	Bool DeleteFriend(HandleUserFile hUserFile, unsigned int friendId);

	// -------------------------------------
	Bool AddGoData(sGoHeader* header, void* data, int size, HandleUserFile hUserFile);

	// -------------------------------------
	void GetGoDataPath(char* path, sGoHeader* header);

private:
	char m_szUserRoot[256];

	static csvryuser* me;	

	int m_nUsedMemory;
	int m_nLoadedUserCnt;
	int m_nUnUsedUser;
	int m_nRegisteredEmailCount;
	void InitUdpPortTable();
	sUdpTables* m_UdpTables;
	HandleMutex m_hUdpTableMutex;
	unsigned long m_UdpTestServerAddress;

	int m_sockProxy;
	int m_port_proxy;
	unsigned long m_proxy_addr;
	char m_szProxyServerName[64];

	cyiHashTable* m_UserHash;
	cyiHashTable* m_GroupHash;
	cyiHashTable* m_EmailHash;
	cyiList*	m_listEmail;
	unsigned int m_nRegisterPoint;
	unsigned int m_nInitialFriendCount;
	address_t 	m_myAddress;
	int m_udpPorts[10];
	int m_udpSocks[10];
	char m_szCommonDir[256];
	Bool m_bIsTerminate;

	friend void xOnLoadUserData(const char* pszPath, unsigned int id);
	friend void xOnLoadGroupData(const char* pszPath, unsigned int id);
	friend void xOnLoadEmailAddress(const char* pszPath, unsigned int id);
};


#endif
