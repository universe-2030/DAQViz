#pragma once

#include "GL/glut.h"
#include "GL/GL.h"
#include "GL/GLU.h"

// BallControl 대화 상자

#define TIME_ELAPSE 20

class BallControl : public CDialogEx
{
	DECLARE_DYNAMIC(BallControl)

public:
	BallControl(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~BallControl();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DAQVIZ_DIALOG_BALL_CONTROL };
#endif

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	HGLRC	m_hRC;
	CDC*	m_pDC;
	HDC		m_hDC;

	int count = 0;
	int count_horizontal = 0;
	int count_vertical = 0;
	double rot = 0.0;

	double fAspect;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	void GLResize(int cx, int cy);
	void GLRenderScene();

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
