#pragma once

#include <vector>
#include "GL/glut.h"
#include "GL/GL.h"
#include "GL/GLU.h"

#define TIMER_MAIN 1
#define TIMER_RENDER 2
#define TIMER_ANIMATION 3
#define TIME_ELAPSE 20

enum Render_Ball {
	MAIN_BALL,
	RENDER_BALL,
};

// BallControl 대화 상자

class BallControl : public CDialogEx
{
	DECLARE_DYNAMIC(BallControl)

public:
	BallControl(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	BallControl(int _m_Start_idx, int _m_End_idx, int _m_Num_idx,
				Render_Ball _species, CWnd* pParent = nullptr);
	BallControl(int _m_Start_idx, int _m_End_idx, int _m_Num_idx,
				const std::vector<double>* _MotionLabel_plot,
				const std::vector<double>* _Estimation_plot,
				Render_Ball _species, CWnd* pParent = nullptr);
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
	int m_Start_idx;
	int m_End_idx;
	int m_Num_idx;
	Render_Ball species;

	// Vector
	const std::vector<double>* MotionLabel_plot;
	const std::vector<double>* Estimation_plot;

	// For animation bar
	int Current_idx = 0;
	int TimeStep = 1;

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
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	void GLResize(int cx, int cy);
	void GLRenderScene();
	void GLRenderScene_Animation();

	void Set_Current_idx(UINT _Current_idx);
	void Set_TimeStep(UINT _TimeStep);
	void Set_AnimationTimer();
	void Kill_AnimationTimer();

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
