#include "yi.h"
#include <process.h>
#include "glsGoban.h"
#include "yistd.h"
#include "yiStream.h"
#include "yiBitmapBlender.h"
#include "yctrlGobanResource.h"

//#define __xSHOW_STONE_BLOCK__ 


// ------------------------------------------------------------------
unsigned int __stdcall xOnDrawBackGround(void* param)
{
	cglsGoban* ctrl = (cglsGoban*)param;

	unsigned int res =  ctrl->_DrawBackGroundLayer();
	_endthreadex(0);

	return res;
}

// ------------------------------------------------------------------
void xOnDrawBackGroundx(void* param)
{
	cglsGoban* ctrl = (cglsGoban*)param;

	unsigned int res =  ctrl->_DrawBackGroundLayer();
	_endthread();

	return ;
}

// ------------------------------------------------------------------
unsigned int cglsGoban::_DrawBackGroundLayer(Bool bIsInit)
{

	yiMutexLockEx(m_hDrawMutex);
	
	xTRACE("cglsGoban::_DrawBackGroundLayer() called");
	//DrawMutexLock();
	Bitmap* bg = cyctrlGobanResource::GetInstance().GetImage(eRIDB_GOBAN);

	if (bIsInit)
	{
		if (bg)
		{
			m_BgLayer[0]->blt(0, 0, bg);
			m_BgLayer[1]->blt(0, 0, bg);
		}
		yiMutexUnlockEx(m_hDrawMutex);
		m_nDrawedIndex=0;
		xTRACE("cglsGoban::_DrawBackGroundLayer() exit (init)");
		m_BgLayerFlag[0]=0;
		m_BgLayerFlag[1]=0;

		return 0;
	}

	cyiSurface* sur = m_BgLayer[1-m_UsedLayerIndex];
	

	if (bg)
	{
		sur->blt(0, 0, bg);
		m_BgLayerFlag[1-m_UsedLayerIndex]=0;
		//m_nDrawedIndex=1-m_UsedLayerIndex;

	}

	yiMutexUnlockEx(m_hDrawMutex);
	xTRACE("cglsGoban::_DrawBackGroundLayer() exit ");

	//DrawMutexUnlock();
	return 0;
}

// ----------------------------------------
Bitmap* cglsGoban::GetCurrentBitmap(sDrawItem* item)
{
	if (m_item.pCurrentBmp)
		delete m_item.pCurrentBmp;

	if (m_BgLayer[0]==0)
		return 0;

	m_item.pCurrentBmp = m_BgLayer[0]->Export(0, 0, 0, 0, 0, m_w, m_h);
	return m_item.pCurrentBmp;
}


// -----------------------------
void cglsGoban::Update(Bitmap* bmp, RECT* rect, RECT* _childRect, int offsetX, int offsetY, Bool bIsLocalBufferUpdate)
{
	//xTRACE("cglsGoban::Update %d, %d", GetDrawOrder(), m_UsedLayerIndex);
	RECT paintRect={_childRect->left, _childRect->top, _childRect->right+_childRect->left, _childRect->bottom+_childRect->top};
	cyiSurface* sur = m_BgLayer[0];

	if (sur==0)
		return ;

#ifdef NEW_STYLE
	//if (m_bluredBg)
	//{
	//	yiMutexLockEx(m_hDrawMutex);
	//	cyiBitmapBlender::BitmapCopy(bmp, m_bluredBg, rect->left, rect->top, _childRect->left, _childRect->top, _childRect->right, _childRect->bottom);
	//	yiMutexUnlockEx(m_hDrawMutex);
	//}
	//else
	//{
	sur->Export(bmp, rect->left, rect->top, _childRect->left, _childRect->top, _childRect->right, _childRect->bottom);
	//}
#else
		sur->Export(bmp, rect->left, rect->top, _childRect->left, _childRect->top, _childRect->right, _childRect->bottom);
#endif

yiMutexUnlockEx(m_hDrawMutex);

	//DrawBackGroundLayer();

	if (m_children==0)
		return ;

	for (cyiNode* node=m_children->GetNodeTopFrom(); node; node=node->np)
	{
		cGlassInterface* obj = (cGlassInterface*)node->ref;


		//if (obj->m_bIsNotOwnDraw==False)
		//{
		//	// WM_PAINTメッセージで処理するようにさせる。

		//	continue;
		//}

		// これで良いかどうか。。。
		if (IsWindowVisible(obj->m_hWnd)==False)
		{
			continue;
		}

		RECT crect = {obj->m_x, obj->m_y, obj->m_w, obj->m_h};
		RECT retRect;

		if (xIsInRect(&crect, _childRect, retRect)==False)
		{
			continue;
		}

		RECT xrect = {retRect.left+m_nAbsX-offsetX, retRect.top+m_nAbsY-offsetY, retRect.right, retRect.bottom};
		retRect.left-=obj->m_x;
		retRect.top-=obj->m_y;
#ifdef _DEBUG
		//xTRACE("iam:%s(%d), obj:%s(%d) Enter", m_szClassName, GetDrawOrder(), obj->m_szClassName, obj->GetDrawOrder());
#endif
		obj->Update(bmp, &xrect, &retRect, offsetX, offsetY);
#ifdef _DEBUG
		//xTRACE("iam:%s(%d), obj:%s(%d) Exit", m_szClassName, GetDrawOrder(), obj->m_szClassName, obj->GetDrawOrder());
#endif

		if (xIS_GLS_FLAG(obj->m_flag, eGF_REQUEST_UPDATE)==True)
		{
			//if (m_glsDrawInterface)
			//	m_glsDrawInterface->m_nRequestUpdateCount++;
			//else
			//	m_nRequestUpdateCount++;
			xOFF_GLS_FLAG(obj->m_flag, eGF_REQUEST_UPDATE);
		}
	}	

	//cyctrlbase::InvalidateRect(&paintRect);
}


