#pragma once
#define USER_NAME_LENGTH 16

#define USER_DATA_COUNT 7

// 0 file.
#pragma pack(push, 1)
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
} sRegisterUserInfo;

// 1 file.
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
	unsigned char  reserved[32]; // 0: max friend cnt, 
} sBaseUserInfo;


#pragma pack(pop)

// 2file.
typedef struct {
	unsigned int id;
	char szUserName[USER_NAME_LENGTH];
} sUser;

// 2file.
typedef unsigned int xgoid_t[4];
typedef sUser sFriendList;

typedef struct {
	goid_t id;
	char szBlackName[USER_NAME_LENGTH];
	char szWhiteName[USER_NAME_LENGTH];
	unsigned short result;
	unsigned short nLastOrder;
} sGoHeaderPacket;

#pragma pack(push, 1)
typedef struct {
        sRegisterUserInfo       reg;
        sBaseUserInfo           login;
        sUser                   list[1];
} sAddFriendOk;
#pragma pack(pop)
