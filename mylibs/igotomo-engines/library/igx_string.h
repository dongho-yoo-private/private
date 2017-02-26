/**
 * Copyright 2015 (c) 
 * authorized by dongho.yoo.
 * 
 * @brief 文字列を扱うライブラリ
*/

#ifndef _IGX_STRING_H_
#define _IGX_STRING_H_

#include "igx_types.h"
#include "library/igx_mem_page.h"

#ifdef __cplusplus
#   define IGX_STRING_EXTERN extern "C"
#else
#   define IGX_STRING_EXTERN extern
#endif

#ifdef __CAST_IS_EXPLICIT__
#   define IGX_STR_OPERATOR explicit operator
#else
#   define IGX_STR_OPERATOR operator
#endif
/**
* @brief 文字列の長さが長い場合に後ろに...を付ける時のバッファー定義マクロ
*
* typedef struct {
*      char name[MAX_NAME_LEN];
*      char event_name[MAX_EVENT_LEN);
*                   :
*                   :
* } any_type_t;
*
*                   :
* any_type_t any_data;
*
*           :
* igx_strcpy_limit_length(any_data.name, sizeof(name), src);
*/
typedef enum {
    kCharSetUtf8=0,
    kCharSetSJis,
    kCharSetGb2312,
    kCharSetBig5,
    kCharSetEucKr
} igx_charset_t;

/**
* @brief 文字列の処理を行うための構造体。\n
* 文字列のパースなどに便利
*/
typedef struct igx_string_t {
    /*@brief 文字のポインター*/
    char*    str;
    /*@brief 文字列の長さ */
    ubit16_t length;
    /*@brief ハッシュキー*/
    ubit16_t hash_key;
#ifdef __cplusplus
    inline IGX_STR_OPERATOR char*()
    {
        return str;
    }
    inline IGX_STR_OPERATOR const char*()
    {
        return str;
    }
    inline igx_string_t& operator=(igx_string_t& _)
    {
        *this=_;
        return *this;
    }
    inline igx_string_t& operator=(igx_string_t& _) const
    {
        *this=_;
        return (igx_string_t&)*this;
    }
    inline igx_string_t& operator=(const igx_string_t& _) const
    {
        *this=_;
        return (igx_string_t&)*this;
    }
    inline igx_string_t& operator=(const igx_string_t& _) 
    {
        *this=_;
        return *this;
    }

#endif
} igx_string_t;

/* TODO. 
template <typename SIZE>
class itx_string_t
{
    ubit32_t length;
    char     str[SIZE];
};
*/

