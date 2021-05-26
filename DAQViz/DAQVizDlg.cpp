// DAQVizDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "DAQViz.h"
#include "DAQVizDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx {
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CDAQVizDlg 대화 상자

CDAQVizDlg::CDAQVizDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAQVIZ_DIALOG, pParent), 
	m_radioTrainingMode(1), m_radioStreamingMode(0),
	m_radioSaveMode(1), m_radiosEMGDAQDev(0),
	m_radioUseFingerFlex(0), m_radioUseWristFlex(0) {
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDAQVizDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_SWITCH, m_btnSwitch);

	DDX_Control(pDX, IDC_TEXT_SAVEFOLDERPATH, m_textSaveFolderPath);
	DDX_Control(pDX, IDC_EDIT_SAVEFOLDERPATH, m_editSaveFolderPath);

	DDX_Control(pDX, IDC_TEXT_COMBO, m_textSelectDlg);
	DDX_Control(pDX, IDC_COMBO_DLG_SELECT, m_comboSelectDlg);

	DDX_Control(pDX, IDC_TEXT_TRAINING_MODE, m_textTrainingMode);
	DDX_Radio(pDX, IDC_RADIO_UNSUPERVISED, (int&)m_radioTrainingMode);
	DDX_Control(pDX, IDC_BTN_PARAMETER_LOAD, m_btnParameterLoad);
	DDX_Control(pDX, IDC_EDIT_PARAMETER_LOAD_NAME, m_editParameterLoadName);

	DDX_Control(pDX, IDC_TEXT_DATA_STREAMING, m_textDataStreamingMode);
	DDX_Radio(pDX, IDC_RADIO_DATA_STREAMING_RT, (int&)m_radioStreamingMode);

	DDX_Control(pDX, IDC_BTN_LOAD, m_btnLoad);
	DDX_Control(pDX, IDC_EDIT_LOAD_NAME, m_editLoadName);

	DDX_Control(pDX, IDC_TEXT_SAVE_MODE, m_textSaveMode);
	DDX_Radio(pDX, IDC_RADIO_SAVE_IMMEDIATE, (int&)m_radioSaveMode);

	DDX_Control(pDX, IDC_TEXT_SEMG_DAQ_DEVICE, m_textsEMGDAQDev);
	DDX_Radio(pDX, IDC_RADIO_DEVICE_DELSYS, (int&)m_radiosEMGDAQDev);

	DDX_Control(pDX, IDC_TEXT_USE_FINGER_FLEX, m_textUseFingerFlex);
	DDX_Radio(pDX, IDC_RADIO_USE_FINGER_FLEX_YES, (int&)m_radioUseFingerFlex);

	DDX_Control(pDX, IDC_TEXT_USE_WRIST_FLEX, m_textUseWristFlex);
	DDX_Radio(pDX, IDC_RADIO_USE_WRIST_FLEX_YES, (int&)m_radioUseWristFlex);

	DDX_Control(pDX, IDC_EDIT_STATUS_BAR, m_editStatusBar);

	DDX_Control(pDX, IDC_TEXT_CONTROL_TIME, m_textControlTime);
	DDX_Control(pDX, IDC_EDIT_CONTROL_TIME, m_editControlTime);
	DDX_Control(pDX, IDC_TEXT_TIME_START_IDX, m_textStartIdx);
	DDX_Control(pDX, IDC_TEXT_TIME_END_IDX, m_textEndIdx);
	DDX_Control(pDX, IDC_EDIT_TIME_START_IDX, m_editStartIdx);
	DDX_Control(pDX, IDC_EDIT_TIME_END_IDX, m_editEndIdx);
	DDX_Control(pDX, IDC_TEXT_NUM_SEMG_CH, m_textNumsEMGCH);
	DDX_Control(pDX, IDC_EDIT_NUM_SEMG_CH, m_editNumsEMGCH);
	DDX_Control(pDX, IDC_TEXT_NUM_FINGER_FLEX_CH, m_textNumFingerFlexCH);
	DDX_Control(pDX, IDC_TEXT_NUM_WRIST_FLEX_CH, m_textNumWristFlexCH);
	DDX_Control(pDX, IDC_EDIT_NUM_FINGER_FLEX_CH, m_editNumFingerFlexCH);
	DDX_Control(pDX, IDC_EDIT_NUM_WRIST_FLEX_CH, m_editNumWristFlexCH);
}

BEGIN_MESSAGE_MAP(CDAQVizDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SWITCH, &CDAQVizDlg::OnBnClickedBtnSwitch)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_UNSUPERVISED, IDC_RADIO_TEST, RadioCtrl)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_DATA_STREAMING_RT, IDC_RADIO_DATA_STREAMING_LOAD, RadioCtrl)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_SAVE_IMMEDIATE, IDC_RADIO_STOP_AND_RUN_STACK_OFF, RadioCtrl)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_DEVICE_DELSYS, IDC_RADIO_DEVICE_FRANKFURT, RadioCtrl)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_USE_WRIST_FLEX_YES, IDC_RADIO_USE_WRIST_FLEX_NO, RadioCtrl)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_USE_FINGER_FLEX_YES, IDC_RADIO_USE_FINGER_FLEX_NO, RadioCtrl)
	ON_BN_CLICKED(IDC_BTN_LOAD, &CDAQVizDlg::OnBnClickedBtnLoad)
	ON_CBN_SELCHANGE(IDC_COMBO_DLG_SELECT, &CDAQVizDlg::OnCbnSelchangeComboDlgSelect)
//	ON_WM_SIZING()
ON_WM_SIZE()
ON_WM_TIMER()
ON_WM_DESTROY()
ON_EN_CHANGE(IDC_EDIT_NUM_SEMG_CH, &CDAQVizDlg::OnEnChangeEditNumSemgCh)
ON_EN_CHANGE(IDC_EDIT_NUM_FINGER_FLEX_CH, &CDAQVizDlg::OnEnChangeEditNumFlexCh)
ON_EN_CHANGE(IDC_EDIT_NUM_WRIST_FLEX_CH, &CDAQVizDlg::OnEnChangeEditNumImuCh)
ON_BN_CLICKED(IDC_BTN_PARAMETER_LOAD, &CDAQVizDlg::OnBnClickedBtnParameterLoad)
END_MESSAGE_MAP()

// CDAQVizDlg 메시지 처리기

