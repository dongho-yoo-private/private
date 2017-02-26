#include "yctrlGobanResource.h"
#include "resource.h"
#include "yistd.h"

typedef struct xsTextResource {
	int id;
	const char* a;
	const wchar_t* w;
} sTextResource;

// ---------------------------------
sTextResource g_TextResource[] = {
	{eGTRID_JOSEKIDICTIONARY, "定石辞典", L"定石辞典"},
	{eGTRID_SAVE_CONFIRM, "%sを保存しますか？", L"%sを保存しますか？"},
	{eGTRID_DELETE_CONFIRM, "%sを削除しますか？", L"%sを削除しますか？"},
	{eGTRID_CLOSE_CONFIRM, "%sを閉じますか？", L"%sを閉じますか？"},
	{eGTRID_WARNING, "警告", L"警告"},
	{eGTRID_INFORMATION, "お知らせ", L"お知らせ"},
	{eGTRID_REFERENCE, "参考図", L"参考図"},
	{eGTRID_NOTITLED, "名前なし", L"名前なし"},
	{eGTRID_MAIN, "本図", L"本図"},
	{eGTRID_TITLE, "タイトル", L"タイトル"},
	{eGTRID_DATE, "対局日時", L"対局日時"},
	{eGTRID_CONDITION, "対局条件", L"対局条件"},
	{eGTRID_LIMIT_TIME, "持ち時間", L"持ち時間"},
	{eGTRID_RESULT, "結果", L"結果"},
	{eGTRID_GAMENAME, "試合名", L"試合名"},
	{eGTRID_PLACE, "対局場所", L"対局場所"},
	{eGTRID_TEAI, "手合い", L"手合い"},
	{eGTRID_KOMI, "コミ", L"コミ"},
	{eGTRID_PLAYRESULT, "対局結果", L"対局結果"},
	{eGTRID_MOKU, "目",L"目"},//目
	{eGTRID_HALF,"半", L"半"}, //半
	{eGTRID_AGEHAMA,"アゲハマ", L"アゲハマ"}, //アゲハマ
	{eGTRID_REMAIN_TIME,"残り時間", L"残り時間"}, //残り時間
	{eGTRID_SEISEKI,"戦績", L"戦績"}, //対戦成績
	{eGTRID_TAISENN_SEISEKI,"対戦戦績", L"対戦戦績"}, //対戦成績
	{eGTRID_WIN,"勝", L"勝"}, //半
	{eGTRID_MATCH,"戦", L"戦"}, //半
	{eGTRID_LOSE,"負", L"負"}, //半
	{eGTRID_DRAW,"引き分け", L"引き分け"}, //半
	{eGTRID_BLACK,"黒", L"黒"}, //半
	{eGTRID_WHITE,"白", L"白"}, //半
	{0, 0, 0}
};

// ---------------------------------
static Bitmap* xLoadGoban(HINSTANCE hInstance=0);

// ---------------------------------
static Bitmap* xMakeBlackStone(Bool IsNormal, float scale);

// ---------------------------------
static Bitmap* xMakeWhiteStone(Bool IsNormal, float scale);


// ---------------------------------
static Bitmap* xMakeBackground(Bitmap* goban, int nLineCnt, float scale, int w, int h, Bitmap** pureGoban);

// ---------------------------------
static Bitmap* xMakeTriAngle(Bitmap* bmp, Bool IsBlack, float scale, color_t=0);

// ---------------------------------
static Bitmap* xMakeStoneShadow(float scale);

// ----------------------------------
static Bitmap* xMakeAreaBmp(Bool bIsBlack, float scale);

// ----------------------------------
static Bitmap* xMakeCross(float scale, Bool bIsCursor, color_t color);

// ----------------------------------
static Bitmap* xMakeAddCursor(float scale);

// ----------------------------------
static Bitmap* xMakeDoubleCircle(float scale, color_t color);

// ----------------------------------
static Bitmap* xMakeCircleInChar(float scale, wchar_t* ch, Font* font, color_t color, color_t color_bg);

// ----------------------------------
static Bitmap* xMakeCircle(float scale, color_t color);

// ----------------------------------
static Bitmap* xMakeRectangle(float scale, color_t color);


// ---------------------------------
static Bitmap* xMakeTriAngleEx(float scale, color_t color);


// ---------------------------------
cyctrlGobanResource::cyctrlGobanResource(void)
{
	// 碁盤
	m_bmpGoban=0;
	m_bmpPureGoban=0;

	// 線イメージ
	m_Line=0;

	// 碁石(通常・透明)
	m_BlackStone=0;
	m_BlackStone_T=0;
	m_WhiteStone=0;
	m_WhiteStone_T=0;
	
	// ポインター
	m_TriAngleB=0;
	m_TryAngleW=0;

	// 碁石の影
	m_StoneShadow=0;

	// 石追加時のカーソル
	m_bmpAddCursor=0;

	// ----------------------
	m_bmpRedCrossCursor=0;

	// ----------------------
	m_bmpRedCross=0;


	// システムフォント(一般的に使われるフォント、スケール1.0時 12.0、スケール最小：9.0)
	m_SystemFont=0;

	// ボタンなどに使われるフォント(一般的に使われるフォント、スケール1.0時 11.0、スケール最小：8.0)
	m_ButtonFont=0;

	// ボタンなどに使われるフォント(一般的に使われるフォント、スケール1.0時 11.0、スケール最小：8.0)
	m_ButtonFontSmall=0;

	m_ButtonFontLarge=0;

	// タブなどに使われるフォント(横向きフォント、スケール1.0時 11.0、スケール最小：9.0)
	m_TabFont=0;

	// ラベル用フォント
	m_LabelFont=0;

	// ラベル用フォント
	m_NoFont=0;

	// ラベル用小さなフォント
	m_LabelFontSmall=0;

	// ラベル用ブランチ用フォント
	m_fntBranch=0;

	m_BackGround=0;
	m_scale=1.0;

}

// ---------------------------------
cyctrlGobanResource::~cyctrlGobanResource(void)
{
}

// -----------------------------------
cyctrlGobanResource* cyctrlGobanResource::m_me=0;


// -----------------------------------
cyctrlGobanResource& cyctrlGobanResource::GetInstance()

{
	if (m_me==0)
	{
		m_me = new cyctrlGobanResource();
		//m_me->Initialize();
	}

	return *m_me;
}

// ---------------------------------------
void cyctrlGobanResource::Initialize(float scale/*=1.0*/, HINSTANCE hInstance, int w, int h, int yOffset)
{
	/*
	// 碁盤
	m_bmpGoban = xLoadGoban(hInstance);

	// 線イメージ
	if (w==0)
	{
		m_BackGround = xMakeBackground(m_bmpGoban, 19, scale, 0, 0, &m_bmpPureGoban);
	}
	else
	{
		m_BackGround = GetGobanBitmap(w, h, yOffset, 19, coor, 0, &m_bmpPureGoban);
	}

	// TODO: 9,13路
	m_BackGround9=0;
	m_BackGround13=0;

	// 碁石(通常・透明)
	m_BlackStone = xMakeBlackStone(True, scale);
	m_BlackStone_T = xMakeBlackStone(False, scale);
	m_WhiteStone = xMakeWhiteStone(True, scale);
	m_WhiteStone_T = xMakeWhiteStone(False, scale);

	// 碁石の影
	m_StoneShadow = xMakeStoneShadow(scale);

	// ポインター
	m_TriAngleB = xMakeTriAngle(m_BlackStone, True, scale);
	m_TryAngleW = xMakeTriAngle(m_BlackStone, False, scale);

	// 削除カーソル
	m_bmpRedCrossCursor = xMakeCross(scale, True, Color::Red);

	// 削除ラベル？
	m_bmpRedCross = xMakeCross(scale, False, Color::Red);

	// 石追加時のカーソル
	m_bmpAddCursor = xMakeAddCursor(scale);

	// Font生成 
	m_SystemFont = this->CreateFont(L"Meiryo", 9.0, 12.0, scale);
	m_ButtonFont = this->CreateFont(L"Meiryo", 5.0, 14.0, scale);
	m_ButtonFontLarge = this->CreateFont(L"Meiryo", 8.0, 15.0, scale);
	m_ButtonFontSmall = this->CreateFont(L"Meiryo", 4.0, 13.0, scale);
	m_TabFont = this->CreateFont(L"Meiryo", 8.0, 11.0, scale);
	m_LabelFont = new Font(L"Meiryo UI Bold", 20.0*scale);
	m_NoFont = new Font(L"Meiryo  UI Bold", 17.0*scale);

	m_LabelFontSmall  = new Font(L"Meiryo UI Bold", 17.0*scale, FontStyleBold);

	m_fntBranch = new Font(L"Meiryo UI Bold", 16.0*scale, FontStyleBold);

	// TODO
	m_StoneCross=0;

	m_scale=scale;
	*/
}

