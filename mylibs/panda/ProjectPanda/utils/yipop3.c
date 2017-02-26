#include "yipop3.h"
#include "yitime.h"

#ifndef _WIN32
#define yistrTrim yistrTrimEx
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#endif

#ifdef STATIC_COMPILE
#	define yisend_ssl(a, b,c) yisend(a, b, c, False)
#	define yirecv_ssl(a, b, c, d) yirecv(a, b, c, d)
#	define yiconnect_ssl yiconnect
#	define yiclose_ssl yiclose
#	define sockssl_t int
#endif

typedef struct {
	sockssl_t sock;
	HandleList hMailList;
	unsigned int last_error;
} sPop3;

#define DBS 128

Bool xIsOk(const char* buffer)
{
	if (buffer[0]=='+')
	{
		return True;
	}
	
	return False;
}

#define xISOK(buffer) (buffer[0]=='+')


#include "yiencodex.h"



static char* stristr(const char* src, const char* strSearch)
{
	char* pIndex=(char*)src;
	int len = strlen(strSearch);
	char* pSearch=(char*)strSearch;
	char caseChar = 'a'-'A';
	int i;

	while(*pIndex)
	{
		Bool bIsSame=True;
		for (i=0; i<len; i++, pIndex++)
		{
			if ((pSearch[i]>='A' &&
					pSearch[i]<='z')!=0)
			{
				if (pSearch[i]==*pIndex)
				{
					continue;
				}

				if (pSearch[i]<'a')
				{
					if ((pSearch[i]+caseChar)==*pIndex)
					{
						continue;
					}
				}
				else
				{
					if ((pSearch[i]-caseChar)==*pIndex)
					{
						continue;
					}
				}

				bIsSame=False;
				pIndex++;
				break;
			}

			if (*pIndex!=pSearch[i])
			{
				bIsSame=False;
				pIndex++;

				break;
			}
		}

		if (bIsSame==True)
		{
			return pIndex-len;
		}
	}

	return 0;

}

// --------------------------------------
// 0: japanese
// 1: korean
// 2: chinese
// 3: english
const char* yiMimeGetDefaultLocaleCharset(int locale)
{
	switch(locale)
	{
	case 0:
		return "iso-2022-jp";
	default:
		return "utf-8";
	}

	return 0;
}


// -------------------------------------------
// case1. "xxxxx"
// case2. xxxxx
// case3. -?sjis?B?asldkas?-
// case4. -?sjis?Q?asldkas?-
char* yiMimeStringGet(char* dest, const char* pszMimeStr)
{
	char* pIndex=(char*)pszMimeStr;

	// case 3 or 4
	if (pIndex[0]=='=' && pIndex[1]=='?')
	{
		// case 1, 2
		char* pszDisplay = 0;
		int n = 0;
		const char* pCharset;
		const char* pEn;
		const char* pDisplay;

		pszDisplay = yistrBetween(pIndex, "=?", "?=", False);
		n = yistrReplaceChar(pszDisplay, '?', '\0');
		if (n==2)
		{
			char* ex=0;
			pCharset	= yistrzGet(pszDisplay, 0, 0);
			pEn			= yistrzGet(pszDisplay, 1, 0);
			pDisplay	= yistrzGet(pszDisplay, 2, 0);

			if (pEn[0]=='B')
			{
				int len=0;
				ex = (char*)yiToPlain(pDisplay);
				if (ex) {
					len = xBIN_SIZE(ex);
					ex[len]=0;
				}
			}

			if (strcmp(pCharset, "iso-2022-jp")==0)
			{
				yiToSjis(ex?ex:pDisplay);
			}
			else if (strcmp(pCharset, "utf-8")==0)
			{
				char* pSjis = yiUtf8ToMulti(dest, ex?ex:pDisplay, 0);

				if (ex)
					yiBinaryDelete(ex);

				if (dest==0)
					dest=pSjis;
				return dest;
			}
			else if (strcmp(pCharset, "euc-jp")==0)
			{
				yiToSjis(ex?ex:pDisplay);
			}

			
			if (dest)
				strcpy(dest, ex?ex:pDisplay);
			else
				dest = yistrheap(ex?ex:pDisplay);

		}
		else
		{
			if (dest)
				*dest=0;
			
			dest=0;

		}
		yifree(pszDisplay);	
		return dest;
	}
	else
	{
		pIndex=yistrTrimIndex(pIndex, " ");
		if (pIndex[0]=="\"")
		{
			if (dest)
				yistrchcpy(dest, &pIndex[1], "\"");
			else
				dest = yistrheapex(&pIndex[1], "\"");

		}
		else
		{
			if (dest)
				strcpy(dest, pIndex);
			else
				dest=yistrheap(pIndex);
		}
	}

	return dest;
}

// -------------------------------------------
char* yiMimeString(const char* pszString, const char* charset, Bool bEncodeBase64)
{
	int len;// = strlen(pszString);
	int len_charset;
	int sum=0;
	char* ret;
	char* p = pszString;
	Bool bIsAll7Bit=True;
	int cnt=0;

	while(p[cnt])
	{
		if ((p[cnt]&0x80)!=0)
		{
			bIsAll7Bit=False;
			break;
		}
		cnt++;
	}

	if (bIsAll7Bit==True)
	{
		return 0;
	}
	
	if (charset==0)
	{
		// Asciiコードのみなど。
		ret = yistrheap(pszString);
		return ret;
	}
	else if (*charset==0)
		charset = "sjis";

	if (bEncodeBase64)
		p = yiToBase64(pszString, strlen(pszString), &len);


	len = strlen(p);
	len_charset = strlen(charset);

	ret = yialloc(len+len_charset+4+2+1+16);

	sprintf(ret, "=?%s?%c?%s?=", charset, bEncodeBase64?'B':'Q', p);

	if (bEncodeBase64)
		yifree(p);

	return ret;
}

