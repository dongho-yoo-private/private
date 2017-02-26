#include "yi.h"
#include <process.h>
#include "yctrlGoban.h"
#include "yistd.h"
#include "yiStream.h"
#include "yiBitmapBlender.h"
#include "yctrlGobanResource.h"




// ------------------------------------------------------------------
unsigned int __stdcall xOnDrawBackGround(void* param)
{
	cyctrlGoban* ctrl = (cyctrlGoban*)param;

	unsigned int res =  ctrl->_DrawBackGroundLayer();
	_endthreadex(0);

	return res;
}

// ------------------------------------------------------------------
void xOnDrawBackGroundx(void* param)
{
	cyctrlGoban* ctrl = (cyctrlGoban*)param;

	unsigned int res =  ctrl->_DrawBackGroundLayer();
	_endthread();

	return ;
}

// ------------------------------------------------------------------
unsigned int cyctrlGoban::_DrawBackGroundLayer(Bool bIsInit)
{
	yiMutexLockEx(m_hDrawMutex);
	//DrawMutexLock();
	Bitmap* bg = cyctrlGobanResource::GetInstance().GetImage(eRIDB_GOBAN);

	if (bIsInit)
	{
		m_BgLayer[0]->blt(0, 0, bg);
		m_BgLayer[1]->blt(0, 0, bg);
		yiMutexUnlockEx(m_hDrawMutex);
		m_nDrawedIndex=0;
		return 0;
	}

	cyiSurface* sur = m_BgLayer[1-m_UsedLayerIndex];
	

	if (bg)
	{
		sur->blt(0, 0, bg);
		m_nDrawedIndex=1-m_UsedLayerIndex;
	}

	yiMutexUnlockEx(m_hDrawMutex);
	//DrawMutexUnlock();
	return 0;
}


// ------------------------------------------------------------------
void cyctrlGoban::DrawBackGroundLayer()
{
	unsigned int Id;

	if (m_nDrawedIndex==(1-m_UsedLayerIndex))
	{
		return ;
	}
	//_beginthread(xOnDrawBackGroundx, 0, this);
	if (m_hBgThread)
		CloseHandle(m_hBgThread);

	m_hBgThread = (HANDLE)_beginthreadex(0, 0, xOnDrawBackGround, this, 0, &Id);

	//CloseHandle(hThread);

	return ;
}

// ------------------------------------------------------------------
void cyctrlGoban::DrawStonez(stone_t** stone, Bool bIsShowNo/*=False*/, int nNoFrom/*=0*/)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	int nLineCnt = eng->GetLineCount();
	cyiSurface* surf = m_BgLayer[1-m_UsedLayerIndex];
	Bitmap* bmpShadow = cyctrlGobanResource::GetInstance().GetImage(eRIDB_STONE_SHADOW); 
	Bitmap* bmpStoneB = cyctrlGobanResource::GetInstance().GetImage(eRIDB_BSTONE); 
	Bitmap* bmpStoneW = cyctrlGobanResource::GetInstance().GetImage(eRIDB_WSTONE); 
	Bool bCreateThread=False;

	m_UsedLayerIndex=1-m_UsedLayerIndex;

	if (stone==0)
	{
		stone=eng->GetStoneMap();
	}

	if (eng)
	{
		int n = eng->GetShowNo();

		if (n!=-1)
		{
			bIsShowNo=True;
		}
		nNoFrom=n;
	}

	//m_UsedLayerIndex=1-m_UsedLayerIndex;
	if (m_nCpuCount > 1)
	{
		//bCreateThread=True;
		//m_bReqiredLayerUpdate=False;
		//DrawBackGroundLayer();
		m_bReqiredLayerUpdate=True;
	}
	else
	{
		m_bReqiredLayerUpdate=True;
	}

	int w = bmpShadow->GetWidth();
	int h = bmpShadow->GetHeight();

	// 影を描画
	for (int i=0; i<nLineCnt; i++)
	{
		for (int j=0; j<nLineCnt; j++)
		{
			if (stone[i][j]!=STONE_NONE)
			{
				surf->blt(xCENTER(m_scaled_point[i], w), xCENTER(m_scaled_point[j], h), bmpShadow, eDA_LEFT_TOP);
			}
		}
	}

	w = bmpStoneB->GetWidth();
	h = bmpStoneB->GetHeight();
	// 碁石を描画
	for (int i=0; i<nLineCnt; i++)
	{
		for (int j=0; j<nLineCnt; j++)
		{
			switch((stone[i][j]&0xFFFF))
			{
				case STONE_BLACK:
					surf->blt(xCENTER(m_scaled_point[i], w), xCENTER(m_scaled_point[j], h), bmpStoneB, eDA_LEFT_TOP);
				break;

				case STONE_WHITE:
					surf->blt(xCENTER(m_scaled_point[i], w), xCENTER(m_scaled_point[j], h), bmpStoneW, eDA_LEFT_TOP);
				break;

				default:
					break;
			}
		}
	}

	// 最後の着手にしるしをつける
	{
		stone_t stone = eng->GetLastStone();
		int i=STONE_POS_X(stone);
		int j=STONE_POS_Y(stone);
		int type=STONE_TYPE(stone);


		if (type!=STONE_BLACK &&
				type!=STONE_WHITE)
			return ;

		Bitmap* bmpStoneTri = type==STONE_BLACK?cyctrlGobanResource::GetInstance().GetImage(eRIDB_BTRI):cyctrlGobanResource::GetInstance().GetImage(eRIDB_WTRI);

		surf->blt(xCENTER(m_scaled_point[i], bmpStoneTri->GetWidth()), xCENTER(m_scaled_point[j], bmpStoneTri->GetHeight()), bmpStoneTri, eDA_LEFT_TOP);
	}

	return ;
}

