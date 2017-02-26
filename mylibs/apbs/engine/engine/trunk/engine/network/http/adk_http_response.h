#ifndef __ADK_HTTP_RESPONSE_H__
#define __ADK_HTTP_RESPONSE_H__

#include <core/types.h>

#ifdef __cplusplus
ADK_BEGIN_NAMESPACE
#endif

/** @brief レスポンスデータ
*/
typedef struct {
	/**@brief レスポンス時のステータス\n
	* ex) 200, 505....
	*/
	S32 status;
	/**@brief レスポンスデータサイズ\n
	* 値がゼロの場合は、dataは無効。
  * data が持っている終端 '\0' はsizeには含まれない。
	*/
	S32 size;
	/**@brief ContentType
	* ex) "text/html", "application/octet-stream", ...
	*/
	Char* contentType;
	/**@brief レスポンスデータ\n
   *        データの終端には '\0' が含まれる。
	*/
	Char* data;
} THttpResponse;


//#ifdef HTTP_USING_C_PLUS_PLUS

/** \brief C++用のレスポンスクラス\n
*/
class HttpResponse {
public:
	/** \brief コンテントタイプを取得します。
	*/
	const Char* GetContentType() const ;

	/** \brief statusを取得します。
	*/
	U32 GetStatus() const ;

	/** \brief コンテントタイプを比較します。\n
	*   GetContentTypeを使う場合との違いは大文字小文字区別なく比較する事。
	*/
	Bool IsContentTypeSame(const Char* contentType) const ;

	/** \brief データのサイズを取得します。
   *         data が持っている終端 '\0' はsizeには含まれない。
	*/
	S32 GetDataSize() const ;

	/** \brief データを取得します。
   *         データの終端には '\0' が含まれる。
	*/
	const char* GetData() const ;

	void operator delete(void* p);

	THttpResponse response;
};

//#endif /*HTTP_USING_C_PLUS_PLUS*/

#ifdef __cplusplus
ADK_END_NAMESPACE
#endif

#endif /* __ADK_HTTP_RESPONSE_H__*/
