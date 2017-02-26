/** \brief NetworkProtocol usage.
* \code
* // 1. Case http protocol.
* 
* NetworkProtocol* np = System::GetNetworkProtocol(server);
* NpHttpPacket data("https://xxx.xxxxx.com:500/api/aaaa/", AnyData, AnyDataSize, keepAliveTime,
*					"urlencode-xxx-applitcation", "utf-8");
* NpHttpResponse response;
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
* const adk::HttpResponse* response = data.GetHttpResponse();
* // or 
* const adk::HttpResponse* response = (const adk::HttpResponse*)data.Getresponse();
* 
* if (response->GetResult()!=200)
* {
* 	// TODO. http error.
* }
* 
* // TODO. 
* if (response->IsContentTypeSame("text/plan")==0)
* {
* 	ConvertXXXX(response->data, response->size);
* }
* else if (response->IsContentTypeSame("gzipBase64")==0)
* {
* 	ConvertGZip(response->data, response->);
* }
*			:
*			:
* // delete for response.
* delete response;
*
*
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
* NpRawPacket packet(&header, sizeof(header), &body, sizeof(body));
* Result result = np->Request(&packet);
* if (result!=ResultSuccess)
* {
* 	// TODO. network error
* }
*
* // 
* const AnyHeader* p = (const AnyHader*)packet.GetResponse();
* 
* if (p->result!=####)
* {
* 	// TODO. error form server
* }
* :
* :
* packet.DeleteResponse(p);
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
* \endcode
*
*/
#ifndef __ADK_NETWORK_PROTOCOL_H__
#define __ADK_NETWORK_PROTOCOL_H__

#include <core/types.h>
#include <debug_profiling/error/result.h>
#include <network/http/adk_np_types.h>
#include <network/http/intern/http.h>

ADK_BEGIN_NAMESPACE

class INpResponse;
class INpPacket;
class INpMethod;

/** @brief
*/
class NetworkProtocol
{
public:
	/** @brief constructor&destructor
	*/
	NetworkProtocol();
	virtual ~NetworkProtocol();

	/** \brief 初期化処理を行います。
	*
	*\return Result 成功した場合はResultSuccessを返します。
	*\retval ResultOutOfRange 失敗時のエラー
	*/
	Result Initialize();

	/** \brief 終了処理を行います。
	*
	* \param isForced falseの場合は通信終了を待ってから終了します。trueの場合は強制的に通信を終了させます。
	*\return Result 成功した場合はResultSuccessを返します。
	*\retval ResultOutOfRange 失敗時のエラー
	*/
	Result Terminate(Bool isForced);

	/**\brief リクエストをサーバへ送ります。\n
	* httpのようにrest型のプロトコルに関では対応していません。
	* 
	*\param [in] hostname ホスト名 (ex hostname.com, xxx.xxx.xxx.xxx, protocol://hostname.com:port ....
	*\param [in] port番号、hostnameにportが記載されていれば、hostnameのポート番号を優先、0はデフォルトポート
	*\param [in] name ログイン名
	*\param [in] password ログインパスワード
	*
	*\return Result 成功した場合はコネクト成功
	*\retval ResultUnknown 
	*\retval ResultNetwork 
	*/
	Result Connect(const char* hostname, S32 port=0, const Char* name=0, const Char* password=0);

	/**\brief コネクションをクローズします。
	*/
	void Close();

	/**\brief リクエストをサーバへ送ります。
	* 
	*\param [in] data 送信データのアドレス
	*\param [in] size 送信データのサイズ
	*
	*\return Result 成功した場合はResultSuccessを返します。
	*\retval ResultUnknown 
	*\retval ResultNetwork 
	*/
	//Result Request(void* data, U32 datasize);

	/**\brief リクエストをサーバへ送ります。
	* 
	*\param [in] kStringData送信データのアドレス(文字列)
	*
	*\return Result 成功した場合はResultSuccessを返します。
	*\retval ResultUnknown 
	*\retval ResultNetwork 
	*/
	//Result Request(const Char* kStringData);

	/**\brief リクエストをサーバへ送ります。\n
	* 特殊なパケットを送る際に使われます。(ex http, https ....)
	* 
	*\param [in] packet IRequestPacket形式のデータ
	*\param [in] timeout ミリ秒
	*
	*\return Result 成功した場合はResultSuccessを返します。
	*\retval ResultUnknown 
	*\retval ResultNetwork 
	*/
	Result Request(INpPacket* packet, S32 timeout=0);

private:
	INpMethod* mMethodz;
	INpMethod* mCurrentMethod;
	S32 mReadTimeout;
};

ADK_END_NAMESPACE

#endif
