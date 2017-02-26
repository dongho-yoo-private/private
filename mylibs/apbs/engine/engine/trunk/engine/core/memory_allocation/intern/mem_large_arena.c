/**
 * \file mem_large_arena.h
 * \ingroup adkgrp_guardalloc
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief 大きいサイズのアロケータ用アリーナ定義部
 */

#include <core/memory_allocation/intern/mem_large_arena.h>

#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <core/debug/debug_assert.h>
#include <core/memory_allocation/guarded_alloc.h>
#include <core/memory_allocation/intern/mem_block.h>
#include <core/memory_allocation/intern/mem_i_arena.h>
#include <core/memory_allocation/intern/mem_internal_alloc.h>
#include <core/library/lib_path_util-c_api.h>

#ifdef ADK_WITH_MEMORY_CALLSTACK
#define ADK_WITH_MEMORY_CALLSTACK_MAX (16)
#endif

/* header */
/*
 * \brief メモリブロックのヘッダ情報
 */
struct TMemHead {
  /* 0x00 */
  unsigned int head;
  /* 0x04 */
  struct TMemHead *next;
  /* 0x08 */
  struct TMemHead *prev;
  /* 0x0c */
  unsigned int size;
  /* 0x10 */
  unsigned char use;
  /* 0x11 */
  unsigned char method;
  /* 0x12 */
  unsigned char reserve[2];
/* 0x14 */
#ifdef ADK_WITH_MEMORY_NAME
  const char *name;
  /* 0x18 */
  const char *name_null;
/* 0x1c */
#endif /* ADK_WITH_MEMORY_NAME */
#ifdef ADK_WITH_MEMORY_BLOCK
  const char *block;
  /* 0x20 */
  const char *block_null;
/* 0x24 */
#endif /* ADK_WITH_MEMORY_BLOCK */
#ifdef ADK_WITH_MEMORY_CALLSTACK
  void *callstack[ADK_WITH_MEMORY_CALLSTACK_MAX];
  int callstack_count;
#endif
  unsigned int foot;
  /* 0x28 */
};

/* small arena */
struct LargeArena {
  /**
   * \~english
   * \~japanese
   *  \brief バッファ情報
   */
  struct {
    /**
     * \~english
     * \~japanese
     *  \brief プールが管理するバッファアドレス
     */
    void *addr;
    /**
     * \~english
     * \~japanese
     *  \brief プールが管理するバッファサイズ
     */
    size_t size;
  } buffer;
  /**
   * \~english
   * \~japanese
   *  \brief 前回アクセスしたバッファ
   */
  struct {
    void *front;
    void *enroute;
    void *back;
  } pos;
  /**
   * \~english
   * \~japanese
   *  \brief 使用状況
   */
  struct {
    /**
     * \~english
     * \~japanese
     *  \brief 使用しているサイズ
     */
    size_t size;
    /**
     * \~english
     * \~japanese
     *  \brief 使用カウント数
     */
    size_t count;
    size_t peek;
  } use;
};

/* local function implementation */
static void file_write(FILE *fp, const char *format, ...) {
  static const size_t max = 512;
  char buf[max];
  size_t len = 0;
  va_list args;

  va_start(args, format);
  len += vsprintf(buf + len, format, args);
  ADK_ASSERT_MSG(len <= max, "ADK_GA: file_write(): Out of resource.");
  va_end(args);

  fwrite(buf, len, 1, fp);
}

