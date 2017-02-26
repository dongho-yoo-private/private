/**
 * \file lib_object_list.cc
 * \ingroup adkgrp_library
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *   \brief オブジェクトリスト
 */

#include <core/library/lib_object_list.h>

ADK_BEGIN_NAMESPACE

/* extern function definition */
Bool ObjectListAddHead(TListBase *lb, IObject *obj) {
  if (lb == 0) {
    return false;
  }
  if (obj == 0) {
    return false;
  }

  if (obj->GetNext() != 0 || obj->GetPrev() != 0) {
    return false;
  }

  obj->SetNext((IObject *)lb->first);
  obj->SetPrev(0);

  if (lb->first != 0) {
    ((IObject *)lb->first)->SetPrev(obj);
  }
  if (lb->last == 0) {
    lb->last = obj;
  }
  lb->first = obj;

  return true;
}
Bool ObjectListAddTail(TListBase *lb, IObject *obj) {
  if (lb == 0) {
    return false;
  }
  if (obj == 0) {
    return false;
  }

  if (obj->GetNext() != 0 || obj->GetPrev() != 0) {
    return false;
  }

  obj->SetNext(0);
  obj->SetPrev((IObject *)lb->last);

  if (lb->last != 0) {
    ((IObject *)lb->last)->SetNext(obj);
  }
  if (lb->first == 0) {
    lb->first = obj;
  }
  lb->last = obj;

  return true;
}
Bool ObjectListRemLink(TListBase *lb, IObject *obj) {
  if (lb == 0) {
    return false;
  }
  if (obj == 0) {
    return false;
  }

  if (obj->GetNext() != 0) {
    obj->GetNext()->SetPrev(obj->GetPrev());
  }
  if (obj->GetPrev() != 0) {
    obj->GetPrev()->SetNext(obj->GetNext());
  }

  if (lb->last == obj) {
    lb->last = obj->GetPrev();
  }
  if (lb->first == obj) {
    lb->first = obj->GetNext();
  }

  obj->SetPrev(0);
  obj->SetNext(0);

  return true;
}
int ObjectListCount(TListBase *lb) {
  int ret = 0;

  if (lb != 0) {
    IObject *obj = (IObject *)lb->first;
    while (obj != 0) {
      ret++;
      obj = obj->GetNext();
    }
  }

  return ret;
}

ADK_END_NAMESPACE

