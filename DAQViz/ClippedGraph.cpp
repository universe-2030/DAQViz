// ClippedGraph.cpp: 구현 파일
//

#include "pch.h"
#include "DAQViz.h"
#include "DAQVizDlg.h"
#include "ClippedGraph.h"
#include "afxdialogex.h"
#include "GraphClipping.h"

// ClippedGraph 대화 상자

IMPLEMENT_DYNAMIC(ClippedGraph, CDialogEx)

ClippedGraph::ClippedGraph(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAQVIZ_DIALOG_CLIPPED_GRAPH, pParent) {
	Num_CH = NUM_CH;
}

ClippedGraph::ClippedGraph(int _m_Start_idx, int _m_End_idx,
						int _m_Num_idx, int _Num_CH, 
						const std::vector<double>* _sEMG_plot,
						const std::vector<double>* _Finger_plot,
						const std::vector<double>* _Finger_slope_plot,
						const std::vector<double>* _Wrist_plot,
						const std::vector<double>* _Wrist_slope_plot,
						const std::vector<double>* _MotionLabel_plot,
						const std::vector<double>* _MotionEstimation_plot,
						Render _species, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAQVIZ_DIALOG_CLIPPED_GRAPH, pParent) {
	m_Start_idx = _m_Start_idx;
	m_End_idx = _m_End_idx;
	m_Num_idx = _m_Num_idx;
	Num_CH = _Num_CH;

	sEMG_plot = _sEMG_plot;

	Finger_plot = _Finger_plot;
	Finger_slope_plot = _Finger_slope_plot;
	Wrist_plot = _Wrist_plot;
	Wrist_slope_plot = _Wrist_slope_plot;

	MotionLabel_plot = _MotionLabel_plot;
	MotionEstimation_plot = _MotionEstimation_plot;

	species = _species;
}

ClippedGraph::~ClippedGraph() {
	Delete_Dynamic_Alloc();
}

void ClippedGraph::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(ClippedGraph, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// ClippedGraph 메시지 처리기

BOOL ClippedGraph::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	X_pos = new double[m_Num_idx];
	for (int i = 0; i < m_Num_idx; i++) {
		X_pos[i] = X_Axis_start + (X_Axis_end - X_Axis_start) / (double)(m_Num_idx) * i;
	}

	SetTimer(TIMER_RENDER, TIME_ELAPSE, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void ClippedGraph::OnPaint() {
	if (IsIconic()) {
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else {
		CDialogEx::OnPaint();

		wglMakeCurrent(m_hDC, m_hRC);
		if (species == TOTAL)
			GLRenderScene_Total();
		else if (species == ANIMATION)
			GLRenderScene_Animation();
		SwapBuffers(m_hDC);
		wglMakeCurrent(m_hDC, NULL);
	}
}

void ClippedGraph::OnDestroy() {
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	wglDeleteContext(m_hRC);
	::ReleaseDC(m_hWnd, m_hDC);
}

void ClippedGraph::OnTimer(UINT_PTR nIDEvent) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent) {
	case TIMER_RENDER:

		break;
	case TIMER_ANIMATION:
		Current_idx += TimeStep;
		if (Current_idx > m_Num_idx)
			Current_idx = 0;
		break;
	}
	this->Invalidate(FALSE);

	CDialogEx::OnTimer(nIDEvent);
}

int ClippedGraph::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	int nPixelFormat;
	m_hDC = ::GetDC(m_hWnd);

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0, 0,
		0, 0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	nPixelFormat = ChoosePixelFormat(m_hDC, &pfd);
	VERIFY(SetPixelFormat(m_hDC, nPixelFormat, &pfd));
	m_hRC = wglCreateContext(m_hDC);
	VERIFY(wglMakeCurrent(m_hDC, m_hRC));
	wglMakeCurrent(NULL, NULL);

	return 0;
}

void ClippedGraph::OnSize(UINT nType, int cx, int cy) {
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	VERIFY(wglMakeCurrent(m_hDC, m_hRC));
	GLResize(cx, cy);
	VERIFY(wglMakeCurrent(NULL, NULL));
}

BOOL ClippedGraph::OnEraseBkgnd(CDC* pDC) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// return CDialogEx::OnEraseBkgnd(pDC);
	return FALSE;
}

