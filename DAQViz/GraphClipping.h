#pragma once
#include "ClippedGraph.h"

// GraphClipping 대화 상자
#define TIMER_GRAPH_CLIPPING 1
#define TIME_ELAPSE 20

class GraphClipping : public CDialogEx {
	DECLARE_DYNAMIC(GraphClipping)

public:
	GraphClipping(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	GraphClipping(UINT start_idx, UINT end_idx, UINT _m_count, CWnd* pParent = nullptr);
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
};