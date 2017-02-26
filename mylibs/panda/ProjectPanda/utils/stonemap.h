#pragma once
//#include <windows.h>
#include "yiGoDefs.h"


// ----------------------
typedef struct tagMap {
	int map[LINE_DEFAULT+2][LINE_DEFAULT+2];
} sMap;

// ----------------------
#ifdef _WIN32
class cMap : public cyiBase
#else
class cMap 
#endif
{
public:
	cMap(int nLineCnt)
	{
		map = (stone_t**)yialloc(sizeof(int*)*(nLineCnt+2));
		_map = (stone_t**)yialloc(sizeof(int*)*nLineCnt);
		extra = (stone_t**)yialloc(sizeof(int*)*(nLineCnt+2));
		_extra = (stone_t**)yialloc(sizeof(int*)*(nLineCnt));
		area = (stone_t**)yialloc(sizeof(int*)*(nLineCnt+2));
		_area = (stone_t**)yialloc(sizeof(int*)*nLineCnt);

		for (int i=0; i<nLineCnt+2; i++)
		{
			map[i] = (stone_t*)yialloci(sizeof(int)*(nLineCnt+2));
			extra[i] = (stone_t*)yialloci(sizeof(int)*(nLineCnt+2));
			area[i] = (stone_t*)yialloci(sizeof(int)*(nLineCnt+2));
		}

		for (int i=0; i<nLineCnt; i++)
		{
			_map[i] = &map[i+1][1];
			_extra[i]=&extra[i+1][1];
			_area[i]=&area[i+1][1];
		}
	};

	~cMap()
	{
		for (int i=0; i<LINE_DEFAULT+2; i++)
		{
			yifree(map[i]);
			yifree(extra[i]);
			yifree(area[i]);
		}
		yifree(map);
		yifree(_map);
		yifree(extra);
		yifree(_extra);
		yifree(area);
		yifree(_area);
	};

	stone_t** map;
	stone_t** _map;
	stone_t** extra;  // 上位16ビットは、オーダー、残り、あげ石など
	stone_t** _extra;
	stone_t** area;
	stone_t** _area;
};


// ------------------------------------------------------
#ifdef _WIN32
class xEXPORT cStoneMap
#else
class cStoneMap
#endif
{
public:
	// -----------------------------------------------------
	cStoneMap(void);

	// -----------------------------------------------------
	~cStoneMap(void);

	// -----------------------------------------------------
	void Init(eGoLineCount cnt=LINE_DEFAULT);

	// -----------------------------------------------------
	void Clear();


	// -----------------------------------------------------
	//
	//
	// @retval: 取った石の数（-1:石を置くことができない)
	int SetMap(int i, int j,  eStoneType stone, aStoneList stonelist, Bool bisAdded=False);

	// -----------------------------------------------------
	int SetMap(stone_t stone, aStoneList stonelist, Bool bisAdded=False);

	// -----------------------------------------------------
	stone_t GetStoneInfo(int i,int j);

	// -----------------------------------------------------
	int GetBlockId(int i, int j);

	// -----------------------------------------------------
	int GetCatchedStone(Bool bIsStoneWhite);

	// -----------------------------------------------------
	stone_t GetKou();

	// -----------------------------------------------------
	void Export(sMap copyMap, int until);

	// -----------------------------------------------------
	stone_t** GetMap(Bool bCreate=False);

	// -----------------------------------------------------
	stone_t** GetMapExtra();

	// -----------------------------------------------------
	ubit8_t* ExportBitz();

	// -----------------------------------------------------
	int dumyGetAreamap(int i, int j);

	// -----------------------------------------------------
	Bool IsDead(int i, int j);

	// -----------------------------------------------------
	void Copy(cStoneMap& map);

	// -----------------------------------------------------
	stone_t** StartCalculate();

	// ---------------------------------------------------
	stone_t** EndCalculate(int* nBlackArea, int* nWhiteArea, int* nBlackDeadStone, int* nWhiteDeadStone);


	// -----------------------------------------------------
	stone_t** SetDeadStone(stone_t stone, int* nBlackDeadStone, int* nWhiteDeadStone);

private: // private functions

	// -----------------------------------------------------
	stone_t** UpdateAreaMap(int* nBlackDeadStone, int* nWhiteDeadStone, int* nBlackArea=0, int* nWhiteArea=0);

	// -----------------------------------------------------
	short GenerateBlockId();

	// -----------------------------------------------------
	int ReplaceBlockId(short beforeId, short afterId);

	// -----------------------------------------------------
	int CreateBlock(int i, int j);


	// -----------------------------------------------------
	int xGetBlockMapCnt(int id);

	// -----------------------------------------------------
	void RemoveBlockId();

	// -----------------------------------------------------
	Bool IsBlockSurrounded(long nBlockId);

	// -----------------------------------------------------
	int RemoveBlock(long blockId, aStoneList list);

	// -----------------------------------------------------
	int CheckCatchedStone(int i, int j, eStoneType type, int blockId, aStoneList list);

	// -----------------------------------------------------
	stone_t IsAtari(int i, int j);

	// -----------------------------------------------------
	void MakeBlankGroup();

	void  ChangeAraeMap(int before, int after);

	// -----------------------------------------------------
	// 特定の石が囲まれている領域番号のリストを取得する。
	int GetSurroundedArea(stone_t stone);

	// -----------------------------------------------------
	// 領域を統合する。
	int IntegrateArea(int id, eStoneType type);

private:

	//sMap m_mapData;
	cMap m_mapData;
	int	 m_blockMap[LINE_DEFAULT+2][LINE_DEFAULT+2];
	int m_nLastIndex;
	Bool  m_bBlockMaskId[MAX_GO_BLOCK_COUNT];
	unsigned int m_AreaMap[LINE_DEFAULT+2][LINE_DEFAULT+2];
	int m_lastblankId;

	int m_nLineCnt;
	int m_nBlockCnt;

	int m_nBlackAgeStone;
	int m_nWhiteAgeStone;
	Bool	m_bKouActive;
	stone_t m_kou;
};
