#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "sycgibase.h"
#include "yistd.h"

// -----------------------
csycgibase::csycgibase()
{
	m_nLen=0;
	m_pBuffer=0;
}

// -----------------------
csycgibase::~csycgibase()
{
}


// -----------------------
Bool csycgibase::start(char* buffer, int nBufferSize)
{
	const char* pszMethod = getenv("REQUEST_METHOD");

	if (pszMethod==0)
	{
		return False;
	}

	if (memcmp(pszMethod, "GET", 3)==0)
	{
		const char* pszQueryStr = getenv("QUERY_STRING");

		if (pszQueryStr ==0)
		{
			return False;
		}
		m_nLen = strlen(pszQueryStr);
		m_pBuffer = buffer;
		strncpy(m_pBuffer, pszQueryStr, nBufferSize);
	}
	else
	{
		const char* pszLen = getenv("CONTENT_LENGTH");
		if (pszLen==0)
		{
			return False;
		}

		m_nLen = atoi(pszLen);

		if (m_nLen >nBufferSize-1)
		{
			return False;
		}

		m_pBuffer = buffer;
	
		if (read(0, buffer, m_nLen)!=m_nLen)
		{
			return False;
		}
	
		m_pBuffer[m_nLen]=0;
	}
}

// -----------------------
static void __convert__(char* buffer, const char* p)
{
	char* pIndex = (char*)p;

	while(*pIndex)
	{
		if (*pIndex=='%')
		{
			int n1 = (int)(pIndex[1]-'0');

			if (n1<0 || n1>9)
			{
				n1 = (int)(pIndex[1]-'A')+10;
			}

			int n2 = (int)(pIndex[2]-'0');

			if (n2<0 || n2>9)
			{
				n2 = (int)(pIndex[2]-'A')+10;
			}

			*buffer = (char)((n1<<4)|n2);
			
			pIndex++;
			pIndex++;
			pIndex++;
			buffer++;
			continue;
		}

		*buffer++=*pIndex++;
	}

	*buffer=0;
}

#define MAX_VALUE_STR 512

// -----------------------
Bool csycgibase::GetValue(const char* name, char* pszValue, Bool bIsConvert)
{
	char* p = strstr(m_pBuffer, name);

	if (p==0)
	{
		return False;
	}
	p+=strlen(name);

	if (bIsConvert==False)
	{
		yistrcpych(pszValue, p, '&');
		return True;
	}

	{
		char buffer[MAX_VALUE_STR];
		yistrcpych(buffer, p, '&');

		__convert__(pszValue, buffer);
	}	

	return True;
}