// ---------------------------------------
Font* cyctrlGobanResource::CreateFont(wchar_t* fontname, float fMin, float fMax, float scale)
{
	return new Font(fontname, fMin+(fMax-fMin)*scale);
}


#define __STONE_ADJUST_SCALE__ (float)(1.00)
// ---------------------------------------
void cyctrlGobanResource::SetBitmapScale(float scale, int w, int h, POINTF* coor, int yOffset)
{

	if (m_scale==scale)
		return ;

	// 碁盤
	if (m_BackGround)
		delete m_BackGround;

	if (m_bmpPureGoban)
		delete m_bmpPureGoban;

	//m_BackGround = xMakeBackground(m_bmpGoban, 19, scale, w, h, &m_bmpPureGoban);
	m_BackGround = GetGobanBitmap(w, h, yOffset, 19, coor, 0, &m_bmpPureGoban);

	// TODO: 13, 9
	m_BackGround13=0;
	m_BackGround9=0;

	// 碁石(通常・透明)
	if (m_BlackStone)
		delete m_BlackStone;
	if (m_BlackStone_T)
		delete m_BlackStone_T;
	if (m_WhiteStone)
		delete m_WhiteStone;
	if (m_WhiteStone_T)
		delete m_WhiteStone_T;

	m_BlackStone = xMakeBlackStone(True, scale*__STONE_ADJUST_SCALE__);
	m_BlackStone_T = xMakeBlackStone(False, scale*__STONE_ADJUST_SCALE__);
	m_WhiteStone = xMakeWhiteStone(True, scale*__STONE_ADJUST_SCALE__);
	m_WhiteStone_T = xMakeWhiteStone(False, scale*__STONE_ADJUST_SCALE__);

	// ポインター
	if (m_TriAngleB)
		delete m_TriAngleB;
	if (m_TryAngleW)
		delete m_TryAngleW;

	m_TriAngleB = xMakeTriAngle(m_BlackStone, True, scale);
	m_TryAngleW = xMakeTriAngle(m_BlackStone, False, scale);

	if (m_bmpRedCrossCursor)
		delete m_bmpRedCrossCursor;
	// 削除カーソル
	m_bmpRedCrossCursor = xMakeCross(scale, True, Color::Red);

	if (m_bmpRedCross)
		delete m_bmpRedCross;
	// 削除ラベル？
	m_bmpRedCross = xMakeCross(scale, False, Color::Red);

	// 石追加時のカーソル
	if (m_bmpAddCursor)
		m_bmpAddCursor = xMakeAddCursor(scale);

	m_StoneShadow = xMakeStoneShadow(scale);


	if (m_SystemFont)
		delete m_SystemFont;

	if (m_ButtonFont)
		delete m_ButtonFont;

	if (m_ButtonFontSmall)
		delete m_ButtonFontSmall;

	if (m_ButtonFontLarge)
		delete m_ButtonFontLarge;

	if (m_TabFont)
		delete m_TabFont;

	if (m_LabelFont)
		delete m_LabelFont;

	if (m_NoFont)
		delete m_NoFont;

	if (m_LabelFontSmall)
		delete m_LabelFontSmall;

	if (m_fntBranch)
		delete m_fntBranch;

	// Font生成 
	m_SystemFont = this->CreateFont(L"Meiryo", 9.0, 12.0, scale);
	m_ButtonFont = this->CreateFont(L"Meiryo", 5.0, 14.0, scale);
	m_ButtonFontSmall = this->CreateFont(L"Meiryo", 4.0, 13.0, scale);
	m_ButtonFontLarge = this->CreateFont(L"Meiryo", 6.0, 15.0, scale);

	m_TabFont = this->CreateFont(L"Meiryo", 8.0, 11.5, scale);
	m_LabelFont = new Font(L"Meiryo", 18.0*scale, ::FontStyleBold);
	m_NoFont = new Font(L"Meiryo", 17.0*scale);

	m_LabelFontSmall  = new Font(L"Meirio", 17.0*scale, FontStyleBold);

	m_fntBranch = new Font(L"Meiryo", 16.0*scale, FontStyleBold);

	m_scale=scale;
}

// ---------------------------------------
void cyctrlGobanResource::GetScaledSize(float& nGobanWidth, float& nLineStartOffset, float& nBlockWidth)
{
	nGobanWidth = m_fGobanSize;
	nLineStartOffset = m_fLineStartOffset;
	nBlockWidth = m_fBlockWidth;
}



// ---------------------------------------
Bitmap* cyctrlGobanResource::GetImage(int id)
{
	switch(id)
	{
		case eRIDB_PURE_GOBAN:
			return m_bmpPureGoban;
		case eRIDB_GOBAN:
			return m_BackGround;
		case eRIDB_GOBAN9:
			return m_BackGround9;
		case eRIDB_GOBAN13:
			return m_BackGround13;
		case eRIDB_BSTONE:
			return m_BlackStone;
		case eRIDB_WSTONE:
			return m_WhiteStone;
		case eRIDB_BSTONE_T:
			return m_BlackStone_T;
		case eRIDB_WSTONE_T:
			return m_WhiteStone_T;
		case eRIDB_BTRI:
			return m_TriAngleB;
		case eRIDB_WTRI:
			return m_TryAngleW;
		case eRIDB_STONE_SHADOW:
			return m_StoneShadow;
		case eRIDB_RED_CROSS_CURSOR:
			return m_bmpRedCrossCursor;
		case eRIDB_RED_CROSS:
			return m_bmpRedCross;
		case eRIDB_AREA_B:
			return xMakeAreaBmp(True, m_scale);
		case eRIDB_AREA_W:
			return xMakeAreaBmp(False, m_scale);
		case eRIDB_ADD_CURSOR:
			return 	m_bmpAddCursor;


		default:
			break;
	}
	return 0;
}


// ---------------------------------------
// 	eLS_SQURE=0xFF01,
// 	eLS_CIRCLE=0xFF02,
// 	eLS_TRIANGLE=0xFF03,
// 	eLS_MA		=0xFF04,

// 	eLS_GOOD, // 好手 青◎
// 	eLS_BAD,  // 悪手 赤×
// 	eLS_NOGOOD, // △　黄色
// 	eLS_KIKI, // □ 青
// 	eLS_NORMAL, // ○ 緑
// 	eLS_TRICK, // !○ 白 はめて
// 	eLS_DEFICULT, // ?○ 難解
// 	eLS_SENTE, // >○ 白
// 	eLS_TESUZI // ☆ 白
//eLS_NORMAL=0xFE11, // 普通の手（無難な手）
//eLS_KYUSYO=0xFE12, // 急所
//eLS_CASE=0xFE13, // 場合の手
//eLS_KATACHI=0xFE14, // 形
//eLS_MYOSYU=0xFE15, // 妙手
//eLS_ABSOLUTE=0xFE16, // 絶対
//eLS_BADSUJI=0xFE17 // 悪筋
//eLS_SIMPLE=0xFE10,

