// DAQVizChildOpenGL.cpp: 구현 파일
//

#include "pch.h"
#include "DAQViz.h"
#include "DAQVizChildOpenGL.h"
#include "afxdialogex.h"

// DAQVizChildOpenGL 대화 상자

IMPLEMENT_DYNAMIC(DAQVizChildOpenGL, CDialogEx)

DAQVizChildOpenGL::DAQVizChildOpenGL(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAQVIZ_DIALOG_CHILD_OPENGL, pParent) {
	N_sEMG_CH = N_SEMG_CH;
}

DAQVizChildOpenGL::DAQVizChildOpenGL(int N_sEMG, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAQVIZ_DIALOG_CHILD_OPENGL, pParent) {
	N_sEMG_CH = N_sEMG;
}

DAQVizChildOpenGL::~DAQVizChildOpenGL() {

}

void DAQVizChildOpenGL::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(DAQVizChildOpenGL, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// DAQVizChildOpenGL 메시지 처리기

BOOL DAQVizChildOpenGL::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	initialize_Variable();

	SetTimer(1000, TIME_ELAPSE, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void DAQVizChildOpenGL::OnDestroy() {
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	wglDeleteContext(m_hRC);
	::ReleaseDC(m_hWnd, m_hDC);
}


void DAQVizChildOpenGL::OnTimer(UINT_PTR nIDEvent) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
	count++;
	this->Invalidate(FALSE);
}


int DAQVizChildOpenGL::OnCreate(LPCREATESTRUCT lpCreateStruct) {
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


void DAQVizChildOpenGL::OnSize(UINT nType, int cx, int cy) {
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	VERIFY(wglMakeCurrent(m_hDC, m_hRC));
	GLResize(cx, cy);
	VERIFY(wglMakeCurrent(NULL, NULL));
}


BOOL DAQVizChildOpenGL::OnEraseBkgnd(CDC* pDC) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// return CDialogEx::OnEraseBkgnd(pDC);
	return FALSE;
}


void DAQVizChildOpenGL::OnPaint()
{
	if (IsIconic())
	{
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
		GLRenderScene();
		SwapBuffers(m_hDC);
		wglMakeCurrent(m_hDC, NULL);
	}
}

void DAQVizChildOpenGL::GLResize(int cx, int cy) {
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

void DAQVizChildOpenGL::GLRenderScene(void) {
	// TODO: 여기에 구현 코드 추가.
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glEnable(GL_DEPTH_TEST);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glShadeModel(GL_FLAT);
	glLoadIdentity();

	gluLookAt(0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Draw polygon frames
	for (int i = 0; i < N_GRID_STEP; i++) {
		if (i == N_GRID_STEP - 1)
			glLineWidth(2.0);
		else
			glLineWidth(1.0);

		glColor3f(0.0f, 0.0f, 0.0f);
		double rad = 1 / (double)N_GRID_STEP * (i + 1);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < N_sEMG_CH; j++) {
			glVertex3f(rad * cos(PI / 2.0 + 2 / (double)N_sEMG_CH * PI * j),
					   1 / fAspect * rad * sin(PI / 2.0 + 2 / (double)N_sEMG_CH * PI * j),
					   0);
		}
		glVertex3f(rad * cos(PI / 2.0), 1 / fAspect * rad * sin(PI / 2.0), 0);
		glEnd();
	}

	// Draw center - vertex lines
	glLineWidth(0.1);
	for (int i = 0; i < N_sEMG_CH; i++) {
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, 0, 0);
		glVertex3f(cos(PI / 2.0 + 2 / (double)N_sEMG_CH * PI * i),
				   1 / fAspect * sin(PI / 2.0 + 2 / (double)N_sEMG_CH * PI * i),
				   0);
		glEnd();
	}

	// Draw sEMG data (Not normalized)
	glLineWidth(5.0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	for (int j = 0; j < N_sEMG_CH; j++) {
		glVertex3f(sEMG_data[j] * cos(PI / 2.0 + 2 / (double)N_sEMG_CH * PI * j),
			1 / fAspect * sEMG_data[j] * sin(PI / 2.0 + 2 / (double)N_sEMG_CH * PI * j),
			0);

	}
	glVertex3f(sEMG_data[0] * cos(PI / 2.0), 1 / fAspect * sEMG_data[0] * sin(PI / 2.0), 0);
	glEnd();

	// Draw sEMG data (Normalized)
	glLineWidth(5.0);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	for (int j = 0; j < N_sEMG_CH; j++) {
		glVertex3f(sEMG_data_normalized[j] * cos(PI / 2.0 + 2 / (double)N_sEMG_CH * PI * j),
			1 / fAspect * sEMG_data_normalized[j] * sin(PI / 2.0 + 2 / (double)N_sEMG_CH * PI * j),
			0);

	}
	glVertex3f(sEMG_data_normalized[0] * cos(PI / 2.0), 1 / fAspect * sEMG_data_normalized[0] * sin(PI / 2.0), 0);
	glEnd();

	//glTranslatef(0.0f, 0.0f, -5.0f);
	//glRotatef(rot, 0.0f, 1.0f, 0.0f);

	//glBegin(GL_TRIANGLES);
	//glColor3f(1.0f, 0.0f, 0.0f);
	//glVertex3f(0.0f, 1.0f, 0.0f); // { Front }
	//glVertex3f(-1.0f, -1.0f, 1.0f); // { Front }
	//glVertex3f(1.0f, -1.0f, 1.0f); // { Front }

	//glColor3f(0.0f, 1.0f, 0.0f);
	//glVertex3f(0.0f, 1.0f, 0.0f); // { Right }
	//glVertex3f(1.0f, -1.0f, 1.0f); // { Right }
	//glVertex3f(1.0f, -1.0f, -1.0f); // { Right }

	//glColor3f(0.0f, 0.0f, 1.0f);
	//glVertex3f(0.0f, 1.0f, 0.0f); // { Back }
	//glVertex3f(1.0f, -1.0f, -1.0f); // { Back }
	//glVertex3f(-1.0f, -1.0f, -1.0f); // { Back }

	//glColor3f(1.0f, 0.0f, 1.0f);
	//glVertex3f(0.0f, 1.0f, 0.0f); // { Left }
	//glVertex3f(-1.0f, -1.0f, -1.0f); // { Left }
	//glVertex3f(-1.0f, -1.0f, 1.0f); // { Left }
	//glEnd();

	//rot += 1.1f;

	glPopMatrix();
	glFlush();
}

void DAQVizChildOpenGL::initialize_Variable() {
	sEMG_data = new double[N_sEMG_CH];
	memset(sEMG_data, 0.0, 2 * sizeof(sEMG_data) * N_sEMG_CH);

	sEMG_data_normalized = new double[N_sEMG_CH];
	memset(sEMG_data_normalized, 0.0, 2 * sizeof(sEMG_data_normalized) * N_sEMG_CH);
}

void DAQVizChildOpenGL::Set_sEMG_data(double* _sEMG_input) {
	for (int i = 0; i < N_sEMG_CH; i++)
		sEMG_data[i] = _sEMG_input[i];

	double sum_sEMG_data = 0.0;
	for (int i = 0; i < N_sEMG_CH; i++)
		sum_sEMG_data += abs(sEMG_data[i]);

	for (int i = 0; i < N_sEMG_CH; i++)
		sEMG_data_normalized[i] = sEMG_data[i] / sum_sEMG_data;
}