/**
 * \file log.cpp
 * \ingrup log
 * \author Kohei konno
 * \~english
 * \~japanese
 * \brief デバッグ機能(詳細はwikiを参考）
 * \参考wiki http://wiki.applibot.co.jp/pages/viewpage.action?pageId=5603926
 */

#include <core/debug/logging.h>

#include <stdarg.h>
#include <assert.h>
#include <time.h>
#include <string.h>

ADK_BEGIN_NAMESPACE

Log* Log::instance_ = 0;

Log::Log()
    : log_level_(kNoneLog),
      log_mode_(kConsoleLog),
      log_output_(kStartLog),
      file_(stdout)
{
          strncpy(filename_, LOG_DEFAULT_FILENAME, LOG_FILENAME_MAX_SIZE);
}

Log::~Log() {}

Log* Log::GetInstance() { return instance_; }

bool Log::Initialize() {
  bool ret = true;

  if (instance_ != 0) {
    ret = false;
  } else {
    instance_ = new Log();
    if (instance_ == 0) {
      ret = false;
    } else {
      ret = true;
    }
  }
  assert(ret == true);
  return ret;
}
bool Log::Terminate() {
  bool ret = true;
  if (instance_ == NULL) {
    ret = false;
  } else {
    delete instance_, instance_ = NULL;
  }
  return ret;
}

void Log::setLogOutput(TLogOutput flag) { log_output_ = flag; }

void Log::setLogLevel(TLogLevel level) { log_level_ = level; }

void Log::setLogMode(TLogMode mode) { log_mode_ = mode; }

void Log::setDateTime() {
  time_t timer;
  time(&timer);
  struct tm* local;
  local = localtime(&timer);

  int year = local->tm_year + 1900;
  int mon = local->tm_mon + 1;
  int day = local->tm_mday;
  int hour = local->tm_hour;
  int min = local->tm_min;
  int sec = local->tm_sec;

  memset(time_buf_, 0, LOG_DATETIME_SIZE);
  sprintf(time_buf_, "%02d-%02d-%02d %02d:%02d:%02d", year, mon, day, hour, min,
          sec);
}

void Log::setFileName(const char* filename) {
  sprintf(filename_, "%s", filename);
}

void Log::output(const char* header, TLogLevel level, const char* func,
                 const char* format, ...) {

  if (log_output_ == kStopLog) {
    return;
  }

  if (log_level_ == kNoneLog) {
    return;
  }

  if (log_level_ & level) {

    char str[LOG_MAX_SIZE] = {0, };

    va_list ap;
    assert(str != NULL);
    va_start(ap, format);
    int result = vsnprintf(str, LOG_MAX_SIZE, format, ap);
    if (result > LOG_MAX_SIZE) {
      fprintf(stderr, "表示可能LOGは%d文字以内です。\n", LOG_MAX_SIZE);
      assert(result < LOG_MAX_SIZE);
    }
    va_end(ap);

    setDateTime();

    char log[LOG_MAX_SIZE] = {0, };

    sprintf(log, "%s %s [%s] %s\n", time_buf_, header, func, str);

    if (log_mode_ == kConsoleLog) {
      fprintf(stdout, "%s", log);
    } else if (log_mode_ == kFileLog) {
      if (filename_[0] == '\0') {
        sprintf(filename_, "%s", LOG_DEFAULT_FILENAME);
      }

      file_ = fopen(filename_, "a");
      if (file_ == NULL) {
        fprintf(stderr, "cannot open file : %s\n", filename_);
        fclose(file_);
        assert(file_ != NULL);
      }
      fprintf(file_, "%s", log);

      if (EOF == fclose(file_)) {
        fprintf(stderr, "fclose failed\n");
        assert(EOF != fclose(file_));
      }
    }
  }
}
ADK_END_NAMESPACE