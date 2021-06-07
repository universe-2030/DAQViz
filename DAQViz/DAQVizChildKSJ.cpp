// DAQVizChildKSJ.cpp: 구현 파일
//

#include "pch.h"
#include "DAQViz.h"
#include "DAQVizDlg.h"
#include "DAQVizChildKSJ.h"
#include "afxdialogex.h"

// DAQVizChildKSJ 대화 상자

IMPLEMENT_DYNAMIC(DAQVizChildKSJ, CDialogEx)

DAQVizChildKSJ::DAQVizChildKSJ(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAQVIZ_DIALOG_CHILD_KSJ, pParent) {
	N_sEMG = DELSYS_CH_MAX;
	N_Flex = FINGER_CH_MAX;
	N_IMU = WRIST_CH_MAX;
}

DAQVizChildKSJ::DAQVizChildKSJ(int _N_sEMG_CH, int _N_Flex_CH, int _N_IMU_CH,
								CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAQVIZ_DIALOG_CHILD_KSJ, pParent) {
	N_sEMG = _N_sEMG_CH;
	N_Flex = _N_Flex_CH;
	N_IMU = _N_IMU_CH;
}

DAQVizChildKSJ::~DAQVizChildKSJ() {
	delete sEMG_MAV;
	delete Flex_raw;
	delete IMU_raw;
}

void DAQVizChildKSJ::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TEXT_SEMG_PLOT, m_textsEMGPlot);
	DDX_Control(pDX, IDC_TEXT_MOTION, m_textMotionRendering);
	DDX_Control(pDX, IDC_TEXT_BALL_CONTROL, m_textBallControl);
}

BEGIN_MESSAGE_MAP(DAQVizChildKSJ, CDialogEx)
	ON_STN_CLICKED(IDC_GRAPH_SEMG_MAV_1, &DAQVizChildKSJ::OnClickedGraphSemgMav1)
	ON_STN_CLICKED(IDC_GRAPH_SEMG_MAV_2, &DAQVizChildKSJ::OnClickedGraphSemgMav2)
	ON_STN_CLICKED(IDC_GRAPH_SEMG_MAV_3, &DAQVizChildKSJ::OnClickedGraphSemgMav3)
	ON_STN_CLICKED(IDC_GRAPH_FINGER_FLEX, &DAQVizChildKSJ::OnClickedGraphFingerFlex)
	ON_STN_CLICKED(IDC_GRAPH_FINGER_FLEX_SLOPE, &DAQVizChildKSJ::OnClickedGraphFingerFlexSlope)
	ON_STN_CLICKED(IDC_GRAPH_WRIST_FLEX, &DAQVizChildKSJ::OnClickedGraphWristFlex)
	ON_STN_CLICKED(IDC_GRAPH_WRIST_FLEX_SLOPE, &DAQVizChildKSJ::OnClickedGraphWristFlexSlope)
	ON_STN_CLICKED(IDC_GRAPH_MOTION_LABEL_EST, &DAQVizChildKSJ::OnClickedGraphMotionLabelEst)
END_MESSAGE_MAP()

// DAQVizChildKSJ 메시지 처리기

BOOL DAQVizChildKSJ::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Initialize_Variable();
	Initialize_GUI();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void DAQVizChildKSJ::Initialize_Variable() {
	p_RectCtrl = new CRect[N_GRAPH];
	p_RectPlot = new CRect[N_GRAPH];
	p_RectPlot_fin = new CRect[N_GRAPH];
	m_NumClicked = new UINT[N_GRAPH];
	memset(m_NumClicked, 0, sizeof(m_NumClicked) * N_GRAPH);

	rtGraph_sEMG_MAV = new COScopeCtrl*[3];
	rtGraph_Finger = new COScopeCtrl*;
	rtGraph_Finger_slope = new COScopeCtrl*;
	rtGraph_Wrist = new COScopeCtrl*;
	rtGraph_Wrist_slope = new COScopeCtrl*;
	rtGraph_Label_Est = new COScopeCtrl*;

	sEMG_MAV = new double[N_sEMG];
	Flex_raw = new double[N_Flex];
	IMU_raw = new double[N_IMU];

	Pt_forth = 0;
	Pt_back = 0;
}

