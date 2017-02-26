#ifndef __HTTP_REQ_PACKET_H__
#define __HTTP_REQ_PACKET_H__

#include <core/types.h>
#include <network/http/adk_i_np_packet.h>
#include <network/http/adk_network_types.h>
#include <network/http/adk_np_types.h>

#define HTTP_USING_C_PLUS_PLUS
#include <network/http/adk_http_response.h>

#define MAX_HEADER_LEN (1023)
ADK_BEGIN_NAMESPACE

class NpHttpMethod;

/** \brief HTTPプロトコル専用のパケットクラス　
*/
class NpHttpPacket : public INpPacket
{
public:
	/** \breif constructor for GET method. 
	*
	* \param url ex) "http://www.abc.com:8080/~mypage/abc.cgi?name=xxxx,pass=abcd"
	* \param keepAliveTime http KeepAlive time for seconds.
	* \param contentType ContentType ex) "application/x-wwww-form-urlencoded"
	* \param characterSet ex) "utf-8", "shift-jis" ... and so on.
	*/
	NpHttpPacket (const Char* url, S32 keepAliveTime=0,
			const Char* contentType="application/x-www-form-urlencoded", 
							const Char* characterSet="utf-8");

	/** \breif constructor for POST method. 
	*
	* \param url ex) "http://www.abc.com:8080/~mypage/abc.cgi"
	* \param data ex) "name=xxxx,pass=abcd"
	* \param dataSize data size
	* \param keepAliveTime http KeepAlive time for seconds.
	* \param contentType ContentType ex) "application/x-www-form-urlencoded"
	* \param characterSet ex) "utf-8", "shift-jis" ... and so on.
	*/
	NpHttpPacket (const Char* url, const void* data, S32 dataSize, S32 keepAliveTime=0,
				const Char* contentType="application/x-www-form-urlencoded", 
							const Char* characterSet="utf-8");

	/** \breif constructor for POST/GET method. 
	*
	* \param url ex) "http://www.abc.com:8080/~mypage/abc.cgi"
	* \param data ex) "name=xxxx,pass=abcd"
	* \param dataSize data size (0 is GET method)
	* \param headers  
	* \param headerCount 
	*/
	NpHttpPacket(const Char* url, const void* data, S32 dataSize, const TRequestHttpHeader* headers, int arrayCount);

	virtual ~NpHttpPacket ();

	/** \breif constructor for GET method. 
	*
	* \param url ex) "http://www.abc.com:8080/~mypage/abc.cgi?name=xxxx,pass=abcd"
	* \param keepAliveTime http KeepAlive time for seconds.
	* \param contentType ContentType ex) "application/x-wwww-form-urlencoded"
	* \param characterSet ex) "utf-8", "shift-jis" ... and so on.
	*/
	void Set(const Char* url, S32 keepAliveTime=0,
			const Char* contentType="application/x-www-form-urlencoded", 
							const Char* characterSet="utf-8");

	/** \breif constructor for POST/GET method. 
	*
	* \param url ex) "http://www.abc.com:8080/~mypage/abc.cgi"
	* \param data ex) "name=xxxx,pass=abcd"
	* \param dataSize data size (0 is GET method)
	* \param headers  
	* \param headerCount 
	*/
	void Set(const Char* url, const void* data, S32 dataSize, const TRequestHttpHeader* headers, int headerCount);

	/** \breif Set for POST method. 
	*
	* \param url ex) "http://www.abc.com:8080/~mypage/abc.cgi"
	* \param data data 
	* \param dataSize data size
	* \param keepAliveTime http KeepAlive time for seconds.
	* \param contentType ContentType ex) "application/x-wwww-form-urlencoded"
	* \param characterSet ex) "utf-8", "shift-jis" ... and so on.
	*/
	void Set(const Char* url, const void* data, S32 dataSize, S32 keepAliveTime=0, 
			const Char* contentType="application/x-wwww-form-urlencoded", 
							const Char* characterSet="utf-8");


	/* \brief ヘッダーを取得します。
	*
	* \return ヘッダーのアドレス
	*/
	const void* GetHeader();

	/* \brief ヘッダーサイズを取得します。
	*
	* \return ヘッダーのサイズ
	*/
	U32 GetHeaderSize();

	/* \brief データを取得します。
	*
	* \return データのアドレス
	*/
	const void* GetData();

	/* \brief データサイズを取得します。
	*
	* \return データのサイズ
	*/
	virtual U32 GetDataSize();

	/* \brief 文字列のプロトコルを返します。
	*
	* \return プロトコル("http", "svn", "telent"... )
	*/
	virtual const Char* GetProtocol();

	/* \brief レスポンスを取得します。\n
	*  ADK_HttpのTHttpDetailのisAutoFreeがfalseの場合は、\n
	*  必ずレスポンスは使い終わった後、各自でdeleteしてください。\n
	* ex)\n
	* const HttpResponse* res = packet.GetResponse()\n
	* :\n
	* :\n
	* delete res;
	*/
	const HttpResponse* GetHttpResponse();

	/** \brief GetResponseで取得したデータを削除します。
	*
	*\param p GetResponseで取得したデータの先頭ポインター　
	*/
	void DeleteResponse(const void* p);

private:
	char mProtocol[MAX_PROTOCOL_LEN+1];
	char mHttpHeader[MAX_HEADER_LEN+1];
	char mHostName[MAX_HOSTNAME_LEN+1];
	S32 mPort;
	Bool mUseSSL;
	void* mData;
	U32 mDataSize;
	S32 mHeaderSize;

	friend class NpHttpMethod;
};

ADK_END_NAMESPACE
#endif
