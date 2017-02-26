#pragma once


// cInformationCtrl ダイアログ

enum eInformationControlMode {
	eICM_PLAY,
	eICM_EDIT,
	eICM_ENJOY
};

typedef struct xsInformationControl 
{
	BOOL bIsBlack;
	char szTitle[32];
	char szUserName[16];
	int  level;
	unsigned int time;
	int ageishi;
	unsigned int matches;
	unsigned int total_score;
} sInformationControl;

class cInformationCtrl : public CDialog
{
	DECLARE_DYNAMIC(cInformationCtrl)

public:
	cInformationCtrl(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~cInformationCtrl();

public:
	eInformationControlMode mode;

	// --------------------------
	void Set(sInformationControl* info);

	// --------------------------
	void SetTimeMinus(int sec);

	// --------------------------
	void AddAgeStone(int n);

	// --------------------------
	afx_msg void OnPaint();


// ダイアログ データ
	enum { IDD = IDD_INFORMATION_CTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
