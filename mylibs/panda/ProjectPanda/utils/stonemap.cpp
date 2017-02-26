#include "StoneMap.h"


#define TEMPORARY_ID	361
#define DELETE_ID		362
#define INVALID_ID		363


cStoneMap::cStoneMap(void)
: m_mapData(19)
{
}

cStoneMap::~cStoneMap(void)
{
}


 // -----------------------------------------------------
void cStoneMap::Init(eGoLineCount cnt)
{
	m_nLastIndex = 0;

	m_nLineCnt	= cnt;
	m_nBlockCnt	= cnt*cnt;

	Clear();
}

 // -----------------------------------------------------
void cStoneMap::Clear()
{
    for (int i = 0; i < m_nBlockCnt; i++)
    {
        m_bBlockMaskId[i] = False;
    }

   for (int i = 0; i < m_nLineCnt+2; i++)
    for (int j = 0; j < m_nLineCnt+2; j++)
	{
		m_mapData.map[i][j] = eST_INVALID;
		m_mapData.area[i][j] = 0;
		m_blockMap[i][j]=-1;
	}

    for (int i = 1; i < m_nLineCnt+1; i++)
    {
        for (int j = 1; j < m_nLineCnt+1; j++)
        {
            m_mapData.map[i][j] = eST_NONE;
			m_mapData.area[i][j] = 0;
            m_blockMap[i][j] = -1;
        }
    }

	m_bKouActive=False;

	m_nBlackAgeStone=0;
	m_nWhiteAgeStone=0;
	m_nLastIndex = 0;
}

// -----------------------------------------------------
int cStoneMap::SetMap(stone_t stone, aStoneList stonelist, Bool bIsAdded)
{
	return SetMap(STONE_POS_X(stone), STONE_POS_Y(stone), STONE_TYPE(stone), stonelist, bIsAdded);
}

// -----------------------------------------------------
int cStoneMap::SetMap(int i, int j,  eStoneType stone, aStoneList stonelist, Bool bIsAdded)
{
	int _i=i+1;
	int _j=j+1;

	if ( i >= m_nLineCnt ||
			j >= m_nLineCnt)
	{
		return -1;
	}

	if (m_bKouActive==True && STONE_POS_X(m_kou)==i && STONE_POS_Y(m_kou)==j)
	{
		return -1;
	}

	m_bKouActive=False;

	if (stone == eST_BPASS ||
			stone == eST_WPASS)
	{
		m_nLastIndex++;
		return 0;
	}

    if (m_mapData.map[_i][_j] != eST_NONE)
    {
        return -1;
    }

    m_mapData.map[_i][_j]=(char)stone;

    int nNewBlockId=CreateBlock(_i, _j);

    int catchedStone = CheckCatchedStone(_i, _j,stone,  nNewBlockId, stonelist);

    if (catchedStone == 0)
    {
        long nBlockId = m_blockMap[_i][_j];

        // 石をいれてはいけないケース。
        if (IsBlockSurrounded(nBlockId) == True)
        {
            m_mapData.map[_i][_j] = eST_NONE;
			if (xGetBlockMapCnt(nBlockId)==1)
	            m_bBlockMaskId[nBlockId] = False;
            m_blockMap[_i][_j] = -1;


            return -1;
        }
    }

	if (catchedStone==1)
	{
		stone_t temp = IsAtari(i, j);

		if (temp!=STONE_EOF)
		{
			m_kou = temp;
			m_bKouActive=True;

		}
	}

	if (stonelist)
	{
		stonelist[catchedStone] = STONE_EOF;
	}

	if (bIsAdded==False)
		m_mapData.extra[_i][_j] = ++m_nLastIndex;
	else
		m_mapData.extra[_i][_j] = 0;

    return catchedStone;
}

// -----------------------------------------------------
stone_t cStoneMap::GetStoneInfo(int i,int j)
{
	return MAKESTONE(m_mapData.map[i+1][j+1], m_mapData.extra[i+1][j+1]&0xFFFF, i, j);
}

// -----------------------------------------------------
int cStoneMap::GetBlockId(int i, int j)
{
	return m_blockMap[i+1][j+1];
}


// -----------------------------------------------------
stone_t cStoneMap::GetKou()
{
	if (m_bKouActive==True)
			return m_kou;

	return STONE_EOF;
}

// -----------------------------------------------------
int cStoneMap::GetCatchedStone(Bool bIsStoneWhite)
{
	return bIsStoneWhite ? this->m_nWhiteAgeStone:this->m_nBlackAgeStone;
}

// -----------------------------------------------------
void cStoneMap::Export(sMap copyMap, int until)
{
    //for (int i = 1; i < m_nLineCnt; i++)
    //{
    //    for (int j = 1; j < m_nLineCnt; j++)
    //    {
    //        copyMap.map[i][j] = mapData.map[i][j];
    //    }
    //}
}

