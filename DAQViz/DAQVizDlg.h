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
#include "Model.h"

#include "../TwinCAT/TwinCAT/Timer_TwinCAT.h"
#include "DAQVizChildKSJ.h"

#include "NI_AI_AO_DO.h"

#define TS 0.001
#define Fs 1000

#define TEST_FLAG 1

#define	TIMER_EDIT 1

#define CALI_START 0.300
#define CALI_END 1.500

#define N_GRAPH 8

#define DELSYS_CH_MAX 16
#define FRANKFURT_CH_MAX 8
#define FINGER_CH_MAX 5
#define WRIST_CH_MAX 2

#define FLEX_ANALOG_ABS_MAX 0.75

#define SAVE_FOLDER_PATH_MACRO "D:/Training-free algorithm/"

#define X_POS_INIT 0.0
#define X_POS_MIN -0.5
#define X_POS_MAX 0.5
#define X_POS_STEP_SIZE 0.001

#define Y_POS_INIT 0.0
#define Y_POS_MIN -0.5
#define Y_POS_MAX 0.5
#define Y_POS_STEP_SIZE 0.001

#define RAD_INIT 0.3
#define RAD_MIN 0.1
#define RAD_MAX 0.9
#define RAD_STEP_SIZE 0.001

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
	UINT Num_Finger_CH;
	UINT Num_Wrist_CH;

	// MFC control variables
	CButton m_btnSwitch;
	BOOL m_flag_Switch = 0;

	CStatic m_textSaveFolderPath;
	CEdit m_editSaveFolderPath;

	CStatic m_textSelectDlg;
	CComboBox m_comboSelectDlg;

	CStatic m_textTrainingMode;
	UINT m_radioTrainingMode;
	CButton m_btnParameterLoad;
	CEdit m_editParameterLoadName;

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

	std::vector<double>* Finger_raw_stack;
	std::vector<double>* Finger_slope_stack;
	std::vector<double>* Wrist_raw_stack;
	std::vector<double>* Wrist_slope_stack;

	std::vector<double>* MotionLabel;
	std::vector<double>* MotionEstimation;

	double X_pos_ball;
	double Y_pos_ball;
	double Rad_ball;

	std::vector<double>* X_pos_ball_stack;
	std::vector<double>* Y_pos_ball_stack;
	std::vector<double>* Rad_ball_stack;

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

	// Saving folder
	CString SaveFolderPath_Main;
	CString SaveFolderPath;
	CString SaveFolderName;

	// Error & Status text
	CString	error_text;
	CString stat = L"";

	// Elapsed time variables
	double Time_DAQ_elapse;
	double Time_RTGraph_elapse;

	// Load the model parameter file
	ifstream inFile_model_param;
	bool isModelParamLoaded;

	// Load the previously saved file
	ifstream inFile_data;
	bool isDataLoaded;

	// DAQ device
	DELSYSDAQ* DELSYS_Dev;
	MatchDevice* MATCH_Dev;
	bool isMATCHconnected;

	// NI analog input modules
	NI_AI_sEMG* AI_sEMG;
	NI_AI_Flex* AI_Flex;

	// Pointer variables
	double* sEMG_raw_plot;
	double* sEMG_abs_plot;
	double* sEMG_MAV_plot;

	float64* Flex_data;
	float64* Flex_data_calib;
	float64* Flex_data_prev;
	float64* Flex_slope;
	float64* Flex_slope_prev;

	float64* Finger_data;
	float64* Finger_slope;
	float64* Wrist_data;
	float64* Wrist_slope;

	double* Label_Est;

	// Calibration
	int cali_count = 0;

	// Training flag
	bool bTrained = FALSE;

	// Signal processor
	SignalProcessor* SigProc;

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
	bool Get_TimerStarted();
	
	const std::vector<double>* Get_sEMG_raw_stack();
	const std::vector<double>* Get_sEMG_abs_stack();
	const std::vector<double>* Get_sEMG_MAV_stack();

	const std::vector<double>* Get_Finger_raw_stack();
	const std::vector<double>* Get_Finger_slope_stack();
	const std::vector<double>* Get_Wrist_raw_stack();
	const std::vector<double>* Get_Wrist_slope_stack();

	const std::vector<double>* Get_MotionLabel_stack();
	const std::vector<double>* Get_MotionEstimation_stack();

	const std::vector<double>* Get_X_pos_ball_stack();
	const std::vector<double>* Get_Y_pos_ball_stack();
	const std::vector<double>* Get_Rad_ball_stack();

	// Stack & Save
	void StackData (double* _sEMG_raw,
					double* _sEMG_abs,
					double* _sEMG_MAV,
					double* _Finger_raw,
					double* _Finger_slope,
					double* _Wrist_raw,
					double* _Wrist_slope,
					double _MotionLabel_current,
					double _MotionEstimation_current,
					double _X_pos,
					double _Y_pos,
					double _Rad,
					double _Time_DAQ_elapse,
					double _Time_RTGraph_elapse);
	void SaveData(CString SaveFolderName);
	afx_msg void OnEnChangeEditNumSemgCh();
	afx_msg void OnEnChangeEditNumFlexCh();
	afx_msg void OnEnChangeEditNumImuCh();
	afx_msg void OnBnClickedBtnParameterLoad();
};