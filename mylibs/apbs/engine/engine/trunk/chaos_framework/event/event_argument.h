/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file event_argument.h
 * \ingroup event_group
 * \~english
 * \~japanese
 *  \brief イベントで使用する引数クラス定義部
 */

#ifndef EVENT_ARGUMENT_H_
#define EVENT_ARGUMENT_H_

#include <core/types.h>
#include <core/memory_allocation/guarded_alloc.h>
#include <core/debug/debug_assert.h>

/**
 * \~english
 * \~japanese
 *  \brief イベントが持つ引数値クラス
 */
class EventArgument {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief 引数タイプ
   */
  typedef enum {
    kUnknown = 0x0000,
    kBool = 0x0001,
    kTrue = 0x0002,
    kFalse = 0x0004,
    kNumber = 0x0008,
    kS32 = 0x0010,
    kU32 = 0x0020,
    kS64 = 0x0040,
    kU64 = 0x0080,
    kDouble = 0x0100,
    kString = 0x0200,
    kBinary = 0x4000
  }TType;

  /**
   * \~english
   * \~japanese
   *  \brief デフォルトコンストラクタ
   */
  EventArgument();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  ~EventArgument();

  /**
   * \~english
   * \~japanese
   *  \brief 引数情報を解放する
   */
  void Release();

  /**
   * \~english
   * \~japanese
   *  \brief キーの文字列長を返します
   *  \return 文字列長
   */
  ADK_INLINE size_t GetKeyLength() const;
  /**
   * \~english
   * \~japanese
   *  \brief キーの文字列を返します
   *  \return 文字列
   */
  ADK_INLINE const Char *GetKey() const;
  /**
   * \~english
   * \~japanese
   *  \brief S32値を設定する
   *  \param[in] key キー名
   *  \param[in] value S32
   */
  void SetS32(const Char *key,S32 value);
  /**
   * \~english
   * \~japanese
   *  \brief S32値を返します
   *  \return \ref S32
   */
  ADK_INLINE S32 GetS32() const;
  /**
   * \~english
   * \~japanese
   *  \brief U32値を設定する
   *  \param[in] key キー名
   *  \param[in] value U32
   */
  void SetU32(const Char *key,U32 value);
  /**
   * \~english
   * \~japanese
   *  \brief U32値を返します
   *  \return \ref U32
   */
  ADK_INLINE U32 GetU32() const;
  /**
   * \~english
   * \~japanese
   *  \brief S64値を設定する
   *  \param[in] key キー名
   *  \param[in] value S64
   */
  void SetS64(const Char *key,S64 value);
  /**
   * \~english
   * \~japanese
   *  \brief S64値を返します
   *  \return \ref S64
   */
  ADK_INLINE S64 GetS64() const;
  /**
   * \~english
   * \~japanese
   *  \brief U64値を設定する
   *  \param[in] key キー名
   *  \param[in] value U64
   */
  void SetU64(const Char *key,U64 value);
  /**
   * \~english
   * \~japanese
   *  \brief U64値を返します
   *  \return \ref U64
   */
  ADK_INLINE U64 GetU64() const;
  /**
   * \~english
   * \~japanese
   *  \brief Double値を設定する
   *  \param[in] key キー名
   *  \param[in] value Double
   */
  void SetDouble(const Char *key,Double value);
  /**
   * \~english
   * \~japanese
   *  \brief Double値を返します
   *  \return \ref Double
   */
  ADK_INLINE Double GetDouble() const;
  /**
   * \~english
   * \~japanese
   *  \brief 文字列を設定する
   *  \param[in] key キー名
   *  \param[in] value 文字列
   */
  void SetString(const Char *key,const Char *value);
  /**
   * \~english
   * \~japanese
   *  \brief 文字列長を返します
   *  \return 文字列長
   */
  ADK_INLINE size_t GetStringLength() const;
  /**
   * \~english
   * \~japanese
   *  \brief 文字列を返します
   *  \return 文字列
   */
  ADK_INLINE const Char *GetString() const;
  /**
   * \~english
   * \~japanese
   *  \brief Booleanを設定する
   *  \param[in] key キー名
   *  \param[in] value boolean
   */
  void SetBool(const Char *key,Bool value);
  /**
   * \~english
   * \~japanese
   *  \brief Boolean を返します
   *  \return boolean
   */
  ADK_INLINE Bool GetBool() const;
  /**
   * \~english
   * \~japanese
   *  \brief バイナリを設定する
   *  \param[in] key キー名
   *  \param[in] binary バイナリの先頭アドレス
   *  \param[in] size バイナリサイズ
   */
  void SetBinary(const Char *key,const void *binary,size_t size);
  /**
   * \~english
   * \~japanese
   *  \brief バイナリサイズを返します
   *  \return \ref size_t
   */
  ADK_INLINE size_t GetBinarySize() const;
  /**
   * \~english
   * \~japanese
   *  \brief バイナリを返します
   *  \return バイナリの先頭アドレス
   */
  ADK_INLINE const void* GetBinary() const;
 private:
  typedef struct {
    Char *str;
    U32 length;
  }TString;
  typedef union {
    struct {
      S32 i;
      Byte reserve[4];
    }i;
    struct {
      U32 u;
      Byte reserve[4];
    }u;
    S64 i64;
    U64 u64;
    Double d;
  }TNumber;
  typedef struct {
    Bool boolean;
    Byte reserve[4];
  }TBool;
  typedef struct {
    void *addr;
    U32 size;
  }TBinary;
  typedef union {
    TString s;
    TNumber n;
    TBool b;
    TBinary bin;
  }TValue;

  /**
   * \~english
   * \~japanese
   *  \brief キー名を設定する
   *  \param[in] name キー名
   */
  void SetKey(const Char *name);

  /**
   * \~english
   * \~japanese
   *  \brief キー名の最大長
   */
  static const S32 kKeyNameLength = 15;
  /**
   * \~english
   * \~japanese
   *  \brief 引数名
   */
  struct {
    Char name[kKeyNameLength+1];
    S32 length;
  }key_;
  /**
   * \~english
   * \~japanese
   *  \brief 値本体
   */
  TValue value_;
  /**
   * \~english
   * \~japanese
   *  \brief 値のタイプ
   */
  TType type_;

  /* Non-Copyable MixIn */
  ADK_NON_COPYABLE_CLASS(EventArgument);
  /* ADK Guardallocator funcation */
  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("EventArgument");
};
ADK_INLINE size_t EventArgument::GetKeyLength() const {
  return key_.length;
}
ADK_INLINE const Char *EventArgument::GetKey() const {
  return key_.name;
}
ADK_INLINE S32 EventArgument::GetS32() const {
  return value_.n.i.i;
}
ADK_INLINE U32 EventArgument::GetU32() const {
  return value_.n.u.u;
}
ADK_INLINE S64 EventArgument::GetS64() const {
  return value_.n.i64;
}
ADK_INLINE U64 EventArgument::GetU64() const {
  return value_.n.u64;
}
ADK_INLINE Double EventArgument::GetDouble() const {
  return value_.n.d;
}
ADK_INLINE size_t EventArgument::GetStringLength() const {
  return value_.s.length;
}
ADK_INLINE const Char *EventArgument::GetString() const {
  return value_.s.str;
}
ADK_INLINE Bool EventArgument::GetBool() const {
  return value_.b.boolean;
}
ADK_INLINE size_t EventArgument::GetBinarySize() const {
  return value_.bin.size;
}
ADK_INLINE const void* EventArgument::GetBinary() const {
  return value_.bin.addr;
}

#endif  /* EVENT_ARGUMENT_H_ */

