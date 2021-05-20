// DAQVizChildOpenGL2.cpp: 구현 파일
//

#include "pch.h"
#include "DAQViz.h"
#include "DAQVizChildOpenGL2.h"
#include "afxdialogex.h"

using namespace std;
const double R2D = 180.0 / M_PI;
const double D2R = 1 / R2D;

int			screenWidth = 1600;
int			screenHeight = 900;
GLFrame		frameCamera;
float		delta = 0;
int			cursorPos[2];
//GLfloat		whiteLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
//GLfloat		sourceLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
//GLfloat		lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
bool		cameraControlMode = false;
GLFrame		hand[2];
Hand		rightHand(&hand[0]);
Hand		leftHand(&hand[1]);
int			jointIndex = 0;

// DAQVizChildOpenGL2 대화 상자

IMPLEMENT_DYNAMIC(DAQVizChildOpenGL2, CDialogEx)

DAQVizChildOpenGL2::DAQVizChildOpenGL2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAQVIZ_DIALOG_CHILD_OPENGL_2, pParent) {

}

DAQVizChildOpenGL2::~DAQVizChildOpenGL2() {

}

void DAQVizChildOpenGL2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(DAQVizChildOpenGL2, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// DAQVizChildOpenGL2 메시지 처리기

BOOL DAQVizChildOpenGL2::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	int argc = 1;
	char* argv[] = { "Hello", "Hello" };
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL 3D Model Demo");

	SetupRC();
	SetTimer(1000, 50, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void DAQVizChildOpenGL2::OnPaint() {
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
		RenderScene();
		SwapBuffers(m_hDC);
		wglMakeCurrent(m_hDC, NULL);
	}
}

void DAQVizChildOpenGL2::OnDestroy() {
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	wglDeleteContext(m_hRC);
	::ReleaseDC(m_hWnd, m_hDC);
}


void DAQVizChildOpenGL2::OnTimer(UINT_PTR nIDEvent) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);

	if (++jointIndex > 14)
		jointIndex = 0;
	rightHand.setJointIndex(jointIndex);
	this->Invalidate(FALSE);
}


int DAQVizChildOpenGL2::OnCreate(LPCREATESTRUCT lpCreateStruct) {
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

void DAQVizChildOpenGL2::OnSize(UINT nType, int cx, int cy) {
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	VERIFY(wglMakeCurrent(m_hDC, m_hRC));
	ChangeSize(cx, cy);
	VERIFY(wglMakeCurrent(NULL, NULL));
}

BOOL DAQVizChildOpenGL2::OnEraseBkgnd(CDC* pDC) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// return CDialogEx::OnEraseBkgnd(pDC);
	return FALSE;
}

void DAQVizChildOpenGL2::SetupRC()
{
	int iSphere;

	// Bluish background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	// Draw everything as wire frame
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//MODEL = glmReadOBJ("hand.obj");


	//glEnable(GL_LIGHTING);
	// Setup and enable light 0
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	//glEnable(GL_LIGHT0);

	// Enable color tracking
	//glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Randomly place the sphere inhabitants

	//for (iSphere = 0; iSphere < NUM_SPHERES; iSphere++)
	//{
	//	// Pick a random location between -20 and 20 at .1 increments
	//	float x = ((float)((rand() % 400) - 200) * 0.1f);
	//	float z = (float)((rand() % 400) - 200) * 0.1f;
	//	spheres[iSphere].SetOrigin(x, 0.0f, z);
	//}

	M3DVector3f up = { -0.614193, 0.690447, 0.382165 };
	M3DVector3f origin = { 5.12, 7.09, -5.57 };
	// M3DVector3f look = { -0.51938, -0.71827, 0.4629628 }; // Original look
	M3DVector3f look = { -1.5, -0.81827, 0.4629628 };
	frameCamera.SetForwardVector(look);
	frameCamera.SetOrigin(origin);
	frameCamera.SetUpVector(up);
	frameCamera.Normalize();
}

///////////////////////////////////////////////////////////
// Draw a gridded ground
void DAQVizChildOpenGL2::DrawGround(void)
{
	const GLfloat fExtent = 100.0f;
	const GLfloat fStep = 0.5f;
	GLfloat y = -0.4f;
	GLfloat iLine;

	glLineWidth(1);
	glColor3ub(0, 150, 150);
	glBegin(GL_LINES);
	for (iLine = -fExtent; iLine <= fExtent; iLine += fStep)
	{
		glVertex3f(iLine, y, fExtent);    // Draw Z lines
		glVertex3f(iLine, y, -fExtent);

		glVertex3f(fExtent, y, iLine); // Draw X lines
		glVertex3f(-fExtent, y, iLine);
	}
	glEnd();

	//gltDrawUnitAxes();
}

