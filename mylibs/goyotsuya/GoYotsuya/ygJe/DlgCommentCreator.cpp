#include "DlgCommentCreator.h"
#include "resource.h"

cDlgCommentCreator::cDlgCommentCreator(void)
{
}

cDlgCommentCreator::~cDlgCommentCreator(void)
{
}

// -----------------------
void cDlgCommentCreator::xSaveCComment(const char* pszName)
{
	FILE* fp = fopen(".\\data\\cc.txt", "w");

	fseek(fp, 0, 0);


	HWND hCmbWnd = GetDlgItem(m_hWnd, IDC_CMB_OBJECT);
	int n = SendMessage(hCmbWnd, CB_GETCOUNT, 0, 0);

	for (int i=0; i<n; i++)
	{
		char buffer[4096];
		SendMessage(hCmbWnd, CB_GETLBTEXT, i, (LPARAM)buffer);
		fprintf(fp, "%d: %s\r\n", 1, buffer); 
	}


	hCmbWnd = GetDlgItem(m_hWnd, IDC_CMB_SECOND);
	n = SendMessage(hCmbWnd, CB_GETCOUNT, 0, 0);

	for (int i=0; i<n; i++)
	{
		char buffer[4096];
		SendMessage(hCmbWnd, CB_GETLBTEXT, i, (LPARAM)buffer);
		fprintf(fp, "%d: %s\r\n", 2, buffer); 
	}

	hCmbWnd = GetDlgItem(m_hWnd, IDC_CMB_THIRD);
	n = SendMessage(hCmbWnd, CB_GETCOUNT, 0, 0);

	for (int i=0; i<n; i++)
	{
		char buffer[4096];
		SendMessage(hCmbWnd, CB_GETLBTEXT, i, (LPARAM)buffer);
		fprintf(fp, "%d: %s\r\n", 3, buffer); 
	}

	hCmbWnd = GetDlgItem(m_hWnd, IDC_CMB_FOURTH);
	n = SendMessage(hCmbWnd, CB_GETCOUNT, 0, 0);

	for (int i=0; i<n; i++)
	{
		char buffer[4096];
		SendMessage(hCmbWnd, CB_GETLBTEXT, i, (LPARAM)buffer);
		fprintf(fp, "%d: %s\r\n", 4, buffer); 
	}

	fclose(fp);
}


// -----------------------
LRESULT cDlgCommentCreator::OnCreate(WPARAM wp, LPARAM lp)
{
	int size;
	char szText[256];
	int index;
	char szBuffer[4096];

	FILE* fp = fopen(".\\data\\cc.txt", "r");

	m_szResult[0]=0;

	if (fp==0)
	{
		//cyctrlbase::OnOK();
		return 0;
	}


	while(fscanf(fp, "%d: %s", &index, szText)!=EOF)
	{
		HWND hCmbWnd;
		if (index==1)
		{
			hCmbWnd = GetDlgItem(m_hWnd, IDC_CMB_OBJECT);
		}
		else if (index==2)
		{
			hCmbWnd = GetDlgItem(m_hWnd, IDC_CMB_SECOND);
		}
		else if (index==3)
		{
			hCmbWnd = GetDlgItem(m_hWnd, IDC_CMB_THIRD);
		}
		else if (index==4)
		{
			hCmbWnd = GetDlgItem(m_hWnd, IDC_CMB_FOURTH);
		}
		else
		{
			continue;
		}

		SendMessage(hCmbWnd, CB_ADDSTRING, 0, (LPARAM)szText);
	}

	fclose(fp);

	fp = fopen(".\\data\\cchistory.txt", "r");

	if (fp==0)
		return 0;

	while(fgets(szBuffer, 4096, fp)!=0)
	{
		if (strlen(szBuffer)<4)
		{
			continue;
		}
		int n = strlen(szBuffer);
		szBuffer[n-2]=0;
		m_history.addref(yistrheap(szBuffer));
		SendMessage(GetDlgItem(m_hWnd, IDC_CMB_HISTORY), CB_INSERTSTRING, 0, (LPARAM)szBuffer);
	}

	fclose(fp);

	return 0;
}

