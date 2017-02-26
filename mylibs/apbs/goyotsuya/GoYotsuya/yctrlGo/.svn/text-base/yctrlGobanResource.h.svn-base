#pragma once

#include <atlbase.h>
#include <gdiplus.h>

#ifndef __GDI_PLUS__
#define __GDI_PLUS__
using namespace Gdiplus;
#pragma comment(lib, "GdiPlus.lib") 
#endif

#include "yi.h"

//#define ORG_STONE_SIZE 54
//#define ORG_START_INDEX 39
//#define ORG_GOBAN_SIZE 1050


// ブロックサイズ
#define MAX_BLOCK_SIZE 54

// 石のサイズ
#define MAX_STONE_SIZE 54

// 碁盤の余白
#define MAX_START_INDEX 32



// 碁盤の全体サイズ
#ifndef NEW_STYLE
#define MAX_GOBAN_SIZE ((MAX_BLOCK_SIZE*18)+(MAX_START_INDEX<<1))
#define ORG_GOBAN_SIZE MAX_GOBAN_SIZE
#else
#define MAX_CONTROL_SIZE 54
#define MAX_GOBAN_WIDTH ((MAX_BLOCK_SIZE*18)+(MAX_START_INDEX<<1))
#define MAX_GOBAN_HEIGHT (MAX_GOBAN_WIDTH+(MAX_CONTROL_SIZE<<1))
#endif

#define ORG_STONE_SIZE MAX_BLOCK_SIZE
#define ORG_START_INDEX MAX_START_INDEX


enum eGoResourceId {
	eRIDB_GOBAN=1,
	eRIDB_PURE_GOBAN,
	eRIDB_BSTONE,
	eRIDB_WSTONE,
	eRIDB_BSTONE_T,
	eRIDB_WSTONE_T,
	eRIDB_BTRI,
	eRIDB_WTRI,
	eRIDB_STONE_SHADOW,
	eRIDB_GOBAN_PURE,
	eRIDF_SYSTEM,
	eRIDF_TAB_CONTROL,
	eRIDF_BUTTON,
	eRIDF_BUTTON_SMALL,
	eRIDF_BUTTON_LARGE,
	eRIDF_LABEL,
	eRIDF_LABEL_SMALL,
	eRIDF_BRANCH,
	eRIDB_GOBAN13,
	eRIDB_GOBAN9,
	eRIDB_RED_CROSS,
	eRIDB_RED_CROSS_CURSOR,
	eRIDB_ADD_CURSOR,
	eRIDB_AREA_B,
	eRIDB_AREA_W,
	eRIDF_NO
};

// ---------------------------
enum eGoCtrlResourceId {
	// General Button
	eGCRID_PLAY,
	eGCRID_BACK,
	eGCRID_NEXT,
	eGCRID_PREV,
	eGCRID_TOP,
	eGCRID_END,
	eGCRID_SHOW_NO,
	eGCRID_SHOW_NO_FROM,
	
	// extended Buttons
	eGCRID_ADD_STONE,
	eGCRID_CMB_LABEL,
	eGCRID_CMB_INIT_STONE,
	eGCRID_NEXT_BRANCH,
	eGCRID_PREV_BRANCH,
	eGCRID_SEARCH_COMMENTS,

	eGCRID_TEXT_BOX,
	eGCRID_COMMENT_BOX,
	eGCRID_BLACK_INFO_BOX,
	eGCRID_WHITE_INFO_BOX,
	eGCRID_GAME_INFO_BOX,
};

// ---------------------------
enum eGoTextResourceId {
	eGTRID_JOSEKIDICTIONARY=0,
	eGTRID_SAVE_CONFIRM=1, //"%sを保存しますか？", L"%sを保存しますか？"},
	eGTRID_DELETE_CONFIRM=2, //"%sを削除しますか？", L"%sを削除しますか？"},
	eGTRID_CLOSE_CONFIRM=3, //"%sを閉じますか？", L"%sを閉じますか？"},
	eGTRID_WARNING=4, //"警告", L"警告"},
	eGTRID_INFORMATION=5, //"お知らせ", L"お知らせ"},
	eGTRID_REFERENCE=6,
	eGTRID_NOTITLED=7,
	eGTRID_MAIN=8,
	eGTRID_TITLE=9, //"タイトル"
	eGTRID_DATE=10, // 対局日時
	eGTRID_CONDITION=11, // 対局条件（手合い・コミ）
	eGTRID_LIMIT_TIME=12, // 持ち時間
	eGTRID_RESULT=13, // 結果
	eGTRID_GAMENAME=14, // 大会名
	eGTRID_PLACE=15, // 場所
	eGTRID_TEAI=16,
	eGTRID_KOMI=17,
	eGTRID_PLAYRESULT=18,
	eGTRID_MOKU=19, //目
	eGTRID_HALF=20, //半
	eGTRID_AGEHAMA=21, // アゲハま
	eGTRID_REMAIN_TIME=22, // 残り時間
	eGTRID_SEISEKI=23, // 成績
	eGTRID_TAISENN_SEISEKI=24, // 対戦成績
	eGTRID_WIN=25, // 勝
	eGTRID_MATCH=26, // 戦
	eGTRID_LOSE=27, // 負
	eGTRID_DRAW=28, // 引き分け
	eGTRID_BLACK=29, // 黒
	eGTRID_WHITE=30, // 白

