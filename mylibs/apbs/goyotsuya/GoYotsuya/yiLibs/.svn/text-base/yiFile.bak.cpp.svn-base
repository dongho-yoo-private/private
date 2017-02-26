#include "yiFile.h"
//#include <windows.h>

cyiFile::cyiFile(void)
{
}

cyiFile::~cyiFile(void)
{
}

// +++ +++ +++ +++ +++ +++ +++ +++ +++ 
void* cyiFile::Load(LPCSTR filename, long& size)
{
	HANDLE hFile = ::CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile==NULL)
		return NULL;

	size = (DWORD) ::GetFileSize(hFile, NULL);

	void* buffer = (void*)::GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, size+16);

	if (buffer==NULL)
	{
		CloseHandle(hFile);
		return NULL;
	}


	DWORD nRead=0;
	if (::ReadFile(hFile, buffer, size, &nRead, NULL)==FALSE)
	{
		CloseHandle(hFile);
		return NULL;
	}

	CloseHandle(hFile);
	return buffer;
}

// +++ +++ +++ +++ +++ +++ +++ +++ +++ 
void cyiFile::UnLoad(void* p)
{
	GlobalFree(p);
}

// +++ +++ +++ +++ +++ +++ +++ +++ +++ 
bool cyiFile::Read(LPCSTR filename, void* buffer, int size)
{
	return cyiFile::Read(filename, buffer, 0, size);
}

// +++ +++ +++ +++ +++ +++ +++ +++ +++ 
bool cyiFile::Read(LPCSTR filename, void* buffer, long index, int size)
{
	HANDLE hFile = ::CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile==NULL)
		return false;

	if (index!=0)
	{
		DWORD filep = ::SetFilePointer(hFile, index, NULL, FILE_BEGIN);
		if (filep!=index)
		{
			CloseHandle(hFile);
			return false;
		}
	}


	DWORD nRead=0;
	if (::ReadFile(hFile, buffer, size, &nRead, NULL)==FALSE)
	{
		CloseHandle(hFile);
		return false;
	}

	CloseHandle(hFile);
	return true;
}

// +++ +++ +++ +++ +++ +++ +++ +++ +++ 
bool cyiFile::Write(LPCSTR filename, void* buffer, int size)
{
	return cyiFile::Write(filename, buffer, 0, size);
}

// +++ +++ +++ +++ +++ +++ +++ +++ +++ 
bool cyiFile::Append(LPCSTR filename, void* buffer, int size)
{
	return cyiFile::Write(filename, buffer, -1, size);
}

// +++ +++ +++ +++ +++ +++ +++ +++ +++ 
bool cyiFile::Write(LPCSTR filename, void* buffer, int index, int size)
{
	 HANDLE hFile = ::CreateFile(filename, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile==NULL)
			return false;

	if (index!=0)
	{
		DWORD filep = ::SetFilePointer(hFile, index, NULL, FILE_BEGIN);

		if (filep!=index)
		{
			CloseHandle(hFile);
			return false;
		}
	}
	else if (index==-1) // for append
	{
		DWORD filep = ::SetFilePointer(hFile, 0, NULL, FILE_END);
	}

	DWORD nWrite=0;
	if (::WriteFile(hFile, buffer, size, &nWrite, NULL)==FALSE)
	{
		CloseHandle(hFile);
		return false;
	}

	::SetEndOfFile(hFile);
	::CloseHandle(hFile);
	return true;
}

// +++ +++ +++ +++ +++ +++ +++ +++ +++ 
bool cyiFile::OpenFileDialog(HWND hWndParent, LPCSTR title, LPCSTR initPath, LPCSTR Filters, LPCSTR FilterLabel, LPSTR out_filename)
{
	 OPENFILENAME    ofn;

	 memset(&ofn,0,sizeof(OPENFILENAME));

	 ofn.lStructSize= sizeof(OPENFILENAME);
	 ofn.hwndOwner= hWndParent;
	 ofn.lpstrFilter= Filters;  // "txt(*.txt)\0*.txt\0All files(*.*)\0*.*\0\0"
	 ofn.lpstrFile= out_filename;
	 ofn.lpstrFileTitle= NULL;
	 ofn.nMaxFile= MAX_PATH;
	 ofn.Flags= OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER;
	 ofn.lpstrDefExt= FilterLabel;
	 ofn.lpstrTitle= title;
	 ofn.lpstrInitialDir= initPath;

	 if (GetOpenFileName(&ofn)!=TRUE)
	 {   
		return FALSE;
	 }
	 return TRUE;
}

// +++ +++ +++ +++ +++ +++ +++ +++ +++ 
LPCSTR cyiFile::GetFileName(LPCSTR fullpath, LPSTR path)
{
	size_t n = ::strlen(fullpath);
	char* pIndex = (char*)&fullpath[n-1];
	int cnt=n;
	
	while(cnt--)
	{
		if (*pIndex==L'\\')
		{
			pIndex++;
			break;
		}

		pIndex--;
	}

	return (LPCSTR)pIndex;
}