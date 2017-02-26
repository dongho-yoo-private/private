#include "svryproxy.h"
#include <stdlib.h>
#include <sys/time.h>

// --------------------------
csvryproxy::csvryproxy(const char* servername)
	: csvrybase(servername)
{
}

// --------------------------
csvryproxy::~csvryproxy()
{
}

// -------------------------------------
Bool csvryproxy::OnAccepted(int sock, int index, void** pExtra)
{
	syipacket packet;
	sProxyConnectData proxy;	

	if (yirecvx(sock, &packet, &proxy, m_security, 500)!=sizeof(proxy))
	{
		return False;
	}

	if (packet.id!=ePXPID_CONNECT)
	{
		return False;
	}

	// OK.
	if (m_table[proxy.id].passport==proxy.passport)
	{
		if (m_table[proxy.id].sock1!=0)
		{
			m_table[proxy.id].sock2=sock;
			m_table[proxy.id].created_time=0xFFFFFFFF;
			yisendx(sock, ePXPID_CONNECT_OK, m_security, 0, 0, 0, True);
			yisendx(m_table[proxy.id].sock1, ePXPID_CONNECT_OK, m_security, 0, 0, 0, True);
			return True;
		}
	}

	m_table[proxy.id].sock1=sock;

	return True;
}

// -------------------------------------
Bool csvryproxy::OnReqGetPassport(int* id, int* passport)
{
	yiMutexLock(m_hMutex);
	struct timeval tv;/* = {0,0};*/
	if (gettimeofday(&tv, 0)!=0)
	{
		yiMutexUnlock(m_hMutex);
		return False;
	}

	for (int i=0; i<m_nMaxTableCnt; i++)
	{
		if (m_table[i].created_time==0)
		{
			m_table[i].created_time = tv.tv_sec;
			*id=i;
			*passport=rand();
			m_table[i].passport=*passport;
			yiMutexUnlock(m_hMutex);
			return i;			
		}
	}

	for (int i=0; i<m_nMaxTableCnt; i++)
	{
		if (m_table[i].created_time==0xFFFFFFFF)
			continue;

		int n = tv.tv_sec-m_table[i].created_time;

		if (n<0 ||
			n>1)
		{
			m_table[i].created_time = tv.tv_sec;
			*id=i;
			*passport=rand();
			m_table[i].passport=*passport;
			yiMutexUnlock(m_hMutex);
		}
	}
	yiMutexUnlock(m_hMutex);
	
	return False;
}

// -------------------------------------
Bool csvryproxy::OnMessageLocal(int sock, syipacket& packet, void** pExtraData)
{
	if (packet.id==ePID_REQ_PASSPORT)
	{
		int id, passport;
		if (OnReqGetPassport(&id, &passport)==True)
		{
			sProxyPassportInfo info = {m_ServerInfo.nPortNo, passport, id};
			yisendx(sock, ePID_REQ_PASSPORT, 0, 0, &info, sizeof(info), False);
			return True;
		}
		return False;
	}
	return csvrybase::OnMessageLocal(sock, packet, pExtraData);
}

// -------------------------------------
Bool csvryproxy::OnPreMessage(int sock, void** pExtraData, Bool& bIsHandled)
{
	bIsHandled=True;
	if (pExtraData==0)
	{
		return False;
	}

	sProxyTable* p = (sProxyTable*)*pExtraData;

	if (p==0)
	{
		return False;
	}

	char buffer[4096];
	int n = yirecv(sock, buffer, m_ServerInfo.nMaxPacketSize, 0);

	if (n<=0)
	{
		return False;
	}

	if (yisend((p->sock1==sock)?p->sock2:p->sock1, buffer, n, True)!=n)
	{
		yiTrace("packet is loses....\n");
		return True;
	}

	return True;
}

// -----------------------------------
void csvryproxy::OnClientClose(int sock, void** pExtraDataRef)
{
	if (pExtraDataRef==0)
	{
		return ;
	}

	yiMutexLock(m_hMutex);
	sProxyTable* p = (sProxyTable*)*pExtraDataRef;

	if (p==0)
	{
		yiMutexUnlock(m_hMutex);
		return ;
	}

	if (p->sock1==sock)
	{
		close(p->sock2);
	}
	else
	{
		close(p->sock1);
	}

	p->created_time=0;
	p->sock1=0;
	p->sock2=0;
	p->passport=0;

	yiMutexUnlock(m_hMutex);
}

// --------------------------
void csvryproxy::InitTable()
{
	m_nMaxTableCnt = m_ServerInfo.extra[0];
	int n = sizeof(sProxyTable)*m_nMaxTableCnt;
	m_table = (sProxyTable*)yialloc(n);
	memset(m_table, 0, n);
	m_hMutex = yiMutexCreate(0);

	srand(time(0));
}

Bool csvryproxy::OnServerStart()
{
	InitTable();
	return True;
}

	//
Bool csvryproxy::OnServerEnd()
{
	yifree(m_table);
	yiMutexDestroy(m_hMutex);
	return True;
}


