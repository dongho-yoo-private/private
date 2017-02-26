#ifndef __yiGODEFS_H__
#define __yiGODEFS_H__

#include "yi.h"
#include "yistd.h"


#define JOSEKI_DICTIONARY_MODE 1

// ----------------------------------
enum eStoneType
{
	eST_NONE=0,
	eST_BLACK,
	eST_WHITE,
	eST_WPASS,
	eST_BPASS,
	eST_PASS		= 0xFFFFFFFE,
	eST_EOF			= 0xFFFFFFFE,
	eST_INVALID		= 0xFFFFFFFF
	//STONE_INVALID=0x0000007F
};

// --------------------------------
enum eInputMode
{
	eIM_ENJOY,
	eIM_ENJOY_NODE_TOP,
	eIM_PLAY,
	eIM_EDIT,
	eIM_EDIT_ADDSTONE,
	eIM_INITSTONE_MODE,
	eIM_FIND_COMMENT,
	eIM_INVALID,
	eIM_CALCULATE,
	eIM_CALCULATE_END,
	eIM_LABEL,
	eIM_DELETE,
	eIM_LOCK,
	eIM_INPUT_LOCK,
	eIM_STUDY,
	eIM_WATCH,
	eIM_WAIT_USER,
	eIM_PREV=-2,
	eIM_NOCHANGE=-1
};

// --------------------------------
enum eMouseMode {
	eMM_INIT_STONE = 0x80,
	eMM_RBUTTON_WHITE = 0x01,
	eMM_RBUTTON_RETURN = 0x03,
	eMM_USE_CENTER_RETURN = 0x08,
	eMM_MOUSE_LOCK = 0x10,
	eMM_DEFAULT = eMM_RBUTTON_RETURN,
	eMM_NOCHANGE = -1
};

enum eBranchViewMode {
	eBVM_NORMAL=0,
	eBVM_HIDE=1,
	eBVM_SHOW_ONE_NODE
};

enum eGoContentsMode {
	eGCM_NORMAL,
	eGCM_PLAY,
	eGCM_REFERENCE,
	eGCM_REFERENCE2,
	eGCM_PREVIEW,
	eGCM_JOSEKI_EDITOR,
	eGCM_JOSEKI_PLAYER
};

// ----------------------------------------
enum eGoFileType {
	eGFT_SGF,
	eGFT_YGF,
	eGFT_SIMPLE_YGF,
	eGFT_BIN
};

// ------------------------------------
typedef struct xsGobanContentsDetail {
	int nLineCount;
	eBranchViewMode eBranchViewModeValue;
	eInputMode eInputModeValue;
	eMouseMode eMouseModeValue;
	eGoContentsMode eContentsMode;
	eCharacterSet eCharSet;
	eGoFileType eFormat;
}sGobanContentsDetail;


// ----------------------------------------
enum eLabelType {
	eLT_USER,
	eLT_ROMA_LARGE,
	eLT_ROMA_SMALL,
	eLT_NUMERIC,
	eLT_SHAPE,
	eLT_LABLE_CLOSE,

	// sub type (shape code)
	eLS_SQURE=0xFE01,
	eLS_CIRCLE=0xFE02,
	eLS_TRIANGLE=0xFE03,
	eLS_MA		=0xFE04,
	eLS_GOOD	=0xFEF1,
	eLS_BAD		=0xFEF2,
	eLS_NOGOOD	=0xFEF3,
	eLS_CAREFULL=0xFEF4,
	eLS_INFO	=0xFEF5,
	eLS_GOOD2	=0xFEF6,
	eLS_CASE	=0xFEF7
};

