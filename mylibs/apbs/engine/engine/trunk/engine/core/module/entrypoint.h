/**
 * \file entrypoint.h
 * \ingroup grp_adk
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief スタートアップ&シャットダウンモジュール定義群
 */

#ifndef ENTRYPOINT_H_
#define ENTRYPOINT_H_

#include <core/types.h>

/**
 * \~english
 * \~japanese
 *  \brief アプリケーションを初期化するタイミングで呼び出される関数
 *  \return 初期化の成功判定。以下の値を返します
 *  \retval true 初期化に成功したときに返します
 *  \retval false 初期化に失敗したときに返します
 */
extern Bool ADK_InitializeApplication();
/**
 * \~english
 * \~japanese
 *  \brief アプリケーション初期化後に呼び出されるメイン関数エントリーポイント\n
 *  \return 処理の成功判定。以下の値を返します
 *  \retval true 初期化に成功したときに返します
 *  \retval false 初期化に失敗したときに返します
 */
extern Bool ADK_Main();

#endif  /* ENTRYPOINT_H_ */

