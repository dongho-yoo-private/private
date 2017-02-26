/**
* Copyright (c) 2015 igotomo.
*
* Authorized by dongho.yoo.
*/
#include "io/igx_encrypt.h"
#include "debug/igx_debug.h"
#include "library/igx_string.h"
#include <stdio.h>
#include <string.h>


const char*  igx_mime_get_default_local_charset(int locale)
{
    return "utf-8";
}
char* igx_mime_decrypt_base64(char* buffer, size_t* buffer_size, const char* mime, size_t mime_size)
{
    char* p = (char*)mime;
    char charset[32];
    size_t charset_len;

    if (*p=='\"')
    {
        ++p;
    }
    if (p[mime_size-1]=='\"')
    {
        --mime_size;
    }

    if (p[mime_size-1]!='='||p[mime_size-2]!='=')
    {
        return 0;
    }
    mime_size-=2;

    /** mime 形式でない **/
    if ((*p=='='&&*(p+1)=='?')==false)
    {
        return 0;
    }
    if (sscanf(p, "=?%s?", charset)!=1)
    {
        return 0;
    }
    charset_len=strlen(charset);
    p+=(charset_len+3); /* charset_len + '=?' + '?' */
    if (*p!='B' || *p!='Q')
    {
        return 0;
    }

    /* base64 */
    if (*p=='B')
    {
        ++p;++p;
        return igx_decrypt_base64(buffer, buffer_size, p, mime_size-(p-mime));
    }

    /* quoted-printable */
    /* TODO. */

    return 0;
}
char* igx_mime_encrypt_base64(char* buffer, size_t* buffer_size, const char* text, const size_t text_len, const char* charset)
{
    size_t b64_buffer_size = *buffer_size;
    char* p = buffer;
    p += sprintf(p, "=?%s?B?", charset);

    if (igx_encrypt_base64(p, &b64_buffer_size, text, text_len)==0)
    {
        IGX_ERROR("igx_encrypt_mime_base64 encode error! %s", text);
        return 0;
    }
    p+=b64_buffer_size;
    *p='?',++p;
    *p='=',++p;
    *p=0;

    *buffer_size = (p-buffer);
    return buffer;
}
bool_t igx_is_mime_format(const char* text, size_t len)
{
    char* p = (char*)text;
    if (*p=='\"')
    {
        ++p;
        --len;
    }
    if (*p!='='||*(p+1)!='?')
    {
        return false;
    }

    if (p[len-1]!='='||p[len-2]!='?')
    {
        return false;
    }

    return true;
}
/**
  case1. =?charset?B?string?= <email>
  case2. =?charser?Q?string?= <email>
  case3. "displayname" <email>
  case4. <email>
  case5. email 
*/
bool_t igx_mime_get_name_and(const char* email_address, size_t email_len, igx_string_t* display, igx_string_t* email)
{
    char* p = (char*)email_address;
    char* space = igx_str_ch(p, ' ');

    if (space==0)
    {
        /* emailしか入ってない場合 */
        display->str=0; 
        display->length=0;
        email->str=p;
        email->length=email_len;
    }
    else
    {
        email->str = ++space;
        email->length = strlen(email->str);
        display->str=p;
        display->length=(space-p);
    }
    if (*email->str=='<')
    {
        ++email->str;
        email->length-=2;
    }

    return true;
}
bool_t igx_mime_is_7bit(const char* text, size_t len)
{
    int i;
    char* p = (char*)text;
    for (i=0; i<len; ++i)
    {
        if ((p[i]&0x80)!=0)
        {
            return false;
        }
    }
    return true;
}
size_t igx_mime_user(char* buffer, size_t buffer_size, const igx_string_t* name, const igx_string_t* email, const char* charset)
{
    bit32_t n=0;

    if (name==0)
    {
        goto EMAIL;
    }

    if (igx_mime_is_7bit(name->str, name->length)==true)
    {
        memcpy(buffer+n, name->str, name->length);
        n+=name->length;
    }
    else
    {
        size_t _buffer_size = buffer_size-n-1;
        igx_mime_encrypt_base64(buffer+n, &_buffer_size, name->str, name->length, charset);
        n+=_buffer_size;
    }

    buffer[n++]=' ';
EMAIL:
    buffer[n++]='<';
    memcpy(buffer+n, email->str, email->length);
    n+=email->length;
    buffer[n++]='>';
    buffer[n++]='\0';
    return n;
}