// ------------------------------------------------------------------
void cglsGoban::DrawBackGroundLayer()
{
	unsigned int Id;

	return ;

	if (m_nDrawedIndex==(1-m_UsedLayerIndex))
	{
		return ;
	}
	//_beginthread(xOnDrawBackGroundx, 0, this);
	if (m_hBgThread)
	{
		CloseHandle(m_hBgThread);
		m_hBgThread=0;
	}

	m_hBgThread = (HANDLE)_beginthreadex(0, 0, xOnDrawBackGround, this, 0, &Id);

	//CloseHandle(hThread);

	return ;
}

// ------------------------------------------------------------------
void cglsGoban::DrawStonez(stone_t** stone, Bool bIsShowNo/*=False*/, int nNoFrom/*=0*/)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	int nLineCnt = eng->GetLineCount();
	cyiSurface* surf = m_BgLayer[0];//m_BgLayer[1-m_UsedLayerIndex];
	Bitmap* bmpShadow = cyctrlGobanResource::GetInstance().GetImage(eRIDB_STONE_SHADOW); 
	Bitmap* bmpStoneB = cyctrlGobanResource::GetInstance().GetImage(eRIDB_BSTONE); 
	Bitmap* bmpStoneW = cyctrlGobanResource::GetInstance().GetImage(eRIDB_WSTONE); 
	Bool bCreateThread=False;

	if (surf==0)
		return ;


	/*
	if (m_BgLayerFlag[1-m_UsedLayerIndex])
	{
		Bitmap* bg = cyctrlGobanResource::GetInstance().GetImage(eRIDB_GOBAN);
		if (bg)
		{
			m_BgLayer[1-m_UsedLayerIndex]->blt(0, 0, bg);
		}
		//m_BgLayerFlag[1-m_UsedLayerIndex]
	}

	m_UsedLayerIndex=1-m_UsedLayerIndex;
	*/

	Bitmap* bg = cyctrlGobanResource::GetInstance().GetImage(eRIDB_GOBAN);
	if (bg)
	{
		m_BgLayer[0]->blt(0, 0, bg);
	}

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
			if (stone[i][j]!=eST_NONE)
			{
				surf->blt(xCENTER(m_scaled_point[i].x, w)+1, xCENTER(m_scaled_point[j].y, h)+1, bmpShadow, eDA_LEFT_TOP);
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
				case eST_BLACK:
					surf->blt(xCENTER(m_scaled_point[i].x, w), xCENTER(m_scaled_point[j].y, h), bmpStoneB, eDA_LEFT_TOP);
				break;

				case eST_WHITE:
					surf->blt(xCENTER(m_scaled_point[i].x, w), xCENTER(m_scaled_point[j].y, h), bmpStoneW, eDA_LEFT_TOP);
				break;

				default:
					break;
			}
#ifdef	__xSHOW_STONE_BLOCK__
			{
				wchar_t szNum[5];
				int n = eng->GetBlockId(i, j);
				Bitmap* bmpStr=0;
				wsprintfW(szNum, L"%d", n);
				if ((stone[i][j]&0xFFFF)==eST_BLACK)
				{
					bmpStr=cyiShape::BitmapString(0, szNum, cyctrlGobanResource::GetInstance().GetFont(eRIDF_LABEL), &SolidBrush(Color::White));
				}
				else if ((stone[i][j]&0xFFFF)==eST_WHITE)
				{
					bmpStr=cyiShape::BitmapString(0, szNum, cyctrlGobanResource::GetInstance().GetFont(eRIDF_LABEL), &SolidBrush(Color::Black));
				}
				
				if (bmpStr)
				{
					surf->blt(xCENTER(m_scaled_point[i].x, bmpStr->GetWidth()), xCENTER(m_scaled_point[j].y, bmpStr->GetHeight()), bmpStr, eDA_LEFT_TOP);
					delete bmpStr;
				}

			}
