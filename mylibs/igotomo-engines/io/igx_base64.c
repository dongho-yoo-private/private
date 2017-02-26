#include "igx_encrypt.h"
#include <stdlib.h>
#include <string.h>

#define IGX_BASE64_MALLOC malloc
#define IGX_BASE64_FREE free

char base64table[64] = {0, };// = {

static void* IGX_BASE64_MALLOC_I(bit32_t size)
{
  void* p = IGX_BASE64_MALLOC(size);
  return memset(p, 0, size);
}

// --------------------------------
void init_base64()
{
  bit32_t i=0;
  bit32_t cnt=0;

  for (i=0; i<('Z'-'A')+1; i++)
    base64table[i]=i+'A';

  cnt=i;
  for (i=0; i<('z'-'a')+1; i++)
  {
    base64table[cnt++]=i+'a';
  }

  for (i=0; i<('9'-'0')+1; i++)
  {
    base64table[cnt++]=i+'0';
  }

  base64table[62]='+';
  base64table[63]='/';
}

unsigned char get_6bit_code(unsigned char ch)
{
  bit32_t i=0;
  //printf("get_6bit_code: %c, %x\n", ch, (int)ch);
  for (i=0; i<64; i++)
  {
    if (ch==base64table[i])
    {
      //printf("get_6bit_code: same %c=%c, ret=%d\n", ch, base64table[i], i);
      return (unsigned char)i;
    }  
  }

  return 0xFF;
}

const unsigned char firstMaskBits[8]  = { 0xFC, 0x7E, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01};
const unsigned char secondMaskBits[8] = { 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8};
const char shiftBits[8] = { 2, 1, 0, -1, -2, -3, -4, -5 };

// --------------------------------
static char get_bitz(unsigned char* p, bit32_t sumBits, bit32_t size)
{
  bit32_t s8 = sumBits/8;
  bit32_t s = sumBits%8;
  unsigned char ret=0;
  unsigned char v1, v2; //=p[s8], v2=p[s8+1];

  if (sumBits >= (size<<3))
  {
    return 0xFF;
  }

  v1=p[s8];

  if (shiftBits[s]>=0)
  {
    ret = ((firstMaskBits[s]&v1)>>shiftBits[s]);
    return ret;
  }

  v2=p[s8+1];
  ret = ((firstMaskBits[s]&v1)<<(-shiftBits[s]));

  if (sumBits+6 > (size<<3))
  {
    return ret;  
  }
  ret|=((secondMaskBits[s]&v2)>>(10-s));

  return ret;

}
void* igx_encrypt_base64(void* dest, size_t* out_size, const void* src, size_t size)
{
    bit32_t i;
    char* ret = 0;
    unsigned char* pIndex=(unsigned char*)src;
    bit32_t newSize = (bit32_t)((size<<3)/6);
    bit32_t remain = (size<<3)%6;
    bit32_t cnt=0;
    bit32_t sumBits=0;
  
    newSize = (remain==0)?newSize:newSize+1;
  
    if ((newSize&0x03)!=0)
    {
      newSize+=(4-(newSize&0x03));
    }
  
    if (base64table[0]==0)
    {
      init_base64();
    }
  
    if (dest==0)
    {
        ret = (char*)IGX_BASE64_MALLOC(newSize+1);
    }
    else
    {
        ret = (char*)dest;
    }
  
    if (ret==0)
    {
      return 0;
    }
    ret[newSize]=0;
  
    /*memset(ret, '=', newSize);*/
    for (i=0; i<newSize; i++)
    {
      unsigned char n = get_bitz(pIndex, sumBits, size);
  
      if (n==0xFF)
        break;
  
      ret[cnt]=base64table[n];
      ++cnt;
      sumBits+=6;
    }

    memset(ret+cnt, '=', newSize-cnt);
    
    if (out_size)
    {
      *out_size=newSize;
    }
  
    return ret;
}

unsigned char restoreMask[9]={0, 0x20, 0x30, 0x38, 0x3C, 0x3E, 0x3F, 0x3F, 0x3F};
bit32_t restoreShiftBits[9] = {0, 5, 4, 3, 2, 1, 0, -1, -2};

