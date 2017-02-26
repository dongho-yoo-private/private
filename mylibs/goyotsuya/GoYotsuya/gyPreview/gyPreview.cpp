// gyPreview.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"

//#define _EXAMPLE

#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#include "gyPreView.h"
#include "yistd.h"

#pragma comment (lib, "shlwapi.lib")

//const CLSID CLSID_GYP = {0x112143a6, 0x62c1, 0x4478, {0x9e, 0x8f, 0x87, 0x26, 0x99, 0x25, 0x5e, 0x2e}};

// {0E64B778-416D-458b-B777-700EBA8D04C7}
static const CLSID  CLSID_GYP = { 0xe64b778, 0x416d, 0x458b, { 0xb7, 0x77, 0x70, 0xe, 0xba, 0x8d, 0x4, 0xc7 } };
const TCHAR g_szClsid[] = TEXT("{0E64B778-416D-458b-B777-700EBA8D04C7}");
const TCHAR g_szHandlerName[] = TEXT("GoYotsuya SGF Player");
const TCHAR g_szExt[] = TEXT(".sgf");


LONG      g_lLocks = 0;
HINSTANCE g_hinstDll = NULL;

LONG LockModule(BOOL bLock);
BOOL CreateRegistryKey(HKEY hKeyRoot, LPTSTR lpszKey, LPTSTR lpszValue, LPTSTR lpszData);


// CPreviewHandler


CPreviewHandler::CPreviewHandler()
{
	m_cRef = 1;
	m_hwndParent = NULL;
	m_hwndPreview = NULL;
	m_pSite = NULL;
	m_pFrame = NULL;
	m_goban=0;
	m_pStream=0;

	//AddRef
	LockModule(TRUE);
}

CPreviewHandler::~CPreviewHandler()
{
	LockModule(FALSE);
	//if (m_pStream)
	//{
	//	m_pStream->Release();
	//	m_pStream=0;
	//}

	//if (m_goban)
	//{
	//	m_goban->Destroy();
	//	delete m_goban;
	//	m_goban=0;
	//}
	//m_cRef=0;
}

STDMETHODIMP CPreviewHandler::QueryInterface(REFIID riid, void **ppvObject)
{
	*ppvObject = NULL;

	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IPreviewHandler))
		*ppvObject = static_cast<IPreviewHandler *>(this);
	else if (IsEqualIID(riid, IID_IObjectWithSite))
		*ppvObject = static_cast<IObjectWithSite *>(this);
	else if (IsEqualIID(riid, IID_IOleWindow))
		*ppvObject = static_cast<IOleWindow *>(this);
	else if (IsEqualIID(riid, IID_IInitializeWithStream))
		*ppvObject = static_cast<IInitializeWithStream *>(this);
	else
		return E_NOINTERFACE;

	AddRef();

	return S_OK;
}

STDMETHODIMP_(ULONG) CPreviewHandler::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CPreviewHandler::Release()
{
	if (InterlockedDecrement(&m_cRef) == 0) {
		delete this;
		return 0;
	}

	return m_cRef;
}

STDMETHODIMP CPreviewHandler::SetWindow(HWND hwnd, const RECT *prc)
{
	if (hwnd)
	{
		m_hwndParent = hwnd;
	}

	if (prc)
	{
		m_rc = *prc;
	}

	if (m_goban!=0)
	{
		//SetParent(*m_goban, m_hwndParent);

		if (hwnd)
		{
			m_goban->SetParent(m_hwndParent);
		}

		if (prc)
		{
			m_goban->ReSize(m_rc.right - m_rc.left, m_rc.bottom - m_rc.top);
		}
	}

	return S_OK;
}

STDMETHODIMP CPreviewHandler::SetRect(const RECT *prc)
{
	if (prc==0)
	{
		return E_INVALIDARG;
	}

	m_rc = *prc;

	if (m_goban!=0)
	{
		m_goban->ReSize(m_rc.right - m_rc.left, m_rc.bottom - m_rc.top);
	}

	
	return S_OK;
}
STDMETHODIMP CPreviewHandler::DoPreview(VOID)
{
	if (m_goban!=0)
	{
		return E_FAIL;
	}

	if (m_pStream==0)
		return E_FAIL;

	//ShowWindow(m_hwndParent, SW_SHOW);

	m_goban = new cglsGobanContainer();
	if (m_goban->Create(g_hinstDll, 0, 0, 0, m_rc.right - m_rc.left, m_rc.bottom - m_rc.top, (HICON)m_hwndParent, (HICON)0, &Font(L"Meiryo UI", 11.0), 0, 0, WS_CHILD|WS_VISIBLE)==False)
	{
		delete m_goban;
		m_goban=0;
		return E_FAIL;
	}
	if (m_goban->SetStream(m_pStream)==False)
	{
		m_goban->Destroy();
		delete m_goban;
		m_goban=0;
		m_hwndPreview=0;
		m_pStream->Release();
		m_pStream=0;
		return E_FAIL;
	}
	//m_pStream->Release();
	//m_pStream=0;
	m_hwndPreview=m_goban->m_hWnd;

	return S_OK;
}

