#include "yiclient.h"
#include "yi.h"
#include "yistd.h"
#include "yiEncoder.h"

#include <stdio.h>
#ifdef __POSIX__
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <netdb.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#include <io.h>
#endif


// --------------------------------
long cyiClient::htol(const char* pszHostName, int index)
{
	//host2addr(HOSTNAME, &addr.sin_addr);
	int i=0;
	struct hostent* host = gethostbyname(pszHostName);
	return *((long*)host->h_addr_list[index]);
}

// --------------------------------
long cyiClient::itol(const char* pszIpAddress)
{
	return inet_addr(pszIpAddress);
	unsigned long a, b, c, d;
	if (sscanf(pszIpAddress, "%d.%d.%d.%d", &a, &b, &c, &d)!=4)
	{
		return 0;
	}

	return __ipaddress__(a, b, c, d);
}
#ifdef __POSIX__
// ------------------------------
cyiClient::cyiClient(const char* pszPortName, const char* pszServerName)
{
	m_bNoEncode=True;

	m_nPortNo=-1;
	//yistrcpy(m_szPortName, pszPortName);
	strcpy(m_szPortName, pszPortName);

	if (pszServerName)
	{
		strcpy(m_szServerName, pszServerName);
		//yistrcpy(m_szServerName, pszServerName);
		m_bNoEncode=False;

		cyiEncoder::securityCode(m_szServerName, __YOTSUYA_RANDOM_NUMBER__ , __STD_MASK__ , &m_security.nRandNo, &m_security.nMask, &m_security.nSecurity);

		//yiTrace("security code (%x, %x, %x)\n", m_security.nRandNo, m_security.nMask, m_security.nSecurity);
	}
}
#endif

// ------------------------------
cyiClient::cyiClient(unsigned int address, int port, const char* pszServerName)
{
	m_bNoEncode=True;

	*((long*)m_szPortName)=0;

	m_nPortNo=port;
	m_addr = address;


	if (pszServerName)
	{
		yistrcpy(m_szServerName, pszServerName);
		m_bNoEncode=False;

		cyiEncoder::securityCode(m_szServerName, __YOTSUYA_RANDOM_NUMBER__ , __STD_MASK__ , &m_security.nRandNo, &m_security.nMask, &m_security.nSecurity);
	}
}

// --------------------------------
cyiClient::~cyiClient()
{
	disconnect();
}

// --------------------------------
Bool cyiClient::connect(int nRetry)
{
	if (m_nPortNo!=-1)
	{
		return connect_svr(nRetry);
	}
#ifdef __POSIX__
	return connect_local(nRetry);
#endif
	return False;
}


// --------------------------------
Bool cyiClient::disconnect()
{
	closesocket(m_sock);
	return True;
}

// --------------------------------
int cyiClient::write(void* buffer, int size)
{
	return ::write(m_sock, buffer, size);
}

// --------------------------------
Bool cyiClient::write(ssyNormalPacket& packet)
{
	if (m_bNoEncode==False)
	{
		return cyiPacket::WriteNormalPacket(m_sock, packet, m_security, True);
	}

	if (::write(m_sock, &packet, sizeof(packet))!=sizeof(packet))
	{
		return False;
	}

	return True;
}

// --------------------------------
Bool cyiClient::writer(ssyNormalPacket& packet, int nTimeOut)
{
	if (m_bNoEncode==False)
	{
		if (cyiPacket::WriteNormalPacket(m_sock, packet, m_security, True)==False)
		{
			return False;
		}

	}

	return 		cyiClient::read(packet, nTimeOut);
}

// --------------------------------
Bool cyiClient::writex(ssyNormalPacket& packet, const void* extData, int extSize)
{
	if (m_bNoEncode==False)
	{
		return cyiPacket::WriteNormalPacket(m_sock, packet, extData, extSize, m_security, True);
	}

	if (::write(m_sock, &packet, sizeof(packet))!=sizeof(packet))
	{
		return False;
	}

	if (::write(m_sock, extData, extSize)!=extSize)
	{
		return False;
	}

	return True;
}