BOOL CDAQVizDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	Initialize_Variable();
	Initialize_NI();
	Initialize_GUI();
	Initialize_MFC();
	Initialize_SaveFolder();
	Initialize_LogonU();

	std::cout << "EE" << std::endl;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDAQVizDlg::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDAQVizDlg::OnPaint() {
	if (IsIconic()) {
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else {
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDAQVizDlg::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}

void CDAQVizDlg::Initialize_Variable() {
	m_time = 0.0;
	m_count = 0;

	TimerStarted = FALSE;
	if (m_radioSaveMode == 0)
		b_SaveImmediate_Dlg = TRUE;
	else
		b_SaveImmediate_Dlg = FALSE;

	isMATCHconnected = FALSE;

	isDataLoaded = FALSE;
	isParameterLoaded = FALSE;

	X_pos_ball = X_POS_INIT;
	Y_pos_ball = Y_POS_INIT;
	Rad_ball = RAD_INIT;
}

void CDAQVizDlg::Initialize_NI() {
	AI_sEMG = new NI_AI_sEMG("Dev1/ai0:15", DELSYS_CH_MAX);
	NI_AI_sEMG::InitializeNI();

	AI_Flex = new NI_AI_Flex("Dev2/ai0:6", FINGER_CH_MAX + WRIST_CH_MAX);
	NI_AI_Flex::InitializeNI();
}

void CDAQVizDlg::Initialize_MFC() {
	SetTimer(TIMER_EDIT, 20, NULL);
}

void CDAQVizDlg::Initialize_GUI() {
	Set_Font(m_btnSwitch, 40, 15);
	Set_Font(m_textSaveFolderPath, 20, 8);
	Set_Font(m_textSelectDlg, 20, 8);
	Set_Font(m_textTrainingMode, 20, 8);
	Set_Font(m_textDataStreamingMode, 20, 8);
	Set_Font(m_textSaveMode, 20, 8);
	Set_Font(m_textsEMGDAQDev, 20, 8);
	Set_Font(m_textUseWristFlex, 20, 8);
	Set_Font(m_textUseFingerFlex, 20, 8);
	Set_Font(m_textControlTime, 20, 8);
	Set_Font(m_editControlTime, 20, 8);
	Set_Font(m_textStartIdx, 20, 8);
	Set_Font(m_editStartIdx, 20, 8);
	Set_Font(m_textEndIdx, 20, 8);
	Set_Font(m_editEndIdx, 20, 8);
	Set_Font(m_textNumsEMGCH, 20, 8);
	Set_Font(m_editNumsEMGCH, 20, 8);
	Set_Font(m_textNumFingerFlexCH, 20, 8);
	Set_Font(m_editNumFingerFlexCH, 20, 8);
	Set_Font(m_textNumWristFlexCH, 20, 8);
	Set_Font(m_editNumWristFlexCH, 20, 8);

	m_comboSelectDlg.AddString(_T("1. Sejin Kim"));
	// m_comboSelectDlg.AddString(_T("2. Another users"));
	m_comboSelectDlg.SetCurSel(0);

	m_editParameterLoadName.SetWindowText(_T("Select files"));
	m_editLoadName.SetWindowText(_T("Select files"));
	m_editStartIdx.SetWindowText(_T("Not selected"));
	m_editEndIdx.SetWindowText(_T("Not selected"));

	CString temp;
	temp.Format(_T("%d"), DELSYS_CH_MAX);
	m_editNumsEMGCH.SetWindowTextW(temp);
	temp.Format(_T("%d"), FINGER_CH_MAX);
	m_editNumFingerFlexCH.SetWindowTextW(temp);
	temp.Format(_T("%d"), WRIST_CH_MAX);
	m_editNumWristFlexCH.SetWindowTextW(temp);

	if (m_radioTrainingMode == 0 || m_radioTrainingMode == 1)
		m_btnParameterLoad.EnableWindow(FALSE);
	else
		m_btnParameterLoad.EnableWindow(TRUE);

	if (m_radioStreamingMode == 0)
		m_btnLoad.EnableWindow(FALSE);
	else
		m_btnLoad.EnableWindow(TRUE);

	CRect rectofDialogArea;
	GetDlgItem(IDC_SLOT_CHILDDLG)->GetWindowRect(&rectofDialogArea);
	ScreenToClient(&rectofDialogArea);

	p_ChildDlg_KSJ = new DAQVizChildKSJ(DELSYS_CH_MAX, FINGER_CH_MAX, WRIST_CH_MAX);
	p_ChildDlg_KSJ->Create(IDD_DAQVIZ_DIALOG_CHILD_KSJ, this);
	p_ChildDlg_KSJ->ShowWindow(SW_SHOW);
	p_ChildDlg_KSJ->MoveWindow(rectofDialogArea);

	SetWindowPos(NULL, -1920, -1080, 0, 0, SWP_NOSIZE);
}

void CDAQVizDlg::Initialize_SaveFolder() {
	SaveFolderPath_Main = SAVE_FOLDER_PATH_MACRO;

	char timeDateBuf[80];
	time_t now = time(0);
	struct tm tstruct;
	tstruct = *localtime(&now);

	SaveFolderName = "";
	SaveFolderName += "Data_";
	strftime(timeDateBuf, sizeof(timeDateBuf), "%y%m%d", &tstruct);
	SaveFolderName += timeDateBuf;
	SaveFolderName += "_";
	strftime(timeDateBuf, sizeof(timeDateBuf), "%H%M%S", &tstruct);
	SaveFolderName += timeDateBuf;

	if (m_radioSaveMode == 0)
		SaveFolderPath = SaveFolderPath_Main + SaveFolderName + _T("_Unsupervised");
	else if (m_radioSaveMode == 1)
		SaveFolderPath = SaveFolderPath_Main + SaveFolderName + _T("_Training");
	else if (m_radioSaveMode == 2)
		SaveFolderPath = SaveFolderPath_Main + SaveFolderName + _T("_Test");

	m_editSaveFolderPath.SetWindowText(SaveFolderPath);
}

void CDAQVizDlg::Initialize_LogonU() {
	MATCH_Dev = new MatchDevice();
	if (MATCH_Dev->InitMATCH()) {
		MessageBox(_T("Unable to initialize the MATCH device"), _T("Warning"), MB_ICONERROR);
	}

	MATCH_Dev->GetDataAddress();

	if (MATCH_Dev->OpenMATCH()) {
		MessageBox(_T("Unable to open the MATCH device"), _T("Warning"), MB_ICONERROR);
		MATCH_Dev->CloseMATCH();

		GetDlgItem(IDC_RADIO_DEVICE_FRANKFURT)->EnableWindow(FALSE);
	}
	else
		isMATCHconnected = TRUE;
}

void CDAQVizDlg::Dynamic_Allocation() {
	sEMG_raw_plot = new double[Num_sEMG_CH];
	memset(sEMG_raw_plot, 0.0, 2 * sizeof(sEMG_raw_plot) * Num_sEMG_CH);
	sEMG_abs_plot = new double[Num_sEMG_CH];
	memset(sEMG_abs_plot, 0.0, 2 * sizeof(sEMG_abs_plot) * Num_sEMG_CH);
	sEMG_MAV_plot = new double[Num_sEMG_CH];
	memset(sEMG_MAV_plot, 0.0, 2 * sizeof(sEMG_MAV_plot) * Num_sEMG_CH);

	Flex_data_calib = new float64[Num_Flex_CH];
	memset(Flex_data_calib, 0.0, 2 * sizeof(Flex_data_calib) * Num_Flex_CH);
	Flex_data_prev = new float64[Num_Flex_CH];
	memset(Flex_data_prev, 0.0, 2 * sizeof(Flex_data_prev) * Num_Flex_CH);
	Flex_slope = new float64[Num_Flex_CH];
	memset(Flex_slope, 0.0, 2 * sizeof(Flex_slope) * Num_Flex_CH);
	Flex_slope_prev = new float64[Num_Flex_CH];
	memset(Flex_slope_prev, 0.0, 2 * sizeof(Flex_slope_prev) * Num_Flex_CH);

	Finger_data = new float64[Num_Finger_CH];
	memset(Finger_data, 0.0, 2 * sizeof(Finger_data) * Num_Finger_CH);
	Finger_slope = new float64[Num_Finger_CH];
	memset(Finger_slope, 0.0, 2 * sizeof(Finger_slope) * Num_Finger_CH);
	Wrist_data = new float64[Num_Wrist_CH];
	memset(Wrist_data, 0.0, 2 * sizeof(Wrist_data) * Num_Wrist_CH);
	Wrist_slope = new float64[Num_Wrist_CH];
	memset(Wrist_slope, 0.0, 2 * sizeof(Wrist_slope) * Num_Wrist_CH);

	Label_Est = new double*[2];
	for (int i = 0; i < 2; i++) {
		Label_Est[i] = new double[3];
		memset(Label_Est[i], 0.0, 2 * sizeof(Label_Est[i]) * 2);
	}

	sEMG_raw_window = new double* [Num_sEMG_CH];
	for (int i = 0; i < Num_sEMG_CH; i++) {
		sEMG_raw_window[i] = new double[WIN_SIZE];
		memset(sEMG_raw_window[i], 0.0, 2 * sizeof(sEMG_raw_window[i]) * WIN_SIZE);
	}
	sEMG_raw_stack = new std::vector<double>[Num_sEMG_CH];
	sEMG_abs_stack = new std::vector<double>[Num_sEMG_CH];
	sEMG_MAV_stack = new std::vector<double>[Num_sEMG_CH];
	
	Finger_raw_stack = new std::vector<double>[Num_Finger_CH];
	Finger_slope_stack = new std::vector<double>[Num_Finger_CH];
	Wrist_raw_stack = new std::vector<double>[Num_Wrist_CH];
	Wrist_slope_stack = new std::vector<double>[Num_Wrist_CH];

	MotionLabel = new std::vector<double>[MOTION_DOF];
	MotionEstimation = new std::vector<double>[MOTION_DOF];

	X_pos_ball_stack = new std::vector<double>;
	Y_pos_ball_stack = new std::vector<double>;
	Rad_ball_stack = new std::vector<double>;

	SigProc = new SignalProcessor(Num_sEMG_CH, Num_Finger_CH, Num_Wrist_CH, TS, Fs, 30);
}

void CDAQVizDlg::Dynamic_Free() {
	delete sEMG_raw_plot;
	delete sEMG_abs_plot;
	delete sEMG_MAV_plot;

	delete Flex_data_calib;
	delete Flex_data_prev;
	delete Flex_slope;
	delete Flex_slope_prev;

	delete Finger_data;
	delete Finger_slope;
	delete Wrist_data;
	delete Wrist_slope;
}

void CDAQVizDlg::Set_Font(CButton& Btn_, UINT Height_, UINT Width_) {
	CFont Font_;
	Font_.CreateFont(Height_, Width_,
		0, 0,
		FW_NORMAL,
		FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH,
		_T("Arial"));
	
	Btn_.SetFont(&Font_);
}

void CDAQVizDlg::Set_Font(CEdit& Edit_, UINT Height_, UINT Width_) {
	CFont Font_;
	Font_.CreateFont(Height_, Width_,
		0, 0,
		FW_NORMAL,
		FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH,
		_T("Arial"));

	Edit_.SetFont(&Font_);
}

void CDAQVizDlg::Set_Font(CStatic& Text_, UINT Height_, UINT Width_) {
	CFont Font_;
	Font_.CreateFont(Height_, Width_,
		0, 0,
		FW_NORMAL,
		FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH,
		_T("Arial"));
	Text_.SetFont(&Font_);
}

void CDAQVizDlg::OnBnClickedBtnSwitch() {
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_flag_Switch) { // Turned OFF -> ON
		if (!TimerStarted) {
			TimerStarted = TRUE;
			
			Set_loaded_Data_stack();
			Set_loaded_Param_stack();

			AfxBeginThread(MainThreadFunc, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		}
		else {
			g_csExitThread.Lock();
			pShared_Data->bContinue = TRUE;
			g_csExitThread.Unlock();
		}

		Set_MFC_Control_Availability(FALSE);
		m_btnSwitch.SetWindowText(_T("Stop"));
		m_flag_Switch = TRUE;
	}
	else { // Turned ON -> OFF
		if (m_radioSaveMode != 0) {
			Set_MFC_Control_Availability(TRUE);
			m_btnSwitch.SetWindowText(_T("Start"));
		}

		g_csExitThread.Lock();
		pShared_Data->bContinue = FALSE;
		g_csExitThread.Unlock();
		m_flag_Switch = FALSE;
	}
}

void CDAQVizDlg::OnBnClickedBtnLoad() {
	// Allow to select multiple files
	CFileDialog read_file(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, _T("파일 선택 (.txt) | *.txt"));

	CString temp;
	int nMaxFiles = MAX_FILES;
	int nBuffSize = nMaxFiles * MAX_PATH + 1;
	read_file.m_ofn.nMaxFile = nBuffSize; // To increase buffer size
	read_file.m_ofn.lpstrFile = temp.GetBuffer(nBuffSize);
	read_file.m_ofn.lpstrTitle = _T("Load할 데이터 텍스트 파일을 선택하세요.");
	read_file.m_ofn.lpstrInitialDir = _T("E:/OneDrive - postech.ac.kr/연구/### 데이터/DAQViz data/");

	CString m_filename;
	CString m_filedir;

	m_filelist_dir_Data = new CString[NUM_FILE_LOAD];
	m_filelist_name_Data = new CString[NUM_FILE_LOAD];

	UINT m_filelist_idx = 0;
	if (read_file.DoModal() == IDOK) {
		POSITION pos = read_file.GetStartPosition();

		// Load the .txt files
		while (pos != NULL) {
			m_filedir = read_file.GetNextPathName(pos);
			m_filename = m_filedir;
			m_filename.Delete(0, m_filedir.ReverseFind('\\') + 1);

			m_filelist_dir_Data[m_filelist_idx] = m_filedir;
			m_filelist_name_Data[m_filelist_idx] = m_filename;
			m_filelist_idx++;
		}

		// Check for loading wrong file
		for (int i = 0; i < NUM_FILE_LOAD; i++) {
			if (m_filelist_name_Data[0] != _T("Finger_flex_raw.txt")) {
				MessageBox(_T("First loading file should be 'Finger_flex_raw.txt'."),
							_T("Notice"), MB_OK | MB_ICONWARNING);
				delete[] m_filelist_dir_Data;
				delete[] m_filelist_name_Data;
				return;
			}
			if (m_filelist_name_Data[1] != _T("Motion_label.txt")) {
				MessageBox(_T("Second loading file should be 'Motion_label.txt'."),
							_T("Notice"), MB_OK | MB_ICONWARNING);
				delete[] m_filelist_dir_Data;
				delete[] m_filelist_name_Data;
				return;
			}
			if (m_filelist_name_Data[2] != _T("sEMG_MAV.txt")) {
				MessageBox(_T("Third loading file should be 'sEMG_MAV.txt'."),
							_T("Notice"), MB_OK | MB_ICONWARNING);
				delete[] m_filelist_dir_Data;
				delete[] m_filelist_name_Data;
				return;
			}
			if (m_filelist_name_Data[3] != _T("Wrist_flex_raw.txt")) {
				MessageBox(_T("Fourth loading file should be 'Wrist_flex_raw.txt'."),
							_T("Notice"), MB_OK | MB_ICONWARNING);
				delete[] m_filelist_dir_Data;
				delete[] m_filelist_name_Data;
				return;
			}
		}
		
		m_editLoadName.SetWindowText(_T("Files loaded"));
		m_editStatusBar.SetWindowText(stat += "[USER] Data load success \r\n");
		m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());

		// Enable switch
		isDataLoaded = TRUE;
		if (m_radioTrainingMode == 0 || m_radioTrainingMode == 1)
			m_btnSwitch.EnableWindow(TRUE);
		else if (m_radioTrainingMode == 2) {
			if (isParameterLoaded)
				m_btnSwitch.EnableWindow(TRUE);
			else
				m_btnSwitch.EnableWindow(FALSE);
		}
	}
}

void CDAQVizDlg::OnCbnSelchangeComboDlgSelect() {
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CRect rectofDialogArea;

	if (m_comboSelectDlg.GetCurSel() == 0) {
		GetDlgItem(IDC_SLOT_CHILDDLG)->GetWindowRect(&rectofDialogArea);
		ScreenToClient(&rectofDialogArea);

		if (p_ChildDlg_KSJ != NULL) {
			p_ChildDlg_KSJ->DestroyWindow();
			p_ChildDlg_KSJ = NULL;
		}

		p_ChildDlg_KSJ = new DAQVizChildKSJ();
		p_ChildDlg_KSJ->Create(IDD_DAQVIZ_DIALOG_CHILD_KSJ, this);
		p_ChildDlg_KSJ->ShowWindow(SW_SHOW);
		p_ChildDlg_KSJ->MoveWindow(rectofDialogArea);
	}
	else {
		GetDlgItem(IDC_SLOT_CHILDDLG)->GetWindowRect(&rectofDialogArea);
		ScreenToClient(&rectofDialogArea);

		if (p_ChildDlg_KSJ != NULL) {
			p_ChildDlg_KSJ->DestroyWindow();
			p_ChildDlg_KSJ = NULL;
		}

		// Should be replaced with another dialogs
		p_ChildDlg_KSJ = new DAQVizChildKSJ();
		p_ChildDlg_KSJ->Create(IDD_DAQVIZ_DIALOG_CHILD_KSJ, this);
		p_ChildDlg_KSJ->ShowWindow(SW_SHOW);
		p_ChildDlg_KSJ->MoveWindow(rectofDialogArea);
	}
}

void CDAQVizDlg::OnSize(UINT nType, int cx, int cy) {
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CDAQVizDlg::Set_loaded_Data_stack() {
	// In case of offline data loading
	if (m_radioStreamingMode == 1) {
		inFile_data = new ifstream[NUM_FILE_LOAD];
		N_CH_each_data = new int[NUM_FILE_LOAD];
		memset(N_CH_each_data, 0, sizeof(N_CH_each_data) * NUM_FILE_LOAD);

		for (int i = 0; i < NUM_FILE_LOAD; i++) {
			// 0. Open the file
			inFile_data[i].open(m_filelist_dir_Data[i]);

			// 1. Check the number of channels for each files
			if (inFile_data[i].is_open()) {
				CString CStr_temp;
				inFile_data[i].getline(Data_getline, 1000, '\n');
				CStr_temp = Data_getline;

				for (int j = 0; j < CStr_temp.GetLength(); j++) {
					if (CStr_temp[j] == ' ')
						N_CH_each_data[i]++;
				}
				N_CH_each_data[i]++;

				std::cout << N_CH_each_data[i] << std::endl;
			}
		}

		Finger_raw_stack_loaded = new std::vector<double>[N_CH_each_data[0]];
		MotionLabel_loaded = new std::vector<double>[N_CH_each_data[1]];
		sEMG_MAV_stack_loaded = new std::vector<double>[N_CH_each_data[2]];
		Wrist_raw_stack_loaded = new std::vector<double>[N_CH_each_data[3]];

		m_editStatusBar.SetWindowText(stat += "[USER] Data loading start... \r\n");
		m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());

		CString CStr_temp;
		CString data_Str;
		int _temp = 0;
		for (int i = 0; i < NUM_FILE_LOAD; i++) {
			while (!inFile_data[i].eof()) {
				inFile_data[i].getline(Data_getline, 10000, '\n');
				CStr_temp = Data_getline;

				int CH_idx = 0;
				data_Str = _T("");
				for (int j = 0; j < CStr_temp.GetLength(); j++) {
					if (CStr_temp[j] != ' ') {
						data_Str += CStr_temp[j];
						if (j == CStr_temp.GetLength() - 1) {
							double data_val = _wtof(data_Str);

							if (i == 0)
								Finger_raw_stack_loaded[CH_idx].push_back(data_val);
							else if (i == 1)
								MotionLabel_loaded[CH_idx].push_back(data_val);
							else if (i == 2)
								sEMG_MAV_stack_loaded[CH_idx].push_back(data_val);
							else if (i == 3)
								Wrist_raw_stack_loaded[CH_idx].push_back(data_val);
						}
					}
					else {
						double data_val = _wtof(data_Str);

						if (i == 0)
							Finger_raw_stack_loaded[CH_idx].push_back(data_val);
						else if (i == 1)
							MotionLabel_loaded[CH_idx].push_back(data_val);
						else if (i == 2)
							sEMG_MAV_stack_loaded[CH_idx].push_back(data_val);
						else if (i == 3)
							Wrist_raw_stack_loaded[CH_idx].push_back(data_val);
						CH_idx++;

						data_Str = _T("");
					}
				}
			}
		}
		m_editStatusBar.SetWindowText(stat += "[USER] Data loading end \r\n");
		m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());

		std::cout << Finger_raw_stack_loaded[0].size() << std::endl;
		std::cout << MotionLabel_loaded[0].size() << std::endl;
		std::cout << sEMG_MAV_stack_loaded[0].size() << std::endl;
		std::cout << Wrist_raw_stack_loaded[0].size() << std::endl;
	}
}

