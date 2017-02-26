#ifndef __udp_base_h__
#define __udp_base_h__

#include "yisock.h"

// -----------------------
class cyiUdpBase
{
public:
	cyiUdpBase();
	~cyiUdpBase();
#ifdef WIN32
	// -----------------------
	int listen(const char* pszAdmin, long addr_yotsuya, int port, HWND hNotifyWnd);
#else
	// -----------------------
	int listen(const char* pszAdmin, long addr_yotsuya, int port, int wrPipe);
#endif

	// -----------------------
	int connect(long addr, int port, const char* pszServerName);

	// -----------------------
	int accept(long& addr, int& port);

	// -----------------------
	int write(int fd, const void* buffer, int n);

	// -----------------------
	int read(int fd, void* buffer, int n);

protected:
	// -----------------------
	virtual Bool OnServerStart();

	// -----------------------
	virtual Bool OnServerEnd();

	// -----------------------
	virtual Bool OnMessage(int fd, );

private:
	long m_addr;
	int  m_port;
	long m_addr_yotsuya;
	int  m_port_yotsuya;

private:
	void __recieve__();
	
};

#endif
