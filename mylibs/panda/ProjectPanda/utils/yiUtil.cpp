#include "yistd.h"
#include "yiUtil.h"
#include "zlib.h" // /usr/include/zlib

#ifdef _WIN32
#include <SHLOBJ.H>
#pragma comment(lib, "Advapi32.lib")
#endif


#ifdef _WIN32
#define xRegSetString(hKey, key, str) RegSetValueExA(hKey, key, 0, REG_SZ, (const BYTE*)str, strlen(str)) 

// -----------------------------------------------------
Bool yiUtilRegisterSoftware(const char* pszAppRoot, sSoftwareInfo* info, const char* pszExecAppName)
{
	HKEY hSoft=0;
	char szSubKey[256];
	wsprintf(szSubKey, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%s", info->szSoftwareName);
	if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, szSubKey, 0,0 , 0, KEY_ALL_ACCESS, 0, &hSoft, 0)!=ERROR_SUCCESS)
	{
		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, szSubKey, 0, KEY_ALL_ACCESS, &hSoft)!=ERROR_SUCCESS)
		{
			return False;
		}
	}

	xRegSetString(hSoft, "DisplayIcon",pszExecAppName);//, strlen(pszExecAppName)); 
	xRegSetString(hSoft, "DisplayName", info->szSoftwareNameDisplay);//, strlen(pszExecAppName)); 
	xRegSetString(hSoft, "DisplayVersion", info->szVersion);//, strlen(pszExecAppName)); 
	xRegSetString(hSoft, "Publisher", info->szProduct);//, strlen(pszExecAppName)); 

	char szTime[64];
	unsigned int day=0, time = cyiTime::GetLocalTime(day);
	cyiTime::TimeToString(szTime, day, 0, 0, "");
	xRegSetString(hSoft, "InstallDate", szTime);//, strlen(pszExecAppName)); 
	xRegSetString(hSoft, "InstallLocation", pszAppRoot);//, strlen(pszExecAppName)); 

	char szUninstall[256];
	wsprintf(szUninstall, "%s\\UnInstall.exe", pszAppRoot);
	xRegSetString(hSoft, "UninstallString", szUninstall);//, strlen(pszExecAppName)); 

	RegCloseKey(hSoft);

	return True;
}

// -----------------------------------------------------
Bool yiUtilUnRegisterSoftware(sSoftwareInfo* info)
{
	HKEY hSoft=0;
	char szSubKey[256];
	wsprintf(szSubKey, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%s", info->szSoftwareName);
	RegDeleteKey(HKEY_LOCAL_MACHINE, szSubKey);
	return True;
}

// -----------------------------------------------------
Bool yiUtilGetSoftwarePath(char* path, const char* pszAppName)
{
	HKEY hSoft=0;
	DWORD n=256;
	DWORD type=0;

	char szSubKey[256];
	wsprintf(szSubKey, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%s", pszAppName);

	path[0]=0;
	if(RegGetValueA(HKEY_CURRENT_USER, szSubKey, 
		"InstallLocation", RRF_RT_ANY, &type, path, &n)!=ERROR_SUCCESS)
	{
		return False;
	}
	path[n]=0;
	return True;
}

#define BITMAP32(w, h)	yiUtilNewBitmap(w, h, PixelFormat32bppARGB);//new Bitmap(w, h, PixelFormat32bppARGB)
#define DELETEBMP(bmp)	yiUtilDeleteBitmap(bmp);

#include <shellapi.h>


#include "yiencodex.h"
#define SECURE_KEY 0x73D200A4



// -------------------------------
Bool yiSetProperty(const char* pszFileName, const char* key, const char* value)
{
	int n=0;
	int len = strlen(key);
	for (int i=0; i<len; i++)
	{
		n+=(int)key[0]*73;
	}

	char* pKey		= yiencode_str(key, SECURE_KEY|(n&0x0000FF00));
	char* pValue	= yiencode_str(value, SECURE_KEY|(n&0x0000FF00));

	Bool res = WritePrivateProfileStringA("Main Properties", pKey, pValue, pszFileName);

	yifree(pKey);
	yifree(pValue);

	return True;
}

// -------------------------------
Bool yiSetPropertyInt(const char* pszFileName, const char* key, int n)
{
	char value[32];
	int len = strlen(key);
	int k=0;

	wsprintf(value, "%d", n);

	for (int i=0; i<len; i++)
	{
		k+=(int)key[0]*73;
	}

	char* pKey		= yiencode_str(key, SECURE_KEY|(k&0x0000FF00));
	char* pValue	= yiencode_str(value, SECURE_KEY|(k&0x0000FF00));

	Bool res = WritePrivateProfileStringA("Main Properties", pKey, pValue, pszFileName);

	yifree(pKey);
	yifree(pValue);

	return 0;
}

// -------------------------------
Bool yiGetProperty(const char* pszFileName, const char* key, char* value, const char* defaultValue)
{
	int n=0;
	int len = strlen(key);
	for (int i=0; i<len; i++)
	{
		n+=(int)key[0]*73;
	}
	char* pKey		= yiencode_str(key, SECURE_KEY|(n&0x0000FF00));
	char* pDefault  = defaultValue?yiencode_str(defaultValue, SECURE_KEY|(n&0x0000FF00)):0;
	char buffer[256];
	if (GetPrivateProfileString("Main Properties",  pKey, pDefault, buffer, 256, pszFileName)!=0)
	{
		char* p = yidecode_str(buffer);
		strcpy(value, p);

		yifree(p);
		yifree(pKey);
		yifree(pDefault);
		return True;
	}

	yifree(pKey);
	yifree(pDefault);

	return False;
}

// -------------------------------
int yiGetPropertyInt(const char* pszFileName, const char* key, int defaultValue)
{
	char defval[32];
	char ret[256];

	itoa(defaultValue, defval, 10);

	if (yiGetProperty(pszFileName, key, ret, defval)==False)
	{
		return defaultValue;
	}

	if (yiIsNumeric(ret)==True)
	{
		return atoi(ret);
	}

	return defaultValue;
}


// -------------------------------
int yiUtilGetCpuCount()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);

	return info.dwNumberOfProcessors;
}

