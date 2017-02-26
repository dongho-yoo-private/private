#pragma once

// -------------------------------------
//typedef struct xsyiLoginUserInfo {
//	unsigned int sock;
//	unsigned int id;
//	unsigned int security;
//	unsigned int passport;
//	char szUserName[16];
//	HandleMutex hMutex;
//	sNetworkInfo	network;
//} syiLoginUserInfo;



// -------------------------------------
#pragma pack(push, 1)
typedef struct {
	unsigned int	reserved;
	unsigned int	id;
	char		szName[16];
	char		szPassword[8];
	sNetworkInfo	network;
} syiLoginData;
#pragma pack(pop)

// -------------------------------------
typedef struct syiLoginReply {
	unsigned int id;
	unsigned int index;
	unsigned int sock;
	unsigned int security;
	Bool	     network;
} syiLoginReply;

/*
typedef struct {
	unsigned short level;
	unsigned short wins;
	unsigned short loses;
	unsigned short draws;
	unsigned int score;
}sGoScore;

// -------------------------------------
// ユーザ情報（実際にファイルにある情報である。
typedef struct {
	char szUserId[16];
	char szPassword[8]; // この値は取得できない
	sGoScore score;
}syiUserInfo;

typedef struct {
	unsigned int id;
	unsigned int ownerId;
	char szGroupName[32];
	unsigned int grade;
	sGoScore score;
}syiGroupInfo;

*/