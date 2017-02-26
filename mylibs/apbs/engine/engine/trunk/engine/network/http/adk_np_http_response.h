#ifndef __NPHTTPRESPONSE_H__
#define __NPHTTPRESPONSE_H__

#include <core/types.h>
#include <network/http/adk_i_np_response.h>

ADK_BEGIN_NAMESPACE

class NpHttpResponse : public INpResponse
{
public:
	/** constructor&destructor */
	NpHttpResponse();
	virtual ~NpHttpResponse();

	/** \brief 結果を取得します。
	*
	* \return 結果
	*/
	virtual S32 GetResult() ;

	/** \brief データを取得します。
	*
	* \return データ
	*/
	virtual const void* GetData() ;

	/** \brief データを取得します。
	*
	* \return データのサイズ
	*/
	virtual U32 GetDataSize() ;

	/** \brief データを取得します。
	* \return 文字列のContentType
	*/
	const Char* GetContentType() ;

	/** \brief ContentTypeが同じかどうかをチェックします。\n
	*	GetContentType()で取得して比較するのとの違いは、\n
	*	この関数の中では大文字小文字を区別せず比較する事。
	*
	* \return 同じならtrue, 同じでなければfalse
	*/
	Bool IsContentTypeSame(const Char* contentType);

private:
	S32 mStatus;
	U32 mDataSize;
	const Char* mData;
	const Char* mContentType;

	void Set(S32 status, const void* data, U32 size, const Char* contentType);
	friend class NpHttpMethod;
};

ADK_END_NAMESPACE

#endif
