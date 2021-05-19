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
	m_radioTrainingMode(0), m_radioStreamingMode(0),
	m_radioSaveMode(0), m_radiosEMGDAQDev(0),
	m_radioUseIMU(0), m_radioUseFlexSensor(0)
{
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
	DDX_Radio(pDX, IDC_RADIO_TRAINING_ONLINE, (int&)m_radioTrainingMode);

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
}

BEGIN_MESSAGE_MAP(CDAQVizDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SWITCH, &CDAQVizDlg::OnBnClickedBtnSwitch)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_TRAINING_ONLINE, IDC_RADIO_TRAINING_OFFLINE, RadioCtrl)
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
	Initialize_RTOS();

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
	sEMG_temp = new double[4];
	sEMG_temp_abs = new double[4];

	Flex_data_calib = new float64[N_FLEX];
	memset(Flex_data_calib, 0.0, 2 * sizeof(Flex_data_calib) * N_FLEX);

	sEMG_temp_16CH = new double[16];
	memset(sEMG_temp_16CH, 0.0, 2 * sizeof(sEMG_temp_16CH) * 16);
}

void CDAQVizDlg::Initialize_NI() {
	AI_sEMG = new NI_AI_sEMG("Dev1/ai0:15", 16);
	NI_AI_sEMG::InitializeNI();

	AI_Flex = new NI_AI_Flex("Dev2/ai0:4", N_FLEX);
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

	m_comboSelectDlg.AddString(_T("1. Sejin Kim"));
	m_comboSelectDlg.AddString(_T("2. Another users"));

	m_comboSelectDlg.SetCurSel(0);

	m_editLoadName.SetWindowText(_T("Select file"));

	m_editStartIdx.SetWindowText(_T("Not selected"));
	m_editEndIdx.SetWindowText(_T("Not selected"));

	if (m_radioStreamingMode == 0)
		m_btnLoad.EnableWindow(FALSE);
	else
		m_btnLoad.EnableWindow(TRUE);

	CRect rectofDialogArea;
	GetDlgItem(IDC_SLOT_CHILDDLG)->GetWindowRect(&rectofDialogArea);
	ScreenToClient(&rectofDialogArea);

	p_ChildDlg_KSJ = new DAQVizChildKSJ();
	p_ChildDlg_KSJ->Create(IDD_DAQVIZ_DIALOG_CHILD_KSJ, this);
	p_ChildDlg_KSJ->ShowWindow(SW_SHOW);
	p_ChildDlg_KSJ->MoveWindow(rectofDialogArea);

	p_BallControl = new BallControl();
	p_BallControl->Create(IDD_DAQVIZ_DIALOG_BALL_CONTROL, this);
	p_BallControl->ShowWindow(SW_SHOW);

	SetWindowPos(NULL, -1920, -1080, 0, 0, SWP_NOSIZE);
}

void CDAQVizDlg::Initialize_SaveFolder() {
	SaveFolderPath = "D:/Training-free algorithm/";

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

	SaveFolderPath += SaveFolderName;
	m_editSaveFolderPath.SetWindowText(SaveFolderPath);
}

void CDAQVizDlg::Initialize_LogonU() {

}

