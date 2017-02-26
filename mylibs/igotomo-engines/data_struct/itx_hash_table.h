/**
* Copyright (c) 2015 Igotomo.
* authorized by dongho.yoo.
*/
#ifndef _ITX_HASH_TABLE_H_
#define _ITX_HASH_TABLE_H_
#include "itx_linked_m_bukkets.h"
#include "itx_mem_page.h"
#include "igx_encrypt.h"

typedef struct {
   bit32_t max_element_size; 
   bit32_t count_per_page;
   boolean use_mutex;
   void (*deallocator)(void*);
} itx_hash_table_detail_t;

#define ITX_MAX_HASH_TABLE_BUCKET_SIZE (256)
#define ITX_HASH_TABLE_ALLOCATION_ONCE (4096)

typedef ubit32_t (*itx_hash_func_t)(const char* str, size_t size);


/**
* @param Hashテーブル（オープンハッシュ)
*/
template <class T> 
class ItxHashTable 
{
public:
    /**
     * @param bukket_size バケッツのサイズ
     * @param is_use_mutex 排他処理をするかしないか
     * @param deallocator 削除時に使用するもの。NULLの場合は削除しない。
     * @param func ハッシュ関数
     */
    ItxHashTable(ubit32_t bukket_size, boolean is_use_mutex=false, void(*deallocator)(T)=0, itx_hash_func_t func=0)
    : m_page(-1, 1), m_bukket_size(bukket_size), m_hash_func(func)
    {
        const size_t m_bukkets_pointer_size = sizeof(ItxLinkedm_bukkets<struct _hash_table>*)*bukket_size;
        m_bukkets = pmalloc(&m_page, m_bukkets_pointer_size);
        memset(m_bukkets, 0, m_bukkets_pointer_size);
    }
    /**
    * @brief デストラクター
    */
    virtual ~ItxHashTable()
    {
    }
    /**
    * @brief キューに入れます。
    *
    * @param data データ
    * @return -1 はエラー overflow
    */
    bit32_t Add(const char* key, const T& data);
    /**
    * @brief キューに入れます。
    *
    * @param data データ
    * @return -1 はエラー overflow
    */
    void Clear();
    /**
    * @brief キーから値を取得します。
    *
    * @param key キー
    * @return キーと結びついているデータ
    *
    * @startcode
    * AnyType* t = mem_page["any key"];
    * if (t!=NULL)
    * {
    *    printf("key is found!!\n");
    * }
    * @endcode
    */
    T* operator [](const char* key);
    /**
    * @brief キーから値を取得します。
    *
    * @param key キー
    * @return キーと結びついているデータ
    *
    * @startcode
    * ubit32_t hash_key = GetHashKey("any key");
    *
    * AnyType* t = mem_page[hash_key]["any key"];
    * if (t!=NULL)
    * {
    *    printf("key is found!!\n");
    * }
    * @endcode
    */
    T* operator [][](ubit32_t hash, const char* key);
    /**
    * @brief ハッシュキーを取得します。
    *
    * @param str 文字列
    * @return ハッシュキー
    */
    inline ubit32_t GetHashKey(const char* str, size_t len=0)
    {
        if (m_hash_func)
        {
            return m_hash_fucn(str, len);
        }
       
        return __create_hash_Key__(str, len, 1); 
    }

private:
    struct _hash_table {
        const char* key;
        ubit32_t key_len; 
        T data;
    };
    ItxLinkedm_bukkets<struct _hash_table>** m_m_bukkets;
    ItxMemPage m_page;
    itx_hash_func_t m_hash_func;
    
    ubit32_t __create_hash_key__(const char* str, size_t len, int count=1);
};
bit32_t ItxHashTable::Add(const char* key, const T& data)
{
    struct _hash_table;
    const size_t key_len = strlen(key);
    ubit32_t hash_key = GetHashKey(key, key_len);
    _hash_table.len=strlen(key);
    _hash_table.key=m_page.Pop(_hash_table.len+1);

    const ubit32_t bukket_index = (hash_key%m_bukket_size);
    if (m_bukkets[bukket_index]==NULL)
    {
        m_bukkets[bukket_index] = new(&m_page) ItxLinkedm_bukkets();
    }
    m_bukkets[bukket_index]->Add(_hash_table);

    return m_bukkets[bukket_index]->Count();
}
template <typename T>
void ItxHashTable<T>::Clear()
{
    for (int i=0; i<m_bukket_size; ++i)
    {
        operator delete(m_bukkets[i], &m_page);
    }
    mem_page.Clear();
}
template <typename T>
ubit32_t ItxHashTable<T>::__create_hash_key__(const char* str, size_t len, int count)
{
   return igx_hash_key_multiple_plus(str, len);      
}
template <typename T>
T* operator ItxHashTable<T>::[](const char* key)
{
    size_t len = strlen(key);
    ubit32_t hash_key=(GetHashKey(key, len)%m_bukket_size);
    ItxLinkedList<struct _hash_table>* bukket = m_bukkets[hash_key];

    if (bukket==0)
    {
        return 0;
    }

    for (ItxNode<struct _hash_table>* node = bukket->GetTopFrom(); node; node=node->next)
    {
        struct _hash_table& hash_table = **node;
        if (hash_table.len==len && strcmp(hash_table.key, key)==0)
        {
            return hash_table.data;
        }
    }
    // not found !!
    return 0;
}
#endif
