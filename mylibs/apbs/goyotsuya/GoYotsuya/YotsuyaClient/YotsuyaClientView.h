// YotsuyaClientView.h : CYotsuyaClientView クラスのインターフェイス
//


#pragma once

//#include "GobanCtrlBase.h"



class CYotsuyaClientView : public CView
{
protected: // シリアル化からのみ作成します。
	CYotsuyaClientView();
	DECLARE_DYNCREATE(CYotsuyaClientView)

// 属性
public:
	CYotsuyaClientDoc* GetDocument() const;

// 操作
public:

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 実装
public:
	virtual ~CYotsuyaClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);


public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // YotsuyaClientView.cpp のデバッグ バージョン
inline CYotsuyaClientDoc* CYotsuyaClientView::GetDocument() const
   { return reinterpret_cast<CYotsuyaClientDoc*>(m_pDocument); }
#endif