UINT CDAQVizDlg::MainThreadFunc(LPVOID IParam) {
	((CDAQVizDlg*)IParam)->MainStart();
	return TRUE;
}

void CDAQVizDlg::Set_loaded_Param_stack() {
	// In case of test session
	if (m_radioTrainingMode == 2) {
		// 0. Dynamic allocation & open the file
		inFile_parameters = new ifstream[NUM_FILE_LOAD_PARAMETER];
		N_CH_each_Param = new int[NUM_FILE_LOAD_PARAMETER];
		memset(N_CH_each_Param, 0, sizeof(N_CH_each_Param) * NUM_FILE_LOAD_PARAMETER);
	}
}

int CDAQVizDlg::MainStart() {
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	LARGE_INTEGER Counter_DAQ_Start, Counter_DAQ_End, Counter_DAQ_Frequency;
	LARGE_INTEGER Counter_RTGraph_Start, Counter_RTGraph_End, Counter_RTGraph_Frequency;
	QueryPerformanceFrequency(&Counter_DAQ_Frequency);
	QueryPerformanceFrequency(&Counter_RTGraph_Frequency);

	// Dynamic allocation
	Dynamic_Allocation();

	// Mutex Generation
	if (hMutex = CreateMutex(NULL, TRUE, L"nMutex"))
		m_editStatusBar.SetWindowText(stat += "[USER] Creating Mutex Success \r\n");
	else {
		error_text.Format(_T("%d"), GetLastError());
		m_editStatusBar.SetWindowText(stat += "[USER] Error: Creating Mutex Failed: ");
		m_editStatusBar.SetWindowText(stat += error_text);
		m_editStatusBar.SetWindowText(stat += "\r\n");
		goto error;
	}
	m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());

	// Shared Memory Generation
	if (hMemory = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Shared_Data), L"nMemory"))
		m_editStatusBar.SetWindowText(stat += "[USER] Creating Shared Memory Success \r\n");
	else {
		error_text.Format(_T("%d"), GetLastError());
		m_editStatusBar.SetWindowText(stat += "[USER] Error: Creating Shared Memory Failed: ");
		m_editStatusBar.SetWindowText(stat += error_text);
		m_editStatusBar.SetWindowText(stat += "\r\n");
		goto error;
	}
	m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());

	// Return Address of Shared Memory
	if (pShared_Data = (Shared_Data*)::MapViewOfFile(hMemory, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(Shared_Data)))
		m_editStatusBar.SetWindowText(stat += "[USER] Data address returned \r\n");
	else {
		error_text.Format(_T("%d"), GetLastError());
		m_editStatusBar.SetWindowText(stat += "[USER] Error:Address Return Failed: ");
		m_editStatusBar.SetWindowText(stat += error_text);
		m_editStatusBar.SetWindowText(stat += "\r\n");
		goto error;
	}
	m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());

	pShared_Data->bContinue = FALSE;
	pShared_Data->bSaveImmediate = b_SaveImmediate_Dlg;
	pShared_Data->bProcessEnd = FALSE;
	pShared_Data->bFirst = TRUE;
	pShared_Data->iNextOwner = THREAD_MAIN;
	pShared_Data->time = 0.0;
	pShared_Data->count = 0;

	// TwinCAT Process Execution (exe 파일 주소 확인)
	if (CreateProcess(_T("../TwinCAT/Debug/TwinCAT.exe"),
		_T("TwinCAT.exe nMemory nMutex"), NULL, NULL, FALSE, 0, NULL, NULL, &startupinfo, &processinfo))
		m_editStatusBar.SetWindowText(stat += "[USER] Creating Process Success \r\n");
	else {
		error_text.Format(_T("%d"), GetLastError());
		m_editStatusBar.SetWindowText(stat += "[USER] Error:Creating Process Failed: ");
		m_editStatusBar.SetWindowText(stat += error_text);
		m_editStatusBar.SetWindowText(stat += "\r\n");
		goto error;
	}
	m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());

	pShared_Data->iNextOwner = THREAD_TWINCAT;
	ReleaseMutex(hMutex);

	while (!pShared_Data->bProcessEnd) {
		WaitForSingleObject(hMutex, INFINITE);
		while (pShared_Data->iNextOwner != THREAD_MAIN) {
			ReleaseMutex(hMutex);
			WaitForSingleObject(hMutex, INFINITE);
		}

		if (pShared_Data->bFirst) {
			pShared_Data->bContinue = TRUE;
			pShared_Data->bFirst = FALSE;
			m_editStatusBar.SetWindowText(stat += "[USER] Start Timer \r\n");
			m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());

			AI_Flex->ReadOneStep(); // To remove the trash value

			pShared_Data->iNextOwner = THREAD_CALLBACK;
			ReleaseMutex(hMutex);
		}
		else if (pShared_Data->bContinue) {
			////////////////////////////////////////// DAQ //////////////////////////////////////////
			m_time = pShared_Data->time;
			m_count = pShared_Data->count;

			// Tic
			QueryPerformanceCounter(&Counter_DAQ_Start);

			// DAQ Body
			AI_sEMG->ReadOneStep();
			sEMG_raw_plot = AI_sEMG->Get_m_ReadValue();
			for (int i = 0; i < Num_sEMG_CH; i++) {
				sEMG_abs_plot[i] = abs(sEMG_raw_plot[i]);

				double sEMG_MAV_temp = 0.0;
				if (m_count <= WIN_SIZE) {
					sEMG_raw_window[i][m_count - 1] = sEMG_raw_plot[i];

					for (int j = 0; j < m_count; j++) {
						sEMG_MAV_temp += abs(sEMG_raw_window[i][j]);
					}
				}
				else {
					for (int j = 0; j < WIN_SIZE - 1; j++) {
						sEMG_raw_window[i][j] = sEMG_raw_window[i][j + 1];
					}
					sEMG_raw_window[i][WIN_SIZE - 1] = sEMG_raw_plot[i];

					for (int j = 0; j < WIN_SIZE; j++) {
						sEMG_MAV_temp += abs(sEMG_raw_window[i][j]);
					}
				}
				
				sEMG_MAV_temp /= WIN_SIZE;
				sEMG_MAV_plot[i] = sEMG_MAV_temp;
			}

			AI_Flex->ReadOneStep();
			Flex_data = AI_Flex->Get_m_ReadValue();

			if (CALI_START <= pShared_Data->time && pShared_Data->time <= CALI_END) {
				if (pShared_Data->count == CALI_START * 1000) {
					m_editStatusBar.SetWindowText(stat += "[USER] Calibration start \r\n");
					m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());
				}

				cali_count++;
				// Calibration DAQ - sEMG


				// Calibration DAQ - Flex
				for (int i = 0; i < Num_Flex_CH; i++)
					Flex_data_calib[i] += Flex_data[i];

				if (pShared_Data->count == CALI_END * 1000) {
					m_editStatusBar.SetWindowText(stat += "[USER] Calibration end \r\n");
					m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());

					for (int i = 0; i < Num_Flex_CH; i++)
						Flex_data_calib[i] /= (double)cali_count;
				}
			}
			else {
				for (int i = 0; i < Num_Flex_CH; i++) {
					Flex_data[i] -= Flex_data_calib[i];
				}
			}

			// Finger & wrist data
			for (int i = 0; i < Num_Flex_CH; i++) {
				if (0 <= i && i < Num_Finger_CH)
					Finger_data[i] = Flex_data[i];
				else if (Num_Finger_CH <= i && i < Num_Flex_CH)
					Wrist_data[i - Num_Finger_CH] = Flex_data[i];
			}

			// Flex slope data
			if (m_count == 1) {
				for (int i = 0; i < Num_Flex_CH; i++) {
					Flex_slope[i] = SigProc->FilteredDerivative(Flex_data[i],
															Flex_data[i], Flex_slope_prev[i]);
					Flex_data_prev[i] = Flex_data[i];
					Flex_slope_prev[i] = Flex_slope[i];
				}
			}
			else {
				for (int i = 0; i < Num_Flex_CH; i++) {
					Flex_slope[i] = SigProc->FilteredDerivative(Flex_data_prev[i],
															Flex_data[i], Flex_slope_prev[i]);
					Flex_data_prev[i] = Flex_data[i];
					Flex_slope_prev[i] = Flex_slope[i];
				}
			}

			// Finger & wrist slope
			for (int i = 0; i < Num_Flex_CH; i++) {
				if (0 <= i && i < Num_Finger_CH)
					Finger_slope[i] = Flex_slope[i];
				else if (Num_Finger_CH <= i && i < Num_Flex_CH)
					Wrist_slope[i - Num_Finger_CH] = Flex_slope[i];
			}

			// Motion classification
			Label_Est[0][0] = SigProc->MotionClassification_Flex(Finger_data, Wrist_data)[0]; // Finger motion label (w/ flex sensor)
			Label_Est[0][1] = SigProc->MotionClassification_Flex(Finger_data, Wrist_data)[1]; // Wrist F/E label (w/ flex sensor)
			Label_Est[0][2] = SigProc->MotionClassification_Flex(Finger_data, Wrist_data)[2]; // Wrist R/U Label (w/ flex sensor)
			Label_Est[1][0] = 0; // Finger motion estimation (Classifier necessary)
			Label_Est[1][1] = 0; // Wrist F/E estimation (Classifier necessary)
			Label_Est[1][2] = 0; // Wrist R/U estimation (Classifier necessary)

			// Ball control
			if (Label_Est[0][0] == LABEL_POWER_GRIP)
				Rad_ball -= RAD_STEP_SIZE;
			else if (Label_Est[0][0] == LABEL_HAND_OPEN)
				Rad_ball += RAD_STEP_SIZE;

			if (Label_Est[0][1] == LABEL_WRIST_FLEXION)
				X_pos_ball -= X_POS_STEP_SIZE;
			else if (Label_Est[0][1] == LABEL_WRIST_EXTENSION)
				X_pos_ball += X_POS_STEP_SIZE;

			if (Label_Est[0][2] == LABEL_WRIST_RADIAL)
				Y_pos_ball += Y_POS_STEP_SIZE;
			else if (Label_Est[0][2] == LABEL_WRIST_ULNAR)
				Y_pos_ball -= Y_POS_STEP_SIZE;

			if (X_pos_ball <= X_POS_MIN)
				X_pos_ball = X_POS_MIN;
			else if (X_pos_ball >= X_POS_MAX)
				X_pos_ball = X_POS_MAX;

			if (Y_pos_ball <= Y_POS_MIN)
				Y_pos_ball = Y_POS_MIN;
			else if (Y_pos_ball >= Y_POS_MAX)
				Y_pos_ball = Y_POS_MAX;

			if (Rad_ball <= RAD_MIN)
				Rad_ball = RAD_MIN;
			else if (Rad_ball >= RAD_MAX)
				Rad_ball = RAD_MAX;

			// Toc
			QueryPerformanceCounter(&Counter_DAQ_End);
			Time_DAQ_elapse = (double)(Counter_DAQ_End.QuadPart - Counter_DAQ_Start.QuadPart)
				/ (double)Counter_DAQ_Frequency.QuadPart * 1000; // ms scale

			//////////////////////////////////////// RTGraph ////////////////////////////////////////
			// Tic
			QueryPerformanceCounter(&Counter_RTGraph_Start);

			// sEMG data assignment
			for (int i = 0; i < Num_sEMG_CH; i++) {
				// sEMG_raw_plot[i] = abs(sin(2 * PI * 0.5 * (i + 1) * m_time));
				sEMG_abs_plot[i] = abs(sin(2 * PI * 0.5 * (i + 1) * m_time));
				sEMG_MAV_plot[i] = abs(sin(2 * PI * 0.5 * (i + 1) * m_time));
			}

			// Polygon
			p_ChildDlg_KSJ->Get_OpenGLPointer()->Set_sEMG_data(sEMG_MAV_plot);

			if (pShared_Data->count % N_GRAPH == 0) {
				p_ChildDlg_KSJ->Plot_graph(sEMG_raw_plot, p_ChildDlg_KSJ->Get_rtGraph_sEMG_MAV()[0]);
			}
			else if (pShared_Data->count % N_GRAPH == 1) {
				p_ChildDlg_KSJ->Plot_graph(sEMG_MAV_plot, p_ChildDlg_KSJ->Get_rtGraph_sEMG_MAV()[1]);
			}
			else if (pShared_Data->count % N_GRAPH == 2) {
				p_ChildDlg_KSJ->Plot_graph(sEMG_MAV_plot, p_ChildDlg_KSJ->Get_rtGraph_sEMG_MAV()[2]);
			}
			else if (pShared_Data->count % N_GRAPH == 3) {
				p_ChildDlg_KSJ->Plot_graph(Finger_data, p_ChildDlg_KSJ->Get_rtGraph_Finger()[0]);
			}
			else if (pShared_Data->count % N_GRAPH == 4) {
				p_ChildDlg_KSJ->Plot_graph(Finger_slope, p_ChildDlg_KSJ->Get_rtGraph_Finger_slope()[0]);
			}
			else if (pShared_Data->count % N_GRAPH == 5) {
				p_ChildDlg_KSJ->Plot_graph(Wrist_data, p_ChildDlg_KSJ->Get_rtGraph_Wrist()[0]);
			}
			else if (pShared_Data->count % N_GRAPH == 6) {
				p_ChildDlg_KSJ->Plot_graph(Wrist_slope, p_ChildDlg_KSJ->Get_rtGraph_Wrist_slope()[0]);
			}
			else if (pShared_Data->count % N_GRAPH == 7) {
				p_ChildDlg_KSJ->Plot_graph(Label_Est[0], p_ChildDlg_KSJ->Get_rtGraph_Label_Est()[0]);
			}

			// Toc
			QueryPerformanceCounter(&Counter_RTGraph_End);
			Time_RTGraph_elapse = (double)(Counter_RTGraph_End.QuadPart - Counter_RTGraph_Start.QuadPart)
														/ (double)Counter_RTGraph_Frequency.QuadPart * 1000; // ms scale

			// Stack the data
			StackData(m_time, Time_DAQ_elapse, Time_RTGraph_elapse,
					sEMG_raw_plot, sEMG_abs_plot, sEMG_MAV_plot,
					Finger_data, Finger_slope, Wrist_data, Wrist_slope,
					Label_Est[0], Label_Est[1],
					X_pos_ball, Y_pos_ball, Rad_ball);

			pShared_Data->iNextOwner = THREAD_CALLBACK;
			ReleaseMutex(hMutex);
		}
	}

	pShared_Data->iNextOwner = THREAD_TWINCAT;
	ReleaseMutex(hMutex);

	WaitForSingleObject(hMutex, INFINITE);
	while (pShared_Data->iNextOwner != THREAD_MAIN) {
		ReleaseMutex(hMutex);
		WaitForSingleObject(hMutex, INFINITE);
	}

	Set_MFC_Control_Availability(FALSE);

	// Save the data
	SaveData(SaveFolderPath);

	m_btnSwitch.SetWindowText(_T("End"));
	m_btnSwitch.EnableWindow(FALSE);

