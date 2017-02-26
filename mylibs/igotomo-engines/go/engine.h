/**
* Copyright (c) 2015 igotomo
*
* Authorized by dongho.yoo.
*/
#ifndef _GO_ENGINE_H_
#define _GO_ENGINE_H_

#include "go_types.h"
#include "sgf.h"
#include "data_struct/itx_stack.h"
#include "data_struct/itx_queue.h"

#ifndef STORE 
#   define GO_ENGINE_DEBUG 
#endif

namespace go {

/**
* @brief 囲碁エンジン
*
* @startcode
* go::sgf sgf;
* sgf.Import("sample.sgf");
*
* go::engine engine(sgf);
* go::stone_t (*stone)[19] = engine.GetCurrent();
* const go::sgf_header_t& header = engine.GetHeader();
* int size         = header.event.rule.size;
*
* for (int i=0; i<size; ++i)
* {
      for (int j=0; j<size; ++j)
      {
          DrawStone(stone[j][i]);
      }
  }
  const int init_stone_count = engine.GetInitStoneCount();
  for (int i=0; i<init_stone_count; ++i)
  {
      go::label_t label = engine.GetInitStone(i);
      DrawStone(label);
  }
  const int label_count = engine.GetLabelCount();
  for (int i=0; i<label_count; ++i)
  {
      go::label_t label = engine.GetLabel(i);
      DrawLabel(label);
  }
*
* @endcode
*/
class engine 
{
public:
    typedef enum {
        /*@brief 編集モード*/
        kModeEdit,
        /*@brief 対局モード */
        kModePlay,
        /*@brief 死活モード */
        kModeDeathLife
    } mode_t;
    typedef enum {
        /** @brief 通常の状態 */
        kStateNormal,
        /** @brief 地合計算の状態です。@ref StartCalculate */
        kStateStartCalculate,
        /** @brief 地合計算の状態です。@ref EndCalculate */
        kStateEndCalculate,
        /** @brief 読み込む専用 **/
        kStateReadOnly,
    } state_t;

public:
    /**
    * @brief デフォルトコンストラクター
    * @param state 初期ステータスはkStateEditかkStatePlay, kReadOnlyのみ指定可能です。
    */
    engine(mode_t mode=kModeEdit);
    /**
    * @brief デストラクター
    */
    virtual ~engine();

/** +++++++++++++++ initailize +++++++++++++++++++++++ **/
    /*
    * @brief ヘッダーの内容で初期化します。（編集などに向いている）
    *
    * @return true 成功、false 失敗　
    */
    bool_t Initialize(const sgf_header_t& header);
    /**
    * @brief sgf形式のファイルから初期化を行います。
    * @return true 成功、false 失敗
    */
    bool_t Initialize(const char* file_name);
    /**
    * @brief sgf形式のデータから初期化を行います。
    * @param sgf_data データ
    * @param data_size データサイズ (0の場合は、内部で長さを計ります。)
    * @return true 成功、false 失敗
    */
    bool_t Initialize(const void* sgf_data, size_t data_size=0);
    /**
    * @brief 終了処理をおこないます。
    */
    void Terminate();
    /**
    * @brief データをクリアーします。\n
    * この状態は、コンストラクターが呼び出された直後の状態となるので、
    * 新たにInitializeを呼ぶ事が可能です。
    */
    void Clear();
    /**
    * @brief sgfを取得します。\n
    * 注意すべきは、constになっているので、export系の限定の使い方となります。
    * @return @ref go::sgf
    *
    * @startcode
    *
    *         :
    *         :
    * go::engine engine();
    * if (engine.Initialize("sample.sgf")==false)
    * {
    *    return false;
    * }
    *
    *         :
    * engine.AddStone(3, 3);
    * engine.AddComment("この場合の急所は3・3");
    *
    * const go::sgf& sgf = engine.GetSgf();
    * if (sgf.ExportFile("sample.sgf")==false)
    * {
    *      return false;
    * }
    * @endcode
    */
    const sgf& GetSgf() const;

/** +++++++++++++++ interface to input +++++++++++++++ **/
    /**
    * @brief 石を置きます。
    * @param x x座標　
    * @param y y座標
    * @param is_pass パスの場合はtrue
    *
    * @retval 石の情報が返ります。
    * @retval 成功 石の情報
    * @retval 失敗 kSTONE_ERROR
    */
    stone_t AddStone(int x, int y, bool_t is_pass=false);
    stone_t AddStone(int x, int y, bool_t is_pass=false) const;
    /**
    * @brief 手順と関係ない置き石を追加します。\n
    * 同じ場所に置くと削除されます。
    *
    * @return 置き石の数
    */
    bit32_t AddInitStone(int x, int y, stone::type_t type);
    bit32_t AddInitStone(int x, int y, stone::type_t type) const;
    /**
    * @brief ラベルを設定します。\n
    * 既に同じ場所にラベルがある場合は削除されます。
    * @param x x座標　
    * @param y y座標
    * @param type タイプ
    *
    * @return ラベルの数
    */
    bit32_t AddLabel(int x, int y, label::type_t type, char ch);
    /**
    * @brief コメントを追加します。
    *
    * @param comment コメント(UTF-8形式のみ）
    * @return コメントの長さ。
    */
    bit32_t AddComment(const char* comment);

/** +++++++++++++++ interface to draw +++++++++++++++ **/
    /**
    * @brief 現在の碁盤の配列を取得します。\n
    * 最大19路盤向けの19×19の配列。
    * 他のサイズも共通で使います。
    *
    * @return stone_tの19×19の配列(9、13路盤も19配列で受け取ります。)
    * @startcode
        ex)
                        :
                        :
        stone_t (*stones)[19] = engine.GetCurrent();
	contst goban_size = engine.GetGobanSize();
        for (int i=0; i<goban_size; ++i)
        {
            for (int j=0; j<goban_size; ++j)
            {
                draw_stone(stones[i][j].type, stones[i][j].x, stones[i][j].y);
            }
        }
    * @endcode
    */
    const stone_t (*GetCurrent() const)[kMaxBoardSize]; 
    /**
    * @brief 先頭へ移動します。
    *
    * @return stone_tの19×19の配列
    */
    const stone_t (*MoveTop() const)[kMaxBoardSize]; 
    /**
    * @brief 最後へ移動します。\n
    * 注意すべきは途中でブランチがある場合は、最後まで行かずブランチを選択させる為に止まります。\n
    * この場合、ブランチを選択させて次に進ませるベキです。
    * @return stone_tの19×19の配列
    * TODO. 仕様をどうするか。。。
    */
    const stone_t (*MoveEnd() const)[kMaxBoardSize];
    /**
    * @brief 現在からoffset分移動します。マイナスの場合は前に移動します。\n
    * もし、offsetが範囲を超えた場合は先頭か最後に移動します。
    * @param offset 0より大きい場合は、後ろへ、小さい場合は前へ移動します。\n
    * 0の場合は現在の碁盤の石の配列を返します。\n
    *
    * @return stone_tの19×19の配列\n
    * 注意すべきは、置き石なども一緒に混ざっていること。
    */
    const stone_t (*MoveFromCurrent(int offset) const)[kMaxBoardSize]; 
    /**
    * @brief 次のブランチまで移動します。
    *
    * @return stone_tの19×19の配列
    */
    const stone_t (*MoveNextBranch() const)[kMaxBoardSize]; 
    /**
    * @brief まえのブランチまで移動します。
    *
    * @return stone_tの19×19の配列
    */
    const stone_t (*MovePrevBranch() const)[kMaxBoardSize]; 
    /**
    * @brief 次のコメントまで移動します。
    *
    * @return stone_tの19×19の配列
    */
    const stone_t (*MoveNextComment() const)[kMaxBoardSize];
    /**
    * @brief まえのコメントまで移動します。
    *
    * @return stone_tの19×19の配列
    */
    const stone_t (*MovePrevComment() const)[kMaxBoardSize]; 
    /**
    * @brief 現在の石の情報を取得します。
    * @return 最後に置かれた石。
    */
    inline stone_t GetStone() const
    {
        stone_node_t* node = m_sgf.GetCurrent();
        if (node!=0)
        {
            return node->data.stone;
        }
        return kNULL_STONE;
    }
    /**
    * @brief 現在のコメントを取得します。
    * @return コメント
    */
    inline const char* GetComment() const
    {
        stone_node_t* node = m_sgf.GetCurrent();
        if (node)
        {
            return node->data.comment;
        }
        return 0;
    }
    /**
    * @brief 現在のラベルを取得します。主に描画時に使います。
    * @param [out] label ラベルを受け取るバッファー
    * @param [int] label ラベルを受け取るバッファーの配列の数
    * @return ラベルの数
    */
    bit32_t GetLabel(label_t label[], bit32_t label_buffer_count) const;
    /**
    * @brief 現在のブランチ情報を取得します。　
    * @param [out] branch ブランチを受け取るバッファー
    * @param [int] branches_buffer_count ブランチを受け取るバッファーの配列の数
    * @return ブランチの数
    */
    bit32_t GetBranch(stone_t branch[], bit32_t branches_buffer_count) const;
    /**
    * @param 現在までに取られた石の数を取得します。
    */
	void GetDeadStone(bit32_t& black, bit32_t& white) const;

/** +++++++++++++++ about edit +++++++++++++++ **/
    /**
    * @brief ヘッダー情報を取得します。
    */
    inline const sgf_header_t& GetHeader() const
    {
        return m_sgf.m_header;
    }
    /**
    * @brief ヘッダーの内容を更新します。
    */
    inline void UpdateHeader(const sgf_header_t& header)
    {
        m_sgf.m_header=header;
    }
    /**
    * @brief sgfオブジェクトを取得します。
    * @param このオブジェクトからsgf形式のデータを出力したり、データを設定したりします。
    */
    inline sgf& GetSgf()
    {
        return m_sgf;
    }
    inline bit32_t GetGobanSize()
    {
        return m_sgf.GetGobanSize();
    }
/** +++++++++++++++ for operation +++++++++++++++ **/
    /**
    * @brief 現在のモードを設定します。
    * @param mode @ref engine::state_t
    */
    inline void SetState(engine::state_t state)
    {
        m_state=state;
    }
    /**
    * @brief 現在のモードを取得します。　
    * @return mode @ref engine::state_t
    */
    inline engine::state_t GetState() const
    {
        return m_state;
    }
    /**
    * @brief 地合計算モードを開始します。\n
    * 地合計算モードになると、AddStone時に石は追加されません。\n
    * 逆に石のない所をタップするとエラーになります。\n
    * 試合計算モードに入るとGetStoneで入るstone_tのselectedフラグが1になっている場合があります。\n
    * その場合は、その石はあげ石として指定されていると判定すべきで、×表記とか透明度を返るなどの処理を行うべきです。
    */
    const stone_t (*StartCalculate())[kMaxBoardSize];
    /**
    * @brief アゲハマの設定が終わったら、地合計算モードを終了します。\n
    * 返される石の配列は石をそのまま表示してはいけません。\n
    * セキやダメ以外の場合、必ず白か黒の属性を持ちます。\n
    * ただし、石のない所には必ずstone_tのconfirmedフラグが1になっているので、\n
    * そういう場合は、どっちかの地であるのか分かるように描画すべきです。\n
    * もし、対局終了後（黒と白が連続でパスをした段階）も、次の手順がある場合は、あげいしの指定ですので、\n
    * 再生時に対局終了後の次のノードにプレイができる場合、内部的にはEndCalculate状態になります。\n
    * この状態では石を追加したり、することができません。（ただし、コメントを追加することは可能です。)
    *
    * @param [out] result 結果が返ってきます。\n
    * 注意すべきはこの結果はsgfにはまだ反映されていません。\n
    * 反映するには、GetSgfHeaderからヘッダーと取ってきて、結果を設定し、UpdateHeaderを呼び出してください。
    */
	const stone_t (*EndCalculate(result_t& result))[kMaxBoardSize]; 

/** +++++++++++++++ other +++++++++++++++ **/
    /**
    * @brief 何か追加データを設定します。
    *
    * @param p データ
    * @param deallocator ゼロでない場合は、engineの削除時にdeallocatorを呼び出します。\n
    * ゼロの場合は何も行いません。
    */
    inline void SetExtraData(void* p, void (*deallocator)(void*)=0)
    {
        m_extra.data=p;
        m_extra.deallocator=deallocator;
    }
    /**
    * @brief 追加データを取得します。
    */
    inline void* GetExtraData() const
    {
        return m_extra.data;
    }
    /**
    * @brief 碁盤のサイズを取得します。
    * @return 碁盤のサイズ
    */
    inline const bit32_t GetGobanSize() const
    {
        return m_goban_size;
    }
#ifdef GO_ENGINE_DEBUG 
    void __print_text_goban__();
    void __show_block__();
#endif
private:
/** 内部計算ブロックの最大の数 **/
#define MAX_BLOCK_ID_COUNT  ((kMaxBoardSize * kMaxBoardSize )/2)

