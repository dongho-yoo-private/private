#ifndef _yipop3_h__
#define _yipop3_h__

#include "yi.h"
#include "yistd.h"
#include "yisock.h"
#include "yiListx.h"

// --------------------------------------
EXTERN const char* yiMimeGetDefaultLocaleCharset(int locale);

// -------------------------------------------
EXTERN char* yiMimeString(const char* pszString, const char* charset, Bool bEncodeBase64);

// -------------------------------------------
// sjiså`éÆÇ…ïœä∑Ç≥ÇÍÇ‹Ç∑ÅB
EXTERN Bool yiMimeGetNameAnd(const char* pszEmail, char* display, char* email);

// -------------------------------------------
EXTERN void yiDeleteMimeString(const char* pszString);


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
EXTERN HandlePop3 yipop3Create(address_t addr, int port, const char* pszUserName, const char* pszPassword, Bool useSSL);

// --------------------------------------
EXTERN void yipop3Destroy(HandlePop3 hPop, Bool deleteAttachedFile, Bool bSendQuitCommand);

// --------------------------------------
EXTERN HandleList yipop3UniqIdListening(HandlePop3 hPop);

// --------------------------------------
EXTERN sMailFormat* yipop3Get(HandlePop3 hPop, int id, const char* pszSearchFilter, Bool bIsNotContain);

// --------------------------------------
EXTERN unsigned int yipop3GetLastError(HandlePop3 hPop);


// --------------------------------------
EXTERN void yipop3Release(sMailFormat* data);

// --------------------------------------
EXTERN Bool yiPop3Delete(HandlePop3 hPop, int index);

// --------------------------------------
EXTERN int yipop3Stat(HandlePop3 hPop, int* all);

#endif