// -------------------------------------------
void yiDeleteMimeString(const char* pszString)
{
	yifree((void*)pszString);
}
// --------------------------------------
// case1. =?charset?B?string?= <email>
// case2. =?charser?Q?string?= <email>
// case3. "displayname" <email>
// case4. <email>
// case5. email
Bool yiMimeGetNameAnd(const char* pszEmail, char* display, char* email)
{
	char* pIndex=yistrTrimIndex(pszEmail, " \t\r\n");
	char* pIndexEnd=strstr(pszEmail, "\n");
	char* pEmailStart=strstr(pszEmail, "<");

	if (pEmailStart>pIndexEnd)
		pEmailStart=0;
	
	if (pIndex[0]=='=' && pIndex[1]=='?')
	{
		// case 1, 2
		char* pszDisplay = 0;
		int n = 0;
		const char* pCharset;
		const char* pEn;
		const char* pDisplay;

		if (display==0)
			goto SKIP_DISPLAY;

		pszDisplay = yistrBetween(pIndex, "=?", "?=", False);
		n = yistrReplaceChar(pszDisplay, '?', '\0');
		if (n==2)
		{
			char* ex=0;
			pCharset	= yistrzGet(pszDisplay, 0, 0);
			pEn			= yistrzGet(pszDisplay, 1, 0);
			pDisplay	= yistrzGet(pszDisplay, 2, 0);

			if (pEn[0]=='B')
			{
				int len=0;
				ex = (char*)yiToPlain(pDisplay);

				if (ex)
				{
					len = xBIN_SIZE(ex);
					ex[len]=0;
				}
			}

			if (strcmp(pCharset, "iso-2022-jp")==0)
			{
				yiToSjis(ex?ex:pDisplay);
				//yiJisToSjis(pDisplay);
				//strcpy(display, pDisplay);
			}
			else if (strcmp(pCharset, "utf-8")==0)
			{
				char* pSjis = yiUtf8ToMulti(0, ex?ex:pDisplay, 0);

				if (ex)
					yiBinaryDelete(ex);
				ex=yistrheap(pSjis);
				yifree(pSjis);
			}
			else if (strcmp(pCharset, "euc-jp")==0)
			{
				yiToSjis(ex?ex:pDisplay);
			}

			strcpy(display, ex?ex:pDisplay);
		}
		else
		{
			/*if (pEmailStart!=0 &&
					pszEmail!=pEmailStart)
			{
				yistrchcpy(display, pszEmail, '<');
			}
			else
			{
				*display=0;
			}*/
			*display=0;

		}
		yifree(pszDisplay);

SKIP_DISPLAY:
		if (email==0)
		{
			return True;
		}

		/*if (pEmailStart!=0)
		{
			yistrchcpy(email, ++pEmailStart, '>');
			return True;
		}*/

		{
			char* pDisplayEnd=strstr(pIndex, "?=");
			char* pEmail;

			
			if (pDisplayEnd==0)
			{
				// invalid format.
				*email=0;
				return False;
			}
			pDisplayEnd+=2;
			pEmail = yistrBetween(pDisplayEnd, "<", ">", False);
			
			if (pEmail==0)
			{
				*email=0;
				return False;
			}
			
			strcpy(email, pEmail);
		}
		return True;
	} // case 1 or 2.

	if (pIndex[0]=='\"')
	{ 
		// case 3.
		char* pszDisplay = yistrBetween(pIndex, "\"", "\"", False);
		char* pszEmailAddr = yistrBetween(pIndex, "<", ">", False);

		if (display)
		{
			if (pszDisplay==0)
				*display=0;
			else
				strcpy(display, pszDisplay);
		}

		if (email)
		{
			if (pszEmailAddr==0)
			{
				*email=0;
				return False;
			}
			strcpy(email, pszEmailAddr);
		}
		return True;
	} // case 3.

	if (display)
		*display=0;

	if (email==0)
		return True;

	if (pIndex[0]=='<')
	{
		char* pszEmailAddr = yistrBetween(pIndex, "<", ">", False);	
		
		if (pszEmailAddr==0)
		{
			*email=0;
			return False;
		}
		strcpy(email, pszEmailAddr);
	}

	if (pEmailStart!=0 &&
			pszEmail!=pEmailStart)
	{
		if (display)
			yistrchcpy(display, pszEmail, '<');

		if (email)
			yistrchcpy(email, ++pEmailStart, '>');

		return True;
	}
	else
	{
		yistrcpyUntil(email, pIndex, " \t\r\n\0");
	}

	return True;
}


#define SSL_DEFAULT_TIMEOUT 100

#define xSendCommand(a, b, c) _xSendCommand(a, b, c, DBS-1, SSL_DEFAULT_TIMEOUT)

// --------------------------------------
static int _xSendCommand(sockssl_t sock, char* buffer, char* ret, int size, int nTimeOut)
{
	int n = strlen(buffer);
	int nRetryCnt=3;
	if (yisend_ssl(sock, buffer, n)!=n)
	{
		//yiclose_ssl(sock);
		YIDEBUG("xSendCommand(%s) Send Error!", buffer);
		return 0;
	}

	YIDEBUG("xSendCommand(%s) success!", buffer);
	ret[0]=0;

	if (nTimeOut==0)
		nTimeOut = SSL_DEFAULT_TIMEOUT;
RETRY:
	n = yirecv_ssl(sock, ret, size, nTimeOut);

	if (n==0)
	{
		//yiclose_ssl(sock);
		if (nRetryCnt--)
		{
			nTimeOut = SSL_DEFAULT_TIMEOUT;
			Sleep(100);
			goto RETRY;
		}
		return 0;
	}

	if (xISOK(ret)==False)
	{
		YIDEBUG("_xSendCommand(%s)... n:%d, result:%s", buffer, n, ret);
		//yiclose_ssl(sock);
		return -1;
	}

	ret[n]=0;
	return n;
}

