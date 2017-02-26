/**
 * \file mem_i_arena.c
 * \ingroup adkgrp_guardalloc
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief メモリアリーナインターフェース実装部
 */

#include <core/memory_allocation/intern/mem_i_arena.h>

#include <core/debug/debug_assert.h>

/* arena types */
/**
 * \~english
 * \~japanese
 *  \brief メモリアリーナ名の最大長
 */
#define NAME_LEN (16)
/**
 * \~english
 * \~japanese
 *  \brief メモリアリーナ最大数
 */
#define MAX_ARENA (16)
/**
 * \~english
 * \~japanese
 *  \brief メモリアリーナ
 */
struct MEM_TArena {
  char name[NAME_LEN];

  struct {
    size_t min, max;
  } buffer;

  size_t (*get_buffer_size)(size_t, size_t);
  Bool (*init)(MEM_TArena *, void *, size_t, size_t);
  Bool (*term)(MEM_TArena *);

  void *(*malloc)(MEM_TArena *, size_t, size_t, const char *, ADK_TAllocMethod);
  void *(*calloc)(MEM_TArena *, size_t, size_t, const char *, ADK_TAllocMethod);
  void (*free)(MEM_TArena *, void *);

  size_t (*get_addr_size)(MEM_TArena *, void *);
  size_t (*get_arena_size)(MEM_TArena *);
  size_t (*get_header_size)(MEM_TArena *);
  size_t (*get_tot_size)(MEM_TArena *);
  size_t (*get_allocation_count)(MEM_TArena *);
  size_t (*get_tot_blocksize)(MEM_TArena *, const char *);
  size_t (*check_address)(MEM_TArena *, const void *);

  Bool (*print_info)(MEM_TArena *);
  Bool (*now_print_info)(MEM_TArena *);
  Bool (*output_memory_list_for_txt)(MEM_TArena *, const Char *, const Char *,
                                     Bool);

  void *userdata;
};
/**
 * \~english
 * \~japanese
 *  \brief メモリアリーナ情報
 */
struct TArenaInfo {
  /**
   * \~english
   * \~japanese
   *  \brief 作成されたアリーナ数
   */
  S32 arenaCount;
  /**
   * \~english
   * \~japanese
   *  \brief 作成されたアリーナ数
   */
  MEM_TArena arenas[MAX_ARENA];
} gArena;

/* arena api function definition */
MEM_TArena *MEM_CreateArena(const Char *name) {
  MEM_TArena *ret = &gArena.arenas[0];

  ADK_ASSERT_MSG(gArena.arenaCount < MAX_ARENA,
                 "ADK_GA: MEM_CreateArena(): Failed to create arena.");

  for (S32 index = 0; index < MAX_ARENA; ++index, ++ret) {
    /*
     * とりあえず一つでも関数ポインタが
     * 設定されていなければ初期化されてないとする
     */
    if (ret->get_buffer_size != 0) {
      continue;
    }
    strncpy(ret->name, name, NAME_LEN);
    ++gArena.arenaCount;
    break;
  }

  return ret;
}
Bool MEM_DisposeArena(MEM_TArena *arena) {
  Bool ret;

  ADK_ASSERT_MSG(arena->get_buffer_size != 0,
                 "ADK_GA: MEM_DisposeArena(): Not initiailized arena.");
  if (arena->get_buffer_size == 0) {
    ret = false;
  } else {
    memset(arena, 0, sizeof(MEM_TArena));
    ret = true;
  }

  return ret;
}
void MEM_SetFuncs(
    MEM_TArena *arena, size_t (*_get_buffer_size)(size_t, size_t),
    Bool (*_init)(MEM_TArena *, void *, size_t, size_t),
    Bool (*_term)(MEM_TArena *),
    void *(*_malloc)(MEM_TArena *, size_t, size_t, const char *,
                     ADK_TAllocMethod),
    void *(*_calloc)(MEM_TArena *, size_t, size_t, const char *,
                     ADK_TAllocMethod),
    void (*_free)(MEM_TArena *, void *),
    size_t (*_get_addr_size)(MEM_TArena *, void *),
    size_t (*_get_arena_size)(MEM_TArena *),
    size_t (*_get_header_size)(MEM_TArena *),
    size_t (*_get_tot_size)(MEM_TArena *),
    size_t (*_get_allocation_count)(MEM_TArena *),
    size_t (*_get_tot_blocksize)(MEM_TArena *, const char *blockname),
    size_t (*_check_address)(MEM_TArena *, const void *),
    Bool (*_print_info)(MEM_TArena *), Bool (*_now_print_info)(MEM_TArena *),
    Bool (*_output_memory_list_for_txt)(MEM_TArena *, const Char *,
                                        const Char *, Bool),
    void *userdata) {
  arena->get_buffer_size = _get_buffer_size;
  arena->init = _init;
  arena->term = _term;

  arena->malloc = _malloc;
  arena->calloc = _calloc;
  arena->free = _free;

  arena->get_addr_size = _get_addr_size;
  arena->get_arena_size = _get_arena_size;
  arena->get_header_size = _get_header_size;
  arena->get_tot_size = _get_tot_size;
  arena->get_allocation_count = _get_allocation_count;
  arena->get_tot_blocksize = _get_tot_blocksize;
  arena->check_address = _check_address;

  arena->print_info = _print_info;
  arena->now_print_info = _now_print_info;
  arena->output_memory_list_for_txt = _output_memory_list_for_txt;

  arena->userdata = userdata;
}
void *MEM_ArenaGetUserData(MEM_TArena *arena) { return arena->userdata; }
const Char *MEM_ArenaGetName(MEM_TArena *arena) { return arena->name; }

