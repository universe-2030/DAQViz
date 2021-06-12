#pragma once

#include <iostream>
#include <vector>
#include <windows.h>
#include <gltools.h>
#include <glframe.h>
#include "GL/glut.h"
#include "GL/GL.h"
#include "GL/GLU.h"
#include "GL/glew.h"
#include "glm.h"
#include "Hand.h"

#define TIMER_MAIN 1
#define TIMER_RENDER 2
#define TIMER_ANIMATION 3
#define TIME_ELAPSE 20

#define PI 3.14159265358

enum Render_Hand {
	MAIN_HAND,
	RENDER_HAND,
};

// DAQVizChildOpenGL2 대화 상자

class DAQVizChildOpenGL2 : public CDialogEx
{
	DECLARE_DYNAMIC(DAQVizChildOpenGL2)

public:
	DAQVizChildOpenGL2(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	DAQVizChildOpenGL2(int _m_Start_idx,
					int _m_End_idx, int _m_Num_idx,
					const std::vector<double>* _Finger_plot,
					const std::vector<double>* _Wrist_plot,
					Render_Hand _species, bool _b_glutInit,
					CWnd* pParent = nullptr);   // 표준 생성자입니다.
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
	int m_Start_idx;
	int m_End_idx;
	int m_Num_idx;
	Render_Hand species;

	// Vector
	const std::vector<double>* Finger_plot;
	const std::vector<double>* Wrist_plot;

	// For animation bar
	int Current_idx = 0;
	int TimeStep = 1;

	// glut initialization boolean
	bool b_glutInit = FALSE;

	HGLRC	m_hRC;
	CDC*	m_pDC;
	HDC		m_hDC;

	float root_plot;
	float first_plot;
	float second_plot;

	float wrist_FE_plot;
	float wrist_RU_plot;

	float elbow_FE_plot;
	float elbow_IE_plot;
	float elbow_AA_plot;
	
	float shoulder_FE_plot;
	float shoulder_IE_plot;
	float shoulder_AA_plot;

	float root_animation;
	float first_animation;
	float second_animation;
	float wrist_FE_animation;
	float wrist_RU_animation;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	void SetupRC();
	void DrawGround();

	void Convert_jointangle();
	void Convert_jointangle(int _Current_idx);
	void RenderScene();
	void RenderScene_Animation();
	void objectAnimate(int i);

	void Set_Current_idx(UINT _Current_idx);
	void Set_TimeStep(UINT _TimeStep);
	void Set_AnimationTimer();
	void Kill_AnimationTimer();

	void Set_Redraw(UINT _m_Start_idx, UINT _m_End_idx, UINT _m_Num_idx);

	void myKeys(unsigned char key, int x, int y);
	void SpecialKeys(int key, int x, int y);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMotion(int x, int y);
	void mouseScroll(int button, int dir, int x, int y);
	void ChangeSize(int w, int h);
};