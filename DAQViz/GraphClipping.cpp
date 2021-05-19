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

GraphClipping::GraphClipping(UINT start_idx, UINT end_idx, UINT _m_count, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAQVIZ_DIALOG_GRAPH_CLIPPING, pParent) {
	m_StartIdx = start_idx;
	m_EndIdx = end_idx;
	m_NumIdx = m_EndIdx - m_StartIdx + 1;
	m_count = _m_count;
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
	DDX_Control(pDX, IDC_BTN_ANIMATION_RUN, m_btnRun);
	DDX_Control(pDX, IDC_SCROLLBAR_ANIMATION, m_scrollBar);
}

BEGIN_MESSAGE_MAP(GraphClipping, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_REDRAW, &GraphClipping::OnBnClickedBtnRedraw)
	ON_BN_CLICKED(IDC_BTN_ANIMATION_RUN, &GraphClipping::OnBnClickedBtnAnimationRun)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

// GraphClipping 메시지 처리기

BOOL GraphClipping::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Initialize_GUI();
	Initialize_Variable();

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
	pMainDlg->Set_Font(m_btnRun, 20, 8);

	CString temp;
	temp.Format(_T("%d"), m_StartIdx);
	m_editStartIdx.SetWindowText(temp);
	temp.Format(_T("%d"), m_EndIdx);
	m_editEndIdx.SetWindowText(temp);

	CRect rectofDialogArea;
	GetDlgItem(IDC_PLOT_CLIPPED_GRAPH)->GetWindowRect(&rectofDialogArea);
	ScreenToClient(&rectofDialogArea);

	p_ClippedGraph = new ClippedGraph();
	p_ClippedGraph->Create(IDD_DAQVIZ_DIALOG_CLIPPED_GRAPH, this);
	p_ClippedGraph->ShowWindow(SW_SHOW);
	p_ClippedGraph->MoveWindow(rectofDialogArea);

	GetDlgItem(IDC_PLOT_ANIMATION)->GetWindowRect(&rectofDialogArea);
	ScreenToClient(&rectofDialogArea);

	p_ClippedGraph_2 = new ClippedGraph();
	p_ClippedGraph_2->Create(IDD_DAQVIZ_DIALOG_CLIPPED_GRAPH, this);
	p_ClippedGraph_2->ShowWindow(SW_SHOW);
	p_ClippedGraph_2->MoveWindow(rectofDialogArea);

	m_scrollBar.SetScrollRange(0, m_NumIdx - 1);
	m_scrollBar.SetScrollPos(ScrollPos);
}

void GraphClipping::Initialize_Variable() {
	ScrollPos = 0;
}

void GraphClipping::OnBnClickedBtnRedraw() {
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;

	m_editStartIdx.GetWindowText(temp);
	UINT m_StartIdx_temp = _ttoi(temp);
	m_editEndIdx.GetWindowText(temp);
	UINT m_EndIdx_temp = _ttoi(temp);

	if (m_StartIdx_temp <= m_EndIdx_temp &&
		m_EndIdx_temp <= m_count) {
		m_StartIdx = m_StartIdx_temp;
		m_EndIdx = m_EndIdx_temp;

		m_NumIdx = m_EndIdx - m_StartIdx + 1;
		m_scrollBar.SetScrollRange(0, m_NumIdx - 1);

		ScrollPos = 0;
		m_scrollBar.SetScrollPos(ScrollPos);
	}
	else {
		if (m_StartIdx_temp > m_EndIdx_temp) {
			MessageBox(_T("Start index is larger than end index."),
					   _T("Caution"), MB_OK | MB_ICONWARNING);
			temp.Format(_T("%d"), m_StartIdx);
			m_editStartIdx.SetWindowText(temp);
		}
		else {
			MessageBox(_T("End index is larger than maximum index."),
					   _T("Caution"), MB_OK | MB_ICONWARNING);
			temp.Format(_T("%d"), m_EndIdx);
			m_editEndIdx.SetWindowText(temp);
		}
	}
}

void GraphClipping::OnBnClickedBtnAnimationRun() {
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}

void GraphClipping::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ScrollPos = nPos;

	SCROLLINFO scrInfo;
	memset(&scrInfo, 0, sizeof(scrInfo));
	if (FALSE != m_scrollBar.GetScrollInfo(&scrInfo)) {
		switch (nSBCode) {
		case SB_LINELEFT:
			scrInfo.nPos -= 1;
			break;
		case SB_LINERIGHT:
			scrInfo.nPos += 1;
			break;
		case SB_PAGELEFT:
			scrInfo.nPos -= 30;
			break;
		case SB_PAGERIGHT:
			scrInfo.nPos += 30;
			break;
		case SB_THUMBTRACK:
			scrInfo.nPos = nPos;
			break;
		}
		ScrollPos = scrInfo.nPos;
		m_scrollBar.SetScrollInfo(&scrInfo);
	}

	CString temp;
	temp.Format(_T("%d"), scrInfo.nPos);
	GetDlgItem(IDC_EDIT_TEST_IDX)->SetWindowText(temp);

	p_ClippedGraph_2->Set_count_horizontal(ScrollPos);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}