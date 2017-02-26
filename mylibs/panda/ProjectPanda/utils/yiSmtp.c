#include "yistd.h"
#include "yiSmtp.h"
//#include "yitrace.h"

#ifdef STATIC_COMPILE
#	define yisend_ssl(a, b,c) yisend(a, b, c, False)
#	define yirecv_ssl(a, b, c, d) yirecv(a, b, c, d)
#	define yiconnect_ssl yiconnect
#	define yiclose_ssl yiclose
#	define sockssl_t int
#endif

typedef struct {
	char* pszServerName; // 0x80
	int port; 
	char* id; // 0x40
	char* pass;// 0x20
	char* from; // 0x10
	char* from2; // 0x08
	char* toz; // 0x04
	char* title; // 0x02
	char* body; // 0x01
	void* file;
	int   size;
	unsigned int deleteFlag;
} sSmtpParam;

//#include <pthread.h>

// ------------------------------------
#ifndef _WIN32
void* xSendSmtp(void* param)
#else
void xSendSmtp(void* param)
#endif
{
	sSmtpParam* s = (sSmtpParam*)param;//yialloc(sizeof(sSmtpParam));

	/*printf("xSendSmtp!\n");
	printf("ServerName:%s\n", s->pszServerName);
	printf("id:%s\n", s->id);
	printf("pass:%s\n", s->pass);
	printf("from:%s\n", s->from);
	printf("from2:%s\n", s->from2);
	printf("to:%s\n", s->toz);
	printf("title:%s\n", s->title);
	printf("body:%s\n", s->body);*/
	yiSmtpSend(s->pszServerName, s->port, s->id, s->pass, s->from, s->from2, s->toz, s->title, s->body, False, s->deleteFlag);

	if (s->pszServerName && (s->deleteFlag&0x80)!=0)
		yifree(s->pszServerName);
	if (s->id && (s->deleteFlag&0x40)!=0)
		yifree(s->id);
	if (s->pass && (s->deleteFlag&0x20)!=0)
		yifree(s->pass);
	if (s->from&& (s->deleteFlag&0x10)!=0)
		yifree(s->from);
	if (s->from2&& (s->deleteFlag&0x08)!=0)
		yifree(s->from2);
	if (s->toz&& (s->deleteFlag&0x04)!=0)
		yifree(s->toz);
	if (s->title&& (s->deleteFlag&0x02)!=0)
		yifree(s->title);
	if (s->body&& (s->deleteFlag&0x01)!=0)
		yifree(s->body);
	yifree(s);

#ifdef _WIN32
	_endthread();
#endif
	return ;
}

static int xMakeErrorCode(const char* buffer)
{
	return (buffer[0]-'0')*100+(buffer[1]-'0')*10+(buffer[2]-'0');
}

#define MAX_RCV_BUFFER 256


#define AUTH_LOGIN "AUTH LOGIN\r\n"
#define AUTH_LOGIN_LEN 12 

#define BODY "DATA\r\n"
#define BODY_LEN 6

