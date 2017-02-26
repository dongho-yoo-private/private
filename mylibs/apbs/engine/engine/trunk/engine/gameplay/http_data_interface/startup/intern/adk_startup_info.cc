/**
 * \file adk_startup_info.cc
 * \ingroup adkgrp_startup
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief ADKの起動情報に関する実装部
 */

#include <gameplay/http_data_interface/startup/adk_startup_info.h>

#include <core/debug/logging_types.h>
#include <core/library/lib_file_operations.h>
#include <core/library/lib_path_util.h>
#include <debug_profiling/error/result.h>
#include <debug_profiling/error/result_types.h>

ADK_BEGIN_NAMESPACE

ADK_StartupInfo::ADK_StartupInfo() { memset(&_binary, 0, sizeof(_binary)); }
ADK_StartupInfo::~ADK_StartupInfo() {}
Result ADK_StartupInfo::Initialize(const char *filename) {
  Result ret = ResultSuccess;
  Char path[FILE_NAME_MAX_LEN];

  if (LIB_GetResourceDir(path, FILE_PATH_MAX_LEN) == false) {
    ret = ADK_RESULT_PROGRAM(ResultUnknown, kStartupInfoComponent,
                             0 /*ERR_STARTUP_INFO_INITIALIZE*/,
                             0 /*ERR_STARTUP_INFO_INITIALIZE*/);
  } else {
    const S32 len = (S32)strlen(path);
    path[len] = '/';
    strcpy(&path[len + 1], filename);

    LIB_TFileObject *file_object = LIB_FileOpen(path, LIB_kFileTypeRead);
    if (file_object == 0) {
      ret = ResultUnknown;
    } else {
      _binary.size = (U32)LIB_FileSize(file_object);
      _binary.addr = (Byte *)ADK_Malloc(_binary.size + 1, "StartupInfo::file");
      if (_binary.addr == 0) {
        ret = ResultOutOfResource;
      } else {
        LIB_FileRead(_binary.addr, file_object, _binary.size);
        _binary.addr[_binary.size] = '\0';
        ret = _jdoc.Parse((Char *)_binary.addr, false, 0);
      }
      LIB_FileClose(file_object), file_object = 0;
    }
  }

  return ret;
}
Result ADK_StartupInfo::Terminate() {
  Result ret = ResultSuccess;
  ret = _jdoc.Release();
  if (_binary.addr == 0) {
    ret = ResultNotInitialized;
  } else {
    ADK_Free(_binary.addr), _binary.addr = 0;
  }

  return ret;
}
Bool ADK_StartupInfo::IsDebug() const {
  const LIB_TJsonMember &debug = _jdoc["debug"];
  if (debug.data.IsValid() == false) {
    return false;
  } else {
    const LIB_TJsonMember &on_off = debug.data["on/off"];
    if (on_off.data.IsValid() == false) {
      return false;
    } else {
      return on_off.data.GetBool();
    }
  }
}
TLogLevel ADK_StartupInfo::GetLogLevel() const {
  const LIB_TJsonMember &debug = _jdoc["debug"];
  if (debug.data.IsValid() == false) {
    return kNoneLog;
  } else {
    const LIB_TJsonMember &level = debug.data["debug level"];
    if (level.data.IsValid() == false) {
      return kNoneLog;
    } else {
      const Char *level_string = level.data.GetString();
      if (strstr(level_string, "info") != 0) {
        return kInfoLog;
      } else if (strstr(level_string, "fatal") != 0) {
        return kFatalLog;
      } else if (strstr(level_string, "warn") != 0) {
        return kWarningLog;
      } else if (strstr(level_string, "debug") != 0) {
        return kDebugLog;
      } else if (strstr(level_string, "always") != 0) {
        return kAllLog;
      } else {
        return kNoneLog;
      }
    }
  }
}
TLogMode ADK_StartupInfo::GetLogOutput() const {
  const LIB_TJsonMember &debug = _jdoc["debug"];
  if (debug.data.IsValid() == false) {
    return kConsoleLog;
  } else {
    const LIB_TJsonMember &output = debug.data["output"];
    if (output.data.IsValid() == false) {
      return kConsoleLog;
    } else {
      const Char *output_string = output.data.GetString();
      if (strstr(output_string, "console") != 0) {
        return kConsoleLog;
      }
      if (strstr(output_string, "file") != 0) {
        return kFileLog;
      }
      if (strstr(output_string, "syslog") != 0) {
        return kConsoleLog;
      } else {
        return kConsoleLog;
      }
    }
  }
}
const Char *ADK_StartupInfo::GetLogFileName() const {
  const LIB_TJsonMember &debug = _jdoc["debug"];
  if (debug.data.IsValid() == false) {
    return 0;
  } else {
    const LIB_TJsonMember &filename = debug.data["output"];
    if (filename.data.IsValid() == false) {
      return 0;
    } else {
      return filename.data.GetString();
    }
  }
}
const Char *ADK_StartupInfo::GetMasterServerUrl() const {
  const LIB_TJsonMember &hostlist = _jdoc["host list"];
  if (hostlist.data.IsValid() == false) {
    return 0;
  } else {
    const LIB_TJsonMember &master = hostlist.data["master"];
    if (master.data.IsValid() == false) {
      return 0;
    } else {
      return master.data.GetString();
    }
  }
}
S32 ADK_StartupInfo::GetHostListCount() const {
  const LIB_TJsonMember &hostlist = _jdoc["host list"];
  if (hostlist.data.IsValid() == false) {
    return 0;
  } else {
    const LIB_TJsonMember &debug = hostlist.data["debug"];
    if (debug.data.IsValid() == false) {
      return 0;
    } else {
      return debug.data.Size();
    }
  }
}
const Char *ADK_StartupInfo::GetHost(S32 host) const {
  const LIB_TJsonMember &hostlist = _jdoc["host list"];
  if (hostlist.data.IsValid() == false) {
    return 0;
  } else {
    const LIB_TJsonMember &debug = hostlist.data["debug"];
    if (debug.data.IsValid() == false) {
      return 0;
    } else {
      return debug.data.GetMember(host).data.GetString();
    }
  }
}
const Char *ADK_StartupInfo::GetHostDisplayName(S32 host) const {
  const LIB_TJsonMember &hostlist = _jdoc["host list"];
  if (hostlist.data.IsValid() == false) {
    return 0;
  } else {
    const LIB_TJsonMember &debug = hostlist.data["debug"];
    if (debug.name.IsValid() == false) {
      return 0;
    } else {
      return debug.data.GetMember(host).name.GetString();
    }
  }
}
S32 ADK_StartupInfo::GetKeepAliveTime() const {
  const LIB_TJsonMember &network = _jdoc["network"];
  if (network.data.IsValid() == false) {
    return 0;
  } else {
    const LIB_TJsonMember &keep_alive = network.data["keep alive"];
    if (keep_alive.data.IsValid() == false) {
      return 0;
    } else {
      return keep_alive.data.GetS32();
    }
  }
}
S32 ADK_StartupInfo::GetAutoRetryCount() const {
  const LIB_TJsonMember &network = _jdoc["network"];
  if (network.data.IsValid() == false) {
    return 0;
  } else {
    const LIB_TJsonMember &count = network.data["auto retry count"];
    if (count.data.IsValid() == false) {
      return 0;
    } else {
      return count.data.GetS32();
    }
  }
}
S32 ADK_StartupInfo::GetConnectTimeout() const {
  const LIB_TJsonMember &network = _jdoc["network"];
  if (network.data.IsValid() == false) {
    return 0;
  } else {
    const LIB_TJsonMember &timeout = network.data["connect timeout"];
    if (timeout.data.IsValid() == false) {
      return 0;
    } else {
      return timeout.data.GetS32();
    }
  }
}
S32 ADK_StartupInfo::GetReadTimeout() const {
  const LIB_TJsonMember &network = _jdoc["network"];
  if (network.data.IsValid() == false) {
    return 0;
  } else {
    const LIB_TJsonMember &timeout = network.data["read timeout"];
    if (timeout.data.IsValid() == false) {
      return 0;
    } else {
      return timeout.data.GetS32();
    }
  }
}
U32 ADK_StartupInfo::GetHttpKeyLength() const {
  const LIB_TJsonMember &network = _jdoc["network"];
  if (network.data.IsValid() == false) {
    return 64;
  } else {
    const LIB_TJsonMember &timeout = network.data["key length"];
    if (timeout.data.IsValid() == false) {
      return 64;
    } else {
      return timeout.data.GetU32();
    }
  }
}
U32 ADK_StartupInfo::GetHttpValueLength() const {
  const LIB_TJsonMember &network = _jdoc["network"];
  if (network.data.IsValid() == false) {
    return 256;
  } else {
    const LIB_TJsonMember &timeout = network.data["value length"];
    if (timeout.data.IsValid() == false) {
      return 256;
    } else {
      return timeout.data.GetU32();
    }
  }
}

