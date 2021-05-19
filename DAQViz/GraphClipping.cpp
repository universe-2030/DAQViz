// GraphClipping.cpp: 구현 파일
//

#include "pch.h"
#include "DAQViz.h"
#include "DAQVizDlg.h"
#include "GraphClipping.h"
#include "afxdialogex.h"

// GraphClipping 대화 상자

IMPLEMENT_DYNAMIC(GraphClipping, CDialogEx)

GraphClipping::GraphClipping(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAQVIZ_DIALOG_GRAPH_CLIPPING, pParent) {

}

GraphClipping::GraphClipping(UINT start_idx, UINT end_idx, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAQVIZ_DIALOG_GRAPH_CLIPPING, pParent) {
	m_StartIdx = start_idx;
	m_EndIdx = end_idx;
}

GraphClipping::~GraphClipping() {

}

void GraphClipping::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TEXT_CLIPPING_START_IDX, m_textStartIdx);
	DDX_Control(pDX, IDC_TEXT_CLIPPING_END_IDX, m_textEndIdx);
	DDX_Control(pDX, IDC_EDIT_CLIPPING_START_IDX, m_editStartIdx);
	DDX_Control(pDX, IDC_EDIT_CLIPPING_END_IDX, m_editEndIdx);
	DDX_Control(pDX, IDC_BTN_REDRAW, m_btnRedraw);
}

BEGIN_MESSAGE_MAP(GraphClipping, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_REDRAW, &GraphClipping::OnBnClickedBtnRedraw)
END_MESSAGE_MAP()

// GraphClipping 메시지 처리기

BOOL GraphClipping::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Initialize_GUI();

	SetWindowPos(NULL, 0, -1080, 0, 0, SWP_NOSIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void GraphClipping::Initialize_GUI() {
	CDAQVizDlg* pMainDlg = new CDAQVizDlg();
	pMainDlg->Set_Font(m_textStartIdx, 20, 8);
	pMainDlg->Set_Font(m_editStartIdx, 20, 8);
	pMainDlg->Set_Font(m_textEndIdx, 20, 8);
	pMainDlg->Set_Font(m_editEndIdx, 20, 8);
	pMainDlg->Set_Font(m_btnRedraw, 20, 8);

	CString temp;
	temp.Format(_T("%u"), m_StartIdx);
	m_editStartIdx.SetWindowText(temp);
	temp.Format(_T("%u"), m_EndIdx);
	m_editEndIdx.SetWindowText(temp);

	CRect rectofDialogArea;
	GetDlgItem(IDC_PLOT_CLIPPED_GRAPH)->GetWindowRect(&rectofDialogArea);
	ScreenToClient(&rectofDialogArea);

	p_ClippedGraph = new ClippedGraph();
	p_ClippedGraph->Create(IDD_DAQVIZ_DIALOG_CLIPPED_GRAPH, this);
	p_ClippedGraph->ShowWindow(SW_SHOW);
	p_ClippedGraph->MoveWindow(rectofDialogArea);
}

void GraphClipping::OnBnClickedBtnRedraw() {
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}
