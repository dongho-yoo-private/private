//
//  tdd_game_flow_list.cpp
//  Chaos
//
//  Created by B04680 on 2014/02/03.
//
//

#include "tdd/tdd_game_flow_list.h"
CHAOS_BEGIN_NAMESPACE
std::vector<const char*> TDDGameFlowList::getGameFlowList(){
  
  const char* gameFlowArray[] = {"Flow1"
    ,"Flow2"
    ,"Flow3"
    ,"マイページからギルドバトル"
  };
  int index;
  int max = ARRAY_LENGTH(gameFlowArray);
  for (index = 0; index < max; ++index) {
    nameList.push_back(gameFlowArray[index]);
  }
  
  return nameList;
  
}
CHAOS_END_NAMESPACE