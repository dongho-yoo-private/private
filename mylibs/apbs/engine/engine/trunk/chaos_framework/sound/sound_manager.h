//
//  sound_manager.h
//  Chaos
//
//  Created by B04680 on 2014/02/06.
//
//

#ifndef __Chaos__sound_manager__
#define __Chaos__sound_manager__

#include "application/application_types.h"

#include "sound/sound_types.h"
#include <cocos2d.h>

#define SOUND_MANAGER_FILENAME "fileName"
#define SOUND_MANAGER_DELAY_TILE "delayTime"

USING_NS_CC;

CHAOS_BEGIN_NAMESPACE
/**
 * \brief sound_managerでのファイル再生完了イベントを取得するデリゲート
 */
class SoundManagerDelegate{
public:
  virtual void finishSoundWithFileName(const char* fileName) = 0;
};
/**
 * \brief BGM（ループされる背景音）, SE(SoundEffect), Jingle(画面導入時の１回だけの音)\
 *　などの再生を管理するクラスapplication_managerクラスが持ちます。
 */
class SoundManager : public CCObject{
private:
  /**
   * \brief SoundManagerDelegate用変数
   */
  SoundManagerDelegate *pSoundManagerDelegate_;
  /**
   * \brief 再生中のBGMファイル名です。
   */
  const char* playingBGMFileName_;
  /**
   * \brief 再生中のSEファイル名です。
   */
  const char* playingSoundEffectFileName_;
  /** TODO.apk4app : 使い方の確認
   * \brief SEの再生時間リストを持つ変数
   */
  CCArray *durationSoundEffectList_;
  /** TODO.apk4app : 使い方の確認
   * \brief スケジューラー
   */
  CCScheduler *scheduler_;
  /** TODO.apk4app : 使い方の確認
   * \brief Pauseフラグ
   */
  bool pauseFlag_;
  /** TODO.apk4app : 使い方の確認
   * \brief SEのアップデートフラグ
   */
  void updateSoundEffectDuration(float duration);
public:
  /**
   * \brief Constructor & Destructor
   */
  SoundManager();
  virtual ~SoundManager();
  /**
   * \brief SoundManagerDelegateのミュテーター
   */
  void setSoundManagerDelegate(SoundManagerDelegate *delegate){
    pSoundManagerDelegate_ = delegate;
  }
  /**
   * \brief BGMを再生します。
   * \param fileName ファイル名
   * \param loopFlag ループフラグ（選択）
   */
  void playBGM(const char* fileName, bool loopFlag = true);
  /**
   * \brief Jingleを再生します。
   * \param fileName ファイル名
   */
  void playJingle(const char* fileName);
  /**
   * \brief SEを再生します。
   * \param fileName ファイル名
   * \param loopFlag ループフラグ（選択）
   */
  void playSoundEffect(const char* fileName, bool loopFlag = true);
  /** TODO.adk4app : 実装確認
   * \brief SEを再生時間を指定して再生します。
   * \param fileName ファイル名
   * \param addDuration 再生時間
   */
  void playSoundEffectWithAddDuration(const char* filename, float addDuration);
  
  void pauseBGM();
  void resumeBGM();
  void resumeBGMForSchedule(float duration);
  
  void stopBGM();
  void stopSoundEffect();
  
  void playFinish(float duration);
  
  void playSoundEffectDelay(const char* fileName, float delayTime);
  
  void removeDelayList();
  
};

CHAOS_END_NAMESPACE
#endif /* defined(__Chaos__sound_manager__) */
