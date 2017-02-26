#pragma once
#define NEW_STYLE

#include "yi.h"
#include <windows.h>
#include "yiGoDefs.h"
#include "yiGoEngine.h"
#include "yiList.h"
#include "yiQueue.h"
#include "yctrls.h"
//#include "cyctrlx.h"
#include "yiDrawObject.h"
#include "yimutex.h"
#include "GlassInterface.h"
#include "yiResource.h"

#include "glsTabCtrl.h"
#include "glsButton.h"

#ifndef NEW_STYLE
#include "glsGobanControl.h"
#include "glsTitlePannel.h"
#include "glsInformationDlg.h"
#include "glsStoneInfoDlg.h"
#include "glsPlayerPannel.h"
#include "glsTextPannel.h"
#include "glsMyView.h"
#include "glsMyFriendView.h"
#else
#include "glsGobanControlEx.h"
#include "glsControlBar.h"
#endif

#include "glsPopupMenu.h"
#include "glsPreviewGoban.h"



#include "glsToolContainer.h"
#include "glsDrawObject.h"
#include "glsImageBox.h"
#include "glsListMenu.h"

#include "UserManager.h"
#include "glsGobanCommon.h"


//#include "cydlgx.h"

#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")

#define xCHILD_ID_TAB			(128+2)
#define xCHILD_ID_CTRL_FOR_EDIT (128+3)
#define xCHILD_ID_CTRL_FOR_PLAY (128+4)
#define xCHILD_ID_CTRL_FOR_CAL	(128+5)
#define xCHILD_ID_CTRL_FOR_ENJOY (128+6)

#define xCHILD_ID_TITLE_DIALOG (128+7)

#define CHILD_ID_CURSOR (64+1)
#define CHILD_ID_POS_X  (64+2)
#define CHILD_ID_POS_Y  (64+3)

#define CHILD_ID_GOBAN 1
#define CHILD_ID_GOBAN_TAB xCHILD_ID_TAB
#define CHILD_ID_GOBAN_MENU (128+8)
#define CHILD_ID_GOBAN_PREVIEW (128+9)

#define xCHILD_ID_INFORMATION_DIALOG (128+10)

#define xCHILD_ID_BLACK_PANNEL (128+11)
#define xCHILD_ID_WHITE_PANNEL (128+12)
#define xCHILD_ID_TEXT_PANNEL (128+13)
#define xCHILD_TOOL (128+14)

#define xCHILD_ID_BTN_MYPAGE (256+16)
#define xCHILD_ID_BTN_MYFRIEND (256+17)
#define xCHILD_ID_BTN_MYGROUP (256+18)
#define xCHILD_ID_BTN_GROUP_DETAIL (256+19)
#define xCHILD_ID_BTN_DETAIL (256+20)

#define xCHILD_ID_STONE_INFO_DLG (256+21)

#define xCHILD_ID_RESULT (256+32)

#define xCHILD_ID_STUDY_START 512


// MESSAGEBOX 512～512+128
#define xID_MESSAGEBOX 512

// BALOON (512+128)～+128
#define xID_BALOON (512+128)

#define xCHILD_EDITBOX (xID_BALOON+128)

#define CHILD_ID_GOBAN_END 5

#define WM_PUTSTONE (WM_USER+256)
#define WM_GOBAN_CHANGED (WM_USER+257)

// 最大作成可能な碁盤の数
#define MAX_GOBAN_COUNT 128

//#ifdef _DEBUG
//#pragma comment(lib, "C:\\MyProject\\Libs\\yiLibsd.lib")
//#else
//#pragma comment(lib, "C:\\MyProject\\Libs\\yiLibs.lib")

//#endif



enum eGobanLayer {
	eGL_STONE = 0x01,
	eGL_LABEL = 0x02, // ラベル、数字、地合計算など（描画時動的に生成）
	eGL_CURSOR = 0x04,//
	eGL_CALCULATE = 0x10,
	eGL_CALCULATE_END = 0x20,
	eGL_ALL = eGL_STONE|eGL_LABEL
};


#define INDEX_TOP 0x1FFFFFFF
#define INDEX_END 0x0FFFFFFF

// ------------------------------------
//typedef struct xsGobanContentsDetail {
//	int nLineCount;
//	eBranchViewMode nBranchViewMode;
//	eInputMode input;
//	eMouseMode mouseMode;
//	eGoContentsMode mode;
//	eCharacterSet eCharSet;
//	eGoFileType format, 
//}sGobanContentsDetail;

enum eGobanControlPos {
	eGCP_LEFT=0x01,
	eGCP_CENTER=0x02,
	eGCP_RIGHT=0x04,
	eGCP_TOP=0x10,
	eGCP_VCENTER=0x20,
	eGCP_BOTTOM=0x40,

	eGCP_DEFAULT=(eGCP_RIGHT|eGCP_BOTTOM)
};

enum eGobanViewMode {
	eGVM_NONE=0,
	eGVM_ALL=0xFF
};



typedef struct xsGobanDetail
{
	int nDefaultLineCount;
	Bool bIsPreviewGotoSame;
	Bool bIsPreviewClikFocus;
	eGobanAppMode mode;
	//Bool bIsJosekiEditorMode;
	Bool bIsDefaultLabelShow;
	Bool bIsPreviewNewTab;
	Bool bUseContorlClik;
	Bool bUseShiftClick;
	Bool bIsPrevewShow;
	Bool bIsPreviewPopup;
	Bool bIsNormalQuit;
	Bool bIsOldInterface;
	eGobanControlPos eControlPos;
	cglsPopupMenu* menu;
} sGobanDetail;

