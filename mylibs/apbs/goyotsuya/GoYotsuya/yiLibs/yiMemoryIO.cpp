#include "yiMemoryIO.h"

cyiMemoryIO::cyiMemoryIO(void)
{
	InitMember();
}

cyiMemoryIO::~cyiMemoryIO(void)
{
}

void cyiMemoryIO::InitMember()
{
	m_buffer = 0;
	m_nCurrentPos = 0;
	m_nSize = 0;
	m_nFullSize = 0;
}

// ---------------------------------
Bool cyiMemoryIO::Create(int nBufferSize)
{
	if (m_nFullSize==0)
		m_nFullSize+=4096;

	m_buffer = yialloc(m_nFullSize);

	return m_buffer ? True:False;
}

// ---------------------------------
void cyiMemoryIO::Destroy()
{
	yifree(m_buffer);
}

#include "yiEncoder.h"
// ---------------------------------
Bool cyiMemoryIO::Load(const char* pszFileName, Bool bIsCompressed/*=False*/, const char* pszHeader/*=""*/)
{
	int size;
	void* p = yiFileLoad(pszFileName, &size);
	char* pIndex = (char*)p;
	void* pBuffer;

	if (p==0)
		return False;
	
	if (bIsCompressed==True)
	{
		int len = strlen(pszHeader);
		unsigned long* size = (unsigned long*)&pIndex[len];
		//int out_size;

		if (memcmp(p, pszHeader, len)!=0)
		{
			yiFileUnload(p);
			return False;
		}

		pBuffer = yialloc(*size+1);
		//if (cyiEncoder::UnCompress(pBuffer, &pIndex[len+sizeof(unsigned long)], (int)(*size-len-sizeof(unsigned long)), out_size)==False)
		//{
		//	yiFileUnload(p);
		//	return False;
		//}
		memcpy(pBuffer, &pIndex[len+sizeof(unsigned long)], *size);
		m_nSize=*size;
		m_nFullSize=*size;

		yiFileUnload(p);
		p=pBuffer;
	}

	if (m_buffer)
		yifree(m_buffer);

	m_buffer=p;
	return True;
}


// ---------------------------------
Bool cyiMemoryIO::Save(const char* pszFileName, Bool bIsCompressed/*=False*/, const char* pszHeader/*=""*/)
{
	if (bIsCompressed==False)
	{
		if (yiFileWrite(pszFileName, m_buffer, 0, m_nSize)!=True)
		{
			return False;
		}
	}

	char* pCompressed = (char*)yialloci(m_nSize+strlen(pszHeader)+sizeof(unsigned long));
	int len = strlen(pszHeader);
	strncpy(pCompressed, pszHeader, len);
	char* pBuffer = &pCompressed[len+sizeof(unsigned long)];

	int* out_size=(int*)&pCompressed[len];

	//if (cyiEncoder::Compress(pBuffer, this->m_buffer, m_nSize, *out_size)==True)
	//{
	//	//goto WRITE;
	//}

	//if (yiFileWrite(pszFileName, pCompressed, 0, m_nSize+len+sizeof(unsigned long))==True)
	//	return True;

	// ‚Æ‚è‚ ‚¦‚¸
	*out_size=m_nSize;
	memcpy(pBuffer, m_buffer, m_nSize);
	if (yiFileWrite(pszFileName, pCompressed, 0, m_nSize+len+sizeof(unsigned long))==True)
	{
		yifree(pCompressed);
		return True;
	}

	yifree(pCompressed);
	return False;
}

// ---------------------------------
int cyiMemoryIO::Write(void* buffer, int n)
{
	char*pIndex=(char*)m_buffer;

	if (n>(m_nFullSize-m_nCurrentPos))
	{
		m_buffer = yirealloc(m_buffer, (m_nFullSize=m_nFullSize+(n>4096 ? n:4096)));
		pIndex=(char*)m_buffer;
	}

	memcpy(&pIndex[m_nCurrentPos], buffer, n);
	m_nCurrentPos+=n;

	m_nSize=m_nSize>m_nCurrentPos?m_nSize:m_nCurrentPos;

	return n;
}

// ---------------------------------
int cyiMemoryIO::Read(void* buffer, int n)
{
	char*pIndex=(char*)m_buffer;

	if (n>(m_nFullSize-m_nCurrentPos))
	{
		n=m_nFullSize-m_nCurrentPos;
	}
	memcpy(buffer, &pIndex[m_nCurrentPos], n);
	m_nCurrentPos+=n;
	return n;
}

// ---------------------------------
int cyiMemoryIO::Seek(int n, eSeekPos pos/*=eSP_BEGIN*/)
{
	if (pos==eSP_BEGIN)
		m_nCurrentPos=n;
	else if (pos==eSP_END)
		m_nCurrentPos=(m_nSize-n);
	else if (pos==eSP_CUR)
		m_nCurrentPos=m_nCurrentPos+n;

	return m_nCurrentPos;

}

// ---------------------------------
int cyiMemoryIO::SetEnd()
{
	return m_nSize=m_nCurrentPos;
}
