/**
 * Copyright 2014 Applibot Inc.
 * \author Dongho.Yoo
 * \file i_parameter.h
 * \ingroup parameter_manager_group
 * \~english
 * \~japanese
 *  \brief アプリケーション側で使うパラメータクラスの親クラス
 */

#ifndef __I_PARAMETER_H__
#define __I_PARAMETER_H__

#include "application/application_types.h"
#include <core/types.h>
#include <core/library/lib_json_document.h>

CHAOS_BEGIN_NAMESPACE

/**
* \brief アプリケーション側で使われるパラメータークラス。\n
* このクラスを継承して使います。
*/
class IParameter
{
public:
  IParameter();
  virtual ~IParameter();

protected:
  /**
  *\brief 次のIParameterを設定します。\n
  * このメソッドはParameterManagerでしか呼び出されません。
  *
  *\param parameter 次のパラメーター
  */
  ADK_INLINE void SetNext(IParameter* parameter)
  {
    next_=parameter;
  }

  /**
  *\brief 次のIParameterのIDを取得します。\n
  * このメソッドはParameterManagerでしか呼び出されません。
  *
  *\param parameter 次のパラメーター
  */
  ADK_INLINE S32 GetId() 
  {
    return id_;
  }

  /**
  *\brief 次のIParameterのIDを設定します。\n
  * このメソッドはParameterManagerでしか呼び出されません。
  *
  *\param parameter 次のパラメーター
  */
  ADK_INLINE void SetId(S32 id)
  {
    id_ = id;
  }

  /**
  *\brief 次のIParameterを取得します。\n
  * このメソッドはParameterManagerでしか呼び出されません。
  *
  *\param parameter 次のパラメーター
  */
  ADK_INLINE const IParameter* GetNext() 
  {
    return next_;
  }

  /**
  *\brief 次のIParameterのIDを取得します。\n
  * このメソッドはParameterManagerでしか呼び出されません。
  *
  *\return id
  */
  ADK_INLINE S32 GetId() const
  {
    return id_;
  }

  /**
  * \brief JsonDocumentのデータを事前に設定します。
  * このメソッドはParameterManagerでしか呼び出されません。
  *
  * \param json JsonDocument形式のデータ
  */
  virtual void OnPreload(const adk::LIB_JsonDocument& json);

  /**
  * \brief 初期化します。
  * このメソッドはParameterManagerでしか呼び出されません。
  *
  */
  virtual Bool Initialize() = 0;

  /**
  * \brief 終了します。
  * このメソッドはParameterManagerでしか呼び出されません。
  *
  */
  virtual Bool Terminate() = 0;



private:
  IParameter* next_;
  S32 id_;

  friend class ParameterManager;
};

CHAOS_END_NAMESPACE

#endif
