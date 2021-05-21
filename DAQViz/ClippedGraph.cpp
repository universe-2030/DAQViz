// ClippedGraph.cpp: 구현 파일
//

#include "pch.h"
#include "DAQViz.h"
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
						const std::vector<double>* _Flex_plot,
						Render _species, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAQVIZ_DIALOG_CLIPPED_GRAPH, pParent) {
	m_Start_idx = _m_Start_idx;
	m_End_idx = _m_End_idx;
	m_Num_idx = _m_Num_idx;
	Num_CH = _Num_CH;

	sEMG_plot = _sEMG_plot;
	Flex_plot = _Flex_plot;

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
		X_pos[i] = -2.5f + 5.0f / (double)(m_Num_idx) * i;
	}

	Y_val = new double*[Num_CH];
	for (int i = 0; i < Num_CH; i++) {
		Y_val[i] = new double[m_Num_idx];
		for (int j = 0; j < m_Num_idx; j++) {
			Y_val[i][j] = rand() / (double)(RAND_MAX);
		}
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
		this->Invalidate(FALSE);
		break;
	case TIMER_ANIMATION:
		Current_idx += TimeStep;
		if (Current_idx > m_Num_idx)
			Current_idx = 0;
		break;
	}
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
		glVertex3f(-2.5f, 2.7f - (GRAPH_Y_LEN_TOTAL + GRAPH_Y_INTERVAL_TOTAL) * i, 0.0f);
		glVertex3f(-2.5f, 2.7f - (GRAPH_Y_LEN_TOTAL + GRAPH_Y_INTERVAL_TOTAL) * i - GRAPH_Y_LEN_TOTAL, 0.0f);
		glEnd();

		// Y axis
		glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-2.5f, 2.7f - (GRAPH_Y_LEN_TOTAL + GRAPH_Y_INTERVAL_TOTAL) * i - GRAPH_Y_LEN_TOTAL, 0.0f);
		glVertex3f(2.5f, 2.7f - (GRAPH_Y_LEN_TOTAL + GRAPH_Y_INTERVAL_TOTAL) * i - GRAPH_Y_LEN_TOTAL, 0.0f);
		glEnd();
	}

	/////////////////////////////// Graph ///////////////////////////////
	glLineWidth(1.5);
	for (int i = 0; i < Num_CH; i++) {
		int temp = i / NUM_GRAPH_ANALYSIS;

		if (i % NUM_GRAPH_ANALYSIS == 0)
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (i % NUM_GRAPH_ANALYSIS == 1)
			glColor3f(0.0f, 1.0f, 0.0f);
		else if (i % NUM_GRAPH_ANALYSIS == 2)
			glColor3f(0.0f, 0.0f, 1.0f);
		else if (i % NUM_GRAPH_ANALYSIS == 3)
			glColor3f(1.0f, 1.0f, 0.0f);

		glBegin(GL_LINE_STRIP);
			for (int j = 0; j < m_Num_idx; j++) {
				glVertex3f(X_pos[j], 2.7f - GRAPH_Y_INTERVAL_TOTAL * temp
							- GRAPH_Y_LEN_TOTAL * (temp + 1) + GRAPH_Y_LEN_TOTAL * sEMG_plot[i][j + m_Start_idx], 0.0f);
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
		glVertex3f(-2.5f, 2.7f - (GRAPH_Y_LEN_ANI + GRAPH_Y_INTERVAL_ANI) * i, 0.0f);
		glVertex3f(-2.5f, 2.7f - (GRAPH_Y_LEN_ANI + GRAPH_Y_INTERVAL_ANI) * i - GRAPH_Y_LEN_ANI, 0.0f);
		glEnd();

		// Y axis
		glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-2.5f, 2.7f - (GRAPH_Y_LEN_ANI + GRAPH_Y_INTERVAL_ANI) * i - GRAPH_Y_LEN_ANI, 0.0f);
		glVertex3f(2.5f, 2.7f - (GRAPH_Y_LEN_ANI + GRAPH_Y_INTERVAL_ANI) * i - GRAPH_Y_LEN_ANI, 0.0f);
		glEnd();
	}

	/////////////////////////////// Graph ///////////////////////////////
	glLineWidth(1.5);
	for (int i = 0; i < Num_CH; i++) {
		int temp = i / NUM_GRAPH_ANALYSIS;

		if (i % NUM_GRAPH_ANALYSIS == 0)
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (i % NUM_GRAPH_ANALYSIS == 1)
			glColor3f(0.0f, 1.0f, 0.0f);
		else if (i % NUM_GRAPH_ANALYSIS == 2)
			glColor3f(0.0f, 0.0f, 1.0f);
		else if (i % NUM_GRAPH_ANALYSIS == 3)
			glColor3f(1.0f, 1.0f, 0.0f);

		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < m_Num_idx; j++) {
			glVertex3f(X_pos[j], 2.7f - GRAPH_Y_INTERVAL_ANI * temp
				- GRAPH_Y_LEN_ANI * (temp + 1) + GRAPH_Y_LEN_ANI * sEMG_plot[i][j + m_Start_idx], 0.0f);
		}
		glEnd();
	}

	/////////////////////////////// Graph current index ///////////////////////////////
	glLineWidth(5.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex3f(X_pos[Current_idx], 3.0f, 0.0f);
		glVertex3f(X_pos[Current_idx], -1.0f, 0.0f);
	glEnd();

	/////////////////////////////// Polygon frame ///////////////////////////////
	X_polygon = -2.0f;
	Y_polygon = -2.0f;
	Rad_max = 0.8;
	for (int i = 0; i < N_GRID_STEP; i++) {
		if (i == N_GRID_STEP - 1)
			glLineWidth(2.0);
		else
			glLineWidth(0.3);

		glColor3f(0.0f, 0.0f, 0.0f);
		double rad = Rad_max / (double)N_GRID_STEP * (i + 1);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < 5; j++) {
			glVertex3f(X_polygon + rad * cos(PI / 2.0 + 2 / 5.0 * PI * j),
				Y_polygon + 1 / fAspect * rad * sin(PI / 2.0 + 2 / 5.0 * PI * j), 0);
		}
		glVertex3f(X_polygon + rad * cos(PI / 2.0), Y_polygon + 1 / fAspect * rad * sin(PI / 2.0), 0);
		glEnd();
	}

	// Draw center - vertex lines
	glLineWidth(0.1);
	for (int i = 0; i < 5; i++) {
		glBegin(GL_LINE_STRIP);
		glVertex3f(X_polygon, Y_polygon, 0);
		glVertex3f(X_polygon + Rad_max * cos(PI / 2.0 + 2 / 5.0 * PI * i),
			Y_polygon + 1 / fAspect * Rad_max * sin(PI / 2.0 + 2 / 5.0 * PI * i), 0);
		glEnd();
	}

	X_polygon = 0.0f;
	Y_polygon = -2.0f;
	Rad_max = 0.8;
	for (int i = 0; i < N_GRID_STEP; i++) {
		if (i == N_GRID_STEP - 1)
			glLineWidth(2.0);
		else
			glLineWidth(0.3);

		glColor3f(0.0f, 0.0f, 0.0f);
		double rad = Rad_max / (double)N_GRID_STEP * (i + 1);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < 5; j++) {
			glVertex3f(X_polygon + rad * cos(PI / 2.0 + 2 / 5.0 * PI * j),
				Y_polygon + 1 / fAspect * rad * sin(PI / 2.0 + 2 / 5.0 * PI * j), 0);
		}
		glVertex3f(X_polygon + rad * cos(PI / 2.0), Y_polygon + 1 / fAspect * rad * sin(PI / 2.0), 0);
		glEnd();
	}

	// Draw center - vertex lines
	glLineWidth(0.1);
	for (int i = 0; i < 5; i++) {
		glBegin(GL_LINE_STRIP);
		glVertex3f(X_polygon, Y_polygon, 0);
		glVertex3f(X_polygon + Rad_max * cos(PI / 2.0 + 2 / 5.0 * PI * i),
			Y_polygon + 1 / fAspect * Rad_max * sin(PI / 2.0 + 2 / 5.0 * PI * i), 0);
		glEnd();
	}

	X_polygon = 2.0f;
	Y_polygon = -2.0f;
	Rad_max = 0.8;
	for (int i = 0; i < N_GRID_STEP; i++) {
		if (i == N_GRID_STEP - 1)
			glLineWidth(2.0);
		else
			glLineWidth(0.3);

		glColor3f(0.0f, 0.0f, 0.0f);
		double rad = Rad_max / (double)N_GRID_STEP * (i + 1);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < 6; j++) {
			glVertex3f(X_polygon + rad * cos(PI / 2.0 + 2 / 6.0 * PI * j),
				Y_polygon + 1 / fAspect * rad * sin(PI / 2.0 + 2 / 6.0 * PI * j), 0);
		}
		glVertex3f(X_polygon + rad * cos(PI / 2.0), Y_polygon + 1 / fAspect * rad * sin(PI / 2.0), 0);
		glEnd();
	}

	// Draw center - vertex lines
	glLineWidth(0.1);
	for (int i = 0; i < 6; i++) {
		glBegin(GL_LINE_STRIP);
		glVertex3f(X_polygon, Y_polygon, 0);
		glVertex3f(X_polygon + Rad_max * cos(PI / 2.0 + 2 / 6.0 * PI * i),
			Y_polygon + 1 / fAspect * Rad_max * sin(PI / 2.0 + 2 / 6.0 * PI * i), 0);
		glEnd();
	}

	/////////////////////////////// Polygon data ///////////////////////////////
	X_polygon = -2.0f;
	Y_polygon = -2.0f;
	Rad_max = 0.8;
	int start_idx = 0;
	int end_idx = 4;

	glLineWidth(5.0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	for (int i = start_idx; i <= end_idx; i++) {
		double rad = Rad_max * sEMG_plot[i][Current_idx + m_Start_idx];
		glVertex3f(X_polygon + rad * cos(PI / 2.0 + 2 / (double)(end_idx - start_idx + 1) * PI * (i - start_idx)),
			Y_polygon + 1 / fAspect * rad * sin(PI / 2.0 + 2 / (double)(end_idx - start_idx + 1) * PI * (i - start_idx)), 0);
	}
	glVertex3f(X_polygon + Rad_max * sEMG_plot[start_idx][Current_idx + m_Start_idx] * cos(PI / 2.0),
			   Y_polygon + 1 / fAspect * Rad_max * sEMG_plot[start_idx][Current_idx + m_Start_idx] * sin(PI / 2.0), 0);
	glEnd();

	X_polygon = 0.0f;
	Y_polygon = -2.0f;
	Rad_max = 0.8;
	start_idx = 5;
	end_idx = 9;

	glLineWidth(5.0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	for (int i = start_idx; i <= end_idx; i++) {
		double rad = Rad_max * sEMG_plot[i][Current_idx + m_Start_idx];
		glVertex3f(X_polygon + rad * cos(PI / 2.0 + 2 / (double)(end_idx - start_idx + 1) * PI * (i - start_idx)),
			Y_polygon + 1 / fAspect * rad * sin(PI / 2.0 + 2 / (double)(end_idx - start_idx + 1) * PI * (i - start_idx)), 0);
	}
	glVertex3f(X_polygon + Rad_max * sEMG_plot[start_idx][Current_idx + m_Start_idx] * cos(PI / 2.0),
		Y_polygon + 1 / fAspect * Rad_max * sEMG_plot[start_idx][Current_idx + m_Start_idx] * sin(PI / 2.0), 0);
	glEnd();

	X_polygon = 2.0f;
	Y_polygon = -2.0f;
	Rad_max = 0.8;
	start_idx = 10;
	end_idx = 15;

	glLineWidth(5.0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	for (int i = start_idx; i <= end_idx; i++) {
		double rad = Rad_max * sEMG_plot[i][Current_idx + m_Start_idx];
		glVertex3f(X_polygon + rad * cos(PI / 2.0 + 2 / (double)(end_idx - start_idx + 1) * PI * (i - start_idx)),
			Y_polygon + 1 / fAspect * rad * sin(PI / 2.0 + 2 / (double)(end_idx - start_idx + 1) * PI * (i - start_idx)), 0);
	}
	glVertex3f(X_polygon + Rad_max * sEMG_plot[start_idx][Current_idx + m_Start_idx] * cos(PI / 2.0),
		Y_polygon + 1 / fAspect * Rad_max * sEMG_plot[start_idx][Current_idx + m_Start_idx] * sin(PI / 2.0), 0);
	glEnd();

	glPopMatrix();
	glFlush();
}

void ClippedGraph::Set_Current_idx(UINT _Current_idx) {
	Current_idx = _Current_idx;
}

void ClippedGraph::Set_TimeStep(UINT _TimeStep) {
	TimeStep = _TimeStep;
}

void ClippedGraph::Set_AnimiationTimer() {
	SetTimer(TIMER_ANIMATION, TIME_ELAPSE, NULL);
}

void ClippedGraph::Kill_AnimiationTimer() {
	KillTimer(TIMER_ANIMATION);
}

int ClippedGraph::Get_Current_idx() {
	return Current_idx;
}

void ClippedGraph::Delete_Dynamic_Alloc() {
	delete X_pos;
	for (int i = 0; i < Num_CH; i++)
		delete Y_val[i];
	delete Y_val;
}

void ClippedGraph::Set_Dynamic_Alloc(UINT _m_Num_idx) {
	m_Num_idx = _m_Num_idx;

	X_pos = new double[m_Num_idx];
	for (int i = 0; i < m_Num_idx; i++) {
		X_pos[i] = -2.5f + 5.0f / (double)(m_Num_idx)*i;
	}

	Y_val = new double* [Num_CH];
	for (int i = 0; i < Num_CH; i++) {
		Y_val[i] = new double[m_Num_idx];
		for (int j = 0; j < m_Num_idx; j++) {
			Y_val[i][j] = rand() / (double)(RAND_MAX);
		}
	}
}