// ------------------------------------------------------------------
void cyctrlGoban::DrawStonezCalculate(stone_t** stonearea)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	int nLineCnt = eng->GetLineCount();
	cyiSurface* surf = m_BgLayer[1-m_UsedLayerIndex];
	Bitmap* bmpStoneB = cyctrlGobanResource::GetInstance().GetImage(eRIDB_BSTONE); 
	Bitmap* bmpStoneW = cyctrlGobanResource::GetInstance().GetImage(eRIDB_WSTONE); 
	Bitmap* bmpStoneBT = cyctrlGobanResource::GetInstance().GetImage(eRIDB_BSTONE_T);
	Bitmap* bmpStoneWT = cyctrlGobanResource::GetInstance().GetImage(eRIDB_WSTONE_T);
	Bitmap* bmpRedCross = cyctrlGobanResource::GetInstance().GetImage(eRIDB_RED_CROSS);

	Bitmap* bmpShadow = cyctrlGobanResource::GetInstance().GetImage(eRIDB_STONE_SHADOW);
	Bitmap* bmpBlackArea = cyctrlGobanResource::GetInstance().GetImage(eRIDB_AREA_B);
	Bitmap* bmpWhiteArea = cyctrlGobanResource::GetInstance().GetImage(eRIDB_AREA_W);


	m_UsedLayerIndex=1-m_UsedLayerIndex;
			m_bReqiredLayerUpdate=True;

	Bool bCreateThread=False;

	//m_UsedLayerIndex=1-m_UsedLayerIndex;
	if (m_nCpuCount > 1)
	{
		bCreateThread=True;
		m_bReqiredLayerUpdate=False;
		DrawBackGroundLayer();
	}
	else
	{
		m_bReqiredLayerUpdate=True;
	}

	stone_t** stone = eng->GetStoneMap();

	// 影を描画
	for (int i=0; i<nLineCnt; i++)
	{
		for (int j=0; j<nLineCnt; j++)
		{
			if (stone[i][j]!=STONE_NONE)
			{
				surf->blt(xCENTER(m_scaled_point[i], bmpShadow->GetWidth()), xCENTER(m_scaled_point[j], bmpShadow->GetHeight()), bmpShadow, eDA_LEFT_TOP);
			}
		}
	}

	// 碁石を描画
	for (int i=0; i<nLineCnt; i++)
	{
		for (int j=0; j<nLineCnt; j++)
		{
			int x = xCENTER(m_scaled_point[i], bmpStoneB->GetWidth());
			int y = xCENTER(m_scaled_point[j], bmpStoneB->GetHeight());


			switch(stone[i][j])
			{
				case STONE_BLACK:
					surf->blt(x, y, bmpStoneB, eDA_LEFT_TOP);
				break;
				case STONE_WHITE:
					surf->blt(x, y, bmpStoneW, eDA_LEFT_TOP);
				break;
			}
			//	case STONE_BLACK_DEAD:
			//		// 今のところはとりあえず、透明な石にする。
			//		surf->blt(x, y,  bmpStoneWT, eDA_LEFT_TOP);
			//		surf->blt(xCENTER(m_scaled_point[i], bmpRedCross->GetWidth()), xCENTER(m_scaled_point[j], bmpRedCross->GetHeight()), bmpRedCross, eDA_LEFT_TOP);
			//		break;
			//	case STONE_WHITE_DEAD:
			//		// 今のところはとりあえず、とめ否石にする。
			//		surf->blt(x, y,  bmpStoneBT, eDA_LEFT_TOP);
			//		surf->blt(xCENTER(m_scaled_point[i], bmpRedCross->GetWidth()), xCENTER(m_scaled_point[j], bmpRedCross->GetHeight()), bmpRedCross, eDA_LEFT_TOP);
			//		break;
			//	default:
			//		break;
			//}
		}
	}

	// テスト用
	// id描画
	//for (int i=0; i<nLineCnt; i++)
	//{
	//	for (int j=0; j<nLineCnt; j++)
	//	{
	//		//if (stone[i][j]==STONE_NONE)
	//		//{
	//			int id = stonearea[i][j]&0xFF;
	//			wchar_t ch[16];
	//			wsprintfW(ch, L"%d", id);
	//			Bitmap* bmp = cyiShape::BitmapString(0, ch, cyctrlGobanResource::GetInstance().GetFont(eRIDF_LABEL), &SolidBrush(Color::Black));
	//			surf->blt(xCENTER(m_scaled_point[i], bmp->GetWidth()), xCENTER(m_scaled_point[j], bmp->GetHeight()), bmp, eDA_LEFT_TOP);
	//			delete bmp;
	//		//}
	//	}
	//}

	// 石の領域を描画
	for (int i=0; i<nLineCnt; i++)
	{
		for (int j=0; j<nLineCnt; j++)
		{
			stone_t area=(stonearea[i][j]>>8)&0xFF;
			stone_t area_id=stonearea[i][j]&0xFF;

			if (area!=STONE_NONE)	
				surf->blt(xCENTER(m_scaled_point[i], bmpBlackArea->GetWidth()), xCENTER(m_scaled_point[j], bmpBlackArea->GetHeight()), area==STONE_BLACK?bmpBlackArea:bmpWhiteArea, eDA_LEFT_TOP);

			if (stone[i][j]!=STONE_NONE &&
					area_id!=0)
			{
				surf->blt(xCENTER(m_scaled_point[i], bmpRedCross->GetWidth()), xCENTER(m_scaled_point[j], bmpRedCross->GetHeight()), bmpRedCross, eDA_LEFT_TOP);
			}
		}
	}

}

