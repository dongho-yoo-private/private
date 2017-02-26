/**
* Copyright (c) 2015
*
* Authorized by dongho.yoo.
*/
#ifndef __IGX_POP3_H__
#define __IGX_POP3_H__

#include "../igx_socket.h"

#ifdef __cplusplus
#   define IGX_POP3_EXTERN extern "C"
#else
#   define IGX_POP3_EXTERN extern
#endif


// -------------------------------------------
IGX_POP3_EXTERN char* yiMimeString(const char* pszString, const char* charset, Bool bEncodeBase64);

// -------------------------------------------
// sjis`®ÉÏ·³êÜ·B
IGX_POP3_EXTERN Bool yiMimeGetNameAnd(const char* pszEmail, char* display, char* email);

// -------------------------------------------
IGX_POP3_EXTERN void yiDeleteMimeString(const char* pszString);


typedef Handle HandlePop3;

typedef struct {
	int  id;
	//int  index;
	int  size;
	char uuid[128];
}sMailList;

typedef struct {
	char*	from;
	strz*	to;
	strz*	cc;
	strz*	bcc;
	char*	subject;
	char*	body;
	//char*	filename;
	HandleList	hBinary;
	//int		size;
	char	uid[64];
	unsigned int day;
	unsigned int time;
	unsigned int err;
}sMailFormat;

typedef struct {
	char*  pszFileName;
	char*  pszContentsType;
	Binary bin;
} sMailAttatchedFile;

// --------------------------------------
IGX_POP3_EXTERN HandlePop3 yipop3Create(address_t addr, int port, const char* pszUserName, const char* pszPassword, Bool useSSL);

// --------------------------------------
IGX_POP3_EXTERN void yipop3Destroy(HandlePop3 hPop, Bool deleteAttachedFile, Bool bSendQuitCommand);

// --------------------------------------
IGX_POP3_EXTERN HandleList yipop3UniqIdListening(HandlePop3 hPop);

// --------------------------------------
IGX_POP3_EXTERN sMailFormat* yipop3Get(HandlePop3 hPop, int id, const char* pszSearchFilter, Bool bIsNotContain);

// --------------------------------------
IGX_POP3_EXTERN unsigned int yipop3GetLastError(HandlePop3 hPop);


// --------------------------------------
IGX_POP3_EXTERN void yipop3Release(sMailFormat* data);

// --------------------------------------
IGX_POP3_EXTERN Bool yiPop3Delete(HandlePop3 hPop, int index);

// --------------------------------------
IGX_POP3_EXTERN int yipop3Stat(HandlePop3 hPop, int* all);



#endif


