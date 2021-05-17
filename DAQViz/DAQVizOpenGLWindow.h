#pragma once


// DAQVizOpenGLWindow 대화 상자

class DAQVizOpenGLWindow : public CDialogEx
{
	DECLARE_DYNAMIC(DAQVizOpenGLWindow)

public:
	DAQVizOpenGLWindow(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~DAQVizOpenGLWindow();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DAQVIZ_DIALOG_NEW_WINDOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