// -------------------------------
Bool yiUtilSetSystemTray(HWND hWnd, int id,HICON hIcon, unsigned int notifyMsg, const char* tip)
{
	NOTIFYICONDATA data;
	memset(&data, 0, sizeof(data));

    data.cbSize = sizeof( NOTIFYICONDATA) ;
    data.uID = id;
    data.hWnd = hWnd;
    data.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP ;
    data.hIcon = hIcon;
    data.uCallbackMessage = notifyMsg;

	if (tip!=0)
	strcpy(data.szTip, tip);
	return Shell_NotifyIcon( NIM_ADD, &data);
}

// -------------------------------
Bool yiUtilDeleteFromSystemTray(HWND hWnd, int id)
{
	NOTIFYICONDATA notif;

	memset(&notif, 0, sizeof(notif));
	notif.cbSize = sizeof(NOTIFYICONDATA);
	notif.hWnd = hWnd;
	notif.uID = id;
	notif.uFlags = 0;

	return Shell_NotifyIcon(NIM_DELETE, &notif);
}


// -------------------------------
Bool yiUtilOutSystemTrayMessage(HWND hWnd, int id, const char* pszTitle, const char* pszMessage, int iconId)
{
	NOTIFYICONDATA data;
	memset(&data, 0, sizeof(data));

    data.cbSize = sizeof( NOTIFYICONDATA) ;
    data.uID = id;
    data.hWnd = hWnd;
    data.uFlags = NIF_INFO;

	if (pszTitle)
		strcpy(data.szInfoTitle, pszTitle);
	if (pszMessage)
		strcpy(data.szInfo, pszMessage);

	data.dwInfoFlags = iconId;
	return Shell_NotifyIcon(NIM_MODIFY, &data);
}


// -------------------------------
static Bool xRegDeleteKeySub(HKEY hKey, const char* pszSubKey)
{
	HKEY hKeySub = 0;
	int cnt=0;
	Bool bIsSuccess=True;
	
	if (::RegOpenKeyEx(hKey, pszSubKey, KEY_ALL_ACCESS, 0, &hKeySub)!=ERROR_SUCCESS)
	{
		return False;
	}

	while(1)
	{
		FILETIME time;
		DWORD size=64;
		char szSubKey[128];

		if (RegEnumKeyExA(hKeySub, cnt++, szSubKey, &size, 0, 0, 0, &time)!=ERROR_SUCCESS)
		{
			break;
		}
		bIsSuccess = xRegDeleteKeySub(hKeySub, szSubKey);
	}

	for (int i=0; i<cnt; i++)
	{
		FILETIME time;
		DWORD size=64;
		char szSubKey[128];

		if (RegEnumKeyExA(hKeySub, 0, szSubKey, &size, 0, 0, 0, &time)!=ERROR_SUCCESS)
		{
			break;
		}
		if (RegDeleteKey(hKeySub, szSubKey)/*, KEY_WOW64_32KEY, 0)*/!=ERROR_SUCCESS)
		{
			bIsSuccess=False;
		}
	}

	return bIsSuccess;
}

