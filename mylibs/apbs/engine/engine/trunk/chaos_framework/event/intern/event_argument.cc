/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file event_argument.cc
 * \ingroup event_group
 * \~english
 * \~japanese
 *  \brief イベントで使用する引数クラス実装部
 */

#include "event/event_argument.h"

EventArgument::EventArgument()
  : type_(kUnknown){
  memset(&key_,0,sizeof(key_));
}
EventArgument::~EventArgument() {
}
void EventArgument::Release() {
  if (type_==kBinary) {
    ADK_Free(value_.bin.addr), value_.bin.addr = 0;
  } else if (type_==kString) {
    ADK_Free(value_.s.str), value_.s.str = 0;
  }
  type_ = kUnknown;
}
void EventArgument::SetS32(const Char *key,S32 value) {
  SetKey(key);
  type_ = (TType)(kNumber|kS32);
  value_.n.i.i = value;
}
void EventArgument::SetU32(const Char *key,U32 value) {
  SetKey(key);
  type_ = (TType)(kNumber|kU32);
  value_.n.u.u = value;
}
void EventArgument::SetS64(const Char *key,S64 value) {
  SetKey(key);
  type_ = (TType)(kNumber|kS64);
  value_.n.i64 = value;
}
void EventArgument::SetU64(const Char *key,U64 value) {
  SetKey(key);
  type_ = (TType)(kNumber|kU64);
  value_.n.u64 = value;
}
void EventArgument::SetDouble(const Char *key,Double value) {
  SetKey(key);
  type_ = (TType)(kNumber|kDouble);
  value_.n.d = value;
}
void EventArgument::SetString(const Char *key,const Char *value) {
  ADK_ASSERT(value);
  SetKey(key);
  type_ = kString;
  value_.s.length = strlen(value);
  value_.s.str = (Char*)ADK_Malloc(value_.s.length+1,"Argument::SetString");
  ADK_ASSERT(value_.s.str);
  memcpy(value_.s.str,value,value_.s.length);
  value_.s.str[value_.s.length] = 0;
}
void EventArgument::SetBool(const Char *key,Bool value) {
  SetKey(key);
  type_ = (TType)(kBool|(value==false?kFalse:kTrue));
  value_.b.boolean = value;
}
void EventArgument::SetBinary(const Char *key,const void *binary,size_t size) {
  ADK_ASSERT(binary);
  SetKey(key);
  type_ = kBinary;
  value_.bin.size = size;
  value_.bin.addr = (Byte*)ADK_Malloc(value_.bin.size,"Argument::Bianry");
  ADK_ASSERT(value_.bin.addr);
  memcpy(value_.bin.addr,binary,value_.bin.size);
}
void EventArgument::SetKey(const Char *name) {
  ADK_ASSERT(name);
  key_.length = strlen(name);
  ADK_ASSERT(key_.length<kKeyNameLength);
  memcpy(key_.name,name,key_.length);
  key_.name[key_.length] = '\0';
}

