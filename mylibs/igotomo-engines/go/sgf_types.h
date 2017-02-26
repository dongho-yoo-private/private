#ifndef _SGF_TYPES_H_
#define _SGF_TYPES_H_

#include "../types.h"
#include "go_types.h"

namespace go {

/** 石のノード*/
typedef struct sgf_node_t{
    stone_t stone;
    char* comment;
    struct {
        sgf_node_t* prev;
        sgf_node_t* next;
        sgf_node_t* child;
    } pointer;
} sgf_node_t;

#define SGF_MAX_TITLE_LEN (127)
#define SGF_MAX_PLACE_LEN (63)

typedef struct sgf_header_t {
    /*タイトル (XXX戦第X局)*/
    char title[SGF_MAX_TITLE_LEN+1];
    /*場所*/
    char place[SGF_MAX_TITLE_LEN+1];
    /*説明*/
    char* comment;
    /* 置き石の数 */
    bit8_t handycap; 
    /* プレイヤー情報*/
    struct {
        char name[SGF_MAX_NAME_LEN+1];
        char level[SGF_MAX_LEVEL_LEN+1]; /* 1k, 1d, ... */
        unsigned bit32_t remain_time;
        unsigned bit16_t catched_stone_count;
        void* extra_info;
    } player[2]; /*0:black, 1:white*/
    /* 結果 */
    struct {
        stone_type_t winner;
        win_type_t   type;
        bit16_t      score;
    } result;
} sgf_header_t;

}

#endif
