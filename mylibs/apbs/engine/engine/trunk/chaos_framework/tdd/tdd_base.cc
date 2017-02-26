//
//  tdd_base.cpp
//  Chaos
//
//  Created by B04680 on 2014/02/03.
//
//

#include "tdd/tdd_base.h"
CHAOS_BEGIN_NAMESPACE

TDDBase::TDDBase(){
  
}
TDDBase::~TDDBase(){
  
}

TDDMenu* TDDBase::createTopMenu(){

  TDDMenu *layer = TDDMenu::create();
  TDDMenuList *menuList = new TDDMenuList();
  
  std::vector<const char*> itemList = menuList->getMenuList();
  layer->setTableItemList(itemList);
  
  delete menuList;

  return layer;
}
CHAOS_END_NAMESPACE

