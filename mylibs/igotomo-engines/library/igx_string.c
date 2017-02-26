#include "igx_string.h"
#include "igx_memory.h"
#include "igx_mem_page.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <iconv.h>
#include <iconv.h>
#include <errno.h>

#define IGX_STRING_MALLOC(size, comment) IGX_MALLOC(size, comment)
#define IGX_STRING_FREE IGX_FREE 

#define IS_ALPHABET(a) IGX_STR_IS_ALPHABET(a)
#define DIFF ('a'-'A')
#define UPPER_CASE(a) ((a<'a')?a:(a-DIFF))

char* igx_strcpy_until(char* dest, const char* src, const char* until)
{
    char* p = strstr(src, until);
    
    if (p!=0)
    {
        const size_t len = p-src;
        memcpy(dest, src, len);
        dest[len]=0;
        return dest;
    }
    return strcpy(dest, src);
}
static char* find_char(const char* src, char ch)
{
    char* p = (char*)src;
    while((*p!=0)&&(*p!=ch)) ++p;

    return (*p==0)?0:p;
}
static char* find_case_char(const char* src, char ch)
{
    char* p = (char*)src;
    char ch2 = IS_ALPHABET(ch)?ch:0;
    
    if (ch2==0)
    {
        while((*p!=0)&&(*p!=ch)) ++p;
        return (*p==0)?0:p;
    }

    ch2=(ch2<'a')?(ch2+DIFF):(ch2-DIFF);
    while((*p!=0)&&((*p!=ch)&&(*p!=ch2))) ++p;
    return (*p==0)?0:p;
}
char* igx_strcpy_until_c(char* dest, const char* src, char until)
{
    char* p = find_char(src, until);
    
    if (p!=0)
    {
        const size_t len = p-src;
        memcpy(dest, src, len);
        dest[len]=0;
        return dest;
    }
    return strcpy(dest, src);
}
size_t igx_strlen_until(const char* src, const char* until)
{
    char* p = strstr(src, until);
    return (size_t)(p==0)?strlen(src):(p-src);
}
size_t igx_strlen_until_c(const char* src, char until)
{
    char* p = find_char(src, until);
    return (size_t)(p==0)?strlen(src):(p-src);
}
igx_string_t* igx_get_between_s(igx_string_t* out, const char* src, const char* start, const char* end)
{
    const size_t start_len = strlen(start);
    const size_t end_len   = strlen(end);
    bit32_t start_count=0;
    char* p_start = strstr(src, start);
    char* p_end   = strstr(p_start+start_len, end);
    char* p_start_next;
    bit32_t i;
    size_t len;

    if (p_start==0 ||
            p_end==0)
    {
        out->str=0;
        out->length=0;
        return 0;
    }

    p_start_next = p_start+start_len;
    while((p_start_next=strstr(p_start_next, start))!=0 &&
                p_start_next<p_end) p_start_next+=start_len, ++start_count;

    for (i=0; i<start_count; ++i)
    {
        p_end = strstr(p_end+end_len, end);

        if (p_end==0)
        {
            return 0;
        }
    }

    len = p_end-(p_start+start_len);
    if (out==0)
    {
        out = (igx_string_t*)IGX_STRING_MALLOC(len+1, "igx_get_between_s");
    }
    memcpy(out->str, p_start+start_len, len);
    out->length=len;
    out->length = len;
    out->str[len]=0;
    out->length=len;

    return out;
}
igx_string_t* igx_get_between_c(igx_string_t* out, const char* src, char start, char end)
{
    bit32_t start_count=0, i;
    char* p_start = find_char(src, start);
    char* p_end   = find_char(p_start+1, end);
    char* p_start_next;
    size_t len;

    if (p_start==0 ||
            p_end==0)
    {
        return 0;
    }

    p_start_next = p_start+1;
    while((p_start_next=find_char(p_start_next, start))!=0 &&
                p_start_next<p_end) ++p_start_next, ++start_count;

    for (i=0; i<start_count; ++i)
    {
        p_end = find_char(p_end+1, end);

        if (p_end==0)
        {
            return 0;
        }
    }

    len = p_end-(p_start+1);
    if (out==0)
    {
        out = (igx_string_t*)IGX_STRING_MALLOC(len+1, "igx_get_between_c");
    }
    memcpy(out->str, p_start+1, len);
    out->str[len]=0;
    out->length=len;

    return out;
}
// --------------- utf8のための文字数テーブル
const static struct {
    unsigned char start_code;
    unsigned char end_code;
    unsigned char reserved[2];
} code[] = {
    {0x00, 0x7f, {0, 0}}, // 1文字
    {0xc2, 0xdf, {0, 0}}, // 2文字
    {0xe0, 0xef, {0, 0}}, // 3文字
    {0xf0, 0xf7, {0, 0}}, // 4文字
    {0xf8, 0xfb, {0, 0}}, // 5文字
    {0xfc, 0xfd, {0, 0}}  // 6文字
};
static const bit32_t code_count = sizeof(code)/sizeof(code[0]);
size_t igx_get_utf8_char_count(const char* src)
{
    unsigned char* p = (unsigned char*)src;
    ubit32_t cnt = 0, i;

    while(*p)
    {
        bool_t is_code_valid=false;
        
        // ループ使わずに展開した方が早いが。。。
        for (i=0; i<code_count; ++i)
        {
            if (*p>=code[i].start_code&& *p<=code[i].end_code)
            {
                cnt+=i+1;
                p+=i+1;
                is_code_valid=true;
                break;
            }
        }

        if (is_code_valid==false)
        {
            return 0;
        }
    }
    return cnt;    
}
size_t igx_get_utf8_size_in_buffer(const char* src, size_t buffer_size, bool_t* is_complete)
{
    unsigned char* p = (unsigned char*)src;
    ubit32_t cnt = 0, i, last_size=0;
    *is_complete=false;

    while(*p || cnt<buffer_size)
    {
        bool_t is_code_valid=false;
        
        for (i=0; i<code_count; ++i)
        {
            if (*p>=code[i].start_code&& *p<=code[i].end_code)
            {
                last_size=i+1;
                cnt+=last_size;
                p+=last_size;
                is_code_valid=true;
                break;
            }
        }

        if (is_code_valid==false)
        {
            return 0;
        }
    }

    if (*p==0)
    {
        *is_complete=true;
    }
    return (cnt-last_size);
}
static size_t get_utf_char_len(char c)
{
    size_t i;
    for (i=0; i<code_count; ++i)
    {
        if (c>=code[i].start_code&& c<=code[i].end_code)
        {
            return i+1;
        }
    } /* for */
    
    // UTF-8でない？
    return 0;
}
char* igx_get_utf8_next_char(char* dest, const char* src)
{
    const size_t len = get_utf_char_len(*src); 
    memcpy(dest, src, len);
    dest[len]=0;
    return (char*)src+len;
}
igx_string_t* igx_get_next_line(igx_string_t* dest, const char* src)
{
    char* p = (char*)src;

    dest->str=0;
    dest->length=0;
    while(*p!=0&&*p!='\n') ++p;

    if (*p==0)
    {
        /* 次の行がない。*/
        dest->str=(char*)src;
        dest->length=p-src;
        return 0;
    }
    if (p[-1]=='\r')
    {
        dest->length=(p-src)-2;
    }
    else
    {
        dest->length=p-src-1;
    }
    dest->str=++p;
    return (igx_string_t*)src;
}
void igx_string_free(char* p)
{
    IGX_STRING_FREE(p);
}
int igx_stricmp(const char* s1, const char* s2)
{
    const size_t len = strlen(s1);
    int i;
    if (len!=strlen(s2))
    {
        return -1;
    }

    for (i=0;i<len;++i)
    {
        if (s1[i]==s2[i])
        {
            continue;
        }
        if (IS_ALPHABET(s1[i])==true && (UPPER_CASE(s1[i])==UPPER_CASE(s2[i])))
        {
            continue;
        }

        return s2[i]-s1[i];
    }

    return 0;
}
bit32_t igx_str_count_ch(const char* src, size_t src_size, char ch)
{
    bit32_t cnt=0;
    src_size=(src_size==0)?strlen(src):src_size;
    for (bit32_t i=0; i<src_size; ++i)
    {
        if (src[i]==ch)
        {
            ++cnt;
        }
    }
    return cnt;
}
bit32_t igx_str_remove_ch(char* src, char ch)
{
    size_t ret=0;
    bit32_t i=0;
    for (;src[i]!=0;++i)
    {
        if (src[i]==ch)
        {
            bit32_t j=i;
            for (;src[j]!=0;++j)
            {
                src[j]=src[j+1];
            }
            ++ret;
        } /* if ch */
    } /* for i */

    return ret;
}
char* igx_str_ch(const char* src, char ch)
{
    char* p = (char*)src;
    while (*p!=0&&*p!=ch) ++p;

    return (*p==0)?0:p;
}
char* igx_str_until(igx_string_t* str, const char* src, const char* until)
{
    size_t length;
    char* p = strstr(src, until);
    if (p==0)
    {
        return 0; 
    }
    str->length=p-str->str;
    str->str=p;

    return p;
}
char* igx_str_case_until(igx_string_t* str, const char* src, const char* until)
{
    size_t length;
    char* p = strcasestr(src, until);
    if (p==0)
    {
        return 0; 
    }
    str->length=p-str->str;
    str->str=p;

    return p;
}
char* igx_str_until_c(igx_string_t* str, const char* src, const char until)
{
    char* p = find_char(src, until);
    
    if (p==0)
    {
        return 0;
    }

    str->length=p-str->str;
    str->str=p;
    return p;
}
char* igx_str_case_until_c(igx_string_t* str, const char* src, const char until)
{
    char* p = find_case_char(src, until);
    if (p==0)
    {
        return 0;
    }

    str->length=p-str->str;
    str->str=p;
    return p;
}
void igx_str_set_hash(igx_string_t* str)
{
    /* TODO */
}
igx_string_t* igx_str_load(const char* p, HITX mem_page)
{
    size_t len = strlen(p);
    igx_string_t* ret;
    if (mem_page!=0)
    {
        ret = (igx_string_t*)pmalloc_c(mem_page, sizeof(igx_string_t)+len+1);
    }
    else
    {
        ret = (igx_string_t*)IGX_STRING_MALLOC(sizeof(igx_string_t)+len+1, "igx_str_load");
    }
    memcpy(ret+1, p, len+1);
    ret->str=(char*)(ret+1);
    return ret;
}
void igx_str_unload(igx_string_t* str, HITX mem_page)
{
    if (mem_page==0)
    {
        IGX_STRING_FREE(str);
    }
}
bit32_t igx_strcmp(igx_string_t* str1, igx_string_t* str2)
{
    if (str1->hash_key!=str2->hash_key)
    {
        return -1;
    }
    if (str1->length!=str2->length)
    {
        return -1;
    }
    return memcmp(str1, str2, str1->length);
}
bit32_t igx_strcasecmp(igx_string_t* str1, igx_string_t* str2)
{
    if (str1->length!=str2->length)
    {
        return -1;
    }
    return strncasecmp(str1->str, str2->str, str1->length);
}

