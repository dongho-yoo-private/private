#include "yctrlGoban.h"
#include "yctrlGobanResource.h"

// ----------------------------------------
static stone_t xChangeStoneCoordinate(stone_t stone, int nLineCnt)
{
	int x = STONE_POS_X(stone);
	int y = STONE_POS_Y(stone);
	int type = STONE_TYPE(stone);
	int index = STONE_ORDER(stone);

	x=nLineCnt-x-1;
	y=nLineCnt-y-1;
	return MAKESTONE(type, index, x, y);
}

// ---------------------------------------------------------------
LRESULT cyctrlGoban::OnCreate(WPARAM wp, LPARAM lp)
{
	RECT rect;
	GetClientRect(m_hWnd, &rect);



	// �h���b�O�A���h�h���b�v��L��
	DragAcceptFiles(m_hWnd, TRUE);

	if (m_GoEnginez[m_nCurrentGoId]==0)
	{
		unsigned int flag=0;
		unsigned int condition=	cyiGoEngine::DefaultCondition(flag);

		m_nCurrentGoId = AddForEdit("", "", "no title", condition, flag);
	}

	return 0;
}

// ---------------------------------------------------------------
LRESULT cyctrlGoban::OnDropFiles(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	HDROP hDrop=(HDROP)wParam;
	char buffer[MAX_PATH];
	
	DragQueryFile(hDrop, 0, (LPSTR)buffer, MAX_PATH);
	DragFinish(hDrop);

	goid_t id = AddForEditByFile(buffer);
	
	PostMessage(m_hOwner, WM_GOBAN_ADDED, 0, (LPARAM)id);

	Update(eGL_ALL);
	
	return 0;
}

// ---------------------------------------------------------------
// �}�E�X�����ꂽ�ꍇ�A�J�[�\�����B���B
LRESULT cyctrlGoban::OnMouseLeave(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	DrawCursor(0, -1, -1, True);
	return 0;
}

// ---------------------------------------------------------------
// MouseMove���́A�J�[�\����\������B
LRESULT cyctrlGoban::OnMouseMove(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	POINT ps;
	int xPos   =  lParam & 0xFFFF;
	int yPos   = (lParam >> 16) & 0xFFFF;

	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (eng==0)
		return 0;

	eInputMode InputMode = eng->GetInputMode();
	// �`�悵�Ȃ��P�[�X
	// 1. eIM_LOCK��
	// 2. �n���v�Z��
	// 3. �������A�����̔Ԃł͂Ȃ��Ƃ�
	
	if (InputMode==eIM_LOCK)
	{
		DrawCursor(0, -1, -1, True);
		return 0;
	}

	// �������Ȃ��B
	if (InputMode==eIM_FIND_COMMENT)
	{
		DrawCursor(0, -1, -1, True);
		return 0;
	}



	if (/*InputMode==eInputMode::eIM_CALCULATE ||*/
		InputMode==eInputMode::eIM_CALCULATE_END)
	{
		DrawCursor(0, -1, -1, True);
		return 0;
	}

	if (InputMode==eInputMode::eIM_PLAY)
	{
		Bool bIamBlack = IAM_BLACK(eng->GetHeader()->condition);
		stone_t stone = eng->GetLastStone();

		if (STONE_TYPE(stone)==STONE_BLACK && bIamBlack==True)
		{
			DrawCursor(0, -1, -1, True);
			return 0;
		}

		if (STONE_TYPE(stone)==STONE_WHITE && bIamBlack==True)
		{
			DrawCursor(0, -1, -1, True);
			return 0;
		}
	}



	if (GetNearCoordinate(xPos, yPos, &ps)==False)
	{
		DrawCursor(0, -1, -1, True);
		return 0;
	}


	if (InputMode==eIM_LABEL)
	{
		DrawCursor(0, ps.x, ps.y, False);
		return 0;
	}

	if (InputMode==eIM_INITSTONE_MODE)
	{
		DrawCursor(0, ps.x, ps.y, True);
		return 0;
	}

	DrawCursor(0, ps.x, ps.y, STONE_TYPE(eng->GetLastStone())==STONE_BLACK?True:False );

	return 0;
}

