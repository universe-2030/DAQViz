#pragma once
#include <vector>
#include <math.h>

#define N_OUTPUT 50
#define SCALE_FACTOR 10
#define PI 3.14159265358

#define FINGER_FLEX_THRES_ABS 0.05
#define WRIST_FE_THRES_ABS 0.05
#define WRIST_RU_THRES_ABS 0.1

#define LABEL_POWER_GRIP 1
#define LABEL_HAND_OPEN 2
#define LABEL_WRIST_FLEXION 3
#define LABEL_WRIST_EXTENSION 4
#define LABEL_WRIST_RADIAL 5
#define LABEL_WRIST_ULNAR 6
#define LABEL_OTHERS 0

#define SEMG_BASE_SCALE 1.1

class SignalProcessor {
private:
	// The number of sEMG channel
	int N_sEMG_CH;

	// The number of Flex sensors
	int N_Finger_CH;

	// The number of IMU sensors
	int N_Wrist_CH;

	// Sampling time & frequency
	double T_S;
	double Fs;

	// Filtered derivative variable
	double w_c_LPF;

public:
	SignalProcessor(); // Default constructors
	SignalProcessor(int Num_sEMG, int Num_Finger, int Num_Wrist,
					double time_step, double sampling_rate, double cutoff_LPF);
	~SignalProcessor();

	double LowPassFilter(double Prev_input, double Current_input, double Prev_output);

	double FilteredDerivative(double Prev_input, double Current_input, double Prev_output);

	UINT* MotionClassification (const double* _sEMG_MAV_base,
								const double* _sEMG_MAV,
								const double* _Finger_data,
								const double* _Wrist_data);
	UINT MotionClassification_Flex_Slope(const double* _Finger_slope, const double* _Wrist_slope);

	UINT* MotionEstimation_sEMG(const double* _sEMG_data);

	int Factorial(int n);

	/////////////////////////////////////// Not used ///////////////////////////////////////	
//private:
//	// Notch filter variables
//	double w_o;
//	double w_c;
//
//	// Bayesian filter variables
//	double N_output;
//	double ratemax;
//	double inscale;
//	double alpha;
//	double beta;
//
//	double** prior;
//	double** measurement_model;
//	double** driving_signal;
//	double** posterior;
//
//	// Threshold variables
//	double Onset_threshold_value;
//	double Onset_threshold_slope;
//	double Window_size;
//
//	double Prev_slope;
//
//	// Bayesian filter variables
//	int max_idx = 0;
//	double PeakIndex = 0.0;
//
//public:
//	SignalProcessor(int N_ch, double cutoff_central, double cutoff_width,
//					double time_step, double cutoff_LPF,
//					double sampling_rate, double n_output,
//					double rate_max, double in_scale, double _alpha,
//					double _beta, double onset_threshold_value,
//					double onset_threshold_slope, int window_size);
//
//	void SetParameter(double cutoff_central, double cutoff_width,
//					double time_step, double cutoff_LPF,
//					double onset_threshold_value,
//					double onset_threshold_slope, int window_size);
//
//	double NotchFilter(double Dprev_input, double Prev_input,
//					double Currert_input, double Dprev_output, double Prev_output);
//
//	double TeagerKaiserOperation(double Prev_input, double Current_input, double Future_input);
//
//	double BayesianFilter(double EMG_val, int ch_idx, Filter_type Filter);
};