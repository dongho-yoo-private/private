 
#pragma once
#include <windows.h>

class cyiFile
{
public:
 cyiFile(void);
 ~cyiFile(void);

 // +++ +++ +++ +++ +++ +++ +++ +++ +++ 
 static void* Load(LPCSTR filename, long& size);

 // +++ +++ +++ +++ +++ +++ +++ +++ +++ 
 static void UnLoad(void* p);

 // +++ +++ +++ +++ +++ +++ +++ +++ +++ 
 static bool Read(LPCSTR filename, void* buffer, int size);

 // +++ +++ +++ +++ +++ +++ +++ +++ +++ 
 static bool Read(LPCSTR filename, void* buffer, long index, int size);

 // +++ +++ +++ +++ +++ +++ +++ +++ +++ 
 static bool Write(LPCSTR filename, void* buffer, int size);

 // +++ +++ +++ +++ +++ +++ +++ +++ +++ 
 static bool Append(LPCSTR filename, void* buffer, int size);

 // +++ +++ +++ +++ +++ +++ +++ +++ +++ 
 static bool Write(LPCSTR filename, void* buffer, int index, int size);

 // +++ +++ +++ +++ +++ +++ +++ +++ +++ 
 static bool OpenFileDialog(HWND hWndParent, LPCSTR title, LPCSTR initPath, LPCSTR Filters, LPCSTR FilterLabel, LPSTR out_filename);
// +++ +++ +++ +++ +++ +++ +++ +++ +++ 
static LPCSTR GetFileName(LPCSTR fullpath, LPSTR path=NULL);



};

