/**
* Copyright (c) 2015 
*
* authorized dongho.yoo.
* @brief sgf 
* @ref http://www.geocities.jp/nakamiya_town/SGF.html
*/
#ifndef __SGF_H__
#define __SGF_H__

#include "go_types.h"
#include "library/igx_mem_page.h"
#include "data_struct/itx_linked_list.h"

namespace go {

#define PRINT_TO_DIRECT 1
#define ITX_SGF_INITIAL_NODE_SIZE (32)

/**64ビットで役50ノード分、*/
#define DEFAULT_SGF_STONE_BUFFER_SIZE (4096)
#define DEFAULT_SGF_STRING_BUFFER_SIZE (1024)


/**
* @brief 囲碁のデータ構造を保持しているクラス。
*/
class sgf {
friend class engine;
public:
    /**
    * @brief コンストラクター
    */
    sgf();
    /**
    * @brief コンストラクター
    */
    sgf(IgxMemPage* mempage);
    /**
    * @brief ですトラクター　
    */
    virtual ~sgf();
    /**
    * @brief 初期化します。
    *
    * @param data sgf形式のデータ
    * @param size データサイズ
    * @param deallocator 削除するdeallocator NULLの場合は、削除しない。
    * @return true成功、false失敗
    */
    bool_t Initialize(void* data, size_t size, void (*deallocator)(void*)=0);
    /**
    * @brief 初期化します。
    * @param filename sgf形式のファイル名
    *
    * @return true成功、false失敗
    */
    bool_t Initialize(const char* filename);
    /**
    * @brief 初期化します。
    * @param header ヘッダー
    * @return true成功、false失敗
    */
    bool_t Initialize(const sgf_header_t& header);
    /**
    * @brief 終了処理を行います。
    */
    void Terminate();
    /**
    * @brief sgf形式のデータを出力します。
    *
    * @param buffer バッファー
    * @param buffer_size バッファーサイズ
    * @param exported_size 出力サイズ
    * @param application_name アプリケーション名
    * @param version アプリケーションバージョン
    *
    * @return 成功した場合、sgf形式のテキストの先頭のアドレスを返します。
    */
    char* Export(char* buffer, size_t buffer_size, size_t* exported_size, const char* application_name=0, const char* version=0) const;
    /**
    * @brief sgf形式のデータをファイルに出力します。
    * @param filename 出力するファイル名
    * @param application_name アプリケーション名
    * @param version アプリケーションバージョン
    *
    * @return true成功、false失敗
    */
    bool_t ExportFile(const char* filename, const char* application_name=0, const char* version=0) const;
    /**
    * @brief ヘッダーを取得します。
    * 
    * @return ヘッダー
    */
    inline const sgf_header_t& GetHeader()
    {
        return m_header;
    }
    /**
    * @brief 指定したバッファーにヘッダーを作成します。
    *
    * @param buffer バッファー(バッファーがNULLの場合は内部的にヒープ上にメモリを確保します。\n
    * 指定したバッファーと戻り値のバッファーの値が違う場合は、sgf::DiposeBuffer()を呼び出してください。
    * @param buffer_size バッファーサイズ バッファーサイズを超える場合はヒープ上にメモリを確保します。
    *
    * @return ヘッダー
    */
    static const sgf_header_t* MakeSgfHeader(char* buffer, size_t buffer_size, const sgf_header_t* header);
    /**
    * @brief ヘッダーを取得します。
    * 
    * @return ヘッダー
    */
    inline void UpdateHeader(const sgf_header_t& header)
    {
        set_header(&header);
    }
    inline bit32_t GetGobanSize() const
    {
        const bit32_t index = m_header.event.rules.size;
        const bit32_t size[] = {19, 13, 9};
        return size[index];
    }
    inline bit32_t GetCurrentOrder() const
    {
        return m_curr->data.stone.order;
    }

private:
    sgf_header_t  m_header;
    ItxMemPage<1024>    m_mem_string_page;
    IgxMemPage    *m_mem_stone_page;
    void*         m_expaned_allocate_memory;
    size_t        m_expected_length;

    /** @brief ノードトップ*/
    stone_node_t m_top;
    stone_node_t* m_curr;
    char*        m_converted_string; // -> これなんだっけ？
    /** 廃止した方がいいかな。。。**/
    struct {
        char* top;
        char* body;
    } m_data;
    /* 使わなくて良し。
    struct stone_page {
        struct stone_page* next;
        stone_node_t nodes[1];
    }* m_node_pool;
    struct {
        size_t total_comment_size;
        size_t node_count;
        size_t add_stone_count;
    } m_memory;*/