void *PrevMalloc(MEM_TArena *arena, size_t size, size_t align) {
  struct TMemHead *ret = 0;
  struct LargeArena *la = (struct LargeArena *)MEM_ArenaGetUserData(arena);

  /* 前回確保した領域から空き領域を探す */
  struct TMemHead *head = (struct TMemHead *)la->pos.enroute;
  for (size_t count = 0; head != 0; head = head->next, ++count) {
    if (head->use != 0) continue;
    /*
     * このヘッダが持つアドレスのアライメントが使えるものか調べる
     * 方法として、ヘッダが持つアドレスのアライメントを行い、
     * 元のアドレスと差があるかどうかで調べる
     */
    const size_t addr = (size_t)head + sizeof(struct TMemHead);
    const size_t align_addr = ADK_ALIGN(addr, align);
    if (addr != align_addr) {
      /*
       * アライメントが違うので、
       * このヘッダ内に新しくヘッダを作成できるか調べる
       */
      const size_t search_addr =
          ADK_ALIGN(addr + sizeof(struct TMemHead), align);
      const size_t end_addr = addr + head->size;
      if (end_addr <= search_addr) {
        continue;
      }
      if (end_addr - search_addr < size) {
        continue;
      }
      /*
       * 作成できるので新しい領域を作成する
       */
      struct TMemHead *htmp =
          (struct TMemHead *)(search_addr - sizeof(struct TMemHead));
      htmp->next = head->next;
      htmp->prev = head;
      htmp->size = (unsigned int)((size_t)end_addr - search_addr);
      htmp->use = 0;
#ifdef ADK_WITH_MEMORY_NAME
      htmp->name = 0;
      htmp->name_null = 0;
#endif /* ADK_WITH_MEMORY_NAME */
#ifdef ADK_WITH_MEMORY_BLOCK
      htmp->block = 0;
      htmp->block_null = 0;
#endif /* ADK_WITH_MEMORY_BLOCK */
#ifdef ADK_WITH_MEMORY_CALLSTACK
      htmp->callstack_count = 0;
#endif /* ADK_WITH_MEMORY_CALLSTACK */
      htmp->head = htmp->foot = 0xDEADC0DE;
      if (htmp->next != 0) {
        htmp->next->prev = htmp;
      }
      head->next = htmp;
      head->size = (unsigned int)((size_t)htmp - addr);
#if 0
      if (head->size==0)
      {
        if (head->prev!=0)
        {
          head->prev->size += head->size+sizeof(struct TMemHead);
          head->prev->next = head->next;
          if (head->next!=0)
          {
            head->next->prev = head->prev;
          }
          if (head==la->pos.enroute)
          {
            la->pos.enroute = head->prev;
          }
        }
      }
#endif

      head = htmp;
      if (head->size == size) {
        head->use = 1;
        ret = head;
        break;
      } else {
        /*
         * 確保しようとしているサイズが、
         * ヘッダが持つ領域より小さいなら余りに新しい空き領域を作成する
         * このとき余り領域にヘッダサイズ領域に満たない場合余り領域も一緒に返す
         */
        const size_t rest = head->size - size;
        if (rest < sizeof(struct TMemHead)) {
          head->use = 1;
          ret = head;
          break;
        } else {
          struct TMemHead *empty =
              (struct TMemHead *)((size_t)head + sizeof(struct TMemHead) +
                                  size);
          empty->head = empty->foot = 0xDEADC0DE;
          empty->prev = head;
          empty->next = head->next;
          empty->size =
              (unsigned int)(head->size - sizeof(struct TMemHead) - size);
          empty->use = 0;
#ifdef ADK_WITH_MEMORY_NAME
          empty->name = 0;
          empty->name_null = 0;
#endif /* ADK_WITH_MEMORY_NAME */
#ifdef ADK_WITH_MEMORY_BLOCK
          empty->block = 0;
          empty->block_null = 0;
#endif /* ADK_WITH_MEMORY_BLOCK */
#ifdef ADK_WITH_MEMORY_CALLSTACK
          empty->callstack_count = 0;
#endif /* ADK_WITH_MEMORY_CALLSTACK */
          if (empty->next != 0) {
            empty->next->prev = empty;
          }

          head->head = head->foot = 0xDEADC0DE;
          head->next = empty;
          head->use = 1;
          head->size = (unsigned int)size;
          ret = head;
          break;
        }
      }
    } else {
      if (head->size < size) {
        continue;
      }
      if (head->size == size) {
        head->use = 1;
        ret = head;
        break;
      } else {
        /*
         * 確保しようとしているサイズが、
         * ヘッダが持つ領域より小さいなら余りに新しい空き領域を作成する
         * このとき余り領域にヘッダサイズ領域に満たない場合余り領域も一緒に返す
         */
        const size_t rest = head->size - size;
        if (rest <= sizeof(struct TMemHead)) {
          head->use = 1;
          ret = head;
          break;
        } else {
          struct TMemHead *empty =
              (struct TMemHead *)((size_t)head + sizeof(struct TMemHead) +
                                  size);
          empty->head = empty->foot = 0xDEADC0DE;
          empty->prev = head;
          empty->next = head->next;
          empty->size =
              (unsigned int)(head->size - sizeof(struct TMemHead) - size);
          empty->use = 0;
#ifdef ADK_WITH_MEMORY_NAME
          empty->name = 0;
          empty->name_null = 0;
#endif /* ADK_WITH_MEMORY_NAME */
#ifdef ADK_WITH_MEMORY_BLOCK
          empty->block = 0;
          empty->block_null = 0;
#endif /* ADK_WITH_MEMORY_BLOCK */
#ifdef ADK_WITH_MEMORY_CALLSTACK
          empty->callstack_count = 0;
#endif /* ADK_WITH_MEMORY_CALLSTACK */
          if (empty->next != 0) {
            empty->next->prev = empty;
          }

          head->head = head->foot = 0xDEADC0DE;
          head->next = empty;
          head->use = 1;
          head->size = (unsigned int)size;
          ret = head;
          break;
        }
      }
    }
  }

  return ret;
}
void *FrontMalloc(MEM_TArena *arena, size_t size, size_t align) {
  struct TMemHead *ret = 0;
  struct LargeArena *la = (struct LargeArena *)MEM_ArenaGetUserData(arena);

  struct TMemHead *head = (struct TMemHead *)la->pos.front;
  for (; head; head = head->next) {
    if (head->use != 0) {
      continue;
    }
    /*
     * このヘッダが持つアドレスのアライメントが使えるものか調べる
     * 方法として、ヘッダが持つアドレスのアライメントを行い、
     * 元のアドレスと差があるかどうかで調べる
     */
    const size_t addr = (size_t)head + sizeof(struct TMemHead);
    const size_t align_addr = ADK_ALIGN(addr, align);
    if (addr != align_addr) {
      /*
       * アライメントが違うので、このヘッダ内に新しくヘッダを作成できるか調べる
       */
      const size_t search_addr =
          ADK_ALIGN(addr + sizeof(struct TMemHead), align);
      const size_t end_addr = addr + head->size;
      if (end_addr <= search_addr || end_addr - search_addr < size) {
        continue;
      }
      /*
       * 作成できるので新しい領域を作成する
       */
      struct TMemHead *htmp =
          (struct TMemHead *)(search_addr - sizeof(struct TMemHead));
      htmp->head = htmp->foot = 0xDEADC0DE;
      htmp->next = head->next;
      if (htmp->next != 0) {
        htmp->next->prev = htmp;
      }
      htmp->prev = head;
      htmp->size = (unsigned int)(end_addr - search_addr);
      htmp->use = 0;
#ifdef ADK_WITH_MEMORY_NAME
      htmp->name = 0;
      htmp->name_null = 0;
#endif /* ADK_WITH_MEMORY_NAME */
#ifdef ADK_WITH_MEMORY_BLOCK
      htmp->block = 0;
      htmp->block_null = 0;
#endif /* ADK_WITH_MEMORY_BLOCK */
#ifdef ADK_WITH_MEMORY_CALLSTACK
      htmp->callstack_count = 0;
#endif /* ADK_WITH_MEMORY_CALLSTACK */
      head->next = htmp;
      head->size = (unsigned int)((size_t)htmp - addr);
      head->head = head->foot = 0xDEADC0DE;

      head = htmp;
      if (head->size == size) {
        head->use = 1;
        ret = head;
        break;
      } else {
        /*
         * 確保しようとしているサイズが、
         * ヘッダが持つ領域より小さいなら余りに新しい空き領域を作成する
         * このとき余り領域にヘッダサイズ領域に満たない場合余り領域も一緒に返す
         */
        const size_t rest = head->size - size;
        if (rest < sizeof(struct TMemHead)) {
          head->use = 1;
          ret = head;
          break;
        } else {
          struct TMemHead *empty =
              (struct TMemHead *)((size_t)head + sizeof(struct TMemHead) +
                                  size);
          empty->head = empty->foot = 0xDEADC0DE;
          empty->prev = head;
          empty->next = head->next;
          if (empty->next != 0) {
            empty->next->prev = empty;
          }
          empty->size =
              (unsigned int)(head->size - sizeof(struct TMemHead) - size);
          empty->use = 0;
#ifdef ADK_WITH_MEMORY_NAME
          empty->name = 0;
          empty->name_null = 0;
#endif /* ADK_WITH_MEMORY_NAME */
#ifdef ADK_WITH_MEMORY_BLOCK
          empty->block = 0;
          empty->block_null = 0;
#endif /* ADK_WITH_MEMORY_BLOCK */
#ifdef ADK_WITH_MEMORY_CALLSTACK
          empty->callstack_count = 0;
#endif /* ADK_WITH_MEMORY_CALLSTACK */

          head->head = head->foot = 0xDEADC0DE;
          head->next = empty;
          head->use = 1;
          head->size = (unsigned int)size;
          ret = head;
          break;
        }
      }
    } else {
      if (head->size < size) {
        continue;
      }
      if (head->size == size) {
        head->use = 1;
        ret = head;
        break;
      } else {
        /*
         * 確保しようとしているサイズが、
         * ヘッダが持つ領域より小さいなら余りに新しい空き領域を作成する
         * このとき余り領域にヘッダサイズ領域に満たない場合余り領域も一緒に返す
         */
        const size_t rest = head->size - size;
        if (rest <= sizeof(struct TMemHead)) {
          head->use = 1;
          ret = head;
          break;
        } else {
          struct TMemHead *empty =
              (struct TMemHead *)((size_t)head + sizeof(struct TMemHead) +
                                  size);
          empty->head = empty->foot = 0xDEADC0DE;
          empty->prev = head;
          empty->next = head->next;
          if (empty->next != 0) {
            empty->next->prev = empty;
          }
          empty->size =
              (unsigned int)(head->size - sizeof(struct TMemHead) - size);
          empty->use = 0;
#ifdef ADK_WITH_MEMORY_NAME
          empty->name = 0;
          empty->name_null = 0;
#endif /* ADK_WITH_MEMORY_NAME */
#ifdef ADK_WITH_MEMORY_BLOCK
          empty->block = 0;
          empty->block_null = 0;
#endif /* ADK_WITH_MEMORY_BLOCK */
#ifdef ADK_WITH_MEMORY_CALLSTACK
          empty->callstack_count = 0;
#endif /* ADK_WITH_MEMORY_CALLSTACK */

          head->head = head->foot = 0xDEADC0DE;
          head->next = empty;
          head->use = 1;
          head->size = (unsigned int)size;
          ret = head;
          break;
        }
      }
    }
  }

  return ret;
}
void *BackMalloc(MEM_TArena *arena, size_t size, size_t align) {
  struct TMemHead *ret = 0;
  struct LargeArena *la = (struct LargeArena *)MEM_ArenaGetUserData(arena);

  struct TMemHead *head = (struct TMemHead *)la->pos.back;
  for (; head; head = head->prev) {
    if (head->use != 0) {
      continue;
    }
    /*
     * このヘッダが持つ領域内に確保したいサイズが収まるか調べる。
     */
    if (head->size < size) {
      continue;
    }
    /*
     * ヘッダが持つ領域の後ろから確保したいサイズのアドレスを求める。
     */
    const size_t headAreaAddr = (size_t)head + sizeof(struct TMemHead);
    const size_t headAreaEndAddr = (headAreaAddr + head->size);
    size_t addr = headAreaEndAddr - size;
    /*
     * 確保したいアドレスが、確保したいアライン位置か調べる
     */
    if (ADK_ALIGN(addr, align) != addr) {
      /*
       * アライメントしたアドレスが元と違っていたら確保したいアドレスではない
       * アドレスのアライメントサイズ分、手前から再度アラインを行い、
       * 最終的な確保したいアドレスを求める
       */
      addr -= align;
      addr = ADK_ALIGN(addr, align);
    }
    /*
     * 求めたアドレスがヘッダ領域も考慮して確保できるか調べる
     */
    ret = (struct TMemHead *)(addr - sizeof(struct TMemHead));
    if ((size_t)ret < headAreaAddr) {
      /*
       * ヘッダのが持つ領域のアドレスより手前ならサイズが満たないので、
       * このヘッダ領域では確保できない
       */
      ret = 0;
      continue;
    } else if ((size_t)ret == headAreaAddr) {
      /* ヘッダが持つ領域のアドレスと同じなら、このヘッダをそのまま使う */
      head->use = 1;
      ret = head;
      break;
    } else {
      /* それ以外なら、ヘッダと新しいヘッダで領域を分割する */
      ret->head = ret->foot = 0xDEADC0DE;
      ret->use = 1;
      ret->next = head->next;
      if (ret->next != 0) {
        ret->next->prev = ret;
      }
      ret->prev = head;
      ret->size = (unsigned int)(headAreaEndAddr - addr);
#ifdef ADK_WITH_MEMORY_NAME
      ret->name = 0;
      ret->name_null = 0;
#endif /* ADK_WITH_MEMORY_NAME */
#ifdef ADK_WITH_MEMORY_BLOCK
      ret->block = 0;
      ret->block_null = 0;
#endif /* ADK_WITH_MEMORY_BLOCK */
#ifdef ADK_WITH_MEMORY_CALLSTACK
      ret->callstack_count = 0;
#endif /* ADK_WITH_MEMORY_CALLSTACK */

      head->head = head->foot = 0xDEADC0DE;
      head->next = ret;
      head->size = (unsigned int)((size_t)ret - headAreaAddr);
      break;
    }
  }

  return ret;
}

