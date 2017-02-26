//
//  tdd_types.h
//  Chaos
//
//  Created by B04680 on 2014/02/03.
//
//

#ifndef Chaos_tdd_types_h
#define Chaos_tdd_types_h

#include "vector.h"
/**
 * \~english
 * \~japanese
 *   \brief vectorのメンバー数を数えるマクロ
 */
#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))
#include "cocos-ext.h"
#include "application/application_types.h"

//CCLOG("TRACE : [%s],%s",typeid(this).name(),__FUNCTION__);
#define LOG_TRACE CCLOG("TRACE : [%s],%s",typeid(this).name(),__FUNCTION__)
#define LOG_TRACE_PARAM(param) CCLOG("TRACE : [%s],%s param = %s",typeid(this).name(),__FUNCTION__,param)
#define LOG_TRACE_PARAM_INT(param) CCLOG("TRACE : [%s],%s param = %d",typeid(this).name(),__FUNCTION__,param)
/**
 * \~english
 * \~japanese
 *   \brief TDDで使う構造体、共通includeなどを定義
 */

//--------------------------------------------------
// struct def
//--------------------------------------------------
/**
 * \~english
 * \~japanese
 *   \brief Parts用の構造体
*/
struct TDDPartsData{
  const char* partsName;
  cocos2d::extension::CCLayerLoader *loader;
};
/**
 * \~english
 * \~japanese
 *   \brief LoadSceneに使用する構造体
 リストに表示する文字列、
 読み込むccbiファイル名、
 ccbi用のCCLayerLoader名、
 パーツの配列（ccbiファイル名、loader名）
 */
struct TDDSceneData{
  const char* listName;
  const char* sceneCcbiName;
  cocos2d::extension::CCLayerLoader *loader;
  std::vector<TDDPartsData> parts;
};
/**
 * \~english
 * \~japanese
 *   \brief Apiテストに使用する構造体
 */
struct TDDRequestData{
  const char* listName;
  const char* dataResoruce;
};
/**
 * \~english
 * \~japanese
 *   \brief GameFlowに使用する構造体、
 */
struct TDDGameFlowData{
  const char* listName;
};

#endif