#define SHAPE_WIDTH (1.0+m_scale)
Bitmap*  cyctrlGobanResource::GetLabelImage(unsigned int _type, color_t color)
{
	eLabelType type = (eLabelType)_type;
	float shape_size=ORG_STONE_SIZE*m_scale*0.8;

	switch(type)
	{
		case eLS_SQURE:
		{
			return xMakeRectangle(m_scale, (color_t)color);//Color::Black);
		}
		case eLS_CIRCLE:
		{
			return xMakeCircle(m_scale, (color_t)color);//Color::Black);
		}
		case eLS_TRIANGLE:
		{
			return xMakeTriAngleEx(m_scale, color);
		}
		case eLS_MA:
		{
			return xMakeCross(m_scale, False, (color_t)color);//Color::Black);
		}

		case eLS_GOOD:
		{
			return xMakeCircle(m_scale, (color_t)Color::LightGreen);
		}
		case eLS_BAD:
		{
			return xMakeCross(m_scale, False, (color_t)Color::Red);
		}
		case eLS_NOGOOD:
		{
			return xMakeTriAngleEx(m_scale, (color_t)Color::Yellow);
		}
		case eLS_CAREFULL:
		{
			return xMakeCircleInChar(m_scale, L"※", GetFont(eRIDF_LABEL), Color::Black, Color::Black);
		}
		case eLS_INFO:
		{
			return xMakeCircleInChar(m_scale, L"※", GetFont(eRIDF_LABEL), Color::White, Color::White);
		}
		case eLS_GOOD2:
		{
			return xMakeDoubleCircle(m_scale, (color_t)Color::LightGreen);
		}
		case eLS_CASE:
		{
			return xMakeCircle(m_scale, (color_t)Color::Yellow);
		}
		/*
		case eLS_ABSOLUTE:
		{
			return xMakeDoubleCircle(m_scale, (color_t)Color::Blue);
		}
		case eLS_BADSUJI:
		{
			return xMakeCross(m_scale, False, (color_t)Color::DarkRed);
		}

		case eLS_BLACK_KIKI:
		{
			return xMakeCircleInChar(m_scale, L"×", GetFont(eRIDF_LABEL), (color_t)Color::Black, (color_t)Color::Black);
		}
		case eLS_WHITE_KIKI:
		{
			return xMakeCircleInChar(m_scale, L"×", GetFont(eRIDF_LABEL), (color_t)Color::White, (color_t)Color::White);
		}
		case eLS_NORMAL:
		{
			return xMakeCircle(m_scale, (color_t)Color::LightGreen);
		}
		case eLS_HONTE:
		{
			return xMakeCircle(m_scale, (color_t)Color::Blue);
		}
		case eLS_TRICK:
		{
			return xMakeCircleInChar(m_scale, L"※", GetFont(eRIDF_LABEL), Color::Blue, Color::Blue);
		}
		case eLS_DEFICULT:
		{
			return xMakeCircleInChar(m_scale, L"?", GetFont(eRIDF_LABEL), Color::Black, Color::Black);
		}
		case eLS_SENTE:
		{
			return xMakeCircleInChar(m_scale, L"先", GetFont(eRIDF_LABEL_SMALL), Color::White, Color::White);
		}
		case eLS_TESUZI:
		{
			return xMakeCircleInChar(m_scale, L"★", GetFont(eRIDF_LABEL), Color::LightGreen, Color::LightGreen);
			break;
		}
		case eLS_KYOSYU:
		{
			return xMakeCircleInChar(m_scale, L"★", GetFont(eRIDF_LABEL), Color::Black, Color::Black);
			break;
		}
		case eLS_KYUSYO:
		{
			return xMakeCircleInChar(m_scale, L"★", GetFont(eRIDF_LABEL), Color::Red, Color::Red);
			break;
		}
		case eLS_MYOSYU:
		{
			return xMakeCircleInChar(m_scale, L"★", GetFont(eRIDF_LABEL), Color::Yellow, Color::Yellow);
			break;
		}
		case eLS_CASE:
		{
			return xMakeCircleInChar(m_scale, L"※", GetFont(eRIDF_LABEL), Color::White, Color::White);
			break;
		}
		case eLS_SIMPLE:
		{
			return xMakeCircleInChar(m_scale, L"※", GetFont(eRIDF_LABEL), Color::LightGreen, Color::LightGreen);
			break;
		}
		case eLS_KATACHI:
		{
			return xMakeCircleInChar(m_scale, L"※", GetFont(eRIDF_LABEL), Color::Orange, Color::Orange);
			break;
		}
		case eLS_SICHO:
		{
			return xMakeDoubleCircle(m_scale, (color_t)Color::Green);
			//return xMakeCircleInChar(m_scale, L"※", GetFont(eRIDF_LABEL), Color::Orange, Color::Orange);
			break;
		}
		case eLS_KOU:
		{
			return xMakeDoubleCircle(m_scale, (color_t)Color::CadetBlue);
			//return xMakeCircleInChar(m_scale, L"※", GetFont(eRIDF_LABEL), Color::Orange, Color::Orange);
			break;
		}
		*/
		//case eLS_USOTE:
		//{
		//	return xMakeCircleInChar(m_scale, L"※", GetFont(eRIDF_LABEL), Color::Yellow, Color::Yellow);
		//	break;
		//}
		default:
			return 0;
	}

	return 0;
}

// ---------------------------------------
const wchar_t* cyctrlGobanResource::GetStringW(int id, eGoCtrlLocale eLocale)
{
	if (eLocale==eGCL_JAPANESE)
	{
		return g_TextResource[id].w;
	}

	return 0;
}

// ---------------------------------------
const char* cyctrlGobanResource::GetStringA(int id, eGoCtrlLocale eLocale)
{
	if (eLocale==eGCL_JAPANESE)
	{
		return g_TextResource[id].a;
	}

	return 0;
}

// ---------------------------------------
// 6bit: pro or ama
// level 5bit
// 9段 -> 1
// 8段 -> 2
//		:
//		:
// 初段 -> 9
// １級 -> 10
//		:
// 18級 -> 28
// レッスンプロ：29 (院生など)
// セミプロ：30 (院生など)
int cyctrlGobanResource::GetLevelString(int nLevel, wchar_t* pszBuffer, eGoCtrlLocale eLocale)
{
	Bool bIsPro = (nLevel>>6)==1?True:False;
	nLevel = (nLevel&(~(1<<6)));

	pszBuffer[0]=0;
	if (nLevel==0)
	{
		return 0;
	}

	if (eLocale==eGCL_JAPANESE)
	{
		const wchar_t* jano[] = {L"初", L"二", L"三", L"四", L"五", L"六", L"七", L"八", L"九"};
		bIsPro=True;
		if (nLevel<10)
		{
			wsprintfW(pszBuffer, L"%s%s%s", bIsPro==False?L"アマ":L"", jano[9-nLevel], L"段");
		}
		else if (nLevel==29)
		{
			wsprintfW(pszBuffer, L"セミプロ");
		}
		else if (nLevel==28)
		{
			wsprintfW(pszBuffer, L"先生");
		}
		else
		{
			wsprintfW(pszBuffer, L"%d%s", nLevel-10+1, L"級");
		}
	}
	//else if (eLocale==eGCL_KOREAN)
	//{
	//	if (nLevel<10)
	//	{
	//		wsprintf(pszBuffer, L"%s%s%s", bIsPro==False?L"アマ":L"", jano[nLevel-9], L"段");
	//	}
	//	else
	//	{
	//		wsprintf(pszBuffer, L"%d%s", nLevel-10, L"級");
	//	}
	//}
	else
	{
		if (nLevel<10)
		{
			wsprintfW(pszBuffer, L"%s%dd", bIsPro==False?L"ama ":L"", nLevel-9);
		}
		else if (nLevel==30)
		{
			wsprintfW(pszBuffer, L"semi pro");
		}
		else if (nLevel==29)
		{
			return 0;
			//wsprintf(pszBuffer, L"先生");
		}
		else
		{
			wsprintfW(pszBuffer, L"%dk", nLevel-10);
		}
	}

	return wcslen(pszBuffer);
}


