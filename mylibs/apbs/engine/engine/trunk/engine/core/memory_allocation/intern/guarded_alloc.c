/**
 * \file adk_guard_alloc.c
 * \ingroup adkgrp_guardalloc
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief ガードアロケータAPI実装部
 */

#include <pthread.h>

#include <core/debug/debug_assert.h>
#include <core/memory_allocation/guarded_alloc.h>
#include <core/memory_allocation/intern/mem_block.h>
#include <core/memory_allocation/intern/mem_i_arena.h>
#include <core/memory_allocation/intern/mem_internal_alloc.h>
#include <core/memory_allocation/intern/mem_large_arena.h>
#include <core/memory_allocation/intern/mem_small_arena.h>
#include <core/memory_allocation/intern/mem_startup.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \~english
 * \~japanese
 *  \brief デフォルトのアライメント値
 */
#define DEFAULT_ALIGN (16)

/**
 * \~english
 * \~japanese
 *  \brief メモリガードアロケータ情報
 */
struct {
  struct {
    /**
     * \~english
     * \~japanese
     *  \brief ガードアロケータの有無
     */
    U32 isValid : 1;
    /**
     * \~english
     * \~japanese
     *  \brief 強制的にLibCのメモリアロケータを使用する
     */
    U32 forceLibc : 1;
    /**
     * \~english
     * \~japanese
     *  \brief 予約領域
     */
    U32 reserve : 30;
  } option;
  pthread_mutex_t mutex;
} gGA = {{0, 1, 0}};

/**
 * \~english
 * \~japanese
 *  \brief メモリ機構のロック処理を行います
 */
static void mem_lock() { pthread_mutex_lock(&gGA.mutex); }
/**
 * \~english
 * \~japanese
 *  \brief メモリ機構のアンロック処理を行います
 */
static void mem_unlock() { pthread_mutex_unlock(&gGA.mutex); }

/* initialize & terminate */
Bool ADK_InitAlloc() {
  Bool ret = true;
  Bool result;

  memset(&gGA.option, 0, sizeof(gGA.option));
  gGA.option.isValid = 1;

  gGA.option.forceLibc = 1;
  if (MEM_InitStartup() == false) {
    result = false;
  } else {
    if (MEM_IsUse() == false) {
      result = false;
    } else {
      const S32 arenacount = MEM_StartupGetArenaCount();
      size_t totsize = 0;

      result = true;
      for (S32 index = 0; index < arenacount; ++index) {
        MEM_TArena *arena;
        MEM_TStartupArena sa;
        size_t bufsize;
        void *buffer;

        if (MEM_StartupGetArena(&sa, index) == false) {
          result = false;
          break;
        }

        if (sa.max == 0xffffffff) {
          arena = MEM_CreateLargeArena(sa.name);
          if (arena == 0) {
            printf("ADK_GA: Failed to create large arena.\n");
            result = false;
            break;
          }
        } else {
          arena = MEM_CreateSmallArena(sa.name);
          if (arena == 0) {
            printf("ADK_GA: Failed to create small arena.\n");
            result = false;
            break;
          }
        }
        bufsize = MEM_GetArenaBufferSize(arena, sa.size, sa.max);
        buffer = MEM_InternalMalloc(bufsize);
        if (buffer == 0) {
          printf("ADK_GA: Out of resource Arena.\n");
          result = false;
          break;
        }
        if (MEM_InitArena(arena, buffer, sa.size, bufsize, sa.min, sa.max) ==
            false) {
          printf("ADK_GA: Failed to initialize arena.\n");
          result = false;
          break;
        }

        totsize += MEM_ArenaGetTotSize(arena);
      }

      MEM_TermStartup();

      if (result != false) {
        MEM_PrintArenaInfo();
        printf("ADK_GA: Total buffer size = 0x%08x(%zu)\n",
               (unsigned int)totsize, totsize);
      }
    }
  }
  gGA.option.forceLibc =
      result == false ? 1 : 0; /* 起動情報の取得に失敗した場合、
                                * ガードアロケータを起動しない
                                */

  if (gGA.option.forceLibc != 0) {
    printf("ADK_GA: Use libc allocator.\n");
  } else {
    printf("ADK_GA: Use ADK Guarded Allocator.\n");
    pthread_mutex_init(&gGA.mutex, 0);
    printf("ADK_GA: Success to pthread_mutex_init().\n");
  }

  return ret;
}
Bool ADK_TermAlloc() {
  Bool ret = false;

  pthread_mutex_destroy(&gGA.mutex);

  return ret;
}

