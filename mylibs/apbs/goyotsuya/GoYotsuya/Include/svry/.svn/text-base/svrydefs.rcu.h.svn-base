

// -------------------------------------
enum eErrorRcu {
	eERCU_OK= 0,
	eERCU_INVALID_YOUR_INDEX= -1,
	eERCU_INVALID_REMOTE_INDEX= -2,
	eERCU_SERVER_IS_BUSY= -3,
	eERCU_NO_SUPPORT=-4
};

// -------------------------------------
typedef struct xsUthpConnectData {
	int myIndex;
	int remoteIndex;
	char password[8];
} sUthpConnectData;

// -------------------------------------
enum eConnectMethod {
	eCM_DIRECT,
	eCM_DIRECT_TCP,
	eCM_WAIT,
	eCM_HOLE_PUNCHING,
	eCM_USE_STUNE,
	eCM_TCP_PROXY,
	eCM_REGISTER_NETWORK
};

// -------------------------------------
typedef struct xsUthpConnectReply{
	int result;
	unsigned int address;
	int port;
	int randNo;
	eConnectMethod method;
} sUthpConnectReply;

// -------------------------------------
typedef struct {
	unsigned int randNo;
	sAddress local;
} sUthpTest;

// -------------------------------------
typedef struct xsUthpTestResult{
	sAddress me;
	sAddress remote;
	sAddress remoteLocal;
} sUthpTestResult;

// -------------------------------------
typedef struct xsUthpProxyInfo {
	int result;
	unsigned int address;
	int port;
	int passport;
	int id;
} sUthpProxyInfo;

// eUSPID_REQ_CHECK_NAT_TYPE
// sAddress (address, port)

// eUSPID_RPY_CHECK_NAT_TYPE
// 0: result
// 1: port1
// 2: port2
// 3: randomKey
//
//
// eUSPID_RPY_CHECK_NAT_TYPE from udp.
// sGerneralParam
// 1: eNatType
// 2: your address
// 3: your port

//eUSPID_RPY_CONNECT_UDP
// 0: result.
// 1: remote address.
// 2: remote port.
// 3: randomNo.
// 4: method.



