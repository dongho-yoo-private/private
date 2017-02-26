#pragma once

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
#include "yctrlbase.h"
#include "yiResource.h"
//#include "cydlgx.h"

#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")

#define WM_PUTSTONE (WM_USER+256)
#define WM_GOBAN_CHANGED (WM_USER+257)

// 最大作成可能な碁盤の数
#define MAX_GOBAN_COUNT 128

//#ifdef _DEBUG
//#pragma comment(lib, "C:\\MyProject\\Libs\\yiLibsd.lib")
//#else
//#pragma comment(lib, "C:\\MyProject\\Libs\\yiLibs.lib")

//#endif




#define WM_GOBAN_ADDED			(WM_USER + 277)
#define WM_CHANGE_DEAD_STONE	(WM_GOBAN_ADDED+1)
#define WM_NODE_CHANGED			(WM_CHANGE_DEAD_STONE+1)
#define WM_INITSTONE			(WM_NODE_CHANGED+1)
#define WM_LOCALE_CHANGED		(WM_INITSTONE+1)
#define WM_PUT_STONED			(WM_LOCALE_CHANGED+1)
#define WM_QUIT_OPTIMIZE		(WM_PUT_STONED+1)
#define WM_QUIT_SEARCHRESULT	(WM_QUIT_OPTIMIZE+1)

enum eGobanLayer {
	eGL_STONE = 0x01,
	eGL_LABEL = 0x02, // ラベル、数字、地合計算など（描画時動的に生成）
	eGL_CURSOR = 0x04,//
	eGL_CALCULATE = 0x10,
	eGL_CALCULATE_END = 0x20,
	eGL_ALL = eGL_STONE|eGL_LABEL
};

enum eBranchViewMode {
	eBVM_NORMAL=0,
	eBVM_HIDE=1,
	eBVM_SHOW_ONE_NODE
};

#define INDEX_TOP 0x1FFFFFFF
#define INDEX_END 0x0FFFFFFF

// ------------------------------------
class  __declspec(dllexport)  cyctrlGoban : public cyctrlbase
{
public:
	cyctrlGoban(void);
	~cyctrlGoban(void);


// 碁盤生成・削除など

	// ------------------------------------------------------
	BOOL Create(long x, long y, long w, long h, long nDefaultLineCount,  HWND hOwnerWnd);

	// ------------------------------------------------------
	void Destroy();


private:
	// ------------------------------------------------------
	goid_t ReserveId();

	// ------------------------------------------------------
	void FreeId(goid_t id);

public:
	// ------------------------------------------------------
	goid_t AddForPlay(goid_t reservedId,  const char* pszBlackName, const char* pszWhiteName, const char* pszTitle, unsigned int condition, unsigned int flag);

	// ------------------------------------------------------
	goid_t AddForEditByFile(const char* pszFileName, eGoFileType type = eGFT_YGF);

	// ------------------------------------------------------
	goid_t AddForEdit(const char* pszBlackName, const char* pszWhiteName, const char* pszTitle, unsigned int condition, unsigned int flag);

	// ------------------------------------------------------
	goid_t Add(eInputMode input, eMouseMode mouseMode, sGoHeader* header, goid_t reservedId=0);

	// ------------------------------------------------------
	void Remove(goid_t id);

	// ------------------------------------------------------
	unsigned int GetCount();

	// -----------------------------------------
	goid_t GetCurrentGoId();

	// -----------------------------------------
	// ロケール変更により、再描画が行われる。
	void ChangeLocale(goid_t id, eCharacterSet eLocale=eCS_JAPANESE);



// 選択されている碁盤に対する機能
	// ------------------------------------------------------
	// 番号表示	
	void ShowNo(Bool bIsTrue, int nFrom=0);

	// -------------------------------
	// 石を置く
	stone_t** PutStone(goid_t id, stone_t stone);

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
	stone_t** Goto(goid_t id, int nIndex, aStoneList branches);//, eInputMode mode=eIM_ENJOY);
	
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
	void DeleteNode(cyiTreeNode* node);


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

	// ------------------------------------------------------
	Bool Resize(long w, long h);

	// ------------------------------------------------------
	Bool Resize(int x, int y, long w, long h);

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
	int ShowMessage(unsigned int id, const wchar_t* message, int flag, int timeout=0);

	// ------------------------------------
	// とりあえず廃止
	//virtual Bitmap* GetBackGround(int x, int y, int w, int h);

	// ------------------------------------------------------
	BOOL ChangeGoban(goid_t id);

