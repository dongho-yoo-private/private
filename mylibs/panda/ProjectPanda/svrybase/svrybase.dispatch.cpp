#include "svrybase.h"
#include "yiEncoder.h"
#include "yistd.h"
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

// ----------------------------------------------------------------------
void csvrybase::OnLimitedDispatch(unsigned long param1, unsigned long param2, unsigned long param3)
{
	int acceptFd = (int)param3;
	int sock = param1;
	Bool bIsDispatchLocal=param2;
	sServerInformation* pInfo = &m_ServerInfo;
	fd_set fds;

	if (bIsDispatchLocal==True)
	{
		pInfo=&m_LocalInfo;
	}

	FD_ZERO(&fds);
	FD_SET(sock, &fds);

	struct timeval tv = {pInfo->nLimitedTypeTimeOut, 0};
	int res = select(sock+1, &fds, 0, 0, &tv);
	if (res<0)
	{
		SYSLOG(LogLevel::ERROR, "select error %d(errno:%d)\n", res, errno);
		return ;
	}
	else if (res==0)
	{
		close(sock);
		return ;
	}

	syipacket packet;

	if (yirecvx(sock, &packet, 0, pInfo->bIsPacketClear?0:m_security, 0)!=0)
	{
		SYSLOG(LogLevel::ERROR, "invalid packet (local)\n");
		close(sock);
		return ;
	}

	if (bIsDispatchLocal==True)
	{
		OnMessageLocal(sock, packet);
	}
	else
	{
		OnMessage(sock, packet);
	}

	close(sock);
}

// ----------------------------------------------------------------------
static Bool __OnAccepted__(void* pClass, int sock, int id, void** pExtra)
{
	csvrybase* p = (csvrybase*)pClass;
	return p->OnAccepted(sock, id, pExtra);
}

// ----------------------------------------------------------------------
static Bool __OnClosed__(void* pClass, int sock, int index, void** pExtra)
{
	csvrybase* p = (csvrybase*)pClass;
	p->OnClientClose(sock, pExtra);
	close(sock);
	return True;
}

// ----------------------------------------------------------------------
void csvrybase::OnDispatch(unsigned long param1, unsigned long param2, unsigned long param3)
{
	int	acceptFd = (int)param3;
	Bool	bIsLocalDispatch = (Bool)param2;
	cConnectionInfo* pConnInfo = (cConnectionInfo*)param1;
	sServerInformation* pInfo = (bIsLocalDispatch==True)? &m_LocalInfo: &m_ServerInfo;

	//yiTrace("csvrybase::OnDispatch()\n");	

	cyiQueue fds;
	//sSecurityInfo info = {m_security.nRandNo, m_security.nMask, m_security.nSecurity};
	//
	char* pRecvBuffer = (char*)yialloc(pInfo->nMaxPacketSize);
	assert(pRecvBuffer);

	while(1)
	{
		fds.clear();
		if (pConnInfo->Wait(fds, __OnAccepted__, __OnClosed__, this)!=True)
		{
			break;
		}

		int n = fds.count();
		int index;
		int* pfds = pConnInfo->GetFds();

		for (int i=0; i<n; i++)
		{
			if (fds.deq((yiAnyValue&)index)==False)
			{
				yiTrace("assert\n");
				assert(0);
			}

			Bool bHandled=False;
			if (bIsLocalDispatch==False &&
				 OnPreMessage(pfds[index], pConnInfo->GetExtraDataPtr(index), bHandled)==False)
			{
				OnClientClose(pfds[index], pConnInfo->GetExtraDataPtr(index));
				close(pfds[index]);
				pConnInfo->RemoveFd(index);
				continue;
			}

			if (bHandled==True)
			{
				continue;
			}

			syipacket packet;
			if (yirecvx(pfds[index], &packet, 0, pInfo->bIsPacketClear ? 0:m_security, 0)!=0)
			{
				SYSLOG(LogLevel::ERROR, "invalid packet.... (index:%d)\n", index);
			
				OnClientClose(pfds[index], pConnInfo->GetExtraDataPtr(index));
				close(pfds[index]);
				pConnInfo->RemoveFd(index);
				continue;
			}

			Bool res;
			if (bIsLocalDispatch ==True)
			{
				res = OnMessageLocal(pfds[index], packet, pConnInfo->GetExtraDataPtr(index));

				if (res==False)
				{
					break;
				}
			}
			else
			{
				if (packet.sizex > pInfo->nMaxPacketSize)
				{
					SYSLOG(LogLevel::ERROR, "packet size too long %d\n", 0xFFFF&packet.sizex);
					OnClientClose(pfds[index], pConnInfo->GetExtraDataPtr(index));
					close(pfds[index]);
					pConnInfo->RemoveFd(index);
					continue;
				}

				if (packet.sizex==0)
				{
					res = OnMessagex(pfds[index], packet.id, pRecvBuffer, 0, pConnInfo->GetExtraDataPtr(index));
				}
				else
				{
					if (yirecv(pfds[index], pRecvBuffer, packet.sizex, 0)!=packet.sizex)
					{
						SYSLOG(LogLevel::ERROR, "packet size invalid size not %d\n", 0xFFFF&packet.sizex);
						OnClientClose(pfds[index], pConnInfo->GetExtraDataPtr(index));
						close(pfds[index]);
						pConnInfo->RemoveFd(index);
						continue;
					}
					yidecodex(pRecvBuffer, packet.sizex, packet.encode_key);
					res = OnMessagex(pfds[index], packet.id, pRecvBuffer, packet.sizex, pConnInfo->GetExtraDataPtr(index));
					
					// checksum check.
				}

			}

			if (res==False)
			{
				OnClientClose(pfds[index], pConnInfo->GetExtraDataPtr(index));
				close(pfds[index]);
				pConnInfo->RemoveFd(index);
				continue;
			}

			//yiTrace("messsage success %d,%d\n", index, pfds[index]);
			//pConnInfo->FdSet(index);
		} // for
	} // while

	syipacket packet;

	packet.id=m_QuitMode;
	packet.security = m_security;
	yiencodex(&packet, sizeof(packet), m_security);

	pConnInfo->BroadCastMessage(&packet, sizeof(packet));
	pConnInfo->RemoveAll();

	yifree(pRecvBuffer);
	yiTrace("csvrybase::OnDispatch() exit\n");	
}