enum eStoneValue {
	eSV_NORMAL=0, // ���ʂ̎�i����Ȏ�j
	eSV_HONTE, // �� ��
	eSV_NOGOOD, // ���@���F
	eSV_BAD,  // ���� �ԁ~
	eSV_GOOD, // �D�� ��
	eSV_QUESTION, // �^���
	eSV_TRICK, // !�� �� �͂߂�
	eSV_DEFICULT, // ?�� ���
	eSV_SENTE, // >�� ��
	eSV_TESUZI, // �� ��
	eSV_KYOSYU, // ���� �i��؍��j
	eSV_SIMPLE,  // �Ȗ���
	eSV_KYUSYO, // �}��
	eSV_CASE, // �ꍇ�̎�
	eSV_KATACHI, // �`
	eSV_MYOSYU, // ����
	eSV_ABSOLUTE, // ���
	eSV_BADSUJI, // ����
	eSV_SICHO, // �V�`���E���ǂ��Ƃ�
	eSV_NOSICHO,
	eSV_KOU, // �R�E�U�C�������Ƃ�
	eSV_NOKOU,
	eSV_BLACK_KIKI,
	eSV_WHITE_KIKI
};

// ----------------------------------------
//JX[hl]
enum eJosekiNameHigh {
	eJNH_KOMOKU=1,
	eJNH_MOKUHAZUSHI,
	eJNH_TAKAMOKU,
	eJNH_HOSHI,
	eJNH_SANSAN
};

// ----------------------------------------
//JX[hl]
enum eJosekiNameLow {
	eJNM_KOKEIMA=1, // ���j�n������
	eJNM_IKENTAKA, // ��ԍ�������
	eJNM_NIKENTAKA, // ��ԍ�������
	eJNM_OKEIMA, // ��j�n������
	eJNM_TAKA, // ��������
	eJNM_SUMI, // ���݂�����
	eJNM_ETC // ���̑�
};

// ----------------------------------------
//JD[no]
enum eJosekiNameDetail
{
	eJND_IKEN_BASHAMI,
	eJND_NIKEN_BASHAMI,
	eJND_IKEN_TAKABASHAMI,
	eJND_NIKEN_TAKABASHAMI,
	eJND_SANKEN_BASHAMI,
	eJND_SANKEN_TAKABASHAMI,
	eJND_KOSUMI,
	eJND_SHITATSUKE,
	eJND_UETSUKE,
	eJND_IKENTOBI,
	eJND_KOKEIMA,
	eJND_OGEIMA,
	eJND_RYOUGAKARI,
	eJND_ETC,
};

// 0: �s��
// 1: �݊p�̕ʂ�
// 2: ���D��
// 3: ���D��
// 4: �����ǂ�
// 5: �����ǂ�
// 6: ���Ԃ�
// 7: ���Ԃ�
enum eGoResultStatus {
	eGRS_UNKNOWN=0,
	eGRS_SAME,
	eGS_BLACK_GOOD=(0x80|2),
	eGS_WHITE_GOOD=(0x40|2),
	eGS_BLACK_LITTLE_GOOD=(0x80|3),
	eGS_WHITE_LITTLE_GOOD=(0x40|3),
	eGS_BLACK_TEREBLE=(0x80|4),
	eGS_WHITE_TEREBLE=(0x40|4),
	eGRS_BLACK_OTHER=(0x80|5), // �󋵎���
	eGRS_WHITE_OTHER=(0x40|5)  // �󋵎���
};

enum eGoResultOption {
	eGRO_HAMETE=0x01,
	eGRO_NERAI=0x02,
	eGRO_SENTE=0x04,
	eGRO_EDIT_COMPRETE=0x08, // for joseki dictionary only
	eGRO_JOSEKI=0x10, // ���
	eGRO_OKIGO = 0x20,
	eGRO_SICHOU = 0x40, // �V�`���E����
	eGRO_FAVORATE = 0x80,
	eGRO_PLAYED=0x0100, // �Đ��ςݒ��
	eGRO_JOSEKI_OTHER=0x0200, // ��Εω�
	eGRO_JOSEKI_AFTER=0x0400,
	eGRO_JOSEKI_TENUKI=0x0800, // �蔲�����
	eGRO_JOSEKI_CASE=0x1000, // �ꍇ�̎�
	eGRO_JOSEKI_KATEGORI=(eGRO_JOSEKI|eGRO_HAMETE|eGRO_JOSEKI_AFTER|eGRO_JOSEKI_TENUKI|eGRO_JOSEKI_CASE)


};