void DAQVizChildKSJ::Initialize_GUI() {
	CDAQVizDlg* pMainDlg = new CDAQVizDlg();
	pMainDlg->Set_Font(m_textsEMGPlot, 20, 8);
	pMainDlg->Set_Font(m_textMotionRendering, 20, 8);
	pMainDlg->Set_Font(m_textBallControl, 20, 8);

	rtGraph_sEMG_MAV[0] = Initialize_graph(IDC_GRAPH_SEMG_MAV_1, 1, 5, rtGraph_sEMG_MAV[0], SEMG_MAV);
	rtGraph_sEMG_MAV[1] = Initialize_graph(IDC_GRAPH_SEMG_MAV_2, 6, 10, rtGraph_sEMG_MAV[1], SEMG_MAV);
	rtGraph_sEMG_MAV[2] = Initialize_graph(IDC_GRAPH_SEMG_MAV_3, 11, 16, rtGraph_sEMG_MAV[2], SEMG_MAV);

	rtGraph_Finger[0] = Initialize_graph(IDC_GRAPH_FINGER_FLEX, 1, 5, rtGraph_Finger[0], FINGER);

	rtGraph_Finger_slope[0] = Initialize_graph(IDC_GRAPH_FINGER_FLEX_SLOPE, 1, 5, rtGraph_Finger_slope[0], FINGER_SLOPE);

	rtGraph_Wrist[0] = Initialize_graph(IDC_GRAPH_WRIST_FLEX, 1, 2, rtGraph_Wrist[0], WRIST);

	rtGraph_Wrist_slope[0] = Initialize_graph(IDC_GRAPH_WRIST_FLEX_SLOPE, 1, 2, rtGraph_Wrist_slope[0], WRIST_SLOPE);

	rtGraph_Label_Est[0] = Initialize_graph(IDC_GRAPH_MOTION_LABEL_EST, 1, 2, rtGraph_Label_Est[0], LABEL_EST);
	
	// Get m_rectPlot region with graph controls
	for (int i = 0; i < N_GRAPH; i++) {
		UINT ID_Ctrl = IDC_GRAPH_SEMG_MAV_1 + i;

		GetDlgItem(ID_Ctrl)->GetWindowRect(p_RectCtrl[i]);
		ScreenToClient(p_RectCtrl[i]);

		if (0 <= i && i < 3)
			p_RectPlot[i] = rtGraph_sEMG_MAV[i]->Get_m_rectPlot();
		else if (i == 3)
			p_RectPlot[i] = rtGraph_Finger[0]->Get_m_rectPlot();
		else if (i == 4)
			p_RectPlot[i] = rtGraph_Finger_slope[0]->Get_m_rectPlot();
		else if (i == 5)
			p_RectPlot[i] = rtGraph_Wrist[0]->Get_m_rectPlot();
		else if (i == 6)
			p_RectPlot[i] = rtGraph_Wrist_slope[0]->Get_m_rectPlot();
		else if (i == 7)
			p_RectPlot[i] = rtGraph_Label_Est[0]->Get_m_rectPlot();

		p_RectPlot_fin[i].left = p_RectCtrl[i].left + p_RectPlot[i].left;
		p_RectPlot_fin[i].top = p_RectCtrl[i].top + p_RectPlot[i].top;
		p_RectPlot_fin[i].right = p_RectCtrl[i].left + p_RectPlot[i].right;
		p_RectPlot_fin[i].bottom = p_RectCtrl[i].top + p_RectPlot[i].bottom;
	}

	CRect rectofDialogArea;
	GetDlgItem(IDC_PLOT_OPENGL)->GetWindowRect(&rectofDialogArea);
	ScreenToClient(&rectofDialogArea);

	p_ChildOpenGL = new DAQVizChildOpenGL(N_sEMG, N_MOTIONS);
	p_ChildOpenGL->Create(IDD_DAQVIZ_DIALOG_CHILD_OPENGL, this);
	p_ChildOpenGL->ShowWindow(SW_SHOW);
	p_ChildOpenGL->MoveWindow(rectofDialogArea);

	GetDlgItem(IDC_PLOT_MOTION)->GetWindowRect(&rectofDialogArea);
	ScreenToClient(&rectofDialogArea);

	p_ChildOpenGL_2 = new DAQVizChildOpenGL2();
	p_ChildOpenGL_2->Create(IDD_DAQVIZ_DIALOG_CHILD_OPENGL_2, this);
	p_ChildOpenGL_2->ShowWindow(SW_SHOW);
	p_ChildOpenGL_2->MoveWindow(rectofDialogArea);
	
	GetDlgItem(IDC_PLOT_BALL_CONTROL)->GetWindowRect(&rectofDialogArea);
	ScreenToClient(&rectofDialogArea);

	p_BallControl = new BallControl();
	p_BallControl->Create(IDD_DAQVIZ_DIALOG_BALL_CONTROL, this);
	p_BallControl->ShowWindow(SW_SHOW);
	p_BallControl->MoveWindow(rectofDialogArea);

	delete pMainDlg;
}

