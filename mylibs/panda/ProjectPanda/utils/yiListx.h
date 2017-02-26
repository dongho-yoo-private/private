#ifndef _yilistx_h__
#define _yilistx_h__

#include "yi.h"

typedef Handle HandleList;

typedef struct _syiNode{
	union {
		IntPtr n;
		void* ref;
	};
	struct _syiNode* pp;
	struct _syiNode* np;
} syiNode;

#ifndef EXTERN
#	ifdef __cplusplus
#		define EXTERN extern "C" //__declspec(dllexport)
#	else
#		define EXTERN extern //__declspec(dllexport)
#	endif
#	ifdef _WIN32
#		ifndef STATIC_COMPILE
#			undef EXTERN
#			ifdef __cplusplus
#				define EXTERN extern "C" __declspec(dllexport)
#			else
#				define EXTERN extern __declspec(dllexport)
#			endif
#		endif
#	endif
#endif

// -------------------------------
EXTERN HandleList yiListCreate(Bool bUseMutex, int nSpinCount);

// -------------------------------
EXTERN void yiListDestroy(HandleList hList);

// -------------------------------
EXTERN int yiListAdd(HandleList hList, IntPtr anyValue);

#define yimListAdd(a, b) yiListAdd(a, (IntPtr)b)

// -------------------------------
EXTERN int yiListInsert(HandleList hList, int index, IntPtr anyValue);

// -------------------------------
EXTERN IntPtr yiListRemoveEx(HandleList hList, int index);

// -------------------------------
EXTERN void yiListRemove(HandleList hList, syiNode* node);

// -------------------------------
EXTERN void yiListClear(HandleList hList);

// -------------------------------
EXTERN syiNode* yiListGetTopFrom(HandleList hList, int index);

// -------------------------------
EXTERN syiNode* yiListGetTailFrom(HandleList hList, int index);

// -------------------------------
EXTERN int yiListCount(HandleList hList);


#endif
