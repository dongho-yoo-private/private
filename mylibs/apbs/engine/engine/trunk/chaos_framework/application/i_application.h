/**
 * Copyright 2014 Applibot Inc.
 * \author Dongho.Yoo
 * \file i_parameter.h
 * \ingroup application_manager_group
 * \~english
 * \~japanese
 *  \brief アプリケーションの初期化、終了などを行うインターフェースクラス。
 */

#ifndef __I_APPLICATION_H__
#define __I_APPLICATION_H__

#include "application/application_types.h"
#include "scene/system/scene/scene_types.h"
#include <core/types.h>
#include <core/library/lib_json_document.h>

CHAOS_BEGIN_NAMESPACE

/**
* \brief アプリケーション側ではこのクラスをApplicationManagerに登録する事で開始されます。
*/
class IApplication
{
public:
  /**
  * \brief 初期化します。
  * このメソッドはParameterManagerでしか呼び出されません。
  *
  * return 結果を返します。
  * retval true 成功
  * retval false 成功
  */
  virtual Bool Initialize() = 0;

  /**
  * \brief はじめに表示するシーンIDを返します。
  */
  virtual TSceneId GetFirstSceneId() = 0;

  /**
  * \brief 終了します。
  * このメソッドはParameterManagerでしか呼び出されません。
  *
  */
  virtual Bool Terminate() = 0;

};

CHAOS_END_NAMESPACE

#endif
