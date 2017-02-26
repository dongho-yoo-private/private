/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file event_system.h
 * \ingroup event_group
 * \~english
 * \~japanese
 *  \brief アプリケーション扱うイベントの操作を行うシステム定義部
 */

#ifndef EVENT_SYSTEM_H_
#define EVENT_SYSTEM_H_

#include "application/application_types.h"
#include "event/event.h"

CHAOS_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 *  \brief アプリケーション実行中に発生するイベントの操作/管理を行うクラス
 */
class EventSystem {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デフォルトコンストラクタ
   */
  EventSystem();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  ~EventSystem();

  /**
   * \~english
   * \~japanese
   *  \brief イベントシステムの初期化を行います
   *  \return 初期化の成功判定。以下の値が返ります
   *  \retval true 初期化に成功したときに返ります
   *  \retval false 初期化に失敗したときに返ります
   */
  bool Initialize();
  /**
   * \~english
   * \~japanese
   *  \brief イベントシステムの終了を行います
   *  \return 終了の成功判定。以下の値が返ります
   *  \retval true 終了に成功したときに返ります
   *  \retval false 終了に失敗したときに返ります
   */
  bool Terminate();

  /**
   * \~english
   * \~japanese
   *  \brief 蓄えているイベントをディスパッチする
   */
  void DispatchEvent();
  /**
   * \~english
   * \~japanese
   *  \brief 新しいイベントを作成して返します
   *  \return 作成の成功判定。以下の値が返ります
   *  \retval \ref Event 作成に成功したときに返ります
   *  \retval null 作成に失敗したときに返ります
   */
  Event* CreateEvent();
  /**
   * \~english
   * \~japanese
   *  \brief イベントを追加する
   *  \param[in] event \ref Event
   */
  void AddEvent(Event *event);
 private:
  /**
   * \~english
   * \~japanese
   *  \brief イベントオブジェクト
   */
  typedef struct TEvent {
    TEvent *next;
    Event event;
  }TEvent;
  /**
   * \~english
   * \~japanese
   *  \brief 該当する \ref TEvent オブジェクトを探します
   */
  TEvent* FoundEvent(Event *event);

  /**
   * \~english
   * \~japanese
   *  \brief イベントの最大数
   */
  static const S32 kEventMax = 4;
  /**
   * \~english
   * \~japanese
   *  \brief イベント群
   */
  TEvent events_[kEventMax];
  /**
   * \~english
   * \~japanese
   *  \brief 空いているイベントリスト
   */
  void *free_event_list_;
  /**
   * \~english
   * \~japanese
   *  \brief 実行すべきイベントリスト
   */
  void *do_event_list_;

  /* Non-Copyable MixIn */
  ADK_NON_COPYABLE_CLASS(EventSystem);
  /* ADK Guardallocator funcation */
  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("EventSystem");
};

CHAOS_END_NAMESPACE

#endif  /* EVENT_SYSTEM_H_ */

