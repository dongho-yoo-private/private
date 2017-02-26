#ifndef __SYPACKET_h__
#define __SYPACKET_h__

enum eYotuyaPacketId {
	EPID_TERMINATE = 0x80,
	EPID_TERMINATE_FORCED=0x01 | EPID_TERMINATE,
	EPID_REQUEST_TERMINATE=0x02 | EPID_TERMINATE,
	EPID_ECHO = 1,
	EPID_GET_STATUS,
	EPID_ADD_CONNECTION,

	EPID_USER_DEFINE = 1024
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

// ------------------------------
class csyPacket
{
public:
	// ------------------------------
	static bool ReadNormalPacket(int sock, ssyNormalPacket& packet, sSecurityInfo& info); 

	// ------------------------------
	static bool WriteNormalPacket(int sock, ssyNormalPacket& packet, sSecurityInfo& info); 

	// ------------------------------
	static bool WriteNormalPacket(int sock, ssyNormalPacket& packet, void* extData, int extSize, sSecurityInfo& info); 

	// ------------------------------
	static bool InitPacket(ssyNormalPacket& packet); 

	// ------------------------------
	static bool write(int sock, void* buffer, int size);
};

#endif
