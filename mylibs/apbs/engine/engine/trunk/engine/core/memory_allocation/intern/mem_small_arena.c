/**
 * \file mem_small_arena.h
 * \ingroup adkgrp_guardalloc
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief 小さいサイズのアロケータ用アリーナ定義部
 */

#include <core/memory_allocation/intern/mem_small_arena.h>

#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <inttypes.h>

#include <core/debug/debug_assert.h>
#include <core/debug/stack_trace.h>
#include <core/memory_allocation/guarded_alloc.h>
#include <core/memory_allocation/intern/mem_block.h>
#include <core/memory_allocation/intern/mem_internal_alloc.h>
#include <core/memory_allocation/intern/mem_internal_alloc.h>
#include <core/memory_allocation/intern/mem_list.h>
#include <core/library/lib_path_util-c_api.h>

#define WITH_STACKTRACE
#define MAX_BACKTRACE (64)

/* header */
/**
 * \brief メモリブロックのヘッダ情報
 */
struct TMemHead {
  /* 0x00 */
  void *next;
#ifdef ADK_WITH_MEMORY_NAME
  /* 0x04 */
  const char *name;
  /* 0x08 */
  const char *name_null;
#endif /* ADK_WITH_MEMORY_NAME */
#ifdef ADK_WITH_MEMORY_BLOCK
  /* 0x0c */
  const char *block;
  /* 0x10 */
  const char *block_null;
/* 0x14 */
#endif /* ADK_WITH_MEMORY_BLOCK */
  unsigned int size;
/* 0x18 */
#ifdef WITH_STACKTRACE
  const void *symbols[MAX_BACKTRACE];
  int symbol_count;
#endif /* WITH_STACKTRACE */
};

