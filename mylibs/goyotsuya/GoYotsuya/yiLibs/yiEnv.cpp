#include "yiEnv.h"

cyiEnv::cyiEnv(void)
{
}

cyiEnv::~cyiEnv(void)
{
}


const char* cyiEnv::GetYotsuyaRootDir()
{
	return "c:\\yotsuya";
}


const char* cyiEnv::GetYotsuyaUserDir()
{
	return "c:\\yotsuya\\users";
}

const char* cyiEnv::GetYotsuyaUserDir(unsigned int id, const char* pszUserName)
{
	static char buffer[256];

	if (id!=0)
	{
		wsprintf(buffer, "%s\\%d", GetYotsuyaUserDir(), id);
		return buffer;
	}
	else
	{
		wsprintf(buffer, "%s\\%s\\%s", GetYotsuyaUserDir(), "noid", pszUserName);
		return buffer;
	}

	return 0;
}

// -------------------------
const char* cyiEnv::GetYotsuyaCacheDir()
{
	return "c:\\yotsuya\\cached";
}

void cyiEnv::MakeEnv()
{
	char buffer[256];

	CreateDirectory(GetYotsuyaRootDir(), 0);
	CreateDirectory(GetYotsuyaUserDir(), 0);
	CreateDirectory(GetYotsuyaCacheDir(), 0);

	wsprintf(buffer, "%s\\noid", GetYotsuyaUserDir());
	CreateDirectory(buffer, 0);
}


// ------------------------------
char* yiToMultiByteCode(char* dest, const wchar_t* pszSrc, int set)
{
	int codePage=CP_ACP;
	int length=0;//wcslen(pszSrc);
	Bool bIsUsedDefaultChar=False;

	if (set==0 ||
			set==eCS_JAPANESE)
	{
		codePage=932;
	}
	else if (set==eCS_KOREAN)
	{
		codePage=949;
	}
	else if (set==eCS_CHINESE)
	{
		codePage=950;
	}
	else if (set==eCS_UTF8)
	{
		codePage=CP_UTF8;
	}
	else
	{
		return 0;
	}


	length=WideCharToMultiByte(codePage, 0, pszSrc, -1, 0, 0, 0,  0);

	if (dest==RET_SIZE)
		return (char* )length;


	if (dest==0)
	{
		dest = (char*)yialloci(length+1);
	}
	
	WideCharToMultiByte(codePage, 0, pszSrc, -1, dest, length*2, 0, 0);
	return dest;
}

//char g_xxxCharCodeBuffer[4096];

// ------------------------------
wchar_t* yiToUnicode(wchar_t* dest, const char* pszSrc, int set)
{
	static wchar_t szTempBuffer[256];
	int codePage=CP_ACP;
	int size=0;
	

	if (pszSrc==0)
	{
		if (dest)
			*dest=0;
		return 0;
	}

	size = strlen(pszSrc);

	//if (dest==0)
	//	dest = szTempBuffer;

	if (set==0 ||
			set==eCS_JAPANESE)
	{
		codePage=932;
	}
	else if (set==eCS_KOREAN)
	{
		codePage=949;
	}
	else if (set==eCS_CHINESE)
	{
		codePage=950;
	}
	else if (set==eCS_UTF8)
	{
		codePage=CP_UTF8;
	}
	else
	{
		return 0;
	}

	//if (dest==RET_SIZE)
	//{
	//	return size;
	//}

	if (dest)
		*dest=0;

	//if (pszSrc==0)
	//{
	//	return 0;
	//}

	int n;
	if (dest==0)
	{
		n = MultiByteToWideChar(codePage, MB_PRECOMPOSED, pszSrc, size, 0, 0);

		if (n==0)
		{
			return 0;
		}

		dest = (wchar_t*)yialloc((n<<1)+2);

	}

	if((n=MultiByteToWideChar(codePage, MB_PRECOMPOSED, pszSrc, size, dest, size))==0)
	{
		return 0;
	}
	dest[n]=0;
	return dest;
}

// ------------------------------
char* yiToUtf8(char* dest, const char* pszSrc, int set)
{
	wchar_t* p = yiToUnicode(0, pszSrc, set);
	char* ret = yiToMultiByteCode(dest, p, eCS_UTF8);
	yifree(p);
	return ret;
}