	// -------------------------------------------------------
	// 地合い計算モードに切り替え、あげ石などの指定
	void CalculateMode(goid_t id);	

	// -------------------------------------------------------
	// 地合い計算終了。
	stone_t** CalculateEnd(goid_t id, int* xnBlack, int* xnWhite);


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


	// -------------------------------------------------------
	cyiTreeNode* cyctrlGoban::GetCurrentNode();


	// -------------------------------------------------------
	UINT GetSize();

	// ----------------------------------------
	Bool Save(const char* pszFileName, eGoFileType type=eGFT_SGF, goid_t id=0xFFFFFFFF);

	// commands
	// コマンド

	// 一手再生
	void Play();

	// -------------------------------------------------------
	void SearchResult();

	// ------------------------------------------------------
	void CreateMapIamge(cyiTreeNode* node);

	// ------------------------------------------------------
	void SetBrancheViewMode(eBranchViewMode mode=eBVM_NORMAL);

	// -----------------------------------
	int OptimizeNode(int* nProgress, Bool* canceled);

	// -----------------------------------
	int GetFullNodeCnt();

public:
	//HWND	m_hWnd;

	// -----------------------------------
	Bool IsChanged();

	// -----------------------------------
	void SetChangedFlag(Bool bIsChanged);

////
private:

	// -----------------------------
	void InitMember();

	// ----------------------------------------
	void InitGraphics();

	// ----------------------------------------
	int SetBranch(cyiGoEngine* eng, stone_t* pass=0);

	// ---------------------------------------------------------------
	Bool IsBranches(int x, int y);


	// ----------------------------
	// 碁盤のサイズ
	int m_nGobanWidth;

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
	void DrawCursor(HDC hdc, int i, int j, Bool bIsBlack);

	// ------------------------------------------------------------------
	//
	// 0: デフォルトノーマル
	// 1: 手形
	int m_prevMouseCursorType;
	void ChangeMouseCursor(int type);

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
	void ChangeCurrentGoban(goid_t id);

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







protected:
	// ---------------------------------------
	virtual BOOL OnPaint(HDC hdc, PAINTSTRUCT* ps=0);

	// ---------------------------------------------------------------
	virtual LRESULT OnCreate(WPARAM wp, LPARAM lp);

	virtual void OnThreadStart(int id, int p1, int p2);

	// ---------------------------------------
	LRESULT OnMouseLeave(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------
	LRESULT OnMouseMove(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------
	LRESULT OnLButtonDown(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------
	LRESULT OnLButtonUp(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------
	LRESULT OnRButtonDown(WPARAM wp, LPARAM lp, Bool& handled);

	// ---------------------------------------------------------------
	LRESULT OnDropFiles(WPARAM wp, LPARAM lp, Bool& bIsHandled);



DEFINE_MESSAGES_BEGIN
	MESSAGE(OnMouseMove, WM_MOUSEMOVE)
	MESSAGE(OnMouseLeave, WM_MOUSELEAVE)
	MESSAGE(OnLButtonDown, WM_LBUTTONDOWN)
	MESSAGE(OnLButtonUp, WM_LBUTTONUP)
	MESSAGE(OnRButtonDown, WM_RBUTTONDOWN)
	MESSAGE(OnDropFiles, WM_DROPFILES)
DEFINE_MESSAGES_END


	// インスタンス
	HINSTANCE m_hInstance;

	// おやwindow
	HWND	m_hOwner;

	// バックグラウンド描画スレッド
	HANDLE m_hBgThread;

	// CPUの数（スレッドのタイミングを計るため)
	unsigned int m_nCpuCount;

	// 
	int m_nDrawedIndex;

	// 碁盤のエンジン
	cyiGoEngine* m_GoEnginez[MAX_GOBAN_COUNT];

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

	// ブランチ表示モード
	eBranchViewMode m_eBranchViewMode;

	//
	aStoneList m_aCurrentLabelList;


	float m_scaled_point[LINE_DEFAULT];

	int m_stone_cnt;
	int m_EnjoiModeCnt;
	BOOL m_bIsWhiteRButtonMode;
	BOOL m_bShowNo;
	bool bIsImageLoaded;

	int m_prevX;
	int m_prevY;

	cyiList m_datas;
	cyiQueue m_remove_list;

	int m_nNoIndex[LINE_DEFAULT][LINE_DEFAULT];

	//stone_t m_laststone;
	
	//cyctrlx m_TabCtrl;



};
