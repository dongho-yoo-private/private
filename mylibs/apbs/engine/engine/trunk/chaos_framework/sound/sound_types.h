//
//  sount_types.h
//  Chaos
//
//  Created by B04680 on 2014/02/06.
//
//

#ifndef Chaos_sount_types_h
#define Chaos_sount_types_h

//TODO.adk4app : ファイルパスなどはResourceManagerに持たせる
#ifdef ANDROID_CHAOS
#define DOWNLOAD_SOUND_INCLUDE_PATH CCFileUtils::sharedFileUtils()->getWritablePath() + "sound/"
#else
#define DOWNLOAD_SOUND_INCLUDE_PATH CCFileUtils::sharedFileUtils()->getWritablePath() + "Caches/sound/"
#endif



#define BGM_THEME "bgm_theme.mp3"
#define JINGLE_LEVEL_UP "jin_01_level_up.mp3"



#endif
