// DAQVizDlg.h: 헤더 파일

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cstdlib>

#include "DELSYSDAQ.h"
#include "MatchDevice.h"
#include "SignalProcessor.h"

#include "../TwinCAT/TwinCAT/Timer_TwinCAT.h"
#include "DAQVizChildKSJ.h"
#include "DAQVizChildOpenGL.h"
#include "BallControl.h"

#include "NI_AI_AO_DO.h"

#include "TrainedModel.h"
#include "TestModel.h"

#define TEST_FLAG 1

#define	TIMER_EDIT 1

#define N_SEMG_CH 16
#define N_FLEX_CH 5
#define N_IMU_CH 2

#define CALI_START 0.100
#define CALI_END 1.500

#define N_GRAPH 5

#define DELSYS_CH_MAX 16
#define FRANKFURT_CH_MAX 8
#define FLEX_CH_MAX 5
#define IMU_CH_MAX 2

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
	// The number of channels
	UINT Num_sEMG_CH;
	UINT Num_Flex_CH;
	UINT Num_IMU_CH;

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
	
	CStatic m_textUseFlexSensor;
	UINT    m_radioUseFlexSensor;
	
	CStatic m_textUseIMU;
	UINT m_radioUseIMU;

	CEdit m_editStatusBar;

	CStatic m_textControlTime;
	CEdit   m_editControlTime;

	CStatic		m_textStartIdx;
	CEdit		m_editStartIdx;
	CStatic		m_textEndIdx;
	CEdit		m_editEndIdx;

	CStatic		m_textNumsEMGCH;
	CEdit		m_editNumsEMGCH;
	CStatic		m_textNumFlexCH;
	CEdit		m_editNumFlexCH;
	CStatic		m_textNumIMUCH;
	CEdit		m_editNumIMUCH;

	// Container variables
	std::vector<double>* sEMG_raw_stack;
	std::vector<double>* sEMG_abs_stack;
	std::vector<double>* sEMG_MAV_stack;

	std::vector<double>* Flex_raw_stack;
	std::vector<double>* Flex_LPF_stack;

	std::vector<double>* IMU_raw_stack;
	std::vector<double>* IMU_LPF_stack;

	std::vector<double> Time_DAQ_elapse_stack;
	std::vector<double> Time_RTGraph_elapse_stack;

	// TwinCAT variables
	HANDLE hMutex;
	HANDLE hMemory;
	STARTUPINFO	startupinfo;
	PROCESS_INFORMATION	processinfo;

	struct Shared_Data* pShared_Data;

	CCriticalSection g_csExitThread;
	volatile bool g_bExitThread = true;

	double m_time;
	UINT m_count;

	bool TimerStarted;
	bool b_SaveImmediate_Dlg;

	// Child dialogs
	DAQVizChildKSJ* p_ChildDlg_KSJ;
	BallControl* p_BallControl;

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
	bool File_loaded_or_not;

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
	double* sEMG_temp_MAV;

	float64* Flex_data;
	float64* Flex_data_calib;
	float64* Flex_data_LPF;

	double* sEMG_temp_16CH;

	// Calibration
	int cali_count = 0;

	// Training flag
	bool bTrained = FALSE;

	// Signal processor
	SignalProcessor SigProc;

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

	// Dynamic allocation
	void Dynamic_Allocation();
	void Dynamic_Free();

	// MFC Controls
	afx_msg void RadioCtrl(UINT ID);
	afx_msg void OnBnClickedBtnSwitch();
	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnCbnSelchangeComboDlgSelect();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

	void Set_StartIdx(int _idx);
	void Set_EndIdx(int _idx);
	void Initialize_StartIdx();
	void Initialize_EndIdx();

	void Set_MFC_Control_Availability(bool _isAvailable);

	// Utilities
	void Set_Font(CButton& Btn_, UINT Height_, UINT Width_);
	void Set_Font(CEdit& Text_, UINT Height_, UINT Width_);
	void Set_Font(CStatic& Text_, UINT Height_, UINT Width_);

	// Get functions
	double Get_m_time();
	UINT Get_m_count();

	// Stack & Save
	void StackData (double* _sEMG_raw,
					double* _sEMG_abs,
					double* _sEMG_MAV,
					double* _Flex_raw,
					double* _Flex_LPF,
					double* _IMU_raw,
					double* _IMU_LPF,
					double _Time_DAQ_elapse,
					double _Time_RTGraph_elapse);
	void SaveData();	
	afx_msg void OnEnChangeEditNumSemgCh();
	afx_msg void OnEnChangeEditNumFlexCh();
	afx_msg void OnEnChangeEditNumImuCh();
};