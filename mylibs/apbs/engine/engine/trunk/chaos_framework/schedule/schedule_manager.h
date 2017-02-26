//
//  schedule_manager.h
//  af-chaos_framework
//
//  Created by CJ0048 on 2014/04/09.
//  Copyright (c) 2014年 B04350. All rights reserved.
//

#ifndef __af_chaos_framework__schedule_manager__
#define __af_chaos_framework__schedule_manager__

#include <core/types.h>
#include "application/application_types.h"
#include "schedule/schedule_types.h"

CHAOS_BEGIN_NAMESPACE

class ScheduleManager {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デフォルトコンストラクタ
   */
  ScheduleManager();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~ScheduleManager();
  /**
   * \~english
   * \~japanese
   *  \brief 初期処理をします。
   */
  Bool Initialize();
  /**
   * \~english
   * \~japanese
   *  \brief 終了処理をします。
   */
  Bool Terminate();
  
  /**
   * \~english
   * \~japanese
   *  \brief スケジュールを登録します。
   *  \param interval 実行間隔(0の場合は１度だけ実行します)
   *  \param callback コールバック
   *  \param user_param 任意の値を設定するユーザーパラメータ
   *  \return 登録したスケジュールを識別するための値(識別ID)を返します。
   */
  S32 AddSchedule(F32 interval, TSchedullerCallback callback, void* user_param);
  
  /**
   * \~english
   * \~japanese
   *  \brief 一度だけ実行するスケジュールを登録します。
   *  \param interval 実行間隔
   *  \param callback コールバック
   *  \param user_param 任意の値を設定するユーザーパラメータ
   *  \return 登録したスケジュールを識別するための値(識別ID)を返します。
   */
  S32 AddScheduleOnce(F32 interval, TSchedullerCallback callback, void* user_param);
  
  /**
   * \~english
   * \~japanese
   *  \brief スケジュールを解除します。
   *  \param id スケジュール識別ID
   */
  void RemoveSchedule(S32 id);
  
  /**
   * \~english
   * \~japanese
   *  \brief 蓄えているスケジュールをディスパッチします。
   */
  void DispatchSchedule();
  
 private:
  /**
   * \~english
   * \~japanese
   *  スケジュールを登録する構造体
   */
  typedef struct TScheduleTable {
    /**
     * \~english
     * \~japanese
     *   利用中かどうか(true:利用中 false:利用していない)
     */
    Bool isUsed;
    /**
     * \~english
     * \~japanese
     *   スケジュール登録した時間
     */
    F32 added_time;
    /**
     * \~english
     * \~japanese
     *   登録されたスケジュールを実行する時間間隔(msec)
     */
    F32 time_interval;
    /**
     * \~english
     * \~japanese
     *   1回実行したらスケジュールを解除するかどうかのフラグ(true:１回だけ実行）
     */
    Bool isOnce;
    /**
     * \~english
     * \~japanese
     *   何回目の実行か（初期値1）
     */
    S32 times;
    /**
     * \~english
     * \~japanese
     *   コールバック関数
     */
    TSchedullerCallback callback;
    /**
     * \~english
     * \~japanese
     *   任意の値を設定する
     */
    void* user_param;
  } TScheduleTable;
  
  TScheduleTable schedule_list_[MAX_SCHEDULE_COUNT];
  
};


CHAOS_END_NAMESPACE

#endif /* defined(__af_chaos_framework__schedule_manager__) */
