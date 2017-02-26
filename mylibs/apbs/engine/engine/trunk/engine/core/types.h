/**
 * \file types.h
 * \ingroup grp_adk
 * \author tomonori tanaka
 * \~english
 * \~japanese
 *  \brief ADK の基本定義
 */
/**
 * \~english
 * \~japanese
 *  \page page_adk_programming_rules プログラミング規則
 *  \tableofcontents
 *  \section file_format 1 ソースファイルのファイルフォーマットについて
 *    \subsection file_format_utf8 1.1 UTF-8 の BOM 付きを、
 *                        基本のフォーマットとする
 *       プログラム作成中に文字列を扱う際、 UTF-8 を利用するにはファイルフォーマットが UTF-8 である必要があるため。\n
 *       UTF-16 については L"文字列" とすれば対応できるので問題ない。
 *  \section guardedalloc 2 メモリ仕様
 *    \subsection new_delete_taboo 2.1 new/delete の禁止
 *       ADK は new/delete はオーバーロードしていません。\n
 *       上記した理由により構造体や配列など動的に確保を行う場合は \ref ADK_Malloc() / \ref ADK_Calloc() / \ref ADK_Free() を使用して下さい。\n
 *      \n
 *       それでもクラスについては、コンストラクタ/デストラクタの呼び出しが必要ですので new/delete を使用したい状況があります。\n
 *       そのような状況に対応するために \link new_delete_how_to_use 1.2 クラスへの new/delete の実装 \endlink を参照してください。
 *    \subsection new_delete_how_to_use 2.2 クラスへの new/delete の実装
 *       ADK は new/delete はオーバーロードしていません。\n
 *       クラスの実装を行う際に new/delete を使用するために以下のようにしてクラス毎に new/delete を実装する必要があります。
 *      \code
 *       class TestClass{
 *       public:
 *         ADK_MEM_CXX_CLASS_ALLOC_FUNCS("TestClass");
 *       };
 *      \endcode
 *       上記サンプルのように実装することで
 *      \code
 *       {
 *         TestClass  *temp = new TestClass;
 *         delete temp;
 *       }
 *      \endcode
 *       と実装したとき new/delete は TestClass の new/delete が呼び出されるようになります。
 *  \section prefix 3 プレフィックス
 *    \subsection prefix_typedef 3.1 定義
 *       変数や構造体の定義には先頭に 'T' を付加する
 *      \code
 *       // 変数定義
 *       typedef S32  TUniformid;
 *      \endcode
 *      \code
 *       // 構造体定義
 *       typedef struct{
 *       }TUniform;
 *      \endcode
 *    \subsection prefix_const 3.2 定数
 *       変数や構造体の定義には先頭に 'k' を付加する
 *      \code
 *       // const 定数
 *       static const S32  kConst = 0;
 *      \endcode
 *      \code
 *       // enum 定数
 *       enum{
 *         kEnum0,
 *         kEnum1,
 *         kEnum2,
 *         kEnum3
 *       };
 *      \endcode
 *    \subsection prefix_tab 3.2 タブサイズ
 *       タブのサイズは、半角スペース 4 つとする。
 *  \section brace 4 ブレイス '{' / '}' の使いについて
 *       ブレイスの位置については、
 *      \code
 *       if()
 *       {
 *       }
 *      \endcode
 *       とし
 *      \code
 *       if() {
 *       }
 *      \endcode
 *       とはしない。\n
 *       これは関数/メソッド/for/whileの定義でも同じ
 */


#ifndef TYPES_H_
#define TYPES_H_

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* support definition */
/* 最終的にはプリプロセッサとかに定義する？ */
/**
 * \~english
 * \~japanese
 *  \brief デバッグ定義
 */
//#if defined(DEBUG)
#  define ADK_DEBUG
//#endif
/**
 * \~english
 * \~japanese
 *  \brief ログ出力サポートプリプロセッサ
 */
#define ADK_LOG_SUPPORT
/**
 * \~english
 * \~japanese
 *  \brief inline サポートプリプロセッサ
 */
#define ADK_INLINE_SUPPORT
#ifdef ADK_DEBUG
  /**
   * \~english
   * \~japanese
   *  \brief assert サポートプリプロセッサ
   */
#  define ADK_ASSERT_SUPPORT
#endif  /* ADK_DEBUG */
/**
 * \~english
 * \~japanese
 *  \brief cocos サポートプリプロセッサ(cocosを排除するまでは存在させる)
 */
