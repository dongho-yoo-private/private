/**
 * Copyright 2014 Applibot Inc.
 * \author hiromu kato
 * \file parameter_manager.h
 * \ingroup parameter_manager_group
 * \~english
 * \~japanese
 *  \brief アプリケーションで利用するデータのマネジメントに関する定義部
 */
#ifndef CHAOS_PARAMETER_MANAGER_H_
#define CHAOS_PARAMETER_MANAGER_H_

#include "application/application_manager.h"
#include <core/types.h>
#include "parameter/i_parameter.h"

CHAOS_BEGIN_NAMESPACE

/**
* \brief パラメーターを管理するクラス
*/
class ParameterManager
{
public:
  /* constructor & destructor */
  /**
   * \~english
   * \~japanese
   *   \brief コンストラクタ
   */
  ParameterManager();
  
  /**
   * \~english
   * \~japanese
   *   \brief デストラクタ
   */
  virtual ~ParameterManager();

  /**
   * \brief マスターデータダウンロード時など、事前ロード時に呼び出されます。
   *
   * \param json \ref adk::LIB_JsonDocument
  */
  void OnPreload(const adk::LIB_JsonDocument& json);
  
  /**
   * \~english
   * \~japanese
   *  \brief データマネージャを初期化する
   *  \return 終了処理の成功判定。以下の値が返ります
   *  \retval true 終了処理に成功したときに返ります
   *  \retval false 終了処理に成功したときに返ります
   */
  Bool Initialize();

  /**
   * \~english
   * \~japanese
   *  \brief データマネージャを終了する
   */
  void Terminate();

  /**
  * \brief パラメータクラスを登録します。
  * 
  * \param parameter_id パラメーターのID
  * \param parameter パラメーターのインスタンス
  * 
  * \startcode
    // ex
    ParameterManager* pm = ApplicationManager::GetInstance()->GetParameterManager();

    pm->RegisterParameter(kParameterIdPvp, new PvpParameter());
    pm->RegisterParameter(kParameterIdUser, new UserParameter());
              :
              :
  * \endcode
  */
  void RegisterParameter(S32 parameter_id, IParameter* parameter);

  /**
  * \brief パラメータクラスを登録を外します。
  * 
  * \param parameter パラメーターのインスタンス
  */
  void UnRegisterParameter(IParameter* parameter);

  /**
  * \brief RegisterParameterで登録したパラメータを取得します。　
  * 
  * \param parameter_id パラメーターのID
  * \return パラメーターのインスタンス
  * \startcode
    // ex
    ParameterManager* pm = ApplicationManager::GetInstance()->GetParameterManager();
    const PvpParameter* pvp = pm->GetParameter(kParameterIdPvp);
  * \endcode
  */
  IParameter* GetParameter(S32 parameter_id) const;

private:
  IParameter* parameter_top_;
};

CHAOS_END_NAMESPACE

#endif
