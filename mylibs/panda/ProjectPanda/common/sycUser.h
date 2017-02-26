#ifndef __syicuser_h__
#define __syicuser_h__

#define YI_MAX_NAME_LENGTH 24
#define YI_MAX_PASSWORD_LENGTH 8
#define YI_MAX_EMAIL_LENGTH 120
#define YI_MAX_USER_PATH 32
#define YI_MAX_NICK_NAME_LENGTH 32
#define YI_MAX_COMMENT_LENGTH 1024

// ------------------------------
// $USER_TOP/xxx/xxx/xxx/xxx
// for client.
typedef struct _sycUserBasicInfo{
	Bit32 id;
	Bit32 birthday;
	Bit16 address;
	Bit16 contry;
	Bit32 exfield[2]; /* exfield1 : 32-28: level, 27: sex, 26: show age, exfield2: */
	char name[YI_MAX_NAME_LENGTH];
	char email_addr[YI_MAX_EMAIL_LENGTH];
	char password[YI_MAX_PASSWORD_LENGTH];
} ssycUserBasicInfo;

// -------------------------------
// $PRIVATE_TOP/xxx/xxx/xxx/xxx/extend
// extra user data.
typedef struct _sycUserExtendInfo{
	Bit32 nLastLoginTime;
	Bit16 basic_sync_id;
	Bit16 friends_sync_id;
	Bit16 groups_sync_id;
	Bit16 detail_sync_id;
	Bit32 go_sync_id;
	Bit32 total_point;
	Bit32 current_point;
	char  nick_name[YI_MAX_NICK_NAME_LENGTH];
} ssycUserExtendInfo;

// ------------------------------
// $PRIVATE_TOP/xxx/xxx/xxx/xxx/details
// detail data.
typedef struct _sycUserDetailInfo {
	char szCommone[YI_MAX_COMMENT_LENGTH];
	unsigned int macaddress1;
	unsigned int macaddress2;
	//sycUserCreditInfo creditinfo;
} sycUserDetailInfo;

// ------------------------------
// $PRIVATE_TOP/xxx/xxx/xxx/xxx/go
//
typedef struct xsyicUserGoInfo {
	Bit32 matches;
	Bit32 wins;
	Bit32 loses;
	Bit32 cwins;
	Bit32 closes;
	Bit32 bestplay;
	Bit32 lastedinfo; /*12:12:5*/
} ssycUserPlayedInfo;

//
// black dir.
// $PRIVATE_TOP/xxx/xxx/xxx/xxx/current/cashed.n (n= 0..9)
//
// $PRIVATE_TOP/xxx/xxx/xxx/xxx/current/cashed.n (n= 0..9)
//

// ------------------------------
// for server
typedef struct xsycUserServerInfo
{
	ssycUserBasicInfo 	info;
	ssycUserPlayedInfo 	played;
	Bit32 status32; /*31-29: status, ... */
	cyiList* current_go;
} sycUserServerInfo;


/** for version. 1.0.0.
// ------------------------------
typedef struct _sycUserPerchaseInfo {
	unsigned int date;
	unsigned short price;
	unsigned short product_code;
	Bit32 flag32; // 31-30: perchase method. 9-0: trial terms. 
	Bit32 product_attribute_code;
	Bit32 perchase_cgi_code;

	char szProductName[YI_MAX_PRODUCT_NAME_LENGTH];
}

// ------------------------------
typedef struct _sycUserCreditInfo {
	int  nPerchaseHistoryCnt;
	unsigned int  nTotalPoint;
	unsigned int  nUsedPoint;
	unsigned int  nTotalPerchase;
	char szAddress[YI_MAX_ADDRESS_LENGTH];
	char szTelephoneNo[YI_MAX_TEL_LENGTH];
} sycUserCreditInfo;
*/



// ------------------------------
enum eUserLoginInfoExtra {
	eUSER_LOGINED=1,
	eUSER_PAY_USER =0x20000000,
	eUSER_TEST     =0x40000000,
	eUSER_SPECIAL  =0x80000000
};




// define packets for user.

//
// PacketIdz.
// about the management of user.
//
enum eUserPacketId {
	eUPID_REQUEST_LOGIN = 256,
	eUPID_REQUEST_SIMPLE_LOGIN = eUPID_REQUEST_LOGIN,
	eUPID_REQUEST_DATA,
	eUPID_UPDATE_USER_DATA,
	eUPID_GET_USER_INFO,
	eUPID_GET_USER_DETAIL,
	eUPID_SEARCH_USER_ID,
	eUPID_GET_PASSPORT,
	eUPID_ADD_MY_FRIEND,
	eUPID_GET_MY_FRIENDS_STATUS,
	eUPID_SEND_MESSAGE,
	eUPID_NOLIMITED_GO,
	eUPID_REQUEST_PREREGISTER,
	eUPID_REQUEST_REGISTER,
	eUPID_REQUEST_LOGOUT,
	eUPID_REQUEST_MAILADDRESS,
	eUPID_END
};

//
enum eUpdateFileInfo {
	eUFI_GO_EXEC = 1,
	eUFI_GO_RES_DLL = 2,
	eUFI_GO_FONT = 4,
	eUFI_GDI_PLUS = 8
};

//
typedef struct xsSyncIds {
	int basicId;
	int detailId;
	int groupId;
	int system_msgId;
	int myfriendId;
	int gohistoryId;
	int reserved[2];
} sSyncIds;