#define QUIT "QUIT\r\n"
#define QUIT_LEN 6
// ------------------------------------
int yiSmtpSend(const char* pszServerName, int port, const char* id, const char* pass, 
				const char* from, const char* from2, const char* toz, const char* title, const char* body, Bool bIsThread, unsigned int deleteFlag)
{
	address_t address;
	int err_code=0;
	int sock;
	char buffer[MAX_RCV_BUFFER];

	//printf("yiSmtpSend(%x, %x, %x, %x, %x, %x, %x) called\n", pszServerName, id, pass, from, from2, title, body);
	//printf("yiSmtpSend(%s, %s, %s, %s, %s, %s, %s) called\n", pszServerName, id, pass, from, from2, title, body);
	
	if (bIsThread==True)
	{
#ifndef _WIN32
		pthread_t th;
#endif
		sSmtpParam* s = (sSmtpParam*)yialloc(sizeof(sSmtpParam));
		s->pszServerName=(char*)pszServerName;	
		s->port=port;	
		s->id=(char*)id;	
		s->pass=(char*)pass;	
		s->from=(char*)from;	
		s->from2=(char*)from2;	
		s->toz=(char*)toz;	
		s->title=(char*)title;	
		s->body=(char*)body;
		s->deleteFlag=deleteFlag;
	//printf("yiSmtpSend(%x, %x, %x, %x, %x, %x, %x) called\n", pszServerName, id, pass, from, from2, title, body);
#ifndef _WIN32
		if (pthread_create(&th, 0, xSendSmtp, s)!=0)
#else
		if (_beginthread(xSendSmtp, 0, s)!=0)
#endif
		{
			yifree(s);
			return -1;
		}
#ifndef _WIN32
		pthread_detach(&th);
#endif
		return 0;
	}

	address = yiitol(pszServerName);
	if (address==0xFFFFFFFF)
	{
		address=yihtol(pszServerName, 0);
	}
	
	if ((sock=yiconnect(address, port, 1))<0)
	{
		return -1;
	}
	yirecv(sock, buffer, MAX_RCV_BUFFER, 0);

	if (buffer[0]!='2' || 
		buffer[1]!='2' ||
			buffer[2]!='0')
	{
		goto SEND_FAILURE;
	}

	if (id)
	{
		char x[64];

		//printf(AUTH_LOGIN);
		if (yisend(sock, AUTH_LOGIN, AUTH_LOGIN_LEN, False)!=AUTH_LOGIN_LEN)
		{
			goto SEND_FAILURE;
		}

		if (yirecv(sock, buffer, MAX_RCV_BUFFER, 0)<0)
		{
			goto SEND_FAILURE;
		}

		//printf(buffer);
		
		if (buffer[0]!='3' ||
			buffer[1]!='3' ||
				buffer[2]!='4')
		{
			err_code=xMakeErrorCode(buffer);
			goto SEND_FAILURE;
		}

		sprintf(x, "%s\r\n", id);
		if (yisend(sock, x, strlen(x), False)!=strlen(x))
		{
			goto SEND_FAILURE;
		}

		if (yirecv(sock, buffer, MAX_RCV_BUFFER, 0)<0)
		{
			goto SEND_FAILURE;
		}

		if (buffer[0]!='3' ||
			buffer[1]!='3' ||
				buffer[2]!='4')
		{
			err_code=xMakeErrorCode(buffer);
			goto SEND_FAILURE;
		}
		
		sprintf(x, "%s\r\n", pass);
		//printf(x);
		if (yisend(sock, x, strlen(x), False)!=strlen(x))
		{
			goto SEND_FAILURE;
		}

		if (yirecv(sock, buffer, MAX_RCV_BUFFER, 0)<0)
		{
			goto SEND_FAILURE;
		}

		//printf(buffer);
		if (buffer[0]!='2' ||
			buffer[1]!='3' ||
				buffer[2]!='5')
		{
			err_code=xMakeErrorCode(buffer);
			goto SEND_FAILURE;
		}
	}

	if (from)
	{
		char x[128];
		sprintf(x, "MAIL FROM: %s\r\n", from);
		if (yisend(sock, x, strlen(x), False)!=strlen(x))
		{
			goto SEND_FAILURE;
		}
		if (yirecv(sock, buffer, MAX_RCV_BUFFER, 0)<0)
		{
			err_code=xMakeErrorCode(buffer);
			goto SEND_FAILURE;
		}
		//printf(buffer);

		if (buffer[0]!='2' ||
			buffer[1]!='5' ||
				buffer[2]!='0')
		{
			goto SEND_FAILURE;
		}
	}	
	
	if (toz)
	{
		char x[128];
		int index=0;
		char* tox = (char*)toz;
		while(1)
		{
			int len = strlen(&tox[index]);
			sprintf(x, "RCPT TO: %s\r\n", &tox[index]);
			
			//printf(x);
			if (yisend(sock, x, strlen(x), False)!=strlen(x))
			{
				goto SEND_FAILURE;
			}
			if (yirecv(sock, buffer, MAX_RCV_BUFFER, 0)<0)
			{
				goto SEND_FAILURE;
			}
			if (buffer[0]!='2' ||
				buffer[1]!='5' ||
					buffer[2]!='0')
			{
				err_code=xMakeErrorCode(buffer);
				YIERROR("send mail error:%d\n", err_code);
				//goto SEND_FAILURE;
			}

			if (tox[len+1]==0)
			{
				break;
			}
			tox=&tox[len+1];
		}
	}

	// body.
	//
	//printf(BODY);
	if (yisend(sock, BODY, BODY_LEN, False)!=BODY_LEN)
	{
		goto SEND_FAILURE;
	}

	if (yirecv(sock, buffer, MAX_RCV_BUFFER, 0)<0)
	{
		goto SEND_FAILURE;
	}
	//printf(buffer);
	
	if (buffer[0]!='3' ||
		buffer[1]!='5' ||
			buffer[2]!='4')
	{
		err_code=xMakeErrorCode(buffer);
		goto SEND_FAILURE;
	}

	//printf(body);
	{
		int length = strlen(body)+256;
		char* pszBody = (char*)yialloc(length);
	
		//printf("trace.................1 (length:%d)\n", length);
		//printf("body:%s\n", body);
		sprintf(pszBody, "From: %s\r\nSubject: %s\r\nMIME-Version:1.0\r\nContent-type:text/plain; charset=iso-2022-jp\r\nContent-Transfer-Encoding: 7bit\r\n\r\n%s\r\n.\r\n", 
					from2?from2:from, title, body);
		//printf("trace.................2 (length:%d, pszBody:%d)\n", length, strlen(pszBody));	
		length=strlen(pszBody);
		//printf("trace.................3 (length:%d, pszBody:%d)\n", length, strlen(pszBody));	
		if (yisend(sock, pszBody, length, False)!=length)
		{
			goto SEND_FAILURE;
		}
			
		if (yirecv(sock, buffer, MAX_RCV_BUFFER, 0)<0)
		{
			goto SEND_FAILURE;
		}
		//printf(pszBody);

		//printf(buffer);

		if (buffer[0]!='2' ||
			buffer[1]!='5' ||
				buffer[2]!='0')
		{
			err_code=xMakeErrorCode(buffer);
			goto SEND_FAILURE;
		}

		
	}

	//printf(QUIT);
	if (yisend(sock, QUIT, QUIT_LEN, False)!=QUIT_LEN)
	{
		goto SEND_FAILURE;
	}

	if (yirecv(sock, buffer, MAX_RCV_BUFFER, 0)<0)
	{
		goto SEND_FAILURE;
	}
	yiclose(sock);

	//YIDEBUG("yiSmtpSend() OK\n");

	return 0;

SEND_FAILURE:
	yiclose(sock);	
	//YIDEBUG("yiSmtpSend() Failure! %s\n", toz);
	return err_code;
}



