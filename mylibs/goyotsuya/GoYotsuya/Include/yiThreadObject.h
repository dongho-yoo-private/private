#pragma once
#include "yistd.h"

#define THREAD_ANY_ID (-1)

class xEXPORT cyiThreadObject
{
public:
	cyiThreadObject(void);
	virtual ~cyiThreadObject(void);

	// --------------------------------
	Bool StartThread(int id, IntPtr nParam1=0, IntPtr nParam2=0);

	// --------------------------------
	void EndThreadz();


	// --------------------------------
	// res: False -> スレッドが終了していない。
	// タイムアウト
	// nTimeOut==INFINITE ブロック
	Bool WaitThread(int id, int nTimeOut);

	// --------------------------------
	virtual void OnThreadStart(int id, int param1, int param2);

	// --------------------------------
	static unsigned int __stdcall xOnThread(void* param);

private:
	cyiList m_listThread;

};
