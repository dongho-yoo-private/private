#ifndef __syc_packetuserserver_h__
#define __syc_packetuserserver_h__

//
// PacketIdz.
// about the management of user.
//
enum eUserPacketId {
	eUPID_REQUEST_LOGIN = 256
	eUPID_REQUEST_DATA,
	eUPID_UPDATE_USER_DATA,
	eUPID_GET_USER_INFO,
	eUPID_GET_USER_DETAIL,
	eUPID_SEARCH_USER_ID,
	eUPID_GET_PASSPORT,
	eUPID_ADD_MY_FRIEND,
	eUPID_GET_MY_FRIENDS_STATUS
};

//
// PacketId: eUPID_REQUEST_LOGIN 
//
// packet.data.param[0]: (rand1&rand2&rand3)
// packet.data.param[1]: userId
// packet.data.param[2]: last sync no.
//
// success: 
//    data.param[0]=0
//    data.param[1]= eULPF_USER_BASIC sycnId
//    data.param[2]= eULPF_USER_DETAIL syncId / eULPF_MESSAGE syncId
//    data.param[3]= eULPF_MYFRIENDS syncId / eULPF_SYSTEM_MESSAGE syncId
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
	eULPF_SYSTEM_MESSAGE = 	1<<25
};

// error code.
enum eUserLoginErrorCode {
	eULEC_INVALID_PASSWORD,
	eULEC_INVALID_USERNAME
};

// --------------------------
typedef struct _sLoginExtraData
{
	char encoded_id[__str_encoded_size__(YI_MAX_NAME_LENGTH)];
	char encoded_password[__str_encoded_size__(YI_MAX_PASSWORD_LENGTH)];
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

#endif
