#ifndef __svrydefs_h__
#define __svrydefs_h__

// -------------------------------------
enum eUserLocalPacketId {
	eULPID_PREREGISTER=0x80,
	eULPID_REGISTER,
	eULPID_RPY_REGISTER
};

// -------------------------------------
enum eUserServerPacketId {
	eUSPID_NO_ID_LOGIN=1,
	eUSPID_REQ_CONNECT_UDP=2,
	eUSPID_REQ_CONNECT_UDP_OK=3,
	eUSPID_RPY_CONNECT_UDP=4,
	eUSPID_REQ_CONNECT_TCP_PROXY=5,
	eUSPID_RPY_CONNECT_TCP_PROXY=6,
	eUSPID_REQ_CHECK_MY_NETWORK=7,
	eUSPID_RPY_CHECK_MY_NETWORK=8,
	eUSPID_REQ_REGISTER_MY_NETWORK=9,
	eUSPID_RPY_REGISTER_MY_NETWORK=10,
	eUSPID_NFY_CONNECT_UDP=11,
	eUSPID_REQ_CONNECT_UDP_BY_ID=12,
	eUSPID_REQ_REGISTER_EMAIL=13,
	eUSPID_REQ_REGISTER_USER=14,
	eUSPID_LOGIN=15,
		// 0: result, (eGE_OK=0, eGE_INVALID_USER=-2, eGE_INVALID_PASSWORD=-5)
		// 1: userId,
		// 2: socket index (room no)
		// 3: security no.
	eUSPID_REQ_REGISTER_USER_EMAIL=16,
	eUSPID_NO_LOGIN=17,
	eUSPID_REQ_RECONIZED_CODE=18,
	eUSPID_RELEASE_EMAIL=19,
	eUSPID_REQ_QUERY_USER=20,
	eUSPID_ZERO_FILE=21,
	eUSPID_ONE_FILE=22, // USER_DATA_COUNT数文のフラグ
	eUSPID_REQ_ADD_FRIEND=23, // -1: remote invalid, -2: system error, -3: other, 1: already register
	eUSPID_REQ_DELETE_FRIEND=24,
	eUSPID_NOTIFY_MSG32=25, /*0: sezuenceNo, 1: updatedIndex, 2: 31byte data*/
	eUSPID_GET_MSG32=26,
	eUSPID_GET_FRIEND_LIST=27,
	eUSPID_GET_MSG128=28,
	eUSPID_REQ_ADD_FRIEND_OK=29,
	eUSPID_LOGIN_OK=30,
	eUSPID_REQ_FILES=31,
		// USER_DATA_COUNT bytes: (update mask)
	eUSPID_REQ_GO_HEADER=32,
		// 0: update count.
		// if (update count over zero).
		// next data.
	eUSPID_REQ_GODATA=34,
		// sSimpleGoData
	eUSPID_REQ_DEL_MSG32=35,
	eUSPID_UP_GODATA=36,
	eUSPID_RELOGIN=37,
	eUSPID_UP_GODATA2=38,
	eUSPID_REQ_BUY_POINT=39,
	eUSPID_REQ_BUY_POINT2=40,
	eUSPID_DEL_GOHEADER=41,
	eUSPID_DEL_GODATA=42,
	eUSPID_REQ_CONNECT_UDP_ME=43,
	eUSPID_REQ_BUY_SPETIAL=0xFF
};

// -------------------------------------
typedef struct {
	unsigned int 	remoteId;
	unsigned int	day;
	unsigned int	time;
	unsigned int	result;
	/*unsigned int	flags; // 31: Iam black. 
			       // 30-29: 00 unknown 10 black win, 01 white win, 11 draw.
			       // 28: time over.
			       // 27-19: last order. (size is order<<1)
			       // 17: is half score.
			       // 16-9: score.
	//char 		szUserId[16];*/
} __attribute__((aligned(1))) sSimpleGoHeader; // 16 bytes.

// -------------------------------------
typedef struct {
	sSimpleGoHeader hd;
	unsigned int condition;
	unsigned int condition2;
	char szPlace[32];	
	unsigned short stones[0];
} __attribute__((aligned(1))) sSimpleGoData; 

// -------------------------------------
enum eNatType {
	eNT_UNKNOWN=0,
	eNT_NONE,
	eNT_FULL_CON,
	eNT_LIMITED_OR_FULL,
	eNT_LIMITED,
	eNT_LIMITED_PORT,
	eNT_SYMMETRIC,
	eNT_UPNP
};


// -------------------------------------
enum eGeneralError {
	eGE_OK= 0,
	eGE_INVALID_PARAM= -1,
	eGE_INVALID_USER= -2,
	eGE_SERVER_IS_BUSY= -3,
	eGE_DUPLICATE_ID=-4,
	eGE_INVALID_USER_PASSWORD=-5,
	eGE_UNKNOWN=-6,
	eGE_REMOTE_BUSY=-7
};

// -------------------------------------
typedef struct xsAddress {
	unsigned int address;
	int port;
} sAddress;

// --------------------------------
typedef struct xsNetworkInfo {
	eNatType type;
	unsigned int address;
	int port;
	unsigned int index;
	unsigned int security;
	unsigned int localaddr;
	unsigned int localport;
} sNetworkInfo;

// --------------------------------
typedef struct {
	unsigned int 	groupId;
	unsigned int 	ownerId;
	char		szGroupName[32];
	unsigned short  nGroupLimitedMember;
	unsigned short  groupLisense;
	unsigned int	dayGroupStarted;
	unsigned int 	timeGroupStarted;
} sGroupInfo;

// --------------------------------
enum eCreditMethod {
	eCM_CREDIT_CARD=1,
	eCM_FROM_BANK=2,
	eCM_SPETIAL=4
};

// --------------------------------
typedef struct {
	unsigned int	day;
	unsigned int 	time;
	unsigned int	point;
	unsigned int 	flags; // 31-29: eCreditMethod.
} sCreditHistory;

// --------------------------------
typedef struct {
	unsigned int porductId;
	unsigned int point;
} sBuyHistory;

// --------------------------------
typedef struct {
	unsigned short 	no[8];
	unsigned char  	pass[4];
	unsigned int	point;
	unsigned int 	flags;
} sReqBuyPointByCreditCard;

// --------------------------------
typedef struct {
	unsigned int userId;
	char szEmailAddress[80];
} sReqBuyPointByBank;

#define HASH_DEFINITION_FILE ".list"

#include "svrydefs.login.h"
#include "svrydefs.rcu.h"
#include "svrydefs.proxy.h"
#include "svrydefs.register.h"
#include "svrydefs.load.h"

#endif