// ----------------------------------------
enum eCommentCode {
	eCC_GOOD=0, // �D��
	eCC_LITTLE_GOOD,
	eCC_BROKEN,
	eCC_GOOD_FOR_INFURENCE,
	eCC_GOOD_FOR_AREA,
	eCC_GOOD_FOR_KIKI,

	eCC_GOKAKU=0x8000, // �݊p�̕ʂ�
	eCC_GOOD_ATARI,
	eCC_PASS
};



#define MAX_GO_BLOCK_COUNT 361


typedef unsigned int stone_t;

typedef stone_t aStoneList[MAX_GO_BLOCK_COUNT];

// ----------------------------------
// �O���[�o����ID�ŁA�����I�ȊǗ��Ŏg����B
// 
typedef unsigned int goid_t;

class ugoid_t {
public:
	ugoid_t(unsigned int bId=0, unsigned int wId=0)
	{
		b_id=bId;
		w_id=wId;
		time=cyiTime::GetLocalTime(day);
	}

	ugoid_t(ugoid_t& uid)
	{
		b_id=uid.b_id;
		w_id=uid.w_id;
		time=uid.time;
		day=uid.day;
	}

	Bool operator == (const ugoid_t& id)
	{
		if (b_id==id.b_id &&
				w_id==id.w_id &&
					time==id.time &&
						day==id.day)
		{
			return True;
		}
		return False;
	}

	ugoid_t& operator = (ugoid_t& id)
	{
		b_id=id.b_id;
		w_id=id.w_id;
		day=id.day;
		time=id.time;
		return *this;
	}

	void set(ugoid_t& id)
	{
		b_id=id.b_id;
		w_id=id.w_id;
		day=id.day;
		time=id.time;
	}
	

	unsigned int b_id;
	unsigned int w_id;
	unsigned int day;
	unsigned int time;
	//time64_t time;
};



#define MAKESTONE(type, order, x, y) (((type)<<28)|((order)<<16) | ((y)<<8) | (x))

#define MAKELABEL(type, code, x, y) (((STONE_LABEL)<<28)|((type)<<24) |((code)<<16) | ((y)<<8) | (x))


#define STONE_POS_X(s)	(0x000000FF&s)
#define STONE_POS_Y(s)	((0x0000FF00&s)>>8)
#define STONE_ORDER(s)	((0x0FFF0000&s)>>16)
#define STONE_TYPE(s)	((eStoneType)(s>>28))
#define STONE_EOF		0xFFFFFFFE
#define STONE_TOP		0

typedef unsigned int label_t;

#define LABEL_POS_X(s)	(0x000000FF&s)
#define LABEL_POS_Y(s)	((0x0000FF00&s)>>8)
#define LABEL_CODE(s)	((0x00FFFF00&s)>>16)
#define LABEL_TYPE(s)	((0x0F000000&s)>>24)

enum eGoLineCount
{
	LINE_19 = 19,
	LINE_13 = 13,
	LINE_9 = 9,
	LINE_DEFAULT = LINE_19
};



// �΋Ǐ���
enum ePlayGeneralCondition {
	ePGC_IAM_BLACK = 1<<31,
	ePGC_HANDICAP_ON = 1<<30,
	ePGC_HALF_SCORE	 = 1<<17,
	ePGC_REVERSE_KOMI = 1<<16,
	
};

#define SET_BIT_ON(val, n) (val |=(1<<n))
#define SET_BIT_OFF(val, n) (val &=~(1<<n))

#define SET_BITz_ON(val, mask, n) (val =((val&(~((mask)<<(n)))) | ((mask)<<(n))))
#define SET_BITz_OFF(val, mask, n) (val &=~((mask)<<(n)))


#define SET_IAM_BLACK(con) (con = (con | 0x80000000))
#define SET_IAM_WHITE(con) (con = (con & 0x7FFFFFFF))
#define IAM_BLACK(con) ((con&0x80000000)!=0)

#define SET_HANDICAP(con) (con=(con | ePGC_HANDICAP_ON))
#define SET_NIGIRI(con)   (con=(con&(~ePGC_HANDICAP_ON)))
#define IS_NIGIRI(con) ((con&ePGC_HANDICAP_ON)==0)

