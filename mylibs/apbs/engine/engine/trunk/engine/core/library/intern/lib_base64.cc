#include <core/library/lib_base64.h>

#include <debug_profiling/error/result_types.h>
#include <debug_profiling/error/err_library_result_types.h>
#include <core/memory_allocation/guarded_alloc.h>

char base64table[64] = {0, };// = {

#define yialloc(a) ADK_Malloc(a, "LIB_BASE64")
#define yifree(a) ADK_Free(a)

static void* yialloci(S32 size)
{
  void* p = yialloc(size);
  return memset(p, 0, size);
}

ADK_BEGIN_NAMESPACE


// --------------------------------
void yiInitBase64()
{
  S32 i=0;
  S32 cnt=0;

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

/*  printf("yiInitBase64!!\n");

  for (i=0; i<64; i++)
  {
    printf("%02x : ", i);
    putchar(base64table[i]);
    putchar('\n');
  }
*/
}

unsigned char xGet6BitCode(unsigned char ch)
{
  S32 i=0;
  //printf("xGet6BitCode: %c, %x\n", ch, (int)ch);
  for (i=0; i<64; i++)
  {
    if (ch==base64table[i])
    {
      //printf("xGet6BitCode: same %c=%c, ret=%d\n", ch, base64table[i], i);
      return (unsigned char)i;
    }  
  }

  return 0xFF;
}

const unsigned char firstMaskBits[8]  = { 0xFC, 0x7E, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01};
const unsigned char secondMaskBits[8] = { 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8};
const char shiftBits[8] = { 2, 1, 0, -1, -2, -3, -4, -5 };