	eGTRID_EOF
};

// ---------------------------
enum eGoCtrlButtonResourceId {
	eGCBRID_PLAY 
};

enum eGoCtrlLocale {
	eGCL_JAPANESE=0,
	eGCL_KOREAN=1,
	eGCL_CHINESE=2,
	eGCL_ENGLISH=3
};

class  xEXPORT cyctrlGobanResource
{
public:
	cyctrlGobanResource(void);
	~cyctrlGobanResource(void);

	static cyctrlGobanResource& GetInstance();

	// ---------------------------------------
	void Initialize(float scale=1.0, HINSTANCE hInstance=0, int w=0, int h=0, int yOffset=0);

	// ---------------------------------------
	void SetBitmapScale(float scale, int w, int h, POINTF* corr=0, int yOffset=0);

	// ---------------------------------------
	Bitmap* GetImage(int id);

	// ---------------------------------------
	Font* GetFont(int id);

	// ---------------------------------------
	Bitmap* GetBtnResourceId(int id, int state);

	// ---------------------------------------
	Bitmap* MakeLabel(wchar_t ch);

	// ---------------------------------------
	void GetScaledSize(float& nGobanWidth, float& nLineStartOffset, float& nBlackWidth);

	// ---------------------------------------
	Bitmap* GetGobanBitmap(int w, int nLineCount, float* coor, int* nBlockSize);

	// ---------------------------------------
	Bitmap* GetGobanBitmap(int w, int h, float yOffset, int nLineCount, POINTF* coor, int* nBlockSize, Bitmap** bmpPureGoban=0);

	// ---------------------------------------
	Bitmap* GetLabelImage(unsigned int type, color_t color);

	// ---------------------------------------
	const wchar_t* GetStringW(int id, eGoCtrlLocale eLocale=eGCL_JAPANESE);

	// ---------------------------------------
	const char* GetStringA(int id, eGoCtrlLocale eLocale=eGCL_JAPANESE);

	// ---------------------------------------
	// 6bit: pro or ama
	// level 5bit
	// 9段 -> 1
	// 8段 -> 2
	//		:
	//		:
	// 初段 -> 9
	// 初級 -> 10
	//		:
	// 18級 -> 28
	// レッスンプロ：29 (院生など)
	// セミプロ：30 (院生など)
	int GetLevelString(int nLevel, wchar_t* pszBuffer, eGoCtrlLocale eLocale=eGCL_JAPANESE);

private:
	// ---------------------------------------
	Font* CreateFont(wchar_t* fontname, float fMin, float fMax, float scale);

	static cyctrlGobanResource* m_me;

	// 碁盤(最大サイズ)
	Bitmap* m_bmpGoban;

	// 
	Bitmap* m_bmpPureGoban;

	// 線をあわせた碁盤
	Bitmap* m_BackGround;
	Bitmap* m_BackGround9;
	Bitmap* m_BackGround13;


	// 線イメージ
	Bitmap* m_Line; //= xMakeLine(nLineCnt);

	// 碁石(通常・透明)
	Bitmap* m_BlackStone;
	Bitmap* m_BlackStone_T;
	Bitmap* m_WhiteStone;
	Bitmap* m_WhiteStone_T;

	// 碁石の影
	Bitmap* m_StoneShadow;

	// ポインター
	Bitmap* m_TriAngleB;
	Bitmap* m_TryAngleW;

	// 削除カーソル
	Bitmap* m_bmpRedCrossCursor;

	// 削除ラベル
	Bitmap* m_bmpRedCross;

	// あげ石
	Bitmap* m_StoneCross;

	// 石追加時のカーソル
	Bitmap* m_bmpAddCursor;

	float m_scale;



	// システムフォント(一般的に使われるフォント、スケール1.0時 12.0、スケール最小：9.0)
	Font* m_SystemFont;

	// ボタンなどに使われるフォント(一般的に使われるフォント、スケール1.0時 11.0、スケール最小：8.0)
	Font* m_ButtonFont;

	// ボタンなどに使われるフォント(一般的に使われるフォント、スケール1.0時 11.0、スケール最小：8.0)
	Font* m_ButtonFontSmall;

	Font* m_ButtonFontLarge;

	// タブなどに使われるフォント(横向きフォント、スケール1.0時 11.0、スケール最小：9.0)
	Font* m_TabFont;

	// ラベル用フォント
	Font* m_LabelFont;

	// ラベル用フォント
	Font* m_NoFont;

	// ラベル用小さなフォント
	Font* m_LabelFontSmall;

	// ラベル用ブランチ用フォント
	Font* m_fntBranch;



public:
	float m_fGobanSize;
	float m_fLineStartOffset;//m_nLineStartOffset;
	float m_fBlockWidth;//m_nBlockWidth;

	// --------------------------
	eGoCtrlLocale m_eSystemLocale;

};
