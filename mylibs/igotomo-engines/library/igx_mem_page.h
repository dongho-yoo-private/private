/**
 * Copyright 2015 (c) igotomo.
 * authorized by dongho.yoo.
*/
#ifndef _ITX_PMALLOC_H_
#define _ITX_PMALLOC_H_

#include "igx_types.h"

#ifdef __cplusplus
#include "thread/igx_thread.h"
#include "library/igx_memory.h"
#include <string.h>
#include <stdlib.h>

#ifndef STORE
#   define ITX_MEM_PAGE_DEBUG
#endif


typedef struct {
    /**
    * @brief 追加で割り当てる事を許すかどうか(固定バッファーのみ)\n
    * デフォルトはfalse\n
    * 追加の割当はページ単位で割り当てます。
    */
    bool_t additional_allocation;
    /**
    * @brief リングバッファーの様に、ページの領域を超えた場合に、先頭から割り当て直すかどうか。\n
    * デフォルトはfalse
    */
    bool_t is_ring_buffer;
    /**
    * @brief アライメント（デフォルトは8）
    */
    ubit8_t alignment;
} igx_mem_page_option_t;

/**
* @brief 一種のメモリプール。\n
* ただし、削除はプール全体を削除する場合に使う。\n
* 考え方として、メモリの割当ての回数を減らしたい場合に有効。\n
* 内部的にはシステムのページサイズ単位で割り当てるので、メモリのフラグメンテーションが置きにくい。\n
* 例えば、何かのパースのように頻繁に割り当てるケースに適している。\n
* 
* 実際にメモリのフラグメンテーションを減らすために、メモリを解放するタイミングを\n
* 計らって一気に解放する場合などは、このクラスを使います。
*/
class IgxMemPage 
{
public:
    /**
    * @brief コンストラクター
    * @param buffer プールで使われるバッファー
    * @param buffer バッファーサイズ
    * @param alignment Popで割り当てた際に、メモリのアドレスをalignmentに合わせます。\n
    * デフォルトは8なので、8バイトに合わせます。\n
    * 主に文字列を扱う際には1に指定すると効率がよい。
    */
    IgxMemPage(char* buffer, size_t buffer_size, const igx_mem_page_option_t* option=0);
    /**
    * @brief コンストラクター
    * @param page_count 最大のページの数(-1で無限)
    * @param alignment Popで割り当てた際に、メモリのアドレスをalignmentに合わせます。\n
    * デフォルトは8なので、8バイトに合わせます。\n
    * 主に文字列を扱う際には1に指定すると効率がよい。
    */
    IgxMemPage(bit32_t max_page_count=-1, const igx_mem_page_option_t* option=0);
    /**
    * @brief ですトラクター
    */
    virtual ~IgxMemPage();
    /**
    * @brief メモリを割り当てます。
    * 
    * @param size サイズ
    * @return 割り当てられたメモリのアドレス
    */
    void* Pop(size_t size);
    /**
    * @brief メモリを解放します。\n
    * 必ず、解放されるとは限らないが、以下の条件で解放扱いになる。\n
    *  1. ページ内の残りのメモリより大きい場合\n
    *  2. ページ内の残りのメモリとつながっている場合
    * 基本的にIgxMemPageは全体の解放処理以外、解放は行わないポリシーだが、\n
    * メモリの効率を少しでも上げる為の対策。
    * 
    * @param Popで割り当てられたメモリアドレス
    */
    void Release(void* p);
    /**
    * @brief 全てのページをクリアーします。
    */
    void Clear();
    /**
    * @brief 初期化します。
    *
    * @param buffer プールで使われるバッファー
    * @param buffer_size バッファーサイズ
    * @param is_allocation バッファーが足りない場合に追加でメモリを割り当てるかどうか
    * @param alignment Popで割り当てた際に、メモリのアドレスをalignmentに合わせます。\n
    * デフォルトは8なので、8バイトに合わせます。\n
    * 主に文字列を扱う際には1に指定すると効率がよい。
    *
    * @return false失敗　
    */
    bool_t Initialize(char* buffer, size_t buffer_size, bool_t is_allocation=false, ubit8_t alignment=0);
    /**
    * @brief 初期化します。
    *
    * @param page_size ページサイズ
    * @param page_count 最大のページの数(-1で無限)
    * @param alignment Popで割り当てた際に、メモリのアドレスをalignmentに合わせます。\n
    * デフォルトは8なので、8バイトに合わせます。\n
    * 主に文字列を扱う際には1に指定すると効率がよい。
    *
    * @return false失敗　
    */
    bool_t Initialize(size_t page_count=-1,  ubit8_t alignment=8);
    /**
    *  @brief 文字列をページの領域にコピーして、先頭アドレスを返します。
    *
    * @param src 文字列の先頭アドレス。
    * @param src_size 文字列の長さ
    * @return 複製された文字列のアドレス。
    */
    inline char* StrDup(const char* src, size_t src_size=0)
    {
        src_size=(src_size==0)?strlen(src):src_size;
        char* p = (char*)Pop(src_size+1);
        memcpy(p, src, src_size);
        p[src_size]=0;
        return p;
    }
    /**
    * @brief データをページの領域にコピーして、先頭アドレスを返します。
    *
    * @param src データの先頭アドレス。
    * @param src_size データの長さ
    * @return 複製された文字列のアドレス。
    */
    inline void* MemCpy(const void* src, size_t src_size)
    {
        return memcpy(Pop(src_size), src, src_size);
    }
private:
    struct page {
        struct page* next;
        bit32_t remain_memory;
        ubit8_t* indicator;
        ubit8_t data[4];
    }* m_page_top, *m_page_end;