// --------------------------------
static char xGetBitz(unsigned char* p, S32 sumBits, S32 size)
{
  S32 s8 = sumBits/8;
  S32 s = sumBits%8;
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

// --------------------------------
Result LIB_Base64Encode(char** dest, const void* buffer, S32 size, S32* changedSize)
{
  S32 i;
  char* ret = 0;
  unsigned char* pIndex=(unsigned char*)buffer;
  S32 newSize = (size<<3)/6;
  S32 remain = (size<<3)%6;
  S32 cnt=0;
  S32 sumBits=0;

  newSize = (remain==0)?newSize:newSize+1;

  if ((newSize&0x03)!=0)
  {
    newSize+=(4-(newSize&0x03));
  }

  if (base64table[0]==0)
  {
    yiInitBase64();
  }

  ret = (char*)yialloc(newSize+1);

  if (ret==0)
  {
    return ADK_RESULT(ResultOutOfResource, false, false, kLibraryComponent, kLibBase64, 0);
  }
  ret[newSize]=0;

  //printf("new size -----> %d->%d\n", size, newSize);


  //printf("start encode......\n");
  memset(ret, '=', newSize);
  for (i=0; i<newSize; i++)
  {
    unsigned char n = xGetBitz(pIndex, sumBits, size);//sBit, eBit);

    if (n==0xFF)
      break;

    ret[cnt]=base64table[n];
    //putchar(ret[cnt]);
    //printf("%x:%c ", (int)ret[cnt], ret[cnt]);
    cnt++;
    sumBits+=6;
  }
  
  if (changedSize)
  {
    *changedSize=newSize;
  }

  *dest = ret;
  return ResultSuccess;
}

unsigned char restoreMask[9]={0, 0x20, 0x30, 0x38, 0x3C, 0x3E, 0x3F, 0x3F, 0x3F};
S32 restoreShiftBits[9] = {0, 5, 4, 3, 2, 1, 0, -1, -2};

// --------------------------------
static S32 xRestoreBits(unsigned char* pNewBuffer, unsigned char code, S32 i)
{
  S32 startBit    = i*6;
  S32 nStartOffset = (startBit>>3);
  S32 nRemainBits  = 8-(startBit&0x07);
  unsigned char x = restoreMask[nRemainBits]&code;

  //printf("restore bits.... startBit:%d, nStartOffset:%d, nRemainBits:%d, mask:%x, code:%x, x is %x\n", startBit, nStartOffset, nRemainBits, (int)restoreMask[nRemainBits], (int)code, (unsigned int)x);

  //printf("shiftbits:%d\n", restoreShiftBits[nRemainBits]);
  if (restoreShiftBits[nRemainBits]>0)
  {
    x=(x>>restoreShiftBits[nRemainBits]);
  }
  else
  {
    x=(x<<(-restoreShiftBits[nRemainBits]));
  }

  //printf("restore bits.... x:%x\n", (unsigned int)x);

  pNewBuffer[nStartOffset]|=x;
  //printf("restore bits.... new buffer x:%x\n", (unsigned int)pNewBuffer[nStartOffset]);
  
  if (restoreMask[nRemainBits]!=0x3F)
  {
    nStartOffset++;
    x = (~restoreMask[nRemainBits])&code;
    nRemainBits = 6-nRemainBits;
    nRemainBits = 8-nRemainBits;
    pNewBuffer[nStartOffset]|=(x<<nRemainBits);
  }  

  //printf("restore bits.... new buffer x:%x\n", (unsigned int)pNewBuffer[nStartOffset]);
  return nStartOffset;
}

#define xIS_ALPHABET(a) (a>='A' && a<='z')
#define xIS_NUMERIC(a)  (a>='0' && a<='9')

// --------------------------------
int xIsBase64Format(const char* buffer, int size)
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


// --------------------------------
static Result _yiToPlain(void** dest, const char* buffer, S32* convertedSize, Bool bIsForced)
{
  S32 i, k;
  S32 cnt=0;
  size_t size=strlen(buffer);
  char* pIndex=(char*)buffer;
  char* pNewBuffer=0;
  S32 dummy=(buffer[size-1]!='=')?1:0;

  if (base64table[0]==0)
  {
    yiInitBase64();
  }

  if (xIsBase64Format(buffer, size)==0)
  {
    return ADK_RESULT(ResultUnknown, false, false, kLibraryComponent, kLibBase64, 0);
  }

  // 1. remove '='
  /*while(1)
  {
    putchar(pIndex[size-cnt-1]);
    if (pIndex[size-cnt-1]=='=')
    {
      pIndex[size-cnt-1]=0;
      cnt++;
      continue;
    }
    break;
  }*/

  cnt = (size-cnt);  

  pNewBuffer = (char*)yialloci(cnt+sizeof(int));

  if (pNewBuffer==0)
  {
    return ADK_RESULT(ResultOutOfResource, false, false, kLibraryComponent, kLibBase64, 1);
  }

  //if (changedSize)
  //  *changedSize=0;

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

    code = xGet6BitCode(pIndex[i]);  

    if (code==0xFF)
    {
      if (bIsForced==true)
      {
        // 無視する。
        continue;
      }

      yifree(pNewBuffer);
      //YIERROR("_yiToPlain() invalid code!, pIndex[%d]:%x\n", i, (int)pIndex[i]);
      return ADK_RESULT(ResultUnknown, false, false, kLibraryComponent, kLibBase64, 1);
    }

    size = xRestoreBits((unsigned char*)pNewBuffer, code, k++);
    //printf(pNewBuffer);
    //putchar('\n');
  }

  if (convertedSize)
    *convertedSize=size+dummy;
  pNewBuffer[size+dummy]=0;

  *dest = pNewBuffer;
  return ResultSuccess;
}

#if 0
// --------------------------------
Binary yiToPlain(const char* buffer)
{
  return _yiToPlain(buffer, False);
}
#endif

Result LIB_Base64Decode(void** dest, const char* source, S32* convertedSize)
{
  return _yiToPlain(dest, source, convertedSize, true);
}

void LIB_Base64Free(void* p)
{
  if (p)
    yifree(p);
}


#ifdef __UNIT_TEST__
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

ADK_END_NAMESPACE