// Called to draw scene
void DAQVizChildOpenGL2::RenderScene(void)
{
	//frameCamera.Normalize();
	//frametorus.Normalize();
	//cout << "Camera Position: (" << frameCamera.GetOriginX() << ", " << frameCamera.GetOriginY() << ", " << frameCamera.GetOriginZ() << ")" << endl;
	//M3DVector3f tmp;
	//frameCamera.GetForwardVector(tmp);
	//cout << "Camera Lookat: (" << tmp[0] << ", " << tmp[1] << ", " << tmp[2] << ")" << endl;

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	frameCamera.ApplyCameraTransform();

	// Draw the ground
	DrawGround();
	glTranslatef(0, 1, 0);
	//??짫╁ㅦ1?ⓒΞglBegin() ... glEnd()⒡첩퍀ず큍?か톱
#pragma region PolygonObject
	glPushMatrix();
	const float vertices[8][3] = { { -1, -1, -1 },
									{ 1, -1, -1 },
									{ 1, 1, -1 },
									{ -1, 1, -1 },
									{ -1, -1, 1 },
									{ 1, -1, 1 },
									{ 1, 1, 1 },
									{ -1, 1, 1 } };
	glTranslatef(1, 0, 1);
	//glScalef(0.1, 0.1, 0.1);
	glColor3ub(255, 0, 255);

#pragma region PolygonObject_Solid

	// glEnable(GL_POLYGON_OFFSET_FILL);
	// glPolygonOffset(.5, .5);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Back
	//glColor3ub(0, 0, 0);
	//glBegin(GL_POLYGON);
	//glNormal3f(0, 0, -1);
	//glVertex3fv(vertices[0]);
	//glVertex3fv(vertices[1]);
	//glVertex3fv(vertices[2]);
	//glVertex3fv(vertices[3]);
	//glEnd();

	////Front
	//glColor3ub(255, 0, 255);
	//glBegin(GL_POLYGON);
	//glNormal3f(0, 0, 1);
	//glVertex3fv(vertices[4]);
	//glVertex3fv(vertices[5]);
	//glVertex3fv(vertices[6]);
	//glVertex3fv(vertices[7]);
	//glEnd();

	////Top
	//glColor3ub(0, 0, 255);
	//glBegin(GL_POLYGON);
	//glNormal3f(0, 1, 0);
	//glVertex3fv(vertices[0]);
	//glVertex3fv(vertices[1]);
	//glVertex3fv(vertices[2]);
	//glVertex3fv(vertices[3]);
	//glEnd();

	////Bottom
	//glColor3ub(255, 255, 255);
	//glBegin(GL_POLYGON);
	//glNormal3f(0, -1, 0);
	//glVertex3fv(vertices[1]);
	//glVertex3fv(vertices[0]);
	//glVertex3fv(vertices[4]);
	//glVertex3fv(vertices[5]);
	//glEnd();

	////Left
	//glColor3ub(0, 255, 0);
	//glBegin(GL_POLYGON);
	//glVertex3fv(vertices[0]);
	//glVertex3fv(vertices[3]);
	//glVertex3fv(vertices[7]);
	//glVertex3fv(vertices[4]);
	//glEnd();

	////Right
	//glColor3ub(255, 0, 0);
	//glBegin(GL_POLYGON);
	//glVertex3fv(vertices[1]);
	//glVertex3fv(vertices[2]);
	//glVertex3fv(vertices[6]);
	//glVertex3fv(vertices[5]);
	//glEnd();
	//glDisable(GL_POLYGON_OFFSET_FILL);
#pragma endregion

#pragma region PolygonObject_Wire
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glColor3ub(255, 255, 255);
	//glBegin(GL_POLYGON);
	//glVertex3fv(vertices[0]);
	//glVertex3fv(vertices[1]);
	//glVertex3fv(vertices[2]);
	//glVertex3fv(vertices[3]);
	//glEnd();

	////Front
	//glBegin(GL_POLYGON);
	//glVertex3fv(vertices[4]);
	//glVertex3fv(vertices[5]);
	//glVertex3fv(vertices[6]);
	//glVertex3fv(vertices[7]);
	//glEnd();

	////Top
	//glBegin(GL_POLYGON);
	//glVertex3fv(vertices[0]);
	//glVertex3fv(vertices[1]);
	//glVertex3fv(vertices[2]);
	//glVertex3fv(vertices[3]);
	//glEnd();

	////Bottom
	//glBegin(GL_POLYGON);
	//glVertex3fv(vertices[1]);
	//glVertex3fv(vertices[0]);
	//glVertex3fv(vertices[4]);
	//glVertex3fv(vertices[5]);
	//glEnd();

	////Left
	//glBegin(GL_POLYGON);
	//glVertex3fv(vertices[0]);
	//glVertex3fv(vertices[3]);
	//glVertex3fv(vertices[7]);
	//glVertex3fv(vertices[4]);
	//glEnd();

	////Right
	//glBegin(GL_POLYGON);
	//glVertex3fv(vertices[1]);
	//glVertex3fv(vertices[2]);
	//glVertex3fv(vertices[6]);
	//glVertex3fv(vertices[5]);
	//glEnd();
#pragma endregion

	glPopMatrix();
#pragma endregion


	//쬞Ξ셕?glutWire*()큍?か톱좥첩퍀쩧╁ㅦⓦ┳짽?츙?ずか톱
#pragma region JointObject
	glPushMatrix();
	//glRotatef(90, 0, 0, 1);
	rightHand.Render();
	glPopMatrix();
#pragma endregion

	glPopMatrix();

	// Do the buffer Swap
	glutSwapBuffers();
}