// --------------------------------------
HandlePop3 yipop3Create(address_t addr, int port, const char* pszUserName, const char* pszPassword, Bool useSSL)
{
	char buffer[64];
	char retb[DBS];
	int n=0;
	int ret=0;
	int recved=0;
	sockssl_t sock=0;
	sPop3* s=0;

	if (useSSL)
	{
		sock = yiconnect_ssl(addr, port, 3);
		if (sock==0)
			return 0;
	}
	else
	{
#ifndef STATIC_COMPILE
		sock = (sockssl_t)yialloci(sizeof(*sock));
		sock->sock = yiconnect(addr, port, 3);
		if (sock->sock<0)
			return 0;
#else
		sock=yiconnect(addr, port, 3);
		if (sock<0)
			return 0;
#endif

	}

		
	n = yirecv_ssl(sock, retb, DBS, 500);

	if (n==0)
	{
		yiclose_ssl(sock);
		return 0;
	}

	if (xIsOk(retb)==False)
	{
		yiclose_ssl(sock);
		return 0;
	}

	wsprintf(buffer, "USER %s\r\n", pszUserName);
	
	ret = xSendCommand(sock, buffer, retb);
	if (ret==-1 ||
			ret==0)
	{
		yiclose_ssl(sock);
		return 0;
	}

	wsprintf(buffer, "PASS %s\r\n", pszPassword);
	
	ret = _xSendCommand(sock, buffer, retb, DBS-1, 500);
	if (ret==-1 ||
			ret==0)
	{
		yiclose_ssl(sock);
		return 0;
	}

	s = yialloci(sizeof(*s));
	s->sock=sock;
	return (HandlePop3)s;
}

// --------------------------------------
void yipop3Destroy(HandlePop3 hPop, Bool bDeleteAttatchedFile, Bool bSendQuitCommand)
{
	char ret[DBS];
	int n=0;
	int size=0;
	sPop3* s = (sPop3*)hPop;


	if (s==0)
		return ;

	if (bSendQuitCommand==True)
	{
		if (xSendCommand((sockssl_t)s->sock, "QUIT\r\n", ret)==False)
		{
			
		}
	}

	if (s->hMailList)
	{
		syiNode* node;
		int i,j;
		int cnt = yiListCount(s->hMailList);
		int cnt2=0;
		for (i=0; i<cnt; i++)
		{
			yifree(yiListRemoveEx(s->hMailList, 0));
			//for (node=yiListGetTopFrom(s->hMailList, 0);node!=0; node=node->np)
			//{
			//	sMailList* mail = (sMailList*)node->ref;


			//	sMailFormat* mail = (sMailFormat*)node->ref;

			//	if (mail==0)
			//		continue;

			//	yistrzp_del(mail->bcc);
			//	yistrzp_del(mail->cc);
			//	yistrzp_del(mail->to);
			//	yifree(mail->from);
			//	yifree(mail->subject);


			//	cnt2=yiListCount(mail->hBinary);
			//	for (j=0; j<cnt2; j++)
			//	//for (node=yiListGetTopFrom(mail->hBinary, 0);node!=0; node=node->np)
			//	{
			//		sFileBinaryFormat* file = yiListRemoveEx(mail->hBinary, i);//(sFileBinaryFormat*)node->ref;

			//		if (bDeleteAttatchedFile==True)
			//		{
			//			yiBinaryDelete((Binary)file);
			//		}
			//	}

			//	yiListDestroy(mail->hBinary);

			//}

			//yifree(yiListRemoveEx(s->hMailList, 0));
		}

		yiListDestroy(s->hMailList);
	}

	yiclose_ssl((sockssl_t)s->sock);
	yifree(s);
}

// --------------------------------------
int yipop3Stat(HandlePop3 hPop, int* all)
{
	char ret[DBS];
	int n=0;
	int size=0;
	sPop3* s = (sPop3*)hPop;

	if (xSendCommand((sockssl_t)s->sock, "STAT\r\n", ret)==False)
	{
		return -1;
	}

	sscanf(&ret[4], "%d %d", &n, &size);

	if (all)
		*all=size;

	return n;
}

// --------------------------------------
Bool yiPop3Delete(HandlePop3 hPop, int index)
{
	char ret[DBS];
	sPop3* s = (sPop3*)hPop;

	char szCommand[32];
	wsprintf(szCommand, "DELE %d\r\n", index);
	if (xSendCommand((sockssl_t)s->sock, szCommand, ret)==False)
	{
		return False;
	}

	return True;
}


// --------------------------------------
static char* xNextLine(const char* p)
{
	char* pNext = strstr(p, "\r\n");
	if (pNext==0)
		return 0;
	pNext+=2;

	if (*pNext==0)
		return 0;

	return pNext;
}

