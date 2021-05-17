#include "pch.h"
#include "NI_AI_AO_DO.h"

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

// Static variables initialization
TaskHandle NI_AI_sEMG::m_TaskHandle = 0;
float64* NI_AI_sEMG::m_ReadValue = NULL;
int NI_AI_sEMG::m_Number = 0;
bool NI_AI_sEMG::m_First = 1;

TaskHandle NI_AI_Flex::m_TaskHandle = 0;
float64* NI_AI_Flex::m_ReadValue = NULL;
int NI_AI_Flex::m_Number = 0;
bool NI_AI_Flex::m_First = 1;

TaskHandle NI_AO::m_TaskHandle = 0;
float64* NI_AO::m_WriteValue = NULL;
int NI_AO::m_Number = 0;
bool NI_AO::m_First = 1;

TaskHandle NI_DO::m_TaskHandle = 0;
UINT8* NI_DO::m_WriteValue = NULL;
int NI_DO::m_Number = 0;
bool NI_DO::m_First = 1;

IO::IO() {}

IO::~IO() {}

void IO::LowPassFilter(double Input_prev, double Input_present, double Output_prev, double* Output_present) {
	*Output_present = ALPHA_LPF * ((Input_present + Input_prev) / 2) + (1 - ALPHA_LPF) * Output_prev;
}