void ClippedGraph::GLResize(int cx, int cy) {
	// TODO: 여기에 구현 코드 추가.
	if (cy == 0)
		cy = 1;

	glViewport(0, 0, cx, cy);

	fAspect = (GLfloat)cy / (GLfloat)cx;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0f, fAspect, 1.0f, 10000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ClippedGraph::GLRenderScene_Total(void) {
	// TODO: 여기에 구현 코드 추가.
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	// clear the drawing buffer.

	gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	///////////////////////////// X-Y Axis (sEMG + Flex sensor) /////////////////////////////
	glLoadIdentity();
	glColor3f(0.0f, 0.0f, 0.0f);

	glTranslatef(0.0, 0.0, -5.0);

	glLineWidth(3);
	for (int i = 0; i < NUM_GRAPH_ANALYSIS; i++) {
		// X axis
		glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 0.0f);
		if (i != 3 && i != 4 && i != 5 && i != 6) {
			glVertex3f(X_Axis_start, 2.7f - (GRAPH_Y_LEN_TOTAL + GRAPH_Y_INTERVAL_TOTAL) * i - GRAPH_Y_LEN_TOTAL, 0.0f);
			glVertex3f(X_Axis_end, 2.7f - (GRAPH_Y_LEN_TOTAL + GRAPH_Y_INTERVAL_TOTAL) * i - GRAPH_Y_LEN_TOTAL, 0.0f);
		}
		else if (i == 3) {
			glVertex3f(X_Axis_start, 2.7f - (GRAPH_Y_LEN_TOTAL + GRAPH_Y_INTERVAL_TOTAL) * i - GRAPH_Y_LEN_TOTAL * 0.2, 0.0f);
			glVertex3f(X_Axis_end, 2.7f - (GRAPH_Y_LEN_TOTAL + GRAPH_Y_INTERVAL_TOTAL) * i - GRAPH_Y_LEN_TOTAL * 0.2, 0.0f);
		}
		else if (i == 4 || i == 5 || i == 6) {
			glVertex3f(X_Axis_start, 2.7f - (GRAPH_Y_LEN_TOTAL + GRAPH_Y_INTERVAL_TOTAL) * i - GRAPH_Y_LEN_TOTAL * 0.5, 0.0f);
			glVertex3f(X_Axis_end, 2.7f - (GRAPH_Y_LEN_TOTAL + GRAPH_Y_INTERVAL_TOTAL) * i - GRAPH_Y_LEN_TOTAL * 0.5, 0.0f);
		}
		glEnd();

		// Y axis
		glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 0.0f);
			glVertex3f(X_Axis_start, 2.7f - (GRAPH_Y_LEN_TOTAL + GRAPH_Y_INTERVAL_TOTAL) * i, 0.0f);
			glVertex3f(X_Axis_start, 2.7f - (GRAPH_Y_LEN_TOTAL + GRAPH_Y_INTERVAL_TOTAL) * i - GRAPH_Y_LEN_TOTAL, 0.0f);
		glEnd();
	}

	/////////////////////////////// sEMG Graph ///////////////////////////////
	glLineWidth(1.5);
	for (int i = 0; i < Num_CH; i++) {
		int N_Y_interval;
		int N_Y_len;
		if (0 <= i && i < 5) { // CH 1 - 5
			N_Y_interval = 0;
			N_Y_len = 1;
		}
		else if (5 <= i && i < 10) { // CH 6 - 10
			N_Y_interval = 1;
			N_Y_len = 2;
		}
		else if (10 <= i && i < 16) { // CH 11 - 16
			N_Y_interval = 2;
			N_Y_len = 3;
		}

		if (i == 0 || i == 5 || i == 10)
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (i == 1 || i == 6 || i == 11)
			glColor3f(0.0f, 1.0f, 0.0f);
		else if (i == 2 || i == 7 || i == 12)
			glColor3f(0.0f, 0.0f, 1.0f);
		else if (i == 3 || i == 8 || i == 13)
			glColor3f(1.0f, 1.0f, 0.0f);
		else if (i == 4 || i == 9 || i == 14)
			glColor3f(1.0f, 0.0f, 1.0f);
		else if (i == 15)
			glColor3f(0.0f, 1.0f, 1.0f);

		glBegin(GL_LINE_STRIP);
			for (int j = 0; j < m_Num_idx; j++) {
				glVertex3f(X_pos[j], 2.7f - GRAPH_Y_INTERVAL_TOTAL * N_Y_interval
							- GRAPH_Y_LEN_TOTAL * N_Y_len + GRAPH_Y_LEN_TOTAL * sEMG_plot[i][j + m_Start_idx - 1] / SEMG_VAL_MAX, 0.0f);
			}
		glEnd();
	}

	/////////////////////////////// Finger graph ///////////////////////////////
	glLineWidth(1.5);
	double N_Y_interval = 3;
	double N_Y_len = 3.2;
	for (int i = 0; i < 5; i++) {
		if (i == 0)
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (i == 1)
			glColor3f(0.0f, 1.0f, 0.0f);
		else if (i == 2)
			glColor3f(0.0f, 0.0f, 1.0f);
		else if (i == 3)
			glColor3f(1.0f, 1.0f, 0.0f);
		else if (i == 4)
			glColor3f(1.0f, 0.0f, 1.0f);

		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < m_Num_idx; j++) {
			glVertex3f(X_pos[j], 2.7f - GRAPH_Y_INTERVAL_TOTAL * N_Y_interval
				- GRAPH_Y_LEN_TOTAL * N_Y_len + 0.8 * GRAPH_Y_LEN_TOTAL * Finger_plot[i][j + m_Start_idx - 1] / FINGER_VAL_MAX, 0.0f);
		}
		glEnd();
	}

	/////////////////////////////// Finger slope graph ///////////////////////////////
	glLineWidth(1.5);
	N_Y_interval = 4;
	N_Y_len = 4.5;
	for (int i = 0; i < 5; i++) {
		if (i == 0)
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (i == 1)
			glColor3f(0.0f, 1.0f, 0.0f);
		else if (i == 2)
			glColor3f(0.0f, 0.0f, 1.0f);
		else if (i == 3)
			glColor3f(1.0f, 1.0f, 0.0f);
		else if (i == 4)
			glColor3f(1.0f, 0.0f, 1.0f);

		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < m_Num_idx; j++) {
			glVertex3f(X_pos[j], 2.7f - GRAPH_Y_INTERVAL_TOTAL * N_Y_interval
				- GRAPH_Y_LEN_TOTAL * N_Y_len + 0.5 * GRAPH_Y_LEN_TOTAL * Finger_slope_plot[i][j + m_Start_idx - 1] / FINGER_SLOPE_MAX, 0.0f);
		}
		glEnd();
	}

	/////////////////////////////////// Wrist graph ///////////////////////////////////
	glLineWidth(1.5);
	N_Y_interval = 5;
	N_Y_len = 5.5;
	for (int i = 0; i < 2; i++) {
		if (i == 0)
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (i == 1)
			glColor3f(0.0f, 0.0f, 1.0f);

		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < m_Num_idx; j++) {
			glVertex3f(X_pos[j], 2.7f - GRAPH_Y_INTERVAL_TOTAL * N_Y_interval
				- GRAPH_Y_LEN_TOTAL * N_Y_len + 0.5 * GRAPH_Y_LEN_TOTAL * Wrist_plot[i][j + m_Start_idx - 1] / WRIST_VAL_MAX, 0.0f);
		}
		glEnd();
	}

	/////////////////////////////////// Wrist slope graph ///////////////////////////////////
	glLineWidth(1.5);
	N_Y_interval = 6;
	N_Y_len = 6.5;
	for (int i = 0; i < 2; i++) {
		if (i == 0)
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (i == 1)
			glColor3f(0.0f, 0.0f, 1.0f);

		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < m_Num_idx; j++) {
			glVertex3f(X_pos[j], 2.7f - GRAPH_Y_INTERVAL_TOTAL * N_Y_interval
				- GRAPH_Y_LEN_TOTAL * N_Y_len + 0.5 * GRAPH_Y_LEN_TOTAL * Wrist_slope_plot[i][j + m_Start_idx - 1] / WRIST_SLOPE_MAX, 0.0f);
		}
		glEnd();
	}

	/////////////////////////////// Motion index graph ///////////////////////////////
	glLineWidth(1.5);
	N_Y_interval = 7;
	N_Y_len = 8;
	for (int i = 0; i < 2; i++) {
		if (i == 0)
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (i == 1)
			glColor3f(0.0f, 0.0f, 1.0f);

		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < m_Num_idx; j++) {
			if (i == 0)
				glVertex3f(X_pos[j], 2.7f - GRAPH_Y_INTERVAL_TOTAL * N_Y_interval
					- GRAPH_Y_LEN_TOTAL * N_Y_len + GRAPH_Y_LEN_TOTAL * MotionLabel_plot[0][j + m_Start_idx - 1] / MOTION_IDX_MAX, 0.0f);
			else if (i == 1)
				glVertex3f(X_pos[j], 2.7f - GRAPH_Y_INTERVAL_TOTAL * N_Y_interval
					- GRAPH_Y_LEN_TOTAL * N_Y_len + GRAPH_Y_LEN_TOTAL * MotionEstimation_plot[0][j + m_Start_idx - 1] / MOTION_IDX_MAX, 0.0f);
		}
		glEnd();
	}

	glPopMatrix();
	glFlush();
}

