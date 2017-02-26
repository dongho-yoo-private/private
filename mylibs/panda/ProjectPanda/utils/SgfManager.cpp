#include "SgfManager.h"
#include "yiGoDefs.h"

#include <string.h>
#include "yiStream.h"


static Bool xIsStringNull(const char* p)
{
	if (p==0)
		return True;

	if (*p==0)
		return True;

	while(*p)
	{
		if (*p!='\r' &&
				*p!='\n' &&
					*p!='\t' &&
						*p!=' ')
		{
			return False;
		}

		p++;
	}

	return True;
}

// ----------------------------------------
static void xGetStoneList19(int n, POINT* pos, Bool bIsBlack)
{
	if (n>9)
		n=9;

	if (bIsBlack==False)
	{
		if (n==3)
		{
			pos[0].x=3;
			pos[0].y=3;
			pos[1].x=15;
			pos[1].y=3;
			pos[2].x=3;
			pos[2].y=15;
			return ;
		}
	}


	if (n==2)
	{
		pos[0].x=15;
		pos[0].y=3;
		pos[1].x=3;
		pos[1].y=15;
		return ;
	}
	if (n==3)
	{
		pos[0].x=15;
		pos[0].y=3;
		pos[1].x=15;
		pos[1].y=15;
		pos[2].x=3;
		pos[2].y=15;
		return ;
	}

	if (n==4)
	{
		pos[0].x=3;
		pos[0].y=3;
		pos[1].x=15;
		pos[1].y=15;
		pos[2].x=3;
		pos[2].y=15;
		pos[3].x=15;
		pos[3].y=3;
		return ;
	}

	if (n==5)
	{
		pos[0].x=3;
		pos[0].y=3;
		pos[1].x=15;
		pos[1].y=15;
		pos[2].x=3;
		pos[2].y=15;
		pos[3].x=15;
		pos[3].y=3;
		pos[4].x=9;
		pos[4].y=9;
		return ;
	}

	if (n==6)
	{
		pos[0].x=3;
		pos[0].y=3;
		pos[1].x=15;
		pos[1].y=15;
		pos[2].x=3;
		pos[2].y=15;
		pos[3].x=15;
		pos[3].y=3;
		pos[4].x=3;
		pos[4].y=9;
		pos[5].x=15;
		pos[5].y=9;
		return ;
	}

	if (n==7)
	{
		pos[0].x=3;
		pos[0].y=3;
		pos[1].x=15;
		pos[1].y=15;
		pos[2].x=3;
		pos[2].y=15;
		pos[3].x=15;
		pos[3].y=3;
		pos[4].x=3;
		pos[4].y=9;
		pos[5].x=15;
		pos[5].y=9;
		pos[6].x=9;
		pos[6].y=9;
		return ;
	}

	if (n==8)
	{
		pos[0].x=3;
		pos[0].y=3;
		pos[1].x=15;
		pos[1].y=15;
		pos[2].x=3;
		pos[2].y=15;
		pos[3].x=15;
		pos[3].y=3;
		pos[4].x=3;
		pos[4].y=9;
		pos[5].x=15;
		pos[5].y=9;
		pos[6].x=9;
		pos[6].y=3;
		pos[7].x=9;
		pos[7].y=15;
		return ;
	}
	if (n==9)
	{
		pos[0].x=3;
		pos[0].y=3;
		pos[1].x=15;
		pos[1].y=15;
		pos[2].x=3;
		pos[2].y=15;
		pos[3].x=15;
		pos[3].y=3;
		pos[4].x=3;
		pos[4].y=9;
		pos[5].x=15;
		pos[5].y=9;
		pos[6].x=9;
		pos[6].y=3;
		pos[7].x=9;
		pos[7].y=15;
		pos[8].x=9;
		pos[8].y=9;
		return ;
	}
}

static Bool xIsRangeValid(stone_t stone, int nMaxLine)
{
	int i=STONE_POS_X(stone);
	int j=STONE_POS_Y(stone);

	if (i>nMaxLine-1)
	{
		return False;
	}
	if (j>nMaxLine-1)
	{
		return False;
	}
	return True;
}

cSgfManager::cSgfManager(void)
{
	//m_node;//=NULL;
	m_sgfRaw=NULL;
	m_header=0;
	m_pszRootComment=0;
	m_nTotalNodeCnt=0;
	m_nLinkNo=0;
	m_bIschanged=False;
	m_pNodeTop=&m_node;
	m_szTitle[0]=0;
	m_szFileName[0]=0;
	m_nMaxRefNo=0;
	m_bsNgf=False;
	m_currentNode=0;
	m_bIsRepair=0;
}

cSgfManager::~cSgfManager(void)
{
	Destroy();

	//DeleteNode(&m_node);
	//if (m_header)
	//	yifree(m_header);

	//if (m_sgfRaw)
	//	yifree(m_sgfRaw);

	
}

// -----------------------------------
void cSgfManager::Init(sGoHeader* header/*=0*/, int locale)
{
	m_currentNode = &m_node;
	m_header=header;

	m_Locale=locale;
	m_nTotalNodeCnt=0;
	//m_nLineCnt=19;
	m_bIsLoading=False;
	m_pNodeTop=&m_node;

	//if (header)
	//	m_nLineCnt = GET_LINE_COUNT(header->flag32);

}

// -----------------------------------
static char* xstrcpy(char* p, char* psrc, char end)
{
	//int cnt=15;

	while(1)//*psrc && *psrc!=end && cnt!=0)
	{
		if ((unsigned char)(*psrc)<0x80)
		{
			if (*psrc==end ||
					*psrc==0)
			{
				break;
			}
			*p++=*psrc++;
			continue;
		}
		*p++=*psrc++;
		*p++=*psrc++;
	}
	*p=0;

	return psrc;
}

// -----------------------------------
static int xstrlen(char* p, char end)
{
	int cnt=0;
	//while(*p!=0&&*p!=end) {cnt++;p++;}
	while(1)//*psrc && *psrc!=end && cnt!=0)
	{
		if ((unsigned char)(*p)<0x80)
		{
			if (*p==end ||
					*p==0)
			{
				break;
			}
			p++;
			cnt++;
			continue;
		}
		p++;p++;
		cnt++;cnt++;
	}

	return cnt;
}

// -----------------------------------
static char* xstrtoheap(char* p, char end)
{
	int n = xstrlen(p, end);

	char* ret= (char*)yitoheap(p, n+1);
	ret[n]=(char)0;

	return ret;
}


// -----------------------------------
static char* FindTag(void* sgf, const char* tag, char* value)
{
	char* pIndex;
	if ((pIndex = (char*)strstr((const char*)sgf, tag))==0)
	{
		*value=0;
		return 0;
	}

	int len = strlen(tag);
	pIndex=&pIndex[len];

	return (xstrcpy(value, pIndex, ']')+1);
}

// -----------------------------------
static void xGetLevelString(int level, char* pszLevel, int locale)
{
	level = GET_LEVEL_ONLY(level);
	if (locale==0)
	{
		if (level<10)
		{
			wsprintf(pszLevel, "%dd", 10-level);
		}
		else if (level<28)
		{
			wsprintf(pszLevel, "%dk", level-10+1);
		}
		else if (level==28)
		{
			wsprintf(pszLevel, "セミプロ", level-10+1);
		}
		else if (level==29)
		{
			wsprintf(pszLevel, "先生", level-10+1);
		}
		else
		{
			pszLevel[0]=0;
		}
	}
}


// -----------------------------------
static int xGetLevel(const char* pszLevel)
{
	char* pIndex=(char*)pszLevel;
	Bool bIsDan=False;

	while(*pIndex)
	{
		if (*pIndex=='k' || 
				*pIndex=='K')
		{
			bIsDan=False;
			break;
		}
		else if (*pIndex=='d' ||
					*pIndex=='D')
		{
			bIsDan=True;
			break;
		}

		pIndex++;
	}

	// unknown.
	if (*pIndex==0)
	{
		return -1;
		bIsDan=True;
		pIndex=(char*)&pszLevel[1];
	}

	{
		char temp[16];
		int  n=(int)(pIndex-pszLevel);
		int level;
		strncpy(temp, pszLevel, n);
		temp[n]=0;

		if (yiatoi(temp, &level)==False)
		{
			return -1;
		}

		if (bIsDan)
		{
			return (9-level+1);
		}
		else
		{
			return (9+level);
		}
	}

	return -1;
}

// -----------------------------------
char* xFindBody(const char* p);

// -----------------------------------
Bool cSgfManager::ParseSgfHeader(void* sgf, sGoHeader* header)
{
	IntPtr res=0;
	char	value[16];
	int		nValue;
	float	fValue;

	memset(header, 0, sizeof(*header));

	if (FindTag(sgf, "GM[", value)!=0)
	{
		if (yiatoi(value, &nValue)==False)
		{
			return False;
		}
		if (nValue!=1)
		{
			return False;
		}
	}

	if (FindTag(sgf, "FF[", value)!=0)
	{
		if (yiatoi(value, &nValue)==False)
		{
			return False;
		}

		if (nValue!=4)
		{
			//return False;
		}
	}

	if ((res=(int)FindTag(sgf, "KM[", value))==0)
	{
		res =(int) FindTag(sgf, "KO[", value);
	}

	if (res)
	{
		if (yiatof(value, &fValue)==False)
		{
			SET_KOMI(header->condition, 0);
		}
		else
		{
			int komi=(int)fValue;
			int abskomi = abs(komi);

			if (abskomi>0x7F)
			{
				abskomi=0x7F;
			}

			SET_KOMI(header->condition, abskomi);
			if (komi<0)
			{
				SET_REVERSE_KOMI(header->condition);
			}

			if (fValue-((float)komi)!=0.0)
			{
				SET_HALF_SCORE(header->condition);
			}
		}
	}

	if (FindTag(sgf, "HA[", value)!=0)
	{
		if (yiatof(value, &fValue)==True)
		{
			if (fValue > 0 &&
					fValue < 32)
			{
				SET_HANDICAP(header->condition);
				SET_INIT_STONE(header->condition, (int)fValue);
			}
		}
	}

	{
		char buffer[4096];
		if (FindTag(sgf, "TB[", buffer)!=0)
		{
			//int cnt=0;
			//header->pre_bstone = new cyiList();
		}

		if (FindTag(sgf, "TW[", buffer)!=0)
		{
			//header->pre_wstone = new cyiList();
		}

		if (FindTag(sgf, "GC[", buffer)!=0)
		{
			header->pszComment = yistrheap(buffer);
		}
		else if (FindTag(sgf, "AN[", buffer)!=0)
		{
			header->pszComment = yistrheap(buffer);
		}

		if (FindTag(sgf, "EV[", buffer)!=0)
		{
			header->pszEventName = yistrheap(buffer);
		}
		else if (FindTag(sgf, "TE[", buffer)!=0)
		{
			header->pszEventName = yistrheap(buffer);
		}

		// 回戦
		if (FindTag(sgf, "RO[", buffer)!=0)
		{
			header->pszRo = yistrheap(buffer);
		}

		// 対局者
		if (FindTag(sgf, "PB[", buffer)!=0)
		{
			header->pszBlackName = yistrheap(buffer);
		}

		if (FindTag(sgf, "PW[", buffer)!=0)
		{
			header->pszWhiteName = yistrheap(buffer);
		}

		// 場所
		if (FindTag(sgf, "PC[", buffer)!=0)
		{
			header->pszPlace = yistrheap(buffer);
		}

		header->pszBlackLevel=0;
		header->pszWhiteLevel=0;
		// 段位
		if (FindTag(sgf, "BR[", buffer)!=0)
		{
			int level = xGetLevel(buffer);
			
			if (level!=-1)
				SET_BLACK_LEVEL(header->flag32, level);
			else
				header->pszBlackLevel=yistrheap(buffer);//, ']');
		}
		if (FindTag(sgf, "WR[", buffer)!=0)
		{
			int level = xGetLevel(buffer);
			if (level!=-1)
				SET_WHITE_LEVEL(header->flag32, level);
			else
				header->pszWhiteLevel=yistrheap(buffer);//, ']');
		}

		//if (FindTag(sgf, "xBR[", buffer)!=0)
		//{
		//	int level = 
		//}

		// チーム
		if (FindTag(sgf, "BT[", buffer)!=0)
		{
			header->pszBTeam = yistrheap(buffer);
		}
		if (FindTag(sgf, "WT[", buffer)!=0)
		{
			header->pszWTeam = yistrheap(buffer);
		}

		// 結果 RE[B+2.5]（黒２目半勝ち）
		if (FindTag(sgf, "RE[", buffer)!=0)
		{
			if (buffer[0]=='B')
			{
				SET_BLACK_WIN(header->flag32);

				if (buffer[1]=='+')
				{
					if (buffer[2]=='R')
					{
						SET_SCORE(header->flag32, 0xFF);
					}
					else
					{
						float n;
						if (yiatof(&buffer[2], &n)==True)
						{
							SET_SCORE(header->flag32, (int)n);
						}
					}
				}
			}
			else if (buffer[0]=='W')
			{
				SET_WHITE_WIN(header->flag32);

				if (buffer[1]=='+')
				{
					if (buffer[2]=='R')
					{
						SET_SCORE(header->flag32, 0xFF);
					}
					else
					{
						float n;
						if (yiatof(&buffer[2], &n)==True)
						{
							SET_SCORE(header->flag32, (int)n);
						}
					}
				}
			}
			else if (buffer[0]=='0' || buffer[0]=='D' || buffer[0]=='V') // ジゴ
			{
				SET_DRAW(header->flag32);
			}
			else
			{
				SET_CANCELED(header->flag32);
			}
		}

		// 持ち時間(秒単位）
		if (FindTag(sgf, "TM[", buffer)!=0)
		{
			int sec;
			if (yiatoi(buffer, &sec)==True)
			{
				
				SET_PLAY_TIME(header->condition, (sec/60));
				//header->remainTime=sec;
			}
		}

		//if (FindTag(sgf, "xRM[", buffer)!=0)
		//{
		//	int sec;
		//	if (yiatoi(buffer, &sec)==True)
		//	{
		//		SET_SECOND_READ2(header->condition, sec);
		//		
		//		//SET_PLAY_TIME(header->condition, (n/60));
		//		//header->remainTime=sec;
		//	}
		//}

		//if (FindTag(sgf, "xRC[", buffer)!=0)
		//{
		//	int n;
		//	if (yiatoi(buffer, &n)==True)
		//	{
		//		SET_SECOND_READ_COUNT(header->condition2, n);
		//		
		//		//SET_PLAY_TIME(header->condition, (n/60));
		//		//header->remainTime=sec;
		//	}
		//}

		// 秒読み回数
		if (FindTag(sgf, "xSC[", buffer)!=0)
		{
			int n;
			if (yiatoi(buffer, &n)==True)
			{
				SET_SECOND_READ_COUNT(header->condition2, n);
				//header->remainSec=(n<<24);
			}
		}

		// 秒読みの長さ
		if (FindTag(sgf, "xST[", buffer)!=0)
		{
			int n;
			if (yiatoi(buffer, &n)==True)
			{
				SET_SECOND_READ2(header->condition, n);
				//header->remainSec|=n;
			}
		}

		// 対局日時　DT[2005-09-04]
		if (FindTag(sgf, "DT[", buffer)!=0)
		{
			int year, month, date;

			if (sscanf(buffer, "%d-%d-%d", &year, &month, &date)==3)
			{
				header->playDate = MAKEDAY(year, month, date);//((year<< 12) | (month<<6) | date);
			}
			else if (sscanf(buffer, "%d/%d/%d", &year, &month, &date)==3)
			{
				header->playDate = MAKEDAY(year, month, date);//((year<< 12) | (month<<6) | date);
			}
			else
			{
				// 今日の日付にする？
				header->playDate=0;
			}
		}

		// サイズ
		if (FindTag(sgf, "SZ[", buffer)!=0)
		{
			int n = (int)(buffer[0]-'0')*10+(int)(buffer[1]-'0');

			n = n>19?19:n;
			SET_LINE_COUNT(header->flag32, n);
		}
		else
		{
			SET_LINE_COUNT(header->flag32, 19);
		}

		char *pIndex=(char*)sgf;
		int  nInitStoneCnt=0;
		char* pszBody = xFindBody(pIndex);

		while(1)
		{
			//
			if ((pIndex=FindTag(pIndex, "AB[", buffer))!=0)
			{
				if (pIndex>pszBody)
				{
					break;
				}
				int i = buffer[0]-'a';
				int j = buffer[1]-'a';

				InitStone(MAKESTONE(eST_BLACK, 0, i, j));
				nInitStoneCnt++;
			}
			else
				break;

			while(1)
			{
				if (*pIndex=='[' && pIndex[3]==']')
				{
					int i = pIndex[1]-'a';
					int j = pIndex[2]-'a';

					InitStone(MAKESTONE(eST_BLACK, 0, i, j));
					nInitStoneCnt++;
					pIndex=&pIndex[4];
				}
				else
					break;
			}
		}
		

		SET_INIT_STONE(header->condition, nInitStoneCnt);
	}

	return True;
}

// -----------------------------------
// command
// 1: black B
// 2: white W
// 3: black pass B[]
// 4: white pass W[]
// 5: comment C[]
// 6: Circle CR
// 7: Triangle TR
// 8: Square SQ
// 9: MA 
//10: Label LB[xx:あ]
//11: Start Branch (
//12: End Branch )
//13: AB
//14: AW
//15: CX // Comment Code [no:no]
//16: ST // Status eGoResultOption参照
//17: OX[no] // オプションコード
//21: FX[no] //人気度(1-5)
//22: DE[no] // 難易度(1-5)
//23: MF[no] // マスターフラグ(成功すれば+1)
//24: JX[no,no,no] // 定石名
//25: XR[ref] // レファレンス番号
//26: XC[ref] // レファレンス番号
//27: XP[]
//28: XF[]
//29: XS[ref] // リンク本体
//30: XL[ref] // リンク参照
//31: xREF( // 参考
//32: xSV[
//34: xPD[ // 再生済み　定石辞典でしか使われない。

char* xFindNextCommand(const char* pszBody, int&command)
{
	char* pIndex = (char*)pszBody;

	while(*pIndex)
	{
		switch(*pIndex)
		{
			case '[':
			{
				while(*pIndex!=']' && *pIndex!=0) pIndex++;
			
				break;
			}
			case 'A':
			{
				if (pIndex[1]=='B' && pIndex[2]=='[')
				{
					command=13;
					++pIndex;
					return ++pIndex;
				}
				if (pIndex[1]=='W'&& pIndex[2]=='[')
				{
					command=14;
					++pIndex;
					return ++pIndex;
				}
				break;
			}
			case 'B':
			{
				if (pIndex[-1]=='L' || 
						pIndex[-1]=='A' ||
							pIndex[-1]=='W')
				{
					break;
				}
				if (pIndex[1]!='[')
				{
					break;
				}
				if (pIndex[2]==']')
				{
					command=3;
					return ++pIndex;
				}

				command=1;
				return ++pIndex;
			}
			case 'C':
			{
				if (pIndex[-1]=='X')
				{
					++pIndex;
					break;
				}
				if (pIndex[1]=='X' && pIndex[2]=='[')
				{
					command=15;
					return (pIndex+=2);
				}
				if (pIndex[1]=='[')
				{
					command=5;
					return ++pIndex;
				}
				else if (pIndex[1]=='R' && pIndex[2]=='[')
				{
					command=6;
					return ++pIndex;
				}
				break;
			}
			case 'D':
			{
				if (pIndex[1]=='E' && pIndex[2]=='[')
				{
					command=22;
					return (pIndex+=2);
				}
			}
			case 'F':
			{
				if (pIndex[1]=='X' && pIndex[2]=='[')
				{
					command=21;
					return (pIndex+=2);
				}
				break;
			}
			case 'J':
			{
				if (pIndex[1]=='X' && pIndex[2]=='[')
				{
					command=24;
					return (pIndex+=2);
				}
				break;
			}
			case 'L':
			{
				if (pIndex[1]=='B' && pIndex[2]=='[')
				{
					command=10;
					return ++pIndex;
				}
				break;
			}
			case 'M':
			{
				if (pIndex[1]=='F' &&
						pIndex[2]=='[')
				{
					command=23;
					return (pIndex+=2);
				}
				break;
			}
			case 'O':
			{
				if (pIndex[1]=='X' &&
						pIndex[2]=='[')
				{
					command=17;
					return (pIndex+=2);
				}
				break;
			}
			case 'W':
			{
				if (pIndex[-1]=='L' || 
						pIndex[-1]=='A')
				{
					break;
				}

				if (pIndex[1]!='[')
				{
					if (pIndex[1]=='B' && pIndex[2]=='[') // WB
					{
						command=14;
						++pIndex;
						return ++pIndex;
					}
					break;
				}
				if (pIndex[2]==']')
				{
					command=4;
					return ++pIndex;
				}

				command=2;
				return ++pIndex;
			}

			case 'T':
			{
				if (pIndex[1]=='R' && pIndex[2]=='[')
				{
					command=8;
					return ++pIndex;
				}
				break;
			}
			case 'S':
			{

				if (pIndex[1]=='T' && pIndex[2]=='[')
				{
					command=16;
					return (pIndex+=2);
				}
				if (pIndex[1]=='Q' && pIndex[2]=='[')
				{
					command=7;
					return ++pIndex;
				}
				break;
			}
			case 'X':
			{
				if (pIndex[2]!='[')
					break;

				if (pIndex[1]=='R')
				{
					command=25;
					return (pIndex+=3);
				}
				else if (pIndex[1]=='P' && pIndex[3]==']')
				{
					command=27;
					return (pIndex+=4);
				}
				else if (pIndex[1]=='F'&& pIndex[3]==']')
				{
					command=28;
					return (pIndex+=4);
				}
				else if (pIndex[1]=='C')
				{
					command=26;
					return (pIndex+=3);
				}
				else if (pIndex[1]=='S') // XS
				{
					command=29;
					return (pIndex+=3);
				}
				else if (pIndex[1]=='L') // Xs
				{
					command=30;
					return (pIndex+=3);
				}

				break;
			}
			case 'x': // for yotsuya format
			{
				if (strncmp(&pIndex[1], "REF(", 4)==0)
				{
					command=31;
					return (pIndex+=5);
				}
				else if (strncmp(&pIndex[1], "SV[", 3)==0)
				{
					command=32;
					return (pIndex+=4);
				}
				else if (strncmp(&pIndex[1], "PD[", 3)==0)
				{
					command=34;
					return (pIndex+=4);
				}

				break;
			}
			case '(':
			{
				if (pIndex[1]==';')
					++pIndex;
				command=11;
				return ++pIndex;
			}
			case ')':
			{
				command=12;
				return ++pIndex;
			}
			default:
			{
				break;
			}
		} // switch
		pIndex++;
	} // while

	return 0;
}