//
// PacketId: eUPID_REQUEST_LOGIN 
//
// packet.data.param[0]: (rand1&rand2&rand3)
// packet.data.param[1]: userId
// packet.data.param[2]: last sync no.
// packet.data.param[3]: version. (a.b.c.d)
//
// success: 
//    data.param[0]=0
//    data.param[1]= eULPF_USER_BASIC sycnId / eULPF_USER_DETAIL
//    data.param[2]= eULPF_GROUPS syncId / eULPF_MESSAGE syncId
//    data.param[3]= eULPF_MYFRIENDS syncId / eULPF_SYSTEM_MESSAGE syncId
//
//    exdata.
//    sSyncIds
// 
// need upgrade:
//    data.param[0]=1
//    data.param[1]=passport
//    data.param[2]=uploadmask
//    data.param[3]=exdata size
//
//    exdata:
//    char server_addr[64];
//    unsigned int port;
//
// failure: 5 seconds wait and closed();
//    data.param[0]=エラーコード
//
// extradata:sLoginExtraData
//

// packet flags
enum eUserLonginPacketFlags {
	eULPF_USER_BASIC= 	1<<31,
	eULPF_USER_DETAIL= 	1<<30,
	eULPF_MY_FRIENDS = 	1<<29,
	eULPF_GO = 		1<<28,
	eULPF_EVENT = 		1<<27,
	eULPF_MESSAGE = 	1<<26,
	eULPF_SYSTEM_MESSAGE = 	1<<25,
	eULPF_GROUPS= 		1<<24
};

// error code.
enum eUserLoginErrorCode {
	eULEC_INVALID_PASSWORD,
	eULEC_INVALID_USERNAME
};

#define __ENCODED_STR_SIZE__(a) (a<<1)

// --------------------------
typedef struct _sLoginExtraData
{
	char encoded_id[__ENCODED_STR_SIZE__(YI_MAX_NAME_LENGTH)];
	char encoded_password[__ENCODED_STR_SIZE__(YI_MAX_PASSWORD_LENGTH)];
} sLoginExtraData;

//
// PacketId: eUPID_REQUEST_DATA
//
// packet.param[0] = data bit wize (eUserLonginPacketFlags)
//    data.param[1]= eULPF_USER_BASIC sycnId
//    data.param[2]= eULPF_USER_DETAIL syncId / eULPF_MESSAGE syncId
//    data.param[3]= eULPF_MYFRIENDS syncId / eULPF_SYSTEM_MESSAGE syncId
//
//
// success: 
// packet.param[0]=0
// packet.param[1]=total packet size.
//
// ClearPacket.
// (extraData)
//
// sycUserBasicData
// long detailsize
// mydetail
// long messeagesize if size is null, next myfriendsize...
// messages
// long myfriendsize
// myfriends
// long systemmessagesize
// systemmessages
//
// error:
// packet.param[0] = eUserRequestDataError;
//

// ---------------------------------------
enum eUserRequestDataError {
	eURDE_SYSTEM,
	eURDE_BUSY
};

//
//PacketId:eUPID_UPDATE_USER_DATA
//
// packet.param[0] = data bit wize (eUserLonginPacketFlags)
//
//    data.param[1]= eULPF_USER_BASIC sycnId   
//    data.param[2]= eULPF_USER_DETAIL syncId  
//    data.param[3] = detail size.
//
//    success:
//    data.param[0]=0
//    data.param[1] = new sycnId
//    data.param[2] = new syncId
//
//    failure: eUserRequestDataError

//
// PacketId: eUPID_GET_USER_INFO
//
// data.param[0] = userId;
// data.param[1] = 0: default, 1: id only
// 
// exData:
// encodedname (__encoded_str_len__(YI_MAX_NAME_LENGTH))
//
//
// success:
// data.param[0] = 0
// data.param[1] = full userId
// data.param[2] = 1: user is logined. 
// 
// extData:
// sycUserBasicInfo
//
// failure:
// data.param[0] = 0xFFFFFFFF (not found)
// data.param[0] = 0xFFFFFFFE (invalid user)
//

//
// PacketId: eUPID_GET_USER_DETAIL
//
// data.param[0] = userId;
// data.param[1] = syncId
//
//
// success:
// data.param[0] = 0
// data.param[1] = new syncId
// data.param[2] = size
//
// extData:
// sycUserBasicInfo
//
// failure:
// data.param[0] = 0xFFFFFFFF (not found)
// data.param[0] = 0xFFFFFFFE (invalid user)
//

//
// PacketId: eUPID_GET_PASSPORT
//
// success:
// data.param[0] size
// data.param[1] port
// data.param[2] address
// 
// passportdata
//

//
// PacketId: eUPID_GET_MY_FRIENDS_STATUS
//
// success:
// data.param[0]=0
// data.param[1]= /|login status||wait status|/
//
// failure:
// data.param[0]=0xFFFFFFFF (??)

//
// PacketId: eUPID_ADD_MY_FRIEND
//
// data.param[0]=friendId.
//
// success:
// data.param[0]=0
// data.param[1]=syncId;
//
// failure:
// data.param[0]=0xFFFFFFFF (not found)
// data.param[0]=0xFFFFFFFE (io error)
//

//
// PacketId: eUPID_SEND_MESSAGE
// 
// data.param[0]=userId;
// data.param[1]=sizeof(string)
// 
// extdata:
// string. (ClearType)
//
// success:
// data.param[0]=0
//
// failure:
// data.param[0]=0xFFFFFFFF (user logouted, stored.)
// data.param[0]=0xFFFFFFFE (denined)
//

//
// PacketId: eUPID_NOLIMITED_GO
// 
// data.param[0]=0 (request: .... 0,2,4,.... )
// data.param[1]=userId;
// 
// result: 
// success: data.param[0]=0
// failure: data.param[0]=0xFFFFFFFF
//
// notify:
// data.param[0]=1 (notify: ... 1,3,5...)
// data.param[1] (0:OK, 1:Cancel, 2: conditionChanged)
// 
// extdata. (param[1]=2 or requestNo:0)
// ssycPlayInfo
//

#endif