// ------------------------------
// http://www.lab.its55.com/?p=33 を参照。
char* yiUtf8ToMulti(char* dest, const char* pszSrc, int set)
{
	// 1. utfをunicodeに変換
	wchar_t*p = yiToUnicode(0, pszSrc, eCS_UTF8);
	char* ret = yiToMultiByteCode(dest, p, set);
	yifree(p);
	return ret;
}

//static char *sjistojis(unsigned char sjis[2])
//{
//	static unsigned char jis[2];
//
//	jis[0]=sjis[0];
//	jis[1]=sjis[1];
//
//	if(jis[0]<=0x9f)
//		jis[0]-=0x71;
//	else
//		jis[0]-=0xb1;
//
//	jis[0]*=2;
//	jis[0]++;
//
//	if(jis[1]>=0x7F)
//		jis[1]-=0x01;
//
//	if(jis[1]>=0x9e){
//		jis[1]-=0x7d;
//		jis[0]++;
//	}else
//		jis[1]-=0x1f;
//	return jis;
//}
//
//static char *jistosjis(unsigned char jis[2])
//{
//	static unsigned char sjis[2];
//
//	sjis[0]=jis[0];
//	sjis[1]=jis[1];
//
//	if(sjis[0]%2==1)
//		sjis[1]+=0x1f;
//	else
//		sjis[1]+=0x7d;
//
//	if(sjis[1]>=0x7f)
//		sjis[1]++
//
//	sjis[0]=(sjis[0]-0x21)/2+0x81;
//
//	if(sjis[0]>=0x9e)
//		sjis[0]+=0x40;
//
//	return sjis;
//}



// Thanks to http://www-cms.phys.s.u-tokyo.ac.jp/~naoki/CIPINTRO/CCGI/kanjicod.html

#define ASCII    0x00
#define EUC      0x01
#define SJIS     0x02
#define JIS      0x04
#define JAPANESE 0xff

//---- 半角カナの存在を無視した漢字コードの判定関数
int KanjiCode( char* text )
{
  for( u_char* ptr=(u_char*)text; *ptr; ptr++ ){
    if( *ptr == 0x1b && *(ptr+1) == '$' ) return  JIS;
    if( *ptr < 0x80 ) continue;
    if( 0x81 <= *ptr && *ptr <= 0x9F ) return SJIS;
    if( 0xA1 <= *ptr && *ptr <= 0xDF ) return  EUC;
    if( *(ptr+1) <= 0xA0 )             return SJIS;
  }
  return ASCII;
}


//---- JIS文字をEUC文字に変換する関数
inline void JIStoEUC( u_char& knj1, u_char& knj2 )
{
  knj1 |= 0x80;  knj2 |= 0x80;
}

//---- EUC文字をJIS文字に変換する関数
inline void EUCtoJIS( u_char& knj1, u_char& knj2 )
{
  knj1 &= ~0x80;  knj2 &= ~0x80;
}

//---- JIS文字をSJIS文字に変換する関数
inline void JIStoSJIS( u_char& knj1, u_char& knj2 )
{
  if( knj1 & 0x01 ){
    knj1 >>= 1;
    if( knj1 < 0x2F ) knj1 += 0x71; else knj1 -= 0x4F;
    if( knj2 > 0x5F ) knj2 += 0x20; else knj2 += 0x1F;
  }else{
    knj1 >>= 1;
    if( knj1 < 0x2F ) knj1 += 0x70; else knj1 -= 0x50;
    knj2 += 0x7E;
  }
}

//---- SJIS文字をJIS文字に変換する関数
inline void SJIStoJIS( u_char& knj1, u_char& knj2 )
{
  knj1 <<= 1;
  if( knj2 < 0x9F ){
    if( knj1 < 0x3F ) knj1 += 0x1F; else knj1 -= 0x61;
    if( knj2 > 0x7E ) knj2 -= 0x20; else knj2 -= 0x1F;
  }else{
    if( knj1 < 0x3F ) knj1 += 0x20; else knj1 -= 0x60;
    knj2 -= 0x7E;
  }
}

