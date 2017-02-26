/**
 * \file mem_internal_alloc_ios.cc
 * \ingroup adkgrp_guardalloc
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief 内部メモリアロケータ for iOS
 *  \ingroup api
 */

#include <core/memory_allocation/intern/mem_internal_alloc.h>

#include <stdlib.h>
#include <memory.h>

#include <core/library/lib_file_operations.h>
#include <core/library/lib_json_document.h>
#include <core/library/lib_path_util.h>
#include <core/memory_allocation/guarded_alloc.h>
#include <debug_profiling/error/result_types.h>

/**
 * \~english
 * \~japanese
 *  \brief メモリ乗っ取り定義
 */
//#define MEMORY_OVERLOAD

#if defined(MEMORY_OVERLOAD) && defined(__GNUC__)

#include <new>
#include <dlfcn.h>   // RTLD_NEXT, dlsym()
#include <unistd.h>  // _exit()
#include <pthread.h>

static void init(void) __attribute__((constructor));
static void fini(void) __attribute__((destructor));

static void* (*libc_malloc)(size_t);
static void* (*libc_calloc)(size_t, size_t);
static void* (*libc_realloc)(void*, size_t);
static void* (*libc_memalign)(size_t, size_t);
static void (*libc_free)(void*);

static bool gIsInit = false;
static void init() {
  if (gIsInit != false) {
    return;
  }

  libc_malloc = (void* (*)(size_t))dlsym(RTLD_NEXT, "malloc");
  libc_calloc = (void* (*)(size_t, size_t))dlsym(RTLD_NEXT, "calloc");
  libc_realloc = (void* (*)(void*, size_t))dlsym(RTLD_NEXT, "realloc");
  libc_memalign = (void* (*)(size_t, size_t))dlsym(RTLD_NEXT, "memalign");
  libc_free = (void (*)(void*))dlsym(RTLD_NEXT, "free");

  ADK_InitAlloc();

  gIsInit = true;
}
static void fini() { gIsInit = false; }
void* malloc(size_t size) {
  init();

  void* ret;

  ret = ADK_Malloc(size, "libc-malloc");

  return ret;
}
void* calloc(size_t nmemb, size_t size) {
  init();

  void* ret = ADK_Calloc(nmemb, size, "libc-calloc");

  return ret;
}
void* realloc(void* ptr, size_t size) {
  init();

  void* ret = ADK_Realloc(ptr, size, "libc-realloc");

  return ret;
}
void* memalign(size_t alignment, size_t size) {
  init();

  void* ret = ADK_Malloc(size, "libc-memalign");

  return ret;
}
void free(void* addr) {
  if (addr) {
    ADK_Free(addr);
  }
}

#if 0
void *operator new(size_t size,const char *str) throw(std::bad_alloc) {
  init();
  return ADK_Malloc(size,str);
}
void *operator new[](size_t size,const char *str) throw(std::bad_alloc) {
  init();
  return ADK_Malloc(size,str);
}
#endif

void* operator new(size_t size) {
  init();
  return ADK_Malloc(size, "C++/anonymous");
}
void operator delete(void* p) { ADK_Free(p); }
void* operator new [](size_t size) {
  init();
  return ADK_Malloc(size, "C++/anonymous");
} void
operator delete [](void* p) { ADK_Free(p); } void*
operator new(std::size_t size, const std::nothrow_t&) {
  init();
  return ADK_Malloc(size, "C++/anonymous");
}
void operator delete(void* p, const std::nothrow_t&) { ADK_Free(p); }
void* operator new [](std::size_t size, const std::nothrow_t&) {
  init();
  return ADK_Malloc(size, "C++/anonymous");
} void
operator delete [](void* p, const std::nothrow_t&) { ADK_Free(p); }
#endif /* MEMORY_OVERLOAD && __GNUC__ */
void* MEM_InternalMalloc(size_t size) {
#if defined(MEMORY_OVERLOAD)
  return (*libc_malloc)(size);
#else
  return ::malloc(size);
#endif
}
void* MEM_InternalCalloc(size_t nmemb, size_t size) {
#if defined(MEMORY_OVERLOAD)
  return (*libc_calloc)(nmemb, size);
#else
  return ::calloc(nmemb, size);
#endif
}
void* MEM_InternalRealloc(void* ptr, size_t size) {
#if defined(MEMORY_OVERLOAD)
  return (*libc_realloc)(ptr, size);
#else
  return ::realloc(ptr, size);
#endif
}
void* MEM_InternalMemalign(size_t alignment, size_t size) {
#if defined(MEMORY_OVERLOAD)
  return (*libc_memalign)(alignment, size);
#else
  return ::memalign(alignment, size);
#endif
}
void MEM_InternalFree(void* addr) {
#if defined(MEMORY_OVERLOAD)
  (*libc_free)(addr);
#else
  ::free(addr);
#endif
}

