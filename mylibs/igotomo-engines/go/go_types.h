/**
* Copyright (c) 2015 Igotomo.
* authorized by dongho.yoo.
*/
#ifndef __GO_TYPES_H__
#define __GO_TYPES_H__

#include "igx_types.h"
#include "library/igx_time.h"
#include "library/igx_string.h"
#include "data_struct/itx_linked_list.h"

/**@brief 囲碁のエンジンは汎用的なモジュールではないのでネームスペースを付けた方が良いと思った。*/
namespace go {

typedef enum
{
    kBoardSize19=0,
    kBoardSize13,
    kBoardSize9
} board_size_t;

#define kMaxBoardSize (19)

namespace stone {
typedef enum
{
    kNone=0,
    kBlack,
    kWhite,
    kInvalid,
    kEof=kInvalid
} type_t;
}

namespace label {
typedef enum
{
    kNone=0,
    kCircle,
    kTriAngle,
    kDiamond,
    kRectangle
} type_t;
}

#define MAX_STONE_ORDER (511)
/**
* @brief 碁石データ構造 (32ビット版)\n
* 実際の個々の石の情報はこのデータを使います。\n
* 注意すべきは描画時はこの構造体は使われません。
*/
typedef union  __stone__
{
   bit32_t stone;
   struct {
       /** @brief 石の種別 @ref stone_type_t */
       unsigned type: 2;
       /** @brief パス */
       unsigned is_pass:1; 
       /** @brief 石の座標 x*/
       unsigned x:5;
       /** @brief 石の座標 y*/
       unsigned y:5;
       /** @brief このフラグは地合計算時選択されたあげ石 @ref StartCalculate*/
       unsigned selected:1; 
       /** @brief 手順 512手まで*/
       unsigned order:9; 
       /** @brief 0:どちらでもない。1:黒、2:白 */
       unsigned area_flag:2;
       /** @brief レザーブ */
       unsigned reserved:7;
   };
   bool_t operator==(union __stone__& _stone)
   {
        return (type==_stone.type&&x==_stone.x&&y==_stone.y);
   }
   bool_t operator==(const union __stone__& _stone)
   {
        return (type==_stone.type&&x==_stone.x&&y==_stone.y);
   }
   bool_t operator!=(union __stone__& _stone)
   {
        return !(type==_stone.type&&x==_stone.x&&y==_stone.y);
   }
   bool_t operator!=(const union __stone__& _stone)
   {
        return !(type==_stone.type&&x==_stone.x&&y==_stone.y);
   }
   void set(bit32_t x, bit32_t y, stone::type_t type, bit32_t order=0)
   {
        stone=0;
        this->x=x;
        this->y=x;
        this->type=type;
        this->order=order;
   }

} stone_t;

typedef stone_t (*goban_t)[kMaxBoardSize];

extern const stone_t kNullStone;
extern const stone_t kStoneError;


#define kNULL_STONE (go::kNullStone)
#define kSTONE_ERROR (go::kStoneError)
#define IS_NULL_STONE(stone) ((stone_t)stone==(stone_t)kNullStone)
#define REVERSE_STONE(type) ((type==stone::kBlack)?stone::kWhite:stone::kBlack)

/**
* @brief 碁石データ構造 (8ビット版)\n
* サムネール専用のものです。\n
* 8bitに４つの碁石がはいります。
*/
typedef union  __stone_thumbnail__
{
    ubit8_t stone;
    struct {
       /** @brief 石の種別 @ref stone_type_t */
       unsigned type1: 2;
       unsigned type2: 2;
       unsigned type3: 2;
       unsigned type4: 2;
    };
} stone_th_t;
/**
* @brief 碁石のサムネール版のデータ構造です。
* @startcode
 ex)
 stone_thumbnail_t stones;
 size_t size;
 if (igx_file_load("sample.itb", &stones, sizeof(stones), &size)!=0)
 {
    int cnt=0, x=0,y=0;
    const int size = 19;
    const int bit_size = 2;
    const int count = size*size; // 361 

    for (int i=0; i<size; ++i, count=(i>>2))
    {
        const bit32_t shift = (sizeof(stone_th_t)-bit_size)-((i&0x03)<<1);
        DrawStone(x++, y, (go::stone::type_t)(stones[count].stone>>shift));
        if (x==size) x=0, ++y;
    }
 }
 else 
 {
    IGX_CRITICAL("%s is not found", "sample.itb");
 }

* @endcode
*/
typedef stone_th_t (*stone_thumnail_t)[91];

/**
* @brief ラベルを表示するためのデータ構造
*/
typedef union {
    ubit32_t label;
    struct {
        /** @brief 座標 x*/
        unsigned x:5;
        /** @brief 座標 y*/
        unsigned y:5;
        /** @brief @ref label_type_t **/
        unsigned type:3; 
        unsigned ch:8;
        unsigned reserved:11;
    };
} label_t;

#define ITX_MAX_PLAYER_NAEM_LEN (31)
#define ITX_MAX_PLAYER_POST_NAME_LEN (31)
#define ITX_MAX_EVENT_NAME_LEN (63)

typedef struct __rules__ {
     /**@brief KM[] コミ 250まで？*/
     unsigned komi:8; 
     /**@brief コミをもらう方 1:black, 0:white*/
     unsigned komi_is_black:1; 
     /**@brief 半目あり 1:あり0:なし*/
     unsigned half_score:1;
     /**@brief 制限時間あり・なし*/
     unsigned limit_time:1;
     /**@brief 長い秒読みの回数 OT[] 1〜15*/
     unsigned long_second_count:4;
     /**@brief OT[] 長い秒読みの秒数 30秒 7分30秒までOT[5x30], OT[25 moves/ 10 min].... */
     unsigned long_second_term:4;
     /**@brief STM[] 無制限秒読みの長さ 0: 無し 1:10, 2:15, 3:30, 4:60, 5:90, 6:120, 7:180 3分まで*/
     unsigned short_second_term:3;
     /**@brief RU[] 0:日本、1:中国*/
     unsigned rule:1;
     /**@brief 碁盤サイズ 0:19,1:13、2:9*/
     unsigned size:2;
     /**@brief 置き石の数 0-15まで*/
     unsigned handycap:4;
     /**@brief */
     unsigned reserved:2;
} rules_t;
/**@brief 対局情報*/
typedef struct __event__ {
    /**@brief EV 対局名*/
    char *name;
    /*@brief PC 対局場所*/
    char *place;
    /*@brief C 対局概要*/
    char *comment;
    /**@brief RO 回数 */
    char *count;
    /**@brief DT 対局開始日時*/
    igx_time_t date;
    /**@brief CA 文字コード*/
    igx_charset_t char_set;
    /**@brief AP sgfを作成したアプリケーション名*/
    char* application;
    /**@brief 何か必要なときに。。。*/
    bit8_t reserved[3];
    /**@brief 対局のルール*/
    rules_t rules;
    /**@brief TM 制限時間*/
    igx_time_t limit_time;
} event_t;

namespace level {
/** @brief 段位 */
typedef enum {
    /** @brief プロ２段 (go::level::kPro|2) */
    kPro = 0x8000,
    /** @brief 7級 (go::level::kKyu|7), アマ３段 (3) */
    kKyu = 0x4000,
    /** @brief 不明*/
    kUnknown=0xF000
}type_t;
}

/**@brief 対局者情報*/
typedef struct __player__ {
   /**@brief PB, PW プレーヤの表示名*/
   char* name;
   /**@brief BTN WTN プレーヤの称号*/
   char* title;
   /**@brief BT, WT プレーヤのチーム*/
   char* team;
   /**@brief BR, WR プレーヤの気力 level_type_tを使用*/
   bit32_t level;
   /**@brief 残り時間 いらない？*/
   igx_time_t time;
}player_t;

/**@brief 結果 RE */
typedef struct __result__{
    /**@brief 0:なし、1:黒の勝ち、2:黒の負け、3:引き分け*/
    unsigned result:2;
    /**@brief 中押し:1*/
    unsigned type:1;
    /**@brief score:〜361*/
    unsigned score:9;
    /**@brief */
    unsigned reserved:4;
} result_t;

/**@brief SGF形式のヘッダー*/
typedef struct {
    /**@brief 対局情報*/
    event_t event;
    /**@brief 対局者情報, 0:黒、1:白*/
    player_t player[2];
    /**@brief 結果*/
    result_t result;
    /**@brief igotomo追加フォーマット*/
    // TODO.
} sgf_header_t;

typedef struct __stone_node__ {
    struct {
        /**@brief 石*/
        stone_t stone;
        /**@brief 置き石などのリスト*/
        ItxLinkedList<stone_t>* initial_stone_list;
        /**@brief ラベルリスト*/
        ItxLinkedList<label_t>* label; 
        /**@brief コメント*/
        char* comment;
        /**@brief 使用時間*/
        float remain_time;
    } data;

    struct __stone_node__* prev;
    struct __stone_node__* next;
    struct __stone_node__* parent;
    ItxLinkedList<struct __stone_node__*>* branches;
} stone_node_t; /* 32bit : 36bytes, 64bit 72bytes */

/**@brief 最大の碁石の数 **/
#define kMaxStoneCount (361)

} /* namespace go */

#endif 
