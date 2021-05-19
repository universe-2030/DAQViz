#pragma once
#include "ClippedGraph.h"

// GraphClipping 대화 상자

class GraphClipping : public CDialogEx {
	DECLARE_DYNAMIC(GraphClipping)

public:
	GraphClipping(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	GraphClipping(UINT start_idx, UINT end_idx, double _m_time, CWnd* pParent = nullptr);
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

	UINT m_StartIdx;
	UINT m_EndIdx;
	double m_time; // To check whether m_EndIdx exceeds m_time

public:
	virtual BOOL OnInitDialog();

	CStatic m_textStartIdx;
	CStatic m_textEndIdx;
	CEdit m_editStartIdx;
	CEdit m_editEndIdx;
	CButton m_btnRedraw;
	CButton m_btnRun;

	void Initialize_GUI();
	afx_msg void OnBnClickedBtnRedraw();
	afx_msg void OnBnClickedBtnAnimationRun();
};