//---- EUC文字をSJIS文字に変換する関数
inline void EUCtoSJIS( u_char& knj1, u_char& knj2 )
{
  if( knj1 & 0x01 ){
    knj1 >>= 1;
    if( knj1 < 0x6F ) knj1 += 0x31; else knj1 += 0x71;
    if( knj2 > 0xDF ) knj2 -= 0x60; else knj2 -= 0x61;
  }else{
    knj1 >>= 1;
    if( knj1 < 0x6F ) knj1 += 0x30; else knj1 += 0x70;
    knj2 -= 0x02;
  }
}

//---- SJIS文字をEUC文字に変換する関数
inline void SJIStoEUC( u_char& knj1, u_char& knj2 )
{
  knj1 <<= 1;
  if( knj2 < 0x9F ){
    if( knj1 < 0x3F ) knj1 -= 0x61; else knj1 += 0x1F;
    if( knj2 > 0x7E ) knj2 += 0x60; else knj2 += 0x61;
  }else{
    if( knj1 < 0x3F ) knj1 -= 0x60; else knj1 += 0x20;
    knj2 += 0x02;
  }
}

static void JIStoEUC( char* text )
{
  int mode = ASCII;

  u_char *wr, *re;
  for( wr=re=(u_char*)text; *re; re++ ){
    if( (re[0]=='\x1b' && re[1]=='$' && re[2] == 'B' ) ||
        (re[0]=='\x1b' && re[1]=='$' && re[2] == '@' ) ){
      re+=2; 
      mode = JAPANESE;
      continue;
    }else if( (re[0]=='\x0f') ||
        (re[0]=='\x1b' && re[1]=='(' && re[2] == 'B' ) ||
        (re[0]=='\x1b' && re[1]=='(' && re[2] == 'J' ) ){
      re+=2; 
      mode = ASCII;
      continue;
    }else if( (re[0]=='\x0e') ||
        (re[0]=='\x1b' && re[1]=='(' && re[2] == 'I' ) ){
      re+=2; 
      mode = ASCII; // hankaku IGNORE
      continue;
    }

    if( mode == ASCII ){
      *wr++ = *re;
      continue;
    }
    *wr++ = *re;
    if( !(*wr = *++re) ) break;
    JIStoEUC( *(wr-1), *wr );
    wr++;
  }
  *wr='\0';
}

static void EUCtoJIS( char* text )
{
  int mode=ASCII;

  char* buf2 = strdup(text);
  u_char *wr, *re;
  for( wr=(u_char*)text, re=(u_char*)buf2; *re; re++ ){
    if( *re < 0x80 ){
      if( mode != ASCII ){
	mode  = ASCII;
	*wr++ = '\x1b';	*wr++ = '(';	*wr++ = 'B';
      }
      *wr++ = *re;
    }else{
      if( mode != JAPANESE ){
	mode  = JAPANESE;
	*wr++ = '\x1b';	*wr++ = '$';	*wr++ = 'B';
      }
      *wr++ = *re;
      if( !(*wr = *++re) ) break;
      EUCtoJIS( *(wr-1), *wr );
      wr++;
    }
  }
  if( mode != ASCII ){
    mode  = ASCII;
    *wr++ = '\x1b';  *wr++ = '(';  *wr++ = 'B';
  }
  *wr = '\0';

  free(buf2);
}

static void JIStoSJIS( char* text )
{
  int mode = ASCII;

  u_char *wr, *re;
  for( wr=re=(u_char*)text; *re; re++ ){
    if( (re[0]=='\x1b' && re[1]=='$' && re[2] == 'B' ) ||
        (re[0]=='\x1b' && re[1]=='$' && re[2] == '@' ) ){
      re+=2; 
      mode = JAPANESE;
      continue;
    }else if( (re[0]=='\x0f') ||
        (re[0]=='\x1b' && re[1]=='(' && re[2] == 'B' ) ||
        (re[0]=='\x1b' && re[1]=='(' && re[2] == 'J' ) ){
      re+=2; 
      mode = ASCII;
      continue;
    }else if( (re[0]=='\x0e') ||
        (re[0]=='\x1b' && re[1]=='(' && re[2] == 'I' ) ){
      re+=2; 
      mode = ASCII; // hankaku IGNORE
      continue;
    }

    if( mode == ASCII ){
      *wr++ = *re;
      continue;
    }
    *wr++ = *re;
    if( !(*wr = *++re) ) break;
    JIStoSJIS( *(wr-1), *wr );
    wr++;
  }
  *wr='\0';
}