//typedef struct {
//	int nMessageBox;
//
//} sGobanExtraData;

typedef struct {
	//// 秒読みに突入したかどうか？
	//Bool bIsSecondRead;
	//// 残り秒読みカウント
	//int  nRemainSecondReadCount;
	//// 開始時間
	//unsigned int nStartMilliSec;
	//// 全体時間(0:無し、0xFFFFFFFF:無限)
	//unsigned int nTotalMilliSec;
	//// 残り時間
	//unsigned int nRemainMilliSec;

	unsigned int startTime;
	unsigned int usedTime;
	unsigned int totalUsedTime;
	unsigned int remainCount;

} sGoRemainTime;

#ifdef _DEBUG
//#define _TIMER_DEBUG_ 1

#endif

#include "glsBaloon.h"
typedef struct {
	sGoRemainTime black;
	sGoRemainTime white;
	unsigned int timeStart;
	unsigned int condition1;
	unsigned int condition2;
	ugoid_t		 uid;
	void*		hUser;
	cglsDialog* dlg;
	unsigned int last_put_time;
	int hagasiCnt;
	cglsBaloon* baloon;

#ifdef NEW_STYLE
	int nLastControlIndex;
#endif
} sGoPlayInfo;



typedef struct {
	sGoHeader header;
	unsigned int n;
	map_t map;
} sGoHeaderEx;

//typedef struct {
//	int goid;
//	cglsDialog* dlg;
//	unsigned int last_put_time;
//	int hagasiCnt;
//} sGobanExtraDetail;

enum eGobanControlType
{
	eGCT_TITLE=0,
	eGCT_CONTROL,
	eGCT_VIEW,
	eGCT_COUNT
};

enum eGobanMode
{
	eGM_VIEW,
	eGM_JOSEKI_JITEN,
	eGM_GO_MESSAGE,
	eGM_MAIL_GO
};

enum eGobanControlBaseId {
	eGCBID_BASE	= eGCS_BASE+1,
	eGCBID_EDIT = eGCS_EDITOR+1,
	eGCBID_CALCULATE = eGCS_CALCULATE+1

};

// ------------------------------------
class  xEXPORT  cglsGoban : public cGlassInterface, public cglsDrawObject
{
public:
	cglsGoban(void);
	virtual ~cglsGoban(void);


// 碁盤生成・削除など
	// ----------------------------------------
	static ugoid_t& MakeUniversalGoId(ubit32_t myId, ubit32_t remoteId, sSimpleGoHeader* hd, Bool* bReverse=0);

	// -------------------------------------------------------
	//static ugoid_t& MakeUniversalGoId(unsigned int black, unsigned int white);

	// ----------------------------------------
	static stone_t xChangeStoneCoordinate(stone_t stone, int nLineCnt);

	// ------------------------------------------------------
	static void xDefaultDetail(sGobanDetail* detail);

	// ------------------------------------------------------
	//BOOL Create(long x, long y, long w, long h, long nDefaultLineCount, HWND hOwnerWnd);

	// ------------------------------------------------------
	// 最終的にはこのように
	BOOL Create(HWND hWndParent, long x, long y, long w, long h, CUserManager* mgr=0, sGobanDetail* detail=0);



	// ------------------------------------------------------
	// プレビューなどで使われる形式。
	Bool CreateForSimple(cGlassInterface* parent, HINSTANCE hInstance, int id, int x, int y, int w, int h);

	// ------------------------------------------------------
	void Destroy();

	// ------------------------------------------------------
	goid_t ReserveId();

	
	// ------------------------------------------------------
	void SetSendStoneId(int id);

	// ------------------------------------------------------
	void SetAddNewId(int id);


	// ------------------------------------------------------
	cyiGoEngine* GetCurrentGoEngine();

	// ------------------------------------------------------
	goid_t* GetCurrentPlayGobanList();

	// ------------------------------------------------------
	sSimpleGoHeader* GetSimpleGoHeader(goid_t id);

	// ------------------------------------------------------
	const char* GetFileName(goid_t id);

	// ------------------------------------------------------
#define NODE_END (cyiTreeNode*)0xFFFFFFFF
#define NODE_CUR 0
	goid_t CopyAndAdd(goid_t id=0, eInputMode inputMode=eIM_EDIT, cyiTreeNode* node=NODE_CUR, Bool bUntilCur=True);

	void ReleaseMergeData(cyiList* list);

	// ------------------------------------------------------
	cyiList* GetMergeData(goid_t goid, char* file, Bool bIsNormalSgf=True);

private:

	// ------------------------------------------------------
	void FreeId(goid_t id);

	// ------------------------------------------------------
	unsigned int m_nSendStoneId;

	// ------------------------------------------------------
	unsigned int m_nAddNewId;

	// ------------------------------------------------------
	HCURSOR m_hCurrentCursor;

	// ------------------------------------------------------
	Bitmap* xCreatePosImage(int index, Bool bIsVertical);

	HINSTANCE m_hResourceInstance;

	Bool m_bIsPreviewMode;


	// ------------------------------------------------------
	//cglsImageBox* m_imbResult;

	// ------------------------------------------------------
	cglsImageBox* m_imbCursor;

public:


