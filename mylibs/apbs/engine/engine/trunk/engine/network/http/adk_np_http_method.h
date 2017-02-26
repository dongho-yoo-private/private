#ifndef __ADK_HTTP_METHOD_H__
#define __ADK_HTTP_METHOD_H__

#include <core/library/lib_thread.h>
#include <core/types.h>
#include <network/http/adk_i_np_method.h>
#include <network/http/adk_np_http_response.h>
#include <network/http/intern/http.h>

#define MAX_HTTP_CLOSE_WAIT_RETRY_COUNT (5)

ADK_BEGIN_NAMESPACE

class INpReqPacket;

/** \brief
*/
class NpHttpMethod : public INpMethod 
{
public:
	/** \brief constructor&destructor
	*/
	NpHttpMethod();
	virtual ~NpHttpMethod();

	/** \brief 処理できるプロトコルかどうかを返します。
	*
	* \return true処理可能なプロトコル、false処理可能でない。
	*/
	virtual Bool IsMyProtocol(const Char* protocol);

	/** \brief rest機構かどうかを返します。
	*
	* \return true Restである、false Restでない。\n
	* Restタイプでない場合は、必ずConnect関数を実装する必要があります。
	*/
	virtual Bool IsTypeRest();

	/** \brief サーバへ要求をします。
	*
	* \param data 文字列のデータ
	* \timeout タイムアウト(ミリ秒）でフォルト-1はブロック
	*
	* \return エラーコードを返します。
	* \retval kNPE_SUCCESS 成功
	* \retval kNPE_CONNECT コネクトエラー(Rest Typeのみ)
	* \retval kNPE_CONNECT_TIMEOUT コネクトタイムアウト(Rest Typeのみ）
	* \retval kNPE_WRITE 送信エラー
	* \retval kNPE_READ  受信エラー
	* \retval kNPE_READ_TIMEOUT  受信タイムアウト
	* \retval kNPE_PEER サーバ側の問題
	* \retval kNPE_NO_RESOUCE リソース不足
	* \retval kNPE_UNKNOWN 不明なエラー
	*/
	virtual TNPError Request(INpPacket* packet, S32 timeout=NO_TIMEOUT);

	/** \brief リソースを解放します。\n
	*   \param bIsForced trueの場合は強制終了、falseの場合は通信終了を待ちます。
	*/
	virtual void Close(Bool bIsForced);
private:
	NpHttpResponse mResponse;
	HandleHttp mhHttp;
	HandleMutex mhMutex;
};

ADK_END_NAMESPACE

#endif
