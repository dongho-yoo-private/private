/**
 * \file adk_host_list.cc
 * \ingroup adkgrp_hostlist
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief 接続先サーバー情報を実装部
 */

#include <gameplay/http_data_interface/hostlist/adk_host_list.h>

#include <string.h>

#include <core/debug/debug_assert.h>
#include <core/debug/logging.h>
#include <core/library/lib_string.h>
#include <debug_profiling/error/err_hostlist_result_types.h>
#include <gameplay/http_data_interface/startup/adk_startup_info.h>
#include <gameplay/http_data_interface/system/system.h>

ADK_BEGIN_NAMESPACE

/* chaos server */
/**
 * \~english
 * \~japanese
 *   \brief 接続先サーバー情報
 *   \attention いまはココでネイティブのアクセス先定義をしているが、これは違う\n
 *              今後は、特定のWAS等へアクセスする事で、\n
 *              ネイティブで使用する接続先の数と、それぞれのURLが取得できるようになるのが望ましい\n
 *              \n
 *              そうなれば、全て自動化されるメリットがあり、こうやって様々情報をデータ化することでプログラマの作業がいらなくなる。\n
 */

/* constructor & destructor */
ADK_HostList::ADK_HostList() : _hostCounts(0), _masterUrl(0) {
  memset(&_accessPoint, 0, sizeof(_accessPoint));
}
ADK_HostList::~ADK_HostList() {
  if (_accessPoint.suffix != 0) {
    ADK_Free(_accessPoint.suffix), _accessPoint.suffix = 0;
  }

  if (_accessPoint.url != 0) {
    ADK_Free(_accessPoint.url), _accessPoint.url = 0;
  }
}

/* class implementation */
Result ADK_HostList::Initialize() {
  Result ret = ResultSuccess;

  System* system = System::GetSystem();
  if (system == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kHostListComponent,
                             0 /*ERR_HOST_LIST_RESULT_INITIALIZE*/, 0);
  }

  ADK_StartupInfo* info = system->GetStartupInfo();
  if (info == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kHostListComponent,
                             0 /*ERR_HOSTLIST_RESULT_INITIALIZE*/, 1);
  }

  _masterUrl = (Char*)info->GetMasterServerUrl();
#if 0
  if (_masterUrl==0)
  {
    // TODO. とりあえず、Unknownで
    return ADK_RESULT(ResultUnknown, true, true, kStartupInfoComponent, 0, kHLE_MASTER_WAS_NOT_FOUND);
  }
#endif

  // TODO. でバッグモードじゃない場合は、ここからホストリストを取得してくる。
  // 取得したホストリストは保存する必要があるのかどうなのか。。。
  _hostCounts = info->GetHostListCount();

  return ResultSuccess;
}

Result ADK_HostList::Terminate() {
  // TODO. おそらく特にすることなし。
  // HostListのアドレスは、ADK_StartupInfoのメモリをそのまま参照しているので、
  // ADK_StartupInfoが終了しない限り問題無し。
  return ResultSuccess;
}

/* server management functionally */
const Char* ADK_HostList::GetMasterServerUrl() const { return _masterUrl; }
int ADK_HostList::GetServerCount() const { return _hostCounts; }
const char* ADK_HostList::GetServerUrl(S32 server) const {
  if (server >= _hostCounts) {
    return 0;
  } else {
    System* system = System::GetSystem();
    ADK_ASSERT(system);
    ADK_StartupInfo* info = system->GetStartupInfo();
    ADK_ASSERT(info);
    return info->GetHost(server);
  }
}
const char* ADK_HostList::GetServerDisplayName(S32 server) const {
  if (server >= _hostCounts) {
    return 0;
  } else {
    System* system = System::GetSystem();
    ADK_ASSERT(system);
    ADK_StartupInfo* info = system->GetStartupInfo();
    ADK_ASSERT(info);
    return info->GetHostDisplayName(server);
  }
}
Result ADK_HostList::SetAccessPoint(const char* url) {
  Result ret = ResultSuccess;

  if (url == 0) {
    ret = ADK_RESULT_PROGRAM(ResultInvalidVariable, kHostListComponent,
                             0 /*ERR_HOST_LIST_RESULT_SET_ACCESS_POINT*/, 0);
  } else {
    if (_accessPoint.url != 0) {
      ADK_Free(_accessPoint.url), _accessPoint.url = 0;
    }

    _accessPoint.url = LIB_strdup(url);
    if (_accessPoint.url == 0) {
      ret = ADK_RESULT_PROGRAM(ResultOutOfResource, kHostListComponent,
                               0 /*ERR_HOST_LIST_RESULT_SET_ACCESS_POINT*/, 1);
    }
  }

  return ret;
}
const char* ADK_HostList::GetAccessPoint() const {
  ADK_ASSERT(_accessPoint.url);
  return _accessPoint.url;
}
const char* ADK_HostList::GetAccessPointSuffix() const {
  ADK_ASSERT(_accessPoint.suffix);
  return _accessPoint.suffix;
}

ADK_END_NAMESPACE