// -----------------------------------
char* xFindBody(const char* p)
{
	char* pIndex = (char*)p;

	while(*pIndex)
	{
	
		if (pIndex[0]==';')
		{
			pIndex++;

			while (*pIndex=='\r' || *pIndex=='\n')
			{
				pIndex++;
			}

			// ラベルも含める？
			if ((*pIndex=='B' || *pIndex=='W' || *pIndex=='C') && pIndex[1]=='[')
			{
				return pIndex;
			}
			continue;
		}

		//if (pIndex[0]=='C' && pIndex[1]=='[')
		//{
		//	return pIndex;
		//}

		pIndex++;
	}

	return 0;
}

//#define xConvertCoord(a) a

Bool __xxg_bIsCoord_Rotate=False;

void __xxSetRotateModeEnable()
{
	__xxg_bIsCoord_Rotate=True;
}

#ifndef xConvertCoord
// -----------------------------------
// 臨時的
stone_t xConvertCoord(stone_t stone)
{
	if (__xxg_bIsCoord_Rotate==False)
	{
		return stone;
	}

	int x = STONE_POS_X(stone);
	int y = STONE_POS_Y(stone);
	eStoneType type = STONE_TYPE(stone);

	if (type!=eST_BLACK &&
			type!=eST_WHITE)
	{
		return stone;
	}

	int tempX=18-x;

	int xx = y;
	int yy = tempX;

	xx=18-xx;

	return MAKESTONE(type, 0, xx, yy);
}

#endif

//#define xConvertCoordx(a, b) 

#ifndef xConvertCoordx
// -----------------------------------
// 臨時的
void xConvertCoordx(int& x, int& y)
{
	if (__xxg_bIsCoord_Rotate==False)
	{
		return ;
	}
	int tempX=18-x;

	int xx = y;
	int yy = tempX;
		xx=18-xx;

	x=xx;
	y=yy;
}
#endif

int gTempDupCnt=0;

// -----------------------------------
Bool cSgfManager::IsDuplicatedReferenceNo(int no)
{
	for (cyiNode* node = m_RefList.GetNodeTopFrom(); node; node=node->np)
	{
		if ((int)node->data==no)
		{
			gTempDupCnt++;
			return True;
		}
	}

	return False;
}

// -----------------------------------
Bool cSgfManager::ParseSgf(void* sgf, int nSize)
{
	char* pIndex = (char*)sgf;
	int len = strlen(pIndex);

	pIndex = xFindBody((const char*)sgf);

	if (pIndex==0)
	{
		return False;
	}

	if (pIndex[-1]==';' &&
			pIndex[-2]=='(')
	{
		pIndex=&pIndex[-2];
	}

	// 初期コメントの検索
	{
		char* p = (char*)strstr((const char*)sgf, "C[");
		if (p!=0 &&
				p<pIndex)
		{
			p+=2;
			m_header->pszComment=xstrtoheap(p, ']');
		}
	}


	ToTop();

	int cnt=0;
	int nMaxLine = GET_LINE_COUNT(m_header->flag32);
	cyiStack branchStack;

	if (nMaxLine>19)
	{
		return False;
	}

	branchStack.clear();

	while(1)
	{
		int cmd;
		int nIndex = (pIndex-(char*)sgf);

		if (nIndex>=len)
			break;

		pIndex = xFindNextCommand(pIndex, cmd);

		// EOF
		if (pIndex==0)
		{
			break;
		}

		switch(cmd)
		{
			case 13: // AB
			case 1: // B[]
			{
				Bool bIsCreateNew=True;
				eStoneType type=eST_BLACK;
AB:
				stone_t stone = MAKESTONE(type, 0, (int)(pIndex[1])-'a', (int)(pIndex[2]-'a'));

				//if (stone&0xFFFF==0)
				//{
				//	Sleep(100);
				//}
				if (xIsRangeValid(stone, nMaxLine)==False)
				{
					stone_t stone = MAKESTONE(eST_BPASS, 0, 0, 0);
					Add(xConvertCoord(stone), -1);
					break;
				}

				if (cmd==1 && m_bIsRepair)
				{
					cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();
					if (info!=0 && info->stone!=eST_INVALID)
					{
						eStoneType type = STONE_TYPE(info->stone);

						if (type==eST_BLACK)
						{
							stone_t stone = 0;
							MAKESTONE(eST_WPASS, 0, 0, 0);
							Add(stone, -1, False);
						}
					}
				}


				Add(xConvertCoord(stone), -1, cmd==13?True:False, bIsCreateNew);

				if (cmd==13)
				{
					if (pIndex[4]=='[' && pIndex[7]==']')
					{
						pIndex=&pIndex[4];
						bIsCreateNew=False;
						goto AB;
					}
					else if (pIndex[4]=='A' && pIndex[5]=='B' && pIndex[8]=='[')
					{
						pIndex=&pIndex[6];
						bIsCreateNew=False;
						type=eST_BLACK;
						goto AB;
					}
					else if (pIndex[4]==';' && pIndex[5]=='A' && pIndex[6]=='B' && pIndex[7]=='[')
					{
						pIndex=&pIndex[7];
						bIsCreateNew=False;
						type=eST_BLACK;
						goto AB;
					}
					else if (pIndex[4]=='A' && pIndex[5]=='W' && pIndex[8]=='[')
					{
						pIndex=&pIndex[6];
						bIsCreateNew=False;
						type=eST_WHITE;
						goto AB;
					}
					else if (pIndex[4]==';' && pIndex[5]=='A' && pIndex[6]=='W' && pIndex[7]=='[')
					{
						pIndex=&pIndex[7];
						bIsCreateNew=False;
						type=eST_WHITE;
						goto AB;
					}
				}
				break;
			}
			case 14: // AW
			case 2: // W[]
			{
				Bool bIsCreateNew=True;
				eStoneType type=eST_WHITE;

AW:
				stone_t stone = MAKESTONE(type, 0, (int)(pIndex[1])-'a', (int)(pIndex[2]-'a'));

				if (xIsRangeValid(stone, nMaxLine)==False)
				{
					stone_t stone = MAKESTONE(eST_WPASS, 0, 0, 0);
					Add(xConvertCoord(stone), -1);//, cmd==14?True:False);
					break;
				}

				if (cmd==2 && m_bIsRepair)
				{
					cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();
					if (info!=0 && info->stone!=eST_INVALID)
					{
						eStoneType type = STONE_TYPE(info->stone);

						if (type==eST_WHITE)
						{
							stone_t stone = 0;
							MAKESTONE(eST_BPASS, 0, 0, 0);
							Add(stone, -1, False);
						}
					}
				}

				Add(xConvertCoord(stone), -1, cmd==14?True:False, bIsCreateNew);


				//Add(xConvertCoord(stone), -1);
				if (cmd==14)
				{
					if (pIndex[4]=='[' && pIndex[7]==']')
					{
						pIndex=&pIndex[4];
						bIsCreateNew=False;

						goto AW;
					}
					else if (pIndex[4]=='A' && pIndex[5]=='W' && pIndex[8]=='[')
					{
						pIndex=&pIndex[6];
						bIsCreateNew=False;
						type=eST_WHITE;
						goto AW;
					}
					else if (pIndex[4]==';' && pIndex[5]=='A' && pIndex[6]=='W' && pIndex[7]=='[')
					{
						pIndex=&pIndex[7];
						bIsCreateNew=False;
						type=eST_WHITE;
						goto AW;
					}
					else if (pIndex[4]=='A' && pIndex[5]=='B' && pIndex[8]=='[')
					{
						pIndex=&pIndex[6];
						bIsCreateNew=False;
						type=eST_BLACK;
						goto AW;
					}
					else if (pIndex[4]==';' && pIndex[5]=='A' && pIndex[6]=='B' && pIndex[7]=='[')
					{
						pIndex=&pIndex[7];
						bIsCreateNew=False;
						type=eST_BLACK;

						goto AW;
					}

				}
				break;
			}
			case 5: // C[]
			{
				int len = xstrlen(&pIndex[1], ']');
				char* pszComment = xstrtoheap(&pIndex[1], ']');

				if (xIsStringNull(pszComment)==True)
				{
					break;
				}

				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

				if (info==0)
				{
					if (m_currentNode->GetParent()==0)
					{
						m_pszRootComment = xstrtoheap(&pIndex[1], ']');
						break;
					}

					info = new cSgfStoneInfo();
				}

				info->comment = pszComment;//xstrtoheap(&pIndex[1], ']');

			
				//if (strstr(info->comment, "GOOD VARIATION"))
				//{
				//	if (info->exinfo==0)
				//	{
				//		info->exinfo = (sSgfExtendInfo*)yialloci(sizeof(sSgfExtendInfo));
				//	}

				//	info->exinfo->option|=eGRO_JOSEKI;
				//}

				break;
			}
			case 3:
			{
				stone_t stone = MAKESTONE(eST_BPASS, 0, 0, 0);
				Add(stone, -1);
				break;
			}
			case 4:
			{
				stone_t stone = MAKESTONE(eST_WPASS, 0, 0, 0);
				Add(stone, -1);
				break;
			}
			case 6: // CR
			{
				// TODO:
				int i=pIndex[2]-'a';
				int j=pIndex[3]-'a';
				xConvertCoordx(i, j);
				AddLabel(i, j, eLS_CIRCLE);
				break;
			}
			case 7: // SQ
			{
				// TODO:
				int i=pIndex[2]-'a';
				int j=pIndex[3]-'a';
				xConvertCoordx(i, j);
				AddLabel(i, j, eLS_SQURE);
				break;
			}
			case 8: // TR
			{
				// TODO:
				int i=pIndex[2]-'a';
				int j=pIndex[3]-'a';
				xConvertCoordx(i, j);
				AddLabel(i, j, eLS_TRIANGLE);
				break;
			}
			case 9:
			{
				break;
			}
			case 10: // Label
			{
				wchar_t ch[2]={0, 0};
				char* p = pIndex;
				p++;
				while(*p=='[')
				{
					int i=p[1]-'a';
					int j=p[2]-'a';
					if (xIsRangeValid(MAKESTONE(0, 0, i, j), nMaxLine)==False)
					{
						return False;
					}

					if (p[5]==']')
					{
						if (p[4]>='0' && p[4]<='9')
						{
							ch[0]=MAKEWORD((p[4]-'0'), 0xFF);
						}
						else
						{
							ch[0]=(wchar_t)p[4];
						}
						p=&p[6];
					}
					else
					{
						if (p[4]>='0' && p[4]<='9')
						{
							ch[0]=MAKEWORD(((p[4]-'0')*10+(p[5]-'0')), 0xFF);
						}
						else
						{
							ch[0]=MAKEWORD(p[4], p[5]);
						}
						p=&p[7];
					}
					xConvertCoordx(i, j);
					AddLabel(i, j, ch[0]);
				}
				break;
			}
			case 11: // start branch (
			{
				branchStack.push((yiAnyValue)m_currentNode);
				break;
			}
			case 12: // end branch )
			{
				cyiTreeNode* node=0;
//#ifdef JOSEKI_DICTIONARY_MODE
//				if (m_currentNode->GetChildList().count()==0)
//				{
//					CreateMapImage(m_currentNode);
//				}
//#endif
				if (branchStack.pop((yiAnyValue&)node)==True)
				{
					m_currentNode=node;
				}
				break;
			}

			case 15: // CX
			case 16: // ST
			case 17: // OX
			case 21: // FX
			case 22: // DE
			case 23: // MF
			{
				int n;
				char* p = pIndex;
				char buffer[16];

				p++;
				pIndex=yistrchcpy(buffer, p, ']');

				if (yiatoi(buffer, &n)==False)
				{
					break;
				}
				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

				if (info==0)
					break;

				if (info->exinfo==0)
					info->exinfo = (sSgfExtendInfo*)yialloci(sizeof(sSgfExtendInfo));

				if (cmd==16)
					info->exinfo->status=(unsigned char)n;
				//else if (cmd==15)
				//	info->exinfo->comment_code=(unsigned char)n;
				else if (cmd==17)
					info->exinfo->option=(unsigned short)n;
				else if (cmd==21)
					info->exinfo->flag32|= (n<<28);
				else if (cmd==22)
					info->exinfo->flag32|= (n<<24);
				else if (cmd==23)
					info->exinfo->flag32|= (n<<20);
				break;
			}
			// 定石名
			case 24:
				{
				int h, l, d;
				int n;
				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();
				if (info==0)
					break;
				
				pIndex++;
				n = sscanf(pIndex, "%d,%d,%d", &h,&l,&d);

				if (n!=3)
					break;

				if (info->exinfo==0)
					info->exinfo = (sSgfExtendInfo*)yialloci(sizeof(sSgfExtendInfo));

				SET_JOSEKI_NAME(info->exinfo->flag32, h, l, d);

				break;
			}
			case 25: // XR[]
			{
				char buffer[16];
				int n;
				buffer[0]=0;
				pIndex=yistrchcpy(buffer, pIndex, ']');

				if (yiatoi(buffer, &n)==False)
				{
					break;
				}

				// referenceを作成しない
				if (IsDuplicatedReferenceNo(n)==True)
				{
					break;
				}

				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();
				info->refNo=n;
				m_RefList.add((yiAnyValue)m_currentNode);
				m_nMaxRefNo=m_nMaxRefNo<info->refNo?info->refNo:m_nMaxRefNo;
				break;
			}
			case 26: // XC[]
			{
				char buffer[16];
				int n;
				buffer[0]=0;
				pIndex=yistrchcpy(buffer, pIndex, ']');

				if (yiatoi(buffer, &n)==False)
				{
					break;
				}

				for (cyiNode* node=m_RefList.GetNodeTopFrom(); node; node=node->np)
				{
					cyiTreeNode* t = (cyiTreeNode*)node->data;
					cSgfStoneInfo* info = (cSgfStoneInfo*)t->GetData();

					if (info->refNo==n)
					{
						cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();
						info->ref=t;
						cSgfStoneInfo* info2= (cSgfStoneInfo*)t->GetData();
						if (info2->referenceChild==0)
						{
							info2->referenceChild=new cyiList();
						}
						info2->referenceChild->addref(m_currentNode);
						break;
					}
				}
				break;
			}
			// 再生済み
			case 27:
			{
				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();
				if (info==0)
					break;

				info->flag32|=0x80000000;
				break;
			}
			// お気に入り
			case 28:
			{
				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();
				if (info==0)
					break;

				info->flag32|=0x40000000;
				break;
			}
			case 29: // XS
			{
				char buffer[16];
				int n;

				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();
				if (info==0)
					break;

				buffer[0]=0;
				pIndex=yistrchcpy(buffer, pIndex, ']');

				if (yiatoi(buffer, &n)==False)
				{
					break;
				}
				
				m_nLinkNo = m_nLinkNo > n ? m_nLinkNo:n;

				Bool bIsFound=False;
				void** buff;
				for (cyiNode* node=m_LinkList.GetNodeTopFrom(); node; node=node->np)
				{
					buff= (void**)node->data;

					if (buff==0)
						continue; //?

					if ((int)buff[0]==n)
					{
						bIsFound=True;
						buff[1]=m_currentNode;
						break;
					}
				}

				if (bIsFound==False)
				{
					void* value[2] = {(void*)n, (void*)m_currentNode};
					m_LinkList.add((yiAnyValue)yitoheap(value, sizeof(value)));
				}

				info->linkNo=n;

				break;
			}
			case 30: // XL
			{
				char buffer[16];
				int n;

				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();
				if (info==0)
					break;

				buffer[0]=0;
				pIndex=yistrchcpy(buffer, pIndex, ']');

				if (yiatoi(buffer, &n)==False)
				{
					break;
				}

				if (info->link==0)
				{
					info->link = new cyiList();
				}

				info->link->add((yiAnyValue)n);

				break;
			}
			case 31:
			{
				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();
				if (info==0)
				{
					info = new cSgfStoneInfo();
				}

				int n;
				cyiTreeNode* pRefTree=AddReference(0, n);
				branchStack.push((yiAnyValue)m_currentNode);
				m_currentNode=pRefTree;
					
				break;
			}
			case 32:
			{
				// ありえない
				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();
				int n=0;
				char buffer[16];

				if (info==0)
				{
					break;
				}


				buffer[0]=0;
				pIndex=yistrchcpy(buffer, pIndex, ']');

				if (yiatoi(buffer, &n)==False)
				{
					pIndex++;
					break;
				}

				if (info->exinfo==0)
				{
					info->exinfo = (sSgfExtendInfo*)yialloci(sizeof(sSgfExtendInfo));
				}

				info->exinfo->stonevalue=n;

				break;
			}
			case 34:
			{
				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

				if (info==0)
					break;

				info->flag32|=(1<<19);
				break;
			}
			default:
			{
				// unknown command ignored
				//Sleep(10);
				break;
			}
		} // switch end
	}// while end

	return True;
}


#include "yiFile.h"

// -----------------------------------
Bool cSgfManager::ImportBlock(const void* block, int size, Bool bIsRepair)
{
	m_bIsLoadSuccess=False;
	m_RefList.clear();
	m_bIschanged=False;
	m_bIsLoading=True;
	m_bIsRepair=bIsRepair;
	// check. ngfx

	m_sgfRaw=(void*)block;
	m_blocksize=size;


	// check binary format.
	{
		char* pHeader=(char*)block;
		if (pHeader[0]=='B' &&
				pHeader[1]=='G' &&
					pHeader[2]=='F')
		{
			if (LoadBinaryGoFormat(block, size)==True)
			{
				m_sgfRaw=0;
				return True;
			}
		}
	}

	// check. ngf
	sSimpleGoData* data = (sSimpleGoData*)m_sgfRaw;
	Bool bIsNgf=ImportSimpleGoData(data, m_blocksize);	
	
	if (bIsNgf==True &&
			m_szFileName[0]!=0)
	{
		m_bIsLoading=False;
		m_bIschanged=False;
		m_bIsLoadSuccess=True;

		SET_LINE_COUNT(m_header->flag32, 19);

		//if (m_szFileName[0]!=0)
		//{
		//	strcpy(m_szFileName, pszFileName);
		//	strcat(m_szFileName,".sgf");
		//}
		//m_nLineCnt=19;
		m_sgfRaw=0;

		return True;
	}

	bIsNgf=ImportSimpleGoData((sSimpleGoData*)((unsigned int)m_sgfRaw+4), m_blocksize-4);

	if (bIsNgf==True)
	{	
		m_bIsLoading=False;
		m_bIschanged=False;
		m_bIsLoadSuccess=True;
		//if (m_szFileName[0]!=0)
		//{
		//	strcpy(m_szFileName, pszFileName);
		//	strcat(m_szFileName,".sgf");
		//}
		//m_nLineCnt=19;
		SET_LINE_COUNT(m_header->flag32, 19);
		m_sgfRaw=0;

		return True;
	}

	// it is sgf

	((char*)m_sgfRaw)[m_blocksize]=(char)0;

	m_header = (sGoHeader*)yialloci(sizeof(sGoHeader));
	Bool res = ParseSgfHeader(m_sgfRaw, m_header);

	if (res==False)
	{
		//yiFileUnload(m_sgfRaw);
		m_bIsLoading=False;
		m_sgfRaw=0;

		return False;
	}

	res = ParseSgf(m_sgfRaw, m_blocksize);

	//m_nLineCnt=GET_LINE_COUNT(m_header->flag32);

	if (res==False)
	{
		//yiFileUnload(m_sgfRaw);
		m_bIsLoading=False;
		m_sgfRaw=0;

		return False;
	}
	m_sgfRaw=0;

	m_bIsLoadSuccess=True;
	//if (res==True)
	//{
	//	SetTitle(yiPathGetFileName(pszFileName));
	//}

	ToTop();

	int nJoseki = this->m_ResultList.count();

	m_bIsLoading=False;
	m_bIschanged=False;
	m_sgfRaw=0;

	return res;
}

