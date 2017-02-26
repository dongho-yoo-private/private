/**
 * \file dbg_stack_trace.cc
 * \ingroup adkgrp_debug
 * \author dongho.yoo
 * \ingroup debug
 * \~english
 * \~japanese
 *   \brief コールスタックに関する定義部
 */


#ifdef WITH_IOS
#  include <dlfcn.h>
#  include <execinfo.h>
#endif


#include <core/debug/stack_trace.h>


//ADK_BEGIN_NAMESPACE


//#define USE_YOO_SYSTEM


/* local function implementation ---------*/
#define xMAX_CACHE_NUM (512)

/** シンボルキャッシュリスト*/
struct __simbol_cache
{
    void*           pAddr;
    const char*     simbol;
    unsigned int    hits;
};
static  int g_listCnt;

/** シンボルキャッシュリスト*/
static struct __simbol_cache  g_SimbolCache[xMAX_CACHE_NUM];

/** シンボルを追加する*/
void xAddToList(void* pAddr, const char* simbol)
{
    /*if (g_listCnt==xMAX_CACHE_NUM)
     {
     // 一杯ならば、ソートして、半分の飽きを作る。
     int i, n = xMAX_CACHE_NUM>>1;
     xSortList();
     for (i=0; i<n; i++)
     g_sortedList[i]->pAddr=0;
     }
     */
    // とりあえず、いっぱいになったらはじく
    // これだけでも結構軽くなると思うが。。。
    if (g_listCnt==xMAX_CACHE_NUM)
    {
        return ;
    }
    
    g_SimbolCache[g_listCnt].pAddr=pAddr;
    g_SimbolCache[g_listCnt++].simbol=simbol;
    
    return ;
}


const char* xGetFromList(void* pAddr)
{
    int i=0;
    for (i=0; i<g_listCnt; i++)
    {
        if (pAddr==g_SimbolCache[i].pAddr)
        {
            return g_SimbolCache[i].simbol;
        }
    }
    
    return 0;
}


/* extern function implementation ---------*/
S32 DBG_GetBackTrace( const void *simbols[], S32 max)
{
#  ifdef USE_YOO_SYSTEM
    void** pBp=(void**)&simbols; // %ebp-4
    void**   pRetAddr;
    Dl_info info;
    int i=0;
    int failure=0;
    void* pCurrentEbp;

    pBp++; //%ebp
    
    pCurrentEbp=(void*)pBp;

    //アライメントを合わすために残りは0で埋めているので。。。
    //ゼロじゃなくなったところがスタックに保存された%ebpの値。
    
    while(*pBp==0) pBp++;
    
    // %ebpの次には戻り先のアドレス
    pRetAddr=pBp+1;
    
    i=0;
    while(*pBp!=0 && i!=max)
    {
      simbols[i]=0;
      
      if (failure==0)
      {
        simbols[i]=xGetFromList(*pRetAddr);
      }
      
      if (simbols[i]==0)
      {
        failure=1;
        if (dladdr(*pRetAddr, &info)==0)
        {
          break;
        }
        simbols[i]=(const char*)info.dli_sname;
        xAddToList(*pRetAddr, (const char*)simbols[i]);
      }
      
      pBp = (void**)*pBp;
      
      if (((unsigned int)pBp&0xFFFF0000)==0)
      {
        i++;
        break;
      }
      pRetAddr=pBp+1;
      i++;
    }
    
    return i;
#  else
#    ifdef WITH_IOS
    return backtrace( (void**)simbols, max);
#    else
    return 0;
#    endif
#  endif
}

#ifdef WITH_IOS
#  include <cxxabi.h>
#endif

const Char *DBG_GetSymbolName(const void *addr)
{
  const Char  *ret;
#ifdef WITH_IOS
  Dl_info  info;
  S32  result;

  result = dladdr( addr, &info);
  if (result==0) {
    ret = 0;
  } else {
#  if 0
    ret = info.dli_sname;
#  else
    int status = 0;
    ret = abi::__cxa_demangle(info.dli_sname,0,0,&status);
#  endif
  }
#else
    ret = NULL;
#endif

  return ret;
}


//ADK_END_NAMESPACE


