//
//  tdd_base.h
//  Chaos
//
//  Created by B04680 on 2014/02/03.
//
//

#ifndef __Chaos__tdd_base__
#define __Chaos__tdd_base__

#include "tdd/tdd_types.h"
#include "tdd/tdd_menu.h"
#include "tdd/tdd_menu_list.h"

CHAOS_BEGIN_NAMESPACE
/**
 * \~english
 * \~japanese
 *  \brief TDD全体を管理、統括するクラスです。
 *  このクラスから \ref TDDMenu や \ref TDDApiList にアクセスし、
 *  テストを行います。
 */
class TDDBase{
private:
  
public:
  TDDBase();
  virtual ~TDDBase();

  /**
   * \~english
   * \~japanese
   *   \brief TDDのトップメニューを返します。
   *   \return TDDMenuを返します。
   */
  TDDMenu *createTopMenu();
  
};
CHAOS_END_NAMESPACE
#endif /* defined(__Chaos__tdd_base__) */
