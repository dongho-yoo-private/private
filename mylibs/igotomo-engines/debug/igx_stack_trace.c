#include <dlfcn.h>
#include <execinfo.h>
#include <assert.h>
#include <stdarg.h>
#include "igx_stack_trace.h"
#include "igx_debug.h"

bit32_t igx_get_stacktrace(void* simbols[], bit32_t max_count)
{
#if 0
    void** p_bp=(void**)&simbols; // %ebp-4
    void**   ret_addr;
    Dl_info info;
    int i=0;
    int failure=0;
    void* current_ebp;

    p_bp++; //%ebp
    current_ebp=(void*)p_bp;

    //アライメントを合わすために残りは0で埋めているので。。。
    //ゼロじゃなくなったところがスタックに保存された%ebpの値。
    while(*p_bp==0) p_bp++;
    
    // %ebpの次には戻り先のアドレス
    ret_addr=p_bp+1;
    
    i=0;
    while(*p_bp!=0 && i!=max_count)
    {
        simbols[i]=*ret_addr;
        p_bp = (void**)*p_bp;
        if (((unsigned int)p_bp&0xFFFF0000)==0)
        {
            i++;
            break;
        }
    }
    
    return i;
#else
    return backtrace( (void**)simbols, max_count);
#endif
}
const char* igx_get_symbole_name(void* addr)
{
  const char *ret;
  Dl_info  info;
  bit32_t  result;

  result = dladdr( addr, &info);
  if( result==0)
  {
    ret = 0;
  }
  else
  {
    //int status = 0;
    //ret = abi::__cxa_demangle(info.dli_sname,0,0,&status);
    ret = info.dli_sname;
  }

  return ret;
}
void igx_print_stacktrace(void* addr[], bit32_t count, bit32_t start)
{
    bit32_t i=start;
    for (;i<count; ++i)
    {
        if (addr[i]==0)
        {
            continue;
        }
        IGX_DEBUG("%s", igx_get_symbole_name(addr[i]));
    }
}
void igx_print_current_stacktrace()
{
    void* addr[256];
    bit32_t n = igx_get_stacktrace(addr, array_sizeof(addr));
    igx_print_stacktrace(addr, n, 2);
}
void igx_assert(bool_t condition, const char* format, ...)
{
    if (condition==0)
    {
        void* stacks[128];
        int n, i;
	    va_list args;
        va_start(args, format);

        IGX_CRITICAL("=========== ASSERT!! ============");
        IGX_CRITICAL(format, args);
        IGX_CRITICAL("=========== BACKTRACE!! ============");
        n = igx_get_stacktrace(stacks, 128);
        for (i=2; i<n;++i)
        {
            IGX_CRITICAL("%s", igx_get_symbole_name(stacks[i]));
        }
        IGX_CRITICAL("====================================");
        va_end(args);
        assert(false);
    }
}
