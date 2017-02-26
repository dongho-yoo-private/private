/**
 * \file lib_device_osx.mm
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief デバイス情報に関する実装部です for OSX
 */


#include <core/library/lib_device.h>

#include <sys/sysctl.h>

#include <core/debug/debug_assert.h>
#include <core/debug/logging.h>
#include <core/memory_allocation/guarded_alloc.h>


ADK_BEGIN_NAMESPACE


/* extern function implementation */
const Char *DEV_GetDeviceName() {
  ADK_ASSERT(0);
  return 0;
}
const Char *DEV_GetOSName() {
  ADK_ASSERT(0);
  return 0;
}
const Char *DEV_GetOSVersion() {
  ADK_ASSERT(0);
  return 0;
}
const Char *DEV_GetLanguage() {
  ADK_ASSERT(0);
  return 0;
}
const Char *DEV_GetDifferenceGMT() {
  ADK_ASSERT(0);
  return 0;
}


ADK_END_NAMESPACE


