#include "SgfManager.h"
#include "yctrlGoDefs.h"

#include <string.h>

//static void* wsxLoadFile(LPCSTR filename, long& size)
//{
//	HANDLE hFile = ::CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	if (hFile==NULL)
//	{
//		return NULL;
//	}
//
//	size = ::GetFileSize(hFile, NULL);
//
//	char* buffer = (char*)GlobalAlloc(GMEM_FIXED, size);
//	DWORD nRead;
//
//	if (::ReadFile(hFile, buffer, size, &nRead, NULL)==FALSE)
//	{
//		::CloseHandle(hFile);
//
//		return NULL;
//	}
//	::CloseHandle(hFile);
//
//	return buffer;
//}
//
//static void wsxUnloadFile(void* p)
//{
//	GlobalFree(p);
//}


cSgfManager::cSgfManager(void)
{
	m_node=NULL;
	m_sgfRaw=NULL;
}

cSgfManager::~cSgfManager(void)
{
}

// -----------------------------------
void cSgfManager::Init()
{
	//if (m_node!=NULL)
	//{
	//	delete m_node;
	//}

	//m_node = new cYotsuyaNode();
	m_currentNode = &m_node;

	if (m_sgfRaw!=NULL)
	{
		yiFileUnload(m_sgfRaw);
	}
}

// -----------------------------------
static char* xstrcpy(char* p, char* psrc, char end)
{
	int cnt=15;
	while(*psrc!=end && cnt!=0)
	{
		*p++=*psrc++;
		cnt--;
	}
	*p=0;

	return psrc;
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

	return xstrcpy(value, pIndex, ']');
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
			bIsDan=True;
			break;
		}
		else if (*pIndex=='d' ||
					*pIndex=='D')
		{
			break;
		}

		pIndex++;
	}

	// unknown.
	if (*pIndex==0)
	{
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
Bool cSgfManager::ParseSgfHeader(void* sgf, sGoHeader* header)
{
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

	if (FindTag(sgf, "KM[", value)==0)
	{
		FindTag(sgf, "KO[", value);
	}

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

	if (FindTag(sgf, "HA[", value)!=0)
	{
		if (yiatof(value, &fValue)==True)
		{
			if (nValue > 0 &&
					nValue < 32)
			{
				SET_HANDICAP(header->condition);
				SET_INIT_STONE(header->condition, nValue);
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
			header->pszBlackName = yistrheap(buffer);
		}

		// 場所
		if (FindTag(sgf, "PC[", buffer)!=0)
		{
			header->pszPlace = yistrheap(buffer);
		}

		// 段位
		if (FindTag(sgf, "BR[", buffer)!=0)
		{
			int level = xGetLevel(buffer);

			SET_BLACK_LEVEL(header->flag32, level);
		}
		if (FindTag(sgf, "WR[", buffer)!=0)
		{
			int level = xGetLevel(buffer);
			SET_WHITE_LEVEL(header->flag32, level);
		}

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
			else if (buffer[1]=='W')
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
				header->remainTime=sec;
			}
		}

		// 対局日時　DT[2005-09-04]
		if (FindTag(sgf, "DT[", buffer)!=0)
		{
			int year, month, date;

			sscanf(buffer, "%d-%d-%d", &year, &month, &date);
			header->playDate = ((year<< 12) | (month<<6) | date);
		}
	}

	return True;
}

// -----------------------------------
Bool cSgfManager::ParseSgf(void* sgf, int nSize)
{
	char* pIndex = (char*)sgf;

	pIndex = strstr(pIndex, ";");
	pIndex++;
	pIndex = strstr(pIndex, ";");

	ToTop();

	int cnt=0;
	while(1)
	{
		pIndex = strstr(pIndex, "B[");

		if (pIndex==NULL)
			break;

		stone_t stone = MAKESTONE(STONE_BLACK, cnt++, (int)(pIndex[2]-'a'), (int)(pIndex[3]-'a'));
		Add(stone, 0, NULL);

		pIndex++;
		pIndex = strstr(pIndex, "W[");

		if (pIndex==NULL)
			break;

		stone = MAKESTONE(STONE_WHITE, cnt++, (int)(pIndex[2]-'a'), (int)(pIndex[3]-'a'));
		Add(stone, 0, NULL);

		pIndex++;

	}

	return true;
}


#include "yiFile.h"

