#include "svryupdate.h"


// -------------------------
csvryupdate::csvryupdate(const char* pszServerName)
	: csvrybase(servername)
{

}

// -------------------------
csvryupdate::~csvryupdate()
{
}

// -------------------------
sPackageInfoHeader* LoadPackageFile(const char* pszFileName)
{
	void* p = yiFileLoad(pszFileName);

	if (p==0)
		return 0;

	char* pIndex = (char*)p;
	pIndex = strstr(pIndex, "current version ");

	if (pIndex==0)
		return 0;

	pIndex=&pIndex[strlen("current version ")];

	int a, b, c, d;
	sscanf("%d.%d.%d.%d", &a, &b, &c, &d);

	pIndex=strstr(pIndex, "\n");

	while(1)
	{
		pIndex=strstr(pIndex, "[");
		sscanf("%d.%d.%d.%d", &a, &b, &c, &d);
	}


}

// -------------------------
Bool csvryupdate::OnServerStart()
{
	m_pPackageInfo = LoadPackageFile("yGo.update");

	if (pPackageList==NULL)
		return False;

	return True;
}

// -------------------------
Bool csvryupdate::OnMessage(int sock, ssyNormalPacket& packet, void** pExtraDataRef)
{
	switch(packet.id)
	{
		case 
		default:
			return False;
	};

	return True;
}

