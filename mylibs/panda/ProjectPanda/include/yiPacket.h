#ifndef __SYPACKET_h__
#define __SYPACKET_h__

#define __YOTSUYA_RANDOM_NUMBER__ 0xe7350ac1
#define __STD_MASK__ 0x3C2D4B5A

#include "yi.h"

enum eYotuyaPacketId {
	ePID_TERMINATE = 0x80,
	ePID_TERMINATE_FORCED=0x01 | ePID_TERMINATE,
	ePID_REQUEST_TERMINATE=0x02 | ePID_TERMINATE,
	ePID_ECHO = 1,
	ePID_GET_STATUS,
	ePID_ADD_CONNECTION,

	ePID_USER_DEFINE = 1024
};

// -----------------------------
typedef struct xsSecurityInfo{
	int nRandNo;
	int nMask;
	int nSecurity;
}sSecurityInfo;

// -----------------------------
typedef struct xssyNormalPacket {
	int reserved;
	int reserved2;
	unsigned int size;
	unsigned int id;
	union __data__ {
		char data[16];	
		int  param[4];
	}data;
} ssyNormalPacket;

#define __YOTSUYA_NORMAL_PACKET_SIZE__ (sizeof(ssyNormalPacket)<<1)

// -------------------
enum ePacketResult
{
	ePR_SUCCESS=0,
	ePR_INVALID_PACKET,
	ePR_INVALID_SOCKET
};

// ------------------------------
class cyiPacket
{
public:
	// ------------------------------
	static ePacketResult ReadNormalPacket(int sock, ssyNormalPacket& packet, sSecurityInfo& info); 

	// ------------------------------
	static Bool WriteNormalPacket(int sock, ssyNormalPacket& packet, sSecurityInfo& info, Bool bIsSyn=False); 

	// ------------------------------
	static Bool WriteNormalPacket(int sock, ssyNormalPacket& packet, const void* extData, int extSize, sSecurityInfo& info, Bool bIsSyn=False); 

	// ------------------------------
	static Bool InitPacket(ssyNormalPacket& packet); 

	// ------------------------------
	static Bool write(int sock, const void* buffer, int size);

	// ------------------------------
	static Bool timeout(int sock, int nMilliSecond);
};

#endif
