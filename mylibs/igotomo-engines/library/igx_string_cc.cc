/**
 * Copyright 2015 (c) 
 * authorized by dongho.yoo.
 * 
 * @brief 文字列を扱うIgxStringクラス
*/
#include "igx_string.h"
#include <stdio.h>
#include <string.h>

IgxString::IgxString()
{
    init_member();
}
IgxString::IgxString(const char* p)
{
    init_member();
    Set(p, strlen(p));
}
IgxString::IgxString(IgxMemPage* mem_page, const char* p)
{
    init_member();
    m_mem_page=mem_page;
    Copy(p, strlen(p));
}
size_t IgxString::Copy(const char* str, size_t len)
{
    len = (len==0)?strlen(str):len;

    if (m_buffer_size>len)
    {
        memcpy(m_str.str, str, len+1);
        return len;
    }

    m_flags.is_allocated=1;
    if (m_str.str!=0)
    {
        if (m_mem_page==0)
        {
            IGX_FREE(m_str.str);
        }
    }
    m_str.str = strdup(str, len);
    return len;
}
bit32_t IgxString::CopyWithFormat(const char* format, ...)
{
	va_list args;
    bit32_t ret;

    va_start(args, format);
    if (m_mem_page!=0)
    {
        ret = copy_with_format_h(0, 0, format, args);
        va_end(args);
        return ret;
    }
    ret = copy_with_format(0, 0, format, args);
    va_end(args);
    return ret;
}
size_t IgxString::Add(const char* string, size_t len)
{
    return strcat(string, len);
}
bit32_t IgxString::AddWithFormat(const char* format, ...)
{
	va_list args;
    bit32_t ret;

    va_start(args, format);
    if (m_mem_page!=0)
    {
        ret = copy_with_format_h(0, 1, format, args);
        va_end(args);
        return ret;
    }
    ret = copy_with_format(0, 1, format, args);
    va_end(args);
    return ret;
}
size_t IgxString::Insert(size_t index, const char* string, size_t len/*=0*/)
{
    return strinsert(index, string, len);
}
bit32_t IgxString::InsertWithFormat(size_t index, const char* format, ...)
{
	va_list args;
    bit32_t ret;

    va_start(args, format);
    if (m_mem_page!=0)
    {
        ret = copy_with_format_h(index, 2, format, args);
        va_end(args);
        return ret;
    }
    ret = copy_with_format(index, 2, format, args);
    va_end(args);
    return ret;
}
size_t IgxString::Set(const char* str, size_t len) const
{
    m_buffer_size=0;
    m_str.str=(char*)str;
    m_str.length=(len==0)?strlen(str):len;
    return m_str.length;
}
bit32_t IgxString::Find(const char* str, bit32_t index/*=0*/)
{
    char* (*find[2])(const char* , const char*) = {&::strstr, &::strcasestr};
    char* ret = find[m_flags.ignore_case](m_str.str, str);

    return (ret==0)?-1:(ret-str);
}
bit32_t IgxString::Find(const char* string, bit32_t index/*=0*/) const
{
    return ((IgxString*)this)->Find(string, index);
}
bit32_t IgxString::Find(const char ch, bit32_t index/*=0*/)
{
    const size_t count = m_str.length;
    for (bit32_t i=0; i<count; ++i)
    {
        if (m_str.str[i]==ch)
        {
            return i;
        }
    }
    return -1;
}
bit32_t IgxString::Find(const char ch, bit32_t index/*=0*/) const
{
    return ((IgxString*)this)->Find(ch, index);
}
/** TODO. 大文字小文字対応。。。。**/
bit32_t IgxString::FindFromTail(const char* str, bit32_t index)
{
    const size_t key_len = strlen(str), len = m_str.length;
    char* key = (char*)(str+key_len);
    char* p   = m_str.str+len, *p_key=(char*)key;
    const char first_pattern = *(str+key_len-1);

    for (bit32_t i=len-index-1; i>=0; --i)
    {
        char x_ch=0;
        bit32_t x_index=0, cnt=0;
        bool_t is_found=true;
        for (bit32_t j=key_len-1; j>=0; --j, ++cnt)
        {
            if (p_key[j]!=p[i-cnt])
            {
                x_ch=p[i-cnt];
                x_index=i-cnt;
                is_found=false;
                break;
            }
        }
        if (is_found==true)
        {
            return i;
        }
        if (x_ch==first_pattern)
        {
            i=x_index;
        }
    }
    return -1;
}
bit32_t IgxString::FindFromTail(const char* str, bit32_t index) const
{
    return ((IgxString*)this)->FindFromTail(str, index);
}
bit32_t IgxString::FindFromTail(char ch, bit32_t index)
{
    const size_t len = m_str.length;
    char* p   = m_str.str+len;
    for (bit32_t i=len-index-1; i>=0; --i)
    {
        if (p[i]==ch)
        {
            return i;
        }
    }
    return -1;
}
bit32_t IgxString::FindFromTail(char ch, bit32_t index) const
{
    return ((IgxString*)this)->FindFromTail(ch, index);
}
bool_t IgxString::BeginWith(const char* word, size_t word_len, bit32_t index/*=0*/)
{
    const size_t len = word_len==0?strlen(word):word_len; 
    if (m_str.length<len)
    {
        return false;
    }

    if (m_flags.ignore_case==true)
    {
        return strncasecmp(m_str.str, word, len)==0;
    }
    return memcmp(m_str.str, word, len)==0;
}
bool_t IgxString::BeginWith(const char* word, size_t word_len, bit32_t index/*=0*/) const
{
    return ((IgxString*)this)->BeginWith(word, word_len, index);
}
bool_t IgxString::EndWith(const char* word, size_t word_len/*=0*/, bit32_t index/*=0*/)
{
    const size_t len = word_len==0?strlen(word):word_len; 
    if (m_str.length<len)
    {
        return false;
    }
    return BeginWith(word, len, (bit32_t)(m_str.length-len));
}
bool_t IgxString::EndWith(const char* word, size_t word_len/*=0*/, bit32_t index/*=0*/) const
{
    return ((IgxString*)this)->EndWith(word, word_len, index);
}
igx_string_t* IgxString::SubString(igx_string_t& str, bit32_t s, bit32_t e) const
{
    str.str=m_str.str+s;
    str.length = e-s;
    return &str;
}
igx_string_t* IgxString::SubString(igx_string_t& str, bit32_t s, bit32_t e)
{
    return ((const IgxString*)this)->SubString(str, s, e);
}
bit32_t IgxString::ParseWithFormat(const char* format, ...)
{
    va_list args;
    bit32_t ret;
    va_start(args, format);
    ret = ((IgxString*)this)->ParseWithFormat(0, format,args);
    return ret;
}
bit32_t IgxString::ParseWithFormat(const char* format, ...) const
{
    va_list args;
    bit32_t ret;
    va_start(args, format);
    ret = ((IgxString*)this)->ParseWithFormat(0, format,args);
    return ret;
}
bit32_t IgxString::ParseWithFormat(bit32_t index, const char* format, ...)
{
    va_list args;
    bit32_t ret;
    va_start(args, format);
    ret = sscanf(m_str.str+index, format, args);
    va_end(args);

    return ret;
}
bit32_t IgxString::ParseWithFormat(bit32_t index, const char* format, ...) const
{
    va_list args;
    bit32_t ret;
    va_start(args, format);
    ret = ((IgxString*)this)->ParseWithFormat(index, format,args);
    return ret;
}
bit32_t IgxString::Replace(const char* src, const char* rep)
{
    /** TODO **/
#if 0
    char* p = (char*)m_str.str;
    const size_t src_len = strlen(src);
    const size_t rep_len = strlen(rep);
    bit32_t count=0;
    size_t new_size = m_str.length;
    ItxQueueBuffer<char*, 4096> queue;

    while(p = strstr(p, src) queue.EnQueue(p), ++count, p+=src_len;

    if (count==0)
    {
        /* 何も変換されていない*/
        return 0;
    }

    if (src_len==rep_len)
    {
        return ;
    }
    if (src_len<rep_len)
    {
        new_size += (rep_len-src_len)*count;
        p = str_malloc(new_size);
    }
#endif
    return 0;
}
char* IgxString::str_malloc(size_t len)
{
    return (m_mem_page!=0)?(char*)pmalloc(m_mem_page, len+1):(char*)IGX_MALLOC(len+1, "IgxString");
}
void IgxString::str_free()
{
    if (m_mem_page==0)
    {
        if (m_str.str!=0)
        {
            IGX_FREE(m_str.str);
        }
    }
    else
    {
        pfree(m_mem_page, m_str.str);
    }
}
char* IgxString::strdup(const char* str, size_t len)
{
    char* ret = (char*)memcpy(str_malloc(len), str, len+1);
    m_buffer_size=len+1;
    return ret;
}
size_t IgxString::strcat(const char* str, size_t len)
{
    const size_t new_size = m_str.length+len;
    if (m_buffer_size>new_size)
    {
        memcpy(m_str.str+m_str.length, str, len+1);
        m_str.length=new_size;
        return new_size;
    }
    char* temp = (char*)memcpy(str_malloc(new_size), m_str.str, m_str.length);
    memcpy(temp+m_str.length, str, len+1);

    str_free();
    m_str.str=temp;
    m_str.length=new_size;
    m_buffer_size=new_size+1;

    return new_size;
}
size_t IgxString::strinsert(bit32_t index, const char* str, size_t len)
{
    const size_t new_size = m_str.length+len;

RETRY:
    if (m_buffer_size>new_size)
    {
        memmove(m_str.str+index+len, m_str.str+index, m_str.length-index+1);
        memcpy(m_str.str+index, str, len);
        m_str.length=new_size;
        return new_size;
    }
    else
    {
        char* temp = str_malloc(new_size);
        memcpy(temp, m_str.str, m_str.length);
        m_str.length=new_size;
        m_str.str=temp;
        m_buffer_size = new_size+1;
        goto RETRY;
    }

    return 0;

}
void  IgxString::init_member()
{
    memset(&m_str, 0, sizeof(m_str));
    memset(&m_flags, 0, sizeof(m_flags));
    m_buffer_size=0;
    m_working_buffer=0;
    m_flags.ignore_case=1;
}
bit32_t IgxString::copy_with_format_h(bit32_t index, bit32_t type, const char* format, va_list ap)
{
    if (m_working_buffer==0)
    {
        m_working_buffer = (char*)m_mem_page->Pop(IGX_STRING_MAX_FORMAT_STR_BUFFER_LEN);
    }
    char* buffer=m_working_buffer;
    const size_t len = IGX_STRING_MAX_FORMAT_STR_BUFFER_LEN-1;
    bit32_t n = snprintf(buffer, len, format, ap);

    if (n==len)
    {
        buffer[len]=0;
    }

    if (type==0)
    {
        return Copy(buffer, n);
    }
    else if (type==1)
    {
        return strcat(buffer, n);
    }
    else
    {
        return strinsert(index, buffer, n);
    }
    return 0;
}
bit32_t IgxString::copy_with_format(bit32_t index, bit32_t type, const char* format, va_list ap)
{
    char buffer[IGX_STRING_MAX_FORMAT_STR_BUFFER_LEN];
    const size_t len = sizeof(buffer)-1;
    bit32_t n = snprintf(buffer, len, format, ap);

    if (n==len)
    {
        buffer[len]=0;
    }

    if (type==0)
    {
        return Copy(buffer, n);
    }
    else if (type==1)
    {
        return strcat(buffer, n);
    }
    else
    {
        return strinsert(index, buffer, n);
    }
    return 0;
}