/* allocator management functionally */
void *ADK_Malloc(size_t len, const char *str) {
  void *ret;

  mem_lock();

  if (gGA.option.isValid == 0 || gGA.option.forceLibc != 0) {
    ret = MEM_InternalMalloc(len);
  } else {
    MEM_TArena *a = MEM_GetArenaFromLen(len);
    if (a == 0) {
      ADK_ASSERT_MSG(0, "ADK_GA: Failed to found arena.");
    } else {
      MEM_BlockIncrement();
      ret = MEM_ArenaMalloc(a, len, DEFAULT_ALIGN, str, ADK_kPrevAlloc);
    }
  }

  mem_unlock();

  return ret;
}
void *ADK_Calloc(size_t n, size_t len, const char *str) {
  void *ret;

  mem_lock();

  if (gGA.option.isValid == 0 || gGA.option.forceLibc != 0) {
    ret = MEM_InternalCalloc(n, len);
  } else {
    size_t tot = n * len;
    MEM_TArena *a = MEM_GetArenaFromLen(tot);
    if (a == 0) {
      ADK_ASSERT_MSG(0, "ADK_GA: Failed to found arena.");
    } else {
      MEM_BlockIncrement();
      ret = MEM_ArenaCalloc(a, tot, DEFAULT_ALIGN, str, ADK_kPrevAlloc);
    }
  }

  mem_unlock();

  return ret;
}
void *ADK_Realloc(void *ptr, size_t len, const char *str) {
  void *ret;

  if (ptr == 0) {
    ret = ADK_Malloc(len, str);
    ptr = ret;
  } else {
    mem_lock();

    if (gGA.option.isValid == 0 || gGA.option.forceLibc != 0) {
      ret = MEM_InternalRealloc(ptr, len);

      mem_unlock();
    } else {
      MEM_TArena *a = MEM_GetArenaFromLen(len);
      if (a == 0) {
        ADK_ASSERT_MSG(0, "ADK_GA: Failed to found arena.");
      } else {
        size_t prevsize = ADK_GetAddrSize(ptr);

        MEM_BlockIncrement();
        ret = MEM_ArenaMalloc(a, len, DEFAULT_ALIGN, str, ADK_kPrevAlloc);
        if (ret != 0) {
          if (len < prevsize) {
            memcpy(ret, ptr, len);
          } else {
            memcpy(ret, ptr, prevsize);
          }
        }
      }

      mem_unlock();

      ADK_Free(ptr);
    }
  }

  return ret;
}
void ADK_Free(void *addr) {
  mem_lock();

  if (gGA.option.forceLibc != 0) {
    MEM_InternalFree(addr);
  } else {
    if (addr != 0) {
      MEM_TArena *a = MEM_GetArenaFromAddr(addr);
      if (a == 0) {
        MEM_InternalFree(addr);
      } else {
        MEM_ArenaFree(a, addr);
      }
    }
  }

  mem_unlock();
}
size_t ADK_GetAddrSize(void *addr) {
  if (gGA.option.forceLibc != 0) {
    return 0;
  }

  MEM_TArena *a = MEM_GetArenaFromAddr(addr);
  ADK_ASSERT_MSG(a, "ADK_GA: Failed to found arena.");

  return MEM_ArenaGetAddrSize(a, addr);
}
void ADK_EnableGuardAllocator() {
  if (gGA.option.forceLibc != 0) {
    return;
  }

  mem_lock();
  gGA.option.isValid = 1;
  mem_unlock();
}
void ADK_DisableGuardAllocator() {
  if (gGA.option.forceLibc != 0) {
    return;
  }

  mem_lock();
  gGA.option.isValid = 0;
  mem_unlock();
}
Bool ADK_IsValid() { return gGA.option.isValid; }

/* thread safe management functionally */
void ADK_SetLockCallback(void (*lock)(void), void (*unlock)(void)) {}
void ADK_LockMemory() {}
void ADK_UnlockMemory() {}

/* debug management functionally */
void ADK_OutputMemoryListForTXT(Bool isLock, const char *filename,
                                const char *blockname, Bool output_backtrace) {
  if (gGA.option.forceLibc != 0) {
    return;
  }

  if (isLock != false) {
    mem_lock();
  }

  MEM_OutputBlockInfo();
  MEM_ArenaOutputMemoryListForTXT(filename, blockname, output_backtrace);

  if (isLock != false) {
    mem_unlock();
  }
}
Bool ADK_CheckBufferAddress(const void *addr) {
  MEM_TArena *a =
      gGA.option.forceLibc != 0 ? 0 : MEM_GetArenaFromAddr((void *)addr);

  return a == 0 ? false : true;
}
void ADK_PrintMemoryInformation() { MEM_ArenaPrintMemoryInformation(); }
ADK_MEMBLOCK_HANDLE ADK_BeginMemoryBlock(const char *name) {
  ADK_MEMBLOCK_HANDLE h;

  h = gGA.option.forceLibc != 0 ? 0 : MEM_BlockStart(name);

  return h;
}
Bool ADK_EndMemoryBlock(ADK_MEMBLOCK_HANDLE h) {
  Bool ret;

  if (gGA.option.forceLibc != 0) {
    ret = false;
  } else {
    ret = MEM_BlockStop(h);
  }

  return ret;
}
size_t ADK_GetAllocationCount() { return MEM_ArenaGetAllocationCount(); }

#ifdef __cplusplus
}
#endif /* __cplusplus */