#if 0
// -------------------------------
ADK_StartupInfo::ADK_StartupInfo()
      : 
  //------------- variables for ADK_Log
  mIsDebugOn(false),
  mLogLevel(kLogLevelAlways),
  mLogOutput(kLogOutputConsole),
  mLogFileName(0),
  //------------- variables for ADK_HostList
  mStrMasterHostName(0),
  mHosts(0),
  mHostComments(0),
  mHostCount(0),
  mHostKeys(0),
  //------------- variables for ADK_NetworkProtocol
  mKeepAliveTime(0),
  mAutoRetryCount(3),
  mConnectTimeout(10),
  mReadTimeout(10)
{
}

// -------------------------------
ADK_StartupInfo::~ADK_StartupInfo()
{
  if (mHosts)
    ADK_Free(mHosts);
  if (mHostComments)
    ADK_Free(mHostComments);
  if (mHostKeys)
    LIB_ProfileReleaseEnumKeys((const Char**)mHostKeys, mHostCount);
}

// -------------------------------
Result ADK_StartupInfo::Initialize(const char* filename)
{
  char path[FILE_NAME_MAX_LEN+FILE_PATH_MAX_LEN+1];

  if (LIB_GetResourceDir(path, FILE_PATH_MAX_LEN)==false)
  {
    ADK_FATAL_LOG("ADK_StartupInfo::Load(%s) LIB_GetResourceDir failure!\n", filename);
    return ADK_RESULT(ResultNotFileExist, true, false, kStartupInfoComponent, 0, kSIE_RESOURCE_DIR_NOT_FOUND);
  }
  S32 len = (S32)strlen(path);
  path[len] = '/';
  strcpy(&path[len+1], filename);

  HandleProfile hProfile = LIB_ProfileOpen(path);

  TStartupInfoError err = LoadDebugInfo(hProfile);

  if (err!=kSIE_SUCCESS)
  {
    return ADK_RESULT(ResultUnknown, true, true, kStartupInfoComponent, 1, err);
  }

  if ((err = LoadHostListInfo(hProfile))!=kSIE_SUCCESS)
  {
    return ADK_RESULT(ResultUnknown, true, true, kStartupInfoComponent, 2, err);
  }

  if ((err = LoadNetworkInfo(hProfile))!=kSIE_SUCCESS)
  {
    return ADK_RESULT(ResultUnknown, true, true, kStartupInfoComponent, 3, err);
  }

    return ResultSuccess;//ADK_RESULT(ResultUnknown, true, true, kStartupInfoComponent, 3, err);
}