#endif
		}
	}

	// 最後の着手にしるしをつける
	{
		stone_t stone = eng->GetLastStone();
		int i=STONE_POS_X(stone);
		int j=STONE_POS_Y(stone);
		int type=STONE_TYPE(stone);


		if (type!=eST_BLACK &&
				type!=eST_WHITE)
			return ;

		Bitmap* bmpStoneTri = type==eST_BLACK?cyctrlGobanResource::GetInstance().GetImage(eRIDB_BTRI):cyctrlGobanResource::GetInstance().GetImage(eRIDB_WTRI);

		surf->blt(xCENTER(m_scaled_point[i].x, bmpStoneTri->GetWidth()), xCENTER(m_scaled_point[j].y, bmpStoneTri->GetHeight()), bmpStoneTri, eDA_LEFT_TOP);
	}

	return ;
}

// ------------------------------------------------------------------
void cglsGoban::DrawStonezCalculate(stone_t** stonearea)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	int nLineCnt = eng->GetLineCount();
	cyiSurface* surf = m_BgLayer[0];//m_BgLayer[1-m_UsedLayerIndex];

	Bitmap* bg = cyctrlGobanResource::GetInstance().GetImage(eRIDB_GOBAN);
	if (bg)
	{
		surf->blt(0, 0, bg);
	}

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
		//DrawBackGroundLayer();
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
			if (stone[i][j]!=eST_NONE)
			{
				surf->blt(xCENTER(m_scaled_point[i].x, bmpShadow->GetWidth()), xCENTER(m_scaled_point[j].y, bmpShadow->GetHeight()), bmpShadow, eDA_LEFT_TOP);
			}
		}
	}

	// 碁石を描画
	for (int i=0; i<nLineCnt; i++)
	{
		for (int j=0; j<nLineCnt; j++)
		{
			int x = xCENTER(m_scaled_point[i].x, bmpStoneB->GetWidth());
			int y = xCENTER(m_scaled_point[j].y, bmpStoneB->GetHeight());


			switch(stone[i][j])
			{
				case eST_BLACK:
					surf->blt(x, y, bmpStoneB, eDA_LEFT_TOP);
				break;
				case eST_WHITE:
					surf->blt(x, y, bmpStoneW, eDA_LEFT_TOP);
				break;
			}
			//	case eST_BLACK_DEAD:
			//		// 今のところはとりあえず、透明な石にする。
			//		surf->blt(x, y,  bmpStoneWT, eDA_LEFT_TOP);
			//		surf->blt(xCENTER(m_scaled_point[i], bmpRedCross->GetWidth()), xCENTER(m_scaled_point[j], bmpRedCross->GetHeight()), bmpRedCross, eDA_LEFT_TOP);
			//		break;
			//	case eST_WHITE_DEAD:
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
	//		//if (stone[i][j]==eST_NONE)
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

			if (area!=eST_NONE)
			{
				surf->blt(xCENTER(m_scaled_point[i].x, bmpBlackArea->GetWidth()), xCENTER(m_scaled_point[j].y, bmpBlackArea->GetHeight()), area==eST_BLACK?bmpBlackArea:bmpWhiteArea, eDA_LEFT_TOP);
			}

			if (stone[i][j]!=eST_NONE &&
					area_id!=0)
			{
				surf->blt(xCENTER(m_scaled_point[i].x, bmpRedCross->GetWidth()), xCENTER(m_scaled_point[j].y, bmpRedCross->GetHeight()), bmpRedCross, eDA_LEFT_TOP);
			}
		}
	}

}

