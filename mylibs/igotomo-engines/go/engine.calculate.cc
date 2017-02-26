/**
* Copyright (c) 2015 igotomo
*
* Authorized by dongho.yoo.
* @file engine.calculate.cc
* @brief 計算に関係するモジュール
*/
#include "engine.h"

namespace go {

#define ARRAY_LEFT(i, j) m_operation.goban[i-1][j]
#define ARRAY_RIGHT(i, j) m_operation.goban[i+1][j]
#define ARRAY_TOP(i, j) m_operation.goban[i][j-1]
#define ARRAY_BOTTOM(i, j) m_operation.goban[i][j+1]

void engine::init_area(bit32_t size)
{
    /** エリアーを初期化 **/
    for (bit32_t i=0; i<size+2; ++i)
    {
        for (bit32_t j=0; j<size+2; ++j)
        {
            m_operation.goban[i][j].area_id=0;
        }
    }
}
bit32_t engine::update_area(bit32_t size)
{
    for (int i=1; i<size+1; ++i)
    {
        for (int j=1; j<size+1; ++j)
        {
            /* ある点に対して、石がおかれていない場合、
               周りの点のidを適用する。
               もし、周りの点もidがなければ新たなidをつける。
               石が置かれているところのidはゼロである。 */
            if (m_operation.goban[i][j].type==stone::kNone)
            {
                if (ARRAY_LEFT(i, j).area_id != 0)
                {
                    m_operation.goban[i][j].area_id=ARRAY_LEFT(i, j).area_id;
                    continue;
                }
                if (ARRAY_TOP(i, j).area_id != 0)
                {
                    m_operation.goban[i][j].area_id=ARRAY_TOP(i, j).area_id;
                    continue;
                }
                if (ARRAY_RIGHT(i, j).area_id != 0)
                {
                    m_operation.goban[i][j].area_id=ARRAY_RIGHT(i, j).area_id;
                    continue;
                }
                if (ARRAY_BOTTOM(i, j).area_id != 0)
                {
                    m_operation.goban[i][j].area_id=ARRAY_BOTTOM(i, j).area_id;
                    continue;
                }
                /** 周りの石にエリアーIDが付与されていない場合は、新しいIDにする **/
                m_operation.goban[i][j].area_id=++m_operation.last_area_id;
            }
        }// for j
    }// for i

    return m_operation.last_area_id;
}
void  engine::change_area_map(const bit32_t before, const bit32_t after, const bit32_t goban_size)
{
    for (int i=1; i<goban_size+1; ++i)
    {
        for (int j=1; j<goban_size+1;++j)
        {
            if (m_operation.goban[i][j].area_id==before)
            {
                m_operation.goban[i][j].area_id=after;
            }
        }
    }
}
void engine::integrate_area_id(const bit32_t max_area_id, const bit32_t goban_size)
{
    bit32_t id;

    for (id=1; id<max_area_id; ++id)
    {
        for (int i=1; i<goban_size+1; ++i)
        {
            bool_t is_changed = false;
            for (int j=1; j<goban_size+1; ++j)
            {
                is_changed = false;
                if (m_operation.goban[i][j].area_id==id)
                {
                    if (ARRAY_RIGHT(i, j).area_id!=0 &&
                        ARRAY_RIGHT(i, j).area_id!=id)
                    {
                        is_changed=true;
                        change_area_map(id, ARRAY_RIGHT(i, j).area_id, goban_size);
                        break;
                    }
                    if (ARRAY_LEFT(i, j).area_id!=0 &&
                        ARRAY_LEFT(i, j).area_id!=id)
                    {
                        is_changed=true;
                        change_area_map(id, ARRAY_LEFT(i, j).area_id, goban_size);
                        break;
                    }
                    if (ARRAY_TOP(i, j).area_id!=0 &&
                        ARRAY_TOP(i, j).area_id!=id)
                    {
                        is_changed=true;
                        change_area_map(id, ARRAY_TOP(i, j).area_id, goban_size);
                        break;
                    }
                    if (ARRAY_BOTTOM(i, j).area_id!=0 &&
                        ARRAY_BOTTOM(i, j).area_id!=id)
                    {
                        is_changed=true;
                        change_area_map(id, ARRAY_BOTTOM(i, j).area_id, goban_size);
                        break;
                    }
               }
            } /* for j */

            /** 一個でも重なっていたら終了して、次のIDを検索する **/
            if (is_changed==true)
                break;
        } /* for i */
    }
}
void engine::calculate_confirm(const bit32_t max_area_id, const bit32_t goban_size)
{
    /* 領域が白なのか黒なのか、確定されていないのかの情報を入れる。
     m_operation.gobanの下位8ビットは、id
     m_operation.gobanの上位8ビットは、0(未確定領域), eST_BLACK, eST_WHITEである。*/
    for (bit32_t id=1; id<max_area_id; ++id)
    {
        stone::type_t flag = stone::kInvalid;
        bool_t is_unknown_area=false;
        for (int i=1; i<goban_size+1; i++)
        {
            for (int j=1; j<goban_size+1; j++)
            {
                if (m_operation.goban[i][j].area_id==id)
                {
                    /** right(石が置いてある場合) **/
                    if (ARRAY_RIGHT(i, j).area_id==0)
                    {
                        const stone::type_t type = (const stone::type_t)ARRAY_RIGHT(i, j).type;
                        if (type==stone::kBlack||type==stone::kWhite)
                        {
                            if (flag==stone::kInvalid)
                            {
                                flag = type;
                            }
                            else
                            {
                                if (flag!=type)
                                {
                                    is_unknown_area=true;
                                    break;
                                }
                            }
                        }
                    }
                    if (ARRAY_BOTTOM(i, j).area_id==0)
                    {
                        const stone::type_t type = (const stone::type_t)ARRAY_BOTTOM(i, j).type;
                        if (type==stone::kBlack||type==stone::kWhite)
                        {
                            if (flag==stone::kInvalid)
                            {
                                flag = type;
                            }
                            else
                            {
                                if (flag!=type)
                                {
                                    is_unknown_area=true;
                                    break;
                                }
                            }
                        }
                    }
                    if (ARRAY_LEFT(i, j).area_id==0)
                    {
                        const stone::type_t type = (const stone::type_t )ARRAY_LEFT(i, j).type;
                        if (type==stone::kBlack||type==stone::kWhite)
                        {
                            if (flag==stone::kInvalid)
                            {
                                flag = type;
                            }
                            else
                            {
                                if (flag!=type)
                                {
                                    is_unknown_area=true;
                                    break;
                                }
                            }
                        }
                    }
                    if (ARRAY_TOP(i, j).area_id==0)
                    {
                        const stone::type_t type = (const stone::type_t )ARRAY_TOP(i, j).type;
                        if (type==stone::kBlack||type==stone::kWhite)
                        {
                            if (flag==stone::kInvalid)
                            {
                                flag = type;
                            }
                            else
                            {
                                if (flag!=type)
                                {
                                    is_unknown_area=true;
                                    break;
                                }
                            }
                        }
                    }
                } /* if id==area_id */

                if (is_unknown_area==true)
                {
                    break;
                }
            } // for i

            if (is_unknown_area==true)
            {
                break;
            }
        } /* for j */

        if (is_unknown_area==false)
        {
            for (int i=1; i<goban_size+1; ++i)
            {
                for (int j=1; j<goban_size+1;++j)
                {
                    if (m_operation.goban[i][j].area_id==id)
                    {
                        // 確定領域
                        m_operation.goban[i][j].area_flag=flag;
                        if (flag==stone::kBlack)
                        {
                            ++m_black.area;
                        }
                        else
                        {
                            ++m_white.area;
                        }
                    }
                } /* for j */
            } /* for i */
        } /* if unknown_area */
    } /* for id */
}
void engine::start_calculate()
{
    bit32_t goban_size = m_sgf.GetGobanSize();
    /** areaを初期化します。**/
    init_area(goban_size);
    /** zero areaを割り当てます。**/
    m_operation.last_area_id = update_area(goban_size);
    /** areaIDを統合出来る場合は統合します。**/
    integrate_area_id(m_operation.last_area_id, goban_size);
    /** エリアーを確定します。**/
    calculate_confirm(m_operation.last_area_id, goban_size);
}
bool_t engine::restore_stone(stone_t stone, bit32_t goban_size)
{
    int _i = stone.x+1;
    int _j = stone.y+1;
    const stone::type_t dead_stone_type = (const stone::type_t)stone.type;
    const stone::type_t area_stone_type = REVERSE_STONE(stone.type);
    const bit32_t block_id = m_operation.goban[_i][_j].block_id;
    const bit32_t selected_area_id = m_operation.goban[_i][_j].area_id;

    IGX_ASSERT(selected_area_id);
    if (selected_area_id==0)
    {
        return false;
    }

    for (int i=0; i<goban_size; ++i)
    {
        for (int j=0; j<goban_size; ++j)
        {
            const stone::type_t type = (const stone::type_t)m_operation.goban[i][j].type;
            if (m_operation.goban[i][j].area_id==selected_area_id)
            {
                if (type==stone::kNone)
                {
                    if (m_operation.goban[i][j].area_flag==stone::kBlack)
                    {
                        --m_black.area;
                    }
                    else
                    {
                        --m_white.area;
                    }
                }
                else if (type==stone::kBlack)
                {
                    --m_white.catched_stone;
                }
                else
                {
                    --m_black.catched_stone;
                }
                m_operation.goban[i][j].area_flag=stone::kNone;
                m_operation.goban[i][j].area_id = 0;
            } /** if selected **/
        } /* for j */
    } /* for i */

    m_operation.last_area_id = update_area(goban_size);

    return true;

}
/** 周りにあるエリアーIDを検索する **/
bit32_t engine::get_surrounded_area(stone_t stone, size_t goban_size)
{
    const bit32_t _i = stone.x+1;
    const bit32_t _j = stone.y+1;
    const bit32_t type = stone.type;
    const bit32_t block_id = m_operation.goban[_i][_j].block_id;

    if (type==stone::kNone||
            type==stone::kInvalid)
    {
        /* ありえない？ */
        return 0;
    }

    /** まずは、まわりを探し、周りに既にエリアーIDがあれば、その値を返します。 **/
    if (ARRAY_LEFT(_i, _j).area_id!=0 &&
        ARRAY_LEFT(_i, _j).type==stone::kNone)
        return ARRAY_LEFT(_i, _j).area_id;
    if (ARRAY_RIGHT(_i, _j).area_id!=0 &&
        ARRAY_RIGHT(_i, _j).type==stone::kNone)
        return ARRAY_RIGHT(_i, _j).area_id;
    if (ARRAY_TOP(_i, _j).area_id!=0 &&
        ARRAY_TOP(_i, _j).type==stone::kNone)
        return ARRAY_TOP(_i, _j).area_id;
    if (ARRAY_BOTTOM(_i, _j).area_id!=0 &&
        ARRAY_BOTTOM(_i, _j).type==stone::kNone)
        return ARRAY_BOTTOM(_i, _j).area_id;

    /** もし、周りになにかしら石に囲まれている場合は、
        同じブロックの周りを探します。 **/
    for (int i=1; i<goban_size-1; ++i)
    {
        for (int j=1; j<goban_size-1; ++j)
        {
            if (m_operation.goban[i][j].block_id==block_id)
            {
                if (ARRAY_LEFT(i, j).area_id!=0 &&
                ARRAY_LEFT(i, j).type==stone::kNone)
                    return ARRAY_LEFT(i, j).area_id;
                if (ARRAY_RIGHT(i, j).area_id!=0 &&
                ARRAY_RIGHT(i, j).type==stone::kNone)
                    return ARRAY_RIGHT(i, j).area_id;
                if (ARRAY_RIGHT(i, j).area_id!=0 &&
                ARRAY_TOP(i, j).type==stone::kNone)
                    return ARRAY_TOP(i, j).area_id;
                if (ARRAY_BOTTOM(i, j).area_id!=0 &&
                ARRAY_BOTTOM(i, j).type==stone::kNone)
                    return ARRAY_BOTTOM(i, j).area_id;
            }
        }
    }
    /** 見つからなかった場合 **/
    return 0;
}
int engine::integrate_area(bit32_t id, stone::type_t type, const bit32_t goban_size)
{
    bool_t id_flags[256] = {false, };

    for (int i=1; i<goban_size+1; ++i)
    {
        for (int j=1; j<goban_size+1; ++j)
        {
            /* idが一致したら、周りに隣接する別のidがあるかチェックする。*/
            if (m_operation.goban[i][j].area_id==id)
            {
                {
                    const bit32_t area_id= ARRAY_LEFT(i, j).area_id;
                    if (area_id!=0&&area_id!=id)
                    {
                        id_flags[area_id]=true;
                    }
                }
                {
                    const bit32_t area_id= ARRAY_RIGHT(i, j).area_id;
                    if (area_id!=0&&area_id!=id)
                    {
                        id_flags[area_id]=true;
                    }
                }
                {
                    const bit32_t area_id = ARRAY_TOP(i, j).area_id;
                    if (area_id!=0&&area_id!=id)
                    {
                        id_flags[area_id]=true;
                    }
                }
                {
                    const bit32_t area_id = ARRAY_BOTTOM(i, j).area_id;
                    if (area_id!=0&&area_id!=id)
                    {
                        id_flags[area_id]=true;
                    }
                }
            } /* if (m_operation.goban[i][j].area_id==id) */
        } /* for i */
    } /* for j */

    int cnt=0;
    const bit32_t size = array_sizeof(id_flags);
    for (int k = 0; k<size; ++k)
    {
        if (id_flags[k]==false)
        {
            continue;
        }

        cnt++;

        for (int i=1; i<goban_size+1; ++i)
        {
            for (int j=1; j<goban_size+1; ++j)
            {
                if (m_operation.goban[i][j].area_id==k)
                {
                    m_operation.goban[i][j].area_id=id;
                }
            }
        }
   }

    /* 要らない ？*/
#if 0
    /* 隣接しているIDを統合する。*/
    int cnt=0;
    const bit32_t size = array_sizeof(id_flags);
    for (int k = 0; k<size; ++k)
    {
        if (id_flags[k]==false)
        {
            continue;
        }

        cnt++;
        for (int i=1; i<goban_size+1; i++)
        {
            for (int j=1; j<goban_size+1; j++)
            {
                if (m_operation.goban[i][j].area_id==k)
                {
                }
            }
        }
    }
#endif

    /** 領域を書き換える。**/
    for (int i=1; i<goban_size+1; i++)
    {
        for (int j=1; j<goban_size+1; j++)
        {
            if (m_operation.goban[i][j].area_id==id)
            {
                m_operation.goban[i][j].area_flag=type;
            }
        }
    }

    return cnt;
}
void engine::update_calculated_goban(const size_t goban_size)
{
    memset(&m_black, 0, sizeof(m_black));
    memset(&m_white, 0, sizeof(m_white));

    for (int i=1; i<goban_size+1; ++i)
    {
        for (int j=1; j<goban_size+1; ++j)
        {
            const __stone_t__ stone = m_operation.goban[i][j];
            if (stone.area_id==0)
            {
                continue;
            }

            const stone::type_t area = (const stone::type_t)stone.area_flag;
            const stone::type_t type = (const stone::type_t)stone.type;

            if (area==stone::kBlack)
            {
                ++m_black.area;
            }
            else if (area==stone::kWhite)
            {
                ++m_white.area;
            }
            if (type==stone::kBlack)
            {
                ++m_white.catched_stone;
            }
            else if (type==stone::kWhite)
            {
                ++m_black.catched_stone;
            }
        } /** for j **/
    } /** for i **/
}

/** この関数が呼び出されるというのは、何かしら石をクリックしている。**/
void engine::set_dead_stone(stone_t stone)
{
    const bit32_t goban_size = m_sgf.GetGobanSize();
    int _i = stone.x+1;
    int _j = stone.y+1;
    const stone::type_t dead_stone_type = (const stone::type_t)stone.type;
    const stone::type_t area_stone_type = REVERSE_STONE(stone.type);
    const bit32_t block_id = m_operation.goban[_i][_j].block_id;
    bit32_t blank_id=0;
    bit32_t area_id=0;
    bit32_t integrated=0;

    /** 地合計算時、上げ石選択 **/
    /* 1. もし、石が既に選択されていれば、元に戻す */
    if (restore_stone(stone, goban_size)==true)
    {
        return ;
    }

    /* 1. まず、関係のある領域IDを取得する。*/
    area_id = get_surrounded_area(stone, goban_size);

    /* 2. もしIDがなければ、選択された石の領域を新たない領域IDにする。*/
    if (area_id==0)
    {
        area_id = m_operation.goban[_i][_j].area_id = ++m_operation.last_area_id;
    }

SEARCH_DEAD_STONE:
    /* 3. 領域IDで検索し、IDの周りの同じ色の石を領域化する。*/
    /* 4. 領域化されないまで繰り返す。*/
    while(1)
    {
        bool_t is_changed = false;
        for (int i=1; i<goban_size+1; ++i)
        {
            for (int j=1; j<goban_size+1;++j)
            {
                const __stone_t__ area_stone = m_operation.goban[i][j];
                if (area_stone.area_id==area_id)
                {
                    /** もし、まだ領域が決まってない、石が隣にあれば、領域IDを変える **/
                    if (ARRAY_LEFT(i, j).area_id==0 &&
                            ARRAY_LEFT(i, j).type==dead_stone_type)
                    {
                        ARRAY_LEFT(i,j).area_id=area_id;
                        is_changed=true;
                    }
                    if (ARRAY_RIGHT(i, j).area_id==0 &&
                            ARRAY_RIGHT(i, j).type==dead_stone_type)
                    {
                        ARRAY_RIGHT(i,j).area_id=area_id;
                        is_changed=true;
                    }
                    if (ARRAY_TOP(i, j).area_id==0 &&
                            ARRAY_TOP(i, j).type==dead_stone_type)
                    {
                        ARRAY_TOP(i,j).area_id=area_id;
                        is_changed=true;
                    }
                    if (ARRAY_BOTTOM(i, j).area_id==0 &&
                            ARRAY_BOTTOM(i, j).type==dead_stone_type)
                    {
                        ARRAY_BOTTOM(i,j).area_id=area_id;
                        is_changed=true;
                    }

                }// if
            }// for j
        } // for i
        if (is_changed==false)
            break;
    } /* while */

    /**  6. 隣接する領域IDがあれば統合する。**/
    integrated = integrate_area(area_id, area_stone_type, goban_size);

    if (integrated==0)
    {
        /** 7. 領域に色を付ける。**/
        update_calculated_goban(goban_size);
        return ;
    }
   
    /** 統合される領域がなくなるまでまわす **/
    goto SEARCH_DEAD_STONE;

    return ;

}

} /** namespace go **/