size_t MEM_GetArenaBufferSize(MEM_TArena *arena, size_t arenaSize, size_t max) {
  return (*arena->get_buffer_size)(arenaSize, max);
}
Bool MEM_InitArena(MEM_TArena *arena, void *buffer, size_t arenaSize,
                   size_t bufferSize, size_t min, size_t max) {
  arena->buffer.min = min;
  arena->buffer.max = max;

  return (*arena->init)(arena, buffer, arenaSize, bufferSize);
}
Bool MEM_TermArena(MEM_TArena *arena) { return (*arena->term)(arena); }
MEM_TArena *MEM_GetArenaFromLen(size_t len) {
  MEM_TArena *ret = &gArena.arenas[0];
  S32 index;

  ADK_ASSERT_MSG(gArena.arenaCount > 0,
                 "ADK_GA: MEM_GetArenaFromLen(): Not created arena.");

  for (index = 0; index < MAX_ARENA; ++index, ++ret) {
    /*
     * とりあえず一つでも関数ポインタが
     * 設定されていなければ初期化されてないとする
     */
    if (ret->get_buffer_size == 0) {
      continue;
    }
    if (ret->buffer.min <= len && ret->buffer.max >= len) {
      break;
    }
  }

  return ret;
}
MEM_TArena *MEM_GetArenaFromAddr(void *addr) {
  MEM_TArena *ret = &gArena.arenas[0];
  S32 index;

  ADK_ASSERT_MSG(gArena.arenaCount > 0,
                 "ADK_GA: MemGetArenaFromAddr(): Not created arena.");

  for (index = 0; index < MAX_ARENA; ++index, ++ret) {
    /*
     * とりあえず一つでも関数ポインタが
     * 設定されていなければ初期化されてないとする
     */
    if (ret->get_buffer_size == 0) {
      continue;
    }
    if (MEM_ArenaCheckAddress(ret, addr) != false) {
      return ret;
    }
  }

  return 0;
}
size_t MEM_ArenaGetMin(MEM_TArena *arena) { return arena->buffer.min; }
size_t MEM_ArenaGetMax(MEM_TArena *arena) { return arena->buffer.max; }

void *MEM_ArenaMalloc(MEM_TArena *arena, size_t size, size_t align,
                      const char *str, ADK_TAllocMethod method) {
  return (*arena->malloc)(arena, size, align, str, method);
}
void *MEM_ArenaCalloc(MEM_TArena *arena, size_t size, size_t align,
                      const char *str, ADK_TAllocMethod method) {
  return (*arena->calloc)(arena, size, align, str, method);
}
void MEM_ArenaFree(MEM_TArena *arena, void *addr) {
  (*arena->free)(arena, addr);
}