// ------------------------------------------
TStartupInfoError ADK_StartupInfo::LoadDebugInfo(HandleProfile hProfile)
{
    Char val[32];
    
  if (LIB_ProfileReadStringEx(hProfile, "Debug", "debug", val)==false)
  {
    return kSIE_KEY_WAS_NOT_FOUND;
  }
  if (strcmp(val, "ON")==0)
    mIsDebugOn=true;
  else
    mIsDebugOn=false;

  if (LIB_ProfileReadStringEx(hProfile, "Debug", "debuglvl", val)==false)
  {
    return kSIE_KEY_WAS_NOT_FOUND;
  }

  if (strcmp(val, "fatal")==0)
  {
    mLogLevel=kLogLevelFatal; 
  }
  else if (strcmp(val, "warn")==0)
  {
    mLogLevel=(TLogLevel)(kLogLevelFatal|kLogLevelWarn);
  }
  else if (strcmp(val, "info")==0)
  {
    mLogLevel=(TLogLevel)(kLogLevelFatal|kLogLevelWarn|kLogLevelInfo);
  }
  else if (strcmp(val, "trace")==0)
  {
    mLogLevel=(TLogLevel)(kLogLevelFatal|kLogLevelWarn|kLogLevelInfo|kLogLevelTrace);
  }
  else if (strcmp(val, "always")==0)
  {
    mLogLevel=kLogLevelAlways;
  }
  else
  {
    return kSIE_INVALID_VALUE;
  }

  if (LIB_ProfileReadStringEx(hProfile, "Debug", "output", val)==false)
  {
    return kSIE_KEY_WAS_NOT_FOUND;
  }

  if (strcmp(val, "console")==0)
  {
    mLogOutput = kLogOutputConsole;
  }
  else if (strcmp(val, "file")==0)
  {
    mLogOutput = kLogOutputFile;
  }
  else
  {
    return kSIE_INVALID_VALUE;
  }

  if (mLogOutput==kLogOutputFile)
  {
    char path[FILE_NAME_MAX_LEN+1];
    if (LIB_ProfileReadStringEx(hProfile, "Debug", "filename", path)==true)
    {
      mLogFileName = LIB_strdup(path);  
    }
    else
    {
      mLogFileName=0;
    }
  }

  return kSIE_SUCCESS;
}