	// ------------------------------------------------------
	goid_t AddForPlay(goid_t rId, sGoPlayInfo* info, const char* pszBlackName, const char* pszWhiteName, const char* pszTitle, 
		unsigned int condition, unsigned int condition2, unsigned int flag=0, ugoid_t* uid=0, sSimpleGoData* godata=0, int godataSize=0);

	// ------------------------------------------------------
	goid_t AddForPlayBySimpleGoData(ubit32_t myId, ugoid_t& id, sSimpleGoData* data);

	// ------------------------------------------------------
	//goid_t AddForPlay(goid_t rId, sGoPlayInfo* info, sSimpleGoData* data, int size);

	// ------------------------------------------------------
	goid_t AddForStream(IStream* stream);


	// ------------------------------------------------------
	goid_t AddForWatch(sSimpleGoData* header, int size, ugoid_t& id, const char* pszBlackName, const char* pszWhiteName);

	// ------------------------------------------------------
	goid_t AddForEditBySimpleGodata(sSimpleGoData* data, int size, const char* pszBlackName=0, const char* pszWhiteName=0, eInputMode eInputMode=eIM_EDIT);

	// ------------------------------------------------------
	goid_t AddForEditByFile(const char* pszFileName, sGobanContentsDetail* detail=0);

	// ------------------------------------------------------
	goid_t AddForEditByFile(void* p, int size, sGobanContentsDetail* detail=0);

	// ------------------------------------------------------
	goid_t AddForEdit(const char* pszBlackName, const char* pszWhiteName, const char* pszTitle, unsigned int condition, unsigned int flag);

	// ------------------------------------------------------
	goid_t Add(sGoHeader* header, goid_t reservedId=0, sGobanContentsDetail* detail=0, void* pExtraData=0, ugoid_t* uid=0, color_t color=0, Bool bHasButton=True);

	// ------------------------------------------------------
	goid_t AddByNode(cyiTreeNode* node, const char* pszTitle, Bool bIsEditable=False, Bool bIsActivate=False);

	// ------------------------------------------------------
	goid_t AddByReference(const char* pszTitle, Bool bIsEditable=False);

	// ------------------------------------------------------
	goid_t FindGoban(ugoid_t& id);

	// ------------------------------------------------------
	sSimpleGoData* ExportSimpleGoData(unsigned int goid, int* size);

	// ------------------------------------------------------
	sSimpleGoData* ExportSimpleGoData(ugoid_t& id, int* size);



	// ------------------------------------------------------
	ugoid_t* GetUniversalGoid(goid_t id);

	// ------------------------------------------------------
	Bool GetPlayTime(goid_t goid, int* cur, int* cnt, Bool bIsBlack);

	// ------------------------------------------------------
	int ImWin(goid_t goid);

	// ------------------------------------------------------
	int DeleteGoban(goid_t id, Bool bIsFree);

	// ------------------------------------------------------
	const sGobanDetail* GetGobanDetail();

	// ------------------------------------------------------
	int RegisterNodeChangeNotifyWindow(HWND hWnd);

	// ------------------------------------------------------
	int UnregisterNodeChangeNotifyWindow(HWND hWnd);

	// ------------------------------------------------------
	int ShowMessage(int id, const wchar_t* psaMessage, const wchar_t* pCaption, int btn, int notifycode=0);

	// ------------------------------------------------------
	Bool ShowBaloon(goid_t id, const wchar_t* pszText, int i, int j);


	// ------------------------------------------------------
	int MessageIdToGoId(int messageId);

	// ------------------------------------------------------
	//int ShowQuestion(goid_t goid, int id);

	// ------------------------------------------------------
	void PauseTimer(goid_t goid);

	// ------------------------------------------------------
	void PlayTimer(goid_t goid);

	// ------------------------------------------------------
	Bool StartTimer(goid_t goid);

	// ------------------------------------------------------
	unsigned int StopTimer(goid_t goid, unsigned int nUsedTime);

	// ------------------------------------------------------
	int GetRemainTime(goid_t goid, Bool bIsBlack);

	// ------------------------------------------------------
	int GetTotalUsedTime(goid_t goid, Bool bIsBlack);

	// ------------------------------------------------------
	void SetTotalRemainTime(goid_t goid, int time, Bool bIsBlack);

	// ------------------------------------------------------
	void SetPlayTime(goid_t goid, int curr, int nRemainReadSecCount, Bool bIsBlack);

	// ------------------------------------------------------
	void SetRemainTime(goid_t goid, int time, Bool bIsBlack, Bool bIsMinus=True);

	// ------------------------------------------------------
	void AppendMessage(goid_t id, const wchar_t* message, int size);

	// ------------------------------------------------------
	void SetMyInfo();

	// ------------------------------------------------------
	cglsToolContainer* GetContainer();

	// ------------------------------------------------------
	void ShowResult(goid_t id, unsigned int result);

	// ------------------------------------------------------
	cyiList* GetCommentList(goid_t id);

	// ------------------------------------------------------
	void ReleaseCommentList(cyiList* list);

	// ------------------------------------------------------
	cyiList* GetProblemList(goid_t id);

	// ------------------------------------------------------
	void ReleaseProblemList(cyiList* list);

	// ------------------------------------------------------
	//Bool ChangeOrderForced(Bool bIsOrderBlack);
	Bool ChangeTimer(goid_t id, Bool bIsOrderBlack, unsigned int usedTime, sGoPlayInfo* info, Bool bIsLastRemainTime=False);