void DAQVizChildOpenGL2::objectAnimate(int i)
{
	if (i < 50)
	{
		for (int j = 0; j < 14; j++)
		{
			rightHand.setJointIndex(j);
			rightHand.fingerRotateDown();
		}
	}
	else if (i >= 50 && i < 60)
	{
		hand[0].MoveForward(-0.3);
	}
	else if (i >= 60 && i < 70)
	{
		hand[0].MoveForward(0.3);
	}
	else
	{
		for (int j = 0; j < 14; j++)
		{
			rightHand.setJointIndex(j);
			rightHand.fingerRotateUp();
		}
	}
}
//void bf(int i)
//{
//	/*
//	Your Implementation
//	*/
//
//	const double rad = 3.14159265359 / 180;
//	const float dist = sqrtf(pow(frameCamera.GetOriginX() - frametorus.GetOriginX(), 2) + pow(frameCamera.GetOriginZ() - frametorus.GetOriginZ(), 2));
//
//	frameCamera.RotateLocalY(1 * D2R);
//
//	frameCamera.MoveRight(-dist*sin(rad));
//
//	if ((i >= 0 && i < 90) || (i >= 270 && i < 360))
//		frameCamera.MoveForward(dist - dist*cos(rad));
//	else
//		frameCamera.MoveForward(-(dist - dist*cos(rad)));
//
//	if (i < 360)
//		glutTimerFunc(33, bf, ++i);
//
//	glutPostRedisplay();
//}

void DAQVizChildOpenGL2::myKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		/* Your Implementation */
		//Ex:  frametorus.????
		//hand[0].MoveForward(.1f);
		if (++jointIndex > 14)
			jointIndex = 0;
		rightHand.setJointIndex(jointIndex);

		break;
	case 's':
		/* Your Implementation */
		//hand[0].MoveForward(-.1f);
		if (--jointIndex < 0)
			jointIndex = 14;
		rightHand.setJointIndex(jointIndex);

		break;
	case 'a':
		/* Your Implementation */
		//hand[0].MoveRight(.1f);
		rightHand.fingerRotateUp();
		break;
	case 'd':
		/* Your Implementation */
		//hand[0].MoveRight(-.1f);
		rightHand.fingerRotateDown();
		break;

	case 'b':
		objectAnimate(0);
		break;
	case 'p':
		cout << "Camera Position: (" << frameCamera.GetOriginX() << ", " << frameCamera.GetOriginY() << ", " << frameCamera.GetOriginZ() << ")" << endl;
		M3DVector3f tmp;
		frameCamera.GetForwardVector(tmp);
		cout << "Camera Lookat: (" << tmp[0] << ", " << tmp[1] << ", " << tmp[2] << ")" << endl;
		frameCamera.GetUpVector(tmp);
		cout << "Camera Up: (" << tmp[0] << ", " << tmp[1] << ", " << tmp[2] << ")" << endl;
		break;

	case 'r':
		M3DVector3f up = { -0.614193, 0.690447, 0.382165 };
		M3DVector3f origin = { 5.12, 7.09, -5.57 };
		M3DVector3f look = { -0.51938, -0.71827, 0.4629628 };
		frameCamera.SetForwardVector(look);
		frameCamera.SetOrigin(origin);
		frameCamera.SetUpVector(up);
		frameCamera.Normalize();
		break;

	}
	glutPostRedisplay();
}

