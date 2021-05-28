// BallControl.cpp: 구현 파일
//

#include "pch.h"
#include "DAQViz.h"
#include "DAQVizDlg.h"
#include "BallControl.h"
#include "afxdialogex.h"

// BallControl 대화 상자

IMPLEMENT_DYNAMIC(BallControl, CDialogEx)

BallControl::BallControl(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAQVIZ_DIALOG_BALL_CONTROL, pParent) {
	species = MAIN_BALL;
}

BallControl::BallControl(int _m_Start_idx,
					int _m_End_idx, int _m_Num_idx,
					const std::vector<double>* _X_pos,
					const std::vector<double>* _Y_pos,
					const std::vector<double>* _Rad,
					Render_Ball _species, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAQVIZ_DIALOG_CHILD_OPENGL_2, pParent) {

	m_Start_idx = _m_Start_idx;
	m_End_idx = _m_End_idx;
	m_Num_idx = _m_Num_idx;

	X_pos = _X_pos;
	Y_pos = _Y_pos;
	Rad = _Rad;

	species = _species;
}

BallControl::~BallControl() {

}

void BallControl::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(BallControl, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// BallControl 메시지 처리기

BOOL BallControl::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	if (species == MAIN_BALL)
		SetTimer(TIMER_MAIN, TIME_ELAPSE, NULL);
	else if (species == RENDER_BALL) {
		KillTimer(TIMER_MAIN);
		SetTimer(TIMER_RENDER, TIME_ELAPSE, NULL);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void BallControl::OnPaint() {
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
		if (species == MAIN_BALL)
			GLRenderScene();
		else if (species == RENDER_BALL)
			GLRenderScene_Animation();
		SwapBuffers(m_hDC);
		wglMakeCurrent(m_hDC, NULL);
	}
}

void BallControl::Set_Current_idx(UINT _Current_idx) {
	Current_idx = _Current_idx;
}

void BallControl::Set_TimeStep(UINT _TimeStep) {
	TimeStep = _TimeStep;
}

void BallControl::Set_AnimationTimer() {
	SetTimer(TIMER_ANIMATION, TIME_ELAPSE, NULL);
}

void BallControl::Kill_AnimationTimer() {
	KillTimer(TIMER_ANIMATION);
}

void BallControl::Set_Redraw(UINT _m_Start_idx, UINT _m_End_idx, UINT _m_Num_idx) {
	m_Start_idx = _m_Start_idx;
	m_End_idx = _m_End_idx;
	m_Num_idx = _m_Num_idx;
}

void BallControl::OnTimer(UINT_PTR nIDEvent) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);

	switch (nIDEvent) {
	case TIMER_MAIN:
		/*if (species == MAIN_BALL) {
			GLRenderScene();
		}	*/
		break;
	case TIMER_RENDER:
		if (species == RENDER_BALL) {
			GLRenderScene_Animation();
		}
		break;
	case TIMER_ANIMATION:
		if (species == RENDER_BALL) {
			Current_idx += TimeStep;
			if (Current_idx > m_Num_idx)
				Current_idx = 0;
		}
		break;
	}
	this->Invalidate(FALSE);
}

int BallControl::OnCreate(LPCREATESTRUCT lpCreateStruct) {
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

void BallControl::OnSize(UINT nType, int cx, int cy) {
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	VERIFY(wglMakeCurrent(m_hDC, m_hRC));
	GLResize(cx, cy);
	VERIFY(wglMakeCurrent(NULL, NULL));

}

BOOL BallControl::OnEraseBkgnd(CDC* pDC) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// 	return CDialogEx::OnEraseBkgnd(pDC);
	return FALSE;
}