	// ------------------------------------------------------
	Bool SetDeadStone(int nBlack, int White);

	// ------------------------------------------------------
	void SetSecondReadCount(goid_t id, Bool bIsOrderBlack, int nCount);

	// ------------------------------------------------------
	int GetSecondReadCount(goid_t id, Bool bIsOrderBlack);

	// ------------------------------------------------------
	void SetSecondRead(goid_t id, int n);

#ifdef NEW_STYLE
	virtual Bool GetEffectedBg(Bitmap* dest, int x, int y, int sx, int sy, int w, int h);

	// ------------------------------------------------------
	void SetSubTitle(wchar_t* title);

	// ------------------------------------------------------
	const wchar_t* GetSubTitle();
#endif

private:
	// ------------------------------------------------------
	goid_t xAddReference(goid_t id, cyiTreeNode* node, int nIndex, goid_t parent, cyiTreeNode* from=0);

	// ------------------------------------------------------
	int xDeleteReference(goid_t id, Bool bIsFree=False);

	// ------------------------------------------------------
	Bool xUpdateTab();

#ifndef NEW_STYLE
	// ------------------------------------------------------
	Bool UpdateMyView(int index, unsigned int userId, unsigned int groupId, int flag);

	// ------------------------------------------------------
	cglsDialog* GetMyView(int index);

	// ------------------------------------------------------
	Bool xCreateMyViews(CUserManager* mgr);

	// ------------------------------------------------------
	Bool xCreateMyButtons();
#endif



	// ------------------------------------------------------
	virtual Bool CreateBitmap(sDrawItem* item, void* pExtra=0);

	//----------------------------------------
	//Bool xCreateSlideControl();

public:
#ifdef NEW_STYLE
	// about control.
	//----------------------------------------
	Bool AddControl(cGlassInterface* gls, eGobanControlType type, eGobanMode eGobanMode);

	//----------------------------------------
	Bool ShowControl(int id, eGobanControlType type);

	//----------------------------------------
	cGlassInterface* GetControl(int id, eGobanControlType type);

	// ---------------------------------------
	void SetTitleText(const wchar_t* text);

	// ---------------------------------------
	void SetTitleText(const char* text);
#endif



	// ------------------------------------------------------
	void ChangeOrder();

	// ------------------------------------------------------
	void Remove(goid_t id);

	// ------------------------------------------------------
	unsigned int GetCount();

	// -----------------------------------------
	goid_t GetCurrentGoId();

	// -----------------------------------------
	void* GetExtraData(goid_t id);

	// -----------------------------------------
	Bool IsCurrentBlack(goid_t id=0);
	
	// -----------------------------------------
	Bool ImBlack(goid_t id=0);

	// -----------------------------------------
	void GetSize(int* all=0, int* goban=0, int* tab=0);

	// -----------------------------------------
	float GetScale();

	// -----------------------------------------
	// ロケール変更により、再描画が行われる。
	void ChangeLocale(goid_t id, eCharacterSet eLocale=eCS_JAPANESE);

protected:
	// -----------------------------
	virtual void Update(Bitmap* bmp, RECT* rect=0, RECT* _childRect=0, int offsetX=0, int offsetY=0, Bool bIsLocalBufferUpdate=False);
public:

// 選択されている碁盤に対する機能
	// ------------------------------------------------------
	// 番号表示	
	void ShowNo(Bool bIsTrue, int nFrom=0);

	// -------------------------------
	// 石を置く
	stone_t** PutStone(goid_t id, stone_t stone, unsigned int usedMilliSec=0, Bool bIsFromNetwork=False);

	// -------------------------------
	stone_t** PutStone(ugoid_t& uid, stone_t stone, unsigned int usedMilliSec=0);

	// -------------------------------
	void PlayByNode(cyiTreeNode* node);


	// -------------------------------
	// ラベルを置く
	Bool PutLabel(goid_t id, int i, int j, wchar_t label);

	// -------------------------------
	// ラベルリストを取得
	cyiList* GetLabel(goid_t id);

	// -------------------------------
	// コメントを入力
	Bool PutComment(goid_t id, LPCSTR comment);

	// -------------------------------
	// コメントを取得
	LPCSTR GetComment(goid_t id, LPCSTR comment);

	// --------------------------------
	//
	stone_t** Goto(goid_t id, int nIndex, aStoneList branches, Bool bFromNetwork=False);//, eInputMode mode=eIM_ENJOY);
	
	// --------------------------------
	Bool StartFindComment();

	// --------------------------------
	Bool EndFindComment();

	// --------------------------------
	stone_t** GotoNextComment(int nIndex=-1);

	// --------------------------------
	stone_t** GotoPrevComment();

	// --------------------------------
	stone_t** GotoNextBranch(goid_t id, aStoneList branches);

	// --------------------------------
	stone_t** GotoPrevBranch(goid_t id, aStoneList branches);

	// --------------------------------
	void FreeInitStoneMode(int count);

	// ------------------------------------------------------
	void DeleteNode(cyiTreeNode* node, Bool bIsCurrentOnly=False);

	// ------------------------------------------------------
	void DeleteNode(goid_t goid, cyiTreeNode* node, Bool bIsCurrentOnly=False);

	// ------------------------------------------------------
	void NotifyNodeChanged(Bool bIsForced=False, Bool bIsNotUseQueue=False);