// ------------------------------------------------------------------
void cyctrlGoban::DrawStonezCalculateEnd(stone_t** stone_area)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	int nLineCnt = eng->GetLineCount();
	cyiSurface* surf = m_BgLayer[m_UsedLayerIndex];

	Bitmap* bmpStoneB = cyctrlGobanResource::GetInstance().GetImage(eRIDB_BSTONE); 
	Bitmap* bmpStoneW = cyctrlGobanResource::GetInstance().GetImage(eRIDB_WSTONE); 

	Bitmap* bmpAreaB  = cyctrlGobanResource::GetInstance().GetImage(eRIDB_AREA_W);
	Bitmap* bmpAreaW = cyctrlGobanResource::GetInstance().GetImage(eRIDB_AREA_W);
	Bitmap* bmpShadow = cyctrlGobanResource::GetInstance().GetImage(eRIDB_STONE_SHADOW);

	m_UsedLayerIndex=1-m_UsedLayerIndex;
			m_bReqiredLayerUpdate=True;

	stone_t** stone = eng->GetStoneMap();

	Bool bCreateThread=False;

	//m_UsedLayerIndex=1-m_UsedLayerIndex;
	if (m_nCpuCount > 1)
	{
		bCreateThread=True;
		m_bReqiredLayerUpdate=False;
		DrawBackGroundLayer();
	}
	else
	{
		m_bReqiredLayerUpdate=True;
	}

	// 影を描画
	for (int i=0; i<nLineCnt; i++)
	{
		for (int j=0; j<nLineCnt; j++)
		{
			if (stone_area[i][j]!=0)
				continue;

			if (stone[i][j]!=STONE_NONE)
			{
				surf->blt(xCENTER(m_scaled_point[i], bmpShadow->GetWidth()), xCENTER(m_scaled_point[j], bmpShadow->GetHeight()), bmpShadow, eDA_LEFT_TOP);
			}
		}
	}

	// 碁石を描画
	for (int i=0; i<nLineCnt; i++)
	{
		for (int j=0; j<nLineCnt; j++)
		{
			float x = m_scaled_point[i];
			float y = m_scaled_point[j];
			if ((stone_area[i][j]&0xFF)==STONE_BLACK)
			{
				surf->blt(xCENTER(x, bmpAreaB->GetWidth()), xCENTER(y, bmpAreaB->GetHeight()), bmpAreaB, eDA_LEFT_TOP);
				continue;
			}
			else if (stone_area[i][j]&0xFF==STONE_WHITE)
			{
				surf->blt(xCENTER(x, bmpAreaB->GetWidth()), xCENTER(y, bmpAreaW->GetHeight()), bmpAreaW, eDA_LEFT_TOP);
				continue;
			}

			switch((stone[i][j]&0xFFFF))
			{
				case STONE_BLACK:
					surf->blt(xCENTER(x, bmpAreaB->GetWidth()), xCENTER(y, bmpStoneB->GetHeight()), bmpStoneB, eDA_LEFT_TOP);
				break;
				case STONE_WHITE:
					surf->blt(xCENTER(x, bmpAreaB->GetWidth()), xCENTER(y, bmpStoneW->GetHeight()), bmpStoneW, eDA_LEFT_TOP);
				break;
				default:
					break;
			}
		}
	}

	delete bmpAreaB;
	delete bmpAreaW;
}



