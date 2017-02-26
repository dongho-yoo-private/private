#include "yctrlGo.h"

cyctrlGo::cyctrlGo(void)
{
}

cyctrlGo::~cyctrlGo(void)
{
}

// ------------------------------
LRESULT cyctrlGo::OnCreate(WPARAM wp, LPARAM lp)
{
	RECT rect;

	::GetClientRect(m_hWnd, &rect);
	m_goban = new cyctrlGoban();
	m_goban->Create(0, 0, rect.right, rect.bottom, 19, m_hWnd, m_hInstance);
	int n = m_goban->GetSize();

	this->m_ctrler = new cyctrlController();
	m_ctrler->Create(m_hWnd, NULL, eID_CONTROLLER, n, rect.bottom-eCTRLER_HEIGHT);

	m_ctrler->Show(eCBS_DEFAULT);


	return 0;
}