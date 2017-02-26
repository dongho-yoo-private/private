/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file event.h
 * \ingroup event_group
 * \~english
 * \~japanese
 *  \brief アプリケーションで作成されるイベント実装部
 */

#include "event/event.h"

CHAOS_BEGIN_NAMESPACE

Event::Event()
  : type_(kUnknown),
    argument_count_(0) {
}
Event::~Event() {
}

void Event::Release() {
  EventArgument *argument = arguments_;
  for(S32 count=0;count<kArgumentMax;++count,++argument) {
    argument->Release();
  }
  argument_count_ = 0;
}
bool Event::Clone(Event *event) {
  bool ret = true;

  return ret;
}
void Event::AddS32(const Char *key,S32 value) {
  EventArgument *event = EmptyArgument();
  event->SetS32(key,value);
}
void Event::AddU32(const Char *key,U32 value) {
  EventArgument *event = EmptyArgument();
  event->SetU32(key,value);
}
void Event::AddS64(const Char *key,S64 value) {
  EventArgument *event = EmptyArgument();
  event->SetS64(key,value);
}
void Event::AddU64(const Char *key,U64 value) {
  EventArgument *event = EmptyArgument();
  event->SetU64(key,value);
}
void Event::AddDouble(const Char *key,Double value) {
  EventArgument *event = EmptyArgument();
  event->SetDouble(key,value);
}
void Event::AddString(const Char *key,const Char *value) {
  EventArgument *event = EmptyArgument();
  event->SetString(key,value);
}
void Event::AddBool(const Char *key,Bool value) {
  EventArgument *event = EmptyArgument();
  event->SetBool(key,value);
}
void Event::AddBinary(const Char *key,const void *binary,size_t size) {
  EventArgument *event = EmptyArgument();
  event->SetBinary(key,binary,size);
}
const EventArgument* Event::KeyForArgument(const Char *key) const {
  ADK_ASSERT(key);
  const EventArgument *argument = arguments_;
  size_t key_length = strlen(key);
  for(S32 count=0;count<argument_count_;++count,++argument) {
    if (arguments_[count].GetKeyLength()==key_length &&
        strcmp(argument->GetKey(),key)==0) {
      return &arguments_[count];
    }
  }

  return 0;
}
EventArgument *Event::EmptyArgument() {
  EventArgument *ret = &arguments_[argument_count_];
  ADK_ASSERT(argument_count_<kArgumentMax);
  ++argument_count_;
  return ret;
}

CHAOS_END_NAMESPACE

