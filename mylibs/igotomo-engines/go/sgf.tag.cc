#include "sgf.h"
#include "data_struct/itx_queue.h"
#include <stdio.h>

namespace go {

namespace tag {

#define GO_TAG_MALLOC(size) igx_malloc(size, "go::tag::");

/** for B,W */
char* get_stone(const char* _p, stone_t& stone, ubit32_t order)
{
    char* p = (char*)_p;
    /** クラッシュしないように！ **/
    if (*p==0&&*(p+1)==0&&*(p+2)==0)
    {
        IGX_WARN("pointer is null !");
        return 0;
    }
    if (*p=='B'||*p=='W')
    {
        if (*(p+1)=='[')
        {
            stone.stone=0;
            char x,y;

            stone.type = (*p=='B')?stone::kBlack:stone::kWhite;
            if (*(p+2)==']')
            {
                /* PASS */
                stone.is_pass=1;
                stone.order = order;
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
                stone.is_pass=1;
                stone.order = order;
                IGX_INFO("pass!!", p+2);
                return p+5;
            }

            stone.x    = x-'a';
            stone.y    = y-'a';
            stone.order = order;
            return p+5;
        }
    }

    return 0;
}
/** for LB,TR,RT,MA,CR **/
char* get_label(const char* _p, ItxLinkedList<label_t>** label_list, IgxMemPage* page)
{
    char* p = (char*)_p;
    char x, y, ch=0;
    label_t label;
    go::label::type_t type;

    for (int i=0; i<6; ++i)
    {
        if (p[i]==0)
        {
            return 0;
        }
    }
    if (*p=='L'&&*(p+1)=='B'&&*(p+2)=='[')
    {
        /* label */
        if (sscanf(p+3, "%c%c:", &x, &y)==2)
        {
            type=label::kNone;
            if (sscanf(p+5, ":%c]", &ch)!=1)
            {
                /**invalid format**/
                return p+3;
            }
            /** LB[dd:A] */
            p+=8;
        }
        else
        {
            /**invalid format**/
            return p+3;
        }
    }
    else if ((*p='C')&&(*(p+1)=='R')&&(*(p+2)=='['))
    {
        /* circle */ 
        type = label::kCircle;
    }
    else if ((*p='T')&&(*(p+1)=='R')&&(*(p+2)=='['))
    {
        /* triangle */ 
        type = label::kTriAngle;
    }
    else if ((*p='S')&&(*(p+1)=='Q')&&(*(p+2)=='['))
    {
        /* rectangle */ 
        type = label::kRectangle;
    }
    else if ((*p='M')&&(*(p+1)=='A')&&(*(p+2)=='['))
    {
        /* daiamond */ 
        type = label::kDiamond;
    }
    else
    {
        return 0;
    }

    if (type!=0)
    {
        if (sscanf(p+3, "%c%c]", &x, &y)!=2)
        {
            /** invalid format */
            return p+3;
        }
        p+=6;
    }
    label.label = 0;
    label.type = type;
    label.x=x;
    label.y=y;
    label.ch=ch;

    if (*label_list==0)
    {
        if (page!=0)
        {
            *label_list = new(page) ItxLinkedList<label_t>(4);
        }
        else
        {
            *label_list = IGX_NEW("sgf.tag.label") ItxLinkedList<label_t>(4);
        }
    }
    (*label_list)->Add(label);

    return p;
}
/** for C */
char* get_comment(const char* _p, char** comment, IgxMemPage* page)
{
    char* p = (char*)_p;
    if (p[0]==0||p[1]==0)
    {
        return p+1;
    }
    if (*p=='C'&&*p=='[')
    {
        IGX_DEBUG("%s", p);
        getchar();
        size_t len=0;
        for (;*p;++len, ++p)
        {
            if (*p==']'&&*(p-1)!='\\')
            {
                break;
            }
        }
        if (*p==0)
        {
            return p;
        }
        memcpy(*comment = (char*)pmalloc(page, len+1), p, len);
        *comment[len]=0;
        IGX_INFO("comment:%s", comment);
    }
    return p;
}
/** for AB,AW */
char* get_initstone(const char* _p, ItxLinkedList<stone_t>** init_stone_list, IgxMemPage* page)
{
    char* p = (char*)_p;
    char x, y;
    stone_t stone;

    for (int i=0;i<6; ++i)
    {
        if (p[i]==0)
        {
            return p+i;
        }
    }

    if (*p=='A')
    {
        if (*(p+1)=='B' || *(p+1)=='W')
        {
NEXT_PARSE:
            if (sscanf(p+2, "[%c%c]", &x, &y)!=2)
            {
                /** invalid format !? */
                return p+2;
            }
            stone.type = *(p+1)=='B'? stone::kBlack:stone::kWhite;
            stone.x=x;
            stone.y=y;
            p+=6;
        }
        else
        {
            return 0;
        }
    }
    else
    {
       return 0;
    }

    if (init_stone_list==0)
    {
        if (page!=0)
        {
            /** 初期値をどれくらいにすべきかな。。。**/
            *init_stone_list = new(page) ItxLinkedList<stone_t>(16);
        }
        else
        {
            *init_stone_list = IGX_NEW("go::tag::get_initstone") ItxLinkedList<stone_t>(16);
        }
    }
    (*init_stone_list)->Add(stone);

    /** 連続して石がある場合 **/
    if (*p=='[')
    {
        p-=2;
        goto NEXT_PARSE;
    }
    return p;
}
/** for BL,WL,OB,OW **/
char* get_time(const char* sgf, float& remain_time)
{
    char* p = (char*)sgf;
    
    if ((*p=='B'||*p=='W')&&(*(p+1)=='L')&&*(p+2)=='[')
    {
        if (sscanf(p+3, "%f]", &remain_time)!=1)
        {
            return p+4;
        }
        size_t n = igx_strlen_until_c(p+2, ']');
        return p+2+n;
    }
    return 0;
}

}} /* namespace tag */