static void SJIStoJIS( char* text )
{
  int mode = ASCII;

  char* buf2 = strdup(text);
  u_char *wr, *re;
  for( wr=(u_char*)text, re=(u_char*)buf2; *re; re++ ){
    if( *re < 0x80 ){
      if( mode != ASCII ){
	mode  = ASCII;
	*wr++ = '\x1b';	*wr++ = '(';	*wr++ = 'B';
      }
      *wr++ = *re;
    }else{
      if( mode != JAPANESE ){
	mode  = JAPANESE;
	*wr++ = '\x1b';	*wr++ = '$';	*wr++ = 'B';
      }
      *wr++ = *re;
      if( !(*wr = *++re) ) break;
      SJIStoJIS( *(wr-1), *wr );
      wr++;
    }
  }
  if( mode != ASCII ){
    mode  = ASCII;
    *wr++ = '\x1b';  *wr++ = '(';  *wr++ = 'B';
  }
  *wr = '\0';

  free(buf2);
}

static void EUCtoSJIS( char* text )
{
  for( u_char* ptr=(u_char*)text; *ptr; ptr++ ){
    if( *ptr < 0x80 ) continue;
    if( !(*++ptr) ) break;
    EUCtoSJIS( *(ptr-1), *ptr );
  }
}

static void SJIStoEUC( char* text )
{
  for( u_char* ptr=(u_char*)text; *ptr; ptr++ ){
    if( *ptr < 0x80 ) continue;
    if( !(*++ptr) ) break;
    SJIStoEUC( *(ptr-1), *ptr );
  }
}

static void ANYtoJIS( char* text )
{
  switch( KanjiCode( text ) ){
    case JIS  :                    break;
    case EUC  : EUCtoJIS( text );  break;
    case SJIS : SJIStoJIS( text ); break;
    case ASCII:                    break;
  }
}

static void ANYtoEUC( char* text )
{
  switch( KanjiCode( text ) ){
    case JIS  : JIStoEUC( text );  break;
    case EUC  :                    break;
    case SJIS : SJIStoEUC( text ); break;
    case ASCII:                    break;
  }
}

static void ANYtoSJIS( char* text )
{
  switch( KanjiCode( text ) ){
    case JIS  : JIStoSJIS( text ); break;
    case EUC  : EUCtoSJIS( text ); break;
    case SJIS :                    break;
    case ASCII:                    break;
  }
}

static void EUC_HtoZ( u_char& kana0, u_char& kana1 ){
  static u_char kana_table[] = {
    0xA1,0xA3,0xA1,0xD6,0xA1,0xD7,0xA1,0xA2,
    0xA1,0xA6,0xA5,0xF2,0xA5,0xA1,0xA5,0xA3,
    0xA5,0xA5,0xA5,0xA7,0xA5,0xA9,0xA5,0xE3,
    0xA5,0xE5,0xA5,0xE7,0xA5,0xC3,0xA1,0xBC,
    0xA5,0xA2,0xA5,0xA4,0xA5,0xA6,0xA5,0xA8,
    0xA5,0xAA,0xA5,0xAB,0xA5,0xAD,0xA5,0xAF,
    0xA5,0xB1,0xA5,0xB3,0xA5,0xB5,0xA5,0xB7,
    0xA5,0xB9,0xA5,0xBB,0xA5,0xBD,0xA5,0xBF,
    0xA5,0xC1,0xA5,0xC4,0xA5,0xC6,0xA5,0xC8,
    0xA5,0xCA,0xA5,0xCB,0xA5,0xCC,0xA5,0xCD,
    0xA5,0xCE,0xA5,0xCF,0xA5,0xD2,0xA5,0xD5,
    0xA5,0xD8,0xA5,0xDB,0xA5,0xDE,0xA5,0xDF,
    0xA5,0xE0,0xA5,0xE1,0xA5,0xE2,0xA5,0xE4,
    0xA5,0xE6,0xA5,0xE8,0xA5,0xE9,0xA5,0xEA,
    0xA5,0xEB,0xA5,0xEC,0xA5,0xED,0xA5,0xEF,
    0xA5,0xF3,0xA1,0xAB,0xA1,0xAC };

  if( kana0!=0x8E ) return;
  if( !(0x21<=kana1 && kana1<=0x5F ) ) return;

  const int index = 2*(kana1-0x21);
  kana0 = kana_table[index+0];
  kana1 = kana_table[index+1];
}

