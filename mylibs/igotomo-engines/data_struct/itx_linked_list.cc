/**
* Copyright (c) 2015 igotomo
* authorized by dongho.yoo.
*
* @brief ItxLinkedListのCの為のラッパー関数群
*/
#include "itx_linked_list.h"
#include "library/igx_mem_page.h"
#include "igx_linked_list.h"

extern "C" {

HITX igx_llist_init(char* buffer, size_t buffer_size)
{
    const size_t size = sizeof(ItxLinkedList<void*>);
    ItxLinkedList<void*>* list = (ItxLinkedList<void*>*)buffer;
    IGX_ASSERT(size<buffer_size);
    *list = ItxLinkedList<void*>(buffer+size, buffer_size-size);

    return (HITX)list;
}
HITX igx_llist_new(bit32_t count_per_page, bool_t use_mutex, void (*deallocator)(void*), HITX mempage)
{
    IgxMemPage* mem_page = (IgxMemPage* )mempage;
    ItxLinkedList<void*>* list;

    if (mem_page!=0)
    {
        list = new(mem_page) ItxLinkedList<void*>(count_per_page, mem_page,  use_mutex, deallocator);
    }
    else
    {
        list = IGX_NEW("igx_llist_new") ItxLinkedList<void*>(count_per_page, -1, use_mutex, deallocator);
    }
   return (HITX)list;
}
#define LLIST_CAST(list, handle) ItxLinkedList<void*>* list = (ItxLinkedList<void*>*)handle;
void igx_llist_delete(HITX handle)
{
    LLIST_CAST(list, handle);
    list->~ItxLinkedList();
    IGX_DEL(list);
}
bit32_t igx_llist_add(HITX handle, void* p, bit32_t index)
{
    LLIST_CAST(list, handle);
    return list->Add(p, index);
}
bit32_t igx_llist_insert(HITX handle, void* p, bit32_t index)
{
    LLIST_CAST(list, handle);
    return list->Insert(p, index);
}
igx_llist_node_t* igx_llist_get_top_from(HITX handle, bit32_t index)
{
    LLIST_CAST(list, handle);
    ItxNode<void*>* node = list->GetTopFrom(index);
    igx_llist_node_t* temp = (igx_llist_node_t*)node; 
    return (igx_llist_node_t*)node;
}
igx_llist_node_t* igx_llist_get_bottom_from(HITX handle, bit32_t index)
{
    LLIST_CAST(list, handle);
    return (igx_llist_node_t*)list->GetBottomFrom(index);
}
bit32_t igx_llist_remove_top_from(HITX handle, bit32_t index, void** out)
{
    LLIST_CAST(list, handle);
    return list->RemoveTopFrom(index, out);
}
bit32_t igx_llist_remove_bottom_from(HITX handle, bit32_t index, void** out)
{
    LLIST_CAST(list, handle);
    return list->RemoveBottomFrom(index, out);
}
bit32_t igx_llist_remove_node(HITX handle, igx_llist_node_t* node)
{
    LLIST_CAST(list, handle);
    return list->RemoveNode((ItxNode<void*>*)node);
}
void igx_llist_clear(HITX handle)
{
    LLIST_CAST(list, handle);
    list->Clear();
}
void igx_llist_lock(HITX handle)
{
    LLIST_CAST(list, handle);
    list->lock();
}
void igx_llist_unlock(HITX handle)
{
    LLIST_CAST(list, handle);
    list->unlock();
}
ubit32_t igx_llist_count(HITX handle)
{
    LLIST_CAST(list, handle);
    return list->Count();
}
bit32_t igx_llist_push(HITX handle, void* data)
{
    LLIST_CAST(list, handle);
    return list->Add(data);
}
bit32_t igx_llist_pop(HITX handle, void** data)
{
    LLIST_CAST(list, handle);
    return list->RemoveBottomFrom(0, data);
}
bit32_t igx_llist_enqueue(HITX handle, void* data)
{
    LLIST_CAST(list, handle);
    return list->Insert(data);
}
bit32_t igx_llist_dequeue(HITX handle, void** data)
{
    LLIST_CAST(list, handle);
    return list->RemoveBottomFrom(0, data);
}

} /* extern "C" */
