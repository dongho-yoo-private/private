#include "yiShmObject.h"
#include "yiUtil.h"

// -------------------------------
typedef struct xsSharedMemory {
	HANDLE hFileMap;
	HANDLE hFile;
	HANDLE hMutex;
	HANDLE hEvent;
	char* buffer;
	int nFileSize;
} sSharedMemory;

// -------------------------------
typedef struct xsSharedMemoryFormat {
	int top;
	int bottom;
	int size;
	int remain;
} sSharedMemoryFormat;



// -----------------------------------
HSHAREDMEMORY CreateSharedMemory(const char* name, int nMemorySize, BOOL bUseMutex, BOOL bUseLikeMessage, const char* pszFileName)
{
	SECURITY_ATTRIBUTES sa;
	sSharedMemory* mem = 0;
	int nTotalMemorySize = 0;

	if (yiNullDACL(&sa)==False)
		return 0;

	mem = (sSharedMemory*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(sSharedMemory));
	nTotalMemorySize = nMemorySize;
	if (mem==0)
	{
		return INVALID_HANDLE_VALUE;
	}

	if (bUseLikeMessage==TRUE)
	{
		char eventname[256];
		wsprintfA(eventname, "%s.event", name);
		mem->hEvent = CreateEventA(&sa, FALSE, FALSE, eventname);

		if (mem->hEvent==0)
		{
			goto _ERROR_;
		}

		nTotalMemorySize+=sizeof(sSharedMemoryFormat);
	}
	
	if (bUseMutex==TRUE)
	{
		char mutexname[256];
		wsprintfA(mutexname, "%s.mutex", name);

		mem->hMutex = CreateMutexA(&sa, FALSE, mutexname);
		if (mem->hMutex==0)
		{
			goto _ERROR_;
		}
	}

	if (pszFileName)
	{
		mem->hFile = CreateFileA(pszFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (mem->hFile==INVALID_HANDLE_VALUE)
		{
			goto _ERROR_;
		}
		SetFilePointer(mem->hFile, 0, 0, FILE_BEGIN);
		mem->nFileSize=nMemorySize+sizeof(sSharedMemoryFormat);
		goto SKIP_FILE_MAPPING;
	}

	mem->hFileMap = CreateFileMappingA((HANDLE)0xffffffff,
										&sa,
										PAGE_READWRITE | SEC_COMMIT,
										0, nTotalMemorySize, 
										name);
	if (mem->hFileMap==0 ||
			mem->hFileMap==INVALID_HANDLE_VALUE)
	{
		goto _ERROR_;
	}

SKIP_FILE_MAPPING:
	if (mem->hFileMap==0)
	{
		mem->buffer = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(sSharedMemoryFormat)+nMemorySize);
	}
	else
	{
		mem->buffer = (char*)MapViewOfFile(mem->hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, nTotalMemorySize);
	}


	if (bUseLikeMessage==TRUE)
	{
		sSharedMemoryFormat* p = (sSharedMemoryFormat* )mem->buffer;

		p->top=0;
		p->bottom=0;
		p->size=nMemorySize;
		p->bottom=0;
		p->remain = nMemorySize;

		if (mem->hFileMap==0)
		{
			DWORD n;
			WriteFile(mem->hFile, p, nTotalMemorySize+sizeof(sSharedMemoryFormat), &n, 0);
			SetEndOfFile(mem->hFile);
		}
	}

	yiDestroyNullDACL(&sa);

	return (HSHAREDMEMORY)mem;

_ERROR_:
	if (mem->hEvent)
		CloseHandle(mem->hEvent);
	if (mem->hMutex)
		CloseHandle(mem->hMutex);

	if (mem->hFile)
		CloseHandle(mem->hFile);

	yiDestroyNullDACL(&sa);
	HeapFree(GetProcessHeap(), 0, mem);
	return INVALID_HANDLE_VALUE;

}

