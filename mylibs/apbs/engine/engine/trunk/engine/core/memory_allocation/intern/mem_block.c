/**
 * \file adk_memory_block.cc
 * \ingroup adkgrp_guardalloc
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief メモリブロッククラス定義部
 */

#include <core/memory_allocation/intern/mem_block.h>

#include <core/debug/debug_assert.h>

/**
 * \~english
 * \~japanese
 *  \brief link list
 */
typedef struct ListLink {
  struct ListLink *next, *prev;
} ListLink;

/**
 * \~english
 * \~japanese
 *  \brief メモりオブジェクト最大数
 */
#define MEMORY_OBJECT_MAX (1024 * 10)
/**
 * \~english
 * \~japanese
 *  \brief メモリオブジェクト名の最大長
 */
#define MEMORY_OBJECT_NAME_LEN (32)

/**
 * \~english
 * \~japanese
 *  \brief メモリオブジェクト群
 */
typedef struct {
  void *first, *last;
} TObjects;
/**
 * \~english
 * \~japanese
 *  \brief メモリオブジェクト
 */
typedef struct TObject {
  struct TObject *next, *prev;
  int use;
  char name[MEMORY_OBJECT_NAME_LEN];
  char nameNull;
  struct TObject *parent;
  TObjects child;
  size_t allocCount;
} TObject;
/**
 * \~english
 * \~japanese
 *  \brief メモリブロック
 */
struct MemBlock {
  TObject objects[MEMORY_OBJECT_MAX];
  TObjects object;
  TObject *nowObject;
} gBlock;

/* local function implementation */
static Bool BlockListAddTail(TObjects *lb, void *vlink) {
  ListLink *link = (ListLink *)vlink;

  if (link == 0 || lb == 0) {
    return false;
  }

  if (link->next != 0 || link->prev != 0) {
    return false;
  }

  link->next = 0;
  link->prev = (ListLink *)lb->last;

  if (lb->last != 0) {
    ((ListLink *)(lb)->last)->next = link;
  }
  if (lb->first == 0) {
    lb->first = link;
  }
  lb->last = link;

  return true;
}
/**
 * \~english
 * \~japanese
 *   \brief 空きオブジェクトを返します
 */
static TObject *GetEmptyObject() {
  TObject *object = gBlock.objects;

  for (int index = 0; index < MEMORY_OBJECT_MAX; ++index, ++object) {
    if (object->use == 0) {
      return object;
    }
  }

  return 0;
}
/**
 * \~english
 * \~japanese
 *   \brief 同じ名前のオブジェクトを返します
 */
static TObject *GetSameObject(const char *name) {
  TObject *o = gBlock.objects;

  for (int index = 0; index < MEMORY_OBJECT_MAX; ++index, ++o) {
    if (o->use == 0) {
      continue;
    }
    if (strcmp(o->name, name) == 0) {
      return o;
    }
  }

  return 0;
}

/* extern function implementation */
ADK_MEMBLOCK_HANDLE MEM_BlockStart(const char *name) {
  TObject *object;

  object = GetSameObject(name);
  if (object == 0) {
    object = GetEmptyObject();
  }

  ADK_ASSERT_MSG(object,
                 "ADK_GA: MEM_BlockStart(): Failed to get empty object.");
  object->use = 1;
  strncpy(object->name, name, MEMORY_OBJECT_NAME_LEN);
  object->parent = gBlock.nowObject;
  object->allocCount = 0;
  BlockListAddTail(
      object->parent == 0 ? &gBlock.object : &object->parent->child, object);
  gBlock.nowObject = object;

  return (ADK_MEMBLOCK_HANDLE)object;
}
Bool MEM_BlockStop(ADK_MEMBLOCK_HANDLE h) {
  TObject *object = (TObject *)h;
  ADK_ASSERT_MSG(object, "ADK_GA: MEM_BlockStop(): Invalid variable.");

  gBlock.nowObject = object->parent;

  return true;
}
const char *MEM_BlockGetNowName() {
  return gBlock.nowObject == 0 ? 0 : gBlock.nowObject->name;
}
void MEM_BlockIncrement() {
  if (gBlock.nowObject != 0) {
    gBlock.nowObject->allocCount++;
  }
}
void MEM_OutputBlockInfo() {
  TObject *object = gBlock.objects;

  for (S32 index = 0; index < MEMORY_OBJECT_MAX; ++index, ++object) {
    if (object->use == 0) {
      continue;
    }
    printf("ADK_GA: BLOCK(%s): Alloc count = %u\n", object->name,
           (unsigned int)object->allocCount);
  }
}

