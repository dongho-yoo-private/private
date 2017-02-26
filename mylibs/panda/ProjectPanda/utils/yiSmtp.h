#ifndef __yiSMTP_H__
#define __yiSMTP_H__

#include "yi.h"
#include "yisock.h"
#include "yipop3.h"

#include "yifile.h"

#ifdef __cplusplus
#	ifdef EXTERN
#		undef EXTERN
#	endif
#	define EXTERN extern "C"
#else
#	ifdef EXTERN
#		undef EXTERN
#	endif
#	define EXTERN extern
#endif

// -------------------------------------------
EXTERN int yiSmtpSend(const char* pszServerName, int port, const char* id, const char* pass, const char* from, const char* from2, 
						const char* toz, const char* title, const char* body, 
							Bool bIsThread, unsigned int deleteFlag);



typedef Handle HandleSmtp;

// -------------------------------------------
EXTERN HandleSmtp yiSmtpCreate(address_t addr, int port, const char* id, const char* pass, Bool bUseSsl);

// -------------------------------------------
EXTERN int yiSmtpAddTo(HandleSmtp hSmtp, const char* addr, const char* displayName, const char* charset);

// -------------------------------------------
EXTERN int yiSmtpSetFrom(HandleSmtp hSmtp, const char* addr, const char* displayName, const char* charset);

// -------------------------------------------
EXTERN int yiSmtpSetReply(HandleSmtp hSmtp, const char* addr, const char* displayName, const char* charset);

// -------------------------------------------
EXTERN int yiSmtpAddCc(HandleSmtp hSmtp, const char* addr, const char* displayName, const char* charset);

// -------------------------------------------
EXTERN int yiSmtpAddBcc(HandleSmtp hSmtp, const char* addr, const char* displayName, const char* charset);

// -------------------------------------------
EXTERN void yiSmtpSetSubject(HandleSmtp hSmtp, const char* subject, const char* charset);

// -------------------------------------------
EXTERN int yiSmtpAddFile(HandleSmtp hSmtp, const void* data, int size, const char* name, const char* type, const char* mimeEncode);

// -------------------------------------------
EXTERN int yiSmtpAddFileEx(HandleSmtp hSmtp, sFileBinaryFormat* bin, const char* type, const char* mimeEncode);

// -------------------------------------------
EXTERN void yiSmtpSetBody(HandleSmtp hSmtp, const char* data, const char* contentsType, const char* charset, const char* mimeEncode);


#ifdef _WIN32
// retvarl: 
// 0: success,
// 1: connect error.
// 2: id error.
// 3: pass error.
// 4: header error.
// 5: body error.
// 6: send error.
// 7: other.
EXTERN int yiSmtpSendEx(HandleSmtp hSmtp, Bool bSendThread, HWND hNotifyWnd, UINT msg);
#else
// -------------------------------------------
EXTERN int yiSmtpSendEx(HandleSmtp hSmtp, Bool bSendThread);
#endif

// -------------------------------------------
EXTERN void yiSmtpDestroy(HandleSmtp hSmtp);

#endif //__yiSMTP_H__