// ------------------------------------------------------------------
void cyctrlGoban::DrawLabel(int i, int j, wchar_t label, stone_t stone)
{
	char* pszLabel = (char*)&label;
	wchar_t label_w[4]={0, 0, 0, 0};
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	Bitmap* bmpLabel=0;
	cyiSurface* surf = m_BgLayer[m_UsedLayerIndex];

	color_t color = stone==STONE_BLACK?Color::White:Color::Black;

	if (pszLabel[0]==(char)0)
	{
		char buffer[16];
		memcpy(buffer, &label, sizeof(wchar_t));
		yiToUnicode(&label_w[0], (const char*)buffer, eng->GetLocale());
	}

	if (HIBYTE(label)==0xFE)
	{
		bmpLabel = cyctrlGobanResource::GetInstance().GetLabelImage((eLabelType)label, color);
	}
/*	else if (label==eLS_TRIANGLE)
	{
		bmpLabel = cyctrlGobanResource::GetInstance().GetLabelImage(eLS_TRIANGLE, color);
	}
	else if (label==eLS_SQURE)
	{
		bmpLabel = new Bitmap(shape_size, shape_size);
		Graphics g(bmpLabel);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		g.DrawRectangle(&Pen(color, 3.0*m_scale), RectF(SHAPE_WIDTH, SHAPE_WIDTH, bmpLabel->GetWidth()-SHAPE_WIDTH*2, bmpLabel->GetHeight()-SHAPE_WIDTH*2));

	}
	else if (label==eLS_CIRCLE)
	{
		bmpLabel = new Bitmap(shape_size, shape_size);
		Graphics g(bmpLabel);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		g.DrawEllipse(&Pen(color, 3.0*m_scale), RectF(SHAPE_WIDTH, SHAPE_WIDTH, bmpLabel->GetWidth()-SHAPE_WIDTH*2, bmpLabel->GetHeight()-SHAPE_WIDTH*2));
		
	}*/
	else if (HIBYTE(label)==0xFF)
	{
		wchar_t buffer[16];
		wsprintfW(buffer, L"%d", LOBYTE(label));
		wcscpy(label_w, buffer);
	}
	else
	{
		char buffer[16]={0, };

		memcpy(buffer, &label, sizeof(wchar_t));
		yiToUnicode(&label_w[0], buffer, eng->GetLocale());
		label_w[0]=label;
	}

	if (bmpLabel==0)
	{
		unsigned int color = Color::Black;
		eShapeTextStyle style = (stone==STONE_NONE)?eShapeTextStyle::eSTS_DROP_SHADOW : eShapeTextStyle::eSTS_GROW;
		
		if (stone==STONE_BLACK)
		{
			color=Color::White;
		}
		else if (stone==STONE_WHITE)
		{
			color=Color::Black;
		}

		bmpLabel = cyiShape::BitmapString(0, label_w, cyctrlGobanResource::GetInstance().GetFont(eRIDF_LABEL), &SolidBrush(color), style, (0xFF000000|(~color))); 
	}

	Bitmap* goban = cyctrlGobanResource::GetInstance().GetImage(eRIDB_PURE_GOBAN);
	Bitmap* ret=bmpLabel;
	
	int x =xCENTER(m_scaled_point[i], bmpLabel->GetWidth());
	int y =xCENTER(m_scaled_point[j], bmpLabel->GetHeight());

	if (stone==STONE_NONE)
	{
		ret = new Bitmap(bmpLabel->GetWidth()+4, bmpLabel->GetHeight()+4, PixelFormat32bppARGB);
		Graphics g(ret);
		g.SetSmoothingMode(SmoothingModeHighQuality);
		g.DrawImage(goban, 0, 0, x, y, bmpLabel->GetWidth(), bmpLabel->GetHeight(), UnitPixel);
		g.DrawImage(bmpLabel, 0, 0);
	}


	surf->blt(x, y, ret, eDA_LEFT_TOP);

	delete bmpLabel;

	if (stone==STONE_NONE)
		delete ret;
}

// ------------------------------------------------------------------
void cyctrlGoban::DrawNo()
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (eng==0)
		return ;

	cyiSurface* surf = m_BgLayer[m_UsedLayerIndex];
	stone_t** stone=eng->GetStoneMap();
	int n = eng->GetShowNo();
	int nNoFrom;
	int nLineCnt = eng->GetLineCount();

	Bool bIsShowNo;
	if (n!=-1)
	{
		bIsShowNo=True;
	}
	nNoFrom=n;

	// 数字の表示
	if (bIsShowNo==True)
	{
		stone_t** order = eng->GetStoneOrderMap();
		stone_t laststone = eng->GetLastStone();
		int last_x = STONE_POS_X(laststone);
		int last_y = STONE_POS_Y(laststone);
		stone_t labelz[19][19];

		memset(labelz, 0, sizeof(labelz));

		int k=0;
		while(m_aCurrentLabelList[k]!=STONE_EOF)
		{
			int x = m_aCurrentLabelList[k]>>24;
			int y = (m_aCurrentLabelList[k]>>16)&0xFF;

			labelz[x][y]=1;
			k++;
		}

		for (int i=0; i<nLineCnt; i++)
		{
			for (int j=0; j<nLineCnt; j++)
			{
				wchar_t buffer[16];
				Bitmap* bmp;

				if (stone[i][j]==STONE_NONE)
					continue;

		
				//if (i==last_x &&
				//		j==last_y)
				//{
				//		continue;
				//}

				if (labelz[i][j]!=0)
				{
					continue;
				}

				if ((order[i][j]&0xFFFF)==0xFFFF)
				{
					continue;
				}

				if (stone[i][j]==STONE_BLACK)
				{
					wsprintfW(buffer, L"%d", order[i][j]+1);
					bmp = cyiShape::BitmapString(0, buffer, cyctrlGobanResource::GetInstance().GetFont(eGoResourceId::eRIDF_NO), &SolidBrush(Color::White), eShapeTextStyle::eSTS_GROW, (color_t)Color::Black);

				}
				else
				{
					wsprintfW(buffer, L"%d", order[i][j]+1);
					bmp = cyiShape::BitmapString(0, buffer, cyctrlGobanResource::GetInstance().GetFont(eGoResourceId::eRIDF_NO), &SolidBrush(Color::Black), eShapeTextStyle::eSTS_GROW, (color_t)Color::White);
				}

				surf->blt(xCENTER(m_scaled_point[i], bmp->GetWidth()), xCENTER(m_scaled_point[j], bmp->GetHeight()), bmp, eDA_LEFT_TOP);
				delete bmp;
			}
		}
	}
}

