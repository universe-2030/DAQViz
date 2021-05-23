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

GraphClipping::GraphClipping(UINT start_idx, UINT end_idx, UINT _m_count,
							const std::vector<double>* _sEMG_plot,
							const std::vector<double>* _Flex_plot,
							const std::vector<double>* _MotionLabel_plot,
							const std::vector<double>* _MotionEstimation_plot,
							const std::vector<double>* _X_ball_plot,
							const std::vector<double>* _Y_ball_plot,
							const std::vector<double>* _Rad_ball_plot,
							CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAQVIZ_DIALOG_GRAPH_CLIPPING, pParent) {
	m_StartIdx = start_idx;
	m_EndIdx = end_idx;
	m_NumIdx = m_EndIdx - m_StartIdx + 1;
	m_count = _m_count;

	sEMG_plot = _sEMG_plot;
	Flex_plot = _Flex_plot;

	MotionLabel = _MotionLabel_plot;
	MotionEstimation = _MotionEstimation_plot;

	X_ball_plot = _X_ball_plot;
	Y_ball_plot = _Y_ball_plot;
	Rad_ball_plot = _Rad_ball_plot;
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
	DDX_Control(pDX, IDC_EDIT_ANIMATION_IDX, m_editAnimationIdx);
	DDX_Control(pDX, IDC_TEXT_TIMESTEP, m_textTimeStep);
	DDX_Control(pDX, IDC_EDIT_TIMESTEP, m_editTimeStep);
	DDX_Control(pDX, IDC_SCROLLBAR_ANIMATION, m_scrollBar);
	DDX_Control(pDX, IDC_BTN_ANIMATION_RUN, m_btnRun);
}

BEGIN_MESSAGE_MAP(GraphClipping, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_REDRAW, &GraphClipping::OnBnClickedBtnRedraw)
	ON_BN_CLICKED(IDC_BTN_ANIMATION_RUN, &GraphClipping::OnBnClickedBtnAnimationRun)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_TIMESTEP, &GraphClipping::OnEnChangeEditTimestep)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// GraphClipping 메시지 처리기

BOOL GraphClipping::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Initialize_Variable();
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
	pMainDlg->Set_Font(m_btnRun, 20, 8);
	pMainDlg->Set_Font(m_editAnimationIdx, 20, 8);
	pMainDlg->Set_Font(m_textTimeStep, 20, 8);
	pMainDlg->Set_Font(m_editTimeStep, 20, 8);

	CString temp;
	temp.Format(_T("%d"), m_StartIdx);
	m_editStartIdx.SetWindowText(temp);
	temp.Format(_T("%d"), m_EndIdx);
	m_editEndIdx.SetWindowText(temp);
	temp.Format(_T("%d"), m_TimeStep);
	m_editTimeStep.SetWindowText(temp);
	temp.Format(_T("%d"), m_StartIdx);
	m_editAnimationIdx.SetWindowText(temp);

	CRect rectofDialogArea;
	GetDlgItem(IDC_PLOT_CLIPPED_GRAPH)->GetWindowRect(&rectofDialogArea);
	ScreenToClient(&rectofDialogArea);

	p_ClippedGraph = new ClippedGraph(m_StartIdx, m_EndIdx, m_NumIdx,
									NUM_CH, sEMG_plot, Flex_plot, Render::TOTAL);
	p_ClippedGraph->Create(IDD_DAQVIZ_DIALOG_CLIPPED_GRAPH, this);
	p_ClippedGraph->ShowWindow(SW_SHOW);
	p_ClippedGraph->MoveWindow(rectofDialogArea);

	GetDlgItem(IDC_PLOT_ANIMATION)->GetWindowRect(&rectofDialogArea);
	ScreenToClient(&rectofDialogArea);

	p_ClippedGraph_2 = new ClippedGraph(m_StartIdx, m_EndIdx, m_NumIdx,
										NUM_CH, sEMG_plot, Flex_plot, Render::ANIMATION);
	p_ClippedGraph_2->Create(IDD_DAQVIZ_DIALOG_CLIPPED_GRAPH, this);
	p_ClippedGraph_2->ShowWindow(SW_SHOW);
	p_ClippedGraph_2->MoveWindow(rectofDialogArea);

	GetDlgItem(IDC_PLOT_ANIMATION_HAND)->GetWindowRect(&rectofDialogArea);
	ScreenToClient(&rectofDialogArea);

	p_HandMotion = new DAQVizChildOpenGL2(m_StartIdx, m_EndIdx, m_NumIdx,
										Flex_plot, Render_Hand::RENDER_HAND, TRUE);
	p_HandMotion->Create(IDD_DAQVIZ_DIALOG_CHILD_OPENGL_2, this);
	p_HandMotion->ShowWindow(SW_SHOW);
	p_HandMotion->MoveWindow(rectofDialogArea);

	GetDlgItem(IDC_PLOT_ANIMATION_BALL_CONTROL)->GetWindowRect(&rectofDialogArea);
	ScreenToClient(&rectofDialogArea);

	p_BallControl = new BallControl(m_StartIdx, m_EndIdx, m_NumIdx,
								X_ball_plot, Y_ball_plot, Rad_ball_plot, Render_Ball::RENDER_BALL);
	p_BallControl->Create(IDD_DAQVIZ_DIALOG_BALL_CONTROL, this);
	p_BallControl->ShowWindow(SW_SHOW);
	p_BallControl->MoveWindow(rectofDialogArea);

	m_scrollBar.SetScrollRange(0, m_NumIdx - 1);
	m_scrollBar.SetScrollPos(ScrollPos);
}

