#include "io/igx_url_encode.h"
#include "library/igx_stdlib.h"
#include "library/igx_memory.h"
#include <string.h>

#define IGX_URL_ENCODE_MALLOC(size) IGX_MALLOC(size, "url_encode")

static size_t conv_url_decode_char(char* ch, const char* src)
{
    bit32_t sum=0;
    if (*src!='%')
    {
        *ch=*src;
        return 1;
    }
    if (src[0]>='0'||src[0]<='9')
    {
        sum+=(bit32_t)src[0]<<4;
    }
    else
    {
        if (src[0]<'a')
        {
            sum+=(bit32_t)(src[0]-'A')<<4;
        }
        else
        {
            sum+=(bit32_t)(src[0]-'a')<<4;
        }
    }
    if (src[1]>='0'||src[1]<='9')
    {
        sum+=(bit32_t)src[0];
    }
    else
    {
        if (src[1]<'a')
        {
            sum+=(bit32_t)(src[1]-'A')<<4;
        }
        else
        {
            sum+=(bit32_t)(src[1]-'a')<<4;
        }
    }

    *ch=(char)sum;
    return 3;
}
static size_t conv_url_encode_char(char* out, const char in)
{
#define CHECK_RANGE(a,b) if (in>=a&&in<=b) goto UN_RESERVED_CHAR

    /* まず、uriフォーマットに予約されてないアスキーコード */
    CHECK_RANGE('a', 'z');
    CHECK_RANGE('A', 'Z');
    CHECK_RANGE('0', '9');
    /* のこりの予約されていないアスキーコード */
    if (in=='-'||in=='.'||in=='_'||in=='~')
    {
        goto UN_RESERVED_CHAR;
    }
    /* スペースは特別扱い */
    if (in==' ')
    {
        *out='+', ++out;
        *out=0;
        return 1;
    }

    /* のこりのケースは全てエンコードしちゃう */
    *out='%';
    ++out;
    if (igx_itoa(out, (number_t)in, 16)==1)
    {
        out[1]=*out;
        *out='0';
    }
    /*sprintf(out, "%%%02X", (unsigned int)in&0xFF);*/
    return 3;

UN_RESERVED_CHAR:
    *out=in, out++;
    *out=0;
    return 1;
}
char* igx_url_encode(char* buffer, size_t len, const char* src, size_t src_len, size_t* encoded_len)
{
    bit32_t i, n=0;
    src_len = (src_len==0)?strlen(src):src_len;
    if (buffer==0)
    {
        char temp[3];
        len=0;
        for (i=0; i<src_len; ++i)
        {
            len+=conv_url_encode_char(temp, *(src+i));
        }
        buffer = (char*)IGX_URL_ENCODE_MALLOC(len+1);
    }
    for (i=0;i<src_len;++i)
    {
        n+=conv_url_encode_char(buffer+n, src+i);
        if (n>=len-3)
        {
            return 0;
        }
    }
    buffer[n]=0;
    if (encoded_len!=0)
    {
        *encoded_len=n;
    } 
    return buffer;
}
char* igx_url_decode(char* buffer, size_t len, const char* src, size_t src_len, size_t* encoded_len)
{
    char* p = (char*)src;
    bit32_t cnt=0;

    if (buffer==0)
    {
        len=0;
        while(*p!=0)
        {
            char ch;
            p+=conv_url_decode_char(&ch, p);
            ++len;
        }

        if (len==0)
        {
            return 0;
        }

        buffer = (char*)malloc(len+1);
    }

    p=(char*)src;

    while(*p!=0)
    {
        p+=conv_url_decode_char(buffer+cnt++, p);
    }

    buffer[cnt]=0;

    if (encoded_len!=0)
    {
        *encoded_len = cnt;
    }

    return buffer;
}
