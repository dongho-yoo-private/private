/**
* Copyright (c) 2015 
*
* authorized dongho.yoo.
* @brief sgf parse body 
*/
#include "sgf.h"
#include "debug/igx_debug.h"
#include "intern/go_libs.h"
#include "library/igx_stdlib.h"
#include "library/igx_string.h"
#include <stdio.h>

namespace go {

#define LIMIT_REMAIN_BUFFER_SIZE (128)

#define CREATE_STONE_NODE() (stone_node_t*)memset(pmalloc(m_mem_stone_page, sizeof(stone_node_t)), 0, sizeof(stone_node_t))
const char* sgf::parse_branch(const char* sgf, stone_node_t* node, ubit32_t order)
{
    char* p = (char*)sgf;
    ++p;
    if (node->branches==0)
    {
        node->branches = new(m_mem_stone_page) ItxLinkedList<stone_node_t*>(8, m_mem_stone_page);
    }
    stone_node_t* new_node = CREATE_STONE_NODE();
    /* parentがある場合のみ、ブランチのトップ*/
    new_node->parent = node;
    node->branches->Add(new_node);
    return parse_body((const char*)p, new_node, order);
}
const char* sgf::parse_body(const char* sgf, stone_node_t* before, ubit32_t order)
{
    char* p = (char*)sgf;
    stone_node_t* node = before;

   
#define CHECK_SUCCESS(ret) \
if (ret!=0)\
{\
    if (p!=ret)\
    {\
        p=ret;\
        continue;\
    }\
}
#define SKIP_CHAR(p) \
if ((*p>'z'||*p<'A')&&(*p!=';')&&(*p!='(')&&(*p!=')'))\
{\
    ++p;\
    continue;\
}
    while(*p!=0 && *p!=')')
    {
        const char ch = *p;
        char* ret;

        SKIP_CHAR(p);
        
        if (ch=='(')
        {
            p = (char*)parse_branch(p+1, node, order);
            ++p;
            continue;
        }
        if (ch==';')
        {
            stone_node_t* temp = node;
            node->next = CREATE_STONE_NODE();
            node=node->next;
            node->prev = temp;
            ++p;
            continue;
        }

        if (*p=='[')
        {
            p=dispatch_tag(p, node);
            if (p==0)
            {
                /* invalid sgf format */
                return 0;
            }
            continue;
        }
        ++p;
    } /* while */
    return (const char*)p;
}
char* sgf::dispatch_tag(char* p, stone_node_t* node)
{
    char* p_tag_end = p;
    char* p_tag_start= p;
    char* p_next;
    bit32_t tag_len=0;
    /* タグはアルファベットなので、アルファベットが終わるまで検索する。*/
    --p;
    while((IGX_STR_IS_ALPHABET(*p)!=false)&&tag_len++!=3)--p;

    if (tag_len==3)
    {
        /* unsupport format! */
        IGX_WARN("unsupport tag! %c%c%c", *p, p[1], p[2]);
        return ++p_tag_end;
    }
    p=++p;

    if (tag_len==1)
    {
        if (*p=='B'||*p=='W')
        {
            return parse_stone(p, node);
        }
        else if (*p=='C')
        {
            return parse_comment(p, node);
        }
        else
        {
            IGX_WARN("no support tag %c[]", *p);
            return p_tag_end;
        }
    }
    else if (tag_len==2)
    {
        if (*p=='A' && (*(p+1)=='B'||*(p+1)=='W'))
        {
            return parse_init_stone(p, node);
        }
        else if ((*p=='B'||*p=='W')&&(*(p+1)=='L'))
        {
            return parse_remain_time(p, node);
        }
        else
        {
            const char* label_tags[] = {
                "LB", "CR", "TR", "MA", "RT"
            };
            const size_t label_tag_count = array_sizeof(label_tags);
            for (bit32_t i=0; i<label_tag_count; ++i)
            {
                if (*p==label_tags[i][0] && *(p+1)==label_tags[i][1])
                {
                    return parse_label(p, node, (label::type_t)i);
                }
            }

            IGX_WARN("no support tag! %c%c[]", *p, *(p+1));
            return p_tag_end;
        }
        
    }
    else
    {
        IGX_WARN("no support tag! %c%c%c[]", *p, *(p+1), *(p+2));
    }
    return p_tag_end;
}/* sgf::dispatch_tag */
char* sgf::parse_stone(const char* _p, stone_node_t* node)
{
    char* p = (char*)_p, x,y;
    const bit32_t goban_size = GetGobanSize();
    stone_node_t* prev = node->prev;
    bit32_t order=0;

    memset(&node->data.stone, 0, sizeof(stone_t));
    node->data.stone.type=(*p=='B')?stone::kBlack:stone::kWhite;

    if (prev==0)
    {
        prev = node->parent;
        /* is not top */
        if (prev!=0)
        {
            order=prev->data.stone.order+1;
        }
    }
    node->data.stone.order=order;

    /* pass */
    if (*(p+2)==']')
    {
       node->data.stone.is_pass=1;
       return p+3;
    }
    if (sscanf(p+2, "%c%c]", &x, &y)!=2)
    {
        /* invalid format */
        IGX_ERROR("invalid format!! %s", p+2);
        return 0;
    }
    if (x=='x'||y=='x')
    {
        /* PASS */
        node->data.stone.is_pass=1;
        return p+5;
    }

    node->data.stone.x    = x-'a';
    node->data.stone.y    = y-'a';

    /* error check! */
    if (node->data.stone.x<goban_size &&
            node->data.stone.y<goban_size)
    {
        return p+5;
    }

    /* invalid format! */
    IGX_ERROR("stone position:%c,%c!!", x, y);
    return 0;

} /* sgf::parse_stone */

char* sgf::parse_comment(const char* _p, stone_node_t* node)
{
    char* p = (char*)_p;
    const char* p_comment_start = (char*)_p+2; /* C[ */
    size_t len=0;

    for (;*p;++len, ++p)
    {
        if (*p==']'&&*(p-1)!='\\')
        {
            break;
        }
    }

    /* ] をとりのぞく */
    --len;

    if (*p==0)
    {
        /* invalid format! */
        return 0;
    }
    node->data.comment = (char*)memcpy((char*)pmalloc(&m_mem_string_page, len), p_comment_start, len-1);
    node->data.comment[len]=0;

    igx_str_remove_ch(node->data.comment, '\\');
    return ((char*)p_comment_start+len);
} /* sgf::parse_comment */
char* sgf::parse_init_stone(const char* _p, stone_node_t* node)
{
    char* p = (char*)_p;
    const bit32_t goban_size = GetGobanSize();
    stone::type_t type =(*(p+1)=='B')?stone::kBlack:stone::kWhite;
    bit32_t stone_count=0;
    p+=2; /*'['*/

    while(*p=='[')
    {
        char x, y;
        stone_t stone = kNULL_STONE;
        if (sscanf(p, "[%c%c]", &x, &y)!=2)
        {
            /* invalid format! */
            IGX_ERROR("invalid init stone position %c%c", *(p+1), *(p+2));
            return 0;
        }
        stone.x = x-'a';
        stone.y = y-'a';
        
        /* エラーチェック*/
        if (stone.x>=goban_size ||
                stone.y>=goban_size)
        {
            IGX_ERROR("invalid stone position %d,%d", stone.x, stone.y);
            return 0;
        }

        ++stone_count;
        if (node->data.initial_stone_list==0)
        {
            node->data.initial_stone_list = new(m_mem_stone_page) ItxLinkedList<stone_t>(16);
            IGX_ASSERT(node->data.initial_stone_list);
        }

        node->data.initial_stone_list->Add(stone);
        p+=4;
        if (*p=='\r')
        {
            ++p;
        }
        if (*p=='\n')
        {
            ++p;
        }
    }/* while */

    if (stone_count==0)
    {
        /* invalid format ! */
        IGX_ERROR("%c%c[] stone count is zero", *_p, *(_p+1));
        return 0;
    }

    return p;
} /* sgf::parse_init_stone */
char* sgf::parse_remain_time(const char* _p, stone_node_t* node)
{
    float remain_time;
    char* p=(char*)_p;
    if (sscanf(p+2, "[%f]", &remain_time)!=1)
    {
        /* invalid format! */
        IGX_ERROR("%c%c[] invalid format!", *p, *(p+1));
        return 0;
    }
    node->data.remain_time=remain_time;

    for (;*p; ++p)
    {
        if (*(p-1)!='\\'&&*p==']')
        {
            return ++p;
        }
    }
    return 0;
}/* sgf::parse_remain_time */
char* sgf::parse_label(const char* _p, stone_node_t* node, label::type_t type)
{
    char* p = (char*)_p+2;
    label_t label = {0, };
    bit32_t label_count=0;
    const bit32_t goban_size = GetGobanSize();

    label.type=type;

    while(*p=='[')
    {
        char x, y, ch=0;
        if (type!=0 && sscanf(p, "[%c%c]", &x, &y)!=2)
        {
            /* invalid format! */
            IGX_ERROR("invalid init stone position %c%c", *(p+1), *(p+2));
            return 0;
        }
        else if (type==0 && sscanf(p, "[%c%c:%c]", &x,&y,&ch)!=3)
        {
            /* invalid format! */
            IGX_ERROR("invalid init stone position %c%c", *(p+1), *(p+2));
        }
        label.x = x-'a';
        label.y = y-'a';
        label.ch=ch;
        
        /* エラーチェック*/
        if (label.x>=goban_size ||
                label.y>=goban_size)
        {
            IGX_ERROR("invalid stone position %d,%d", label.x, label.y);
            return 0;
        }

        ++label_count;
        if (node->data.label==0)
        {
            node->data.label= new(m_mem_stone_page) ItxLinkedList<label_t>(4);
            IGX_ASSERT(node->data.label);
        }

        node->data.label->Add(label);
        p+=4;
        if (*p=='\r')
        {
            ++p;
        }
        if (*p=='\n')
        {
            ++p;
        }
    }/* while */

    if (label_count==0)
    {
        IGX_ERROR("%c%c[] label count is zeor!", *_p, *(_p+1));
        return 0;
    }
    return p;
} /* sgf::parse_label */
#if PRINT_TO_DIRECT==1
#   define OUT_PRINT(...) sprintf(p, __VA_ARGS__)
#else
#   define OUT_PRINT(...) dprintf(fd, __VA_ARGS__)
#endif

#if PRINT_TO_DIRECT==1
char* sgf::export_body(char* buffer, const stone_node_t* _node) const
{
    stone_node_t* node=(stone_node_t*)_node;
    char* p = buffer;
#else
bit32_t sgf::export_body(int fd, stone_node_t* node)
{
    bit32_t p=0;
#endif
    if (node==(stone_node_t*)&m_top)
    {
        node=node->next;
    }
    for (;node;node=node->next)
    {
        /** branches */    
        if (node->data.stone.stone==0)
        {
            IGX_ASSERT(node->branches);
            /** branch **/
            for (ItxNode<stone_node_t*>* i = node->branches->GetTopFrom(); i; i=i->next)
            {
                p+=OUT_PRINT("(");
                p=export_body(p, **i);
            }
    
            return p;
        }
        /** B[], W[] */
        const char stone_ch = (node->data.stone.type==stone::kBlack)?'B':'W';
        p+=OUT_PRINT(";%c[%c%c]", stone_ch, node->data.stone.x+'a', node->data.stone.y+'a');
        /** BL[], WL[] */
        if (node->data.remain_time!=0.0)
        {
            p+=OUT_PRINT("%cL[%.3f]", stone_ch, node->data.remain_time);
        }
        /** LB, */
        if (node->data.label)
        {
            ItxLinkedList<label_t>* list = node->data.label;
            for (ItxNode<label_t>* node = list->GetTopFrom(); node; node=node->next)
            {
                const label_t label = **node;
                const char* str = libs::get_label_string((go::label::type_t)label.type);
                if (str==0)
                {
                    p+=OUT_PRINT("LB[%c%c:%c]", (char)label.x+'a', (char)label.y+'a', label.ch);
                }
                else
                {
                    p+=OUT_PRINT("%s[%c%c]", str, (char)label.x+'a', (char)label.y+'a');
                }
            }
        }
        /** AB,AW */
        if (node->data.initial_stone_list!=0)
        {
            ItxLinkedList<stone_t>* stones= node->data.initial_stone_list;
            for (ItxNode<stone_t>* node=stones->GetTopFrom(); node; node=node->next)
            {
                const stone_t stone = **node;
                const char ch = (stone.type==stone::kBlack)?'B':'W';
                p+=OUT_PRINT("%c[%c%c]", ch, (char)stone.x+'a', (char)stone.y+'a');
            }
        }
        /** C **/
        if (node->data.comment!=0)
        {
           p+=OUT_PRINT("C[%s]", node->data.comment);
        }
#ifdef DEBUG
        *p=0;
#endif

    } /** for **/

    p+=OUT_PRINT(")");
    *p=0;

    return p;
}


} /** namespace go */

