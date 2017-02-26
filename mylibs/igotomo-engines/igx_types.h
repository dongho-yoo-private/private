/**
* Copyright (c) 2015 igotomo.
* Authorized by dongho.yoo.
* 
* @brief base type for igx project.
*/
#ifndef __IGX_TYPES_H__
#define __IGX_TYPES_H__

#include <sys/types.h>

/** @brief 64ビット */
typedef long long bit64_t;
/** @brief 32ビット */
typedef int 	bit32_t;
/** @brief 8ビット */
typedef char 	bit8_t;
/** @brief 16ビット */
typedef short	bit16_t;
/** @brief ポインター*/
typedef void*	pointer_t;
/** @brief 64bit unsigned */
typedef unsigned long long  ubit64_t;
/** @brief 32bit unsigned */
typedef unsigned int        ubit32_t;
/** @brief 8bit unsigned */
typedef unsigned char       ubit8_t;
/** @brief 16bit unsigned */
typedef unsigned short      ubit16_t;
/** @brief Cの為のハンドル */
typedef intptr_t* HITX;
/** @brief 整数 */
typedef bit64_t number_t;

#ifndef __cplusplus
/** @brief boolean **/
typedef enum bool_t {
    kFalse=0,
    kTrue=1
} bool_t;

#   ifndef false
#       define false (kFalse)
#       define true  (kTrue)
#   endif
#else
typedef bit32_t bool_t;
#endif

#ifndef WIN32
#   define __POSIX__
#else
#   define OS_IS_WIN32 1
#endif


/* _OS_IS_ =  */
#if OS_IS_MACOSX==1
#   define __BSD__
#elif OS_IS_IOS==1
#   define __BSD__
#elif OS_IS_ANDROID==1
#   define __LINUX__
#endif

#define array_sizeof(a) (sizeof(a)/sizeof(a[0]))


#endif
