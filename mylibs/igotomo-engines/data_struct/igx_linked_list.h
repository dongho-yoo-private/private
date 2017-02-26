/**
* Copyright (c) 2015 igotomo.
* authorized by dongho.yoo
*
* @brief C用のLinkedList インーターフェース\n
* 内部ではItxLinkedListを使っているラッパー関数群
*/
#ifndef _IGX_LINKED_LISTH_H_
#define _IGX_LINKED_LISTH_H_

#ifdef __cplusplus
#   define IGX_LLIST_EXTERN extern "C"
#else
#   define IGX_LLIST_EXTERN extern
#endif


typedef struct __igx_llist_node__ {
    void* data; 
    struct __igx_llist_node__* prev;
    struct __igx_llist_node__* next;
} igx_llist_node_t;

/**
* @param buffer 内部で使われる固定バッファー
* @param size   バッファーのサイズ
* @param is_create_mutex ミューテックスOn/Off
* @return ハンドル
* @startcode
*       :
*       :
* char buffer[4096];
* HITX handle = igx_llist_init(buffer, sizeof(buffer));
*       : 
* igx_llist_add(handle, (void*)any_data1, 0);
* igx_llist_add(handle, (void*)any_data2, 0);
* igx_llist_add(handle, (void*)any_data3, 0);
* igx_llist_add(handle, (void*)any_data4, 0);
*       :
*       :
* @endcode
*/
IGX_LLIST_EXTERN HITX igx_llist_init(char* buffer, size_t buffer_size);
/*
* @brief リストを生成します。
* @param count_per_page 一ページに入るエレメントの数。\n
* ページ単位でメモリを割り当てます。-1はデフォルト値 
* @param is_create_mutex  ミューテックスOn/OFF
* @param deallocator メモリから削除時のコールバック
* @param HITX メモリの割当をページ単位で行う場合指定します。
* @return ハンドル
* @startcode
*       :
* HITX handle = igx_llist_new(32, true, free, handle_of_mem_page);
*       : 
* igx_llist_add(handle, (void*)any_data1, 0);
* igx_llist_add(handle, (void*)any_data2, 0);
* igx_llist_add(handle, (void*)any_data3, 0);
* igx_llist_add(handle, (void*)any_data4, 0);
*       :
*       :
* igx_llist_delete(handle);
* @endcode

*/
IGX_LLIST_EXTERN HITX igx_llist_new(bit32_t count_per_page, bool_t use_mutex, void (*deallocator)(void*), HITX mempage);
/**
* @brief igx_llist_newを呼んだ際に、最後にリストを削除します。\n
 * igx_llist_new時にmempageを指定してない場合のみ呼び出してください。
* @param list ハンドル
*/
IGX_LLIST_EXTERN void igx_llist_delete(HITX list);
/*
 * @brief インデックスの後ろに追加します。\n
 * @brief indexの前に追加します。
 * @param data データ
 *
 * @return データの数
*/
IGX_LLIST_EXTERN bit32_t igx_llist_add(HITX list, void* p, bit32_t index);
/**
* @brief indexの前に挿入します。
* @param data データ
*
* @return データの数
*/
IGX_LLIST_EXTERN bit32_t igx_llist_insert(HITX list, void* p, bit32_t index);
/**
* @brief トップからindex番目のデータを取得します。
* @param data データ
* @return ノード
*
* @startcode
* igx_llist_node_t* node = igx_llist_get_top_from(h_list, 0);
* for (;node!=0; node=node->next)
* {
*     (AnyType)* p = node->data;
*                   :
*                   :
* }
* @endcode
*/
IGX_LLIST_EXTERN igx_llist_node_t* igx_llist_get_top_from(HITX list, bit32_t index);
/**
* @brief 後ろからindex番目のデータを取得します。
* @param list  handle
* @param data データ
* @return ノード
*
* @startcode
* igx_llist_node_t* node = igx_llist_get_bottom_from(h_list, 0);
* for (;node!=0; node=node->prev)
* {
*     (AnyType)* p = node->data;
*                   :
*                   :
* }
* @endcode
*/
IGX_LLIST_EXTERN igx_llist_node_t* igx_llist_get_bottom_from(HITX list, bit32_t index);
/**
* @brief 前からindex番目のデータを削除します。
*
* @param list  handle
* @param index インデックス
* @param out 削除されるデータ、ゼロの場合は無視されます。
* @return 残りのデータの数
*/
IGX_LLIST_EXTERN bit32_t igx_llist_remove_top_from(HITX list, bit32_t index, void** out);
/**
* @brief 後ろからindex番目のデータを削除します。
*
* @param list  handle
* @param index インデックス
* @param out 削除されるデータ、ゼロの場合は無視されます。
* @return 残りのデータの数
*/
IGX_LLIST_EXTERN bit32_t igx_llist_remove_bottom_from(HITX list, bit32_t index, void** out);
/**
* @brief 指定したノードを削除します。
* @param list  handle
* @param t ノード
* @return 残りのデータの数 (-1は空っぽ)
*/
IGX_LLIST_EXTERN bit32_t igx_llist_remove_node(HITX list, igx_llist_node_t* node);
/**
* @brief リストの中のデータをクリアーします。
* @param list  handle
*/
IGX_LLIST_EXTERN void igx_llist_clear(HITX list);
/**
* @brief 生成時にuse_mutex=trueにした際にはロックします。
* @param list  handle
*/
IGX_LLIST_EXTERN void igx_llist_lock(HITX list);
/**
* @brief 生成時にuse_mutex=trueにした際にはアンロックします。
* @param list  handle
*/
IGX_LLIST_EXTERN void igx_llist_unlock(HITX list);
/**
* @brief エレメントの数を取ります。
* @param handle handle
* @return 数
*/
IGX_LLIST_EXTERN ubit32_t igx_llist_count(HITX handle);
/**
* @brief プッシュします。
*
* @param handle handle
* @param data データ
* @return -1 はエラー overflow
*/
IGX_LLIST_EXTERN bit32_t igx_llist_push(HITX handle, void* data);
/**
* @brief ポップします。
*
* @param handle handle
* @param out データ
* @return -1 はempty.
*/
IGX_LLIST_EXTERN bit32_t igx_llist_pop(HITX handle, void** data);
/**
* @brief キューに入れます。
*
* @param handle handle
* @param data データ
* @return -1 はエラー overflow
*/
IGX_LLIST_EXTERN bit32_t igx_llist_enqueue(HITX handle, void* data);
/**
* @brief キューから取り出します。
*
* @param handle handle
* @param out データ
* @return -1 はempty.
*/
IGX_LLIST_EXTERN bit32_t igx_llist_dequeue(HITX handle, void** data);


#endif
