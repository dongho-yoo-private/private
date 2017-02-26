#include "svryuser.h"

// --------------------------
Bool csvryuser::OnReqTcpProxyConnection(int sock, int myIndex, int remote, void** pExtraDataRef)
{
	syiLoginUserInfo* p = (syiLoginUserInfo*)*pExtraDataRef;
	if (p==0 ||
		p->index!=myIndex ||
			myIndex==remote)
	{
		sUthpProxyInfo reply = {eERCU_INVALID_YOUR_INDEX, 0, 0, 0, 0};
		if (yisendx(sock, eUSPID_RPY_CONNECT_TCP_PROXY, m_security, p->security, &reply, sizeof(reply), True)!=sizeof(reply))
		{
			return True;
		}
	}

	syiLoginUserInfo* pUserInfo = GetUserLoginInfo(remote);
	if (pUserInfo==0)
	{
		sUthpProxyInfo reply = {eERCU_INVALID_REMOTE_INDEX, 0, 0, 0, 0};
		if (yisendx(sock, eUSPID_RPY_CONNECT_TCP_PROXY, m_security, p->security, &reply, sizeof(reply), True)!=sizeof(reply))
		{
			return True;
		}
	}

	sUthpConnectData data = {remote, myIndex};
	if (yisendx(pUserInfo->sock, eUSPID_REQ_CONNECT_TCP_PROXY, m_security, p->security, &data, sizeof(data), True)!=sizeof(data))
	{
		sUthpProxyInfo reply = {eERCU_SERVER_IS_BUSY, 0, 0, 0, 0};
		if (yisendx(sock, eUSPID_REQ_CONNECT_TCP_PROXY, m_security, p->security, &reply, sizeof(reply), True)!=sizeof(reply))
		{
			return True;
		}
		return True;
	}

	return True;
}

// --------------------------
Bool csvryuser::OnReqTcpProxyConnectionOK(int sock, int myIndex, int remote, void** pExtraDataRef)
{
	syiLoginUserInfo* p = (syiLoginUserInfo*)*pExtraDataRef;
	if (p==0 ||
		p->index!=myIndex ||
			myIndex==remote)
	{
		sUthpProxyInfo reply = {eERCU_INVALID_YOUR_INDEX, 0, 0, 0, 0};
		if (yisendx(sock, eUSPID_RPY_CONNECT_TCP_PROXY, m_security, p->security, &reply, sizeof(reply), True)!=sizeof(reply))
		{
			return True;
		}
	}

	syiLoginUserInfo* pUserInfo = GetUserLoginInfo(remote);
	if (pUserInfo==0)
	{
		sUthpProxyInfo reply = {eERCU_INVALID_REMOTE_INDEX, 0, 0, 0, 0};
		if (yisendx(sock, eUSPID_RPY_CONNECT_TCP_PROXY, m_security, p->security, &reply, sizeof(reply), True)!=sizeof(reply))
		{
			return True;
		}
	}

	int passport, id, port;
	Bool res = GetPassportFromProxy(&passport, &id, &port);

	if (res==False)
	{
		sUthpProxyInfo reply = {eERCU_SERVER_IS_BUSY, 0, 0, 0, 0};
		yisendx(sock, eUSPID_RPY_CONNECT_TCP_PROXY, m_security, p->security, &reply, sizeof(reply), True);
		yisendx(info->sock, eUSPID_RPY_CONNECT_TCP_PROXY, m_security, info->security, &reply, sizeof(reply), True);
		return True;
	}

	sUthpProxyInfo info;
	info.result	= eERCU_OK;
	info.address 	= m_ProxyServerAddress;
	info.port	= m_ProxyServerPort;
	info.passport   = passport;
	info.port 	= port;

	yisendx(sock, eUSPID_RPY_CONNECT_TCP_PROXY, m_security, p->security, &info, sizeof(info), True);
	yisendx(info->sock, eUSPID_RPY_CONNECT_TCP_PROXY, m_security, info->security, &info, sizeof(info), True);
	
	return True;
}
