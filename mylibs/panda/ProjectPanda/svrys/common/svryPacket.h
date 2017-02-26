#ifndef __svryPacket_h__
#define __svryPacket_h__

/////// User Server //////
//
#define PID_PRE_REGISTER_USER (1<<16)

enum eUserServerPacketId {
	ePID_PRE_REGISTER = ePID_USER+1,
	ePID_REGISTER_USER,
	ePID_REGISTER_SOFT,
	ePID_LOGIN,
	ePID_LOGOUT,
	ePID_SYNC_MY_DATA,
	ePID_GET_DATA	
};

#endif