// Respond to arrow keys by moving the camera frame of reference
void DAQVizChildOpenGL2::SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		frameCamera.MoveForward(0.05f);
		hand[0].MoveForward(-0.05f);
	}
	if (key == GLUT_KEY_DOWN)
	{
		frameCamera.MoveForward(-0.05f);
		hand[0].MoveForward(0.05f);
	}
	if (key == GLUT_KEY_LEFT)
	{
		frameCamera.MoveRight(0.05f);
		hand[0].MoveRight(-0.05f);
		//frameCamera.RotateLocalY(0.1f);
	}
	if (key == GLUT_KEY_RIGHT)
	{
		frameCamera.MoveRight(-0.05f);
		hand[0].MoveRight(0.05f);
		//frameCamera.RotateLocalY(-0.1f);
	}
	// Refresh the Window
	glutPostRedisplay();
}

void DAQVizChildOpenGL2::mouseClicked(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		cameraControlMode = true;
	}
	else
	{
		cameraControlMode = false;
	}
}

int rotateCount = 0;
void DAQVizChildOpenGL2::mouseMotion(int x, int y)
{
	y = screenHeight - y;
	if (cameraControlMode)
	{
		if (x - cursorPos[0] > 2)
			frameCamera.MoveRight(0.1f);
		else if (x - cursorPos[0] < -2)
			frameCamera.MoveRight(-0.1f);



		//cout << "Camera Up: (" << tmp[0] << ", " << tmp[1] << ", " << tmp[2] << ")" << endl;
		if (y - cursorPos[1] > 2 && rotateCount > -30)
		{

			rotateCount--;
			frameCamera.RotateLocalX(-3 * D2R);
			const float dist = sqrtf(pow(frameCamera.GetOriginX() - hand[0].GetOriginX(), 2) +
				pow(frameCamera.GetOriginY() - hand[0].GetOriginY(), 2) +
				pow(frameCamera.GetOriginZ() - hand[0].GetOriginZ(), 2));
			frameCamera.MoveUp(-dist * sin(3 * D2R));
			frameCamera.MoveForward(-(dist - dist * cosf(3 * D2R)));

		}
		else if (y - cursorPos[1] < -2 && rotateCount < 30)
		{

			rotateCount++;
			frameCamera.RotateLocalX(3 * D2R);
			const float dist = sqrtf(pow(frameCamera.GetOriginX() - hand[0].GetOriginX(), 2) +
				pow(frameCamera.GetOriginY() - hand[0].GetOriginY(), 2) +
				pow(frameCamera.GetOriginZ() - hand[0].GetOriginZ(), 2));
			frameCamera.MoveUp(dist * sin(3 * D2R));
			frameCamera.MoveForward(dist - dist * cosf(3 * D2R));
		}

	}
	M3DVector3f camera;
	M3DVector3f object;
	frameCamera.GetOrigin(camera);
	hand[0].GetOrigin(object);
	M3DVector3f look = { object[0] - camera[0], object[1] - camera[1], object[2] - camera[2] };
	frameCamera.SetForwardVector(look);
	frameCamera.Normalize();

	cursorPos[0] = x;
	cursorPos[1] = y;

	//cout << "Cursor: " << cursorPos[0] << "," << cursorPos[1] << endl;
}

void DAQVizChildOpenGL2::mouseScroll(int button, int dir, int x, int y)
{
	if (dir > 0)
		frameCamera.MoveForward(0.2f);
	else
		frameCamera.MoveForward(-0.2f);
}

///////////////////////////////////////////////////////////
// Called by GLUT library when idle (window not being
// resized or moved)


void DAQVizChildOpenGL2::ChangeSize(int w, int h)
{
	GLfloat fAspect;

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	screenWidth = w;
	screenHeight = h;

	glViewport(0, 0, screenWidth, screenHeight);

	fAspect = (GLfloat)screenWidth / (GLfloat)screenHeight;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the clipping volume
	gluPerspective(70.0f, fAspect, 0.1f, 50.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}