#pragma once
#include "NIDAQmx.h"

// Filter variables
#define CUTOFF_LPF 30	//LPF Cutoff Freq
#define CUTOFF_FD 20	//FD Cutoff Freq
#define ALPHA_LPF ( (2 * CUTOFF_LPF * TS ) / (2 + CUTOFF_LPF* TS ) )	//LPF와 filtered에서 쓰이는 weight값.
#define ALPHA_FD ( ( 2 * CUTOFF_FD * TS ) / (2 + CUTOFF_FD* TS ) )	//LPF와 filtered에서 쓰이는 weight값.
#define TS 0.001
#define PI 3.14159265
#define SAMP_FREQ 1000

class IO {
public:
	IO();
	virtual ~IO();
	void LowPassFilter(double Input_prev, double Input_present, double Output_prev, double* Output_present);
	void FilteredDerivative(double Input_prev, double Input_present, double Output_prev, double* Output_present);
};

class NI_AI_sEMG : public IO {
private:
	static TaskHandle	m_TaskHandle;

public:
	static bool			m_First;
	static float64* m_ReadValue;
	static int			m_Number;

public:
	NI_AI_sEMG(const char* DevAINum, int Num_CH);
	virtual ~NI_AI_sEMG();
	static void			InitializeNI();
	static void			ReadOneStep();
	float64* Get_m_ReadValue();
};

class NI_AI_Flex : public IO {
private:
	static TaskHandle	m_TaskHandle;

public:
	static bool			m_First;
	static float64*		m_ReadValue;
	static int			m_Number;

public:
	NI_AI_Flex(const char* DevAINum, int Num_CH);
	virtual ~NI_AI_Flex();
	static void			InitializeNI();
	static void			ReadOneStep();
	float64*			Get_m_ReadValue();
};

class NI_AO : public IO {
private:
	static TaskHandle	m_TaskHandle;

public:
	static bool			m_First;
	static float64*		m_WriteValue;
	static int			m_Number;

public:
	NI_AO(const char* DevAoNum, int Num_CH);
	virtual ~NI_AO();
	static void InitializeNI();
	static void WriteOneStep(float64 Volt);
};

class NI_DO : public IO {
private:
	static TaskHandle m_TaskHandle;
	
public:
	static bool			m_First;
	static UINT8*		m_WriteValue;
	static int			m_Number;

public:
	NI_DO(const char* DevDoNum);
	virtual ~NI_DO();
	static void InitializeNI();
	static void WriteOneStep(UINT8 logic);
};