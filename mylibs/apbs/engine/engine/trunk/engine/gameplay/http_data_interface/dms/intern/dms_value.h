/**
 * \file dms_value.h
 * \ingroup adkgrp_dms
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief DMSで扱うスキーマの値クラス定義部
 */


#ifndef DMS_SCHEMA_VALUE_H_
#define DMS_SCHEMA_VALUE_H_


#include <core/types.h>
#include <gameplay/http_data_interface/dms/dms_schema_types.h>
#include <debug_profiling/error/result.h>
#include <core/memory_allocation/guarded_alloc.h>


ADK_BEGIN_NAMESPACE


/* class definition */
/**
 * \~english
 * \~japanese
 *   \brief DMSスキーマ値クラス
 */
class DMS_Value {
public:
  /* constructor & destructor */
  /**
   * \~english
   * \~japanese
   *   \brief コンストラクタ
   */
  DMS_Value();
  /**
   * \~english
   * \~japanese
   *   \brief デストラクタ
   */
  virtual ~DMS_Value();

  /* value management functionally */
  /**
   * \~english
   * \~japanese
   *   \brief Booleanを作成します
   *   \param[in] key キー
   */
  Result InitializeBool(const Char *key,U32 keylen,Bool b);
  /**
   * \~english
   * \~japanese
   *   \brief 文字列を作成します
   *   \param[in] key キー
   */
  Result InitializeString(const Char *key,U32 keylen,const Char *str,U32 len);
  /**
   * \~english
   * \~japanese
   *   \brief S32を作成します
   *   \param[in] key キー
   */
  Result InitializeS32(const Char *key,U32 keylen,S32 value);
  /**
   * \~english
   * \~japanese
   *   \brief U32を作成します
   *   \param[in] key キー
   */
  Result InitializeU32(const Char *key,U32 keylen,U32 value);
  /**
   * \~english
   * \~japanese
   *   \brief S64を作成します
   *   \param[in] key キー
   */
  Result InitializeS64(const Char *key,U32 keylen,S64 value);
  /**
   * \~english
   * \~japanese
   *   \brief U64を作成します
   *   \param[in] key キー
   */
  Result InitializeU64(const Char *key,U32 keylen,U64 value);
  /**
   * \~english
   * \~japanese
   *   \brief Doubleを作成します
   *   \param[in] key キー
   */
  Result InitializeDouble(const Char *key,U32 keylen,Double value);
  /**
   * \~english
   * \~japanese
   *   \brief 配列を初期化します
   *   \param[in] key キー
   *   \param[in] count 配列数
   *   \param[in] size 配列全体のサイズ
   */
  Result InitializeArray(const Char *key,U32 keylen,S32 count,U32 size);
  /**
   * \~english
   * \~japanese
   *   \brief オブジェクトを初期化します
   *   \param[in] key キー
   *   \param[in] count 配列数
   *   \param[in] size 配列全体のサイズ
   */
  Result InitializeObject(const Char *key,U32 keylen,U32 size);
  /**
   * \~english
   * \~japanese
   *   \brief 初期化された値を終了します
   *   \return 値の成功判定。以下の値が返ります
   *   \retval ResultSuccess      終了に成功したときに返ります
   *   \retval ResultNotInitialized  初期化処理が行われていないときに返ります
   */
  Result Terminate();

  /* value */
  /**
   * \~english
   * \~japanese
   *   \brief キーを返します
   *   \return 文字列
   */
  ADK_INLINE const Char* GetKey() const;
  /**
   * \~english
   * \~japanese
   *   \brief キーの文字列長を返します
   *   \return 文字列長
   */
  ADK_INLINE U32 GetKeyLength() const;
  /**
   * \~english
   * \~japanese
   *   \brief 値の数を返します
   */
  ADK_INLINE S32 GetCount() const;
  /**
   * \~english
   * \~japanese
   *   \brief 値のサイズを返します
   */
  ADK_INLINE U32 GetSize() const;
  /**
   * \~english
   * \~japanese
   *   \brief Booleanとして値を返します
   *   \return Bool
   */
  Bool GetBoolean() const;
  /**
   * \~english
   * \~japanese
   *   \brief 文字列として値を返します
   *   \return 文字列
   */
  const Char* GetString() const;
  /**
   * \~english
   * \~japanese
   *   \brief S32として値を返します
   *   \return S32
   */
  S32 GetS32() const;
  /**
   * \~english
   * \~japanese
   *   \brief U32として値を返します
   *   \return U32
   */
  U32 GetU32() const;
  /**
   * \~english
   * \~japanese
   *   \brief S64として値を返します
   *   \return S64
   */
  S64 GetS64() const;
  /**
   * \~english
   * \~japanese
   *   \brief U64として値を返します
   *   \return U64
   */
  U64 GetU64() const;
  /**
   * \~english
   * \~japanese
   *   \brief Doubleとして値を返します
   *   \return Double
   */
  Double GetDouble() const;
  /**
   * \~english
   * \~japanese
   *   \brief 配列として値を返します
   *   \return 配列
   */
  const void* GetArray() const;
  /**
   * \~english
   * \~japanese
   *   \brief オブジェクトとして値を返します
   *   \return オブジェクト
   */
  const void* GetObject() const;

private:
  /* vars */
  /**
   * \~english
   * \~japanese
   *   \brief 値タイプ
   */
  DMS_TValueType  type_;
  /**
   * \~english
   * \~japanese
   *   \brief キー
   */
  struct{
    const Char  *str;
    U32  len;
  }key_;
  /**
   * \~english
   * \~japanese
   *   \brief 値情報
   */
  struct {
    S32  count;
    U32  size;
    union {
      struct {
        S32  b;
        Byte  reserve[4];
      }b;
      union {
        struct {
          S32  i;
          Byte  reserve[4];
        }i;
        struct {
          U32  u;
          Byte  reserve[4];
        }u;
        S64  i64;
        U64  u64;
        Double  d;
      }n;
      struct {
        U64  addr;
      }s;
      struct {
        U64  addr;
      }a;
      struct {
        U64  addr;
      }o;
    }v;
  }value_;

  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("DMS_Value");
  ADK_NON_COPYABLE_CLASS(DMS_Value);
};
ADK_INLINE const Char* DMS_Value::GetKey() const {
  return key_.str;
}
ADK_INLINE U32 DMS_Value::GetKeyLength() const {
  return key_.len;
}
ADK_INLINE S32 DMS_Value::GetCount() const {
  return value_.count;
}
ADK_INLINE U32 DMS_Value::GetSize() const {
  return value_.size;
}


ADK_END_NAMESPACE


#endif  /* DMS_SCHEMA_VALUE_H_ */