// ------------------------------------------------------------------
void cglsGoban::DrawStonezCalculateEnd(stone_t** stone_area)
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	int nLineCnt = eng->GetLineCount();
	cyiSurface* surf = m_BgLayer[0];//m_BgLayer[m_UsedLayerIndex];

	Bitmap* bg = cyctrlGobanResource::GetInstance().GetImage(eRIDB_GOBAN);
	if (bg)
	{
		surf->blt(0, 0, bg);
	}


	Bitmap* bmpStoneB = cyctrlGobanResource::GetInstance().GetImage(eRIDB_BSTONE); 
	Bitmap* bmpStoneW = cyctrlGobanResource::GetInstance().GetImage(eRIDB_WSTONE); 

	Bitmap* bmpAreaB  = cyctrlGobanResource::GetInstance().GetImage(eRIDB_AREA_B);
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

			if (stone[i][j]!=eST_NONE)
			{

				surf->blt(xCENTER(m_scaled_point[i].x, bmpShadow->GetWidth()), xCENTER(m_scaled_point[j].y, bmpShadow->GetHeight()), bmpShadow, eDA_LEFT_TOP);
			}
		}
	}

	// 碁石を描画
	for (int i=0; i<nLineCnt; i++)
	{
		for (int j=0; j<nLineCnt; j++)
		{
			float x = m_scaled_point[i].x;
			float y = m_scaled_point[j].y;

			stone_t area=(stone_area[i][j]>>8)&0xFF;
			stone_t area_id=stone_area[i][j]&0xFF;

			if (area==eST_BLACK)
			{
				surf->blt(xCENTER(x, bmpAreaB->GetWidth()), xCENTER(y, bmpAreaB->GetHeight()), bmpAreaB, eDA_LEFT_TOP);
				continue;
			}
			else if (area==eST_WHITE)
			{
				surf->blt(xCENTER(x, bmpAreaB->GetWidth()), xCENTER(y, bmpAreaW->GetHeight()), bmpAreaW, eDA_LEFT_TOP);
				continue;
			}


			if (stone[i][j]!=eST_NONE &&
					area_id!=0)
			{
				continue;
			}

			switch((stone[i][j]&0xFFFF))
			{
				case eST_BLACK:
					surf->blt(xCENTER(x, bmpStoneB->GetWidth()), xCENTER(y, bmpStoneB->GetHeight()), bmpStoneB, eDA_LEFT_TOP);
				break;
				case eST_WHITE:
					surf->blt(xCENTER(x, bmpStoneB->GetWidth()), xCENTER(y, bmpStoneW->GetHeight()), bmpStoneW, eDA_LEFT_TOP);
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
void cglsGoban::DrawLabel(int i, int j, wchar_t label, stone_t stone)
{
	char* pszLabel = (char*)&label;
	wchar_t label_w[4]={0, 0, 0, 0};
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	Bitmap* bmpLabel=0;
	cyiSurface* surf = m_BgLayer[0];//m_UsedLayerIndex];

	if (surf==0)
		return ;

	color_t color = stone==eST_BLACK?Color::White:Color::Black;

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
		eShapeTextStyle style = (stone==eST_NONE)?eShapeTextStyle::eSTS_DROP_SHADOW : eShapeTextStyle::eSTS_GROW;
		
		if (stone==eST_BLACK)
		{
			color=Color::White;
		}
		else if (stone==eST_WHITE)
		{
			color=Color::Black;
		}

		bmpLabel = cyiShape::BitmapString(0, label_w, cyctrlGobanResource::GetInstance().GetFont(eRIDF_LABEL), &SolidBrush(color), style, (0xFF000000|(~color))); 
	}

	Bitmap* goban = cyctrlGobanResource::GetInstance().GetImage(eRIDB_PURE_GOBAN);
	Bitmap* ret=bmpLabel;
	
	int x =xCENTER(m_scaled_point[i].x, bmpLabel->GetWidth());
	int y =xCENTER(m_scaled_point[j].y, bmpLabel->GetHeight());

	if (stone==eST_NONE)
	{
		ret = new Bitmap(bmpLabel->GetWidth(), bmpLabel->GetHeight(), PixelFormat32bppARGB);
		Graphics g(ret);
		g.SetSmoothingMode(SmoothingModeHighQuality);
		g.DrawImage(goban, 0, 0, x, y, bmpLabel->GetWidth(), bmpLabel->GetHeight(), UnitPixel);
		g.DrawImage(bmpLabel, 0, 0);
	}


	surf->blt(x, y, ret, eDA_LEFT_TOP);

	delete bmpLabel;

	if (stone==eST_NONE)
		delete ret;
}

// ------------------------------------------------------------------
void cglsGoban::DrawNo()
{
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (eng==0)
		return ;

	cyiSurface* surf = m_BgLayer[0];//m_UsedLayerIndex];
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

				if (stone[i][j]==eST_NONE)
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
				else if ((order[i][j]&0xFFFF)==0)
					continue;

				if (stone[i][j]==eST_BLACK)
				{
					wsprintfW(buffer, L"%d", order[i][j]);
					bmp = cyiShape::BitmapString(0, buffer, cyctrlGobanResource::GetInstance().GetFont(eGoResourceId::eRIDF_NO), &SolidBrush(Color::White), eShapeTextStyle::eSTS_DROP_SHADOW, (color_t)Color::Black);

				}
				else
				{
					wsprintfW(buffer, L"%d", order[i][j]);
					bmp = cyiShape::BitmapString(0, buffer, cyctrlGobanResource::GetInstance().GetFont(eGoResourceId::eRIDF_NO), &SolidBrush(Color::Black), eShapeTextStyle::eSTS_DROP_SHADOW, (color_t)Color::White);
				}

				surf->blt(xCENTER(m_scaled_point[i].x, bmp->GetWidth())+1, xCENTER(m_scaled_point[j].y, bmp->GetHeight())+1, bmp, eDA_LEFT_TOP);
				delete bmp;
			}
		}
	}
}

