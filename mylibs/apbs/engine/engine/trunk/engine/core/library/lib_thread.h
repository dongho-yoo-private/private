/**
 * \author dongho yoo
 * \file LIB_Thread.h
 * \~english
 * \~japanese
 * 	\brief 
*/

#ifndef __LIB_THREAD_H__
#define __LIB_THREAD_H__

#include <core/types.h>

#define __USE_PTHREAD__

ADK_BEGIN_NAMESPACE

ADK_HANDLE(HandleMutex);

//#ifdef _IOS
/** @brief LIB_Thread生成時のコールバックの型
*/
typedef void (*LIB_TThreadCallback)(void*);
//#endif

/**@brife スレッドを生成します。
*
* @param LIB_TThreadCallback形式のcallback関数のアドレス
* @param param コールバックに渡される引数
* @param isJoinable trueの場合、Joinalbeになります。falseの場合はスレッド終了時にリソースは自動的に解放されます。
*/
IntPtr LIB_ThreadCreate(LIB_TThreadCallback pfnCallback, void* param, Bool isJoinable);

/**@brife Joinableなスレッドを待ちます。
*
* @param threadId LIB_ThreadCreateで生成されたID\n
* LIB_ThreadCreate時にisJoinableがtrueである場合には必ずLIB_ThreadJoinで終了してください。\n
* isJoinableがfalseだった場合は、falseが返ります。
*
* @return 成功した場合はtrue、失敗した場合はfalseを返します。
*/
Bool LIB_ThreadJoin(IntPtr threadId);

/**@brife スレッドIDを取得します。
*
* @param id LIB_ThreadCreateで生成されたID, 0指定の場合現在のスレッドIDが返ります。
*
* @return 指定したハンドルのスレッドID, 0指定時は現在のスレッドID
*/
U32 LIB_ThreadGetThreadId(U32 id);


/**@brife Mutexを生成します。
*
* @return mutexハンドル
*/
HandleMutex LIB_MutexCreate();

/** \brief Mutexのハンドルのサイズ
*/
S32 LIB_MutexGetHandleSize();

/**@brife Mutexを生成します。
* @param buffer ハンドルを格納するバッファー,バッファーサイズはLIB_MutexGetHandleSize()で取得したサイズでなければ、なりません。
* @return mutexハンドル
*/
HandleMutex LIB_MutexCreateInBuffer(void* buffer);

/**@brife Mutexのハンドルを解放します。
*
* @return mutexハンドル
*/
void LIB_MutexDestroy(HandleMutex hMutex);

/**@brife Mutexをロックします。
*
* @param hMutex LIB_MutexCreateで生成されたハンドル
* @param try 	trueの場合はロックがかかっているかどうかをみて、ロックがかっていなければロックします。
*
* @return 成功した場合はtrue、失敗した場合はfalseを返します。\n
* tryがtrueの場合は、既にロックがかかっている場合にfalseを返します。
*/
Bool LIB_MutexLock(HandleMutex hMutex, Bool isTry);

/**@brife Mutexをあんロックします。
*
* @param threadId LIB_ThreadCreateで生成されたハンドル
*
* @return 成功した場合はtrue、失敗した場合はfalseを返します。
*/
Bool LIB_MutexUnlock(HandleMutex hMutex);


/** きれいに隠蔽できないのでやめる。
HandleEvent LIB_EventCreate(LIB_TEventType type);

Bool LIB_EventAdd(S32 id);

LiB_EventSet(S32 id, void* p, S32 size);

LIB_TEventResult LIB_EventWait(HandleEvent hEvent, U32* timeout);

S32 LIB_EventCount();

S32 LIB_EventGet(S32 index, void* buffer);




HandleEvent hEvent = LIB_EventCreate(LIB_kEventTypePipe);
LIB_EventAdd(EVENT_ID_ACK);
LIB_EventAdd(EVENT_ID_REQUEST);
LIB_EventAdd(EVENT_ID_QUIT);

ADK_System::GetInstance()->SetEvent(hEvent);

while(1)
{
	U32 timeout=10000; // 10秒
	LIB_TEventWaitResult res = LIB_EventWait(hEvent, &timeout);

	if (res==LIB_kEWR_EVENT)
	{
		int cnt = LIB_EventCount();
		for (int i=0; i<cnt; i++)
		{
			// TODO.
			char buffer[64];
			U32 id = LIB_EventGet(i, buffer);
			switch(id)
			{
				case EVENT_ID_ACK:
					ADK_Request* req = (ADK_Request*)buffer;
				break;
				case EVENT_ID_REQUEST:
					ADK_Request* req = (ADK_Request*)buffer;
				break;
				case EVENT_ID_QUIT:
					if (buffer[0]==0)
					{
						// 正常終了
					}
					else
					{
						// 異常終了
					}
				break;
			}
		}
	}
	else if (res==LIB_kEWR_TIME_OUT)
	{
		// TODO.
		// ack?
	}
	else // error.
	{
		// TODO. 
		System::GetInstance()->DoCriticalQuitProcess();
		break;
	}
}

LIB_EventClose(hEvent);


:
:


// for client

HandleEvent hEvent = ADK_System::GetEvent();

if (hEvent)
{
	ADK_Request* req = new ADK_Request(.....);
	LIB_EventSet(EVENT_ID_REQUESt, req, sizeof(ADK_Request*));
}

*/

ADK_END_NAMESPACE
#endif
