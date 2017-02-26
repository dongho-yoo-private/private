//
//  tdd_game_flow_list.h
//  Chaos
//
//  Created by B04680 on 2014/02/03.
//
//

#ifndef __Chaos__tdd_game_flow_list__
#define __Chaos__tdd_game_flow_list__

#include "tdd/tdd_types.h"

CHAOS_BEGIN_NAMESPACE
class TDDGameFlowList{
private:
  std::vector<const char*> nameList;
public:
  std::vector<const char*> getGameFlowList();
  
};

CHAOS_END_NAMESPACE


#endif /* defined(__Chaos__tdd_game_flow_list__) */