#define ADK_COCOS_SUPPORT
/**
 * \~english
 * \~japanese
 *  \brief Native サポートプリプロセッサ(Native を排除するまでは存在させる)
 */
#define ADK_NATIVE_SUPPORT
/**
 * \~english
 * \~japanese
 *  \brief SSL サポートプリプロセッサ(SSL を排除するまでは存在させる)
 */
//#define ADK_SSL_SUPPORT
/**
 * \~english
 * \~japanese
 *  \brief ADKでJSONをサポートしている定義
 */
#define ADK_WITH_JSON_SUPPORT


/* namespace */
/**
 * \~english
 * \~japanese
 *  \brief ADK の開始ネームスペース
 */
#define ADK_BEGIN_NAMESPACE  namespace adk{
/**
 * \~english
 * \~japanese
 *  \brief ADK の終了ネームスペース
 */
#define ADK_END_NAMESPACE  }


/* inline */
#ifdef ADK_INLINE_SUPPORT
  /**
   * \~english
   * \~japanese
   *  \brief ADK 用 inline 定義
   */
  #define ADK_INLINE  inline
#else
  /**
   * \~english
   * \~japanese
   *  \brief ADK 用 inline 定義
   */
  #define ADK_INLINE
#endif


/* platform typedef */
#if defined(WITH_IOS)|defined(WITH_ANDROID)
  /**
   * \~english
   * \~japanese
   *  \brief 8 bit signed
   */
  typedef int8_t S8;
  /**
   * \~english
   * \~japanese
   *  \brief 8 bit unsigned
   */
  typedef uint8_t U8;
  /**
   * \~english
   * \~japanese
   *  \brief 16 bit signed
   */
  typedef int16_t S16;
  /**
   * \~english
   * \~japanese
   *  \brief 16 bit unsigned
   */
  typedef uint16_t U16;
  /**
   * \~english
   * \~japanese
   *  \brief 32 bit signed
   */
  typedef int32_t S32;
  /**
   * \~english
   * \~japanese
   *  \brief 32 bit unsigned
   */
  typedef uint32_t U32;
  /**
   * \~english
   * \~japanese
   *  \brief 64 bit signed
   */
  typedef int64_t S64;
  /**
   * \~english
   * \~japanese
   *  \brief 64 bit unsigned
   */
  typedef uint64_t U64;
  /**
   * \~english
   * \~japanese
   *  \brief float
   */
  typedef float F32;
  /**
   * \~english
   * \~japanese
   *  \brief double
   */
  typedef double Double;
  /**
   * \~english
   * \~japanese
   *  \brief byte
   */
  typedef uint8_t Byte;
  /**
   * \~english
   * \~japanese
   *  \brief char
   */
  typedef char Char;
  /**
   * \~english
   * \~japanese
   *  \brief wchar
   */
  typedef wchar_t Wchar;
  /**
   * \~english
   * \~japanese
   *  \brief 
   */
# ifdef intptr_t
  typedef intptr_t IntPtr;
# else
  typedef void* IntPtr;
# endif /* intptr_t */
#endif
/**
 * \~english
 * \~japanese
 *  \brief boolean
 */
typedef S32  Bool;
/**
 * \~english
 * \~japanese
 *  \brief size
 */
#ifndef false
  /**
   * \~english
   * \~japanese
   *  \brief false
   */
  #define false  (0)
#endif  /* false */
#ifndef true
  /**
   * \~english
   * \~japanese
   *  \brief true
   */
  #define true  (1)
#endif  /* true */


/* endian */
#define WITH_LITTLE_ENDIAN  0
#define WITH_BIG_ENDIAN  1
#ifdef __BYTE_ORDER__
  #if __BYTE_ORDER__==__ORDER_LITTLE_ENDIAN__
    #define WITH_ENDIAN  WITH_LITTLE_ENDIAN
  #else
    #define WITH_ENDIAN  WITH_BIG_ENDIAN
  #endif
#else
  #define WITH_ENDIAN  WITH_LITTLE_ENDIAN
#endif  /* __BYTE_ORDER__ */

/* handle type */
/**
 * \~english
 * \~japanese
 *  \brief ADK用の汎用ハンドル
 */
#define ADK_HANDLE(name) \
  typedef struct name##__ { int unused; } *name

/**
 * \~english
 * \~japanese
 *  \brief アライメント値を求めるマクロ
 */