    struct {
        unsigned use_heap:1;
        unsigned first_page_external:1;
        unsigned alignment:4;
        unsigned reserved:2;
        unsigned page_count:16;
        unsigned max_page_count:8;
    } m_flags; /* 8bit */
    igx_mutex_t m_mutex;

    struct page* create_page(size_t size);
    size_t get_page_data_size();
#ifdef ITX_MEM_PAGE_DEBUG
    bit32_t  m_allocated_page_count;
public:
    /**
    * @brief デバッグ用。割り当てたメモリのサイズを取得します。
    * @return 割り当てたメモリのサイズ (bytes)
    */
    inline ubit32_t GetUsingMemorySize()
    {
        return m_allocated_page_count*igx_get_page_size();
    }
    /**
    * @brief デバッグ用。割り当てたページの数を取得します。
    * @return ページの数
    */
    inline ubit32_t GetUsingPageCount()
    {
        return m_allocated_page_count;
    }
    /**
    * @brief 割り当てられたページのメモリの使用状況を出力します。
    */
    static void PrintStatus();
private:
    /**
    * デバッグ用。
    */
    static void Register(IgxMemPage* mem_page);
    /**
    * デバッグ用。
    */
    static void UnRegister(IgxMemPage* mem_page);
    /**
    * デバッグ用。
    */
    //static ItxLinkedListBuffer<IgxMemPage*, 32> ____mem_page_list___;
#endif
};