// -----------------------------------------------------
stone_t** cStoneMap::GetMap(Bool bCreate)
{
	if (bCreate==False)
		return m_mapData._map;

	char** stone = (char**)yialloc(sizeof(stone_t*)*m_nLineCnt);

	for (int i=0; i<m_nLineCnt; i++)
	{
		stone[i]=(char*)yialloc(sizeof(stone_t)*m_nLineCnt);
		for (int j=0; j<m_nLineCnt; j++)
		{
			stone[i][j]=(char)m_mapData._map[i][j];
		}
	}

	return (stone_t**)stone;
}

// -----------------------------------------------------
ubit8_t* cStoneMap::ExportBitz()
{
	ubit8_t* map = (ubit8_t*)yialloci(sizeof(stonemap_t));

	int nBytePointer=0;//sizeof(stonemap_t)-1;
	int nBitPointer=6;

	for (int i=1; i<this->m_nLineCnt+1; i++)
	{
		for (int j=1; j<m_nLineCnt+1; j++)
		{
			ubit8_t n = m_mapData.map[i][j] & 0xFF;
			map[nBytePointer]|=(n<<nBitPointer);
			nBitPointer-=2;

			if (nBitPointer<0)
			{
				nBitPointer=6;
				nBytePointer++;
			}
		}
	}
	return map;
}

// -----------------------------------------------------
stone_t** cStoneMap::GetMapExtra()
{
	return m_mapData._extra;
}

// -----------------------------------------------------
Bool cStoneMap::IsDead(int i, int j)
{
	return m_mapData.area[i][j]?TRUE:FALSE;
}

// private

// -----------------------------------------------------
short cStoneMap::GenerateBlockId()
{
    for (int i = 0; i < m_nBlockCnt; i++)
    {
        if (m_bBlockMaskId[i] == False)
        {
            m_bBlockMaskId[i] = True;
            return (short)i;
        }
    }
    
    // no case
    return -1;
}


// -----------------------------------------------------
int cStoneMap::ReplaceBlockId(short beforeId, short afterId)
{
    Bool bReplaced = False;
    int cnt = 0;

    for (int i = 1; i < m_nLineCnt+1; i++)
    {
        for (int j = 1; j < m_nLineCnt+1; j++)
        {
            if (m_blockMap[i][j] == beforeId)
            {
                m_blockMap[i][j] = afterId;
                bReplaced = True;
                cnt++;
            }
        }
    }

    if (afterId == m_nBlockCnt)
    {
        //return cnt;

		if (bReplaced==True)
		{
			m_bBlockMaskId[beforeId]=False;
		}

		return afterId;
    }

    if (bReplaced==True&&beforeId<m_nBlockCnt)
    {
        m_bBlockMaskId[beforeId]=False;
    }

    return afterId;//cnt;
}

int cStoneMap::xGetBlockMapCnt(int id)
{
	int cnt=0;

	if (id==-1)
		return 0;

	for (int i=0; i<m_nLineCnt+1; i++)
		for (int j=0; j<m_nLineCnt+1; j++)
			if (m_blockMap[i][j]==id) cnt++;

	return cnt;
}

// -----------------------------------------------------
int cStoneMap::CreateBlock(int i, int j)
{
    if (m_mapData.map[i][j] == eST_NONE)
    {
        return 0;
    }

    if ((m_mapData.map[i][j] == m_mapData.map[i -1][j]))
    {
        int nBlockCount = ReplaceBlockId(m_blockMap[i -1][j], TEMPORARY_ID);
        m_blockMap[i -1][j]=m_blockMap[i][j]=TEMPORARY_ID;
    }

    if ((m_mapData.map[i][j] == m_mapData.map[i +1][j]))
    {
        int nBlockCount = ReplaceBlockId(m_blockMap[i +1][j], TEMPORARY_ID);
        m_blockMap[i +1][j]=m_blockMap[i][j]=TEMPORARY_ID;
    }

    if ((m_mapData.map[i][j] == m_mapData.map[i][ j-1]))
    {
        int nBlockCount = ReplaceBlockId(m_blockMap[i][ j - 1], TEMPORARY_ID);
        m_blockMap[i][j-1]=m_blockMap[i][j]=TEMPORARY_ID;
    }

    if ((m_mapData.map[i][j] == m_mapData.map[i][j+1]))
    {
        int nBlockCount = ReplaceBlockId(m_blockMap[i][j + 1], TEMPORARY_ID);
        m_blockMap[i][j+1]=m_blockMap[i][j]=TEMPORARY_ID;
    }

    if (m_blockMap[i][j]==TEMPORARY_ID)
    {
        return ReplaceBlockId(TEMPORARY_ID, GenerateBlockId());
    }


    // dependent block
    m_blockMap[i][j] = GenerateBlockId();

    return m_blockMap[i][j];
}