static void SJIS_HtoZ( u_char& kana0, u_char& kana1 ){
  static u_char kana_table[] = {
    0x81,0x42,0x81,0x75,0x81,0x76,0x81,0x41,
    0x81,0x45,0x83,0x92,0x83,0x40,0x83,0x42,
    0x83,0x44,0x83,0x46,0x83,0x48,0x83,0x83,
    0x83,0x85,0x83,0x87,0x83,0x62,0x81,0x5B,
    0x83,0x41,0x83,0x43,0x83,0x45,0x83,0x47,
    0x83,0x49,0x83,0x4A,0x83,0x4C,0x83,0x4E,
    0x83,0x50,0x83,0x52,0x83,0x54,0x83,0x56,
    0x83,0x58,0x83,0x5A,0x83,0x5C,0x83,0x5E,
    0x83,0x60,0x83,0x63,0x83,0x65,0x83,0x67,
    0x83,0x69,0x83,0x6A,0x83,0x6B,0x83,0x6C,
    0x83,0x6D,0x83,0x6E,0x83,0x71,0x83,0x74,
    0x83,0x77,0x83,0x7A,0x83,0x7D,0x83,0x7E,
    0x83,0x80,0x83,0x81,0x83,0x82,0x83,0x84,
    0x83,0x86,0x83,0x88,0x83,0x89,0x83,0x8A,
    0x83,0x8B,0x83,0x8C,0x83,0x8D,0x83,0x8F,
    0x83,0x93,0x81,0x4A,0x81,0x4B };


  if( !(0xA1<=kana0 && kana0<=0xDF ) ) return;

  const int index = 2*(kana0-0xA1);
  kana0 = kana_table[index+0];
  kana1 = kana_table[index+1];
}

static void JIS_HtoZ( u_char& kana0, u_char& kana1 ){
  static u_char kana_table[] = {
    0x21,0x23,0x21,0x56,0x21,0x57,0x21,0x22,
    0x21,0x26,0x25,0x72,0x25,0x21,0x25,0x23,
    0x25,0x25,0x25,0x27,0x25,0x29,0x25,0x63,
    0x25,0x65,0x25,0x67,0x25,0x43,0x21,0x3C,
    0x25,0x22,0x25,0x24,0x25,0x26,0x25,0x28,
    0x25,0x2A,0x25,0x2B,0x25,0x2D,0x25,0x2F,
    0x25,0x31,0x25,0x33,0x25,0x35,0x25,0x37,
    0x25,0x39,0x25,0x3B,0x25,0x3D,0x25,0x3F,
    0x25,0x41,0x25,0x44,0x25,0x46,0x25,0x48,
    0x25,0x4A,0x25,0x4B,0x25,0x4C,0x25,0x4D,
    0x25,0x4E,0x25,0x4F,0x25,0x52,0x25,0x55,
    0x25,0x58,0x25,0x5B,0x25,0x5E,0x25,0x5F,
    0x25,0x60,0x25,0x61,0x25,0x62,0x25,0x64,
    0x25,0x66,0x25,0x68,0x25,0x69,0x25,0x6A,
    0x25,0x6B,0x25,0x6C,0x25,0x6D,0x25,0x6F,
    0x25,0x73,0x21,0x2B,0x21,0x2C };

  if( !(0x21<=kana0 && kana0<=0x5F ) ) return;

  const int index = 2*(kana0-0x21);
  kana0 = kana_table[index+0];
  kana1 = kana_table[index+1];
}

// ------------------------------
char* yiToSjis(char* txt)
{
	ANYtoSJIS(txt);
	return txt;
}

// ------------------------------
char* yiToJis(char* txt)
{
	ANYtoJIS(txt);
	return txt;

}

// ------------------------------
char* yiToEuc(char* txt)
{
	ANYtoEUC(txt);
	return txt;

}