/**
* @brief 特定の文字列まで文字列をコピーします。
* @param dest 格納するバッファー
* @param src  ソース
* @param until 特定の文字列
* @return dest
*/
IGX_STRING_EXTERN char* igx_strcpy_until(char* dest, const char* src, const char* until);
/**
* @brief 特定の文字まで文字列をコピーします。
* @param dest 格納するバッファー
* @param src  ソース
* @param until 特定の文字
* @return dest
*/
IGX_STRING_EXTERN char* igx_strcpy_until_c(char* dest, const char* src, char until);
/**
* @brief 特定の文字列までの情報を取得します。
* @param dest 情報を取得する構造体
* @param str  検索対象
* @param util 特定の文字列

* @return 検索した文字列の先頭アドレス
*/
IGX_STRING_EXTERN char* igx_str_until(igx_string_t* str, const char* source, const char* util);
/**
* @brief 特定の文字列までの情報を取得します。(大文字小文字を無視します。)
* @param dest 情報を取得する構造体
* @param str  検索対象
* @param util 特定の文字列

* @return 検索した文字列の先頭アドレス
*/
IGX_STRING_EXTERN char* igx_str_case_until(igx_string_t* str, const char* source, const char* util);
/**
* @brief 特定の文字までの情報を取得します。
* @param dest 情報を取得する構造体
* @param str  検索対象
* @param util 特定の文字

* @return 検索した文字列の先頭アドレス
*/
IGX_STRING_EXTERN char* igx_str_until_c(igx_string_t* str, const char* source, const char util);
/**
* @brief 特定の文字までの情報を取得します。(大文字小文字を無視します。)
* @param dest 情報を取得する構造体
* @param str  検索対象
* @param util 特定の文字

* @return 検索した文字列の先頭アドレス
*/
IGX_STRING_EXTERN char* igx_str_case_until_c(igx_string_t* str, const char* source, const char util);
/**
* @brief 文字列を比較します。（内部では文字列の長さやハッシュキーを使って比較するので、\n
* 沢山の文字列を検索するには処理が早い。 \n
* (※ソートには向いていない。)
*
* @param str1 比較する文字列1
* @param str1 比較する文字列2
* 
* @return 0の場合は同じ、0でない場合は同じでない。
*/
IGX_STRING_EXTERN bit32_t igx_strcmp(igx_string_t* str1, igx_string_t* str2);
/**
* @brief 文字列を大文字小文字区別なしで比較します。（内部では文字列の長さやハッシュキーを使って比較するので、\n
* 沢山の文字列を検索するには処理が早い。 \n
* (※ソートには向いていない。)
*
* @param str1 比較する文字列1
* @param str1 比較する文字列2
* 
* @return 0の場合は同じ、0でない場合は同じでない。
*/
IGX_STRING_EXTERN bit32_t igx_strcasecmp(igx_string_t* str1, igx_string_t* str2);
/*
* @brief ハッシュキーを付与します。
*/
IGX_STRING_EXTERN void igx_str_set_hash(igx_string_t* str);
/**
* @brief igx_string_t形式のデータをメモリ上にロードします。
* @param p文字列
* @param mem_page 0の場合は無視
* @return @ref igx_string_t
*/
IGX_STRING_EXTERN igx_string_t* igx_str_load(const char* p, HITX mem_page);
/**
* @brief igx_str_loadでロードした文字列を削除します。
* @param str igx_str_loadでロードした文字列
* @param mem_page igx_str_loadで指定したmem_page
*/
IGX_STRING_EXTERN void igx_str_unload(igx_string_t* str, HITX mem_page);
/**
* @brief 特定の文字までの文字列の長さを取得します。
* @param src  ソース
* @param until 特定の文字列
* @return 文字列の長さ
*/
IGX_STRING_EXTERN size_t igx_strlen_until(const char* src, const char* until);
/**
* @brief 特定の文字までの文字列の長さを取得します。
* @param src  ソース
* @param until 特定の文字
* @return 文字列の長さ
*/
IGX_STRING_EXTERN size_t igx_strlen_until_c(const char* src, char until);
/**
* @brief 特定の文字を探し、startとendの間に囲まれている文字列を取得します。
* @param dest 格納するバッファー
* @param src  元のソース
* @param start 開始文字列
* @param end 終了文字列
* @return dest
* @retval 0 見つからない。
*/
IGX_STRING_EXTERN igx_string_t* igx_get_between_s(igx_string_t* dest, const char* src, const char* start, const char* end);
//IGX_STRING_EXTERN char* igx_get_between_s(char* dest, const char* src, const char* start, const char* end);
/**
* @brief 特定の文字を探し、startとendの間に囲まれている文字列を取得します。
* @param out 格納するバッファー
* @param src  元のソース
* @param start 開始文字
* @param end 終了文字
* @return dest
* @retval 0 見つからない。
*/
IGX_STRING_EXTERN igx_string_t* igx_get_between_c(igx_string_t* out, const char* src, char start, char end);
//IGX_STRING_EXTERN char* igx_get_between_c(char* dest, const char* src, char start, char end);
/**
* @brief utf8の文字列の数を取得します。
* @param src 文字列
* @return 文字の数
*/
IGX_STRING_EXTERN size_t igx_get_utf8_char_count(const char* src);
/**
* @brief utf-8の次の文字列を取得します。
* @param dest 次の文字を格納するバッファー
* @param src 検索対象の文字列
* @return 次の文字
*/
IGX_STRING_EXTERN char* igx_get_utf8_next_char(char* dest, const char* src);
/**
* @brief バッファーサイズに収まる文字列のインデックスを取得します。\n
* 文字列を右の用に省略する際に使います。 ex "長い文字列..."
*
* @param src 文字列
* @param buffer_size バッファーサイズ　
* @param is_complete 全て収まるかどうか
* @return バッファーにきれいに収まるバイト数
*/
IGX_STRING_EXTERN size_t igx_get_utf8_size_in_buffer(const char* src, size_t buffer_size, bool_t* is_complete);
/**
* @brief 文字の数分バッファーにコピーします。　
* @param dest コピーするバッファー
* @param src コピーする文字列
* @param num_of_char コピーする文字数

* @return コピーされた文字数
*/
IGX_STRING_EXTERN size_t igx_get_utf8_copy_char(char* dest, const char* src, size_t num_of_char);
/**
* @brief 次の一行を取得します。
*
* @param dest 行を格納するバッファー
* @param src 検索対象の文字列
* @return 次の行の始まり
*/
IGX_STRING_EXTERN igx_string_t* igx_get_next_line(igx_string_t* dest, const char* src);
/**
* @brief igxで割り当てられたメモリを削除します。
*/
IGX_STRING_EXTERN void igx_string_free(char* p);
/**
* @brief 指定した文字を探します。
* 
* @param src 検索対象の文字列
* @param ch 検索する文字
*
* @return 成功したら検索した文字の先頭アドレスを返します。失敗時は0
*/
IGX_STRING_EXTERN char* igx_str_ch(const char* src, char ch);