void BallControl::GLResize(int cx, int cy) {
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

void BallControl::GLRenderScene(void) {
	// TODO: 여기에 구현 코드 추가.
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	// clear the drawing buffer.

	gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	////////////////////////////// Sphere //////////////////////////////
	glLoadIdentity();
	// Blue color used to draw.
	glColor3f(0.0, 0.0, 1.0);

	// traslate the draw by z = -4.0
	// Note this when you decrease z like -8.0 the drawing will looks far , or smaller.
	CDAQVizDlg* pMainDlg = (CDAQVizDlg*)AfxGetMainWnd();
	if (pMainDlg != NULL) {
		if (pMainDlg->Get_TimerStarted()) {
			UINT X_idx_temp = pMainDlg->Get_X_pos_ball_stack()[0].size();
			UINT Y_idx_temp = pMainDlg->Get_Y_pos_ball_stack()[0].size();
			UINT Rad_idx_temp = pMainDlg->Get_Rad_ball_stack()[0].size();

			if (X_idx_temp > 0 && Y_idx_temp > 0 && Rad_idx_temp > 0) {
				glTranslatef(pMainDlg->Get_X_pos_ball_stack()[0][X_idx_temp - 1],
							pMainDlg->Get_Y_pos_ball_stack()[0][Y_idx_temp - 1],
							-5.0);

				glutSolidSphere(pMainDlg->Get_Rad_ball_stack()[0][Rad_idx_temp - 1],
								50,
								50);
				// Flush buffers to screen

				////////////////////////////// Axis //////////////////////////////
				GLfloat Axis_min = -100.0f;
				GLfloat Axis_max = 100.0f;

				glLineWidth(3);
				glBegin(GL_LINES);
				glColor3f(1.0f, 1.0f, 1.0f);
				glVertex3f(Axis_min, -pMainDlg->Get_Y_pos_ball_stack()[0][Y_idx_temp - 1], 0.0f);
				glVertex3f(Axis_max, -pMainDlg->Get_Y_pos_ball_stack()[0][Y_idx_temp - 1], 0.0f);
				glEnd();

				glLineWidth(3);
				glBegin(GL_LINES);
				glColor3f(1.0f, 1.0f, 1.0f);
				glVertex3f(-pMainDlg->Get_X_pos_ball_stack()[0][X_idx_temp - 1], Axis_min, 0.0f);
				glVertex3f(-pMainDlg->Get_X_pos_ball_stack()[0][X_idx_temp - 1], Axis_max, 0.0f);
				glEnd();
			}
		}
	}

	glPopMatrix();
	glFlush();
}

void BallControl::GLRenderScene_Animation(void) {
	// TODO: 여기에 구현 코드 추가.
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	// clear the drawing buffer.

	gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	////////////////////////////// Sphere //////////////////////////////
	glLoadIdentity();
	// Blue color used to draw.
	glColor3f(0.0, 0.0, 1.0);
	// traslate the draw by z = -4.0
	// Note this when you decrease z like -8.0 the drawing will looks far , or smaller.

	CDAQVizDlg* pMainDlg = (CDAQVizDlg*)AfxGetMainWnd();
	if (pMainDlg != NULL) {
		if (pMainDlg->Get_TimerStarted()) {
			glTranslatef(X_pos[0][Current_idx + m_Start_idx - 1],
						 Y_pos[0][Current_idx + m_Start_idx - 1],
						 -5.0);
			// built-in (glut library) function , draw you a sphere.
			glutSolidSphere(Rad[0][Current_idx + m_Start_idx - 1],
							50, 50);
			// Flush buffers to screen

			////////////////////////////// Axis //////////////////////////////
			GLfloat Axis_min = -100.0f;
			GLfloat Axis_max = 100.0f;

			glLineWidth(3);
			glBegin(GL_LINES);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(Axis_min,
					-Y_pos[0][Current_idx + m_Start_idx - 1],
					0.0f);
			glVertex3f(Axis_max,
					-Y_pos[0][Current_idx + m_Start_idx - 1],
					0.0f);
			glEnd();

			glLineWidth(3);
			glBegin(GL_LINES);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-X_pos[0][Current_idx + m_Start_idx - 1],
					Axis_min, 0.0f);
			glVertex3f(-X_pos[0][Current_idx + m_Start_idx - 1],
					Axis_max, 0.0f);
			glEnd();
		}
	}

	glPopMatrix();
	glFlush();
}

void BallControl::OnClose() {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	KillTimer(TIMER_RENDER);
	SetTimer(TIMER_MAIN, TIME_ELAPSE, NULL);

	CDialogEx::OnClose();
}
