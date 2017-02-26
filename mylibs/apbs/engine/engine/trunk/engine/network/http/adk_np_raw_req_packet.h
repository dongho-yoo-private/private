#ifndef __HTTP_REQ_PACKET_H__
#define __HTTP_REQ_PACKET_H__

#include <adk_types.h>
class NpRawReqPacket : public IRequestPacket
{
public:
	/** \breif constructor for GET method. 
	*
	* \param header
	* \param headerSize
	* \param body
	* \param bodySize
	*/
	NpRawReqPacket(const void* header, S32 headerSize,
					void* body, S32 bodySize);

	/** \breif constructor for POST method. 
	*
	* \param packet
	* \param packetSize
	*/
	NpRawReqPacket(const void* packet, S32 packetSize);

	virtual ~HttpReqPacket ();

	/** \breif constructor for GET method. 
	*
	* \param packet
	* \param packetSize
	*/
	void Set(const void* packet, S32 packetSize);

	/** \breif Set for POST method. 
	*
	* \param header
	* \param headerSize
	* \param body
	* \param bodySize
	*/
	void Set(const void* header, S32 headerSize,
					void* body, S32 bodySize);
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
	const U32 GetHeaderSize();

	/* \brief データを取得します。
	*
	* \return データのアドレス
	*/
	const void* GetData();

	/* \brief データサイズを取得します。
	*
	* \return データのサイズ
	*/
	const U32 GetDataSize();
private:
	void* mHeader;
	U32 mHeaderSize;
	void* mData;
	U32 mDataSize;
};

#endif