// ---------------------------------------
Font* cyctrlGobanResource::GetFont(int id)
{
	switch(id)
	{
		case eRIDF_SYSTEM:
			return m_SystemFont;
		case eRIDF_TAB_CONTROL:
			return m_TabFont;
		case eRIDF_BUTTON:
			return m_ButtonFont;
		case eRIDF_BUTTON_LARGE:
			return m_ButtonFontLarge;
		case eRIDF_LABEL:
			return m_LabelFont;
		case eRIDF_NO:
			return m_NoFont;
		case eRIDF_LABEL_SMALL:
			return m_LabelFontSmall;
		case eRIDF_BUTTON_SMALL:
			return m_ButtonFontSmall;
		case eRIDF_BRANCH:
			return m_fntBranch;
		default:
			break;
	}
	return 0;
}



// ---------------------------------------
static Bitmap* xLoadGoban(HINSTANCE hInstance)
{
	IO::Stream stream;

	//hInstance = m_hInstance;//hInstance==0?GetModuleHandle("yctrlGo.dll"):hInstance;
	stream.CreateOnResource(hInstance, MAKEINTRESOURCE(IDB_GOBAN), "JPG");

	return new Bitmap(stream);
}

// ---------------------------------------
static Bitmap* xMakeGoban(Bitmap* org, float scale)
{
	Bitmap* bmp = new Bitmap(org->GetWidth()*scale, org->GetHeight()*scale, PixelFormat32bppARGB);
	Graphics g(bmp);
	g.DrawImage(org, 0, 0);

	return bmp;
}



// ---------------------------------
static Bitmap* xMakeBlackStone(Bool IsNormal, float scale)
{
	Bitmap* bmp = new Bitmap(ORG_STONE_SIZE*scale, ORG_STONE_SIZE*scale, PixelFormat32bppARGB);
	Graphics g(bmp);

	UINT stone_width = bmp->GetWidth();
	UINT stone_height = bmp->GetWidth();
	Bitmap bmptemp(stone_width, stone_height, PixelFormat32bppARGB);
	Graphics gtemp(&bmptemp);

	float _w = stone_width/2;
	float s = stone_width/2-_w/2;

	// ○を描画
	gtemp.FillEllipse(&SolidBrush(0xFFFFFFFF), (float)s, (float)s, (float)_w, (float)_w);

	// 反射される光のイメージを作成
	Bitmap* pLight = cyiBitmapBlender::GaussBlureInnerGrow(&bmptemp, 10.0*scale, 0.7);

	Bitmap stone(stone_width, stone_height);
	Graphics gstone(&stone);

	// スケールが0.3より大きい場合のみ（約10ピクセル程度)
	// 輪郭をややぼやける。
//	if (scale>0.3)
//	{
		// ○のイメージを作成して、
		gstone.SetSmoothingMode(SmoothingModeHighQuality);
		gstone.FillEllipse(&SolidBrush(Color::Black), 0, 0, stone_width-0.1, stone_height-0.1);

		cyiBitmapBlender::GaussBlureInnerGrow(&stone, 0.5*10*scale);
//	}

	gstone.DrawImage(pLight,(float) -0.6*_w/2, (float)-0.6*_w/2);

	g.DrawImage(&stone, 0, 0);

	if (!IsNormal) // 碁石の透明イメージ(30%)
	{
		Bitmap* t = cyiBitmapBlender::Transparent(bmp, 0.7);
		delete bmp;
		delete pLight;
		return t;
	}

	// イメージの光を削除。
	delete pLight;
	return bmp;
}

// ---------------------------------
static Bitmap* xMakeWhiteStone(Bool IsNormal, float scale)
{
	Bitmap* bmp = new Bitmap(ORG_STONE_SIZE*scale, ORG_STONE_SIZE*scale, PixelFormat32bppARGB);
	Graphics g(bmp);

	UINT stone_width = bmp->GetWidth();
	UINT stone_height = bmp->GetWidth();
	Bitmap bmptemp(stone_width, stone_height, PixelFormat32bppARGB);
	Graphics gtemp(&bmptemp);

	//gtemp.SetSmoothingMode(SmoothingMode::SmoothingModeHighSpeed);

	float _w = stone_width/3;
	float s = stone_width/2-_w/2;

	// ○を描画
	gtemp.FillEllipse(&SolidBrush(0xFFFFFFFF), (float)s, (float)s, (float)_w, (float)_w);

	// 光を描画
	Bitmap* pLight = cyiBitmapBlender::GaussBlureInnerGrow(&bmptemp, 6.0*scale, 1.0);

	Bitmap stone(stone_width, stone_height);
	Graphics gstone(&stone);
	gstone.Clear(Color::Transparent);

	gstone.SetSmoothingMode(SmoothingModeHighQuality);

	// 影を描画
	gstone.FillEllipse(&SolidBrush(Color(255, 235,235,235)), 0, 0, stone_width-1, stone_height-1);

	// 輪郭をぼかす
	if (scale>0.3)
	{
		cyiBitmapBlender::GaussBlureInnerGrow(&stone, 0.5*scale);
	}

	// ここまで出来上がったビットマップをコピー
	Bitmap* mask = cyiBitmapBlender::BitmapCopy(NULL, &stone);
	
	// 貝殻作成
	{
		float k=stone_height/5;

		LinearGradientBrush brush(PointF(0, 0), PointF(stone_width>>2, 0), Color::LightGray, Color::White);
		Bitmap* moyou = cyiShape::FillMikatsuki(NULL, stone_width>>2, stone_height*4/5, &brush);
		cyiBitmapBlender::GaussBlureInnerGrow(moyou, 1.0*scale, 0.3);

		Bitmap* mouT = cyiBitmapBlender::Transparent(moyou, 0.5);
		delete moyou;

		for (int i=0; i<4; i++)
			gstone.DrawImage(mouT, (float)i*k, (float)stone_height/10);
			//g.DrawLine(&Pen(Color::Pink), i*k, 0.0, i*k, stone_height);

		//cyiBitmapBlender::Reverse(moyou);

		//for (int i=4; i<8; i++)
		//	gstone.DrawImage(moyou, (float)i*k, (float)0);

		
		//gstone.DrawEllipse(&Pen(Color(64, 0,0,0)), 0, 0, stone_width-1, stone_height-1);

		delete mouT;
	}

	//{
	//	Bitmap shade(stone_width, stone_height, PixelFormat32bppARGB);
	//	Graphics g(&shade);
	//	gstone.SetSmoothingMode(SmoothingModeHighQuality);
	//	g.DrawEllipse(&Pen(Color::Black), 0, 0, stone_width-2, stone_height-2);

	//	cyiBitmapBlender::GaussBlureInnerGrow(&shade, 5.0*m_scale, 0.3*m_scale);
	//	gstone.DrawImage(&shade, -1, -1);

	//}


	gstone.DrawImage(pLight,(float) -_w/2, (float)-_w/2);
	cyiBitmapBlender::SetMask(&stone, mask, 0, 0);

	delete mask;

	g.DrawImage(&stone, 0, 0);

	if (!IsNormal)// 碁石の透明イメージ(30%)
	{
		Bitmap* t = cyiBitmapBlender::Transparent(bmp, 0.7);
		delete bmp;
		delete pLight;
		return t;
	}

	//g.DrawImage(cyiBitmapBlender::GaussBlureInnerGrow(&stone, 0.5*m_scale), 0,0);//, 1.0, 1.0, (float)bmp->GetWidth()+2, (float)bmp->GetHeight()+2, Unit::UnitPixel);

	delete pLight;
	return bmp;
}