// -----------------------------------------------------
 Bool cStoneMap::IsBlockSurrounded(long nBlockId)
{
	if (nBlockId==-1)
		return False;

    for (int i = 1; i < m_nLineCnt+1; i++)
    {
        for (int j = 1; j < m_nLineCnt+1; j++)
        {
            if (m_blockMap[i][j] == nBlockId)
            {
                if (m_mapData.map[i + 1][ j] == eST_NONE)
                {
                    return False;
                }
                if (m_mapData.map[i - 1][ j] == eST_NONE)
                {
                    return False;
                }
                if (m_mapData.map[i][ j + 1] == eST_NONE)
                {
                    return False;
                }
                if (m_mapData.map[i][ j-1] == eST_NONE)
                {
                    return False;
                }
           }

        }
    }

    return True;
}


 
// -----------------------------------------------------
void cStoneMap::RemoveBlockId()
{
    for (int i = 1; i < m_nLineCnt+1; i++)
    {
        for (int j = 1; j < m_nLineCnt+1; j++)
        {
            if (m_blockMap[i][j] == (long)DELETE_ID)
            {
                m_blockMap[i][j] = -1;
                
            }
        }
    }
                
}


// -----------------------------------------------------
int cStoneMap::RemoveBlock(long blockId, aStoneList list)
{

    int cnt = 0;

    for (int i = 1; i < m_nLineCnt+1; i++)
    {
        for (int j = 1; j < m_nLineCnt+1; j++)
        {
            if (m_blockMap[i][j] == blockId)
            {
				if (list)
					list[cnt]=MAKESTONE(eST_NONE, m_mapData.extra[i][j]&0xFFFF, i, j);

                m_mapData.map[i][j] = eST_NONE;
                m_blockMap[i][j] = DELETE_ID;
                m_bBlockMaskId[blockId] = False;

                cnt++;
            }
        }
    }

	if (list)
		list[cnt]=STONE_EOF;
    return cnt;
}



// -----------------------------------------------------
int cStoneMap::CheckCatchedStone(int i, int j, eStoneType type, int blockId, aStoneList list)
{
    int nAgeStone = 0;
	int block;

     // pon nuki check.
	block=m_blockMap[i + 1][ j];
    if (block!=blockId && IsBlockSurrounded(block) == True)
    {
//		if (type==m_mapData.map[i+1][j])
//			return -2;
		nAgeStone += RemoveBlock(block, list ? &list[nAgeStone]:0);
    }

    // pon nuki check.
	block=m_blockMap[i-1][ j];
    if (block!=blockId && IsBlockSurrounded(block) == True)
    {
//		if (type==m_mapData.map[i+1][j])
//			return -2;
        nAgeStone += RemoveBlock(block, list ? &list[nAgeStone]:0);
    }

    // pon nuki check.
	block=m_blockMap[i][j+1];
    if (block!=blockId && IsBlockSurrounded(block) == True)
    {
//		if (type==m_mapData.map[i+1][j])
//			return -2;
        nAgeStone += RemoveBlock(block,list ? &list[nAgeStone]:0);
    }

    // pon nuki check.
	block=m_blockMap[i][j-1];
    if (block!=blockId && IsBlockSurrounded(block) == True)
    {
		//if (type==m_mapData.map[i+1][j])
		//	return -2;
        nAgeStone += RemoveBlock(block, list ? &list[nAgeStone]:0);
    }

	if (nAgeStone)
	{
		RemoveBlockId();
	   
	}

	if (m_mapData.map[i+1][j+1]==eST_BLACK)
	{
		m_nWhiteAgeStone+=nAgeStone;
	}
	else if (m_mapData.map[i+1][j+1]==eST_WHITE)
	{
		m_nBlackAgeStone+=nAgeStone;
	}

	return nAgeStone;
}

// -----------------------------------------------------
stone_t cStoneMap::IsAtari(int i, int j)
{
	long _i=i+1, _j=j+1;
	long stone = m_mapData.map[_i][_j];
	long reverseStone = stone == eST_BLACK ? eST_WHITE:eST_BLACK;
	long surround_cnt=0;
	long none_cnt=0; 
	stone_t ret = STONE_EOF;

	if (m_mapData.map[_i+1][_j]==eST_NONE)
	{
		none_cnt++;
		ret = MAKESTONE(eST_NONE, 0, i, j);
	}

	if (m_mapData.map[_i][_j+1]==eST_NONE)
	{
		none_cnt++;
		ret = MAKESTONE(eST_NONE, 0, i, j+1);
	}
	
	if (m_mapData.map[_i-1][_j]==eST_NONE)
	{
		none_cnt++;
		ret = MAKESTONE(eST_NONE, 0, i-1, j);
	}
	
	if (m_mapData.map[_i][_j-1]==eST_NONE)
	{
		none_cnt++;
		ret = MAKESTONE(eST_NONE, 0, i, j-1);
	}

	if (none_cnt!=1)
	{
		return STONE_EOF;
	}

	if (m_mapData.map[_i+1][_j]==reverseStone ||
			m_mapData.map[_i+1][_j]==eST_INVALID)
	{
		surround_cnt++;
	}

	if (m_mapData.map[_i][_j-1]==reverseStone ||
			m_mapData.map[_i][_j-1]==eST_INVALID)
	{
		surround_cnt++;
	}

	if (m_mapData.map[_i-1][_j]==reverseStone ||
			m_mapData.map[_i-1][_j]==eST_INVALID)
	{
		surround_cnt++;
	}

	if (m_mapData.map[_i][_j+1]==reverseStone ||
		m_mapData.map[_i][_j+1]==eST_INVALID)
	{
		surround_cnt++;
	}

	if (surround_cnt==3)
	{
		return ret;
	}

	return STONE_EOF;

}