static size_t la_get_buffer_size(size_t arenaSize, size_t max) {
  return arenaSize;
}
static Bool la_init(MEM_TArena *arena, void *buffer, size_t arenaSize,
                    size_t bufferSize) {
  Bool ret = true;
  struct LargeArena *la = (struct LargeArena *)MEM_ArenaGetUserData(arena);

  if (buffer == 0) {
    ADK_ASSERT_MSG(buffer, "ADK_GA: la_init(): Invalid variable.");
    ret = false;
  } else {
    la->buffer.addr = buffer;
    memset(la->buffer.addr, 0x00, bufferSize);
    la->buffer.size = bufferSize;

    ADK_ASSERT_MSG(la->buffer.size > sizeof(struct TMemHead),
                   "ADK_GA: la_init(): Buffer size is insufficient.");
    if (la->buffer.size <= sizeof(struct TMemHead)) {
      ret = false;
    } else {
      struct TMemHead *head = (struct TMemHead *)la->buffer.addr;
      head->head = head->foot = 0xDEADC0DE;
      head->next = 0;
      head->prev = 0;
      head->size = (unsigned int)(la->buffer.size - sizeof(struct TMemHead));
      head->use = 0;
      head->method = ADK_kPrevAlloc;
#ifdef ADK_WITH_MEMORY_NAME
      head->name = 0;
      head->name_null = 0;
#endif /* ADK_WITH_MEMORY_NAME */
#ifdef ADK_WITH_MEMORY_BLOCK
      head->block = 0;
      head->block_null = 0;
#endif /* ADK_WITH_MEMORY_BLOCK */
#ifdef ADK_WITH_MEMORY_CALLSTACK
      head->callstack_count = 0;
#endif /* ADK_WITH_MEMORY_CALLSTACK */
      la->pos.front = head;
      la->pos.enroute = head;
      la->pos.back = head;
    }
  }

  return ret;
}
static Bool la_term(MEM_TArena *arena) {
  struct LargeArena *la = (struct LargeArena *)MEM_ArenaGetUserData(arena);
  Bool ret = false;

  MEM_InternalFree(la), la = 0;

  return ret;
}