// ------------------------------------------------------------------
void cglsGoban::DrawBranch(int index, stone_t stone)
{
	cyiGoEngine* eng = GetCurrentGoEngine();

	if (eng==0)
		return ;

	cyiSurface* surf = m_BgLayer[0];//m_UsedLayerIndex];

	if (surf==0)
		return ;

	wchar_t branch[16];
	int i=STONE_POS_X(stone);
	int j=STONE_POS_Y(stone);

	//xSET_GOBAN_MODE(m_flag, detail->eContentsMode);
	if (m_detail.bIsOldInterface==False)
	{
		if (xGET_GOBAN_MODE(eng->m_flag)==eGCM_JOSEKI_EDITOR)
		{
			DrawLabel(i, j, eLS_GOOD,0);
			return ;
		}
	}




	wsprintfW(branch, L"B%d", index);
	
	color_t color= (STONE_TYPE(stone)==eST_WHITE)?Color::White:Color::Black;

	Bitmap* bmpBranch = cyiShape::BitmapString(0, branch, cyctrlGobanResource::GetInstance().GetFont(eRIDF_BRANCH), &SolidBrush(Color(color)), eShapeTextStyle::eSTS_DROP_SHADOW, 0xFFFFFFFF); 

	Bitmap* goban = cyctrlGobanResource::GetInstance().GetImage(eRIDB_PURE_GOBAN);
	Bitmap* ret = new Bitmap(bmpBranch->GetWidth(), bmpBranch->GetHeight(), PixelFormat32bppARGB);
	Graphics g(ret);

	int x =xCENTER(m_scaled_point[i].x, bmpBranch->GetWidth());
	int y =xCENTER(m_scaled_point[j].y, bmpBranch->GetHeight());

	g.SetSmoothingMode(SmoothingModeHighQuality);
	g.DrawImage(goban, 0, 0, x, y, bmpBranch->GetWidth(), bmpBranch->GetHeight(), UnitPixel);
	g.DrawImage(bmpBranch, 0, 0);

	surf->blt(x, y, ret, eDA_LEFT_TOP);

	delete bmpBranch;
	delete ret;
}

// ------------------------------------------------------------------
void cglsGoban::DrawPassBranch(int x, int y)
{
	cyiSurface* surf = m_BgLayer[0];//m_UsedLayerIndex];

	if (surf==0)
		return ;

	Bitmap* bmpBranch = cyiShape::BitmapString(0, L"P", cyctrlGobanResource::GetInstance().GetFont(eRIDF_LABEL), &SolidBrush(Color::LightGreen), eShapeTextStyle::eSTS_DROP_SHADOW, 0xFFFFFFFF); 

	x =xCENTER(m_scaled_point[x].x, bmpBranch->GetWidth());
	y =xCENTER(m_scaled_point[y].y, bmpBranch->GetHeight());
	surf->blt(x, y, bmpBranch, eDA_LEFT_TOP);

	delete bmpBranch;
}

// ------------------------------------------------------------------
void cglsGoban::ClearCursor()
{
	m_prev_cursor_x=-1;
	m_prev_cursor_y=-1;

	if (m_bmpCursorBg)
		delete m_bmpCursorBg;

	m_bmpCursorBg=0;
}

