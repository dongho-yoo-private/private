/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file game_manager.h
 * \ingroup game_manager_group
 * \~english
 * \~japanese
 *  \brief ゲーム中のコンポーネント管理定義部
 */

#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include <core/types.h>

#include "application/application_manager.h"
#include "scene/system/scene/scene_types.h"

CHAOS_BEGIN_NAMESPACE

/* forward declaration */
class GameLoop;

/**
 * \~english
 * \~japanese
 *  \brief ゲーム中で使用するコンポーネントを管理し進行の手助けを行うマネージャ
 */
class GameManager {
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デフォルトコンストラクタ
   */
  GameManager();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  ~GameManager();

  /**
   * \~english
   * \~japanese
   *  \brief ゲームマネージャを初期化する
   *  \return 初期化処理の成功判定。以下の値が返ります
   *  \retval true 初期化処理に成功したときに返ります
   *  \retval false 初期化処理に成功したときに返ります
   */
  Bool Initialize();
  /**
   * \~english
   * \~japanese
   *  \brief ゲームマネージャを終了する
   *  \return 終了処理の成功判定。以下の値が返ります
   *  \retval true 終了処理に成功したときに返ります
   *  \retval false 終了処理に成功したときに返ります
   */
  Bool Terminate();

  /**
   * \~english
   * \~japanese
   *  \brief シーンの切り替えをリクエストする
   *  \param[in] scene_id 切り替えたい \ref TSceneId
   *  \param[in] binary 切り替え先のシーンへ送るバイナリデータ
   *  \param[in] binary_size バイナリデータサイズ
   *  \return リクエストの成功判定
   */
  Bool RequestEventReplaceScene(
      TSceneId scene_id,const void *binary,size_t binary_size);
  /**
   * \~english
   * \~japanese
   *  \brief データの更新が完了したときことをイベントとしてリクエストする
   *  \param[in] is_success 成功しているかどうかのフラグ
   *  \return リクエストの成功判定
   */
  Bool RequestEventDataUpdateCompleted(Bool is_success,const void *userdata);

 private:
  /**
   * \~english
   * \~japanese
   *  \brief データリクエストキュー
   */
  typedef struct TRequestDataQueue {
    TRequestDataQueue *next;
    TSceneId scene_id;
  }TRequestDataQueue;

  /**
   * \~english
   * \~japanese
   *  \brief シーン切り替えイベントを追加する
   *  \param[in] scene_id 切り替える \ref TSceneId
   *  \return 追加の成功判定。以下の値が返ります
   *  \retval true 追加に成功したときに返ります
   *  \retval false 追加に失敗したときに返ります
   */
  Bool AddEventReplaceScene(TSceneId scene_id);

  /**
   * \~english
   * \~japanese
   *  \brief ゲームループオブジェクト
   */
  GameLoop *game_loop_;
  /**
   * \~english
   * \~japanese
   *  \brief データリクエスト最大数
   */
  static const S32 kMaxDataRequest = 2;
  /**
   * \~english
   * \~japanese
   *  \brief データのリクエストに関する情報
   */
  struct {
    TRequestDataQueue queues[kMaxDataRequest];
    void *queue_list;
  }request_data_;

  /* Non-Copyable MixIn */
  ADK_NON_COPYABLE_CLASS(GameManager);
  /* ADK Guardallocator funcation */
  ADK_MEM_CXX_CLASS_ALLOC_FUNCS("GameManager");
};

CHAOS_END_NAMESPACE

#endif  /* GAME_MANAGER_H_ */

