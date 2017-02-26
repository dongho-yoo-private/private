#ifndef __ADK_INP_RESPONSE_H__
#define __ADK_INP_RESPONSE_H__

#include <core/types.h>

ADK_BEGIN_NAMESPACE

/** @brief
*/
class INpResponse
{
public:
	/** @brief 
	*/
	virtual S32 GetResult() = 0 ;

	/** @brief
	*/
	virtual U32 GetDataSize() = 0 ;

	/** @brief
	*/
	virtual const void* GetData() = 0 ;

};

ADK_END_NAMESPACE

#endif