typedef struct {
	address_t	addr;
	int			port;
	Bool		useSsl;
	HandleList	hToList;
	HandleList  hCcList;
	HandleList  hBccList;
	HandleList  hFileList;
	char* from;
	char* from2;
	char* pszSubject;
	int   size;
	char* body;
	char* contentsType;
	char* charset;
	char* mimeEncode;
	char id[32];
	char pass[32];
	//void* p;
	//char* dataType;
	//char* filename;
	//char* dataencoding;
#ifdef _WIN32
	HWND hWnd;
	UINT msg;
#endif
} sSmtpDetail;

typedef struct {
	sFileBinaryFormat* file;
	char* dataType;
	char* dataencoding;
} sSmtpFileFormat;

// -------------------------------------------
HandleSmtp yiSmtpCreate(address_t addr, int port, const char* id, const char* pass, Bool bUseSsl)
{
	sSmtpDetail* s = (sSmtpDetail*)yialloci(sizeof(sSmtpDetail));

	if (s==0)
		return 0;

	s->addr=addr;
	s->port=port;
	s->useSsl=bUseSsl;
	strcpy(s->id, id);
	strcpy(s->pass, pass);

	return (HandleSmtp)s;
}

// -------------------------------------------
int yiSmtpAddTo(HandleSmtp hSmtp, const char* addr, const char* displayName, const char* charset)
{
	sSmtpDetail* s = (sSmtpDetail*)hSmtp;
	char buffer[128];

	if (s->hToList==0)
	{
		s->hToList = yiListCreate(0, 0);
	}

	if (displayName)
	{
		char* p = yiMimeString(displayName, charset, True);
		wsprintf(buffer, "%s%s%s <%s>", p?"":"\"", p?p:displayName, p?"":"\"", addr);
		yiDeleteMimeString(p);
	}
	else
	{
		wsprintf(buffer, "<%s>", addr);
	}

	return yiListAdd(s->hToList, (IntPtr)yistrheap(buffer));
}

// -------------------------------------------
int yiSmtpAddCc(HandleSmtp hSmtp, const char* addr, const char* displayName, const char* charset)
{
	sSmtpDetail* s = (sSmtpDetail*)hSmtp;
	char buffer[128];

	if (s->hCcList==0)
	{
		s->hCcList = yiListCreate(0, 0);
	}

	if (displayName)
	{
		char* p = yiMimeString(displayName, charset, True);
		wsprintf(buffer, "%s <%s>", p?p:displayName, addr);
		yifree(p);
	}
	else
	{
		wsprintf(buffer, "%s", addr);
	}

	return yiListAdd(s->hCcList, (IntPtr)yistrheap(buffer));
}