// -----------------------------------
Bool cSgfManager::LoadBinaryGoFormat(IStream& st, int locale)
{
	char buffer[16];
	void** xHeader=0;
	int* xSize=0;
	m_bIsLoadSuccess=False;
	m_RefList.clear();
	m_bIschanged=False;
	m_bIsLoading=True;

	st.Read(buffer, 4, 0);

	if (memcmp(buffer, "BGF\0", 4)!=0)
	{
		return False;
	}
	

	int nHeaderSize=0;
	st.Read(&nHeaderSize, sizeof(int), 0);

	if (nHeaderSize!=0)
	{
		void*p=yialloc(nHeaderSize);
		st.Read(p, nHeaderSize, 0);

		if (xHeader)
		{
			*xHeader=p;
		}
		if (xSize)
		{
			*xSize=nHeaderSize;
		}
	}
	
	m_header = (sGoHeader*)yialloci(sizeof(sGoHeader));

	st.Read(&m_header->condition, 4, 0);
	st.Read(&m_header->condition2, 4, 0);
	st.Read(&m_header->flag32, 4, 0);


	// ここからはチャンクデータ
	unsigned char chunk=0;

	while(chunk!=0xFF)
	{
		if (st.Read(&chunk, 1, 0)!=S_OK)
		{
			return False;
		}

		// 黒の名前
		if (chunk==0x81)
		{
			st.Read(&chunk, 1, 0);
			if (chunk)
			{
				m_header->pszBlackName = (char*)yialloci(chunk+1);
				st.Read(m_header->pszBlackName, chunk, 0);
			}
			continue;
		}
		// 白の名前
		if (chunk==0x82)
		{
			st.Read(&chunk, 1, 0);
			if (chunk)
			{
				m_header->pszWhiteName = (char*)yialloci(chunk+1);
				st.Read(m_header->pszWhiteName, chunk, 0);
			}
			continue;
		}

		if (chunk==0x83)
		{
			st.Read(&chunk, 1, 0);
			if (chunk)
			{
				m_header->pszEventName = (char*)yialloci(chunk+1);
				st.Read(m_header->pszEventName, chunk, 0);
			}
			continue;
		}

		if (chunk==0x84)
		{
			st.Read(&chunk, 1, 0);
			if (chunk)
			{
				m_header->pszComment = (char*)yialloci(chunk+1);
				st.Read(m_header->pszComment, chunk, 0);
			}
		}
	}

	// ここからボディー

	ToTop();

	int cnt=0;
	int nMaxLine = GET_LINE_COUNT(m_header->flag32);
	cyiStack branchStack;

	if (nMaxLine>19)
	{
		return False;
	}

	branchStack.clear();


	Bool bIsLastStoneBlack=-1;
	while(1)
	{
		int cmd=0;

		if (st.Read(&chunk, 1, 0)!=S_OK)
		{
			// EOF.
			return False;
		}

		if (chunk==0)
		{
			break;
		}

		cmd = chunk&0x3F;

		switch(cmd)
		{
			case 13: // AB
			case 1: // B[]
			{
				Bool bIsCreateNew=True;
				eStoneType type=eST_BLACK;
				unsigned char pos=0;
				int x=0, y=0;
AB:
				st.Read(&pos, 1, 0);

				x=(pos>>4);
				y=pos&0x0F;
				x|=(chunk&0x80)?0x10:0;
				y|=(chunk&0x40)?0x10:0;

				stone_t stone = MAKESTONE(type, 0, x, y);//(int)(pIndex[1])-'a', (int)(pIndex[2]-'a'));

				//if (stone&0xFFFF==0)
				//{
				//	Sleep(100);
				//}
				if (xIsRangeValid(stone, nMaxLine)==False)
				{
					stone_t stone = MAKESTONE(eST_BPASS, 0, 0, 0);
					Add(xConvertCoord(stone), -1);
					break;
				}

				if (bIsLastStoneBlack==1 && m_bIsRepair==True)
				{
					stone_t stone = 0;
					MAKESTONE(eST_BPASS, 0, 0, 0);
					Add(stone, -1, False);
				}

				Add(xConvertCoord(stone), -1, cmd==13?True:False, bIsCreateNew);

				if (cmd==1)
				{
					bIsLastStoneBlack=1;
				}
				break;
			}
			case 14: // AW
			case 2: // W[]
			{
				Bool bIsCreateNew=True;
				eStoneType type=eST_WHITE;
				unsigned char pos=0;
				int x=0, y=0;
AW:
				st.Read(&pos, 1, 0);

				x=(pos>>4);
				y=pos&0x0F;
				x|=(chunk&0x80)?0x10:0;
				y|=(chunk&0x40)?0x10:0;

				stone_t stone = MAKESTONE(type, 0, x, y);//(int)(pIndex[1])-'a', (int)(pIndex[2]-'a'));

				if (xIsRangeValid(stone, nMaxLine)==False)
				{
					stone_t stone = MAKESTONE(eST_WPASS, 0, 0, 0);
					Add(xConvertCoord(stone), -1);//, cmd==14?True:False);
					break;
				}

				if (bIsLastStoneBlack==0 && m_bIsRepair==True)
				{
					stone_t stone = 0;
					MAKESTONE(eST_WPASS, 0, 0, 0);
					Add(stone, -1, False);
				}

				Add(xConvertCoord(stone), -1, cmd==14?True:False, bIsCreateNew);
				
				if (cmd==2)
				{
					bIsLastStoneBlack=0;
				}
				break;
			}
			case 5: // C[]
			{
				uBit16 len;
				st.Read(&len, 2, 0);

				if (len==0)
				{
					break;
				}

				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

				if (info==0)
				{
					if (m_currentNode->GetParent()==0)
					{
						m_pszRootComment = (char*)yialloci(len+1);
						st.Read(m_pszRootComment, len, 0);
						break;
					}

					info = new cSgfStoneInfo();
				}

				info->comment = (char*)yialloci(len+1);
				st.Read(info->comment , len, 0);

				break;
			}
			case 3:
			{
				stone_t stone = MAKESTONE(eST_BPASS, 0, 0, 0);
				Add(stone, -1);
				break;
			}
			case 4:
			{
				stone_t stone = MAKESTONE(eST_WPASS, 0, 0, 0);
				Add(stone, -1);
				break;
			}
			case 6: // CR
			{
				// TODO:
				unsigned char pos=0;
				int i=0, j=0;
				st.Read(&pos, 1, 0);

				i=(pos>>4);
				j=pos&0x0F;
				i|=(chunk&0x80)?0x10:0;
				j|=(chunk&0x40)?0x10:0;

				xConvertCoordx(i, j);
				AddLabel(i, j, eLS_CIRCLE);
				break;
			}
			case 7: // SQ
			{
				// TODO:
				unsigned char pos=0;
				int i=0, j=0;
				st.Read(&pos, 1, 0);

				i=(pos>>4);
				j=pos&0x0F;
				i|=(chunk&0x80)?0x10:0;
				j|=(chunk&0x40)?0x10:0;


				xConvertCoordx(i, j);
				AddLabel(i, j, eLS_SQURE);
				break;
			}
			case 8: // TR
			{
				// TODO:
				unsigned char pos=0;
				int i=0, j=0;
				st.Read(&pos, 1, 0);

				i=(pos>>4);
				j=pos&0x0F;
				i|=(chunk&0x80)?0x10:0;
				j|=(chunk&0x40)?0x10:0;


				xConvertCoordx(i, j);
				AddLabel(i, j, eLS_TRIANGLE);
				break;
			}
			case 9:
			{
				break;
			}
			case 10: // Label
			{
				wchar_t ch[2]={0, 0};
				unsigned char pos=0;
				int i=0, j=0;
				st.Read(&pos, 1, 0);

				i=(pos>>4);
				j=pos&0x0F;
				i|=(chunk&0x80)?0x10:0;
				j|=(chunk&0x40)?0x10:0;


				st.Read(&ch[0], 2, 0);
				xConvertCoordx(i, j);
				AddLabel(i, j, ch[0]);
				break;
			}
			case 11: // start branch (
			{
				branchStack.push((yiAnyValue)m_currentNode);
				break;
			}
			case 12: // end branch )
			{
				cyiTreeNode* node=0;
//#ifdef JOSEKI_DICTIONARY_MODE
//				if (m_currentNode->GetChildList().count()==0)
//				{
//					CreateMapImage(m_currentNode);
//				}
//#endif
				if (branchStack.pop((yiAnyValue&)node)==True)
				{
					m_currentNode=node;
				}
				break;
			}

			case 15: // CX
			case 16: // ST
			case 17: // OX
			case 21: // FX
			case 22: // DE
			case 23: // MF
			{
				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

				if (info==0)
					break;

				if (info->exinfo==0)
					info->exinfo = (sSgfExtendInfo*)yialloci(sizeof(sSgfExtendInfo));

				if (cmd!=17)
				{
					unsigned char n=0;
					st.Read(&n, 1, 0);
					if (cmd==16)
						info->exinfo->status=(unsigned char)n;
					else if (cmd==21)
						info->exinfo->flag32|= (n<<28);
					else if (cmd==22)
						info->exinfo->flag32|= (n<<24);
					else if (cmd==23)
						info->exinfo->flag32|= (n<<20);
				}
				else
				{
					unsigned short n=0;
					st.Read(&n, 2, 0);
					info->exinfo->option=(unsigned short)n;
				}
				break;
			}
			// 定石名
			case 24:
			{
				unsigned char h, l, d;
				int n;
				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();
				if (info==0)
					break;
				
				st.Read(&h, 1, 0);
				st.Read(&l, 1, 0);
				st.Read(&d, 1, 0);

				if (info->exinfo==0)
					info->exinfo = (sSgfExtendInfo*)yialloci(sizeof(sSgfExtendInfo));

				SET_JOSEKI_NAME(info->exinfo->flag32, h, l, d);

				break;
			}
			case 25: // XR[]
			{
				char buffer[16];
				unsigned short n=0;

				st.Read(&n, 2, 0);

				// referenceを作成しない
				if (IsDuplicatedReferenceNo(n)==True)
				{
					break;
				}

				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();
				info->refNo=n;
				m_RefList.add((yiAnyValue)m_currentNode);
				m_nMaxRefNo=m_nMaxRefNo<info->refNo?info->refNo:m_nMaxRefNo;
				break;
			}
			case 26: // XC[]
			{
				char buffer[16];
				unsigned short n=0;

				st.Read(&n, 2, 0);

				for (cyiNode* node=m_RefList.GetNodeTopFrom(); node; node=node->np)
				{
					cyiTreeNode* t = (cyiTreeNode*)node->data;
					cSgfStoneInfo* info = (cSgfStoneInfo*)t->GetData();

					if (info->refNo==n)
					{
						cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();
						info->ref=t;
						cSgfStoneInfo* info2= (cSgfStoneInfo*)t->GetData();
						if (info2->referenceChild==0)
						{
							info2->referenceChild=new cyiList();
						}
						info2->referenceChild->addref(m_currentNode);
						break;
					}
				}
				break;
			}
			// 再生済み
			case 27:
			{
				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();
				if (info==0)
					break;

				info->flag32|=0x80000000;
				break;
			}
			// お気に入り
			case 28:
			{
				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();
				if (info==0)
					break;

				info->flag32|=0x40000000;
				break;
			}
			case 29: // XS
			{
				char buffer[16];
				unsigned short n;

				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();
				if (info==0)
					break;

				st.Read(&n, 2, 0);
				m_nLinkNo = m_nLinkNo > n ? m_nLinkNo:n;

				Bool bIsFound=False;
				void** buff;
				for (cyiNode* node=m_LinkList.GetNodeTopFrom(); node; node=node->np)
				{
					buff= (void**)node->data;

					if (buff==0)
						continue; //?

					if ((int)buff[0]==n)
					{
						bIsFound=True;
						buff[1]=m_currentNode;
						break;
					}
				}

				if (bIsFound==False)
				{
					void* value[2] = {(void*)n, (void*)m_currentNode};
					m_LinkList.add((yiAnyValue)yitoheap(value, sizeof(value)));
				}

				info->linkNo=n;

				break;
			}
			case 30: // XL
			{
				char buffer[16];
				unsigned short n=0;

				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();
				if (info==0)
					break;

				st.Read(&n, 2, 0);

				if (info->link==0)
				{
					info->link = new cyiList();
				}

				info->link->add((yiAnyValue)n);

				break;
			}
			case 31:
			{
				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();
				if (info==0)
				{
					info = new cSgfStoneInfo();
				}

				int n;
				cyiTreeNode* pRefTree=AddReference(0, n);
				branchStack.push((yiAnyValue)m_currentNode);
				m_currentNode=pRefTree;
					
				break;
			}
			case 32:
			{
				// ありえない
				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();
				unsigned char n=0;
				char buffer[16];

				if (info==0)
				{
					break;
				}


				st.Read(&n, 1, 0);

				if (info->exinfo==0)
				{
					info->exinfo = (sSgfExtendInfo*)yialloci(sizeof(sSgfExtendInfo));
				}

				info->exinfo->stonevalue=n;

				break;
			}
			case 34:
			{
				cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

				if (info==0)
					break;

				info->flag32|=(1<<19);
				break;
			}
			default:
			{
				// unknown command ignored
				//Sleep(10);
				break;
			}
		} // switch end

	} // end of while

	return True;

}

// -----------------------------------
Bool cSgfManager::LoadBinaryGoFormat(const void* p, int size, int locale)
{
	cyiMemoryStream st;
	LARGE_INTEGER n;
	st.Write(p, size, 0);
	n.LowPart=0;
	n.HighPart=0;
	st.Seek(n, 0, 0);

	return LoadBinaryGoFormat(st, locale);
}


// -----------------------------------
Bool cSgfManager::LoadBinaryGoFormat(LPCSTR pszFileName, void** xHeader, int* xSize, int locale)
{
	Bool res = False;
	cyiMemoryStream st;

	m_Locale=locale;
	if (m_sgfRaw!=NULL)
	{
		yiFileUnload(m_sgfRaw);
		m_sgfRaw=0;
	}

	if (st.ImportFromFile(pszFileName)==False)
	{
		return False;
	}

	res= LoadBinaryGoFormat(st, locale);

	m_sgfRaw=0;
	yiFileUnload(m_sgfRaw);

	return res;
}


// -----------------------------------
Bool cSgfManager::LoadSgfFile(LPCSTR pszFileName, Bool bIsRepair, int locale)
{
	Bool res;
	m_Locale=locale;
	if (m_sgfRaw!=NULL)
	{
		yiFileUnload(m_sgfRaw);
		m_sgfRaw=0;
	}

	m_bIsLoadSuccess=False;
	m_RefList.clear();
	m_bIschanged=False;
	m_bIsLoading=True;
	m_bIsRepair=bIsRepair;


	m_sgfRaw = yiFileLoad(pszFileName, (int*)&m_blocksize);

	if (m_sgfRaw==0)
	{
		m_bIsLoading=False;
		return False;
	}
	strcpy(m_szFileName, pszFileName);
	res = ImportBlock(m_sgfRaw, m_blocksize);

	yiFileUnload(m_sgfRaw);

	return res;


}

// -----------------------------------
void cSgfManager::UnloadSgfFile(void* data)
{
	yiFileUnload(data);
}

