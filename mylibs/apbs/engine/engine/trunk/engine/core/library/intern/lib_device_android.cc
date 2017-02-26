/**
 * \file lib_device_ios.mm
 * \ingroup adkgrp_library
 * \author hiroaki hosono
 * \~english
 * \~japanese
 *   \brief デバイス情報に関する実装部です
 */

#include <core/library/lib_device.h>


#include <core/debug/logging.h>
#include <core/debug/debug_assert.h>
#include <core/memory_allocation/guarded_alloc.h>

#include <jni.h>
#include <platform/android/jni/JniHelper.h>

#define UUID_KEY "uuid"
#define MAX_UUID_SIZE (63)

ADK_BEGIN_NAMESPACE

using namespace cocos2d;

static Char* gDeviceToken;

const Char* kAndroidPackageName = "jp/co/applibot/engine/androidlib/LibDeviceAndroid";

/* extern function implementation */
const Char *DEV_GetDeviceName() {
  JniMethodInfo t;
  jstring ret;
  const char* str;

  if (JniHelper::getStaticMethodInfo(t, kAndroidPackageName, "getDeviceName", "()Ljava/lang/String;")) {
    ret = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
    t.env->DeleteLocalRef(t.classID);
  }
  str = t.env->GetStringUTFChars(ret,0);

  Char* ret_str = strdup(str);

  t.env->ReleaseStringUTFChars(ret, str);

  return ret_str;
}
const char *DEV_GetOSName() {
  return "android";
}
const char *DEV_GetOSVersion() {

  JniMethodInfo method_info;
  jstring ret;

  if (!JniHelper::getStaticMethodInfo(method_info, kAndroidPackageName, "getOSVersion", "()Ljava/lang/String;")) {
    return "Error";
  }
  ret = (jstring)method_info.env->CallStaticObjectMethod(method_info.classID, method_info.methodID);
  method_info.env->DeleteLocalRef(method_info.classID);

  const char* str = method_info.env->GetStringUTFChars(ret,0);
  char* ret_str = strdup(str);
  method_info.env->ReleaseStringUTFChars(ret, str);
  return ret_str;
}
const char *DEV_GetLanguage() {
  JniMethodInfo t;
  jstring ret;
  const char* str;

  if (JniHelper::getStaticMethodInfo(t, kAndroidPackageName, "getLanguage", "()Ljava/lang/String;")) {
    ret = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
    t.env->DeleteLocalRef(t.classID);
  }

  str = t.env->GetStringUTFChars(ret,0);

  char* ret_str = strdup(str);

  t.env->ReleaseStringUTFChars(ret, str);

  return ret_str;
}
const Char *DEV_GetCountryCode() {
  JniMethodInfo t;
  jstring ret;
  const char* str;

  if (JniHelper::getStaticMethodInfo(t, kAndroidPackageName, "getCountryCode", "()Ljava/lang/String;")) {
    ret = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
    t.env->DeleteLocalRef(t.classID);
  }

  str = t.env->GetStringUTFChars(ret,0);

  char* ret_str = strdup(str);

  t.env->ReleaseStringUTFChars(ret, str);

  return ret_str;
}
const char *DEV_GetDifferenceGMT() {
  JniMethodInfo t;
  jstring ret;
  const char* str;

  if (JniHelper::getStaticMethodInfo(t, kAndroidPackageName, "getDifferenceGMT", "()Ljava/lang/String;")) {
    ret = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
    t.env->DeleteLocalRef(t.classID);
  }

  str = t.env->GetStringUTFChars(ret,0);

  char* ret_str = strdup(str);

  t.env->ReleaseStringUTFChars(ret, str);

  return ret_str;
}
const char *DEV_GetUuId() {
  JniMethodInfo t;
  jstring ret;
  const char* str;

  if (JniHelper::getStaticMethodInfo(t, kAndroidPackageName, "getUuId", "()Ljava/lang/String;")) {
    ret = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
    t.env->DeleteLocalRef(t.classID);
  }

  str = t.env->GetStringUTFChars(ret,0);

  char* ret_str = strdup(str);

  t.env->ReleaseStringUTFChars(ret, str);

  return ret_str;
}

const char* DEV_GetIdentifierForVender()
{
  JniMethodInfo t;
  jstring ret;
  const char* str;

  if (JniHelper::getStaticMethodInfo(t, kAndroidPackageName, "getIdentifierForVender", "()Ljava/lang/String;")) {
    ret = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
    t.env->DeleteLocalRef(t.classID);
  }

  str = t.env->GetStringUTFChars(ret,0);

  char* ret_str = strdup(str);

  t.env->ReleaseStringUTFChars(ret, str);

  return ret_str;
}
const Char* DEV_GetIdentifierForAdvertiser()
{
    return "";
}
const Char* DEV_GetAndroidId()
{
  JniMethodInfo method_info;
  jstring ret;

  if (!JniHelper::getStaticMethodInfo(method_info, kAndroidPackageName, "getAndroidId", "()Ljava/lang/String;")) {
    return "";
  }

  ret = (jstring)method_info.env->CallStaticObjectMethod(method_info.classID, method_info.methodID);
  method_info.env->DeleteLocalRef(method_info.classID);

  const char* str = method_info.env->GetStringUTFChars(ret,0);
  char* ret_str = strdup(str);
  method_info.env->ReleaseStringUTFChars(ret, str);
  return ret_str;

}