#define SET_INIT_STONE_CLR(con) (con = (con&(~(0x1F<<25))))
#define SET_INIT_STONE(con, n) (con=(con | (n<<25)))
#define GET_INIT_STONE(con) ((con>>25)&0x0000001F)

#define KOMI_CLR(con) (con = con & (~(0xFF<<17)))
#define SET_KOMI(con, n) (con=(con | (n<<18)))
#define GET_KOMI(con) ((con>>18)&0x7F)

#define SET_HALF_SCORE(con) (con=(con | (1<<17)))
#define HALF_SCORE(con) ((con&(1<<17))!=0)

#define REVERSE_KOMI_CLR(con) (con=con&(~ePGC_REVERSE_KOMI))
#define SET_REVERSE_KOMI(con) (con=(con | ePGC_REVERSE_KOMI))
#define REVERSE_KOMI(con) ((con&ePGC_REVERSE_KOMI)!=0)

// 8�r�b�g, 16�r�b�g�ڂ���i127 -> 127*5��-> 20����
#define SET_PLAY_TIME(con, n) (con=(con| ((((n/5)&0x7F)|0x80)<<8)))
#define GET_PLAY_TIME(con) (((con>>8)&0x7F)*5)
#define IS_LIMITED_PLAY_TIME(con) (((con>>8)&0x80)!=0)

// 30�b�P�� (3�r�b�g�F8*30=240�b)
#define SET_SECOND_READ2(con, n) (con=(con|(((n/30)&0x07)<<5)))
#define GET_SECOND_READ2(con) (((con>>5)&0x07)*30)

// 10�b�P��(5�r�b�g : 32*10=320�b)
#define SET_SECOND_READ(con, n) (con=(con|(n&0x1F)/10))
#define GET_SECOND_READ(con) ((con&0x1F)*10)


// �b�ǂ݉� (6�r�b�g�F50��)
#define SET_SECOND_READ_COUNT(con2, n) (con2=(con2 | ((0x3F&n)<<11)))
#define GET_SECOND_READ_COUNT(con2) ((con2>>11)&0x3F)



// �҂����i10��܂Łj
#define SET_WAITABLE_COUNT(con2, n) (con2=(con2|(n&0x0f)))
#define GET_WAITABLE(con2) (con2&0x0F)

// ������
#define SET_HUU_COUNT(con2, n) (con2=(con2|(n<<4)))
#define GET_HUU_COUNT(con2) ((con2>>4)&0x0F)

// �ꎞ���f
#define SET_PLAYPAUSE(con2) (con2=(con2|0x0100))
#define OFF_PLAYPAUSE(con2) (con2=(con2&(~(0x0100))))
#define IS_PLAYPAUSE(con2) (((con2>>8)&0x01)!=0)

// ��Ȃ��胂�[�h
#define SET_TENAORI(con2, n) (con2=(con2|n<<9))
#define IS_TENAORI(con2) ((con2>>9)&0x03)


//
enum ePlayExtenedCondition {
	ePEC_WAITABLE	= 1<<31, // �҂�������
	ePEC_SLOW_GO	= 1<<30, // ��������
	ePEC_STOPABLE	= 1<<29, // �����Ă���
	ePEC_STOPABLE_EX = 1<<28 // �����ȕ�����

};


#define SET_BLACK_WIN(flag) (flag=(0x3FFFFFFF&flag)|(1<<30))//SET_BITz_ON(flag, 0x10, 30)
#define SET_WHITE_WIN(flag) (flag=(0x3FFFFFFF&flag)|(2<<30))//SET_BITz_ON(flag, 0x20, 30)
#define SET_DRAW(flag) (flag=(0x3FFFFFFF&flag)|(3<<30))//SET_BITz_ON(flag, 0x30, 30)
#define SET_CANCELED(flag) (flag=(0x3FFFFFFF&flag))//SET_BITz_ON(flag, 0x00, 30)

#define GET_WINER(flag) ((flag>>30)&0x03)

#define SET_LINE_COUNT(flag, n) (flag=((((n-4)&0xF)<<4)|(flag&(~0xf0))))
#define GET_LINE_COUNT(flag) (4+((flag&0xF0)>>4))

