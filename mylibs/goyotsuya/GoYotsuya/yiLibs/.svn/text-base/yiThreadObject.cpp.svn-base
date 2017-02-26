
#include "yiThreadObject.h"
#include <process.h>

cyiThreadObject::cyiThreadObject(void)
: m_listThread(True)
{
}

cyiThreadObject::~cyiThreadObject(void)
{
	//EndThreadz();
}

typedef struct xsThreadParam
{
	int id;
	int param1;
	int param2;
	cyiThreadObject* pClass;
	HANDLE hThread;
}sThreadParam;

// --------------------------------
Bool cyiThreadObject::StartThread(int id, IntPtr param1, IntPtr param2)
{
	sThreadParam* param = (sThreadParam* )yialloci(sizeof(sThreadParam));
	unsigned int xid;

	//xTRACE("cyiThreadObject::StartThread(%d) list cnt:%d", id, m_listThread.count());
	if (id==THREAD_ANY_ID)
	{
		goto SKIP_TO_CHECK;
	}

	m_listThread.lock();

	for (cyiNode* node = m_listThread.GetNodeTopFrom(0); node; node=node->np)
	{
		sThreadParam* xparam = (sThreadParam*)node->data;

		if (xparam->id==id)
		{
			m_listThread.unlock();
			xTRACE("cyiThreadObject::StartThread() Already id=%d", id);
			return False;
		}
	}
	m_listThread.unlock();

SKIP_TO_CHECK:

	param->pClass=this;
	param->id=id;
	param->param1=param1;
	param->param2=param2;
	//xTRACE("cyiThreadObject::StartThread(%d) handle %x, class %x, %x", id, param->hThread, this, param->pClass);

	if (id!=THREAD_ANY_ID)
	{
		m_listThread.lock();
		m_listThread.add((yiAnyValue)param);
		m_listThread.unlock();
	}

	if (param==0)
	{
		return False;
	}

	param->hThread = (HANDLE)_beginthreadex(0, 0, cyiThreadObject::xOnThread, param,0, &xid);
	//xTRACE("cyiThreadObject::StartThread(%d) handle %x", id, param->hThread);

	if (param->hThread==0)
	{
		//////xTRACE("cyiThreadObject::StartThread() param->hThread is NULL");
		YIERROR("_beginthreadex failure! error:%d!", GetLastError());
		return False;
	}

	return True;
}

// --------------------------------
void cyiThreadObject::EndThreadz()
{
	if (m_listThread.count()==0)
	{
		return;
	}

	m_listThread.lock();

	for (cyiNode* node = m_listThread.GetNodeTopFrom(0); node; node=node->np)
	{
		sThreadParam* param = (sThreadParam*)node->data;
		m_listThread.unlock();
		DWORD res = WaitForSingleObject(param->hThread, INFINITE);
		m_listThread.lock();
		m_listThread.GetNodeTopFrom();
		//CloseHandle(param->hThread);
	}
	m_listThread.clear();
	m_listThread.unlock();

}

// --------------------------------
void cyiThreadObject::OnThreadStart(int id, int param1,int param2)
{
	return ;
}

// --------------------------------
Bool cyiThreadObject::WaitThread(int id, int nTimeOut)
{
	//xTRACE("cyiThreadObject::WaitThread(%d) TimeOut=%d, %x", id, nTimeOut, this);
	m_listThread.lock();
	//xTRACE("cyiThreadObject::WaitThread(%d) TimeOut=%d Enter", id, nTimeOut);
	for (cyiNode* node = m_listThread.GetNodeTopFrom(0); node; node=node->np)
	{
		sThreadParam* param = (sThreadParam*)node->data;
		////xTRACE("cyiThreadObject::WaitThread(%d) handle %x, %d", id, param->hThread, param->id);
		if (param->id==id)
		{
			m_listThread.unlock();
			DWORD res = WaitForSingleObject(param->hThread, nTimeOut);
			return res==WAIT_OBJECT_0 ? True:False;
		}
	}

	// Šù‚ÉI—¹‚µ‚½ê‡?
	m_listThread.unlock();
	return True;
	
}


// --------------------------------
unsigned int __stdcall cyiThreadObject::xOnThread(void* param)
{
	sThreadParam* p = (sThreadParam*)param;
	//xTRACE("cyiThreadObject::xOnThread called, %x", p->pClass);
	p->pClass->OnThreadStart(p->id, p->param1, p->param2);

	if (p->id==THREAD_ANY_ID)
	{
		CloseHandle(p->hThread);
		yifree(p);
		_endthreadex(0);
		return 0;
	}

	p->pClass->m_listThread.lock();
	for (cyiNode* node = p->pClass->m_listThread.GetNodeTopFrom(0); node; node=node->np)
	{
		sThreadParam* x=(sThreadParam*)node->data;
		if (x->id==p->id)
		{
			p->pClass->m_listThread.remove(node);
			break;
		}
	}
	CloseHandle(p->hThread);
	p->pClass->m_listThread.unlock();

	yifree(p);
	_endthreadex(0);

	return 0;
}