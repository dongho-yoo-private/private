/**
 * Copyright 2014 Applibot Inc.
 * \author hiromu kato
 * \file parameter_manager.cc
 * \ingroup parameter_manager_group
 * \~english
 * \~japanese
 *  \brief アプリケーションで利用するデータのマネジメントに関する実装部
 */

#include "parameter/parameter_manager.h"


CHAOS_BEGIN_NAMESPACE

/**
 * \~english
 * \~japanese
 *  \brief デフォルトコンストラクタ
 */
ParameterManager::ParameterManager()
  :
  parameter_top_(0)
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
  for (IParameter* cur = parameter_top_; cur; cur=cur->next_)
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
   if (parameter_top_)
   {
      for (IParameter* cur=parameter_top_; cur; )
      {
        IParameter* temp=cur;
        cur=cur->next_; 
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
  if (parameter->Initialize()==false)
  {
    ADK_ASSERT(0);
  }
  
  if (parameter_top_==0)
  {
    parameter_top_=parameter;
    parameter_top_->SetId(parameter_id);
    parameter_top_->SetNext(0);
    return ;
  }

  IParameter* cur;
  for (cur = parameter_top_; cur; cur=cur->next_)
  {
    if (cur->GetNext()==0)
    {
      cur->SetNext(parameter);
      parameter->SetNext(0);
      parameter->SetId(parameter_id);
      break;
    }
  }

}

/**
* \brief パラメータクラスを登録を外します。
* 
* \param parameter パラメーターのインスタンス
*/
void ParameterManager::UnRegisterParameter(IParameter* parameter)
{
  if (parameter_top_==0)
  {
    return;
  }

  IParameter* cur, *before=0;
  for (cur = parameter_top_; cur; cur=cur->next_)
  {
    if (cur->GetId()==parameter->GetId())
    {
      IParameter* next = (IParameter*)parameter->GetNext();

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
  if (parameter_top_==0)
  {
    return 0;
  }

  IParameter* cur;
  for (cur = parameter_top_; cur; cur=cur->next_)
  {
    if (cur->GetId()==parameter_id)
    {
      return cur;
    }
  }
  return 0;
}


CHAOS_END_NAMESPACE
