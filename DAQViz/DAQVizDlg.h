// DAQVizDlg.h: 헤더 파일

#pragma once

#include <iostream>
#include <fstream>
#include <string>
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

#define WIN_SIZE 250

#define DELSYS_CH_MAX 16
#define FRANKFURT_CH_MAX 8
#define FINGER_CH_MAX 5
#define WRIST_CH_MAX 2

#define FINGER_ANALOG_ABS_MAX 0.75
#define WRIST_FE_ANALOG_ABS_MAX 0.3
#define WRIST_RU_ANALOG_ABS_MAX 0.3

#define MOTION_DOF 3

#define SAVE_FOLDER_PATH_MACRO _T("E:/OneDrive - postech.ac.kr/연구/### 데이터/DAQViz data/")

#define X_POS_INIT 0.0
#define X_POS_MIN -2.0
#define X_POS_MAX 2.0
#define X_POS_STEP_SIZE 0.001

#define Y_POS_INIT 0.0
#define Y_POS_MIN -2.0
#define Y_POS_MAX 2.0
#define Y_POS_STEP_SIZE 0.001

#define RAD_INIT 0.4
#define RAD_MIN 0.1
#define RAD_MAX 1.2
#define RAD_STEP_SIZE 0.001

#define NUM_FILE_LOAD 4
#define NUM_FILE_LOAD_PARAMETER 2
#define MAX_FILES 1000
#define MAX_PATH 150

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
	
	CStatic m_textUseFingerFlex;
	UINT    m_radioUseFingerFlex;
	
	CStatic m_textUseWristFlex;
	UINT m_radioUseWristFlex;

	CEdit m_editStatusBar;

	CStatic m_textControlTime;
	CEdit   m_editControlTime;

	CStatic		m_textStartIdx;
	CEdit		m_editStartIdx;
	CStatic		m_textEndIdx;
	CEdit		m_editEndIdx;

	CStatic		m_textNumsEMGCH;
	CEdit		m_editNumsEMGCH;
	CStatic		m_textNumFingerFlexCH;
	CEdit		m_editNumFingerFlexCH;
	CStatic		m_textNumWristFlexCH;
	CEdit		m_editNumWristFlexCH;

	// Container variables
	std::vector<double> Time_stack;
	std::vector<double> Time_DAQ_elapse_stack;
	std::vector<double> Time_RTGraph_elapse_stack;

	double** sEMG_raw_window;
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

	// Container variables - only for loaded data
	std::vector<double>* sEMG_MAV_stack_loaded;
	std::vector<double>* Finger_raw_stack_loaded;
	std::vector<double>* Wrist_raw_stack_loaded;
	std::vector<double>* MotionLabel_loaded;
	
	// Container variables - only for loaded parameters


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
	CString stat = _T("");

	// Elapsed time variables
	double Time_DAQ_elapse;
	double Time_RTGraph_elapse;

	// Load the previously saved file
	CString* m_filelist_dir_Data;
	CString* m_filelist_name_Data;
	char Data_getline[10000];
	ifstream* inFile_data;
	int* N_CH_each_data;
	bool isDataLoaded;

	CString* m_filelist_dir_Param;
	CString* m_filelist_name_Param;
	char Parameters_getline[1000];
	ifstream* inFile_parameters;
	int* N_CH_each_Param;
	bool isParameterLoaded;

	// DAQ device
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

	double** Label_Est;

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

	// Set variables for loaded data
	void Set_Loaded_Data();

	// Set variables for loaded parameters
	void Set_Loaded_Model_Parameters();

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

	void Set_loaded_Data_stack();
	void Set_loaded_Param_stack();

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
	void StackData (double _m_time,
					double _Time_DAQ_elapse,
					double _Time_RTGraph_elapse,
					double* _sEMG_raw,
					double* _sEMG_abs,
					double* _sEMG_MAV,
					double* _Finger_raw,
					double* _Finger_slope,
					double* _Wrist_raw,
					double* _Wrist_slope,
					double* _MotionLabel_current,
					double* _MotionEstimation_current,
					double _X_pos,
					double _Y_pos,
					double _Rad);
	void SaveData(CString SaveFolderName);
	void SaveParameters(CString SaveFolderName);
	void SaveModel(CString SaveFolderName);

	ofstream f_time, f_time_elapsed_DAQ, f_time_elapsed_RTGraph;
	ofstream f_sEMG_raw, f_sEMG_abs, f_sEMG_MAV;
	ofstream f_Finger_raw, f_Finger_slope;
	ofstream f_Wrist_raw, f_Wrist_slope;
	ofstream f_MotionLabel, f_MotionEstimation;
	ofstream f_X_pos_ball, f_Y_pos_ball, f_Rad_ball;
	ofstream f_parameters;
	ofstream f_model_sEMG_mean, f_model_sEMG_std;

	afx_msg void OnEnChangeEditNumSemgCh();
	afx_msg void OnEnChangeEditNumFlexCh();
	afx_msg void OnEnChangeEditNumImuCh();
	afx_msg void OnBnClickedBtnParameterLoad();
};