COScopeCtrl* DAQVizChildKSJ::Initialize_graph (int ID, int idx_start, int idx_end,
									   COScopeCtrl* rtGraph, Graph_class _class) {
	CRect _rtGraph;
	GetDlgItem(ID)->GetWindowRect(_rtGraph);
	ScreenToClient(_rtGraph);

	int N_graph = idx_end - idx_start + 1;
	if (idx_start == 11)
		N_graph--;
	rtGraph = new COScopeCtrl(N_graph);
	rtGraph->Create(WS_VISIBLE | WS_CHILD, _rtGraph, this, ID);

	CString temp_str;
	for (int _idx = idx_start; _idx <= idx_end; _idx++) {
		if (_class == SEMG_MAV) {
			if (_idx != 13) {
				temp_str.Format(_T("CH %d"), _idx);
				rtGraph->SetRanges(SEMG_MAV_MIN, SEMG_MAV_MAX);
				rtGraph->autofitYscale = true;
				rtGraph->SetYUnits(_T("sEMG MAV"));
				rtGraph->SetXUnits(_T("Time"));

				int _idx_rev = _idx - idx_start;
				if (idx_start == 11 && _idx_rev >= 3)
					_idx_rev--;
				rtGraph->SetLegendLabel(temp_str, _idx_rev);

				if (_idx_rev == 0)
					rtGraph->SetPlotColor(RGB(255, 0, 0), _idx_rev);
				else if (_idx_rev == 1)
					rtGraph->SetPlotColor(RGB(0, 255, 0), _idx_rev);
				else if (_idx_rev == 2)
					rtGraph->SetPlotColor(RGB(0, 0, 255), _idx_rev);
				else if (_idx_rev == 3)
					rtGraph->SetPlotColor(RGB(255, 255, 0), _idx_rev);
				else if (_idx_rev == 4)
					rtGraph->SetPlotColor(RGB(255, 0, 255), _idx_rev);
				else if (_idx_rev == 5)
					rtGraph->SetPlotColor(RGB(0, 255, 255), _idx_rev);

				rtGraph->InvalidateCtrl();
			}
		}
		else if (_class == FINGER) {
			if (_idx == 1)
				temp_str = _T("Thumb");
			else if (_idx == 2)
				temp_str = _T("Index");
			else if (_idx == 3)
				temp_str = _T("Middle");
			else if (_idx == 4)
				temp_str = _T("Ring");
			else if (_idx == 5)
				temp_str = _T("Pinky");

			rtGraph->SetRanges(FINGER_MIN, FINGER_MAX);
			rtGraph->autofitYscale = true;
			rtGraph->SetYUnits(_T("Finger"));
			rtGraph->SetXUnits(_T("Time"));

			int _idx_rev = _idx - idx_start;
			rtGraph->SetLegendLabel(temp_str, _idx_rev);

			if (_idx_rev == 0)
				rtGraph->SetPlotColor(RGB(255, 0, 0), _idx_rev);
			else if (_idx_rev == 1)
				rtGraph->SetPlotColor(RGB(0, 255, 0), _idx_rev);
			else if (_idx_rev == 2)
				rtGraph->SetPlotColor(RGB(0, 0, 255), _idx_rev);
			else if (_idx_rev == 3)
				rtGraph->SetPlotColor(RGB(255, 255, 0), _idx_rev);
			else if (_idx_rev == 4)
				rtGraph->SetPlotColor(RGB(255, 0, 255), _idx_rev);

			rtGraph->InvalidateCtrl();
		}
		else if (_class == FINGER_SLOPE) {
			if (_idx == 1)
				temp_str = _T("Thumb");
			else if (_idx == 2)
				temp_str = _T("Index");
			else if (_idx == 3)
				temp_str = _T("Middle");
			else if (_idx == 4)
				temp_str = _T("Ring");
			else if (_idx == 5)
				temp_str = _T("Pinky");

			rtGraph->SetRanges(FINGER_SLOPE_MIN, FINGER_SLOPE_MAX);
			rtGraph->autofitYscale = true;
			rtGraph->SetYUnits(_T("Finger slope"));
			rtGraph->SetXUnits(_T("Time"));

			int _idx_rev = _idx - idx_start;
			rtGraph->SetLegendLabel(temp_str, _idx_rev);

			if (_idx_rev == 0)
				rtGraph->SetPlotColor(RGB(255, 0, 0), _idx_rev);
			else if (_idx_rev == 1)
				rtGraph->SetPlotColor(RGB(0, 255, 0), _idx_rev);
			else if (_idx_rev == 2)
				rtGraph->SetPlotColor(RGB(0, 0, 255), _idx_rev);
			else if (_idx_rev == 3)
				rtGraph->SetPlotColor(RGB(255, 255, 0), _idx_rev);
			else if (_idx_rev == 4)
				rtGraph->SetPlotColor(RGB(255, 0, 255), _idx_rev);

			rtGraph->InvalidateCtrl();
		}
		else if (_class == WRIST) {
			if (_idx == 1)
				temp_str = _T("Flex / Ext");
			else if (_idx == 2)
				temp_str = _T("Radial / Ulnar");

			rtGraph->SetRanges(WRIST_MIN, WRIST_MAX);
			rtGraph->autofitYscale = true;
			rtGraph->SetYUnits(_T("Wrist"));
			rtGraph->SetXUnits(_T("Time"));

			int _idx_rev = _idx - idx_start;
			rtGraph->SetLegendLabel(temp_str, _idx_rev);

			if (_idx_rev == 0)
				rtGraph->SetPlotColor(RGB(255, 0, 0), _idx_rev);
			else if (_idx_rev == 1)
				rtGraph->SetPlotColor(RGB(0, 0, 255), _idx_rev);

			rtGraph->InvalidateCtrl();
		}
		else if (_class == WRIST_SLOPE) {
			if (_idx == 1)
				temp_str = _T("Flex / Ext");
			else if (_idx == 2)
				temp_str = _T("Radial / Ulnar");

			rtGraph->SetRanges(WRIST_SLOPE_MIN, WRIST_SLOPE_MAX);
			rtGraph->autofitYscale = true;
			rtGraph->SetYUnits(_T("Wrist slope"));
			rtGraph->SetXUnits(_T("Time"));

			int _idx_rev = _idx - idx_start;
			rtGraph->SetLegendLabel(temp_str, _idx_rev);

			if (_idx_rev == 0)
				rtGraph->SetPlotColor(RGB(255, 0, 0), _idx_rev);
			else if (_idx_rev == 1)
				rtGraph->SetPlotColor(RGB(0, 0, 255), _idx_rev);

			rtGraph->InvalidateCtrl();
		}
		else if (_class == LABEL_EST) {
			if (_idx == 1)
				temp_str = _T("Motion label");
			else if (_idx == 2)
				temp_str = _T("Motion estimation");

			rtGraph->SetRanges(LABEL_EST_MIN, LABEL_EST_MAX);
			rtGraph->autofitYscale = true;
			rtGraph->SetYUnits(_T("Motion index"));
			rtGraph->SetXUnits(_T("Time"));

			int _idx_rev = _idx - idx_start;
			rtGraph->SetLegendLabel(temp_str, _idx_rev);

			if (_idx_rev == 0)
				rtGraph->SetPlotColor(RGB(255, 0, 0), _idx_rev);
			else if (_idx_rev == 1)
				rtGraph->SetPlotColor(RGB(0, 0, 255), _idx_rev);

			rtGraph->InvalidateCtrl();
		}
	}

	return rtGraph;
}