// -------------------------------------------
int yiSmtpAddBcc(HandleSmtp hSmtp, const char* addr, const char* displayName, const char* charset)
{
	sSmtpDetail* s = (sSmtpDetail*)hSmtp;
	char buffer[128];

	if (s->hBccList==0)
	{
		s->hBccList = yiListCreate(0, 0);
	}

	if (displayName)
	{
		char* p = yiMimeString(displayName, charset, True);
		wsprintf(buffer, "%s <%s>", p?p:displayName, addr);
		yifree(p);
	}
	else
	{
		wsprintf(buffer, "%s", addr);
	}

	return yiListAdd(s->hBccList, (IntPtr)yistrheap(buffer));
}

// -------------------------------------------
int yiSmtpSetFrom(HandleSmtp hSmtp, const char* addr, const char* displayName, const char* charset)
{
	sSmtpDetail* s = (sSmtpDetail*)hSmtp;
	char buffer[128];
	
	if (displayName)
	{
		char* p = yiMimeString(displayName, charset, True);
		wsprintf(buffer, "%s <%s>", p?p:displayName, addr);
		yifree(p);
	}
	else
	{
		wsprintf(buffer, "%s", addr);
	}
	s->from = yistrheap(buffer);
	return 0;
}

// -------------------------------------------
int yiSmtpSetReply(HandleSmtp hSmtp, const char* addr, const char* displayName, const char* charset)
{
	sSmtpDetail* s = (sSmtpDetail*)hSmtp;
	char buffer[128];
	
	if (displayName)
	{
		char* p = yiMimeString(displayName, charset, True);
		wsprintf(buffer, "%s <%s>", p?p:displayName, addr);
		yifree(p);
	}
	else
	{
		wsprintf(buffer, "%s", addr);
	}
	s->from2 = yistrheap(buffer);
	return 0;
}



// -------------------------------------------
void yiSmtpSetSubject(HandleSmtp hSmtp, const char* subject, const char* charset)
{
	sSmtpDetail* s = (sSmtpDetail*)hSmtp;
	char buffer[256];
	char size=0;
	char* pEn=0;

	s->pszSubject = yiMimeString(subject, charset, True);//yistrheap(buffer);
	
	if (s->pszSubject==0)
		s->pszSubject = yistrheap(subject);
}

// -------------------------------------------
int yiSmtpAddFile(HandleSmtp hSmtp, const void* data, int size, const char* name, const char* type, const char* mimeEncode)
{
	/*sSmtpDetail* s = (sSmtpDetail*)hSmtp;
	s->p=yitoheap(data, size);
	s->size=size;

	if (type==0)
		type="application/binary";

	s->dataType=yistrheap(type);
	s->filename=yistrheap(name);

	if (mimeEncode)
		s->dataencoding=yistrheap(mimeEncode);
	else
		s->dataencoding=yistrheap("base64");
*/
	return 0;
}

// -------------------------------------------
int yiSmtpAddFileEx(HandleSmtp hSmtp, sFileBinaryFormat* bin, const char* type, const char* mimeEncode)
{
	sSmtpDetail* s = (sSmtpDetail*)hSmtp;
	sSmtpFileFormat x={bin, yistrheap(type), yistrheap(mimeEncode)};
	if (s->hFileList==0)
	{
		s->hFileList = yiListCreate(0, 0);
	}

	yiListAdd(s->hFileList, (IntPtr)yitoheap(&x, sizeof(x)));

	return 0;
}


// -------------------------------------------
void yiSmtpSetBody(HandleSmtp hSmtp, const char* data, const char* contentsType, const char* charset, const char* mimeEncode)
{
	sSmtpDetail* s = (sSmtpDetail*)hSmtp;

	s->body=yistrheap(data);

	if (contentsType==0)
	{
		contentsType="text/plain";
	}
	s->contentsType=yistrheap(contentsType);

	if (charset==0)
	{
		charset=yiMimeGetDefaultLocaleCharset(0);
	}

	s->charset=yistrheap(charset);

	if (mimeEncode==0)
	{
		mimeEncode="7bit";
	}
	s->mimeEncode=yistrheap(mimeEncode);

}

#ifdef _WIN32
void _xSmtpSend(void* param)
#else
void* _xSmtpSend(void* param)
#endif
{
	sSmtpDetail* s=(sSmtpDetail*)param;

#ifdef _WIN32
	yiSmtpSendEx((HandleSmtp)param, False, s->hWnd, s->msg);
	_endthread();
#else
	yiSmtpSendEx((HandleSmtp)param, False);
#endif
}