#define SET_USER_INIT_STONE(flag) SET_BIT_ON(flag, 3)
#define SET_USER_INIT_STONE_OFF(flag) SET_BIT_OFF(flag, 3)

#define GET_USER_INIT_STONE(flag) (flag>>3)

enum eGoResult {
	eGR_BLACK_WIN=0x01,
	eGR_WHITE_WIN=0x02,
	eGR_DRAW=0x03,
	eGR_CANCELED=0x00
};
#define GET_RESULT(flag) (flag>>31)

// score is 29-22.
#define SET_SCORE(flag, n) (flag=((~(0xFF<<22)&flag)|(((n&0xFF)<<22))))//SET_BITz_ON(flag, n, 22)
#define GET_SCORE(flag)		((flag>>22)&0xFF)



// black level is 20-15
// white level is 14-9
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

#define SET_BLACK_AMA(flag) SET_BIT_ON(flag, 20)
#define SET_BLACK_PRO(flag) SET_BIT_OFF(flag, 20)
#define SET_BLACK_LEVEL_OFF(flag) (flag&=(~(0x1F<<15)))
#define SET_BLACK_LEVEL(flag, n) (flag|=(n<<15))//SET_BITz_ON(flag, n, 15)
#define GET_BLACK_LEVEL(flag) ((flag>>15)&(0x3F))

#define SET_WHITE_PRO(flag) SET_BIT_OFF(flag, 14)
#define SET_WHITE_AMA(flag) SET_BIT_ON(flag, 14)
#define SET_WHITE_LEVEL_OFF(flag) (flag&=(~(0x1F<<9)))
#define SET_WHITE_LEVEL(flag, n) (flag|=(n<<9))
#define GET_WHITE_LEVEL(flag) ((flag>>9)&(0x3F))

#define IS_AMATURE(level) ((level&0x20)!=0)
#define GET_LEVEL_ONLY(level) (level&0x1F)



// ----------------------------------------
typedef struct xsGoHeader {
	char* pszEventName;
	char* pszBlackName;
	char* pszWhiteName;
	char* pszPlace;
	char* pszComment;
	char* pszBTeam;
	char* pszWTeam;
	char* pszWhiteLevel;
	char* pszBlackLevel;
	char* pszRo;
	unsigned int condition;
	unsigned int condition2;
	unsigned int flag32; /*10:black win, 01:white win, 11: draw, 00:canceled or inavlied, 
						 29-22:score(FF is giveup), 20-15: black level, 13-8: while level, 7-4: Line Count 3:User InitStone*/
	unsigned int remainTime;
	unsigned int remainSec;
	unsigned int playDate;
	cyiList* pre_bstone;
	cyiList* pre_wstone;
} sGoHeader;

#define MAX_GROUP_NAME_LENGTH (32+1)

// -----------------------------------------
// �O���[�v���
// id: -1 -> ��ʑ΋ǎ�
// id: 0 -> ����
typedef struct {
	unsigned int id;
	wchar_t name[MAX_GROUP_NAME_LENGTH];
	cyiList* serverList;
}sGoYotsuyaGroup;


typedef struct {
	unsigned int id;
	unsigned int address;
	wchar_t name[16];
}sGoYotsuyaUser;

typedef struct {
	wchar_t szName[16];
	wchar_t szComment[32+1];
	int id;
	unsigned int condition1;
	unsigned int condition2;
} sGoYotsuyaGoRules;

typedef unsigned char map_t[91];

typedef struct 
{
	unsigned int goid[4];
	unsigned int id;
	char szFileName[128];
	char szBlackName[32];
	char szWhiteName[32];
	unsigned int condition;
	unsigned int condition2;
	unsigned int flag;
	unsigned int day;
	map_t		map;
	unsigned char reserved[1];
	unsigned int last_order;
} sSimpleYgfHeader;