typedef bit32_t HSTRING_ADD;
/**
* @brief igxで割り当てられたメモリを削除します。
*
* @param buffer 文字列を書き込むバッファー
* @param buffer_size バッファーの最大サイズ
* @param string 文字列
* @param handle ハンドル
* @return 文字列の先頭アドレス　
*
* @startcode
ex)

struct any {
    char* name;
    char* title;
    int   any_value;
        :
        :
} *any_value;

    const char* name    = "any name";
    const char* title   = "any title";
    const size_t string_buffer_size = strlen(name)+1+strlen(any_value)+1;
    HSTRING_ADD handle;

    any_value = (struct any*)malloc(sizeof(struct any)+string_buffer_size);
            :
            :
            :


#define STRING_ADD(string) igx_string_add(any_value+1, string_buffer, string, &handle);

    any_value->name = STRING_ADD(name);
    STRING_ADD(title);
    STRING_ADD(name);

* @endcode
*
*/
IGX_STRING_EXTERN char* igx_string_add(void* buffer, size_t buffer_size, const char* string, HSTRING_ADD* handle);
/**
* @brief 文字コードを変換します。
*
* @param out 文字コードを受け取るバッファーのアドレス
* @param [in][out]buffer_size バッファーサイズ
* @param source 変換対象の文字列
* @param [in][out]source_size 文字列の長さ
* @param charset 文字コード
*
* @return 変換に失敗した際には0が返ります。outがNULLの場合は、igx_string_t* が返るのでキャストして使ってください。
*/
IGX_STRING_EXTERN char* igx_convert_utf8(char** out, size_t* buffer_size, char** source, size_t* source_len, const char* charset);
/**
* @breif 特定ロケールのcharsetを取得します。
* @param local_name ex) "ja", "ko", "zn" ....
* @param index ロケールのインデックス
* 
* @return ロケールのcharset

* @startcode
    while(const char* charset = igx_get_charset("ja", i++)) printf("%s\n", charset);
* @endcode
*/
IGX_STRING_EXTERN const char* igx_get_charset(const char* locale_name, bit32_t index);
/**
* @brief 大文字小文字関係なく文字列を比較します。
* 廃止予定 (strcasecmpをつかうこと！)
*
* @param s1 比較対象の文字列1
* @param s2 比較対象の文字列2
* @return 0は同じ、同じでなければ比較した文字の差分
*/
IGX_STRING_EXTERN int   _igx_stricmp(const char* s1, const char* s2);
/**
* @brief 文字列の中にchがいくつ含まれているかを返します。
* 
* @param src 検索対象の文字列
* @param src_size ソースのサイズ　
* @param ch  検索する文字
* 
* @return 検索する文字の数
*/
IGX_STRING_EXTERN bit32_t igx_str_count_ch(const char* src, size_t src_size, char ch);
/**
* @brief バッファーの中の指定した文字を取り除きます。　
*
* @param src ソース
* @param ch  取り除く文字
* @param size  srcの長さ、ゼロの場合は内部で長さを計る。
* @return 取り除いた際の文字列の長さ
*/
IGX_STRING_EXTERN bit32_t igx_str_remove_ch(char* src, char ch);
/**
* @brief バッファーに文字列をコピーします。バッファーが足りない場合は、足りない分"..."を付けます。　\n
* UTF-8 only.
*
* @param buffer バッファー
* @param size バッファーサイズ
* @param src  コピー元のバッファー
* @return コピーされた文字列のサイズ
*/
IGX_STRING_EXTERN size_t igx_strcpy_limit_length(char* buffer, size_t buffer_size, const char* src);
/**
* @brief igx_string_t形式をbufferにコピーします。
* @param [out] buffer 文字列を格納するバッファー
* @param buffer_size バッファーサイズ
* @param igx_str igx_string_tタイプの文字列
* 
* @return bufferのアドレス
*/
IGX_STRING_EXTERN char* igx_strcpy(char* buffer, size_t buffer_size, const igx_string_t* igx_str);