// --------------------------------------
static char* xStrCat(HandleList hList, const char* pszSeparator)
{
	int length=0;
	syiNode* node;
	const int seperatorLength = pszSeparator?strlen(pszSeparator):0;
	char* pRet=0;

	for (node = yiListGetTopFrom(hList, 0); node; node=node->np)
	{
		char* p = (char*)node->ref;

		if (p==0)
			continue;

		length+=strlen(p);

		if (node->np!=0)
		{
			length+=seperatorLength;
		}
	}

	pRet = (char*)yialloc(length+1);
	pRet[length]=0;
	pRet[0]=0;

	for (node = yiListGetTopFrom(hList, 0); node; node=node->np)
	{
		char* p = (char*)node->ref;

		if (p==0)
			continue;

		strcat(pRet, p);

		if (pszSeparator!=0 &&
				node->np!=0)
		{
			strcat(pRet, pszSeparator);
		}
	}
	return pRet;
}

#include "yiencodex.h"

#ifdef _WIN32

int yiSmtpSendEx(HandleSmtp hSmtp, Bool bSendThread, HWND hNotifyWnd, UINT msg)
#else
// -------------------------------------------
int yiSmtpSendEx(HandleSmtp hSmtp, Bool bSendThread)
#endif
{
	sSmtpDetail* s = (sSmtpDetail*)hSmtp;
	sockssl_t sock=0;
	int retCode=0;
	char buffer[MAX_RCV_BUFFER+1];
	HandleList hBodyStr=0;
	char szBoundary[64];
	char* pszCc=0;
	char* pszBcc=0;
	char* pszTo=0;
	int send_cnt=0;
	HandleList hToList=0;

	szBoundary[0]=0;
	if (bSendThread)
	{
#ifdef _WIN32
		sSmtpDetail* s = (sSmtpDetail*)hSmtp;
		s->hWnd=hNotifyWnd;
		s->msg=msg;
		_beginthread(_xSmtpSend, 0, hSmtp);
#else
		// pthread?
#endif
	}

	buffer[MAX_RCV_BUFFER]=0;

	// ここから。。。
	if (s->useSsl==True)
	{
		if ((sock=yiconnect_ssl(s->addr, s->port, 1))==0)
		{
			retCode=1;
			goto SEND_FAILURE;
		}
	}
	else
	{
		int n=0;
		if ((n=yiconnect(s->addr, s->port, 1))<0)
		{
			retCode=1;
			goto SEND_FAILURE;
		}
#ifndef STATIC_COMPILE
		sock = (sockssl_t)yialloci(sizeof(*s));
		sock->sock=n;
#else
		sock=n;
#endif
	}

	if (yirecv_ssl(sock, buffer, MAX_RCV_BUFFER, 500)<4)
	{
		return 1;
	}

	if (buffer[0]!='2' || 
		buffer[1]!='2' ||
			buffer[2]!='0')
	{
		retCode=1;
		goto SEND_FAILURE;
	}

	if (s->id)
	{
		char x[64];
		char* pBase64;

		//printf(AUTH_LOGIN);
		if (yisend_ssl(sock, AUTH_LOGIN, AUTH_LOGIN_LEN)!=AUTH_LOGIN_LEN)
		{
			retCode=2;
			goto SEND_FAILURE;
		}

		if (yirecv_ssl(sock, buffer, MAX_RCV_BUFFER, 500)<0)
		{
			retCode=2;
			goto SEND_FAILURE;
		}

		//printf(buffer);
		
		if (buffer[0]!='3' ||
			buffer[1]!='3' ||
				buffer[2]!='4')
		{
			retCode=2;
			goto SEND_FAILURE;
		}

		pBase64=yiToBase64(s->id, strlen(s->id), 0);
		sprintf(x, "%s\r\n", pBase64);
		if (yisend_ssl(sock, x, strlen(x))!=strlen(x))
		{
			retCode=2;
			goto SEND_FAILURE;
		}

		if (yirecv_ssl(sock, buffer, MAX_RCV_BUFFER, 500)<0)
		{
			retCode=2;
			goto SEND_FAILURE;
		}

		if (buffer[0]!='3' ||
			buffer[1]!='3' ||
				buffer[2]!='4')
		{
			retCode=2;
			goto SEND_FAILURE;
		}
		
		yifree(pBase64);
		pBase64=yiToBase64(s->pass, strlen(s->pass), 0);
		sprintf(x, "%s\r\n", pBase64);
		//printf(x);
		if (yisend_ssl(sock, x, strlen(x))!=strlen(x))
		{
			retCode=3;
			goto SEND_FAILURE;
		}

		if (yirecv_ssl(sock, buffer, MAX_RCV_BUFFER, 500)<0)
		{
			retCode=3;
			goto SEND_FAILURE;
		}

		//printf(buffer);
		if (buffer[0]!='2' ||
			buffer[1]!='3' ||
				buffer[2]!='5')
		{
			retCode=3;
			goto SEND_FAILURE;
		}
	}

	YIDEBUG("yiSmtpSendEx..... login success!");

	// ここまでこれたらログイン成功。

	// From:
	if (s->from)
	{
		char x[128];
		sprintf(x, "MAIL FROM: %s\r\n", s->from);
		if (yisend_ssl(sock, x, strlen(x))!=strlen(x))
		{
			retCode=4;
			goto SEND_FAILURE;
		}
		if (yirecv_ssl(sock, buffer, MAX_RCV_BUFFER, INFINITE)<0)
		{
			retCode=4;
			goto SEND_FAILURE;
		}
		//printf(buffer);

		if (buffer[0]!='2' ||
			buffer[1]!='5' ||
				buffer[2]!='0')
		{
			retCode=4;
			goto SEND_FAILURE;
		}
	}

	hToList = s->hToList;

SEND:
	send_cnt++;
	if (hToList)
	{
		char x[128];
		int index=0;
		syiNode* node;

		for (node=yiListGetTopFrom(hToList, 0); node; node=node->np)
		{
			char* to = (char*)node->ref;
			int len = strlen(to);

			sprintf(x, "RCPT TO: %s\r\n", to);
			if (yisend_ssl(sock, x, strlen(x))!=strlen(x))
			{
				retCode=4;
				goto SEND_FAILURE;
			}
			if (yirecv_ssl(sock, buffer, MAX_RCV_BUFFER, INFINITE)<0)
			{
				retCode=4;
				goto SEND_FAILURE;
			}

			if (buffer[0]!='2' ||
				buffer[1]!='5' ||
					buffer[2]!='0')
			{
				retCode=4;
				goto SEND_FAILURE;
			}
		}
	}

	if (send_cnt<3)
	{
		if (send_cnt==1)
			hToList = s->hCcList;
		else if (send_cnt==2)
			hToList = s->hBccList;

		hBodyStr = yiListCreate(0, 0);

		goto SEND;
	}

	// ここからBody
	if (yisend_ssl(sock, BODY, BODY_LEN)!=BODY_LEN)
	{
		retCode=5;
		goto SEND_FAILURE;
	}

	if (yirecv_ssl(sock, buffer, MAX_RCV_BUFFER, INFINITE)<0)
	{
		retCode=5;
		goto SEND_FAILURE;
	}
	//printf(buffer);
	
	if (buffer[0]!='3' ||
		buffer[1]!='5' ||
			buffer[2]!='4')
	{
		retCode=5;
		goto SEND_FAILURE;
	}

	hBodyStr = yiListCreate(0, 0);

	if (s->hToList)
	{
		pszTo = xStrCat(s->hToList, ", ");//",\r\n\t");
		yimListAdd(hBodyStr, "To: ");
		yimListAdd(hBodyStr, pszTo);
		yimListAdd(hBodyStr, "\r\n");
	}

	if (s->hCcList)
	{
		pszCc = xStrCat(s->hCcList, ", ");
		yimListAdd(hBodyStr, "CC: ");
		yimListAdd(hBodyStr, pszCc);
		yimListAdd(hBodyStr, "\r\n");
	}

	if (s->hBccList)
	{
		pszBcc = xStrCat(s->hBccList, ", ");
		yimListAdd(hBodyStr, "BCC: ");
		yimListAdd(hBodyStr, pszBcc);
		yimListAdd(hBodyStr, "\r\n");
	}


	// 1. fromを追加
	if (s->from2)
	{
		yimListAdd(hBodyStr, "From: ");
		yimListAdd(hBodyStr, s->from2);
	}

	yimListAdd(hBodyStr, "\r\n");
	yimListAdd(hBodyStr, "Subject: ");
	yimListAdd(hBodyStr, s->pszSubject);
	yimListAdd(hBodyStr, "\r\n");
	yimListAdd(hBodyStr, "MIME-Version:1.0\r\n");

	// 2. もし添付ファイルがある場合は。。。
	if (s->hFileList!=0)
	{
		yimListAdd(hBodyStr, "Content-Type: multipart/mixed;\r\n\t");
		sprintf(szBoundary, "--=%x-%2x-%5x.%x", hBodyStr, s->from, s->body, s->hToList);
		yimListAdd(hBodyStr, "boundary=\"");
		yimListAdd(hBodyStr, szBoundary);
		yimListAdd(hBodyStr, "\"\r\n");
		//yimListAdd(hBodyStr, "\r\n");
		yimListAdd(hBodyStr, "Content-Language: ja");
		yimListAdd(hBodyStr, "\r\n");
		yimListAdd(hBodyStr, "\r\n");
		yimListAdd(hBodyStr, "\r\n");
		yimListAdd(hBodyStr, "--");
		yimListAdd(hBodyStr, szBoundary);

		yimListAdd(hBodyStr, "\r\n");
/*
		int cnt = yiListCount(s->hFileList);

		for (int i=0; i<cnt; i++)
		{
			syiNode* node = yiListGetTopFrom(s->hFileList, i);
			sSmtpFileFormat* format = (sSmtpFileFormat* )node->ref;

		}
*/
	}

	// contents
	yimListAdd(hBodyStr, "Content-type: ");
	yimListAdd(hBodyStr, s->contentsType);
	yimListAdd(hBodyStr, "; charset=");
	yimListAdd(hBodyStr, s->charset);
	yimListAdd(hBodyStr, "\r\n");

	if (s->hFileList==0)
	{
		yimListAdd(hBodyStr, "Content-Language: ja");
		yimListAdd(hBodyStr, "\r\n");
	}

	yimListAdd(hBodyStr, "Content-transfer-encoding: ");
	yimListAdd(hBodyStr, s->mimeEncode);
	yimListAdd(hBodyStr, "\r\n");	
	yimListAdd(hBodyStr, "\r\n");	
	yimListAdd(hBodyStr, s->body);
	//yimListAdd(hBodyStr, "\r\n");	
	//yimListAdd(hBodyStr, "\r\n");

	if (s->hFileList==0)
	{
		syiNode* node;
		int nTotalLength=0;
		char* pRet=0;
		int len=0;

		//yimListAdd(hBodyStr, "\r\n.\r\n");

		pRet = xStrCat(hBodyStr, 0);

		if (yisend_ssl(sock, pRet, (len=strlen(pRet)))!=len)
		{
			retCode=6;
			goto SEND_FAILURE;
		}

	}
	else
	{
		char* pBase64=0;
		char* pRet=0;
		int len=0;
		int i=0;
		int cnt = yiListCount(s->hFileList);

		for (i=0; i<cnt; i++)
		{
			syiNode* node = yiListGetTopFrom(s->hFileList, i);
			sSmtpFileFormat* sf = (sSmtpFileFormat* )node->ref;

			if (stricmp(sf->dataencoding, "base64")==0)
			{
				int n;
				pBase64 = yiToBase64(sf->file->data, sf->file->size, &n);

				if (pBase64==0)
				{
					//if (s->dataencoding)
					//	yifree(s->dataencoding);

					yistrheap("quoted-printable");
				}
			}

			yimListAdd(hBodyStr, "\r\n");	
			yimListAdd(hBodyStr, "\r\n");
			yimListAdd(hBodyStr, "--");
			yimListAdd(hBodyStr, szBoundary);
			yimListAdd(hBodyStr, "\r\n");
			yimListAdd(hBodyStr, "Content-type: ");
			yimListAdd(hBodyStr, sf->dataType);
			yimListAdd(hBodyStr, ";\r\n\tname=\"");
			yimListAdd(hBodyStr, sf->file->name);
			yimListAdd(hBodyStr, "\"\r\n");
			yimListAdd(hBodyStr, "Content-transfer-encoding: ");
			yimListAdd(hBodyStr, sf->dataencoding);
			yimListAdd(hBodyStr, "\r\n");
			yimListAdd(hBodyStr, "Content-Disposition: attachment;");
			yimListAdd(hBodyStr, "\r\n\tfilename=\"");
			yimListAdd(hBodyStr, sf->file->name);
			yimListAdd(hBodyStr, "\"\r\n");
			yimListAdd(hBodyStr, "\r\n");

			pRet = xStrCat(hBodyStr, 0);

			//yiFileWrite("c:\\yotsuya\\mail.txt", pRet, -1, strlen(pRet)); 

			//yifree(s->dataencoding);
			//yifree(s->dataType);
			if (yisend_ssl(sock, pRet, (len=strlen(pRet)))!=len)
			{
				retCode=6;
				goto SEND_FAILURE;
			}

			if (pBase64)
			{
				if (yisend_ssl(sock, pBase64, (len=strlen(pBase64)))!=len)
				{
					retCode=6;
					yifree(pBase64);
					goto SEND_FAILURE;
				}
				//yiFileWrite("c:\\yotsuya\\mail.txt", pBase64, -1, strlen(pBase64)); 
				yifree(pBase64);
			}
			else
			{
				if (yisend_ssl(sock, sf->file->data, sf->file->size)!=sf->file->size)
				{
					retCode=6;
					goto SEND_FAILURE;
				}
			}

			yiListClear(hBodyStr);
		}

		//DeleteFileA("c:\\yotsuya\\sent.mail");
		//yiFileWrite("c:\\yotsuya\\sent.mail", pRet, 0, strlen(pRet));

		/*if (yisend_ssl(sock, pRet, (len=strlen(pRet)))!=len)
		{
			retCode=6;
			goto SEND_FAILURE;
		}*/


		// 最後に。。。
		{
			char buffer[128];
			int len=0;
			int ret=0;

			//sprintf(buffer, "\r\n--%s--\r\n\r\n.\r\n", szBoundary);

			if (szBoundary[0]!=0)
			{
				sprintf(buffer, "\r\n--%s--\r\n", szBoundary);
				len = strlen(buffer);
				if ((ret=yisend_ssl(sock, buffer, len))!=len)
				{
					retCode=6;
					goto SEND_FAILURE;
				}
			}

			sprintf(buffer, "\r\n.\r\n");
			len = strlen(buffer);
			if ((ret=yisend_ssl(sock, buffer, len))!=len)
			{
				retCode=6;
				goto SEND_FAILURE;
			}
		}
	}

	if (yirecv_ssl(sock, buffer, MAX_RCV_BUFFER, INFINITE)<0)
	{
		retCode=6;
		goto SEND_FAILURE;
	}
	//printf(buffer);
	
	if (buffer[0]!='2' ||
		buffer[1]!='5' ||
			buffer[2]!='0')
	{
		retCode=6;
		goto SEND_FAILURE;
	}

	if (yisend_ssl(sock, QUIT, QUIT_LEN)!=QUIT_LEN)
	{
		goto SEND_FAILURE;
	}

	if (yirecv_ssl(sock, buffer, MAX_RCV_BUFFER, INFINITE)<0)
	{
		goto SEND_FAILURE;
	}

	if (pszCc)
		yifree(pszCc);
	if (pszBcc)
		yifree(pszBcc);

	yiclose_ssl(sock);
	YIDEBUG("yiSmtpSendEx..... success");
#ifdef _WIN32
	if (s->hWnd!=0)
	{
		PostMessage(s->hWnd, s->msg, 0, hSmtp);
	}
#endif
	return 0;

SEND_FAILURE:

#ifdef _WIN32
	if (s->hWnd!=0)
	{
		PostMessage(s->hWnd, s->msg, retCode, hSmtp);
	}
#endif
	if (pszCc)
		yifree(pszCc);
	if (pszBcc)
		yifree(pszBcc);

	yiclose_ssl(sock);

	YIDEBUG("yiSmtpSendEx..... failure retCode:%d", retCode);

	return retCode;
}

