/**
 * Copyright 2014 Applibot Inc.
 * \author Dongho.Yoo
 * \file i_parameter.cc
 * \ingroup parameter_manager_group
 * \~english
 * \~japanese
 *  \brief アプリケーション側で使うパラメータクラスの親クラス
 */

#include "parameter/i_parameter.h"

CHAOS_BEGIN_NAMESPACE

IParameter::IParameter()
: next_(0),
id_(0)
{
}

IParameter::~IParameter()
{
}

void IParameter::OnPreload(const adk::LIB_JsonDocument& json)
{
  return ;
}


CHAOS_END_NAMESPACE
