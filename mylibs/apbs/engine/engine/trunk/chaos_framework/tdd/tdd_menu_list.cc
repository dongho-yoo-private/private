//
//  tdd_menu_list.cpp
//  Chaos
//
//  Created by B04680 on 2014/02/03.
//
//

#include "tdd/tdd_menu_list.h"
CHAOS_BEGIN_NAMESPACE
std::vector<const char*> TDDMenuList::getMenuList(){
  
  const char* nameArr[] = {"Request Data"
    ,"Load Scene"
    ,"Game Flow"
    ,"Parse Data"
    
  };
  
  int index;
  int max = ARRAY_LENGTH(nameArr);
  for (index = 0; index < max; ++index) {
    nameList.push_back(nameArr[index]);
  }
  return nameList;
}
CHAOS_END_NAMESPACE