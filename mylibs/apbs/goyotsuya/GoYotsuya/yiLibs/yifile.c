#include "yifile.h"
#include "yistd.h"

int __buffer_size_once = __BUFFERSIZE_PER_ONCE__;

#ifdef __WIN32__
#include "yifile.win32.c"
#else
//#include "yifile.posix.c"
#endif


// --------------------------------------------------------------------
unsigned int yiGetCheckSum(void* buffer, int size, int n)
{
	unsigned char* p = (unsigned char*)buffer;
	unsigned int check_sum=0;

	while(p<(((unsigned char*)buffer)+size))
	{
		check_sum+= (unsigned int)*p;
		p+=n;
	}

	return check_sum;
}


// --------------------------------------------------------------------
void yiSetBuffer(int n)
{
	__buffer_size_once = n;
}



// --------------------------------------------------------------------
Bool yiFileOpenDialog(HWND hWndParent, LPCSTR title, LPCSTR initPath, LPCSTR Filters, LPCSTR FilterLabel, LPSTR out_filename)
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

// --------------------------------------------------------------------
Bool yiFileSaveDialog(HWND hWndParent, LPCSTR title, LPCSTR initPath, LPCSTR Filters, LPCSTR FilterLabel, LPSTR out_filename)
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
	 ofn.Flags = OFN_OVERWRITEPROMPT;

	 if (GetSaveFileName(&ofn)!=TRUE)
	 {   
		return FALSE;
	 }
	 return TRUE;
}


#include <shlobj.h> // このヘッダーのインクルードが必要です
static int _stdcall CallBackProc( HWND hwnd, UINT uiMsg, LPARAM lParam, LPARAM lpData)
{
	if(uiMsg == BFFM_INITIALIZED){
		// 初期化処理
		// カーソルの初期値を入れとく。
		if( lpData != NULL){
			SendMessage( hwnd, BFFM_SETSELECTION, TRUE, lpData );
		}
	}
	return(0);
}




// --------------------------------------------------------------------
xEXTERN_C Bool yiFileOpenPath(HWND hWndParent, LPCSTR title, LPSTR path)
{
	BROWSEINFO bInfo;
	LPITEMIDLIST pIDList;
	TCHAR szDisplayName[MAX_PATH];

    bInfo.hwndOwner             = hWndParent;						// ダイアログの親ウインドウのハンドル
    bInfo.pidlRoot              = 0;                             // ルートフォルダを示すITEMIDLISTのポインタ (NULLの場合デスクトップフォルダが使われます）
    bInfo.pszDisplayName        = path;                    // 選択されたフォルダ名を受け取るバッファのポインタ
    bInfo.lpszTitle             = title;             // ツリービューの上部に表示される文字列 
    bInfo.ulFlags               = BIF_NEWDIALOGSTYLE|BIF_RETURNONLYFSDIRS;             // 表示されるフォルダの種類を示すフラグ
    bInfo.lpfn                  = CallBackProc;                             // BrowseCallbackProc関数のポインタ
    bInfo.lParam                = (LPARAM)path;  	// コールバック関数に渡す値

    // フォルダ選択ダイアログを表示
    pIDList = SHBrowseForFolder(&bInfo);
    if(pIDList == NULL){

        // 戻り値がNULLの場合、フォルダが選択されずにダイアログが閉じられたことを意味します。
		return False;

    }else{

        // ItemIDListをパス名に変換します
        if(!SHGetPathFromIDList(pIDList, path)){
            // エラー処理
			return False;
        }
        // szDisplayNameに選択されたフォルダのパスが入っています


        // 最後にpIDListのポイントしているメモリを開放します
        CoTaskMemFree( pIDList );
    }


	return True;
}

// --------------------------------------------------------------------
LPCSTR yiPathGetFileName(LPCSTR fullpath)
{
	size_t n = strlen(fullpath);
	char* pIndex = (char*)&fullpath[n-1];
	int cnt=n-1;
	
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

// --------------------------------------------------------------------
void yiFileGetNameAndExtW(const wchar_t* fullpath, wchar_t* path, wchar_t* name, wchar_t* ext)
{
	wchar_t* pIndex=(wchar_t*)fullpath;
	int n = wcslen(fullpath);
	int i=0;

	// まずはパス検索
	for ( i=n-1; i>=0; i--)
	{
		if (pIndex[i]=='\\')
		{
			if (path)
			{
				wcsncpy(path,fullpath, i);
				path[i]=0;
			}
			break;
		}
	}

	// 名前検索
	{
		int nNameIndex = i?++i:i;
		for ( i=n-1; i>=0; i--)
		{
			if (pIndex[i]=='.')
			{
				if (name)
				{
					wcsncpy(name, &pIndex[nNameIndex], i-nNameIndex);
					name[i-nNameIndex]=0;
				}
				break;
			}

		}
	}

	// 拡張子検索
	if (ext)
	{
		if (pIndex[i]!='.')
		{
			ext[0]=0;
			return ;
		}

		wcscpy(ext, &pIndex[i+1]);
	}

	return ;
}

#include<Shellapi.h>
#include<shlobj.h>

// --------------------------------------------------------------------
Bool yiRemoveForced(const char* dir, Bool bIsMoveTrush)
{
	int ret=0;
	SHFILEOPSTRUCT s;
	char path[256]={0,};

	ZeroMemory(&s, sizeof(s));
	strcpy(path, dir);



	s.wFunc=FO_DELETE;
	s.pFrom=path;
	s.fFlags = (FOF_NO_UI|FOF_SILENT);

	if (bIsMoveTrush)
	{
		s.fFlags|=FOF_ALLOWUNDO;
	}

	ret = SHFileOperation(&s);

	return ret==0?True:False;
}

// --------------------------------------------------------------------
void yiFileGetNameAndExt(LPCSTR fullpath, LPSTR path, LPSTR name, LPSTR ext)
{
	char* pIndex=(char*)fullpath;
	int n = strlen(fullpath);
	int i=0;

	// まずはパス検索
	for ( i=n-1; i>=0; i--)
	{
		if (pIndex[i]=='\\')
		{
			if (path)
			{
				strncpy(path,fullpath, i);
				path[i]=0;
			}
			break;
		}
	}

	// 名前検索
	{
		int nNameIndex = i?++i:i;
		for ( i=n-1; i>=0; i--)
		{
			if (pIndex[i]=='.')
			{
				if (name)
				{
					strncpy(name, &pIndex[nNameIndex], i-nNameIndex);
					name[i-nNameIndex]=0;
				}
				break;
			}

		}
	}

	// 拡張子検索
	if (ext)
	{
		if (pIndex[i]!='.')
		{
			ext[0]=0;
			return ;
		}

		strcpy(ext, &pIndex[i+1]);
	}

	return ;
}