size_t MEM_ArenaGetAddrSize(MEM_TArena *arena, void *addr) {
  return (*arena->get_addr_size)(arena, addr);
}
size_t MEM_ArenaGetArenaSize(MEM_TArena *arena) {
  return (*arena->get_arena_size)(arena);
}
size_t MEM_ArenaGetHeaderSize(MEM_TArena *arena) {
  return (*arena->get_header_size)(arena);
}
size_t MEM_ArenaGetTotSize(MEM_TArena *arena) {
  return (*arena->get_tot_size)(arena);
}
size_t MEM_ArenaGetTotBlockSize(const char *blockname) {
  size_t ret = 0;
  MEM_TArena *arena = &gArena.arenas[0];
  for (S32 index = 0; index < MAX_ARENA; ++index, ++arena) {
    /*
     * とりあえず一つでも関数ポインタが
     * 設定されていなければ初期化されてないとする
     */
    if (arena->get_buffer_size == 0) {
      continue;
    }
    ret += (*arena->get_tot_blocksize)(arena, blockname);
  }

  return ret;
}
Bool MEM_ArenaCheckAddress(MEM_TArena *arena, const void *addr) {
  return (*arena->check_address)(arena, addr);
}

/* debug management functionally */
void MEM_PrintArenaInfo() {
  MEM_TArena *arena = &gArena.arenas[0];

  ADK_ASSERT_MSG(
      gArena.arenaCount > 0,
      "ADK_GA: MEM_ArenaOutputMemoryListForTXT(): Not created arena.");

  for (S32 index = 0; index < MAX_ARENA; ++index, ++arena) {
    /*
     * とりあえず一つでも関数ポインタが
     * 設定されていなければ初期化されてないとする
     */
    if (arena->get_buffer_size == 0) {
      continue;
    }
    (*arena->print_info)(arena);
  }
}
void MEM_ArenaOutputMemoryListForTXT(const Char *filename,
                                     const Char *blockname,
                                     Bool output_backtrace) {
  MEM_TArena *arena = &gArena.arenas[0];

  ADK_ASSERT_MSG(
      gArena.arenaCount > 0,
      "ADK_GA: MEM_ArenaOutputMemoryListForTXT(): Not created arena.");

  printf("%zu\n", MEM_ArenaGetTotBlockSize("jdoc"));
  printf("%zu\n", MEM_ArenaGetTotBlockSize("jconv"));

  for (S32 index = 0; index < MAX_ARENA; ++index, ++arena) {
    /*
     * とりあえず一つでも関数ポインタが
     * 設定されていなければ初期化されてないとする
     */
    if (arena->get_buffer_size == 0) {
      continue;
    }
    (*arena->output_memory_list_for_txt)(arena, filename, blockname,
                                         output_backtrace);
  }
}
void MEM_ArenaPrintMemoryInformation() {
  MEM_TArena *arena = &gArena.arenas[0];

  ADK_ASSERT_MSG(
      gArena.arenaCount > 0,
      "ADK_GA: MEM_ArenaPrintMemoryInformation(): Not created arena.");

  for (S32 index = 0; index < MAX_ARENA; ++index, ++arena) {
    /*
     * とりあえず一つでも関数ポインタが
     * 設定されていなければ初期化されてないとする
     */
    if (arena->get_buffer_size == 0) {
      continue;
    }
    (*arena->now_print_info)(arena);
  }
}
size_t MEM_ArenaGetAllocationCount() {
  size_t ret = 0;
  MEM_TArena *arena = &gArena.arenas[0];

  ADK_ASSERT_MSG(gArena.arenaCount > 0,
                 "ADK_GA: MEM_ArenaGetAllocationCount(): Not created arena.");

  for (S32 index = 0; index < MAX_ARENA; ++index, ++arena) {
    /*
     * とりあえず一つでも関数ポインタが
     * 設定されていなければ初期化されてないとする
     */
    if (arena->get_buffer_size == 0) {
      continue;
    }
    ret += (*arena->get_allocation_count)(arena);
  }

  return ret;
}