void DAQVizChildKSJ::Plot_graph(double* data, COScopeCtrl* rtGraph) {
	rtGraph->AppendPoints(data);
}

COScopeCtrl** DAQVizChildKSJ::Get_rtGraph_sEMG_MAV() {
	return rtGraph_sEMG_MAV;
}

COScopeCtrl** DAQVizChildKSJ::Get_rtGraph_Finger() {
	return rtGraph_Finger;
}

COScopeCtrl** DAQVizChildKSJ::Get_rtGraph_Finger_slope() {
	return rtGraph_Finger_slope;
}

COScopeCtrl** DAQVizChildKSJ::Get_rtGraph_Wrist() {
	return rtGraph_Wrist;
}

COScopeCtrl** DAQVizChildKSJ::Get_rtGraph_Wrist_slope() {
	return rtGraph_Wrist_slope;
}

COScopeCtrl** DAQVizChildKSJ::Get_rtGraph_Label_Est() {
	return rtGraph_Label_Est;
}

DAQVizChildOpenGL* DAQVizChildKSJ::Get_OpenGLPointer() {
	return p_ChildOpenGL;
}

void DAQVizChildKSJ::OnClickedGraphSemgMav1() {
	Cursor_set(0);
}

void DAQVizChildKSJ::OnClickedGraphSemgMav2() {
	Cursor_set(1);
}