// --------------------------------------
HandleList yipop3UniqIdListening(HandlePop3 hPop)
{
	char buffer[DBS];
	int nTotalReceived=0;
	syiNode* node=0;
	HandleList hMem = yiListCreate(0, 0);
	HandleList hMailList=0;
	int recved=0;
	int nReceivedSum=0;
	int cnt = yipop3Stat(hPop, 0);
	int i;
	char* idsBuffer = yialloc(cnt*64); // 1行につき64バイトだと仮定する。
	sPop3* s = (sPop3*)hPop;


	YIDEBUG("yipop3UniqIdListening() cnt:%d, mem:%d bytes.", cnt, cnt*64);

	if (cnt==0)
		return 0;

RETRY:
	if ((recved=_xSendCommand((sockssl_t)s->sock, "UIDL\r\n", idsBuffer, cnt*64, 500))<=0)
	{
		return 0;
	}

	YIDEBUG("yipop3UniqIdListening() %d bytes. received", recved);

	//yiFileWrite("c:\\yotsuya\\maillist.txt", idsBuffer, -1, recved);
	
	nTotalReceived=recved;

	// データを一個のブロックにする。
	nReceivedSum=recved;//strlen(idsBuffer);
	if (nTotalReceived)
	{
		char*p=idsBuffer;
		char*pIndex=xNextLine(p);
		
		hMailList = yiListCreate(0, 0);
			
		for (i=0; i<cnt; i++)
		{
			int no=0;
			char uuid[64];
			sMailList* sMail;
			int ret = 0;
			
			if (pIndex)
			{
				ret = sscanf(pIndex, "%d %s", &no, uuid);
			}

			YIDEBUG("yipop3UniqIdListening.... ret:%d, i:%d", ret, i);
			if (ret!=2)
			{
				// もう一度recceive;
				int nReceived = 0;
				int nRetryCnt = 5;

XRETRY:
				YIDEBUG("yipop3UniqIdListening.... Retry:%d", nRetryCnt);
				nReceived = yirecv_ssl((sockssl_t)s->sock, &idsBuffer[nReceivedSum], cnt*64, 100);

				if (nReceived<=0)
				{
					if (nRetryCnt)
					{
						nRetryCnt--;
						goto XRETRY;
					}
					
					if (idsBuffer)
						yifree(idsBuffer);

					yiListDestroy(hMailList);
					// TODO.

					s->last_error=-2;
					return 0;
				}
				nReceivedSum+=nReceived;
				i--;

				if (pIndex==0)
					pIndex=xNextLine(p);

				//pIndex=xNextLine(pIndex);
				continue;
			}


			YIDEBUG("yipop3UniqIdListening.... trace........1");

			sMail = (sMailList*)yialloci(sizeof(*sMail));
			YIDEBUG("yipop3UniqIdListening.... trace........2");
			sMail->id=no;
			YIDEBUG("yipop3UniqIdListening.... trace........3");
			strcpy(sMail->uuid, uuid);
			YIDEBUG("yipop3UniqIdListening.... trace........4");
			yiListAdd(hMailList, (IntPtr)sMail);

			YIDEBUG("yipop3UniqIdListening.... trace........5");
			if (yiListCount(hMailList)==cnt)
				break;

			YIDEBUG("yipop3UniqIdListening.... trace........7");
			{
				char* pSearch = xNextLine(pIndex);
			YIDEBUG("yipop3UniqIdListening.... trace........8");

				if (pSearch==0)
					goto XRETRY;
			YIDEBUG("yipop3UniqIdListening.... trace........9");
				pIndex=pSearch;
			YIDEBUG("yipop3UniqIdListening.... trace........10");
			}
		} // for

		//while(pIndex)
		//{


		//	s = (sMailList*)yialloci(sizeof(*s));
		//	s->id=no;
		//	strcpy(s->uuid, uuid);
		//	yiListAdd(hMailList, (IntPtr)s);

		//	pIndex=xNextLine(pIndex);
		//}

		s->hMailList=hMailList;

		if (idsBuffer)
			yifree(idsBuffer);
		return hMailList;
	}

	if (idsBuffer)
		yifree(idsBuffer);
	return 0;

}

// --------------------------------------
static char* xGetOneLine(const char* p)
{
	char* pEnd=p;
	int len=0;

SEARCH_END:
	while(*pEnd!=0 && *pEnd!='\r'&&*pEnd!='\n') pEnd++;

	if (*pEnd=='\r')
	{
		if (pEnd[2]=='\t') // 続きがある。
		{
			pEnd+=2;
			goto SEARCH_END;
		}
	}
	else if (*pEnd=='\n')
	{
		if (pEnd[2]=='\t') // 続きがある。
		{
			pEnd+=2;
			goto SEARCH_END;
		}
	}
	
	len = pEnd-p;
	pEnd = yitoheap(p, len+1);
	pEnd[len]=0;
	return pEnd;
}

// --------------------------------------
static strz* xGetMailList(const char* p)
{
	char szDisplay[32];
	char szEmail[32];
	char* pOneLine;
	char* pEmail;
	char* pTemp;
	strz* ret;
	strz  list;
	int   cnt, i;
	
	// case1. To: xxxx@xxx.com, =?sjis?B?.......?= <yyyy@yyyy.com>, ....
	// case2. To: xxxx@xxx.com,\r\n\txxxx@xxxx.com, ....
	//p+=3;

	// まず、メールアドレスの後ろまでを探し出す。
	pOneLine=xGetOneLine(p);
	if (strstr(pOneLine, "@")==0)
	{
		yifree(pOneLine);
		return 0;
		//char* pNextLine = strstr(p, "\n");
		//char* pNextOneLine = xGetOneLine(++pNextLine);
		//int len = strlen(pOneLine)+strlen(pNextOneLine);
		//char* pTemp = (char*)yialloc(len+32);
		//strcpy(pTemp, pOneLine);
		//strcat(pTemp, pNextOneLine);
		//yifree(pOneLine);
		////yifree(pNextOneLine);
		//pOneLine=pTemp;
	}
	pEmail=yistrReplaceStr(pOneLine, "\r\n\t", "");



	if (pEmail)
		yifree(pOneLine);
	else
		pEmail=pOneLine;

	{
		char* temp = yistrTrim(0, pEmail, " \"");
		yifree(pEmail);
		pEmail=temp;
	}

	// ここで全てケース1の状態に
	list=yistrSplitEx(pEmail, ",");

	yifree(pEmail);

	cnt = yistrzCount(list);
	ret = yistrzp(cnt);

	for (i=0; i<cnt; i++)
	{
		yiMimeGetNameAnd(yistrzGet(list, i, 0), szDisplay, szEmail);	
		yistrzpSet(ret, i, yistrz(0, szDisplay, szEmail, 0));
	}
	
	yifree(list);
	return ret;
}

