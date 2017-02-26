//
//  tdd_api_list.h
//  Chaos
//
//  Created by B04680 on 2014/02/03.
//
//

#ifndef __Chaos__tdd_api_list__
#define __Chaos__tdd_api_list__

#include "tdd/tdd_types.h"

CHAOS_BEGIN_NAMESPACE
/**
 * \~english
 * \~japanese
 * \brief Apiリストを格納するクラス
 */
class TDDApiList{
private:
  std::vector<const char*> nameList;
  std::vector<const char*> apiList;
  
public:
  std::vector<const char*> getApiList();
  
};
CHAOS_END_NAMESPACE
#endif /* defined(__Chaos__tdd_api_list__) */
