#include "io/igx_encrypt.h"
#include "io/igx_checksum.h"
#include "debug/igx_log.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define ENCRYPT_MALLOC malloc
#define IGX_DEFAULT_ENCRYPT_KEY (0xab1c35ed)
#define IGX_ENCRYPT_INITIAL_VERSION (10000)
//#define ENCRYPT_DEBUG printf
//#define ENCRYPT_DEBUG 

static ubit32_t encrypt_error_no = 0;

#if 0
typedef void* (*igx_encrypt_fnc_t)(void*, size_t*, const void*, size_t, ubit32_t);

static void* encrypt_v1(void* dest, size_t* dest_size, 
                    const void* src, size_t src_size, ubit32_t key);

static ubit32_t g_using_version = IGX_ENCRYPT_INITIAL_VERSION;
static igx_encrypt_fnc_t g_using_fn_encrypt = encrypt_v1;
#endif

typedef struct {
    ubit16_t size;
    ubit16_t checksum;
    bit32_t version;
    bit32_t rand_no;
    ubit32_t noise;
} encrypt_header_t;


static void shuffle_v1(void* dest, const void* src, size_t src_size, ubit32_t key)
{
    bit32_t i, rand_no_offset=(key&0x03);
    ubit8_t* p = (ubit8_t*)src, *p_dest = (ubit8_t*)dest;
    const bit32_t count = (bit32_t)(src_size>>1);
    union {
        ubit32_t bit32;
        ubit8_t  bit8[4];
    } rand_no;
    rand_no.bit32 = (key==0)?IGX_DEFAULT_ENCRYPT_KEY:key;

    for (i=0; i<count; ++i, ++rand_no_offset)
    {
       bit32_t s = (rand_no_offset&0x03);
       bit8_t en1 = (p[i]&rand_no.bit8[s])|(p[src_size-i-1]&(~rand_no.bit8[s]));
       bit8_t en2 = (p[i]&(~rand_no.bit8[s]))|(p[src_size-i-1]&(rand_no.bit8[s]));
       p_dest[i]=(en1^rand_no.bit8[s]);
       p_dest[src_size-i-1]=(en2^rand_no.bit8[s]);
    }
    if ((src_size&1)!=0)
    {
        // 奇数の場合
        p_dest[count]=p[count];
    }
}
static void restore_v1(void* dest, const void* src, size_t src_size, ubit32_t key)
{
    ubit8_t* p = (ubit8_t*)src, *p_dest = (ubit8_t*)dest;
    bit32_t i, rand_no_offset=(key&0x03);
    const bit32_t count = (const bit32_t)(src_size>>1);
    union {
        ubit32_t bit32;
        ubit8_t  bit8[4];
    } rand_no;
    rand_no.bit32 = (key==0)?IGX_DEFAULT_ENCRYPT_KEY:key;

    for (i=0; i<count; ++i, ++rand_no_offset)
    {
       bit32_t s = (rand_no_offset&0x03);
       p[i]^=rand_no.bit8[s];
       p[src_size-i-1]^=rand_no.bit8[s];
       bit8_t en1 = (p[i]&(~rand_no.bit8[s]))|(p[src_size-i-1]&(rand_no.bit8[s]));
       bit8_t en2 = (p[i]&(rand_no.bit8[s]))|(p[src_size-i-1]&(~rand_no.bit8[s]));
       p_dest[i]=en2;
       p_dest[src_size-i-1]=en1;
    }
    if ((src_size&1)!=0)
    {
        // 奇数の場合
        p_dest[count]=p[count];
    }
}
static void make_encrypt_header(encrypt_header_t* header, ubit16_t size, ubit16_t checksum, bit32_t version, bit32_t rand_no, bit32_t key)
{
    encrypt_header_t in = {size, checksum, version, rand_no, rand()};
    header->size=size;
    header->checksum=checksum;
    header->version=version;
    header->rand_no=rand_no;

    //ENCRYPT_DEBUG("header, %zu,%d,%d,%x\n", size, (int)checksum, (int)version, (int)rand_no);
    
    shuffle_v1(header, &in, sizeof(in), key);
}
void* igx_encrypt(void* dest, size_t* dest_size, 
                    const void* src, size_t src_size, ubit32_t key)
{
    ubit8_t* p_dest;
    const size_t _dest_size = src_size+sizeof(encrypt_header_t);

    encrypt_header_t* header; 
    union {
        ubit32_t bit32;
        ubit8_t  bit8[4];
    } rand_no;

    srand((long)time(0));
    rand_no.bit32=rand();

    *dest_size=0;
    if (dest==0)
    {
        dest = ENCRYPT_MALLOC(_dest_size+1);
    }
    p_dest = (ubit8_t*)dest;
    header = (encrypt_header_t*)dest;
    shuffle_v1(header+1, src, src_size, rand_no.bit32);
    make_encrypt_header(header, src_size, (ubit16_t)igx_get_checksum(header+1, src_size, 0), IGX_ENCRYPT_INITIAL_VERSION, rand_no.bit32, key);

    *dest_size = _dest_size;
    p_dest[_dest_size]=0;

    return dest;
}
void* igx_decrypt(void* dest, size_t* dest_size, const void* src, size_t src_size, ubit32_t key)
{
    ubit8_t* p_dest;
    encrypt_header_t header;
    encrypt_header_t* src_header = (encrypt_header_t*)src;
    ubit16_t check_sum;

    restore_v1(&header, src, sizeof(encrypt_header_t), key);

    if (header.version!=IGX_ENCRYPT_INITIAL_VERSION)
    {
        encrypt_error_no = 2;
        return 0;
    }
    check_sum = (ubit16_t)igx_get_checksum(src_header+1, header.size, 0);
    //ENCRYPT_DEBUG("checksum:%d\n", (int)check_sum);
    
    if (check_sum!=header.checksum)
    {
        encrypt_error_no = 3;
        return 0;
    }

    if (dest==0)
    {
        dest = malloc(header.size+1);
    }
    p_dest=(ubit8_t*)dest;
    p_dest[header.size]=0;
    restore_v1(dest, src_header+1, header.size, header.rand_no);
    *dest_size = header.size;

    return dest;
}
void igx_shuffle(void* dest, const void* src, size_t src_size, ubit32_t key)
{
    return shuffle_v1(dest, src, src_size, key);
}
void igx_restore(void* dest, const void* src, size_t src_size, ubit32_t key)
{
    return restore_v1(dest, src, src_size, key);
}
const char* g_str_error[] =
{
    "success!",
    "restore error!",
    "no support version!",
    "invalid checksum!",
    "restore error!"
};
const char* igx_encrypt_last_error(ubit32_t* error_no)
{
    if (error_no!=0)
    {
        *error_no=encrypt_error_no;
    }
    return g_str_error[encrypt_error_no]; 
}
size_t igx_encrypted_size(size_t original_size)
{
    return original_size+sizeof(encrypt_header_t);
}
ubit32_t igx_hash_key_multiple_plus(const char* str, size_t len)
{
    ubit32_t hash=1, i;
    len=len?len:strlen(str);
    for (i=0; i<len; i++)
    {
        ubit32_t s = (ubit32_t)str[i];
        int n = (s&0x03);
        switch(n)
        {
            case 0:
                hash+=s;
            break;
            case 1:
                hash*=s;
            break;
            case 2:
                hash*=s, hash+=(s>>1);
            break;
        }
    }
    return hash;
}


