#include "svryuser.h"
#include "sycTypes.h"
#include "sycUser.h"

// --------------------------
csvryuser::csvryuser(const char* servername)
	: csvrybase(servername)
	 , m_PreRegisterEmailList(True)
{
}

// --------------------------
csvryuser::~csvryuser()
{
}

// --------------------------
Bool csvryuser::OnMessage(int sock, ssyNormalPacket& packet, void** pExtraDataRef)
{
	switch(packet.id)
	{
		case eUPID_REQUEST_SIMPLE_LOGIN:
		case eUPID_REQUEST_LOGIN:
			// for beta edition.
			return OnSimpleLogIn(sock, packet, pExtraDataRef);
		//case eUPID_REQUEST_USER_INFO:
		//	return OnRequestUserInfo(sock, packet);
		case eUPID_REQUEST_DATA:
			return OnRequestUserInfo(sock, packet);
			
		default:
			return False;
	}
	return True;		
}

//
Bool csvryuser::OnMessageLocal(int sock, ssyNormalPacket& packet, void** pExtraDataRef)
{
	switch(packet.id)
	{
		case eUPID_REQUEST_PREREGISTER:
			return OnPreRegister(sock, packet);
		case eUPID_REQUEST_REGISTER:
			return OnRegister(sock, packet);
		case eUPID_REQUEST_LOGOUT:
			return OnLogOut(sock, packet);
		case eUPID_REQUEST_MAILADDRESS:
		{
			return OnRequestEmailAddress(sock, packet.data.param[0], packet.data.param[1]);
		}
		default:
			break;
	}

	return csvrybase::OnMessageLocal(sock, packet);
}

// --------------------------
Bool csvryuser::OnServerStart()
{
	if ((m_pUserHash= m_UserDataHash.Load(csycEnv::GetUserRoot(), 8, this, &csvryuser::OnUserDataLoad))==0)
	{
		return False;
	}

	if ((m_pEmailhash= m_EmailHash.Load(csycEnv::GetEmailRoot(), 8))==0)
	{
		return False;
	}

	return True;
}

Bool csvryuser::OnServerEnd()
{
	m_UserDataHash.Delete();
	m_EmailHash.Delete();

	return True;
}



// --------------------------
void csvryuser::OnUserDataLoad(Bit32 layer, void* pExtraData)
{
}