static void *la_malloc(MEM_TArena *arena, size_t size, size_t align,
                       const char *name, ADK_TAllocMethod method) {
  void *ret = 0;
  struct LargeArena *la = (struct LargeArena *)MEM_ArenaGetUserData(arena);
  struct TMemHead *hnew = 0;

#ifdef CHECK_ENROUTE
  CheckEnroute();
#endif

  size = ADK_ALIGN(size, 4); /* 最低でも4byteアラインにする */

  //  method = size>=1024*2 ? ADK_kBackAlloc : method;
  switch (method) {
    case ADK_kPrevAlloc: {
      hnew = (struct TMemHead *)PrevMalloc(arena, size, align);
      if (hnew == 0) {
        hnew = (struct TMemHead *)FrontMalloc(arena, size, align);
        if (hnew != 0) {
          hnew->method = ADK_kFrontAlloc;
        }
      } else {
        hnew->method = ADK_kPrevAlloc;
      }
      break;
    }
    case ADK_kFrontAlloc: {
      hnew = (struct TMemHead *)FrontMalloc(arena, size, align);
      if (hnew != 0) {
        hnew->method = ADK_kFrontAlloc;
      }
      break;
    }
    case ADK_kBackAlloc: {
      hnew = (struct TMemHead *)BackMalloc(arena, size, align);
      if (hnew != 0) {
        hnew->method = ADK_kBackAlloc;
      }
      break;
    }
  }

  if (hnew == 0) {
    printf("ADK_GA: Out of resource(%s)\n", MEM_ArenaGetName(arena));
    ADK_OutputMemoryListForTXT(false, 0, 0, false);
    ADK_ASSERT(false);
    ret = 0;
  } else {
#ifdef ADK_WITH_MEMORY_NAME
    hnew->name = name;
    hnew->name_null = 0;
#endif /* ADK_WITH_MEMORY_NAME */
#ifdef ADK_WITH_MEMORY_BLOCK
    hnew->block = MEM_BlockGetNowName();
    hnew->block_null = 0;
#endif /* ADK_WITH_MEMORY_BLOCK */
#ifdef ADK_WITH_MEMORY_CALLSTACK
    hnew->callstack_count =
        adk::DBG_GetBackTrace(hnew->callstack, ADK_WITH_MEMORY_CALLSTACK_MAX);
#endif /* ADK_WITH_MEMORY_CALLSTACK */
    hnew->head = hnew->foot = 0xDEADC0DE;

    if (method == ADK_kPrevAlloc) {
      la->pos.enroute = hnew->next == 0 ? la->buffer.addr : hnew->next;
    }
    if (la->pos.enroute > la->pos.back) {
      la->pos.back = la->pos.enroute;
    }

    la->use.size += size;
    la->use.count++;
    if (la->use.size > la->use.peek) {
      la->use.peek = la->use.size;
    }

    ret = (struct TMemHead *)((size_t)hnew + sizeof(struct TMemHead));
    memset(ret, 0x55, size);
  }

#ifdef CHECK_ENROUTE
  CheckEnroute();
#endif

  return ret;
}
static void *la_calloc(MEM_TArena *arena, size_t size, size_t align,
                       const char *name, ADK_TAllocMethod method) {
  void *ret;

  ret = la_malloc(arena, size, align, name, method);
  if (ret != 0) {
    memset(ret, 0, size);
  }

  return ret;
}
static void la_free(MEM_TArena *arena, void *addr) {
  struct LargeArena *la = (struct LargeArena *)MEM_ArenaGetUserData(arena);
#ifdef CHECK_ENROUTE
  CheckEnroute();
#endif

  struct TMemHead *heap = (struct TMemHead *)((size_t)addr -
                                              sizeof(struct TMemHead)),
                  *prev, *next;

  ADK_ASSERT_MSG(heap->use != 0, "ADK_GA: la_free(): Invalid variable.");
  ADK_ASSERT_MSG(heap->head == 0xDEADC0DE,
                 "ADK_GA: la_free(): Broken variable.");
  ADK_ASSERT_MSG(heap->foot == 0xDEADC0DE,
                 "ADK_GA: la_free(): Broken variable.");

  la->use.size -= heap->size;
  la->use.count--;
  heap->use = 0;
#ifdef ADK_WITH_MEMORY_NAME
  heap->name = 0;
  heap->name_null = 0;
#endif /* ADK_WITH_MEMORY_NAME */
#ifdef ADK_WITH_MEMORY_BLOCK
  heap->block = 0;
  heap->block_null = 0;
#endif /* ADK_WITH_MEMORY_BLOCK */
#ifdef ADK_WITH_MEMORY_CALLSTACK
  heap->callstack_count = 0;
#endif /* ADK_WITH_MEMORY_CALLSTACK */

  /* 開放するヒープ領域が、現在保持している最後尾より後ろのアドレスなら入れ替える
   */
  if ((intptr_t)heap > (intptr_t)la->pos.back) {
    la->pos.back = (void *)heap;
  }

  /* フリー領域のくっつけチェック */
  prev = heap->prev;
  if (prev != 0) {
    if (prev->use == 0) {
      prev->size += sizeof(struct TMemHead) + heap->size;
      prev->next = heap->next;
      if (heap->next != 0) {
        heap->next->prev = prev;
      }
      if (heap == la->pos.enroute) {
        if (la->pos.enroute == la->pos.back) {
          la->pos.back = prev;
        }
        la->pos.enroute = prev;
      }

      if (heap == la->pos.back) {
        la->pos.back = prev;
      }

      heap = prev;
    }
  }

  next = heap->next;
  if (next != 0) {
    if (next->use == 0) {
      heap->size += sizeof(struct TMemHead) + next->size;
      heap->next = next->next;
      if (next->next != 0) {
        next->next->prev = heap;
      }
      if (next == la->pos.back) {
        la->pos.back = heap;
      }
      if (next == la->pos.enroute) {
        if (la->pos.enroute == la->pos.back) {
          la->pos.back = heap;
        }
        la->pos.enroute = heap;
      }
    }
  }

#ifdef CHECK_ENROUTE
  CheckEnroute();
#endif
}

