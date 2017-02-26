
#include "yi.h"
#include <windows.h>
#include <atlbase.h>
//#include "glsGoban.h"
#include "glsGobanContainer.h"

class CPreviewHandler : public IPreviewHandler, public IObjectWithSite, public IOleWindow, public IInitializeWithStream
{
public:
	STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	
	STDMETHODIMP SetWindow(HWND hwnd, const RECT *prc);
	STDMETHODIMP SetRect(const RECT *prc);
	STDMETHODIMP DoPreview(VOID);
	STDMETHODIMP Unload(VOID);
	STDMETHODIMP SetFocus(VOID);
	STDMETHODIMP QueryFocus(HWND *phwnd);
	STDMETHODIMP TranslateAccelerator(MSG *pmsg);

	STDMETHODIMP SetSite(IUnknown *pUnkSite);
	STDMETHODIMP GetSite(REFIID riid, void **ppvSite);

	STDMETHODIMP GetWindow(HWND *phwnd);
	STDMETHODIMP ContextSensitiveHelp(BOOL fEnterMode);

	STDMETHODIMP Initialize(IStream *pstream, DWORD grfMode);

	CPreviewHandler();
	~CPreviewHandler();

private:
	LONG                 m_cRef;
	HWND                 m_hwndParent;
	HWND                 m_hwndPreview;
	RECT                 m_rc;
	IUnknown             *m_pSite;
	IPreviewHandlerFrame *m_pFrame;	

	cglsGobanContainer* m_goban;
	IStream* m_pStream;
};

class CClassFactory : public IClassFactory
{
public:
	STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	
	STDMETHODIMP CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject);
	STDMETHODIMP LockServer(BOOL fLock);
};

