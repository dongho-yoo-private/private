#ifndef __yi_h__
#define __yi_h__

#ifdef _WIN32
#ifndef FD_SETSIZE 
#define FD_SETSIZE 256
#endif
#include <winsock2.h>
#include <windows.h>
#endif

typedef int bit32_t;
typedef unsigned int ubit32_t;
typedef ubit32_t point_t;
typedef ubit32_t color_t;


#define xPOINT(pos) (pos&0xFFFF)
#define yPOINT(pos) (pos>>16)
#define SETPOINT(x, y) ((y<<16)|x)

#ifdef __POSIX__
#	define FASTCALL __attribute__((fastcall)) 
#	define CDECL __attribute__((cdecl)) 
#	define STDCALL __attribute__((stdcall)) 
#else
#	define FASTCALL 
#	ifndef CDECL
#		define CDECL __cdecl
#	endif
#	define STDCALL __stdcall
#endif

#ifdef __cplusplus
#define xEXTERN_C extern "C"
#include <iostream>
using namespace std;
#else
#define xEXTERN_C extern
#endif



/*#define MEM_ZERO(a) yimemzero(&a, sizeof(a))*/
#define MEM_ZERO(a) memset(&a, 0, sizeof(a))

#ifndef int32
typedef int int32;
#endif

#ifndef Int32
typedef int Int32;
typedef bit32_t uInt32;
#endif

typedef unsigned short bit16_t;
typedef short Bit16;
typedef unsigned short uBit16;

typedef unsigned char Bit8;

#ifdef _WIN64
#	define __long_byte_shift__	3
#	define __long_bit_shift__	6
#	define __long_bit_cnt__		64
#	define __LONG_SIGNED_BIT__	0x8000000000000000
#	define __LONG_FULL_BIT__	0xFFFFFFFFFFFFFFFF
#	define YISTR(str) (const char*)(str"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0")
typedef unsigned __int64	ubit64_t;
typedef __int64				bit64_t;
typedef bit64_t				IntPtr;
typedef bit64_t				LongPtr;
typedef ubit64_t			uIntPtr;
typedef ubit64_t			uLongPtr;
typedef __int64				Int64;
typedef __int64				Long64;
#else
#	define __long_byte_shift__	2
#	define __long_bit_shift__	5
#	define __long_bit_cnt__		32
#	define __LONG_SIGNED_BIT__	0x80000000
#	define __LONG_FULL_BIT__	0xFFFFFFFF
#	define YISTR(str) (const char*)(str"\0\0\0\0\0\0\0")

typedef bit32_t IntPtr;
typedef bit32_t LongPtr;
typedef ubit32_t uIntPtr;
typedef ubit32_t uLongPtr;
typedef int		Int32;
typedef long	Long32;
#endif

#define xBIN_SIZE(bin) ((unsigned int*)bin)[-1]
typedef void* Binary;
typedef bit32_t Bool;
#define True 1
#define False 0

typedef LongPtr Handle;
typedef unsigned char HandleBuffer[32];


#ifdef _WIN64
#define GWL_HINSTANCE	GWLP_HINSTANCE
#define GWL_USERDATA	GWLP_USERDATA
#endif

#define __long_shift__ __long_byte_shift__	

#define __shift_mod__(n, a) (~(__LONG_FULL_BIT__<<n)&(long)a)

#define bytetobit(n) (n<<3)

#define sizeofbit(a) bytetobit(sizeof(a))

#ifndef offsetof
#define xoffsetof(type, member) (bit32)(&(((type*)0)->member))
#endif


#define __sse2_bit_shift__ 4
#define YI_ALIGN_SHIFT __sse2_bit_shift__ 

#define YI_ALIGNMENT 16
#define YI_DEF_STRLEN 8

#define __YI_ALIGNED__ __attribute__((aligned(YI_ALIGNMENT)))

#define __is_yi_aligned__(x, y) \
	!(__shift_mod__(YI_ALIGN_SHIFT, (long)x) || __shift_mod__(YI_ALIGN_SHIFT, (long)y))

