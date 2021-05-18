// GraphClipping.cpp: 구현 파일
//

#include "pch.h"
#include "DAQViz.h"
#include "GraphClipping.h"
#include "afxdialogex.h"

// GraphClipping 대화 상자

IMPLEMENT_DYNAMIC(GraphClipping, CDialogEx)

GraphClipping::GraphClipping(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAQVIZ_DIALOG_GRAPH_CLIPPING, pParent) {

}

GraphClipping::~GraphClipping() {

}

void GraphClipping::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(GraphClipping, CDialogEx)
END_MESSAGE_MAP()

// GraphClipping 메시지 처리기

BOOL GraphClipping::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(NULL, 0, -1080, 0, 0, SWP_NOSIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}