// -----------------------------------------------------
int cStoneMap::dumyGetAreamap(int i, int j)
{
	return m_mapData.area[i+1][j+1];
}

#define xLEFT(a, x, y)	a[x-1][y]
#define xRIGHT(a, x, y) a[x+1][y]
#define xUP(a, x, y)	a[x][y-1]
#define xDOWN(a, x, y)	a[x][y+1]

// -----------------------------------------------------
int cStoneMap::GetSurroundedArea(stone_t stone)
{
	int _i = STONE_POS_X(stone)+1;
	int _j = STONE_POS_Y(stone)+1;
	int type = STONE_TYPE(stone);//==eST_WHITE)?eST_BLACK:eST_WHITE;
	int nBlockId = this->m_blockMap[_i][_j];

	if (type==eST_NONE ||
			type==eST_INVALID)
		return 0;

	// まず、周りを探す。
	if ((xLEFT(m_mapData.area, _i, _j))!=0 &&
		(xLEFT(m_mapData.area, _i, _j)>>8)==0)
		return xLEFT(m_mapData.area, _i, _j);
	if ((xRIGHT(m_mapData.area, _i, _j))!=0 &&
		(xRIGHT(m_mapData.area, _i, _j)>>8)==0)
		return xRIGHT(m_mapData.area, _i, _j);
	if ((xUP(m_mapData.area, _i, _j))!=0 &&
		(xUP(m_mapData.area, _i, _j)>>8)==0)
		return xUP(m_mapData.area, _i, _j);
	if ((xDOWN(m_mapData.area, _i, _j))!=0 &&
		(xDOWN(m_mapData.area, _i, _j)>>8)==0)
		return xDOWN(m_mapData.area, _i, _j);

	// なければ同じブロックの周りを探す。
	for (int i=1; i<m_nLineCnt-1; i++)
	{
		for (int j=1; j<m_nLineCnt-1; j++)
		{
			if (m_blockMap[i][j]==nBlockId)
			{
				if ((xLEFT(m_mapData.area, i, j))!=0 &&
						(xLEFT(m_mapData.area, i, j)>>8)==0)
					return xLEFT(m_mapData.area, i, j)&0xFF;

				if ((xRIGHT(m_mapData.area, i, j))!=0 &&
						(xRIGHT(m_mapData.area, i, j)>>8)==0)
					return xRIGHT(m_mapData.area, i, j)&0xFF;

				if ((xUP(m_mapData.area, i, j))!=0 &&
						(xUP(m_mapData.area, i, j)>>8)==0)
					return xUP(m_mapData.area, i, j)&0xFF;

				if ((xDOWN(m_mapData.area, i, j))!=0 &&
						(xDOWN(m_mapData.area, i, j)>>8)==0)
					return xDOWN(m_mapData.area, i, j)&0xFF;
			}
		}
	}
	// 見つからなかった場合
	return 0;
}



// -----------------------------------------------------
int cStoneMap::IntegrateArea(int id, eStoneType type)
{
	bool id_flags[256] = {false, };

	memset(id_flags, 0, sizeof(id_flags));

	for (int i=1; i<m_nLineCnt+1; i++)
	{
		for (int j=1; j<m_nLineCnt+1; j++)
		{
			// idが一致したら、周りに隣接する別のidがあるかチェックする。
			if ((m_mapData.area[i][j]&0xFF)==id)
			{
				int xId=xLEFT(m_mapData.area, i, j)&0xFF;
				if (xId!=0 && xId!=id)
				{
					id_flags[xId]=true;
				}
				xId=xRIGHT(m_mapData.area, i, j)&0xFF;
				if (xId!=0 && xId!=id)
				{
					id_flags[xId]=true;
				}
				xId=xUP(m_mapData.area, i, j)&0xFF;
				if (xId!=0 && xId!=id)
				{
					id_flags[xId]=true;
				}
				xId=xDOWN(m_mapData.area, i, j)&0xFF;
				if (xId!=0 && xId!=id)
				{
					id_flags[xId]=true;
				}
			}// if 
		}// for i
	}// for j

	// 隣接しているIDを統合する。
	int cnt=0;
	for (int k = 0; k<256; k++)
	{
		if (id_flags[k]==false)
			continue;

		cnt++;
		for (int i=1; i<m_nLineCnt+1; i++)
		{
			for (int j=1; j<m_nLineCnt+1; j++)
			{
				if ((m_mapData.area[i][j]&0xFF) == k)
				{
					m_mapData.area[i][j]=(k<<24)|(id&0xFF);
				}
			}
		}
	}

	// 領域を書き換える。
	for (int i=1; i<m_nLineCnt+1; i++)
	{
		for (int j=1; j<m_nLineCnt+1; j++)
		{
			if ((m_mapData.area[i][j]&0xFF)==id)
			{
				m_mapData.area[i][j]&=0xFF000000;
				m_mapData.area[i][j]=m_mapData.area[i][j] | (type<<8 | id);
			}
		}
	}

	return cnt;
}