error:
	CloseHandle(hMutex);
	CloseHandle(hMemory);
	m_editStatusBar.SetWindowText(stat += "[USER] System Closed");
	m_editStatusBar.SetWindowText(stat += "\r\n");
	m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());
	return 0;
}

void CDAQVizDlg::RadioCtrl(UINT ID) {
	UpdateData(TRUE);

	if (IDC_RADIO_UNSUPERVISED <= ID && ID <= IDC_RADIO_TEST) {
		if (TEST_FLAG) {
			CString radio_val;
			radio_val.Format(_T("%d"), m_radioTrainingMode);
			GetDlgItem(IDC_EDIT_TEST)->SetWindowText(radio_val);
		}

		CString temp = SaveFolderPath_Main + SaveFolderName;
		switch (m_radioTrainingMode) {
		case 0:
			if (isParameterLoaded) {
				isParameterLoaded = FALSE;
				m_editParameterLoadName.SetWindowText(_T("Select files"));
				m_editStatusBar.SetWindowText(stat += "[USER] Parameter unload success \r\n");
				m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());
			}

			m_btnParameterLoad.EnableWindow(FALSE);
			if (m_radioStreamingMode == 0) {
				temp += _T("_Unsupervised_Online");
				m_btnSwitch.EnableWindow(TRUE);
			}
			else if (m_radioStreamingMode == 1) {
				temp += _T("_Unsupervised_Offline");
				if (isDataLoaded)
					m_btnSwitch.EnableWindow(TRUE);
				else
					m_btnSwitch.EnableWindow(FALSE);
			}
			SaveFolderPath = temp;
			m_editSaveFolderPath.SetWindowText(SaveFolderPath);

			break;
		case 1:
			if (isParameterLoaded) {
				isParameterLoaded = FALSE;
				m_editParameterLoadName.SetWindowText(_T("Select files"));
				m_editStatusBar.SetWindowText(stat += "[USER] Parameter unload success \r\n");
				m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());
			}

			m_btnParameterLoad.EnableWindow(FALSE);
			if (m_radioStreamingMode == 0) {
				temp += _T("_Training_Online");
				m_btnSwitch.EnableWindow(TRUE);
			}
			else if (m_radioStreamingMode == 1) {
				temp += _T("_Training_Offline");
				if (isDataLoaded)
					m_btnSwitch.EnableWindow(TRUE);
				else
					m_btnSwitch.EnableWindow(FALSE);
			}
			SaveFolderPath = temp;
			m_editSaveFolderPath.SetWindowText(SaveFolderPath);

			break;
		case 2:
			isParameterLoaded = FALSE;
			m_btnParameterLoad.EnableWindow(TRUE);
			if (m_radioStreamingMode == 0) {
				temp += _T("_Test_Online");
			}
			else if (m_radioStreamingMode == 1) {
				temp += _T("_Test_Offline");
			}
			SaveFolderPath = temp;
			m_editSaveFolderPath.SetWindowText(SaveFolderPath);

			m_btnSwitch.EnableWindow(FALSE);

			break;
		}
	}
	else if (IDC_RADIO_DATA_STREAMING_RT <= ID && ID <= IDC_RADIO_DATA_STREAMING_LOAD) {
		if (TEST_FLAG) {
			CString radio_val;
			radio_val.Format(_T("%d"), m_radioStreamingMode);
			GetDlgItem(IDC_EDIT_TEST)->SetWindowText(radio_val);
		}

		CString temp = SaveFolderPath_Main + SaveFolderName;
		switch (m_radioStreamingMode) {
		case 0:
			if (isDataLoaded) {
				isDataLoaded = FALSE;
				m_editLoadName.SetWindowText(_T("Select files"));
				m_editStatusBar.SetWindowText(stat += "[USER] Data unload success \r\n");
				m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());
			}

			m_btnLoad.EnableWindow(FALSE);
			if (m_radioTrainingMode == 0) {
				temp += _T("_Unsupervised_Online");
				m_btnSwitch.EnableWindow(TRUE);
			}
			else if (m_radioTrainingMode == 1) {
				temp += _T("_Training_Online");
				m_btnSwitch.EnableWindow(TRUE);
			}
			else if (m_radioTrainingMode == 2) {
				temp += _T("_Test_Online");
				if (isParameterLoaded)
					m_btnSwitch.EnableWindow(TRUE);
				else
					m_btnSwitch.EnableWindow(FALSE);
			}
			SaveFolderPath = temp;
			m_editSaveFolderPath.SetWindowText(SaveFolderPath);

			break;
		case 1:
			isDataLoaded = FALSE;
			m_btnLoad.EnableWindow(TRUE);
			if (m_radioTrainingMode == 0)
				temp += _T("_Unsupervised_Offline");
			else if (m_radioTrainingMode == 1)
				temp += _T("_Training_Offline");
			else if (m_radioTrainingMode == 2)
				temp += _T("_Test_Offline");
			m_btnSwitch.EnableWindow(FALSE);

			SaveFolderPath = temp;
			m_editSaveFolderPath.SetWindowText(SaveFolderPath);

			break;
		}
	}
	else if (IDC_RADIO_SAVE_IMMEDIATE <= ID && ID <= IDC_RADIO_STOP_AND_RUN_STACK_OFF) {
		if (TEST_FLAG) {
			CString radio_val;
			radio_val.Format(_T("%d"), m_radioSaveMode);
			GetDlgItem(IDC_EDIT_TEST)->SetWindowText(radio_val);
		}
		switch (m_radioSaveMode) {
		case 0:
			if (TimerStarted) {
				g_csExitThread.Lock();
				pShared_Data->bSaveImmediate = TRUE;
				g_csExitThread.Unlock();
			}
			else {
				b_SaveImmediate_Dlg = TRUE;
			}
			break;
		case 1:
		case 2:
			if (TimerStarted) {
				g_csExitThread.Lock();
				pShared_Data->bSaveImmediate = FALSE;
				g_csExitThread.Unlock();
			}
			else {
				b_SaveImmediate_Dlg = FALSE;
			}
			break;
		}

	}
	else if (IDC_RADIO_DEVICE_DELSYS <= ID && ID <= IDC_RADIO_DEVICE_FRANKFURT) {
		if (TEST_FLAG) {
			CString radio_val;
			radio_val.Format(_T("%d"), m_radiosEMGDAQDev);
			GetDlgItem(IDC_EDIT_TEST)->SetWindowText(radio_val);
		}
		CString temp;
		switch (m_radiosEMGDAQDev) {
		case 0:
			Num_sEMG_CH = DELSYS_CH_MAX;
			break;
		case 1:
			Num_sEMG_CH = FRANKFURT_CH_MAX;
			break;
		}
		temp.Format(_T("%d"), Num_sEMG_CH);
		m_editNumsEMGCH.SetWindowText(temp);
	}
	else if (IDC_RADIO_USE_FINGER_FLEX_YES <= ID && ID <= IDC_RADIO_USE_FINGER_FLEX_NO) {
		if (TEST_FLAG) {
			CString radio_val;
			radio_val.Format(_T("%d"), m_radioUseFingerFlex);
			GetDlgItem(IDC_EDIT_TEST)->SetWindowText(radio_val);
		}
		CString temp;
		switch (m_radioUseFingerFlex) {
		case 0:
			Num_Finger_CH = FINGER_CH_MAX;
			Num_Flex_CH = Num_Finger_CH + Num_Wrist_CH;
			m_editNumFingerFlexCH.EnableWindow(TRUE);
			temp.Format(_T("%d"), Num_Finger_CH);
			break;
		case 1:
			Num_Finger_CH = FINGER_CH_MAX;
			Num_Flex_CH = Num_Finger_CH + Num_Wrist_CH;
			m_editNumFingerFlexCH.EnableWindow(FALSE);
			temp.Format(_T("%d"), 0);
			break;
		}
		m_editNumFingerFlexCH.SetWindowText(temp);
	}
	else if (IDC_RADIO_USE_WRIST_FLEX_YES <= ID && ID <= IDC_RADIO_USE_WRIST_FLEX_NO) {
		if (TEST_FLAG) {
			CString radio_val;
			radio_val.Format(_T("%d"), m_radioUseWristFlex);
			GetDlgItem(IDC_EDIT_TEST)->SetWindowText(radio_val);
		}
		CString temp;
		switch (m_radioUseWristFlex) {
		case 0:
			Num_Wrist_CH = WRIST_CH_MAX;
			Num_Flex_CH = Num_Finger_CH + Num_Wrist_CH;
			m_editNumWristFlexCH.EnableWindow(TRUE);
			temp.Format(_T("%d"), Num_Wrist_CH);
			break;
		case 1:
			Num_Wrist_CH = WRIST_CH_MAX;
			Num_Flex_CH = Num_Finger_CH + Num_Wrist_CH;
			m_editNumWristFlexCH.EnableWindow(FALSE);
			temp.Format(_T("%d"), 0);
			break;
		}
		m_editNumWristFlexCH.SetWindowText(temp);
	}
}