#define MAX_LOCALE_CHARSET_COUNT (13)
struct {
    const char* locale;
    const char* charset[MAX_LOCALE_CHARSET_COUNT];
} __locale_code[] = 
{
    {"ja", "EUC-JP", "SHIFT_JIS", "ISO-2022-JP", "ISO-2022-JP-2", "ISO-2022-JP-1", "",},
    {"ko", "EUC-KR", "CP949", "ISO-2022-KR", "JOHAB", "", },
    {"zh", "EUC-CN", "HZ", "CP936", "GB18030", "EUC-TW", "BIG5", "CP950", "BIG5-HKSCS", "BIG5-HKSCS:2004", "BIG5-HKSCS:2001", "BIG5-HKSCS:1999", "ISO-20220-CN", ""},
    {"vi", "VISCII", "TCVN", "CP1258", "", },
    {"th", "TIS-620", "CP874", "MacThai", "", }, 
    {0, }
};

/* europer */
#define OTHER_CHAR_SET "ISO-8859-"
const char* _other_charset[] = {
    "ISO-9959-1",
    "ISO-9959-2",
    "ISO-9959-3",
    "ISO-9959-4",
    "ISO-9959-5",
    "ISO-9959-6",
    "ISO-9959-7",
    "ISO-9959-8",
    "ISO-9959-9",
    "ISO-9959-10",
    "ISO-9959-11",
    "ISO-9959-12",
    "ISO-9959-13",
    "ISO-9959-14",
    "ISO-9959-15",
    0
};
size_t igx_strcpy_limit_length(char* buffer, size_t buffer_size, const char* src)
{
    /* TODO. */
    return 0;
}
const char* igx_get_charset(const char* locale_name, bit32_t index)
{
    bool_t found=false;
    bit32_t i;
    for (i=0;__locale_code[i].locale;++i)
    {
        if (__locale_code[i].locale[0]==locale_name[0] &&
                __locale_code[i].locale[1]==locale_name[1])
        {
            found=true;
            break;
        }
    }
    if (found==false)
    {
        return _other_charset[index];
    }
    return __locale_code[i].charset[index];
}
char* igx_convert_utf8(char** out, size_t* buffer_size, char** src, size_t* src_len, const char* charset)
{
    iconv_t ic;
    bit32_t i;
    ubit8_t charset_upper[32];
    ubit8_t* p=(ubit8_t*)charset;

    for (;*p!=0&&i<sizeof(charset_upper);++i, ++p)
    {
        charset_upper[i]=(ubit8_t)toupper((int)*p);
    }
    charset_upper[i]=0;

    ic = iconv_open("UTF-8", (char*)charset_upper);
    if (ic==(iconv_t)-1)
    {
        if (errno!=E2BIG)
        {
            *buffer_size=0;
        }
        iconv_close(ic);
        return 0;
    }

    if (iconv(ic, src, src_len, out, buffer_size)!=(iconv_t)(intptr_t)-1)
    {
        iconv_close(ic);
        return *out;
    }
    iconv_close(ic);
    return 0;
}
char* igx_strcpy(char* buffer, size_t buffer_size, const igx_string_t* igx_str)
{
    size_t s_len = (buffer_size>igx_str->length)?igx_str->length:buffer_size;
    memcpy(buffer, igx_str->str, s_len-1);
    buffer[s_len-1]=0;
    return buffer;
}
