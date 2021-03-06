// yiLibTesterConsole.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"


#define _WINDLL
#include "yistd.h"
#include "yiEncoder.h"
#include "yisock.h"
#undef _WINDLL

#include "yit.h"


#include <windows.h>

typedef struct {
	int x;
	int y;
}sAnyStruct;


int _tmain_other(int argc, _TCHAR* argv[])
{
	char szFileName[256];
	szFileName[0]=0;
	//cyitList<int> xlist;
	cyitrb<int> xring(10);
	cyitrb<sAnyStruct> xring2(10);
	yiInitWinsock(2,2);

	//WritePrivateProfileString("abc", "abc", "0", "c:\\vfrwin\\abc.ini"); 
	//int x = GetPrivateProfileInt("abc", "abc", 0, "c:\\vfrwin\\abc.ini");

	int xsize;
	void* pText = yiFileLoad("c:\\vfrwin\\a.txt", &xsize);
	int sockx=yiconnect(0, 8122, 0);

	if (sockx<0)
		return 0;
	yisend(sockx, pText, xsize, 0);

	yiclose(sockx);

	const char* pszEnd="QUIT";
	sockx = yiconnect(0, 8122, 0);
	yisend(sockx, (void*)pszEnd, 4, 0);
	yiclose(sockx);




	int sock = yiudp(0, -1);

	if (sock<0)
	{

		return 0;
	}

	//address_t host = yiitol("127.0.0.1");

	if (yisendxu(sock, 0, 0, 0, 0, 0, 0, 8192, True, 0)<0)
	{
		return 0;
	}

	address_t addr=0;
	int port=0;
	void* data=0;
	int n;
	syipacketu packet;
	char bufferx[512];

	//while(1)
	//{
	//n = recvfrom(sock, bufferx, 512, 0, 0, 0);
	//n = recvfrom(sock, bufferx, 512, 0, 0, 0);
	//}


	if ((n=yirecvxul(sock, &packet, bufferx, 0, -1, &addr, &port, &data))<0)
	{
		return 0;
	}

	yiFileWrite("c:\\vfrwin\\b.jpg", data, 0, n);


	for (int i=0; i<10; i++)
	{
		int n = xring.enq(i);
		printf("enq %d = %d\n", i, n);
	}
	for (int i=0; i<3; i++)
	{
		int val;
		int n = xring.deq(val);
		printf("enq %d = %d\n", val, n);
	}
	for (int i=0; i<10; i++)
	{
		int n = xring.enq(i);
		printf("enq %d = %d\n", i, n);
	}

	while(1)
	{
		int i;
		int n = xring.deq(i);
		if (n==-1)
			break;
		printf("deq %d = %d\n", i, n);

	}
	getchar();

	if (yiFileOpenDialog(::GetDesktopWindow(), "開く", "*.sgf", "sgf(*.sgf);ygf(*.ygf)", "label", szFileName)==False)
	{
		return 0;
	}

	int size;
	void* buffer;

	buffer = yiFileLoad(szFileName, &size);

	if (buffer==0)
		return 0;

	void* out = yialloc(size);

	int out_size;
	if (cyiEncoder::Compress(out, buffer, size, out_size)==False)
	{
		yifree(out);
		yifree(buffer);
		return False;
	}

	printf("compress ok.... (%d->%d)\n", size, out_size);
	//yifree(buffer);

	yiFileWrite("compressedfile.zip", out, 0, out_size);

	void* buffer2 = yialloc(size);


	int ex=0;
	if (cyiEncoder::UnCompress(buffer2, out, out_size, ex)==False)
	{
		yifree(out);
		yifree(buffer);
		return 0;
	}
	printf("uncompress ok.... (%d->%d)\n", out_size, ex);

	if (memcmp(buffer, buffer2, ex)==0)
	{
		printf("binary is same !!\n");
	}

	yiFileWrite("extract", buffer2, 0, ex);
	yifree(out);
	yifree(buffer);

	getchar();

	return 0;
}

