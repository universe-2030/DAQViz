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
	m_radioUseFlexSensor(0), m_radioUseIMU(0) {
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

	DDX_Control(pDX, IDC_TEXT_USE_LOGONU_IMU, m_textUseIMU);
	DDX_Radio(pDX, IDC_RADIO_USE_IMU_LOGONU_YES, (int&)m_radioUseIMU);

	DDX_Control(pDX, IDC_TEXT_USE_FLEX_SENSOR, m_textUseFlexSensor);
	DDX_Radio(pDX, IDC_RADIO_USE_FLEX_SENSOR_YES, (int&)m_radioUseFlexSensor);

	DDX_Control(pDX, IDC_EDIT_STATUS_BAR, m_editStatusBar);

	DDX_Control(pDX, IDC_TEXT_CONTROL_TIME, m_textControlTime);
	DDX_Control(pDX, IDC_EDIT_CONTROL_TIME, m_editControlTime);
	DDX_Control(pDX, IDC_TEXT_TIME_START_IDX, m_textStartIdx);
	DDX_Control(pDX, IDC_TEXT_TIME_END_IDX, m_textEndIdx);
	DDX_Control(pDX, IDC_EDIT_TIME_START_IDX, m_editStartIdx);
	DDX_Control(pDX, IDC_EDIT_TIME_END_IDX, m_editEndIdx);
	DDX_Control(pDX, IDC_TEXT_NUM_SEMG_CH, m_textNumsEMGCH);
	DDX_Control(pDX, IDC_EDIT_NUM_SEMG_CH, m_editNumsEMGCH);
	DDX_Control(pDX, IDC_TEXT_NUM_FLEX_CH, m_textNumFlexCH);
	DDX_Control(pDX, IDC_TEXT_NUM_IMU_CH, m_textNumIMUCH);
	DDX_Control(pDX, IDC_EDIT_NUM_FLEX_CH, m_editNumFlexCH);
	DDX_Control(pDX, IDC_EDIT_NUM_IMU_CH, m_editNumIMUCH);
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
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_USE_IMU_LOGONU_YES, IDC_RADIO_USE_IMU_LOGONU_NO, RadioCtrl)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_USE_FLEX_SENSOR_YES, IDC_RADIO_USE_FLEX_SENSOR_NO, RadioCtrl)
	ON_BN_CLICKED(IDC_BTN_LOAD, &CDAQVizDlg::OnBnClickedBtnLoad)
	ON_CBN_SELCHANGE(IDC_COMBO_DLG_SELECT, &CDAQVizDlg::OnCbnSelchangeComboDlgSelect)