// ------------------------------------------------------------------
void cyctrlGoban::DrawBranch(int index, stone_t stone)
{
	wchar_t branch[16];
	int i=STONE_POS_X(stone);
	int j=STONE_POS_Y(stone);
	cyiSurface* surf = m_BgLayer[m_UsedLayerIndex];

	wsprintfW(branch, L"B%d", index);
	
	Bitmap* bmpBranch = cyiShape::BitmapString(0, branch, cyctrlGobanResource::GetInstance().GetFont(eRIDF_LABEL), &SolidBrush(Color::White), eShapeTextStyle::eSTS_DROP_SHADOW, 0xFFFFFFFF); 

	Bitmap* goban = cyctrlGobanResource::GetInstance().GetImage(eRIDB_PURE_GOBAN);
	Bitmap* ret = new Bitmap(bmpBranch->GetWidth(), bmpBranch->GetHeight(), PixelFormat32bppARGB);
	Graphics g(ret);

	int x =xCENTER(m_scaled_point[i], bmpBranch->GetWidth());
	int y =xCENTER(m_scaled_point[j], bmpBranch->GetHeight());

	g.SetSmoothingMode(SmoothingModeHighQuality);
	g.DrawImage(goban, 0, 0, x, y, bmpBranch->GetWidth(), bmpBranch->GetHeight(), UnitPixel);
	g.DrawImage(bmpBranch, 0, 0);

	surf->blt(x, y, ret, eDA_LEFT_TOP);

	delete bmpBranch;
	delete ret;
}

// ------------------------------------------------------------------
void cyctrlGoban::DrawPassBranch(int x, int y)
{
	cyiSurface* surf = m_BgLayer[m_UsedLayerIndex];
	Bitmap* bmpBranch = cyiShape::BitmapString(0, L"P", cyctrlGobanResource::GetInstance().GetFont(eRIDF_LABEL), &SolidBrush(Color::LightGreen), eShapeTextStyle::eSTS_DROP_SHADOW, 0xFFFFFFFF); 

	x =xCENTER(m_scaled_point[x], bmpBranch->GetWidth());
	y =xCENTER(m_scaled_point[y], bmpBranch->GetHeight());
	surf->blt(x, y, bmpBranch, eDA_LEFT_TOP);

	delete bmpBranch;
}

// ------------------------------------------------------------------
void cyctrlGoban::ClearCursor()
{
	m_prev_cursor_x=-1;
	m_prev_cursor_y=-1;

	if (m_bmpCursorBg)
		delete m_bmpCursorBg;

	m_bmpCursorBg=0;
}