// -----------------------------------------------------
stone_t** cStoneMap::SetDeadStone(stone_t stone, int* nBlackDeadStone, int* nWhiteDeadStone)
{
	int _i = STONE_POS_X(stone)+1;
	int _j = STONE_POS_Y(stone)+1;
	eStoneType dead_stone_type = STONE_TYPE(stone);//==eST_WHITE?eST_BLACK:eST_WHITE;
	eStoneType area_stone_type = STONE_TYPE(stone)==eST_WHITE?eST_BLACK:eST_WHITE;
	int nBlockId = m_blockMap[_i][_j];
	int blankId=0;
	int nAreaId=0;
	int nIntegrated=0;


	// 1. もし、指定された石が既に領域化されているのならば、
	// 2. 石があり、領域のIDがマッチする領域であれば、復旧する。

	// 領域がゼロでない場合
	// 即ち、石がおいてない場合
	// 元に戻す。
	if (m_mapData.area[_i][_j]!=0)
	{
		int selectedId=m_mapData.area[_i][_j] & 0x000000FF;
		
		for (int i=1; i<m_nLineCnt+1; i++)
		{
			for (int j=1; j<m_nLineCnt+1;j++)
			{
				// 領域のIDがあり、なおかつ石がおいてある場合のみ
				if (selectedId==(m_mapData.area[i][j]&0x000000FF) &&
						m_mapData.map[i][j]!=eST_NONE)
				{
					// 領域をゼロにもどす。
					// 上げ石の取り消し
					m_mapData.area[i][j]=0;
				}

				// 統合された領域があるかも知れないので、
				// 領域を元に戻す。
				if ((m_mapData.area[i][j]&0xFF)==selectedId)
				{
					int n = m_mapData.area[i][j]>>24;
					if (n!=0)
					{
						m_mapData.area[i][j]=n;
					}
				}
			}
		}

		return UpdateAreaMap(nBlackDeadStone, nWhiteDeadStone);
	}


	// 1. まず、関係のある領域IDを取得する。
	nAreaId = GetSurroundedArea(stone);

	// 2. もしIDがなければ、選択された石の領域を新たない領域IDにする。
	if (nAreaId==0)
	{
		nAreaId = m_mapData.area[_i][_j]=m_lastblankId++;
	}

SEARCH_DEAD_STONE:
	// 3. 領域IDで検索し、IDの周りの同じ色の石を領域化する。
	// 4. 領域化されないまで繰り返す。

	while(1)
	{
		Bool bIsChanged=False;
		for (int i=1; i<m_nLineCnt+1; i++)
		{
			for (int j=1; j<m_nLineCnt+1;j++)
			{
				if ((m_mapData.area[i][j]&0xFF)==nAreaId)
				{
					if (xLEFT(m_mapData.area, i,j)==0 &&
							xLEFT(m_mapData.map, i, j)==dead_stone_type)
					{
						xLEFT(m_mapData.area, i,j)=nAreaId;
						bIsChanged=True;
					}
					if (xRIGHT(m_mapData.area, i,j)==0 &&
							xRIGHT(m_mapData.map, i, j)==dead_stone_type)
					{
						xRIGHT(m_mapData.area, i,j)=nAreaId;
						bIsChanged=True;
					}
					if (xUP(m_mapData.area, i,j)==0 &&
							xUP(m_mapData.map, i, j)==dead_stone_type)
					{
						xUP(m_mapData.area, i,j)=nAreaId;
						bIsChanged=True;
					}
					if (xDOWN(m_mapData.area, i,j)==0 &&
							xDOWN(m_mapData.map, i, j)==dead_stone_type)
					{
						xDOWN(m_mapData.area, i,j)=nAreaId;
						bIsChanged=True;
					}

				}// if
			}// for j
		} // for i
		if (bIsChanged==False)
			break;
	} // while

	// 6. 隣接する領域IDがあれば統合する。
	// 7. 領域に色を付ける。
	nIntegrated = IntegrateArea(nAreaId, area_stone_type);

	if (nIntegrated==0)
	{
		return UpdateAreaMap(nBlackDeadStone, nWhiteDeadStone);
	}
	
	goto SEARCH_DEAD_STONE;

	return 0;
}