/* small arena */
struct SmallArena {
  /**
   * \~english
   * \~japanese
   *  \brief バッファ情報
   */
  struct {
    void *addr;
    size_t size;
    struct {
      size_t count;
      size_t min;
      size_t max;
    } span;
  } buffer;
  /**
   * \~english
   * \~japanese
   *  \brief ヘッダ情報
   */
  struct {
    void *addr;
    size_t size;
    void *freelist;
  } header;
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
#define xIS_NUMERIC(a) (a >= '0' && a <= '9')
char *ConvSimbolToCpp(char *dest, char *src) {
  const char *_cppPrefix = "_Z";
  const short cppPrefix = *((short *)_cppPrefix);
  short cppSrc = *((short *)src);
  char *p;
  int offset = 0;

  if (cppSrc != cppPrefix) {
    return src;
  }
  p = (char *)&src[2];  // _Z

  // class or namespace
  if (*p == 'N') {
    p++;
    while (*p) {
      char no[16];
      int len = 0;
      if (xIS_NUMERIC(*p)) {
        int i = 0;
        while (xIS_NUMERIC(*p)) {
          no[i++] = *p;
          p++;
        }
        no[i] = 0;
        len = atoi(no);
      } else {
        // 最後かな？
        break;
      }

      if (len != 0) {
        if (offset != 0) {
          memcpy(&dest[offset], "::", 2);
          offset += 2;
        }
        memcpy(&dest[offset], p, len);
        offset += len;
        p += len;
        dest[offset] = 0;
        continue;
      }

      // invalid format.
      return src;
    }  // while (*p)
  } else {
    //多分、そのままでよい。
    strcpy(dest, p);
  }

  return dest;
}
static void file_write(FILE *fp, const char *format, ...) {
  static const size_t max = 4096;
  char buf[max];
  size_t len = 0;
  va_list args;

  va_start(args, format);
  len += vsprintf(buf + len, format, args);
  if (len >= max) {
    ADK_ASSERT_MSG(len <= max, "ADK_GA: file_write(): Out of resource.");
  }
  va_end(args);

  fwrite(buf, len, 1, fp);
}

static size_t GetBufferAddr(MEM_TArena *arena, void *head) {
  size_t ret;
  struct SmallArena *sa = (struct SmallArena *)MEM_ArenaGetUserData(arena);
  size_t pos =
      ((size_t)head - (size_t)sa->header.addr) / sizeof(struct TMemHead);

  ret = (size_t)sa->buffer.addr + (sa->buffer.span.max * pos);

  return ret;
}

static size_t sa_get_buffer_size(size_t arena_size, size_t max) {
  const size_t head_count = arena_size / max;
  const size_t headSize = head_count * sizeof(struct TMemHead);

  return arena_size + headSize;
}
static Bool sa_init(MEM_TArena *arena, void *buffer, size_t arena_size,
                    size_t bufferSize) {
  Bool ret = true;
  struct SmallArena *sa = (struct SmallArena *)MEM_ArenaGetUserData(arena);

  if (buffer == 0) {
    ADK_ASSERT_MSG(buffer, "ADK_GA: sa_init(): Invalid variable.");
    ret = false;
  } else {
    sa->buffer.size = arena_size;
    sa->buffer.addr = buffer;
    sa->buffer.span.min = MEM_ArenaGetMin(arena);
    sa->buffer.span.max = MEM_ArenaGetMax(arena);
    sa->buffer.span.count = sa->buffer.size / sa->buffer.span.max;
    memset(sa->buffer.addr, 0x00, sa->buffer.size);

    sa->header.addr = (void *)((size_t)sa->buffer.addr + arena_size);
    sa->header.size = sizeof(struct TMemHead) * sa->buffer.span.count;
    memset(sa->header.addr, 0, sa->header.size);
    struct TMemHead *head =
        &((struct TMemHead *)sa->header.addr)[sa->buffer.span.count - 1];
    for (size_t count = 0; count < sa->buffer.span.count; ++count, --head) {
      MEM_ListPush(&sa->header.freelist, head);
    }
  }

  return ret;
}
static Bool sa_term(MEM_TArena *arena) {
  struct SmallArena *sa = (struct SmallArena *)MEM_ArenaGetUserData(arena);
  Bool ret = false;

  MEM_InternalFree(sa), sa = 0;

  MEM_DisposeArena(arena);

  return ret;
}
static void *sa_malloc(MEM_TArena *arena, size_t size, size_t align,
                       const char *name, ADK_TAllocMethod method) {
  void *ret = 0;
  struct SmallArena *sa = (struct SmallArena *)MEM_ArenaGetUserData(arena);
  struct TMemHead *hnew =
      sa->header.freelist == 0
          ? 0
          : (struct TMemHead *)MEM_ListPop(&sa->header.freelist);
  if (hnew == 0) {
    printf("Out of resource(%s)\n", MEM_ArenaGetName(arena));
    ADK_OutputMemoryListForTXT(false, "no_empty", 0, false);
    ADK_ASSERT(hnew != 0);
    ret = 0;
  } else {
    hnew->next = 0;
#ifdef ADK_WITH_MEMORY_NAME
    hnew->name = name;
    hnew->name_null = 0;
#endif /* ADK_WITH_MEMORY_NAME */
#ifdef ADK_WITH_MEMORY_BLOCK
    hnew->block = MEM_BlockGetNowName();
    hnew->block_null = 0;
#endif /* ADK_WITH_MEMORY_BLOCK */
#ifdef WITH_STACKTRACE
    hnew->symbol_count = DBG_GetBackTrace(hnew->symbols, MAX_BACKTRACE);
#endif /* WITH_STACKTRACE */
    hnew->size = (unsigned int)size;

    sa->use.size += sa->buffer.span.max;
    sa->use.count++;
    if (sa->use.size > sa->use.peek) {
      sa->use.peek = sa->use.size;
    }

    ret = (void *)GetBufferAddr(arena, hnew);
    memset(ret, 0x55, sa->buffer.span.max);
  }

  return ret;
}
static void *sa_calloc(MEM_TArena *arena, size_t size, size_t align,
                       const char *name, ADK_TAllocMethod method) {
  void *ret;

  ret = sa_malloc(arena, size, align, name, method);
  if (ret != 0) {
    memset(ret, 0, size);
  }

  return ret;
}
static void sa_free(MEM_TArena *arena, void *addr) {
  struct SmallArena *sa = (struct SmallArena *)MEM_ArenaGetUserData(arena);
  const size_t pos =
      ((size_t)addr - (size_t)sa->buffer.addr) / sa->buffer.span.max;
  struct TMemHead *head = (struct TMemHead *)((size_t)sa->header.addr +
                                              (pos * sizeof(struct TMemHead)));

  ADK_ASSERT_MSG(head->next == 0, "ADK_GA: sa_free(): Invalid variable.");

#ifdef ADK_WITH_MEMORY_NAME
  head->name = 0;
  head->name_null = 0;
#endif /* ADK_WITH_MEMORY_NAME */
#ifdef ADK_WITH_MEMORY_BLOCK
  head->block = 0;
  head->block_null = 0;
#endif /* ADK_WITH_MEMORY_BLOCK */
#ifdef WITH_STACKTRACE
  head->symbol_count = 0;
#endif /* WITH_STACKTRACE */
  MEM_ListPush(&sa->header.freelist, head);

  sa->use.size -= sa->buffer.span.max;
  sa->use.count--;
}

static size_t sa_get_addr_size(MEM_TArena *arena, void *addr) {
  return MEM_ArenaGetMax(arena);
}
static size_t sa_get_arena_size(MEM_TArena *arena) {
  struct SmallArena *sa = (struct SmallArena *)MEM_ArenaGetUserData(arena);
  return sa->buffer.size;
}
static size_t sa_get_header_size(MEM_TArena *arena) {
  struct SmallArena *sa = (struct SmallArena *)MEM_ArenaGetUserData(arena);
  return sa->header.size;
}
static size_t sa_get_tot_size(MEM_TArena *arena) {
  return sa_get_arena_size(arena) + sa_get_header_size(arena);
}
static size_t sa_get_allocation_count(MEM_TArena *arena) {
  struct SmallArena *sa = (struct SmallArena *)MEM_ArenaGetUserData(arena);
  return sa->use.count;
}
static size_t sa_get_tot_blocksize(MEM_TArena *arena, const char *blockname) {
  size_t ret = 0;
  struct SmallArena *sa = (struct SmallArena *)MEM_ArenaGetUserData(arena);
  struct TMemHead *head = (struct TMemHead *)sa->header.addr;
  if (head != 0) {
    const size_t end = (size_t)sa->header.addr + sa->header.size;
    while ((size_t)head < end) {
#ifdef ADK_WITH_MEMORY_BLOCK
      if (head->next == 0 && head->block != 0) {
        if (strcmp(head->block, blockname) == 0) {
          ret += head->size;
        }
      }
#endif /* ADK_WITH_MEMORY_BLOCK */
      ++head;
    }
  }

  return ret;
}
static size_t sa_check_address(MEM_TArena *arena, const void *addr) {
  struct SmallArena *sa = (struct SmallArena *)MEM_ArenaGetUserData(arena);
  const size_t start = (size_t)sa->buffer.addr;
  const size_t end = start + sa->buffer.size;
  const size_t pos = (size_t)addr;
  Bool ret;

  if (pos < start || pos >= end) {
    ret = false;
  } else {
    ret = true;
  }

  return ret;
}

static Bool sa_print_info(MEM_TArena *arena) {
  struct SmallArena *sa = (struct SmallArena *)MEM_ArenaGetUserData(arena);

  printf("+---------------------------------------------------+\n");
  printf("| Memory arena info(%s)\n", MEM_ArenaGetName(arena));
  printf("+------------+------------+------------+------------+\n");
  printf("| arena size | arena addr | head size  | head addr  |\n");
  printf("+------------+------------+------------+------------+\n");
  printf("| 0x%08x | 0x%08x | 0x%08x | 0x%08x |\n",
         (unsigned int)sa->buffer.size, (unsigned int)((size_t)sa->buffer.addr),
         (unsigned int)sa->header.size,
         (unsigned int)((size_t)sa->header.addr));
  printf("+------------+------------+------------+------------+\n");

  return true;
}
static Bool sa_now_print_info(MEM_TArena *arena) {
  struct SmallArena *sa = (struct SmallArena *)MEM_ArenaGetUserData(arena);
  size_t total_size = sa->use.count * sa->buffer.span.max;

  printf("+--------------------------+\n");
  printf("| Memory arena info(%s)\n", MEM_ArenaGetName(arena));
  printf("+----------------------------+-------------+\n");
  printf("| total size                 | total count |\n");
  printf("+----------------------------+-------------+\n");
  printf("| 0x%08x(%10zubyte) | %11zu |\n", (U32)total_size, total_size,
         sa->use.count);
  printf("+----------------------------+-------------+\n");

  return true;
}
#if 1
static Bool sa_output_memory_list_for_txt(MEM_TArena *arena,
                                          const Char *filename,
                                          const Char *blockname,
                                          Bool output_backtrace) {
  struct SmallArena *sa = (struct SmallArena *)MEM_ArenaGetUserData(arena);
  const U32 kMaxLen = 512;
  char docdir[kMaxLen], memdir[kMaxLen], path[kMaxLen];

  if (ADKLIB_GetTemporaryDir(docdir, 256) == false) {
    return false;
  }
  sprintf(memdir, "%s/memdump", docdir);
  mkdir(memdir, 0777);
  sprintf(path, "%s/memdump - %s_%s.txt", memdir,
          filename == 0 ? " " : filename, MEM_ArenaGetName(arena));

  FILE *fp = fopen(path, "wb");
  if (fp != 0) {
    printf("ADK_GA: Output MemoryDump = %s\n", path);
    struct TMemHead *head = (struct TMemHead *)sa->header.addr;
    if (head != 0) {
      const size_t end = (size_t)sa->header.addr + sa->header.size;
      size_t freeSize = 0, freeCount = 0, useSize = 0, useCount = 0;
      file_write(fp, "+---------------------------+\n");
      file_write(fp, "| Print memory list         |\n");
      file_write(fp, "+-----+----------+----------+--+\n");
      file_write(fp, "| use |   size   |   addr   | [name] (block)\n");
      file_write(fp, "+-----+----------+----------+--+\n");
      while ((size_t)head < end) {
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
          file_write(fp, "|    %d|0x%08x|0x%08x|", head->next == 0 ? 1 : 0,
                     head->size, GetBufferAddr(arena, head));
#ifdef ADK_WITH_MEMORY_NAME
          file_write(fp, "[%s]", head->name == 0 ? " " : head->name);
#endif /* ADK_WITH_MEMORY_NAME */
#ifdef ADK_WITH_MEMORY_BLOCK
          if (head->block == 0) {
            file_write(fp, "( )");
          } else {
            file_write(fp, "(%s)", head->block);
          }
#endif /* ADK_WITH_MEMORY_BLOCK */
#ifdef WITH_STACKTRACE
          if (output_backtrace != false) {
            file_write(fp, " : ");
            for (int count = 0; count < head->symbol_count; ++count) {
              const char *symbol = DBG_GetSymbolName(head->symbols[count]);
              if (symbol != 0) {
#if 0
                char demangle_name[128];
                ConvSimbolToCpp(demangle_name,symbol);
                file_write(fp," %s <-",demangle_name);
#else
                file_write(fp, " %s <-", symbol);
                free((char *)symbol);
#endif
              }
            }
          }
#endif /* WITH_STACKTRACE */
          file_write(fp, "\n");
        }
        if (head->next == 0) {
          useSize += sa->buffer.span.max;
          useCount++;
        } else {
          freeSize += sa->buffer.span.max;
          freeCount++;
        }
        head++;
      }
      file_write(fp, "+-----+----------+--+\n");

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
                 sa->buffer.size, sa->buffer.size,
                 (float)sa->buffer.size / 1024.0f / 1024.0f);
      file_write(fp, "| Use size        : 0x%08x(%016u / %0.3fmb)\n", useSize,
                 useSize, (float)useSize / 1024.0f / 1024.0f);
      file_write(fp, "| Use count       : 0x%08x(%016u / %0.3fmb)\n", useCount,
                 useCount);
      file_write(fp, "| Free size       : 0x%08x(%016u / %0.3fmb)\n", freeSize,
                 freeSize, (float)useSize / 1024.0f / 1024.0f);
      file_write(fp, "| Free count      : 0x%08x(%016u / %0.3fmb)\n", freeCount,
                 freeCount);
      file_write(
          fp,
          "+------------------------------------------------------------+\n");
    }
    fclose(fp), fp = 0;
  }

  return true;
}
#else
static Bool sa_output_memory_list_for_txt(MEM_TArena *arena,
                                          const Char *filename) {
  struct SmallArena *sa = (struct SmallArena *)MEM_ArenaGetUserData(arena);
  const U32 kMaxLen = 256;
  char docdir[kMaxLen], memdir[kMaxLen], path[kMaxLen];

  if (ADKLIB_GetTemporaryDir(docdir, 256) == false) return false;
  sprintf(memdir, "%s/memdump", docdir);
  mkdir(memdir, 0777);
  sprintf(path, "%s/memdump - %s_%s.txt", memdir,
          filename == 0 ? " " : filename, MEM_ArenaGetName(arena));

  FILE *fp = fopen(path, "wb");
  if (fp != 0) {
    printf("ADK_GA: Output MemoryDump = %s\n", path);
    struct TMemHead *head = (struct TMemHead *)sa->header.addr;
    if (head != 0) {
      const size_t end = (size_t)sa->header.addr + sa->header.size;
      size_t freeSize = 0, freeCount = 0, useSize = 0, useCount = 0;
      file_write(fp, "+---------------------------+\n");
      file_write(fp, "| Print memory list         |\n");
      file_write(fp, "+-----+----------+----------+--+\n");
      file_write(fp, "| use |   size   |   addr   | name(block)\n");
      file_write(fp, "+-----+----------+----------+--+\n");
      while ((size_t)head < end) {
#ifdef ADK_WITH_MEMORY_NAME
#ifdef ADK_WITH_MEMORY_BLOCK
        file_write(fp, "%d|%5d|0x%08x|%s(%s)\n", head->next == 0 ? 1 : 0,
                   head->size, GetBufferAddr(arena, head),
                   head->name == 0 ? "null" : head->name,
                   head->block == 0 ? "block" : head->block);
#else  /* ADK_WITH_MEMORY_BLOCK */
        file_write(fp, "%d|%5d|0x%08x|%s\n", head->next == 0 ? 1 : 0,
                   head->size, GetBufferAddr(arena, head),
                   head->name == 0 ? "null" : head->name);
#endif /* ADK_WITH_MEMORY_BLOCK */
#else  /* ADK_WITH_MEMORY_NAME */
#ifdef ADK_WITH_MEMORY_BLOCK
        file_write(fp, "%d|%5d|0x%08x|(%s)\n", head->next == 0 ? 1 : 0,
                   head->size, GetBufferAddr(arena, head),
                   head->block == 0 ? "block" : head->block);
#else  /* ADK_WITH_MEMORY_BLOCK */
        file_write(fp, "%d|%5d|0x%08x|\n", head->next == 0 ? 1 : 0, head->size,
                   GetBufferAddr(arena, head));
#endif /* ADK_WITH_MEMORY_BLOCK */
#endif /* ADK_WITH_MEMORY_NAME */
        if (head->next == 0) {
          useSize += sa->buffer.span.max;
          useCount++;
        } else {
          freeSize += sa->buffer.span.max;
          freeCount++;
        }
        head++;
      }
      file_write(fp, "+-----+----------+--+\n");

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
                 sa->buffer.size, sa->buffer.size,
                 (float)sa->buffer.size / 1024.0f / 1024.0f);
      file_write(fp, "| Use size        : 0x%08x(%016u / %0.3fmb)\n", useSize,
                 useSize, (float)useSize / 1024.0f / 1024.0f);
      file_write(fp, "| Use count       : 0x%08x(%016u / %0.3fmb)\n", useCount,
                 useCount);
      file_write(fp, "| Free size       : 0x%08x(%016u / %0.3fmb)\n", freeSize,
                 freeSize, (float)useSize / 1024.0f / 1024.0f);
      file_write(fp, "| Free count      : 0x%08x(%016u / %0.3fmb)\n", freeCount,
                 freeCount);
      file_write(
          fp,
          "+------------------------------------------------------------+\n");
    }
    fclose(fp), fp = 0;
  }

  return true;
}
#endif

/* arena api function implementation */
MEM_TArena *MEM_CreateSmallArena(const Char *name) {
  MEM_TArena *ret = MEM_CreateArena(name);
  ADK_ASSERT_MSG(ret,
                 "ADK_GA: MEM_CreateSmallArena(): Failed to create arena.");

  if (ret != 0) {
    struct SmallArena *sa =
        (struct SmallArena *)MEM_InternalMalloc(sizeof(struct SmallArena));
    ADK_ASSERT_MSG(sa, "ADK_GA: MEM_CreateSmallArena(): Out of resource.");
    memset(sa, 0, sizeof(struct SmallArena));

    MEM_SetFuncs(ret, sa_get_buffer_size, sa_init, sa_term, sa_malloc,
                 sa_calloc, sa_free, sa_get_addr_size, sa_get_arena_size,
                 sa_get_header_size, sa_get_tot_size, sa_get_allocation_count,
                 sa_get_tot_blocksize, sa_check_address, sa_print_info,
                 sa_now_print_info, sa_output_memory_list_for_txt, sa);
  }

  return ret;
}

