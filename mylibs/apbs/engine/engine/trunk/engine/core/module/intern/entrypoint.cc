/**
 * \file entorypoint.h
 * \ingroup grp_adk
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief スタートアップ&シャットダウンモジュール定義群
 */

#include <core/types.h>
#include <core/module/intern/application_main.h>

int main(int argc,char *argv[]) {
  return ApplicationMain(argc,argv);
}

