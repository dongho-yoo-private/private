#pragma once


// cInformationCtrl �_�C�A���O

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
	cInformationCtrl(CWnd* pParent = NULL);   // �W���R���X�g���N�^
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


// �_�C�A���O �f�[�^
	enum { IDD = IDD_INFORMATION_CTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
};