const char** __monthes[] = {
	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec",
	0
};

// --------------------------------------
//  Sun, 24 Oct 2010 11:46:03 +0900 
//#include "yiTime.h"
static unsigned int xGetDayTime(unsigned int* time, const char* pszDate)
{
	int y;
	int m;
	int d;
	int hh;
	int mm;
	int ss;
	char dd[8];
	int xx;
	char szM[8];
	int i;
	char* pIndex=0;

	if (pszDate[5]==' ')
	{
		memcpy(dd, &pszDate[6], 3);
		pIndex=&pszDate[10];
	}
	else
	{
		memcpy(dd, &pszDate[5], 3);
		pIndex=&pszDate[9];
	}
	dd[3]=0;

	if (sscanf(pIndex, "%d %s %d %d:%d:%d %d", &d, szM, &y, &hh, &mm, &ss, &xx)!=7)
	{
		if (time)
			*time=0;
		return 0;
	}

	for (i=0; i<12; i++)
	{
		if (stricmp(__monthes, szM)==0)
		{
			mm=i+1;
			break;
		}
	}

	// 差分を引く
	{
		int dh = xx/100;
		int dm = xx%100;
		
		hh+=dh;

		if (hh<0)
		{
			hh=12+hh;
		}
		else if (hh>23)
		{
			hh-=23;
		}
	}

	if (time)
		*time=MAKETIME(0, 0, 0, hh, mm, ss);

	return MAKEDAY(y, m, d);

}

static Bool xIsLeapYear(int y)
{
	Bool bIsReapYear=False;

	if (y%4==0)
	{
		bIsReapYear=True;
		if (y%100==0)
		{
			bIsReapYear=False;
			if (y%400==0)
			{
				bIsReapYear=True;
			}
		}
	}

	return bIsReapYear;
}