const Char* DEV_GetAdvertisingId()
{
    JniMethodInfo method_info;
    jstring ret;
    
    if (!JniHelper::getStaticMethodInfo(method_info, kAndroidPackageName, "getAdvertisingId", "()Ljava/lang/String;")) {
        return "";
    }
    
    ret = (jstring)method_info.env->CallStaticObjectMethod(method_info.classID, method_info.methodID);
    method_info.env->DeleteLocalRef(method_info.classID);
    
    const char* str = method_info.env->GetStringUTFChars(ret,0);
    char* ret_str = strdup(str);
    method_info.env->ReleaseStringUTFChars(ret, str);
    return ret_str;
    
}

const char* DEV_GetOpenUdId()
{
  JniMethodInfo t;
  jstring ret;
  const char* str;

  if (JniHelper::getStaticMethodInfo(t, kAndroidPackageName, "getOpenUdId", "()Ljava/lang/String;")) {
    ret = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
    t.env->DeleteLocalRef(t.classID);
  }

  str = t.env->GetStringUTFChars(ret,0);

  char* ret_str = strdup(str);

  t.env->ReleaseStringUTFChars(ret, str);

  return ret_str;
}

const char *DEV_GetSecureUdId()
{
  JniMethodInfo t;
  jstring ret;
  const char* str;

  if (JniHelper::getStaticMethodInfo(t, kAndroidPackageName, "getSecureUdId", "()Ljava/lang/String;")) {
    ret = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
    t.env->DeleteLocalRef(t.classID);
  }

  str = t.env->GetStringUTFChars(ret,0);

  char* ret_str = strdup(str);

  t.env->ReleaseStringUTFChars(ret, str);

  return ret_str;
}

Char android_device_token[255 + 1];

const Char *DEV_GetDeviceToken()
{
  return android_device_token;
}

void DEV_SetDeviceToken(const unsigned char* data, unsigned int size) {
  gDeviceToken = new Char[ size*2+1 ];
  Char* out = android_device_token;

  Char buf[4];
  for( int lp = 0; lp < size; ++lp ){
    sprintf(buf, "%02x", data[lp]);
    *(out++) = buf[0];
    *(out++) = buf[1];
  }

  *out = 0;
}

const char *DEV_GetMacAddress()
{    
  JniMethodInfo method_info;
  jstring ret;

  if (!JniHelper::getStaticMethodInfo(method_info, kAndroidPackageName, "getMacAddress", "()Ljava/lang/String;")) {
    return "Error";
  }

  ret = (jstring)method_info.env->CallStaticObjectMethod(method_info.classID, method_info.methodID);
  method_info.env->DeleteLocalRef(method_info.classID);

  const char* str = method_info.env->GetStringUTFChars(ret,0);
  char* ret_str = strdup(str);
  method_info.env->ReleaseStringUTFChars(ret, str);
  return ret_str;
}

const char *DEV_GetEmailAddress()
{
    JniMethodInfo method_info;
    jstring ret;
    
    if (!JniHelper::getStaticMethodInfo(method_info, kAndroidPackageName, "getEmailAddress", "()Ljava/lang/String;")) {
        return "Error";
    }
    
    ret = (jstring)method_info.env->CallStaticObjectMethod(method_info.classID, method_info.methodID);
    method_info.env->DeleteLocalRef(method_info.classID);
    
    const char* str = method_info.env->GetStringUTFChars(ret,0);
    char* ret_str = strdup(str);
    method_info.env->ReleaseStringUTFChars(ret, str);
    return ret_str;
}

U64 DEV_GetFileSystemSize()
{
  return 0ull;
}

U64 DEV_GetFileSystemFreeSize()
{
  return 0ull;
}

U64 DEV_GetSystemFreeMemory()
{
  return 0ull;

}

U32 DEV_GetInstallationCount()
{
  return 0;
}

bool DEV_SetUuId(const char* uuid)
{
  JniMethodInfo method_info;

  if (!JniHelper::getStaticMethodInfo(method_info, kAndroidPackageName, "setUuId", "(Ljava/lang/String;);")) {
    return false;
  }

  method_info.env->CallStaticObjectMethod(method_info.classID, method_info.methodID, method_info.env->NewStringUTF(uuid));
  method_info.env->DeleteLocalRef(method_info.classID);
  return true;
}
ADK_END_NAMESPACE
