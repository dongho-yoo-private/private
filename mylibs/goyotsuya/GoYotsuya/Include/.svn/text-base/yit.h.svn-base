#pragma once

// ----------------------------
#ifdef __cplusplus
#include "yi.h"
#include "yitList.h"
#include "yitRingBuffer.h"
#include "yitTreeNode.h"

// ----------------------------
template <class T> T* yitAlloc(int size=0)
{
	if (size)
		return (T*)yialloci(size);
	return (T*)yialloci(sizeof(T));
}

// ----------------------------
template <class T> T* yitToHeap(T& data)
{
	T* t=(T*)yialloci(sizeof(T));
	memcpy(t, &data, sizeof(T));

	return t;
}

#endif