#define ADK_ALIGN(size,alignment) \
  ((size+(alignment-1))&~(alignment-1))

/**
 * \~english
 * \~japanese
 *  \brief ハンドル作成マクロ
 */
#define ADK_DECLARATE_HANDLE(name) \
  typedef struct name##__ { int unused; } *name
/**
 * \~english
 * \~japanese
 *  \brief Non-Copyable MixIn
 */
#define ADK_NON_COPYABLE_CLASS(name) \
  name(const name&); \
  void operator=(const name&)

#if defined(WITH_WIN32)
# define ALIGNED16(a)	__declspec(align(16)) a
# define ALIGNED8(a)	__declspec(align(8)) a
#elif defined(WITH_COCOA)
# define ALIGNED16(a)	__declspec(align(16)) a
# define ALIGNED8(a)	__declspec(align(8)) a
#elif defined(WITH_CARBON)
# define ALIGNED16(a)	__declspec(align(16)) a
# define ALIGNED8(a)	__declspec(align(8)) a
#elif defined(WITH_IOS)
//# define ALIGNED16(a)	a __declspec(align(16))
//# define ALIGNED8(a)	a __declspec(align(8))
/* EXC_ARM_DA_ALIGNが発生するため */
# define ALIGNED16(a)	a
# define ALIGNED8(a)	a
#elif defined(WITH_X11)
# define ALIGNED16(a)	__declspec(align(16)) a
# define ALIGNED8(a)	__declspec(align(8)) a
#elif defined(WITH_ANDROID)
# define ALIGNED16(a)	a __attribute__ ((aligned (16)))
# define ALIGNED8(a)	a __attribute__ ((aligned (8)))
#elif defined(WITH_PSP)
# define ALIGNED16(a)	a __attribute__ ((aligned (16)))
# define ALIGNED8(a)	a __attribute__ ((aligned (8)))
#elif defined(WITH_VITA)
# define ALIGNED16(a)	a __attribute__ ((aligned (16)))
# define ALIGNED8(a)	a __attribute__ ((aligned (8)))
#elif defined(WITH_PS3)
# define ALIGNED16(a)	a __attribute__ ((aligned (16)))
# define ALIGNED8(a)	a __attribute__ ((aligned (8)))
#elif defined(WITH_PS4)
# define ALIGNED16(a)	a __attribute__ ((aligned (16)))
# define ALIGNED8(a)	a __attribute__ ((aligned (8)))
#elif defined(WITH_XBOX360)
# define ALIGNED16(a)	__declspec(align(16)) a
# define ALIGNED8(a)	__declspec(align(8)) a
#elif defined(WITH_WII)
# define ALIGNED16(a)	__declspec(align(16)) a
# define ALIGNED8(a)	__declspec(align(8)) a
#elif defined(WITH_3DS)
# define ALIGNED16(a)	__declspec(align(16)) a
# define ALIGNED8(a)	__declspec(align(8)) a
#endif



/* doxygen group definition */
/**
 * \~english
 * \~japanese
 *  \defgroup grp_adk ADKに関するグループです
 */
/**
 * \~english
 * \~japanese
 *  \defgroup grp_galloc ガードアロケータに関するグループです
 */
/**
 * \~english
 * \~japanese
 *   defgroup grp_api API に関するグループです
 */
/**
 * \~english
 * \~japanese
 *   defgroup grp_debug デバッグ機能に関するグループです
 */
/**
 * \~english
 * \~japanese
 *   defgroup grp_dms データマネジメントシステムに関するグループです
 */
/**
 * \~english
 * \~japanese
 *   defgroup grp_error エラー機能に関するグループです
 */
/**
 * \~english
 * \~japanese
 *   defgroup grp_hostlist ホストリストに関するグループです
 */
/**
 * \~english
 * \~japanese
 *   defgroup grp_library ADKで使用する基本となるライブラリに関するグループです
 */
/**
 * \~english
 * \~japanese
 *   defgroup grp_network ネットワーク機能に関するグループです
 */
/**
 * \~english
 * \~japanese
 *   defgroup grp_request リクエスト機能に関するグループです
 */
/**
 * \~english
 * \~japanese
 *   defgroup grp_startup 起動情報機能に関するグループです
 */
/**
 * \~english
 * \~japanese
 *   defgroup grp_system ADKのシステムに関するグループです
 */


#endif  /* TYPES_H_ */


