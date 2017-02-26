#pragma once

#include "yistd.h"

enum eSeekPos {
	eSP_BEGIN,
	eSP_END,
	eSP_CUR
};

class xEXPORT cyiMemoryIO
{
public:
	cyiMemoryIO(void);
	~cyiMemoryIO(void);

	// ---------------------------------
	Bool Create(int nBufferSize);

	// ---------------------------------
	void Destroy();

	// ---------------------------------
	Bool Load(const char* pszFileName, Bool bIsCompressed=False, const char* pszHeader="");

	// ---------------------------------
	Bool Save(const char* pszFileName, Bool bIsCompressed=False, const char* pszHeader="");

	// ---------------------------------
	int Write(void* buffer, int n);

	// ---------------------------------
	int Read(void* buffer, int n);

	// ---------------------------------
	int Seek(int n, eSeekPos pos=eSP_BEGIN);

	// ---------------------------------
	int SetEnd();

private:
	void InitMember();

	void* m_buffer;
	int   m_nCurrentPos;
	int   m_nSize;
	int	  m_nFullSize;
};