void DAQVizChildKSJ::OnClickedGraphSemgMav3() {
	Cursor_set(2);
}

void DAQVizChildKSJ::OnClickedGraphFingerFlex() {
	Cursor_set(3);
}

void DAQVizChildKSJ::OnClickedGraphFingerFlexSlope() {
	Cursor_set(4);
}

void DAQVizChildKSJ::OnClickedGraphWristFlex() {
	Cursor_set(5);
}

void DAQVizChildKSJ::OnClickedGraphWristFlexSlope() {
	Cursor_set(6);
}

void DAQVizChildKSJ::OnClickedGraphMotionLabelEst() {
	Cursor_set(7);
}

void DAQVizChildKSJ::Cursor_set(UINT graph_idx) {
	CDAQVizDlg* pMainDlg = (CDAQVizDlg*)AfxGetMainWnd();

	for (int i = 0; i < N_GRAPH; i++)
		if (i != graph_idx)
			if (m_NumClicked[i] > 0) {
				MessageBox(_T("Please select another point in same picture control."),
						   _T("Notice"), MB_OK | MB_ICONINFORMATION);
				return;
			}

	if (m_NumClicked[graph_idx] == 0) {
		CPoint point_cur;
		::GetCursorPos(&point_cur);
		ScreenToClient(&point_cur);

		if (p_RectPlot_fin[graph_idx].left <= point_cur.x &&
			point_cur.x <= p_RectPlot_fin[graph_idx].right &&
			p_RectPlot_fin[graph_idx].top <= point_cur.y &&
			point_cur.y <= p_RectPlot_fin[graph_idx].bottom) {
			
			point_cur.x -= p_RectPlot_fin[graph_idx].right;
			point_cur.x *= N_GRAPH;
			point_cur.x += pMainDlg->Get_m_count() + graph_idx;
			point_cur.y -= p_RectPlot_fin[graph_idx].top;

			m_NumClicked[graph_idx] += 1;
			Pt_forth = point_cur.x;
			pMainDlg->Set_StartIdx(point_cur.x);
		}
		else {
			MessageBox(_T("Please click the plot space."),
					   _T("Notice"), MB_OKCANCEL | MB_ICONINFORMATION);
		}
	}
	else if (m_NumClicked[graph_idx] == 1) {
		CPoint point_cur;
		::GetCursorPos(&point_cur);
		ScreenToClient(&point_cur);

		if (p_RectPlot_fin[graph_idx].left <= point_cur.x &&
			point_cur.x <= p_RectPlot_fin[graph_idx].right &&
			p_RectPlot_fin[graph_idx].top <= point_cur.y &&
			point_cur.y <= p_RectPlot_fin[graph_idx].bottom) {
			
			int check = point_cur.x;
			check -= p_RectPlot_fin[graph_idx].right;
			check *= N_GRAPH;
			check += pMainDlg->Get_m_count() + graph_idx;
			
			if (Pt_forth > check) {
				MessageBox(_T("Select the point after the start index."),
						   _T("Notice"), MB_OK | MB_ICONINFORMATION);
			}
			else {
				point_cur.x -= p_RectPlot_fin[graph_idx].right;
				point_cur.x *= N_GRAPH;
				point_cur.x += pMainDlg->Get_m_count() + graph_idx;
				point_cur.y -= p_RectPlot_fin[graph_idx].top;

				Pt_back = point_cur.x;
				pMainDlg->Set_EndIdx((UINT)point_cur.x);

				// Generate analysis dialog
				// 1. Ask whether to generate the analysis window or not
				if (Pt_forth >= 0 && Pt_back >= Pt_forth) {
					if (IDYES == AfxMessageBox(_T("Want to analyze the clipped data?"), MB_YESNO)) {
						// 2. Generate new window
						Clip_window = new GraphClipping(pMainDlg->Get_Num_sEMG_CH(),
									Pt_forth, Pt_back, pMainDlg->Get_m_count(),
									pMainDlg->Get_sEMG_MAV_stack(), pMainDlg->Get_Finger_raw_stack(),
									pMainDlg->Get_Finger_slope_stack(), pMainDlg->Get_Wrist_raw_stack(),
									pMainDlg->Get_Wrist_slope_stack(), pMainDlg->Get_MotionLabel_stack(),
									pMainDlg->Get_MotionEstimation_stack(), pMainDlg->Get_X_pos_ball_stack(),
									pMainDlg->Get_Y_pos_ball_stack(), pMainDlg->Get_Rad_ball_stack());
						Clip_window->Create(IDD_DAQVIZ_DIALOG_GRAPH_CLIPPING, this);
						Clip_window->ShowWindow(SW_SHOW);
					}
					else {
						Pt_forth = 0;
						Pt_back = 0;
						pMainDlg->Initialize_StartIdx();
						pMainDlg->Initialize_EndIdx();
					}
				}
				else {
					if (Pt_forth < 0) {
						MessageBox(_T("Start index is negative."),
							_T("Notice"), MB_OK | MB_ICONWARNING);
					}
					else {
						MessageBox(_T("End index is smaller than start index."),
							_T("Notice"), MB_OK | MB_ICONWARNING);
					}
					Pt_forth = 0;
					Pt_back = 0;
					pMainDlg->Initialize_StartIdx();
					pMainDlg->Initialize_EndIdx();
				}
				
				m_NumClicked[graph_idx] = 0;
			}
		}
		else {
			MessageBox(_T("Please click the plot space."),
					   _T("Notice"), MB_OK | MB_ICONWARNING);
		}
	}
}