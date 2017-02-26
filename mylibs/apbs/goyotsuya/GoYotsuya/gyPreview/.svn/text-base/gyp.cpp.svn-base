// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#pragma comment(lib, "shlwapi.lib")

#include "yi.h"
#include "glsGobanContainer.h"
#include <shlwapi.h>
#include <shobjidl.h>   // IPreviewHandler, IShellItem, IInitializeWithItem, IParentAndItem
#include <richedit.h>   // MSFTEDIT_CLASS
#include <msxml6.h>     // for xml-related interfaces (to parse the custom .recipe file format)
#include <assert.h>     // for assert
#include <new>

template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

inline int RECTWIDTH(const RECT &rc)
{
    return (rc.right - rc.left);
}

inline int RECTHEIGHT(const RECT &rc )
{
    return (rc.bottom - rc.top);
}

class CGoPreviewHandler : public IObjectWithSite,
                              public IPreviewHandler,
                              public IOleWindow,
                              public IInitializeWithStream
{
public:
    CGoPreviewHandler() : _cRef(1), _hwndParent(NULL), m_goban(NULL),
        _pStream(NULL), _punkSite(NULL)
    {
    }

    virtual ~CGoPreviewHandler()
    {
        if (m_goban)
        {
			m_goban->Destroy();
			delete m_goban;
			m_goban=0;
            //DestroyWindow(m_goban);
        }

        SafeRelease(&_punkSite);
        SafeRelease(&_pStream);

    }

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv)
    {
        *ppv = NULL;
        static const QITAB qit[] =
        {
            QITABENT(CGoPreviewHandler, IObjectWithSite),
            QITABENT(CGoPreviewHandler, IOleWindow),
            QITABENT(CGoPreviewHandler, IInitializeWithStream),
            QITABENT(CGoPreviewHandler, IPreviewHandler),
            { 0 },
        };
        return QISearch(this, qit, riid, ppv);
    }

    IFACEMETHODIMP_(ULONG) AddRef()
    {
        return InterlockedIncrement(&_cRef);
    }

    IFACEMETHODIMP_(ULONG) Release()
    {
        ULONG cRef = InterlockedDecrement(&_cRef);
        if (!cRef)
        {
            delete this;
        }
        return cRef;
    }

    // IObjectWithSite
    IFACEMETHODIMP SetSite(IUnknown *punkSite);
    IFACEMETHODIMP GetSite(REFIID riid, void **ppv);

    // IPreviewHandler
    IFACEMETHODIMP SetWindow(HWND hwnd, const RECT *prc);
    IFACEMETHODIMP SetFocus();
    IFACEMETHODIMP QueryFocus(HWND *phwnd);
    IFACEMETHODIMP TranslateAccelerator(MSG *pmsg);
    IFACEMETHODIMP SetRect(const RECT *prc);
    IFACEMETHODIMP DoPreview();
    IFACEMETHODIMP Unload();

    // IOleWindow
    IFACEMETHODIMP GetWindow(HWND *phwnd);
    IFACEMETHODIMP ContextSensitiveHelp(BOOL fEnterMode);

    // IInitializeWithStream
    IFACEMETHODIMP Initialize(IStream *pStream, DWORD grfMode);

    long _cRef;
    HWND                    _hwndParent;        // parent window that hosts the previewer window; do NOT DestroyWindow this
    RECT                    _rcParent;          // bounding rect of the parent window
    IUnknown                *_punkSite;         // site pointer from host, used to get _pFrame
    IStream                 *_pStream;          // the stream for the file we are previewing
	cglsGobanContainer		*m_goban;
};

// IPreviewHandler
// This method gets called when the previewer gets created
HRESULT CGoPreviewHandler::SetWindow(HWND hwnd, const RECT *prc)
{
    if (hwnd && prc)
    {
        _hwndParent = hwnd; // cache the HWND for later use
        _rcParent   = *prc; // cache the RECT for later use

		if (m_goban!=0)
		{
			//SetParent(*m_goban, _hwndParent);

			if (hwnd)
			{
				m_goban->SetParent(_hwndParent);
			}

			if (prc)
			{
				m_goban->ReSize(RECTWIDTH(_rcParent), RECTHEIGHT(_rcParent), True);
				m_goban->InvalidateRect(0);
			}
		}
	}

	return S_OK;
}

HRESULT CGoPreviewHandler::SetFocus()
{
    HRESULT hr = S_FALSE;
    if (m_goban)
    {
        ::SetFocus(*m_goban);
        hr = S_OK;
    }
    return hr;
}

HRESULT CGoPreviewHandler::QueryFocus(HWND *phwnd)
{
    HRESULT hr = E_INVALIDARG;
    if (phwnd)
    {
        *phwnd = ::GetFocus();
        if (*phwnd)
        {
            hr = S_OK;
        }
        else
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
        }
    }
    return hr;
}