// --------------------------------------------
Bitmap* xMakeStoneShadow(float scale)
{
	Bitmap* bmp = new Bitmap((ORG_STONE_SIZE)*scale+4, (ORG_STONE_SIZE)*scale+4, PixelFormat32bppARGB);
	UINT w = bmp->GetWidth();
	UINT h = bmp->GetHeight();

	Graphics g(bmp);
	g.FillEllipse(&SolidBrush(Color::Black), 2, 2, w-4, h-4);
	Bitmap* temp = cyiBitmapBlender::GaussBlureXX(bmp, 2.0, 0, 0);
	Bitmap* shadow = cyiBitmapBlender::Transparent(temp, 0.5*scale);
	//Bitmap* shadow = cyiBitmapBlender::GaussBlureInnerGrow(bmp, 1.0, 0.3*scale);

	delete temp;
	delete bmp;
	return shadow;
}

typedef struct __line_table__ {
	color_t col[5];
} line_table;

static line_table glinetbl[5] = {
	{ 0, 0, 0, 0, 0},
	{ 0, 0, 255, 0, 0},
	{ 0, 128, 255, 128, 0},
	{ 0, 255, 255, 255, 0},
	{ 0, 255, 255, 255, 0}
};

// -----------------------------------------
static Bitmap* xGetLine(float w, UINT len, color_t color, int direction=0/*0 is horizental, 1 is vertical*/)
{
	UINT _w = (UINT)w;

	line_table* tbl1 = &glinetbl[_w];
	line_table* tbl2 = &glinetbl[_w+1];
	line_table tbl;

	float factor = w-(float)_w;

	for (int i=0; i<5; i++)
	{
		tbl.col[i] = (1.0-factor)*tbl1->col[i]+factor*tbl2->col[i];
	}

	if (direction==0)
	{
		Bitmap line_base(1, 5, PixelFormat32bppARGB);
		for (int i=0; i<5; i++)
			line_base.SetPixel(0, i, Color(tbl.col[i] , 0, 0, 0));

		Bitmap* line = new Bitmap(len, 5, PixelFormat32bppARGB);
		Graphics g(line);
		//g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		for (UINT i=0; i<len; i++)
			g.DrawImage(&line_base, i, 0);

		return line;
	}

	Bitmap line_base(5, 1, PixelFormat32bppARGB);
	for (int i=0; i<5; i++)
		line_base.SetPixel(i, 0, Color(tbl.col[i] , 0, 0, 0));

	Bitmap* line = new Bitmap(5, len, PixelFormat32bppARGB);
	Graphics g(line);
	//g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	for (UINT i=0; i<len; i++)
		g.DrawImage(&line_base, 0, i);

	return line;
}


static POINT __star__[9] = {
	{3, 3},
	{9, 3},
	{15, 3},
	{3, 9},
	{9, 9},
	{15, 9},
	{3, 15},
	{9, 15},
	{15, 15}
};


#ifdef NEW_STYLE

// ---------------------------------
static Bitmap* xMakeBackground(Bitmap* goban, int nLineCnt, float scale, int _w, int _h, Bitmap** bmpPureGoban)
{
	float w = (float)(_w?_w:(float)MAX_GOBAN_WIDTH*scale);  // 碁盤のサイズ
	float h = (float)(_h?_h:(float)MAX_GOBAN_HEIGHT*scale);  // 碁盤のサイズ
	float s = (float)ORG_START_INDEX*scale; // 線の始まりの幅
	float b = (float)ORG_STONE_SIZE*scale;  // 一ブロックの幅
	float yOffset = _w==_h ? 0.0:(float)MAX_CONTROL_SIZE*scale;

	cyctrlGobanResource::GetInstance().m_fGobanSize=w;
	cyctrlGobanResource::GetInstance().m_fLineStartOffset = s;//m_nLineStartOffset;
	cyctrlGobanResource::GetInstance().m_fBlockWidth = b;//m_nBlockWidth;

	//if (*bmpPureGoban!=0)
	//	delete *bmpPureGoban;

	_w=_w?_w:(int)w;
	_h=_h?_h:(int)h;
	Bitmap* bmp = new Bitmap(_w, _h, PixelFormat32bppARGB);
	*bmpPureGoban = new Bitmap(_w, _h, PixelFormat32bppARGB);
	Graphics g(bmp);
	Graphics gPureGoban(*bmpPureGoban);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	UINT rect_width = w-s-s; // 純粋な枠のサイズ

	color_t color=Color::Black;
	float pen_width=2;
	//__adjust__(m_scale, color, pen_width);

	// バックに碁盤を描画
	g.DrawImage(goban, 0, 0, (int)_w, (int)_h);

	gPureGoban.DrawImage(bmp, 0, 0);

	// 外枠の太さ
	float border_scale = 2.0*scale;

	color_t border_color=0xFF000000;

	if (border_scale < 0.5)
	{
		color_t a = (color_t)((border_scale*2)*255);
		border_color = __COLORp__(a, 0, 0, 0);
	}

	Bitmap* pBorderH = xGetLine(border_scale, rect_width,  border_color);
	Bitmap* pBorderV = xGetLine(border_scale, rect_width, border_color, 1);

	
	UINT bwx = (pBorderH->GetHeight())>>1;

	g.DrawImage(pBorderH, (float)s, (float)(s+rect_width-bwx-1)+yOffset);
	g.DrawImage(pBorderH, (float)s, (float)(s-bwx)+yOffset);

	g.DrawImage(pBorderV, (float)(s-bwx), (float)s+yOffset);
	g.DrawImage(pBorderV, (float)(s+rect_width-bwx-1),(float) s+yOffset);

	float line_scale = 1.5*scale;

	color_t line_color=0xFF000000;

	// スケールが0.5より小さければ
	//if (line_color < 0.5)
	if (line_scale < 0.5)
	{
		color_t a = (color_t)((line_scale*2)*255);
		border_color = __COLORp__(a, 0, 0, 0);
	}
	delete pBorderV;
	delete pBorderH;

	Bitmap* pLineH = ::xGetLine(line_scale, rect_width-2,  line_color);
	Bitmap* pLineV = ::xGetLine(line_scale, rect_width-2, line_color, 1);

	UINT lwx = (pLineH->GetHeight())>>1;

	//UINT _b = __INTEGER__(b);
	for (int i=1; i<nLineCnt-1; i++)
	{
		UINT _start = (UINT)((float)i*b);

		g.DrawImage(pLineV, (float)(s+1+_start-lwx), (float)(s+1)+yOffset);
		g.DrawImage(pLineH, (float)(s+1),(float) (s+1+_start-lwx)+yOffset);

		//m_scaled_point[i] = s+1+_start;
		//g.DrawLine(&pen, s+_start, s, s+_start, s+rect_width);
		//g.DrawLine(&pen, s, s+_start, s+rect_width, s+_start);
	}

	//m_scaled_point[0]=s+1;
	//m_scaled_point[m_nLine-1]=s+1+(UINT)(m_nLine-1)*b;

	g.DrawRectangle(&Pen(Color::Black), 0, (INT)0, (UINT)w-1, (UINT)h-1);

	float star_w = scale*11.0;
	float star_offset=star_w/2;

	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	// 星を書く
	if (nLineCnt==19)
	{
		for (int i=0; i<9; i++)
		{
			UINT x = __star__[i].x*b;
			UINT y = __star__[i].y*b;

			g.FillEllipse(&SolidBrush(Color(line_color>>24, 0, 0, 0)), s+x-star_offset+1, s+y-star_offset+1+yOffset, star_w, star_w);
		}
	}
	
	return bmp;
}

