/**
* Copyright (c) 2016 Applibot Inc.
* authorized by dongho.yoo.
*/
#ifndef _LIB_ITERATOR_H_
#define _LIB_ITERATOR_H_

#include "library/library_types.h"

namespace lib {
#define kErrorMagicCode (intptr_t)(0xBADC0DEF)
/**
* @brief ノード
*/
template <class T> 
struct Iterator {
    T data;
    Iterator<T>* prev;
    Iterator<T>* next;
    T& operator*()
    {
        return data;
    }
    const T& operator*() const
    {
        return data;
    }
    Iterator<T>& operator ++()
    {
        if (next==NULL)
        {
            Iterator<T> end;
            return end;
        }
        *this=*next;
        return *next;
    }
    Iterator<T>& operator --()
    {
        *this=*prev;
        return *prev;
    }
    Bool operator ==(Iterator<T>& x)
    {
        return (memcmp(this, &x, sizeof(*this))==0);
    }
    Bool operator ==(const Iterator<T>& x)
    {
        return (memcmp(this, &x, sizeof(*this))==0);
    }
    Bool operator ==(Iterator<T>& x) const
    {
        return (memcmp(this, &x, sizeof(*this))==0);
    }
    Bool operator !=(Iterator<T>& x)
    {
        return (memcmp(this, &x, sizeof(*this))!=0);
    }
    Bool operator !=(const Iterator<T>&x)
    {
        return (memcmp(this, &x, sizeof(*this))!=0);
    }
    Bool operator !=(Iterator<T>& x) const
    {
        return (memcmp(this, &x, sizeof(*this))!=0);
    }
    Bool IsBegin() 
    { 
        return (prev==0); 
    }
    Bool IsBegin() const
    { 
        return (prev==0); 
    }
    Bool IsEnd() 
    { 
        return (next==0); 
    }
    Bool IsEnd() const
    { 
        return (next==0); 
    }
    Bool IsInvalid()
    {
        return (next==kErrorMagicCode&&prev==kErrorMagicCode);
    }
    Bool IsInvalid() const
    {
        return (next==kErrorMagicCode&&prev==kErrorMagicCode);
    }
    Iterator():next(NULL),prev(NULL){}
}; /* struct Iterator */

} /* namespace lib */

#endif