// -----------------------------------
Bool cSgfManager::LoadSgfFile(LPCSTR pszFileName)
{
	if (m_sgfRaw!=NULL)
	{
		yiFileUnload(m_sgfRaw);
	}

	m_bIsLoadSuccess=False;

	m_sgfRaw = yiFileLoad(pszFileName, (int*)&m_blocksize);

	if (m_sgfRaw==0)
	{
		return False;
	}

	Bool res = ParseSgfHeader(m_sgfRaw, &m_header);

	if (res==False)
	{
		yiFileUnload(m_sgfRaw);
		return False;
	}

	res = ParseSgf(m_sgfRaw, m_blocksize);

	if (res==False)
	{
		yiFileUnload(m_sgfRaw);
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
Bool cSgfManager::SaveSgfFile(LPCSTR pszFileName)
{
	
	return True;
}

// -----------------------------------
void cSgfManager::UnloadSgfFile(void* data)
{
	yiFileUnload(data);
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
void cSgfManager::InitStone(stone_t stone)
{
	m_InitStone.add((yiAnyValue)(new cSgfStoneInfo(stone, 0, (char*)0)));
	//if (m_hFileCashe)
	//{
	//	AddCashe(stone);
	//}
}

// -----------------------------------
int cSgfManager::Add(stone_t stone, char label, LPCSTR comment, BOOL bIsCurrentNodeFixed)
{
	int ret;

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
				return cnt;
			}
			cnt++;
		}
	}

	cyiTreeNode* node = new cyiTreeNode((yiAnyValue)(new cSgfStoneInfo(stone, label, (char*)comment)));

	m_currentNode->add(node);
	ret = (int)m_currentNode->GetChildList().count();
	m_currentNode = node;

	//if (m_hFileCashe)
	//{
	//	AddCashe(stone);
	//}

	return ret-1;
}

// -----------------------------------
bool cSgfManager::Delete(cyiTreeNode* node)
{
	if (node!=NULL)
	{
		m_currentNode = (cyiTreeNode*)node->GetParent();
		delete node;
		return true;
	}

	if (m_currentNode==NULL)
	{
		return true;
	}
	else if (m_currentNode->GetParent()==NULL)
	{
		return true;
	}

	cyiTreeNode* tempNode = m_currentNode;

	m_currentNode = (cyiTreeNode*)m_currentNode->GetParent();
	delete tempNode;

	return true;
}


// -----------------------------------
cyiTreeNode* cSgfManager::Play(int nBranchIndex)
{
	if (m_currentNode==NULL)
	{
		return NULL;
	}
	cyiList& list= (cyiList&)m_currentNode->GetChildList();

	int cnt=0;
	for (cyiNode* pListNode=list.GetNodeTopFrom(0); pListNode!=NULL; pListNode=pListNode->np)
	{
		if (cnt++==nBranchIndex)
		{
			m_currentNode = (cyiTreeNode*)pListNode->data;
			m_stack.push((yiAnyValue)nBranchIndex);
			return m_currentNode;
		}
	}
	return NULL;
}

// -----------------------------------
cyiTreeNode* cSgfManager::Back()
{
	if (m_currentNode==NULL)
	{
		return NULL;
	}
	cyiTreeNode* node = (cyiTreeNode* )m_currentNode->GetParent();

	if (node==NULL)
	{
		return NULL;
	}

	yiAnyValue n;
	m_stack.pop(n);

	return (m_currentNode=node);
}



// -----------------------------------
long cSgfManager::GetBranchCount()
{
	if (m_currentNode==NULL)
	{
		return 0;
	}

	return (long)m_currentNode->GetChildList().count();
}

// -----------------------------------
bool cSgfManager::GetBranches(stone_t stone_list[])
{
	if (m_currentNode==NULL)
	{
		return false;
	}
	cyiList& list = (cyiList& )m_currentNode->GetChildList();

	if (list.count()==0)
	{
		return false;
	}

	int cnt=0;
	for (cyiNode* pListNode=list.GetNodeTopFrom(0); pListNode!=NULL; pListNode=pListNode->np)
	{
		cyiTreeNode* treeNode = (cyiTreeNode*)pListNode->data;
		stone_list[cnt++] = ((cSgfStoneInfo*)treeNode->GetData())->stone;
	}
	stone_list[cnt]=STONE_EOF;
	return true;
}

// -----------------------------------
bool  cSgfManager::ToTop()
{
	m_currentNode=&m_node;
	m_stack.clear();
	return true;
}

// -----------------------------------
void cSgfManager::Copy(cSgfManager& mgr)
{
	wsprintf(mgr.m_szTitle, "Copied %s", m_szTitle);
	mgr.ParseSgf(m_sgfRaw, m_blocksize);

	cyiStack tempStack;
	
	yiAnyValue val;
	
	while (m_stack.pop(val)==true)
	{
		tempStack.push(val);
	}

	cyiTreeNode* pNode = &m_node;

	while (tempStack.pop(val)==true)
	{
		m_stack.push(val);
		mgr.m_stack.push(val);
		pNode = (cyiTreeNode*)pNode->GetChildList().GetNodeTopFrom(val)->data;

		if (pNode==NULL)
			return ;

		m_currentNode = pNode;
	}
}

