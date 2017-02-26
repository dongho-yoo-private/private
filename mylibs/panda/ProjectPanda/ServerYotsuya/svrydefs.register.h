#ifndef __xRegisterDefs__
#define __xRegisterDefs__
// -------------------------------------
/*enum eUserLocalPacketId{
	eULPID_NO_ID_REGISTER=1
};*/

#define USER_NAME_LENGTH 16

#define USER_DATA_COUNT 7

// 0: register info
#define REGISTER_INFO(p) (sRegisterUserInfo*)p[0]
// 1: login info
#define BASE_INFO(p)	(sBaseUserInfo*)p[1]
// 2: friend list
#define FRIEND_INFO(p)	(sUser*)p[2]
// 3: msg32 
#define MSG32(p)	(sMsg32*)p[3]

#define LOGIN_INFO(p) (syiLoginUserInfo*)p[USER_DATA_COUNT]

#define LOGIN_INFO2(p) (syiLoginUserInfo2*)p[USER_DATA_COUNT]

#define WAIT_QUEUE(p) (sMsg8*)p[USER_DATA_COUNT+1]

#define GO_DATA(p)	(cyiList*)p[4];

#define PAUSED_GO(p)	(sSimpleGoHeader*)p[5];

#define GETID(p)	(p[0]==0?0:((sRegisterUserInfo*)p[0])->id)
// 4: credit history.
// 5: play history.
// 6: login info (system)
// 

// 0 file.
typedef struct {
	unsigned int id;
	char szUserName[USER_NAME_LENGTH];
	char szPassword[8];
	char szEmailAddress[80];
	unsigned int emailId;
	unsigned short nContryCode;
	unsigned short nAreaCode;
	unsigned int	birthday;
	unsigned int 	flag; // flag 31: true:male, false:femail, 30:openflag:nAreaCode, 29:openflag:birthday, .... 7-0: level
} __attribute__((aligned(1))) sRegisterUserInfo;


typedef struct {
	unsigned int id;
	unsigned int userId;
	char szEmailAddress[80];
} __attribute__((aligned(1))) sRegisterEmailInfo;
// 1 file.
//
enum eUpdateLoginFileFlag {
	eULFF_UPDATE_TIME=0x01,
	eULFF_UPDATE_SEQUENCE_NO=0x02,
	eULFF_UPDATE_POINT=0x04,
	eULFF_UPDATE_SCORE=0x08,
	eULFF_UPDATE_ALL=0xFF
};

// -------------------------------------
typedef struct {
	unsigned int lastLoginDate;
	unsigned int lastLoginTime;
	unsigned int point;
	unsigned short total;
	unsigned short draws;
	unsigned short wins;
	unsigned short loses;
	unsigned int	currLevel;
	unsigned int 	currScore;
	unsigned int   sequenceNo[USER_DATA_COUNT];
	unsigned int   sizes[USER_DATA_COUNT];
	unsigned char  reserved[32]; // 0: license code. (go yotsuya license code), 0: free, 1: godata no limited.), reserved[0]: 31: on is administrator. off is normal user.
} __attribute__((aligned(1))) sBaseUserInfo;


// 2file.
typedef struct {
	unsigned int id;
	char szUserName[USER_NAME_LENGTH];
} sUser;

//
typedef unsigned int goid_t[4];

typedef sUser sFriendList;

typedef struct {
	goid_t id;
	char szBlackName[USER_NAME_LENGTH];
	char szWhiteName[USER_NAME_LENGTH];
	unsigned short result;
	unsigned short nLastOrder;
} sGoHeader;

// -------------------------------------
/*typedef struct xsyiUserRegisterData {
	unsigned int id;
	char szUserName[16];
	char szPassword[8];
	char szEmailAddress[64];
	unsigned int job;
	unsigned int birthday;
	unsigned int area;	
	unsigned int level;	
} syiUserRegisterData;

// -------------------------------------
typedef struct xsyi_a_file
{
	syiUserRegisterData base;
	unsigned int userId;
	unsigned int last_logined;
	unsigned int last_updated;
} syi_a_file;

*/

typedef struct {
	sRegisterUserInfo 	reg;
	sBaseUserInfo 		login;
	sUser			list[1];
} __attribute__((aligned(1))) sAddFriendOk;
#endif

