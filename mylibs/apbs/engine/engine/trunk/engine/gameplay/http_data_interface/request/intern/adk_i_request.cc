/**
 * \file adk_i_request.cc
 * \ingroup adkgrp_request
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief リクエストインターフェース実装部
 */

#include <gameplay/http_data_interface/request/adk_i_request.h>

#include <stdio.h>
#include <sys/time.h>

#include <core/debug/logging.h>
#include <core/library/lib_json_document.h>
#include <core/library/lib_string.h>
#include <core/library/lib_url_encode.h>
#include <debug_profiling/error/err_request_result_types.h>
#include <network/http/adk_np_http_response.h>
#include <gameplay/http_data_interface/system/system.h>

ADK_BEGIN_NAMESPACE

/* class implementaiton */
/* constructor & destructor */
IRequest::IRequest() : _requestType(kRequestUnknown), url_(0) {
  memset(&data_, 0, sizeof(data_));
  memset(&response_, 0, sizeof(response_));
}
IRequest::~IRequest() {
  if (data_.addr != 0) {
    ADK_Free(data_.addr), data_.addr = 0;
  }
}

/* request management functionally */
Result IRequest::Initialize(const char *url, TRequestType reqtype,
                            const Char *requestData) {
  Result ret = ResultSuccess;
  const System *system = System::GetSystem();

  _requestType = reqtype;

  {
    const U32 aplen = (U32)strlen(system->GetAccessPoint());
    const U32 urllen = (U32)strlen(url);
    const U32 totlen = aplen + urllen + 1; /* api + url + '/' */

    url_ = (Char *)ADK_Malloc(totlen + 1, "Request::URL");
    if (url_ == 0) {
      ret = ADK_RESULT_PROGRAM(ResultOutOfResource, kRequestComponent,
                               kRequestInterface,
                               ERR_I_REQUEST_INITIALIZE_OUT_OF_RESOURCE);
      goto createerr;
    }
    sprintf(url_, "%s%s", system->GetAccessPoint(), url);
  }

  {
    data_.addr = (Char *)requestData;
    data_.size = requestData != 0 ? LIB_strlen(requestData) : 0;
#if 0
    Char  *jsonstr = CreateCommonHeader();
    if (jsonstr==0)  
    {
      ret = ADK_RESULT( ResultUnknown, true, false, kRequestComponent, 0, 0);
    }
    else
    {
      U32  urllen;
      Char *urlenc;

      ret = LIB_URLEncode( (Char**)&urlenc, jsonstr);
      if (ret.IsSuccess()!=false)
      {
#if 1
        data_.addr = LIB_strdup(jsonstr);
        sprintf( (Char*)data_.addr, "%s", jsons!tr);
        data_.size = LIB_strlen( (const Char*)data_.addr);
#else
#if 0
        data_.addr = urlenc;
        data_.size = LIB_strlen( (Char*)data_.addr);
#else
        urllen = 5+LIB_strlen(urlenc);
        data_.addr = (Char*)ADK_Calloc( 1, urllen+1, "Json>URLEnc");
        if (data_.addr==0)
        {
          ret = ADK_RESULT( ResultOutOfResource, true, false, kRequestComponent, 0, 0);
        }
        else
        {
          sprintf( (Char*)data_.addr, "json=%s", urlenc);
          data_.size = LIB_strlen( (const Char*)data_.addr);
        }
        ADK_Free( urlenc), urlenc = 0;
#endif
#endif
      }

      DisposeCommonHeader( jsonstr), jsonstr = 0;
    }
#endif
  }

  return ret;

createerr:

  Terminate();

  return ret;
}
Result IRequest::Terminate() {
  Result ret = ResultSuccess;
  System *system = System::GetSystem();

  if (system == 0) {
    return ADK_RESULT_PROGRAM(ResultNotInitialized, kRequestComponent,
                              kRequestInterface,
                              ERR_I_REQUEST_TERMINATE_NOT_INITIALIZED_SYSTEM);
  }

  if (response_.response != 0) {
    system->DisposeRequestToWas(response_.response), response_.response = 0;
    response_.status = 0;
    response_.contentType = 0;
  }

  if (url_ == 0) {
    return ADK_RESULT_PROGRAM(ResultNotInitialized, kRequestComponent,
                              kRequestInterface,
                              ERR_I_REQUEST_TERMINATE_NOT_INITIALIZED_URL);
  } else {
    ADK_Free(url_), url_ = 0;
  }

  _requestType = kRequestUnknown;

  return ret;
}
Result IRequest::DoRequest() {
  Result ret = ResultSuccess;

  if (url_ == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kRequestComponent,
                             kRequestInterface,
                             ERR_I_REQUEST_DO_REQUEST_NOT_INITIALIZED);
    goto doerr;
  }

  switch (_requestType) {
    case kRequestResponse: {
      ret = DoRequestResponse();
      break;
    }
    case kRequestAck: {
      ret = DoRequestAck();
      break;
    }
    case kRequestUnknown: {
      ADK_ASSERT(false);
      break;
    }
  }

  return ret;

doerr:

  return ret;
}
Result IRequest::DoRequestResponse() {
  Result ret = ResultSuccess;
  System *system = System::GetSystem();

  if (system == 0) {
    ret = ADK_RESULT_PROGRAM(ResultNotInitialized, kRequestComponent,
                             kRequestInterface,
                             ERR_I_REQUEST_DO_REQUEST_RESPONSE_NOT_INITIALIZED);
  } else {
    ret = system->RequestToWas((const HttpResponse **)&response_.response, url_,
                               data_.addr, data_.size);
    if (ret.IsSuccess() != false) {
      response_.status = response_.response->GetStatus();
      response_.contentType =
          ((NpHttpResponse *)response_.response)->GetContentType();
      response_.data = response_.response->GetData();
      response_.size = response_.response->GetDataSize();
    }
  }

  return ret;
}
Result IRequest::DoRequestAck() {
  Result ret = ResultSuccess;
  ADK_ASSERT(0);
  return ret;
}

#if 0
/* header management functionally */
void IRequest::DisposeCommonHeader( Char *header) {
  ADK_ASSERT(header);
  ADK_Free( header), header = 0;
}
#endif

ADK_END_NAMESPACE