	// ------------------------------------------------------
	LRESULT OnSearchResultEnd(WPARAM wParam, LPARAM lParam, Bool& handled);



// 碁の属性に関係するもの
	// ------------------------------------------------------
	LPCSTR GetTitle(goid_t id=0);

	// ------------------------------------------------------
	LPCSTR GetBlackName(goid_t id=0);

	// ------------------------------------------------------
	void SetBlackName(LPCSTR name, goid_t id=0);

	// ------------------------------------------------------
	LPCSTR GetWhiteName(goid_t id=0);

	// ------------------------------------------------------
	void SetWhiteName(LPCSTR name, goid_t id=0);

	// ------------------------------------------------------
	unsigned int GetDetailInfo(goid_t id=0);

	// ------------------------------------------------------
	void SetDetailInfo(unsigned int condition, goid_t id=0);

	// ------------------------------------------------------
	unsigned int GetDetailFlag(goid_t id=0);

	// ------------------------------------------------------
	void SetDetailFlag(unsigned int flag, goid_t id=0);

#ifndef NEW_STYLE
	// ------------------------------------------------------
	Bool Resize(long w, long h, Bool bIsNoActivate=False);

	// ------------------------------------------------------
	Bool Resize(int x, int y, long w, long h, Bool bIsNoActivate=False);
#endif

#ifdef NEW_STYLE
#define Resize ReSize
	// ------------------------------------------------------
	virtual Bool ReSize(UINT x, UINT y, Bool bIsRepaint=False);

	// ------------------------------------------------------
	virtual Bool ReSize(int x, int y, long w, long h, Bool bIsNoActivate=False);
#endif

	int GetRealWidth();

	// ------------------------------------------------------
	virtual Bool Move(unsigned int x, unsigned int y, Bool bIsNoActivate=False);

	// ------------------------------------------------------
	void SetNotifyWindow(HWND hWnd);

	// ------------------------------------------------------
	// -1を指定すると現在すでに出力されているラベルの連番にする。
	Bool SetLabelMode(eLabelType type, wchar_t label=-1);

	// -------------------------------
	// いるかな？
	void PlayStop(unsigned int id);

	// 定石辞典のためにだけ追加
	Bool m_bLabelShow;
	// -------------------------------
	Bool ShowHideLabel();

	// -------------------------------
	Bool ShowCursor(Bool bIsShow);

	// -------------------------------
	//int ShowMessage(unsigned int id, const wchar_t* message, int flag, int timeout=0);

	// ------------------------------------
	// とりあえず廃止
	//virtual Bitmap* GetBackGround(int x, int y, int w, int h);

	// ------------------------------------------------------
	BOOL ChangeGoban(goid_t id);

	// -------------------------------------------------------
	// 地合い計算モードに切り替え、あげ石などの指定
	void StartCalculateMode(goid_t id);	

	// -------------------------------------------------------
	// 地合い計算終了。
	stone_t** CalculateEnd(goid_t id, int* xnBlack, int* xnWhite, int& winer, int& score, Bool bShowDialog=True);


	// -------------------------------------------------------
	// 地合い計算結果をキャンセル
	Bool CancelCalculateResult(goid_t id, Bool bReturnPlayMode=False);

	// -------------------------------------------------------
	// 地合い計算した後
	// 結果表示は時間指定が可能で、-1の場合入力があれば消える。
	// 表示が消えてからは、編集モードに切り替わる。
	Bool ShowResult(int nShowTime=-1);

	// ------------------------------------------------------------------
	// 碁盤の入力モード切り換え
	void ChangeInputMode(eInputMode input);

	void ChangeInputMode(goid_t id, eInputMode input);

	eInputMode GetInputMode(goid_t id);

	// -------------------------------------------------------
	cyiTreeNode* cglsGoban::GetCurrentNode();


	// -------------------------------------------------------
	//UINT GetSize();

	// ----------------------------------------
	Bool Save(const char* pszFileName, eGoFileType type=eGFT_SGF, goid_t id=0xFFFFFFFF);

	// ----------------------------------------
	Binary Export(const char* pszFileName, sSoftwareLisenseCode* license);

	// ----------------------------------------
	Bool GetLicenseCode(const char* pszFileName, sSoftwareLisenseCode* license);


	// ----------------------------------------
	virtual Bitmap* GetCurrentBitmap(sDrawItem* item=0);

	// commands
	// コマンド

	// 一手再生
	void Play();

	// -------------------------------------------------------
	void SearchResult(goid_t id=0);

	// ------------------------------------------------------
	void CreateMapIamge(cyiTreeNode* node);

	// ------------------------------------------------------
	void SetBrancheViewMode(eBranchViewMode mode=eBVM_NORMAL);

	// -----------------------------------
	int OptimizeNode(int* nProgress, Bool* canceled);

	// -----------------------------------
	int GetFullNodeCnt();

	// -----------------------------------
	Bool ShowPreviewWindow(Bool bIsShow=True);

	// ---------------------------------------------------------------
	int GetCurrentOrder(goid_t id);

	// ---------------------------------------------------------------
	Bool IsNoDisplayComment(goid_t id);

	// ---------------------------------------------------------------
	void AppendComment(goid_t id, const char* pszComment);

	//// -----------------------------------
	//Bool SetMyView(sMyViewInfo* info, eMyViewStyle style = eMVS_NORMAL, CUserManager* mgr=0);

	//// -----------------------------------
	//Bool SetMyFriendView(CUserManager* mgr);

	//// -----------------------------------
	//cglsMyView* GetMyView();

