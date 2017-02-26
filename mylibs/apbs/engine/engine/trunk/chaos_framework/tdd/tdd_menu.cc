//
//  tdd_menu.cpp
//  Chaos
//
//  Created by B04680 on 2014/02/03.
//
//

#include "tdd/tdd_menu.h"

#define TDD_HEIGHT_TABLEVIEW_TITLE (400)
#define TDD_HEIGHT_TABLEVIEW_WARNING (100)
#define TDD_HEIGHT_TABLEVIEW (200)
#define TDD_WIDTH_TABLEVIEW (200)
#define TDD_TABLE_VIEW_FONT_NAME "Arial"

#define TDD_TABLE_VIEW_BG_IMAGE_NAME "tdd_tv_bg.png"
#define TDD_CLOSE_BUTTON_IMAGE_NAME "Icon-72.png"

#include "tdd/tdd_scene_list.h"
#include "scene/layer/ccb_scene_title.h"
#include "scene/parts/ccb_scene_parts_title_start.h"

#include "application/application_manager.h"
#include "game/game_manager.h"

CHAOS_BEGIN_NAMESPACE
enum kMenuType{
  kTDDMenuRequestData = 0,
  kTDDMenuLoadScene,
  kTDDMenuGameFlow,
  kTDDMenuParseData
};


TDDMenu::TDDMenu()
:menuLevel_(0)
,tableView(NULL)
{
  
}
TDDMenu::~TDDMenu()
{
  
}
//==============================
//初期化
//==============================
bool TDDMenu::init(){
  if (!CCLayer::init()) {
    return false;
  }
//  menuLevel_  = 10;
  // 背景色を変更
  CCLayerColor *color = CCLayerColor::create(ccc4(0,0,0,50.0f));
  this->addChild(color);
  
  windowSize = CCDirector::sharedDirector()->getWinSize();
  //Titleラベル
  CCLabelTTF *label = CCLabelTTF::create(typeid(TDDMenu).name(), TDD_TABLE_VIEW_FONT_NAME, 25);
  label->setPosition(ccp(windowSize.width/2, TDD_HEIGHT_TABLEVIEW_TITLE));
  this->addChild(label);
  
  //テーブルビューの背景イメージ
  CCSprite *sprite = CCSprite::create(TDD_TABLE_VIEW_BG_IMAGE_NAME);
  CCSpriteFrame *spriteFrame = CCSpriteFrame::create(TDD_TABLE_VIEW_BG_IMAGE_NAME, CCRect(0, 0, sprite->getContentSize().width, sprite->getContentSize().height));
  CCScale9Sprite *listFrame = CCScale9Sprite::createWithSpriteFrame(spriteFrame, CCRectMake(10, 10, sprite->getContentSize().width - 20, sprite->getContentSize().height - 20));
  
  listFrame->setPosition(ccp(windowSize.width / 2, windowSize.height/2));
  listFrame->setContentSize(CCSize(listFrame->getContentSize().width + 200, listFrame->getContentSize().height));
  this->setTouchPriority(100);
  this->addChild(listFrame);
  
  //閉じるボタン
  CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                                        TDD_CLOSE_BUTTON_IMAGE_NAME,
                                                        TDD_CLOSE_BUTTON_IMAGE_NAME,
                                                        this,
                                                        menu_selector(TDDMenu::menuCloseCallback) );
  CCPoint listFramePosition = listFrame->getPosition();
  CCSize listFrameSize = listFrame->getContentSize();
  pCloseItem->setPosition( ccp(listFramePosition.x + listFrameSize.width/2, listFramePosition.y + listFrameSize.height/2) );
  CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
  pMenu->setPosition( CCPointZero );
  this->addChild(pMenu, 1);
  
  return true;
}
//==============================
//Public Method
//==============================
void TDDMenu::setTableItemList(std::vector<const char*> itemList){
  
  cellNameList = itemList;
  setTableView();
}

//==============================
//Private Method
//==============================
void TDDMenu::setTableView(){
  CCLog("%s",__FUNCTION__);
  tableView = CCTableView::create(this, CCSizeMake(TDD_WIDTH_TABLEVIEW, TDD_HEIGHT_TABLEVIEW));
  
  //縦表示
  tableView->setDirection(kCCScrollViewDirectionVertical);
  //表示順番
  tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
  CCSize vSize = tableView->getViewSize();
  tableView->setPosition(ccp((windowSize.width-vSize.width)/2,(windowSize.height - vSize.height)/2));
  tableView->setDelegate(this);
  
  this->addChild(tableView,1);
  
  tableView->reloadData();
}
void TDDMenu::showWarningMessage(){
  CCLabelTTF* label = CCLabelTTF::create("Please make a target api", "Arial", 28);
  label->setPosition(ccp(windowSize.width/2, TDD_HEIGHT_TABLEVIEW_WARNING));
  label->setColor(ccc3(255, 0, 0));
  this->addChild(label);
}