// -----------------------------------
// 
Bool cSgfManager::WriteSgfBody(HANDLE hFile, cyiTreeNode* t, Bool xxxFlag)
{
	char write_buffer[256];
	char* buffer = write_buffer;
	char* write_buffer_ex=0;
	char* pIndex;
	int   nStartPos ;
	int	  nCommentLen=0;
	int	  nChildCnt=0;
	DWORD nWritten=0;
	char cmd[16];
	stone_t stone;

	write_buffer[0]=(char)0;

	if (t==0)
		return False;



	// 分岐があるところまで書き込む。
	cSgfStoneInfo* info = (cSgfStoneInfo* )t->GetData();

	if (t->GetParent()!=0 &&
			info==0)
	{
		return False;
	}

	if (info==0)
		goto SKIP_STONE;

	if (info->comment)
	{
		nCommentLen= strlen(info->comment);
	}

	// ツリーが深くなりすぎてスタックが足りなくなる問題があるので、
	// ヒープ上で処理する。
	write_buffer_ex = (char*)yialloci(nCommentLen+4096);
	buffer=write_buffer_ex;
	
	stone = info->stone;
	{

		if (STONE_TYPE(stone)==eST_BLACK)
		{
			cmd[0]=';';
			if (info->nOrder==0xFFFF)
			{
				cmd[1]='A';
				cmd[2]='B';
			}
			else
			{
				cmd[1]='B';
			}

			//if (STONE_POS_X(stone)==0 && STONE_POS_Y(stone)==0)
			//{
			//	Sleep(10);
			//}
		}
		else if (STONE_TYPE(stone)==eST_WHITE)
		{
			cmd[0]=';';
			if (info->nOrder==0xFFFF)
			{
				cmd[1]='A';
				cmd[2]='W';
			}
			else
			{
				cmd[0]=';';
				cmd[1]='W';
			}
		}
		else if (STONE_TYPE(stone)==eST_BPASS)
		{
			cmd[0]=';';
			cmd[1]='B';
			cmd[2]='[';
			cmd[3]=']';
			cmd[4]=0;
			goto SKIP_SET_POS;
		}
		else if (STONE_TYPE(stone)==eST_WPASS)
		{
			cmd[0]=';';
			cmd[1]='W';
			cmd[2]='[';
			cmd[3]=']';
			cmd[4]=0;
			goto SKIP_SET_POS;
		}
		else
		{
			// ???
			goto JUMP_TO_COMMENT;
		}
	}


	if (info->nOrder!=0xFFFF)
	{
		cmd[2]='[';
		cmd[3]=STONE_POS_X(stone)+'a';
		cmd[4]=STONE_POS_Y(stone)+'a';
		cmd[5]=']';
		cmd[6]=0;
	}
	else
	{
		cmd[3]='[';
		cmd[4]=STONE_POS_X(stone)+'a';
		cmd[5]=STONE_POS_Y(stone)+'a';
		cmd[6]=']';
		cmd[7]=0;
	}
	//if (strcmp(cmd, ";B[aa]")==0)
	//{
	//	Sleep(100);
	//}

SKIP_SET_POS:
	strcpy(buffer, cmd);

JUMP_TO_COMMENT:
	// コメントがあれば
	if (info->comment && info->m_AddStoneList==0)
	{
		char* p = strstr(info->comment, "[");
		while(p)
		{
			if (p)
			{
				*p='<';
			}
			p = strstr(p, "[");
		}

		p = strstr(info->comment, "]");
		while(p)
		{
			if (p)
			{
				*p='>';
			}
			p = strstr(p, "]");
		}

		strcat(buffer, "C[");
		strcat(buffer, info->comment);
		strcat(buffer, "]");
	}

	nStartPos = strlen(buffer);
	pIndex = &buffer[nStartPos];


	// ABもしくはAWがあれば、
	if (info->m_AddStoneList)
	{
		int n = info->m_AddStoneList->count();

		if (n!=0)
		{
			for (cyiNode* node=info->m_AddStoneList->GetNodeTopFrom(0); node; node=node->np)
			{
				char b[16];
				stone_t stone = (stone_t)node->data;
				int x = STONE_POS_X(stone);
				int y = STONE_POS_Y(stone);

				if (STONE_TYPE(stone)==eST_BLACK)
				{
					wsprintf(b, "AB[%c%c]", (char)x+'a', (char)y+'a');
				}
				else
				{
					wsprintf(b, "AW[%c%c]", (char)x+'a', (char)y+'a');
				}
				strcat(buffer, b);
				nStartPos+=6;
			}
		}
	
		// コメントがあれば
		if (info->comment)
		{
			char* p = strstr(info->comment, "[");
			while(p)
			{
				if (p)
				{
					*p='<';
				}
				p = strstr(p, "[");
			}

			p = strstr(info->comment, "]");
			while(p)
			{
				if (p)
				{
					*p='>';
				}
				p = strstr(p, "]");
			}

			strcat(buffer, "C[");
			strcat(buffer, info->comment);

			strcat(buffer, "]");
			nStartPos+=strlen(info->comment)+3;
		}
	}


	pIndex = &buffer[nStartPos];

	// ラベルがあれば
	if (info->label)
	{

		for (cyiNode* node=info->label->GetNodeTopFrom(0); node; node=node->np)
		{
			unsigned int label = (unsigned int )node->ref;
			wchar_t ch = label&0xFFFF;
			int i = label>>24;
			int j = 0xFF&(label>>16);
			Bool bIsSymbol=False;

			if (label==eLS_SQURE)
			{
				*pIndex='S'; pIndex++;
				*pIndex='Q'; pIndex++;
				*pIndex='['; pIndex++;
				bIsSymbol=True;
			}
			else if (label==eLS_CIRCLE)
			{
				*pIndex='C'; pIndex++;
				*pIndex='R'; pIndex++;
				*pIndex='['; pIndex++;
				bIsSymbol=True;
			}
			else if (label==eLS_TRIANGLE)
			{
				*pIndex='T'; pIndex++;
				*pIndex='R'; pIndex++;
				*pIndex='['; pIndex++;
				bIsSymbol=True;
			}
			else
			{
				*pIndex='L'; pIndex++;
				*pIndex='B'; pIndex++;
				*pIndex='['; pIndex++;
			}

			*pIndex=(char)i+'a'; pIndex++;
			*pIndex=(char)j+'a'; pIndex++;

			if (bIsSymbol==True)
			{
				*pIndex=']'; pIndex++;			
				continue;
			}

			*pIndex=':'; pIndex++;

			if ((unsigned int)ch<255)
			{
				*pIndex=(char)ch; pIndex++;
			}
			else
			{
				*((wchar_t*)pIndex)=ch; pIndex++;pIndex++;
			}

			*pIndex=']'; pIndex++;			
		}
	} // if label

	*pIndex=(char)0;
	
	{
		int n = strlen(buffer);

		//if (!write_buffer_ex && n>255)
		//{
		//	Sleep(10);
		//}

		if (WriteFile(hFile, buffer, n, &nWritten, 0)==False)
		{
			return False;
		}
	}

	// 追加情報があれば
	if (info->exinfo)
	{
		char exbuffer[128];
		exbuffer[0]=0;

		//if (info->exinfo->comment_code!=0)
		//{
		//	char buff[16];
		//	wsprintf(buff, "CX[%d]", (unsigned int)(info->exinfo->comment_code & 0xFF));
		//	strcat(exbuffer, buff);
		//}
		
		if ((info->exinfo->stonevalue!=0) && (STONE_TYPE(info->stone)==eST_BLACK ||
													STONE_TYPE(info->stone)==eST_WHITE) )
		{
			char buff[16];
			wsprintf(buff, "xSV[%d]", info->exinfo->stonevalue);
			strcat(exbuffer, buff);
		}

		if (info->exinfo->option!=0)
		{
			char buff[16];
			wsprintf(buff, "OX[%d]", (unsigned int)(info->exinfo->option & 0xFFFF));
			strcat(exbuffer, buff);
		}

		if (info->exinfo->status!=0)
		{
			char buff[16];
			wsprintf(buff, "ST[%d]", (unsigned int)(info->exinfo->status & 0xFF));
			strcat(exbuffer, buff);
		}

		if (info->exinfo->flag32!=0)
		{
			int level = info->exinfo->flag32>>28;

			if (level)
			{
				char buff[16];
				wsprintf(buff, "FX[%d]", level);
				strcat(exbuffer, buff);
			}

			level = (info->exinfo->flag32>>24)&0x0F;

			if (level)
			{
				char buff[16];
				wsprintf(buff, "DE[%d]", level);
				strcat(exbuffer, buff);
			}

			level = (info->exinfo->flag32>>20)&0x0F;

			if (level)
			{
				char buff[16];
				wsprintf(buff, "MF[%d]", level);
				strcat(exbuffer, buff);
			}

			int joseki_code = GET_JOSEKI_NAME(info->exinfo->flag32);

			if (joseki_code)
			{
				char buff[16];
				int h = GET_JOSEKI_HIGH(joseki_code);
				int l = GET_JOSEKI_LOW(joseki_code);
				int d = GET_JOSEKI_DETAIL(joseki_code);

				wsprintf(buff, "JX[%d,%d,%d]", h, l, d);
				strcat(exbuffer, buff);
			}

		}

		if (exbuffer[0]!=0)
		{
			if (WriteFile(hFile, exbuffer, strlen(exbuffer), &nWritten, 0)==False)
			{
				return False;
			}
		}
	}

	// レファレンスがあれば
	if (info->ref)
	{
		char buff[16];
		cSgfStoneInfo* x = (cSgfStoneInfo*)info->ref->GetData();
		int n = x->refNo;
		wsprintf(buff, "XC[%d]", n);
		if (WriteFile(hFile, buff, strlen(buff), &nWritten, 0)==False)
		{
			return False;
		}

	}
	else if (info->refNo)
	{
		char buff[16];
		wsprintf(buff, "XR[%d]", info->refNo);
		if (WriteFile(hFile, buff, strlen(buff), &nWritten, 0)==False)
		{
			return False;
		}
	}

	// もしリンクがあれば、
	if (info->link)
	{
		char buff[16];
		for (cyiNode* node=info->link->GetNodeTopFrom(); node; node=node->np)
		{
			int n = (int)node->data;

			wsprintf(buff, "XL[%d]", n);
			if (WriteFile(hFile, buff, strlen(buff), &nWritten, 0)==False)
			{
				return False;
			}
		}
	}
	// リンク先であれば、
	if (info->linkNo!=0)
	{
		char buff[16];
		wsprintf(buff, "XS[%d]", info->linkNo);
		if (WriteFile(hFile, buff, strlen(buff), &nWritten, 0)==False)
		{
			return False;
		}
	}


	// 再生済みならば

	if (xxxFlag==False)
	{
		if (info->flag32&(1<<19))
		{
			if (t->GetChildList().count()==0 ||
					((info->exinfo!=0)&&(info->exinfo->option!=0)))
			{
				if (WriteFile(hFile, "xPD[]", 5, &nWritten, 0)==False)
				{
					return False;
				}
			}
		}

		// お気に入りならば
		if (info->flag32&0x40000000)
		{
			if (WriteFile(hFile, "XF[]", 4, &nWritten, 0)==False)
			{
				return False;
			}
		}
	}

	// 参考図があれば、
	if (info->reference)
	{
		for (cyiNode* node=info->reference->GetNodeTopFrom(); node; node=node->np)
		{

			cyiTreeNode* x=(cyiTreeNode*)node->ref;
			cSgfStoneInfo* xinfo=(cSgfStoneInfo*)x->GetData();

			if (xinfo==0)
			{
				continue;
			}

			if (WriteFile(hFile, "xREF(", 5, &nWritten, 0)==False)
			{
				return False;
			}

#ifdef _xDEBUG
		{
			char buffer[128];
			wsprintf(buffer, "xSTART<%x>", t);
			WriteFile(hFile, buffer, strlen(buffer), &nWritten, 0);
		}
#endif
			if (WriteSgfBody(hFile, x, xxxFlag)==False)
			{
				return False;
			}
			
			//if (WriteFile(hFile, ");", 2, &nWritten, 0)==False)
			//{
			//	return False;
			//}

		}
	}

SKIP_STONE:
	if (write_buffer_ex)
	{
		yifree(write_buffer_ex);
		write_buffer_ex=0;
	}
	

	// 分岐が終わったら')'を付け、WriteBody呼び出し
	if ((nChildCnt=t->GetChildList().count())==0)
	{
		DWORD nWritten=0;
		char branch_end[] = {')',  0};
#ifdef _xDEBUG
		{
			char buffer[128];
			wsprintf(buffer, "xEND<%x>", t);
			WriteFile(hFile, buffer, strlen(buffer), &nWritten, 0);
		}
#endif
		if (WriteFile(hFile, branch_end, strlen(branch_end), &nWritten, 0)==False)
		{
			return False;
		}


		return True;
	}
	else if (nChildCnt==1)
	{
		cyiNode* node = t->GetChildList().GetNodeTopFrom(0);
		cyiTreeNode* ct = (cyiTreeNode*)node->data;
		
		if (WriteSgfBody(hFile, ct, xxxFlag)==False)
		{
			return False;
		}
		return True;
	}


	for (cyiNode* node = t->GetChildList().GetNodeTopFrom(0); node; node=node->np)
	{
		char buffer[16];
		cyiTreeNode* ct = (cyiTreeNode*)node->data;

		strcpy(buffer, "\r\n(");
		if (WriteFile(hFile, buffer, strlen(buffer), &nWritten, 0)==False)
		{
			return False;
		}
#ifdef _xDEBUG
		{
			char buffer[128];
			wsprintf(buffer, "xSTART<%x>", t);
			WriteFile(hFile, buffer, strlen(buffer), &nWritten, 0);
		}
#endif	
		if (WriteSgfBody(hFile, ct, xxxFlag)==False)
		{
			return False;
		}
		//buffer[0]=')';
		//buffer[1]=(char)0;
		
		//if (WriteFile(hFile, buffer, strlen(buffer), &nWritten, 0)==False)
		//{
		//	return False;
		//}

	}

	// すべての子の処理が終わってから閉じる。
	if (t!=&m_node)
	{
		DWORD nWritten=0;
		char branch_end[] = {')',  0};

		if (WriteFile(hFile, branch_end, strlen(branch_end), &nWritten, 0)==False)
		{
			return False;
		}
	}

	//if (t->GetParent()==0)
	//{
	//	DWORD nWritten=0;
	//	char branch_end[] = {')',  0};

	//	if (WriteFile(hFile, branch_end, strlen(branch_end), &nWritten, 0)==False)
	//	{
	//		return False;
	//	}
	//}

	return True;

}

// -----------------------------------
static char* xTimeToStr(unsigned int time, char* str)
{
	int year = time>>12;
	int month = (time>>5)&0x0000000F;
	int date  = time&0x1F;

	wsprintf(str, "%d-%02d-%02d", DAY_YEAR(time), DAY_MON(time), DAY_DAY(time));//year, month, date);
	return str;
}


// -----------------------------------
Bool cSgfManager::IsChanged()
{
	return m_bIschanged;
}

// -----------------------------------
void cSgfManager::SetChangedFlag(Bool bIsChanged)
{
	m_bIschanged=bIsChanged;
}

// -----------------------------------
// 
Bool cSgfManager::SaveSgfFile(LPCSTR pszFileName, Bool xxxFlag)
{
	char szHeader[4096];
	char szResult[32];
	char timebuff[32];

	if (pszFileName==0 && m_szFileName[0]==0)
	{
		return False;
	}

	//if (m_bsNgf==True)
	//{
	//	return False;
	//}

	int n = GET_WINER(m_header->flag32);

	if (n==0)
	{
		szResult[0]=0;
	}
	else if (n==3)
	{
		wsprintf(szResult, "RE[0]");
	}
	else
	{
		if (GET_SCORE(m_header->flag32)==0xFF)
		{
			wsprintf(szResult, "RE[%c+R]", n==1?'B':'W');//, ?
		}
		else
		{
			wsprintf(szResult, "RE[%c+%d%s]", n==1?'B':'W', GET_SCORE(m_header->flag32), HALF_SCORE(m_header->condition)?".5":"");
		}
		
	}

	wsprintf(szHeader, "(;CA[%s]FF[4]GM[1]AP[%s:%s]SZ[%d]EV[%s]DT[%s]PB[%s]PW[%s]KM[%d.%d]HA[%d]%sPC[%s]", 
		"shift_jis", // CA
		"GoYotsuya", "V0.0.5", //AP
		GET_LINE_COUNT(m_header->flag32), //SZ
		m_header->pszEventName?m_header->pszEventName:"", //EV
		xTimeToStr(m_header->playDate, timebuff), //DT
		m_header->pszBlackName ? m_header->pszBlackName:"", //PB
		m_header->pszWhiteName ? m_header->pszWhiteName:"", //PW
		GET_KOMI(m_header->condition), HALF_SCORE(m_header->condition)?5:0, //KM
		IS_NIGIRI(m_header->condition)?0:GET_INIT_STONE(m_header->condition),
		szResult,
		m_header->pszPlace?m_header->pszPlace:""); //RE

	int nLimitedTime = GET_PLAY_TIME(m_header->condition);
	int nSecondRead	 = GET_SECOND_READ2(m_header->condition);
	int nSecondReadCnt = GET_SECOND_READ_COUNT(m_header->condition2);

	if (nLimitedTime)
	{
		char xbuffer[32];
		wsprintf(xbuffer, "TM[%d]", nLimitedTime*60);
		strcat(szHeader, xbuffer);
	}
	if (nSecondRead)
	{
		char xbuffer[32];
		wsprintf(xbuffer, "xST[%d]", nSecondRead);
		strcat(szHeader, xbuffer);
	}
	if (nSecondReadCnt)
	{
		char xbuffer[32];
		wsprintf(xbuffer, "xSC[%d]", nSecondReadCnt);
		strcat(szHeader, xbuffer);
	}



	if (m_header->pszBlackLevel)
	{
		char xbuffer[32];
		wsprintf(xbuffer, "BR[%s]", m_header->pszBlackLevel);
		strcat(szHeader, xbuffer);
	}
	else
	{
		char xbuffer[32];
		int black_level = GET_BLACK_LEVEL(m_header->flag32);

		xGetLevelString(black_level, xbuffer, 0);

		if (xbuffer[0]!=0)
		{
			char xxxx[32];
			wsprintf(xxxx, "BR[%s]", xbuffer);
			strcat(szHeader, xxxx);
		}
	}

	if (m_header->pszWhiteLevel)
	{
		char xbuffer[32];
		wsprintf(xbuffer, "WR[%s]", m_header->pszWhiteLevel);
		strcat(szHeader, xbuffer);
	}
	else
	{
		char xbuffer[32];
		int white_level = GET_WHITE_LEVEL(m_header->flag32);

		xGetLevelString(white_level, xbuffer, 0);

		if (xbuffer[0]!=0)
		{
			char xxxx[32];
			wsprintf(xxxx, "WR[%s]", xbuffer);
			strcat(szHeader, xxxx);
		}
	}




	int nInitStone = GET_INIT_STONE(m_header->condition);

	if (nInitStone>1)
	{
		strcat(szHeader, "AB");

		for (cyiNode* node = m_InitStone.GetNodeTopFrom(0); node; node=node->np)
		{
			char buffer[16];
			stone_t stone = (stone_t)node->ref;

			wsprintf(buffer, "[%c%c]", (char)STONE_POS_X(stone) + 'a', (char)STONE_POS_Y(stone)+'a');
			strcat(szHeader, buffer);
		}
	}

	int nHeaderLen = strlen(szHeader);

	if (pszFileName==0)
	{
		pszFileName = m_szFileName;
	}

	HANDLE hFile = CreateFileA(pszFileName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile==INVALID_HANDLE_VALUE)
	{
		return False;
	}


	//strcat(szHeader, "\r\n");


	SetFilePointer(hFile, 0, 0, FILE_BEGIN);

	DWORD nWritten=0;
	if (WriteFile(hFile, szHeader, nHeaderLen, &nWritten, NULL)==FALSE)
	{
		CloseHandle(hFile);
		return False;
	}

	if (nWritten!=nHeaderLen)
	{
		CloseHandle(hFile);
	}

	if (m_header->pszComment)
	{
		if (WriteFile(hFile, "C[", 2, &nWritten, NULL)==FALSE)
		{
			CloseHandle(hFile);
			return False;
		}
		if (WriteFile(hFile, m_header->pszComment, strlen(m_header->pszComment), &nWritten, NULL)==FALSE)
		{
			CloseHandle(hFile);
			return False;
		}
		if (WriteFile(hFile, "]", 1, &nWritten, NULL)==FALSE)
		{
			CloseHandle(hFile);
			return False;
		}
	}

	if (WriteFile(hFile, "\r\n", 2, &nWritten, NULL)==FALSE)
	{
		CloseHandle(hFile);
		return False;
	}

	if (WriteSgfBody(hFile, &m_node, xxxFlag)==False)
	{
		CloseHandle(hFile);
		return False;
	}

	// ヘッダーに対するカッコ
	WriteFile(hFile, "\r\n\r\n)", strlen("\r\n\r\n)"), &nWritten, 0);

	SetEndOfFile(hFile);
	CloseHandle(hFile);

	m_bIschanged=False;


	return True;
}


// -----------------------------------
// -----------------------------------
// command
// 1: black B
// 2: white W
// 3: black pass B[]
// 4: white pass W[]
// 5: comment C[]
// 6: Circle CR
// 7: Triangle TR
// 8: Square SQ
// 9: MA 
//10: Label LB[xx:あ]
//11: Start Branch (
//12: End Branch )
//13: AB
//14: AW
//15: CX // Comment Code [no:no]
//16: ST // Status eGoResultOption参照
//17: OX[no] // オプションコード
//21: FX[no] //人気度(1-5)
//22: DE[no] // 難易度(1-5)
//23: MF[no] // マスターフラグ(成功すれば+1)
//24: JX[no,no,no] // 定石名
//25: XR[ref] // レファレンス番号
//26: XC[ref] // レファレンス番号
//27: XP[]
//28: XF[]
//29: XS[ref] // リンク本体
//30: XL[ref] // リンク参照
//31: xREF( // 参考
//32: xSV[
//34: xPD[ // 再生済み　定石辞典でしか使われない。
Bool cSgfManager::WriteBinaryGoFormat(cyiMemoryStream* stream, cyiTreeNode* t, cyiTreeNode* pTopNode)
{
	int   nStartPos ;
	int	  nCommentLen=0;
	int	  nChildCnt=0;
	DWORD nWritten=0;
	char cmd[16];
	stone_t stone;
	unsigned char chunk=0;

	if (t==0)
		return False;



	// 分岐があるところまで書き込む。
	cSgfStoneInfo* info = (cSgfStoneInfo* )t->GetData();

	if (t->GetParent()!=0 &&
			info==0)
	{
		return False;
	}

	if (info==0)
		goto SKIP_STONE;

	if (info->comment)
	{
		nCommentLen= strlen(info->comment);
	}
	
	stone = info->stone;
	{

		if (STONE_TYPE(stone)==eST_BLACK)
		{

			if (info->nOrder==0xFFFF)
			{
				chunk=13;
			}
			else
			{
				chunk=1;
			}
			//stream->Write(&chunk, 1, 0);
		}
		else if (STONE_TYPE(stone)==eST_WHITE)
		{
			if (info->nOrder==0xFFFF)
			{
				chunk=14;
			}
			else
			{
				chunk=2;
			}
		}
		else if (STONE_TYPE(stone)==eST_BPASS)
		{
			chunk=3;
			stream->Write(&chunk, 1, 0);
			//goto SKIP_SET_POS;
		}
		else if (STONE_TYPE(stone)==eST_WPASS)
		{
			chunk=4;
			//goto SKIP_SET_POS;
			stream->Write(&chunk, 1, 0);
		}
		else
		{
			// ???
			goto JUMP_TO_COMMENT;
		}
	}


	if (chunk!=3 &&
			chunk!=4)
	{
		int x = STONE_POS_X(stone);
		int y = STONE_POS_Y(stone);
		
		chunk|=((x&0x10)!=0?0x80:0);
		chunk|=((y&0x10)!=0?0x40:0);
		stream->Write(&chunk, 1, 0);
		chunk=((x&0x0F)<<4)|(y&0x0F);
		stream->Write(&chunk, 1, 0);
	}


JUMP_TO_COMMENT:
	// コメントがあれば
	if (info->comment && info->m_AddStoneList==0)
	{
		unsigned short len = strlen(info->comment);

		if (len)
		{
			chunk=5;
			stream->Write(&chunk, 1, 0);
			stream->Write(&len, 2, 0);
			stream->Write(info->comment, len, 0);
		}
	}

	// ABもしくはAWがあれば、
	if (info->m_AddStoneList)
	{
		int n = info->m_AddStoneList->count();

		if (n!=0)
		{
			for (cyiNode* node=info->m_AddStoneList->GetNodeTopFrom(0); node; node=node->np)
			{
				char b[16];
				stone_t stone = (stone_t)node->data;
				int x = STONE_POS_X(stone);
				int y = STONE_POS_Y(stone);

				if (STONE_TYPE(stone)==eST_BLACK)
				{
					chunk=13;
				}
				else
				{
					chunk=14;
				}

				chunk|=((x&0x10)!=0?0x80:0);
				chunk|=((y&0x10)!=0?0x40:0);
				stream->Write(&chunk, 1, 0);
				chunk=((x&0x0F)<<4)|(y&0x0F);
				stream->Write(&chunk, 1, 0);

			}
		}
	
		// コメントがあれば
		if (info->comment)
		{
			unsigned short len = strlen(info->comment);

			if (len)
			{
				chunk=5;
				stream->Write(&chunk, 1, 0);
				stream->Write(&len, 2, 0);
				stream->Write(info->comment, len, 0);
			}
		}
	}

	// ラベルがあれば
	if (info->label)
	{
// 6: Circle CR
// 7: Triangle TR
// 8: Square SQ
// 9: MA 
//10: Label LB[xx:あ]

		for (cyiNode* node=info->label->GetNodeTopFrom(0); node; node=node->np)
		{
			unsigned int label = (unsigned int )node->ref;
			wchar_t ch = label&0xFFFF;
			int i = label>>24;
			int j = 0xFF&(label>>16);
			Bool bIsSymbol=False;

			if (label==eLS_SQURE)
			{
				chunk=8;
				bIsSymbol=True;
			}
			else if (label==eLS_CIRCLE)
			{
				chunk=6;
				bIsSymbol=True;
			}
			else if (label==eLS_TRIANGLE)
			{
				chunk=7;
				bIsSymbol=True;
			}
			else if (label==eLS_MA)
			{
				chunk=9;
				bIsSymbol=True;
			}
			else
			{
				chunk=10;
			}

			chunk|=((i&0x10)!=0?0x80:0);
			chunk|=((j&0x10)!=0?0x40:0);
			stream->Write(&chunk, 1, 0);
			chunk=((i&0x0F)<<4)|(j&0x0F);
			stream->Write(&chunk, 1, 0);


			if (bIsSymbol==False)
			{
				stream->Write(&ch, 2, 0);
			}
	
		}
	} // if label

	// 追加情報があれば
	// 定石辞典など。
	if (info->exinfo)
	{
		char exbuffer[128];
		exbuffer[0]=0;
		
		if ((info->exinfo->stonevalue!=0) && (STONE_TYPE(info->stone)==eST_BLACK ||
													STONE_TYPE(info->stone)==eST_WHITE) )
		{
			// xSV. Stone Value.
			chunk=32;
			stream->Write(&chunk, 1, 0);
			chunk=(unsigned char)info->exinfo->stonevalue;
			stream->Write(&chunk, 1, 0);
		}

		if (info->exinfo->option!=0)
		{
			// OX. Option.
			chunk=17;
			stream->Write(&chunk, 1, 0);
			unsigned short opt = (unsigned short)(info->exinfo->option&0xFFFF);
			stream->Write(&opt, 2, 0);
		}

		if (info->exinfo->status!=0)
		{
			// ST. Status
			chunk=16;
			stream->Write(&chunk, 1, 0);
			chunk = (unsigned short)(info->exinfo->status&0xFF);
			stream->Write(&chunk, 1, 0);
		}

		if (info->exinfo->flag32!=0)
		{
			
			int level = info->exinfo->flag32>>28;

			if (level)
			{
				//21: FX[no] //人気度(1-5)
				chunk=21;
				stream->Write(&chunk, 1, 0);
				chunk=(unsigned int)level;
				stream->Write(&chunk, 1, 0);
			}

			level = (info->exinfo->flag32>>24)&0x0F;

			if (level)
			{
				//22: DE[no] // 難易度(1-5)
				chunk=22;
				stream->Write(&chunk, 1, 0);
				chunk=(unsigned int)level;
				stream->Write(&chunk, 1, 0);
			}

			level = (info->exinfo->flag32>>20)&0x0F;

			if (level)
			{
				//23: MF[no] // マスターフラグ(成功すれば+1)

				chunk=23;
				stream->Write(&chunk, 1, 0);
				chunk=(unsigned int)level;
				stream->Write(&chunk, 1, 0);
			}

			int joseki_code = GET_JOSEKI_NAME(info->exinfo->flag32);

			if (joseki_code)
			{
				//24: JX[no,no,no] // 定石名
				int h = GET_JOSEKI_HIGH(joseki_code);
				int l = GET_JOSEKI_LOW(joseki_code);
				int d = GET_JOSEKI_DETAIL(joseki_code);

				chunk=24;
				stream->Write(&chunk, 1, 0);

				chunk=(unsigned char)h;
				stream->Write(&chunk, 1, 0);
				chunk=(unsigned char)l;
				stream->Write(&chunk, 1, 0);
				chunk=(unsigned char)d;
				stream->Write(&chunk, 1, 0);

			}

		}
	}

	// レファレンスがあれば
	if (info->ref)
	{
//26: XC[ref] // レファレンス番号

		cSgfStoneInfo* x = (cSgfStoneInfo*)info->ref->GetData();
		unsigned short ref = (unsigned short)x->refNo;
		chunk=26;
		stream->Write(&chunk, 1, 0);
		stream->Write(&ref, 2, 0);
	}
	else if (info->refNo)
	{
//25: XR[ref] // レファレンス番号
		chunk=25;
		unsigned short ref = (unsigned short)info->refNo;

		stream->Write(&chunk, 1, 0);
		stream->Write(&ref, 2, 0);
	}

//27: XP[]

//30: XL[ref] // リンク参照

//32: xSV[


	// もしリンクがあれば、
	if (info->link)
	{
//30: XL[ref] // リンク参照
		for (cyiNode* node=info->link->GetNodeTopFrom(); node; node=node->np)
		{
			unsigned short n = (unsigned short)node->data;
			chunk=30;

			stream->Write(&chunk, 1, 0);
			stream->Write(&n, 2, 0);
		}
	}
	// リンク先であれば、
	if (info->linkNo!=0)
	{
//29: XS[ref] // リンク本体
		unsigned short n = (unsigned short)info->linkNo;

		chunk=29;

		stream->Write(&chunk, 1, 0);
		stream->Write(&n, 2, 0);
	}


	// 再生済みならば
	if (info->flag32&(1<<19))
	{
//34: xPD[ // 再生済み　定石辞典でしか使われない。
		chunk=34;
		stream->Write(&chunk, 1, 0);
	}

	// お気に入りならば
	if (info->flag32&0x40000000)
	{
//28: XF[]
		chunk=28;
		stream->Write(&chunk, 1, 0);
	}

	// 参考図があれば、
	if (info->reference)
	{
//31: xREF( // 参考
		for (cyiNode* node=info->reference->GetNodeTopFrom(); node; node=node->np)
		{

			cyiTreeNode* x=(cyiTreeNode*)node->ref;
			cSgfStoneInfo* xinfo=(cSgfStoneInfo*)x->GetData();

			if (xinfo==0)
			{
				continue;
			}

			chunk=31;
			stream->Write(&chunk, 1, 0);
			//chunk=11;
			//stream->Write(&chunk, 1, 0);
#ifdef _xDEBUG
		{
			char buffer[128];
			wsprintf(buffer, "xSTART<%x>", t);
			WriteFile(hFile, buffer, strlen(buffer), &nWritten, 0);
		}
#endif
			if (WriteBinaryGoFormat(stream, x, pTopNode)==False)
			{
				return False;
			}
			
			//if (WriteFile(hFile, ");", 2, &nWritten, 0)==False)
			//{
			//	return False;
			//}

		}
	}

SKIP_STONE:
	

	// 分岐が終わったら')'を付け、WriteBody呼び出し
	if ((nChildCnt=t->GetChildList().count())==0)
	{
		chunk=12;
		stream->Write(&chunk, 1, 0);
#ifdef _xDEBUG
		{
			char buffer[128];
			wsprintf(buffer, "xEND<%x>", t);
			WriteFile(hFile, buffer, strlen(buffer), &nWritten, 0);
		}
#endif

		return True;
	}
	else if (nChildCnt==1)
	{
		cyiNode* node = t->GetChildList().GetNodeTopFrom(0);
		cyiTreeNode* ct = (cyiTreeNode*)node->data;
		
		if (WriteBinaryGoFormat(stream, ct, pTopNode)==False)
		{
			return False;
		}
		return True;
	}


	for (cyiNode* node = t->GetChildList().GetNodeTopFrom(0); node; node=node->np)
	{
		cyiTreeNode* ct = (cyiTreeNode*)node->data;

		// (
		chunk=11;
		stream->Write(&chunk, 1, 0);

#ifdef _xDEBUG
		{
			char buffer[128];
			wsprintf(buffer, "xSTART<%x>", t);
			WriteFile(hFile, buffer, strlen(buffer), &nWritten, 0);
		}
#endif	
		if (WriteBinaryGoFormat(stream, ct, pTopNode)==False)
		{
			return False;
		}
		//buffer[0]=')';
		//buffer[1]=(char)0;
		
		//if (WriteFile(hFile, buffer, strlen(buffer), &nWritten, 0)==False)
		//{
		//	return False;
		//}

	}

	// すべての子の処理が終わってから閉じる。
	if (t!=pTopNode)
	{
		// )
		chunk=12;
		stream->Write(&chunk, 1, 0);
	}

	return True;
}