// ------------------------------------------------------------------
// 直接デバイスコンテキストに描画するので、最後に呼び出す必要がある。
void cglsGoban::DrawCursor(HDC hdc, int i, int j, Bool bIsBlack, unsigned int key, Bool bIsForced)
{
	cyiGoEngine* eng;
	unsigned int w;
	unsigned int h;
	//Bitmap* cursor=0;
	//Bool bIsReleaseDC=False;
	int nSelectedCursorBitmapId=1;

	xTRACE("cglsGoban::DrawCursor() called\n");
	if (bIsForced==False &&
		m_prev_cursor_x == i &&
			m_prev_cursor_y==j)
	{
		//m_cursor->Hide();
		xTRACE("cglsGoban::DrawCursor() xxxx\n");
		return ;
	}

	// この場合は描画しない。(カーソルを隠す)
	if (i==-1 &&
			j==-1)
	{
		m_prev_cursor_x=-1;
		m_prev_cursor_y=-1;
		//m_cursor->Hide();
		xTRACE("cglsGoban::DrawCursor() Hide\n");

		return ;
	}


	eng = m_GoEnginez[m_nCurrentGoId];

	if (!eng)
	{
		m_cursor->Hide();
		xTRACE("cglsGoban::DrawCursor() Hide\n");

		return ;
	}

	bIsBlack = eng->IsCurrentOrderBlack();
	if (bIsBlack==False)
		nSelectedCursorBitmapId=2;

	if (eng->GetInputMode()==eIM_CALCULATE)
	{
		nSelectedCursorBitmapId=3;
	}

	// ラベル入力モード時
	if (eng->GetInputMode()==eIM_LABEL)
	{
		return ;
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
				m_cursor->Hide();
				xTRACE("cglsGoban::DrawCursor() Hide ... label\n");
	
				return ;
			}
		}

		stone_t stone = eng->GetStone(i, j);

		if (stone==eST_BLACK)
			bmpLabel=m_bmpCurrentLabelB;
		else if (stone==eST_WHITE)
			bmpLabel=m_bmpCurrentLabelW;

		int w = bmpLabel->GetWidth();
		int h = bmpLabel->GetHeight();

		Bitmap* xbmp=m_cursor->SetBitmap(5, cyiBitmapBlender::BitmapCopy(0, bmpLabel), True);
		m_cursor->SelectBitmap(5, False);

		if (xbmp)
		{
			cyiBitmapBlender::DeleteBitmap(xbmp);
		}

		w = m_cursor->m_w;
		h = m_cursor->m_h;
		m_cursor->Move((int)xCENTER(m_scaled_point[i].x, w), (int)xCENTER(m_scaled_point[j].y, h));
		m_cursor->Show();
		m_prev_cursor_x=i;
		m_prev_cursor_y=j;
		return ;

	}

	if (bIsForced==True)
	{
		i=m_prev_cursor_x;
		j=m_prev_cursor_y;
	}

	if (key& MK_CONTROL) // 描画しない
	{
		m_cursor->Hide();
		return ;
	}

	if (key&MK_SHIFT ||
			eng->GetInputMode()==eIM_EDIT_ADDSTONE)
	{
		nSelectedCursorBitmapId=4;
	}

	m_cursor->SelectBitmap(nSelectedCursorBitmapId, False);

	w = m_cursor->m_w;
	h = m_cursor->m_h;


	if (IsBranches(i, j)==True)
	{
		ChangeMouseCursor(1);
	}
	else
	{
		ChangeMouseCursor(0);
	}

	if (eng->GetInputMode()==eIM_CALCULATE)
	{
		m_cursor->Show();
		m_cursor->Move((int)xCENTER(m_scaled_point[i].x, w), (int)xCENTER(m_scaled_point[j].y, h));
		m_prev_cursor_x=i;
		m_prev_cursor_y=j;
		return ;
	}

	// カーソルを描画する。
	if (eng->IsValidHit(MAKESTONE(bIsBlack?eST_BLACK:eST_WHITE, 0, i, j))==True)
	{
		m_cursor->Show();
		m_cursor->Move((int)xCENTER(m_scaled_point[i].x, w), (int)xCENTER(m_scaled_point[j].y, h));
		m_prev_cursor_x=i;
		m_prev_cursor_y=j;
		xTRACE("cglsGoban::DrawCursor() Show ... stone(%d, %d)\n", i,j);
	}
	else
	{
		if (eng->GetShowNo()!=-1)
		{
			m_cursor->Hide();
			m_prev_cursor_x=i;
			m_prev_cursor_y=j;
			return ;
		}

		stone_t stone = eng->GetStoneEx(i, j);
		eStoneType type = (eStoneType)STONE_TYPE(stone);

		if (type==eST_BLACK ||
				type==eST_WHITE)
		{
			int order=STONE_ORDER(stone);

			if (order!=0)
			{
				wchar_t strNo[5];


				wsprintfW(strNo, L"%d", order);
				Bitmap* bmp = cyiShape::BitmapString(0, strNo, cyctrlGobanResource::GetInstance().GetFont(eRIDF_NO), &SolidBrush(type==eST_BLACK?Color::White:Color::Black), eSTS_DROP_SHADOW);
				m_cursor->SetBitmap(7, bmp, True);
				m_cursor->SelectBitmap(7, False);

				int w=m_cursor->m_w;
				int h=m_cursor->m_h;
				m_cursor->Move((int)xCENTER(m_scaled_point[i].x, w)+1, (int)xCENTER(m_scaled_point[j].y, h)+1);
				m_cursor->Show();
			}
			else
			{
				m_cursor->Hide();
			}
		}
		else
			m_cursor->Hide();

	}

	m_prev_cursor_x=i;
	m_prev_cursor_y=j;
}


