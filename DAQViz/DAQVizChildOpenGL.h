#pragma once

#include "GL/glut.h"
#include "GL/GL.h"
#include "GL/GLU.h"

// DAQVizChildOpenGL 대화 상자

class DAQVizChildOpenGL : public CDialogEx
{
	DECLARE_DYNAMIC(DAQVizChildOpenGL)

public:
	DAQVizChildOpenGL(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~DAQVizChildOpenGL();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DAQVIZ_DIALOG_CHILD_OPENGL };
#endif

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CRect m_rectParentDlg;
	
	int count = 0;

	HGLRC	m_hRC;
	CDC*	m_pDC;
	HDC		m_hDC;

public:

	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();

	void GLResize(int cx, int cy);
	void GLRenderScene();
	afx_msg void OnPaint();
};