    /** 
    * @brief ヘッダーをパースします。
    * @param sgf sgfフォーマットのテキストデータ
    * @return 成功したらボディー部の開始アドレスを返します。失敗時は0
    */
    const char* parse_header(const char* sgf, size_t sgf_len);
    /** イベントをパース **/
    void parse_event(const char* sgf);
    /** ルールをパース **/
    void parse_event_rules(const char* sgf);
    /** 結果をパース **/
    void parse_result(const char* sgf);
    /** 対局者をパース **/
    void parse_player(const char* sgf);
    /** ボディー部をパース*/
    const char* parse_body(const char* sgf, stone_node_t* before, ubit32_t order);
    /** ブランチをパース **/
    const char* parse_branch(const char* sgf, stone_node_t* node, ubit32_t order);
    /** タグを割り振る */
    char* dispatch_tag(char* p, stone_node_t* node);
    /** parse B[], W[] */
    char* parse_stone(const char* _p, stone_node_t* node);
    /** parse C[] */
    char* parse_comment(const char* _p, stone_node_t* node);
    /** parse AB[] or WB[] */
    char* parse_init_stone(const char* _p, stone_node_t* node);
    /** parse BL[] or WL[] */
    char* parse_remain_time(const char* _p, stone_node_t* node);
    /** parse LB[],CR[],TR[],MA[],RT[] */
    char* parse_label(const char* _p, stone_node_t* node, label::type_t type);
    /** bodyを探す*/
    static const char* find_body(const char* sgf);
    /** ヘッダーをコピーします。**/
    void set_header(const sgf_header_t* header);
    /** 文字列を文字列のプールにロードします。*/
    char* load_string(const char* str, size_t len);

#if PRINT_TO_DIRECT==1
    /** メモリに出力 **/
    char* export_header(char* buffer,const char* application_name, const char* version) const;
    /** メモリに出力 **/
    char* export_body(char* buffer, const stone_node_t* node) const;
#else
    /** メモリに出力 **/
    bit32_t export_header(char* buffer,const char* application_name, const char* version);
    /** ディスクリプターに出力 **/
    bit32_t export_body(int fd, stone_node_t* node);

#endif

    void* m_sgf;
private:
    /** go_engineで使われるインターフェース*/
    /**
    * @brief 石を追加します。\n
    * 現在のノードが最後でない場合はブランチが作成されます。
    * @return 追加された石の手順
    */
    ubit32_t AddStone(stone_t stone, float time=0.0f);
    /**
    * @brief 現在のノード以下は全て削除されます。
    */
    void RemoveNode();
    /**
    * @brief ラベルを追加します。
    * @return 現在のノードのラベルの数
    */
    ubit32_t AddLabel(label_t label);
    /**
    * @brief コメントを追加します。\n
    * 一個のノードに複数のコメントがある際には、改行扱いで追加されます。
    */
    void AddComment(const char* comment, size_t len=0);
    /**
    * @brief 置き石を追加します。
    *
    * @param stone石の情報
    * @return 現在のノードの置き石の数
    */
    ubit32_t AddInitStone(stone_t stone);
    /**
    * @brief トップへ移動します。
    */
    stone_node_t* MoveTop();
    /**
    * @brief 現在のノードからインデックス分移動します。
    */
    stone_node_t* Move(bit32_t index, bool_t is_ignore_branch=true);
    /**
    * @brief 次のノードへ移動します。
    */
    stone_node_t* MoveNext(stone_t next=kNULL_STONE);
    /**
    * @brief 前のノードへ移動します。
    */
    stone_node_t* MovePrev();
    /**
    * @brief 次のブランチまで移動します。
    */
    stone_node_t* NextBranch();
    /**
    * @brief 現在のノードを取得します。
    */
    inline stone_node_t* GetNode()
    {
        return m_curr;
    }
    /**
    * @brief 置き石を消します。
    */
    bool_t RemoveInitStone(stone_t stone);
    /**
    */
    inline bool_t IsEnd()
    {
        return m_curr->next?false:true;
    }
    /**
    */
    inline bool_t IsTop()
    {
        return (m_curr->prev!=0)?false:true;
    }
    inline void SetCurrentNode(stone_node_t* node)
    {
        m_curr=node;
    }
    inline stone_node_t* GetCurrent() const
    {
        return m_curr;
    }
    /**
    * @brief sgf形式のデータを取り込みます。
    *
    * @param data データ
    * @param size データサイズ
    * @param 削除するdeallocator
    *
    * @return true成功、false失敗
    */
    bool_t Import(const char* data, size_t size, void (*deallocator)(void*)=0);
    /**
    * @brief sgf形式のデータをファイルから取り込みます。
    * @param filename データ
    *
    * @return true成功、false失敗
    */
    bool_t Import(const char* filename);

};

#define STONE_MALLOC(a) pmalloc(&m_mem_stone_page, a)

namespace tag {
extern char* get_stone(const char* p, stone_t& stone, ubit32_t order);
extern char* get_comment(const char* _p, char** comment, IgxMemPage* page);
extern char* get_label(const char* _p, ItxLinkedList<label_t>** label_list, IgxMemPage* page);
extern char* get_time(const char* sgf, float& remain_time);
extern char* get_initstone(const char* _p, ItxLinkedList<stone_t>** init_stone_list, IgxMemPage* page);
}

}


#endif
