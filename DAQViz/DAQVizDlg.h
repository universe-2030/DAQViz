// DAQVizDlg.h: 헤더 파일

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cstdlib>

#include "DELSYSDAQ.h"
#include "MatchDevice.h"

// #include "../TwinCAT/TwinCAT/Timer_TwinCAT.h"
#include "C:/Users/universe2030/Desktop/TwinCAT/TwinCAT/TwinCAT/Timer_TwinCAT.h"		// TwinCAT project header

#include "DAQVizChildKSJ.h"
#include "DAQVizChildOpenGL.h"
#include "DAQVizOpenGLWindow.h"
#include "NI_AI_AO_DO.h"

#include "TrainedModel.h"
#include "TestModel.h"

#define TEST_FLAG 1

#define THREAD_MAIN		0
#define THREAD_TWINCAT  1
#define THREAD_CALLBACK 2

#define	TIMER_EDIT 1

#define N_FLEX 5

#define CALI_START 0.100
#define CALI_END 1.500

// CDAQVizDlg 대화 상자
class CDAQVizDlg : public CDialogEx {
public:
	CDAQVizDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DAQVIZ_DIALOG_MAIN };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	// MFC control variables
	CButton m_btnSwitch;
	BOOL m_flag_Switch = 0;

	CStatic m_textSaveFolderPath;
	CEdit m_editSaveFolderPath;

	CStatic m_textSelectDlg;
	CComboBox m_comboSelectDlg;

	CStatic m_textTrainingMode;
	UINT m_radioTrainingMode;

	CStatic m_textDataStreamingMode;
	UINT m_radioStreamingMode;

	CButton m_btnLoad;
	CEdit m_editLoadName;

	CStatic m_textSaveMode;
	UINT m_radioSaveMode;

	CStatic m_textsEMGDAQDev;
	UINT m_radiosEMGDAQDev;

	CStatic m_textUseIMU;
	UINT m_radioUseIMU;

	CStatic m_textUseFlexSensor;
	UINT m_radioUseFlexSensor;

	CEdit m_editStatusBar;

	CStatic m_textControlTime;
	CEdit m_editControlTime;

	// TwinCAT variables
	HANDLE hMutex;
	HANDLE hMemory;
	STARTUPINFO	startupinfo;
	PROCESS_INFORMATION	processinfo;

	struct Shared_Data* pShared_Data;

	CCriticalSection g_csExitThread;
	volatile bool g_bExitThread = true;

	double m_time = 0.0;

	// Child dialogs
	DAQVizChildKSJ* p_ChildDlg_KSJ;
	DAQVizOpenGLWindow* p_OpenGLWin;

	// Saving folder
	CString SaveFolderPath;
	CString SaveFolderName;

	// Error & Status text
	CString	error_text;
	CString stat = L"";

	// Elapsed time variables
	double Time_DAQ_elapse;
	double Time_RTGraph_elapse;

	// Load the txt file
	ifstream inFile;
	bool File_loaded_or_not = FALSE;

	// DAQ device
	DELSYSDAQ* DELSYS_Dev;
	MatchDevice* MATCH_Dev;

	// NI analog input modules
	NI_AI_sEMG* AI_sEMG;
	NI_AI_Flex* AI_Flex;

	// Trained & Test model
	TrainedModel c_TrainedModel;
	TestModel c_TestModel;

	// Temporary
	double* sEMG_temp;
	double* sEMG_temp_abs;

	float64* Flex_data;
	float64* Flex_data_calib;

	double* sEMG_temp_16CH;

	// Calibration
	int cali_count = 0;

	// Training flag
	bool bTrained = FALSE;

public:
	// Thread functions
	static UINT MainThreadFunc(LPVOID IParam);
	int MainStart();

	// Initialization
	void Initialize_Variable();
	void Initialize_NI();
	void Initialize_MFC();
	void Initialize_GUI();
	void Initialize_SaveFolder();
	void Initialize_LogonU();
	void Initialize_RTOS();	

	// MFC Controls
	afx_msg void RadioCtrl(UINT ID);
	afx_msg void OnBnClickedBtnSwitch();

	// Utilities
	void Set_Font(CButton& Btn_, UINT Height_, UINT Width_);
	void Set_Font(CEdit& Text_, UINT Height_, UINT Width_);
	void Set_Font(CStatic& Text_, UINT Height_, UINT Width_);

	// Stack & Save
	void StackData();
	void SaveData();
	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnCbnSelchangeComboDlgSelect();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	bool bProcessEnd;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};