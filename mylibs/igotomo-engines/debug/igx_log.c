#include "debug/igx_debug.h"
#include "debug/igx_log.h"
#include "library/igx_time.h"
#include "io/igx_file.h"
#include "thread/igx_thread.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#ifdef __POSIX__
#   include <syslog.h>
#endif

static struct {
    igx_log_output_t output;
    igx_log_level_t level;
    char path[256];
    igx_mutex_t mutex;
} g_log_info;


// 'IGX'+'[YY/MM/DD hh:mm:ss]'+'[CRITICAL]: '
#define IGX_LOG_HEADER_SIZE (3+19+11+1)
#define IGX_MAX_LOG_LEN (4096)

static const char* get_tag(igx_log_level_t level)
{
    switch(level)
    {
        case kLogLevelUser:
        return "USER";
        case kLogLevelCritical :
        return "CRITICAL";
        case kLogLevelError:
        return "ERROR";
        case kLogLevelWarn:
        return "WARN";
        case kLogLevelInfo:
        return "INFO";
        case kLogLevelDebug:
        return "DEBUG";
        default:
            IGX_ASSERT_MSG(false, "unknown type!!\n");
    }
    return "";
}

void igx_log(igx_log_level_t level, const char* format, ...)
{
    int n;
	va_list args;
    char     log[IGX_LOG_HEADER_SIZE+IGX_MAX_LOG_LEN+1];
    char     time_buffer[32];
    const char* tag="DEBUG";
    igx_time_t local_time;

    if (level==0 ||
            (level&g_log_info.level)==0)
    {
        return ;
    }
    tag = get_tag(level);
    local_time = igx_time_local_time();
    n = sprintf(log, "IGX[%s][%s] ", igx_time_to_string(time_buffer, local_time, "%02d/%02d/%02d %02d:%02d:%02d"), tag);

    va_start(args, format);
    n+=vsnprintf(log+n, sizeof(log)-n, format, args);
    va_end(args);

    if (n<sizeof(log)-2)
    {
        log[n]='\n';
        log[n+1]=0;
    }
    else
    {
        n=sizeof(log)-2;
        log[n+1]=0;
        log[n]='\n';
    }
    if (g_log_info.output==kLogOutputConsole)
    {
#if (OS_IS_ANDROID==1)
        //__android_log_print?
#elif (OS_IS_WIN32==1)
        // DEBUG_PRINT?
#else
        int fd = fileno(stdout);
        if (level==kLogLevelWarn|level==kLogLevelError|level==kLogLevelCritical)
        {
            fd = fileno(stderr);
        }
        igx_mutex_lock(&g_log_info.mutex);
        if (level==kLogLevelInfo)
        {
            dprintf(fd, "\x1b[32m");
        }
        else if (level==kLogLevelWarn)
        {
            dprintf(fd, "\x1b[33m");
        }
        else if (level==kLogLevelError||level==kLogLevelCritical)
        {
            dprintf(fd, "\x1b[31m");
        }
        dprintf(fd, "%s", log);
        igx_mutex_unlock(&g_log_info.mutex);
        dprintf(fd, "\x1b[39m");
#endif
    }
    else if (g_log_info.output==kLogOutputFile)
    {
        igx_mutex_lock(&g_log_info.mutex);
        igx_file_append(g_log_info.path, log, n);
        igx_mutex_unlock(&g_log_info.mutex);
    }
    else if (g_log_info.output==kLogOutputSyslog)
    {
       syslog(level,(const char*)log, "");
    }
}
void igx_log_init(igx_log_output_t output, igx_log_level_t level, const void* extra_data)
{
    g_log_info.level = level;
    g_log_info.output = output;
    igx_mutex_init(&g_log_info.mutex);

    if (output==kLogOutputFile)
    {
        char time_buffer[32];
        const char* dir = igx_file_writable_path();
        igx_time_t  curr = igx_time_local_time();
        sprintf(g_log_info.path, "%s/%s-%s.log", dir, extra_data, igx_time_to_string(time_buffer, curr, "%02d%02d%02d-%02d%02d%02d"));
    }
    else if (output == kLogOutputSyslog)
    {
	    openlog(extra_data, LOG_PID, LOG_LOCAL2);
    }
}

void igx_print_char(const char* buffer, size_t len)
{
    int i;
    int fd = fileno(stdout);
    for (i=0; i<len; ++i)
    {
        dprintf(fd, "%c", buffer[i]);
    }
    dprintf(fd, "\n");
}

void igx_log_dump(const char* title, const void* buffer, size_t size, size_t column_num)
{
	char* pChar = (char*)buffer;
	char bufferLine[256];
	char bufferAscii[256];
	int i=0;
	int asci=0;
    const int fd = fileno(stdout);

	dprintf(fd, "------------ dump(%s) %zd bytes ------------ \n", title, size);
	
	for (i=0; i<column_num; i++)
	{
		dprintf(fd, "%02d|", (int)i);
	}
	dprintf(fd, "\n");
	for (i=0; i<column_num; i++)
	{
		dprintf(fd, "---");
	}
	//dprintf(fd, "\n");

	bufferAscii[0]=0;
	bufferLine[0]=0;
	for (i=0; i<size; i++) 
	{
		char buffer[16];
		if (i!=0&&i%column_num==0) {
			dprintf(fd, "\n");
			dprintf(fd, "%s", bufferLine);
			dprintf(fd, "   %s", bufferAscii);
			bufferLine[0]=0;
			bufferAscii[0]=0;
			asci=0;
		}
		sprintf(buffer, "%02x|", (int)(0xFF&*pChar));
		strcat(bufferLine, buffer);

		if (*pChar==0 || *pChar=='\n' || *pChar=='\r' ||
			*pChar=='\t' || *pChar<=0x1F)
		{
			bufferAscii[asci]='.';
		}
		else if ((*pChar&0x80)==0)
			bufferAscii[asci]=*pChar;
		else
			bufferAscii[asci]='.';

		//printf(fd, "%02x|", (int)(0xFF&*pChar));
		pChar++;
		asci++;
		bufferAscii[asci]=0;
	}

	dprintf(fd, "\n");
	dprintf(fd, "%s", bufferLine);
	dprintf(fd, "   %s", bufferAscii);
	dprintf(fd, "\n");
}