// -----------------------------
stone_t** cStoneMap::StartCalculate()
{
	int id=1;

	// Areaマップを初期化
	for (int i=0; i<m_nLineCnt+2; i++)
	{
		for (int j=0; j<m_nLineCnt+2; j++)
		{
				m_mapData.area[i][j]=0;//(m_mapData.map[i][j]<<24);
		}
	}


	for (int i=1; i<m_nLineCnt+1; i++)
	{
		for (int j=1; j<m_nLineCnt+1; j++)
		{
			// ある点に対して、石がおかれていない場合、
			// 周りの点のidを適応する。
			// もし、周りの点もidがなければ新たなidをつける。
			// 石が置かれているところのidはゼロである。
			if (m_mapData.map[i][j]==eST_NONE)
			{
				if (m_mapData.area[i-1][j] != 0)
				{
					m_mapData.area[i][j]|=(0xFF&m_mapData.area[i-1][j]);
					continue;
				}
				if (m_mapData.area[i][j-1] != 0)
				{
					m_mapData.area[i][j]|=(0xFF&m_mapData.area[i][j-1]);
					continue;
				}
				if (m_mapData.area[i+1][j] != 0)
				{
					m_mapData.area[i][j]|=(0xFF&m_mapData.area[i+1][j]);
					continue;
				}
				if (m_mapData.area[i][j+1] != 0)
				{
					m_mapData.area[i][j]|=(0xFF&m_mapData.area[i][j+1]);
					continue;
				}
				m_mapData.area[i][j]=id++;
			}
		}// for j
	}// for i

	m_lastblankId=id;

	int _id;


	// つながっているにもかかわらず、idが別のところがあるので、
	// 統合する。
	for (_id=1; _id<id; _id++)
	{
		for (int i=1; i<m_nLineCnt+1; i++)
		{
			BOOL bIsChanged=FALSE;
			for (int j=1; j<m_nLineCnt+1; j++)
			{
				
				bIsChanged=FALSE;
				if (m_mapData.area[i][j]==_id)
				{
					if (m_mapData.area[i+1][j]!=0 &&
						m_mapData.area[i+1][j]!=m_mapData.area[i][j])
					{
						bIsChanged=TRUE;
						ChangeAraeMap(m_mapData.area[i][j],m_mapData.area[i+1][j]);
						break;
					}
					if (m_mapData.area[i][j+1]!=0 &&
						m_mapData.area[i][j+1]!=m_mapData.area[i][j])
					{
						bIsChanged=TRUE;
						ChangeAraeMap(m_mapData.area[i][j],m_mapData.area[i][j+1]);
						break;
					}
					if (m_mapData.area[i-1][j]!=0 &&
						m_mapData.area[i-1][j]!=m_mapData.area[i][j])
					{
						bIsChanged=TRUE;
						ChangeAraeMap(m_mapData.area[i][j],m_mapData.area[i-1][j]);
						break;
					}
					if (m_mapData.area[i][j-1]!=0 &&
						m_mapData.area[i][j-1]!=m_mapData.area[i][j])
					{
						bIsChanged=TRUE;
						ChangeAraeMap(m_mapData.area[i][j],m_mapData.area[i][j-1]);
						break;
					}
				}
			}
			if (bIsChanged==TRUE)
				break;
		}
	}


	// 領域が白なのか黒なのか、確定されていないのかの情報を入れる。
	// m_mapData.areaの下位8ビットは、id
	// m_mapData.areaの上位8ビットは、0(未確定領域), eST_BLACK, eST_WHITEである。
	for (_id=1; _id<id; _id++)
	{
		int flag = -1;
		BOOL bIsUnKnownArea=FALSE;
		for (int i=1; i<m_nLineCnt+1; i++)
		{
			for (int j=1; j<m_nLineCnt+1; j++)
			{
				if ((m_mapData.area[i][j] & 0x000000FF)==_id)
				{
					// right(石が置いてある場合)
					if ((m_mapData.area[i+1][j]&0xFF)==0)
					{
						if (m_mapData.map[i+1][j]!=eST_INVALID )
						{

							if (flag==-1)
							{
								flag = m_mapData.map[i+1][j];
								//continue;
							}
							else
							{
								if (flag!=m_mapData.map[i+1][j])
								{
									bIsUnKnownArea=TRUE;
									break;
								}
							}
						}
					}
					// bottom
					if ((m_mapData.area[i][j+1]&0xFF)==0)
					{
						if (m_mapData.map[i][j+1]!=eST_INVALID)
						{

							if (flag==-1)
							{
								flag = m_mapData.map[i][j+1];
								//continue;
							}
							else
							{
								if (flag!=m_mapData.map[i][j+1])
								{
									bIsUnKnownArea=TRUE;
									break;
								}
							}
						}
					}
					if ((m_mapData.area[i-1][j]&0xFF)==0)
					{
						if (m_mapData.map[i-1][j]!=eST_INVALID)
						{

							if (flag==-1)
							{
								flag = m_mapData.map[i-1][j];
								//continue;
							}
							else
							{
								if (flag!=m_mapData.map[i-1][j])
								{
									bIsUnKnownArea=TRUE;
									break;
								}
							}
						}
					}
					if ((m_mapData.area[i][j-1]&0xFF)==0)
					{
						if (m_mapData.map[i][j-1]!=eST_INVALID)
						{

							if (flag==-1)
							{
								flag = m_mapData.map[i][j-1];
								//continue;
							}
							else
							{
								if (flag!=m_mapData.map[i][j-1])
								{
									bIsUnKnownArea=TRUE;
									break;
								}
							}
						}
					}

				} // if 
			} // for i

			if (bIsUnKnownArea==TRUE)
			{
				break;
			}
		}// for j

		if (bIsUnKnownArea==FALSE)
		{
			for (int i=1; i<m_nLineCnt+1; i++)
			{
				for (int j=1; j<m_nLineCnt+1;j++)
				{
					if (m_mapData.area[i][j]==_id)
					{
						// 確定領域
						m_mapData.area[i][j] |= (flag<<8);
					}
				}// for j
			}// for i
		}
	} // for _id

	return m_mapData._area;
}