// -------------------------------
Bool yiRegDeleteKey(HKEY hKey, const char* pszSubKey, Bool bIsForced)
{
	if (::RegDeleteKey(hKey, pszSubKey)/*, KEY_WOW64_32KEY, 0)*/!=ERROR_SUCCESS)
	{
		if (bIsForced==True)
		{
			Bool res =  xRegDeleteKeySub(hKey, pszSubKey);
			if (RegDeleteKey(hKey, pszSubKey)/*, KEY_WOW64_32KEY, 0)*/!=ERROR_SUCCESS)
			{
				return False;
			}
			return True;
		}

		return False;
	}

	return True;
}

// -------------------------------
HKEY yiUtilRegOpenKeyForAll(HKEY hKey, const char* pszSubKey)
{
	HKEY hKeySub=0;
	if (::RegOpenKeyExA(hKey, pszSubKey, KEY_ALL_ACCESS, 0, &hKeySub)!=ERROR_SUCCESS)
	{
		return 0;
	}

	return hKeySub;
}

// -------------------------------
HKEY yiUtilRegOpenKeyForRead(HKEY hKey, const char* pszSubKey)
{
	HKEY hKeySub=0;
	if (RegOpenKeyExA(hKey, pszSubKey, KEY_READ, 0, &hKeySub)!=ERROR_SUCCESS)
	{
		return 0;
	}

	return hKeySub;
}

// -------------------------------
HKEY yiUtilRegCreateKey(HKEY hKey, const char* pszSubKey)
{
	HKEY hSubKey=0;
	if (RegCreateKeyEx(hKey, pszSubKey, 0,0 , 0, KEY_ALL_ACCESS, 0, &hSubKey, 0)!=ERROR_SUCCESS)
	{
		return 0;
	}

	return hSubKey;
}




// -------------------------------
Bool yiUtilCreateShortCutKey(const char* pszPath, const char* pszSrcPath, const char* pszParam)
{
    HRESULT hres; 
    IShellLink* psl = NULL; 

    // COMインターフェイスを初期化 
    CoInitialize( NULL) ;

    // IShellLink インターフェイスを取得 
    hres = CoCreateInstance(
            CLSID_ShellLink, NULL, 
            CLSCTX_INPROC_SERVER,
            IID_IShellLink,
            (LPVOID *)&psl) ; 
    if(SUCCEEDED(hres))
    { 
        IPersistFile* ppf; 

        // Linkオブジェクトのパスを設定(たとえば、
        // C;\Windows\notepad.exeなど)         
        psl->SetPath(pszSrcPath); 
		psl->SetArguments(pszParam);

        // IPersistFileインターフェイスを取得し、
        // Linkパスファイル名を保存する。
        // これは、IShellLinkの中の
        // ディスク関連オブジェクトを
        // 取得していると思ってください。 
        hres = 
        psl->QueryInterface( IID_IPersistFile,
                                 (void**)&ppf) ; 

        if(SUCCEEDED(hres))
        { 
            WCHAR wsz[MAX_PATH]; 

            // パスをマルチバイトに変換
            // これは、次の関数Saveが
            // マルチバイトの引数のため 
            MultiByteToWideChar(CP_ACP, 0,
                            pszPath, -1,
                            wsz, MAX_PATH);

            // ディスクに保存する 
            hres = ppf->Save(wsz, TRUE); 
            ppf->Release(); 
        } 
        psl->Release(); 
    }
    else
    {
		return False;
    }
	CoUninitialize();

	return True;
}

// ------------------------------------
void yiUtilRectConvert(RECT& rect)
{
	RECT x=rect;
	rect.left=x.top;
	rect.top=x.left;
	rect.right=x.bottom;
	rect.bottom=x.right;
}

// ------------------------------------
Bool yiUtilGetAddRect(RECT* result, RECT* rect1, RECT* rect2)
{
	if (rect1==0)
		*result=*rect2;
	else if (rect2==0)
		*result=*rect1;
	else
	{
		result->left=rect1->left<rect2->left?rect1->left:rect2->left;
		result->top=rect1->top<rect2->top?rect1->top:rect2->top;
		result->right=rect1->right<rect2->right?rect2->right:rect1->right;
		result->bottom=rect1->bottom<rect2->bottom?rect2->bottom:rect1->bottom;
	}
	return True;
}

