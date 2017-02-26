//
//  tdd_menu_list.h
//  Chaos
//
//  Created by B04680 on 2014/02/03.
//
//

#ifndef __Chaos__tdd_menu_list__
#define __Chaos__tdd_menu_list__

#include "tdd/tdd_types.h"

CHAOS_BEGIN_NAMESPACE
class TDDMenuList{
private:
  std::vector<const char*> nameList;
  std::vector<const char*> menuList;
public:
  std::vector<const char*> getMenuList();
};
CHAOS_END_NAMESPACE

#endif /* defined(__Chaos__tdd_menu_list__) */
