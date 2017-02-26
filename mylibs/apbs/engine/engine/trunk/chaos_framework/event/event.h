/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file event.h
 * \ingroup event_group
 * \~english
 * \~japanese
 *  \brief アプリケーションで作成されるイベント定義部
 */

#ifndef EVENT_H_
#define EVENT_H_

#include <core/types.h>
#include <core/memory_allocation/guarded_alloc.h>
#include "application/application_types.h"
#include "event/event_argument.h"

CHAOS_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 *  \brief イベント1単位を管理するイベントオブジェクトクラス
 */
class Event {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief イベントタイプ
   */
  typedef enum {
    /**
     * \~english
     * \~japanese
     *  \brief unknown
     */
    kUnknown,
    /**
     * \~english
     * \~japanese
     *  \brief データの更新に成功した。
     */
    kSuccessfulUpdateData,
    /**
     * \~english
     * \~japanese
     *  \brief データの更新に失敗した。
     */
    kFailedUpdateData,
    /**
     * \~english
     * \~japanese
     *  \brief ADKへのAPI通信をリクエストする
     */
    kIsRequestCompleted,
    /**
     * \~english
     * \~japanese
     *  \brief 次のシーンへ切り替えるイベント
     */
    kNextScene
  }TType;

  /**
   * \~english
   * \~japanese
   *   \brief デフォルトコンストラクタ
   */
  Event();
  /**
   * \~english
   * \~japanese
   *   \brief デストラクタ
   */
  ~Event();

  /**
   * \~english
   * \~japanese
   *  \brief イベントが保持しているデータを削除します
   */
  void Release();
  /**
   * \~english
   * \~japanese
   *  \brief 自信のクローンを作成する
   *  \param[out] event クローン格納先のポインタ
   *  \return 作成の成功判定。以下の値が返ります
   *  \retval true 作成に成功したときに返ります
   *  \retval false 作成に失敗したときに返ります
   */
  bool Clone(Event *event);
  /**
   * \~english
   * \~japanese
   *  \brief イベントのタイプ \ref Event::TType を設定する
   *  \param[in] type \ref Event::TType
   */
  ADK_INLINE void SetType(TType type);
  /**
   * \~english
   * \~japanese
   *  \brief イベントのタイプ \ref Event::TType を返します
   *  \return \ref Event::TType
   */
  ADK_INLINE const TType GetType() const;
  /**
   * \~english
   * \~japanese
   *  \brief S32値を追加する
   *  \param[in] key キー名
   *  \param[in] value S32
   */
  void AddS32(const Char *key,S32 value);
  /**
   * \~english
   * \~japanese
   *  \brief U32値を追加する
   *  \param[in] key キー名
   *  \param[in] value U32
   */
  void AddU32(const Char *key,U32 value);
  /**
   * \~english
   * \~japanese
   *  \brief S64値を追加する
   *  \param[in] key キー名
   *  \param[in] value S64
   */
  void AddS64(const Char *key,S64 value);
  /**
   * \~english
   * \~japanese
   *  \brief U64値を追加する
   *  \param[in] key キー名
   *  \param[in] value U64
   */
  void AddU64(const Char *key,U64 value);
  /**
   * \~english
   * \~japanese
   *  \brief Double値を追加する
   *  \param[in] key キー名
   *  \param[in] value Double
   */
  void AddDouble(const Char *key,Double value);
  /**
   * \~english
   * \~japanese
   *  \brief 文字列を追加する
   *  \param[in] key キー名
   *  \param[in] value 文字列
   */
  void AddString(const Char *key,const Char *value);
  /**
   * \~english
   * \~japanese
   *  \brief Booleanを追加する
   *  \param[in] key キー名
   *  \param[in] value boolean
   */
  void AddBool(const Char *key,Bool value);
  /**
   * \~english
   * \~japanese
   *  \brief バイナリを追加する
   *  \param[in] key キー名
   *  \param[in] binary バイナリの先頭アドレス
   *  \param[in] size バイナリサイズ
   */
  void AddBinary(const Char *key,const void *binary,size_t size);
  /**
   * \~english
   * \~japanese
   *  \brief 設定されている引数の数を返します
   */
  ADK_INLINE S32 GetArgumentCount() const;
  /**
   * \~english
   * \~japanese
   *  \brief 引数のオブジェクトを返します
   *  \param[in] index 0 <= index <= \ref GetArgumentCount()
   *  \return \ref EventArgument
   */
  ADK_INLINE const EventArgument* GetArgument(S32 index) const;
  /**
   * \~english
   * \~japanese
   *  \brief キーから引数のオブジェクトを返します
   *  \param[in] key キー名
   *  \return \ref EventArgument
   */
  const EventArgument* KeyForArgument(const Char *key) const;

 private:
  /**
   * \~english
   * \~japanese
   *  \brief イベントが持てる引数の最大数
   */
  static const S32 kArgumentMax = 8;

  /**
   * \~english
   * \~japanese
   *  \brief 空いている引数オブジェクトを返します
   *  \return \ref Argument
   */
  EventArgument *EmptyArgument();

  /**
   * \~english
   * \~japanese
   *  \brief このイベントのタイプ \ref Event::TType
   */
  TType type_;
  /**
   * \~english
   * \~japanese
   *  \brief 現在の引数値オブジェクトの数
   */
  S32 argument_count_;
  /**
   * \~english
   * \~japanese
   *  \brief 現在の引数値オブジェクトの数
   */
  EventArgument arguments_[kArgumentMax];

  /* Non-Copyable MixIn */
  ADK_NON_COPYABLE_CLASS(Event);
  /* ADK Guardallocator funcation */
  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("Event");
};
ADK_INLINE void Event::SetType(TType type) {
  type_ = type;
}
ADK_INLINE const Event::TType Event::GetType() const {
  return type_;
}
ADK_INLINE S32 Event::GetArgumentCount() const {
  return argument_count_;
}
ADK_INLINE const EventArgument* Event::GetArgument(S32 index) const {
  ADK_ASSERT(index>=0&&index<GetArgumentCount());
  return &arguments_[index];
}

CHAOS_END_NAMESPACE

#endif  /* EVENT_H_ */

