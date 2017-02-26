//
//  sound_manager.cpp
//  Chaos
//
//  Created by B04680 on 2014/02/06.
//
//

#include "sound/sound_manager.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

CHAOS_BEGIN_NAMESPACE
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
// Constructor & Destructor START
SoundManager::SoundManager()
: playingBGMFileName_("")
, pauseFlag_(false){
  
  durationSoundEffectList_ = CCArray::create();
  durationSoundEffectList_->retain();
}

SoundManager::~SoundManager(){
  CC_SAFE_RELEASE(scheduler_);
  CC_SAFE_RELEASE(durationSoundEffectList_);
}
// END
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥

void SoundManager::playBGM(const char* fileName, bool loopFlag){
  CCLOG("%s fileName = %s loopFlag = %c",__FUNCTION__, fileName, loopFlag);
  bool settingUserPlayBGM = true;  //TODO.adk4app : ユーザー設定のBGMを判断する
  if(settingUserPlayBGM){
    if(strcmp(playingBGMFileName_, fileName))
    {//前に再生していたファイルならそのまま
      const char* getFilePathAndName = fileName; //TODO.adk4app : ResourceManagerでファイルパスを取得する
      SimpleAudioEngine::sharedEngine()->playBackgroundMusic(getFilePathAndName, loopFlag);
      playingBGMFileName_ = fileName;
    }//if equal with play BGM
    else if(pauseFlag_)
    {
      this->resumeBGM();
    }
  }//endif userBGMSetting
}
void SoundManager::playJingle(const char* fileName){
  CCLOG("%s fileName = %s ",__FUNCTION__, fileName);
  bool settingUserPlayJingle = true;//TODO.adk4app : ユーザー設定のJingleを判断する
  if(settingUserPlayJingle){
    const char* getFilePathAndName = fileName; //TODO.adk4app : ResourceManagerでファイルパスを取得する
    SimpleAudioEngine::sharedEngine()->playEffect(getFilePathAndName);
  }
}
void SoundManager::playSoundEffect(const char *fileName, bool loopFlag){
  CCLOG("%s fileName = %s, loopFlag = %c", fileName, loopFlag);
  bool settingUserPlaySoundEffect = true;//TODO.adk4app : ユーザー設定のSoundEffectを判断する
  if(settingUserPlaySoundEffect){
    const char* getFilePathAndName = fileName; //TODO.adk4app : ResourceManagerでファイルパスを取得する
    SimpleAudioEngine::sharedEngine()->playEffect(getFilePathAndName, loopFlag);
  }
}
//notice : 現状SoundTestでしか使用されてないー削除対象
void SoundManager::playSoundEffectWithAddDuration(const char *fileName, float addDuration){
  CCLOG("%s fileName = %s, addDuration = %f", fileName, addDuration);
  bool settingUserPlaySoundEffect = true;//TODO.adk4app : ユーザー設定のSoundEffectを判断する
  if(settingUserPlaySoundEffect){
//    const char* getFilePathAndName = fileName; //TODO.adk4app : ResourceManagerでファイルパスを取得する
//    addDuration += SimpleAudioEngine::sharedEngine()->getSouncDuration(fileName);
//    SimpleAudioEngine::sharedEngine()->
    //TODO.adk4app : サウンドファイルの再生時間を取得するためにcocos2dxのフレームワークを修正しているため、中断
  }
}

void SoundManager::pauseBGM(){
  CCLOG("trace %s",__FUNCTION__);
  pauseFlag_ = true;
  SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void SoundManager::resumeBGM()
{
  CCLOG("trace %s",__FUNCTION__);
  pauseFlag_ = false;
  SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
//duration パラメータが使われてないー削除対象
void SoundManager::resumeBGMForSchedule(float duration){
  CCLOG("trace %s duration = %f",__FUNCTION__,duration);
  pauseFlag_ = false;
}
void SoundManager::stopBGM(){
  CCLOG("trace %s",__FUNCTION__);
  playingBGMFileName_ = "null";
  SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}
void SoundManager::stopSoundEffect(){
  CCLOG("trace %s",__FUNCTION__);
  SimpleAudioEngine::sharedEngine()->stopAllEffects();
}
void SoundManager::playSoundEffectDelay(const char *fileName, float delayTime){
  bool settingUserPlaySoundEffect = true;//TODO.adk4app : ユーザー設定のSoundEffectを判断する
  if(settingUserPlaySoundEffect){
    CCDictionary *dic = CCDictionary::create();
    dic->setObject(CCString::create(fileName), SOUND_MANAGER_FILENAME);
    dic->setObject(CCFloat::create(delayTime), SOUND_MANAGER_DELAY_TILE);
    durationSoundEffectList_->addObject(dic);
    if(durationSoundEffectList_->count() == 1){
      scheduler_->scheduleSelector(schedule_selector(SoundManager::updateSoundEffectDuration), this, 0.0f, kCCRepeatForever, 0.0f, false);
    }
  }
}
//notice : durationは使われてない
void SoundManager::playFinish(float duration){
  CCLOG("trace %s duration = %f pSoundManagerDelegate_ = %c",__FUNCTION__,duration, pSoundManagerDelegate_);
  if(pSoundManagerDelegate_)
    pSoundManagerDelegate_->finishSoundWithFileName(playingSoundEffectFileName_);
}
//TODO.adk4app : 動作確認
void SoundManager::updateSoundEffectDuration(float duration){
  
  CCDictionary *dic = NULL;
  CCArray *removeList = CCArray::create();
  int index;
  int max = durationSoundEffectList_->count();
  for (index = 0; index < max; ++index) {
    dic = (CCDictionary*)durationSoundEffectList_->objectAtIndex(index);
    //    dumpCCDictionary TODO.adk4app ログツールに機能として追加
    CCFloat *delayTime = (CCFloat *)dic->objectForKey(SOUND_MANAGER_DELAY_TILE);
    if(duration >= delayTime->getValue())
    {
      CCString *fileName = (CCString*)dic->objectForKey(SOUND_MANAGER_FILENAME);
      this->playSoundEffect(fileName->getCString());
      removeList->addObject(dic);
      if(durationSoundEffectList_->count() < 1)
      {
        scheduler_->unscheduleAllForTarget(this);
      }
    }else
    {
      const float tempDelayTime = delayTime->getValue();
      dic->removeObjectForKey(SOUND_MANAGER_DELAY_TILE);
      dic->setObject(CCFloat::create(tempDelayTime - duration), SOUND_MANAGER_DELAY_TILE);
    }
  }//end of for
  CCDictionary *dic2 = NULL;
  max = removeList->count();
  for (index = 0; index < max; ++index) {
    dic2 = (CCDictionary*)removeList->objectAtIndex(index);
    durationSoundEffectList_->removeObject(dic2);
  }
}
//notice : mission bingoで使用
void SoundManager::removeDelayList(){
  durationSoundEffectList_->removeAllObjects();
  scheduler_->unscheduleAllForTarget(this);
}




CHAOS_END_NAMESPACE