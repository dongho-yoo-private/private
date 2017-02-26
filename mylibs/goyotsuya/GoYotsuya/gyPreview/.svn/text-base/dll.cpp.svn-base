// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "yi.h"
#include "yctrls.h"
#include <objbase.h>
#include <shlwapi.h>
#include <new>

extern HRESULT CGoPreviewHandler_CreateInstance(REFIID riid, void **ppv);

//const CLSID CLSID_GYP    = {0xF654F1BF, 0x54D9, 0x4A2E, {0xB7, 0x03, 0x88, 0x90, 0x91, 0xD3, 0xCB, 0x2D}};


static const CLSID  CLSID_GYP = { 0xe64b778, 0x416d, 0x458b, { 0xb7, 0x77, 0x70, 0xe, 0xba, 0x8d, 0x4, 0xc7 } };
#define SZ_CLSID_GYP     L"{0E64B778-416D-458b-B777-700EBA8D04C7}"
#define SZ_GOPREVIEWHANDLER           L"GoYotsuya SGF Player"
const TCHAR g_szExt[] = TEXT(".sgf");


typedef HRESULT (*PFNCREATEINSTANCE)(REFIID riid, void **ppvObject);
struct CLASS_OBJECT_INIT
{
    const CLSID *pClsid;
    PFNCREATEINSTANCE pfnCreate;
};

// add classes supported by this module here
const CLASS_OBJECT_INIT c_rgClassObjectInit[] =
{
    { &CLSID_GYP, CGoPreviewHandler_CreateInstance },
	{ &CLSID_GYT, CGoThumnailHandler_CreateInstance}
};


long g_cRefModule = 0;

// Handle the the DLL's module
HINSTANCE g_hInst = NULL;

BOOL IsOs64Bit()
{
	HKEY hSubKey;

	if (::RegOpenKeyEx(HKEY_CLASSES_ROOT, "Wow6432Node", 0, KEY_READ, &hSubKey)!=ERROR_SUCCESS)
	{
		return FALSE;
	}
	RegCloseKey(hSubKey);
	return TRUE;
}

// Standard DLL functions
STDAPI_(BOOL) DllMain(HINSTANCE hInstance, DWORD dwReason, void *)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        g_hInst = hInstance;
		yictrl_initialize();
        DisableThreadLibraryCalls(hInstance);
    }
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		yictrl_finalize();
	}
    return TRUE;
}

STDAPI DllCanUnloadNow()
{
    // Only allow the DLL to be unloaded after all outstanding references have been released
    return (g_cRefModule == 0) ? S_OK : S_FALSE;
}

void DllAddRef()
{
    InterlockedIncrement(&g_cRefModule);
}

void DllRelease()
{
    InterlockedDecrement(&g_cRefModule);
}

class CClassFactory : public IClassFactory
{
public:
    static HRESULT CreateInstance(REFCLSID clsid, const CLASS_OBJECT_INIT *pClassObjectInits, size_t cClassObjectInits, REFIID riid, void **ppv)
    {
        *ppv = NULL;
        HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;
        for (size_t i = 0; i < cClassObjectInits; i++)
        {
            if (clsid == *pClassObjectInits[i].pClsid)
            {
                IClassFactory *pClassFactory = new (std::nothrow) CClassFactory(pClassObjectInits[i].pfnCreate);
                hr = pClassFactory ? S_OK : E_OUTOFMEMORY;
                if (SUCCEEDED(hr))
                {
                    hr = pClassFactory->QueryInterface(riid, ppv);
                    pClassFactory->Release();
                }
                break; // match found
            }
        }
        return hr;
    }

    CClassFactory(PFNCREATEINSTANCE pfnCreate) : _cRef(1), _pfnCreate(pfnCreate)
    {
        DllAddRef();
    }

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void ** ppv)
    {
        static const QITAB qit[] =
        {
            QITABENT(CClassFactory, IClassFactory),
            { 0 }
        };
        return QISearch(this, qit, riid, ppv);
    }

    IFACEMETHODIMP_(ULONG) AddRef()
    {
        return InterlockedIncrement(&_cRef);
    }

    IFACEMETHODIMP_(ULONG) Release()
    {
        long cRef = InterlockedDecrement(&_cRef);
        if (cRef == 0)
        {
            delete this;
        }
        return cRef;
    }

    // IClassFactory
    IFACEMETHODIMP CreateInstance(IUnknown *punkOuter, REFIID riid, void **ppv)
    {
        return punkOuter ? CLASS_E_NOAGGREGATION : _pfnCreate(riid, ppv);
    }

    IFACEMETHODIMP LockServer(BOOL fLock)
    {
        if (fLock)
        {
            DllAddRef();
        }
        else
        {
            DllRelease();
        }
        return S_OK;
    }

private:
    ~CClassFactory()
    {
        DllRelease();
    }

    long _cRef;
    PFNCREATEINSTANCE _pfnCreate;
};


STDAPI DllGetClassObject(REFCLSID clsid, REFIID riid, void **ppv)
{
    return CClassFactory::CreateInstance(clsid, c_rgClassObjectInit, ARRAYSIZE(c_rgClassObjectInit), riid, ppv);
}

