#include "svrybase.h"
#include "yistd.h"
#include "yithread.h"
#include <string.h>

// -------------------------------------
void csvrybase::OnCreditWrite(unsigned long param1, unsigned long param2, unsigned long param3)
{
	char* p = (char*)param1;

	if (yiFileCreditWrite(p,  &p[__MAX_PATH__+(sizeof(int)<<1)], (int)__p8to32__(p, __MAX_PATH__), __p8to32__(p, __MAX_PATH__+sizeof(int)))!=eFE_OK)
	{
		m_FileErrorList.lock();
		m_FileErrorList.addref(p);
		m_FileErrorList.unlock();
		return ;
	}

	yifree(p);
	return ;
}

// -------------------------------------
void csvrybase::OnCreditAppendRecord(unsigned long param1, unsigned long param2, unsigned long param3)
{
	char* p = (char*)param1;

	if (yiFileCreditAppendRecord(p,  &p[__MAX_PATH__+(sizeof(int)<<1)], (int)__p8to32__(p, __MAX_PATH__), __p8to32__(p, __MAX_PATH__+sizeof(int)))!=eFE_OK)
	{
		m_FileErrorList.lock();
		m_FileErrorList.addref(p);
		m_FileErrorList.unlock();

		return ;
	}

	yifree(p);
	return ;
}

// -------------------------------------
Bool csvrybase::CreditWrite(const char* pszFileName, const void* buffer, int size, int check_sum_interval)
{
	char* pDatas = (char*)yialloc(size+__MAX_PATH__+sizeof(int)+sizeof(int));
	char* p=pDatas;

	if (pDatas==0)
	{
		return False;
	}

	strncpy(p, pszFileName, __MAX_PATH__);
	p+=__MAX_PATH__;
	memcpy(p, &size, sizeof(int));
	p+=sizeof(int);
	memcpy(p, &check_sum_interval, sizeof(int));
	p+=sizeof(int);
	memcpy(p, buffer, size);

	if (yiThreadStartEx(this, &csvrybase::OnCreditWrite, pDatas)== -1)
	{
		return False;
	}
	return True;
}

// -------------------------------------
Bool csvrybase::CreditAppendRecord(const char* pszFileName, const void* buffer, int size, int check_sum_interval)
{
	char* pDatas = (char*)yialloc(size+__MAX_PATH__+sizeof(int)+sizeof(int));
	char* p=pDatas;

	if (pDatas==0)
	{
		return False;
	}

	memcpy(p, pszFileName, __MAX_PATH__);
	p+=__MAX_PATH__;
	memcpy(p, &size, sizeof(int));
	p+=sizeof(int);
	memcpy(p, &check_sum_interval, sizeof(int));
	p+=sizeof(int);
	memcpy(p, buffer, size);

	if (yiThreadStartEx(this, &csvrybase::OnCreditAppendRecord, pDatas)== -1)
	{
		return False;
	}
	return True;
}