	//// -----------------------------------
	//cglsMyFriendView* GetMyFriendView();

	// -----------------------------------
	Bool ReadHeader(const char* pszFileName, sGoHeaderEx* header);

	// -----------------------------------
	void SetPlayResult(int goid, int winer, int score, Bool bIsShowResult=False, sResult* result=0);

	// -----------------------------------
	Bool GetHeaderEx(int goid, sGoHeaderEx* header);

public:
	//HWND	m_hWnd;

	// -----------------------------------
	Bool Lock(goid_t id);

	// -----------------------------------
	Bool Unlock(goid_t id);

	// -----------------------------------
	Bool IsChanged();

	// -----------------------------------
	Bool SetChanged(goid_t id, Bool bIsChanged);

	// -----------------------------------
	void SetChangedFlag(Bool bIsChanged);

	// ---------------------------------------------------------------
	LRESULT OnShowMenu(HWND hWnd, int code);

	// ---------------------------------------------------------------
	LRESULT OnClickTop(HWND hWnd, int code);

	// ---------------------------------------------------------------
	LRESULT OnClickPP(HWND hWnd, int code);
	
	// ---------------------------------------------------------------
	LRESULT OnClickPrev(HWND hWnd, int code);
	
	// ---------------------------------------------------------------
	LRESULT OnClickNext(HWND hWnd, int code);
	
	// ---------------------------------------------------------------
	LRESULT OnClickNN(HWND hWnd, int code);
	
	// ---------------------------------------------------------------
	LRESULT OnClickEnd(HWND hWnd, int code);

	// ---------------------------------------------------------------
	LRESULT OnResultImageClicked(HWND hWnd, int code);

	// ---------------------------------------------------------------
	LRESULT OnTitleChanged(HWND hWnd, int code);

#ifndef NEW_STYLE
	// ---------------------------------------------------------------
	LRESULT OnChangeView(WPARAM wParam, LPARAM lParam);
#endif

	// ---------------------------------------------------------------
	virtual LRESULT OnNotHandledCommand(WPARAM wp, LPARAM lp);

////
private:

	// -----------------------------------
	Bitmap* CreateResultBitmap(int winer, int score, Bool bIsHalfScore);

	// -----------------------------
	void InitMember();

	// ----------------------------------------
	void InitGraphics(HINSTANCE hInstance=0);

	// ----------------------------------------
	int SetBranch(cyiGoEngine* eng, stone_t* pass=0);

	// ---------------------------------------------------------------
	Bool IsBranches(int x, int y);

	// ---------------------------------------------------------------
	Bool xDeleteTab(int id, Bool bIsDeleteSelf=True);

#ifndef NEW_STYLE
	// ----------------------------
	// 碁盤のサイズ
	int m_nGobanWidth;
#endif

// バックグラウンドレイヤー
// ダブルバッファーリングを行う
	// ------------------------------------------------------------------
	void DrawBackGroundLayer();

	// ------------------------------------------------------------------
	unsigned int _DrawBackGroundLayer(Bool bIsInit=False);

	// ------------------------------------------------------------------
	friend unsigned int __stdcall xOnDrawBackGround(void* param);

	// ------------------------------------------------------------------
	friend void xOnDrawBackGroundx(void* param);

	// ------------------------------------------------------------------
	cyiSurface* m_BgLayer[2];

	// ------------------------------------------------------------------
	Bool m_BgLayerFlag[2];

	// ------------------------------------------------------------------
	unsigned int m_UsedLayerIndex;

	// ------------------------------------------------------------------
	unsigned int m_nRealCoordinateX[19];

	// ------------------------------------------------------------------
	unsigned int m_nRealCoordinateY[19];

	// ------------------------------------------------------------------
	HandleMutexx m_hDrawMutex;

	// ------------------------------------------------------------------
	Bool m_bReqiredLayerUpdate;

	cyiStack m_branches;

// 碁石の描画
	// ------------------------------------------------------------------
	void DrawStonez(stone_t** stone, Bool bIsShowNo=False, int nNoFrom=0);

	// ------------------------------------------------------------------
	void DrawNo();

// ラベルの描画
	// ------------------------------------------------------------------
	void DrawLabel(int i, int j, wchar_t label, stone_t stone);

	// ------------------------------------------------------------------
	// ブランチを描画
	void DrawBranch(int index, stone_t stone);

	// ------------------------------------------------------------------
	void DrawPassBranch(int x, int y);

	// ------------------------------------------------------------------
	// カーソルの描画
	void DrawCursor(HDC hdc, int i, int j, Bool bIsBlack, unsigned int key=0, Bool bIsForced=False);

	// ------------------------------------------------------------------
	//
	// 0: デフォルトノーマル
	// 1: 手形
	int m_prevMouseCursorType;
	void ChangeMouseCursor(int type, Bool bIsForced=False);

	void ClearCursor();

	// ------------------------------------------------------------------
	// あげ石指定時の描画
	void DrawStonezCalculate(stone_t** stone);

	// ------------------------------------------------------------------
	// あげ石を取り出したときの描画
	void DrawStonezCalculateEnd(stone_t** stone_area);



	// ------------------
	void* m_soundMove;

	// ------------------
	int m_prev_cursor_x;

	// ------------------
	int m_prev_cursor_y;

	// ------------------
	Bitmap* m_bmpCursorBg;

	// ------------------
	Bitmap* m_bmpCurrentLabel;

