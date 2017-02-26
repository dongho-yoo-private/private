/**
* Copyright (c) 2015 Igotomo
*/
#include "mime.h"
#include "io/igx_encrypt.h"

/* 
 case1. "xxxxx"
 case2. xxxxx
 case3. =?sjis?B?asldkas?=
 case4. =?sjis?Q?asldkas?= */
igx_string_t* igx_mime_string_get(igx_string_t* dest, const char* mime_string, HTX mmem_page)
{
	char* p =(char*)mime_string;
    char* out=dest;
    size_t len = strlen (p), out_len;

    /* case 2 */
    if (p[0]=='\"'&&p[len-1]=='\"')
    {
        if (dest!=0)
        {
            strcpy(dest->str, 
        }
    }

	/* case 3 or 4 */
	if (p[0]=='=' && p[1]=='?')
	{
		// case 1, 2
		char* display= 0;
		bit32_t n = 0;
        igx_string_t str_display;
		igx_string_t char_set;
		const char* encode;
		const char* p_display;

        if (igx_get_between_s(&str_display, p, "=?", "?=")==0)
        {
            return 0;
        }
        /* charset?BorQ?content */
        if (igx_str_until_c(&char_set, str_display.str, '?')==0)
        {
            *dest=0;
            return 0;
        }
        p=char_set.str+char_set.length+1;
        if ((*p!='B'&&*p!='Q')||*(p+1)!='?')
        {
            if (dest==0)
            {
                *dest=0;
            }
            return 0;
        }

        len = strlen(p+2);
        if (*p=='B')
        {
            out_len=(len*6)/8;
            out_len+=2;
        }
        else
        {
            out_len=len+1;
        }
        p+=2;

        if (dest==0)
        {
            if (mem_page!=0)
            {
                out = pmalloc(mem_page, sizeof(igx_string_t)+out_len);
            }
            else
            {
                out = IGX_MALLOC(sizeof(igx_string_t)+out_len, "igx_mime_string_get");
            }
            out->length=out_len-1;
            out->str=out;
        }

        if (out_len!=len+1)
        {
            /* Base64 */
            if (igx_decrypt_base64(out, out_len, p, len)==0)
            {
                    if (out!=dest)
                    {
                        if (mem_page==0)
                        {
                            IGX_FREE(out);
                        }
                    }
                }
            }
        }
        else
        {
            memcpy(out, p, out_len);
        }

        if (strcasecmp(p_charset, "utf-8")==0)
        {
            return out;
        }

RETRY:
			else if (strcmp(pCharset, "utf-8")==0)
			{
				char* pSjis = yiUtf8ToMulti(dest, ex?ex:pDisplay, 0);

				if (ex)
					yifree(ex);

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
		if (pIndex[0]=="¥"")
		{
			if (dest)
				yistrchcpy(dest, &pIndex[1], "¥"");
			else
				dest = yistrheapex(&pIndex[1], "¥"");

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
	char* pIndex=yistrTrimIndex(pszEmail, " ¥t¥r¥n");
	
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
		n = yistrReplaceChar(pszDisplay, '?', '¥0');
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
					yifree(ex);
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
			*display=0;

		}
		yifree(pszDisplay);

SKIP_DISPLAY:
		if (email==0)
		{
			return True;
		}

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

	if (pIndex[0]=='¥"')
	{ 
		// case 3.
		char* pszDisplay = yistrBetween(pIndex, "¥"", "¥"", False);
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
	else
	{
		yistrcpyUntil(email, pIndex, " ¥t¥r¥n¥0");
	}

	return True;
}


#define SSL_DEFAULT_TIMEOUT 100

#define xSendCommand(a, b, c) _xSendCommand(a, b, c, DBS-1)

// --------------------------------------
static int _xSendCommand(sockssl_t sock, char* buffer, char* ret, int size)
{
	int n = strlen(buffer);
	if (yisend_ssl(sock, buffer, n)!=n)
	{
		//yiclose_ssl(sock);
		return 0;
	}

	YIDEBUG("xSendCommand(%s) success!", buffer);
	ret[0]=0;

	n = yirecv_ssl(sock, ret, size, SSL_DEFAULT_TIMEOUT);

	if (n==0)
	{
		//yiclose_ssl(sock);
		return 0;
	}

	if (xISOK(ret)==False)
	{
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

	wsprintf(buffer, "USER %s¥r¥n", pszUserName);
	
	ret = xSendCommand(sock, buffer, retb);
	if (ret==-1 ||
			ret==0)
	{
		yiclose_ssl(sock);
		return 0;
	}

	wsprintf(buffer, "PASS %s¥r¥n", pszPassword);
	
	ret = xSendCommand(sock, buffer, retb);
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
void yipop3Destroy(HandlePop3 hPop)
{
	sPop3* s = (sPop3*)hPop;

	if (s->hMailList)
	{
		syiNode* node;
		int i;
		int cnt = yiListCount(s->hMailList);
		for (i=0; i<cnt; i++)
			yifree(yiListRemoveEx(s->hMailList, 0));

		yiListDestroy(s->hMailList);
	}

	yiclose_ssl((sockssl_t)s->sock);
}

// --------------------------------------
int yipop3Stat(HandlePop3 hPop, int* all)
{
	char ret[DBS];
	int n=0;
	int size=0;
	sPop3* s = (sPop3*)hPop;

	if (xSendCommand((sockssl_t)s->sock, "STAT¥r¥n", ret)==False)
	{
		return 0;
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

	if (xSendCommand((sockssl_t)s->sock, "DELE¥r¥n", ret)==False)
	{
		return False;
	}

	return True;
}


// --------------------------------------
static char* xNextLine(const char* p)
{
	char* pNext = strstr(p, "¥r¥n");
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
	int cnt = yipop3Stat(hPop, 0);
	char* idsBuffer = yialloc(cnt*64); // 1行につき64バイトだと仮定する。
	sPop3* s = (sPop3*)hPop;


	YIDEBUG("yipop3UniqIdListening() cnt:%d, mem:%d bytes.", cnt, cnt*64);

	if ((recved=_xSendCommand((sockssl_t)s->sock, "UIDL¥r¥n", idsBuffer, cnt*64))<=0)
	{
		return 0;
	}

	YIDEBUG("yipop3UniqIdListening() %d bytes. received", recved);
	
	nTotalReceived=recved;

	// データを一個のブロックにする。
	if (nTotalReceived)
	{
		char*p=idsBuffer;
		char*pIndex=xNextLine(p);
		
		hMailList = yiListCreate(0, 0);
			
		while(pIndex)
		{
			int no=0;
			char uuid[64];
			sMailList* s;
			int ret = sscanf(pIndex, "%d %s", &no, uuid);

			if (ret!=2)
			{
				break;
			}

			s = (sMailList*)yialloci(sizeof(*s));
			s->id=no;
			strcpy(s->uuid, uuid);
			yiListAdd(hMailList, (IntPtr)s);

			pIndex=xNextLine(pIndex);
		}

		{
			sPop3*x = (sPop3*)hPop;
			x->hMailList=hMailList;
		}

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
	while(*pEnd!=0 && *pEnd!='¥r'&&*pEnd!='¥n') pEnd++;

	if (*pEnd=='¥r')
	{
		if (pEnd[2]=='¥t') // 続きがある。
		{
			pEnd+=2;
			goto SEARCH_END;
		}
	}
	else if (*pEnd=='¥n')
	{
		if (pEnd[2]=='¥t') // 続きがある。
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
	// case2. To: xxxx@xxx.com,¥r¥n¥txxxx@xxxx.com, ....
	//p+=3;
	pOneLine=xGetOneLine(p);
	pEmail=yistrReplaceStr(pOneLine, "¥r¥n¥t", "");



	if (pEmail)
		yifree(pOneLine);
	else
		pEmail=pOneLine;

	{
		char* temp = yistrTrim(0, pEmail, " ¥"");
		yifree(pEmail);
		pEmail=temp;
	}

	// ここで全てケース1の状態に
	list=yistrSplit(pEmail, ",");

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

// --------------------------------------
static sMailFormat* xParseMail(const char* p)
{
	char* pFrom = strstr(p, "¥nFrom:");
	char* pTo	= strstr(p, "¥nTo:");
	char* pSubject = strstr(p, "¥nSubject:");
	char* pCc		= strstr(p, "¥nCC:");
	char* pBcc		= strstr(p, "¥nBCC:");

	char* pBoundary;// = strstr(p, "boundary=");
	char* pEof;
	char  szBoundary[128];
	char  szFileName[128];
	int   len;
	char* pBodyIndex=0;
	sMailFormat* pRet;
	char locale[16];
	Bool bIsBase64=True;

	pBodyIndex=strstr(p, "Content-Type");

	if (pBodyIndex==0)
		return 0;
	{
		char* x = strstr(p, "Content-Language:");
		if (x)
		{
			x = yistrTrimIndex(x, " ");
			yistrcpyUntil(locale, x, "¥r¥n");
		}
	}


	pRet=(sMailFormat*)yialloci(sizeof(sMailFormat));

	if (pFrom)
	{
		char szDisplay[128];
		char szEmail[128];

		pFrom+=6;
		pFrom=yistrTrimIndex(pFrom, " ");
		yiMimeGetNameAnd(pFrom, szDisplay, szEmail);
		pRet->from=yistrz(0, szDisplay, szEmail, 0);
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
		char* pContents = strstr(p, "Content-Type:");
		char* pChar;
		pContents+=strlen("Content-Type:");
		pContents=yistrTrimIndex(pContents, " ");
		yistrcpyUntil(value, pContents, "¥r¥n");

		pChar=strstr(pContents, "charset=");
		pChar+=strlen("charset=");
		yistrcpyUntil(charset, pChar, "¥r¥n");
		pChar=yistrTrim(charset, charset, "¥"");
		
		pBodyIndex = strstr(pBodyIndex, "¥r¥n¥r¥n");
		
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
	if (pBoundary[0]=='¥"')
	{
		pBoundary++;
		yistrchcpy(szBoundary, pBoundary, '¥"');
	}
	else
	{
		yistrchcpy(szBoundary, pBoundary, '¥r');
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
		char* pContents = strstr(pBodyIndex, "Content-Type:");
		char* pChar;
		int bodylen=0;

		pContents+=strlen("Content-Type:");
		pContents=yistrTrimIndex(pContents, " ");
		yistrcpyUntil(value, pContents, "¥r¥n");

		pChar=strstr(pContents, "charset=");
		pChar+=strlen("charset=");
		yistrcpyUntil(charset, pChar, "¥r¥n");
		pChar=yistrTrim(charset, charset, "¥"");
		
		pBodyIndex = strstr(pBodyIndex, "¥r¥n¥r¥n");
		
		bodylen = pBoundary-pBodyIndex-4;
		pBodyIndex[bodylen]=0;

		if (strcmp(pChar, "utp-8")==0)
		{
			pRet->body=yiUtf8ToMulti(0, pBodyIndex, 0);
		}
		else
		{
			pRet->body=yistrheap(pBodyIndex);
			yiToSjis(pRet->body);
		}
	}

	//if (pBoundary==0)
	//{
	//	yifree(pRet);
	//	return 0;
	//}

	pBoundary+=len;

	{
		char* pFileName = strstr(pBoundary, "filename=");
		char* p;

		if (pFileName)
		{
			pFileName+=strlen("filename=");

			p = yistrcpyUntil(0, pFileName, "¥r¥n");

			if (p)
			{
				pRet->filename=yistrTrim(0, p, "¥" ");
				yifree(p);
			}
		}


	}

	{
		char* encoding = strstr(pBoundary, "Content-Transfer-Encoding:");
		

		if (encoding==0)
		{
			bIsBase64=False;
		}
		else
		{
			char value[32];
			char* p;
			

			encoding+=strlen("Content-Transfer-Encoding:");
			p = yistrcpyUntil(value, encoding, "¥r¥n"); 
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
	}

	pBoundary = strstr(pBoundary, "¥r¥n¥r¥n");
	pBoundary+=4;

	pEof = strstr(pBoundary, szBoundary);

	if (pEof==0)
	{
		return pRet;
	}

	if (bIsBase64==False)
	{
		pRet->binary = yiBinary(pBoundary, pEof-pBoundary-4, 0);
		pRet->size = xBIN_SIZE(pRet->binary);
	}
	else
	{
		int n = pEof-pBoundary-4;
		char* px = yitoheap(pBoundary, n+1);
		px[n]=0;
		pRet->binary = yiToPlainForced(px);
		pRet->size = xBIN_SIZE(pRet->binary);
	}

	return pRet;
}

// --------------------------------------
sMailFormat* yipop3Get(HandlePop3 hPop, int index)
{
	char cmd[64];
	char buffer[DBS];
	char* pIndex=buffer;
	char* pBinary=0;
	int n=0, size=0;
	unsigned int next=0;
	sPop3* s = (sPop3*)hPop;

	//sprintf(cmd, "RETR %d¥r¥n", index);
	sprintf(cmd, "LIST %d¥r¥n", index);
	if (xSendCommand((sockssl_t)s->sock, cmd, buffer)==False)
	{
		return 0;
	}


	//pIndex=xNextLine(pIndex);
	pIndex+=4;

	if (sscanf(pIndex, "%d %d", &n, &size)!=2)
	{
		return 0;
	}

	YIDEBUG("get file size is %d", size);

	sprintf(cmd, "RETR %d¥r¥n", index);
	n = strlen(cmd);
	if (yisend_ssl((sockssl_t)s->sock, cmd, n)!=n)
	{
		//yiclose_ssl((sockssl_t)hPop);
		return 0;
	}

	//size+=size*1.5;
	size=size+(size>>1);
	pBinary = yialloc(size+32);
RETRY:
	if ((n=yirecv_ssl((sockssl_t)s->sock, pBinary+next, size+32, SSL_DEFAULT_TIMEOUT))>0)
	{
		if (pBinary[0]!='+')
		{
			return 0;
		}
		pIndex=xNextLine(pBinary);

		if ((*pIndex)==0 || n<64)
		{
			// 受信しきれていない。
			YIWARN("yipop3Get() recv_ssl too small packet:%d", n);
			next+=n;
			goto RETRY;
		}

#ifdef _DEBUG
		{
			char buffer[128];
			sprintf(buffer, "c:¥¥yotsuya¥¥%d.mail", index);
			//DeleteFile(buffer);
			yiFileWriteOnce(buffer, pIndex, 0, n);
		}
#endif
		pBinary[n]=0;
		if (pIndex)
		{
			sPop3* x = (sPop3*)hPop;
			sMailFormat* mail = xParseMail(pIndex);

			if (mail==0)
				return 0;

			if (x->hMailList)
			{
				syiNode* node = yiListGetTopFrom(x->hMailList, index+1);
				if (node)
				{
					sMailList* xx = (sMailList*)node->ref;
					strcpy(mail->uid, xx->uuid);
				}
			}
			return mail;
		}
	}

	return 0;
}

// --------------------------------------
void yipop3Release(sMailFormat* data)
{
	yifree(data->body);
	yistrzp_del(data->to);
	yistrzp_del(data->cc);
	yistrzp_del(data->bcc);
	yifree(data->filename);
	yifree(data->from);
	yifree(data->subject);
	
	yifree(data);
}