#include "yiStream.h"
#define xIS_NULL(str) ((str!=0)&&(str[0]!=0))

// -----------------------------------
Bool cSgfManager::SaveBinaryGoFormat(LPCSTR pszFileName, void* header, int nHeaderSize)
{
	cyiMemoryStream* stream = ExportBinaryGoFormat(m_header, &m_node, header, nHeaderSize);


	if (pszFileName)
	{
		Bool res= stream->ExportToFile(pszFileName);
		delete stream;
		return res;
	}
	Bool res = (Bool)stream->ExportToBinary();
	delete stream;
	return res;
}

// -----------------------------------
cyiMemoryStream* cSgfManager::ExportBinaryGoFormat(sGoHeader* goheader, cyiTreeNode* pTopNode, void* header, int nHeaderSize)
{

	sGoHeader* m_header=goheader;
	cyiMemoryStream* stream = new cyiMemoryStream();
	
	if (stream->Write("BGF\0", 4, 0)!=S_OK)
	{
		return False;
	}
	
	if (stream->Write(&nHeaderSize, sizeof(int), 0)!=S_OK)
	{
		return False;
	}

	if (header!=0)
	{
		if (stream->Write(header, nHeaderSize, 0)!=S_OK)
		{
			return False;
		}
	}

	if (stream->Write(&m_header->condition, sizeof(unsigned int), 0)!=S_OK)
	{
		return False;
	}

	if (stream->Write(&m_header->condition2, sizeof(unsigned int), 0)!=S_OK)
	{
		return False;
	}

	if (stream->Write(&m_header->flag32, sizeof(unsigned int), 0)!=S_OK)
	{
		return False;
	}

	// ここからはチャンクデータ
	unsigned char chunk;

	// 黒の名前
	if (xIS_NULL(m_header->pszBlackName))
	{
		chunk=0x81;
		if (stream->Write(&chunk, sizeof(unsigned char), 0)!=S_OK)
		{
			return False;
		}
		chunk=strlen(m_header->pszBlackName);

		if (stream->Write(&chunk, sizeof(unsigned char), 0)!=S_OK)
		{
			return False;
		}

		if (stream->Write(m_header->pszBlackName, (int)chunk, 0)==False)
		{
			return False;
		}

	}
	// 白の名前
	if (xIS_NULL(m_header->pszWhiteName))
	{
		chunk=0x82;
		if (stream->Write(&chunk, sizeof(unsigned char), 0)!=S_OK)
		{
			return False;
		}
		chunk=strlen(m_header->pszWhiteName);

		if (stream->Write(&chunk, sizeof(unsigned char), 0)!=S_OK)
		{
			return False;
		}

		if (stream->Write(m_header->pszWhiteName, (int)chunk, 0)!=S_OK)
		{
			return False;
		}
	}
	// イベント名
	if (xIS_NULL(m_header->pszEventName))
	{
		chunk=0x83;
		if (stream->Write(&chunk, sizeof(unsigned char), 0)!=S_OK)
		{
			return False;
		}
		chunk=strlen(m_header->pszEventName);

		if (stream->Write(&chunk, sizeof(unsigned char), 0)!=S_OK)
		{
			return False;
		}

		if (stream->Write(m_header->pszEventName, (int)chunk, 0)!=S_OK)
		{
			return False;
		}
	}

	// 初期コメント
	if (xIS_NULL(m_header->pszComment))
	{
		chunk=0x84;
		if (stream->Write(&chunk, sizeof(unsigned char), 0)!=S_OK)
		{
			return False;
		}
		chunk=strlen(m_header->pszComment);

		if (stream->Write(&chunk, sizeof(unsigned char), 0)!=S_OK)
		{
			return False;
		}

		if (stream->Write(m_header->pszComment, (int)chunk, 0)!=S_OK)
		{
			return False;
		}
	}

	// イベント名
	//if (m_header->pszEventName)
	//{
	//	chunk=0x83;
	//	if (stream->Write(&chunk, sizeof(unsigned char), 0)==False)
	//	{
	//		return False;
	//	}
	//	chunk=strlen(m_header->pszEventName);

	//	if (stream->Write(&chunk, sizeof(unsigned char), 0)==False)
	//	{
	//		return False;
	//	}

	//	if (stream->Write(m_header->pszEventName, (int)chunk, 0)==False)
	//	{
	//		return False;
	//	}
	//}
	// ばしょ
	// 段位

	// ここからボディ
	chunk=0xFF;
	if (stream->Write(&chunk, 1, 0)!=S_OK)
	{
		return False;
	}

	if (WriteBinaryGoFormat(stream, pTopNode, pTopNode)==True)
	{
		chunk=0;
		stream->Write(&chunk, 1, 0);

		return stream;

		/*if (pszFileName)
		{
			return stream->ExportToFile(pszFileName);
		}
		return (Bool)stream->ExportToBinary();*/
	}

	delete stream;
	return 0;
}

// -----------------------------------
Bool cSgfManager::ParseYgfHeader(cyiMemoryIO* io, sGoHeader* header)
{
	while(1)
	{
		unsigned char chunkId;
		if (io->Read(&chunkId, 1)!=1)
			return False;

		if (chunkId==eYHCID_EOF)
			break;

		switch(chunkId)
		{
			case eYHCID_TITLE:
			{
				unsigned short len;
				io->Read(&len, 2);
				header->pszEventName=(char*)yialloci(len+1);

				io->Read(header->pszEventName, len);
				break;
			}
			case eYHCID_BLACK_NAME:
			{
				unsigned short len;
				io->Read(&len, 2);

				header->pszBlackName=(char*)yialloci(len+1);

				io->Read(header->pszBlackName, len);
				break;
			}
			case eYHCID_WHITE_NAME:
			{
				unsigned short len;
				if (io->Read(&len, 2)==False)
					return False;

				header->pszWhiteName=(char*)yialloci(len+1);

				io->Read(header->pszWhiteName, len);
				break;
			}
			case eYHCID_INITSTONE_COUNT:
			{
				char n;
				if (io->Read(&n, 1)==False)
					return False;

				if (n==0)
					break;

				if ((n&0x80)==0)
				{
					SET_INIT_STONE(header->condition, n);
					break;
				}

				n=(n&0x7F);
				SET_INIT_STONE(header->condition, n);
				
				for (int i=0; i<n; i++)
				{
					unsigned short pos;
					if (io->Read(&pos, 2)==False)
						return False;

					InitStone(MAKESTONE(eST_BLACK, 0, (pos>>8), (pos&0xFF)));
				}
				
				break;
			}
			case eYHCID_KOMI:
			{
				float komi;
				
				if (io->Read(&komi, 4)==False)
					return False;
				
				SET_KOMI(header->condition, (int)komi);

				if (komi-(float)((int)(komi))>0)
				{
					SET_HALF_SCORE(header->condition);
				}
				break;
			}
			case eYHCID_PLAY_DATE:
			{
				if (io->Read(&header->playDate, 4)==False)
					return False;
				break;
			}
			case eYHCID_PLAY_TIME:
			{
				if (io->Read(&header->remainTime, 4)==False)
					return False;
				break;
			}
			case eYHCID_BLACK_LEVEL:
			{
				char level;
				if (io->Read(&level, 1)==False)
					return False;

				SET_BLACK_LEVEL(header->flag32, level);
				break;
			}
			case eYHCID_WHITE_LEVEL:
			{
				char level;
				if (io->Read(&level, 1)==False)
					return False;
				SET_WHITE_LEVEL(header->flag32, level);
				break;
			}
			case eYHCID_PLACE:
			{
				unsigned short len;
				if (io->Read(&len, 2)==False)
					return False;

				header->pszPlace=(char*)yialloci(len+1);

				io->Read(header->pszPlace, len);
				break;
			}
			case eYHCID_PLAY_COMMENT:
			{
				unsigned short len;
				if (io->Read(&len, 2)==False)
					return False;

				header->pszComment=(char*)yialloci(len+1);

				io->Read(header->pszComment, len);
				break;
			}
			case eYHCID_BLACK_TEAM:
			{
				unsigned short len;
				if (io->Read(&len, 2)==False)
					return False;

				header->pszBTeam=(char*)yialloci(len+1);

				io->Read(header->pszBTeam, len);
				break;
			}
			case eYHCID_WHITE_TEAM:
			{
				unsigned short len;
				if (io->Read(&len, 2)==False)
					return False;

				header->pszWTeam=(char*)yialloci(len+1);

				io->Read(header->pszWTeam, len);
				break;
			}
			case eYHCID_RO:
			{
				unsigned short len;
				if (io->Read(&len, 2)==False)
					return False;

				header->pszRo=(char*)yialloci(len+1);

				io->Read(header->pszRo, len);
				break;
			}
			case eYHCID_RESULT:
			{
				unsigned short result;
				if (io->Read(&result, 2)==False)
					return False;

				// 引き分け
				if ((0x7FFF&result)==0 &&
					HALF_SCORE(header->condition)==0)
				{
					SET_DRAW(header->flag32);
					break;
				}

				if ((result&0x8000)!=0)
				{
					SET_BLACK_WIN(header->flag32);
				}
				else
				{
					SET_WHITE_WIN(header->flag32);
				}
				SET_SCORE(header->flag32, (result&0x3FFF));
				break;
			}
			case eYHCID_LOCALE:
			{
				char code;
				if (io->Read(&code, 1)==False)
					return False;
				break;
			}
			case eYHCID_LINE_COUNT:
			{
				char cnt;
				if (io->Read(&cnt, 1)==False)
				{
					return False;
				}
				SET_LINE_COUNT(header->flag32, (int)cnt);
				break;
			}
			case eYHCID_APPNAME:
			{
				char buffer[256];
				unsigned short len;
				if (io->Read(&len, 2)==False)
					return False;


				io->Read(buffer, len);
				break;
			}
			case eYHCID_EOF:
			{
				return True;
			}
			default:
			{
				//Sleep(10);
				break;
			}

		}// switch
	}//while

	return True;
}

// -----------------------------------
Bool cSgfManager::ParseYgf(cyiMemoryIO* io)
{
	int nMaxLine = GET_LINE_COUNT(m_header->flag32);
	cyiStack branchStack;

	ToTop();

	while(1)
	{
		char cmd[2];
		char id;

		if (io->Read(&cmd[0],1)==0)
			return True;

		id = (cmd[0]>>2);

		switch(id)
		{
			case eYCID_B:
			{
				io->Read(&cmd[1], 1);

				unsigned short* pStone = (unsigned short*)cmd;
				int x = (*pStone>>5)&0x1F;
				int y = (*pStone&0x1F);

				stone_t stone=MAKESTONE(eST_BLACK, 0, x, y);
				if (xIsRangeValid(stone, nMaxLine)==False)
				{
					// とりあえず、いまのところは。
					stone_t stone = MAKESTONE(eST_BPASS, 0, 0, 0);
					Add(stone, -1);
					break;
				}

				Add(stone, -1);
				break;
			}
			case eYCID_W:
			{
				io->Read(&cmd[1], 1);

				unsigned short* pStone = (unsigned short*)cmd;
				int x = (*pStone>>5)&0x1F;
				int y = (*pStone&0x1F);

				stone_t stone=MAKESTONE(eST_WHITE, 0, x, y);
				if (xIsRangeValid(stone, nMaxLine)==False)
				{
					// とりあえず、いまのところは。
					stone_t stone = MAKESTONE(eST_BPASS, 0, 0, 0);
					Add(stone, -1);
					break;
				}

				Add(stone, -1);
				break;
			}
			case eYCID_B_PASS:
			{
				stone_t stone = MAKESTONE(eST_BPASS, 0, 0, 0);
				Add(stone, -1);
				break;
			}
			case eYCID_W_PASS:
			{
				stone_t stone = MAKESTONE(eST_WPASS, 0, 0, 0);
				Add(stone, -1);
				break;
			}
			case eYCID_AB:
			{
				io->Read(&cmd[1], 1);

				unsigned short* pStone = (unsigned short*)cmd;
				int x = (*pStone>>5)&0x1F;
				int y = (*pStone&0x1F);

				stone_t stone=MAKESTONE(eST_BLACK, 0, x, y);
				Add(stone, -1, True);
				break;
			}
			case eYCID_AW:
			{
				io->Read(&cmd[1], 1);

				unsigned short* pStone = (unsigned short*)cmd;
				int x = (*pStone>>5)&0x1F;
				int y = (*pStone&0x1F);

				stone_t stone=MAKESTONE(eST_WHITE, 0, x, y);
				Add(stone, -1, True);
				break;
			}
			case eYCID_LABEL_SHAPE:
			{
				char code;
				unsigned short pos;
				io->Read(&code, 1);

				io->Read(&pos, 2);

				AddLabel(HIBYTE(pos),LOBYTE(pos), 0xFF00|code);
				break;
			}
			case eYCID_LABEL:
			{
				char code;
				unsigned short pos;
				io->Read(&code, 1);
				io->Read(&pos, 2);

				AddLabel(HIBYTE(pos),LOBYTE(pos), code);
				break;
			}
			case eYCID_COMMENT:
			{
				int len;
				if (io->Read(&len, 2)==False)
					return False;

				char* pszComment=0;
				
				if (len!=0)
				{
					pszComment=(char*)yialloci(len+1);
					io->Read(pszComment, len);

					cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

					if (info==0)
					{
						m_pszRootComment = pszComment;
						break;
					}

					info->comment =pszComment;

				}


				break;
			}
			// TODO:
			case eYCID_COMMENT_CODE:
			{
				break;
			}
			case eYCID_ENTER_BRANCH:
			{
				branchStack.push((yiAnyValue)m_currentNode);
				break;
			}
			case eYCID_LEAVE_BRANCH:
			{
				cyiTreeNode* node=0;
				if (branchStack.pop((yiAnyValue&)node)==True)
				{
					m_currentNode=node;
				}
				break;
			}
			case eYCID_END:
			{
				return True;
			}

			default:
			{
				break;
			}
		}// switch
	}//while

	return False;

}


// -----------------------------------
Bool cSgfManager::LoadYgfFile(LPCSTR pszFileName, int locale)
{
	cyiMemoryIO io;

	io.Create(4096);

	if (io.Load(pszFileName, True, __YGF_VERSION__)==False)
	{
		io.Destroy();
		return False;
	}

	m_header=(sGoHeader*)yialloci(sizeof(sGoHeader*));

	Bool res = ParseYgfHeader(&io, m_header);

	if (res==False)
	{
		io.Destroy();
		return False;
	}

	res = ParseYgf(&io);

	if (res==False)
	{
		io.Destroy();
		return False;
	}

	m_bIsLoadSuccess=True;
	if (res==True)
	{
		SetTitle(yiPathGetFileName(pszFileName));
	}

	ToTop();

	return res;
}