void IO::FilteredDerivative(double Input_prev, double Input_present, double Output_prev, double *Output_present) {
	*Output_present = ALPHA_FD * ((Input_present - Input_prev) / TS) + (1 - ALPHA_FD) * Output_prev;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
NI_AI_sEMG::NI_AI_sEMG(const char* DevAINum, int Num_CH) : IO() {
	int32	error = 0;
	char	errBuff[2048] = { '\0' };

	if (m_Number == 0) {
		DAQmxCreateTask("", &m_TaskHandle);
	}

	m_Number = Num_CH;
	DAQmxCreateAIVoltageChan(m_TaskHandle, DevAINum, "", DAQmx_Val_RSE, -10.0, 10.0, DAQmx_Val_Volts, NULL);

	DAQmxErrChk(DAQmxCfgSampClkTiming(m_TaskHandle, "", SAMP_FREQ, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 1000));
	DAQmxErrChk(DAQmxStartTask(m_TaskHandle));
	DAQmxErrChk(DAQmxSetReadRelativeTo(m_TaskHandle, DAQmx_Val_MostRecentSamp));
	DAQmxErrChk(DAQmxSetReadOffset(m_TaskHandle, -1));

Error:
	if (DAQmxFailed(error)) {
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
	}
}

NI_AI_sEMG::~NI_AI_sEMG() {
	m_Number = 0;

	if (m_Number == 0) {
		DAQmxStopTask(m_TaskHandle);
		DAQmxClearTask(m_TaskHandle);

		delete m_ReadValue;
	}
}

void NI_AI_sEMG::InitializeNI() {
	m_ReadValue = new float64[sizeof(float64) * m_Number];
	memset(m_ReadValue, 0, sizeof(m_ReadValue));
}

void NI_AI_sEMG::ReadOneStep() {
	int32		error = 0;
	char		errBuff[2048] = { '\0' };
	int32		Read;

	if (m_First == 1) {
		DAQmxReadAnalogF64(m_TaskHandle, -1, 0, DAQmx_Val_GroupByScanNumber,
							m_ReadValue, SAMP_FREQ, &Read, NULL);
		m_First = 0;
	}
	DAQmxErrChk(DAQmxReadAnalogF64(m_TaskHandle, -1, 0, DAQmx_Val_GroupByScanNumber,
							m_ReadValue, SAMP_FREQ, &Read, NULL));

Error:
	if (DAQmxFailed(error)) {
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
	}
}

float64* NI_AI_sEMG::Get_m_ReadValue() {
	float64* read_temp = m_ReadValue;
	return read_temp;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
NI_AI_Flex::NI_AI_Flex(const char* DevAINum, int Num_CH) : IO() {
	int32	error = 0;
	char	errBuff[2048] = { '\0' };

	if (m_Number == 0) {
		DAQmxCreateTask("", &m_TaskHandle);
	}

	m_Number = Num_CH;
	DAQmxCreateAIVoltageChan(m_TaskHandle, DevAINum, "", DAQmx_Val_RSE, -10.0, 10.0, DAQmx_Val_Volts, NULL);

	DAQmxErrChk(DAQmxCfgSampClkTiming(m_TaskHandle, "", SAMP_FREQ, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 1000));
	DAQmxErrChk(DAQmxStartTask(m_TaskHandle));
	DAQmxErrChk(DAQmxSetReadRelativeTo(m_TaskHandle, DAQmx_Val_MostRecentSamp));
	DAQmxErrChk(DAQmxSetReadOffset(m_TaskHandle, -1));

Error:
	if (DAQmxFailed(error)) {
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
	}
}

NI_AI_Flex::~NI_AI_Flex() {
	m_Number = 0;

	if (m_Number == 0) {
		DAQmxStopTask(m_TaskHandle);
		DAQmxClearTask(m_TaskHandle);

		delete m_ReadValue;
	}
}

void NI_AI_Flex::InitializeNI() {
	m_ReadValue = new float64[sizeof(float64) * m_Number];
	memset(m_ReadValue, 0, sizeof(m_ReadValue));
}

void NI_AI_Flex::ReadOneStep() {
	int32		error = 0;
	char		errBuff[2048] = { '\0' };
	int32		Read;

	if (m_First == 1) {
		DAQmxReadAnalogF64(m_TaskHandle, -1, 0, DAQmx_Val_GroupByScanNumber,
			m_ReadValue, SAMP_FREQ, &Read, NULL);
		m_First = 0;
	}
	DAQmxErrChk(DAQmxReadAnalogF64(m_TaskHandle, -1, 0, DAQmx_Val_GroupByScanNumber,
		m_ReadValue, SAMP_FREQ, &Read, NULL));

Error:
	if (DAQmxFailed(error)) {
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
	}
}

float64* NI_AI_Flex::Get_m_ReadValue() {
	float64* read_temp = m_ReadValue;
	return read_temp;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////

NI_AO::NI_AO(const char* DevAONum,int Num_CH) {
	if (m_Number == 0) {
		DAQmxCreateTask("", &m_TaskHandle);
	}

	m_Number = Num_CH;
	DAQmxCreateAOVoltageChan(m_TaskHandle, DevAONum, "", -10.0, 10.0, DAQmx_Val_Volts, NULL);
}

NI_AO::~NI_AO() {
	m_Number = 0;
	
	int32		Write;

	if (m_Number == 0) {
		m_WriteValue[0] = 0;
		DAQmxWriteAnalogF64(m_TaskHandle, 1, 0, DAQmx_Val_WaitInfinitely, DAQmx_Val_GroupByScanNumber, m_WriteValue, &Write, NULL);

		DAQmxStopTask(m_TaskHandle);
		DAQmxClearTask(m_TaskHandle);

		delete m_WriteValue;
	}
}

void NI_AO::InitializeNI() {
	m_WriteValue = new float64[sizeof(float64) * m_Number];
	memset(m_WriteValue, 0, sizeof(m_WriteValue));
	DAQmxStartTask(m_TaskHandle);
}

void NI_AO::WriteOneStep(float64 Volt) {
	int32		error = 0;
	char		errBuff[2048] = { '\0' };
	int32		Write;

	m_WriteValue[0] = Volt;

	if (m_First == 1) {
		DAQmxWriteAnalogF64(m_TaskHandle, 1, 0, DAQmx_Val_WaitInfinitely, DAQmx_Val_GroupByScanNumber, m_WriteValue, &Write, NULL);
		m_First = 0;
	}
	DAQmxErrChk(DAQmxWriteAnalogF64(m_TaskHandle, 1, 0, DAQmx_Val_WaitInfinitely, DAQmx_Val_GroupByScanNumber, m_WriteValue, &Write, NULL));

Error:
	if (DAQmxFailed(error)) {
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
	}
}

NI_DO::NI_DO(const char* DevDoNum) {
	if (m_Number == 0) {
		DAQmxCreateTask("", &m_TaskHandle);
	}

	m_Number = 1;
	DAQmxCreateDOChan(m_TaskHandle, DevDoNum, "", DAQmx_Val_ChanForAllLines);
}

NI_DO::~NI_DO() {
	m_Number = 0;
	int32		Write;

	if (m_Number == 0) {
		m_WriteValue[0] = 0x00;
		DAQmxWriteDigitalU8(m_TaskHandle, 1, 1, DAQmx_Val_WaitInfinitely, DAQmx_Val_GroupByScanNumber, m_WriteValue, &Write, NULL);

		DAQmxStopTask(m_TaskHandle);
		DAQmxClearTask(m_TaskHandle);

		delete m_WriteValue;
	}
}

void NI_DO::InitializeNI() {
	m_WriteValue = new UINT8[sizeof(UINT8) * m_Number];
	memset(m_WriteValue, 0, sizeof(m_WriteValue));
	DAQmxStartTask(m_TaskHandle);
}

void NI_DO::WriteOneStep(UINT8 logic) {
	int32		error = 0;
	char		errBuff[2048] = { '\0' };
	int32		Write;

	m_WriteValue[0] = logic;

	if (m_First == 1) {
		DAQmxWriteDigitalU8(m_TaskHandle, 1, 1, DAQmx_Val_WaitInfinitely, DAQmx_Val_GroupByScanNumber, m_WriteValue, &Write, NULL);
		m_First = 0;
	}
	DAQmxErrChk(DAQmxWriteDigitalU8(m_TaskHandle, 1, 1, DAQmx_Val_WaitInfinitely, DAQmx_Val_GroupByScanNumber, m_WriteValue, &Write, NULL));

Error:
	if (DAQmxFailed(error)) {
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
	}
}