#pragma once

#include "OScopeCtrl.h"
#include "include/GL/glew.h"
#include "include/GL/wglew.h"

#include "DAQVizChildOpenGL.h"
#include "DAQVizChildOpenGL2.h"

// DAQVizChildKSJ 대화 상자

#define SEMG_RAW_MIN -1.2
#define SEMG_RAW_MAX 1.2

#define SEMG_MAV_MIN 0.0
#define SEMG_MAV_MAX 1.2

#define FLEX_SENSOR_MIN -3
#define FLEX_SENSOR_MAX 3

#define IMU_MIN -90
#define IMU_MAX 90

#define N_SEMG_CH 16
#define N_FLEX_CH 5
#define N_IMU_CH 2

enum Graph_class {
	SEMG_RAW,
	SEMG_MAV,
	FLEX_SENSOR,
	IMU
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
	COScopeCtrl** rtGraph_sEMG_raw;
	COScopeCtrl** rtGraph_sEMG_MAV;
	COScopeCtrl** rtGraph_Flex;
	COScopeCtrl** rtGraph_IMU;

	DAQVizChildOpenGL* p_ChildOpenGL;
	DAQVizChildOpenGL2* p_ChildOpenGL_2;

	int N_sEMG;
	int N_Flex;
	int N_IMU;

	// Data containers
	double* sEMG_raw;
	double* sEMG_MAV;

	double* Flex_data;
	double* IMU_data;

public:
	virtual BOOL OnInitDialog();

	void Initialize_Variable();
	void Initialize_GUI();

	COScopeCtrl* Initialize_graph(int ID, int idx_start, int idx_end,
						  COScopeCtrl* rtGraph, Graph_class _class);
	void Plot_graph(double* data, COScopeCtrl* rtGraph);

	COScopeCtrl** Get_rtGraph_sEMG_raw();
	COScopeCtrl** Get_rtGraph_sEMG_MAV();
	COScopeCtrl** Get_rtGraph_Flex();

	CStatic m_textsEMGPlot;
	CStatic m_textMotionRendering;
};