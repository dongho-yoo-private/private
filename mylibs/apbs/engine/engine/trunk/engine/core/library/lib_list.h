/**
 * \author dongho yoo
 * \file lib_list.h
 * \~english
 * \~japanese
 *   \brief Linked List Library.
 */

#ifndef LIB_LIST_H__
#define LIB_LIST_H__

#include <core/types.h>

ADK_BEGIN_NAMESPACE

typedef void* HandleList;


/**@brief このマクロは以下のように使われます。
*
*@code
TListNode* node;
FOR_LIST_BEGIN(hList, node)
  AnyType* val = (AnyType*)NODE_REF(node);
FOR_LIST_END()
*@endcode
*/
#define FOR_LIST_BEGIN(handle, node) \
for (node=LIB_ListGetTopFrom(handle, 0); node; node=node->np)\
{
#define NODE_REF(node) node->ref
#define FOR_LIST_END() }

#if 0
#ifndef True
typedef S32 Bool;
#define True 1
#define False 0
#endif
#endif


/**@brief Linked Listの構造体
*/
typedef struct _TListNode{
  /**@brief 設定される値*/
  void* ref;
  /**@brief 前*/
  struct _TListNode* pp;
  /**@brief 後*/
  struct _TListNode* np;
} TListNode;


#ifndef EXTERN_C
#  ifdef __cplusplus
#    define EXTERN_C extern "C" //__declspec(dllexport)
#  else
#    define EXTERN_C extern //__declspec(dllexport)
#  endif
#  ifdef _WIN32
#    ifndef STATIC_COMPILE
#      undef EXTERN_C
#      ifdef __cplusplus
#        define EXTERN_C extern "C" __declspec(dllexport)
#      else
#        define EXTERN_C extern __declspec(dllexport)
#      endif
#    endif
#  endif
#endif

/** @brief リストを生成します。
*
*@return リストのハンドルを返します。
*/
EXTERN_C HandleList LIB_ListCreate();
#ifdef __MUTEX__
EXTERN_C HandleList LIB_ListCreate(Bool bUseMutex, S32 nSpinCount);
#endif

/** @brief リストを削除します。
*
*@param hList LIB_ListCreateで生成されたハンドル
*/
EXTERN_C void LIB_ListDestroy(HandleList hList);

/** @brief リストの後ろに追加
*
*@param hList LIB_ListCreateで生成されたハンドル
*@param anyValue 追加する値
*
*@return 成功した場合にリストの数が返ります。
*@retval 0: 失敗
*/
EXTERN_C S32 LIB_ListAdd(HandleList hList, void* anyValue);

//#define yimListAdd(a, b) LIB_ListAdd(a, (IntPtr)b)

/** @brief リストに挿入
*
*@param hList LIB_ListCreateで生成されたハンドル
*@param index 挿入するインデックス
*@param anyValue 追加する値
*
*@return 成功した場合にリストの数が返ります。
*@retval 0: 失敗
*/
EXTERN_C S32 LIB_ListInsert(HandleList hList, S32 index, void* anyValue);

/** @brief 特定のインデックスのノード削除
*
*@param hList LIB_ListCreateで生成されたハンドル
*@param index 削除するインデックス
*
*@return 削除したノードに設定した値が返ります。
*@retval 0: 失敗?
*/
EXTERN_C void* LIB_ListRemoveEx(HandleList hList, S32 index);

/** @brief ノード削除
*
*@param hList LIB_ListCreateで生成されたハンドル
*@param node 削除するノード
*
*@return 削除したノードに設定した値が返ります。
*@retval 0: 失敗?
*/
EXTERN_C void LIB_ListRemove(HandleList hList, TListNode* node);

/** @brief リストをクリアーします。\n
*
*@param hList LIB_ListCreateで生成されたハンドル
*/
EXTERN_C void LIB_ListClear(HandleList hList);

/** @brief リストのノードを取得します。
*
*@param hList LIB_ListCreateで生成されたハンドル
*@param index トップから何番目であるか
*
*@return 該当ノードを返します。
*@retval NULL: 該当ノードがない場合
*/

EXTERN_C TListNode* LIB_ListGetTopFrom(HandleList hList, S32 index);

// -------------------------------
/** @brief リストのノードを取得します。
*
*@param hList LIB_ListCreateで生成されたハンドル
*@param index テイルから何番目であるか
*
*@return 該当ノードを返します。
*@retval NULL: 該当ノードがない場合
*/
EXTERN_C TListNode* LIB_ListGetTailFrom(HandleList hList, S32 index);

/** @brief リストの数を返します。
*
*@param hList LIB_ListCreateで生成されたハンドル
*
*@return リストの数
*/
EXTERN_C S32 LIB_ListCount(HandleList hList);

/** @brief リストを動的配列で返します。
*
*@param hList LIB_ListCreateで生成されたハンドル
*
*@return 配列のポインター
*/
EXTERN_C void** LIB_ListExport(HandleList hList);

/** @brief LIB_ListExportで生成された動的配列を削除します。
*
*@param p 動的配列のアドレス
*/
EXTERN_C void LIB_ListExportDelete(void** p);


/* struct -------------------*/
/**
 * \~english
 * \~japanese
 *   \brief base list
 */
typedef struct LIB_TListBase{
  void  *first,
        *last;
}LIB_TListBase;


/* extern function definition -------------------*/
/**
 * \brief リストの先頭に追加します。
 * \param lb [in] リスト
 * \param vlink [in] 追加するリンク
 * \retval TRUEなら成功
 */
Bool LIB_ListAddHead( LIB_TListBase *lb, void *vlink);
/**
 * \brief リストの最後尾に追加します。
 * \param lb [in] リスト
 * \param vlink [in] 追加するリンク
 * \retval TRUEなら成功
 */
Bool LIB_ListAddTail( LIB_TListBase *lb, void *vlink);
/**
 * \brief リストに挿入します。
 * \param lb [in] リスト
 * \param vlinkPrev [in] 挿入する前のリンク
 * \param vlinkNew [in] 追加するリンク
 */
void LIB_ListInsertLink( LIB_TListBase *lb, void *vlinkPrev, void *vlinkNew);
/**
 * \brief リストの任意のリンクの後ろに追加します。
 * \param vlinkPrev [in] 挿入する後ろのリンク
 * \param vlinkNew [in] 追加するリンク
 */
void LIB_ListInsertLinkAfter( LIB_TListBase *lb, void *vlinkPrev, void *vlinkNew);
/**
 * \brief リストの任意のリンクの前に追加します。
 * \param vlinkPrev [in] 挿入する前のリンク
 * \param vlinkNew [in] 追加するリンク
 */
void LIB_ListInsertLinkBefore( LIB_TListBase *lb, void *vlinkPrev, void *vlinkNew);
/**
 * \brief リストからリンクを切り離します。
 * \param lb [in] リスト
 * \param vlink [in] 切り離すリンク
 * \retval TRUEなら成功
 */
Bool LIB_ListRemLink( LIB_TListBase *lb, void *vlink);
/**
 * \brief リストからリンクを削除します。
 * \param lb [in] リスト
 * \param vlink [in] 削除するリンク
 */
void LIB_ListFreeLink( LIB_TListBase *lb, void *vlink);
/**
 * \brief リストすべてを削除します。
 * \param lb [in] リスト
 */
void LIB_ListFreeList( LIB_TListBase *lb);
/**
 * \brief リストのリンク数を返します。
 * \param lb [in] リスト
 * \return リンク数
 */
S32 LIB_ListCount( LIB_TListBase *lb);
/**
 * \brief リンク番号から、リスト内のリンクを探して返します。
 * \param lb [in] リスト
 * \param linkNo [in] リンク番号
 * \return リンクのポインタ
 */
void *LIB_ListFindLink( LIB_TListBase *lb, S32 linkNo);
/**
 * \brief リンクがリストの何番目か返します。
 * \param lb [in] リスト
 * \param vlink [in] 探し出すリンク
 * \return リストが何番目か
 */
S32 LIB_ListFindIndex( LIB_TListBase *lb, void *vlink);
/**
 * \brief リストのソートを行います。
 */
void LIB_ListSortList( LIB_TListBase *lb, S32 (*compare)(void*,void*));

/**
 * \~english
 * \~japanese
 *  \brief 片方向連結リストのプッシュを行います
 *  \param[in/out] list 元となるリスト
 *  \param[in] element プッシュ要素
 */
extern void LIB_ForwardLinkedListPush(void **list, void *element);
/**
 * \~english
 * \~japanese
 *  \brief 片方向連結リストのポップを行います
 *  \param[in/out] list 元となるリスト
 *  \return ポップして抜き出した要素のアドレスが返ります
 */
extern void *LIB_ForwardLinkedListPop(void **list);
/**
 * \~english
 * \~japanese
 *  \brief リストに要素が存在するかどうかを返します
 *  \param[in] 元となるリスト
 *  \return 要素の存在。以下の値が返ります
 *  \retval true 要素が存在したときに返ります
 *  \retval false 要素が存在しないときに返ります
 */
extern Bool LIB_IsForwardLinkedList(void *list);

ADK_END_NAMESPACE

#endif  /* LIB_LIST_ */


