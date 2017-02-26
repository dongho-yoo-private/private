#ifndef __ADK_NETWORK_METHOD_H__
#define __ADK_NETWORK_METHOD_H__

#include <core/library/lib_list.h>
#include <core/types.h>
#include <network/http/adk_np_types.h>
//#include <intern/network/adk_i_np_response.h>

#define NP_COMMON_GLOBAL_MEMORY_SIZE (1024*10) /* 10kbytes */
#define xMAX_HOST_COUNT (32)
#define xMAX_HOST_NAME_LEN (255)

ADK_BEGIN_NAMESPACE

class INpResponse;
class INpPacket;
class NetworkProtocol;

/**\brief pure virtual クラス*/
class INpMethod
{
public:
	INpMethod();
	virtual ~INpMethod();

	/** \brief 処理できるプロトコルかどうかを返します。
	*
	* \return true処理可能なプロトコル、false処理可能でない。
	*/
	virtual Bool IsMyProtocol(const Char* protocol)=0;

	/** \brief rest機構かどうかを返します。
	*
	* \return true Restである、false Restでない。\n
	* Restタイプでない場合は、必ずConnect関数を実装する必要があります。
	*/
	virtual Bool IsTypeRest()=0;

	/** \brief 接続します。
	* \param [in] hostname 接続先のホスト名 (ex "abcd.com", "xxx.xxx.xxx.xxx","telnet://abcd.com:30" ... \n
	* restタイプ出ない場合は、必ず実装する必要があります。
	* \param [in] id ログインID
	* \param [in] password パスワード
	*
	* \return エラーコードを返します。
	* \retval kNPE_SUCCESS 成功
	* \retval kNPE_CONNECT コネクトエラー
	* \retval kNPE_CONNECT_TIMEOUT コネクトタイムアウト
	* \retval kNPE_INVALID_USER	不正なID
	* \retval kNPE_INVALID_PASSWD	不正なパスワード
	*/
	virtual TNPError Connect(const char* hostname, const Char* id=0, const Char* password=0, S32 timeout=-1);

	/**\brief セッションをクローズします。
	* 
	* \param bIsForced trueの場合は強制終了、falseの場合は通信完了後終了します。
	*/
	virtual void Close(Bool bIsForced)=0;

	/** \brief サーバへ要求をします。\n
	* 主にhttpのようなrest typeで使われます。
	*
	* \param  packet 
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
	virtual TNPError Request(INpPacket* packet, S32 timeout=NO_TIMEOUT)=0;

	/** \brief 次のINpMethodを設定します。
	*
	*\param _next次のINpMethod
	*/
	INpMethod* SetNext(INpMethod* _next);

protected:
	address_t SearchHostAddr(const char* hostname);
	address_t AddHostAddr(address_t addr, const char* hostname);

	static char* 	mpStaticMemoryTop;
	static U32	mStaticMemoryOffset;
	static void* 	Malloc(U32 size);

private:
	INpMethod* mNext;
	HandleList mhList;

	friend class NetworkProtocol;
};

ADK_END_NAMESPACE
#endif