	// ------------------
	Bitmap* m_bmpCurrentLabelB;

	// ------------------
	Bitmap* m_bmpCurrentLabelW;

	// ------------------
	wchar_t m_CurrentLabel;

	eLabelType m_eCurrentLabelType;
	
	float GetRealCoordinate(int i);

	void AdjustScale();

	Bool GetNearCoordinate(int x, int y, POINT* ps, int nLineCnt=19);

	// ------------------------------------------------------
	void GetControlPos(int&x, int&y);

	//// ------------------------------------------------------
	//cglsGobanControl* GetGobanControl(eInputMode mode);

	// ------------------------------------------------------
	void xShowEditBox(int x, int y);

	// ------------------------------------------------------
	void ChangeCurrentGoban(goid_t id, Bool bIsNew=True);

#ifndef NEW_STYLE
	// ------------------------------------------------------
	void xResetGobanControl(cyiGoEngine* eng);
#endif

#ifdef NEW_STYLE
#define xResetGobanControl(a) UpdateGobanControl(a)
	void UpdateGobanControl(cyiGoEngine* eng);
	void xResizeGobanControl();
#endif

	// ------------------------------------------------------
	cyiGoEngine* GetGoEngineById(unsigned int id);

	// ------------------------------------------------------
	void Update(eGobanLayer layer = eGL_ALL, stone_t** stone=0, Bool bIsShowNo=False, int nFrom=0);

	// -------------------------------------------------------
	void DrawObject(Graphics* g, float x, float y, Bitmap* bmp);

	// -------------------------------------------------------
	// TODO:
	// Bool CreateTabControl(eTabControlType type, int nFrom);

	// -------------------------------------------------------
	CRITICAL_SECTION m_drawMutex;

	// -------------------------------------------------------
	void DrawInitMutex();

	// -------------------------------------------------------
	void DrawMutexLock();

	// -------------------------------------------------------
	void DrawMutexUnlock();

	// -------------------------------------------------------
	goid_t GenerateGobanId();

	// タブ関係

protected:

	// ---------------------------------------
	LRESULT SelectStone(cyiGoEngine* eng, int x, int y);

	// ---------------------------------------
	virtual Bool OnPaint(HDC hdc, PAINTSTRUCT* ps=0);

	// ---------------------------------------------------------------
	virtual LRESULT OnCreate(WPARAM wp, LPARAM lp);

	virtual void OnThreadStart(int id, int p1, int p2);

	// ---------------------------------------
	//LRESULT OnMouseLeave(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------
	LRESULT OnKeyDown(WPARAM wp, LPARAM lp, Bool& handeld);

	// ---------------------------------------
	LRESULT OnKeySysDown(WPARAM wp, LPARAM lp, Bool& handeld);

	// ---------------------------------------
	LRESULT OnKeySysUp(WPARAM wp, LPARAM lp, Bool& handeld);

	// ---------------------------------------
	LRESULT OnKeyUp(WPARAM wp, LPARAM lp, Bool& handeld);