#ifdef __POSIX__
typedef char yiBuffer[256] __YI_ALIGNED__;
#endif

//#define IPADDRESS(a, b, c, d) ((a<<24) | (b<<16) | (c<<8) | d)
#define __ipaddress__(a, b, c, d) ((a<<24) | (b<<16) | (c<<8) | d)

#define __highword__(n) = ((Int32)n>>16)
#define __loword__(n) = (0x0000FFFF&(int32)n)

#define __highbyte__(n) = ((Int32)n>>8)
#define __lowbyte__(n) = (0x000000FF&(Int32)n)

#define FILE_YOTSUYA_PREFIX 0x53544f59 // "YOTS"

#define credit_sizeof(a) (sizeof(a)-sizeof(Int32))

#define __8to32__(a,b,c,d)  (((uInt32)a<<24)|((uInt32)b<<16)|((uInt32)c<<8)|(uInt32)d)

#define __16to32__(a,b)  (((uInt32)a<<16)|(uInt32)b);

#define __32to8__(value32, n) ((value32>>(n<<3)) & 0x000000FF)

#define __32to16__(value32,n)  ((value32>>(n<<4)) & 0x0000FFFF)

#define __p8to32__(p, n) *((Bit32*)(&p[n]))

#define __set8to32__(p, n, value) (*((Bit32*)(&p[n])) = value)

#define __setbiton__(n32, n) ((1<<n) | n32)

#define __setbitoff__(n32, n) ((~(1<<n)) & n32)

#define __biton__(n32, n) ((1<<n) | n32)

#define __date32__(y, m, d) ((y<<9) | (m<<5) | (d))

#define __year__(date) (date>>9)

#define __month__(date) (((date>>5)&0x0000000F)+1)

#define __day__(date) ((date&0x0000001F)+1)

#define __integer__(f) (((f-(int)f) > 0.5) ? (int)(f+1): (int)f)

#define __integerb__(f) (((f-(int)f) > 0.0) ? (int)(f+1): (int)f)

#define __INTEGER__(f) (((f - (float)((int)f)) < 0.5) ? (INT)f : ((INT)f+1))
#define __INTEGERF__(f) (((f - (float)((int)f)) == 0.0) ? (INT)f : ((INT)f+1))

#ifndef __MAX_PATH__ 
#define __MAX_PATH__ 256
#endif

//#ifdef _WIN32
//#define yiTrace
//#endif

#ifdef _WIN32
#include <windows.h>
#endif

#ifndef _WINDLL
#define _WINDLL
#endif

#ifndef STATIC_COMPILE
#define xEXPORT  __declspec(dllexport)
#undef xEXTERN_C

#ifdef __cplusplus
#define xEXTERN_C extern "C" __declspec(dllexport)
#else
#define xEXTERN_C extern __declspec(dllexport)
#endif

#else
#define xEXPORT 
#ifdef __cplusplus
#		define xEXTERN_C extern "C" 
#else
#		define xEXTERN_C extern 
#endif
#endif

#ifdef STATIC_COMPILE
#define xDLLMAIN DummyMain
#else
#define xDLLMAIN(hinstDLL, fdwReason, lpvReserved) \
DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
#endif



// --------------------------------
typedef struct xsyiGeneralParam {
	ubit32_t param[16];
}syiGeneralParam;

typedef struct {
	int param;
	int size;
	char data[8];
}sGeneralParamEx;

#define sGeneralParam syiGeneralParam

#define GENERAL_PARAM_SIZE(n) (n<<2)
#define GP_SIZE(n) (n<<2)

#define xString(str) (&((short*)str[1]))

#define xStringSize(str) ((*((short*)str)&0x7FFF))

#define xSetString(buffer, str) 


#ifdef __cplusplus
class yiRect {
public:
	bit32_t x;
	bit32_t y;
	bit32_t w;
	bit32_t h;
};

#endif

#define xCENTER(n, w) ((n-((w)>>1))+1)

#define xARGB(a, r, g, b) ((a<<24)|(r<<16)|(g<<8)|b)

#endif
