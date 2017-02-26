#include "../yi.h"
#include "../yisock.h"

// ----------------------------------
int main(int argc, char** argv)
{
	const char* pszServer = (const char*)argv[1];
	int sock;
	char buffer[256];
	const char* pszPass = "PASS 920068\n";
	address_t address;
	address = yihtol(pszServer, 0);	
	sock = yiconnect(address, 110, 1);

	if (sock<0)
	{
		printf("connect failure! %s\n", pszServer);
		return 0;
	}
	memset(buffer, 0, sizeof(buffer));
	yirecv(sock, buffer, 128, 0);
	printf(buffer);

	printf("USER donho_yoo\n");
	yisend(sock, "USER donho_yoo\n", strlen("USER dongho_yoo\n"), False);
	memset(buffer, 0, sizeof(buffer));
	yirecv(sock, buffer, 256, 0);
	printf(buffer);
	//putchar('\n');

	printf(pszPass);
	yisend(sock, pszPass, strlen(pszPass), False);
	yisend(sock, pszPass, strlen(pszPass), False);
	memset(buffer, 0, sizeof(buffer));
	yirecv(sock, buffer, 256, 0);
	printf(buffer);

	close(sock);
	//putchar('\n');

	printf("trace............2\n");

	{
		int sock2;
		address = yihtol(argv[2], 0);	
		sock2 = yiconnect(address, 587, 1);
		if (sock<0)
		{
			printf("connect failure! %s\n", argv[2]);
			return 0;
		}
		memset(buffer, 0, sizeof(buffer));
		yirecv(sock2, buffer, 128, 0);
		printf(buffer);

		memset(buffer, 0, sizeof(buffer));
		yisend(sock2, "AUTH LOGIN\n", strlen("AUTH LOGIN\n"), False);
		yirecv(sock2, buffer, 128, 0);
		printf(buffer);

		memset(buffer, 0, sizeof(buffer));
		yisend(sock2, "ZG9uaG9feW9v\n", strlen("ZG9uaG9feW9v\n"), False);
		memset(buffer, 0, sizeof(buffer));
		yirecv(sock2, buffer, 128, 0);
		printf(buffer);

		memset(buffer, 0, sizeof(buffer));
		yisend(sock2, "OTIwMDY4\n", strlen("OTIwMDY4\n"), False);
		memset(buffer, 0, sizeof(buffer));
		yirecv(sock2, buffer, 128, 0);
		printf(buffer);

		printf("MAIL FROM: donho_yoo@yahoo.co.jp\n");
		yisend(sock2, "MAIL FROM: donho_yoo@yahoo.co.jp\n", strlen("MAIL FROM: donho_yoo@yahoo.co.jp\n"), False);//, strlen("HELO\n"));HELO\n"));
		memset(buffer, 0, sizeof(buffer));
		yirecv(sock2, buffer, 128, 0);
		printf(buffer);

		printf("RCPT TO: dongho.yoo@live.jp\n");//MAIL FROM: donho_yoo@yahoo.co.co.jp\n");
		yisend(sock2, "RCPT TO: dongho.yoo@live.jp\n", strlen("RCPT TO: dongho.yoo@live.jp\n"), False);//, strlen("HELO\n"));HELO\n"));
		memset(buffer, 0, sizeof(buffer));
		yirecv(sock2, buffer, 128, 0);
		printf(buffer);

		printf("RCPT TO: dongho.tfp@gmail.com\n");//MAIL FROM: donho_yoo@yahoo.co.co.jp\n");
		yisend(sock2, "RCPT TO: dongho.tfp@gmail.com\n", strlen("RCPT TO: dongho.tfp@gmail.com\n"), False);//, strlen("HELO\n"));HELO\n"));
		memset(buffer, 0, sizeof(buffer));
		yirecv(sock2, buffer, 128, 0);
		printf(buffer);

		printf("RCPT TO: yoo@fit.co.jp\n");//MAIL FROM: donho_yoo@yahoo.co.co.jp\n");
		yisend(sock2, "RCPT TO: yoo@fit.co.jp\n", strlen("RCPT TO: yoo@fit.co.jp\n"), False);//, strlen("HELO\n"));HELO\n"));
		memset(buffer, 0, sizeof(buffer));
		yirecv(sock2, buffer, 128, 0);
		printf(buffer);

		printf("DATA\n");//MAIL FROM: donho_yoo@yahoo.co.co.jp\n");
		yisend(sock2, "DATA\n", strlen("DATA\n"), False);//, strlen("HELO\n"));HELO\n"));
		memset(buffer, 0, sizeof(buffer));
		yirecv(sock2, buffer, 128, 0);
		printf(buffer);

		{
			char buffer[1024];
			sprintf(buffer, "To: xxx@pakopako.com\r\nFrom: bokoko@goyotsuya.com\r\nSubject: メイルがいくかな？\r\nMIME-Version:1.0\r\nContent-type:text/plain; charset=UTF-8\r\nContent-Transfer-Encoding: 7bit\r\n\r\nこんにちは！文字コードは大丈夫でしょうか？見えてますか？\r\n.\r\n");
			yisend(sock2, buffer, strlen(buffer), False);//"RCPT TO: dongho.yoo@live.jp\n", strlen("RCPT TO: dongho.yoo@live.jp\n"));//, strlen("HELO\n"));HELO\n"));
			memset(buffer, 0, sizeof(buffer));
			yirecv(sock2, buffer, 128, 0);
			printf(buffer);

		}

		printf("QUIT\n");//MAIL FROM: donho_yoo@yahoo.co.co.jp\n");
		yisend(sock2, "QUIT\n", strlen("QUIT\n"), False);//, strlen("HELO\n"));HELO\n"));
		memset(buffer, 0, sizeof(buffer));
		yirecv(sock2, buffer, 128, 0);
		printf(buffer);

		close(sock2);
	}


	return 0;
}

