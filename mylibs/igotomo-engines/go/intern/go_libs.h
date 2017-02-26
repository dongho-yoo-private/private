/**
* Copyright (c) 2015 igotomo
*
* authorized dongho.yoo.
* @brief go内部で使われる専用の共通モジュール
*/
#ifndef _IGX_GO_LIB_H_
#define _IGX_GO_LIB_H_

#include "go/go_types.h"

namespace go { namespace libs {

typedef struct {
    char* comment;
    size_t      comment_len;
    char* init_stone_addr;
    ubit32_t init_stone_count;
    char* label_addr;
    char* label_end_addr;
    ubit32_t label_count;
    stone_t stone;
    char* node_start_addr;
    char* node_end_addr;
} node_info_t;

/** @brief タグを検索し、長さを取り出す。*/
extern const char* find_tag(const char* sgf, const char* tag, size_t* length);

/** @brief タグを検索し、中身を取り出す。*/
extern const char* find_tag(const char* sgf, const char* tag, char* dest);
/** @brief 文字列から'\'を削除します。
*   @param buffer 受け取るバッファー(NULLの場合はメモリを割り当てます。)
*   @param src 文字列
*   @param src_len ソースの文字列の長さ
*   @return 
*/
extern char* remove_escape_sequence(char* buffer, const char* src, size_t src_len);

/** @brief ボディーの先頭アドレスを返します。*/
extern char* find_body(const char* sgf);
/**
* @param ノードの情報を取得します。
*/
extern char* get_node_info(const char* sgf, stone_t* init_stone, label_t* label, char** comment);
/**
* @brief 次のノード情報を取得します。
*
* @return 次のノードの開始アドレス。
*/
extern char* get_node_info(node_info_t& node_info);
/**
* @brief 次のノードを検索します。
*/
extern char* find_next_node(const char* sgf);
/**
* @brief ブランチの最後を探します。
* @return ブランチの最後
*/
extern char* find_branch_end(const char* sgf);
/**
* @brief ヘッダー碁盤サイズのビットを実際の碁盤サイズの整数に変更します。
*/
extern bit32_t get_goban_size(bit32_t n);
/**
* @brief ラベルのタイプからタグの文字列を返します。
*/
extern const char* get_label_string(go::label::type_t type);
/**
* @brief コメントの中に表示しては行けない文字の数を取得します。
* 
* @param comment
* @return 文字列の数
*/
extern bit32_t get_skip_char_count(const char* comment);
/**
* @brief コメントに入っては行けない文字であるかどうか。
*
* @param ch 判定する文字
* @return trueの場合は入ってはいけない文字、falseの場合は問題無し
*/
extern bool_t is_skip_char(char ch);
/**
* @brief 文字列からユーザのレベルを取得します。
*/
extern go::level::type_t get_player_level_from_string(const char* str_level);
/**
* @brief レベルの文字列に変換します。
*
* @param level @ref go::level::type_t
* @return level;
*/
extern char* get_player_level_to_string(char* buffer, bit32_t level);
}}/* namespace go::libs*/


#endif
