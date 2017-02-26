#include "igx_thread.h"
#include "debug/igx_debug.h"

namespace igx { namespace thread {

static void* __on_thread__(void* param)
{
    /* 構造体をコピーするので、呼び出し元が終了しても影響がない 
       mutexで少しオーバヘッドが掛かるが、
       スレッド生成時の度にハンドルを渡すだけ為にメモリを割り当てる必要がない。 */
    struct __thread_args__{
        igx::thread::Target* target;
        igx::thread::callback_t callback;
        void* parameter;
        igx_mutex_t mutex;
    } thread_args = *((struct __thread_args__*)param);

    igx_mutex_unlock(&((struct __thread_args__*)param)->mutex);
    return (thread_args.target->*thread_args.callback)(thread_args.parameter);
}
igx_thread_t createj(void* target, callback_t callback, void* parameter)
{
	igx_thread_t th;
    struct {
        Target* target;
        callback_t callback;
        void* parameter;
        igx_mutex_t mutex;
    } thread_args = {(Target*)target, callback, parameter, {0, }};

#ifdef WIN32
    // TODO.
    //uintptr_t handle = _beginthreadex()
#endif
#ifdef __POSIX__

    igx_mutex_init(&thread_args.mutex);
    igx_mutex_lock(&thread_args.mutex);
	if (pthread_create(&th, 0, __on_thread__, &thread_args)!=0)
	{
        IGX_ERROR("thread create failure!!");
        igx_mutex_unlock(&thread_args.mutex);
		return 0;
	}
    IGX_INFO("thread create success!!");
    // デッドロック!いえぃー！
    igx_mutex_lock(&thread_args.mutex);
    igx_mutex_unlock(&thread_args.mutex);
    igx_mutex_term(&thread_args.mutex);
    IGX_INFO("thread create exit!!");
    return th;
#endif
}
igx_thread_t create(void* target, igx::thread::callback_t callback, void* parameter)
{
#ifdef WIN32
#else
    igx_thread_t ret = createj((igx::thread::Target*)target, callback, parameter);
    pthread_detach(ret);

    return ret;
#endif
}

}} /* end of namespace */

/* +++++++++++++++++++++++++++ for unit test. +++++++++++++++++++++++++++ */

#ifdef __UNIT_TEST__
#include <stdlib.h>
#include <unistd.h>
#include "debug/igx_log.h"
#include "library/igx_memory.h"
#include "data_struct/itx_linked_list.h"
#include "data_struct/itx_queue.h"
#include "data_struct/itx_stack.h"
#include <stdio.h>
#include "library/itx_mem_page.h"

class ThreadTestClass {
public:
    void Start();
    void* OnThread(void* param);
    void* OnThreadReceive(void* param);
private:
    igx_mutex_t m_mutex;
    igx_thread_t m_thread[2];
};

int main(int argc, char** argv)
{
    igx_log_init(kLogOutputConsole, kLogLevelDebug, 0);

    ThreadTestClass test;
    test.Start();

    return 0;
}

typedef struct {
    int id;
    char name[128];
} id_name_t;

#define TEST_NEW IGX_NEW("THREAD_UNITTEST")
 
void ThreadTestClass::Start()
{
    char buffer[4096];
    char buffer2[4096];
    ItxLinkedList<id_name_t>* list = TEST_NEW ItxLinkedList<id_name_t>(100, 16, true);
    ItxQueue<id_name_t> queue(buffer2, sizeof(buffer), true);
    list->Init();

    for (int i=0; i<5; ++i)
    {
        id_name_t item = {i, {0, }};
        sprintf(item.name, "%d-%d", i, i+i);
        list->Add(item);
    }

    for (ItxNode<id_name_t>* node = list->GetTopFrom();node;node=node->next)
    {
       id_name_t& data = **node;
       IGX_DEBUG("id:%d, name:%s", data.id, data.name);
    }
    list->Clear();

    for (int i=0; i<10; ++i)
    {
        id_name_t item = {i, {0, }};
        sprintf(item.name, "%d-%d", i, i+i);
        queue.EnQueue(item);
    }

    while(1)
    {
       id_name_t item;
       int n = queue.DeQueue(&item);
       if (n<0)
       {
         break;
       }

       IGX_DEBUG("id:%d, name:%s", item.id, item.name);
    }

    ItxStack<id_name_t> stack;
    for (int i=0; i<10; ++i)
    {
        id_name_t item = {i, {0, }};
        sprintf(item.name, "%d-%d", i, i+i);
        stack.Push(item);
    }
    while(1)
    {
       id_name_t item;
       int n = stack.Pop(&item);
       if (n<0)
       {
         break;
       }

       IGX_DEBUG("id:%d, name:%s", item.id, item.name);
    }
    
    IGX_DEBUG("ThreadTestClass::Start() !!");
    igx_mutex_init(&m_mutex);
    m_thread[0] = igx::thread::create(this, (igx_thread_callbackex_t)&ThreadTestClass::OnThread, (void*)list);
    m_thread[1] = igx::thread::createj(this, (igx_thread_callbackex_t)&ThreadTestClass::OnThreadReceive, (void*)list);

    igx_thread_join(m_thread[0]);
    igx_thread_join(m_thread[1]);

    igx_memory_print("*");

    IGX_DEL(list);
    igx_memory_print("*");

    ItxMemPage page(256);
    
    page.Pop(100);
    page.Pop(135);
    page.Pop(16);
    page.Pop(29);
    page.Pop(300);
    page.Pop(10);
    page.Pop(50);

    igx_memory_print("*");

    IGX_INFO("ThreadTestClass::Start exit"); 
}
void* ThreadTestClass::OnThreadReceive(void* param)
{
    ItxLinkedList<id_name_t>* list = (ItxLinkedList<id_name_t>*)param;
    id_name_t item;
    IGX_INFO("ThreadTestClass::OnThreadReceive() Enter");
    while(1)
    {
       list->lock();
       int n = list->RemoveTopFrom(0, &item);
       if (n<0)
       {
          IGX_INFO("queue is empty.....");
          list->unlock(); 
          break;
       }
       list->unlock(); 
       IGX_DEBUG("id:%d, name:%s", item.id, item.name);
    }

    return 0;
}
void* ThreadTestClass::OnThread(void* param)
{
    ItxLinkedList<id_name_t>* list = (ItxLinkedList<id_name_t>*)param;
    IGX_INFO("ThreadTestClass::OnThread() Enter");
    
    for (int i=0; i<10; ++i)
    {
        id_name_t item = {i, {0, }};
        sprintf(item.name, "%d-%d", i, i+i);
        list->lock();
        int n = list->Add(item);
        IGX_DEBUG("Add, %s", item.name);
        if (n<0)
        {
            IGX_ERROR("list is full!! \n");
            --i;
            list->unlock();
            continue;
        }
        list->unlock(); 
    }
    

    return 0;
}
#endif


/* +++++++++++++++++++++++++++ for unit test. +++++++++++++++++++++++++++ */