static int xGetSumDate(int y,int  m,int  d)
{
	int mon[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int mon_y[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int* pMon=mon;
	int sum=0;
	int i=0;
	Bool bIsReapYear=xIsLeapYear(y);

	if (bIsReapYear)
		pMon=mon_y;

	sum=365*(y-1);
	sum+=(y-1)/4;
	sum-=(y-1)/100;
	sum-=(y-1)/400;

	for (i=0; i<m-1; i++)
	{
		sum+=pMon[i];
	}

	sum+=d;

	return sum;

}



static int xGetDateFromSum(int sum, int* y, int* m, int* d)
{
	int mon[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int mon_y[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int* pMon=mon;
	int xSum=0;
	
	*y=1;//(sum/366)-1;
	*m=0;
	*d=0;


	while(1)
	{
		int i=0;
		int* pMon = xIsLeapYear(*y)?mon_y:mon;

		for (i=0; i<12; i++)
		{
			int j=0;
			xSum+=pMon[i];
			for (j=0; j<pMon[i]; j++)
			{
				if (xSum==sum)
				{
					*m=(i+1);
					*d=j;
					return ;
				}
			}
		}
		(*y)++;
	}
}

// 27 Feb 2012 18:51:13 
static unsigned int xGetDate(char* pIndex, unsigned int* _time, int locale)
{
	int y=0, m=0, d=0;
	int hh=0, mm=0, ss=0;
	char* pTop=pIndex;
	int nFactor=0;

	if (locale==0)
	{
		char szMon[16];
		char* pDate;
		int i;

		pIndex = strstr(pTop, "+0900");

		if (pIndex==0)
		{
			pIndex=strstr(pTop, "-0000");
			nFactor=9;
			if (pIndex==0)
			{
				pIndex=strstr(pTop, "+0000");
				nFactor=9;

				if (pIndex==0)
				{
					nFactor=0;
					pIndex=strstr(pTop, "Date:");

					if (pIndex==0)
						goto GET_DATE;
				}
			}

		}

		if (*pIndex=='D')
		{
			pDate=pIndex+5;
		}
		else
			pDate = pIndex-21;

		if (pDate<pTop)
		{
			goto GET_DATE;
		}

		if (sscanf(pDate, "%d %s %d %d:%d:%d", &d, szMon, &y, &hh, &mm, &ss)!=6)
		{
			goto GET_DATE;
		}

		for (i=0; i<12; i++)
		{
			if (stricmp(__monthes[i], szMon)==0)
			{
				m=i+1;
				break;
			}
		}

		if (nFactor!=0)
		{
			// ここで時間計算をしてみよう。
			if ((hh+nFactor)>23)
			{
				int sumday = xGetSumDate(y, m, d);
				xGetDateFromSum(sumday+1, &y, &m, &d);
			}
		}
		goto SUCCESS;
	}
GET_DATE:
#ifdef _WIN32
	{
		SYSTEMTIME curr;
		GetLocalTime(&curr);

		y=curr.wYear;
		m=curr.wMonth;
		d=curr.wDay;
		hh=curr.wHour;
		mm=curr.wMinute;
		ss=curr.wSecond;
	}
#else
	{
		time_t t=time(0);
		struct tm* curr = localtime(&t);
		hh=curr->tm_hour;
		mm=curr->tm_min;
		ss=curr->tm_sec;
		y=curr->tm_year;
		m=curr->tm_mon;
		d=curr->tm_mday;
		
	}
#endif

SUCCESS:
	*_time= MAKETIME(0, 0, 0, hh, mm, ss);
	return MAKEDAY(y, m, d);
}

#define MAIL_END "\r\n.\r\n"
// --------------------------------------
static sMailFormat* xParseMail(const char* p, const char* pszSearchFilter, Bool bIsNotContain)
{
	char* pFrom, *pTo, *pSubject, *pCc, *pBcc;
	unsigned int time=0;
	unsigned int day;// = xGetDate(p, &time, 0);//strstr(p, "Date:");//stristr(p, "Date:");
	char* pBoundary;// = strstr(p, "boundary=");
	char* pEof;
	char  szBoundary[128];
	char  szFileName[128];
	int   len=0;
	char* pBodyIndex=0;
	sMailFormat* pRet=0;
	char locale[16];
	Bool bIsBase64=True;

	char* xxxxxxx=0;
	
	if (pszSearchFilter!=0)
	{
		xxxxxxx=strstr(p, pszSearchFilter);
		if (bIsNotContain==True)
		{
			if (xxxxxxx!=0)
				return 0;
		}
		else
		{
			if (xxxxxxx==0)
				return 0;
		}
	}


	pFrom = strstr(p, "\nFrom:");
	pTo	= strstr(p, "\nTo:");
	pSubject = strstr(p, "\nSubject:");
	pCc		= strstr(p, "\nCC:");
	pBcc		= strstr(p, "\nBCC:");
	day = xGetDate(p, &time, 0);//strstr(p, "Date:");//stristr(p, "Date:");

	pBodyIndex=stristr(p, "Content-Type");

	if (pBodyIndex==0)
		return 0;
	{
		char* x = stristr(p, "Content-Language:");
		if (x)
		{
			x = yistrTrimIndex(x, " ");
			yistrcpyUntil(locale, x, "\r\n");
		}
	}


	pRet=(sMailFormat*)yialloci(sizeof(sMailFormat));

	//if (pDate)
	//{
	pRet->day=day;
	pRet->time=time;//xGetDayTime(&pRet->time, pDate);
	//}

	if (pFrom)
	{
		char szDisplay[128];
		char szEmail[128];

		pFrom+=6;
		pFrom=yistrTrimIndex(pFrom, " ");
		yiMimeGetNameAnd(pFrom, szDisplay, szEmail);

		if (szDisplay[0]==0)
		{
			pRet->from = yistrz(0, szEmail, 0);//yistrheap(szEmail);
		}
		else
		{
			pRet->from=yistrz(0, szDisplay, szEmail, 0);
		}
	}

	if (pTo)
	{
		pTo+=4;
		pRet->to = xGetMailList(pTo);
	}

	if (pCc)
	{
		pCc+=4;
		pRet->cc = xGetMailList(pCc);
	}

	if (pBcc)
	{
		pBcc+=5;
		pRet->bcc = xGetMailList(pBcc);
	}

	if (pSubject)
	{
		pSubject+=10;
		pRet->subject=yiMimeStringGet(0, pSubject);
	}

	pBoundary = strstr(p, "boundary=");
	pEof=0;
	len=0;
		
	if (pBoundary==0)
	{
		char value[32];
		char charset[16];
		char* pContents = stristr(p, "Content-Type:");
		char* pChar;
		pContents+=strlen("Content-Type:");
		pContents=yistrTrimIndex(pContents, " ");
		yistrcpyUntil(value, pContents, "\r\n");

		pChar=strstr(pContents, "charset=");
		pChar+=strlen("charset=");
		yistrcpyUntil(charset, pChar, "\r\n");
		pChar=yistrTrim(charset, charset, "\"");
		
		pBodyIndex = strstr(pBodyIndex, "\r\n\r\n");
		
		if (strcmp(pChar, "utp-8")==0)
		{
			pRet->body=yiUtf8ToMulti(0, pBodyIndex, 0);
		}
		else
		{
			pRet->body=yistrheap(pBodyIndex);
			yiToSjis(pRet->body);
		}

		return pRet;
	}

	pBoundary+=strlen("boundary=");
	if (pBoundary[0]=='\"')
	{
		pBoundary++;
		yistrchcpy(szBoundary, pBoundary, '\"');
	}
	else
	{
		yistrchcpy(szBoundary, pBoundary, '\r');
	}

	len=strlen(szBoundary);
	pBoundary+=len;

	pBoundary=strstr(pBoundary, szBoundary);

	if (pBoundary==0)
	{
		yifree(pRet);
		return 0;
	}

	pBoundary+=len;
	pBodyIndex=pBoundary;

	// 次のboundaryが実データ
	pBoundary=strstr(pBoundary, szBoundary);

	if (pBoundary==0)
	{
		yifree(pRet);
		return 0;
	}

	// ここが本分。
	{
		char value[32];
		char charset[16];
		char* pContents = stristr(pBodyIndex, "Content-type:");
		char* pChar;
		int bodylen=0;

		pContents+=strlen("Content-type:");
		pContents=yistrTrimIndex(pContents, " ");
		yistrcpyUntil(value, pContents, "\r\n");

		pChar=strstr(pContents, "charset=");
		pChar+=strlen("charset=");
		yistrcpyUntil(charset, pChar, "\r\n");
		pChar=yistrTrim(charset, charset, "\"");
		
		pBodyIndex = strstr(pBodyIndex, "\r\n\r\n");
		pBodyIndex+=4;
		
		bodylen = pBoundary-pBodyIndex-4;
		pBodyIndex[bodylen]=0;

		if (strcmp(pChar, "utf-8")==0)
		{
			pRet->body=yiUtf8ToMulti(0, pBodyIndex, 0);
		}
		else
		{
			pRet->body=yistrheap(pBodyIndex);
			yiToSjis(pRet->body);
		}
	}// 本文

	//if (pBoundary==0)
	//{
	//	yifree(pRet);
	//	return 0;
	//}

	//pBoundary+=len;

	while(pBoundary)
	{
		char* pFileName = 0;//stristr(pBoundary, "filename=");
		char* pszContensType =0;

		pBoundary+=len;

		pszContensType = stristr(pBoundary, "Content-Type:");
		pFileName = stristr(pBoundary, "filename=");
		pEof = strstr(pBoundary, szBoundary);

		if (pEof<pFileName ||
				pEof<pszContensType)
		{
			pBoundary=pEof;
			continue;
		}

		if (pszContensType==0)
		{
			break;
		}

		if (pFileName==0)
		{
			pFileName=stristr(pBoundary, "name=");
			if (pFileName==0)
			{
				break;
			}
		}
		
		{
			char* encoding = stristr(pBoundary, "Content-transfer-encoding:");
			
			if (encoding==0)
			{
				bIsBase64=False;
			}
			else
			{
				char value[32];
				char* p;
				

				encoding+=strlen("Content-transfer-encoding:");
				p = yistrcpyUntil(value, encoding, "\r\n"); 
				p = yistrTrimIndex(p, " ");
				if (strcmp(p, "base64")==0)
				{
					bIsBase64=True;
				}
				else
				{
					bIsBase64=False;
				}
			}
		} // encoding

		pBoundary = strstr(pBoundary, "\r\n\r\n");
		pBoundary+=4;



		if (pEof==0)
		{
			return pRet;
		}

		if (pRet->hBinary==0)
		{
			pRet->hBinary = yiListCreate(False, 0);
		}

		{
			Binary bin=0;
			sMailAttatchedFile* file=(sMailAttatchedFile*)yialloci(sizeof(sMailAttatchedFile));

			if (pFileName)
			{
				char*p=0;
				pFileName+=strlen("filename=");

				p = yistrcpyUntil(0, pFileName, "\r\n");

				if (p)
				{
					file->pszFileName=yistrTrim(0, p, "\" ");

					if (file->pszFileName==0)
					{
						return 0;
					}

					yifree(p);
				}
			}

			if (pszContensType)
			{
				char*p=0;
				pszContensType+=strlen("Content-Type:");

				p = yistrcpyUntil(0, pszContensType, "\r\n");

				if (p)
				{
					file->pszContentsType=yistrTrim(0, p, "\" ");
					yifree(p);
				}
			}

			if (bIsBase64==False)
			{
				//pRet->size = xBIN_SIZE(pRet->binary);
				bin = yiBinary(pBoundary, pEof-pBoundary-4, 0);
			}
			else
			{
				int n = pEof-pBoundary-4;
				char* px = yitoheap(pBoundary, n+1);
				px[n]=0;
				bin = yiToPlainForced(px);
			}

			file->bin=bin;
			//file->pszContentsType=;
			//file->pszFileName=;

			yiListAdd(pRet->hBinary, file);
		} // attached file

		pBoundary = strstr(pBoundary, szBoundary);
	} // while.

	return pRet;
}

// --------------------------------------
unsigned int yipop3GetLastError(HandlePop3 hPop)
{
	sPop3* s = (sPop3*)hPop;
	if (s)
		return s->last_error;
	return 0;

}

#define MAX_RETRY_COUNT 100
// --------------------------------------
sMailFormat* yipop3Get(HandlePop3 hPop, int id, const char* pszSearchFilter, Bool bIsNotContain)
{
	char cmd[64];
	char buffer[DBS];
	char* pIndex=buffer;
	char* pBinary=0;
	int n=0, size=0;
	unsigned int next=0;
	sPop3* s = (sPop3*)hPop;
	int fileSize=0;
	int nRetryCount=MAX_RETRY_COUNT;

	s->last_error=0;

	//syiNode* node = yiListGetTopFrom(s->hMailList, index);
	//sMailList* pMailList = 0;
	//
	//if (node==0)
	//	return 0;

	//pMailList = (sMailList*)node->ref;

	//sprintf(cmd, "RETR %d\r\n", index);
	sprintf(cmd, "LIST %d\r\n", id/*pMailList->id*//*index*/);
	if (xSendCommand((sockssl_t)s->sock, cmd, buffer)==False)
	{
		YIDEBUG("yipop3Get(%d) LIST xSendCommand=False [%s]", buffer);
		s->last_error=-1;
		return 0;
	}


	//pIndex=xNextLine(pIndex);
	pIndex+=4;

	if (sscanf(pIndex, "%d %d", &n, &size)!=2)
	{
		YIDEBUG("yipop3Get(%d) xSendCommand=False [%s] ret=0", pIndex);
		s->last_error=-2;
		return 0;
	}

	//YIDEBUG("get file size is %d (%s)", size, pIndex);

	sprintf(cmd, "RETR %d\r\n", id/*pMailList->id*/);//index);
	n = strlen(cmd);
	if (yisend_ssl((sockssl_t)s->sock, cmd, n)!=n)
	{
		//yiclose_ssl((sockssl_t)hPop);
		YIDEBUG("yipop3Get(%d) RETR command False ret=0");
		s->last_error=-3;
		return 0;
	}

	//size+=size*1.5;
	fileSize=size;
	size=size+(size>>1);
	pBinary = yialloc(size);//+4096);
RETRY:
	if ((n=yirecv_ssl((sockssl_t)s->sock, pBinary+next, size, SSL_DEFAULT_TIMEOUT))>0)
	{
		if (nRetryCount==MAX_RETRY_COUNT)
		{
			if (pBinary[0]!='+')
			{
				pBinary[32]=0;
				yifree(pBinary);
				//YIDEBUG("yipop3Get() recev failure! index:%d, size:%d %s.... ", index, fileSize, pBinary);
				return 0;
			}
			// ここでサイズを再調整する。
			{
				char temp[128];
				char* pEnd;
				int len =0;
				int before=fileSize;
				yistrchcpy(temp, pBinary, '\n');
				len=strlen(temp);
				pEnd=strstr(&temp[4], " ");
				*pEnd=0;
	
				fileSize = atoi(&temp[4]);
				if (fileSize)
				{
					if (fileSize>before)
					{
						//YIDEBUG("file size is larger.... %d->%d", before, fileSize);
						pBinary = yirealloc(pBinary, fileSize+128);
						//pBinary = yialloc(size+128);
					}
				}
				else
				{
					fileSize=before;
				}
				//n-=(len+1);
			}
		}



		//{
		//	char xxx[32];
		//	strncpy(xxx, pBinary, 32);
		//	xxx[31]=0;
		//	YIDEBUG("xxxxxxxxxxxxxxxxxx:%s", xxx);
		//}

//#ifdef _DEBUG
//		{
//			char buffer[128];
//			sprintf(buffer, "c:\\yotsuya\\%d.mail", index);
//			//DeleteFile(buffer);
//			yiFileWriteOnce(buffer, pIndex, 0, n);
//		}
//#endif

		next+=n;
		if (next<fileSize)
		{
			//YIWARN("yipop3Get() recv_ssl %d<%d.... retry", next, fileSize);
			if (nRetryCount--)
				goto RETRY;
			YIWARN("yipop3Get() recv_ssl Retry count is zero");
			s->last_error=-4;
			goto EXIT;
		}

		if (memcmp(&pBinary[next-5], MAIL_END, 5)!=0)
		//if (pBinary[next-1]!='\n' || pBinary[next-2]!='\n' ||
		//		pBinary[next-3]!='.' || pBinary[next-4]!='\n' || pBinary[next-5]!='\r')
		{
			if (nRetryCount--)
				goto RETRY;
			YIWARN("yipop3Get() recv_ssl Retry count is zero");
			s->last_error=-5;

			goto EXIT;
		}

SUCCESS:
		pIndex=xNextLine(pBinary);

		/*if ((*pIndex)==0 || n<64)
		{
			// 受信しきれていない。
			YIWARN("yipop3Get() recv_ssl too small packet:%d", n);
			next+=n;
			goto RETRY;
		}*/

		pBinary[next]=0;
		YIDEBUG("get success recevied (%d/%d) retry cnt: %d", next, fileSize, MAX_RETRY_COUNT-nRetryCount);
		//yiFileWrite("c:\\yotsuya\\mail.txt", pBinary, -1, next);
		if (pIndex)
		{
			sPop3* x = (sPop3*)hPop;
			sMailFormat* mail = xParseMail(pIndex, pszSearchFilter, bIsNotContain);

			if (mail==0)
			{
				s->last_error=1;
				return 0;
			}

			//strcpy(mail->uid, pMailList->uuid);
			/*if (x->hMailList)
			{
				syiNode* node = yiListGetTopFrom(x->hMailList, index+1);
				if (node)
				{
					sMailList* xx = (sMailList*)node->ref;
					strcpy(mail->uid, xx->uuid);
				}
			}*/
			s->last_error=0;
			return mail;
		}
	}
	else
	{
		if (next>fileSize)
		{
			goto SUCCESS;
		}

		if (n==0 ||
				n==-5)
		{
			// time out?
			if (nRetryCount--)
			{
				Sleep(10);
				goto RETRY;
			}
		}
	}
EXIT:
	YIDEBUG("receive error : %d, nRetryCnt:%d", n, nRetryCount);
	yifree(pBinary);
	//YIDEBUG("get success");

	return 0;
}

// --------------------------------------
void yipop3Release(sMailFormat* data)
{
	yifree(data->body); data->body=0;
	yistrzp_del(data->to); data->to=0;
	yistrzp_del(data->cc); data->cc=0;
	yistrzp_del(data->bcc); data->bcc=0;
	//yifree(data->filename);
	yifree(data->from); data->from=0;
	yifree(data->subject); data->subject=0;

	if (data->hBinary)
	{
		int i=0;
		int cnt = yiListCount(data->hBinary);
		for (i=0; i<cnt; i++)
		{
			sMailAttatchedFile* file=(sMailAttatchedFile*)yiListRemoveEx(data->hBinary, 0);
			if (file)
			{
				if (file->pszContentsType)
					yifree(file->pszContentsType);
				if (file->pszFileName)
					yifree(file->pszFileName);
				if (file->bin)
					yiBinaryDelete(file->bin);
				yifree(file);
			}
		}
		yiListDestroy(data->hBinary);
		data->hBinary=0;
	}

	yifree(data);
	
	// 最後のDestroyで。。。
	//yifree(data);
}