void CDAQVizDlg::Initialize_RTOS() {

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
		AfxBeginThread(MainThreadFunc, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

		m_comboSelectDlg.EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_TRAINING_ONLINE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TRAINING_OFFLINE)->EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_DATA_STREAMING_RT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_DATA_STREAMING_LOAD)->EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_SAVE_IMMEDIATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STOP_AND_RUN_STACK_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STOP_AND_RUN_STACK_OFF)->EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_DEVICE_DELSYS)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_DEVICE_FRANKFURT)->EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_USE_IMU_LOGONU_YES)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_USE_IMU_LOGONU_NO)->EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_USE_FLEX_SENSOR_YES)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_USE_FLEX_SENSOR_NO)->EnableWindow(FALSE);

		if (m_radioStreamingMode == 1) {
			GetDlgItem(IDC_BTN_LOAD)->EnableWindow(FALSE);
		}

		m_btnSwitch.SetWindowText(_T("Stop"));
		m_flag_Switch = TRUE;
	}
	else { // Turned ON -> OFF
		bProcessEnd = TRUE;

		m_comboSelectDlg.EnableWindow(TRUE);

		GetDlgItem(IDC_RADIO_TRAINING_ONLINE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_TRAINING_OFFLINE)->EnableWindow(TRUE);

		GetDlgItem(IDC_RADIO_DATA_STREAMING_RT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_DATA_STREAMING_LOAD)->EnableWindow(TRUE);

		GetDlgItem(IDC_RADIO_SAVE_IMMEDIATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_STOP_AND_RUN_STACK_ON)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_STOP_AND_RUN_STACK_OFF)->EnableWindow(TRUE);

		GetDlgItem(IDC_RADIO_DEVICE_DELSYS)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_DEVICE_FRANKFURT)->EnableWindow(TRUE);

		GetDlgItem(IDC_RADIO_USE_IMU_LOGONU_YES)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_USE_IMU_LOGONU_NO)->EnableWindow(TRUE);

		GetDlgItem(IDC_RADIO_USE_FLEX_SENSOR_YES)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_USE_FLEX_SENSOR_NO)->EnableWindow(TRUE);

		if (m_radioStreamingMode == 1) {
			GetDlgItem(IDC_BTN_LOAD)->EnableWindow(TRUE);
		}

		m_btnSwitch.SetWindowText(_T("Start"));
		m_flag_Switch = FALSE;
	}
}

