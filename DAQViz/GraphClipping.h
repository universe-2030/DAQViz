#pragma once

#include <vector>
#include "ClippedGraph.h"
#include "DAQVizChildOpenGL2.h"
#include "BallControl.h"

// GraphClipping 대화 상자
#define TIMER_GRAPH_CLIPPING 1
#define TIME_ELAPSE 20

class GraphClipping : public CDialogEx {
	DECLARE_DYNAMIC(GraphClipping)

public:
	GraphClipping(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	GraphClipping(UINT _Num_sEMG_CH, UINT start_idx, UINT end_idx, UINT _m_count,
					const std::vector<double>* _sEMG_plot,
					const std::vector<double>* _Finger_plot,
					const std::vector<double>* _Finger_slope_plot,
					const std::vector<double>* _Wrist_plot,
					const std::vector<double>* _Wrist_slope_plot,
					const std::vector<double>* _MotionLabel_plot,
					const std::vector<double>* _MotionEstimation_plot,
					const std::vector<double>* _X_ball_plot,
					const std::vector<double>* _Y_ball_plot,
					const std::vector<double>* _Rad_ball_plot,
					CWnd* pParent = nullptr);
	virtual ~GraphClipping();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DAQVIZ_DIALOG_GRAPH_CLIPPING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	ClippedGraph* p_ClippedGraph;
	ClippedGraph* p_ClippedGraph_2;
	DAQVizChildOpenGL2* p_HandMotion;
	BallControl* p_BallControl;

	// Clipped sEMG data
	const std::vector<double>* sEMG_plot;

	const std::vector<double>* Finger_plot;
	const std::vector<double>* Finger_slope_plot;
	const std::vector<double>* Wrist_plot;
	const std::vector<double>* Wrist_slope_plot;

	const std::vector<double>* MotionLabel_plot;
	const std::vector<double>* MotionEstimation_plot;

	const std::vector<double>* X_ball_plot;
	const std::vector<double>* Y_ball_plot;
	const std::vector<double>* Rad_ball_plot;

	UINT Num_sEMG_CH;
	UINT m_StartIdx;
	UINT m_EndIdx;
	UINT m_NumIdx;
	UINT m_count; // To check whether m_EndIdx exceeds m_time

	int ScrollPos;

	bool isAnimationRun = FALSE;

	int m_TimeStep = 1;

public:
	virtual BOOL OnInitDialog();

	CStatic m_textStartIdx;
	CStatic m_textEndIdx;
	CEdit m_editStartIdx;
	CEdit m_editEndIdx;
	CButton m_btnRedraw;
	CEdit m_editAnimationIdx;
	CScrollBar m_scrollBar;
	CStatic m_textTimeStep;
	CEdit m_editTimeStep;
	CButton m_btnRun;

	void Initialize_GUI();
	void Initialize_Variable();

	void Set_ScrollPos(UINT _ScrollPos);

	afx_msg void OnBnClickedBtnRedraw();
	afx_msg void OnBnClickedBtnAnimationRun();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditTimestep();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
};