// -----------------------------------
Bool cSgfManager::SaveYgfHeader(cyiMemoryIO* io)
{
	unsigned short size;
	char buffer[16];

	// ロケール設定
	buffer[0]=(char)eYHCID_LOCALE;
	buffer[1]=(char)eCS_JAPANESE;
	io->Write(buffer, 2);

	// AP設定
	buffer[0]=(char)eYHCID_APPNAME;
	io->Write(buffer, 1);
	io->Write(&(size=strlen("GoYotsuya V.0.0.5")), 2);
	io->Write("GoYotsuya V.0.0.5", size);

	// サイズ
	buffer[0]=(char)eYHCID_LINE_COUNT;
	buffer[1]=(char)GET_LINE_COUNT(m_header->flag32);
	io->Write(buffer, 2);

	// タイトル
	if (m_header->pszEventName)
	{
		buffer[0]=(char)eYHCID_TITLE;
		io->Write(buffer, 1);
		unsigned short len = strlen(m_header->pszEventName);
		io->Write(&len, 2);
		io->Write(m_header->pszEventName, len);
	}

	if (m_header->playDate)
	{
		buffer[0]=(char)eYHCID_PLAY_DATE;
		io->Write(buffer, 1);
		io->Write(&m_header->playDate, 4);
	}

	if (m_header->pszBlackName)
	{
		buffer[0]=(char)eYHCID_BLACK_NAME;
		io->Write(buffer, 1);
		short len = strlen(m_header->pszBlackName);
		io->Write(&len, 2);
		io->Write(m_header->pszBlackName, len);
	}

	if (m_header->pszWhiteName)
	{
		buffer[0]=(char)eYHCID_WHITE_NAME;
		io->Write(buffer, 1);
		short len = strlen(m_header->pszWhiteName);
		io->Write(&len, 2);
		io->Write(m_header->pszWhiteName, len);
	}

	if (m_header->pszComment)
	{
		buffer[0]=(char)eYHCID_PLAY_COMMENT;
		io->Write(buffer, 1);
		short len = strlen(m_header->pszComment);
		io->Write(&len, 2);
		io->Write(m_header->pszComment, len);
	}

	int nKomi = GET_KOMI(m_header->condition);

	if (nKomi)
	{
		float komi;
		buffer[0]=(char)eYHCID_KOMI;
		io->Write(buffer, 1);
		komi = (float)nKomi+HALF_SCORE(m_header->condition)==True?0.5:0.0;
		io->Write(&komi, 4);
	}

	char han=GET_INIT_STONE(m_header->condition);

	if (han)
	{
		buffer[0]=(char)eYHCID_INITSTONE_COUNT;
		io->Write(buffer, 1);


		cSgfStoneInfo* info = (cSgfStoneInfo*)m_node.GetData();

		if (info!=0 &&
				info->m_AddStoneList!=0 &&
					info->m_AddStoneList->count()==han)
		{
			Bool bIsAllBlack=True;
			for (cyiNode* node=info->m_AddStoneList->GetNodeTopFrom(0); node; node=node->np)
			{
				stone_t stone = (stone_t)node->data;
				
				if (STONE_TYPE(stone)==eST_WHITE)
				{
					bIsAllBlack=False;
					break;
				}
			}

			if (bIsAllBlack==True)
			{
				han|=0x80;
				// 置き石自由指定
				io->Write(&han, 1);
				for (cyiNode* node=info->m_AddStoneList->GetNodeTopFrom(0); node; node=node->np)
				{
					stone_t stone = (stone_t)node->data;
					unsigned short x = (STONE_POS_X(stone)<<8|STONE_POS_Y(stone));
					io->Write(&x, 2);
				}
			}
			else
			{
				// 置き石自動指定?
				io->Write(&han, 1);
			}
		}
		else
		{
			// 置き石自動指定
			io->Write(&han, 1);
		}
	}

	buffer[0]=(char)eYHCID_EOF;
	io->Write(buffer, 1);

	return True;
}

// -----------------------------------
Bool cSgfManager::WriteYgfBody(cyiMemoryIO* io, cyiTreeNode* t)
{
int	  nCommentLen=0;
	int	  nChildCnt=0;
	stone_t stone;

	if (t==0)
		return False;

	// 分岐があるところまで書き込む。
	cSgfStoneInfo* info = (cSgfStoneInfo* )t->GetData();

	if (t->GetParent()!=0 &&
			info==0)
	{
		return False;
	}

	// トップ
	if (t->GetParent()==0)
	{
		int cnt = t->GetChildList().count();

		if (cnt==0)
			return 0;

		for (cyiNode* node=t->GetChildList().GetNodeTopFrom(0); node; node=node->np)
		{
			WriteYgfBody(io, (cyiTreeNode*)node->data);
		}

		return True;
	}

	if (info==0)
		goto SKIP_STONE;

	stone = info->stone;
	{

		if (STONE_TYPE(stone)==eST_BLACK)
		{
			unsigned short cmd = MAKE_SCMD(eYCID_B, STONE_POS_X(stone), STONE_POS_Y(stone));
			io->Write(&cmd, 2);
		}
		else if (STONE_TYPE(stone)==eST_WHITE)
		{
			unsigned short cmd = MAKE_SCMD(eYCID_W, STONE_POS_X(stone), STONE_POS_Y(stone));
			io->Write(&cmd, 2);
		}
		else if (STONE_TYPE(stone)==eST_BPASS)
		{
			char cmd = MAKE_CMD(eYCID_B_PASS);
			io->Write(&cmd, 1);
		}
		else if (STONE_TYPE(stone)==eST_WPASS)
		{
			char cmd = MAKE_CMD(eYCID_W_PASS);
			io->Write(&cmd, 1);
		}
	}

	// AB or AW
	if (info->m_AddStoneList!=0)
	{
		for (cyiNode* node = info->m_AddStoneList->GetNodeTopFrom(0); node; node=node->np)
		{
			stone_t stone = node->data;
			if (STONE_TYPE(stone)==eST_BLACK)
			{
				unsigned short cmd = MAKE_SCMD(eYCID_AB, STONE_POS_X(stone), STONE_POS_Y(stone));
				io->Write(&cmd, 2);
			}
			else if (STONE_TYPE(stone)==eST_WHITE)
			{
				unsigned short cmd = MAKE_SCMD(eYCID_AW, STONE_POS_X(stone), STONE_POS_Y(stone));
				io->Write(&cmd, 2);
			}
		}
	}

	// ラベルがあれば
	if (info->label)
	{
		char cmd;
		char code;
		for (cyiNode* node=info->label->GetNodeTopFrom(0); node; node=node->np)
		{
			unsigned int label = (unsigned int )node->ref;
			wchar_t ch = label&0xFFFF;
			int i = label>>24;
			int j = 0xFF&(label>>16);
			Bool bIsSymbol=False;

			if (label==eLS_SQURE)
			{
				cmd=MAKE_CMD(eYCID_LABEL_SHAPE);
				io->Write(&cmd, 1);
				code=eYLC_RC;
				io->Write(&code, 1);
			}
			else if (label==eLS_CIRCLE)
			{
				cmd=MAKE_CMD(eYCID_LABEL_SHAPE);
				io->Write(&cmd, 1);
				code=eYLC_CR;
				io->Write(&code, 1);
			}
			else if (label==eLS_TRIANGLE)
			{
				cmd=MAKE_CMD(eYCID_LABEL_SHAPE);
				io->Write(&cmd, 1);
				code=eYLC_TR;
				io->Write(&code, 1);
			}
			else if (label==eLS_MA)
			{
				cmd=MAKE_CMD(eYCID_LABEL_SHAPE);
				io->Write(&cmd, 1);
				code=eYLC_MA;
				io->Write(&code, 1);
			}
			else if ((label>>8)==0xFF)
			{
				cmd=MAKE_CMD(eYCID_LABEL_SHAPE);
				io->Write(&cmd, 1);
				code=(label&0xFF);
				io->Write(&code, 1);
			}
			else
			{
				cmd=MAKE_CMD(eYCID_LABEL);
				io->Write(&cmd, 1);
				io->Write(&ch, 2);
			}

			unsigned short pos = MAKEWORD(i, j);
			io->Write(&pos, 2);
		} // for
	} // if label

	if (info->comment)
	{
		unsigned char cmd = MAKE_CMD(eYCID_COMMENT);
		io->Write(&cmd, 1);
		unsigned short len = strlen(info->comment);
		io->Write(&len, 2);
		io->Write(info->comment, len);
	}

SKIP_STONE:

	// 分岐が終わったら')'を付け、WriteBody呼び出し
	if ((nChildCnt=t->GetChildList().count())==0)
	{
		char cmd = MAKE_CMD(eYCID_LEAVE_BRANCH);
		io->Write(&cmd, 1);
		return True;
	}
	else if (nChildCnt==1)
	{
		cyiNode* node = t->GetChildList().GetNodeTopFrom(0);
		cyiTreeNode* ct = (cyiTreeNode*)node->data;
		
		if (WriteYgfBody(io, ct)==False)
		{
			return False;
		}
		return True;
	}


	for (cyiNode* node = t->GetChildList().GetNodeTopFrom(0); node; node=node->np)
	{
		cyiTreeNode* ct = (cyiTreeNode*)node->data;

		char cmd = MAKE_CMD(eYCID_ENTER_BRANCH);
		io->Write(&cmd, 1);

		if (WriteYgfBody(io, ct)==False)
		{
			return False;
		}
	}

	// すべての子の処理が終わってから閉じる。
	{
		char cmd = MAKE_CMD(eYCID_LEAVE_BRANCH);
		io->Write(&cmd, 1);
	}

	// トップブランチ
	if (t->GetParent()==0)
	{
		char cmd = MAKE_CMD(eYCID_END);
		io->Write(&cmd, 1);
	}

	return True;

}

// -----------------------------------
// 
Bool cSgfManager::SaveYgfFile(LPCSTR pszFileName)
{
	cyiMemoryIO io;

	if (io.Create(4096)==False)
		return False;

	if (SaveYgfHeader(&io)==False)
	{
		io.Destroy();
		return False;
	}

	if (WriteYgfBody(&io, &m_node)==False)
	{
		io.Destroy();
		return False;
	}

	io.Save(pszFileName, True, __YGF_VERSION__);

	return False;
}



// -----------------------------------
LPCSTR cSgfManager::GetTitle()
{
	return this->m_szTitle;
}

// -----------------------------------
void cSgfManager::SetTitle(LPCSTR pszTitle)
{
	strcpy(m_szTitle, pszTitle);
}

// -----------------------------------
//stone_t cSgfManager::Get(int n)
//{
//	cYotsuyaNode* pNode = m_node->ChildList[0];
//
//	if (pNode->nChildCount==0)
//	{
//		return STONE_EOF;
//	}
//
//	for (int i=0; i<n; i++)
//	{
//		if (pNode==NULL)
//			break;
//
//		pNode=pNode->ChildList[0];
//	}
//
//
//	if (pNode==NULL ||
//		pNode->nChildCount==0)
//	{
//		return STONE_EOF;
//	}
//
//	return ((sSgfStoneInfo*)pNode->data)->stone;
//}

// -----------------------------------
//Bool cSgfManager::CreateCashe(unsigned int go_id)
//{
//	char buffer[256];
//
//	wsprintf(buffer, "cashe.%d", 
//}

// -----------------------------------
//Bool cSgfManager::CasheEnd()
//{
//}


// -----------------------------------
Bool cSgfManager::InitStone(stone_t stone)
{
	//m_InitStone.add((yiAnyValue)(new cSgfStoneInfo(stone)));

	for (cyiNode* node=m_InitStone.GetNodeTopFrom(0); node; node=node->np)
	{
		stone_t x = (stone_t)node->ref;

		if (x==stone)
		{
			m_InitStone.remove(node);
			return False;
		}
	}

	m_InitStone.addref((void*)stone);
	
	return True;
	//if (m_hFileCashe)
	//{
	//	AddCashe(stone);
	//}
}

// -----------------------------------
cyiList* cSgfManager::GetInitStoneList()
{
	return &m_InitStone;
}

stone_t g_lastStone=0;

// -----------------------------------
int cSgfManager::GetNodeIndex(cyiTreeNode* t)
{
	if (t==0)
		return 0;

	cyiTreeNode* parent = t->GetParent();

	if (parent==0)
		return 0;

	int index=0;
	for (cyiNode* node=parent->GetChildList().GetNodeTopFrom(); node; node=node->np)
	{
		cyiTreeNode* x = (cyiTreeNode*)node->ref;
		cSgfStoneInfo* info = (cSgfStoneInfo*)x->GetData();

		if (info==0)
			continue;
		if (info->stone==eST_INVALID)
			continue;

		if (t==(cyiTreeNode*)node->data)
		{
			return index;
		}
		index++;
	}

	return -1;
}

// -----------------------------------
int cSgfManager::Add(stone_t stone, int nCatchedStone, Bool bIsAdded, Bool bIsNodeCreate)
{
	int ret=0;
	Bool bIsNoNew=False;

	if (bIsAdded==True)
		goto ADDED_STONE;

	{
		cSgfStoneInfo* stoneInfo = (cSgfStoneInfo*)m_currentNode->GetData();
		if (stoneInfo)
		{
			if (stoneInfo->ref)
			{
				m_currentNode=stoneInfo->ref;
			}

			//if (stoneInfo->stone==eST_INVALID)
			//{
			//	bIsNoNew=True;
			//	goto NEW_NODE;
			//}
		}
	}



	if (m_currentNode->GetChildList().count()!=0)
	{
		cyiNode* node = m_currentNode->GetChildList().GetNodeTopFrom(0);

		int cnt=0;
		for (cyiNode* i = node; i!=NULL; i=i->np)
		{
			cyiTreeNode* treeNode = (cyiTreeNode*)i->data;

			cSgfStoneInfo* stoneInfo = (cSgfStoneInfo*)treeNode->GetData();

			if (STONE_TYPE(stoneInfo->stone)==STONE_TYPE(stone) &&
					STONE_POS_X(stoneInfo->stone)==STONE_POS_X(stone) &&
						STONE_POS_Y(stoneInfo->stone)==STONE_POS_Y(stone))
			{
				m_currentNode=treeNode;


				if (m_bIsLoading==False)
				{
					if (m_currentNode->GetChildList().count()==0 ||
							stoneInfo->exinfo!=0)
					{
						// 再生済みフラグ
						stoneInfo->flag32|=(1<<19);
					}
				}
				return cnt;
			}
			cnt++;
		}
	}

//NEW_NODE:

	//if (STONE_TYPE(stone)==eST_BLACK &&
	//		STONE_POS_X(stone)==0 &&
	//			STONE_POS_Y(stone)==0)
	//{
	//	Sleep(10);
	//}

	g_lastStone=stone;

	m_bIschanged=True;

	cSgfStoneInfo* currentInfo=(cSgfStoneInfo*)m_currentNode->GetData();
	cyiTreeNode* node = m_currentNode;//new cyiTreeNode((yiAnyValue)currentInfo);
	ret=1;

	if (bIsNoNew==False)
	{
		currentInfo=new cSgfStoneInfo(stone);
		node = new cyiTreeNode((yiAnyValue)currentInfo);
		m_currentNode->add(node);
		ret = (int)m_currentNode->GetChildList().count();
		m_currentNode = node;
		m_nTotalNodeCnt++;
		currentInfo->flag32|=(1<<19);
	}
	else
	{
		currentInfo->stone=stone;
	}



	if (stone==eST_INVALID)
	{
		return 0;
	}

	// 石の順番や上げ石を計算するため
	if (m_currentNode->GetParent()!=0)
	{
		cSgfStoneInfo* prevInfo = (cSgfStoneInfo*)(m_currentNode->GetParent()->GetData());

		if (prevInfo)
		{
			currentInfo->nBlackCatchedStone=prevInfo->nBlackCatchedStone;
			currentInfo->nBlackCatchedStone=prevInfo->nWhiteCatchedStone;

			currentInfo->nOrder=prevInfo->nOrder+1;
			currentInfo->stone = currentInfo->stone | MAKESTONE(0, currentInfo->nOrder, 0, 0);
		}
		else
		{
			currentInfo->nBlackCatchedStone=0;
			currentInfo->nBlackCatchedStone=0;

			currentInfo->nOrder=1;
			currentInfo->stone = currentInfo->stone | MAKESTONE(0, currentInfo->nOrder, 0, 0);
		}
	}

	// あげ石を計算する。
	if (nCatchedStone!=-1)
	{
		if (STONE_TYPE(stone)==eST_BLACK)
		{
			currentInfo->nBlackCatchedStone+=nCatchedStone;
		}
		else
		{

			currentInfo->nWhiteCatchedStone+=nCatchedStone;
		}
	}

	//if (m_hFileCashe)
	//{
	//	AddCashe(stone);
	//}

	return ret-1;

ADDED_STONE:
	// 順番と関係ない石
	{
		cSgfStoneInfo* stoneInfo = (cSgfStoneInfo*)m_currentNode->GetData();

		if (stoneInfo && stoneInfo->stone==eST_INVALID)
		{
			if (stoneInfo->m_AddStoneList==0)
				stoneInfo->m_AddStoneList=new cyiList();

			stoneInfo->m_AddStoneList->add(stone);
			return 0;
		}

		if (stoneInfo)
		{
			if (stoneInfo->m_AddStoneList==0)
			{
				bIsNodeCreate=True;
			}
			else
			{
				bIsNodeCreate=False;
			}
		}

		if (stoneInfo==0 || bIsNodeCreate==True)
		{
			stoneInfo=new cSgfStoneInfo(eST_INVALID);
			cyiTreeNode* node = new cyiTreeNode((yiAnyValue)stoneInfo);
			m_currentNode->add(node);
			m_currentNode=node;
			m_nTotalNodeCnt++;
		}
		
		if (stoneInfo->m_AddStoneList==0)
			stoneInfo->m_AddStoneList=new cyiList();

		stoneInfo->m_AddStoneList->add(stone);
	}
	return 0;
}

// -----------------------------------
int cSgfManager::AddLabel(int i, int j, wchar_t label)
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

	if (info==0)
	{
		// ラベルだけのツリー
		info = new cSgfStoneInfo(eST_INVALID);
		m_currentNode->SetData((yiAnyValue)info);
	}

	if (info->label==0)
	{
		info->label = new cyiList();
	}

	info->label->addref((void*)((i<<24)|(j<<16)|(0xFFFF&label)));

	m_bIschanged=True;
	return 0;
}

// -----------------------------------
int cSgfManager::RemoveLabel(int i, int j)
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

	if (info==0 || info->label==0)
		return 0;

	for (cyiNode* node=info->label->GetNodeTopFrom(0); node!=0; node=node->np)
	{
		label_t label = (label_t)node->ref;
		int x = label>>24;
		int y = (label>>16)&0xFF;

		if (x==i && y==j)
		{
			info->label->remove(node);
			return 1;
		}
	}
	return 0;
}

// -----------------------------------
int cSgfManager::AddComment(const char* pszComment, Bool bIsOverwrite)
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

	if (bIsOverwrite==True)
	{
		if (info==0)
		{
			info = new cSgfStoneInfo();
		}

		if (info->comment)
		{
			yifree(info->comment);
			info->comment=0;
		}
		info->comment = xstrtoheap((char*)pszComment, (char)0);
		m_bIschanged=True;
		return 0;
	}
	int len = info->comment?strlen(info->comment):0;
	int newLen=strlen(pszComment);

	char* p = (char*)yialloc(len+newLen+4);

	if (info->comment)
	{
		memcpy(p, info->comment, len);
		yifree(info->comment);
		info->comment=0;
	}

	memcpy(&p[len], "\r\n", 2);
	memcpy(&p[len+2], pszComment, newLen);

	p[len+2+newLen]=0;

	info->comment=p;

	m_bIschanged=True;

	return 0;
}

// -----------------------------------
cyiTreeNode* cSgfManager::AddReference(const char* pszRefName, int& index)
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

	if (info==0)
		return 0;

	if (info->reference==0)
	{
		info->reference=new cyiList();
	}

	cyiTreeNode* pNode=m_currentNode;
	cSgfStoneInfo* pRefInfo = new cSgfStoneInfo(eST_INVALID);

	if (pRefInfo->m_AddStoneList==0)
		pRefInfo->m_AddStoneList=new cyiList();

	cyiTreeNode* refTree = new cyiTreeNode();
	refTree->SetData((yiAnyValue)pRefInfo);
	info->reference->addref(refTree);

	pRefInfo->referenceFrom=m_currentNode;//info->reference;

	
	index=info->reference->count()-1;

	return refTree;
}


// -----------------------------------
cyiTreeNode* cSgfManager::CreateReference(const char* pszRefName, int& index)
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

	if (info==0)
		return 0;

	if (info->reference==0)
	{
		info->reference=new cyiList();
	}

	cyiTreeNode* pNode=m_currentNode;
	cSgfStoneInfo* pRefInfo = new cSgfStoneInfo();

	if (pRefInfo->m_AddStoneList==0)
		pRefInfo->m_AddStoneList=new cyiList();

	while(pNode)
	{
		cSgfStoneInfo* info = (cSgfStoneInfo*)pNode->GetData();	

		if (info==0)
		{
			pNode=pNode->GetParent();
			continue;
		}

		if (info->stone!=eST_INVALID)
		{
			pRefInfo->m_AddStoneList->add((yiAnyValue)info->stone);
		}
		
		if (info->m_AddStoneList)
		{
			for (cyiNode* node=info->m_AddStoneList->GetNodeTopFrom(); node; node=node->np)
			{
				pRefInfo->m_AddStoneList->add((yiAnyValue)node->data);
			}
		}

		pNode=pNode->GetParent();
	}

	cyiTreeNode* refTree = new cyiTreeNode();
	refTree->SetData((yiAnyValue)pRefInfo);
	info->reference->addref(refTree);

	pRefInfo->referenceFrom=m_currentNode;//info->reference;

	
	index=info->reference->count()-1;
	return refTree;
}

// -----------------------------------
Bool cSgfManager::AddNode(cyiTreeNode* node)
{
	Bool bIsTop=False;

	if (&m_node==m_currentNode)
	{
		bIsTop=True;
	}

	if (bIsTop==True)
	{
		m_currentNode=node;
		m_pNodeTop=m_currentNode;
	}
	else
	{
		m_currentNode->add(node);
	}

	return True;
}

// ほぼ定石辞典用
// 廃止予定
// -----------------------------------
void cSgfManager::AddCommentCode(int nCommentCode)
{
	//cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

	//if (info->exinfo==0)
	//{
	//	info->exinfo = (sSgfExtendInfo*)yialloci(sizeof(sSgfExtendInfo));
	//}
	//info->exinfo->comment_code=(unsigned char)nCommentCode;

	return ;
}