// ------------------------------------------------------------------
// 直接デバイスコンテキストに描画するので、最後に呼び出す必要がある。
void cyctrlGoban::DrawCursor(HDC hdc, int i, int j, Bool bIsBlack)
{
	if (m_prev_cursor_x == i &&
			m_prev_cursor_y==j)
	{
		return ;
	}




	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
		return ;

	bIsBlack = eng->GetNextStone();
	Bitmap* cursor = cyctrlGobanResource::GetInstance().GetImage(bIsBlack?eRIDB_BSTONE_T:eRIDB_WSTONE_T);
	unsigned int w = cursor->GetWidth();
	unsigned int h = cursor->GetHeight();
	Bool bIsReleaseDC=False;



	if (eng->GetInputMode()==eIM_EDIT_ADDSTONE)
	{
		cursor= cyctrlGobanResource::GetInstance().GetImage(eRIDB_ADD_CURSOR);
		w=cursor->GetWidth();
		h=cursor->GetHeight();
	}

	// スケール調整とかぶってはいけない。
	//DrawMutexLock();

	//if (i==-1 &&
	//		j==-1)
	//{
	//	ChangeMouseCursor(0);
	//}

	if (hdc==0)
	{
		hdc = GetDC();
		bIsReleaseDC=True;
	}

	if (IsBranches(i, j)==True)
	{
		ChangeMouseCursor(1);
	}
	else
	{
		ChangeMouseCursor(0);
	}

	// 以前の背景を消す。
	// PutStoneやPutLabelの場合は、一度カーソルが消え、m_prev_cursorは、-1に設定される。
	// その理由は、カーソルが消えているために消す必要がないためである。
	if (m_prev_cursor_x!=-1 &&
			m_prev_cursor_y!=-1)
	{
		if (eng &&
				eng->GetInputMode()==eIM_LABEL ||
					eng->GetInputMode()==eIM_CALCULATE)
		{
			if (m_bmpCursorBg)
			{
				int w = m_bmpCursorBg->GetWidth();
				int h = m_bmpCursorBg->GetHeight();
				Graphics g(hdc);
				g.DrawImage(m_bmpCursorBg, (int)xCENTER(m_scaled_point[m_prev_cursor_x], w), (int)xCENTER(m_scaled_point[m_prev_cursor_y], h));
			}
		}
		else if (m_bmpCursorBg)
		{
			cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

			stone_t stone = eng->GetStone(m_prev_cursor_x, m_prev_cursor_y);

			if (stone==STONE_NONE)
			{
				Graphics g(hdc);
				g.DrawImage(m_bmpCursorBg, (int)xCENTER(m_scaled_point[m_prev_cursor_x], w), (int)xCENTER(m_scaled_point[m_prev_cursor_y], h));
			}
		}
	}

	// この場合は描画しない。(カーソルを隠す)
	if (i==-1 &&
			j==-1)
	{
		m_prev_cursor_x=-1;
		m_prev_cursor_y=-1;
		delete m_bmpCursorBg;
		m_bmpCursorBg=0;

		if (bIsReleaseDC)
			ReleaseDC(hdc);

		return ;
	}


	if (eng)
	{
		if (eng->GetInputMode()==eIM_CALCULATE)
		{
			Bitmap* bmpCross = cyctrlGobanResource::GetInstance().GetImage(eRIDB_RED_CROSS_CURSOR);
			stone_t** stone = eng->GetStoneMap();
			int w=bmpCross->GetWidth();
			int h=bmpCross->GetHeight();

			if (stone[i][j]!=STONE_NONE)
			{
				if (m_bmpCursorBg)
					delete m_bmpCursorBg;
				m_bmpCursorBg = cyiBitmapBlender::CreateBitmap(hdc, (int)xCENTER(m_scaled_point[i], w), (int)xCENTER(m_scaled_point[j], h), w, h);

				Graphics g(hdc);
				g.DrawImage(bmpCross, (int)xCENTER(m_scaled_point[i], w), (int)xCENTER(m_scaled_point[j], h));

				m_prev_cursor_x=i;
				m_prev_cursor_y=j;
			}
			else
			{
				m_prev_cursor_x=-1;
				m_prev_cursor_y=-1;
			}
			if (bIsReleaseDC)
				ReleaseDC(hdc);
			return ;
		}

		// ラベル入力モード時
		if (eng->GetInputMode()==eIM_LABEL)
		{
			Bitmap* bmpLabel=m_bmpCurrentLabel;

			for (int k=0; k<361; k++)
			{
				if (m_aCurrentLabelList[k]==STONE_EOF)
					break;

				int x = m_aCurrentLabelList[k]>>24;
				int y = (0xFF)&(m_aCurrentLabelList[k]>>16);

				// ラベルが重なる場合は描画しない。
				if (x==i && y==j)
				{
					m_prev_cursor_x=-1;
					m_prev_cursor_y=-1;

					if (bIsReleaseDC)
						ReleaseDC(hdc);

					return ;
				}
			}

			stone_t stone = eng->GetStone(i, j);

			if (stone==STONE_BLACK)
				bmpLabel=m_bmpCurrentLabelB;
			else if (stone==STONE_WHITE)
				bmpLabel=m_bmpCurrentLabelW;

			int w = bmpLabel->GetWidth();
			int h = bmpLabel->GetHeight();

			if (m_bmpCursorBg)
				delete m_bmpCursorBg;
			m_bmpCursorBg = cyiBitmapBlender::CreateBitmap(hdc, (int)xCENTER(m_scaled_point[i], w), (int)xCENTER(m_scaled_point[j], h), w, h);

			Graphics g(hdc);
			g.DrawImage(bmpLabel, (int)xCENTER(m_scaled_point[i], w), (int)xCENTER(m_scaled_point[j], h));

			m_prev_cursor_x=i;
			m_prev_cursor_y=j;

			if (bIsReleaseDC)
				ReleaseDC(hdc);

			return ;
		}

		// カーソルを描画する。
		if (eng->IsValidHit(MAKESTONE(bIsBlack?STONE_BLACK:STONE_WHITE, 0, i, j))==True)
		{
			Graphics g(hdc);
			// 描画の前に背景をコピー
			// ラベルがあれば、ラベルもコピーされる。

			if (m_bmpCursorBg)
				delete m_bmpCursorBg;
			m_bmpCursorBg = cyiBitmapBlender::CreateBitmap(hdc, (int)xCENTER(m_scaled_point[i], w), (int)xCENTER(m_scaled_point[j], h), w, h);

			g.DrawImage(cursor, (int)xCENTER(m_scaled_point[i], w), (int)xCENTER(m_scaled_point[j], h));

			m_prev_cursor_x=i;
			m_prev_cursor_y=j;
		}
		else
		{
			// 石を置けないところでは、-1に設定する。
			m_prev_cursor_x=-1;
			m_prev_cursor_y=-1;
		}
	}


	if (bIsReleaseDC)
		ReleaseDC(hdc);

	//DrawMutexUnlock();

}


