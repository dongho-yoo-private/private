/**
* Copyright (c) 2015 igotomo
*
* Authorized by dongho.yoo.
*/
#include "engine.h"
#include "debug/igx_debug.h"

namespace go {

namespace block {
    static const bit32_t kTemporaryId   = 0xFF;
    static const bit32_t kDeleteId      = 0xFE;
    static const bit32_t kInvalidId     = 0xFC;
    static const bit32_t kNotConfirmed  = 0;
}

#define ARRAY_LEFT(i, j) m_operation.goban[i-1][j]
#define ARRAY_RIGHT(i, j) m_operation.goban[i+1][j]
#define ARRAY_UP(i, j) m_operation.goban[i][j-1]
#define ARRAY_DOWN(i, j) m_operation.goban[i][j+1]

/** コウ判定様のメソッド **/
stone_t engine::is_atari(int i, int j)
{
    stone::type_t stone = (stone::type_t)m_operation.goban[i][j].type;
    stone::type_t reverse_stone = REVERSE_STONE(stone);
    bit32_t surround_cnt=0;
    bit32_t none_cnt=0; 
    stone_t ret = {0, };

    /** 周りの石に一カ所だけ空きがあるかどうかをチェックします。
        空きのあるところがコウになります。 **/
    if (ARRAY_RIGHT(i,j).type==stone::kNone)
    {
        none_cnt++;
        ret.type=stone::kNone;
        ret.x=i+1;
        ret.y=j;
    }
    if (ARRAY_DOWN(i, j).type==stone::kNone)
    {
        none_cnt++;
        ret.type=stone::kNone;
        ret.x=i;
        ret.y=j+1;
    }
     if (ARRAY_LEFT(i, j).type==stone::kNone)
    {
        none_cnt++;
        ret.type=stone::kNone;
        ret.x=i-1;
        ret.y=j;
    }
    if (ARRAY_UP(i, j).type==stone::kNone)
    {
        none_cnt++;
        ret.type=stone::kNone;
        ret.x=i;
        ret.y=j-1;
    }
    if (none_cnt!=1)
    {
        return kSTONE_ERROR;
    }

    /** ここまでの流れでは、ダメアキが一個の場合で、
        次に相手の石や壁に囲まれてるかどうかのチェック **/
    if (m_operation.goban[i+1][j].type==reverse_stone||
            m_operation.goban[i+1][j].type==stone::kInvalid)
    {
        surround_cnt++;
    }

    if (m_operation.goban[i][j-1].type==reverse_stone||
            m_operation.goban[i][j-1].type==stone::kInvalid)
    {
        surround_cnt++;
    }

    if (m_operation.goban[i-1][j].type==reverse_stone ||
            m_operation.goban[i-1][j].type==stone::kInvalid)
    {
        surround_cnt++;
    }

    if (m_operation.goban[i][j+1].type==reverse_stone ||
        m_operation.goban[i][j+1].type==stone::kInvalid)
    {
        surround_cnt++;
    }

    if (surround_cnt==3)
    {
        return ret;
    }

    return kSTONE_ERROR;
}
bit32_t engine::get_block_count(bit16_t id)
{
    int cnt=0;

    if (id==-1)
        return 0;

    for (int i=0; i<m_goban_size+1; ++i)
    {
        for (int j=0; j<m_goban_size+1; ++j)
        {
            if (m_operation.goban[i][j].block_id==id) ++cnt;
        }
    }
    return cnt;
}
int engine::get_catched_stone(bool_t is_stone_white)
{
    return is_stone_white? m_white.catched_stone:m_black.catched_stone;
}
bit16_t engine::generate_block_id()
{
    const bit32_t count = array_sizeof(m_operation.block_idz);
    /** block id 1から使う **/
    for (int i=1; i < count; ++i)
    {
        if (m_operation.block_idz[i] == false)
        {
            m_operation.block_idz[i] = true;
            return (short)i;
        }
    }
    /** あり得ない **/
    return -1;
}
int engine::replace_block_id(bit16_t before_id, bit16_t after_id)
{
    bool_t is_replaced = false;
    int cnt = 0;

    for (int i = 1; i < m_goban_size+1; ++i)
    {
        for (int j = 1; j < m_goban_size+1; ++j)
        {
            if (m_operation.goban[i][j].block_id == before_id)
            {
                m_operation.goban[i][j].block_id  = after_id;

                if (before_id<array_sizeof(m_operation.block_idz))
                {
                    m_operation.block_idz[before_id]=false;
                }
                is_replaced = true;
                cnt++;
            }
        }
    }
    return after_id;//cnt;
}
bit32_t engine::create_block(int i, int j)
{
    __stone_t__ stone = m_operation.goban[i][j];
    if (stone.type == stone::kNone)
    {
        /** 碁石がない場合 */
        IGX_ERROR("??????");
        return 0;
    }
    if (stone.type==ARRAY_LEFT(i,j).type)
    {
        replace_block_id(ARRAY_LEFT(i,j).block_id, block::kTemporaryId);
        m_operation.goban[i][j].block_id=block::kTemporaryId;
    }
    if (stone.type==ARRAY_RIGHT(i,j).type)
    {
        replace_block_id(ARRAY_RIGHT(i,j).block_id, block::kTemporaryId);
        m_operation.goban[i][j].block_id=block::kTemporaryId;
    }
    if (stone.type==ARRAY_UP(i,j).type)
    {
        replace_block_id(ARRAY_UP(i,j).block_id, block::kTemporaryId);
        m_operation.goban[i][j].block_id=block::kTemporaryId;
    }
    if (stone.type==ARRAY_DOWN(i,j).type)
    {
        replace_block_id(ARRAY_DOWN(i,j).block_id, block::kTemporaryId);
        m_operation.goban[i][j].block_id=block::kTemporaryId;
    }

    if (m_operation.goban[i][j].block_id==block::kTemporaryId)
    {
        return replace_block_id(block::kTemporaryId, generate_block_id());
    }

    // independent block
    return m_operation.goban[i][j].block_id = generate_block_id();
}
bool_t engine::add_stone(stone_t stone, bool_t is_added)
{
    const bit32_t i = stone.x;
    const bit32_t j = stone.y;
    const bit32_t _i=i+1, _j=j+1;

    if (m_operation.is_kou_active==true&& m_operation.kou.x==_i && m_operation.kou.y==_j)
    {
        /** 着手禁止 コウの場合 **/
        IGX_WARN("it is kou !");
        return false;
    }

    m_operation.is_kou_active=false;

    if (stone.is_pass==true)
    {
        IGX_INFO("it is PASS!");
        return true;
    }
    if (m_operation.goban[_i][_j].type != stone::kNone)
    {
        IGX_WARN("it is not empty !");
        return false;
    }

    m_operation.goban[_i][_j].type = stone.type;

    const bit16_t new_block_id = create_block(_i, _j);
    IGX_INFO("new block id is %d\n", (bit32_t)new_block_id);
    bit32_t catched_stone = check_catched_stone(_i, _j, (const stone::type_t)stone.type,  new_block_id);
    /** 
    *   ブロックカウントが１個しか残ってない場合、
    *   ブロックIDを回収する。
    */
    if (catched_stone == 0)
    {
        /** 取れた石がないのに、囲まれる場合は着手禁止 **/
        if (is_block_surrounded(new_block_id)==true)
        {
            m_operation.goban[_i][_j].type           = stone::kNone;
            m_operation.goban[_i][_j].block_id       =  block::kNotConfirmed;

            if (get_block_count(new_block_id)==0)
            {
                m_operation.block_idz[new_block_id] = false;
            }
            IGX_WARN("it is dead !!");
            return false;
        }
    }

    /** コウチェック **/
    if (catched_stone==1)
    {
        m_operation.kou = is_atari(_i, _j);

        if (m_operation.kou!=kSTONE_ERROR)
        {
            m_operation.is_kou_active=true;
            IGX_INFO("kou is activated!! (%d,%d)", m_operation.kou.x, m_operation.kou.y);
        }
    }

/** いらないよね？ **/
#if 0
    if (stonelist)
    {
        stonelist[catched_stone].type = stone::kInvalid;
    }
#endif

    if (is_added==false)
        m_operation.goban[_i][_j].order = ++m_operation.last_index;
    else
        m_operation.goban[_i][_j].order = 0;

    if (stone.type==stone::kBlack)
    {
        m_black.catched_stone+=catched_stone;
    }
    else
    {
        m_white.catched_stone+=catched_stone;
    }

    return true;

}
bit32_t engine::add_stone_node(stone_node_t* stone) const
{
    return ((engine*)this)->add_stone_node(stone);
}
bit32_t engine::add_stone_node(stone_node_t* stone)
{
    ItxNode<stone_t>* node = stone->data.initial_stone_list?stone->data.initial_stone_list->GetTopFrom():0;
    for (;node;node=node->next)
    {
        const stone_t stone = **node;
        add_stone(stone, true);
    }
    return add_stone(stone->data.stone, false);
}
void engine::remove_stone(bit32_t x, bit32_t y)
{
    m_operation.goban[x+1][y+1].type=stone::kNone;
}
void engine::export_goban()
{
#define __CONVERT_GOBAN__(a) m_goban[i][j].a = m_operation.goban[i+1][j+1].a
    const bit32_t size = GetGobanSize();
    memset(m_goban, 0, sizeof(m_goban));
    for (bit32_t i=0; i<size; ++i)
    {
        for (bit32_t j=0; j<size; ++j)
        {
            m_goban[i][j].x = i;
            m_goban[i][j].y = j;
            __CONVERT_GOBAN__(type); 
            __CONVERT_GOBAN__(is_pass); 
            __CONVERT_GOBAN__(order); 

            if (m_state==kStateStartCalculate)
            {
                if (m_goban[i][j].type!=stone::kNone &&
                            m_operation.goban[i+1][j+1].area_id!=0)
                {
                    m_goban[i][j].selected=1;
                }
            }
            else if (m_state==kStateEndCalculate)
            {
                if (m_operation.goban[i+1][j+1].area_flag!=stone::kNone)
                {
                    m_goban[i][j].area_flag=m_operation.goban[i+1][j+1].area_flag;
                }
            }
        } /** for j **/
    } /** for i **/

}
void engine::init_goban()
{
    memset(&m_operation, 0, sizeof(m_operation));
    memset(&m_black, 0, sizeof(m_black));
    memset(&m_white, 0, sizeof(m_white));

    ItxLinkedList<stone_t>* list = m_sgf.m_top.data.initial_stone_list;
    if (list!=0&&list->Count()!=0)
    {
        for (ItxNode<stone_t>*node=list->GetTopFrom(); node; node=node->next)
        {
            add_stone(**node, true);
        }
    }
            
}
void engine::init_goban() const
{
    ((engine*)this)->init_goban();
}
bit32_t engine::remove_block(stone::type_t type, bit32_t block_id)
{
    bit32_t cnt = 0;

    for (int i = 1; i < m_goban_size+1; ++i)
    {
        for (int j = 1; j < m_goban_size+1; ++j)
        {
            if (m_operation.goban[i][j].block_id==block_id)
            {
/** 今のところは使わない **/
#if 0
				if (list)
					list[cnt]=MAKESTONE(eST_NONE, m_mapData.extra[i][j]&0xFFFF, i, j);
#endif
                m_operation.goban[i][j].type = stone::kNone;
                m_operation.goban[i][j].block_id = block::kNotConfirmed;
                m_operation.block_idz[block_id]  = false;

                ++cnt;
            }
        }
    }

/** 今のところは使わない **/
#if 0
	if (list)
		list[cnt]=STONE_EOF;
#endif
    return cnt;
}
bit32_t engine::check_catched_stone(bit32_t i, bit32_t j, const stone::type_t type, bit32_t block_id/*, aStoneList list*/)
{
    /**
    * i,jに置いた石によって相手の石が取れたかどうかをチェックします。
    * なので、i,jの周りの相手の石が囲まれたかどうかをチェックし、囲まれていたらそのブロックを取り除きます。
    **/
    bit32_t age_stone = 0;
    bit32_t block;

    /** pon nuki check. **/
    block = ARRAY_RIGHT(i,j).block_id;
    if (block!=block_id&& is_block_surrounded(block)==true)
    {
        age_stone+=remove_block(type, block);
    }
    block = ARRAY_LEFT(i,j).block_id;
    if (block!=block_id&& is_block_surrounded(block)==true)
    {
        age_stone+=remove_block(type, block);
    }
    block = ARRAY_UP(i,j).block_id;
    if (block!=block_id&& is_block_surrounded(block)==true)
    {
        age_stone+=remove_block(type, block);
    }
    block = ARRAY_DOWN(i,j).block_id;
    if (block!=block_id&& is_block_surrounded(block)==true)
    {
        age_stone+=remove_block(type, block);
    }

    return age_stone;
}
bool_t engine::is_block_surrounded(ubit8_t block_id)
{
	if (block_id==0)
    {
		return false;
    }

    for (int i = 1; i < m_goban_size+1; i++)
    {
        for (int j = 1; j < m_goban_size+1; j++)
        {
            if (m_operation.goban[i][j].block_id==block_id)
            {
                if (ARRAY_RIGHT(i,j).type==stone::kNone)
                {
                    return false;
                }
                if (ARRAY_UP(i,j).type==stone::kNone)
                {
                    return false;
                }
                if (ARRAY_LEFT(i,j).type==stone::kNone)
                {
                    return false;
                }
                if (ARRAY_DOWN(i,j).type==stone::kNone)
                {
                    return false;
                }
           } /** if **/
        } /** for j **/
    } /** for i **/

    return true;
}
stone::type_t engine::get_next_stone_type()
{
    const stone_node_t* curr = m_sgf.GetCurrent();


    /** 初手の場合 **/
    if (curr==0 ||
            (curr->prev==0&&curr->parent==0))
    {
        const bit32_t handycap = m_sgf.m_header.event.rules.handycap;
        if (handycap>1)
        {
            return stone::kWhite;
        }
        return stone::kBlack;
    }

    stone::type_t type = (stone::type_t)curr->data.stone.type;

    /** ブランチがある場合 **/
    if (curr->branches!=0 &&
                    type==stone::kNone)
    {
        IGX_ASSERT(curr->prev);
        type = (stone::type_t)curr->prev->data.stone.type;
    }
   
    return REVERSE_STONE(type);
}


} /* end of namespace go */
