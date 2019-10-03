#include "secure_base64.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const char* base64strings="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const size_t base64string_length=64;
static char base64code[128];
static bool_t initialized=false;

static void Init()
{
    int i,n;
    memset(base64code,0xf0,sizeof(base64code));
    for (i='A';i<='Z';++i)
    {
        base64code[i]=(char)i-'A';
    }
    n=base64code['Z']+1;
    for (i='a';i<='z';++i)
    {
        base64code[i]=(char)i-'a'+n;
    }

    n=base64code['z']+1;
    for (i='0';i<='9';++i)
    {
        base64code[i]=n+i-'0';
    }
    n=base64code['9']+1;
    base64code['+']=n;
    base64code['/']=n+1;
    base64code['=']=0;
}
static void ConvBase64TreeBytes(unsigned char* dest,const unsigned char* src)
{
    const char src1=*src,src2=src[1],src3=src[2];
    *dest=base64strings[(src1&0xfc)>>2];
    dest[1]=base64strings[((src1&0x03)<<4)|((src2&0xf0)>>4)];
    dest[2]=base64strings[((src2&0x0f)<<2)|((src3&0xc0)>>6)];
    dest[3]=base64strings[(src3&0x3f)];
}
static void ConvBase64(unsigned char* dest,const unsigned char* src,int bytes)
{
    const char src1=*src,src2=src[1],src3=src[2];
    *dest=base64strings[(src1&0xfc)>>2];
    if (bytes==1)
    {
        dest[1]=base64strings[((src1&0x03)<<4)];
        return;
    }
    dest[1]=base64strings[((src1&0x03)<<4)|((src2&0xf0)>>4)];
    if (bytes==2)
    {
        dest[2]=base64strings[((src2&0x0f)<<2)];
        return ;
    }
    dest[2]=base64strings[((src2&0x0f)<<2)|((src3&0xc0)>>6)];
    dest[3]=base64strings[(src3&0x3f)];
}
static bool_t ConvFlatFourBytes(unsigned char* dest,const char* src)
{
    char n1,n2,n3,n4;
    n1=base64code[*src];
    n2=base64code[src[1]];
    n3=base64code[src[2]];
    n4=base64code[src[3]];
    if ((n1==(char)0xf0)||
            (n2==(char)0xf0)||
                (n3==(char)0xf0)||
                    (n4==(char)0xf0))
    {
        SECURE_ERROR("%s","Invalid BASE64 FORMAT!!!\n");
        return false; /*base64形式ではない*/
    }
    *dest=((n1<<2)|(n2&0x30)>>4);
    dest[1]=((n2&0x0f)<<4)|((n3&0x3c)>>2);
    dest[2]=((n3&0x03)<<6)|n4;
    return true;
}
static bool_t ConvFlat(unsigned char* dest,const char* src,int n)
{
    char n1,n2,n3,n4;
    n1=base64code[*src];
    n2=base64code[src[1]];
    n3=base64code[src[2]];
    n4=base64code[src[3]];

    if (n1==(char)0xf0)
    {
        SECURE_ERROR("%s\n","n1 Invalid BASE64 FORMAT!!!\n");
        return false;
    }
    if (n==1)
    {
        *dest=(n1<<2);
        return true;
    }
    if (n2==(char)0xf0)
    {
        SECURE_ERROR("%s\n","n2 Invalid BASE64 FORMAT!!!\n");
        return false;
    }
    *dest=((n1<<2)|(n2&0x30)>>4);
    if (n==2)
    {
        dest[1]=((n2&(char)0x0f)<<4);
        return true;
    }
    if (n3==(char)0xf0)
    {
        SECURE_ERROR("%s\n","n3 Invalid BASE64 FORMAT!!!\n");
        return false;
    }
    dest[1]=((n2&(char)0x0f)<<4)|((n3&(char)0x3c)>>2);
    if (n==(char)3)
    {
        dest[2]=((n3&(char)0x03)<<6);
        return true;
    }
    if (n4==(char)0xf0)
    {
        SECURE_ERROR("%s\n","n4 Invalid BASE64 FORMAT!!!\n");
        return false;
    }
    dest[2]=((n3&(char)0x03)<<6)|n4;
    return true;
}
static void* base64_decode(const char* data,const size_t data_size,void* dest)
{
    unsigned char* p=(unsigned char*)data;
    unsigned char* p2=(unsigned char*)dest;
    size_t n=data_size>>2;
    size_t r=data_size%4;

    for (int i=0;i<n;++i)
    {
        if (ConvFlatFourBytes(p2,(const char*)p)==false)
        {
            return 0;
        }
        p+=4,p2+=3;
    }
    if (r!=0)
    {
        if (ConvFlat(p2,(const char*)p,r)==false)
        {
            return 0;
        }
    }
    return dest;
}
static void* base64_encode(const void* data,const size_t data_size,void* dest)
{
    unsigned char* p=(unsigned char*)data;
    unsigned char* p2=(unsigned char*)dest;
    size_t n=data_size/3;
    size_t r=data_size%3;
    for (int i=0;i<n;++i)
    {
        ConvBase64TreeBytes(p2,p);
        p+=3,p2+=4;
    }
    if (r!=0)
    {
        ConvBase64(p2,p,r);
        if (r==1)
        {
            p2+=2;
            *p2='=';
            p2[1]='=';
        }
        else if (r==2)
        {
            p2+=3;
            *p2='=';
        }
    }
    return dest;
}
void* BASE64_Encode(const void* data,const size_t data_size,void* out_buffer,const size_t out_buffer_size,size_t* encoded_size)
{
    *encoded_size=((data_size/3)<<2)+(((data_size%3)!=0)?4:0);
    if (out_buffer_size!=0&&*encoded_size>out_buffer_size)
    {
        return 0;
    }
    if (out_buffer==0)
    {
        char*p;
        out_buffer=malloc(*encoded_size+1);
        p=(char*)out_buffer;
        p[*encoded_size]=0;
    }
    return base64_encode(data,data_size,out_buffer);
}
void* BASE64_Decode(const void* data,size_t data_size,void* out_buffer,const size_t out_buffer_size,size_t* encoded_size)
{
    const char* p=(const char*)data;
    int k=(p[data_size-1]=='=')+(p[data_size-2]=='='),r=0;
    if (k!=0)
    {
        r=(k==1)?2:1;
    }
    *encoded_size=(((data_size-k)>>2)*3)+r;
    if (out_buffer_size!=0&&*encoded_size>out_buffer_size)
    {
        SECURE_ERROR("the buffer size too small (%zd!=%zd)\n",out_buffer_size,*encoded_size);
        return 0;
    }
    if (initialized==false)
    {
        Init();
        initialized=true;
    }
    if (out_buffer==0)
    {
        char*p;
        out_buffer=malloc(*encoded_size+1);
        p=(char*)out_buffer;
        p[*encoded_size]=0;
    }
    return base64_decode(data,data_size-k,out_buffer);
}
void BASE64_Free(void* data)
{
    free(data);
}
size_t BASE64_GetBase64Size(size_t n)
{
    return ((n/3)<<2)+(((n%3)!=0)?4:0);
}

#ifdef UNIT_TEST
int main(int argc,const char* argvs[])
{
    char buffer[1024],buffer2[1024];
    size_t n;
    BASE64_Encode(argvs[1],strlen(argvs[1]),buffer,sizeof(buffer),&n);
    buffer[n]=0;
    printf("%s\n",buffer);
    BASE64_Decode(buffer,strlen(buffer),buffer2,sizeof(buffer2),&n);
    buffer2[n]=0;
    printf("%s\n",buffer2);
    return 0; 
}
#endif
