#include "io/igx_checksum.h"

ubit32_t igx_get_checksum(const void* data, size_t size, bit32_t read_bytes)
{
    const int count = size/(read_bytes=(read_bytes==0)?4:read_bytes);
    const int remain = size%read_bytes;
    ubit32_t check_sum=0, i;

    if (read_bytes==1)
    {
        char* p = (char*)data;
        for (i=0; i<count; ++i, p++)
        {
            check_sum+=*p; 
        }
    }
    else if (read_bytes==2)
    {
        ubit16_t* p = (ubit16_t*)data;
        for (i=0; i<count; ++i, p++)
        {
            check_sum+=*p; 
        }
    }
    else if (read_bytes==4)
    {
        ubit32_t* p = (ubit32_t*)data;
        for (i=0; i<count; ++i, p++)
        {
            check_sum+=*p; 
        }
    }

    if (remain!=0)
    {
        char* p = (char*)data;
        p+=(size-remain);
        for (i=0; i<count; ++i, p++)
        {
            check_sum+=*p; 
        }
    }
    return check_sum;
}
