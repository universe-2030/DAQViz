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
	N_sEMG = N_SEMG_CH;
	N_Flex = N_FLEX_CH;
	N_IMU = N_IMU_CH;
}

DAQVizChildKSJ::DAQVizChildKSJ(int _N_sEMG_CH, int _N_Flex_CH, int _N_IMU_CH,
								CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAQVIZ_DIALOG_CHILD_KSJ, pParent) {
	N_sEMG = _N_sEMG_CH;
	N_Flex = _N_Flex_CH;
	N_IMU = _N_IMU_CH;
}

DAQVizChildKSJ::~DAQVizChildKSJ() {
	delete sEMG_raw;
	delete sEMG_MAV;

	delete Flex_data;
	delete IMU_data;
}

void DAQVizChildKSJ::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TEXT_SEMG_PLOT, m_textsEMGPlot);
	DDX_Control(pDX, IDC_TEXT_MOTION, m_textMotionRendering);
}

BEGIN_MESSAGE_MAP(DAQVizChildKSJ, CDialogEx)
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
	Graph_region = new CRect[N_GRAPH];

	rtGraph_sEMG_MAV = new COScopeCtrl*[4];
	rtGraph_Flex = new COScopeCtrl*;
	rtGraph_IMU = new COScopeCtrl*;

	sEMG_MAV = new double[N_sEMG];
	Flex_data = new double[N_Flex];
	IMU_data = new double[N_IMU];
}

void DAQVizChildKSJ::Initialize_GUI() {
	for (int i = 0; i < N_GRAPH; i++) {
		UINT ID_Ctrl = IDC_GRAPH_SEMG_MAV_1 + i;
		GetDlgItem(ID_Ctrl)->GetWindowRect(Graph_region[i]);
		ScreenToClient(Graph_region[i]);

		// For check (temporary)
		std::cout << "Left : " << Graph_region[i].left << std::endl;
		std::cout << "Top : " << Graph_region[i].top << std::endl;
		std::cout << "Right : " << Graph_region[i].right << std::endl;
		std::cout << "Bottom : " << Graph_region[i].bottom << std::endl;
		std::cout << std::endl;
	}

	CDAQVizDlg* pMainDlg = new CDAQVizDlg();
	pMainDlg->Set_Font(m_textsEMGPlot, 20, 8);
	pMainDlg->Set_Font(m_textMotionRendering, 20, 8);

	rtGraph_sEMG_MAV[0] = Initialize_graph(IDC_GRAPH_SEMG_MAV_1, 1, 4, rtGraph_sEMG_MAV[0], SEMG_MAV);
	rtGraph_sEMG_MAV[1] = Initialize_graph(IDC_GRAPH_SEMG_MAV_2, 5, 8, rtGraph_sEMG_MAV[1], SEMG_MAV);
	rtGraph_sEMG_MAV[2] = Initialize_graph(IDC_GRAPH_SEMG_MAV_3, 9, 12, rtGraph_sEMG_MAV[2], SEMG_MAV);
	rtGraph_sEMG_MAV[3] = Initialize_graph(IDC_GRAPH_SEMG_MAV_4, 13, 16, rtGraph_sEMG_MAV[3], SEMG_MAV);

	rtGraph_Flex[0] = Initialize_graph(IDC_GRAPH_FLEX_SENSOR, 1, 5, rtGraph_Flex[0], FLEX_SENSOR);

	rtGraph_IMU[0] = Initialize_graph(IDC_GRAPH_LOGONU_IMU, 1, 2, rtGraph_IMU[0], IMU);

	CRect rectofDialogArea;
	GetDlgItem(IDC_PLOT_OPENGL)->GetWindowRect(&rectofDialogArea);
	ScreenToClient(&rectofDialogArea);

	p_ChildOpenGL = new DAQVizChildOpenGL();
	p_ChildOpenGL->Create(IDD_DAQVIZ_DIALOG_CHILD_OPENGL, this);
	p_ChildOpenGL->ShowWindow(SW_SHOW);
	p_ChildOpenGL->MoveWindow(rectofDialogArea);

	GetDlgItem(IDC_PLOT_MOTION)->GetWindowRect(&rectofDialogArea);
	ScreenToClient(&rectofDialogArea);

	p_ChildOpenGL_2 = new DAQVizChildOpenGL2();
	p_ChildOpenGL_2->Create(IDD_DAQVIZ_DIALOG_CHILD_OPENGL_2, this);
	p_ChildOpenGL_2->ShowWindow(SW_SHOW);
	p_ChildOpenGL_2->MoveWindow(rectofDialogArea);

	delete pMainDlg;
}

COScopeCtrl* DAQVizChildKSJ::Initialize_graph (int ID, int idx_start, int idx_end,
									   COScopeCtrl* rtGraph, Graph_class _class) {
	CRect _rtGraph;
	GetDlgItem(ID)->GetWindowRect(_rtGraph);
	ScreenToClient(_rtGraph);

	int N_graph = idx_end - idx_start + 1;
	rtGraph = new COScopeCtrl(N_graph);
	rtGraph->Create(WS_VISIBLE | WS_CHILD, _rtGraph, this, ID);

	CString temp_str;
	for (int _idx = idx_start; _idx <= idx_end; _idx++) {
		if (_class == SEMG_MAV) {
			temp_str.Format(_T("CH %d"), _idx);
			rtGraph->SetRanges(SEMG_MAV_MIN, SEMG_MAV_MAX);
			rtGraph->autofitYscale = true;
			rtGraph->SetYUnits(_T("sEMG MAV"));
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

			rtGraph->InvalidateCtrl();
		}
		else if (_class == FLEX_SENSOR) {
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

			rtGraph->SetRanges(FLEX_SENSOR_MIN, FLEX_SENSOR_MAX);
			rtGraph->autofitYscale = true;
			rtGraph->SetYUnits(_T("Flex sensor"));
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
		else if (_class == IMU) {
			if (_idx == 1)
				temp_str = _T("Flex / Ext");
			else if (_idx == 2)
				temp_str = _T("Radial / Ulnar");

			rtGraph->SetRanges(IMU_MIN, IMU_MAX);
			rtGraph->autofitYscale = true;
			rtGraph->SetYUnits(_T("Angle [Deg]"));
			rtGraph->SetXUnits(_T("Time"));

			int _idx_rev = _idx - idx_start;
			rtGraph->SetLegendLabel(temp_str, _idx_rev);

			if (_idx_rev == 0)
				rtGraph->SetPlotColor(RGB(255, 0, 0), _idx_rev);
			else if (_idx_rev == 1)
				rtGraph->SetPlotColor(RGB(0, 255, 0), _idx_rev);

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

COScopeCtrl** DAQVizChildKSJ::Get_rtGraph_Flex() {
	return rtGraph_Flex;
}

DAQVizChildOpenGL* DAQVizChildKSJ::Get_OpenGLPointer() {
	return p_ChildOpenGL;
}