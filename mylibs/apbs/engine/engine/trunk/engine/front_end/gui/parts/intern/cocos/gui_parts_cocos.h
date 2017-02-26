/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file parts_cocos.h
 * \ingroup scene
 * \~english
 * \~japanese
 *  \brief シーン中に描画される「パーツ」を表すクラス定義部 for Cocos2d-x
 */

#ifndef PARTS_COCOS_H_
#define PARTS_COCOS_H_

#include <core/types.h>
#include <front_end/gui/parts/intern/gui_parts.h>

/* forward declaration for Cocos2d-x */
namespace cocos2d {
class CCNode;
namespace extension {
class CCNodeLoader;
class CCNodeLoaderLibrary;
}
}

ADK_BEGIN_NAMESPACE

/* forward declaration */
class PartsLoader;
class PartsControl;
class PartsMenu;
class PartsMember;
class PartsCommonNodeLoader;
class GuiPartsSystemCocos;

/**
 * \~english
 * \~japanese
 *  \brief パーツ一つを管理するクラス for Cocos2d-x
 */
class GuiPartsCocos : public GuiParts {
  friend class PartsControl;
  friend class PartsMenu;
  friend class PartsCommonNodeLoader;
  friend class GuiPartsSystemCocos;
 public:
  /**
   * \~english
   * \~japanese
   *  \brief デフォルトコンストラクタ
   */
  GuiPartsCocos();
  /**
   * \~english
   * \~japanese
   *  \brief デストラクタ
   */
  virtual ~GuiPartsCocos();

  /**
   * \~english
   * \~japanese
   *  \brief \ref GuiPartsCocos オブジェクトの初期化処理を行います
   *  \return 初期化の成功判定。以下の値が返ります
   *  \retval true 成功したときに返ります
   *  \retval false 失敗したときに返ります
   */
  virtual Bool Init();

  /**
   * \~english
   * \~japanese
   *  \brief ファイルからパーツを作成する
   *  \param[in] filename パーツのファイル名
   *  \param[in] parts パーツ内に点在するパーツ一つ一つの情報
   *  \param[in] parts_count パーツ内のパーツ数
   *  \return 作成の成功判定。以下の値が返ります
   *  \retval true 作成に成功したときに返ります
   *  \retval false 作成に成功したときに返ります
   */
  virtual Bool CreateWithFile(const Char* filename, const TGuiPartsInfo* parts,
                              S32 parts_count);
  /**
   * \~english
   * \~japanese
   *  \brief パーツを削除する
   *  \return 削除の成功判定。以下の値が返ります
   *  \retval true 削除に成功したときに返ります
   *  \retval false 削除に成功したときに返ります
   */
  virtual Bool Dispose();

  /**
   * \~english
   * \~japanese
   *  \brief カスタムクラスを返します
   *  \param[in] name カスタムクラス名
   *  \return \ret IGuiParts
   */
  virtual IGuiParts *GetCustomClass(const char *name);
  /**
   * \~english
   * \~japnese
   *  \brief \ref IGuiParts 内で保持しているパーツにアタッチする
   *  \param[in] parts アタッチするパーツのインタフェース
   *  \param[in] type パーツタイプ
   *  \return アタッチの成功判定。以下の値が返ります
   *  \retval true アタッチに成功したときに返ります
   *  \retval false アタッチに失敗したときに返ります
   */
  virtual IGuiParts* AttachCreateParts(const Char *partsname, TGuiPartsType type);

  /**
   * \~english
   * \~japanese
   *  \brief ボタンのコールバックを設定します
   *  \param[in] name コールバックを設定するパーツ名
   *  \param[in] callback コールバック関数のポインタ
   *  \param[in] userdata コールバック関数の引数に設定されるユーザーデータ
   *  \return 設定の成功判定。以下の値が返ります
   *  \retval true 設定に成功したときに返ります
   *  \retval false 設定に失敗したときに返ります
   */
  virtual Bool SetButtonCallback(const Char* name, TGuiPartsButtonCallback callback,
                                 void* userdata);
  /**
   * \~english
   * \~japanese
   *  \brief メニューのコールバックを設定します
   *  \param[in] name コールバックを設定するパーツ名
   *  \param[in] callback コールバック関数のポインタ
   *  \param[in] userdata コールバック関数の引数に設定されるユーザーデータ
   *  \return 設定の成功判定。以下の値が返ります
   *  \retval true 設定に成功したときに返ります
   *  \retval false 設定に失敗したときに返ります
   */
  virtual Bool SetMenuCallback(const Char* name,
                               TGuiPartsMenuCallback callback,
                               void* userdata);

  /**
   * \~english
   * \~japanese
   *  \brief \ref cocos2d::CCNode を設定
   */
  void SetNode(void *node);
  /**
   * \~english
   * \~japanese
   *  \brief \ref cocos2d::CCNode を取得する
   */
  void* GetNode();

  virtual void SetTouchPriority(S32 priority);
  virtual S32 GetTouchPriority();
  virtual void SetVisible(Bool visible);
  virtual Bool IsVisible();
  virtual void AddChild(IGuiParts *child);
  virtual void AddChild(IGuiParts *child, S32 zOrder);
  virtual void AddChild(IGuiParts *child, S32 zOrder, S32 tag);

  /**
   * \~english
   * \~japanese
   *  \brief ローダーで作成されたクラスを追加する
   *  \param[in] node \ref cocos2d::CCNode
   *  \param[in] name ローダー名
   */
  void AddLoaderClass(cocos2d::CCNode *node,const char *name);
  