//==============================
//CCTableView DataSource START
//==============================
// セルのサイズを設定する
CCSize TDDMenu::cellSizeForTable(CCTableView* table) {
  return CCSize(TDD_WIDTH_TABLEVIEW, 24);
}
// セルの中身を作成する
CCTableViewCell* TDDMenu::tableCellAtIndex (CCTableView* table,unsigned int idx) {
  
  CCTableViewCell* cell = table->dequeueCell();
  cell = new CCTableViewCell;
  cell->autorelease();
  
  CCString *str = CCString::createWithFormat("%02d %s",idx+1,cellNameList.at(idx));
  
  CCLabelTTF* label = CCLabelTTF::create(str->getCString(), TDD_TABLE_VIEW_FONT_NAME, 14);
  label->setAnchorPoint(ccp(0, 0));
  label->setPosition(ccp(10, 0));
  label->setColor(ccc3(0, 0, 0));
  cell->addChild(label);
  
  return cell;
}

// セルの数を設定する
unsigned int TDDMenu::numberOfCellsInTableView (CCTableView* table) {
  return cellNameList.size();
}

//==============================
//CCTableView DataSource END
//==============================


//==============================
//CCTableView Delegate START
//==============================
// テーブルにタッチした時に発生するイベント
void TDDMenu::tableCellTouched(CCTableView* table, CCTableViewCell* cell) {
  
  CCLOG("%s",__FUNCTION__);
  int idx = cell->getIdx();
  CCLOG("cell index = %d",idx);
  
  CCLOG("menuLevel = %d",menuLevel_);

  //application_managerとの調整のため、コメントアウト
//  CCArray *array = cell->getChildren();
  
//  int menuLevel = TDDBase::getInstance()->getMenuLevel();
//  int menuLevel = 0;
  
  //メニューレベル１でidx判定
  if(menuLevel_ == 0){
    CCLOG("if");
    removeAllChildren();
    TDDMenu *layer = TDDMenu::create();
    layer->menuLevel_ = idx+1;
    std::vector<const char*> itemList;
    switch (idx) {
      case kTDDMenuRequestData:
//        itemList = TDDBase::getInstance()->getApiList();
        break;
      case kTDDMenuLoadScene:

//        itemList = TDDBase::getInstance()->getSceneList();
      {
        TDDSceneList *sceneList = new TDDSceneList();
        itemList = sceneList->getSceneList();
        delete sceneList;
      }
        break;
      case kTDDMenuGameFlow:
//        itemList = TDDBase::getInstance()->getGameFlowList();
        break;
      case kTDDMenuParseData:
//        itemList = TDDBase::getInstance()->getParseDataList();
        break;
    }
    layer->setTableItemList(itemList);
    this->addChild(layer);
  }//if 0
  else{
    CCLOG("else menuLevel = %d",menuLevel_);
    switch (menuLevel_) {
      case 1://TDD request data
        // DM::XXXX::setXXXX();
        // tdd_list[idx].pfnCallbak();
        break;
      case 2://TDD LoadScene
      {
        //TODO.adk4app : SceneManagerクラスからシーンの切り替えを行うように変更
        CCDirector* pDirector = CCDirector::sharedDirector();
        CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
        
        //リストタイプのコード
        TDDSceneList *sceneList = new TDDSceneList();
        TDDSceneData data = sceneList->getSceneData(idx);
        CCLOG("sceneName = %s",data.listName);
        CCLOG("fileName = %s",data.sceneCcbiName);
        
        ccNodeLoaderLibrary->registerCCNodeLoader(data.sceneCcbiName, data.loader);
        
        std::vector<TDDPartsData> parts = data.parts;
        CCLOG("parts size = %d",parts.size());
        for (int i = 0; i < parts.size(); ++i) {
          TDDPartsData item = parts.at(i);
          CCLOG("parsName = %s",item.partsName);
          ccNodeLoaderLibrary->registerCCNodeLoader(item.partsName, item.loader);
        }
        
        CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
        CCString *str = CCString::createWithFormat("%s.ccbi",data.sceneCcbiName);
        CCNode* node = ccbReader->readNodeGraphFromFile(str->getCString());//adk4appのクラスを使用
        ((chaos::CCBSceneTitle*)node)->setAnimationManager(ccbReader->getAnimationManager());//SceneBaseで設定する
        ((chaos::CCBSceneTitle*)node)->setCCBSceneTitleDeletate(NULL);//delegateは受けない（TDDでsceenクラスを呼ぶように変更する予定です。）
        ccNodeLoaderLibrary->unregisterCCNodeLoader(data.sceneCcbiName);
        for (int i = 0; i < parts.size(); ++i) {
          TDDPartsData item = parts.at(i);
          ccNodeLoaderLibrary->unregisterCCNodeLoader(item.partsName);
        }
        ccbReader->release();
        CCScene* pScene = CCScene::create();
        pScene->addChild(node);
        pDirector->replaceScene(pScene);
      }
        break;
      case 3://TDD GameFlow
        break;
      default:
        showWarningMessage();
        break;
    }
  }
}
void TDDMenu::scrollViewDidScroll(CCScrollView* view) {
}

void TDDMenu::scrollViewDidZoom(CCScrollView* view) {
}

void TDDMenu::menuCloseCallback(CCObject* pSender)
{
  CCLOG("%s",__FUNCTION__);
   ApplicationManager::GetInstance()->GetGameManager()->RequestEventReplaceScene(kRootSelect, 0, 0);
}
CHAOS_END_NAMESPACE