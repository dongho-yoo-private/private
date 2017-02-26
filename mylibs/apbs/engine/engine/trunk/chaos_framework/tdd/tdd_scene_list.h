//
//  tdd_scene_list.h
//  Chaos
//
//  Created by B04680 on 2014/02/03.
//
//

#ifndef __Chaos__tdd_scene_list__
#define __Chaos__tdd_scene_list__

#include "tdd/tdd_types.h"
CHAOS_BEGIN_NAMESPACE
/**
 * \~english
 * \~japanese
 * \brief Apiリストを格納するクラス
 */
class TDDSceneList{
  
private:
  std::vector<const char*> nameList;
  std::vector<const char*> sceneList;
  std::vector<cocos2d::extension::CCLayerLoader*> loaderList;
  
  std::vector<TDDPartsData> partsList;
  std::vector<TDDSceneData> sceneDataList;
  TDDPartsData partsData;
public:
  TDDSceneList();
  virtual ~TDDSceneList();
  void setSceneAndPartsData(const char* sceneName, const char* sceneCCbiName, cocos2d::extension::CCLayerLoader *loaderArray, std::vector<TDDPartsData> partsArray);
  std::vector<const char*> getSceneList();
  TDDSceneData getSceneData(int index);
};
CHAOS_END_NAMESPACE

#endif /* defined(__Chaos__tdd_scene_list__) */
