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


// �u���b�N�T�C�Y
#define MAX_BLOCK_SIZE 54

// �΂̃T�C�Y
#define MAX_STONE_SIZE 54

// ��Ղ̗]��
#define MAX_START_INDEX 32



// ��Ղ̑S�̃T�C�Y
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
	eGTRID_SAVE_CONFIRM=1, //"%s��ۑ����܂����H", L"%s��ۑ����܂����H"},
	eGTRID_DELETE_CONFIRM=2, //"%s���폜���܂����H", L"%s���폜���܂����H"},
	eGTRID_CLOSE_CONFIRM=3, //"%s����܂����H", L"%s����܂����H"},
	eGTRID_WARNING=4, //"�x��", L"�x��"},
	eGTRID_INFORMATION=5, //"���m�点", L"���m�点"},
	eGTRID_REFERENCE=6,
	eGTRID_NOTITLED=7,
	eGTRID_MAIN=8,
	eGTRID_TITLE=9, //"�^�C�g��"
	eGTRID_DATE=10, // �΋Ǔ���
	eGTRID_CONDITION=11, // �΋Ǐ����i�荇���E�R�~�j
	eGTRID_LIMIT_TIME=12, // ��������
	eGTRID_RESULT=13, // ����
	eGTRID_GAMENAME=14, // ��
	eGTRID_PLACE=15, // �ꏊ
	eGTRID_TEAI=16,
	eGTRID_KOMI=17,
	eGTRID_PLAYRESULT=18,
	eGTRID_MOKU=19, //��
	eGTRID_HALF=20, //��
	eGTRID_AGEHAMA=21, // �A�Q�n��
	eGTRID_REMAIN_TIME=22, // �c�莞��
	eGTRID_SEISEKI=23, // ����
	eGTRID_TAISENN_SEISEKI=24, // �ΐ퐬��
	eGTRID_WIN=25, // ��
	eGTRID_MATCH=26, // ��
	eGTRID_LOSE=27, // ��
	eGTRID_DRAW=28, // ��������
	eGTRID_BLACK=29, // ��
	eGTRID_WHITE=30, // ��

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
	// 9�i -> 1
	// 8�i -> 2
	//		:
	//		:
	// ���i -> 9
	// ���� -> 10
	//		:
	// 18�� -> 28
	// ���b�X���v���F29 (�@���Ȃ�)
	// �Z�~�v���F30 (�@���Ȃ�)
	int GetLevelString(int nLevel, wchar_t* pszBuffer, eGoCtrlLocale eLocale=eGCL_JAPANESE);

private:
	// ---------------------------------------
	Font* CreateFont(wchar_t* fontname, float fMin, float fMax, float scale);

	static cyctrlGobanResource* m_me;

	// ���(�ő�T�C�Y)
	Bitmap* m_bmpGoban;

	// 
	Bitmap* m_bmpPureGoban;

	// �������킹�����
	Bitmap* m_BackGround;
	Bitmap* m_BackGround9;
	Bitmap* m_BackGround13;


	// ���C���[�W
	Bitmap* m_Line; //= xMakeLine(nLineCnt);

	// ���(�ʏ�E����)
	Bitmap* m_BlackStone;
	Bitmap* m_BlackStone_T;
	Bitmap* m_WhiteStone;
	Bitmap* m_WhiteStone_T;

	// ��΂̉e
	Bitmap* m_StoneShadow;

	// �|�C���^�[
	Bitmap* m_TriAngleB;
	Bitmap* m_TryAngleW;

	// �폜�J�[�\��
	Bitmap* m_bmpRedCrossCursor;

	// �폜���x��
	Bitmap* m_bmpRedCross;

	// ������
	Bitmap* m_StoneCross;

	// �Βǉ����̃J�[�\��
	Bitmap* m_bmpAddCursor;

	float m_scale;



	// �V�X�e���t�H���g(��ʓI�Ɏg����t�H���g�A�X�P�[��1.0�� 12.0�A�X�P�[���ŏ��F9.0)
	Font* m_SystemFont;

	// �{�^���ȂǂɎg����t�H���g(��ʓI�Ɏg����t�H���g�A�X�P�[��1.0�� 11.0�A�X�P�[���ŏ��F8.0)
	Font* m_ButtonFont;

	// �{�^���ȂǂɎg����t�H���g(��ʓI�Ɏg����t�H���g�A�X�P�[��1.0�� 11.0�A�X�P�[���ŏ��F8.0)
	Font* m_ButtonFontSmall;

	Font* m_ButtonFontLarge;

	// �^�u�ȂǂɎg����t�H���g(�������t�H���g�A�X�P�[��1.0�� 11.0�A�X�P�[���ŏ��F9.0)
	Font* m_TabFont;

	// ���x���p�t�H���g
	Font* m_LabelFont;

	// ���x���p�t�H���g
	Font* m_NoFont;

	// ���x���p�����ȃt�H���g
	Font* m_LabelFontSmall;

	// ���x���p�u�����`�p�t�H���g
	Font* m_fntBranch;



public:
	float m_fGobanSize;
	float m_fLineStartOffset;//m_nLineStartOffset;
	float m_fBlockWidth;//m_nBlockWidth;

	// --------------------------
	eGoCtrlLocale m_eSystemLocale;

};
