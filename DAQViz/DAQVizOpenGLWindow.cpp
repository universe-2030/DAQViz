// DAQVizOpenGLWindow.cpp: 구현 파일
//

#include "pch.h"
#include "DAQViz.h"
#include "DAQVizOpenGLWindow.h"
#include "afxdialogex.h"

// DAQVizOpenGLWindow 대화 상자

IMPLEMENT_DYNAMIC(DAQVizOpenGLWindow, CDialogEx)

DAQVizOpenGLWindow::DAQVizOpenGLWindow(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAQVIZ_DIALOG_NEW_WINDOW, pParent) {

}

DAQVizOpenGLWindow::~DAQVizOpenGLWindow() {

}

void DAQVizOpenGLWindow::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(DAQVizOpenGLWindow, CDialogEx)
END_MESSAGE_MAP()

// DAQVizOpenGLWindow 메시지 처리기

BOOL DAQVizOpenGLWindow::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(NULL, 0, -1080, 0, 0, SWP_NOSIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
