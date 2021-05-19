// Animation.cpp: 구현 파일
//

#include "pch.h"
#include "DAQViz.h"
#include "Animation.h"
#include "afxdialogex.h"


// Animation 대화 상자

IMPLEMENT_DYNAMIC(Animation, CDialogEx)

Animation::Animation(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAQVIZ_DIALOG_ANIMATION, pParent) {

}

Animation::~Animation() {

}

void Animation::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(Animation, CDialogEx)
END_MESSAGE_MAP()

// Animation 메시지 처리기
