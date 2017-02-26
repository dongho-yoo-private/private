/**
 * \file result_object.h
 * \ingroup adkgrp_error
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief リザルトオブジェクト
 */

#ifndef ERROR_RESULT_OBJECT_H_
#define ERROR_RESULT_OBJECT_H_

#include <core/types.h>
#include <core/memory_allocation/guarded_alloc.h>

ADK_BEGIN_NAMESPACE

/* class definition */
/**
 * \~english
 * \~japanese
 *  \brief エラーリザルトクラス
 */
class Result {
 public:
  /* constructor & destructor */
  /**
   * \~english
   * \~japanese
   *  \brief デフォルトコンストラクタ
   */
  Result();
  /**
   * \~english
   * \~japanese
   *  \brief パラメータ設定コンストラクタ
   *  \param[in] failure 成功か失敗かを設定\n
   *                      'false'なら成功\n
   *                      'true'なら失敗
   *  \param[in] result リザルト値
   *  \param[in] fatal 致命的なエラー\n
   *                    'false'なら致命的ではないエラー\n
   *                    'true'なら致命的なエラー
   *  \param[in] programError プログラムエラー\n
   *                    'false'ならプログラムエラー\n
   *                    'true'ならプログラムエラー
   */
  Result(Bool failure, U32 result, Bool fatal, Bool programError);
  /**
   * \~english
   * \~japanese
   *  \brief パラメータ設定コンストラクタ
   *  \param[in] fatal 致命的なエラー\n
   *                    'false'なら致命的ではないエラー\n
   *                    'true'なら致命的なエラー
   *  \param[in] programError プログラムエラー\n
   *                    'false'ならプログラムエラー\n
   *                    'true'ならプログラムエラー
   *  \param[in] component コンポーネント \ref TResultComponent の値を設定
   *  \param[in] func 呼び出し関数ID
   *  \param[in] code コード
   */
  Result(const Result &result, Bool fatal, Bool programError, U32 component,
         U32 func, U32 code);
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~Result();

  /* result management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief 成功しているかどうかを返します
   *  \return 成功判定。以下の値が返ります
   *  \retval true  成功している場合に返ります
   *  \retval false  失敗している場合に返ります
   */
  ADK_INLINE Bool IsSuccess() const;
  /**
   * \~english
   * \~japanese
   *  \brief 失敗しているかどうかを返します
   *  \return 失敗判定。以下の値が返ります
   *  \retval true  失敗している場合に返ります
   *  \retval false  成功している場合に返ります
   */
  ADK_INLINE Bool IsFailure() const;
  /**
   * \~english
   * \~japanese
   *  \brief 既存のリザルト値を返します
   *  \return リザルト値
   */
  ADK_INLINE U32 GetResult() const;
  /**
   * \~english
   * \~japanese
   *  \brief 致命的なエラーが発生しているかを返します
   *  \return エラー判定。以下の値が返ります
   *  \retval true  致命的なエラーが発生している場合に返ります
   *  \retval false  致命的なエラーが発生していない場合に返ります
   */
  ADK_INLINE Bool IsFatal() const;
  /**
   * \~english
   * \~japanese
   *  \brief プログラム実装上で修正すべきエラーかどうかを返します
   *  \return エラー判定。以下の値が返ります
   *  \retval true  プログラム実装上で修正が必要なエラーのときに返ります
   *  \retval false  プログラム実装上で修正が必要ないエラーのときに返ります
   */
  ADK_INLINE Bool IsProgramError() const;
  /**
   * \~english
   * \~japanese
   *  \brief コンポーネントを返します
   *  \return \ref TResultComponent
   */
  ADK_INLINE U32 GetComponent() const;
  /**
   * \~english
   * \~japanese
   *  \brief 関数IDを返します
   *  \return ID
   */
  ADK_INLINE U32 GetFunc() const;
  /**
   * \~english
   * \~japanese
   *  \brief エラーコードを返します
   *  \return コード
   */
  ADK_INLINE U32 GetCode() const;
  /**
   * \~english
   * \~japanese
   *  \brief 比較用の値を返します
   *  \return 値
   */
  ADK_INLINE U32 GetCompareValue() const;

  /* output management functionally */
  /**
   * \~english
   * \~japanese
   *  \brief リザルトメッセージを取得します
   *  \param[out] dst メッセージ出力先
   *  \param[in] dstlen 出力先の最大長
   */
  void GetMessage(Char *dst, U32 dstlen) const;

