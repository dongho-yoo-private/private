/**
* Copyright (c) 2015 Igotomo.
* authorized by dongho.yoo.
*/
#ifndef __ITX_NODE_H__
#define __ITX_NODE_H__

/**
* @brief ノード
*/
template <class T> 
struct ItxNode {
    T data;
    ItxNode<T>* prev;
    ItxNode<T>* next;
    T& operator*()
    {
        return data;
    }
    ItxNode<T>& operator ++()
    {
        *this=*next;
        return *next;
    }
    ItxNode<T>& operator --()
    {
        *this=*prev;
        return *prev;
    }
    bool_t IsBegin() 
    { 
        return (prev==0); 
    }
    bool_t IsEnd() 
    { 
        return (prev==0); 
    }

};

#endif