// ---------------------------------------------------------------
// �}�E�X�̍��N���b�N���A�}�E�X�̃{�^������w������Ă���΂�u���B
// �������A�΋ǃ��[�h���̂��ł���B�i�}�E�X�~�X��h�����߂Ɂj
LRESULT cyctrlGoban::OnLButtonUp(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (eng==0)
	{
		return 0;
	}

	eInputMode inputMode = eng->GetInputMode();
	eMouseMode mouseMode = eng->GetMouseMode();

	// �u�ΐݒ肪�I����ĂȂ��ꍇ�B
	if (IAM_BLACK(eng->GetHeader()->condition) &&
				eng->GetCurrentState()!=eCS_PLAY)
	{
		POINT ps;

		int xPos   =  lParam & 0xFFFF;
		int yPos   = (lParam >> 16) & 0xFFFF;
		eStoneType stoneType=STONE_TYPE(eng->GetLastStone());

		if (GetNearCoordinate(xPos, yPos, &ps)==False)
		{
			return 0;
		}

		eng->InitStone(ps.x, ps.y);

		stone_t send_stone = xChangeStoneCoordinate(MAKESTONE(STONE_BLACK, 0, ps.x, ps.y), eng->GetLineCount());
		PostMessage(m_hOwner, WM_INITSTONE, MAKELONG(m_nCurrentGoId, eng->GetRemoteId()), send_stone);
	}

	if (inputMode==eIM_PLAY)
	{
		POINT ps;

		int xPos   =  lParam & 0xFFFF;
		int yPos   = (lParam >> 16) & 0xFFFF;
		eStoneType stoneType=STONE_TYPE(eng->GetLastStone());

		if (GetNearCoordinate(xPos, yPos, &ps)==False)
		{
			return 0;
		}

		// �Q�x�ł��֎~�i�u�΃��[�h�́H�A���肪�p�X������ǂ�����H�j
		if ((stoneType==IAM_BLACK(eng->GetHeader()->condition))?STONE_BLACK:STONE_WHITE)
		{
			return 0;
		}

		stoneType = IAM_BLACK(eng->GetHeader()->condition) ? STONE_BLACK:STONE_WHITE;

		m_branches.clear();
		aStoneList list;
		stone_t** stonez = eng->PutStone(MAKESTONE(stoneType, 0, ps.x, ps.y), list);

		if (stonez==0)
		{
			// ����֎~�̏ꍇ
			return 0;
		}

		::PlaySound((LPCSTR)m_soundMove, GetModuleHandleA("yctrlGo.dll"), SND_ASYNC|SND_MEMORY);

		if (eng->GetShowNo()!=-1)
		{
			// ��΂̂�
			Update((eGobanLayer)(eGL_STONE|eGL_LABEL), stonez);
		}
		else
		{
			// ��΂̂�
			Update(eGL_STONE, stonez);
		}

		stone_t send_stone = xChangeStoneCoordinate(eng->GetLastStone(), eng->GetLineCount());
		PostMessage(m_hOwner, WM_PUTSTONE, MAKELONG(m_nCurrentGoId, eng->GetRemoteId()), send_stone);
	}

	return 0;
}


// ------------------------------------------------------
void cyctrlGoban::SetBrancheViewMode(eBranchViewMode mode/*=eBVM_NORMAL*/)
{
	m_eBranchViewMode=mode;
}

// ---------------------------------------------------------------
int cyctrlGoban::SetBranch(cyiGoEngine* eng, stone_t* stone)
{
	int n;
	aStoneList branches;
	branches[0]=STONE_EOF;
	int nUntil = m_eBranchViewMode==eBVM_NORMAL?1:0;


	if (m_eBranchViewMode==eBVM_HIDE)
	{
		m_branches.clear();
		return 0;
	}

	if (stone)
		*stone=0;

	if (eng==0){
		m_branches.clear();
		return 0;
	}

	if ((n=eng->GetBranchList(branches))>nUntil)
	{
		m_branches.clear();
		for (int i=0; branches[i]!=STONE_EOF; i++)
		{
			eStoneType type = STONE_TYPE(branches[i]);


			if (type==STONE_BLACK ||
					type==STONE_WHITE)
			{
				int x = STONE_POS_X(branches[i]);
				int y = STONE_POS_Y(branches[i]);

				//if (stone && x==0 && y==0)
				//{
				//	stone_t laststone=eng->GetLastStone();
				//	*stone = MAKESTONE(type, 0, STONE_POS_X(laststone), STONE_POS_Y(laststone));
				//	continue;
				//}
				m_branches.push((yiAnyValue)branches[i]);
			}

			else if (stone && (type==STONE_BLACK_PASS ||
					type==STONE_WHITE_PASS))
			{
				stone_t laststone=eng->GetLastStone();
				*stone = MAKESTONE(type, 0, STONE_POS_X(laststone), STONE_POS_Y(laststone));
			}

		}
	}

	return n;
}