// ------------------------------------
Bool yiUtilGetAddRect2(RECT* result, RECT* rect1, RECT* rect2)
{
	if (rect1==0)
		*result=*rect2;
	else if (rect2==0)
		*result=*rect1;
	else
	{
		int w1=rect1->right+rect1->left;
		int h1=rect1->bottom+rect1->top;
		int w2=rect2->right+rect2->left;
		int h2=rect2->bottom+rect2->top;

		result->left=rect1->left<rect2->left?rect1->left:rect2->left;
		result->top=rect1->top<rect2->top?rect1->top:rect2->top;
		result->right=w1<w2?w2:w1;
		result->bottom=h1<h2?h2:h1;

		result->right-=result->left;
		result->bottom-=result->top;
	}
	return True;
}

// ------------------------------------
Bool yiUtilToClipBoard(const wchar_t* pszText)
{
	if (OpenClipboard(0)==FALSE)
	{
		return False;
	}

	if (EmptyClipboard()==FALSE)
	{
		return False;
	}

	int len = wcslen(pszText);

	HGLOBAL    hGlobal = GlobalAlloc(GHND | GMEM_DDESHARE, (len<<1)+sizeof(wchar_t));
	wchar_t* clipInText = (wchar_t*)GlobalLock(hGlobal);
	memcpy(clipInText, pszText, len<<1);
	GlobalUnlock(hGlobal);

	if(!SetClipboardData(CF_UNICODETEXT, hGlobal)) {
		GlobalFree(hGlobal);
		CloseClipboard();
		return False;
	}


	CloseClipboard();
	return True;
}

// ------------------------------------
wchar_t* yiUtilFromClipBoard(wchar_t* pszText, int* length)
{
	if (OpenClipboard(0)==FALSE)
	{
		return 0;
	}

	int len = wcslen(pszText);
	
	HANDLE hClipBoard = GetClipboardData(CF_UNICODETEXT);

	if (hClipBoard==0)
		return 0;

	wchar_t* pText=(wchar_t*)GlobalLock(hClipBoard);
	::wcsncpy(pszText, pText, *length);
	GlobalUnlock(hClipBoard);
	CloseClipboard();

	return pszText;
}

// ------------------------------------
Bool yiUtilPointIsInRect(RECT* rect, int x, int y)
{
	if (x<rect->left)
		return False;
	if (x>rect->right)
		return False;
	if (y<rect->top)
		return False;
	if (y>rect->bottom)
		return False;

	return True;
}

// ---------------------------------------
Bool yiUtilIsInPos(RECT* _rect, int x, int y)
{
	RECT rect = {_rect->left, _rect->top, _rect->left+_rect->right, _rect->top+_rect->bottom};

	if (x<=rect.left)
		return False;
	if (x>=rect.right)
		return False;
	if (y<=rect.top)
		return False;
	if (y>=rect.bottom)
		return False;

	return True;
}

// ---------------------------------------
Bool yiUtilIsInRect(RECT* rect1, RECT* rect2, RECT& ret)
{
	int x,y,w,h;

	if (rect1->left > rect2->left)
	{
		if (rect2->left+rect2->right < rect1->left)
		{
			return False;
		}
		x=rect1->left;
		w=rect1->left+rect1->right;
		w=w>rect2->left+rect2->right?rect2->left+rect2->right:w;
	}
	else
	{
		if (rect1->left+rect1->right < rect2->left)
		{
			return False;
		}
		x=rect2->left;
		w=rect2->left+rect2->right;
		w=w>rect1->left+rect1->right?rect1->left+rect1->right:w;
	}
	w-=x;

	if (rect1->top > rect2->top)
	{
		if (rect2->top+rect2->bottom < rect1->top)
		{
			return False;
		}
		y=rect1->top;
		h=rect1->top+rect1->bottom;
		h=h>rect2->top+rect2->bottom?rect2->top+rect2->bottom:h;
	}
	else
	{
		if (rect1->top+rect1->bottom < rect2->top)
		{
			return False;
		}
		y=rect2->top;
		h=rect2->top+rect2->bottom;
		h=h>rect1->top+rect1->bottom?rect1->top+rect1->bottom:h;
	}

	h-=y;
	ret.left=x;
	ret.right=w;
	ret.top=y;
	ret.bottom=h;

	if (h==0 || w==0)
	{
		return False;
	}
	return True;
}

