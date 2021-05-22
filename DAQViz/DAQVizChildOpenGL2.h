#pragma once

#include <iostream>
#include <gltools.h>
#include <glframe.h>
#include "GL/glut.h"
#include "GL/GL.h"
#include "GL/GLU.h"
#include "GL/glew.h"
#include "glm.h"
#include "Hand.h"

#define PI 3.14159265358

// DAQVizChildOpenGL2 대화 상자

class DAQVizChildOpenGL2 : public CDialogEx
{
	DECLARE_DYNAMIC(DAQVizChildOpenGL2)

public:
	DAQVizChildOpenGL2(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~DAQVizChildOpenGL2();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DAQVIZ_DIALOG_CHILD_OPENGL_2 };
#endif

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	HGLRC	m_hRC;
	CDC*	m_pDC;
	HDC		m_hDC;

	float root_plot;
	float first_plot;
	float second_plot;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	void SetupRC();
	void DrawGround();
	void RenderScene();
	void objectAnimate(int i);

	void myKeys(unsigned char key, int x, int y);
	void SpecialKeys(int key, int x, int y);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMotion(int x, int y);
	void mouseScroll(int button, int dir, int x, int y);
	void ChangeSize(int w, int h);
};