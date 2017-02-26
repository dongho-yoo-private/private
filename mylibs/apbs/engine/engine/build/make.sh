#!/bin/sh

FOUNDATION_NAME="ApplibotFoundation"
PROJECT_NAME="af-engine"
IOS_PATH="ApplibotFoundation/${PROJECT_NAME}/libs/ios"
ANDROID_PATH="ApplibotFoundation/${PROJECT_NAME}/libs/android"

echo "<<<< Build af-adk for Android >>>>"
NDK_PATH="/Users/temp/android/ndk/android-ndk-r9d"
ENGINE_PATH="/Users/temp/Project"

cd $ENGINE_ROOT/build/android/engine/jni
$NDK_PATH/ndk-build clean
$NDK_PATH/ndk-build NDK_DEBUG=1
#lipo \
#  -create ../obj/local/armeabi/libaf-adk.a \
#  ../obj/local/armeabi-v7a/libaf-adk.a \
#  ../obj/local/mips/libaf-adk.a \
#  ../obj/local/x86/libaf-adk.a \
#  -output "build/libs/${PROJECT_NAME}"
cd ../../../

echo "<<<< Create ApplibotFoundation >>>>"
# clean
rm -rf "${FOUNDATION_NAME}"

# create directory
mkdir -p "${FOUNDATION_NAME}"
mkdir -p "${FOUNDATION_NAME}/${PROJECT_NAME}"
mkdir -p "${FOUNDATION_NAME}/${PROJECT_NAME}/inc"
mkdir -p "${FOUNDATION_NAME}/${PROJECT_NAME}/libs"
mkdir -p "${IOS_PATH}"
mkdir -p "${ANDROID_PATH}/engine"
mkdir -p "${ANDROID_PATH}/engine/libs"
mkdir -p "${ANDROID_PATH}/engine/libs/armeabi"
mkdir -p "${ANDROID_PATH}/engine/libs/armeabi-v7a"
mkdir -p "${ANDROID_PATH}/engine/libs/mips"
mkdir -p "${ANDROID_PATH}/engine/libs/x86"

# copy librarys
cp "android/engine/obj/local/armeabi/libaf-engine.a" "${ANDROID_PATH}/engine/libs/armeabi/libaf-engine.a"
cp "android/engine/obj/local/armeabi-v7a/libaf-engine.a" "${ANDROID_PATH}/engine/libs/armeabi-v7a/libaf-engine.a"
# cp "android/obj/local/mips/libaf-adk.a" "${ANDROID_PATH}/libs/mips/libaf-adk.a"
# cp "android/obj/local/x86/libaf-adk.a" "${ANDROID_PATH}/libs/x86/libaf-adk.a"

cp "ios/trunk/ApplibotFoundation/engine/libs/ios/af-engine.a" "${IOS_PATH}/af-engine.a"

# copy headers
mkdir -p ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/api
mkdir -p ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/debug
mkdir -p ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/error
mkdir -p ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/guardalloc
mkdir -p ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/library
mkdir -p ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/library/intern
mkdir -p ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/phase
mkdir -p ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/network
mkdir -p ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/dms
cp ../trunk/engine/adk.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/adk.h
cp ../trunk/engine/adk_types.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/adk_types.h
cp ../trunk/engine/intern/api/adk-api.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/api/adk-api.h
cp ../trunk/engine/intern/debug/dbg_assert.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/debug/dbg_assert.h
cp ../trunk/engine/intern/debug/dbg_log.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/debug/dbg_log.h
cp ../trunk/engine/intern/debug/dbg_log_types.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/debug/dbg_log_types.h
cp ../trunk/engine/intern/dms/dms_schema.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/dms/dms_schema.h
cp ../trunk/engine/intern/dms/dms_schema_types.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/dms/dms_schema_types.h
cp ../trunk/engine/intern/error/adk_result.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/error/adk_result.h
cp ../trunk/engine/intern/error/adk_result_types.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/error/adk_result_types.h
cp ../trunk/engine/intern/guardalloc/adk_guard_alloc.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/guardalloc/adk_guard_alloc.h
cp ../trunk/engine/intern/guardalloc/adk_guard_alloc_types.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/guardalloc/adk_guard_alloc_types.h
cp ../trunk/engine/intern/library/intern/lib_json_value-inl.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/library/intern/lib_json_value-inl.h
cp ../trunk/engine/intern/library/lib_base64.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/library/lib_base64.h
cp ../trunk/engine/intern/library/lib_file_operations.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/library/lib_file_operations.h
cp ../trunk/engine/intern/library/lib_file_types.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/library/lib_file_types.h
cp ../trunk/engine/intern/library/lib_json_document.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/library/lib_json_document.h
cp ../trunk/engine/intern/library/lib_json_types.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/library/lib_json_types.h
cp ../trunk/engine/intern/library/lib_json_value.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/library/lib_json_value.h
cp ../trunk/engine/intern/library/lib_list.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/library/lib_list.h
cp ../trunk/engine/intern/library/lib_path_util.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/library/lib_path_util.h
cp ../trunk/engine/intern/library/lib_stack.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/library/lib_stack.h
cp ../trunk/engine/intern/library/lib_thread.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/library/lib_thread.h
cp ../trunk/engine/intern/library/lib_time.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/library/lib_time.h
cp ../trunk/engine/intern/network/adk_network_types.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/network/adk_network_types.h
cp ../trunk/engine/intern/network/adk_http_response.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/network/adk_http_response.h
cp ../trunk/engine/intern/phase/adk_i_phase.h ${FOUNDATION_NAME}/${PROJECT_NAME}/inc/intern/phase/adk_i_phase.h