#define IGX_STR_IS_ALPHABET(ch) ((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z'))

#ifdef __cplusplus
#include <stdarg.h>
#define IGX_STRING_MAX_FORMAT_STR_BUFFER_LEN (4096)
/**
* @brief 文字列を扱う便利系クラス(いらない？)\n
* 考え方としては、文字列を参照で扱うか、実体で扱うか。\n
* const時は参照として扱い、constでない場合は実体で扱う。\n
* 内部では文字列の長さを保持しているので、文字列の処理が早い。
* 
* @startcode
    const IgxString str;
    str = "any string"; // 参照、以下のような感じ

    if (str=="other string")
    {
        return 0;
    }
    str+="additional string"; // -> build error.
            :
            :
    IgxString str;
    str = "any string"; // ヒープ上に割り当てる
    str+="additional string"; // "any stringaddtional string"
* @endcode
*/
class IgxString {
public:
    /* default contructor */
    IgxString();
    /* 
    * @brief 文字列をコピーして初期化します。文字列はヒープに割り当てられます。
    * @param p 初期化する文字列
    * @param mem_page メモリを割り当てる際のメモリプールのアドレス。\n
    * 0の場合はIgxMemPageを使わない。
    */
    IgxString(IgxMemPage* mem_page, const char* p);
    /**
    * @brief 文字列のバッファーを指定します。このコンストラクターで初期化される場合は、\n
    * 内部的にメモリを割り当てる事はありません。\n
    * もし、バッファーを超えた場合は、バッファーエラーを返します。
    */
    IgxString(char* buffer, size_t buffer_size);
    /* 
    * @brief 文字列をコピーして初期化します。文字列はヒープに割り当てられます。\n
    * IgxMemPageを使う場合は、IgxMemPageに専用のIGX_STRING_MAX_FORMAT_STR_BUFFER_LENのバッファーが容易されますが、
    * 使わない場合は、 スタックにIGX_STRING_MAX_FORMAT_STR_BUFFER_LEN分のスタックを確保します。\n
    * その場合は再帰呼び出しなどには使うべきではありません。
    *
    * @param mem_page 0の場合は使わない。
    * @param format フォーマット
    * 0の場合はIgxMemPageを使わない。
    * @startcode
    * IgxString string(&mem_page, "%04d-%02d%02d %02d:%02d:%02d", year, month, day, hour, min, sec);
    * @endcode
    */
    IgxString(IgxMemPage* mem_page, const char* format, ...);
    /*
    * @brief 文字列を参照にします。\n
    * もし、buffer_sizeが0の場合は、ReadOnlyで見なされます。
    * 
    * @param str 文字列
    * @param buffer_size バッファーサイズ、0の場合はReadOnly
    * 
    * @startcode
    *
    * IgxString str("any strings......");
    * :
    * :
    * char buffer[MAX_BUFFSER_SIZE];
    * IgxString str(buffer, sizeof(buffer));
    */
    IgxString(const char* str);
    /* destructor */
    virtual ~IgxString(){};

/* operatoions */
    /**
    * @brief 文字列をコピーします。
    * @param string 文字列
    * @param len 文字列の長さ（デフォルトでは内部で長さを計ります。)
    *
    * @return コピーされた文字列の長さ
    */
    size_t Copy(const char* string, size_t len=0);
    /**
    * @brief 特定のフォーマットの文字列をコピーします。\n
    * IgxMemPageを使う場合は、IgxMemPageに専用のIGX_STRING_MAX_FORMAT_STR_BUFFER_LENのバッファーが容易されますが、\n
    * 使わない場合は、 スタックにIGX_STRING_MAX_FORMAT_STR_BUFFER_LEN分のスタックを確保します。\n
    * その場合は再帰呼び出しなどには使うべきではありません。
    * IgxString str();
    * str.Copy("%04d-%02d%02d %02d:%02d:%02d", year, month, day, hour, min, sec);
    * printf("%s", (char*)str);
    */
    bit32_t CopyWithFormat(const char* format, ...);
    /**
    * @brief 文字列を後ろに追加します。
    * @param string 文字列
    * @param len 文字列の長さ（デフォルトでは内部で長さを計ります。)
    * 
    * @return 最終的にコピーされた文字列の長さ
    */
    size_t Add(const char* string, size_t len=0);
    /**
    * @brief 特定のフォーマットの文字列をコピーします。
    * @format フォーマット
    * IgxMemPageを使う場合は、IgxMemPageに専用のIGX_STRING_MAX_FORMAT_STR_BUFFER_LENのバッファーが容易されますが、
    * 使わない場合は、 スタックにIGX_STRING_MAX_FORMAT_STR_BUFFER_LEN分のスタックを確保します。\n
    * その場合は再帰呼び出しなどには使うべきではありません。
    */
    bit32_t AddWithFormat(const char* format, ...);
    /**
    * @brief 文字列をindexに挿入します。
    * @param  index インデックス
    * @param string 文字列
    * @param len 文字列の長さ（デフォルトでは内部で長さを計ります。)
    * 
    * @return 最終的にコピーされた文字列の長さ
    */
    size_t Insert(size_t index, const char* string, size_t len=0);
    /**
    * @brief 特定のフォーマットの文字列をindexにコピーします。
    * @param  index インデックス
    * @format フォーマット
    * IgxMemPageを使う場合は、IgxMemPageに専用のIGX_STRING_MAX_FORMAT_STR_BUFFER_LENのバッファーが容易されますが、
    * 使わない場合は、 スタックにIGX_STRING_MAX_FORMAT_STR_BUFFER_LEN分のスタックを確保します。\n
    * その場合は再帰呼び出しなどには使うべきではありません。
    */
    bit32_t InsertWithFormat(size_t index, const char* format, ...);
    /**
    * @brief 文字列を設定します。
    * @param string 文字列
    * @param len 文字列の長さ（デフォルトでは内部で長さを計ります。)
    *
    * @return 設定された文字列の長さ
    */
    size_t Set(const char* string, size_t len=0) const;
    /**
    * @brief 検索時に大文字小文字区別をするかどうかを設定します。\n
    * デフォルトはfalse
    */
    inline void SetIgnoreCase(bool_t ignore);
    inline void SetIgnoreCase(bool_t ignore) const;
    /**
    * @brief 文字列を検索します。
    * @param key 検索対象の文字列
    * @param index  検索を開始するインデックス
    * 
    * @return -1でなければ、文字列が始まるインデックス
    */
    bit32_t Find(const char* key, bit32_t index=0);
    bit32_t Find(const char* key, bit32_t index=0) const;
    /**
    * @brief 文字を検索します。
    * @param ch 検索対象の文字
    * @param index  検索を開始するインデックス
    * 
    * @return -1でなければ、文字列が始まるインデックス
    */
    bit32_t Find(const char ch, bit32_t index=0);
    bit32_t Find(const char ch, bit32_t index=0) const;
    /**
    * @brief 文字列を後ろから検索します。
    * @param key 検索対象の文字列
    * @param index  検索を開始するインデックス
    * 
    * @return -1でなければ、文字列が始まるインデックス
    */
    bit32_t FindFromTail(const char* key, bit32_t index=0);
    bit32_t FindFromTail(const char* key, bit32_t index=0) const;
    /**
    * @brief 文字を後ろから検索します。
    * @param ch 検索対象の文字
    * @param index  検索を開始するインデックス
    * 
    * @return -1でなければ、文字列が始まるインデックス
    */
    bit32_t FindFromTail(char ch, bit32_t index=0);
    bit32_t FindFromTail(char ch, bit32_t index=0) const;
    /**
    * @brief 特定の文字列で始まるかどうかをチェックします。
    * @param word 検索する文字列
    * @param index  検索開始インデックス
    * @return true、false
    */
    bool_t BeginWith(const char* word, size_t word_len=0, bit32_t index=0);
    bool_t BeginWith(const char* word, size_t word_len=0, bit32_t index=0) const;
    /**
    * @brief 文字列を後ろから検索します。
    * @param word 検索する文字列
    * @param index  後ろからの検索開始インデックス
    * @return true、false
    * 
    * @startcode
    * const IgxString str(path);
    *
    *       :
    *       :
    * str.SetIgnoreCase(true);
    * if (str.EndWith(".jpg")!=false)
    * {
    *     DrawJpegFile((const char*)str);
    * }
    * else if (str.EndWith(".png")!=false)
    * {
    *     DrawPngFile((const char*)str);
    * }
    * 
    */
    bool_t EndWith(const char* word, size_t word_len=0, bit32_t index=0);
    bool_t EndWith(const char* word, size_t word_len=0, bit32_t index=0) const;
    /**
    * @brief バッファーにstartからendまでの文字列を取り出します。
    * @param [out]str バッファー
    * @param s 文字列の開始オフセット
    * @param e 取り出す後ろのオフセット　（デフォルトでは最後まで）
    */
    igx_string_t* SubString(igx_string_t& str, bit32_t s, bit32_t e) const;
    igx_string_t* SubString(igx_string_t& str, bit32_t s, bit32_t e);
    /**
    * @brief 文字列をformat形式でパースします。
    * @param format フォーマット
    * @param ... 可変長の変数
    *
    * @return パースされたエレメントの数を返します。
    */
    bit32_t ParseWithFormat(const char* format, ...);
    bit32_t ParseWithFormat(const char* format, ...) const;
    /**
    * @brief 文字列をformat形式でパースします。
    * @param index  開始インデックス
    * @param format フォーマット
    * @param ... 可変長の変数
    *
    * @return パースされたエレメントの数を返します。
    */
    bit32_t ParseWithFormat(bit32_t index, const char* format, ...);
    bit32_t ParseWithFormat(bit32_t index, const char* format, ...) const;
    /**
    * @brief 置換します。
    * @param src 置換前の文字列
    * @param rep 置換後の文字列
    * @return 置換した回数　
    */
    bit32_t Replace(const char* src, const char* rep);
/* operator overloads */
    /*
    * @brief キャストオーバーロード (igx_string_t*)
    */
    inline IGX_STR_OPERATOR const igx_string_t*();
    /*
    * @brief キャストオーバーロード (igx_string_t*)
    */
    inline IGX_STR_OPERATOR const igx_string_t*() const;
    /*
    * @brief キャストオーバーロード (const igx_string_t*)
    */
    inline IGX_STR_OPERATOR igx_string_t*();
    /*
    * @brief キャストオーバーロード (const igx_string_t*)
    */
    inline IGX_STR_OPERATOR igx_string_t*() const;
    /*
    * @brief キャストオーバーロード (const char*)
    */
    inline IGX_STR_OPERATOR const char*();
    /*
    * @brief キャストオーバーロード (const char*)
    */
    inline IGX_STR_OPERATOR const char*() const;
    /*
    * @brief キャストオーバーロード (char*)
    */
    inline IGX_STR_OPERATOR char*();
    /*
    * @brief キャストオーバーロード (char*)
    */
    inline IGX_STR_OPERATOR char*() const;
    /*
    * @brief キャストオーバーロード (size_t)
    */
    inline IGX_STR_OPERATOR size_t();
    /*
    * @brief キャストオーバーロード (size_t)
    */
    inline IGX_STR_OPERATOR size_t() const;
    /*
    * @brief コピー
    */
    inline IgxString& operator=(const char* p);
    /*
    * @brief コピー
    */
    inline IgxString& operator=(char* p);
    /*
    * @brief レファレンス設定
    */
    inline const IgxString& operator=(const char* p) const;
    /*
    * @brief レファレンス設定
    */
    inline const IgxString& operator=(char* p) const;
    /*
    * @brief igx_string_tのコピー
    */
    inline const IgxString& operator=(igx_string_t& p) const;
    /*
    * @brief igx_string_tのコピー
    */
    inline IgxString& operator=(igx_string_t& p);
    /*
    * @brief igx_string_tのコピー
    */
    inline const IgxString& operator=(const igx_string_t& p) const;
    /*
    * @brief igx_string_tのコピー
    */
    inline IgxString& operator=(const igx_string_t& p);
    /*
    * @brief 後ろに追加コピーのみ
    */
    inline IgxString& operator+=(const char* p);
    /* 
    * @brief 後ろに追加コピーのみ
    */
    inline IgxString& operator+=(char* p);
    /* 
    * @brief 比較
    */
    inline bool_t operator==(char* p);
    /* 
    * @brief 比較
    */
    inline const bool_t operator==(char* p) const;
    /* 
    * @brief 比較
    */
    inline bool_t operator==(const char* p);
    /* 
    * @brief 比較
    */
    inline const bool_t operator==(const char* p) const;
    /* 
    * @brief 配列
    */
    inline char operator[](bit32_t index);
    /* 
    * @brief 配列
    */
    inline const char operator[](bit32_t index) const;
private:
    mutable igx_string_t m_str;
    /* バッファーサイズ */
    mutable size_t m_buffer_size;
    /* 作業用のバッファー */
    char*  m_working_buffer;
    /* メモリプール */
    IgxMemPage* m_mem_page;
    /* その他 */
    mutable struct {
        unsigned is_allocated:1;
        unsigned ignore_case:1;
        unsigned using_buffer:1;
        unsigned reserved:29;
    } m_flags;

