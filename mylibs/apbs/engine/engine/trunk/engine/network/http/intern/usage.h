/**
*
* // 1. Case http protocol.
* 
* NetworkProtocol* np = System::GetNetworkProtocol(server);
* NpHttpReqPacket data("https://xxx.xxxxx.com:500/api/aaaa/", AnyData, AnyDataSize, keepAliveTime,
*					"urlencode-xxx-applitcation", "utf-8");
* HttpResponse response;
*
* if (server==kServerTypeXXXXX)
* {
*	data.Set("https://xxx.xxx.com:3030/geturls/", "appname:chaos", strlen(appname:chaos), 0,
*			"text/plan", "utf-8", kHRDM_POST);
* }
*
* // send request.
* Result = np->Request(&data, 1000);
* if (Result!=ResultSuccess)
* {
* 	// TODO. network error
* }
*
* // 
* np->Response(&response);
* 
* if (response.GetResult()!=200)
* {
* 	// TODO. http error.
* }
* 
* // TODO. 
* if (strcmp(response.contentType, "text/plan")==0)
* {
* 	ConvertXXXX(response.GetData(), response.GetSize());
* }
* else if (strcmp(response.GetContentType(), "gzipBase64")==0)
* {
* 	ConvertGZip(response.GetData(), response.GetSize());
* }
*			:
*			:
* // 2. Case Raw.
* NetworkProtocol* np = System::GetNetworkProtocol(kServerXXXX);
* RawResponse response;
*
* Result = np->Connect(hostname, port);
* if (Result!=ResultSuccess)
* {
* 	// TODO. connect error
* }
*
* TAnyHeader header;
* header.id = kID_LOGIN;
* :
* :
* TLoginBody body;
* strcpy(body.name, "hoge");
* strcpy(body.passwd, "hoge123");
*
* // send request.
* NpRawReqPacket packet(&header, sizeof(header), &body, sizeof(body));
* Result = np->Request(&packet);
* if (Result!=ResultSuccess)
* {
* 	// TODO. network error
* }
*
* // 
* np->Response(&response);
* 
* if (response.GetResult()!=####)
* {
* 	// TODO. error form server
* }
*
* 
* // TODO. 
* TAnyPacketHeader* header = (TAnyPacketHeader*)response.GetData();
* if (header->id==????)
* {
*	TAnyPacketBody* body = (TAnyPacketBody*)&header[1];
* }
* else if (....)
* :
* :
* :
* np->Close();
*
*
* // 3. Case like telnet.
* NetworkProtocol* np = System::GetNetworkProtocol(kServerXXXX);
* TelnetResponse response;
* Result = np->Connect("telnet://hostname.com:23", 23, username, password);
*
* if (Result!=ResultSuccess)
* {
* 	// TODO. connect error
* }
*
* // 
* np->Response(&response);
* 
* if (response.GetResult()!=220)
* {
* 	// TODO. user not found 
*	// TODO. invalid password
* }
*
* Result = np->Request("telnet://PASS hoge123", strlen("PASS hoge123"));
*
* np->Response(&response);
* 
* if (Result!=ResultSuccess)
* {
* 	// TODO. network error
* }
*
* if (response.GetResult()!=220)
* {
* 	// TODO.  invalid password.
* }
*
* Result = np->Request("telnet://ls -l");
*
* if (Result!=ResultSuccess)
* {
* 	// TODO. network error
* }
* 
* np->Response(&response);
* // TODO. 
* printf("%s", np->GetData());
* :
* :
* np->Close();
*
*
* // 4. Case like ftp.
*
* NetworkProtocol* np = System::GetNetworkProtocol(server);
* FtpResponse response;
* Result = np->Connect("ftp://hostname.com", username, password);
*
* if (Result!=ResultSuccess)
* {
* 	// TODO. connect error
* }
* // 
* np->Response(&response);
* 
* if (response.GetResult()!=220)
* {
* 	// TODO.  user not found(230?) or invalid password(300?)
* }
*
* Result = np->Request("get abcd.zip");
*
* np->Response(&response);
* 
* if (Result!=ResultSuccess)
* {
* 	// TODO. network error
* }
* 
* if (response.GetResult()!=???)
* {
*	// TODO. file not found.
* }
*
* // TODO. 
* write(fd, resonse.GetData(), response.GetSize());
* :
* :
* np->Close();
*
*
*/