//----------------------------------------
void cglsGoban::Update(eGobanLayer layer, stone_t** stone, Bool bIsShowNo, int nNoFrom)
{
	xTRACE("cglsGoban::Update() layer! %d", GetDrawOrder());
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];

	if (eng==0)
		goto NO_DRAW;

	yiMutexLockEx(m_hDrawMutex);
	if (layer & eGL_CALCULATE)
	{
		DrawStonezCalculate(stone);
		yiMutexUnlockEx(m_hDrawMutex);
		InvalidateRect(0);
		//::InvalidateRect(m_hWnd, 0, FALSE);
		return ;
	}

	if (layer & eGL_CALCULATE_END)
	{
		DrawStonezCalculateEnd(stone);
		yiMutexUnlockEx(m_hDrawMutex);
		InvalidateRect(0, False, True);
		//::InvalidateRect(m_hWnd, 0, FALSE);
		return ;
	}

	// 碁石を描画
	if (stone &&
		(layer & eGL_STONE))
	{
		DrawStonez(stone, bIsShowNo, nNoFrom);
	}
	else if (stone==0 && (layer&eGL_STONE))
	{
		//m_UsedLayerIndex=1-m_UsedLayerIndex;
		//xTRACE("cglsGoban::Update() Change update layer!! %d", m_UsedLayerIndex);

		//if (m_BgLayerFlag[m_UsedLayerIndex])
		//{
		//	xTRACE("cglsGoban::Update() Bg Layer not cleared!");
			Bitmap* bg = cyctrlGobanResource::GetInstance().GetImage(eRIDB_GOBAN);
			if (bg)
				m_BgLayer[0]->blt(0, 0, bg);
		//}


	}
	//m_BgLayerFlag[m_UsedLayerIndex]=True;

	m_aCurrentLabelList[0]=STONE_EOF;
	// ラベルを描画
	if (layer & eGL_LABEL)
	{
		Bool bIsReqFree=False;
		cyiList* list = eng->GetLabel(&bIsReqFree);
		stone_t mask[19][19];

		if (m_bLabelShow==False)
		{
			if (bIsReqFree)
				eng->DeleteLabel(list);
				//delete list;
			list=0;
		}

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

			if (bIsReqFree)
				eng->DeleteLabel(list);
				//delete list;

		}

		stone_t PassStone;
		SetBranch(eng, &PassStone);
		int cnt=m_branches.count();
		Bool bIsNoDrawBranch=False;
		eBranchViewMode eMode=xGET_BRANCH_VIEW_MODE(eng->m_flag);
		if (eMode==eBVM_HIDE)
			bIsNoDrawBranch=True;
		else if (cnt==1 &&
				eMode==eBVM_NORMAL)
			bIsNoDrawBranch=True;

		stone_t ss = eng->GetLastStone();
		int stoneX=STONE_POS_X(ss), stoneY=STONE_POS_Y(ss);

		while(m_branches.count())
		{
			int i,j;
			eStoneType type;
			stone_t stone_branch;
			m_branches.pop((yiAnyValue&)stone_branch);

			i = STONE_POS_X(stone_branch);
			j= STONE_POS_Y(stone_branch);
			type = STONE_TYPE(stone_branch);

			//if (type!=eST_BLACK &&
			//		type!=eST_WHITE)
			//{
			//	continue;
			//}

			if (stoneX==i && stoneY==j)
			{
				cnt--;
				continue;
			}

			if (mask[i][j]==0)
			{
				mask[i][j]=('B'<<8 | (char)cnt);
				DrawBranch(cnt--, stone_branch);
				continue;
			}
			cnt--;
		}

		if (PassStone)
		{
			int px=STONE_POS_X(PassStone);
			int py=STONE_POS_Y(PassStone);

			if (px<0 || px>18 || py<0 || py>18)
			{
				px=0;
				py=0;
			}

			DrawPassBranch(px, py);		
		}

		int nLabelCnt=0;
		m_aCurrentLabelList[0]=STONE_EOF;
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
	InvalidateRect(0, 0, True);
#ifdef NEW_STYLE
	//CreateBluredBitmap();
#endif

	//::InvalidateRect(m_hWnd, 0, FALSE);
}

int g_cntx=0;
// -----------------------------------------
BOOL cglsGoban::OnPaint(HDC hdc, PAINTSTRUCT* ps)
{
	return True;
	if (m_parent==0)
	{
		m_BgLayer[0]->Draw(hdc, 0, 0);
	}

	//Graphics g(hdc);
	//g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	//g.DrawEllipse(&Pen(Color::Orange, 5.0), 10, 10, 200, 200);
	return True;
yiMutexLockEx(m_hDrawMutex);
	ClearCursor();
	xTRACE("cglsGoban::OnPaint(%d) called", m_UsedLayerIndex);

	//if (m_glsDrawInterface && xIS_GLS_FLAG(m_glsDrawInterface->m_flag, eGF_REQ_UPDATE_BUFFER))

	if (m_glsDrawInterface)
	{
		RECT xrect = ps->rcPaint;
		
		InvalidateRect(&xrect);
		return True;
	}

	m_BgLayer[0]->Draw(hdc, 0, 0);

	xTRACE("cglsGoban::OnPaint() exit");

yiMutexUnlockEx(m_hDrawMutex);
	DrawBackGroundLayer();

	return TRUE;
}