// -------------------------------------------
void yiSmtpDestroy(HandleSmtp hSmtp)
{
	sSmtpDetail* s = (sSmtpDetail*)hSmtp;

	if (s->charset)
		yifree(s->charset);

	if (s->body)
		yifree(s->body);
	if (s->charset)
		yifree(s->charset);
	if (s->contentsType)
		yifree(s->contentsType);
	/*if (s->dataencoding)
		yifree(s->dataencoding);
	if (s->dataType)
		yifree(s->dataType);
	if (s->filename)
		yifree(s->filename);*/
	if (s->from)
		yifree(s->from);
	if (s->id)
		yifree(s->id);
	if (s->pass)
		yifree(s->pass);
	if (s->mimeEncode)
		yifree(s->mimeEncode);
	if (s->pszSubject)
		yifree(s->pszSubject);

	if (s->hFileList)
	{
		int n = yiListCount(s->hFileList);
		int i;
		for (i=0; i<n; i++)
		{
			sSmtpFileFormat* p = (sSmtpFileFormat*)yiListRemoveEx(s->hFileList, 0);
			yifree(p->dataencoding);
			yifree(p->dataType);
			yifree(p);
		}
	}

	if (s->from2)
		yifree(s->from2);

	if (s->hToList)
	{
		int n = yiListCount(s->hToList);
		int i;
		for (i=0; i<n; i++)
			yifree((void*)yiListRemoveEx(s->hToList, 0));
	}
	if (s->hBccList)
	{
		int n = yiListCount(s->hBccList);
		int i;
		for (i=0; i<n; i++)
			yifree((void*)yiListRemoveEx(s->hBccList, 0));
	}
	if (s->hCcList)
	{
		int n = yiListCount(s->hCcList);
		int i;
		for (i=0; i<n; i++)
			yifree((void*)yiListRemoveEx(s->hCcList, 0));
	}

	yifree(hSmtp);
	return ;
}