// ---------------------------------------------------------------
Bool cyctrlGoban::IsBranches(int x, int y)
{
	int n;
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	stone_t last_stone;
	aStoneList branches;
	branches[0]=STONE_EOF;

	if (eng==0){
		return 0;
	}

	last_stone=eng->GetLastStone();

	if ((n=eng->GetBranchList(branches))>0)
	{
		m_branches.clear();
		for (int i=0; branches[i]!=STONE_EOF; i++)
		{
			eStoneType type = STONE_TYPE(branches[i]);

			if (type==STONE_BLACK ||
					type==STONE_WHITE)
			{
				int _x = STONE_POS_X(branches[i]);
				int _y = STONE_POS_Y(branches[i]);

				if (x==_x &&
						y==_y)
				{
					return True;
				}
			}
			else if (type==STONE_BLACK_PASS ||
							type==STONE_WHITE_PASS)
			{

				int _x = STONE_POS_X(last_stone);
				int _y = STONE_POS_Y(last_stone);

				if (x==_x &&
						y==_y)
				{
					return True;
				}
			}

		}
	}

	return False;
}



// ---------------------------------------------------------------
// �ҏW���[�h���́A�}�E�X�������ꂽ�^�C�~���O�ŕ`�悷��B
// 
LRESULT cyctrlGoban::OnLButtonDown(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	eInputMode inputMode;
	eMouseMode mouseMode;
	POINT ps;
	int xPos, yPos;

	// ���݁A��Ղ��Ȃ��ꍇ�͕ҏW���[�h�Ƃ��ĐV���Ȍ�Ղ��J��
	if (eng==0)
	{
		unsigned int flag=0;
		unsigned int condition=	cyiGoEngine::DefaultCondition(flag);

		m_nCurrentGoId = AddForEdit("", "", "no title", condition, flag);
		return 0;
	}


	inputMode = eng->GetInputMode();
	mouseMode = eng->GetMouseMode();

	// �n���v�Z�I�����[�h�Ȃ�΁A
	if (inputMode == eIM_CALCULATE_END)
	{
		//eng->SetInputMode(eIM_EDIT);

		//Update(eGL_ALL, eng->GetStoneMap());
		return 0;
	}

	// �R�����g�������[�h���́A�΂⃉�x����u���Ȃ��B
	// �������Ȃ��B
	if (inputMode==eIM_FIND_COMMENT)
	{
		return 0;
	}

	// ���݂̃}�E�X�|�C���g�����W�ɕϊ�
	xPos   =  lParam & 0xFFFF;
	yPos   = (lParam >> 16) & 0xFFFF;

	if (GetNearCoordinate(xPos, yPos, &ps)==False)
	{
		return 0;
	}

	// ���x�����̓��[�h���̓��x����`�悷��B
	if (inputMode == eIM_LABEL)
	{
		cyiList* list = eng->GetLabel();

		if (list!=0 && list->count()!=0)
		{
			for (cyiNode* node=list->GetNodeTopFrom(0); node!=0; node=node->np)
			{
				label_t label = (label_t)node->ref;
				int x = label>>24;
				int y = (label>>16)&0xFF;

				// ���Ƀ��x��������ꍇ�A�폜����B
				if (x==ps.x && y==ps.y)
				{
					eng->RemoveLabel(ps.x, ps.y);
					SetLabelMode(m_eCurrentLabelType);
					Update(eGL_ALL, eng->GetStoneMap());
					return 0;
				}

			}
		}

		// ���x������
		eng->PutLabel(ps.x, ps.y, m_CurrentLabel);
		
		m_prev_cursor_x=-1;
		m_prev_cursor_y=-1;

		SetLabelMode(m_eCurrentLabelType);
		Update(eGL_LABEL);
		return 0;
	}

	// �u���΃��[�h��
	if (inputMode == eIM_INITSTONE_MODE)
	{
		eng->InitStone(ps.x, ps.y);
		Update(eGL_ALL, eng->GetStoneMap());
		return 0;
	}

	// �Βǉ����[�h��
	if (inputMode==(int)eInputMode::eIM_EDIT_ADDSTONE)
	{
		aStoneList branches;
		stone_t** stonez=eng->PutStone(MAKESTONE(STONE_BLACK, 0, ps.x, ps.y), branches, True);
		// �����ŁA���������̃c���[�����ǂ�̂Ȃ�΁A
		// ���x�������邩�ǂ����A�R�����g�����邩�ǂ������`�F�b�N

		PostMessage(m_hOwner, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());

		cyiList* label=eng->GetLabel();

		// ���x��������ꍇ
		if (label!=0)
		{
			Update((eGobanLayer)(eGL_STONE|eGL_LABEL), stonez);
			return 0;
		}

		Update(eGL_STONE, stonez);
		return 0;
	}

	// �ʏ�̕ҏW���[�h
	if (inputMode == (int)eInputMode::eIM_EDIT)
	{
		if (mouseMode == eMouseMode::eMM_RBUTTON_RETURN)
		{
			stone_t laststone = eng->GetLastStone();
			eStoneType type=eng->GetNextStone() ? STONE_BLACK:STONE_WHITE;

			m_branches.clear();	
			aStoneList branches;
			stone_t** stonez=eng->PutStone(MAKESTONE(type, 0, ps.x, ps.y), branches);

			// ����֎~�ƌ��Ȃ��B
			if (stonez==0)
			{
				// ���̏ꍇ�́A����֎~�Ƃ��B�B�B�H
				return 0;
			}

			::PlaySound((LPCSTR)m_soundMove, GetModuleHandleA("yctrlGo.dll"), SND_ASYNC|SND_MEMORY);

			// �R�����g�̍X�V
			PostMessage(m_hOwner, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());

			int nBranchDetermin = m_eBranchViewMode==eBVM_SHOW_ONE_NODE?0:1;
			int nBrances=SetBranch(eng);
			// �}�����ꂵ�Ă���ꍇ�̏���

			cyiList* label=eng->GetLabel();

			::PostMessage(m_hOwner, WM_PUT_STONED, m_nCurrentGoId, (LPARAM)eng->GetCurrentNode());
			// ���x��������ꍇ
			if (label!=0 || nBrances >nBranchDetermin || eng->GetShowNo()!=-1)
			{
				Update((eGobanLayer)(eGL_STONE|eGL_LABEL), stonez);
				return 0;
			}

			Update(eGL_STONE, stonez);



		}
		// �p�~�\��
		else if (mouseMode == eMouseMode::eMM_RBUTTON_WHITE)
		{
			stone_t** stonez=eng->PutStone(MAKESTONE(STONE_BLACK, 0, ps.x, ps.y), 0);
			// �����ŁA���������̃c���[�����ǂ�̂Ȃ�΁A
			// ���x�������邩�ǂ����A�R�����g�����邩�ǂ������`�F�b�N

			// ����͌����v
			if (eng->GetComment()!=0)
			{
				PostMessage(m_hOwner, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());
			}

			cyiList* label=eng->GetLabel();

			// ���x��������ꍇ
			if (label!=0)
			{
				Update((eGobanLayer)(eGL_STONE|eGL_LABEL), stonez);
				return 0;
			}

			Update(eGL_STONE, stonez);
		}
		// �p�~�\��
		else if (eng->GetCurrentState()!=eCS_PLAY)
		{
			eng->InitStone(ps.x, ps.y);
			Update(eGL_STONE, eng->GetStoneMap());
			return 0;
		}
	}
	// �ϐ�Ȃǂ̃��[�h
	else if (inputMode == (int)eInputMode::eIM_ENJOY)
	{
		aStoneList list;
		stone_t** stonez = eng->Goto(1, list);
		cyiList* label = eng->GetLabel();

		// ���x��������ꍇ
		if (label!=0 || eng->GetShowNo()!=-1)
		{
			Update((eGobanLayer)(eGL_STONE|eGL_LABEL), stonez);
			return 0;
		}

		Update(eGL_STONE, stonez);

	}
	// �n�����v�Z��
	else if (inputMode == (int)eInputMode::eIM_CALCULATE)
	{
		// �n���v�Z���̃}�b�v���擾
		stone_t** stonez = eng->GetStoneMap();

		if (stonez[ps.x][ps.y]!=STONE_NONE)
		{
			stone_t stone = MAKESTONE(stonez[ps.x][ps.y], 0, ps.x, ps.y);
			stone_t** ret;

			aStoneList list;

			if ((ret=eng->SetDeadStone(stone))!=0)
			{
				Update(eGL_CALCULATE, ret);
			}

			if (eng->GetRemoteId()!=0)
			{
				stone_t send_stone = xChangeStoneCoordinate(stone, eng->GetLineCount());
				PostMessage(m_hOwner, WM_PUTSTONE, MAKELONG(m_nCurrentGoId, eng->GetRemoteId()), send_stone);
			}
			ClearCursor();
			return 0;
		}
	}

	return 0;
}