// ---------------------------------------
Bool yiNullDACL(SECURITY_ATTRIBUTES* pSA)
{
    PSECURITY_DESCRIPTOR    pSD;

    pSD = (PSECURITY_DESCRIPTOR)yialloci(SECURITY_DESCRIPTOR_MIN_LENGTH);//::LocalAlloc(
                        //LPTR,SECURITY_DESCRIPTOR_MIN_LENGTH);

	pSD=(PSECURITY_DESCRIPTOR)::LocalAlloc(LPTR,SECURITY_DESCRIPTOR_MIN_LENGTH);
    if ( pSD != NULL )
    {
        if ( ::InitializeSecurityDescriptor(
                            pSD,SECURITY_DESCRIPTOR_REVISION ) )
        {
            if ( ::SetSecurityDescriptorDacl( pSD, TRUE,
                                            (PACL)NULL,FALSE ) )
            {
                pSA->nLength = sizeof(SECURITY_ATTRIBUTES);
                pSA->lpSecurityDescriptor = pSD;
                pSA->bInheritHandle = TRUE;
                return TRUE;
            }
        }
		LocalFree(pSD);
		//yifree(pSD);
    }
    return FALSE;

}

BOOL yiDestroyNullDACL( SECURITY_ATTRIBUTES *pSA )
{
    if ( pSA && pSA->lpSecurityDescriptor )
    {
		LocalFree(pSA->lpSecurityDescriptor );
        //yifree( pSA->lpSecurityDescriptor );
        pSA->lpSecurityDescriptor = NULL;
        return TRUE;
    }
    return FALSE;
}


void OnBeep(void* param)
{
	unsigned int lparam = (unsigned int)param;
	if (::Beep(lparam >>16, lparam &0xFFFF)==0)
	{
		xTRACE("yiBeep(%x, %x) error:%d", lparam>>16, lparam&0xFFFF, GetLastError());
	}
}

void yiBeep(int type)
{
	//unsigned int param = ((fre<<16)|(term&0xFFFF));
	//_beginthread(OnBeep, 0, (void*)param);
	MessageBeep(type);
}

#endif // EOF _WIN32

Bool yiIsNumeric(char* buffer)
{
	int cnt=0;
	BOOL bFoundComa=FALSE;
	BOOL bIsMinus=FALSE;

	while(*buffer)
	{
		if (cnt==0)
		{
			if (*buffer==' ')
			{
				buffer++;
				continue;
			}
			if (*buffer=='-')
			{
				buffer++;
				cnt++;
				bIsMinus=TRUE;
				continue;
			}
			if (*buffer=='.')
			{
				return FALSE;
			}
		}

		if (*buffer=='.')
		{
			if (bIsMinus==TRUE && cnt==1)
			{
				return FALSE;
			}
			else if (bFoundComa==TRUE)
			{
				return FALSE;
			}

			bFoundComa=TRUE;
			buffer++;
			cnt++;
			continue;
		}

		if (*buffer < '0' ||
				*buffer> '9')
			return FALSE;

		buffer++;
		cnt++;
	}

	return TRUE;
}

int xIsSame(char ch1, char ch2)
{
	const int nTerm = 'a'-'A';

	if (ch1>='A' && ch1<='z')
	{
		ch1=ch1<'a'?ch1:(ch1-nTerm);
	}

	if (ch2>='A' && ch2<='z')
	{
		ch2=ch2<'a'?ch2:(ch2-nTerm);
	}

	return ch1-ch2;
}

// -------------------------------
int stricmpe(const char* p1, const char* p2)
{
	int cnt1 = strlen(p1);
	int cnt2 = strlen(p2);
	int i=0;

	if (cnt1<cnt2)
		return -1;

	for (i=cnt2-1; i>=0; i++)
	{
		if (xIsSame(*p1, *p2)==0)
		{
			continue;
		}
		return *p2-*p1;
	}

	return 0;
}

// -------------------------------
int yiUtilGetAge(unsigned int birth, unsigned int curr)
{
#if 0
	if (curr==0)
	{
		cyiTime::GetLocalTime(curr);
	}

	int n=0;
	int y = DAY_YEAR(birth);
	int y2 = DAY_YEAR(curr);

	n = y2-y;

	if (n<=0)
		return 0;

	int m = DAY_MON(birth);
	int m2 = DAY_MON(curr);

	if (m2>m)
	{
		return n;
	}

	if (m==m2)
	{
		int d = DAY_DAY(birth);
		int d2= DAY_DAY(curr);

		if (d2>=d)
			return n;
	}

	return n-1;
#endif 
	return 0;
}