void CDAQVizDlg::OnTimer(UINT_PTR nIDEvent) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialogEx::OnTimer(nIDEvent);

	CString m_time_str;
	switch (nIDEvent) {
		case TIMER_EDIT:
			m_time_str.Format(_T("%.3f"), m_time);
			m_editControlTime.SetWindowText(m_time_str);

			break;
	}
}

void CDAQVizDlg::OnDestroy() {
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Dynamic_Free();
	KillTimer(TIMER_EDIT);
}

void CDAQVizDlg::Set_StartIdx(int _idx) {
	CString temp;
	temp.Format(_T("%d"), _idx);
	m_editStartIdx.SetWindowText(temp);
}

void CDAQVizDlg::Set_EndIdx(int _idx) {
	CString temp;
	temp.Format(_T("%d"), _idx);
	m_editEndIdx.SetWindowText(temp);
}

void CDAQVizDlg::Initialize_StartIdx() {
	m_editStartIdx.SetWindowText(_T("Not selected"));
}

void CDAQVizDlg::Initialize_EndIdx() {
	m_editEndIdx.SetWindowText(_T("Not selected"));
}

double CDAQVizDlg::Get_m_time() {
	return m_time;
}

UINT CDAQVizDlg::Get_m_count() {
	return m_count;
}