// -----------------------------------
HSHAREDMEMORY OpenSharedMemory(const char* name, int nMemorySize, BOOL bUseMutex, BOOL bUseLikeMessage, const char* pszFileName)
{
	sSharedMemory* mem = (sSharedMemory*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(sSharedMemory));
	int nTotalMemorySize = nMemorySize;

	if (mem==0)
	{
		return INVALID_HANDLE_VALUE;
	}

	if (bUseLikeMessage==TRUE)
	{
		char eventname[256];
		wsprintfA(eventname, "%s.event", name);
		mem->hEvent = OpenEventA(EVENT_ALL_ACCESS|SYNCHRONIZE, FALSE, eventname);

		if (mem->hEvent==0)
		{
			goto _ERROR_;
		}
		nTotalMemorySize+=sizeof(sSharedMemoryFormat);
	}

	if (bUseMutex==TRUE)
	{
		char mutexname[256];
		wsprintfA(mutexname, "%s.mutex", name);

		mem->hMutex = OpenMutexA(SYNCHRONIZE, TRUE, mutexname);
		if (mem->hMutex==0)
		{
			goto _ERROR_;
		}
	}

	if (pszFileName)
	{		
		mem->hFile = CreateFileA(pszFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (mem->hFile==INVALID_HANDLE_VALUE)
		{
			goto _ERROR_;
		}
		SetFilePointer(mem->hFile, 0, 0, FILE_BEGIN);
		mem->nFileSize=nMemorySize+sizeof(sSharedMemoryFormat);

		goto SKIP_FILE_MAPPING;
	}
	mem->hFileMap = OpenFileMappingA(FILE_MAP_ALL_ACCESS, TRUE, name);

	if (mem->hFileMap==0)
	{
		goto _ERROR_;
	}

SKIP_FILE_MAPPING:
	if (mem->hFileMap)
	{
		mem->buffer = (char*)MapViewOfFile(mem->hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, nMemorySize);
	}
	else
	{
		mem->buffer = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(sSharedMemoryFormat)+nMemorySize);
	}


	return (HSHAREDMEMORY)mem;

_ERROR_:
	HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, mem);
	return INVALID_HANDLE_VALUE;
}

// -----------------------------------
void* GetPtr(HSHAREDMEMORY hShmem)
{
	sSharedMemory* s = (sSharedMemory*)hShmem;

	if (s->hMutex!=0)
	{
		WaitForSingleObject(s->hMutex, INFINITE);
	}

	return s->buffer;
}

// -----------------------------------
void ReleasePtr(HSHAREDMEMORY hShmem)
{
	sSharedMemory* s = (sSharedMemory*)hShmem;
	if (!s->hMutex)
		return ;

	ReleaseMutex(s->hMutex);
}


// -----------------------------------
static BOOL __memcat__(char* _buffer, void* data, int size)
{
	sSharedMemoryFormat* p = (sSharedMemoryFormat*)_buffer;
	char* buffer = &_buffer[sizeof(sSharedMemoryFormat)];
	char* pCurr = &buffer[p->bottom];
	char* pEnd	= &buffer[p->size];
	char* pData;
	int i=0;

	if (size+(int)sizeof(int) > p->remain)
	{
		return FALSE;
	}
	//int remain = (p->bottom-p->top > 0) ? (p->size-(p->bottom-p->top)):p->top-p->bottom;

	//// buffer is full.
	//if (size+(int)sizeof(int) > remain)
	//{
	//	return FALSE;
	//}

	pData = (char*)HeapAlloc(GetProcessHeap(),0 ,size+sizeof(int));
	memcpy(pData, &size, sizeof(int));
	memcpy(&pData[sizeof(int)], data, size);

	while(i!=(size+sizeof(int)))
	{
		*pCurr = pData[i++];
		pCurr=&buffer[++(p->bottom)];
		if (pCurr==pEnd)
		{
			p->bottom=0;
			pCurr=buffer;
		}
	}
	p->remain-=(size+sizeof(int));
	HeapFree(GetProcessHeap(), 0, pData);
	return TRUE;
}

// -----------------------------------
static BOOL __memget__(char* _buffer, void* rcv, int* size)
{
	sSharedMemoryFormat* p = (sSharedMemoryFormat*)_buffer;
	char* buffer = &_buffer[sizeof(sSharedMemoryFormat)];
	char* pCurr = &buffer[p->top];
	char* pEnd	= &buffer[p->size];
	int nDataSize;
	char* pDataSize = (char*)&nDataSize;
	char* pRcvBuffer = (char*)rcv;
	int i=0;

	if (p->size==p->remain)
	{
		*size=0;
		return FALSE;
	}

	for (i=0; i<sizeof(int); i++)
	{
		pDataSize[i]=*pCurr;
		
		pCurr = &buffer[++p->top];

		if (pCurr==pEnd)
		{
			p->top=0;
			pCurr=buffer;
		}
	}

	for (i=0; i<nDataSize; i++)
	{
		*pRcvBuffer=*pCurr;

		pRcvBuffer++;
		pCurr = &buffer[++p->top];

		if (pCurr==pEnd)
		{
			p->top=0;
			pCurr=buffer;
		}
	}

	*size = nDataSize;
	p->remain+=(nDataSize+sizeof(int));

	return TRUE;
}

