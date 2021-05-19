#pragma once


// Animation 대화 상자

class Animation : public CDialogEx {
	DECLARE_DYNAMIC(Animation)

public:
	Animation(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Animation();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DAQVIZ_DIALOG_ANIMATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