    void  init_member();
    char* strdup(const char* str, size_t len);
    size_t strcat(const char* str, size_t len);
    size_t strinsert(bit32_t index, const char* str, size_t len);
    char* str_malloc(size_t size);
    void str_free();

    bit32_t copy_with_format_h(bit32_t index, bit32_t type, const char* format, va_list ap);
    bit32_t copy_with_format(bit32_t index, bit32_t type, const char* format, va_list ap);
};

inline IgxString::IGX_STR_OPERATOR const igx_string_t*()
{
        return &m_str;
}
inline IgxString::IGX_STR_OPERATOR const igx_string_t*() const
{
        return &m_str;
}
inline IgxString::IGX_STR_OPERATOR igx_string_t*()
{
    return &m_str;
}
inline IgxString::IGX_STR_OPERATOR igx_string_t*() const
{
    return (igx_string_t*)&m_str;
}
inline IgxString::IGX_STR_OPERATOR const char*()
{
    return m_str.str;
}
inline IgxString::IGX_STR_OPERATOR const char*() const
{
    return m_str.str;
}
inline IgxString::IGX_STR_OPERATOR char*()
{
    return m_str.str;
}
inline IgxString::IGX_STR_OPERATOR char*() const
{
    return m_str.str;
}
inline IgxString::IGX_STR_OPERATOR size_t()
{
    return m_str.length;
}
inline IgxString::IGX_STR_OPERATOR size_t() const
{
    return m_str.length;
}
inline IgxString& IgxString::operator=(const char* p)
{
    Copy(p);
    return *this;
}
inline const IgxString& IgxString::operator=(char* p) const
{
    Set(p);
    return *this;
}
inline const IgxString& IgxString::operator=(const char* p) const
{
    return (const IgxString&) operator=((char*)p);
}
inline IgxString& IgxString::operator=(char* p)
{
    return operator=((const char*)p);
}
inline IgxString& IgxString::operator=(const igx_string_t& p) 
{
    m_str=p;
    return *this;
}
inline const IgxString& IgxString::operator=(const igx_string_t& p) const
{
    m_str=p;
    return *this;
}
inline IgxString& IgxString::operator=(igx_string_t& p) 
{
    m_str=p;
    return *this;
}
inline const IgxString& IgxString::operator=(igx_string_t& p) const
{
    m_str=p;
    return *this;
}
inline IgxString& IgxString::operator+=(const char* p)
{
    Add(p, strlen(p));
    return *this;
}
inline IgxString& IgxString::operator+=(char* p)
{
    return operator+=((const char*)p);
}
inline bool_t IgxString::operator==(char* p)
{
    if (m_flags.ignore_case==1)
    {
        return strcmp(p, m_str.str)==0?true:false;
    }
    return strcasecmp(p, m_str.str)==0?true:false;
}
inline bool_t IgxString::operator==(const char* p)
{
    return operator==((char*)p);
}
inline char IgxString::operator[](bit32_t index)
{
    return m_str.str[index];
}
inline void IgxString::SetIgnoreCase(bool_t ignore)
{
    m_flags.ignore_case=ignore;
}
inline void IgxString::SetIgnoreCase(bool_t ignore) const
{
    m_flags.ignore_case=ignore;
}

#endif /* clpluplus */


#endif