#if 0
igx_encrypt_fnc_t g_fn_encrypt[] = {encrypt_v1};
void igx_encrypt_init(ubit32_t version)
{
    const ubit32_t index=0;

    g_using_version=version;
    g_using_version=(g_using_version==0)?IGX_ENCRYPT_INITIAL_VERSION:g_using_version;

    g_using_fn_encrypt=g_fn_encrypt[index];
}
#endif

#ifdef __UNIT_TEST__
/* for unit test sample
 * $ make encrypt
 * $ cat [anyfile] | ./encrypt_test
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#define READ_ONCE_BUFFER_SIZE (4095)
char* igx_get_string_from(int from_fd, size_t* size)
{
    int sum=0;
    size_t length;
    char temp[READ_ONCE_BUFFER_SIZE+1];
    char* buffer=0;

    while(1)
    {
        length = read(from_fd, temp, READ_ONCE_BUFFER_SIZE);
        if (length==0)
        {
            buffer[sum]=0;
            break;
        }
        buffer = realloc(buffer, sum+length+1);
        memcpy(buffer+sum, temp, length);
        sum+=length;
    }
    *size = sum;
    return buffer;
}
int main(int argc, const char** argv)
{
    size_t n;
    char* p = igx_get_string_from(fileno(stdin), &n);
    char* encrypt = (char*)malloc(n+16+1);
    char* decrypt = (char*)malloc(n+16+1);
    size_t ret_size;

    printf("abcde:%u\n", igx_hash_key_multiple_plus("abcde", 0)%256);
    printf("xxxx:%u\n", igx_hash_key_multiple_plus("xxxx", 0)%256);
    printf("dongho.yoo:%u\n", igx_hash_key_multiple_plus("dongho.yoo", 0)%256);
    printf("kapao.yoo:%u\n", igx_hash_key_multiple_plus("kapao.yoo", 0)%256);
    printf("makao:%u\n", igx_hash_key_multiple_plus("makao", 0)%256);
    printf("やきにく:%u\n", igx_hash_key_multiple_plus("やきにく", 0)%256);
    printf("ポポロン:%u\n", igx_hash_key_multiple_plus("ポポロン", 0)%256);

    if (p==0)
    {
        printf("no data\n");
        return 0;
    }
    igx_log_init(kLogOutputConsole, kLogLevelAll, 0);

    printf("+++++++++++++ before\n");
    printf("%s\n", p);
    printf("+++++++++++++ encrypt\n");
    igx_encrypt(encrypt, &ret_size, p, n, 0);
    igx_log_dump("encrypt dump", encrypt, ret_size, 16);
    printf("%s\n", encrypt);
    printf("+++++++++++++ decrypt\n");
    igx_decrypt(decrypt, &ret_size, encrypt, ret_size, 0);
    decrypt[n]=0;
    printf("%s\n", decrypt);
    igx_log_dump("decrypt dump", decrypt, ret_size, 16);
    printf("%s\n", igx_encrypt_last_error(0));

    return 0;
}
void* igx_encrypt_base64(void* dest, size_t* out_size, const void* src, size_t src_size)
{
}
void* igx_decrypt_base64(void* dest, size_t* out_size, const void* src, size_t src_size)
{
}

#endif
