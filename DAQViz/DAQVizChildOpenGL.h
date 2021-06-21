#pragma once

#include <algorithm>

#include "GL/glut.h"
#include "GL/GL.h"
#include "GL/GLU.h"

#define N_GRID_STEP 3
#define N_SEMG_CH 16
#define PI 3.14159265358

#define TIME_ELAPSE 20

// DAQVizChildOpenGL 대화 상자

class DAQVizChildOpenGL : public CDialogEx {
	DECLARE_DYNAMIC(DAQVizChildOpenGL)

public:
	DAQVizChildOpenGL(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	DAQVizChildOpenGL(int N_sEMG, int N_motions, CWnd* pParent = nullptr);   // 표준 생성자입니다.
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
	GLfloat fAspect;
	
	int count = 0;

	int N_sEMG_CH;
	int N_motions;
	double* sEMG_data;
	double** sEMG_data_mean;

	HGLRC	m_hRC;
	CDC*	m_pDC;
	HDC		m_hDC;

	double sEMG_polygon_scale;

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

	void renderBitmapCharacter(float x, float y, float z, void* font, char* string);

	// Only draw the polygon from current sEMG
	void Plot_polygon(const double* data, int _m_StartIdx, int _m_EndIdx,
					double _X_center, double _Y_center, double _Rad,
					bool _Normalization, bool _No_Normalization, bool _VertexSphere);

	// Draw 1) polygon from current sEMG and 2) normalized mean sEMG
	void Plot_polygon(const double* data, const double* data_mean, int _m_StartIdx, int _m_EndIdx,
					double _X_center, double _Y_center, double _Rad,
					bool _Normalization, bool _No_Normalization, bool _VertexSphere);

	void initialize_Variable();

	void Set_sEMG_polygon_scale(double _Polygon_scale);
	void Set_N_sEMG_CH(int _N_sEMG_CH);
	void Set_sEMG_data(double* _sEMG_input);
	void Set_sEMG_data_mean(double** _sEMG_mean_input);
};