// --------------------------------
Bool cyiClient::writexr(ssyNormalPacket& packet, const void* extData, int extSize, int nTimeOut)
{
	if (m_bNoEncode==False)
	{
		if (cyiPacket::WriteNormalPacket(m_sock, packet, extData, extSize, m_security, True)==False)
		{
			return False;
		}
	}
	return cyiClient::read(packet, nTimeOut);
}

// --------------------------------
int cyiClient::read(void* buffer, int size, int TimeOutMilliSecond)
{
	if (TimeOutMilliSecond!=-1)
	{
		if (cyiPacket::timeout(m_sock, TimeOutMilliSecond)==True)
		{
			return False;
		}
	}
	return ::read(m_sock, buffer, size);
}

// --------------------------------
Bool cyiClient::read(ssyNormalPacket& packet, int TimeOutMilliSecond)
{
	if (TimeOutMilliSecond!=-1)
	{
		if (cyiPacket::timeout(m_sock, TimeOutMilliSecond)==True)
		{
			return False;
		}
	}
	if (m_bNoEncode==False)
	{
		return cyiPacket::ReadNormalPacket(m_sock, packet, m_security)==ePR_SUCCESS ? True:False;
	}

	return ::read(m_sock, &packet, sizeof(packet))==sizeof(packet) ? True:False;
}

// --------------------------------
Bool cyiClient::readw(ssyNormalPacket& packet, int TimeOutMilliSecond)
{
	Bool res = cyiClient::read(packet, TimeOutMilliSecond);
	if (res==FALSE)
	{
		return False;
	}
	return cyiClient::write(packet);
}

// --------------------------------
Bool cyiClient::readex(void* extData, int extSize, int TimeOutMilliSecond)
{
	if (TimeOutMilliSecond!=-1)
	{
		if (cyiPacket::timeout(m_sock, TimeOutMilliSecond)==True)
		{
			return False;
		}
	}

	if (::read(m_sock, extData, extSize)!=extSize)
	{
		return False;
	}

	return True;
}
#ifdef __POSIX__
// --------------------------------
Bool cyiClient::connect_local(int nRetry)
{
	struct sockaddr_un addr;

	if ((m_sock=  socket(AF_UNIX, SOCK_STREAM, 0))<0)	
	{
		SYSLOG(LogLevel::ERROR, "create socket error \n");
		return -1;
	}

	//MEM_ZERO(addr);
	int addr_size;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;

	//yiTrace("port name: %s\n", m_szPortName);
	strncpy(addr.sun_path, m_szPortName, sizeof(addr.sun_path));

	addr_size = sizeof(addr.sun_family) + strlen(addr.sun_path);

RETRY:
	if (::connect(m_sock, (struct sockaddr *)&addr, addr_size)!=0)
	{
		if (nRetry==0)
		{
			return False;
		}

		yiTrace("connect retry....(To %s)\n", m_szPortName);
		nRetry--;
		goto RETRY;
	}

	return True;
}
#endif

// --------------------------------
Bool cyiClient::connect_svr(int nRetry)
{
	struct sockaddr_in addr;

	if ((m_sock=  socket(AF_INET, SOCK_STREAM, 0))<0)	
	{
		return -1;
	}

	//MEM_ZERO(addr);
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_nPortNo);
	addr.sin_addr.s_addr = m_addr;


RETRY:
	if (::connect(m_sock, (struct sockaddr *)&addr, sizeof(addr))!=0)
	{
		if (nRetry==0)
		{
			return False;
		}

		yiTrace("connect retry....(To %d.%d.%d.%d)\n", ((char*)m_addr)[0], ((char*)m_addr)[1], ((char*)m_addr)[2], ((char*)m_addr)[3]);
		nRetry--;
		goto RETRY;
	}

	return True;
}
