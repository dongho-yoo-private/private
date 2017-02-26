
typedef struct {
	unsigned int id;
	unsigned int sock;
	unsigned int security;	
	sNetworkInfo network;
	HandleMutexx hMutex;
	Bool		isLogined;
	char szUserName[16];
	char szPassword[8];
} syiLoginUserInfo;

typedef struct {
	unsigned int id;
	unsigned int sock;
	unsigned int security;	
	sNetworkInfo network;
	HandleMutexx hMutex;
	Bool		isLogined;
	void** pUserFileTable;
	unsigned int absAddr;
	unsigned int absPort;
} syiLoginUserInfo2;

// -------------------------------------
typedef struct xsyiLoginData {
	unsigned int	reserved;
	unsigned int	id;
	char		szName[16];
	char		szPassword[8];
	sNetworkInfo	network;
} __attribute__((aligned(1))) syiLoginData;

// -------------------------------------
typedef struct syiLoginReply {
	unsigned int id;
	unsigned int index;
	unsigned int sock;
	unsigned int security;
	Bool	     network;
} syiLoginReply;


// -------------------------------------
// id, password, address 
//
//
typedef struct xsyiLoginRecvData {
	unsigned int id;
	unsigned int passport;
	unsigned int index;
	unsigned int lastlogin;
	unsigned int lastupdate;
	sNetworkInfo network;
} syiLoginRecvData;