void ClippedGraph::GLRenderScene_Animation(void) {
	// TODO: 여기에 구현 코드 추가.
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	// clear the drawing buffer.

	gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	///////////////////////////// X-Y Axis (sEMG + Flex sensor) /////////////////////////////
	glLoadIdentity();
	glColor3f(0.0f, 0.0f, 0.0f);

	glTranslatef(0.0, 0.0, -5.0);

	glLineWidth(3);
	for (int i = 0; i < NUM_GRAPH_ANALYSIS; i++) {
		// X axis
		glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 0.0f);
		if (i != 3 && i != 4 && i != 5 && i != 6) {
			glVertex3f(X_Axis_start, 2.7f - (GRAPH_Y_LEN_ANI + GRAPH_Y_INTERVAL_ANI) * i - GRAPH_Y_LEN_ANI, 0.0f);
			glVertex3f(X_Axis_end, 2.7f - (GRAPH_Y_LEN_ANI + GRAPH_Y_INTERVAL_ANI) * i - GRAPH_Y_LEN_ANI, 0.0f);
		}
		else if (i == 3) {
			glVertex3f(X_Axis_start, 2.7f - (GRAPH_Y_LEN_ANI + GRAPH_Y_INTERVAL_ANI) * i - GRAPH_Y_LEN_ANI * 0.2, 0.0f);
			glVertex3f(X_Axis_end, 2.7f - (GRAPH_Y_LEN_ANI + GRAPH_Y_INTERVAL_ANI) * i - GRAPH_Y_LEN_ANI * 0.2, 0.0f);
		}
		else if (i == 4 || i == 5 || i == 6) {
			glVertex3f(X_Axis_start, 2.7f - (GRAPH_Y_LEN_ANI + GRAPH_Y_INTERVAL_ANI) * i - GRAPH_Y_LEN_ANI * 0.5, 0.0f);
			glVertex3f(X_Axis_end, 2.7f - (GRAPH_Y_LEN_ANI + GRAPH_Y_INTERVAL_ANI) * i - GRAPH_Y_LEN_ANI * 0.5, 0.0f);
		}
		glEnd();

		// Y axis
		glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(X_Axis_start, 2.7f - (GRAPH_Y_LEN_ANI + GRAPH_Y_INTERVAL_ANI) * i, 0.0f);
		glVertex3f(X_Axis_start, 2.7f - (GRAPH_Y_LEN_ANI + GRAPH_Y_INTERVAL_ANI) * i - GRAPH_Y_LEN_ANI, 0.0f);
		glEnd();
	}

	/////////////////////////////// sEMG Graph ///////////////////////////////
	glLineWidth(1.5);
	for (int i = 0; i < Num_CH; i++) {
		int N_Y_interval;
		int N_Y_len;
		if (0 <= i && i < 5) { // CH 1 - 5
			N_Y_interval = 0;
			N_Y_len = 1;
		}
		else if (5 <= i && i < 10) { // CH 6 - 10
			N_Y_interval = 1;
			N_Y_len = 2;
		}
		else if (10 <= i && i < 15) { // CH 11 - 16
			N_Y_interval = 2;
			N_Y_len = 3;
		}

		if (i == 0 || i == 5 || i == 10)
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (i == 1 || i == 6 || i == 11)
			glColor3f(0.0f, 1.0f, 0.0f);
		else if (i == 2 || i == 7 || i == 12)
			glColor3f(0.0f, 0.0f, 1.0f);
		else if (i == 3 || i == 8 || i == 13)
			glColor3f(1.0f, 1.0f, 0.0f);
		else if (i == 4 || i == 9 || i == 14)
			glColor3f(1.0f, 0.0f, 1.0f);
		else if (i == 15)
			glColor3f(0.0f, 1.0f, 1.0f);

		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < m_Num_idx; j++) {
			glVertex3f(X_pos[j], 2.7f - GRAPH_Y_INTERVAL_ANI * N_Y_interval
				- GRAPH_Y_LEN_ANI * N_Y_len + GRAPH_Y_LEN_ANI * sEMG_plot[i][j + m_Start_idx - 1] / SEMG_VAL_MAX, 0.0f);
		}
		glEnd();
	}

	/////////////////////////////// Finger graph ///////////////////////////////
	glLineWidth(1.5);
	double N_Y_interval = 3;
	double N_Y_len = 3.2;
	for (int i = 0; i < 5; i++) {
		if (i == 0)
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (i == 1)
			glColor3f(0.0f, 1.0f, 0.0f);
		else if (i == 2)
			glColor3f(0.0f, 0.0f, 1.0f);
		else if (i == 3)
			glColor3f(1.0f, 1.0f, 0.0f);
		else if (i == 4)
			glColor3f(1.0f, 0.0f, 1.0f);

		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < m_Num_idx; j++) {
			glVertex3f(X_pos[j], 2.7f - GRAPH_Y_INTERVAL_ANI * N_Y_interval
				- GRAPH_Y_LEN_ANI * N_Y_len + 0.8 * GRAPH_Y_LEN_ANI * Finger_plot[i][j + m_Start_idx - 1] / FINGER_VAL_MAX, 0.0f);
		}
		glEnd();
	}

	/////////////////////////////// Finger slope graph ///////////////////////////////
	glLineWidth(1.5);
	N_Y_interval = 4;
	N_Y_len = 4.5;
	for (int i = 0; i < 5; i++) {
		if (i == 0)
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (i == 1)
			glColor3f(0.0f, 1.0f, 0.0f);
		else if (i == 2)
			glColor3f(0.0f, 0.0f, 1.0f);
		else if (i == 3)
			glColor3f(1.0f, 1.0f, 0.0f);
		else if (i == 4)
			glColor3f(1.0f, 0.0f, 1.0f);

		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < m_Num_idx; j++) {
			glVertex3f(X_pos[j], 2.7f - GRAPH_Y_INTERVAL_ANI * N_Y_interval
				- GRAPH_Y_LEN_ANI * N_Y_len + 0.5 * GRAPH_Y_LEN_ANI * Finger_slope_plot[i][j + m_Start_idx - 1] / FINGER_SLOPE_MAX, 0.0f);
		}
		glEnd();
	}

	/////////////////////////////////// Wrist graph ///////////////////////////////////
	glLineWidth(1.5);
	N_Y_interval = 5;
	N_Y_len = 5.5;
	for (int i = 0; i < 2; i++) {
		if (i == 0)
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (i == 1)
			glColor3f(0.0f, 0.0f, 1.0f);

		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < m_Num_idx; j++) {
			glVertex3f(X_pos[j], 2.7f - GRAPH_Y_INTERVAL_ANI * N_Y_interval
				- GRAPH_Y_LEN_ANI * N_Y_len + 0.5 * GRAPH_Y_LEN_ANI * Wrist_plot[i][j + m_Start_idx - 1] / WRIST_VAL_MAX, 0.0f);
		}
		glEnd();
	}

	/////////////////////////////////// Wrist slope graph ///////////////////////////////////
	glLineWidth(1.5);
	N_Y_interval = 6;
	N_Y_len = 6.5;
	for (int i = 0; i < 2; i++) {
		if (i == 0)
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (i == 1)
			glColor3f(0.0f, 0.0f, 1.0f);

		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < m_Num_idx; j++) {
			glVertex3f(X_pos[j], 2.7f - GRAPH_Y_INTERVAL_ANI * N_Y_interval
				- GRAPH_Y_LEN_ANI * N_Y_len + 0.5 * GRAPH_Y_LEN_ANI * Wrist_slope_plot[i][j + m_Start_idx - 1] / WRIST_SLOPE_MAX, 0.0f);
		}
		glEnd();
	}

	/////////////////////////////// Motion index graph ///////////////////////////////
	glLineWidth(1.5);
	N_Y_interval = 7;
	N_Y_len = 8;
	for (int i = 0; i < 2; i++) {
		if (i == 0)
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (i == 1)
			glColor3f(0.0f, 0.0f, 1.0f);

		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < m_Num_idx; j++) {
			if (i == 0)
				glVertex3f(X_pos[j], 2.7f - GRAPH_Y_INTERVAL_ANI * N_Y_interval
					- GRAPH_Y_LEN_ANI * N_Y_len + GRAPH_Y_LEN_ANI * MotionLabel_plot[0][j + m_Start_idx - 1] / MOTION_IDX_MAX, 0.0f);
			else if (i == 1)
				glVertex3f(X_pos[j], 2.7f - GRAPH_Y_INTERVAL_ANI * N_Y_interval
					- GRAPH_Y_LEN_ANI * N_Y_len + GRAPH_Y_LEN_ANI * MotionEstimation_plot[0][j + m_Start_idx - 1] / MOTION_IDX_MAX, 0.0f);
		}
		glEnd();
	}

	/////////////////////////////// Graph current index ///////////////////////////////
	glLineWidth(5.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex3f(X_pos[Current_idx], 3.0f, 0.0f);
		glVertex3f(X_pos[Current_idx], -0.5f, 0.0f);
	glEnd();

	/////////////////////////////// Polygon frame ///////////////////////////////
	double* sEMG_plot_temp = new double[Num_CH];
	for (int i = 0; i < Num_CH; i++)
		sEMG_plot_temp[i] = sEMG_plot[i][Current_idx + m_Start_idx - 1];

	// Load the 
	CDAQVizDlg* pMainDlg = (CDAQVizDlg*)AfxGetMainWnd();
	double** sEMG_plot_mean = new double*[N_MOTIONS];
	for (int i = 0; i < N_MOTIONS; i++)
		sEMG_plot_mean[i] = new double[Num_CH];

	if (pMainDlg->Get_m_radioTrainingMode() == 1) { // Training
		for (int i = 0; i < N_MOTIONS; i++)
			for (int j = 0; j < Num_CH; j++)
				sEMG_plot_mean[i][j] = pMainDlg->Get_sEMG_MAV_stack_motionwise_mean_stack()[i][j][Current_idx + m_Start_idx - 1];
	}
	else if (pMainDlg->Get_m_radioTrainingMode() == 2) { // Test
		for (int i = 0; i < N_MOTIONS; i++)
			for (int j = 0; j < Num_CH; j++)
				sEMG_plot_mean[i][j] = pMainDlg->Get_sEMG_mean_Param()[i][j];
	}

	X_polygon = -2.4f;
	Y_polygon = -1.35f;
	Rad_max = 0.8;

	Plot_polygon(sEMG_plot_temp, sEMG_plot_mean[0], 1, 15, X_polygon, Y_polygon, Rad_max, TRUE);

	X_polygon = 0.0f;
	Y_polygon = -1.35f;
	Rad_max = 0.8;

	Plot_polygon(sEMG_plot_temp, sEMG_plot_mean[1], 1, 15, X_polygon, Y_polygon, Rad_max, TRUE);

	X_polygon = 2.4f;
	Y_polygon = -1.35f;
	Rad_max = 0.8;

	Plot_polygon(sEMG_plot_temp, sEMG_plot_mean[2], 1, 15, X_polygon, Y_polygon, Rad_max, TRUE);

	X_polygon = -2.4f;
	Y_polygon = -2.7f;
	Rad_max = 0.8;

	Plot_polygon(sEMG_plot_temp, sEMG_plot_mean[3], 1, 15, X_polygon, Y_polygon, Rad_max, TRUE);

	X_polygon = 0.0f;
	Y_polygon = -2.7f;
	Rad_max = 0.8;

	Plot_polygon(sEMG_plot_temp, sEMG_plot_mean[4], 1, 15, X_polygon, Y_polygon, Rad_max, TRUE);

	X_polygon = 2.4f;
	Y_polygon = -2.7f;
	Rad_max = 0.8;

	Plot_polygon(sEMG_plot_temp, sEMG_plot_mean[5], 1, 15, X_polygon, Y_polygon, Rad_max, TRUE);

	delete sEMG_plot_temp;
}

