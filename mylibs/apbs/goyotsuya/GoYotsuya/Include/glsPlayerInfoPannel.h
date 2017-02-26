#pragma once



class cglsPlayerInfoPannel
{
public:
	cglsPlayerInfoPannel(void);
	~cglsPlayerInfoPannel(void);

	// --------------------------------------------
	Bool Create(HWND m_hWnd, Font* font, int id, int x, int y, int w, int h, const char* pszPlayerName, int nLevel, Bool bIsBlack);
};