   /**
   * \~english
   * \~japnese
   *  \brief \ref IGuiParts 内で保持しているパーツにアタッチする\n
   *  パーツ名重複を避けるためのメソッド。
   *  GuiPartsXXXCocosの中で呼び出されます。
   *  \param[in] parts アタッチするパーツのインタフェース
   *  \param[in] type パーツタイプ
   *  \return アタッチの成功判定。以下の値が返ります
   *  \retval true アタッチに成功したときに返ります
   *  \retval false アタッチに失敗したときに返ります
   */
  IGuiParts* AttachCreatePartsInternal(IGuiParts* parent, const Char *partsname, TGuiPartsType type);



 private:
  /**
   * \~english
   * \~japanese
   *  \brief ccbiファイルのパーシングに必要なローダー群を作成する
   *  \param[in] parts パーツ内に点在するパーツ一つ一つの情報
   *  \param[in] parts_count パーツ内のパーツ数
   *  \return 作成の成功判定。以下の値が返ります
   *  \retval \ref cocos2d::extension::CCNodeLoaderLibrary* 作成に成功したときに
   *  返ります
   *  \reval NULL 作成に失敗したときに返ります
   */
  cocos2d::extension::CCNodeLoaderLibrary* CreateLoaders(
      const TGuiPartsInfo* parts, S32 parts_count);
  /**
   * \~english
   * \~japanese
   *  \brief タイプ別のローダークラスを作成します。
   *  \param[in] parts_type \ref TPartType
   *  \return 作成の成功判定。以下の値が返ります
   *  \retval \ref cocos2d::extension::CCNodeLoaderLibrary* 作成に成功したときに
   *  返ります
   *  \reval NULL 作成に失敗したときに返ります
   */
  cocos2d::extension::CCNodeLoader* CreateLoader(TGuiPartsType parts_type);
  /**
   * \~english
   * \~japanese
   *  \brief メンバーを追加する
   *  \param[in] member \ref PartsMember
   */
  void AddMember(PartsMember *member);
  /**
   * \~english
   * \~japanese
   *  \brief コントロールのコールバックを呼び出します
   *  \param[in] name 呼び出すコールバック名
   */
  Bool DoButtonCallback(const Char *name);
  /**
   * \~english
   * \~japanese
   *  \brief メニューのコールバックを呼び出します
   *  \param[in] name 呼び出すコールバック名
   */
  Bool DoMenuCallback(const Char *name);

  /**
   * \~english
   * \~japanese
   *  \brief 作成した \ref CCBReader から返される \ref CCNode
   */
  cocos2d::CCNode* node_;
  /**
   * \~english
   * \~japanese
   *  \brief ノードをアタッチしたかのフラグ
   */
  Bool attach_node_;
  /**
   * \~english
   * \~japanese
   *  \brief パーツ単位の情報
   */
  typedef struct {
    Char name[kGuiPartsNameLen + 1];
    cocos2d::extension::CCNodeLoader* loader;
    TGuiPartsType type;
  } TPartsClass;
  /**
   * \~english
   * \~japanese
   *  \brief パーツ一つが持つパーツクラス最大数
   */
  static const S32 kMaxPartsClass = 16;
  /**
   * \~english
   * \~japanese
   *  \brief パーツが持つメンバー数
   */
  static const S32 kMaxPartsMember = 64;
  /**
   * \~english
   * \~japanese
   *  \brief ローダーの最大数
   */
  static const S32 kMaxLoader = 64;
  /**
   * \~english
   * \~japanese
   *  \brief ローダー名の最大長
   */
  static const S32 kLoaderNameLen = 63;
  /**
   * \~english
   * \~japanese
   *  \brief パーツが持つパーツクラス群
   */
  struct {
    S32 count;
    TPartsClass classes[kMaxPartsClass];
    S32 member_count;
    PartsMember *members[kMaxPartsMember];
    S32 loader_count;
    IGuiParts *loader_parts[kMaxLoader];
    cocos2d::CCNode *loaders[kMaxLoader];
    char loader_names[kMaxLoader][kLoaderNameLen];
  }loader_;
  /**
   * \~english
   * \~japanese
   *  \brief ボタンコールバック最大数
   */
  static const S32 kMaxButtonCallback = 128;
  /**
   * \~english
   * \~japanese
   *  \brief ボタン情報
   */
  typedef struct {
    TGuiPartsButtonCallback callback;
    char name[kGuiPartsNameLen + 1];
    void *userdata;
  }TButtonCallbackInfo;
  /**
   * \~english
   * \~japanese
   *  \brief ボタン
   */
  struct {
    S32 callback_count;
    TButtonCallbackInfo callbacks[kMaxButtonCallback];
  }button_;
  /**
   * \~english
   * \~japanese
   *  \brief メニューコールバック最大数
   */
  static const S32 kMaxMenuCallback = 16;
  /**
   * \~english
   * \~japanese
   *  \brief メニュー情報
   */
  typedef struct {
    TGuiPartsMenuCallback callback;
    char name[kGuiPartsNameLen + 1];
    void *userdata;
  }TMenuCallbackInfo;
  /**
   * \~english
   * \~japanese
   *  \brief メニュー
   */
  struct {
    S32 callback_count;
    TMenuCallbackInfo callbacks[kMaxMenuCallback];
  }menu_;
  
};

ADK_END_NAMESPACE

#endif /* PARTS_COCOS_H_ */

