/**
 * \file lib_url_scheme.mm
 * \ingroup adkgrp_library
 * \author ishii yasutaka
 * \~english
 * \~japanese
 *   \brief アプリケーション・ストアに関する定義部です
 */


#include <core/library/lib_url_scheme.h>

#include <core/debug/logging.h>
#include <core/debug/debug_assert.h>
#include <core/memory_allocation/guarded_alloc.h>

#include <jni.h>
#include <platform/android/jni/JniHelper.h>

ADK_BEGIN_NAMESPACE

static const Char* kAndroidPackageName = "jp/co/applibot/engine/androidlib/LibUrlSchemeAndroid";


void LIB_OpenUrlScheme(const char* url)
{
    cocos2d::JniMethodInfo method_info;
    
    if (!cocos2d::JniHelper::getStaticMethodInfo(method_info, kAndroidPackageName, "openUrlScheme", "(Ljava/lang/String;)V")) {
        return;
    }
    
    jstring stringyUrl = method_info.env->NewStringUTF(url);
    
    method_info.env->CallStaticObjectMethod(method_info.classID, method_info.methodID, stringyUrl);
    method_info.env->DeleteLocalRef(method_info.classID);
}

ADK_END_NAMESPACE


