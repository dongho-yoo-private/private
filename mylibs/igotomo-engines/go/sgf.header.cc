/**
* Copyright (c) 2015 igotomo
*
* authorized dongho.yoo.
* @brief sgf parse header
*/
#include "sgf.h"
#include "intern/go_libs.h"
#include "library/igx_string.h"
#include "library/igx_stdlib.h"
#include "data_struct/itx_queue.h"
#include "debug/igx_debug.h"

#include <stdio.h>

#define SGF_PRINT_HEADER

namespace go {

const char* sgf::find_body(const char* sgf)
{
    char* p = (char*)sgf;
    bit32_t count=0;
    while(*p!=0)
    {
        if (*p==';')
        {
            if (*(p-1)!='\\')
            {
                if (count==1)
                {
                    return p;
                }
                ++count;
            }
        }
        ++p;
    }
    return 0;
}
const char* sgf::parse_header(const char* sgf, size_t sgf_len)
{
    const size_t tag_value_len = 32;
    char    value[tag_value_len];
    number_t n_value;
    bool_t is_utf8=false;

    const char* p_body = find_body(sgf);

    //IGX_DEBUG("%s", sgf);

    if (p_body==0)
    {
        IGX_ERROR("body is not found!!");
        return 0;
    }
    memset(&m_header, 0, sizeof(m_header));

    /* sgf チェック */
    if (libs::find_tag(sgf, "GM[", value)!=0)
    {
        if (igx_atoi(value, &n_value)==false)
        {
            IGX_ERROR("GM is invalid!!");
            return 0;
        }
#ifdef SGF_PRINT_HEADER
        IGX_INFO("GM is %lld, %s", n_value, value);
#endif
    }
    if (libs::find_tag(sgf, "FF[", value)!=0)
    {
        number_t n;
        bool_t ret = igx_atoi(value, &n);

#ifdef SGF_PRINT_HEADER
        IGX_INFO("FF is %d", n);
#endif
        if (ret==false)
        {
            IGX_ERROR("FF is invalid!!");
            return 0;
        }
        if (ret>4)
        {
            /* どうする？ */
            IGX_ERROR("FF is invalid!!");
            return 0;
        }
    }
    if (libs::find_tag(sgf, "CA[", value)!=0)
    {
#ifdef SGF_PRINT_HEADER
        IGX_INFO("CA is %s", value);
#endif
        // 文字コード
        if (strcasecmp(value, "UTF-8")!=0)
        {
            is_utf8=false;
            /** UTF-8に変換する **/
            //sgf=(const char*)igx_convert_utf8(0, 0, sgf, sgf_len, igx_get_charset_string(value));
        }
    }
    if (libs::find_tag(sgf, "AP[", value)!=0)
    {
        // TODO. application.
#ifdef SGF_PRINT_HEADER
        IGX_INFO("AP is %s", value);
#endif
    }
    size_t comment_len;
    const char* ret = libs::find_tag(sgf, "C[", &comment_len);
    if (ret!=0 && (ret<p_body))
    {
        const bit32_t count = igx_str_count_ch(ret, comment_len, '\\');
        m_top.data.comment=libs::remove_escape_sequence((char*)pmalloc(&m_mem_string_page, comment_len+1+count), ret, comment_len);

#ifdef SGF_PRINT_HEADER
        IGX_INFO("Comment is %s", m_top.data.comment);
#endif
    }
    parse_event_rules(sgf);
    parse_event(sgf);
    parse_player(sgf);
    parse_result(sgf);

    /** 初期置き石は特別処理 
    *  何故かはじめの置き石は、ヘッダー領域にあるので。。。
    **/
    char* p = (char*)sgf;
    p+=2; /** (;をはぶく **/
    //char buffer[1024];
    while(*p!=0 &&*(p+1)!=0&& *p!=';')
    {
        if (*p=='A' && (*(p+1)=='B'||*(p+1)=='W') && *(p+2)=='[')
        {
            char i, j;
            const stone::type_t type = (*(p+1)=='B')?stone::kBlack:stone::kWhite;
            p+=3;
PARSE_NEXT:
            if (sscanf(p, "%c%c]", &i, &j)==2)
            {
                stone_t stone;
                stone.stone=0;
                stone.type=type;
                stone.x=i-'a';
                stone.y=j-'a';

                if (m_top.data.initial_stone_list==0)
                {
                    m_top.data.initial_stone_list = new(m_mem_stone_page) ItxLinkedList<stone_t>(16);
                }
                m_top.data.initial_stone_list->Add(stone);

                IGX_INFO("%d, (%d,%d)", (int)type, (int)stone.x, (int)stone.y);
                p+=3;
                if (*p=='[')
                {
                    ++p;
                    goto PARSE_NEXT;
                }
                continue;
            }
        }
        ++p;
    }
    IGX_INFO("parse header success");

    return p_body;
}
char* sgf::load_string(const char* str, size_t len)
{
    char* ret = (char*)memcpy(pmalloc(&m_mem_string_page, len+1), str, len);
    ret[len]=0;
    return ret;
}
void sgf::parse_event(const char* sgf)
{
    const size_t tag_value_len = 32;
    char    value[tag_value_len];

    const char* tag_start=0;
    size_t tag_length;

    /** 大会の概要*/
    if ((tag_start = libs::find_tag(sgf, "GC[", &tag_length))!=0)
    {
        m_header.event.comment = load_string(tag_start, tag_length);
#ifdef SGF_PRINT_HEADER
        IGX_INFO("GC is %s", m_header.event.comment);
#endif
    }
    else if ((tag_start = libs::find_tag(sgf, "AN[", &tag_length))!=0)
    {
        m_header.event.comment = load_string(tag_start, tag_length);
#ifdef SGF_PRINT_HEADER
        IGX_INFO("AN is %s", m_header.event.comment);
#endif
    }
    /** 大会名 */
    if ((tag_start = libs::find_tag(sgf, "EV[", &tag_length))!=0)
    {
        m_header.event.name = load_string(tag_start, tag_length);
#ifdef SGF_PRINT_HEADER
        IGX_INFO("EV is %s", m_header.event.name);
#endif
    }
    else if ((tag_start = libs::find_tag(sgf, "TE[", &tag_length))!=0)
    {
        m_header.event.name = load_string(tag_start, tag_length);
#ifdef SGF_PRINT_HEADER
        IGX_INFO("TE is %s", m_header.event.name);
#endif
    }
    /** 回戦 */
    if ((tag_start = libs::find_tag(sgf, "RO[", &tag_length))!=0)
    {
        m_header.event.count = load_string(tag_start, tag_length);
#ifdef SGF_PRINT_HEADER
        IGX_INFO("RO is %s", m_header.event.count);
#endif
    }
    /** 対局場所 */
    if ((tag_start = libs::find_tag(sgf, "PC[", &tag_length))!=0)
    {
        m_header.event.place = load_string(tag_start, tag_length);
#ifdef SGF_PRINT_HEADER
        IGX_INFO("PC is %s", m_header.event.place );
#endif
    }
    /** 対局開始日*/
    if (libs::find_tag(sgf, "DT[", value)!=0)
    {
       m_header.event.date = igx_sting_to_time(value, 0); 
       if (m_header.event.date.year==0&&m_header.event.date.month==0&&m_header.event.date.day==0)
       {
           IGX_WARN("no support time format! (%s)", value);
       }
#ifdef SGF_PRINT_HEADER
        IGX_INFO("DT is %s", value);
#endif
    }
    else if (libs::find_tag(sgf, "RD[", value)!=0)
    {
        /** 日本棋院独自仕様*/
        m_header.event.date = igx_sting_to_time(value, 0); 
        if (m_header.event.date.year==0&&m_header.event.date.month==0&&m_header.event.date.day==0)
        {
           IGX_WARN("no support time format! (%s)", value);
        }
#ifdef SGF_PRINT_HEADER
        IGX_INFO("RD is %s", value);
#endif
    }

}
static bit32_t __get_goban_type__(bit32_t size)
{
    switch(size)
    {
        case 13:
            return kBoardSize13;
        case 9:
            return kBoardSize9;
        default:
            break;
    }
    return kBoardSize19;
}
static bit32_t __get_goban_size__(board_size_t size)
{
    switch(size)
    {
        case kBoardSize19:
            return 19;
        case kBoardSize13:
            return 13;
        case kBoardSize9:
            return 9;
    }
    return 19;
}

void sgf::parse_event_rules(const char* sgf)
{
    const size_t tag_value_len = 32;
    char    value[tag_value_len];
    float   f_value;

    /** コミ情報は、KM[]かKO[]から入ってくる*/
    bool_t komi_tag_found=false;
    if ((komi_tag_found=(libs::find_tag(sgf, "KM[", value)!=0))==false)
    {
        komi_tag_found=(libs::find_tag(sgf, "KO[", value)!=0);
    }

    if (komi_tag_found==true)
    {
        if (igx_atof(value, &f_value)==false)
        {
           m_header.event.rules.komi=0;
        }
        else
        {
            bit32_t komi=(bit32_t)f_value;
            bit32_t abskomi = abs(komi);

            abskomi= (abskomi>0xFF)?0xFF:abskomi;
            m_header.event.rules.komi=abskomi;
            m_header.event.rules.komi_is_black=(komi<0);
            m_header.event.rules.half_score = ((f_value-(float)komi)!=0.0f);

#ifdef SGF_PRINT_HEADER
        IGX_INFO("KOMI is %d%s", (int)m_header.event.rules.komi, m_header.event.rules.half_score?".5":"");
#endif
        }
    }
    /** handycap 置き石の数 */ 
    if (libs::find_tag(sgf, "HA[", value)!=0)
    {
        if (igx_atof(value, &f_value)==true)
        {
            bit32_t oki_stone = (bit32_t)f_value;
            oki_stone = (oki_stone>0&&oki_stone<0x10)?oki_stone:0;
            m_header.event.rules.handycap = oki_stone;
#ifdef SGF_PRINT_HEADER
        IGX_INFO("HA is %d", (int)m_header.event.rules.handycap);
#endif
        }
    }
    /** 碁盤サイズ*/
    if (libs::find_tag(sgf, "SZ[", value)!=0)
    {
        bit32_t n = atoi(value);
        m_header.event.rules.size = __get_goban_type__(n);
#ifdef SGF_PRINT_HEADER
        IGX_INFO("SZ is %d", (int)n);
#endif
    }
    else
    {
        m_header.event.rules.size = kBoardSize19;
    }
    /** 持ち時間*/
    if (libs::find_tag(sgf, "TM[", value)!=0)
    {
#ifdef SGF_PRINT_HEADER
        IGX_INFO("TM is %s", value);
#endif
        number_t sec;
        if (igx_atoi(value, &sec)==true)
        {
            IGX_DEBUG("sec:%d", sec);
            m_header.event.rules.limit_time=1;
            m_header.event.limit_time=igx_time_msec_to_time(sec*1000);
#ifdef SGF_PRINT_HEADER
        IGX_INFO("TM is %d", (int)sec);
#endif
        }
    }
    else
    {
        /** 制限時間無し*/
        m_header.event.rules.limit_time=0;
    }
    /** 無限秒読みの情報*/
    if (libs::find_tag(sgf, "STM[", value)!=0)
    {
        number_t sec;
        if (igx_atoi(value, &sec)==true)
        {
            m_header.event.rules.short_second_term=(sec&0x07);
        }
    }
    else
    {
        /** 秒読みも無し */
        m_header.event.rules.short_second_term=0;
    }
    /** 長い秒読みの時間*/
    if (libs::find_tag(sgf, "OT[", value)!=0)
    {
#ifdef SGF_PRINT_HEADER
        IGX_INFO("OT is %s", value);
#endif
        bit32_t sec, count;
        if (sscanf(value, "%dx%d", &sec, &count)==2)
        {
            const ubit32_t max_long_second_term = (0x0f*30);
            sec/=30;
            m_header.event.rules.long_second_count = (count&0x0F);
            m_header.event.rules.long_second_term=(sec>max_long_second_term)?max_long_second_term:sec;
        }
        else if (sscanf(value, "%d", &sec))
        {
            const ubit32_t max_long_second_term = (0x0f*30);
            sec/=30;
            m_header.event.rules.long_second_count= 0;
            m_header.event.rules.long_second_term=(sec>max_long_second_term)?max_long_second_term:sec;
        }
        else
        {
            IGX_WARN("no support format OT[%s]", value);
        }
    }
}
void sgf::parse_result(const char* sgf)
{
    char    value[32];
    float   f_value;

    /** RE[B+R] RE[B+30] RE[D] RE[0] */
    if (libs::find_tag(sgf, "RE[", value)!=0)
    {
#ifdef SGF_PRINT_HEADER
        IGX_INFO("RE is %s", value);
#endif
        bool_t is_win =false;
        if (value[0]=='B')
        {
            m_header.result.result=1;
            is_win =true;
        }
        else if (value[0]=='W')
        {
            m_header.result.result=2;
            is_win =true;
        }
        else if (value[0]=='0' || value[0]=='D' || value[0]=='V') // WS
        {
            m_header.result.result=3;
        }
        else
        {
            m_header.result.result=0;
        }

        if (is_win==true)
        {
            if (value[1]=='+')
            {
                if (value[2]=='R')
                {
                    m_header.result.type=1;
                }
                else
                {
                    if (igx_atof(value+2, &f_value)==true)
                    {
                        m_header.result.score = (bit32_t)((f_value>361)?f_value:f_value);
                    }
                }
            }
        }
    }/* if RE*/
    else
    {
        m_header.result.result=0;
    }
}
void sgf::parse_player(const char* sgf)
{
    const size_t tag_value_len = 32;
    char    value[tag_value_len];
    const char* tag_start=0;
    size_t tag_length;
    const char* name_tag[2] = {"PB[", "PW["};
    const char* rank_tag[2] = {"BR[", "WR["};
    const char* team_tag[2] = {"BT[", "WT["};
    const char* title_tag[2] = {"BTN[", "WTN["}; /*称号*/

    /** プレイヤー情報*/
    for (bit32_t i=0; i<2; ++i)
    {
        if ((tag_start = libs::find_tag(sgf, name_tag[i], &tag_length))!=0)
        {
            m_header.player[i].name = load_string(tag_start, tag_length);
#ifdef SGF_PRINT_HEADER
        IGX_INFO("name is %s", m_header.player[i].name);
#endif
        }
        if ((tag_start = libs::find_tag(sgf, rank_tag[i], value))!=0)
        {
            m_header.player[i].level = libs::get_player_level_from_string(value);
#ifdef SGF_PRINT_HEADER
        IGX_INFO("rank is %s", value);
#endif
        }
        if ((tag_start = libs::find_tag(sgf, team_tag[i], &tag_length))!=0)
        {
            m_header.player[i].team = load_string(tag_start, tag_length);
#ifdef SGF_PRINT_HEADER
        IGX_INFO("team is %s", m_header.player[i].team);
#endif
        }
        if ((tag_start = libs::find_tag(sgf, title_tag[i], &tag_length))!=0)
        {
            m_header.player[i].title = load_string(tag_start, tag_length);
#ifdef SGF_PRINT_HEADER
        IGX_INFO("title is %s", m_header.player[i].title);
#endif
        }
    }/* for */
}
static stone_t* get_handycap_stone_list(bit32_t handycap)
{
    static bool_t  initialized=false;
    static stone_t stones[32];


    if (initialized==false)
    {
        stones[0].set(15, 3, stone::kBlack);
        stones[1].set(3, 15, stone::kBlack);
        stones[2].set(15, 15, stone::kBlack);
        stones[3].set(3, 3, stone::kBlack);
    }
    initialized=true;

    if (handycap<5)
    {
        return stones;
    }

    if (handycap==5)
    {
        stones[4].set(9, 9, stone::kBlack);
    }
    else if (handycap==6)
    {
        stones[4].set(3, 9, stone::kBlack);
        stones[5].set(15, 9, stone::kBlack);
    }
    else if (handycap==7)
    {
        get_handycap_stone_list(5);
        stones[5].set(3, 9, stone::kBlack);
        stones[6].set(15, 9, stone::kBlack);
    }
    else if (handycap==8)
    {
        get_handycap_stone_list(6);
        stones[6].set(9, 15, stone::kBlack);
        stones[7].set(9, 3, stone::kBlack);
    }
    else if (handycap==9)
    {
        get_handycap_stone_list(8);
        stones[8].set(9, 9, stone::kBlack);
    }

    return stones;

}
void sgf::set_header(const sgf_header_t* header)
{
    IGX_ERROR("sgf::set_header called");
    igx_print_current_stacktrace();

    /** とりあえず、コピーしておいて、文字列のポインターだけ保持しておく*/
    m_header=*header;

#define __STR_DUP__(a) \
if (a!=0)\
{\
    a = m_mem_string_page.StrDup(a);\
}
    __STR_DUP__(m_header.event.comment);
    __STR_DUP__(m_header.event.name);
    __STR_DUP__(m_header.event.place);
    __STR_DUP__(m_header.event.count);
    __STR_DUP__(m_header.player[0].name);
    __STR_DUP__(m_header.player[0].team);
    __STR_DUP__(m_header.player[1].name);
    __STR_DUP__(m_header.player[1].team);

    const bit32_t handycap = m_header.event.rules.handycap;
    if (handycap>1)
    {
        m_top.data.initial_stone_list = new(m_mem_stone_page) ItxLinkedList<stone_t>(handycap);
        const stone_t* stones = get_handycap_stone_list(handycap);

        for (bit32_t i=0; i<handycap; ++i)
        {
            m_top.data.initial_stone_list->Add(stones[i]);
        }
    }
}

/** header sample (このヘッダーで500バイト足らずなので、ヘッダーが1kb超えることはまず、あり得ない。
    問題は概要の説明だが、512バイトまで格納するようにする。
    画面の仕様を決める際に、文字列の長さの制限も考えておく。

    ex)
    (;CA[UTF-8]FF[4]GM[1]AP[Igotomo:v1.0.0]SZ[19]EV[富士通杯世界囲碁選手権]RO[決勝戦第２局]PC[日本棋院]TM[2015-07-01]KM[6.5]HA[0]OT[30x5]AB[cc]AB[cp]AB[pp]\n
    PB[小林光一]TN[棋聖]BT[日本]BR[9d]\n
    PW[マイケルレドモンド]TN[アマチュア選手権者]WT[アメリカ]BR[9d]\n
    RE[B+R]\n
    C[初めての日本とアメリカの世界戦の決勝戦。\n本局ははじめから最後まで波乱の続きであり、最後の最後まで予測不能な一局であった]\n
**/
#if PRINT_TO_DIRECT==1
#   define OUT_PRINTF(...) sprintf(p, __VA_ARGS__)
#else
#   define OUT_PRINTF(...) dprintf(fd, __VA_ARGS__)
#endif
/** mmapを使用して汎用性を高める。*/
#if PRINT_TO_DIRECT==1
char* sgf::export_header(char* buffer, const char* application_name, const char* version) const
{
    char* p = buffer;
#else
char* sgf::export_header(int fd, const char* application_name, const char* version)
{
    bit32_t p=0;
#endif

    /* 以下は必須*/
    p += OUT_PRINTF( "(;CA[%s]FF[4]GM[1]SZ[%d]", "UTF-8", __get_goban_size__((board_size_t )m_header.event.rules.size));

    if (application_name!=0)
    {
        if (version!=0)
        {
            p+=OUT_PRINTF( "AP[%s:%s]", application_name, version);
        }
        else
        {
            p+=OUT_PRINTF( "AP[%s]", application_name);
        }
    }

    /*イベント情報*/
    if (m_header.event.name!=0)
    {
        *p='\n',++p;
        p+=OUT_PRINTF( "EV[%s]", m_header.event.name);
    }
    if (m_header.event.count!=0)
    {
        p+=OUT_PRINTF( "RO[%s]", m_header.event.count);
    }
    if (m_header.event.place!=0)
    {
        p+=OUT_PRINTF( "PC[%s]", m_header.event.place);
    }
    if (m_header.event.date.date!=0)
    {
        char time_buffer[32];
        p+=OUT_PRINTF( "DT[%s]", igx_time_to_string(time_buffer, m_header.event.date, "%04d-%02d-%02d %02d:%02d:%02d"));
    }
    /* ルール*/
    /* TM */
    if (m_header.event.limit_time.date!=0)
    {
        char time_buffer[32];
        p += OUT_PRINTF( "TM[%s]", igx_time_to_string(time_buffer, m_header.event.date, "%02d-%02d-%02d %02d:%02d:%02d"));
    }
    /* KM */
    if (m_header.event.rules.komi!=0 || m_header.event.rules.half_score!=0)
    {
        float f_komi = (float)(m_header.event.rules.komi)+(m_header.event.rules.half_score==1?0.5:0.0);
        f_komi = (m_header.event.rules.komi_is_black==true)?f_komi*(-1.0):f_komi;
        p += OUT_PRINTF( "KM[%.1f]", f_komi);
    }
    /* HA */
    if (m_header.event.rules.handycap!=0)
    {
        p += OUT_PRINTF("HA[%d]", (bit32_t)m_header.event.rules.handycap);
    }
    /* OT */
    if (m_header.event.rules.long_second_term!=0)
    {
        bit32_t count, sec;
        count = m_header.event.rules.long_second_count;
        sec   = m_header.event.rules.long_second_term*30;

        if (count)
        {
            p += OUT_PRINTF( "OT[%dx%d]", sec, count);
        }
        else
        {
            p += OUT_PRINTF( "OT[%d]", sec);
        }
    }
    if (m_top.data.initial_stone_list!=0)
    {
        *p='\n',++p;
        ItxLinkedList<stone_t>* stones = m_top.data.initial_stone_list;
        if (stones!=0)
        {
            for (ItxNode<stone_t>* node= stones->GetTopFrom(); node; node=node->next)
            {
                const stone_t stone = **node;
                p+=OUT_PRINTF("A%c[%c%c]", (stone.type==stone::kBlack)?'B':'W', stone.x+'a', stone.y+'a');
            }
        }
    }
    /** プレイヤー情報*/
    for (bit32_t i=0; i<2; ++i)
    {
        if (m_header.player[i].name!=0)
        {
            *p='\n',++p;
            p+=OUT_PRINTF( "P%c[%s]", (i==0)?'B':'W', m_header.player[i].name);
        }
        if (m_header.player[i].title!=0)
        {
            p+=OUT_PRINTF( "%cTN[%s]", (i==0)?'B':'W', m_header.player[i].title);
        }
        if (m_header.player[i].team!=0)
        {
            p+=OUT_PRINTF( "%cT[%s]", (i==0)?'B':'W', m_header.player[i].team);
        }
        if (m_header.player[i].level!=0)
        {
            char level_buffer[32];
            p+=OUT_PRINTF( "%cR[%s]", (i==0)?'B':'W', libs::get_player_level_to_string(level_buffer, m_header.player[i].level));
        }
    }

    /** 結果 */
    if (m_header.result.result!=0)
    {
        *p='\n',++p;
        if (m_header.result.type==1)
        {
            p+=OUT_PRINTF( "RE[%c+R]", (m_header.result.result==1)?'B':'W');
        }
        else
        {
            if (m_header.result.result!=3)
            {
                float score = (float)m_header.result.score+((m_header.event.rules.half_score==1)?0.5:0.0);
                p+=OUT_PRINTF( "RE[%c+%.1f]", (m_header.result.result==1)?'B':'W', score);
            }
            else
            {
                /** 引き分け*/
                p+=OUT_PRINTF( "RE[0]");
            }
        }
    }
    /** コメントは最後に改行付きで書いた方が良い **/
    if (m_header.event.comment!=0)
    {
        p+=OUT_PRINTF("\nC[%s]", m_header.event.place);
    }
    *p='\n',++p;

#if PRINT_TO_DIRECT==1
    return p;
#else
    return (char*)p;
#endif
}


};