 private:
  /* vars */
  /**
   * \~english
   * \~japanese
   *  \brief コード
   */
  union {
    struct {
      /**
       * \~english
       * \~japanese
       *  \brief 失敗判定(0~1)
       */
      U32 failure : 1;
      /**
       * \~english
       * \~japanese
       *  \brief 致命的エラー判定(0~1)
       */
      U32 fatal : 1;
      /**
       * \~english
       * \~japanese
       *  \brief プログラムエラー判定(0~1)
       */
      U32 program : 1;
      /**
       * \~english
       * \~japanese
       *  \brief エラーコード(0~0x1f)
       */
      U32 code : 5;
      /**
       * \~english
       * \~japanese
       *  \brief 関数ID(0~0xff)
       */
      U32 func : 8;
      /**
       * \~english
       * \~japanese
       *  \brief 既存のリザルト値(0~0xff)
       */
      U32 result : 8;
      /**
       * \~english
       * \~japanese
       *  \brief コンポーネント(0~0xff)
       */
      U32 component : 8;
    } bit;
    /**
     * \~english
     * \~japanese
     *  \brief 値全体
     */
    U32 value;
  } code_;

  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("Result");
};

/* class inline impelementation */
ADK_INLINE Bool Result::IsSuccess() const { return !code_.bit.failure; }
ADK_INLINE Bool Result::IsFailure() const { return code_.bit.failure; }
ADK_INLINE U32 Result::GetResult() const { return code_.bit.result; }
ADK_INLINE Bool Result::IsFatal() const { return code_.bit.fatal; }
ADK_INLINE Bool Result::IsProgramError() const { return code_.bit.program; }
ADK_INLINE U32 Result::GetComponent() const { return code_.bit.component; }
ADK_INLINE U32 Result::GetFunc() const { return code_.bit.func; }
ADK_INLINE U32 Result::GetCode() const { return code_.bit.code; }
ADK_INLINE U32 Result::GetCompareValue() const { return code_.value; }

/* result operator implementation */
/**
 * \~english
 * \~japanese
 *  \brief 渡された値と等しいかを返します
 *  \retval true  等しいときに返ります
 *  \retval false  異なるときに返ります
 */
inline Bool operator==(const Result &result1, const Result &result2) {
  return result1.GetCompareValue() == result2.GetCompareValue();
}
/**
 * \~english
 * \~japanese
 *  \brief 渡された値と異なるかを返します
 *  \retval true  異なるときに返ります
 *  \retval false  等しいときに返ります
 */
inline Bool operator!=(const Result &result1, const Result &result2) {
  return result1.GetCompareValue() != result2.GetCompareValue();
}
/**
 * \~english
 * \~japanese
 *  \brief 渡された値と等しいかを返します
 *  \retval true  等しいときに返ります
 *  \retval false  異なるときに返ります
 */
inline Bool operator==(const Result &result, U32 value) {
  return result.GetCompareValue() == value;
}
/**
 * \~english
 * \~japanese
 *  \brief 渡された値と等しいかを返します
 *  \retval true  等しいときに返ります
 *  \retval false  異なるときに返ります
 */
inline Bool operator!=(const Result &result, U32 value) {
  return result.GetCompareValue() != value;
}
/**
 * \~english
 * \~japanese
 *  \brief 渡された値と等しいかを返します
 *  \retval true  等しいときに返ります
 *  \retval false  異なるときに返ります
 */
inline Bool operator==(U32 value, const Result &result) {
  return value == result.GetCompareValue();
}
/**
 * \~english
 * \~japanese
 *  \brief 渡された値と等しいかを返します
 *  \retval true  等しいときに返ります
 *  \retval false  異なるときに返ります
 */
inline Bool operator!=(U32 value, const Result &result) {
  return value != result.GetCompareValue();
}

/* macro definition */
/**
 * \~english
 * \~japanese
 *  \brief 既存のリザルト定義用マクロ
 */
#define ADK_EXSITING_RESULT(name, result, failure, fatal) \
  const Result name(failure, result, fatal, false)

/**
 * \~english
 * \~japanese
 *  \brief 既存のリザルト定義用マクロ
 */
#define ADK_RESULT(result, fatal, programError, component, func, code) \
  Result(result, fatal, programError, component, func, code)

/**
 * \~english
 * \~japanese
 *  \brief 既存のリザルト定義用マクロ for プログラムエラー\n
 */
#define ADK_RESULT_PROGRAM(result, component, func, code) \
  Result(result, true, true, component, func, code)

/**
 * \~english
 * \~japanese
 *  \brief 既存のリザルト定義用マクロ for Fatalエラー\n
 */
#define ADK_RESULT_FATAL(result, component, func, code) \
  Result(result, true, false, component, func, code)

ADK_END_NAMESPACE

#endif /* ERROR_RESULT_H_ */