// ---------------------------------------------------
stone_t** cStoneMap::EndCalculate(int* nBlackArea, int* nWhiteArea, int* nBlackDeadStone, int* nWhiteDeadStone)
{
	if (nBlackDeadStone)
			*(nBlackDeadStone)=0;

	if (nWhiteDeadStone)
			*(nWhiteDeadStone)=0;

	*nBlackArea=0;
	*nWhiteArea=0;

	// この段階で、死石指定が終わっているので、
	// 領域マップの白・黒石の情報は一旦初期化する。
	// そうすることにより、領域マップには、上げ石は取り上げられた状態である。
	for (int i=1; i<m_nLineCnt+1; i++)
	{
		for (int j=1; j<m_nLineCnt+1;j++)
		{
			if (m_mapData.area[i][j]!=0)
			{
				// 指定されたあげ石を取得
				if (((m_mapData.area[i][j]>>8)&0xFF)==eST_BLACK)
				{
					if (nBlackArea)
							(*nBlackArea)++;
				}
				else if (((m_mapData.area[i][j]>>8)&0xFF)==eST_WHITE)
				{
					if (nWhiteArea)
							(*nWhiteArea)++;
				}
			}
			//m_mapData.area[i][j]&=0x000000FF;
		}
	}
	

	if (nBlackDeadStone &&
			nWhiteDeadStone)
	{
		for (int i=1; i<m_nLineCnt+1; i++)
		{
			for (int j=1; j<m_nLineCnt+1;j++)
			{
				if ((m_mapData.area[i][j]&0xFF)!=0)
				{
					if ((m_mapData.map[i][j]==eST_BLACK))
					{
							if (nBlackDeadStone)
								(*nBlackDeadStone)++;
					}

					if ((m_mapData.map[i][j]==eST_WHITE))
					{
							if (nWhiteDeadStone)
								(*nWhiteDeadStone)++;
					}
				}
			}// for j
		}// for i
	}// if

	return m_mapData._area;


	// idの数分回りながら、白・黒のどちらの領域であるのかをチェックする。
	for (int _id=1; _id<m_lastblankId; _id++)
	{
		int flag = -1;
		BOOL bIsUnKnownArea=FALSE;
		for (int i=1; i<m_nLineCnt+1; i++)
		{
			for (int j=1; j<m_nLineCnt+1; j++)
			{
				if ((m_mapData.area[i][j] & 0x000000FF)==_id)
				{
					// 右の石を調べる。
					// 
					if ((m_mapData.area[i+1][j]&0xFF)==0)
					{
						if (m_mapData.map[i+1][j]!=eST_INVALID)
						{

							if (flag==-1)
							{
								flag = m_mapData.map[i+1][j];
								//continue;
							}
							else
							{
								if (flag!=m_mapData.map[i+1][j])
								{
									bIsUnKnownArea=TRUE;
									break;
								}
							}
						}
					}
					if ((m_mapData.area[i][j+1]&0xFF)==0)
					{
						if (m_mapData.map[i][j+1]!=eST_INVALID)
						{

							if (flag==-1)
							{
								flag = m_mapData.map[i][j+1];
								//continue;
							}
							else
							{
								if (flag!=m_mapData.map[i][j+1])
								{
									bIsUnKnownArea=TRUE;
									break;
								}
							}
						}
					}
					if ((m_mapData.area[i-1][j]&0xFF)==0)
					{
						if (m_mapData.map[i-1][j]!=eST_INVALID)
						{

							if (flag==-1)
							{
								flag = m_mapData.map[i-1][j];
								//continue;
							}
							else
							{
								if (flag!=m_mapData.map[i-1][j])
								{
									bIsUnKnownArea=TRUE;
									break;
								}
							}
						}
					}
					if ((m_mapData.area[i][j-1]&0xFF)==0)
					{
						if (m_mapData.map[i][j-1]!=eST_INVALID)
						{

							if (flag==-1)
							{
								flag = m_mapData.map[i][j-1];
								//continue;
							}
							else
							{
								if (flag!=m_mapData.map[i][j-1])
								{
									bIsUnKnownArea=TRUE;
									break;
								}
							}
						}
					}

				} // if 
			} // for i

			if (bIsUnKnownArea==TRUE)
				break;
		}// for j

		if (bIsUnKnownArea==FALSE)
		{
			for (int i=1; i<m_nLineCnt+1; i++)
			{
				for (int j=1; j<m_nLineCnt+1;j++)
				{
					if ((m_mapData.area[i][j]&0xFF)==_id)
					{
						m_mapData.area[i][j] |= (flag<<16);
					}
				}// for j
			}// for i
		}
	} // for _id

	if (nBlackArea &&
			nWhiteArea)
	{
		*nBlackArea=0;
		*nWhiteArea=0;

		for (int i=1; i<m_nLineCnt+1; i++)
		{
			for (int j=1; j<m_nLineCnt+1;j++)
			{
				stone_t stone = ((m_mapData.area[i][j]&0x0000FF00)>>16);
				if (stone==eST_BLACK)
				{
					(*nBlackArea)++;
					m_mapData.area[i][j]=(0x80000000 | eST_BLACK);
				}
				else if (stone==eST_WHITE)
				{
					m_mapData.area[i][j]=(0x80000000 | eST_WHITE);
					(*nWhiteArea)++;
				}
				m_mapData.area[i][j]=0;
			}// for j
		}// for i
	}

	return m_mapData._area;
	// あげ石がない状態のマップ
	//for (int i=1; i<m_nLineCnt+1; i++)
	//{
	//	for (int j=1; j<m_nLineCnt+1;j++)
	//	{
	//		if (m_mapData.area[i][j]!=0)

	//		{
	//			if (m_mapData.map[i][j]==eST_BLACK)
	//			{
	//				this->m_nWhiteAgeStone++;
	//			}
	//			else if (m_mapData.map[i][j]==eST_WHITE)
	//			{
	//				this->m_nBlackAgeStone++;
	//			}
	//		}
	//	}
	//}
}


