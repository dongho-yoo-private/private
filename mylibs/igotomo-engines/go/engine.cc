/**
* Copyright (c) 2015 igotomo
*
* Authorized by dongho.yoo.
*/
#include "engine.h"
#include "debug/igx_debug.h"


#ifdef GO_ENGINE_DEBUG 
#   include <stdio.h>
#endif

namespace go {

const igx_mem_page_option_t engine::m_option = {true, false, 8};

engine::engine(mode_t mode/*=kModeEdit*/)
: m_mem_page(&m_option), m_sgf(&m_mem_page), m_mode(mode), m_goban_size(19)
{
    Clear();
    m_mode=mode;
}
engine::~engine()
{
    Terminate();
}
bool_t engine::Initialize(const sgf_header_t& header)
{
    m_sgf.UpdateHeader(header);
    m_sgf.MoveTop();
    m_goban_size = m_sgf.GetGobanSize();
    return true;
}
bool_t engine::Initialize(const char* file_name)
{
    bool_t ret = m_sgf.Import(file_name);
    if (ret==false)
    {
        return false;
    }

    m_sgf.MoveTop();
    m_goban_size = m_sgf.GetGobanSize();
    init_goban();
    return true;
}
bool_t engine::Initialize(const void* sgf_data, size_t data_size/*=0*/)
{
    bool_t ret = m_sgf.Import((const char*)sgf_data, data_size);
    if (ret==false)
    {
        return false;
    }

    m_sgf.MoveTop();
    m_goban_size = m_sgf.GetGobanSize();
    init_goban();

    return true;

}
void engine::Terminate()
{
    Clear();
}
void engine::Clear()
{
    m_sgf.Terminate();

    memset(m_goban, 0, sizeof(m_goban));
    memset(&m_operation, 0, sizeof(m_operation));
    memset(&m_black, 0, sizeof(m_black));
    memset(&m_white, 0, sizeof(m_white));
    memset(&m_extra, 0, sizeof(m_extra));

    for (bit32_t i=0; i<kMaxBoardSize+2; ++i)
    {
        m_operation.goban[0][i].type=stone::kInvalid;
        m_operation.goban[kMaxBoardSize+1][i].type=stone::kInvalid;
        m_operation.goban[i][0].type=stone::kInvalid;
        m_operation.goban[i][kMaxBoardSize+1].type=stone::kInvalid;
    }
    m_mem_page.Clear();
}
stone_t engine::AddStone(int x, int y, bool_t is_pass/*=false*/) const
{
    return ((engine*)this)->AddStone(x, y, is_pass);
}
stone_t engine::AddStone(int x, int y, bool_t is_pass/*=false*/)
{
    /** 地合計算も終わった場合は、基本的に石を追加させない。
    * 終局後検討したければ、パスの前に戻してブランチを作成すべき。
    **/
    if (m_state==engine::kStateEndCalculate)
    {
        return kSTONE_ERROR;
    }

    stone_t stone;

    stone.stone=0;
    stone.x=x;
    stone.y=y;
    stone.type  = get_next_stone_type();
    stone.order = m_sgf.GetCurrentOrder() + 1;

    /** 計算モード時 **/
    if (m_state==engine::kStateStartCalculate)
    {
        const stone::type_t stone_type = (const stone::type_t)m_operation.goban[x+1][y+1].type;
        if (stone_type==stone::kBlack||
                stone_type==stone::kWhite)
        {
		    set_dead_stone(stone);
            return (stone_t)kNULL_STONE;
        }
        IGX_ASSERT(false);
        return (stone_t)kSTONE_ERROR;
    }
    else if (m_mode==engine::kModePlay)
    {
        /** 再生時にはブランチが作れないようにする **/
        stone_node_t* node = m_sgf.GetCurrent();
        if (node->next!=0)
        {
            if (node->next->data.stone!=stone)
            {
                return (stone_t)kSTONE_ERROR;
            }
        }
    }

    if (add_stone(stone, false)==true)
    {
        m_sgf.AddStone(stone);
        return stone;
    }
    return kSTONE_ERROR;
}
bit32_t engine::AddInitStone(int x, int y, stone::type_t type) const
{
    return ((engine*)this)->AddInitStone(x, y, type);
}
bit32_t engine::AddInitStone(int x, int y, stone::type_t type)
{
    stone_t stone;
    stone.stone=0;
    stone.x=x;
    stone.y=y;
    stone.type = type;

    if (add_stone(stone, true)==true)
    {
        return m_sgf.AddInitStone(stone);
    }
    return -1;
}
bit32_t engine::AddLabel(int x, int y, label::type_t type, char ch)
{
    label_t label;
    label.label=0;
    label.type = type;
    label.ch = ch;
    label.x = x;
    label.y = y;
    return m_sgf.AddLabel(label);
}
bit32_t engine::AddComment(const char* comment)
{
    size_t len = strlen(comment);
    m_sgf.AddComment(comment, len);
    return (bit32_t)len;
}
void engine::get_current(bit32_t i, bit32_t j, const stone_t* dest, const __stone_t__ stone) const
{
    stone_t* p = (stone_t*)dest;

    p->x=i;
    p->y=j;
    p->type  = stone.type;
    p->order = stone.order;
    p->selected = (stone.area_id!=0);
    p->area_flag = stone.area_flag;
}
const stone_t (*engine::GetCurrent() const)[kMaxBoardSize] 
{
    for (int i=0; i<kMaxBoardSize; ++i)
    {
        for (int j=0; j<kMaxBoardSize; ++j)
        {
            const __stone_t__ stone = m_operation.goban[i+1][j+1];
            get_current(j, i, &m_goban[j][i], stone);
        }
    }
    return m_goban;
}
const stone_t (*engine::MoveTop() const)[kMaxBoardSize] 
{
    m_sgf.MoveTop();
    init_goban();

    ItxLinkedList<stone_t>* list = m_sgf.m_top.data.initial_stone_list;
    if (list!=0)
    {
        for (ItxNode<stone_t>* node=list->GetTopFrom(); node; node=node->next)
        {
            const stone_t stone = **node;
            AddInitStone(stone.x, stone.y, (stone::type_t)stone.type);
        }
    }
    return GetCurrent();
}
const stone_t (*engine::MoveEnd() const)[kMaxBoardSize]
{
    while(stone_node_t* node = m_sgf.MoveNext())
    {
        if (node->next==0)
        {
            /** nextが0の場合は、最後か次にブランチがあるかどちらかなので、ここで止める **/
            break;
        }
        bool_t result = add_stone_node(node);
        if (result==false)
        {
            IGX_WARN("add_stone_node failure! (%d, %d)", (bit32_t)node->data.stone.x, (bit32_t)node->data.stone.y);
        }
    }
    return GetCurrent();
}
const stone_t (*engine::MoveFromCurrent(int offset) const)[kMaxBoardSize]
{
    if (offset==0)
    {
        return GetCurrent();
    }

    /* 数手先へ */
    if (offset>0)
    {
        for (int i=0; i<offset; ++i)
        {
            add_stone_node(m_sgf.MoveNext());
        }
        return GetCurrent();
    }

    /* 数手前へ */
    offset=-offset;
    ItxStackBuffer<stone_t, 361> history_stack;
    for (int i=0; i<offset; ++i, m_sgf.MovePrev());

    /* まずはトップまで移動 */
    for (stone_node_t* node = m_sgf.GetCurrent(); (node->prev!=0 || node->parent!=0); node=m_sgf.MovePrev())
    {
        history_stack.Push(node->data.stone);
    }

    /* Clear */
    init_goban();
    stone_t stone;
    while(history_stack.Pop(stone)>=0)
    {
        if (stone.type==stone::kNone)
        {
            /* top */
            m_sgf.MoveNext();
            continue;
        }
        AddStone(stone.x, stone.y, stone.is_pass);
    }

    return GetCurrent();
}
const stone_t (*engine::MoveNextBranch() const)[kMaxBoardSize] 
{
    while(m_sgf.IsEnd()==false)
    {
        stone_node_t* node = m_sgf.MoveNext();
        add_stone_node(node);
        if (node->branches!=0)
        {
            return GetCurrent();
        }
    }
    return 0;
}
const stone_t (*engine::MovePrevBranch() const)[kMaxBoardSize] 
{
    /** TODO. **/
    return 0;
}
const stone_t (*engine::MoveNextComment() const)[kMaxBoardSize]
{
    while(m_sgf.IsEnd()==false)
    {
        stone_node_t* node = m_sgf.MoveNext();
        add_stone_node(node);
        if (node->data.comment!=0&&*node->data.comment!=0)
        {
            return GetCurrent();
        }
    }
    return 0;

}
const stone_t (*engine::MovePrevComment() const)[kMaxBoardSize] 
{
    /** TODO. **/
    return 0;
}
bit32_t engine::GetLabel(label_t label[], bit32_t label_buffer_count) const
{
    stone_node_t* node_curr = m_sgf.GetCurrent();
    bit32_t cnt=0, ret=0;
    if (node_curr!=0)
    {
        if (node_curr->data.label!=0)
        {
            ItxNode<label_t>* node = node_curr->data.label->GetTopFrom();
            ret = (bit32_t)node_curr->data.label->Count();
            for (;node;node=node->next, ++cnt)
            {
                if (cnt==label_buffer_count-1)
                {
                    IGX_WARN("label count over %d", cnt);
                    break;
                }
                label[cnt]=**node;
            }
        }
    }
    return ret;
}
bit32_t engine::GetBranch(stone_t branch[], bit32_t branches_buffer_count) const
{
    stone_node_t* node_curr = m_sgf.GetCurrent();
    bit32_t cnt=0, ret=0;

    if (node_curr!=0)
    {
        if (node_curr->branches!=0)
        {
            ItxNode<stone_node_t*>* node = node_curr->branches->GetTopFrom();
            ret = node_curr->branches->Count();
            for (;node;node=node->next, ++cnt)
            {
                if (cnt==branches_buffer_count-1)
                {
                    IGX_WARN("branches count over %d", cnt);
                    break;
                }
                stone_node_t* stone = **node;
                branch[cnt]=stone->data.stone;
            }
        }
    }
    return ret;
}
void engine::GetDeadStone(bit32_t& black, bit32_t& white) const
{
    black = m_white.catched_stone;
    white = m_black.catched_stone;
}
const stone_t (*engine::StartCalculate())[kMaxBoardSize] 
{
    SetState(kStateStartCalculate);
    start_calculate();
    return GetCurrent();
}
const stone_t (*engine::EndCalculate(result_t& result))[kMaxBoardSize]
{
    m_state = kStateEndCalculate;
    return GetCurrent();
}

#ifdef GO_ENGINE_DEBUG 
/** デバッグ用**/
void engine::__print_text_goban__()
{
    const stone_t (*stones)[19] = GetCurrent();
    printf("                     l l l l l l l l l 1\n");
    printf("   1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9\n");
    for (int i=0; i<m_goban_size; ++i)
    {
        printf("%2d", i+1);
        for (int j=0; j<m_goban_size; ++j)
        {
            char ch = (stones[i][j].type==stone::kBlack)?'B':'W';
            ch = (stones[i][j].type==stone::kNone)?'+':ch;
            putchar(ch);
            putchar(ch);
        }
        putchar('\n');
    }
}
/** デバッグ用**/
void engine::__show_block__()
{
    printf("                     l l l l l l l l l 1\n");
    printf("   1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9\n");
    for (int i=1; i<m_goban_size+1; ++i)
    {
        printf("%2d", i+1);
        for (int j=1; j<m_goban_size+1; ++j)
        {
            const bit32_t block_id = (ubit32_t)m_operation.goban[i][j].block_id;
            if (block_id==0)
            {
                putchar(' '),putchar(' ');
                continue;
            }
            printf("%2x", (block_id&0xFF));
        }
        putchar('\n');
    }
}

#endif


} /* end of namespace go */