    mutable sgf     m_sgf;
    mutable stone_t m_goban[kMaxBoardSize][kMaxBoardSize];
    engine::state_t m_state;
    engine::mode_t m_mode;
    bit32_t m_goban_size;
    ItxMemPage<4096> m_mem_page;

    typedef struct {
        unsigned type:2; /** stone::type_t */
        unsigned is_pass:1; 
        unsigned block_id:8; /** 255まで **/
        unsigned order:9;
        unsigned area_flag:2;   /** 0:未確定 1:黒 2:白 **/
        unsigned area_id:7;     /** 127まで **/
        unsigned reserved:3;
    } __stone_t__;
    mutable struct {
        stone_t kou;
        bool_t is_kou_active;
        __stone_t__ goban[kMaxBoardSize+2][kMaxBoardSize+2];
        __stone_t__ block_goban[kMaxBoardSize+2][kMaxBoardSize+2];
        bit32_t last_index;
        bit32_t last_area_id;
        ubit8_t block_idz[MAX_BLOCK_ID_COUNT];
        ubit8_t block_count;
    } m_operation;
    struct {
        bit16_t area;
        bit16_t catched_stone;
    } m_black, m_white;
    struct {
        void* data;
        void (*deallocator)(void*);
    } m_extra;

private:

    /** for base operation **/    
    stone_t is_atari(int i, int j);
    bool_t  is_block_surrounded(ubit8_t block_id);
    bit32_t check_catched_stone(bit32_t i, bit32_t j, const stone::type_t type, bit32_t block_id/*, aStoneList list*/);
    bit32_t remove_block(stone::type_t type, bit32_t block_id);
    bit32_t get_block_count(bit16_t id);
    int get_catched_stone(bool_t is_stone_white);
    bit16_t generate_block_id();
    int replace_block_id(bit16_t before_id, bit16_t after_id);
    bit32_t create_block(int i, int j);
    bool_t add_stone(stone_t stone, bool_t is_added);
    bit32_t add_stone_node(stone_node_t* stone);
    bit32_t add_stone_node(stone_node_t* stone) const;
    void remove_stone(bit32_t x, bit32_t y);
    void export_goban();
    void init_goban();
    void init_goban() const;
    stone::type_t get_next_stone_type();
    

    /** for calculate **/
    void init_area(bit32_t goban_size);
    bit32_t update_area(bit32_t goban_size);
    void  change_area_map(const bit32_t before, const bit32_t after, const bit32_t goban_size);
    void integrate_area_id(const bit32_t max_area_id, const bit32_t goban_size);
    void calculate_confirm(const bit32_t max_area_id, const bit32_t goban_size);
    void start_calculate();
    bool_t restore_stone(stone_t stone, bit32_t goban_size);
    bit32_t get_surrounded_area(stone_t stone, size_t goban_t);
    int integrate_area(bit32_t area_id, stone::type_t type, const bit32_t goban_size);
    void update_calculated_goban(const size_t goban_size);
    void set_dead_stone(stone_t stone);


    void get_current(bit32_t i, bit32_t j, const stone_t* dest, const __stone_t__ stone) const;

    static const igx_mem_page_option_t m_option;
};

}
#endif