static size_t la_get_addr_size(MEM_TArena *arena, void *addr) {
  size_t ret;
  struct TMemHead *heap =
      (struct TMemHead *)((size_t)addr - sizeof(struct TMemHead));

  ret = heap->size;

  return ret;
}
static size_t la_get_arena_size(MEM_TArena *arena) {
  struct LargeArena *la = (struct LargeArena *)MEM_ArenaGetUserData(arena);

  return la->buffer.size;
}
static size_t la_get_header_size(MEM_TArena *arena) { return 0; }
static size_t la_get_tot_size(MEM_TArena *arena) {
  return la_get_arena_size(arena);
}
static size_t la_get_allocation_count(MEM_TArena *arena) {
  struct LargeArena *la = (struct LargeArena *)MEM_ArenaGetUserData(arena);
  return la->use.count;
}
static size_t la_get_tot_blocksize(MEM_TArena *arena, const char *blockname) {
  return 0;
}
static size_t la_check_address(MEM_TArena *arena, const void *addr) {
  struct LargeArena *la = (struct LargeArena *)MEM_ArenaGetUserData(arena);
  const size_t start = (size_t)la->buffer.addr;
  const size_t end = start + la->buffer.size;
  const size_t pos = (size_t)addr;
  Bool ret;

  if (pos < start || pos >= end) {
    ret = false;
  } else {
    ret = true;
  }

  return ret;
}