#pragma pack(push, 1)
// ---------------------------------
typedef struct {
	unsigned int 	remoteId;
	unsigned int	day;
	unsigned int	time;
	unsigned int	result; 	// 31: Iam black. 
								// 30-29: 00 unknown 10 black win, 01 white win, 11 draw.
								// 28-20: last order. (size is order<<1)
								// 18: is half score.
								// 17-10: score.
								// 0-4: SimpleGoDataVersion
	// char 		szUserId[16];
} sSimpleGoHeader; // 32bytes. 

#define NGFX 0x0505
#define NGFX2 0x1515

// ---------------------------------
typedef struct {
	sSimpleGoHeader hd;
	unsigned int	condition;
	unsigned int	condition2;
	unsigned int	flag32;
	unsigned short	code;
	char			szPlace[30];
	unsigned short	stones[0];
} sSimpleGoDataV1;

// ---------------------------------
typedef struct {
	sSimpleGoHeader hd;
	unsigned int	condition;
	unsigned int	condition2;
	unsigned int	flag32;
	unsigned short	code;
	char			szEventName[30];
	char			szBlackName[16];
	char			szWhiteName[16];
	unsigned int	reserved[4];
	unsigned short	stones[0];
} sSimpleGoDataV2;

#define sSimpleGoData sSimpleGoDataV2

typedef struct {
	unsigned int blackId;
	unsigned int whiteId;
	unsigned int day;
	unsigned int time;
	unsigned int data[4];
} sBCastHeader;

#define BCAST_SIZE(n) (16+n);

// ---------------------------------
typedef struct {
	sSimpleGoHeader hd;
	unsigned int	condition;
	unsigned int	condition2;
	unsigned int	flag32;
	unsigned short	code;
	char			szPlace[30];
	unsigned short	stones[0];
} sSimpleGoDataFile;
#pragma pack(pop)

// 
enum eMailGoFormat {
	eMGF_REQUEST = 0xF3F3,	// condition, condition2, flags32
	eMGF_PLAY	 = 0xE5E5,	// ugoid + ngf format.
	eMGF_WATCH	 = 0x0303,  // ugoid + ngf format.
	eMGF_MSG	 = 0x1515   // zipped sgf format. 
};

// ---------------------------------
typedef struct {
	unsigned int id;	// eMailGoFormat.
	unsigned int size;	// data size.
} sMailGoFormat;

#define GO_QUESTION_1 L"���̎�͂ǂ��ł����H"
#define GO_QUESTION_2 L"���̏�ʂłǂ������ǂ��ł����H"
#define GO_QUESTION_3 L"���͂ǂ��łׂ��ł����H"

#define GO_ANSER_1_1 L"�����������ł��B"
#define GO_ANSER_1_2 L"���肾�Ǝv���܂����B"
#define GO_ANSER_1_3 L"���ʂ��ȁH"
#define GO_ANSER_1_4 L"���������������ł��B"
#define GO_ANSER_1_5 L"�������܂����ȂƎv���܂����B"

#define GO_ANSER_2_1 L"�����ǂ��Ǝv���܂��B"
#define GO_ANSER_2_2 L"�����ǂ��Ǝv���܂��B"
#define GO_ANSER_2_3 L"���������ǂ����ȁH"
#define GO_ANSER_2_4 L"���������ǂ����ȁH"
#define GO_ANSER_2_5 L"������܂���ł����B"

#define GO_ANSER_3_1 L"������܂���B"
#define GO_ANSER_3_2 L"�������炶��ł��悤���Ȃ��ł��B"
#define GO_ANSER_3_3 L"�����ł����ˁH"

	//	btn->AddItem(1, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"�Ԏ�", L"�����ǂ��Ǝv���܂��B");
	//	btn->AddItem(2, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"�Ԏ�", L"�����ǂ��Ǝv���܂��B");
	//	btn->AddItem(3, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"�Ԏ�", L"�����ȁc�H");
	//	btn->AddItem(4, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"�Ԏ�", L"�����ȁc�H");
	//	btn->AddItem(5, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"�Ԏ�", L"�݊p���ȁH");
	//	btn->AddItem(6, 0/*(Bitmap*)eGRID_SHOW_NO*/, L"�Ԏ�", L"�悭������Ȃ��ł��B");

#define GO_ANSWER_1

#endif