void CDAQVizDlg::OnBnClickedBtnLoad() {
	CFileDialog read_file(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, _T("파일 선택 (.txt) | *.txt"));
	read_file.m_ofn.lpstrTitle = _T("Load할 텍스트 파일을 선택하세요.");
	read_file.m_ofn.lpstrInitialDir = _T("../Loading_data");

	CString m_filename;
	CString m_filedir;

	if (read_file.DoModal() == IDOK) {
		m_filename = read_file.GetFileName();
		m_filedir = read_file.GetPathName();

		if (!File_loaded_or_not) {
			File_loaded_or_not = TRUE;
			inFile.open(m_filedir);
			m_editLoadName.SetWindowText(m_filename);
		}
		else {
			inFile.close();
			inFile.open(m_filedir);
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

	pShared_Data->bFirst = TRUE;
	pShared_Data->bContinue = FALSE;
	pShared_Data->iNextOwner = THREAD_MAIN;
	pShared_Data->time = 0.0;
	pShared_Data->count = 0;

	// TwinCAT Process Execution (exe 파일 주소 확인)
	if (CreateProcess(_T("C:/Users/universe2030/Desktop/TwinCAT/TwinCAT/Release/TwinCAT.exe"),
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

	bProcessEnd = FALSE;
	while (!bProcessEnd) {
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

			if (CALI_START <= pShared_Data->time && pShared_Data->time < CALI_END) {
				if (pShared_Data->count == CALI_START * 1000) {
					m_editStatusBar.SetWindowText(stat += "[USER] Calibration start \r\n");
					m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());
				}

				// Calibration DAQ
				cali_count++;
				for (int i = 0; i < N_FLEX; i++)
					Flex_data_calib[i] += Flex_data[i];

				if (pShared_Data->count == CALI_END * 1000) {
					m_editStatusBar.SetWindowText(stat += "[USER] Calibration end \r\n");
					m_editStatusBar.LineScroll(m_editStatusBar.GetLineCount());

					for (int i = 0; i < N_FLEX; i++)
						Flex_data_calib[i] /= (double)cali_count;
				}

			}
			else {
				for (int i = 0; i < N_FLEX; i++) {
					Flex_data[i] -= Flex_data_calib[i];
				}
			}	

			// Toc
			QueryPerformanceCounter(&Counter_DAQ_End);
			Time_DAQ_elapse = (double)(Counter_DAQ_End.QuadPart - Counter_DAQ_Start.QuadPart)
				/ (double)Counter_DAQ_Frequency.QuadPart * 1000; // ms scale

			//////////////////////////////////////// RTGraph ////////////////////////////////////////
			// Tic
			QueryPerformanceCounter(&Counter_RTGraph_Start);

			// RTGraph Body

			// For test
			for (int i = 0; i < 16; i++)
				sEMG_temp_16CH[i] = (double) rand() / (double) RAND_MAX;
			
			p_ChildDlg_KSJ->Get_OpenGLPointer()->Set_sEMG_data(sEMG_temp_16CH);

			sEMG_temp[0] = 0.3 * sin(2 * PI * m_time);
			sEMG_temp[1] = 0.5 * sin(2 * PI * 2 * m_time);
			sEMG_temp[2] = 0.2 * sin(2 * PI * 3 * m_time);
			sEMG_temp[3] = 0.8 * sin(2 * PI * 1.5 * m_time);

			sEMG_temp_abs[0] = abs(0.3 * sin(2 * PI * m_time));
			sEMG_temp_abs[1] = abs(0.5 * sin(2 * PI * 2 * m_time));
			sEMG_temp_abs[2] = abs(0.2 * sin(2 * PI * 3 * m_time));
			sEMG_temp_abs[3] = abs(0.8 * sin(2 * PI * 1.5 * m_time));

			if (pShared_Data->count % N_GRAPH == 0) {
				p_ChildDlg_KSJ->Plot_graph(sEMG_temp_abs, p_ChildDlg_KSJ->Get_rtGraph_sEMG_MAV()[0]);
			}
			else if (pShared_Data->count % N_GRAPH == 1) {
				p_ChildDlg_KSJ->Plot_graph(sEMG_temp_abs, p_ChildDlg_KSJ->Get_rtGraph_sEMG_MAV()[1]);
			}
			else if (pShared_Data->count % N_GRAPH == 2) {
				p_ChildDlg_KSJ->Plot_graph(sEMG_temp_abs, p_ChildDlg_KSJ->Get_rtGraph_sEMG_MAV()[2]);
			}
			else if (pShared_Data->count % N_GRAPH == 3) {
				p_ChildDlg_KSJ->Plot_graph(sEMG_temp_abs, p_ChildDlg_KSJ->Get_rtGraph_sEMG_MAV()[3]);
			}
			else if (pShared_Data->count % N_GRAPH == 4) {
				p_ChildDlg_KSJ->Plot_graph(Flex_data, p_ChildDlg_KSJ->Get_rtGraph_Flex()[0]);
			}

			// Toc
			QueryPerformanceCounter(&Counter_RTGraph_End);
			Time_RTGraph_elapse = (double)(Counter_RTGraph_End.QuadPart - Counter_RTGraph_Start.QuadPart)
				/ (double)Counter_RTGraph_Frequency.QuadPart * 1000; // ms scale

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

	if (IDC_RADIO_TRAINING_ONLINE <= ID && ID <= IDC_RADIO_TRAINING_OFFLINE) {
		if (TEST_FLAG) {
			CString radio_val;
			radio_val.Format(_T("%d"), m_radioTrainingMode);
			GetDlgItem(IDC_EDIT_TEST)->SetWindowText(radio_val);
		}
		switch (m_radioTrainingMode) {
		case 0:

			break;
		case 1:

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

			break;
		case 1:

			break;
		}

	}
	else if (IDC_RADIO_DEVICE_DELSYS <= ID && ID <= IDC_RADIO_DEVICE_FRANKFURT) {
		if (TEST_FLAG) {
			CString radio_val;
			radio_val.Format(_T("%d"), m_radiosEMGDAQDev);
			GetDlgItem(IDC_EDIT_TEST)->SetWindowText(radio_val);
		}
		switch (m_radiosEMGDAQDev) {
		case 0:

			break;
		case 1:

			break;
		}
	}
	else if (IDC_RADIO_USE_IMU_LOGONU_YES <= ID && ID <= IDC_RADIO_USE_IMU_LOGONU_NO) {
		if (TEST_FLAG) {
			CString radio_val;
			radio_val.Format(_T("%d"), m_radioUseIMU);
			GetDlgItem(IDC_EDIT_TEST)->SetWindowText(radio_val);
		}
		switch (m_radioUseIMU) {
		case 0:

			break;
		case 1:

			break;
		}
	}
	else if (IDC_RADIO_USE_FLEX_SENSOR_YES <= ID && ID <= IDC_RADIO_USE_FLEX_SENSOR_NO) {
		if (TEST_FLAG) {
			CString radio_val;
			radio_val.Format(_T("%d"), m_radioUseFlexSensor);
			GetDlgItem(IDC_EDIT_TEST)->SetWindowText(radio_val);
		}
		switch (m_radioUseFlexSensor) {
		case 0:

			break;
		case 1:

			break;
		}
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