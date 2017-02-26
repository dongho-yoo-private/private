//
//  tdd_scene_list.cpp
//  Chaos
//
//  Created by B04680 on 2014/02/03.
//
//

#include "tdd/tdd_scene_list.h"

#include "scene/layer/ccb_scene_types.h"
#include "scene/parts/ccb_parts_types.h"

#include "scene/layer/ccb_scene_title.h"
#include "scene/parts/ccb_scene_parts_title_start.h"

CHAOS_BEGIN_NAMESPACE
//##################################################
// Constructor
//##################################################
TDDSceneList::TDDSceneList(){
  //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  //ここに追加してください
  
  partsData.partsName = CCB_PARTS_TITLE_START;
  partsData.loader = CHAOS::CCBScenePartsTitleStartBuilderLoader::loader();
  partsList.push_back(partsData);

  setSceneAndPartsData("タイトル画面", CCB_SCENE_TITLE, CHAOS::CCBSceneTitleBuilderLoader::loader(), partsList);
  
  //ここに追加してください
  //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
}


TDDSceneList::~TDDSceneList(){

}
std::vector<const char*> TDDSceneList::getSceneList(){
  return nameList;
}

TDDSceneData TDDSceneList::getSceneData(int index){
  return sceneDataList.at(index);
  
}

void TDDSceneList::setSceneAndPartsData(const char* sceneName, const char* sceneCCbiName, cocos2d::extension::CCLayerLoader *loaderArray, std::vector<TDDPartsData> partsArray){
  TDDSceneData data = *new TDDSceneData();
  data.listName = sceneName;
  data.sceneCcbiName = sceneCCbiName;
  data.loader = loaderArray;
  data.parts = partsArray;
  nameList.push_back(sceneName);
  sceneDataList.push_back(data);
}
CHAOS_END_NAMESPACE