void GraphClipping::Initialize_Variable() {
	ScrollPos = 0;
}

void GraphClipping::Set_ScrollPos(UINT _ScrollPos) {
	ScrollPos = _ScrollPos;
	m_scrollBar.SetScrollPos(ScrollPos);
}

void GraphClipping::OnBnClickedBtnRedraw() {
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;

	m_editStartIdx.GetWindowText(temp);
	UINT m_StartIdx_temp = _ttoi(temp);
	m_editEndIdx.GetWindowText(temp);
	UINT m_EndIdx_temp = _ttoi(temp);

	// Exception handler
	if (m_StartIdx_temp <= m_EndIdx_temp &&
		m_EndIdx_temp <= m_count) {
		m_StartIdx = m_StartIdx_temp;
		m_EndIdx = m_EndIdx_temp;

		m_NumIdx = m_EndIdx - m_StartIdx + 1;
		m_scrollBar.SetScrollRange(0, m_NumIdx - 1);

		p_ClippedGraph->Delete_Dynamic_Alloc();
		p_ClippedGraph->Set_Dynamic_Alloc(m_StartIdx, m_EndIdx, m_NumIdx);

		p_ClippedGraph_2->Delete_Dynamic_Alloc();
		p_ClippedGraph_2->Set_Dynamic_Alloc(m_StartIdx, m_EndIdx, m_NumIdx);

		p_HandMotion->Set_Redraw(m_StartIdx, m_EndIdx, m_NumIdx);

		p_BallControl->Set_Redraw(m_StartIdx, m_EndIdx, m_NumIdx);

		ScrollPos = 0;
		p_ClippedGraph_2->Set_Current_idx(ScrollPos);
		p_HandMotion->Set_Current_idx(ScrollPos);
		p_BallControl->Set_Current_idx(ScrollPos);
		m_scrollBar.SetScrollPos(ScrollPos);

		CString temp;
		temp.Format(_T("%d"), ScrollPos + m_StartIdx);
		m_editAnimationIdx.SetWindowText(temp);
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
	if (!isAnimationRun) {
		isAnimationRun = TRUE;
		m_btnRun.SetWindowText(_T("Stop"));
		m_editStartIdx.EnableWindow(FALSE);
		m_editEndIdx.EnableWindow(FALSE);
		m_btnRedraw.EnableWindow(FALSE);
		m_editTimeStep.EnableWindow(FALSE);

		// Run the animation
		SetTimer(TIMER_GRAPH_CLIPPING, TIME_ELAPSE, NULL);
		p_ClippedGraph_2->Set_AnimationTimer();
		p_HandMotion->Set_AnimationTimer();
		p_BallControl->Set_AnimationTimer();
	}
	else {
		isAnimationRun = FALSE;
		m_btnRun.SetWindowText(_T("Run"));
		m_editStartIdx.EnableWindow(TRUE);
		m_editEndIdx.EnableWindow(TRUE);
		m_btnRedraw.EnableWindow(TRUE);
		m_editTimeStep.EnableWindow(TRUE);

		// Stop the animation
		KillTimer(TIMER_GRAPH_CLIPPING);
		p_ClippedGraph_2->Kill_AnimationTimer();
		p_HandMotion->Kill_AnimationTimer();
		p_BallControl->Kill_AnimationTimer();
	}
}

void GraphClipping::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ScrollPos = nPos;

	SCROLLINFO scrInfo;
	memset(&scrInfo, 0, sizeof(scrInfo));
	if (FALSE != m_scrollBar.GetScrollInfo(&scrInfo)) {
		switch (nSBCode) {
		case SB_LINELEFT:
			scrInfo.nPos -= m_TimeStep;
			break;
		case SB_LINERIGHT:
			scrInfo.nPos += m_TimeStep;
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
	temp.Format(_T("%d"), scrInfo.nPos + m_StartIdx);
	m_editAnimationIdx.SetWindowText(temp);

	p_ClippedGraph_2->Set_Current_idx(ScrollPos);
	p_HandMotion->Set_Current_idx(ScrollPos);
	p_BallControl->Set_Current_idx(ScrollPos);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void GraphClipping::OnEnChangeEditTimestep() {
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;
	m_editTimeStep.GetWindowText(temp);
	m_TimeStep = _ttoi(temp);
	if (p_ClippedGraph_2 != NULL)
		p_ClippedGraph_2->Set_TimeStep(m_TimeStep);
	if (p_HandMotion!= NULL)
		p_HandMotion->Set_TimeStep(m_TimeStep);
	if (p_BallControl != NULL)
		p_BallControl->Set_TimeStep(m_TimeStep);
}

void GraphClipping::OnTimer(UINT_PTR nIDEvent) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent) {
	case TIMER_GRAPH_CLIPPING:
		Set_ScrollPos(p_ClippedGraph_2->Get_Current_idx());
		CString temp;
		temp.Format(_T("%d"), ScrollPos + m_StartIdx);
		m_editAnimationIdx.SetWindowText(temp);
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void GraphClipping::OnClose() {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	p_HandMotion->OnClose();
	p_BallControl->OnClose();

	CDialogEx::OnClose();
}