// -----------------------------------
void cSgfManager::SetOptionCode(int Option)
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

	if (info->exinfo==0)
	{
		info->exinfo = (sSgfExtendInfo*)yialloci(sizeof(sSgfExtendInfo));
	}
	info->exinfo->option=(unsigned char)Option;
}

// -----------------------------------
int cSgfManager::GetOptionCode()
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

	if (info->exinfo==0)
	{
		return -1;
	}
	return info->exinfo->option;
}

// -----------------------------------
void cSgfManager::SetStatus(eGoResultStatus status)
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

	if (info->exinfo==0)
	{
		info->exinfo = (sSgfExtendInfo*)yialloci(sizeof(sSgfExtendInfo));
	}
	info->exinfo->status=(unsigned char)status;
}

// -----------------------------------
eGoResultStatus cSgfManager::GetStatus()
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

	if (info->exinfo==0)
	{
		return (eGoResultStatus)-1;
	}
	return (eGoResultStatus)info->exinfo->status;
}

// -----------------------------------
void cSgfManager::SetFlag(int flag)
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

	if (info->exinfo==0)
	{
		info->exinfo = (sSgfExtendInfo*)yialloci(sizeof(sSgfExtendInfo));
	}
	info->exinfo->flag32=(unsigned int)flag;
}

// -----------------------------------
int cSgfManager::GetFlag()
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

	if (info->exinfo==0)
	{
		return -1;
	}
	return (eGoResultStatus)info->exinfo->flag32;
}

// -----------------------------------
int cSgfManager::GetDeadStone(int& nBlack, int& nWhite)
{
	cyiTreeNode* node = m_currentNode;

	if (node==0)
		return -1;

	cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();

	if (info==0)
		return -1;

	nBlack = info->nBlackCatchedStone;
	nWhite = info->nWhiteCatchedStone;
	
	return nBlack+nWhite;
}


// -----------------------------------
Bool cSgfManager::Delete(cyiTreeNode* node)
{
	if (node!=NULL)
	{
		m_currentNode = (cyiTreeNode*)node->GetParent();
		delete node;
		return True;
	}

	if (m_currentNode==NULL)
	{
		return True;
	}
	else if (m_currentNode->GetParent()==NULL)
	{
		return True;
	}

	cyiTreeNode* tempNode = m_currentNode;

	m_currentNode = (cyiTreeNode*)m_currentNode->GetParent();
	delete tempNode;

	return True;
}



// -----------------------------------
stone_t cSgfManager::Play(int nBranchIndex)
{
	if (m_currentNode==NULL)
	{
		return 0;
	}
	cyiList& list= (cyiList&)m_currentNode->GetChildList();

	if (list.count()==0)
	{
		cSgfStoneInfo* info = (cSgfStoneInfo* )m_currentNode->GetData();

		if (info==0 &&
				info->ref==0)
		{
			return STONE_EOF;
		}

		if (info->ref)
		{
			m_currentNode=info->ref;
			list = (cyiList&)m_currentNode->GetChildList();
		}
	}

	int cnt=0;
	for (cyiNode* pListNode=list.GetNodeTopFrom(0); pListNode!=NULL; pListNode=pListNode->np)
	{
		cyiTreeNode* t = (cyiTreeNode*)pListNode->data;
		cSgfStoneInfo* info = (cSgfStoneInfo*)t->GetData();

		if (info==0)
			continue;

		if (info->stone==eST_INVALID)
		{
			//otherNodeList.addref(node);
			continue;
		}

		if ((info->flag32&1)==1)
			continue;

		if (cnt++==nBranchIndex)
		{
			m_currentNode=t;
			//m_currentNode = t;//(cyiTreeNode*)pListNode->data;
			//m_stack.push((yiAnyValue)nBranchIndex);
			//cSgfStoneInfo* info = (cSgfStoneInfo* )m_currentNode->GetData();
			return info->stone;
		}
	}
	return eST_INVALID;
}

// -----------------------------------
stone_t cSgfManager::PlayByNode(cyiTreeNode* t)
{
	m_currentNode=t;

	cSgfStoneInfo* info = (cSgfStoneInfo*)t->GetData();

	if (info)
		return info->stone;

	return eST_INVALID;
}

// -----------------------------------
stone_t cSgfManager::Back()
{
	if (m_currentNode==NULL)
	{
		return STONE_TOP;
	}
	cyiTreeNode* node = (cyiTreeNode* )m_currentNode->GetParent();

	if (node==NULL)
	{
		return STONE_TOP;
	}

	m_currentNode=node;
	
	cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();
	
	return info->stone;
}

// -----------------------------------
char* cSgfManager::GetComment()
{
	cSgfStoneInfo* info = (cSgfStoneInfo* )m_currentNode->GetData();

	if (info)
	{
		return info->comment;
	}

	return m_pszRootComment;
}


//unsigned int	flags; 	// 31: Iam black. 
//						// 30-29: 00 unknown 10 white win, 01 black win, 11 draw.
//						// 28: is half score.
//						// 27-19: score.
//						// 18-15: handicap
//						// 14-6: last order.

// -----------------------------------
// x:0-4, y:5-9, 10: 1:black, 0:white, 11: pass
sSimpleGoData* cSgfManager::ExportSimpleGoData(int* size, int ver)
{

	cyiTreeNode* node = m_currentNode;
	cyiStack stack;
	
	if (node==0)
		return 0;

	stack.clear();
	while(node)
	{
		cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();

		// top?
		if (info==0 ||
				info->stone==eST_INVALID)
		{
			break;
		}

		stack.push((yiAnyValue)info->stone);
		node=node->GetParent();
	}

	//if (stack.count()==0)
	//{
	//	return 0;
	//}

	sSimpleGoData* godata	= (sSimpleGoData*)yialloci(size==0?sizeof(sSimpleGoData):(sizeof(sSimpleGoData)+(stack.count()<<1)+sizeof(unsigned short)));
	sSimpleGoHeader* header=(sSimpleGoHeader*)godata;
	int winer				= GET_WINER(m_header->flag32);
	int score				= GET_SCORE(m_header->flag32);

	//godata->hd.day			 = m_header->playDate;
	header->result			|= IAM_BLACK(m_header->condition)?0x80000000:0;
	//header->result			|= bIsCanceled==True?(1<<14):0;
	header->result			|= IS_NIGIRI(m_header->condition)?(1<<28):0;//HALF_SCORE(m_header->condition)?(1<<28):0;
	header->result			|= (score<<19);
	header->result			|= (winer<<29);
	header->result			|= (GET_INIT_STONE(m_header->condition)<<15);
	header->result			|= (stack.count()<<6);
	//header->result			|= ; version.



	godata->condition		= m_header->condition;
	godata->condition2		= m_header->condition2;
	godata->flag32			= m_header->flag32;
	godata->code			= ver==1?NGFX:NGFX2;

	if (size==0 ||
		stack.count()==0)
	{
		if (size)
			*size=sizeof(sSimpleGoData);
		return (sSimpleGoData*)header;
	}

	unsigned short* data	= 0;
	
	if (ver==1)
	{
		sSimpleGoDataV1* x=(sSimpleGoDataV1*)godata;
		data = x->stones;//(unsigned short*)yialloc(stack.count()<<1);
	}
	else
	{
		data = godata->stones;
	}

	if (m_header->pszPlace)
	{
		if (ver==1)
		{
			sSimpleGoDataV1* p = (sSimpleGoDataV1*)godata;
			strncpy(p->szPlace, m_header->pszPlace, 30);
		}
	}

	if (ver==2)
	{
		if (m_header->pszBlackName)
			strncpy(godata->szBlackName, m_header->pszBlackName, 16);

		if (m_header->pszWhiteName)
			strncpy(godata->szWhiteName, m_header->pszWhiteName, 16);

		if (m_header->pszEventName)
			strncpy(godata->szEventName, m_header->pszEventName, 30);
	}

	int cnt=0;
	yiAnyValue value;
	while(stack.pop(value))
	{
		eStoneType type = STONE_TYPE(value);
		int x = STONE_POS_X(value);
		int y = STONE_POS_Y(value);
		data[cnt]=0;

		if (type==eST_BLACK)
			data[cnt]=(1<<10);
		else if (type==eST_BPASS)
		{
			data[cnt]=(1<<10);
			data[cnt]|=(1<<11);
		}
		else if (type==eST_WPASS)
		{
			data[cnt]|=(1<<11);
		}
		else if (type!=eST_WHITE)
		{
			break;
		}

		data[cnt]|=x;
		data[cnt]|=(y<<5);
		cnt++;
	}

	data[cnt]=0xFFFF;

	if (ver==1)
	{
		*size =(cnt<<1)+sizeof(sSimpleGoDataV1);
	}
	else if (ver==2)
	{
		*size =(cnt<<1)+sizeof(sSimpleGoData);
	}

	return godata;

}
//						// 30-29: 00 unknown 10 white win, 01 black win, 11 draw.
//						// 28: is half score.
//						// 27-19: score.
//						// 18-15: handicap
//						// 14-6: last order.

// -----------------------------------
Bool cSgfManager::ImportSimpleGoData(sSimpleGoData* data, int size, const char* pszBlackName, const char* pszWhiteName)
{
	m_bIschanged=False;
	if (data->code!=0 &&
			data->code!=NGFX &&
				data->code!=0xABCD &&
					data->code!=NGFX2)
	{
		return False;
	}

	//if (size<sizeof(sSimpleGoData))
	//{
	//	return False;
	//}

	int n = (size-sizeof(sSimpleGoData))>>1;

	if (data->code!=NGFX2)
	{
		n = (size-sizeof(sSimpleGoDataV1));
	}

	if (n==0 ||
			n<0)
	{
		return False;
	}


	m_header = (sGoHeader*)yialloci(sizeof(sGoHeader));
	
	//if (data->hd.result&(1<<31))
	//{
	//	SET_IAM_BLACK(m_header->condition);
	//}

	//if (data->hd.result & (1<<14))
	//{
	//	SET_CANCELED(m_header->flag32);
	//}

	//if (data->hd.result & (1<<28))
	//{
	//	SET_HALF_SCORE(m_header->condition);
	//}

	m_header->condition		= data->condition;
	m_header->condition2	= data->condition2;
	m_header->flag32		= data->flag32;

	int nInitStone = GET_INIT_STONE(m_header->condition);//(data->hd.result>>15)&0x0F;

	SET_LINE_COUNT(m_header->flag32, 19);

	if (nInitStone)
	{
		POINT pt[9];
		xGetStoneList19(nInitStone, pt, IAM_BLACK(m_header->condition));

		for (int i=0; i<nInitStone; i++)
		{
			stone_t stone = MAKESTONE(eST_BLACK, 0, pt[i].x, pt[i].y);
			m_InitStone.addref((void*)stone);
		}
	}

	int score = ((data->hd.result>>19)&0xFF);
	SET_SCORE(m_header->flag32, score);

	int winer = (data->hd.result>>29)&0x03;

	if (winer==1)
	{
		SET_BLACK_WIN(m_header->flag32);
	}
	else if (winer==2)
	{
		SET_WHITE_WIN(m_header->flag32);
	}
	else if (winer==3)
	{
		SET_DRAW(m_header->flag32);
	}

	m_header->playDate = data->hd.day;

	if (data->code==NGFX2)
	{
		if (data->szBlackName[0]!=0)
			m_header->pszBlackName = yistrheap(data->szBlackName);
		if (data->szWhiteName[0]!=0)
			m_header->pszWhiteName = yistrheap(data->szWhiteName);

		if (data->szEventName[0]!=0)
			m_header->pszEventName = yistrheap(data->szEventName);
	}
	else
	{
		if (pszBlackName)
		{
			m_header->pszBlackName = yistrheap(pszBlackName);
		}

		if (pszWhiteName)
		{
			m_header->pszWhiteName = yistrheap(pszWhiteName);
		}
	}

	ToTop();

	unsigned short* stones = 0;

	if (data->code!=NGFX2)
	{
		sSimpleGoDataV1* x=(sSimpleGoDataV1*)data;
		stones=x->stones;
	}
	else
	{
		stones=data->stones;
	}

	for (int i=0; i<n; i++)
	{
		if (stones[i]==0xFFFF)
			break;
		int x = (stones[i]&0x1F);
		int y = (stones[i]>>5)&0x1F;
		eStoneType type = (stones[i]&(1<<10))?eST_BLACK:eST_WHITE;

		if (stones[i]&(1<<11))
		{
			if (type==eST_BLACK)
				type=eST_BPASS;
			else
				type=eST_WPASS;
		}

		stone_t stone = MAKESTONE(type, i, x, y);
		Add(stone, -1, False, True);
	}
	ToTop();
	m_bsNgf=True;
	m_bIschanged=False;

	return True;
}


// -----------------------------------
sStonePath* cSgfManager::CreatePath()
{
	 return 0 ;//xCreatePath(m_currentNode);

}


// -----------------------------------
int cSgfManager::GetCommentCode()
{
	//cSgfStoneInfo* info = (cSgfStoneInfo* )m_currentNode->GetData();

	//if (info)
	//{
	//	if (info->exinfo==0)
	//		return -1;

	//	return info->exinfo->comment_code;
	//}

	return -1;
}

// -----------------------------------
Bool cSgfManager::IsNextHasPass()
{
	cyiTreeNode* t=m_currentNode;
	for (cyiNode* node=t->GetChildList().GetNodeTopFrom(); node; node=node->np)
	{
		cyiTreeNode* nodeNext = (cyiTreeNode*)node->data;
		cSgfStoneInfo* info=(cSgfStoneInfo*)nodeNext->GetData();

		if (info==0)
			continue;

		eStoneType type = STONE_TYPE(info->stone);

		if (type==eST_BPASS ||
				type==eST_WPASS)
		{
			return True;
		}
	}
	return False;
}


stone_t cSgfManager::GetNextStone(stone_t stone)
{
	int x = STONE_POS_X(stone);
	int y = STONE_POS_Y(stone);
	cyiTreeNode* t=m_currentNode;

	for (cyiNode* node=t->GetChildList().GetNodeTopFrom(); node; node=node->np)
	{
		cyiTreeNode* nodeNext = (cyiTreeNode*)node->data;
		cSgfStoneInfo* info=(cSgfStoneInfo*)nodeNext->GetData();

		if (info==0)
			continue;

		int next_x = STONE_POS_X(info->stone);
		int next_y = STONE_POS_Y(info->stone);

		if (x==next_x &&
				y==next_y)
		{
			return info->stone;
		}
	}

	return stone;
}


// -----------------------------------
cyiList* cSgfManager::GetLabel()
{
	cSgfStoneInfo* info = (cSgfStoneInfo* )m_currentNode->GetData();

	if (info)
	{
		return info->label;
	}

	return 0;
}

// --------------------------------
cyiList* cSgfManager::GetAddedStone()
{
	if (m_currentNode==0)
		return 0;

	cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

	if (info==0)
		return 0;

	return info->m_AddStoneList;
}

// -----------------------------------
int cSgfManager::GetHideBranches(cyiList& list)
{
	list.clear();

	if (m_currentNode==NULL)
	{
		return 0;
	}
	int n = m_currentNode->GetChildList().count();

	if (n==0)
		return 0;

	for (cyiNode* node=m_currentNode->GetChildList().GetNodeTopFrom(0); node!=0; node=node->np)
	{
		cyiTreeNode* t=(cyiTreeNode*)node->data;
		cSgfStoneInfo* info = (cSgfStoneInfo*)t->GetData();
		eStoneType type = STONE_TYPE(info->stone);

		if (info->ref)
		{
			continue;
		}
		if (info->stone==eST_INVALID)
		{
			list.addref(t);
			continue;
		}
	}

	return list.count();
}

// -----------------------------------
long cSgfManager::GetBranchCount(aStoneList branches/*=0*/)
{
	cyiTreeNode* before=0;
	if (m_currentNode==NULL)
	{
		return 0;
	}

	if (branches==0)
	{
		return (long)m_currentNode->GetChildList().count();
	}

	branches[0]=STONE_EOF;
	int n = m_currentNode->GetChildList().count();

	if (n==0/* || n==1*/)
	{
		cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

		if (info==0 ||
				info->ref==0)
			return n;
		before=m_currentNode;
		m_currentNode=info->ref;
	}

	cyiList& list = m_currentNode->GetChildList();

	int cnt=0;

	//if (bIsHasStone==True)
	//{
	for (cyiNode* node=list.GetNodeTopFrom(0); node!=0; node=node->np)
	{
		cSgfStoneInfo* info = (cSgfStoneInfo*)((cyiTreeNode*)node->data)->GetData();
		eStoneType type = STONE_TYPE(info->stone);

		if (info==0)
		{
			continue;
		}
		
		// hide mode.
		if ((info->flag32&1)==1)
		{
			continue;
		}

		if (info->stone==eST_INVALID)
		{
			//otherNodeList.addref(node);
			continue;
		}

		branches[cnt++]= info->stone;
	}
	branches[cnt]=STONE_EOF;
	//}
	//else
	//{
	//	for (cyiNode* node=list.GetNodeTopFrom(0); node!=0; node=node->np)
	//	{
	//		cSgfStoneInfo* info = (cSgfStoneInfo*)((cyiTreeNode*)node->data)->GetData();
	//		if (info->stone!=eST_INVALID)
	//			continue;

	//		branches[cnt++]= info->stone;
	//	}
	//	branches[cnt]=STONE_EOF;
	//}

	if (before)
		m_currentNode=before;

	return cnt;
}

// -----------------------------------
Bool cSgfManager::ToTop()
{
	m_currentNode=m_pNodeTop;//&m_node;
	m_stack.clear();
	return True;
}

// -----------------------------------
Bool cSgfManager::PrevBranch(int cnt)
{
	cyiTreeNode* node = m_currentNode;

	if (cnt==-1)
		goto WHILE;

	for (int i=0; i<cnt; i++)
	{
		node = (cyiTreeNode*)node->GetParent();
		if (node==0)
			return False;
	}

	m_currentNode=node;
	return True;

WHILE:
	node = (cyiTreeNode*)node->GetParent();
	while(node)
	{
		if (node->GetChildList().count()!=1)
		{
			m_currentNode=node;
			break;
		}
		node = (cyiTreeNode*)node->GetParent();
	}

	return node ? True:False;
}

// -----------------------------------
void cSgfManager::Copy(cSgfManager& mgr)
{
	wsprintf(mgr.m_szTitle, "Copied %s", m_szTitle);
	mgr.ParseSgf(m_sgfRaw, m_blocksize);

	cyiStack tempStack;
	
	yiAnyValue val;
	
	while (m_stack.pop(val)==True)
	{
		tempStack.push(val);
	}

	cyiTreeNode* pNode = &m_node;

	while (tempStack.pop(val)==True)
	{
		m_stack.push(val);
		mgr.m_stack.push(val);
		pNode = (cyiTreeNode*)pNode->GetChildList().GetNodeTopFrom(val)->data;

		if (pNode==NULL)
			return ;

		m_currentNode = pNode;
	}
}

// -----------------------------------
Bool cSgfManager::IsNodeTop()
{
	return m_currentNode->GetParent()==0?True:False;
}

// -----------------------------------
Bool cSgfManager::IsNodeEnd()
{
	return m_currentNode->GetChildList().count()==0?True:False;
}


// -----------------------------------
Bool cSgfManager::SearchNextBranch()
{
	cyiTreeNode* node = m_currentNode;

	while(node)
	{
		int count = node->GetChildList().count();
		
		// End of tree
		if (count==0)
			return False;
		
		if (count==1)
		{
			node = (cyiTreeNode*)node->GetChildList().GetNodeTopFrom(0)->data;
			continue;
		}

		return True;
	}

	return False;
}

// -----------------------------------
Bool cSgfManager::SearchPrevBranch()
{
	cyiTreeNode* node = m_currentNode;

	while(node)
	{
		int count = node->GetChildList().count();
				
		if (count==1 || 
				count==0)
		{
			node = (cyiTreeNode*)node->GetParent();
			continue;
		}

		return True;
	}

	return False;
}

static void xSearchComment(cyiList* result, cyiTreeNode* node)
{
	if (node==0)
		return ;

	cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();

	if (info!=0 && info->comment!=0)
	{
		result->addref(node);
	}

	cyiList& list = node->GetChildList();

	if (list.count()==0)
		return ;

	for (cyiNode* n=list.GetNodeTopFrom(0); n!=0; n=n->np)
	{
		xSearchComment(result, (cyiTreeNode*)n->data);
	}

	return ;
}

// -----------------------------------
cyiList* cSgfManager::GetCommentNodeList()
{
	cyiList* list = new cyiList();

	xSearchComment(list, &m_node);

	if (list->count()==0)
	{
		delete list;
		return 0;
	}

	return list;
}

// -----------------------------------
void cSgfManager::ReleaseCommentNodeList(cyiList* list)
{
	if (list)
		delete list;
}

// -----------------------------------
cyiList* cSgfManager::GetNodeHistory(cyiTreeNode* node)
{
	cyiStack st;

	if (node==0)
		node=m_currentNode;

	for (cyiTreeNode* n=node; n!=0; n=n->GetParent())
	{
		cyiTreeNode* parent = n->GetParent();

		// 親がいない場合は、トップなので履歴がないということになる。
		if (parent==0)
		{
			break;
		}

		cyiList& list2 = parent->GetChildList();
		int nIndex=0;

		for (cyiNode* n2=list2.GetNodeTopFrom(0); n2!=0; n2=n2->np)
		{
			if ((cyiTreeNode*)n2->data==n)
			{
				break;
			}
			nIndex++;
		}
		
		st.push((yiAnyValue)nIndex);
	}

	// トップノード
	if (st.count()==0)
	{
		return 0;
	}

	cyiList* list = new cyiList();

	while(st.count())
	{
		int nIndex=0;
		if (st.pop((yiAnyValue&)nIndex)==True)
		{
			list->add((yiAnyValue)nIndex);
			continue;
		}

		delete list;
		return 0;
	}

	return list;

}