void  cStoneMap::ChangeAraeMap(int before, int after)
{
	for (int i=1; i<LINE_DEFAULT+1; i++)
	{
		for (int j=1; j<LINE_DEFAULT+1;j++)
		{
			if (m_mapData.area[i][j]==before)
			{
				m_mapData.area[i][j]=after;
			}
		}
	}
}


// -----------------------------
void cStoneMap::Copy(cStoneMap& map)
{
	map.m_kou = m_kou;
	map.m_bKouActive = m_bKouActive;
	map.m_nWhiteAgeStone = m_nWhiteAgeStone;
	map.m_nBlackAgeStone = m_nBlackAgeStone;
	map.m_nBlockCnt = m_nBlockCnt;
	map.m_nLineCnt = m_nLineCnt;
	map.m_nLastIndex = m_nLastIndex;

	for (int i=0; i<m_nLineCnt+2; i++)
	{
		for (int j=0; j<m_nLineCnt+2; j++)
		{
			map.m_blockMap[i][j]=m_blockMap[i][j];
			map.m_mapData.map[i][j] = m_mapData.map[i][j];
			map.m_mapData.extra[i][j] = m_mapData.extra[i][j];
		}
	}

	for (int i=0; i<m_nLineCnt; i++)
	{
		map.m_mapData._map[i]=&map.m_mapData.map[i+1][1];
		map.m_mapData._extra[i]=&map.m_mapData.extra[i+1][1];
	}

	for (int i=0; i<MAX_GO_BLOCK_COUNT; i++)
		map.m_bBlockMaskId[i] = m_bBlockMaskId[i];

}



// -----------------------------------------------------
stone_t** cStoneMap::UpdateAreaMap(int* nBlackDeadStone, int* nWhiteDeadStone, int*nBlackArea, int*nWhiteArea)
{
	if (nBlackDeadStone)
		*nBlackDeadStone=0;

	if (nWhiteDeadStone)
		*nWhiteDeadStone=0;
	
	if (nBlackArea)
		*nBlackArea=0;

	if (nWhiteArea)
		*nWhiteArea=0;

	for (int i=1; i<m_nLineCnt+1; i++)
	{
		for (int j=1; j<m_nLineCnt+1; j++)
		{
			stone_t temp = (m_mapData.area[i][j]&0xFF);
			if (temp==0)
			{
				//m_mapData.area[i][j]=m_mapData.map[i][j];
				continue;
			}

			int type = (m_mapData.area[i][j]>>8)&0xFF;

			if (nBlackArea && type==eST_BLACK)
				(*nBlackArea)++;

			if (nWhiteArea && type==eST_WHITE)
				(*nWhiteArea)++;

			if (m_mapData.map[i][j]==eST_BLACK)
			{
				//m_mapData.area[i][j]=eST_BLACK_DEAD;
				if (nBlackDeadStone)
					(*nBlackDeadStone)++;
			}
			else if (m_mapData.map[i][j]==eST_WHITE)
			{
				//m_mapData.area[i][j]=eST_WHITE_DEAD;
				if (nWhiteDeadStone)
					(*nWhiteDeadStone)++;

			}
		}
	}

	return m_mapData._area;
}