//----------------------------------------
void cyctrlGoban::Update(eGobanLayer layer, stone_t** stone, Bool bIsShowNo, int nNoFrom)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (eng==0)
		goto NO_DRAW;

	yiMutexLockEx(m_hDrawMutex);
	if (layer & eGL_CALCULATE)
	{
		DrawStonezCalculate(stone);
		yiMutexUnlockEx(m_hDrawMutex);
		::InvalidateRect(m_hWnd, 0, FALSE);
		return ;
	}

	if (layer & eGL_CALCULATE_END)
	{
		DrawStonezCalculateEnd(stone);
		yiMutexUnlockEx(m_hDrawMutex);
		::InvalidateRect(m_hWnd, 0, FALSE);
		return ;
	}

	// 碁石を描画
	if (stone &&
		(layer & eGL_STONE))
	{
		DrawStonez(stone, bIsShowNo, nNoFrom);
	}

	m_aCurrentLabelList[0]=STONE_EOF;
	// ラベルを描画
	if (layer & eGL_LABEL)
	{
		cyiList* list = eng->GetLabel();
		stone_t mask[19][19];

		if (m_bLabelShow==False)
			list=0;

		memset(mask, 0, sizeof(mask));

		if (list &&
			list->count()>0)
		{
			if (stone==0)
			{
				stone=eng->GetStoneMap();
			}
			for (cyiNode* node=list->GetNodeTopFrom(0); node; node=node->np)
			{
				label_t label = (label_t)node->ref;
				wchar_t code = label&0xFFFF;
				int i = label>>24;
				int j = (label>>16)&0xFF;
				
				DrawLabel(i, j, code, stone[i][j]);
				mask[i][j]=code;
			}
		}

		stone_t PassStone;
		SetBranch(eng, &PassStone);
		int cnt=m_branches.count();
		while(m_branches.count())
		{
			int i,j;
			eStoneType type;
			stone_t stone_branch;
			m_branches.pop((yiAnyValue&)stone_branch);

			i = STONE_POS_X(stone_branch);
			j= STONE_POS_Y(stone_branch);
			type = STONE_TYPE(stone_branch);

			//if (type!=STONE_BLACK &&
			//		type!=STONE_WHITE)
			//{
			//	continue;
			//}

			if (mask[i][j]==0)
			{
				mask[i][j]=('B'<<16 | (char)cnt);
				DrawBranch(cnt--, stone_branch);
				continue;
			}
			cnt--;
		}

		if (PassStone)
		{
			int px=STONE_POS_X(PassStone);
			int py=STONE_POS_Y(PassStone);

			DrawPassBranch(px, py);		
		}

		int nLabelCnt=0;
		for (int i=0; i<19; i++)
		{
			for (int j=0; j<19; j++)
			{
				if (mask[i][j]!=0)
				{
					m_aCurrentLabelList[nLabelCnt++]=(i<<24)|(j<<16)|mask[i][j];
				}
			}
		}
		m_aCurrentLabelList[nLabelCnt]=STONE_EOF;

		// 番号表示
		DrawNo();
	}

	yiMutexUnlockEx(m_hDrawMutex);

NO_DRAW:
	// 実際の描画はWM_PAINTで行う。
	// flip
	//m_UsedLayerIndex=1-m_UsedLayerIndex;
	::InvalidateRect(m_hWnd, 0, FALSE);
}

// -----------------------------------------
BOOL cyctrlGoban::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
yiMutexLockEx(m_hDrawMutex);
	m_BgLayer[m_UsedLayerIndex]->Draw(hdc, 0, 0);

yiMutexUnlockEx(m_hDrawMutex);
	DrawBackGroundLayer();

	return TRUE;
}



//----------------------------------------
// Resize時のスケール変更
void cyctrlGoban::AdjustScale()
{
	yiMutexLockEx(m_hDrawMutex);
	
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	int nLineCnt = 19;

	if (eng!=0)
		nLineCnt=(int)eng->GetLineCount();
	
	m_scale = (float)m_nGobanWidth/(float)MAX_GOBAN_SIZE;
	m_nBlockSize = MAX_BLOCK_SIZE*m_scale;
	for (int i=0; i<nLineCnt; i++)
		m_scaled_point[i] = ((float)MAX_START_INDEX+(float)MAX_BLOCK_SIZE*(float)i)*m_scale+1;

	cyctrlGobanResource::GetInstance().SetBitmapScale(m_scale);

	delete m_BgLayer[0];
	delete m_BgLayer[1];

	int w = cyctrlGobanResource::GetInstance().GetImage(eRIDB_GOBAN)->GetWidth();
	int h = cyctrlGobanResource::GetInstance().GetImage(eRIDB_GOBAN)->GetHeight();
	
	m_BgLayer[0]=new cyiSurface(w, h);
	m_BgLayer[1]=new cyiSurface(w, h);

	_DrawBackGroundLayer(True);

	yiMutexUnlockEx(m_hDrawMutex);

	Update(eGL_ALL, eng->GetStoneMap());
	::InvalidateRect(m_hWnd, 0, FALSE);
}

	
// -------------------------------
Bool cyctrlGoban::ShowHideLabel()
{
	Bool bIsPrev = m_bLabelShow;
	m_bLabelShow=1-m_bLabelShow;
	Update(eGL_ALL, m_GoEnginez[m_nCurrentGoId]->GetStoneMap());
	return bIsPrev;
}

