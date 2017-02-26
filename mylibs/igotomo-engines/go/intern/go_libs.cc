/**
* Copyright (c) 2015 igotomo
*
* authorized dongho.yoo.
* @brief go内部で使われる専用の共通モジュール
*/
#include "go_libs.h"
#include <stdio.h>
#include "library/igx_string.h"
#include "debug/igx_debug.h"

namespace go { namespace libs {

static char* find_end(char* p)
{
    while(*p!=']'&&*p!=0) ++p;

    if (*p==0)
    {
        return 0;
    }

    if (*(p-1)=='\\')
    {
        /*この場合の[は文字列として扱う */
        return find_end(p+1);
    }
    return p;
}

/** タグを検索し、長さを取り出す。*/
const char* find_tag(const char* sgf, const char* tag, size_t* length)
{
    bit32_t len;
    char* p_index;
    // タグがない。
    if ((p_index = (char*)strstr((const char*)sgf, tag))==0)
    {
        *length=0;
        return 0;
    }
    if (IGX_STR_IS_ALPHABET(p_index[-1])==true)
    {
        return find_tag(++p_index, tag, length);
    }
    len = strlen(tag);
    p_index+=len;

    char* p_tag_end = find_end(p_index);

    if (p_tag_end==0)
    {
        return 0;
    }

    *length = p_tag_end-p_index;
    IGX_INFO("%s is found !!", tag);

    return p_index;
}
/** タグを検索し、中身を取り出す。*/
const char* find_tag(const char* sgf, const char* tag, char* dest)
{
    intptr_t len;
    char* p_index;
    // タグがない。
    if ((p_index = (char*)strstr((const char*)sgf, tag))==0)
    {
        *dest=0;
        return 0;
    }
    /** "B["検索時に"AB["が引っかかる場合の対策 **/
    if (IGX_STR_IS_ALPHABET(p_index[-1])==true)
    {
        return find_tag(++p_index, tag, dest);
    }

    len = strlen(tag);
    p_index=p_index+len;

    char* p_tag_end = find_end(p_index);

    if (p_tag_end==0)
    {
        return 0;
    }

    len = p_tag_end-p_index;
    bit32_t dest_len,i;
    for (i=0, dest_len=0; i<len; ++i)
    {
        if (p_index[i]=='\\')
        {
            continue;
        }
        dest[dest_len++]=p_index[i];
    }

    dest[dest_len]=0;
    return p_index;
}
char* find_body(const char* sgf)
{
	char* p = (char*)sgf;

	while(*p)
	{
		if (p[0]==';')
		{
			p++;

			while (*p=='\r' || *p=='\n')
			{
				p++;
			}

			if ((*p=='B' || *p=='W' || *p=='C') && p[1]=='[')
			{
				return p;
			}
			continue;
		}

		++p;
	}

	return 0;
}

bit32_t get_skip_char_count(const char* comment)
{
#define BEGIN_SKIP_CHAR if ( 1
#define SKIP_CHAR(a) || comment[count]==a
#define END_SKIP_CHAR ) ++ret;
    int ret=0;
    int count=0;
    while(comment[count]!=0)
    {
        /** append skip char **/
        BEGIN_SKIP_CHAR 
            SKIP_CHAR('[')
            SKIP_CHAR(']')
            SKIP_CHAR(';')
            SKIP_CHAR('(')
            SKIP_CHAR(')')
        END_SKIP_CHAR
        ++count;
    }

    return ret;
}
bool_t is_skip_char(char ch)
{
#undef  SKIP_CHAR
#undef  BEGIN_SKIP_CHAR 
#undef  END_SKIP_CHAR 
#define BEGIN_SKIP_CHAR if ( 1
#define SKIP_CHAR(a) || ch==a
#define END_SKIP_CHAR ) return true;
    BEGIN_SKIP_CHAR
       SKIP_CHAR('[')
       SKIP_CHAR(']')
       SKIP_CHAR(';')
       SKIP_CHAR('(')
       SKIP_CHAR(')')
    END_SKIP_CHAR

return false;
   
}
go::level::type_t get_player_level_from_string(const char* str_level)
{
    bit32_t level=-1;
    const char* level_format[] = { "%dk", "%d kyu", "%dd", "%d dan"};
    const bit32_t count = array_sizeof(level_format);
    bool_t is_kyu=false;

    for (int i=0; i<count; ++i)
    {
        if (sscanf(str_level, level_format[i], &level)==1)
        {
            if (i==0||i==1)
            {
                is_kyu = true;
                break;
            }
        }
    }

    if (level!=-1)
    {
        return (go::level::type_t) ((is_kyu?go::level::kKyu:0)|level);
    }

    return go::level::kUnknown; 
}
char* get_player_level_to_string(char* buffer, bit32_t level)
{
    if (level&go::level::kKyu)
    {
        sprintf(buffer, "%dk", (int)level&0xFF);
    }
    else
    {
        sprintf(buffer, "%dd", (int)level&0xFF);
    }
    
    return buffer;
}
const char* get_label_string(go::label::type_t type)
{
    if (type==go::label::kCircle) return "CR";
    if (type==go::label::kTriAngle) return "TR";
    if (type==go::label::kDiamond) return "MA";
    if (type==go::label::kRectangle) return "RC";
    //if (type==go::label::kDoubleCircle) return "DR";
    //if (type==go::label::kCross) return "X";

    IGX_ERROR("no support labeltype:%d", type);
    IGX_ASSERT(0);
    return 0;
}
char* remove_escape_sequence(char* buffer, const char* src, size_t src_len)
{
    char*p =(char*)src, *p_dest=buffer;

    if (buffer==0)
    {
        buffer = (char*)igx_malloc(src_len+1, "remove_escape_sequence");
        p_dest=buffer;
    }

    for(;*p;++p)
    {
        if (*p=='\\')
        {
            continue;
        }
        *p_dest=*p;
    }
    *p_dest=0;

    return buffer;
}
// -----------------------------------
// command
// 1: black B
// 2: white W
// 3: black pass B[]
// 4: white pass W[]
// 5: comment C[]
// 6: Circle CR
// 7: Triangle TR
// 8: Square SQ
// 9: MA 
//10: Label LB[xx: ]
//11: Start Branch (
//12: End Branch )
//13: AB
//14: AW
//15: CX // Comment Code [no:no]
//16: ST // Status eGoResultOptionQÆ
//17: OX[no] // IvVR[h
//21: FX[no] //lCx(1-5)
//22: DE[no] // ïÕx(1-5)
//23: MF[no] // }X^[tO(¬÷·êÎ+1)
//24: JX[no,no,no] // èÎ¼
//25: XR[ref] // t@XÔ
//26: XC[ref] // t@XÔ
//27: XP[]
//28: XF[]
//29: XS[ref] // N{Ì
//30: XL[ref] // NQÆ
//31: xREF( // Ql
//32: xSV[
//34: xPD[ // Ä¶ÏÝ@èÎ«TÅµ©gíêÈ¢B

char* get_node_info(const char* sgf, node_info_t& node_info);

}}