// -----------------------------------
void cSgfManager::ReleaseNodeHistory(cyiList* list)
{
	if (list)
		delete list;
}

// -----------------------------------
Bool cSgfManager::AddNewBranch(const char* brancheTag)
{
	cSgfStoneInfo* currentInfo=new cSgfStoneInfo(eST_INVALID);
	cyiTreeNode* node = new cyiTreeNode((yiAnyValue)currentInfo);

	currentInfo->comment = yistrheap(brancheTag);
	m_currentNode->add(node);
	m_currentNode = node;

	return True;
}

// ------------------------------------------------------
static void xRemoveFromParent(cyiTreeNode* node, int* cnt)
{
	if (node==0)
		return ;

	cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();


	//cyiTreeNode* parent = node->GetParent();

	//for (cyiNode* x=parent->GetChildList().GetNodeTopFrom(0); x; x=x->np)
	//{
	//	if (node==(cyiTreeNode*)x->data)
	//	{
	//		parent->GetChildList().remove(x);
	//		break;
	//	}
	//}

	if (info)
		delete info;

	node->GetChildList().clear();

	if (node->GetParent()!=0)
		delete node;
	*cnt--;
}

// ------------------------------------------------------
static void xDeleteNode(cyiTreeNode* node, int* cnt)
{
	if (node==0)
		return ;

	//if (node->GetParent()==0)
	//	return ;
	cSgfStoneInfo* info=(cSgfStoneInfo*)node->GetData();

	if (info->refNo)
	{
		// まず、自分を参照しているノードを参照なし状態にする。
		
		if (info->referenceChild)
		{
/*			for (cyiNode* node=info->referenceChild->GetNodeTopFrom(); node; node=node->np)
			{
				cyiTreeNode* t=(cyiTreeNode*)node->ref;
				cSgfStoneInfo* x = (cSgfStoneInfo*)t->GetData();
				if (x)
					x->ref=0;
			}*/
			delete info->referenceChild;
			info->referenceChild=0;
		}
	}


	for (cyiNode* n=node->GetChildList().GetNodeTopFrom(0); n; n=n->np)
	{
		cyiTreeNode* x = (cyiTreeNode*)n->data;
		xDeleteNode(x, cnt);
	}

	node->GetChildList().clear();

	//cSgfStoneInfo* info=(cSgfStoneInfo*)node->GetData();

	if (info)
	{
		delete info;
	}
	node->SetData(0);
	delete node;

	//xRemoveFromParent(node, cnt);

	//node->GetChildList().clear();
}

// -----------------------------------
Bool cSgfManager::Destroy()
{
	int cnt=0;
	for (cyiNode* node = m_node.GetChildList().GetNodeTopFrom(); node; node=node->np)
	{
		cyiTreeNode* xnode = (cyiTreeNode*)node->ref;
		DeleteNode(xnode);
		node=m_node.GetChildList().GetNodeTopFrom();

		if (node==0)
			break;
	}
	
	if (m_header)
		yifree(m_header);

	m_header=0;

	if (m_sgfRaw)
		yifree(m_sgfRaw);

	m_sgfRaw=0;

	return True;
}

// -----------------------------------
Bool cSgfManager::DeleteNode(cyiTreeNode* node)
{
	cyiTreeNode* parent = node->GetParent();

	if (parent==0)
		return True;

	int cnt=parent->GetChildList().count();

	for (cyiNode* x= parent->GetChildList().GetNodeTopFrom(0); x; x=x->np)
	//for (int i=0; i<cnt; i++)
	{
		if ((cyiTreeNode*)x->data==node)
		{
			parent->GetChildList().remove(x);
			break;
		}
	}

	xDeleteNode(node, &m_nTotalNodeCnt);
	//xRemoveFromParent(node);

	if (parent!=0)
		m_currentNode=parent;


	return True;
}

// -----------------------------------
Bool cSgfManager::DeleteChildren(cyiTreeNode* node)
{
	for (cyiNode* n = node->GetChildList().GetNodeTopFrom(); n; )
	{
		cyiNode* n2=n->np;
		cyiTreeNode* t = (cyiTreeNode*)n->data;
		DeleteNode(t);
		n=n2;
	}

	node->GetChildList().clear();

	return True;
}

// -----------------------------------
Bool cSgfManager::xSearchResult(cyiTreeNode* node)
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();

	if (node==0)
		return True;

	if (info==0)
	{
		goto SKIP_MAP;
	}

	eStoneType type = STONE_TYPE(info->stone);
	
	if (node->GetChildList().count()==0)
	{
		CreateMapImage(node);
		return True;
	}
	else if (info->flag32&0x40000000)
	{
		CreateMapImage(node);
	}
	else if (info->exinfo!=0)
	{
		if (info->exinfo->option!=0 ||
				/*info->exinfo->flag32!=0 ||*/
					info->exinfo->status!=0)
		{
			CreateMapImage(node);
			//return True;
		}
		//if (info->exinfo->flag3232
	}

SKIP_MAP:

	for (cyiNode* nodeList = node->GetChildList().GetNodeTopFrom(); nodeList; nodeList=nodeList->np)
	{
		cyiTreeNode* child = (cyiTreeNode*)nodeList->data;
		xSearchResult(child);
	}

	return True;
}

// -----------------------------------
Bool cSgfManager::SearchResult()
{
	//cyiTreeNode* m_node;

	return xSearchResult(&m_node);
}


#include "stonemap.h"

static char** xCreateMap(int nLine)
{
	char** p = (char**)yialloci(nLine*sizeof(char*));

	for (int i=0; i<nLine; i++)
		p[i] = (char*)yialloci(nLine*sizeof(char));

	return p;
}

static void xDeleteMap(char**p, int nLine)
{
	for (int i=0; i<nLine; i++)
		yifree(p[i]);

	yifree(p);
}

// -----------------------------------
void cSgfManager::CreateMapImage(cyiTreeNode* _node, Bool bIsNoremainResult, Bool* bHasAddStone/*=0*/)
{
	cyiStack tempStack;
	cyiTreeNode* node;
	cStoneMap map;
	cSgfStoneInfo* curInfo = (cSgfStoneInfo*)_node->GetData();


	if (bHasAddStone)
	{
		*bHasAddStone=False;
	}

	if (curInfo==0)
		return ;

	if (curInfo->map)
		return ;

	map.Init();
	map.Clear();

	if (m_InitStone.count()!=0)
	{
		for (cyiNode* node=m_InitStone.GetNodeTopFrom(); node; node=node->np)
		{
			stone_t stone=(stone_t)node->ref;
			aStoneList list;
			map.SetMap(stone, list);
		}
	}

	for (cyiTreeNode* node = _node; node; node=node->GetParent())
	{
		tempStack.push((yiAnyValue)node);
	}

	if (tempStack.count()==0)
		return ;
	

	while(tempStack.pop((yiAnyValue&)node)==True)
	{
		cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();

		if (info==0)
			continue;

		if (info->m_AddStoneList)
		{
			if (bHasAddStone)
			{
				*bHasAddStone=True;
			}

			for (cyiNode* node=info->m_AddStoneList->GetNodeTopFrom(); node; node=node->np)
			{
				aStoneList list;
				stone_t stone=(stone_t)node->data;
				map.SetMap(stone, list);
			}
		}

		eStoneType type = STONE_TYPE(info->stone);

		if (type==eST_BLACK ||
				type==eST_WHITE)
		{
			aStoneList list;
			map.SetMap(info->stone, list);
		}
	}

	stone_t** stonez = map.GetMap();
	int n = GET_LINE_COUNT(m_header->flag32);


	if (curInfo->map!=0)
	{
		xDeleteMap(curInfo->map, n);
	}

	curInfo->map = xCreateMap(n);
	for (int i=0; i<n; i++)
	{
		for (int j=0; j<n; j++)
		{
			if (stonez[i]==0)
				continue;

			curInfo->map[i][j] = (char)stonez[i][j];
		}
	}

	if (bIsNoremainResult==False)
		m_ResultList.add((yiAnyValue)_node);
	return ;
}

// -----------------------------------
int cSgfManager::CreateMapImageEx(cyiTreeNode* _node, int nOrder, map_t _map)
{
	cyiStack tempStack;
	cyiTreeNode* node;
	cStoneMap map;
	int nCount=0;

	map.Init();
	map.Clear();

	if (m_InitStone.count()!=0)
	{
		for (cyiNode* node=m_InitStone.GetNodeTopFrom(); node; node=node->np)
		{
			stone_t stone=(stone_t)node->ref;
			aStoneList list;
			map.SetMap(stone, list);
		}
	}

	if (_node==0)
	{
		cyiTreeNode* node = &m_node;
		m_currentNode=node;
		while(1)
		{
			stone_t stone = Play(0);

			if (stone==eST_INVALID)
				return False;
			if (stone==0)
				return False;

			if (m_currentNode->GetChildList().count()==0)
			{
				nCount++;
				_node=m_currentNode;
				break;
			}

			//if (stone==eST_EOF)
			//{
			//	nCount++;
			//	_node=node;

			//	if (node->m_parent!=0)
			//	{
			//		break;
			//	}
			//}
			nCount++;
			if (nOrder!=-1)
			{
				if (nOrder==nCount)
				{
					break;
				}
			}
		}

	}

	for (cyiTreeNode* node = _node; node; node=node->GetParent())
	{
		tempStack.push((yiAnyValue)node);
	}

	if (tempStack.count()==0)
		return 0;
	

	while(tempStack.pop((yiAnyValue&)node)==True)
	{
		cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();

		if (info==0)
			continue;

		if (info->m_AddStoneList)
		{
			//if (bHasAddStone)
			//{
			//	*bHasAddStone=True;
			//}

			for (cyiNode* node=info->m_AddStoneList->GetNodeTopFrom(); node; node=node->np)
			{
				aStoneList list;
				stone_t stone=(stone_t)node->data;
				map.SetMap(stone, list);
			}
		}

		eStoneType type = STONE_TYPE(info->stone);

		if (type==eST_BLACK ||
				type==eST_WHITE)
		{
			aStoneList list;
			map.SetMap(info->stone, list);
		}
	}

	stone_t** stonez = map.GetMap();
	int n = GET_LINE_COUNT(m_header->flag32);

	memset(_map, 0, sizeof(_map));
	//int w;
	//for (int i=0; i<n; i++)
	//{
	//	for (int j=0; j<n; j++)
	//	{
	//		(w+j)
	//		stonez[i][j]<<?
	//	}
	//	w+=n;
	//}

	return nCount;
}

// -----------------------------------
void cSgfManager::CreateMapImage2(cyiTreeNode* _node, cStoneMap& map)
{
	cyiStack tempStack;
	cyiTreeNode* node;
	cSgfStoneInfo* curInfo = (cSgfStoneInfo*)_node->GetData();

	if (curInfo==0)
		return ;

	map.Init();
	map.Clear();

	for (cyiTreeNode* node = _node; node; node=node->GetParent())
	{
		tempStack.push((yiAnyValue)node);
	}

	if (tempStack.count()==0)
		return ;
	
	if (m_InitStone.count()!=0)
	{
		for (cyiNode* node=m_InitStone.GetNodeTopFrom(); node; node=node->np)
		{
			stone_t stone=(stone_t)node->ref;
			aStoneList list;
			map.SetMap(stone, list);
		}
	}

	while(tempStack.pop((yiAnyValue&)node)==True)
	{
		cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();


		if (info->m_AddStoneList)
		{
			for (cyiNode* node=m_InitStone.GetNodeTopFrom(); node; node=node->np)
			{
				stone_t stone=(stone_t)node->data;
				aStoneList list;
				map.SetMap(stone, list);
			}
		}

		if (info==0)
			continue;

		eStoneType type = STONE_TYPE(info->stone);

		if (type==eST_BLACK ||
				type==eST_WHITE)
		{
			aStoneList list;
			map.SetMap(info->stone, list);
		}
	}



}

// -----------------------------------
static Bool xIsMapSame(char** a, char** b, int nLineCnt)
{
	for (int i=0; i<nLineCnt; i++)
	{
		for (int j=0; j<nLineCnt; j++)
		{
			if (a[i][j]!=b[i][j])
				return False;
		}
	}
	return True;
}


// -----------------------------------
cyiTreeNode* cSgfManager::GetCurrentNode()
{
	return m_currentNode;
}

// -----------------------------------
static cyiList* xSerchNodeByLevel(cyiTreeNode* node, int level, int curLevel, cyiList* list)
{
	if (list==0)
	{
		list=new cyiList();
	}

	if (level==curLevel)
	{
		list->add((yiAnyValue)node);
		return list;
	}

	if (node->GetChildList().count()==0)
	{
		return list;
	}

	for (cyiNode* xNode=node->GetChildList().GetNodeTopFrom(); xNode; xNode=xNode->np)
	{

		xSerchNodeByLevel((cyiTreeNode*)xNode->data, level, curLevel+1, list);
	}

	return list;
}

// -----------------------------------
cyiList* cSgfManager::xSearchSameStoneMap(cyiTreeNode* node, int level)
{
	// まず、同levelのノードを全て検索
	cyiList* list = xSerchNodeByLevel(&m_node, level, 0, 0);
	cyiList* sameList = 0;

	int cnt = list->count();
	if (cnt==0 ||
			cnt==1)
	{
		delete list;
		return 0;
	}

	// TODO. SetLineCount
	cSgfStoneInfo* info=(cSgfStoneInfo*)node->GetData();
	Bool bHasAddStone=0;

	//if (info->reference)
	//{
	//}

	if (info->map==0)
	{
		CreateMapImage(node, True, &bHasAddStone);
	}

	if (bHasAddStone==True)
	{
		return 0;
	}

	int nLineCount = GET_LINE_COUNT(m_header->flag32);

	for (cyiNode* x = list->GetNodeTopFrom(); x; x=x->np)
	{
		cyiTreeNode* xx = (cyiTreeNode*)x->data;
		// TODO. SetLineCount
		cSgfStoneInfo* xinfo=(cSgfStoneInfo*)xx->GetData();

		if (xinfo->map==0)
		{
			CreateMapImage(xx, True);
		}
		//if (xx==node)
		//	continue;

		if (xIsMapSame(info->map, xinfo->map, nLineCount)==True)
		{
			if (sameList==0)
				sameList = new cyiList();
			sameList->add((yiAnyValue)xx);
		}
	}

	delete list;
	return sameList;
}

// -----------------------------------
static void xAddStonze(cyiTreeNode* dest,cyiTreeNode* src)
{
	int destCnt=dest->GetChildList().count();
	for (cyiNode* srcNode=src->GetChildList().GetNodeTopFrom(); srcNode;srcNode=srcNode->np)
	{
		cyiTreeNode* st=(cyiTreeNode*)srcNode->data;
		cSgfStoneInfo* srcInfo = (cSgfStoneInfo* )st->GetData();
		
		int cnt = destCnt;
		for (cyiNode* destNode=dest->GetChildList().GetNodeTopFrom(); cnt;cnt--)
		{
			cyiTreeNode* dt=(cyiTreeNode*)destNode->data;
			cSgfStoneInfo* destInfo = (cSgfStoneInfo* )dt->GetData();

			if (srcInfo->stone==destInfo->stone)
			{
				xAddStonze(dt, st);
				//DeleteNode(st);
				continue;
			}
			dest->GetChildList().add((yiAnyValue)st);
		}
	}
	src->GetChildList().clear();
}

int g_cnt=0;

// -----------------------------------
int cSgfManager::xOptimizeNode(cyiTreeNode* node, int level, int* nProgress, Bool* canceled)
{
	cyiTreeNode* nodeParent=node;
	cyiTreeNode* refNode=0;
	cyiList* list;

	if (node==0)
	{
		return 0;
	}
	xTRACE("cSgfManager::xOptimizeNode(%x, %d, %d): g_cnt:%d", node, level, *nProgress, g_cnt);

	cSgfStoneInfo* info = (cSgfStoneInfo*)node->GetData();
	
	if (info==0 ||
			info->refNo!=0 ||
				info->ref!=0 ||
					info->reference!=0 ||
						info->referenceFrom!=0)
	{
		goto SKIP_SET_REF;
	}
	
	while(nodeParent)
	{
		cSgfStoneInfo* info=(cSgfStoneInfo*)nodeParent->GetData();


		if (info &&
			info->m_AddStoneList)
		{
			goto SKIP_SET_REF;
		}
		nodeParent=nodeParent->GetParent();
	}

	list = xSearchSameStoneMap(node, level);
	if (list==0)
		goto SKIP_SET_REF;
	if (list->count()==1)
	{
		delete list;
		goto SKIP_SET_REF;
	}


	// まず、レファレンスノードがあるかをチェック
	for (cyiNode* xNode=list->GetNodeTopFrom(); xNode; xNode=xNode->np)
	{
		cyiTreeNode* x = (cyiTreeNode*)xNode->data;
		cSgfStoneInfo* info = (cSgfStoneInfo*)x->GetData();

		if (info->refNo!=0)
		{
			refNode=x;
		}
	}

	// まず、レファレンスノードがない場合
	// 自分がレファレンスノードとなる。
	if (refNode==0)
	{
		cyiNode* xNode=list->GetNodeTopFrom();
		refNode = (cyiTreeNode*)xNode->data;
		cSgfStoneInfo* info = (cSgfStoneInfo*)refNode->GetData();
		info->refNo=m_nMaxRefNo++;//m_RefList.count()+1;
		m_RefList.add((yiAnyValue)refNode);
	}

	for (cyiNode* xNode=list->GetNodeTopFrom(); xNode; xNode=xNode->np)
	{
		cyiTreeNode* x = (cyiTreeNode*)xNode->data;

		if (refNode==x)
			continue;

		cSgfStoneInfo* info = (cSgfStoneInfo*)x->GetData();
		info->ref=refNode;

		xAddStonze(refNode, x);
		//for (cyiNode* xxNode=x->GetChildList().GetNodeTopFrom(); xxNode; xxNode=xxNode->np)
		//{
		//	refNode->GetChildList().add((yiAnyValue)xxNode->data);
		//}
		//x->GetChildList().clear();
		//DeleteChildren(x);
	}

	delete list;

SKIP_SET_REF:
	(*nProgress)++;
	if (*canceled==True)
	{
		return -1;
	}

	if (node->GetChildList().count()==0)
	{
		return 0;
	}

	for (cyiNode* n = node->GetChildList().GetNodeTopFrom(); n; n=n->np)
	{
		cyiTreeNode* t = (cyiTreeNode*)n->data;
		if (xOptimizeNode(t, level+1, nProgress, canceled)==-1)
		{
			return -1;
		}
	}

	return 0;
}


// -----------------------------------
int cSgfManager::OptimizeNode(int* nProgress, Bool* canceled)
{
	int nStartCount = m_RefList.count();

	if (xOptimizeNode(&m_node, 0, nProgress, canceled)==-1)
		return -1;

	//if (SaveSgfFile(m_szFileName)==False)
	//{
	//	*canceled=True;
	//	return -1;
	//}

	//DeleteNode(&m_node);
	//
	//if (LoadSgfFile(m_szFileName)==False)
	//{
	//	*canceled=True;
	//	return -1;
	//}
	
	int nEndCount = m_RefList.count();
	//*canceled=True;
	return nEndCount-nStartCount;
}

// -----------------------------------
int cSgfManager::GetFullNodeCnt()
{
	return m_nTotalNodeCnt;
}

// -----------------------------------
int cSgfManager::CreateLink()
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

	if (info==0)
		return 0;

	info->linkNo=++m_nLinkNo;
	void* buffer[2] = {(void*)info->linkNo, (void*)m_currentNode};

	m_LinkList.add((yiAnyValue)yitoheap(buffer, sizeof(buffer)));

	return info->linkNo;
}

// -----------------------------------
int cSgfManager::DeleteLink()
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

	if (info==0)
		return 0;

	info->linkNo=0;

	return 0;
}

// -----------------------------------
int cSgfManager::AddLink(int no)
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

	if (info==0)
		return 0;

	if (info->link==0)
	{
		info->link=new cyiList();
	}

	info->link->add((yiAnyValue)no);

	return 0;
}

// -----------------------------------
int cSgfManager::RemoveLink(int no)
{
	cSgfStoneInfo* info = (cSgfStoneInfo*)m_currentNode->GetData();

	if (info==0)
		return 0;

	if (info->link==0)
	{
		return 0;
	}

	for (cyiNode* node=info->link->GetNodeTopFrom(); node; node=node->np)
	{
		int n = (int)node->data;

		if (n==no)
		{
			info->link->remove(node);
			break;
		}
	}

	return 0;
}

// -----------------------------------
cyiTreeNode* cSgfManager::SearchLink(int no)
{
	for (cyiNode* node = m_LinkList.GetNodeTopFrom();node; node=node->np)
	{
		void** buff = (void**)node->data;

		if (no==(int)buff[0])
		{
			cyiTreeNode* xnode=(cyiTreeNode*)buff[1];

			cSgfStoneInfo* info=(cSgfStoneInfo*)xnode->GetData();

			// リンク先が削除された場合
			if (info==0 || info->linkNo==0)
			{
				m_LinkList.remove(node);
				return 0;
			}
			
			return xnode;
		}
	}

	return 0;
}
