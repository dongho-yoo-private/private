#include "yistd.h"
#include "yisock.h"
#include <windows.h>

//#define _DEBUG_

HandleMutex hTraceMutex=0;
int m_log_sock=0;
unsigned m_address=0;


void yiLogInit()
{
#ifdef _DEBUG_
	if (hTraceMutex==0)
	{
		hTraceMutex = yiMutexCreate(0);
	}

	if (m_log_sock==0)
	{
		m_log_sock = yiudp(0, 0);
	}
	m_address = yiitol("127.0.0.1");
#endif
}
void yiLogFin()
{
#ifdef _DEBUG_
	yiMutexDestroy(hTraceMutex);
	yiclose(m_log_sock);

	m_log_sock=0;
	hTraceMutex=0;
#endif
}

#include <process.h>

void yiTrace(const char* fmt, ...)
{
#ifdef _DEBUG_
	va_list ap;
	char buffer[4096];
	SYSTEMTIME time;
	void** p = (void**)&fmt;
	DWORD id;
	GetLocalTime(&time);

	if (m_log_sock==0)
	{
		yiLogInit();
	}

	//m_address = yiitol("127.0.0.1");

	id = GetProcessId(GetCurrentProcess());
 
	wsprintfA(buffer, "[%4d/%02d/%02d %02d:%02d:%02d] %08x> ", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, id);

	va_start(ap, fmt);
	p++;
	vsprintf(&buffer[32], fmt,(va_list) p);
	strcat(&buffer[32], "\r\n");

	yisendu(m_log_sock, buffer, strlen(buffer)+1, m_address, YOTSUYA_LOG_PORT);
#endif
}

#include "yiMutex.h"

void yiTraceM(const char* fmt, ...)
{
	va_list ap;
	char name[256];
	char buffer[4096];
	void** p = (void**)&fmt;
	SYSTEMTIME time;

	GetLocalTime(&time);


	GetModuleFileName(0, name, 256);
	strcat(name, ".log");

	wsprintfA(buffer, "[%4d/%02d/%02d %02d:%02d:%02d] > ", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	va_start(ap, fmt);
	p++;
	vsprintf(&buffer[24], fmt,(va_list) p);
	strcat(&buffer[24], "\r\n");

	yiFileWrite(name, buffer, -1, strlen(buffer));
}

// ------------------------------
void yiTraceM2(const char* pszFileName, const char* fmt, ...)
{
	va_list ap;
	char name[256];
	char buffer[4096];
	void** p = (void**)&fmt;
	SYSTEMTIME time;

	GetLocalTime(&time);


	//GetModuleFileName(0, name, 256);
	//strcat(name, ".log");

	wsprintfA(buffer, "[%4d/%02d/%02d %02d:%02d:%02d] > ", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	va_start(ap, fmt);
	p++;
	vsprintf(&buffer[24], fmt,(va_list) p);
	strcat(&buffer[24], "\r\n");

	yiFileWrite(pszFileName, buffer, -1, strlen(buffer));
}

int g_yilib_udpSock=0;
int g_yilib_nPort=514;
Bool g_yilib_IsDebug=False;

// ------------------------------
void yiTraceInit(int nPort)
{
	yiInitWinsock(2, 2);
	if (g_yilib_udpSock==0)
	{
		g_yilib_udpSock = yiudp(0, 0);
	}

	if (nPort)
		g_yilib_nPort=nPort;
}

// ------------------------------
void yiTraceM3(unsigned int level, const char* fmt, ...)
{
	int len=0;
	va_list ap;
	char buffer[1024];
	void** p = (void**)&fmt;
	SYSTEMTIME time;
#ifdef _DEBUG
	if (g_yilib_udpSock==0)
	{
		yiTraceInit(0);
		g_yilib_IsDebug=True;
	}
#else
	if (g_yilib_IsDebug==False)
		return ;

	if (g_yilib_udpSock==0)
		return ;
#endif


	GetLocalTime(&time);


	//GetModuleFileName(0, name, 256);
	//strcat(name, ".log");



	wsprintfA(&buffer[4], "#%02d#[%4d/%02d/%02d %02d:%02d:%02d] %08x> ", level, time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, GetCurrentThreadId());
	va_start(ap, fmt);
	p++;

	vsprintf(&buffer[36+4], fmt, (va_list) p);
	strcat(&buffer[36+4], "\r\n");
	len = strlen(&buffer[4]);
	len+=5;

	len = len>1024?1024:len;
	
	if (len==1024)
		buffer[1023]=0;

	level = (1<<level);
	memcpy(buffer, &level, sizeof(unsigned int));
	yisendu(g_yilib_udpSock, buffer, len, 0, g_yilib_nPort);
}
