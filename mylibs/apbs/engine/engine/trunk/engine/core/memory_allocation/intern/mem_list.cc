/**
 * \file mem_list.cc
 * \ingroup guardalloc
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief アロケータ用メモリリスト実装部
 */

#include <core/memory_allocation/intern/mem_list.h>

/* local function implementation */
static ADK_INLINE void *ListNext(void *t) {
  return *(reinterpret_cast<void **>(t));
}

static ADK_INLINE void ListSetNext(void *t, void *n) {
  *(reinterpret_cast<void **>(t)) = n;
}

/* list function implementation */
void MEM_ListPush(void **list, void *element) {
  ListSetNext(element, *list);
  *list = element;
}
void *MEM_ListPop(void **list) {
  void *result = *list;
  *list = ListNext(*list);
  return result;
}
Bool MEM_IsList(void *list) { return list == 0 ? false : true; }

