/**
 * \author dongho yoo
 * \ingroup adkgrp_library
 * \file lib_list.c
 * \~english
 * \~japanese
 *   \brief Linked List Library.
 */

#include <core/library/lib_list.h>
#include <core/memory_allocation/guarded_alloc.h>

ADK_BEGIN_NAMESPACE

#define yialloc(a) ADK_Malloc(a, "LIB_List")
#define yifree ADK_Free

typedef struct {
  TListNode* top;
  TListNode* tail;
  int count;
#if __MUTEX__
  HandleMutexx hMutex;
#endif
} sListc;

// -----------------------
static void* yialloci(int size) {
  void* p = (void*)yialloc(size);

  if (p) return memset(p, 0, size);
  return 0;
}

// -----------------------
static void* yitoheap(const void* data, int size) {
  void* p = (char*)yialloc(size);

  if (p) return memcpy(p, data, size);
  return 0;
}

// -----------------------
static TListNode* __getr__(sListc* s, int n) {
  int i = 0;
  TListNode* p = s->tail;

  if (p == 0) return 0;

  for (i = 0; i < n; i++) {
    p = p->pp;

    if (p == 0) return false;
  }

  return p;
}

// -----------------------
static TListNode* __get__(sListc* s, int n) {
  int i = 0;
  TListNode* p = s->top;

  if (p == 0) return 0;

  for (i = 0; i < n; i++) {
    p = p->np;

    if (p == 0) {
      return 0;
    }
  }

  return p;
}
// -----------------------
static Bool __create_node__(sListc* s, int n, /*yiAnyValue data, */ void* ref) {
  TListNode* p = (TListNode*)yialloci(sizeof(TListNode));

  if (p == 0) return false;

  if (s->top == 0) {
    p->np = 0;
    p->pp = 0;
    s->top = p;
    s->tail = p;
  } else if (n == s->count) {
    p->np = 0;
    p->pp = s->tail;
    s->tail->np = p;
    s->tail = p;
  } else if (n == 0) {
    p->pp = 0;
    p->np = s->top;
    s->top->pp = p;
    s->top = p;
  } else {
    TListNode* next = __get__(s, n);
    TListNode* prev;

    if (next == 0) return false;

    prev = next->pp;

    p->pp = prev;
    p->np = next;
    next->pp = p;
    prev->np = p;
  }

  p->ref = ref;
  s->count++;

  return true;
}

// -------------------------------
#ifdef __MUTEX__
HandleList LIB_ListCreate(Bool bUseMutex, int nSpinCount)
#endif
    HandleList LIB_ListCreate() {
  sListc s = {0, };
#if __MUTEX__
  if (bUseMutex) s.hMutex = yiMutexCreateEx(nSpinCount);
#endif

  return (HandleList)yitoheap(&s, sizeof(s));
}

// -------------------------------
void LIB_ListDestroy(HandleList hList) {
  LIB_ListClear(hList);
  yifree(hList);
}

// -------------------------------
int LIB_ListAdd(HandleList hList, void* data) {
  sListc* s = (sListc*)hList;
  return __create_node__(s, s->count, (void*)data);
}

// -------------------------------
int LIB_ListInsert(HandleList hList, int index, void* data) {
  sListc* s = (sListc*)hList;
  return __create_node__(s, index, (void*)data);
}

// -------------------------------
void LIB_ListRemove(HandleList hList, TListNode* node) {
  sListc* s = (sListc*)hList;
  // if (node==0)
  //  return ;

  TListNode* prev = node->pp;
  TListNode* next = node->np;

  if (prev == 0) {
    s->top = next;
  } else {
    prev->np = next;
  }

  if (next == 0) {
    s->tail = prev;
  } else {
    next->pp = prev;
  }

  yifree(node);

  s->count--;
  return;
}

// -------------------------------
void* LIB_ListRemoveEx(HandleList hList, int index) {
  TListNode* p = __get__((sListc*)hList, index);
  void* ret = 0;

  if (p == 0) return 0;

  ret = p->ref;
  LIB_ListRemove(hList, p);

  return ret;
}

// -------------------------------
void LIB_ListClear(HandleList hList) {
  sListc* s = (sListc*)hList;
  int i;
  int cnt = s->count;
  for (i = 0; i < cnt; i++) {
    LIB_ListRemoveEx(hList, 0);
  }
}

