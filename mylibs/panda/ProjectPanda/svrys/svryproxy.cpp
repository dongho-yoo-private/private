#include "svryproxy.h"
#include "yiEncoder.h"
#include "yiHashFunc.h"
#include "sycProxyUser.h"
#include "string.h"

// --------------------------
csvryproxy::csvryproxy(const char* servername)
	: csvrybase(servername)
{
}

// --------------------------
csvryproxy::~csvryproxy()
{
}

// --------------------------
Bit32 csvryproxy::GetUserId(const char* pszUserName)
{
	int randuser1 = yihf_additional(pszUserName, 8);
	int randuser2 = yihf_encoded_additional(pszUserName, 8);
	int randuser3 = yihf_general(pszUserName, 8);
	int n = strlen(pszUserName);
	int cnt=0;	
	cyiList* list = m_UserDataHash.GetList(__8to32__(randuser1, randuser2, randuser3, 0)); 

	for (cyiNode* p = list->GetNodeTopFrom(0); p!=0; p=p->np)
	{
		++cnt;
		if (memcmp(pszUserName, p->ref, n+1)==0)
		{
			m_UserDataHash.ReleaseList(list);
			return __8to32__(randuser1, randuser2, randuser3, cnt);
		}
	}

	m_UserDataHash.ReleaseList(list);
	return 0;
}

// --------------------------
Bool csvryproxy::OnMessage(int sock, ssyNormalPacket& packet, void** pExtraDataRef)
{
	if (packet.id == 1) // create room
	{
		packet.data.param[0]=sock;
		return cyiPacket::WriteNormalPacket(sock, packet, m_security);
	}
	else if (packet.id == 2) // get room no
	{
		char buffer[32];
		if (::read(sock, buffer, 32)!=32)
		{
			packet.data.param[0]=0xFFFFFFFF;
			return cyiPacket::WriteNormalPacket(sock, packet, m_security);
		}

		Bit32 id = packet.data.param[0];

		if (id==0)
		{
			cyiEncoder::dstr(buffer);
			id = GetUserId(buffer);

			if (id==0)
			{
				packet.data.param[0]=0xFFFFFFFE; // invalid user.
				return cyiPacket::WriteNormalPacket(sock, packet, m_security);
			}
		}

		sUserInfo* pUser = (sUserInfo*)m_UserDataHash.GetData(id, 0); 

		if (pUser==0)
		{
			packet.data.param[0]=0xFFFFFFFE; // invalid user
			return cyiPacket::WriteNormalPacket(sock, packet, m_security);
		}

		if (pUser->sock==0)
		{
			packet.data.param[0]=0xFFFFFFFD; // user logoff
			return cyiPacket::WriteNormalPacket(sock, packet, m_security);
		}
		packet.data.param[0]=pUser->sock;
		packet.data.param[1]=pUser->id;
		return cyiPacket::WriteNormalPacket(sock, packet, m_security);
			
	}
	else if (packet.id==3) // login
	{
		char buffer[64];
		if (::read(sock, buffer, 64)!=64)
		{
			packet.data.param[0]=0xFFFFFFFF;
			return cyiPacket::WriteNormalPacket(sock,  packet, m_security);
		}

		cyiEncoder::dstr(buffer);
		cyiEncoder::dstr(&buffer[32]);

		Bit32 id = packet.data.param[1];

		if (id==0)
		{
			id = GetUserId(buffer);
			if (id==0)
			{
				packet.data.param[0]=0xFFFFFFFE; // invalid id
				return cyiPacket::WriteNormalPacket(sock, packet, m_security);
			}
		}

		sUserInfo* pUser = (sUserInfo*)m_UserDataHash.GetData(id, 0); 

		if (pUser==0)
		{
			packet.data.param[0]=0xFFFFFFFE; // invalid id
			return cyiPacket::WriteNormalPacket(sock, packet, m_security);
		}

		if (memcmp(pUser->password, &buffer[32], 8)!=0)
		{
			packet.data.param[0]=0xFFFFFFFD; //invalid password
			return cyiPacket::WriteNormalPacket(sock, packet, m_security);
		}

		pUser->sock = sock;
		packet.data.param[0]=sock;
		return cyiPacket::WriteNormalPacket(sock, packet, pUser, sizeof(sUserInfo), m_security);
	}
	else if (packet.id==4) // room enter success.
	{
		sUserConnectionInfo* p = (sUserConnectionInfo* )(*pExtraDataRef);
		if (p==0)
		{
			p = (sUserConnectionInfo*)yialloc(sizeof(sUserConnectionInfo));
			p->connects = new cyiList();
			p->connecteds = new cyiList();
			*pExtraDataRef = (void*)p;
		}

		if (packet.data.param[0]!=0)
		{
			p->connects->addref((void*)packet.data.param[0]);
		}
		else
		{
			p->connecteds->addref((void*)packet.data.param[1]);
		}

		p->id = packet.data.param[2];
		return cyiPacket::WriteNormalPacket(sock, packet, m_security);
	}
	else if (packet.id==5) // room leave success.
	{
		sUserConnectionInfo* p = (sUserConnectionInfo* )(*pExtraDataRef);

		if (packet.data.param[0]!=0)
		{
			p->connects->removeref((void*)packet.data.param[0]);
		}
		else
		{
			p->connects->removeref((void*)packet.data.param[1]);
		}

		return cyiPacket::WriteNormalPacket(sock, packet, m_security);
	}
	else if (packet.id==0)
	{
		char buffer[4096];


		if (packet.data.param[2]>4096)
		{
			return False;
		}

		if (packet.data.param[2]!=0)
		{
			::read(sock, buffer, packet.data.param[2]);
			packet.data.param[3]=sock;
			return cyiPacket::WriteNormalPacket(packet.data.param[0], packet, buffer, packet.data.param[2], m_security);
		}
		packet.data.param[3]=sock;
		return cyiPacket::WriteNormalPacket(packet.data.param[0], packet, m_security);
	}

	return False;
}

// --------------------------
//Bool csvryuser::OnMessageLocal(int sock, ssyNormalPacket& packet, void** pExtraDataRef)
//{
//}

// -----------------------------------
void csvryproxy::OnClientClose(int sock, void** pExtraDataRef)
{
	sUserConnectionInfo* p = (sUserConnectionInfo*)(*pExtraDataRef);

	if (p==0)
	{
		return ;
	}

	ssyNormalPacket packet;
	memset(&packet, 0, sizeof(packet));

	packet.data.param[1]=256;

	for (cyiNode* pNode = p->connects->GetNodeTopFrom(0); pNode!=0; pNode=pNode->np)
	{
		int rsock = (int)pNode->ref;
		packet.data.param[2]=sock;
		cyiPacket::WriteNormalPacket(rsock, packet, m_security);
	}

	for (cyiNode* pNode = p->connecteds->GetNodeTopFrom(0); pNode!=0; pNode=pNode->np)
	{
		int rsock = (int)pNode->ref;
		packet.data.param[2]=sock;
		cyiPacket::WriteNormalPacket(rsock, packet, m_security);
	}

	delete p->connects;
	delete p->connecteds;

	yifree(*pExtraDataRef);
	*pExtraDataRef=0;
}

// --------------------------
Bool csvryproxy::OnServerStart()
{
	if ((m_pUserHash=m_UserDataHash.Load(csycEnv::GetUserRoot(), 8, 0, 0))==0)//this, &csvryproxy::OnUserDataLoad))==0)
	{
		//return False;
	}
	return True;
}

// --------------------------
Bool csvryproxy::OnServerEnd()
{
	return True;
}