/**
* @brief IgxMemPage::Popを呼び出すのと同じ
* @param handle \ref IgxMemPage
* @param size サイズ
* @param tag タグ
*
* @return メモリアドレス
*/
extern void* pmalloc(IgxMemPage* handle,  size_t size);
/**
* @brief pmallocで割り当てられたメモリが削除されます。\n
* 必ず、解放されるとは限らないが、以下の条件で解放扱いになる。\n
*  1. ページ内の残りのメモリより大きい場合\n
*  2. ページ内の残りのメモリとつながっている場合
* 基本的にIgxMemPageは全体の解放処理以外、解放は行わないポリシーだが、\n
* メモリの効率を少しでも上げる為の対策。
*
* @param handle @ref IgxMemPage
* @param pmallocで割り当てられたメモリのアドレス
*/
extern void pfree(IgxMemPage* handle, void* mem);
/**
* @brief IgxMemPage::Popを呼び出すのと同じ\n
* ex) \n
*   :
*   :
* #define PNEW new(m_pm)
* AnyClass*p = new(m_pm) AnyClass();
* AnyClass*p = PNEW AnyClass();
* @param handle \ref IgxMemPage
* @param tag タグ
*
* @return メモリアドレス
*/
extern void* operator new(size_t size, IgxMemPage* handle, const char* tag=0);
/**
* @brief IgxMemPage::Popを呼び出すのと同じ\n
* ex) \n
*   :
*   :
* #define PNEW pnew(m_pm)
* AnyClass*p = new(m_pm) AnyClass();
* AnyClass*p = PNEW AnyClass();
* @param handle \ref IgxMemPage
* @param tag タグ
*
* @return メモリアドレス
*/
extern void* operator new[](size_t size, IgxMemPage* handle, const char* tag=0);
/**
* @brief 実際にはメモリは削除されず、IgxMemPageが削除されるタイミングで削除されます。\n
* しかしながら、デストラクターを呼び出す必要がある場合がある場合は呼ぶべきです。
*/
extern void operator delete(void* mem, IgxMemPage* handle, const char* tag=0);
/**
* @brief 実際にはメモリは削除されず、IgxMemPageが削除されるタイミングで削除されます。\n
* しかしながら、デストラクターを呼び出す必要がある場合がある場合は呼ぶべきです。
*/
extern void operator delete[](void* mem, IgxMemPage* handle, const char* tag=0);

/** @brief new/deleteは以下のマクロを利用すると便利です。*/
#define PM_NEW(mem_page) new(mem_page)
#define PM_NEW_ARRAY(mem_page) new[](mem_page)
#define PM_DEL(data) operator delete((void*)data, (IgxMemPage*)0)
#define PM_DEL_ARRAY(data) operator delete[]((void*)data, (IgxMemPage*)0)

template <size_t SIZE>
class ItxMemPage : public IgxMemPage
{
public:
    ItxMemPage (const igx_mem_page_option_t* option=0):IgxMemPage(m_buffer, sizeof(m_buffer), option){}
    virtual ~ItxMemPage(){}
private:
    char m_buffer[SIZE];
};

#else
/* Interface for C */
/**
* @brief IgxMemPageを生成します。
* 
* @param buffer (初期バッファー) NULLならヒープ上に割り当てる
* @param buffer_size バッファーサイズ
* @param alignment 割り当てる際にアライメント
* @param additional_buffer bufferがNULLでない場合、追加バッファーが必要かどうか
* @return ハンドル
*/
extern HITX igx_new_mem_page(char* buffer, size_t buffer_size, bit32_t alignment, bool_t additional_buffer);
/**
* @brief IgxMemPageをクリアします。
* @param handle ハンドル
*/
extern void igx_clear_mem_page(HITX handle);
/**
* @brief IgxMemPageを削除します。
* @param handle ハンドル
*/
extern void igx_delete_mem_page(HITX handle);
/**
* @brief IgxMemPageからメモリを割り当てます。
* @param handle ハンドル
* @param size   割り当てるサイズ
* 
* @return 割り当てられたメモリアドレス
*/
extern void* pmalloc_c(HITX handle, size_t size);
/*
* @brief pmalloc_cから割り当てられたメモリを削除します。\n
* 必ず、解放されるとは限らないが、以下の条件で解放扱いになる。\n
*  1. ページ内の残りのメモリより大きい場合\n
*  2. ページ内の残りのメモリとつながっている場合\n
* 基本的にIgxMemPageは全体の解放処理以外、解放は行わないポリシーだが、\n
* メモリの効率を少しでも上げる為の対策。
*
* @param handle ハンドル
* @param data 削除するデータ
*/
extern void pfree_c(HITX handle, void* data);
/**
* @brief IgxMemPageのメモリのステータスを出力します。
*/
extern void igx_mem_page_print_status();
#endif /* cplusplus */

#endif
