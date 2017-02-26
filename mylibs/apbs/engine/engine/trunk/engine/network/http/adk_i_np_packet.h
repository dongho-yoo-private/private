#ifndef __IREQUEST_PACKET_H__
#define __IREQUEST_PACKET_H__

ADK_BEGIN_NAMESPACE

class NpHttpMethod;

/** ReqPacketのインターフェースクラス
*/
class INpPacket
{
public:
	/* \brief ヘッダーを取得します。
	*
	* \return ヘッダーのアドレス
	*/
	virtual const void* GetHeader()=0 ;

	/* \brief ヘッダーサイズを取得します。
	*
	* \return ヘッダーのサイズ
	*/
	virtual U32 GetHeaderSize()=0 ;

	/* \brief データを取得します。
	*
	* \return データのアドレス
	*/
	virtual const void* GetData()=0 ;

	/* \brief データサイズを取得します。
	*
	* \return データのサイズ
	*/
	virtual U32 GetDataSize()=0 ;

	/* \brief 文字列のプロトコルを返します。
	*
	* \return データのサイズ
	*/
	virtual const Char* GetProtocol()=0 ;

	/* \brief レスポンスを返します。
	*
	* \return void*型のレスポンスを返します。
	*/
	virtual const void* GetResponse() {return mResponse;}

	/* \brief レスポンスを返します。
	*
	* \return void*型のレスポンスを返します。
	*/
	virtual void DeleteResponse(const void* response) {}

protected:
	const void* mResponse;

private:
	virtual void SetResponse(const void* response){mResponse=response;}

	friend class NpHttpMethod;

};

ADK_END_NAMESPACE

#endif