// -----------------------
LRESULT cDlgCommentCreator::OnOK()
{
	Bool bIsFound=False;
	m_szResult[0]=0;

	GetWindowText(GetDlgItem(m_hWnd, IDC_COMMENT), m_szResult, 4096);


	for (cyiNode* node=m_history.GetNodeTopFrom();node;node=node->np)
	{
		if (strcmp(m_szResult, (char*)node->ref)==0)
		{
			bIsFound=True;
			break;
		}
	}
	
	if (bIsFound==False)
	{
		//strcat(m_szResult, "\r\n");
		m_history.addref(yistrheap(m_szResult));

		FILE* fp=fopen(".\\data\\cchistory.txt", "w");

		if (fp)
		{
			for (cyiNode* node=m_history.GetNodeTopFrom();node;node=node->np)
			{
				char szComment[256];
				strcpy(szComment, (char*)node->ref);
				strcat(szComment, "\r\n");
				fputs(szComment, fp);
			}
			fclose(fp);
		}
	}
	::EndDialog(m_hWnd, True);

	return 0;
}

// -----------------------
LRESULT cDlgCommentCreator::OnCancel()
{
	m_szResult[0]=0;

	//cyctrlbase::OnCancel();
	::EndDialog(m_hWnd, True);

	return 0;
}


// -----------------------
LRESULT cDlgCommentCreator::OnBtn(HWND hWnd, int code)
{
	char szText[256];
	GetWindowText(hWnd, szText, 256);
	xAddComment(szText);
	return 0;
}

// -----------------------
LRESULT cDlgCommentCreator::OnLF(HWND hWnd, int code)
{
	xAddComment("\r\n");
	return 0;
}


//// -----------------------
//LRESULT cDlgCommentCreator::OnNO(HWND hWnd, int code)
//{
//	return 0;
//}
//
//// -----------------------
//LRESULT cDlgCommentCreator::OnHA(HWND hWnd, int code)
//{
//	return 0;
//}
//
//// -----------------------
//LRESULT cDlgCommentCreator::OnDEHA(HWND hWnd, int code)
//{
//	return 0;
//}
//
//// -----------------------
//LRESULT cDlgCommentCreator::OnMO(HWND hWnd, int code)
//{
//	return 0;
//}
//
//// -----------------------
//LRESULT cDlgCommentCreator::OnDE(HWND hWnd, int code)
//{
//	return 0;
//}
//
//// -----------------------
//LRESULT cDlgCommentCreator::OnNA(HWND hWnd, int code)
//{
//	return 0;
//}

// -----------------------
void cDlgCommentCreator::xAddComment(const char* pszComment)
{
	char szComment[4096];
	HWND hComment = GetDlgItem(m_hWnd, IDC_COMMENT);

	GetWindowText(hComment, szComment, 4096);

	strcat(szComment, pszComment);

	::SetWindowText(hComment, szComment);
}

// -----------------------
LRESULT cDlgCommentCreator::OnClear(HWND hWnd, int code)
{
	HWND hComment = GetDlgItem(m_hWnd, IDC_COMMENT);
	::SetWindowText(hComment, 0);
	return 0;
}

// -----------------------
LRESULT cDlgCommentCreator::OnCombo(HWND hWnd, int code)
{
	if (code==CBN_SELCHANGE)
	{
		char szText[256];
		int index = SendMessage(hWnd, CB_GETCURSEL, 0, 0);
		SendMessage(hWnd, CB_GETLBTEXT, index, (LPARAM)szText);

		SendMessage(hWnd, CB_DELETESTRING, index, 0);
		SendMessage(hWnd, CB_INSERTSTRING, 0, (LPARAM)szText);

		//GetWindowText(hWnd, szText, 256);
		this->xAddComment(szText);

		this->xSaveCComment(0);
	}
	return 0;
}