void ClippedGraph::Plot_polygon(const double* data, int _m_StartIdx, int _m_EndIdx,
								double _X_center, double _Y_center, double _Rad, bool _Normalization) {
	glPushMatrix();

	glEnable(GL_DEPTH_TEST);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glShadeModel(GL_FLAT);
	glLoadIdentity();

	glTranslatef(0.0, 0.0, -5.0);
	gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	int Num_vertex = _m_EndIdx - _m_StartIdx + 1;

	// Draw polygon frames
	X_polygon = _X_center;
	Y_polygon = _Y_center;
	Rad_max = _Rad;

	for (int i = 0; i < N_GRID_STEP; i++) {
		if (i == N_GRID_STEP - 1)
			glLineWidth(2.0);
		else
			glLineWidth(1.0);

		glColor3f(0.0f, 0.0f, 0.0f);
		double rad = Rad_max / (double)N_GRID_STEP * (i + 1);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < Num_vertex; j++) {
			glVertex3f(X_polygon + rad * cos(PI / 2.0 + 2 / (double)Num_vertex * PI * j),
					Y_polygon + 1 / fAspect * rad * sin(PI / 2.0 + 2 / (double)Num_vertex * PI * j),
					0);
		}
		glVertex3f(X_polygon + rad * cos(PI / 2.0), Y_polygon + 1 / fAspect * rad * sin(PI / 2.0), 0);
		glEnd();
	}

	// Draw center - vertex lines
	glLineWidth(0.1);
	for (int i = 0; i < Num_vertex; i++) {
		glBegin(GL_LINE_STRIP);
		glVertex3f(X_polygon, Y_polygon, 0);
		glVertex3f(X_polygon + Rad_max * cos(PI / 2.0 + 2 / (double)Num_vertex * PI * i),
				Y_polygon + Rad_max / fAspect * sin(PI / 2.0 + 2 / (double)Num_vertex * PI * i),
				0);
		glEnd();
	}

	if (_Normalization) {
		// Draw sEMG data (Normalized)
		double* data_normalized = new double[Num_vertex];
		double max_data = data[_m_StartIdx];

		for (int i = 0; i < Num_vertex; i++)
			if (data[i + _m_StartIdx - 1] >= max_data)
				max_data = data[i + _m_StartIdx - 1];

		for (int i = 0; i < Num_vertex; i++)
			data_normalized[i] = abs(data[i + _m_StartIdx - 1]) / max_data;

		glLineWidth(5.0);
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < Num_vertex; j++) {
			glVertex3f(X_polygon + Rad_max * data_normalized[j] * cos(PI / 2.0 + 2 / (double)Num_vertex * PI * j),
				Y_polygon + Rad_max / fAspect * data_normalized[j] * sin(PI / 2.0 + 2 / (double)Num_vertex * PI * j),
				0);

		}
		glVertex3f(X_polygon + Rad_max * data_normalized[0] * cos(PI / 2.0),
			Y_polygon + Rad_max / fAspect * data_normalized[0] * sin(PI / 2.0),
			0);
		glEnd();		

		delete data_normalized;
	}
	else {
		// Draw sEMG data (Not normalized)
		glLineWidth(5.0);
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
		for (int j = _m_StartIdx - 1; j < _m_EndIdx; j++) {
			glVertex3f(X_polygon + Rad_max * data[j] * cos(PI / 2.0 + 2 / (double)Num_vertex * PI * (j - _m_StartIdx + 1)),
				Y_polygon + 1 / fAspect * Rad_max * data[j] * sin(PI / 2.0 + 2 / (double)Num_vertex * PI * (j - _m_StartIdx + 1)),
				0);

		}
		glVertex3f(X_polygon + Rad_max * data[_m_StartIdx - 1] * cos(PI / 2.0),
			Y_polygon + Rad_max / fAspect * data[_m_StartIdx - 1] * sin(PI / 2.0),
			0);
		glEnd();
	}

	glPopMatrix();
	glFlush();
}

