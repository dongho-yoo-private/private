#ifndef __SYC_TYPES_H__
#define __SYC_TYPES_H__
#include "yi.h"
#include "sycUser.h"

#define YI_MAX_NAME_LENGHT 16
#define YI_MAX_PASSWORD_LENGHT 8
#define YI_MAX_EVENT_NAME_LENGTH 32
#define YI_MAX_EMAIL_LENGHT 84
#define YI_MAX_USER_COMMENT_LENGHT 256
#define YI_TITLE_LENGHT 32
/*
enum eUserServerPacketId {
	eUPID_REQUEST_PREREGISTER=0x00800000,
	eUPID_REQUEST_REGISTER,
	eUPID_REQUEST_LOGIN,
	eUPID_REQUEST_MAILADDRESS,
	eUPID_REQUEST_LOGOUT
};
*/

enum eGoResult {
	eGR_WIN = 2,
	eGR_LOSE = 1,
	eGR_DRAW = 3,
	eGR_CANCELED = 0
};

// --------------------------------------
// 31: order (black 1, white 0)
// 30-27: handicap 0-15
// 26-19: komi 0-255
// 18   : 0.5 komi (yes 1, no 0)
// 17-16: result (10: win, 01: lose, 11: zigo, 00: canceled)
// 15-8 : score 0-255
// 7-0  : reserved
typedef Bit32 playinfo_t;

#define __PI_ORDER__(info) 		(info>>31)
#define __PI_HANDICAP__(info) 		((info>>27)&0x0000000F)
#define __PI_KOMI__(info) 		(int)((char)((info>>19)&0x000000FF))
#define __PI_HALF_KOMI__(info) 		((info>>18)&0x00000001)
#define __PI_IS_RESULT__(info) 		((info>>16)&0x00000003)
#define __PI_IS_SCORE__(info)   	((info>>8)&0x000000FF)
#define __PI_IS_RESERVED__(info)   	(info&0x000000FF)


// --------------------------------------
enum eGoPlayType {
	eGPT_MAJOR_EVENT = 0x8000,
	eGPT_GENERAL = 0x4000,
	eGPT_MINOR_EVENT = 0x2000,
	eGPT_GRADEUP_EVENT = 0x1000,
	eGPT_SLOW_GO = 0x0800,
	eGPT_FRIENDLY_EVENT = 0x0400,
	eGPT_GROUP_GENERAL  = 0x0200,
	eGPT_GOOD_PLAY = 0x0100	
};

// --------------------------------------
// 15: major event
// 14: general
// 13: minor event
// 12: grade up play
// 11: group event
// 10 : slow mode
// 9 : friendly play
// 8 : good play
typedef Bit16 eventinfo_t;



// ------------------------------
typedef struct _sycGoId
{
	Bit32 black_id;
	Bit32 index;
}sycGoId;

// ------------------------------
typedef struct _sycGoInfo {
	Bit32 playes;
	Bit32 wins;
	Bit32 loses;
	Bit32 cwins;
	Bit32 closes;
	Bit32 bestplaycnt;
	Bit32 lated; /* 12bit: win, 12bit: loses: 8: jigo */
	Bit32 reserved;
}sycGoInfo ; /* total 32 bytes. */

// ------------------------------
// for server.
typedef struct _sycUserStatus {
	Bit32 status; /*31: logined, 30: isPlayDesired, ... */
}sycUserStatus ;

// ------------------------------
// for server.
typedef struct _sycUserInfo {
	ssycUserBasicInfo basic;
	sycGoInfo go;
	sycUserStatus status;
} sycUserInfo ;

// ---------------------------------
// 相手名、相手のレベル、棋譜ID、日付け、囲碁区別、勝ち負け、何目などが分かる。
// 黒a vs 白b 5段 2009/12/12 試合 白 3目半勝ち 255手完
typedef struct _sycPlayHistroy { /* /usr_root/xxxx/xxxx/xxxx/id.histories*/
	char name[YI_MAX_NAME_LENGHT];
	sycGoId id;
	Bit16 date; /* from 2010 year. */
	Bit16 eventinfo;
	playinfo_t playinfo; /* reserved is level. */
} sycPlayHistory; /* total 32 bytes */

typedef struct _sycEventHistory {
	char name[YI_MAX_EVENT_NAME_LENGTH];
	Bit32 eventId; /*31-28: (0: private event, 1:grade up event, 2 : minor event, 3: major event) */
	Bit32 groupId; /*0xFFFFFFFF: grade up only */
	Bit32 date; 
	Bit32 participants; /* all of participants*/
	Bit32 result; /*31-28: grade up or down, 27-26:(win, lose, draw), 25:primary, 24-21:n's, 20-16: best n, 15-1: reserved*/
} sycEventHistory ;

// ---------------------------------
typedef struct _sycBroadCastUser {
	char name[YI_MAX_NAME_LENGHT];
	Bit32 useerId;
	Bit32 userInfo; 
}sycBroadCastUser;

// ---------------------------------
typedef struct _sycSimplePlayResultHistroy {
	Bit32 id;
	playinfo_t result;
} sycSimplePlayResultHistroy;

// ---------------------------------
typedef struct _sycUserInfoEx {
	char comment[YI_MAX_USER_COMMENT_LENGHT];
	char title[32];
} sycUserInfoEx;

#endif
