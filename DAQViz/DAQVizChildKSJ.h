#pragma once

#include "OScopeCtrl.h"
#include "GL/glew.h"
#include "GL/wglew.h"

#include "DAQVizChildOpenGL.h"
#include "DAQVizChildOpenGL2.h"
#include "BallControl.h"
#include "GraphClipping.h"

// DAQVizChildKSJ 대화 상자

#define SEMG_RAW_MIN -0.001
#define SEMG_RAW_MAX 0.001

#define SEMG_MAV_MIN 0.0
#define SEMG_MAV_MAX 0.2

#define FINGER_MIN -1.5
#define FINGER_MAX 0.5

#define FINGER_SLOPE_MIN -10.0
#define FINGER_SLOPE_MAX 10.0

#define WRIST_MIN -0.3
#define WRIST_MAX 0.3

#define WRIST_SLOPE_MIN -5.0
#define WRIST_SLOPE_MAX 5.0

#define LABEL_EST_MIN 0
#define LABEL_EST_MAX 7

enum Graph_class {
	SEMG_MAV,
	FINGER,
	FINGER_SLOPE,
	WRIST,
	WRIST_SLOPE,
	LABEL_EST
};

class DAQVizChildKSJ : public CDialogEx {
	DECLARE_DYNAMIC(DAQVizChildKSJ)

public:
	DAQVizChildKSJ(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	DAQVizChildKSJ(int _N_sEMG_CH, int _N_Flex_CH, int _N_IMU_CH,
					CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~DAQVizChildKSJ();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DAQVIZ_DIALOG_CHILD_KSJ };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CRect* p_RectCtrl; // Position of MFC picture control on client window
	CRect* p_RectPlot; // Position of COscopeCtrl plot region on picture control
	CRect* p_RectPlot_fin; // Position of COscopeCtrl plot region on client window
	UINT* m_NumClicked;

	COScopeCtrl** rtGraph_sEMG_MAV;
	COScopeCtrl** rtGraph_Finger;
	COScopeCtrl** rtGraph_Finger_slope;
	COScopeCtrl** rtGraph_Wrist;
	COScopeCtrl** rtGraph_Wrist_slope;
	COScopeCtrl** rtGraph_Label_Est;

	DAQVizChildOpenGL* p_ChildOpenGL;
	DAQVizChildOpenGL2* p_ChildOpenGL_2;
	BallControl* p_BallControl;
	GraphClipping* Clip_window;

	int N_sEMG;
	int N_Flex;
	int N_IMU;

	// Data containers
	double* sEMG_MAV;
	double* Flex_raw;
	double* IMU_raw;

	// Clipping window
	int Pt_forth;
	int Pt_back;

public:
	CStatic m_textsEMGPlot;
	CStatic m_textMotionRendering;

	virtual BOOL OnInitDialog();

	void Initialize_Variable();
	void Initialize_GUI();

	COScopeCtrl* Initialize_graph(int ID, int idx_start, int idx_end,
						  COScopeCtrl* rtGraph, Graph_class _class);
	void Plot_graph(double* data, COScopeCtrl* rtGraph);

	COScopeCtrl** Get_rtGraph_sEMG_MAV();
	COScopeCtrl** Get_rtGraph_Finger();
	COScopeCtrl** Get_rtGraph_Finger_slope();
	COScopeCtrl** Get_rtGraph_Wrist();
	COScopeCtrl** Get_rtGraph_Wrist_slope();
	COScopeCtrl** Get_rtGraph_Label_Est();
	DAQVizChildOpenGL* Get_OpenGLPointer();
	
	void Cursor_set(UINT graph_idx);
	afx_msg void OnClickedGraphSemgMav1();
	afx_msg void OnClickedGraphSemgMav2();
	afx_msg void OnClickedGraphSemgMav3();
	afx_msg void OnClickedGraphFingerFlex();
	afx_msg void OnClickedGraphFingerFlexSlope();
	afx_msg void OnClickedGraphWristFlex();
	CStatic m_textBallControl;
	afx_msg void OnClickedGraphWristFlexSlope();
	afx_msg void OnClickedGraphMotionLabelEst();
};