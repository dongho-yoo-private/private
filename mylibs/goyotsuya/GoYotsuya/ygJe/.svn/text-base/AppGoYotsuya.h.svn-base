#pragma once

#define _WINDLL
#include "yi.h"
#include "yistd.h"
#include "glsGoban.h"
//#include "yctrlPreviewGoban.h"
#include "glsDlgProgress.h"
#include "glsPreviewGoban.h"

#undef _WINDLL

#define WM_NOTIFY_NODE_CHANGED WM_USER+32

enum eAppMode {
	eAM_EDITOR,
	eAM_JOSEKI_EDITOR,
	eAM_REMOCON,
	eAM_JOSEKI_DICTIONARY
};

enum eAppGoYotsuyaCommand {
	eAGY_CMD_PLAY=0x10,
	eAGY_CMD_BACK,
	eAGY_CMD_TOP,
	eAGY_CMD_END,
	eAGY_CMD_NNEXT,
	eAGY_CMD_PPREV,
	eAGY_CMD_NEXT_BRANCH,
	eAGY_CMD_PREV_BRANCH,
	eAGY_CMD_NEXT_COMMENT,
	eAGY_CMD_PREV_COMMENT,
	eAGY_CMD_FIND_COMMENT_START,
	eAGY_CMD_FIND_COMMENT_END,
	eAGY_CMD_SET_COMMENT,
	eAGY_CMD_START_CALCULATE,
	eAGY_CMD_END_CALCULATE,
	eAGY_CMD_CANCEL_CALCULATE,
	eAGY_CMD_OPEN,
	eAGY_CMD_SAVE_SGF,
	eAGY_CMD_SHOW_NO,
	eAGY_CMD_SET_LABEL,
	eAGY_CMD_INIT_STONE,
	eAGY_CMD_CHANGE_INPUT_MODE,
	eAGY_CMD_SAVE_YGF,
	eAGY_CMD_OPEN_YGF,
	eAGY_CMD_DELETE_BRANCH,
	eAGY_CMD_OPTIMIZE
};

// --------------------------------
typedef struct xsDictionaryComment {
	unsigned short id;
	unsigned short len;
	char* pszComment;
} sDictionaryComment;

class cAppGoYotsuya
{
public:
	cAppGoYotsuya();
	~cAppGoYotsuya(void);

	static cAppGoYotsuya& GetInstance();

	// ---------------------------------
	// Global MutexÇ≈ëΩèdãNìÆÇñhÇÆÇ»Ç«ÅB
	Bool Initialize();

	// ---------------------------------
	void OnCreate(HWND hWnd);

	// ---------------------------------
	void OnDestroy();

	// ---------------------------------
	Bool Command(int id, int param1=0, int param2=0);

	// ---------------------------------
	void OnPutStoned(WPARAM wp,LPARAM lp);

	// ---------------------------------
	void OnPreViewSelected(WPARAM wp,LPARAM lp);

	// ---------------------------------
	void SetMode(eAppMode mode);

	// ---------------------------------
	void OnNodeChanged(cyiTreeNode* node, int wParam=0);

	// ---------------------------------
	void CreateMapData(cyiTreeNode* node);

	// ---------------------------------
	// íËêŒé´ìTÇÃÇ›
	cyiList* LoadCommentDictionary(const char* name);
	int SaveCommentToDictionary(const char* name, cyiList* list);
	int DeleteCommentToDictionary(cyiList* list, int index);
	int CommentToTop(cyiList* list, int index);
	int RegisterCommentToDictionary(cyiList* list, char* pszComment);



	// ---------------------------------
	eAppMode GetMode();

	cglsGoban* m_goban;
	//cyctrlPreviewGoban* m_prev;
	//cglsPreviewGoban* m_prev;
	cglsDlgProgress m_ProgressDlg;

	HWND m_hControlWnd;

private:
	HWND m_hWnd;

	static cAppGoYotsuya* m_me;
	eAppMode m_mode;
	char m_szppName[64];
};