// --------------------------------
static bit32_t restore_bits(unsigned char* p_new_buffer, unsigned char code, bit32_t i)
{
  bit32_t startBit    = i*6;
  bit32_t nStartOffset = (startBit>>3);
  bit32_t nRemainBits  = 8-(startBit&0x07);
  unsigned char x = restoreMask[nRemainBits]&code;

  if (restoreShiftBits[nRemainBits]>0)
  {
    x=(x>>restoreShiftBits[nRemainBits]);
  }
  else
  {
    x=(x<<(-restoreShiftBits[nRemainBits]));
  }

  p_new_buffer[nStartOffset]|=x;
  
  if (restoreMask[nRemainBits]!=0x3F)
  {
    nStartOffset++;
    x = (~restoreMask[nRemainBits])&code;
    nRemainBits = 6-nRemainBits;
    nRemainBits = 8-nRemainBits;
    p_new_buffer[nStartOffset]|=(x<<nRemainBits);
  }  

  //printf("restore bits.... new buffer x:%x\n", (unsigned int)p_new_buffer[nStartOffset]);
  return nStartOffset;
}

#define xIS_ALPHABET(a) (a>='A' && a<='z')
#define xIS_NUMERIC(a)  (a>='0' && a<='9')

// --------------------------------
int is_base64_format(const char* buffer, int size)
{
  int i;
  for (i=0; i<size; i++)
  {
    if (xIS_ALPHABET(buffer[i]))
      continue;
    if (xIS_NUMERIC(buffer[i]))
      continue;
    if (buffer[i]=='/' ||
      buffer[i]=='+' ||
      buffer[i]=='=')
      continue;
    return 0;
  }
  return 1;
}


static void* to_plain(void* dest, size_t* out_size, const char* buffer, size_t size, bool_t bIsForced)
{
    bit32_t i, k;
    bit32_t cnt=0;
    char* pIndex=(char*)buffer;
    char* p_new_buffer=dest;
    bit32_t dummy=(buffer[size-1]!='=')?1:0;
  
    if (base64table[0]==0)
    {
      init_base64();
    }
  
    if (is_base64_format(buffer, size)==0)
    {
      return 0;
    }
  
    cnt = (size-cnt);  
    p_new_buffer = dest;
  
    if (dest==0)
    {
      p_new_buffer = IGX_BASE64_MALLOC(cnt+1);
    }
    else
    {
        p_new_buffer = dest;
    }
    p_new_buffer[cnt+sizeof(int)+1]=0;
  
  
    for (i=0,k=0; i<cnt; i++)
    {
      unsigned char code = 0;
      if (pIndex[i]=='=')
      {
        break;
      }
      if (pIndex[i]==0)
      {
        break;
      }
  
      code = get_6bit_code(pIndex[i]);  
  
      if (code==0xFF)
      {
        if (bIsForced==true)
        {
          continue;
        }
  
        if (dest==0)
        {      
          IGX_BASE64_FREE(p_new_buffer);
        }
        return 0;
      }
  
      size = restore_bits((unsigned char*)p_new_buffer, code, k++);
    }
  
    if (out_size)
    {
      *out_size=size+dummy;
    }
    p_new_buffer[size+dummy]=0;
  
    return p_new_buffer;
}

#if 0
// --------------------------------
Binary yiToPlain(const char* buffer)
{
  return to_plain(buffer, False);
}
#endif

void* igx_decrypt_base64(void* dest, size_t* out_size, const void* source, size_t src_size)
{
  return to_plain(dest, out_size, source, src_size, true);
}


#if 0
int main(int argc, char** argv)
{
  int size;
  char* out;
  if (argc==1)
  {
    printf("USAGE: base64_test text\n");
    exit(0);
  }

  printf("%s\n", argv[1]);

  printf("++++++++++++++++++++++++++++++ encode start\n", argv[1]);
  out = LIB_Base64Encode(argv[1], strlen(argv[1]), &size);
  printf("%s\n", out);

  printf("++++++++++++++++++++++++++++++ decode start\n", argv[1]);
  out = (char*)LIB_Base64Decode(out, &size);
  printf("%s\n", out);
  
}
#endif

