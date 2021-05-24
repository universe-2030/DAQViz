﻿#pragma once

#include <iostream>
#include <vector>
#include "GL/glut.h"
#include "GL/GL.h"
#include "GL/GLU.h"

#define TIMER_RENDER 2
#define TIMER_ANIMATION 3
#define TIME_ELAPSE 20

#define MOVE_SCALE 0.002

#define NUM_GRAPH_ANALYSIS 8

#define GRAPH_Y_LEN_TOTAL 0.6
#define GRAPH_Y_INTERVAL_TOTAL 0.1
#define GRAPH_Y_LEN_ANI 0.42
#define GRAPH_Y_INTERVAL_ANI 0.1

#define SEMG_VAL_MAX 1.0
#define FLEX_VAL_MAX 1.5
#define IMU_VAL_MAX 90.0
#define MOTION_IDX_MAX 5.0

#define NUM_CH 16

#define N_GRID_STEP 5

#define PI 3.14159265358

enum Render {
	TOTAL,
	ANIMATION
};

// ClippedGraph 대화 상자

class ClippedGraph : public CDialogEx {
	DECLARE_DYNAMIC(ClippedGraph)

public:
	ClippedGraph(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	ClippedGraph(int _m_Start_idx, int _m_End_idx,
				int _m_Num_idx, int _Num_CH,
				const std::vector<double>* _sEMG_plot,
				const std::vector<double>* _Finger_plot,
				const std::vector<double>* _Finger_slope_plot,
				const std::vector<double>* _Wrist_plot,
				const std::vector<double>* _Wrist_slope_plot,
				const std::vector<double>* _MotionLabel_plot,
				const std::vector<double>* _MotionEstimation_plot,
				Render _species, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ClippedGraph();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DAQVIZ_DIALOG_CLIPPED_GRAPH };
#endif

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	int m_Start_idx;
	int m_End_idx;
	int m_Num_idx;
	int Num_CH;
	Render species;

	HGLRC	m_hRC;
	CDC*	m_pDC;
	HDC		m_hDC;

	double fAspect;

	double* X_pos;

	double X_polygon;
	double Y_polygon;
	double Rad_max;

	// Vector
	const std::vector<double>* sEMG_plot;

	const std::vector<double>* Finger_plot;
	const std::vector<double>* Finger_slope_plot;
	const std::vector<double>* Wrist_plot;
	const std::vector<double>* Wrist_slope_plot;

	const std::vector<double>* MotionLabel_plot;
	const std::vector<double>* MotionEstimation_plot;

	// For animation bar
	int Current_idx = 0;
	int TimeStep = 1;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	void GLResize(int cx, int cy);
	void GLRenderScene_Total();
	void GLRenderScene_Animation();
	void Plot_polygon(const double* data, int _m_StartIdx, int _m_EndIdx,
					double _X_center, double _Y_center, double _Rad);

	void Set_Current_idx(UINT _Current_idx);
	void Set_TimeStep(UINT _TimeStep);
	void Set_AnimationTimer();
	void Kill_AnimationTimer();
	int Get_Current_idx();

	void Delete_Dynamic_Alloc();
	void Set_Dynamic_Alloc(UINT _m_Start_idx, UINT _m_End_idx, UINT _m_Num_idx);
};