// ------------------------------------------------------
Bool cyctrlGoban::SetLabelMode(eLabelType type, wchar_t label)
{
	char* pszLabel = (char*)&label;
	wchar_t label_w[4] ={0, };
	int nDrawMode=0;
	Bitmap* bmpLabel;
	color_t color = Color::Black;
	color_t bgcolor = Color::White;
	eShapeTextStyle style = eShapeTextStyle::eSTS_NONE;
	cyiGoEngine* eng=m_GoEnginez[m_nCurrentGoId];
	float shape_size=m_nBlockSize*m_scale*0.8;

	if (eng==0)
		return False;

	if (type==eLT_LABLE_CLOSE)
	{
		eng->SetInputMode(eIM_PREV);

		return True;
	}
	
	//eng->SetInputMode(eIM_LABEL);

	if (label!=eLT_USER)
	{
		cyiList* list = m_GoEnginez[m_nCurrentGoId]->GetLabel();

		if (list==0 ||
				list->count()==0)
		{
			if (type==eLT_ROMA_LARGE)
				label=L'A';
			else if (type==eLT_ROMA_SMALL)
				label=L'a';
			else if (type==eLT_NUMERIC)
				label=MAKEWORD(1, 0xFF);
			else
			{
				label=type;
			}
			goto SKIP1;
		}

		if (type == eLT_ROMA_LARGE)
		{
			wchar_t max=0;
			for (cyiNode* node=list->GetNodeTopFrom(0); node; node=node->np)
			{
				unsigned short xx = (unsigned short)node->ref;
				wchar_t l = (wchar_t)(0xFFFF&xx);

				if (l >= L'A' &&
						l<='Z')
				{
					max = max>l?max:l;
				}
			}
			label = max ? max+1:L'A';
		}

		else if (type == eLT_ROMA_SMALL)
		{
			wchar_t max=0;
			for (cyiNode* node=list->GetNodeTopFrom(0); node; node=node->np)
			{
				unsigned short xx = (unsigned short)node->ref;
				wchar_t l = (wchar_t)(0xFFFF&xx);

				if (l >= L'a' &&
						l<='z')
				{
					max = max>l?max:l;
				}
			}
			label = max ? max+1:L'a';
		}

		else if (type == eLT_NUMERIC)
		{
			wchar_t max=0;
			for (cyiNode* node=list->GetNodeTopFrom(0); node; node=node->np)
			{
				unsigned short xx = (unsigned short)node->ref;
				wchar_t l = (wchar_t)(0xFFFF&xx);

				if (HIBYTE(l)==0xFF)
				{
					unsigned int no = LOBYTE(l);

					max = max>no?max:no;
				}
			}
			label = MAKEWORD(max+1, 0xFF);
		}
		else
			label = type;
	}

SKIP1:

	m_GoEnginez[m_nCurrentGoId]->SetInputMode(eIM_LABEL);

	if (m_bmpCurrentLabel)
		delete m_bmpCurrentLabel;

	if (m_bmpCurrentLabelB)
		delete m_bmpCurrentLabelB;

	if (m_bmpCurrentLabelW)
		delete m_bmpCurrentLabelW;
	
	m_CurrentLabel=label;
	m_eCurrentLabelType=type;

MAKE_LABEL:
	bmpLabel=0;

	if (nDrawMode==1)
	{
		color = Color::Black;
		bgcolor = Color::White;
		style = eShapeTextStyle::eSTS_GROW;		
	}
	else if (nDrawMode==2)
	{
		bgcolor = Color::Black;
		color = Color::White;
		style = eShapeTextStyle::eSTS_GROW;	
	}

	if (pszLabel[0]==(char)0)
	{
		char buffer[16];
		memcpy(buffer, &label, sizeof(wchar_t));
		yiToUnicode(&label_w[0], (const char*)buffer, eng->GetLocale());
	}
	else if (HIBYTE(label)==0xFE)
	{
		bmpLabel = cyctrlGobanResource::GetInstance().GetLabelImage((eLabelType)label,color);
	}
/*	else if (label==eLS_TRIANGLE)
	{
		Bitmap* tri = cyiShape::DrawRoundTriangle(0, &Pen(color, 2.0*m_scale), RectF(SHAPE_WIDTH, SHAPE_WIDTH, shape_size-SHAPE_WIDTH*2-1, shape_size-SHAPE_WIDTH*2-1), 0, 180);

		bmpLabel = cyiBitmapBlender::RotateCopy(0, tri, 2);
		delete tri;

	}
	else if (label==eLS_SQURE)
	{
		bmpLabel = new Bitmap(shape_size, shape_size);
		Graphics g(bmpLabel);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		g.DrawRectangle(&Pen(color, 3.0*m_scale), RectF(SHAPE_WIDTH, SHAPE_WIDTH, bmpLabel->GetWidth()-SHAPE_WIDTH*2, bmpLabel->GetHeight()-SHAPE_WIDTH*2));
	}
	else if (label==eLS_CIRCLE)
	{
		bmpLabel = new Bitmap(shape_size, shape_size);
		Graphics g(bmpLabel);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		g.DrawEllipse(&Pen(color, 3.0*m_scale), RectF(SHAPE_WIDTH, SHAPE_WIDTH, bmpLabel->GetWidth()-SHAPE_WIDTH*2, bmpLabel->GetHeight()-SHAPE_WIDTH*2));
		
	}*/
	// 数字
	else if (HIBYTE(label)==0xFF)
	{
		wchar_t buffer[16];
		wsprintfW(buffer, L"%d", LOBYTE(label));
		bmpLabel = cyiShape::BitmapString(0, buffer, cyctrlGobanResource::GetInstance().GetFont(eRIDF_LABEL), &SolidBrush(color), style, (0xFF000000|(~color))); 
	}
	else
	{
		char buffer[16]={0, };

		memcpy(buffer, &label, sizeof(wchar_t));
		yiToUnicode(&label_w[0], buffer, eng->GetLocale());
		label_w[0]=label;
	}

	if (bmpLabel==0)
	{
		bmpLabel = cyiShape::BitmapString(0, &label, cyctrlGobanResource::GetInstance().GetFont(eRIDF_LABEL), &SolidBrush(color), style, (0xFF000000|(~color))); 
	}

	if (nDrawMode==0)
	{
		m_bmpCurrentLabel=bmpLabel;
	}
	if (nDrawMode==1)
	{
		m_bmpCurrentLabelW=bmpLabel;
	}
	else if (nDrawMode==2)
	{
		m_bmpCurrentLabelB=bmpLabel;
		return True;
	}

	nDrawMode++;
	goto MAKE_LABEL;

	return True;
	
}