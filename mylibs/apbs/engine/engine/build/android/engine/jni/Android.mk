# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := af-engine
LOCAL_MODULE_FILENAME := libaf-engine

LOCAL_CFLAGS := -DWITH_ANDROID
LOCAL_CONLYFLAGS := -std=gnu99
LOCAL_CPPFLAGS += -std=c++03

LOCAL_EXPORT_C_INCLUDES := \
  $(NDK_PATH)/platforms/android-19/arch-arm/usr/include \
	$(LOCAL_PATH)/../../../../trunk/engine/ \
	$(LOCAL_PATH)/../../../../extern/cocos2d-x

LOCAL_C_INCLUDES := \
  $(NDK_PATH)/platforms/android-19/arch-arm/usr/include \
	$(LOCAL_PATH)/../../../../trunk/engine/ \
	$(LOCAL_PATH)/../../../../extern/cocos2d-x/cocos2dx

LOCAL_SRC_FILES := \
	$(LOCAL_PATH)/../../../../extern/cocos2d-x/cocos2dx/platform/android/jni/JniHelper.cpp \
	$(LOCAL_PATH)/../../../../trunk/engine/core/debug/intern/logging.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/debug/intern/stack_trace.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/debug/intern/stack_trace_android.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/library/intern/intern/ioapi.cpp \
	$(LOCAL_PATH)/../../../../trunk/engine/core/library/intern/intern/unzip.cpp \
	$(LOCAL_PATH)/../../../../trunk/engine/core/library/intern/lib_base64.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/library/intern/lib_device_android.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/library/intern/lib_file_operations_android.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/library/intern/lib_json_document.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/library/intern/lib_json_value.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/library/intern/lib_list.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/library/intern/lib_object_list.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/library/intern/lib_path_util_android.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/library/intern/lib_path_utils-c_api.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/library/intern/lib_path_utils.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/library/intern/lib_profile.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/library/intern/lib_stack.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/library/intern/lib_string.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/library/intern/lib_thread.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/library/intern/lib_zlib.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/library/intern/lib_url_encode.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/library/intern/lib_time.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/library/intern/lib_url_scheme_android.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/memory_allocation/intern/guarded_alloc.c \
	$(LOCAL_PATH)/../../../../trunk/engine/core/memory_allocation/intern/mem_block.c \
	$(LOCAL_PATH)/../../../../trunk/engine/core/memory_allocation/intern/mem_i_arena.c \
	$(LOCAL_PATH)/../../../../trunk/engine/core/memory_allocation/intern/mem_internal_alloc_android.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/memory_allocation/intern/mem_large_arena.c \
	$(LOCAL_PATH)/../../../../trunk/engine/core/memory_allocation/intern/mem_list.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/memory_allocation/intern/mem_small_arena.c \
	$(LOCAL_PATH)/../../../../trunk/engine/core/memory_allocation/intern/mem_startup.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/core/module/intern/entrypoint.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/debug_profiling/error/intern/err_api_result.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/debug_profiling/error/intern/err_dms_result.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/debug_profiling/error/intern/err_error_manager.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/debug_profiling/error/intern/err_error_system_result.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/debug_profiling/error/intern/err_host_list_result.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/debug_profiling/error/intern/err_library_result.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/debug_profiling/error/intern/err_network_protocol_result.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/debug_profiling/error/intern/err_phase_result.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/debug_profiling/error/intern/err_request_manager_result.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/debug_profiling/error/intern/err_result.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/debug_profiling/error/intern/err_startup_info_result.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/debug_profiling/error/intern/err_system_result.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/gameplay/http_data_interface/api/intern/api.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/gameplay/http_data_interface/dms/intern/dms_schema_json.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/gameplay/http_data_interface/dms/intern/adk_dms.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/gameplay/http_data_interface/dms/intern/dms_schema.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/gameplay/http_data_interface/dms/intern/dms_value.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/gameplay/http_data_interface/hostlist/intern/adk_host_list.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/gameplay/http_data_interface/request/intern/adk_i_request.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/gameplay/http_data_interface/request/intern/adk_request_manager.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/gameplay/http_data_interface/startup/intern/adk_startup_info.cc \
	$(LOCAL_PATH)/../../../../trunk/engine/gameplay/http_data_interface/system/intern/system.cc \

include $(BUILD_STATIC_LIBRARY)
