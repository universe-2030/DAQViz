#pragma once

#include <iostream>
#include "GL/glut.h"
#include "GL/GL.h"
#include "GL/GLU.h"

#define TIME_ELAPSE 20
#define MOVE_SCALE 0.002

#define NUM_GRAPH_ANALYSIS 4
#define GRAPH_Y_LEN 0.6
#define GRAPH_Y_INTERVAL 0.15

#define NUM_CH 16

#define N_GRID_STEP 5

#define PI 3.14159265358

// ClippedGraph 대화 상자

class ClippedGraph : public CDialogEx {
	DECLARE_DYNAMIC(ClippedGraph)

public:
	ClippedGraph(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	ClippedGraph(int _m_Num_idx, CWnd* pParent = nullptr);   // 표준 생성자입니다.
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
	int m_Num_idx;

	HGLRC	m_hRC;
	CDC*	m_pDC;
	HDC		m_hDC;

	int count = 0;
	int count_horizontal = 0;
	int count_vertical = 0;
	double rot = 0.0;

	double fAspect;

	double* X_pos;
	double** Y_val;

	double X_polygon;
	double Y_polygon;
	double Rad_max;

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
	void Set_count_horizontal(UINT m_count_horizontal);
};