bool CDAQVizDlg::Get_TimerStarted() {
	return TimerStarted;
}

const std::vector<double>* CDAQVizDlg::Get_sEMG_raw_stack() {
	return sEMG_raw_stack;
}

const std::vector<double>* CDAQVizDlg::Get_sEMG_abs_stack() {
	return sEMG_abs_stack;
}

const std::vector<double>* CDAQVizDlg::Get_sEMG_MAV_stack() {
	return sEMG_MAV_stack;
}

const std::vector<double>* CDAQVizDlg::Get_Finger_raw_stack() {
	return Finger_raw_stack;
}

const std::vector<double>* CDAQVizDlg::Get_Finger_slope_stack() {
	return Finger_slope_stack;
}

const std::vector<double>* CDAQVizDlg::Get_Wrist_raw_stack() {
	return Wrist_raw_stack;
}

const std::vector<double>* CDAQVizDlg::Get_Wrist_slope_stack() {
	return Wrist_slope_stack;
}

const std::vector<double>* CDAQVizDlg::Get_MotionLabel_stack() {
	return MotionLabel;
}

const std::vector<double>* CDAQVizDlg::Get_MotionEstimation_stack() {
	return MotionEstimation;
}

const std::vector<double>* CDAQVizDlg::Get_X_pos_ball_stack() {
	return X_pos_ball_stack;
}

const std::vector<double>* CDAQVizDlg::Get_Y_pos_ball_stack() {
	return Y_pos_ball_stack;
}

const std::vector<double>* CDAQVizDlg::Get_Rad_ball_stack() {
	return Rad_ball_stack;
}

void CDAQVizDlg::Set_MFC_Control_Availability(bool _isAvailable) {
	if (!_isAvailable) {
		m_comboSelectDlg.EnableWindow(_isAvailable);

		GetDlgItem(IDC_RADIO_UNSUPERVISED)->EnableWindow(_isAvailable);
		GetDlgItem(IDC_RADIO_TRAINING)->EnableWindow(_isAvailable);
		GetDlgItem(IDC_RADIO_TEST)->EnableWindow(_isAvailable);

		GetDlgItem(IDC_RADIO_DATA_STREAMING_RT)->EnableWindow(_isAvailable);
		GetDlgItem(IDC_RADIO_DATA_STREAMING_LOAD)->EnableWindow(_isAvailable);
		GetDlgItem(IDC_BTN_LOAD)->EnableWindow(_isAvailable);

		GetDlgItem(IDC_RADIO_DEVICE_DELSYS)->EnableWindow(_isAvailable);
		GetDlgItem(IDC_RADIO_DEVICE_FRANKFURT)->EnableWindow(_isAvailable);

		GetDlgItem(IDC_RADIO_USE_WRIST_FLEX_YES)->EnableWindow(_isAvailable);
		GetDlgItem(IDC_RADIO_USE_WRIST_FLEX_NO)->EnableWindow(_isAvailable);

		GetDlgItem(IDC_RADIO_USE_FINGER_FLEX_YES)->EnableWindow(_isAvailable);
		GetDlgItem(IDC_RADIO_USE_FINGER_FLEX_NO)->EnableWindow(_isAvailable);

		m_editNumsEMGCH.EnableWindow(_isAvailable);
		m_editNumFingerFlexCH.EnableWindow(_isAvailable);
		m_editNumWristFlexCH.EnableWindow(_isAvailable);
	}

	GetDlgItem(IDC_RADIO_SAVE_IMMEDIATE)->EnableWindow(_isAvailable);
	GetDlgItem(IDC_RADIO_STOP_AND_RUN_STACK_ON)->EnableWindow(_isAvailable);
	GetDlgItem(IDC_RADIO_STOP_AND_RUN_STACK_OFF)->EnableWindow(_isAvailable);
}