#else
// ---------------------------------
static Bitmap* xMakeBackground(Bitmap* goban, int nLineCnt, float scale, int _w, int _h, Bitmap** bmpPureGoban)
{
	float w = (float)(_w?_w:(float)ORG_GOBAN_SIZE*scale);  // 碁盤のサイズ
	float s = (float)ORG_START_INDEX*scale; // 線の始まりの幅
	float b = (float)ORG_STONE_SIZE*scale;  // 一ブロックの幅

	cyctrlGobanResource::GetInstance().m_fGobanSize=w;
	cyctrlGobanResource::GetInstance().m_fLineStartOffset = s;//m_nLineStartOffset;
	cyctrlGobanResource::GetInstance().m_fBlockWidth = b;//m_nBlockWidth;

	//if (*bmpPureGoban!=0)
	//	delete *bmpPureGoban;

	_w=_w?_w:(int)w;
	Bitmap* bmp = new Bitmap(_w, _w, PixelFormat32bppARGB);
	*bmpPureGoban = new Bitmap(_w, _w, PixelFormat32bppARGB);
	Graphics g(bmp);
	Graphics gPureGoban(*bmpPureGoban);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);



	UINT rect_width = w-s-s; // 純粋な枠のサイズ

	color_t color=Color::Black;
	float pen_width=2;
	//__adjust__(m_scale, color, pen_width);

	// バックに碁盤を描画
	g.DrawImage(goban, 0, 0, (int)w, (int)w);

	gPureGoban.DrawImage(bmp, 0, 0);

	// 外枠の太さ
	float border_scale = 2.0*scale;

	color_t border_color=0xFF000000;

	if (border_scale < 0.5)
	{
		color_t a = (color_t)((border_scale*2)*255);
		border_color = __COLORp__(a, 0, 0, 0);
	}

	Bitmap* pBorderH = xGetLine(border_scale, rect_width,  border_color);
	Bitmap* pBorderV = xGetLine(border_scale, rect_width, border_color, 1);

	
	UINT bwx = (pBorderH->GetHeight())>>1;

	g.DrawImage(pBorderH, (float)s, (float)(s+rect_width-bwx-1));
	g.DrawImage(pBorderH, (float)s, (float)(s-bwx));

	g.DrawImage(pBorderV, (float)(s-bwx), (float)s);
	g.DrawImage(pBorderV, (float)(s+rect_width-bwx-1),(float) s);

	float line_scale = 1.5*scale;

	color_t line_color=0xFF000000;

	// スケールが0.5より小さければ
	//if (line_color < 0.5)
	if (line_scale < 0.5)
	{
		color_t a = (color_t)((line_scale*2)*255);
		border_color = __COLORp__(a, 0, 0, 0);
	}
	delete pBorderV;
	delete pBorderH;

	Bitmap* pLineH = ::xGetLine(line_scale, rect_width-2,  line_color);
	Bitmap* pLineV = ::xGetLine(line_scale, rect_width-2, line_color, 1);

	UINT lwx = (pLineH->GetHeight())>>1;

	//UINT _b = __INTEGER__(b);
	for (int i=1; i<nLineCnt-1; i++)
	{
		UINT _start = (UINT)(i*b);

		g.DrawImage(pLineV, (float)(s+1+_start-lwx), (float)(s+1));
		g.DrawImage(pLineH, (float)(s+1),(float) (s+1+_start-lwx));

		//m_scaled_point[i] = s+1+_start;
		//g.DrawLine(&pen, s+_start, s, s+_start, s+rect_width);
		//g.DrawLine(&pen, s, s+_start, s+rect_width, s+_start);
	}

	//m_scaled_point[0]=s+1;
	//m_scaled_point[m_nLine-1]=s+1+(UINT)(m_nLine-1)*b;

	g.DrawRectangle(&Pen(Color::Black), 0, 0, (UINT)w-1, (UINT)w-1);

	float star_w = scale*11.0;
	float star_offset=star_w/2;

	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	// 星を書く
	if (nLineCnt==19)
	{
		for (int i=0; i<9; i++)
		{
			UINT x = __star__[i].x*b;
			UINT y = __star__[i].y*b;

			g.FillEllipse(&SolidBrush(Color(line_color>>24, 0, 0, 0)), s+x-star_offset+1, s+y-star_offset+1, star_w, star_w);
		}
	}
	
	return bmp;
}
#endif

// ---------------------------------
static Bitmap* xMakeTriAngleEx(float m_scale, color_t color)
{
	float shape_size=ORG_STONE_SIZE*m_scale*0.6;
	Bitmap* tri = cyiShape::DrawRoundTriangle(0, &Pen(color, SHAPE_WIDTH), RectF(SHAPE_WIDTH, SHAPE_WIDTH, shape_size-SHAPE_WIDTH*2-1, shape_size-SHAPE_WIDTH*2-1),1.0, 180);
	Bitmap* temp = cyiBitmapBlender::RotateCopy(0, tri, 2);
	Bitmap* ret= cyiBitmapBlender::DropShadow(temp, 0, 0, 1.0, 0.5);
	delete temp;
	delete tri;
	return ret;
}

// ---------------------------------
static Bitmap* xMakeTriAngle(Bitmap* bmp, Bool IsBlack, float scale, color_t color)
{
	int w=bmp->GetWidth();
	int h=bmp->GetHeight();
	float margine = w*0.2;
	Bitmap* cir = new Bitmap(w, h, PixelFormat32bppARGB);
	Graphics g(cir);

	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	g.DrawEllipse(&Pen(IsBlack?Color::Yellow:Color::DarkBlue, 1.0+scale), margine, margine, w-margine*2-(1.0+scale), h-margine*2-(1.0+scale));

	return cir;
}


// ---------------------------------
static Bitmap* xMakeAreaBmp(Bool bIsBlack, float scale)
{
	Bitmap* bmp = new Bitmap(MAX_BLOCK_SIZE*scale*0.3, MAX_BLOCK_SIZE*scale*0.3, PixelFormat32bppARGB);
	int w = bmp->GetWidth();
	int h = bmp->GetHeight();

	Graphics g(bmp);
	
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	g.FillEllipse(&SolidBrush(bIsBlack?Color::Black:Color::White), 0, 0, w, h);

	return bmp;
}


// ----------------------------------
Bitmap* xMakeCross(float scale, Bool bIsCursor, color_t color)
{
	float w = MAX_BLOCK_SIZE*scale*0.5;
	float h = MAX_BLOCK_SIZE*scale*0.5;

	Bitmap* bmp = cyiShape::DrawCross(0, &Pen(color, 1.0+scale*2.0), RectF(0, 0, w, h));


	if (bIsCursor)
	{
		Bitmap* cur = cyiBitmapBlender::DropShadow(bmp, scale, scale, 2, 0.3);
		return bmp;
	}

	//return bmp;
	Bitmap* ret= cyiBitmapBlender::DropShadow(bmp, 0, 0, 1.0, 0.5);

	delete bmp;
	return ret;
}