// -------------------------------
TListNode* LIB_ListGetTopFrom(HandleList hList, int index) {
  if (hList) {
    return __get__((sListc*)hList, index);
  }
  return 0;
}

// -------------------------------
TListNode* LIB_ListGetTailFrom(HandleList hList, int index) {
  return __getr__((sListc*)hList, index);
}

// -------------------------------
int LIB_ListCount(HandleList hList) { return ((sListc*)hList)->count; }

// -------------------------------
void** LIB_ListExport(HandleList hList) {
  TListNode* node;
  int cnt = LIB_ListCount(hList);
  int i = 1;
  void** p = (void**)yialloc(sizeof(void*) * (cnt + 1));

  FOR_LIST_BEGIN(hList, node)
  p[i++] = NODE_REF(node);
  FOR_LIST_END()

  p[0] = (void*)((size_t)cnt);
  return &p[1];
}

// -------------------------------
void LIB_ListExportDelete(void** p) {
  if (p) yifree((void*)&p[-1]);
}

/* struct */
/**
 * \~english
 * \~japanese
 *   \brief link list
 */
typedef struct ListLink {
  struct ListLink* next, *prev;
} ListLink;

/* extern function implementation */
Bool LIB_ListAddHead(LIB_TListBase* lb, void* vlink) {
  ListLink* link = (ListLink*)vlink;

  if (link == 0) {
    return false;
  }
  if (lb == 0) {
    return false;
  }

  if (link->next != 0 || link->prev != 0) {
    return false;
  }

  link->next = (ListLink*)lb->first;
  link->prev = 0;

  if (lb->first != 0) {
    ((ListLink*)lb->first)->prev = link;
  }
  if (lb->last == 0) {
    lb->last = link;
  }
  lb->first = link;

  return true;
}
Bool LIB_ListAddTail(LIB_TListBase* lb, void* vlink) {
  ListLink* link = (ListLink*)vlink;

  if (link == 0) {
    return false;
  }
  if (lb == 0) {
    return false;
  }

  if (link->next != 0 || link->prev != 0) {
    return false;
  }

  link->next = 0;
  link->prev = (ListLink*)lb->last;

  if (lb->last != 0) {
    ((ListLink*)(lb)->last)->next = link;
  }
  if (lb->first == 0) {
    lb->first = link;
  }
  lb->last = link;

  return true;
}
void LIB_ListInsertLink(LIB_TListBase* lb, void* vlinkPrev, void* vlinkNew) {
  ListLink* linkPrev = (ListLink*)vlinkPrev, *linkNew = (ListLink*)vlinkNew;

  if (linkNew == 0) {
    return;
  }
  if (lb == 0) {
    return;
  }

  if (lb->first == 0) {
    lb->first = linkNew;
    lb->last = linkNew;
    return;
  }

  if (linkPrev == 0) {
    linkNew->next = (ListLink*)lb->first;
    linkNew->prev = 0;
    linkNew->next->prev = linkNew;
    lb->first = linkNew;
    return;
  }

  if (lb->last == linkPrev) {
    lb->last = linkNew;
  }

  linkNew->next = linkPrev->next;
  linkPrev->next = linkNew;
  if (linkNew->next != 0) {
    linkNew->next->prev = linkNew;
  }
  linkNew->prev = linkPrev;
}
void LIB_ListInsertLinkAfter(LIB_TListBase* lb, void* vlinkPrev,
                             void* vlinkNew) {
  ListLink* linkPrev = (ListLink*)vlinkPrev, *linkNew = (ListLink*)vlinkNew;

  if (linkNew == 0) {
    return;
  }
  if (lb == 0) {
    return;
  }

  if (lb->first == 0) {
    lb->first = linkNew;
    lb->last = linkNew;
    return;
  }

  if (linkPrev == 0) {
    linkNew->prev = 0;
    linkNew->next = (ListLink*)lb->first;
    ((ListLink*)lb->first)->prev = linkNew;
    lb->first = linkNew;
    return;
  }

  if (lb->last == linkPrev) {
    lb->last = linkNew;
  }

  linkNew->next = linkPrev->next;
  linkNew->prev = linkPrev;
  linkPrev->next = linkNew;
  if (linkNew->next != 0) {
    linkNew->next->prev = linkNew;
  }
}
void LIB_ListInsertLinkBefore(LIB_TListBase* lb, void* vlinkNext,
                              void* vlinkNew) {
  ListLink* linkNext = (ListLink*)vlinkNext, *linkNew = (ListLink*)vlinkNew;

  if (linkNew == 0) {
    return;
  }
  if (lb == 0) {
    return;
  }

  if (lb->first == 0) {
    lb->first = linkNew;
    lb->last = linkNew;
    return;
  }

  if (linkNext == 0) {
    linkNew->prev = (ListLink*)lb->last;
    linkNew->next = 0;
    ((ListLink*)lb->last)->next = linkNew;
    lb->last = linkNew;
    return;
  }

  if (lb->first == linkNext) {
    lb->first = linkNew;
  }

  linkNew->next = linkNext;
  linkNew->prev = linkNext->prev;
  linkNext->prev = linkNew;
  if (linkNew->prev != 0) {
    linkNew->prev->next = linkNew;
  }
}
Bool LIB_ListRemLink(LIB_TListBase* lb, void* vlink) {
  ListLink* link = (ListLink*)vlink;

  if (link == 0) {
    return false;
  }
  if (lb == 0) {
    return false;
  }

  if (link->next != 0) {
    link->next->prev = link->prev;
  }
  if (link->prev != 0) {
    link->prev->next = link->next;
  }

  if (lb->last == link) {
    lb->last = link->prev;
  }
  if (lb->first == link) {
    lb->first = link->next;
  }

  return true;
}
void LIB_ListFreeLink(LIB_TListBase* lb, void* vlink) {
  ListLink* link = (ListLink*)vlink;

  if (link == 0) {
    return;
  }
  if (lb == 0) {
    return;
  }

  LIB_ListRemLink(lb, link);
  ADK_Free(link);
}
void LIB_ListFreeList(LIB_TListBase* lb) {
  ListLink* link, *next;

  if (lb == 0) {
    return;
  }

  link = (ListLink*)lb->first;
  while (link != 0) {
    next = link->next;
    ADK_Free(link);
    link = next;
  }

  lb->first = 0;
  lb->last = 0;
}
S32 LIB_ListCount(LIB_TListBase* lb) {
  ListLink* link;
  S32 nList = 0;

  if (lb != 0) {
    link = (ListLink*)lb->first;
    while (link != 0) {
      nList++;
      link = link->next;
    }
  }

  return nList;
}
void* LIB_ListFindLink(LIB_TListBase* lb, S32 linkNo) {
  ListLink* link = 0;

  if (linkNo >= 0) {
    link = (ListLink*)lb->first;
    while (link != 0 && linkNo != 0) {
      linkNo--;
      link = link->next;
    }
  }

  return link;
}
S32 LIB_ListFindIndex(LIB_TListBase* lb, void* vlink) {
  ListLink* link = 0;
  S32 linkIdx = 0;

  if (lb == 0) {
    return -1;
  }
  if (vlink == 0) {
    return -1;
  }

  link = (ListLink*)lb->first;
  while (link != 0) {
    if (link == vlink) {
      return linkIdx;
    }
    linkIdx++;
    link = link->next;
  }

  return -1;
}
void LIB_ListSortList(LIB_TListBase* lb, S32 (*compare)(void*, void*)) {
  ListLink* pCurrent = 0, *previous = 0, *next = 0;

  if (compare == 0) {
    return;
  }
  if (lb == 0) {
    return;
  }

  if (lb->first != lb->last) {
    for (previous = (ListLink*)lb->first, pCurrent = previous->next;
         pCurrent != 0; pCurrent = next) {
      next = pCurrent->next;
      previous = pCurrent->prev;
      LIB_ListRemLink(lb, pCurrent);
      while (previous != 0 && compare(previous, pCurrent) == 1) {
        previous = previous->prev;
      }
      LIB_ListInsertLinkAfter(lb, previous, pCurrent);
    }
  }
}

static ADK_INLINE void* ForwardLinkedListNext(void* t) {
  return *(reinterpret_cast<void**>(t));
}
static ADK_INLINE void ForwardLinkedListSetNext(void* t, void* n) {
  *(reinterpret_cast<void**>(t)) = n;
}
void LIB_ForwardLinkedListPush(void** list, void* element) {
  ForwardLinkedListSetNext(element, *list);
  *list = element;
}
void* LIB_ForwardLinkedListPop(void** list) {
  void* ret = *list;
  *list = ForwardLinkedListNext(*list);
  return ret;
}
Bool LIB_IsForwardLinkedList(void* list) { return list == 0 ? false : true; }

ADK_END_NAMESPACE

