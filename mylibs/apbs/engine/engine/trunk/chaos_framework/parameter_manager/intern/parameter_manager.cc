/**
 * Copyright 2014 Applibot Inc.
 * \author hiromu kato
 * \file parameter_manager.cc
 * \ingroup parameter_manager_group
 * \~english
 * \~japanese
 *  \brief アプリケーションで利用するデータのマネジメントに関する実装部
 */

#include "application/application_types.h"
#include "parameter_manager/parameter_manager.h"

CHAOS_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 *  \brief デフォルトコンストラクタ
 */
ParameterManager::ParameterManager()
  :
  top_parameter_(0)
{
}

/**
 * \~english
 * \~japanese
 *  \brief デストラクタ
 */
ParameterManager::~ParameterManager()
{
}
  
/**
 * \brief マスターデータダウンロード時など、事前ロード時に呼び出されます。
 *
 * \param json \ref adk::LIB_JsonDocument
*/
void ParameterManager::OnPreload(const adk::LIB_JsonDocument& json)
{
  for (IParameter* cur = top_parameter_; cur; cur=cur->np)
  {
    cur->OnPreload(json);
  }
}

/**
 * \~english
 * \~japanese
 *  \brief データマネジャの初期化処理を行います
 */
Bool ParameterManager::Initialize() {
  
  return true;
}

/**
 * \~english
 * \~japanese
 *  \brief データマネジャの終了処理を行います
 */
void ParameterManager::Terminate() {
   if (top_parameter_)
   {
      for (IParameter* cur=top_parameter_; cur; )
      {
        IParameter* temp=cur;
        cur=cur->np; 
        delete temp;
      }
   }
}

/**
* \brief パラメータクラスを登録します。
* 
* \param parameter_id パラメーターのID
* \param parameter パラメーターのインスタンス
*/
void ParameterManager::RegisterParameter(S32 parameter_id, IParameter* parameter)
{
  if (top_parameter_==0)
  {
    top_parameter_=parameter;
    top_parameter_->SetId(parameter_id);
    return ;
  }

  IParameter* cur;
  for (cur = top_parameter_; cur; cur=cur->np)
  {
    if (cur->GetNext()==0)
    {
      cur->SetNext(parameter);
      parameter->SetNext(0);
      parameter->SetId(parameter_id);
      break;
    }
  }

  ADK_ASSERT(0);
}

/**
* \brief パラメータクラスを登録を外します。
* 
* \param parameter パラメーターのインスタンス
*/
void ParameterManager::UnRegisterParameter(IParameter* parameter)
{
  if (top_parameter_==0)
  {
    return 0;
  }

  IParameter* cur, *before=0;
  for (cur = top_parameter_; cur; cur=cur->np)
  {
    if (cur->GetId()==parameter->GetId())
    {
      IParameter* next = parameter->GetNext();

      cur->SetNext(next);
      if (before==0)
      {
        return ;
      }
      before->SetNext(cur); 
      return ;
    }
    before=cur;
  }

  return ;
}

/**
* \brief RegisterParameterで登録したパラメータを取得します。　
* 
* \param parameter_id パラメーターのID
* \return パラメーターのインスタンス
*/
IParameter* ParameterManager::GetParameter(S32 parameter_id) const
{
  if (top_parameter_==0)
  {
    return 0;
  }

  IParameter* cur;
  for (cur = top_parameter_; cur; cur=cur->np)
  {
    if (cur->GetId()==parameter_id)
    {
      return cur;
    }
  }
  return 0;
}


CHAOS_END_NAMESPACE
