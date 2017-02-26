//
//  tdd_api_list.cpp
//  Chaos
//
//  Created by B04680 on 2014/02/03.
//
//

#include "tdd/tdd_api_list.h"
CHAOS_BEGIN_NAMESPACE
std::vector<const char*> TDDApiList::getApiList(){
  const char* nameArr[] = {
    "MyPage Api"
    ,"Gacha Api"
    ,"Skill Api"
  };
  int index;
  int max = ARRAY_LENGTH(nameArr);
  for (index = 0; index < max; ++index) {
    nameList.push_back(nameArr[index]);
  }
  return nameList;
}
CHAOS_END_NAMESPACE
//SceneData TDDApiList::getSceneData(int index){
//
//}