static Bool la_print_info(MEM_TArena *arena) { return true; }
static Bool la_now_print_info(MEM_TArena *arena) {
  struct LargeArena *la = (struct LargeArena *)MEM_ArenaGetUserData(arena);
  size_t total_size = 0, total_count = 0;
  struct TMemHead *head = (struct TMemHead *)la->buffer.addr;

  while (head != 0) {
    if (head->use) {
      total_size += head->size;
      ++total_count;
    }
    head = head->next;
  }
  printf("+--------------------------+\n");
  printf("| Memory arena info(%s)\n", MEM_ArenaGetName(arena));
  printf("+----------------------------+-------------+\n");
  printf("| total size                 | total count |\n");
  printf("+----------------------------+-------------+\n");
  printf("| 0x%08x(%10zubyte) | %11zu |\n", (U32)total_size, total_size,
         total_count);
  printf("+----------------------------+-------------+\n");

  return true;
}
static Bool la_output_memory_list_for_txt(MEM_TArena *arena,
                                          const Char *filename,
                                          const Char *blockname,
                                          Bool output_backtrace) {
  struct LargeArena *la = (struct LargeArena *)MEM_ArenaGetUserData(arena);
#if 1
  /*
   * 現在接続中のテンポラリディレクトリパスを作成
   */
  const U32 kMaxLen = 256;
  char docdir[kMaxLen], memdir[kMaxLen], path[kMaxLen];

  if (ADKLIB_GetTemporaryDir(docdir, 256) == false) {
    return false;
  }
  sprintf(memdir, "%s/memdump", docdir);
  mkdir(memdir, 0777);
  sprintf(path, "%s/memdump - %s_%s.txt", memdir,
          filename == 0 ? " " : filename, MEM_ArenaGetName(arena));
#else
  /*
   * PCのデスクトップパスを作成する(実機だとできない)
   */
  const char *home = getenv("HOME");
  ADK_ASSERT_MSG(
      home, "ADK_GA: la_output_memory_list_for_txt(): Failed to getenv().");
  const char *user = strstr(strstr(strstr(home, "/") + 1, "/") + 1, "/");
  ADK_ASSERT_MSG(
      user,
      "ADK_GA: la_output_memory_list_for_txt(): Failed to create userdir.");
  char userdir[256] = {0};
  char memdir[256] = {0};
  char path[256];

  strncpy(userdir, home, user - home);
  sprintf(memdir, "%s/Desktop/memdump", userdir);
  mkdir(memdir, 0777);
  sprintf(path, "%s/memdump-%s%s.txt", memdir, filename == 0 ? " " : filename,
          MEM_ArenaGetName(arena));
#endif
  FILE *fp = fopen(path, "wb");
  if (fp != 0) {
    printf("ADK_GA: Output MemoryDump = %s\n", path);
    struct TMemHead *head = (struct TMemHead *)la->buffer.addr;
    size_t freeSize = 0, maxFreeSize = 0, smallSize = 0, freeCount = 0,
           useSize = 0, useCount = 0;
    if (head != 0) {
      file_write(
          fp,
          "+------------------------------------------------------------+\n");
      file_write(
          fp,
          "| Print memory list                                          |\n");
      file_write(
          fp,
          "+-----+----------+----------+----------+----------+----------+\n");
      file_write(fp,
                 "| use |   addr   |   size   |   prev   |   next   |  method  "
                 "| name(block)\n");
      file_write(
          fp,
          "+-----+----------+----------+----------+----------+----------+\n");
      while (head != 0) {
        Bool is_write = true;
#ifdef ADK_WITH_MEMORY_BLOCK
        if (blockname != 0) {
          if (head->block == 0) {
            is_write = false;
          } else {
            is_write = strcmp(head->block, blockname) == 0 ? true : false;
          }
        }
#endif /* ADK_WITH_MEMORY_BLOCK */
        if (is_write != false) {
          if (head->size >= 0x100000 && head->use == false) {
            file_write(fp, "|*%4d|0x%08x|0x%08x|0x%08x|0x%08x|%s      |",
                       head->use, head, head->size, head->prev, head->next,
                       "meth");
          } else {
            file_write(fp, "|%5d|0x%08x|0x%08x|0x%08x|0x%08x|%s      |",
                       head->use, head, head->size, head->prev, head->next,
                       "meth");
          }
          file_write(fp, "[%s]", head->name == 0 ? " " : head->name);
#ifdef ADK_WITH_MEMORY_BLOCK
          file_write(fp, "(%s)", head->block == 0 ? " " : head->block);
#endif /* ADK_WITH_MEMORY_BLOCK */
          file_write(fp, "\n");
        }
        if (head->use == 0) {
          maxFreeSize = head->size > maxFreeSize ? head->size : maxFreeSize;
          freeSize += head->size;
          freeCount++;
        } else {
          useSize += head->size;
          useCount++;
          if (head->size > 64) {
            smallSize += head->size;
          }
        }
        head = head->next;
      }
      file_write(
          fp,
          "+-----+----------+----------+----------+----------+----------+\n");

      file_write(
          fp,
          "+------------------------------------------------------------+\n");
      file_write(
          fp,
          "| Print memory info                                          |\n");
      file_write(
          fp,
          "+------------------------------------------------------------+\n");
      file_write(fp, "| Tot size        : 0x%08x(%016u / %0.3fmb)\n",
                 la->buffer.size, la->buffer.size,
                 (float)la->buffer.size / 1024.0f / 1024.0f);
      file_write(fp, "| Peek size       : 0x%08x(%016u / %0.3fmb)\n",
                 la->use.peek, la->use.peek,
                 (float)la->use.peek / 1024.0f / 1024.0f);
      file_write(fp, "| Use size        : 0x%08x(%016u / %0.3fmb)\n", useSize,
                 useSize, (float)useSize / 1024.0f / 1024.0f);
      file_write(fp, "| Use count       : 0x%08x(%016u / %0.3fmb)\n", useCount,
                 useCount);
      file_write(
          fp, "| Use header size : 0x%08x(%016u / %0.3fmb)\n",
          useCount * sizeof(struct TMemHead),
          useCount * sizeof(struct TMemHead),
          (float)(useCount * sizeof(struct TMemHead)) / 1024.0f / 1024.0f);
      file_write(fp, "| Free size       : 0x%08x(%016u / %0.3fmb)\n", freeSize,
                 freeSize, (float)useSize / 1024.0f / 1024.0f);
      file_write(fp, "| Free count      : 0x%08x(%016u / %0.3fmb)\n", freeCount,
                 freeCount);
      file_write(
          fp, "| Free header size: 0x%08x(%016u / %0.3fmb)\n",
          freeCount * sizeof(struct TMemHead),
          freeCount * sizeof(struct TMemHead),
          (float)(freeCount * sizeof(struct TMemHead)) / 1024.0f / 1024.0f);
      file_write(fp, "| Peek free size  : 0x%08x(%016u / %0.3fmb)\n",
                 maxFreeSize, maxFreeSize,
                 (float)(maxFreeSize) / 1024.0f / 1024.0f);
      file_write(fp, "| Tot small size  : 0x%08x(%016u / %0.3fmb)\n", smallSize,
                 smallSize, (float)(smallSize) / 1024.0f / 1024.0f);
      file_write(
          fp,
          "+------------------------------------------------------------+\n");
    }
    fclose(fp), fp = 0;
  }

#ifdef ADK_WITH_MEMORY_CALLSTACK
  sprintf(path, "%s/memdump-%s%s_callstack.txt", memdir,
          filename == 0 ? " " : filename, _name);
  fp = fopen(path, "wb");
  if (fp != 0) {
    struct TMemHead *head = (struct TMemHead *)la->buffer.addr;
    if (head != 0) {
      while (head != 0) {
        Bool is_cs;
        if (head->name != 0) {
          if (strcmp(head->name, "libc-malloc") == 0) {
            is_cs = true;
          } else if (strcmp(head->name, "libc-calloc") == 0) {
            is_cs = true;
          } else if (strcmp(head->name, "libc-realloc") == 0) {
            is_cs = true;
          } else if (strcmp(head->name, "libc-memalign") == 0) {
            is_cs = true;
          } else if (strcmp(head->name, "C++/anonymous") == 0) {
            is_cs = true;
          } else {
            is_cs = false;
          }
        } else {
          is_cs = head->use == 0 ? false : true;
        }

        if (is_cs != false) {
          file_write(fp, "0x%08x\n", head);

          if (head->callstack_count > 0) {
            const int max = head->callstack_count;
            const void **cs = (const void **)head->callstack;
            int tab = 1;
            for (int idx = 0; idx < max; ++idx, ++cs) {
              for (int t = 0; t < tab; ++t) {
                file_write(fp, "\t");
              }

              const Char *name = adk::DBG_GetSimbolName(*cs);
              if (name == 0) {
                continue;
              }
              file_write(fp, "%s\n", name);
              ++tab;
            }
          }
        }
        head = head->next;
      }
    }
    fclose(fp), fp = 0;
  }
#endif /* ADK_WITH_MEMORY_CALLSTACK */

  return true;
}

/* arena api function implementation */
MEM_TArena *MEM_CreateLargeArena(const Char *name) {
  MEM_TArena *ret = MEM_CreateArena(name);
  ADK_ASSERT_MSG(ret,
                 "ADK_GA: MEM_CreateLargeArena(): Failed to create arena.");

  if (ret != 0) {
    struct LargeArena *la =
        (struct LargeArena *)MEM_InternalMalloc(sizeof(struct LargeArena));
    ADK_ASSERT_MSG(la, "ADK_GA: MEM_CreateLargeArena(): Out of resource.");
    memset(la, 0, sizeof(struct LargeArena));

    MEM_SetFuncs(ret, la_get_buffer_size, la_init, la_term, la_malloc,
                 la_calloc, la_free, la_get_addr_size, la_get_arena_size,
                 la_get_header_size, la_get_tot_size, la_get_allocation_count,
                 la_get_tot_blocksize, la_check_address, la_print_info,
                 la_now_print_info, la_output_memory_list_for_txt, la);
  }

  return ret;
}

