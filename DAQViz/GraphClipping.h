#pragma once

// GraphClipping 대화 상자

class GraphClipping : public CDialogEx {
	DECLARE_DYNAMIC(GraphClipping)

public:
	GraphClipping(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~GraphClipping();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DAQVIZ_DIALOG_GRAPH_CLIPPING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