// ----------------------------------
Bitmap* xMakeDoubleCircle(float scale, color_t color)
{
	int rad = (float)((float)MAX_BLOCK_SIZE*scale*0.6);
	int rad_hf=rad>>1;
	Bitmap* bmp = new Bitmap(rad, rad, PixelFormat32bppARGB);

	Graphics g(bmp);
	
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	g.DrawEllipse(&Pen(color, 1.0+scale), (float)0.0, (float)0.0, (float)rad-(1.0+scale), (float)rad-(1.0+scale));
	g.DrawEllipse(&Pen(color, 1.0+scale), (float)rad_hf/2, (float)rad_hf/2, (float)rad_hf-(1.0+scale), (float)rad_hf-(1.0+scale));

	//return bmp;
	Bitmap* ret= cyiBitmapBlender::DropShadow(bmp, 0, 0, 1.0, 0.5);

	delete bmp;
	return ret;

}

// ----------------------------------
Bitmap* xMakeCircle(float scale, color_t color)
{
	int rad = (float)((float)MAX_BLOCK_SIZE*scale*0.6);
	int rad_hf=rad>>1;
	Bitmap* bmp = new Bitmap(rad, rad, PixelFormat32bppARGB);

	Graphics g(bmp);
	
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	g.DrawEllipse(&Pen(color, 1.0+scale), (float)0.0, (float)0.0, (float)rad-(1.0+scale), (float)rad-(1.0+scale));

	//return bmp;
	Bitmap* ret= cyiBitmapBlender::DropShadow(bmp, 0, 0, 1.0, 0.5);

	delete bmp;
	return ret;
}

// ----------------------------------
Bitmap* xMakeRectangle(float m_scale, color_t color)
{
	float shape_size=ORG_STONE_SIZE*m_scale*0.5;

	Bitmap* bmpLabel = new Bitmap(shape_size, shape_size);
	Graphics g(bmpLabel);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	g.DrawRectangle(&Pen(color, 3.0*m_scale), RectF(SHAPE_WIDTH, SHAPE_WIDTH, bmpLabel->GetWidth()-SHAPE_WIDTH*2, bmpLabel->GetHeight()-SHAPE_WIDTH*2));

	Bitmap* ret= cyiBitmapBlender::DropShadow(bmpLabel, 0, 0, 1.0, 0.5);
	delete bmpLabel;

	return ret;
}

// ----------------------------------
Bitmap* xMakeCircleInChar(float scale, wchar_t* ch, Font* font, color_t color, color_t color_txt)
{
	int rad = (float)((float)MAX_BLOCK_SIZE*scale*0.6);
	int rad_hf=rad>>1;
	Bitmap* bmp = new Bitmap(rad, rad, PixelFormat32bppARGB);

	Graphics g(bmp);
	
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	g.DrawEllipse(&Pen(color, 1.0+scale), (float)0.0, (float)0.0, (float)rad-(1.0+scale), (float)rad-(1.0+scale));

	Bitmap* bmpStr = cyiShape::BitmapString(0, ch, font, &SolidBrush(color_txt));

	int w=bmpStr->GetWidth();
	int h=bmpStr->GetHeight();

	g.DrawImage(bmpStr, (INT)((rad-w)/2), (INT)((rad-h)/2)+1);//, (INT)bmpStr->GetWidth(), (INT)bmpStr->GetHeight());
	delete bmpStr;

	Bitmap* ret= cyiBitmapBlender::DropShadow(bmp, 0, 0, 1.0, 0.5);

	delete bmp;

	return ret;
}


// ----------------------------------
Bitmap* xMakeAddCursor(float scale)
{
		//m_LabelFont = new Font(L"Meirio", 20.0*scale);
	Font font(L"Meirio", 20.0*scale);
	Bitmap* left=xMakeCircleInChar(scale, L"L", &font, Color::Black, Color::Black);
	Bitmap* right = xMakeCircleInChar(scale, L"R", &font, Color::White, Color::White);

	Bitmap* bmp = BITMAP32(left->GetWidth()+right->GetWidth(), left->GetHeight());
	Graphics g(bmp);

	g.DrawImage(left, 0, 0);
	g.DrawImage(right, left->GetWidth(), 0);
	/*float w = MAX_BLOCK_SIZE*scale*0.8;
	float wh=w/2;

	Bitmap* bmp = new Bitmap(w, w, PixelFormat32bppARGB);
	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	g.DrawArc(&Pen(Color::Black, 1.0+scale), RectF(0, 0, w-1, w-1), 90, 180);
	g.DrawArc(&Pen(Color::White, 1.0+scale), RectF(0, 0, w-1, w-1), 90, -180);
	g.DrawLine(&Pen(Color::Black, 1.0+scale), wh-1.0, 0.0, wh-1, w);
	g.DrawLine(&Pen(Color::White, 1.0+scale), wh+1.0, 0.0, wh+1, w);

	g.SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAlias);
	g.DrawString(L"L",1, &Font(L"Merio", 17.0*scale), RectF(0.0, 0.0, wh, wh), 0, &SolidBrush(Color::Black));  

	//g.FillEllipse(&SolidBrush(Color::LightGreen),wh-2, wh-2, 4.0, 4.0);// RectF(wh-1, wh-1, 2, 2));

*/
	return bmp;
}

extern HINSTANCE g_hInstanceGoban;

// ---------------------------------------
Bitmap* cyctrlGobanResource::GetGobanBitmap(int _w, int nLineCnt, float* coor, int* blockSize)
{
	Bitmap* goban = m_bmpGoban;

	if (m_bmpGoban==0)
	{
		m_bmpGoban = xLoadGoban(g_hInstanceGoban);//::GetModuleHandleA(name));

		//Initialize(1.0, g_hInstanceGoban);
		goban=m_bmpGoban;
	}

#ifndef NEW_STYLE
	float scale = (float)((float)_w/(float)ORG_GOBAN_SIZE);
	float w = (float)ORG_GOBAN_SIZE*scale;  // 碁盤のサイズ
#else
	float scale = (float)((float)_w/(float)MAX_GOBAN_WIDTH);
	float w = (float)MAX_GOBAN_WIDTH*scale;  // 碁盤のサイズ
#endif

	float s = (float)ORG_START_INDEX*scale; // 線の始まりの幅
	float b = (float)ORG_STONE_SIZE*scale;  // 一ブロックの幅

	//cyctrlGobanResource::GetInstance().m_fGobanSize=w;
	//cyctrlGobanResource::GetInstance().m_fLineStartOffset = s;//m_nLineStartOffset;
	//cyctrlGobanResource::GetInstance().m_fBlockWidth = b;//m_nBlockWidth;

	Bitmap* bmp = new Bitmap(w, w, PixelFormat32bppARGB);
	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);



	UINT rect_width = /*s+*/(UINT)((float)(nLineCnt-1)*b)+2;//-lwx;//w-s-s; // 純粋な枠のサイズ
	*blockSize=b;



	color_t color=Color::Black;
	float pen_width=2;
	//__adjust__(m_scale, color, pen_width);

	// バックに碁盤を描画
	g.DrawImage(goban, 0, 0, (int)w, (int)w);

	// 外枠の太さ
	float border_scale = 3.0*scale;

	color_t border_color=0xFF000000;

	if (border_scale < 0.5)
	{
		color_t a = (color_t)((border_scale*2)*255);
		border_color = __COLORp__(a, 0, 0, 0);
	}

	Bitmap* pBorderH = xGetLine(border_scale, rect_width,  border_color);
	Bitmap* pBorderV = xGetLine(border_scale, rect_width, border_color, 1);

	
	UINT bwx = (pBorderH->GetHeight())>>1;

	g.DrawImage(pBorderH, (float)s, (float)(s-bwx));
	g.DrawImage(pBorderV, (float)(s-bwx), (float)s);

	g.DrawImage(pBorderH, (float)s, (float)(s+rect_width-bwx-1));
	g.DrawImage(pBorderV, (float)(s+rect_width-bwx-1),(float) s);

	float line_scale = 1.5*scale;

	color_t line_color=0xFF000000;

	// スケールが0.5より小さければ
	//if (line_color < 0.5)
	if (line_scale < 0.5)
	{
		color_t a = (color_t)((line_scale*2)*255);
		border_color = __COLORp__(a, 0, 0, 0);
	}
	delete pBorderV;
	delete pBorderH;

	Bitmap* pLineH = ::xGetLine(line_scale, rect_width-2,  line_color);
	Bitmap* pLineV = ::xGetLine(line_scale, rect_width-2, line_color, 1);

	UINT lwx = (pLineH->GetHeight())>>1;

	//UINT _b = __INTEGER__(b);
	coor[0]=(float)(s+1);
	
	for (int i=1; i<nLineCnt-1; i++)
	{
		UINT _start = (UINT)(i*b);

		coor[i]=(float)(s+1+_start);
		g.DrawImage(pLineV, (float)(s+1+_start-lwx), (float)(s+1));
		g.DrawImage(pLineH, (float)(s+1),(float) (s+1+_start-lwx));

		//m_scaled_point[i] = s+1+_start;
		//g.DrawLine(&pen, s+_start, s, s+_start, s+rect_width);
		//g.DrawLine(&pen, s, s+_start, s+rect_width, s+_start);
	}
	coor[nLineCnt-1]=(float)(s+1+(UINT)((nLineCnt-1)*b));



	//m_scaled_point[0]=s+1;
	//m_scaled_point[m_nLine-1]=s+1+(UINT)(m_nLine-1)*b;

	//g.DrawRectangle(&Pen(Color::Black), 0, 0, (UINT)w-2, (UINT)w-2);

	float star_w = scale*11.0;
	float star_offset=star_w/2;

	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	// 星を書く
	if (nLineCnt==19)
	{
		for (int i=0; i<9; i++)
		{
			UINT x = __star__[i].x*b;
			UINT y = __star__[i].y*b;

			g.FillEllipse(&SolidBrush(Color(line_color>>24, 0, 0, 0)), s+x-star_offset+1, s+y-star_offset+1, star_w, star_w);
		}
	}
	
	return bmp;

}