HRESULT CGoPreviewHandler::TranslateAccelerator(MSG *pmsg)
{
    HRESULT hr = S_FALSE;
    IPreviewHandlerFrame *pFrame = NULL;
    if (_punkSite && SUCCEEDED(_punkSite->QueryInterface(&pFrame)))
    {
        // If your previewer has multiple tab stops, you will need to do appropriate first/last child checking.
        // This particular sample previewer has no tabstops, so we want to just forward this message out
        hr = pFrame->TranslateAccelerator(pmsg);
        SafeRelease(&pFrame);
    }
    return hr;
}

// This method gets called when the size of the previewer window changes (user resizes the Reading Pane)
HRESULT CGoPreviewHandler::SetRect(const RECT *prc)
{
    HRESULT hr = E_INVALIDARG;
    if (prc)
    {
        _rcParent = *prc;
        if (m_goban)
        {
			//m_goban->ReSize(RECTWIDTH(_rcParent), RECTHEIGHT(_rcParent));//, m_rc.bottom - m_rc.top);
			m_goban->ReSize(RECTWIDTH(_rcParent), RECTHEIGHT(_rcParent), True);//, m_rc.bottom - m_rc.top);
			m_goban->InvalidateRect(0);
        }
        hr = S_OK;
    }

    return hr;
}

extern HINSTANCE g_hInst;
// The main method that extracts relevant information from the file stream and
// draws content to the previewer window
HRESULT CGoPreviewHandler::DoPreview()
{
    HRESULT hr = E_FAIL;
    if (m_goban == NULL && _pStream) // cannot call more than once (Unload should be called before another DoPreview)
    {
		m_goban = new cglsGobanContainer();

		if (m_goban->Create(g_hInst, 0, 0, 0, RECTWIDTH(_rcParent), RECTHEIGHT(_rcParent), (HICON)_hwndParent, (HICON)0, &Font(L"Meiryo UI", 11.0), 0, 0, WS_CHILD|WS_VISIBLE)==False)
		{
			delete m_goban;
			m_goban=0;
			return E_FAIL;
		}
		if (m_goban->SetStream(_pStream)==False)
		{
			SafeRelease(_pStream);
			return E_FAIL;
		}
		
		SafeRelease(_pStream);
		return S_OK;
		//m_hwndPreview=m_goban->m_hWnd;		
    }
    return hr;
}

// This method gets called when a shell item is de-selected in the listview
HRESULT CGoPreviewHandler::Unload()
{
    SafeRelease(&_pStream);
    if (m_goban)
    {
		m_goban->Destroy();
		delete m_goban;
		m_goban=0;
    }

    return S_OK;
}

// IObjectWithSite methods
HRESULT CGoPreviewHandler::SetSite(IUnknown *punkSite)
{
    SafeRelease(&_punkSite);
    return punkSite ? punkSite->QueryInterface(&_punkSite) : S_OK;
}

HRESULT CGoPreviewHandler::GetSite(REFIID riid, void **ppv)
{
    *ppv = NULL;
    return _punkSite ? _punkSite->QueryInterface(riid, ppv) : E_FAIL;
}

// IOleWindow methods
HRESULT CGoPreviewHandler::GetWindow(HWND* phwnd)
{
    HRESULT hr = E_INVALIDARG;
    if (phwnd)
    {
        *phwnd = _hwndParent;
        hr = S_OK;
    }
    return hr;
}

HRESULT CGoPreviewHandler::ContextSensitiveHelp(BOOL)
{
    return E_NOTIMPL;
}

// IInitializeWithStream methods
// This method gets called when an item gets selected in listview
HRESULT CGoPreviewHandler::Initialize(IStream *pStream, DWORD)
{
    HRESULT hr = E_INVALIDARG;
    if (pStream)
    {
        // Initialize can be called more than once, so release existing valid _pStream
        SafeRelease(&_pStream);

        _pStream = pStream;
        _pStream->AddRef();
        hr = S_OK;
    }

    return hr;
}


HRESULT CGoThumnailHandler_CreateInstance(REFIID riid, void **ppv)
{
	*ppv = NULL;

	CGoThumnailHandler *pNew = new (std::nothrow) CGoThumnailHandler();

    if (SUCCEEDED(hr))
    {
        hr = pNew->QueryInterface(riid, ppv);
        pNew->Release();
    }
    return hr;
}


HRESULT CGoPreviewHandler_CreateInstance(REFIID riid, void **ppv)
{
    *ppv = NULL;

    CGoPreviewHandler *pNew = new (std::nothrow) CGoPreviewHandler();
    HRESULT hr = pNew ? S_OK : E_OUTOFMEMORY;
    if (SUCCEEDED(hr))
    {
        hr = pNew->QueryInterface(riid, ppv);
        pNew->Release();
    }
    return hr;
}