void CDAQVizDlg::StackData (double _m_time,
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
							double _Rad) {
	Time_stack.push_back(_m_time);
	Time_DAQ_elapse_stack.push_back(_Time_DAQ_elapse);
	Time_RTGraph_elapse_stack.push_back(_Time_RTGraph_elapse);

	for (int i = 0; i < Num_sEMG_CH; i++) {
		sEMG_raw_stack[i].push_back(_sEMG_raw[i]);
		sEMG_abs_stack[i].push_back(_sEMG_abs[i]);
		sEMG_MAV_stack[i].push_back(_sEMG_MAV[i]);
	}

	for (int i = 0; i < Num_Finger_CH; i++) {
		Finger_raw_stack[i].push_back(_Finger_raw[i]);
		Finger_slope_stack[i].push_back(_Finger_slope[i]);
	}

	for (int i = 0; i < Num_Wrist_CH; i++) {
		Wrist_raw_stack[i].push_back(_Wrist_raw[i]);
		Wrist_slope_stack[i].push_back(_Wrist_slope[i]);
	}

	for (int i = 0; i < MOTION_DOF; i++) {
		MotionLabel[i].push_back(_MotionLabel_current[i]);
		MotionEstimation[i].push_back(_MotionEstimation_current[i]);
	}

	X_pos_ball_stack[0].push_back(_X_pos);
	Y_pos_ball_stack[0].push_back(_Y_pos);
	Rad_ball_stack[0].push_back(_Rad);
}

void CDAQVizDlg::SaveData(CString SaveFolderName) {
	////////////////////////////////////////////////////////////////////////////////////////
	m_editStatusBar.SetWindowText(stat += "[USER] Save Start!");
	m_editStatusBar.SetWindowText(stat += "\r\n");
	m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());

	////////////////////////////////////// Save code ////////////////////////////////////////
	CreateDirectory(SaveFolderName, NULL);

	CString prefix = SaveFolderName;
	prefix += "/";

	f_time.open(prefix + _T("Time.txt"));
	f_time_elapsed_DAQ.open(prefix + _T("Time_elapsed_DAQ.txt"));
	f_time_elapsed_RTGraph.open(prefix + _T("Time_elapsed_RTGraph.txt"));

	f_sEMG_raw.open(prefix + _T("sEMG_raw.txt"));
	f_sEMG_abs.open(prefix + _T("sEMG_abs.txt"));
	f_sEMG_MAV.open(prefix + _T("sEMG_MAV.txt"));

	f_Finger_raw.open(prefix + _T("Finger_flex_raw.txt"));
	f_Finger_slope.open(prefix + _T("Finger_flex_slope.txt"));
	f_Wrist_raw.open(prefix + _T("Wrist_flex_raw.txt"));
	f_Wrist_slope.open(prefix + _T("Wrist_flex_slope.txt"));

	f_MotionLabel.open(prefix + _T("Motion_label.txt"));
	f_MotionEstimation.open(prefix + _T("Motion_estimation.txt"));

	f_X_pos_ball.open(prefix + _T("X_pos_ball.txt"));
	f_Y_pos_ball.open(prefix + _T("Y_pos_ball.txt"));
	f_Rad_ball.open(prefix + _T("Rad_ball.txt"));

	m_editStatusBar.SetWindowText(stat += _T("[USER] Save Start \r\n"));
	m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());

	m_editStatusBar.SetWindowText(stat += _T("[USER] Saving... \r\n"));
	m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());

	// Time Data Size Log
	ofstream size_log;
	size_log.open(prefix + _T("size_log.txt"));

	size_log << "The length of 'Time_stack' : " << Time_stack.size() << endl;
	size_log << "The length of 'Time_DAQ_elapse_stack' : " << Time_DAQ_elapse_stack.size() << endl;
	size_log << "The length of 'Time_RTGraph_elapse_stack' : " << Time_RTGraph_elapse_stack.size() << endl;
	size_log << endl;

	size_log << "The length of 'sEMG_raw' : " << sEMG_raw_stack[0].size() << endl;
	size_log << "The length of 'sEMG_abs' : " << sEMG_abs_stack[0].size() << endl;
	size_log << "The length of 'sEMG_MAV' : " << sEMG_MAV_stack[0].size() << endl;
	size_log << endl;

	size_log << "The length of 'Finger_raw' : " << Finger_raw_stack[0].size() << endl;
	size_log << "The length of 'Finger_slope' : " << Finger_slope_stack[0].size() << endl;
	size_log << "The length of 'Wrist_raw' : " << Wrist_raw_stack[0].size() << endl;
	size_log << "The length of 'Wrist_slope' : " << Wrist_slope_stack[0].size() << endl;
	size_log << endl;

	size_log << "The length of 'MotionLabel' : " << MotionLabel[0].size() << endl;
	size_log << "The length of 'MotionEstimation' : " << MotionEstimation[0].size() << endl;
	size_log << endl;

	size_log << "The length of 'X_pos_ball_stack' : " << X_pos_ball_stack[0].size() << endl;
	size_log << "The length of 'Y_pos_ball_stack' : " << Y_pos_ball_stack[0].size() << endl;
	size_log << "The length of 'Rad_ball_stack' : " << Rad_ball_stack[0].size() << endl;
	size_log << endl;

	// Saving...
	for (int i = 0; i < (int)Time_stack.size(); i++) {
		f_time << Time_stack[i] << endl;
		f_time_elapsed_DAQ << Time_DAQ_elapse_stack[i] << endl;
		f_time_elapsed_RTGraph << Time_RTGraph_elapse_stack[i] << endl;

		for (int j = 0; j < Num_sEMG_CH; j++) {
			f_sEMG_raw << sEMG_raw_stack[j][i];
			f_sEMG_abs << sEMG_abs_stack[j][i];
			f_sEMG_MAV << sEMG_MAV_stack[j][i];
			if (j != Num_sEMG_CH - 1) {
				f_sEMG_raw << " ";
				f_sEMG_abs << " ";
				f_sEMG_MAV << " ";
			}
			else {
				f_sEMG_raw << endl;
				f_sEMG_abs << endl;
				f_sEMG_MAV << endl;
			}
		}

		for (int j = 0; j < Num_Finger_CH; j++) {
			f_Finger_raw << Finger_raw_stack[j][i];
			f_Finger_slope << Finger_slope_stack[j][i];
			if (j != Num_Finger_CH - 1) {
				f_Finger_raw << " ";
				f_Finger_slope << " ";
			}
			else {
				f_Finger_raw << endl;
				f_Finger_slope << endl;
			}
		}

		for (int j = 0; j < Num_Wrist_CH; j++) {
			f_Wrist_raw << Wrist_raw_stack[j][i];
			f_Wrist_slope << Wrist_slope_stack[j][i];
			if (j != Num_Wrist_CH - 1) {
				f_Wrist_raw << " ";
				f_Wrist_slope << " ";
			}
			else {
				f_Wrist_raw << endl;
				f_Wrist_slope << endl;
			}
		}
		
		for (int j = 0; j < MOTION_DOF; j++) {
			f_MotionLabel << MotionLabel[j][i];
			f_MotionEstimation << MotionEstimation[j][i];
			if (j != MOTION_DOF - 1) {
				f_MotionLabel << " ";
				f_MotionEstimation << " ";
			}
			else {
				f_MotionLabel << endl;
				f_MotionEstimation << endl;
			}
		}

		f_X_pos_ball << X_pos_ball_stack[0][i] << endl;
		f_Y_pos_ball << Y_pos_ball_stack[0][i] << endl;
		f_Rad_ball << Rad_ball_stack[0][i] << endl;
	}

	SaveParameters(SaveFolderName);
	if (m_radioTrainingMode == 1) // Corresponding to training mode
		SaveModel(SaveFolderName);

	////////////////////////////////////////////////////////////////////////////////////////
	m_editStatusBar.SetWindowText(stat += "[USER] Save End!");
	m_editStatusBar.SetWindowText(stat += "\r\n");
	m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());
}