STDMETHODIMP CPreviewHandler::Unload(VOID)
{
	if (m_goban!=0)
	{
		m_goban->Destroy();
		delete m_goban;
		m_goban=0;
		m_hwndPreview=0;
	}

	if (m_pStream!=0)
	{
		m_pStream->Release();
		m_pStream=0;
	}

	m_hwndPreview=0;
	return S_OK;
}

STDMETHODIMP CPreviewHandler::SetFocus(VOID)
{
	if (m_goban)
	{
		::SetFocus(*m_goban);
	}
	return S_OK;
}

STDMETHODIMP CPreviewHandler::QueryFocus(HWND *phwnd)
{
	HRESULT hr = E_INVALIDARG;
	
	if (phwnd != NULL) {
		*phwnd = GetFocus();
		if (*phwnd != NULL)
			hr = S_OK;
		else
			hr = HRESULT_FROM_WIN32(GetLastError());
	}

	return hr;
}

STDMETHODIMP CPreviewHandler::TranslateAccelerator(MSG *pmsg)
{
	HRESULT hr = S_FALSE;
	
	if (m_pFrame != NULL)
		hr = m_pFrame->TranslateAccelerator(pmsg);

	return hr;
}
	
STDMETHODIMP CPreviewHandler::SetSite(IUnknown *pUnkSite)
{
	HRESULT hr;

	if (m_pSite != NULL) {
		m_pSite->Release();
		m_pSite = NULL;
	}

	m_pSite = pUnkSite;

	hr = m_pSite->QueryInterface(IID_PPV_ARGS(&m_pFrame));
	
	return hr;
}

STDMETHODIMP CPreviewHandler::GetSite(REFIID riid, void **ppvSite)
{
	*ppvSite = m_pSite;

	return S_OK;
}

STDMETHODIMP CPreviewHandler::GetWindow(HWND *phwnd)
{
	if (phwnd == NULL)
		return E_INVALIDARG;

	*phwnd = m_hwndParent;

	return S_OK;
}

STDMETHODIMP CPreviewHandler::ContextSensitiveHelp(BOOL fEnterMode)
{
	return E_NOTIMPL;
}

STDMETHODIMP CPreviewHandler::Initialize(IStream *pstream, DWORD grfMode)
{
	if (m_pStream)
	{
		m_pStream->Release();
		m_pStream=0;
	}

	m_pStream=pstream;
	m_pStream->AddRef();
	return S_OK;
}


// CClassFactory


STDMETHODIMP CClassFactory::QueryInterface(REFIID riid, void **ppvObject)
{
	*ppvObject = NULL;

	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IClassFactory))
		*ppvObject = static_cast<IClassFactory *>(this);
	else
		return E_NOINTERFACE;

	AddRef();
	
	return S_OK;
}

STDMETHODIMP_(ULONG) CClassFactory::AddRef()
{
	LockModule(TRUE);
	return 2;
}

STDMETHODIMP_(ULONG) CClassFactory::Release()
{
	LONG n = LockModule(FALSE);

	if (n==0)
		delete this;

	return n;
}

STDMETHODIMP CClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject)
{
	CPreviewHandler *p;
	HRESULT         hr;

	*ppvObject = NULL;

	if (pUnkOuter != NULL)
		return CLASS_E_NOAGGREGATION;

	p = new CPreviewHandler();
	if (p == NULL)
		return E_OUTOFMEMORY;

	hr = p->QueryInterface(riid, ppvObject);
	p->Release();

	return hr;
}

STDMETHODIMP CClassFactory::LockServer(BOOL fLock)
{
	LockModule(fLock);

	return S_OK;
}


// DLL Export