// ---------------------------------------------------------------
// �E�N���b�N���̋���
// 
LRESULT cyctrlGoban::OnRButtonDown(WPARAM wParam, LPARAM lParam, Bool& bIsHandled)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (eng==0)
		return 0;

	eInputMode inputMode = eng->GetInputMode();
	eMouseMode mouseMode = eng->GetMouseMode();

	// ���x�����̓��[�h�Ȃ�΁A�Ō�̃��x�����폜����B
	if (inputMode == eIM_LABEL)
	{
		cyiList* list = eng->GetLabel();

		if (list==0 ||
				list->count()==0)
		{
			return 0;
		}

		cyiNode* node = list->GetNodeTailFrom(0);
		list->remove(node);

		m_prev_cursor_x=-1;
		m_prev_cursor_y=-1;

		SetLabelMode(m_eCurrentLabelType);
		Update(eGL_ALL, eng->GetStoneMap());
		return 0;
	}

	POINT ps;

	int xPos   =  lParam & 0xFFFF;
	int yPos   = (lParam >> 16) & 0xFFFF;

	if (GetNearCoordinate(xPos, yPos, &ps)==False)
	{
		return 0;
	}

	// AB, AW���[�h��
	if (inputMode==eIM_EDIT_ADDSTONE)
	{
		aStoneList branches;
		stone_t** stonez=eng->PutStone(MAKESTONE(STONE_WHITE, 0, ps.x, ps.y), branches, True);
		// �����ŁA���������̃c���[�����ǂ�̂Ȃ�΁A
		// ���x�������邩�ǂ����A�R�����g�����邩�ǂ������`�F�b�N

		PostMessage(m_hOwner, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());

		cyiList* label=eng->GetLabel();

		// ���x��������ꍇ
		if (label!=0)
		{
			Update((eGobanLayer)(eGL_STONE|eGL_LABEL), stonez);
			return 0;
		}

		Update(eGL_STONE, stonez);
		return 0;
	}

	if (inputMode == (int)eInputMode::eIM_EDIT)
	{
		if (mouseMode == eMouseMode::eMM_RBUTTON_RETURN)
		{
			aStoneList list;
			stone_t** stonez = eng->Goto(-1, list);

			if (stonez==0)
				return 0;


			if (list[0]!=STONE_EOF)
			{
				m_branches.clear();
				for (int i=0; list[i]!=STONE_EOF; i++)
				{
					m_branches.push((yiAnyValue)list[i]);
				}
			}

			Update(eGL_ALL, stonez);

		}
		else if (mouseMode==eMouseMode::eMM_RBUTTON_WHITE)
		{
			POINT ps;

			int xPos   =  lParam & 0xFFFF;
			int yPos   = (lParam >> 16) & 0xFFFF;

			if (GetNearCoordinate(xPos, yPos, &ps)==False)
			{
				return 0;
			}
			stone_t stone = MAKESTONE(STONE_WHITE, 0, ps.x, ps.y);
			stone_t** stonez = eng->PutStone(stone, 0);

			if (stonez==0)
				return 0;

			// ����͌����v
			PostMessage(m_hOwner, WM_NODE_CHANGED, 0, (LPARAM)eng->GetCurrentNode());

			cyiList* label=eng->GetLabel();

			// ���x��������ꍇ
			if (label!=0)
			{
				Update((eGobanLayer)(eGL_STONE|eGL_LABEL), stonez);
				return 0;
			}

			Update(eGL_STONE, stonez);
		}
	}

	return 0;
}