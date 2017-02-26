#ifndef __syc_proxy_user_h__
#define __syc_proxy_user_h__
#include "yi.h"
#include "yistd.h"

// ---------------------------------
typedef struct xsUserConnectionInfo{
	Bit32 id;
	cyiList* connects;
	cyiList* connecteds;
} sUserConnectionInfo;


// ---------------------------------
typedef struct xsCreditUserInfo {
	
	long  contrycode;
	Bit32 point;
	char  szCreditCardNo2[4];
	char  szEmailAddress[128];
	char  szCreditCardNo[32];
	char  szCreditCardPassword[4];
	char  szAddress[128];
	char  szTelphone[16];
} sCreditUserInfo;

// ---------------------------------
typedef struct xsGroupUserInfo {
	Bit32 id;
	Bit32 flag32; /*31: logined, 30:admin, 29:activate */
} sGroupUserInfo;

// ---------------------------------
#define MAX_USER 128

// ---------------------------------
typedef struct xsGroupInfo {
	int  sock;
	char szGroupName[32];
	Bit32 id;
	Bit32 password[8];
	Bit32 adminId;
	Bit32 flags32;
	Bit32 license;
	unsigned long sDate;
	unsigned long sLastCreditDate;
	unsigned long sNextCreditDate;
	sGroupUserInfo users[MAX_USER];
} sGroupInfo;

// ---------------------------------
typedef struct xsUserInfo {
	int   sock;
	Bit32 flag32; /*31:special, 30-24: license, 23-0: reserved, */
	Bit32 softwarelicense;
	Bit32 id;
	Bit32 groupId; /* zero is no group*/
	char  password[8];
	char  szName[16];
} sUserInfo;

// ---------------------------------
typedef struct xsAllUserInfo 
{
	sUserInfo info;
	sCreditUserInfo credit;
} sUserAllInfo;

// ---------------------------------
typedef struct xsUserCreditHistroy {
	Bit32 date;
	Bit32 point;
	char  szCreditCardNo[32];
	Bit32 contents;
	char  title[64];
}sUserCreditHistroy ;

#endif