#include "yisock.h"

// ---------------------------------------
void yiUrlGetHostAndPath(const char* url, char* pszServer, char* path, int* port, char* protocol)
{

	if (pszServer)
		pszServer[0]=0;

	if (path)
		path[0]=0;

	if (port)
		*port=0;

	if (protocol)
		strcpy(protocol, "http");

	char* top = (char*)strstr(url, "://");

	if (top)
	{
		if (protocol)
			strcpy(protocol, top);
		top+=3;
	}
	else
	{
		if (protocol)
			strcpy(protocol, "http");

		top=(char*)url;
	}

	char* pEndUrl = (char*)strstr(url, "/");

	if (pEndUrl==0)
	{
		if (pszServer)
			strcpy(pszServer, url);
		goto CHECK_PORT;
	}
	else
	{
		if (pszServer)
			::yistrchcpy(pszServer, url, '/');
	}

	if (path)
	{
		strcpy(path, pEndUrl);
	}

CHECK_PORT:

	char* pPort = (char*)strstr(pszServer, ":");

	if (pPort)
	{
		char szPort[16];
		
		*pPort=0;
		strncpy(szPort, ++pPort, 16);
		szPort[15]=0;
		*port=atoi(szPort);
	}

	return ;
}

char* yiHttpGet(const char* pszUrl, int nTimeOut, int length)//const char* pszTarget, int* length)
{
    char szGetCommand[1024];
	char pszServer[256];
	char pszTarget[256];

	yiUrlGetHostAndPath(pszUrl, pszServer, pszTarget, 0, 0);

	address_t address=yihtol(pszServer, 0);

    int sock = yiconnect(address, 80, 0);

    if (sock<0)
    {
        //printf("connect failure %s\n", argv[1]);
        return 0;
    }


	//sprintf(szGetCommand, "GET %s HTTP/1.0\r\n\r\n", pszTarget);
	sprintf(szGetCommand, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", pszTarget, pszServer);

    if (send(sock, szGetCommand, strlen(szGetCommand), 0)<=0)
    {
            //printf("send failure %s\n", argv[1]);
            yiclose(sock);
            return 0;
    }

	if (length!=0)
	{
		int n=0;
		void* pBuffer=yialloc(length+1);
		if ((n=yirecv(sock, pBuffer, length, 0))<=0)
		{
				//printf("recv failure %s\n", argv[1]);
				yiclose(sock);
				return 0;
		}
		//*length=n;
		((char*)pBuffer)[n]=0;
		yiclose(sock);
		return (char*)pBuffer;
	}
/*	else
	{
		// 未対応
		yiclose(sock);
		return 0;
	}*/

    yiclose(sock);
	return 0;
}

#include "yiStream.h"

// ---------------------------------------
Binary yiHttpGetEx(const char* pszUrl, int nTimeOut, Bool bIsCache)
{
    char szGetCommand[1024];
	char pszServer[256];
	char pszTarget[256];

	yiUrlGetHostAndPath(pszUrl, pszServer, pszTarget, 0, 0);

	address_t address=yihtol(pszServer, 0);

	if (address==0)
		return 0;

    int sock = yiconnect(address, 80, 0);

    if (sock<0)
    {
        //printf("connect failure %s\n", argv[1]);
        return 0;
    }

	sprintf(szGetCommand, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", pszTarget, pszServer);


    if (send(sock, szGetCommand, strlen(szGetCommand), 0)<=0)
    {
            //printf("send failure %s\n", argv[1]);
            yiclose(sock);
            return 0;
    }

	//HandleFile h = 0;
	//
	//if (bIsCache)
	//	h = yiFileCreateTemporaryFile();

	cyiMemoryStream st;

	Bool bIsFirst=True;
	int receveSize=0;
	while(1)
	{
		char buffer[4096];
		int n=0;
		
		if ((n=yirecv(sock, buffer, 4096, 0))<=0)
		{
				//printf("recv failure %s\n", argv[1]);
				yiclose(sock);
				return 0;
		}

		if (bIsFirst==True)
		{
			int responseLength = strlen("HTTP/1.1 200 OK");
			if (strncmp(buffer, "HTTP/1.1 200 OK", responseLength)!=0)
			{
				yiclose(sock);
				return 0;
			}

			char* pxxx = strstr(buffer, "Content-Length:");
			pxxx+=strlen("Content-Length:");

			char  szSize[32];
			yistrchcpy(szSize, pxxx, '\n');
			char* pszSize = yistrTrimEx(0, szSize, "\r ");

			if (yiIsNumeric(pszSize)==False)
			{
				yiclose(sock);
				return 0;
			}
			receveSize = atoi(pszSize);

			char* pBody = strstr(buffer, "\r\n\r\n");
			pBody+=4;
			int nHeaderSize = (unsigned int)pBody-(unsigned int)buffer;

			if (nHeaderSize+receveSize==n)
			{
				// 受信完了
				return yiBinary(&buffer[nHeaderSize], receveSize, 0, 0);
			}

			bIsFirst=False;
			st.Write(&buffer[nHeaderSize], n-nHeaderSize, 0);
			receveSize-=(n-nHeaderSize);
			continue;
		}

		st.Write(buffer, n, 0);
		receveSize-=n;

		if (receveSize==0)
			break;
	}

	yiclose(sock);

	return st.ExportToBinary();

	//if (*length!=0)
	//{
	//	int n=0;
	//	void* pBuffer=yialloc(*length+1);
	//	if ((n=yirecv(sock, pBuffer, *length, 0))<=0)
	//	{
	//			//printf("recv failure %s\n", argv[1]);
	//			yiclose(sock);
	//			return 0;
	//	}
	//	*length=n;
	//	((char*)pBuffer)[*length]=0;
	//	yiclose(sock);
	//	return (char*)pBuffer;
	//}
	//else
	//{
	//	// 未対応
	//	yiclose(sock);
	//	return 0;
	//}

 //   yiclose(sock);
	return 0;
}

#ifdef _WIN32
static const char* g_pszAreaJapan[] = {
	"-",
	"北海道",
	"青森県",
	"岩手県",
	"宮城県",
	"秋田県",
	"山梨県",
	"福島県",
	"茨城県",
	"栃木県",
	"群馬県",
	"埼玉県",
	"千葉県",
	"東京都",
	"神奈川県",
	"新潟県",
	"富山県",
	"石川県",
	"福井県",
	"山梨県",
	"長野県",
	"岐阜県",
	"静岡県",
	"愛知県",
	"三重県",
	"滋賀県",
	"京都府",
	"大阪府",
	"兵庫県",
	"奈良県",
	"和歌山県",
	"鳥取県",
	"島根県",
	"岡山県",
	"広島県",
	"山口県",
	"徳島県",
	"香川県",
	"愛媛県",
	"高知県",
	"福島県",
	"佐賀県",
	"長崎県",
	"熊本県",
	"大分県",
	"宮崎県",
	"鹿児島県",
	"沖縄県",
	0
};


static const wchar_t* g_pszAreaJapanW[] = {
	L"-",
	L"北海道",
	L"青森県",
	L"岩手県",
	L"宮城県",
	L"秋田県",
	L"山梨県",
	L"福島県",
	L"茨城県",
	L"栃木県",
	L"群馬県",
	L"埼玉県",
	L"千葉県",
	L"東京都",
	L"神奈川県",
	L"新潟県",
	L"富山県",
	L"石川県",
	L"福井県",
	L"山梨県",
	L"長野県",
	L"岐阜県",
	L"静岡県",
	L"愛知県",
	L"三重県",
	L"滋賀県",
	L"京都府",
	L"大阪府",
	L"兵庫県",
	L"奈良県",
	L"和歌山県",
	L"鳥取県",
	L"島根県",
	L"岡山県",
	L"広島県",
	L"山口県",
	L"徳島県",
	L"香川県",
	L"愛媛県",
	L"高知県",
	L"福島県",
	L"佐賀県",
	L"長崎県",
	L"熊本県",
	L"大分県",
	L"宮崎県",
	L"鹿児島県",
	L"沖縄県",
	0
};
#endif

// ---------------------------------------
const char* GetAreaName(int code, int locale)
{
#ifdef _WIN3332
	if (locale==0)
	{
		return g_pszAreaJapan[code];
	}
#endif

	return 0;
}

// ---------------------------------------
const wchar_t* GetAreaNameW(int code, int locale)
{
#ifdef _WIN32
	if (locale==0)
	{
		return g_pszAreaJapanW[code];
	}
#endif

	return 0;
}

// ---------------------------------------
Bool yiUtilRegisterExtentions(const char* pszExtensions, const char* pszApplicationName)
{
	// 1. まず HKEY_CLASSES_ROOT\pszExtensions\pszExtensions_auto_file 作成
	// 2. HKEY_CLASSES_ROOT\pszExtensions\pszExtensions_auto_file\shell\open\ -> "pszApplicationPath" "%1"
	return True;
}

// ---------------------------------------
Bool yiUtilRegisterPreview(const char* pszExtensions, const char* CLSID, const char* pszRegisterName)
{
	//char szKey[256];
	//// 1. まず HKEY_CLASS_ROOT\CLSID\{CLSID} -> {6d2b5079-2f0b-48dd-ab7f-97cec514d30b}
	//wsprintf(szSbuKey, "CLSID\\{%s}", CLSID); 
	//HKEY hSub = yiUtilRegOpenKeyForAll(HKEY_CLASSES_ROOT, szSbuKey);

	//if (hSub==0)
	//{
	//	hSub=yiUtilRegCreateKey(HKEY_CLASSES_ROOT, szSubKey);
	//	if (hSub==0)
	//	{
	//		return False;
	//	}
	//}

	//// prevhost 登録
	//if (RegSetValueExA(hSub, "", 0, REG_SZ, pszRegisterName)!=ERROR_SUCCESS)
	//{
	//	RegCloseKey(hSub);
	//	return False;
	//}

	//if (RegSetValueExA(hSub, "AppID", 0, REG_SZ, "{6d2b5079-2f0b-48dd-ab7f-97cec514d30b}")!=ERROR_SUCCESS)
	//{
	//	RegCloseKey(hSub);
	//	return False;
	//}
	//RegCloseKey(hSub);


	//// 2. 次に HKEY_CLASSES_ROOT\pszExtensions\shellex\{8895b1c6-b41f-4c1c-a562-0d564250836f}->CLSID
	//wsprintf(szSbuKey, ".%s\\shellex\\{8895b1c6-b41f-4c1c-a562-0d564250836f}", pszExtensions); 
	//hSub = yiUtilRegOpenKeyForAll(HKEY_CLASSES_ROOT, szSbuKey);

	//if (hSub==0)
	//{
	//	wsprintf(szSubKey, ".%s", pszExtensions);
	//	hSub = yiUtilRegCreateKey(HKEY_CLASSES_ROOT, szSubKey);

	//	if (hSub==0)
	//	{
	//		hSub = yiUtilRegOpenKeyForAll(HKEY_CLASSES_ROOT, szSubKey);

	//		if (hSub==0)
	//			return False;
	//	}

	//	//wsprintf(szSubKey, "shellex", pszExtensions);
	//	HKEY hSubSub=yiUtilRegCreateKey(hSub, "shellex");

	//	if (hSubSub==0)
	//	{
	//		hSubSub = yiUtilRegOpenKeyForAll(hSub, szSubKey);

	//		if (hSub==0)
	//		{
	//			::RegCloseKey(hSubSub);
	//			return False;
	//		}
	//	}
	//	::RegCloseKey(hSub);
	//	hSub=hSubSub;

	//	HKEY hSubSub=yiUtilRegCreateKey(hSub, "{8895b1c6-b41f-4c1c-a562-0d564250836f}");

	//	if (hSubSub==0)
	//	{
	//		hSubSub = yiUtilRegOpenKeyForAll(hSub, szSubKey);

	//		if (hSub==0)
	//		{
	//			::RegCloseKey(hSubSub);
	//			return False;
	//		}

	//	}

	//	RegCloseKey(hSub);
	//	hSub=hSubSub;
	//}

	//wsprintf(szSubKey, "{%s}", CLSID);
	//
	//if (::RegSetValueExA(hSub, NULL, 0, REG_SZ, szSubKey, strlen(szSubKey))!=ERROR_SUCCESS)
	//{
	//	RegCloseKey(hSub);
	//	return False;
	//}


	//// 3. HEKY_LOCAL_MACHINE\SOFTWARE\MicroSoft\Windows\CurrentVersion\PreviewHandlers\CLSID -> pszRegisterName
	//wsprintf(szSubKey, "SOFTWARE\\MicroSoft\\Windows\\CurrentVersion\\PreviewHandlers");

	//hSub = ::yiUtilRegOpenKeyForAll(HKEY_LOCAL_MACHINE, szSubKey);

	//if (hSub==0)
	//	return False;

	//wsprintf(szSubKey, "{%s}", CLSID);
	//if (::RegSetValueExA(hSub, szSubKey, 0, REG_SZ, pszRegisterName, strlen(pszRegisterName))!=ERROR_SUCCESS)
	//{
	//	RegCloseKey(hSub);
	//	return False;
	//}

	//RegCloseKey(hSub);

	return True;
}