// A struct to hold the information required for a registry entry

struct REGISTRY_ENTRY
{
    HKEY   hkeyRoot;
    PCWSTR pszKeyName;
    PCWSTR pszValueName;
    PCWSTR pszData;
};

// Creates a registry key (if needed) and sets the default value of the key

HRESULT CreateRegKeyAndSetValue(const REGISTRY_ENTRY *pRegistryEntry)
{
    HKEY hKey;
    HRESULT hr = HRESULT_FROM_WIN32(RegCreateKeyExW(pRegistryEntry->hkeyRoot, pRegistryEntry->pszKeyName,
                                0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL));
    if (SUCCEEDED(hr))
    {
        hr = HRESULT_FROM_WIN32(RegSetValueExW(hKey, pRegistryEntry->pszValueName, 0, REG_SZ,
                            (LPBYTE) pRegistryEntry->pszData,
                            ((DWORD) wcslen(pRegistryEntry->pszData) + 1) * sizeof(WCHAR)));
        RegCloseKey(hKey);
    }
    return hr;
}

//
// Registers this COM server
//
STDAPI DllRegisterServer()
{
    HRESULT hr;

    WCHAR szModuleName[MAX_PATH];

    if (!GetModuleFileNameW(g_hInst, szModuleName, ARRAYSIZE(szModuleName)))
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
    }
    else
    {
        // List of registry entries we want to create

        const REGISTRY_ENTRY rgRegistryEntries[] =
        {
            // RootKey            KeyName                                                                              ValueName                       Data
            {HKEY_CURRENT_USER,   L"Software\\Classes\\CLSID\\" SZ_CLSID_GYP,                           NULL,                           SZ_GOPREVIEWHANDLER},
            {HKEY_CURRENT_USER,   L"Software\\Classes\\CLSID\\" SZ_CLSID_GYP L"\\InProcServer32",       NULL,                           szModuleName},
            {HKEY_CURRENT_USER,   L"Software\\Classes\\CLSID\\" SZ_CLSID_GYP L"\\InProcServer32",       L"ThreadingModel",              L"Apartment"},
			{HKEY_CURRENT_USER,   L"Software\\Classes\\CLSID\\" SZ_CLSID_GYP,                           L"AppID",                       IsOs64Bit()==True?L"{534A1E02-D58F-44f0-B58B-36CBED287C7C}":L"{6d2b5079-2f0b-48dd-ab7f-97cec514d30b}"},
            {HKEY_CURRENT_USER,   L"Software\\Classes\\.sgf\\ShellEx\\{8895b1c6-b41f-4c1c-a562-0d564250836f}",        NULL,                           SZ_CLSID_GYP},
            {HKEY_CURRENT_USER,   L"Software\\Classes\\.ngf\\ShellEx\\{8895b1c6-b41f-4c1c-a562-0d564250836f}",        NULL,                           SZ_CLSID_GYP},
            {HKEY_CURRENT_USER,   L"Software\\Classes\\.ngfx\\ShellEx\\{8895b1c6-b41f-4c1c-a562-0d564250836f}",        NULL,                           SZ_CLSID_GYP},
            {HKEY_CURRENT_USER,   L"Software\\Microsoft\\Windows\\CurrentVersion\\PreviewHandlers",                     SZ_CLSID_GYP,  SZ_GOPREVIEWHANDLER},
        };

        hr = S_OK;
        for (int i = 0; i < ARRAYSIZE(rgRegistryEntries) && SUCCEEDED(hr); i++)
        {
            hr = CreateRegKeyAndSetValue(&rgRegistryEntries[i]);
        }
    }
    return hr;
}

//
// Unregisters this COM server
//
STDAPI DllUnregisterServer()
{
    HRESULT hr = S_OK;

    const PCWSTR rgpszKeys[] =
    {
        L"Software\\Classes\\CLSID\\" SZ_CLSID_GYP,
        L"Software\\Classes\\.sgf\\ShellEx\\{8895b1c6-b41f-4c1c-a562-0d564250836f}"
        L"Software\\Classes\\.ngf\\ShellEx\\{8895b1c6-b41f-4c1c-a562-0d564250836f}"
        L"Software\\Classes\\.ngfx\\ShellEx\\{8895b1c6-b41f-4c1c-a562-0d564250836f}"
    };

    // Delete the registry entries
    for (int i = 0; i < ARRAYSIZE(rgpszKeys) && SUCCEEDED(hr); i++)
    {
        hr = HRESULT_FROM_WIN32(RegDeleteTreeW(HKEY_CURRENT_USER, rgpszKeys[i]));
        if (hr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
        {
            // If the registry entry has already been deleted, say S_OK.
            hr = S_OK;
        }
    }
    if (SUCCEEDED(hr))
    {
        HKEY hKey;
        if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\PreviewHandlers", 0, KEY_WRITE, &hKey) == ERROR_SUCCESS)
        {
            RegDeleteValueW(hKey, SZ_CLSID_GYP);
            RegCloseKey(hKey);
        }
    }
    return hr;
}