// ---------------------------------------
Bitmap* cyctrlGobanResource::GetGobanBitmap(int w, int h, float yOffset, int nLineCount, POINTF* coor, int* nBlockSize, Bitmap** bmpPureGoban)
{
	if (m_bmpGoban==0)
	{
		char name[128];
		GetModuleFileNameA(0, name, 128);
		m_bmpGoban = xLoadGoban(g_hInstanceGoban);
		//Initialize(1.0, g_hInstanceGoban);
		//goban=m_bmpGoban;
	}

#ifndef NEW_STYLE
	float scale = (float)((float)w/(float)ORG_GOBAN_SIZE);
	//float w = (float)ORG_GOBAN_SIZE*scale;  // 碁盤のサイズ
#else
	float scale = (float)((float)w/(float)MAX_GOBAN_WIDTH);
	//float w = (float)MAX_GOBAN_WIDTH*scale;  // 碁盤のサイズ
#endif

	float s = (float)ORG_START_INDEX*scale; // 線の始まりの幅
	float b = (float)ORG_STONE_SIZE*scale;  // 一ブロックの幅

	//cyctrlGobanResource::GetInstance().m_fGobanSize=w;
	//cyctrlGobanResource::GetInstance().m_fLineStartOffset = s;//m_nLineStartOffset;
	//cyctrlGobanResource::GetInstance().m_fBlockWidth = b;//m_nBlockWidth;

	Bitmap* bmp = new Bitmap(w, h, PixelFormat32bppARGB);
	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	UINT rect_width = /*s+*/(UINT)((nLineCount-1)*b)+2;//-lwx;//w-s-s; // 純粋な枠のサイズ

	if (nBlockSize)
		*nBlockSize=b;

	color_t color=Color::Black;
	float pen_width=2;
	//__adjust__(m_scale, color, pen_width);

	// バックに碁盤を描画
	g.DrawImage(m_bmpGoban, 0, 0, (int)w, (int)h);

	if (bmpPureGoban)
	{
		*bmpPureGoban = cBB::BitmapCopy(0, bmp);
	}

	// 外枠の太さ
	float border_scale = 3.0*scale;

	color_t border_color=0xFF000000;

	if (border_scale < 0.5)
	{
		color_t a = (color_t)((border_scale*2)*255);
		border_color = __COLORp__(a, 0, 0, 0);
	}

	Bitmap* pBorderH = xGetLine(border_scale, rect_width,  border_color);
	Bitmap* pBorderV = xGetLine(border_scale, rect_width, border_color, 1);

	
	UINT bwx = (pBorderH->GetHeight())>>1;

	g.DrawImage(pBorderH, (float)s, (float)(s+rect_width-bwx-1+yOffset));
	g.DrawImage(pBorderH, (float)s, (float)(s-bwx+yOffset));

	g.DrawImage(pBorderV, (float)(s-bwx), (float)(s+yOffset));
	g.DrawImage(pBorderV, (float)(s+rect_width-bwx-1),(float)(s+yOffset));

	float line_scale = 1.5*scale;

	color_t line_color=0xFF000000;

	// スケールが0.5より小さければ
	//if (line_color < 0.5)
	if (line_scale < 0.5)
	{
		color_t a = (color_t)((line_scale*2)*255);
		border_color = __COLORp__(a, 0, 0, 0);
	}
	delete pBorderV;
	delete pBorderH;

	Bitmap* pLineH = ::xGetLine(line_scale, rect_width-2,  line_color);
	Bitmap* pLineV = ::xGetLine(line_scale, rect_width-2, line_color, 1);

	UINT lwx = (pLineH->GetHeight())>>1;

	//UINT _b = __INTEGER__(b);
	coor[0].x=(float)(s+1);
	coor[0].y+=yOffset;
	
	for (int i=1; i<nLineCount-1; i++)
	{
		UINT _start = (UINT)((float)i*b);

		coor[i].x=(float)(s+1+_start);
		coor[i].y=(float)(s+1+_start+yOffset);
		//coor[i]=(float)(s+1+_start-lwx);
		g.DrawImage(pLineV, (float)(s+1+_start-lwx), (float)(s+1+yOffset));
		g.DrawImage(pLineH, (float)(s+1),(float) (s+1+_start-lwx+yOffset));

		//m_scaled_point[i] = s+1+_start;
		//g.DrawLine(&pen, s+_start, s, s+_start, s+rect_width);
		//g.DrawLine(&pen, s, s+_start, s+rect_width, s+_start);
	}
	coor[nLineCount-1].x=(float)(s+1+(UINT)((nLineCount-1)*b));
	coor[nLineCount-1].y=(float)(s+1+(UINT)((nLineCount-1)*b)+yOffset);

	//m_scaled_point[0]=s+1;
	//m_scaled_point[m_nLine-1]=s+1+(UINT)(m_nLine-1)*b;

	//g.DrawRectangle(&Pen(Color::Black), 0, 0, (UINT)w-2, (UINT)w-2);

	float star_w = scale*11.0;
	float star_offset=star_w/2;

	g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

	// 星を書く
	if (nLineCount==19)
	{
		for (int i=0; i<9; i++)
		{
			UINT x = __star__[i].x*b;
			UINT y = __star__[i].y*b;

			g.FillEllipse(&SolidBrush(Color(line_color>>24, 0, 0, 0)), s+x-star_offset+1, s+y-star_offset+1+yOffset, star_w, star_w);
		}
	}

	if (bmpPureGoban)
	{
		g.DrawRectangle(&Pen(Color::Black), 0, 0, w-1, h-1);
	}
	
	return bmp;
}