void CDAQVizDlg::SaveParameters(CString SaveFolderName) {
	CString prefix = SaveFolderName;
	prefix += "/";

	f_parameters.open(prefix + _T("Parameters.txt"));

	f_parameters << "Sensor calibration start time : " << CALI_START << endl;
	f_parameters << "Sensor calibration end time : " << CALI_END << endl;
	f_parameters << endl;

	f_parameters << "MAV window size : " << WIN_SIZE << endl;
	f_parameters << endl;

	f_parameters << "The number of sEMG channels : " << Num_sEMG_CH <<  endl;
	f_parameters << "The number of total flex channels : " << Num_Flex_CH << endl;
	f_parameters << "The number of finger flex channels : " << Num_Finger_CH << endl;
	f_parameters << "The number of wrist flex channels : " << Num_Wrist_CH << endl;
	f_parameters << endl;

	f_parameters << "Finger flex sensor threshold : " << FINGER_FLEX_THRES_ABS << endl;
	f_parameters << "Wrist Flexion / Extension flex sensor threshold : " << WRIST_FE_THRES_ABS << endl;
	f_parameters << "Wrist Radial / Ulnar flex sensor threshold : " << WRIST_RU_THRES_ABS << endl;
	f_parameters << endl;

	f_parameters << "The number of motions on interest : " << N_MOTIONS << endl;
	f_parameters << "Power grip label : " << LABEL_POWER_GRIP << endl;
	f_parameters << "Hand open label : " << LABEL_HAND_OPEN << endl;
	f_parameters << "Wrist flexion label : " << LABEL_WRIST_FLEXION << endl;
	f_parameters << "Wrist extension label : " << LABEL_WRIST_EXTENSION << endl;
	f_parameters << "Wrist radial deviation label : " << LABEL_WRIST_RADIAL << endl;
	f_parameters << "Wrist ulnar deviation label : " << LABEL_WRIST_ULNAR << endl;
	f_parameters << "Others : " << LABEL_OTHERS << endl;
	f_parameters << endl;

	f_parameters << "X_pos initial value : " << X_POS_INIT << endl;
	f_parameters << "X_pos minimal value : " << X_POS_MIN << endl;
	f_parameters << "X_pos maximal value : " << X_POS_MAX << endl;
	f_parameters << "X_pos step size : " << X_POS_STEP_SIZE << endl;
	f_parameters << endl;

	f_parameters << "Y_pos initial value : " << Y_POS_INIT << endl;
	f_parameters << "Y_pos minimal value : " << Y_POS_MIN << endl;
	f_parameters << "Y_pos maximal value : " << Y_POS_MAX << endl;
	f_parameters << "Y_pos step size : " << Y_POS_STEP_SIZE << endl;
	f_parameters << endl;

	f_parameters << "Rad initial value : " << RAD_INIT << endl;
	f_parameters << "Rad minimal value : " << RAD_MIN << endl;
	f_parameters << "Rad maximal value : " << RAD_MAX << endl;
	f_parameters << "Rad step size : " << RAD_STEP_SIZE << endl;
	f_parameters << endl;

	f_parameters << "Max. number of files: " << MAX_FILES << endl;
	f_parameters << "Max path size : " << MAX_PATH << endl;
	f_parameters << endl;
}

void CDAQVizDlg::SaveModel(CString SaveFolderName) {
	CString prefix = SaveFolderName;
	prefix += "/";

	f_model_sEMG_mean.open(prefix + _T("Model_sEMG_mean.txt"));
	f_model_sEMG_std.open(prefix + _T("Model_sEMG_std.txt"));
}

void CDAQVizDlg::OnEnChangeEditNumSemgCh() {
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;

	m_editNumsEMGCH.GetWindowText(temp);
	int Num_sEMG_CH = _ttoi(temp);
	if (m_radiosEMGDAQDev == 0) {
		if (Num_sEMG_CH > DELSYS_CH_MAX) {
			MessageBox(_T("DELSYS Trigno sEMG channel should be smaller than 16."),
						_T("Notice"), MB_OK | MB_ICONWARNING);
			temp.Format(_T("%d"), DELSYS_CH_MAX);
			m_editNumsEMGCH.SetWindowText(temp);
			Num_sEMG_CH = DELSYS_CH_MAX;
		}
		else if (Num_sEMG_CH < 1) {
			MessageBox(_T("sEMG channel should be larger than 1."),
						_T("Notice"), MB_OK | MB_ICONWARNING);
			temp.Format(_T("%d"), 1);
			m_editNumsEMGCH.SetWindowText(temp);
			Num_sEMG_CH = 1;
		}
		else {
			this->Num_sEMG_CH = Num_sEMG_CH;
		}
	}
	else if (m_radiosEMGDAQDev == 1) {
		if (Num_sEMG_CH > FRANKFURT_CH_MAX) {
			MessageBox(_T("LogonU sEMG channel should be smaller than 8."),
						_T("Notice"), MB_OK | MB_ICONWARNING);
			temp.Format(_T("%d"), FRANKFURT_CH_MAX);
			m_editNumsEMGCH.SetWindowText(temp);
			Num_sEMG_CH = FRANKFURT_CH_MAX;
		}
		else if (Num_sEMG_CH < 1) {
			MessageBox(_T("sEMG channel should be larger than 1."),
						_T("Notice"), MB_OK | MB_ICONWARNING);
			temp.Format(_T("%d"), 1);
			m_editNumsEMGCH.SetWindowText(temp);
			Num_sEMG_CH = 1;
		}
		else {
			this->Num_sEMG_CH = Num_sEMG_CH;
		}
	}
}

void CDAQVizDlg::OnEnChangeEditNumFlexCh() {
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;

	m_editNumFingerFlexCH.GetWindowText(temp);
	int Num_finger_CH = _ttoi(temp);
	if (m_radioUseFingerFlex == 0) {
		if (Num_finger_CH > FINGER_CH_MAX) {
			MessageBox(_T("Finger flex sensor channel should be smaller than 5."),
						_T("Notice"), MB_OK | MB_ICONWARNING);
			temp.Format(_T("%d"), FINGER_CH_MAX);
			m_editNumFingerFlexCH.SetWindowText(temp);
			Num_Finger_CH = FINGER_CH_MAX;
			Num_Flex_CH = Num_Finger_CH + Num_Wrist_CH;
		}
		else if (Num_finger_CH < 1) {
			MessageBox(_T("Finger flex sensor channel should be larger than 1."),
						_T("Notice"), MB_OK | MB_ICONWARNING);
			temp.Format(_T("%d"), 1);
			m_editNumFingerFlexCH.SetWindowText(temp);
			Num_Finger_CH = 1;
			Num_Flex_CH = Num_Finger_CH + Num_Wrist_CH;
		}
		else {
			Num_Finger_CH = Num_finger_CH;
			Num_Flex_CH = Num_Finger_CH + Num_Wrist_CH;
		}
	}
}

void CDAQVizDlg::OnEnChangeEditNumImuCh() {
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;

	m_editNumWristFlexCH.GetWindowText(temp);
	int Num_wrist_CH = _ttoi(temp);
	if (m_radioUseWristFlex == 0) {
		if (Num_wrist_CH > WRIST_CH_MAX) {
			MessageBox(_T("Wrist flex sensor channel should be smaller than 2."),
						_T("Notice"), MB_OK | MB_ICONWARNING);
			temp.Format(_T("%d"), WRIST_CH_MAX);
			m_editNumWristFlexCH.SetWindowText(temp);
			Num_Wrist_CH = WRIST_CH_MAX;
			Num_Flex_CH = Num_Finger_CH + Num_Wrist_CH;
		}
		else if (Num_wrist_CH < 1) {
			MessageBox(_T("Wrist flex sensor channel should be larger than 1."),
						_T("Notice"), MB_OK | MB_ICONWARNING);
			temp.Format(_T("%d"), 1);
			m_editNumWristFlexCH.SetWindowText(temp);
			Num_Wrist_CH = 1;
			Num_Flex_CH = Num_Finger_CH + Num_Wrist_CH;
		}
		else {
			Num_Wrist_CH = Num_wrist_CH;
			Num_Flex_CH = Num_Finger_CH + Num_Wrist_CH;
		}
	}
}

void CDAQVizDlg::OnBnClickedBtnParameterLoad() {
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog read_file(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, _T("파일 선택 (.txt) | *.txt"));
	CString temp;
	int nMaxFiles = MAX_FILES;
	int nBuffSize = nMaxFiles * MAX_PATH + 1;
	read_file.m_ofn.nMaxFile = nBuffSize; // To increase buffer size
	read_file.m_ofn.lpstrFile = temp.GetBuffer(nBuffSize);
	read_file.m_ofn.lpstrTitle = _T("Load할 파라미터 텍스트 파일을 선택하세요.");
	read_file.m_ofn.lpstrInitialDir = _T("E:/OneDrive - postech.ac.kr/연구/### 데이터/DAQViz data/");

	CString m_filename;
	CString m_filedir;

	m_filelist_dir_Param = new CString[NUM_FILE_LOAD_PARAMETER];
	m_filelist_name_Param = new CString[NUM_FILE_LOAD_PARAMETER];

	UINT m_filelist_idx = 0;
	if (read_file.DoModal() == IDOK) {
		POSITION pos = read_file.GetStartPosition();

		// Load the .txt files
		while (pos != NULL) {
			m_filedir = read_file.GetNextPathName(pos);
			m_filename = m_filedir;
			m_filename.Delete(0, m_filedir.ReverseFind('\\') + 1);

			m_filelist_dir_Param[m_filelist_idx] = m_filedir;
			m_filelist_name_Param[m_filelist_idx] = m_filename;
			m_filelist_idx++;
		}
	}

	// Check for loading wrong file
	for (int i = 0; i < NUM_FILE_LOAD_PARAMETER; i++) {
		if (m_filelist_name_Param[0] != _T("Model_sEMG_mean.txt")) {
			MessageBox(_T("First loading file should be 'Model_sEMG_mean.txt'."),
						_T("Notice"), MB_OK | MB_ICONWARNING);
			delete[] m_filelist_dir_Param;
			delete[] m_filelist_name_Param;
			return;
		}
		if (m_filelist_name_Param[1] != _T("Model_sEMG_std.txt")) {
			MessageBox(_T("Second loading file should be 'Model_sEMG_std.txt'."),
						_T("Notice"), MB_OK | MB_ICONWARNING);
			delete[] m_filelist_dir_Param;
			delete[] m_filelist_name_Param;
			return;
		}
	}

	m_editParameterLoadName.SetWindowText(_T("Files loaded"));
	m_editStatusBar.SetWindowText(stat += "[USER] Parameter load success \r\n");
	m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());

	// Enable switch
	isParameterLoaded = TRUE;
	if (m_radioStreamingMode == 0)
		m_btnSwitch.EnableWindow(TRUE);
	else if (m_radioStreamingMode == 1) {
		if (isDataLoaded)
			m_btnSwitch.EnableWindow(TRUE);
		else
			m_btnSwitch.EnableWindow(FALSE);
	}
}