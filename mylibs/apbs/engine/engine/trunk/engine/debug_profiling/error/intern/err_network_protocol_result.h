/**
 * \file err_network_protocol_result.h
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief ネットワークプロトコルリザルト定義部
 */

#ifndef ERR_NETOWROK_PROTOCOL_RESULT_H_
#define ERR_NETOWROK_PROTOCOL_RESULT_H_

#include <debug_profiling/error/result.h>

ADK_BEGIN_NAMESPACE

/* extern function implementation */
/**
 * \~english
 * \~japanese
 *   \brief ネットワークプロトコルのリザルトメッセージを返します
 *   \return メッセージ
 */
extern const Char *GetNetworkProtocolResultMessage(const Result &result);

ADK_END_NAMESPACE

#endif /* ERR_NETOWROK_PROTOCOL_RESULT_H_ */

