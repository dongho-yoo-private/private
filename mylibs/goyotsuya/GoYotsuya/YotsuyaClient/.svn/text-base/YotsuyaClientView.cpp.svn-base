// YotsuyaClientView.cpp : CYotsuyaClientView クラスの実装
//

#include "stdafx.h"
#include "YotsuyaClient.h"
#include "YotsuyaClientDoc.h"
#include "YotsuyaClientView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYotsuyaClientView

IMPLEMENT_DYNCREATE(CYotsuyaClientView, CView)

BEGIN_MESSAGE_MAP(CYotsuyaClientView, CView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()

	ON_WM_SIZE()
END_MESSAGE_MAP()

// CYotsuyaClientView コンストラクション/デストラクション

CYotsuyaClientView::CYotsuyaClientView()
{
	// TODO: 構築コードをここに追加します。
	//m_pGobanListMenu=0;

}

CYotsuyaClientView::~CYotsuyaClientView()
{
}

BOOL CYotsuyaClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CYotsuyaClientView 描画

void CYotsuyaClientView::OnDraw(CDC* /*pDC*/)
{
	CYotsuyaClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}


// CYotsuyaClientView 印刷

BOOL CYotsuyaClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CYotsuyaClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CYotsuyaClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}


// CYotsuyaClientView 診断

#ifdef _DEBUG
void CYotsuyaClientView::AssertValid() const
{
	CView::AssertValid();
}

void CYotsuyaClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CYotsuyaClientDoc* CYotsuyaClientView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CYotsuyaClientDoc)));
	return (CYotsuyaClientDoc*)m_pDocument;
}
#endif //_DEBUG


#include "yiEncoder.h"
// CYotsuyaClientView メッセージ ハンドラ




int CYotsuyaClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CYotsuyaClientView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

// ----------------------------------------
//BOOL CYotsuyaClientView::CreateGoban()
//{
//}
//
//// ----------------------------------------
//BOOL CYotsuyaClientView::ResizeGoban()
//{
//	RECT rect;
//	GetClientRect(&rect);
//	m_goban->Resize(rect.right-rect.left, rect.bottom-rect.top);
//	return TRUE;
//}
//
//// ----------------------------------------
//BOOL CYotsuyaClientView::ChangeControl(CDialog* control)
//{
//	m_currentCtrl->ShowWindow(SW_HIDE);
//	AdjustLayout(control);
//	control->ShowWindow(SW_SHOW);
//	return TRUE;
//}