	// ---------------------------------------
	LRESULT OnMouseMove(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------
	LRESULT OnMouseWheel(WPARAM wp, LPARAM lp, BOOL& handled);

	// ---------------------------------------
	LRESULT OnLButtonDown(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------
	LRESULT OnLButtonDblClick(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------
	LRESULT OnRButtonDblClick(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------
	LRESULT OnLButtonUp(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------
	LRESULT OnRButtonDown(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------
	LRESULT OnTimer(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------------------------------
	LRESULT OnDropFiles(WPARAM wp, LPARAM lp, Bool& bIsHandled);

	// ---------------------------------------------------------------
	LRESULT OnNotifyFromTab(int id, int subId, int code);

	// ---------------------------------------------------------------
	LRESULT OnHeaderChanged(WPARAM wp, LPARAM lp, Bool& bIsHandled);


	// ---------------------------------------
	virtual LRESULT OnMouseLeave(WPARAM wp, LPARAM lp);

	// ---------------------------------------
	virtual LRESULT OnMouseEnter(WPARAM wp, LPARAM lp);



	


DEFINE_MESSAGES_BEGIN
	MESSAGE(OnMouseMove, WM_MOUSEMOVE)
	//MESSAGE(OnMouseLeave, WM_MOUSELEAVE)
	MESSAGE(OnLButtonDown, WM_LBUTTONDOWN)
	MESSAGE(OnLButtonUp, WM_LBUTTONUP)
	MESSAGE(OnRButtonDown, WM_RBUTTONDOWN)
	MESSAGE(OnDropFiles, WM_DROPFILES)
	MESSAGE(OnTimer, WM_TIMER)
	MESSAGE(OnKeyDown, WM_KEYDOWN)
	MESSAGE(OnKeySysDown, WM_SYSKEYDOWN)
	MESSAGE(OnKeySysUp, WM_SYSKEYUP)
	MESSAGE(OnKeyUp, WM_KEYUP)
	MESSAGE(OnLButtonDblClick, WM_LBUTTONDBLCLK)
	MESSAGE(OnRButtonDblClick, WM_RBUTTONDBLCLK)
	MESSAGE(OnMouseWheel, WM_MOUSEWHEEL)
	MESSAGE(OnSearchResultEnd, WM_QUIT_SEARCHRESULT)
	MESSAGE(OnHeaderChanged, WM_GO_HEADER_CHANGED)
DEFINE_MESSAGES_END

DEFINE_NOTIFY_BEGIN
	NOTIFY(OnNotifyFromTab, xCHILD_ID_TAB)
DEFINE_NOTIFY_END

DEFINE_COMMANDS_BEGIN
#ifndef NEW_STYLE
	COMMAND(OnClickTop, eGCS_TOP)
	COMMAND(OnClickPP, eGCS_PP)
	COMMAND(OnClickPrev, eGCS_PREV)
	COMMAND(OnClickNext, eGCS_NEXT)
	COMMAND(OnClickNN, eGCS_NN)
	COMMAND(OnClickEnd, eGCS_END)
	COMMANDEX(OnChangeView, xCHILD_ID_BTN_MYPAGE)
	COMMANDEX(OnChangeView, xCHILD_ID_BTN_MYFRIEND)
	COMMANDEX(OnChangeView, xCHILD_ID_BTN_MYGROUP)
	COMMANDEX(OnChangeView, xCHILD_ID_BTN_GROUP_DETAIL)
	COMMANDEX(OnChangeView, xCHILD_ID_BTN_DETAIL)
#endif
	COMMAND(OnShowMenu, CHILD_ID_GOBAN_MENU)
	COMMAND(OnTitleChanged, xCHILD_ID_TITLE_DIALOG)

	COMMAND(OnResultImageClicked, xCHILD_ID_RESULT)
DEFINE_COMMANDS_END

	// インスタンス
	HINSTANCE m_hInstance;

	// おやwindow
	/*HWND	m_hOwner;*/

	// バックグラウンド描画スレッド
	HANDLE m_hBgThread;

	// CPUの数（スレッドのタイミングを計るため)
	unsigned int m_nCpuCount;

	// 
	int m_nDrawedIndex;

	// 碁盤のエンジン
	cyiGoEngine* m_GoEnginez[MAX_GOBAN_COUNT];

	//sGobanExtraDetail* m_goban_detail[MAX_GOBAN_COUNT];

	// 現在の碁盤のID (1～MAX_GOBAN_COUNT-1)
	goid_t m_CurentGobanId;

	// 現在開かれている碁盤の数
	unsigned int m_nGobanCount;

	// 碁盤のサイズのスケール
	float m_scale;

	// 選択されている碁盤のID
	unsigned int m_nCurrentGoId;

	// デフォルトライン数
	unsigned int m_nDefaultLineCount;

	// 碁石のサイズ
	unsigned int m_nBlockSize;

	cyiList* m_listNodeChangeNotifyWnds;

	// ブランチ表示モード
	//eBranchViewMode m_eBranchViewMode;

	//
	aStoneList m_aCurrentLabelList;

	//float m_scaled_point[LINE_DEFAULT];

	POINTF m_scaled_point[LINE_DEFAULT];

	int m_stone_cnt;
	int m_EnjoiModeCnt;
	BOOL m_bIsWhiteRButtonMode;
	BOOL m_bShowNo;
	bool bIsImageLoaded;

	int m_prevX;
	int m_prevY;

	int m_nJumpCount;

	cyiList m_datas;
	cyiQueue m_remove_list;

	int m_nNoIndex[LINE_DEFAULT][LINE_DEFAULT];


	sGobanDetail m_detail;

	cyiTreeNode* m_node_for_timer;


	//カーソル
	cglsImageBox* m_cursor;

	// 座標ポインター
	cglsImageBox* m_imgPosX;

	// 座標ポインター
	cglsImageBox* m_imgPosY;

	cglsPreviewGoban* m_preview;

	cglsEditBox* m_editBox;

#ifndef NEW_STYLE
	// プレビューハンドら―で使われる。
	cglsStatic* m_infoLabel;

	cglsTitlePannel* m_titleDlg;
	cglsInformationDlg* m_infoDlg;
	cglsStoneInfoDlg*	m_stoneInfoDlg;


	cglsGobanControl* m_currentCtrl;
	cglsGobanControl* m_ctrlForPlay;
	cglsGobanControl* m_ctrlForEdit;
	cglsGobanControl* m_ctrlForEnjoy;
	cglsGobanControl* m_ctrlForCalculate;

	cglsPlayerPannel* m_playerBlack;
	cglsPlayerPannel* m_playerWhite;

	cglsTextPannel* m_textPannel;

	// ---------------------
	cglsMyView* m_myView;

	// ---------------------
	cglsMyFriendView* m_myFriendView;

	CUserManager* m_userMgr;

	// ---------------------
	cglsDialog* m_currentView;
#endif

#ifdef NEW_STYLE
	Bitmap* m_bluredBg;
	HANDLE	m_hBlureThread;
	Bool	m_blureStop;

	void CreateBluredBitmap();
	void xCreateBluredBitmap();

	friend unsigned int __stdcall xOnCreateBluredBitmap(void*);

	// ---------------------
	cglsTabCtrl* m_tab;

	// ---------------------
	cglsToolContainer* m_container;

	// ---------------------
	cglsControlBar* m_controlz[eGCT_COUNT];

	// ---------------------
	void xCreateCommonGobanControl();

	// ---------------------
	cyiList* m_listUserControl;

	// ---------------------
	void xGetGobanSize(int&w, int&h);


#endif

	// ---------------------
	cglsListMenu* m_menuStudy;

	int m_nSelectedView;

	POINT m_ptEditBox;
	// ---------------------
	//cglsMyFriendView* m_myFriendView;

	friend class CGoEditorApp;
};
