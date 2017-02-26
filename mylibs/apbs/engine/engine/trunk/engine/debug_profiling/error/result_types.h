/**
 * \file result_types.h
 * \ingruop adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief エラーリザルト定義
 *   \page adkpage_result リザルトの使い方
 *   \tableofcontents
 *   \section adksct_beginning 1.はじめに
 *     ADKで発生したエラー情報や処理結果(成功/失敗)の判定には \ref Result を使用します。\n
 *   このページでは、その \ref Result の使い方について記載しています。
 *   \section adksct_judge 2.成功失敗の判定方法
 *     成功か失敗かを簡単に判定する方法として
\code
if (result.IsSuccess()==true)
{
// 成功したときの処理を行う
}

if (result.IsFailure()==true)
{
// 失敗したときの処理を行う
}
\endcode
 *     というように \ref IsSuccess / \ref IsFailure メソッドを参照することで判定することができます。
 *   \section adksct_fatal 3.進行上、致命的なエラーが発生したか判定
 *     ADKの処理が進行する上で致命的なエラーが発生した場合に判定する方法を以下に記載します
\code
if (reuslt.IsFatal()==true)
{
// 致命的なエラーが発生したときの処理を行う
}
\endcode
 *     このエラーは例えばシステムリソースが不足しており、これ以上進行不能なときなどに発生します。\n
 *     発生した場合は直ちに \ref Terminate() を呼び出し ADK の使用を中止する必要があります。
 *   \section adksct_program 4.プログラムエラーが発生したか判定
 *     プログラムエラーとは、関数やメソッドに不正な引数を設定した場合など実装上でプログラマが修正する必要のあるエラーを示します。
\code
if (reuslt.IsProgramError()==true)
{
// プログラムエラーが発生したときの処理を行う
}
\endcode
 *   \section adksct_info 5.より詳細なエラー情報を取得する
 *     例えば
\code
void AnyFunc()
{
  adk::Result  result = adk::Terminate();

  // 初期化されていないのに呼び出したか判断する
  if(result==adk::ResultNotInitialized)
  {
    // 初期化されていなかったときの処理を行う
  }
}
\endcode
 *     といったように、あらかじめ定義されたリザルトと比較する事で、より詳細なリザルトを判断する事ができます。\n
 *     以下リザルト一覧になります。
 *     | 定義 | 概要 |
 *     | :--- | :--- |
 *     | ResultSuccess | 成功したときに返ります |
 *     | ResultNotInitialized | 初期化処理が行われていないことを示すリザルト |
 *     | ResultDoAlready | 既に実行された処理を再度呼び出されたことを示すリザルト |
 *     | ResultInvalidVariable | 不正な引数が設定されたことを示すリザルト |
 *     | ResultOutOfRange | 引数に設定された値が、指定できる範囲を超えていることを示すリザルト |
 *     | ResultOutOfResource | システムリソースが不足していることを示すリザルト |
 *     | ResultConnectionError | 通信処理に失敗したことを示すリザルト |
 *     | ResultConnectionTimeout | 通信処理がタイムアウトしたことを示すリザルト |
 *     | ResultNotFileExist | ファイルが存在しなかったことを示すリザルト |
 *     | ResultApplicationError | アプリケーションエラーが発生したことを示すリザルト\n このリザルトを取得した場合 \ref Result::GetCode() にエラーの詳細情報 \ref TApplicationError が含まれている |
 *     | ResultUnknown | 不明なエラーが発生したことを示すリザルト |
 */

#ifndef ERROR_RESULT_TYPES_H_
#define ERROR_RESULT_TYPES_H_

#include <debug_profiling/error/result_object.h>

ADK_BEGIN_NAMESPACE

/* component definition */
/**
 * \~english
 * \~japanese
 *   \brief コンポーネント定義
 */
typedef enum{
  kNoneComponent = 0,    /* 0x00 */
  kApiComponent,       /* 0x01 */
  kLibraryComponent,     /* 0x02 */
  kSystemComponent,     /* 0x03 */
  kErrorSystemComponent,   /* 0x04 */
  kLogComponent,       /* 0x05 */
  kStartupInfoComponent,   /* 0x06 */
  kNetworkProtocolComponent, /* 0x07 */
  kHostListComponent,     /* 0x08 */
  kRequestComponent,     /* 0x09 */
  kDmsComponent,       /* 0x0a */
  kPhaseComponent,     /* 0x0b */

  kMaxComponent
}TComponent;


/* exsiting result definition */
/**
 * \~english
 * \~japanese
 *   \brief 成功を示すリザルト
 */
ADK_EXSITING_RESULT(ResultSuccess,0x0,false,false);
/**
 * \~english
 * \~japanese
 *   \brief 初期化処理が行われていないことを示すリザルト
 */
ADK_EXSITING_RESULT(ResultNotInitialized,0x1,true,false);
/**
 * \~english
 * \~japanese
 *   \brief 既に実行された処理を再度呼び出されたことを示すリザルト
 */
ADK_EXSITING_RESULT(ResultDoAlready,0x2,true,false);
/**
 * \~english
 * \~japanese
 *   \brief 不正な引数が設定されたことを示すリザルト
 */
ADK_EXSITING_RESULT(ResultInvalidVariable,0x3,true,false);
/**
 * \~english
 * \~japanese
 *   \brief 引数に設定された値が、指定できる範囲を超えていることを示すリザルト
 */
ADK_EXSITING_RESULT(ResultOutOfRange,0x4,true,false);
/**
 * \~english
 * \~japanese
 *   \brief システムリソースが不足していることを示すリザルト
 */
ADK_EXSITING_RESULT(ResultOutOfResource,0x5,true,false);
/**
 * \~english
 * \~japanese
 *   \brief 通信処理に失敗したことを示すリザルト
 */
ADK_EXSITING_RESULT(ResultConnectionError,0x6,true,false);
/**
 * \~english
 * \~japanese
 *   \brief 通信処理がタイムアウトしたことを示すリザルト
 */
ADK_EXSITING_RESULT(ResultConnectionTimeout,0x7,true,false);
/**
 * \~english
 * \~japanese
 *   \brief ファイルが存在しなかったことを示すリザルト
 */
ADK_EXSITING_RESULT(ResultNotFileExist,0x8,true,false);
/**
 * \~english
 * \~japanese
 *   \brief アプリケーションエラーが発生したことを示すリザルト\n
 *          このリザルトを取得した場合 \ref Result::GetCode() にエラーの詳細情報 \ref TApplicationError が含まれている
 */
ADK_EXSITING_RESULT(ResultApplicationError,0x9,true,false);
/**
 * \~english
 * \~japanese
 *   \brief 不明なエラーが発生したことを示すリザルト
 */
ADK_EXSITING_RESULT(ResultUnknown,0xf,true,false);

ADK_END_NAMESPACE

#endif  /* ERROR_RESULT_TYPE_H_ */