//----------------------------------------
// Resize時のスケール変更
void cglsGoban::AdjustScale()
{
	yiMutexLockEx(m_hDrawMutex);
	
	cyiGoEngine* eng = m_GoEnginez[m_nCurrentGoId];
	int nLineCnt = 19;//m_detail.nDefaultLineCount;

	if (eng!=0)
		nLineCnt=(int)eng->GetLineCount();
	
	m_scale = (float)m_w/(float)MAX_GOBAN_WIDTH;
	m_nBlockSize = MAX_BLOCK_SIZE*m_scale;
	//for (int i=0; i<nLineCnt; i++)
	//	m_scaled_point[i] = ((float)MAX_START_INDEX+(float)MAX_BLOCK_SIZE*(float)i)*m_scale+1;


#ifdef NEW_STYLE
	cyctrlGobanResource::GetInstance().SetBitmapScale(m_scale, m_w, m_h, m_scaled_point, (float)(float)MAX_CONTROL_SIZE*m_scale);
#else
	cyctrlGobanResource::GetInstance().SetBitmapScale(m_scale, m_w, m_h, m_scaled_point, 0);
#endif

	if (m_imgPosX)
	{
		m_imgPosX->ClearBitmaps();
		m_imgPosY->ClearBitmaps();
	}

	if (m_cursor)
	{
		m_cursor->SetBitmap(1, cyctrlGobanResource::GetInstance().GetImage(eRIDB_BSTONE_T), False);
		m_cursor->SetBitmap(2, cyctrlGobanResource::GetInstance().GetImage(eRIDB_WSTONE_T), False);
		m_cursor->SetBitmap(3, cyctrlGobanResource::GetInstance().GetImage(eRIDB_RED_CROSS_CURSOR), False);
		m_cursor->SetBitmap(4, cyctrlGobanResource::GetInstance().GetImage(eRIDB_ADD_CURSOR), False);
	}

	if (m_BgLayer[0])
		delete m_BgLayer[0];
	
	if (m_BgLayer[1])
		delete m_BgLayer[1];

	int w = cyctrlGobanResource::GetInstance().GetImage(eRIDB_GOBAN)->GetWidth();
	int h = cyctrlGobanResource::GetInstance().GetImage(eRIDB_GOBAN)->GetHeight();
	
	m_BgLayer[0]=new cyiSurface(w, h);
	m_BgLayer[1]=new cyiSurface(w, h);

	_DrawBackGroundLayer(True);

	yiMutexUnlockEx(m_hDrawMutex);

	if (eng)
	{
		Update(eGL_ALL, eng->GetStoneMap());
	}
	InvalidateRect(0);
	//::InvalidateRect(m_hWnd, 0, FALSE);
}

	
// -------------------------------
Bool cglsGoban::ShowHideLabel()
{
	Bool bIsPrev = m_bLabelShow;
	m_bLabelShow=1-m_bLabelShow;
	Update(eGL_ALL, m_GoEnginez[m_nCurrentGoId]->GetStoneMap());
	return bIsPrev;
}


// ------------------------------------------------------
Bool cglsGoban::SetLabelMode(eLabelType type, wchar_t label)
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

#ifdef NEW_STYLE

// ------------------------------------------------------
static unsigned int __stdcall xOnCreateBluredBitmap(void* p)
{
	cglsGoban* goban = (cglsGoban*)p;
	goban->xCreateBluredBitmap();
	return 0;
}

// ------------------------------------------------------
void cglsGoban::xCreateBluredBitmap()
{
	yiMutexLockEx(m_hDrawMutex);
	cyiSurface* sur = m_BgLayer[0];

	if (m_bluredBg)
		delete m_bluredBg;

	unsigned int t1 = cyiTime::CurrentmSec(0);
	YIDEBUG("cglsGoban::xCreateBluredBitmap() start!");
	int w, h;
	color_t* raw=sur->GetRaw(w, h);
	m_bluredBg = cyiBitmapBlender::GaussBlureEx(raw, w, h, 5.0, &m_blureStop);
	YIDEBUG("cglsGoban::xCreateBluredBitmap() exit! bmp:%x, %d msec", m_bluredBg, cyiTime::CurrentmSec(t1));
	yiMutexUnlockEx(m_hDrawMutex);

	_endthreadex(0);
	return ;
}

// ------------------------------------------------------
void cglsGoban::CreateBluredBitmap()
{
	m_blureStop=True;
	WaitForSingleObject(m_hBlureThread, INFINITE);
	CloseHandle(m_hBlureThread);

	unsigned int addr;
	m_blureStop=False;
	m_hBlureThread = (HANDLE)_beginthreadex(0, 0, xOnCreateBluredBitmap, this, 0, &addr);
}
#endif
// ------------------------------------------------------
Bitmap* cglsGoban::xCreatePosImage(int index, Bool bIsVertical)
{

	if (bIsVertical==False)
	{
		wchar_t ch[2] = {L'a'+index, 0};
		Font* font = cyctrlGobanResource::GetInstance().GetFont(eRIDF_LABEL);
		Bitmap* bmpStr=cyiShape::BitmapString(0, ch, font, &SolidBrush(Color::White), eSTS_DROP_SHADOW);
		return bmpStr;
	}
	else
	{
		wchar_t ch[2] = {L'A'+index, 0};
		Font* font = cyctrlGobanResource::GetInstance().GetFont(eRIDF_LABEL);
		Bitmap* bmpStr=cyiShape::BitmapString(0, ch, font, &SolidBrush(Color::White), eSTS_DROP_SHADOW);
		//Bitmap* bmpRet=cyiBitmapBlender::RotateCopy(0, bmpStr, 1);
		//delete bmpStr;
		return bmpStr;
	}

	return 0;
}