// ------------------------------------------
TStartupInfoError ADK_StartupInfo::LoadHostListInfo(HandleProfile hProfile)
{
  int n;
  char** pKeys = (char**)LIB_ProfileEnumKeysEx(hProfile, "HostList", &n);

  if (pKeys==0)
  {
    return kSIE_KEY_WAS_NOT_FOUND;
  }

  int cnt=0;
  Bool bIsMasterFound=false;
  mHosts = (char**)ADK_Malloc(sizeof(char*)*(n+1), "ADK_StartupInfo HostList");
  mHostComments = (char**)ADK_Malloc(sizeof(char*)*(n+1), "ADK_StartupInfo HostList Comment");
  for (int i=0; i<n; i++)
  {
    char hostname[MAX_HOSTNAME_LEN+1];
    LIB_ProfileReadStringEx(hProfile, "HostList", pKeys[i], hostname);

    if (bIsMasterFound==false &&
        strcmp(pKeys[i], "master")==0)
    {
      mStrMasterHostName = LIB_strdup(hostname);
      bIsMasterFound=true;
      continue;
    }
    mHosts[cnt] = LIB_strdup(hostname);
    mHostComments[cnt++] = pKeys[i];
  }
  mHostKeys=pKeys;
  mHostCount=cnt;

  if (bIsMasterFound==false)
  {
    return kSIE_KEY_WAS_NOT_FOUND;
  }
  return kSIE_SUCCESS;
}

// ------------------------------------------
TStartupInfoError ADK_StartupInfo::LoadNetworkInfo(HandleProfile hProfile)
{
  if (LIB_ProfileReadIntEx(hProfile, "Network", "KeepAlive", &mKeepAliveTime)==false)
  {
    return kSIE_KEY_WAS_NOT_FOUND;
  }
  if (LIB_ProfileReadIntEx(hProfile, "Network", "AutoRetryCount", &mAutoRetryCount)==false)
  {
    return kSIE_KEY_WAS_NOT_FOUND;
  }
  if (LIB_ProfileReadIntEx(hProfile, "Network", "ConnectTimeout", &mConnectTimeout)==false)
  {
    return kSIE_KEY_WAS_NOT_FOUND;
  }
  if (LIB_ProfileReadIntEx(hProfile, "Network", "ReadTimeout", &mReadTimeout)==false)
  {
    return kSIE_KEY_WAS_NOT_FOUND;
  }

  return kSIE_SUCCESS;
}


// --------------------------- for ADK_HostList


// ------------------------------------------
Bool ADK_StartupInfo::IsDebugOn()
{
  return mIsDebugOn;
}

// ------------------------------------------
TLogLevel ADK_StartupInfo::GetLogLevel()
{
  return mLogLevel;
}

// ------------------------------------------
TLogOutput ADK_StartupInfo::GetLogOutput()
{
  return mLogOutput;
}

// ------------------------------------------
const Char* ADK_StartupInfo::GetLogFileName()
{
  return mLogFileName;
}

// --------------------------- for ADK_HostList


// ------------------------------------------
const Char* ADK_StartupInfo::GetMasterServerUrl()
{
  return mStrMasterHostName;
}

// ------------------------------------------
/*S32 ADK_StartupInfo::GetHostListCount()
{
  return mHostCount;
}*/

// ------------------------------------------
S32 ADK_StartupInfo::GetHostList(Char*** hostName, Char*** hostDisplayName)
{
  *hostName=mHosts;

  if (hostDisplayName)
  {
    *hostDisplayName=mHostComments;
  }
  return mHostCount;
}

// --------------------------- for ADK_NetworkProtocol

// ------------------------------------------
S32 ADK_StartupInfo::GetKeepAliveTime()
{
  return mKeepAliveTime;  
}

// ------------------------------------------
S32 ADK_StartupInfo::GetAutoRetryCount()
{
  return mAutoRetryCount;  
}

// ------------------------------------------
S32 ADK_StartupInfo::GetConnectTimeout()
{
  return mConnectTimeout;  
}

// ------------------------------------------
S32  ADK_StartupInfo::GetReadTimeout()
{
  return mReadTimeout;  
}
#endif

ADK_END_NAMESPACE

