/**
 * \file dms_schema.cc
 * \ingroup adkgrp_dms
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief DMSで扱うスキーマ実装部
 */

#include <gameplay/http_data_interface/dms/dms_schema.h>

#include <unistd.h> /* used for usleep() */

#include <core/debug/debug_assert.h>
#include <core/library/lib_file_operations.h>
#include <core/library/lib_json_document.h>
#include <core/library/lib_string.h>
#include <debug_profiling/error/result_types.h>
#include <gameplay/http_data_interface/api/api.h>
#include <gameplay/http_data_interface/dms/intern/dms_value.h>
#include <gameplay/http_data_interface/request/adk_i_request.h>
#include <gameplay/http_data_interface/system/system.h>

ADK_BEGIN_NAMESPACE

/* dms schema method id */
#define SCHEMA_START (100)
#define SCHEMA_INITIALIZE (SCHEMA_START + 0)
#define SCHEMA_TERMINATE (SCHEMA_START + 1)

/* class implementation */
/* constructor & destructor */
DMS_Schema::DMS_Schema() : errorcallback_(0) {
  memset(&binary_, 0, sizeof(binary_));
  request_.isCompleted = false;
  request_.is_interruption = false;
}
DMS_Schema::~DMS_Schema() {}

/* initialize & terminate */
Result DMS_Schema::Initialize(const Char *filename) {
  Result ret = ResultSuccess;
  LIB_TFileObject *file = 0;

  file = LIB_FileOpen(filename, LIB_kFileTypeRead);
  if (file == 0) {
    ret = ADK_RESULT(ResultNotFileExist, false, false, kDmsComponent,
                     SCHEMA_INITIALIZE, 0);
    goto errend;
  }

  binary_.size = static_cast<U32>(LIB_FileSize(file));
  binary_.addr = static_cast<Byte *>(
      ADK_Malloc(binary_.size + 1, "Schema::bin")); /* add '\0' */
  if (binary_.addr == 0) {
    ret = ADK_RESULT(ResultOutOfResource, true, false, kDmsComponent,
                     SCHEMA_INITIALIZE, 1);
    goto errend;
  } else {
    LIB_FileRead(binary_.addr, file, binary_.size);
    binary_.addr[binary_.size] = 0x00;
  }

  ret = Parse(binary_.addr, binary_.size);
  if (ret.IsSuccess() == false) {
    goto errend;
  }

errend:

  if (file != 0) {
    LIB_FileClose(file), file = 0;
  }

  return ret;
}
Result DMS_Schema::Terminate() {
  DisposeResponseData();

  DisposeSchema();

  if (binary_.addr != 0) {
    ADK_Free(binary_.addr), binary_.addr = 0;
  }

  return ResultSuccess;
}

/* value */
void DMS_Schema::ResetRequest() {}
Result DMS_Schema::Request(const Char *requestData,
                           void (*errorcallback)(S32, const Char *)) {
  Result ret = ResultSuccess;
  System *system = System::GetSystem();

  request_.data = CreateRequestData((void *)system->GetJsonCommonParameter(),
                                    (void *)requestData);
  if (request_.data == 0) {
    ret = ResultUnknown;
  } else {
    errorcallback_ = errorcallback;
    ret = DoRequestStart();
  }

  return ret;
}
Result DMS_Schema::RequestAsync(const Char *requestData,
                                void (*errorcallback)(S32, const Char *)) {
  Result ret = ResultSuccess;
  System *system = System::GetSystem();

  request_.data = CreateRequestData((void *)system->GetJsonCommonParameter(),
                                    (void *)requestData);
  if (request_.data == 0) {
    ret = ResultUnknown;
  } else {
    errorcallback_ = errorcallback;
    DoRequestStartAsync();
  }

  return ret;
}
#if 0
Bool DMS_Schema::GetBoolean( const Char *key, Reuslt *result) const
{
  ADK_ASSERT(key);

  const DMS_Value  *value = FindValue( key);

  return value->GetBoolean();
}
const Char* DMS_Schema::GetString( const Char *key, Reuslt *result) const
{
  ADK_ASSERT(key);

  const DMS_Value  *value = FindValue( key);

  return value->GetString();
}
S32 DMS_Schema::GetS32( const Char *key, Reuslt *result) const
{
  ADK_ASSERT(key);

  const DMS_Value  *value = FindValue( key);

  return value->GetS32();
}
const LIB_TJsonMember& DMS_Schema::GetJsonMember( const Char *key, Result *result) const
{
}
const void* DMS_Schema::GetArray( const Char *key, S32 index) const
{
  ADK_ASSERT(key);

  const DMS_Value  *value = FindValue( key);

  return value->GetArray( index);
}
#endif
Result DMS_Schema::DoRequestStart() {
  Result ret = ResultSuccess;
  System *system = System::GetSystem();
  ADK_ASSERT(system);

  if (IsRequestCompleted() != false) {
    ret = ResultDoAlready;
  } else {
    system->StartErrorProcess();
    ret = system->Request(GetAccessUrl(), IRequest::kRequestResponse, this);
  }

  return ret;
}
Result DMS_Schema::DoRequestStartAsync() {
  Result ret = ResultSuccess;
  System *system = System::GetSystem();
  ADK_ASSERT(system);

  if (IsRequestCompleted() != false) {
    ret = ResultDoAlready;
  } else {
    system->StartErrorProcess();
    ret =
        system->RequestAsync(GetAccessUrl(), IRequest::kRequestResponse, this);
  }

  return ret;
}
Result DMS_Schema::DoRequestWait() {
  Result ret = ResultSuccess;

  while (IsRequestCompleted() == false) {
    adk::HttpDataInterface::DispatchProcess();
    usleep(1);
  }

  return ret;
}
Result DMS_Schema::DoRequestEnd() {
  Result ret = ResultSuccess;
  System *system = System::GetSystem();
  ADK_ASSERT(system);

  system->EndErrorProcess();

  if (adk::HttpDataInterface::IsError() != false) {
    const U32 kMessageLen = 512;
    Char message[kMessageLen];
    S32 code = 0;

    system->GetErrorMessage(&code, message, kMessageLen);
    DoError(code, message);

    ret = ResultUnknown;
  }

  return ret;
}
void DMS_Schema::DoRequestInterruption() {
  DoRequestComplete();
  request_.is_interruption = true;
}

/* error management functionally */
void DMS_Schema::DoError(S32 code, const Char *message) {
  System *system = System::GetSystem();
  ADK_ASSERT(system);

  if (errorcallback_ != 0) {
    (errorcallback_)(code, message);
  }
}

ADK_END_NAMESPACE