// -----------------------------------
BOOL csmSendMsg(HSHAREDMEMORY hShmem, void* data, int size)
{
	sSharedMemory* s = (sSharedMemory*)hShmem;
	BOOL res;

	if (hShmem==INVALID_HANDLE_VALUE || 
			hShmem==NULL)
	{
		return FALSE;
	}

	if (s->hMutex)
	{
		if (WaitForSingleObject(s->hMutex, INFINITE)==WAIT_ABANDONED)
		{
			return FALSE;
		}
	}

	if (s->hFileMap)
	{
		res = __memcat__(s->buffer, data, size);
	}
	else
	{
		DWORD writes;
		DWORD xsize = s->nFileSize;

		SetFilePointer(s->hFile, 0, 0, FILE_BEGIN);
		if (ReadFile(s->hFile, s->buffer, xsize, &writes, 0)==FALSE)
		{
			res=FALSE;
			goto RET;
		}
		res = __memcat__(s->buffer, data, size);

		SetFilePointer(s->hFile, 0, 0, FILE_BEGIN);
		if (WriteFile(s->hFile, s->buffer, xsize, &writes, 0)==FALSE)
		{
			res=FALSE;
			goto RET;
		}
	}
	
RET:
	if (ReleaseMutex(s->hMutex)==FALSE)
	{
		return FALSE;
	}

	if (SetEvent(s->hEvent)==FALSE)
	{
		return FALSE;
	}

	return res;
}

// -----------------------------------
BOOL csmRcvMsg(HSHAREDMEMORY hShmem, void* data, int* size)
{
	sSharedMemory* s = (sSharedMemory*)hShmem;
	BOOL res;

WAIT:
	if (csmBufferEmpty(hShmem)==TRUE)
	{
		if (s->hEvent)
		{
			WaitForSingleObject(s->hEvent, INFINITE);
		}
	}

	if (s->hMutex)
	{
		WaitForSingleObject(s->hMutex, INFINITE);
	}

	if (s->hFileMap)
	{
		res = __memget__(s->buffer, data, size);
	}
	else
	{
		DWORD reads;
		DWORD xsize = s->nFileSize;
		SetFilePointer(s->hFile, 0, 0, FILE_BEGIN);
		if (ReadFile(s->hFile, s->buffer, xsize, &reads, 0)==FALSE)
		{
			res=FALSE;
			goto RET;
		}
		res = __memget__(s->buffer, data, size);

		SetFilePointer(s->hFile, 0, 0, FILE_BEGIN);
		if (WriteFile(s->hFile, s->buffer, xsize, &reads, 0)==FALSE)
		{
			res=FALSE;
			goto RET;
		}
	}

	if (*size==0)
	{
		ReleaseMutex(s->hMutex);
		goto WAIT;
	}
RET:
	ReleaseMutex(s->hMutex);

	return res;
}

// -----------------------------------
void CloseSharedMemory(HSHAREDMEMORY hShmem)
{
	sSharedMemory* s = (sSharedMemory*)hShmem;

	if (s==0)
		return ;

	if (s->hEvent)
		CloseHandle(s->hEvent);

	if (s->hMutex)
		CloseHandle(s->hMutex);


	if (s->hFile)
	{
		HeapFree(GetProcessHeap(), 0, s->buffer);
		CloseHandle(s->hFile);
	}

	if (s->hFileMap)
	{
		UnmapViewOfFile(s->buffer);
		CloseHandle(s->hFileMap);
	}

	HeapFree(GetProcessHeap(),0 ,s);
	return ;
}

// -----------------------------------
BOOL csmBufferClear(HSHAREDMEMORY hShmem)
{
	sSharedMemory* s = (sSharedMemory*)hShmem;
	sSharedMemoryFormat* fmt;
	sSharedMemoryFormat bf;

	if (s==0)
		return FALSE;

	if (s->hMutex)
		WaitForSingleObject(s->hMutex, INFINITE);

	if (s->hFileMap)
	{
		fmt = (sSharedMemoryFormat*)s->buffer;
	}
	else
	{
		DWORD reads;
		SetFilePointer(s->hFile, 0, 0, FILE_BEGIN);
		ReadFile(s->hFile, &bf, sizeof(bf), &reads, 0);
		fmt = &bf;
	}
	
	fmt->remain=fmt->size;
	fmt->top=0;
	fmt->bottom=0;

	if (s->hFileMap==0)
	{
		DWORD writes;
		SetFilePointer(s->hFile, 0, 0, FILE_BEGIN);
		WriteFile(s->hFile, &bf, sizeof(bf), &writes, 0);
	}
	
	ReleaseMutex(s->hMutex);
	return FALSE;
}

// -----------------------------------
BOOL csmBufferEmpty(HSHAREDMEMORY hShmem)
{
	sSharedMemory* s = (sSharedMemory*)hShmem;
	sSharedMemoryFormat* fmt;
	sSharedMemoryFormat bf;

	if (s==0)
		return FALSE;

	if (s->hMutex)
		WaitForSingleObject(s->hMutex, INFINITE);

	if (s->hFileMap)
	{
		fmt = (sSharedMemoryFormat*)s->buffer;
	}
	else
	{
		DWORD reads;
		SetFilePointer(s->hFile, 0, 0, FILE_BEGIN);
		ReadFile(s->hFile, &bf, sizeof(bf), &reads, 0);
		fmt = &bf;
	}
	
	if (fmt->size==fmt->remain)
	{
		ReleaseMutex(s->hMutex);
		return TRUE;
	}
	
	ReleaseMutex(s->hMutex);
	return FALSE;
}