#include "yipop3.h"
#include "yiupnp.h"
int _tmain_pop3(int argc, _TCHAR* argv[])
{
	yiInitWinsock(2, 2);

	if (argv[1]==0)
	{
		argv[2]=0;
		argv[3]=0;
		argv[4]=0;
	}

	address_t addr = ::yihtol(argv[1]==0?"pop.mail.yahoo.co.jp":argv[1], 0);
	int port=argv[2]?atoi(argv[2]):110;

	HandlePop3 hPop=::yipop3Create(addr, port, argv[3]?argv[3]:"cosmosinterface", argv[4]?argv[4]:"9334080", argv[1]?True:False);

	if (hPop==0)
	{
		return 0;
	}

	printf("trace....1\n");
	HandleList hList = ::yipop3UniqIdListening(hPop);
	int all=0;
	int n = yipop3Stat(hPop, &all);
	printf("all.... %d, stat result:%d %dbytes.\n", yiListCount(hList), n, all);
	getchar();

	int cnt=0;
	for (syiNode* node = yiListGetTopFrom(hList, 0); node; node=node->np)
	{
		sMailList* list = (sMailList*)node->ref;
		printf("%d> %d %s\n", cnt, list->id, list->uuid);
		sMailFormat* s = yipop3Get(hPop, cnt++);

		if (s==0)
		{
			printf("xxxxxxxxxxxxxxxxxxx download failure!!\n");
		}
		else
		{
			printf("success... %s\n", s->uid);
		}
	}
	yipop3Destroy(hPop);
	return 0;

	int index;
	printf("enter no > ");
	scanf("%d", &index);

	sMailFormat* s = yipop3Get(hPop, index);
	yipop3Destroy(hPop);

	if (s)
	{
		char* x=0;
		if (s->uid)
		{
			printf("uid:%s\n", s->uid);
		}
		printf("From: %s<%s>\n", (x=(char*)yistrzGet(s->from, 0, 0))==0?"":x, yistrzGet(s->from, 1, 0));

		if (s->to)
		{
			printf("To:");

			int n = yistrzpCount(s->to);
			for (int i=0; i<n; i++)
			{
				strz to = s->to[i];
				printf(" %s<%s>,",(x=(char*)yistrzGet(to, 0, 0))==0?"":x, yistrzGet(to, 1, 0));
			}
			putchar('\n');
		}

		if (s->cc)
		{
			printf("CC:");

			int n = yistrzpCount(s->cc);
			for (int i=0; i<n; i++)
			{
				strz cc = s->cc[i];
				printf(" %s<%s>,",(x=(char*)yistrzGet(cc, 0, 0))==0?"":x, yistrzGet(cc, 1, 0));
			}
			putchar('\n');
		}

		if (s->subject)
		{
			printf("Subject: %s\n", s->subject);
		}
		
		if (s->body)
		{
			printf("body:%s\n", s->body);
		}

		if (s->binary && 
			s->filename)
		{
			DeleteFile(s->filename);
			
			yiFileWriteOnce(s->filename, s->binary, 0, xBIN_SIZE(s->binary));
			printf("file:%s writed(%d bytes)", s->filename, s->size);
		}

	}

	getchar();


	return 0;
}

#include "yismtp.h"
#include "yistd.h"
int _tmain_smtp(int argc, _TCHAR* argv[])
{
	::yiInitWinsock(2, 2);

	address_t addr = ::yihtol("smtp.mail.yahoo.co.jp", 0);
	HandleSmtp hSmtp = ::yiSmtpCreate(addr, 587, "donho_yoo", "920068");
	::yiSmtpAddTo(hSmtp, "dongho.tfp@gmail.com", "dongho", 0);
	::yiSmtpAddTo(hSmtp, "yoo@fit.co.jp", "どんほ",  "sjis");
	::yiSmtpAddCc(hSmtp, "dongho.yoo@live.jp", 0, 0);
	::yiSmtpAddCc(hSmtp, "cosmosinterface@yahoo.co.jp", "きゃかや！", "sjis");
	::yiSmtpAddBcc(hSmtp, "donho_yoo@yahoo.co.jp", 0, 0);
	::yiSmtpSetFrom(hSmtp, "donho_yoo@yahoo.co.jp", "ヤフーメール",  "sjis");
	::yiSmtpSetReply(hSmtp, "admin@goyotsuya.com", "四谷囲碁センター", "sjis");
	::yiSmtpSetSubject(hSmtp, "これは普通に行くのかな？", "sjis");
	::yiSmtpSetBody(hSmtp, "テストメールだけど。。。どうだろ。。。\r\nどうだろう。。。", "text/plain", "sjis", "8bit");

	int size=0;
	void* p = yiFileLoad("c:\\yotsuya\\temp.xlsx", &size);
	::yiSmtpAddFile(hSmtp, p, size, "temp.xlsx", "application/ngf", "base64");
	
	::yiSmtpSendEx(hSmtp, False, 0, 0);
	yifree(p);
	::yiSmtpDestroy(hSmtp);

	getchar();
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	char addr[32];

	::yiInitWinsock(2, 2);

	HandleUpnp hUpnp = ::yiupnpCreatex();
	::yiupnpGetExternalAddressx(hUpnp, addr);

	// return _tmain_other(argc, argv);

	//return _tmain_smtp(argc, argv);
	return _tmain_pop3(argc, argv);
}