//	ON_WM_SIZING()
ON_WM_SIZE()
ON_WM_TIMER()
ON_WM_DESTROY()
ON_EN_CHANGE(IDC_EDIT_NUM_SEMG_CH, &CDAQVizDlg::OnEnChangeEditNumSemgCh)
ON_EN_CHANGE(IDC_EDIT_NUM_FLEX_CH, &CDAQVizDlg::OnEnChangeEditNumFlexCh)
ON_EN_CHANGE(IDC_EDIT_NUM_IMU_CH, &CDAQVizDlg::OnEnChangeEditNumImuCh)
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

	isModelParamLoaded = FALSE;
	isDataLoaded = FALSE;

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
	Set_Font(m_textUseIMU, 20, 8);
	Set_Font(m_textUseFlexSensor, 20, 8);
	Set_Font(m_textControlTime, 20, 8);
	Set_Font(m_editControlTime, 20, 8);
	Set_Font(m_textStartIdx, 20, 8);
	Set_Font(m_editStartIdx, 20, 8);
	Set_Font(m_textEndIdx, 20, 8);
	Set_Font(m_editEndIdx, 20, 8);
	Set_Font(m_textNumsEMGCH, 20, 8);
	Set_Font(m_editNumsEMGCH, 20, 8);
	Set_Font(m_textNumFlexCH, 20, 8);
	Set_Font(m_editNumFlexCH, 20, 8);
	Set_Font(m_textNumIMUCH, 20, 8);
	Set_Font(m_editNumIMUCH, 20, 8);

	m_comboSelectDlg.AddString(_T("1. Sejin Kim"));
	// m_comboSelectDlg.AddString(_T("2. Another users"));
	m_comboSelectDlg.SetCurSel(0);

	m_editParameterLoadName.SetWindowText(_T("Select file"));
	m_editLoadName.SetWindowText(_T("Select file"));
	m_editStartIdx.SetWindowText(_T("Not selected"));
	m_editEndIdx.SetWindowText(_T("Not selected"));

	CString temp;
	temp.Format(_T("%d"), DELSYS_CH_MAX);
	m_editNumsEMGCH.SetWindowTextW(temp);
	temp.Format(_T("%d"), FINGER_CH_MAX);
	m_editNumFlexCH.SetWindowTextW(temp);
	temp.Format(_T("%d"), WRIST_CH_MAX);
	m_editNumIMUCH.SetWindowTextW(temp);

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

		m_radioUseIMU = 1;
		CButton* pCheck = (CButton*)GetDlgItem(IDC_RADIO_USE_IMU_LOGONU_YES);
		pCheck->SetCheck(BST_UNCHECKED);
		pCheck = (CButton*)GetDlgItem(IDC_RADIO_USE_IMU_LOGONU_NO);
		pCheck->SetCheck(BST_CHECKED);
		GetDlgItem(IDC_RADIO_USE_IMU_LOGONU_YES)->EnableWindow(FALSE);

		CString temp;
		temp.Format(_T("%d"), 0);
		m_editNumIMUCH.SetWindowText(temp);
		m_editNumIMUCH.EnableWindow(FALSE);
	}
	else {
		isMATCHconnected = TRUE;
		m_radioUseIMU = 0;
		CButton* pCheck = (CButton*)GetDlgItem(IDC_RADIO_USE_IMU_LOGONU_YES);
		pCheck->SetCheck(BST_CHECKED);
	}
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

	sEMG_raw_stack = new std::vector<double>[Num_sEMG_CH];
	sEMG_abs_stack = new std::vector<double>[Num_sEMG_CH];
	sEMG_MAV_stack = new std::vector<double>[Num_sEMG_CH];
	
	Finger_raw_stack = new std::vector<double>[Num_Finger_CH];
	Finger_slope_stack = new std::vector<double>[Num_Finger_CH];
	Wrist_raw_stack = new std::vector<double>[Num_Wrist_CH];
	Wrist_slope_stack = new std::vector<double>[Num_Wrist_CH];

	MotionLabel = new std::vector<double>;
	MotionEstimation = new std::vector<double>;

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
	CFileDialog read_file(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, _T("파일 선택 (.txt) | *.txt"));
	read_file.m_ofn.lpstrTitle = _T("Load할 데이터 텍스트 파일을 선택하세요.");
	read_file.m_ofn.lpstrInitialDir = _T("../Loading_data");

	CString m_filename;
	CString m_filedir;

	if (read_file.DoModal() == IDOK) {
		m_filename = read_file.GetFileName();
		m_filedir = read_file.GetPathName();

		if (!isDataLoaded) {
			isDataLoaded = TRUE;
			inFile_data.open(m_filedir);
			m_editLoadName.SetWindowText(m_filename);
		}
		else {
			inFile_data.close();
			inFile_data.open(m_filedir);
			m_editLoadName.SetWindowText(m_filename);
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

UINT CDAQVizDlg::MainThreadFunc(LPVOID IParam) {
	((CDAQVizDlg*)IParam)->MainStart();
	return TRUE;
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
			AI_Flex->ReadOneStep();
			Flex_data = AI_Flex->Get_m_ReadValue();

			if (CALI_START <= pShared_Data->time && pShared_Data->time <= CALI_END) {
				if (pShared_Data->count == CALI_START * 1000) {
					m_editStatusBar.SetWindowText(stat += "[USER] Calibration start \r\n");
					m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());
				}

				// Calibration DAQ - sEMG


				// Calibration DAQ - Flex
				cali_count++;
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
			Label_Est[0][0] = SigProc->MotionClassification_Flex(Finger_data, Wrist_data)[0]; // Label
			Label_Est[0][1] = SigProc->MotionClassification_Flex(Finger_data, Wrist_data)[1]; // Label
			Label_Est[0][2] = SigProc->MotionClassification_Flex(Finger_data, Wrist_data)[2]; // Label
			Label_Est[1][0] = 0; // Estimation
			Label_Est[1][1] = 0; // Estimation
			Label_Est[1][2] = 0; // Estimation

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
				sEMG_raw_plot[i] = abs(sin(2 * PI * 0.5 * (i + 1) * m_time));
				sEMG_abs_plot[i] = abs(sin(2 * PI * 0.5 * (i + 1) * m_time));
				sEMG_MAV_plot[i] = abs(sin(2 * PI * 0.5 * (i + 1) * m_time));
			}

			// Polygon
			p_ChildDlg_KSJ->Get_OpenGLPointer()->Set_sEMG_data(sEMG_MAV_plot);

			if (pShared_Data->count % N_GRAPH == 0) {
				p_ChildDlg_KSJ->Plot_graph(sEMG_MAV_plot, p_ChildDlg_KSJ->Get_rtGraph_sEMG_MAV()[0]);
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
			StackData(sEMG_raw_plot, sEMG_abs_plot, sEMG_MAV_plot,
					Finger_data, Finger_slope, Wrist_data, Wrist_slope,
					Label_Est[0][0], Label_Est[1][0],
					X_pos_ball, Y_pos_ball, Rad_ball,
					Time_DAQ_elapse, Time_RTGraph_elapse);

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
			m_btnParameterLoad.EnableWindow(FALSE);

			temp += _T("_Unsupervised");
			SaveFolderPath = temp;
			m_editSaveFolderPath.SetWindowText(SaveFolderPath);

			break;
		case 1:
			m_btnParameterLoad.EnableWindow(FALSE);

			temp += _T("_Training");
			SaveFolderPath = temp;
			m_editSaveFolderPath.SetWindowText(SaveFolderPath);

			break;
		case 2:
			m_btnParameterLoad.EnableWindow(TRUE);

			temp += _T("_Test");
			SaveFolderPath = temp;
			m_editSaveFolderPath.SetWindowText(SaveFolderPath);

			break;
		}
	}
	else if (IDC_RADIO_DATA_STREAMING_RT <= ID && ID <= IDC_RADIO_DATA_STREAMING_LOAD) {
		if (TEST_FLAG) {
			CString radio_val;
			radio_val.Format(_T("%d"), m_radioStreamingMode);
			GetDlgItem(IDC_EDIT_TEST)->SetWindowText(radio_val);
		}
		switch (m_radioStreamingMode) {
		case 0:
			m_btnLoad.EnableWindow(FALSE);
			break;
		case 1:
			m_btnLoad.EnableWindow(TRUE);
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
	else if (IDC_RADIO_USE_FLEX_SENSOR_YES <= ID && ID <= IDC_RADIO_USE_FLEX_SENSOR_NO) {
		if (TEST_FLAG) {
			CString radio_val;
			radio_val.Format(_T("%d"), m_radioUseFlexSensor);
			GetDlgItem(IDC_EDIT_TEST)->SetWindowText(radio_val);
		}
		CString temp;
		switch (m_radioUseFlexSensor) {
		case 0:
			Num_Finger_CH = FINGER_CH_MAX;
			Num_Flex_CH = Num_Finger_CH + Num_Wrist_CH;
			m_editNumFlexCH.EnableWindow(TRUE);
			temp.Format(_T("%d"), Num_Finger_CH);
			break;
		case 1:
			Num_Finger_CH = FINGER_CH_MAX;
			Num_Flex_CH = Num_Finger_CH + Num_Wrist_CH;
			m_editNumFlexCH.EnableWindow(FALSE);
			temp.Format(_T("%d"), 0);
			break;
		}
		m_editNumFlexCH.SetWindowText(temp);
	}
	else if (IDC_RADIO_USE_IMU_LOGONU_YES <= ID && ID <= IDC_RADIO_USE_IMU_LOGONU_NO) {
		if (TEST_FLAG) {
			CString radio_val;
			radio_val.Format(_T("%d"), m_radioUseIMU);
			GetDlgItem(IDC_EDIT_TEST)->SetWindowText(radio_val);
		}
		CString temp;
		switch (m_radioUseIMU) {
		case 0:
			Num_Wrist_CH = WRIST_CH_MAX;
			Num_Flex_CH = Num_Finger_CH + Num_Wrist_CH;
			m_editNumIMUCH.EnableWindow(TRUE);
			temp.Format(_T("%d"), Num_Wrist_CH);
			break;
		case 1:
			Num_Wrist_CH = WRIST_CH_MAX;
			Num_Flex_CH = Num_Finger_CH + Num_Wrist_CH;
			m_editNumIMUCH.EnableWindow(FALSE);
			temp.Format(_T("%d"), 0);
			break;
		}
		m_editNumIMUCH.SetWindowText(temp);
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

		GetDlgItem(IDC_RADIO_USE_IMU_LOGONU_YES)->EnableWindow(_isAvailable);
		GetDlgItem(IDC_RADIO_USE_IMU_LOGONU_NO)->EnableWindow(_isAvailable);

		GetDlgItem(IDC_RADIO_USE_FLEX_SENSOR_YES)->EnableWindow(_isAvailable);
		GetDlgItem(IDC_RADIO_USE_FLEX_SENSOR_NO)->EnableWindow(_isAvailable);

		m_editNumsEMGCH.EnableWindow(_isAvailable);
		m_editNumFlexCH.EnableWindow(_isAvailable);
		m_editNumIMUCH.EnableWindow(_isAvailable);
	}

	GetDlgItem(IDC_RADIO_SAVE_IMMEDIATE)->EnableWindow(_isAvailable);
	GetDlgItem(IDC_RADIO_STOP_AND_RUN_STACK_ON)->EnableWindow(_isAvailable);
	GetDlgItem(IDC_RADIO_STOP_AND_RUN_STACK_OFF)->EnableWindow(_isAvailable);
}

void CDAQVizDlg::StackData (double* _sEMG_raw,
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
							double _Time_RTGraph_elapse) {
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

	MotionLabel[0].push_back(_MotionLabel_current);
	MotionEstimation[0].push_back(_MotionEstimation_current);

	X_pos_ball_stack[0].push_back(_X_pos);
	Y_pos_ball_stack[0].push_back(_Y_pos);
	Rad_ball_stack[0].push_back(_Rad);

	Time_DAQ_elapse_stack.push_back(_Time_DAQ_elapse);
	Time_RTGraph_elapse_stack.push_back(_Time_RTGraph_elapse);
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

	ofstream f_time, f_time_elapsed_DAQ, f_time_elapsed_RTGraph;
	ofstream f_sEMG_raw, f_sEMG_abs, f_sEMG_MAV;
	ofstream f_Flex_raw, f_Flex_LPF;
	ofstream f__raw, f_IMU_LPF;
	ofstream f_sEMG_pattern_num, f_sEMG_pattern_idx, f_sEMG_pattern_log_prob;
	ofstream f_model_parameters;
	

	////////////////////////////////////////////////////////////////////////////////////////
	m_editStatusBar.SetWindowText(stat += "[USER] Save End!");
	m_editStatusBar.SetWindowText(stat += "\r\n");
	m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());
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

	m_editNumFlexCH.GetWindowText(temp);
	int Num_finger_CH = _ttoi(temp);
	if (m_radioUseFlexSensor == 0) {
		if (Num_finger_CH > FINGER_CH_MAX) {
			MessageBox(_T("Flex sensor channel should be smaller than 5."),
						_T("Notice"), MB_OK | MB_ICONWARNING);
			temp.Format(_T("%d"), FINGER_CH_MAX);
			m_editNumFlexCH.SetWindowText(temp);
			Num_Finger_CH = FINGER_CH_MAX;
			Num_Flex_CH = Num_Finger_CH + Num_Wrist_CH;
		}
		else if (Num_finger_CH < 1) {
			MessageBox(_T("Flex sensor channel should be larger than 1."),
						_T("Notice"), MB_OK | MB_ICONWARNING);
			temp.Format(_T("%d"), 1);
			m_editNumFlexCH.SetWindowText(temp);
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

	m_editNumIMUCH.GetWindowText(temp);
	int Num_wrist_CH = _ttoi(temp);
	if (m_radioUseIMU == 0) {
		if (Num_wrist_CH > WRIST_CH_MAX) {
			MessageBox(_T("IMU sensor channel should be smaller than 2."),
						_T("Notice"), MB_OK | MB_ICONWARNING);
			temp.Format(_T("%d"), WRIST_CH_MAX);
			m_editNumIMUCH.SetWindowText(temp);
			Num_Wrist_CH = WRIST_CH_MAX;
			Num_Flex_CH = Num_Finger_CH + Num_Wrist_CH;
		}
		else if (Num_wrist_CH < 1) {
			MessageBox(_T("IMU sensor channel should be larger than 1."),
						_T("Notice"), MB_OK | MB_ICONWARNING);
			temp.Format(_T("%d"), 1);
			m_editNumIMUCH.SetWindowText(temp);
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
	CFileDialog read_file(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, _T("파일 선택 (.txt) | *.txt"));
	read_file.m_ofn.lpstrTitle = _T("Load할 파라미터 텍스트 파일을 선택하세요.");
	read_file.m_ofn.lpstrInitialDir = _T("../Loading_data");

	CString m_filename;
	CString m_filedir;

	if (read_file.DoModal() == IDOK) {
		m_filename = read_file.GetFileName();
		m_filedir = read_file.GetPathName();

		if (!isModelParamLoaded) {
			isModelParamLoaded = TRUE;
			inFile_model_param.open(m_filedir);
			m_editLoadName.SetWindowText(m_filename);
		}
		else {
			inFile_model_param.close();
			inFile_model_param.open(m_filedir);
			m_editLoadName.SetWindowText(m_filename);
		}
	}
}