void ClippedGraph::Plot_polygon(const double* data, const double* data_mean, int _m_StartIdx, int _m_EndIdx,
	double _X_center, double _Y_center, double _Rad, bool _Normalization) {
	glPushMatrix();

	glEnable(GL_DEPTH_TEST);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glShadeModel(GL_FLAT);
	glLoadIdentity();

	glTranslatef(0.0, 0.0, -5.0);
	gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	int Num_vertex = _m_EndIdx - _m_StartIdx + 1;

	// Draw polygon frames
	X_polygon = _X_center;
	Y_polygon = _Y_center;
	Rad_max = _Rad;

	for (int i = 0; i < N_GRID_STEP; i++) {
		if (i == N_GRID_STEP - 1)
			glLineWidth(2.0);
		else
			glLineWidth(1.0);

		glColor3f(0.0f, 0.0f, 0.0f);
		double rad = Rad_max / (double)N_GRID_STEP * (i + 1);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < Num_vertex; j++) {
			glVertex3f(X_polygon + rad * cos(PI / 2.0 + 2 / (double)Num_vertex * PI * j),
				Y_polygon + 1 / fAspect * rad * sin(PI / 2.0 + 2 / (double)Num_vertex * PI * j),
				0);
		}
		glVertex3f(X_polygon + rad * cos(PI / 2.0), Y_polygon + 1 / fAspect * rad * sin(PI / 2.0), 0);
		glEnd();
	}

	// Draw center - vertex lines
	glLineWidth(0.1);
	for (int i = 0; i < Num_vertex; i++) {
		glBegin(GL_LINE_STRIP);
		glVertex3f(X_polygon, Y_polygon, 0);
		glVertex3f(X_polygon + Rad_max * cos(PI / 2.0 + 2 / (double)Num_vertex * PI * i),
			Y_polygon + Rad_max / fAspect * sin(PI / 2.0 + 2 / (double)Num_vertex * PI * i),
			0);
		glEnd();
	}

	if (_Normalization) {
		// Draw sEMG data (Normalization with maximum value being 1)
		double* data_normalized = new double[Num_vertex];
		double max_data = data[_m_StartIdx];
		double* data_mean_normalized = new double[Num_vertex];
		double max_data_mean = data_mean[_m_StartIdx];

		for (int i = 0; i < Num_vertex; i++) {
			if (data[i + _m_StartIdx - 1] >= max_data)
				max_data = data[i + _m_StartIdx - 1];
			if (data_mean[i + _m_StartIdx - 1] >= max_data_mean)
				max_data_mean = data_mean[i + _m_StartIdx - 1];
		}

		for (int i = 0; i < Num_vertex; i++) {
			data_normalized[i] = abs(data[i + _m_StartIdx - 1]) / max_data;
			data_mean_normalized[i] = abs(data_mean[i + _m_StartIdx - 1]) / max_data_mean;
		}

		glLineWidth(5.0);
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < Num_vertex; j++) {
			glVertex3f(X_polygon + Rad_max * data_normalized[j] * cos(PI / 2.0 + 2 / (double)Num_vertex * PI * j),
				Y_polygon + Rad_max / fAspect * data_normalized[j] * sin(PI / 2.0 + 2 / (double)Num_vertex * PI * j),
				0);

		}
		glVertex3f(X_polygon + Rad_max * data_normalized[0] * cos(PI / 2.0),
			Y_polygon + Rad_max / fAspect * data_normalized[0] * sin(PI / 2.0),
			0);
		glEnd();

		glLineWidth(5.0);
		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < Num_vertex; j++) {
			glVertex3f(X_polygon + Rad_max * data_mean_normalized[j] * cos(PI / 2.0 + 2 / (double)Num_vertex * PI * j),
				Y_polygon + Rad_max / fAspect * data_mean_normalized[j] * sin(PI / 2.0 + 2 / (double)Num_vertex * PI * j),
				0);

		}
		glVertex3f(X_polygon + Rad_max * data_mean_normalized[0] * cos(PI / 2.0),
			Y_polygon + Rad_max / fAspect * data_mean_normalized[0] * sin(PI / 2.0),
			0);
		glEnd();

		glPopMatrix();
		glFlush();

		delete data_normalized;
	}
	else {
		// Draw sEMG data (Not normalized)
		glLineWidth(5.0);
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
		for (int j = _m_StartIdx - 1; j < _m_EndIdx; j++) {
			glVertex3f(X_polygon + Rad_max * data[j] * cos(PI / 2.0 + 2 / (double)Num_vertex * PI * (j - _m_StartIdx + 1)),
				Y_polygon + 1 / fAspect * Rad_max * data[j] * sin(PI / 2.0 + 2 / (double)Num_vertex * PI * (j - _m_StartIdx + 1)),
				0);

		}
		glVertex3f(X_polygon + Rad_max * data[_m_StartIdx - 1] * cos(PI / 2.0),
			Y_polygon + Rad_max / fAspect * data[_m_StartIdx - 1] * sin(PI / 2.0),
			0);
		glEnd();

		glLineWidth(5.0);
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
		for (int j = _m_StartIdx - 1; j < _m_EndIdx; j++) {
			glVertex3f(X_polygon + Rad_max * data_mean[j] * cos(PI / 2.0 + 2 / (double)Num_vertex * PI * (j - _m_StartIdx + 1)),
				Y_polygon + 1 / fAspect * Rad_max * data_mean[j] * sin(PI / 2.0 + 2 / (double)Num_vertex * PI * (j - _m_StartIdx + 1)),
				0);

		}
		glVertex3f(X_polygon + Rad_max * data_mean[_m_StartIdx - 1] * cos(PI / 2.0),
			Y_polygon + Rad_max / fAspect * data_mean[_m_StartIdx - 1] * sin(PI / 2.0),
			0);
		glEnd();

		glPopMatrix();
		glFlush();
	}

	glPopMatrix();
	glFlush();
}

void ClippedGraph::Set_Current_idx(UINT _Current_idx) {
	Current_idx = _Current_idx;
}

void ClippedGraph::Set_TimeStep(UINT _TimeStep) {
	TimeStep = _TimeStep;
}

void ClippedGraph::Set_AnimationTimer() {
	SetTimer(TIMER_ANIMATION, TIME_ELAPSE, NULL);
}

void ClippedGraph::Kill_AnimationTimer() {
	KillTimer(TIMER_ANIMATION);
}

int ClippedGraph::Get_Current_idx() {
	return Current_idx;
}

void ClippedGraph::Delete_Dynamic_Alloc() {
	delete X_pos;
}

void ClippedGraph::Set_Dynamic_Alloc(UINT _m_Start_idx, UINT _m_End_idx, UINT _m_Num_idx) {
	m_Start_idx = _m_Start_idx;
	m_End_idx = _m_End_idx;
	m_Num_idx = _m_Num_idx;

	X_pos = new double[m_Num_idx];
	for (int i = 0; i < m_Num_idx; i++) {
		X_pos[i] = X_Axis_start + (X_Axis_end - X_Axis_start) / (double)(m_Num_idx) * i;
	}
}