STDAPI DllCanUnloadNow()
{
	return g_lLocks == 0 ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv)
{
	static CClassFactory serverFactory;
	HRESULT hr;

	*ppv = NULL;
	
	if (IsEqualCLSID(rclsid, CLSID_GYP))
		hr = serverFactory.QueryInterface(riid, ppv);
	else
		hr = CLASS_E_CLASSNOTAVAILABLE;

	return hr;
}

STDAPI DllRegisterServer(void)
{
	TCHAR szModulePath[MAX_PATH];
	TCHAR szKey[256];

	//::SetCurrentDirectoryA("c:\\vfrwin");

	wsprintf(szKey, TEXT("CLSID\\%s"), g_szClsid);

	if (!CreateRegistryKey(HKEY_CLASSES_ROOT, szKey, NULL, TEXT("ShellExtension Sample")))
		return E_FAIL;

	GetModuleFileName(g_hinstDll, szModulePath, sizeof(szModulePath) / sizeof(TCHAR));
	wsprintf(szKey, TEXT("CLSID\\%s\\InprocServer32"), g_szClsid);
	if (!CreateRegistryKey(HKEY_CLASSES_ROOT, szKey, NULL, szModulePath))
		return E_FAIL;
	
	wsprintf(szKey, TEXT("CLSID\\%s\\InprocServer32"), g_szClsid);
	if (!CreateRegistryKey(HKEY_CLASSES_ROOT, szKey, TEXT("ThreadingModel"), TEXT("Apartment")))
		return E_FAIL;

	wsprintf(szKey, TEXT("CLSID\\%s"), g_szClsid);

	//if (!CreateRegistryKey(HKEY_CLASSES_ROOT, szKey, TEXT("AppID"), TEXT("{6d2b5079-2f0b-48dd-ab7f-97cec514d30b}")))
	//	return E_FAIL;
	if (!CreateRegistryKey(HKEY_CLASSES_ROOT, szKey, TEXT("AppID"), TEXT("{534A1E02-D58F-44f0-B58B-36CBED287C7C}")))
		return E_FAIL;

	wsprintf(szKey, TEXT("%s\\shellex\\{8895b1c6-b41f-4c1c-a562-0d564250836f}"), g_szExt);
	if (!CreateRegistryKey(HKEY_CLASSES_ROOT, szKey, NULL, (LPTSTR)g_szClsid))
		return E_FAIL;

	wsprintf(szKey, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\PreviewHandlers"));

	if (!CreateRegistryKey(HKEY_LOCAL_MACHINE, szKey, (LPTSTR)g_szClsid, (LPTSTR)g_szHandlerName))
		return E_FAIL;

	return S_OK;
}

STDAPI DllUnregisterServer(void)
{
	TCHAR szKey[256];

	wsprintf(szKey, TEXT("CLSID\\%s"), g_szClsid);
	SHDeleteKey(HKEY_CLASSES_ROOT, szKey);

	SHDeleteKey(HKEY_CLASSES_ROOT, g_szExt);
	
	SHDeleteValue(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\PreviewHandlers"), (LPTSTR)g_szClsid);

	return S_OK;
}


BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason) {

	case DLL_PROCESS_ATTACH:
		//yiTraceM2("c:\\yotsuya\\yotsuya.log", "DllMain() called");

		//xTRACEF("c:\\yotsuya\\gy.log", "DllMain called");
		g_hinstDll = hinstDll;
		yictrl_initialize();

		DisableThreadLibraryCalls(hinstDll);
		return TRUE;
		case DLL_PROCESS_DETACH:
		{
			yictrl_finalize();
			return TRUE;
		}
		default:
			return TRUE;
	}
}


// Function


LONG LockModule(BOOL bLock)
{
	if (bLock)
		return InterlockedIncrement(&g_lLocks);
	else
		return InterlockedDecrement(&g_lLocks);
}

BOOL CreateRegistryKey(HKEY hKeyRoot, LPTSTR lpszKey, LPTSTR lpszValue, LPTSTR lpszData)
{
	HKEY  hKey;
	LONG  lResult;
	DWORD dwSize;

	lResult = RegCreateKeyEx(hKeyRoot, lpszKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
	if (lResult != ERROR_SUCCESS)
		return FALSE;

	if (lpszData != NULL)
		dwSize = (lstrlen(lpszData) + 1) * sizeof(TCHAR);
	else
		dwSize = 0;

	RegSetValueEx(hKey, lpszValue, 0, REG_SZ, (LPBYTE)lpszData, dwSize);
	RegCloseKey(hKey);
	
	return TRUE;
}

