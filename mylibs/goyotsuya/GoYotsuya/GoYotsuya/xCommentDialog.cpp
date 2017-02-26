#include "yistd.h"
#include "resource.h"

cyiList list;

typedef struct xsComment {
	int id;
	int len;
	Bool bIsLocked;
	char* comment;
}sComment;

// ----------------------------------------------------
static Bool xLoadCommentDictionary(cyiList& list)
{
	list.clear();
	
	//void* p = yiFileLoad("comment.dat", &size);
	//
	//if (p==0)
	//	return False;

	//sComment* s = (sComment*)p;
	//char* pIndex;
	//
	//while(s->len!=0)
	//{
	//	sComment* ret = xCreatesComment(s);
	//	list.addref(ret);
	//	
	//	pIndex=(char*)&s[1];
	//	s=(sComment*)&p[s->len];
	//}

	//yiFileUnload(p);

	return True;
}

// ----------------------------------
INT_PTR CALLBACK xCommentListDlg(HWND hWnd, UINT iMsg, WPARAM wp, LPARAM lp)
{
	switch(iMsg)
	{
		case WM_INITDIALOG:
		{
			list.clear();

			//xLoadCommentDictionary(list);
			//xSyncList(list);

			return 1;
		}
		case WM_COMMAND:
		{
			int ctrlId = (0xFFFF&wp);

			if (ctrlId==IDOK)
			{
				DestroyWindow(hWnd);
				return 1;
			}
			//else if (ctrlId==IDC_TEXT_ADD)
			//{
			//	xSaveCommentDictionary(list);
			//}
			return 1;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 1;
		}
		default:
			break;
	}

	return 0;
}

// ----------------------------------
Bool xShowCommentDialog(HWND hWnd)
{
	HWND hNotifyWnd=hWnd;

	HWND hRemocon = CreateDialogParamA((HINSTANCE)GetWindowLong(hNotifyWnd, GWL_HINSTANCE), MAKEINTRESOURCEA(IDD_COMMENT_DIALOG),hNotifyWnd, xCommentListDlg, (LPARAM)hNotifyWnd);

	RECT rect;
	RECT rect2;
	GetClientRect(hRemocon, &rect);
	GetClientRect(hNotifyWnd, &rect2);
	SetWindowPos(hRemocon, 0, rect2.right-rect.right, rect2.bottom-rect.bottom,0,0, SWP_NOSIZE|SWP_NOZORDER);


	SetFocus(hNotifyWnd);

	return hRemocon?True:False;
}