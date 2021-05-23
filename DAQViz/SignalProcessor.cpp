#include "pch.h"
#include "SignalProcessor.h"

SignalProcessor::SignalProcessor() { // Niave initialization

}

SignalProcessor::SignalProcessor(int Num_sEMG, int Num_Flex, int Num_IMU,
								double time_step, double sampling_rate, double cutoff_LPF) {
	N_sEMG_CH = Num_sEMG;
	N_Flex_CH = Num_Flex;
	N_IMU_CH = Num_IMU;

	T_S = time_step;
	Fs = sampling_rate;

	w_c_LPF = cutoff_LPF;
}

SignalProcessor::~SignalProcessor() { // Niave initialization

}

double SignalProcessor::FilteredDerivative(double Prev_input,
										   double Current_input, double Prev_output) {
	double A1 = 2 + T_S * w_c_LPF;
	double A2 = 2 - T_S * w_c_LPF;
	double B1 = 2 * w_c_LPF;
	double B2 = 2 * w_c_LPF;

	return ((1 / A1) * (A2 * Prev_output + B1 * Current_input - B2 * Prev_input));
}

UINT SignalProcessor::MotionClassification(const double* _Flex_data, const double* _IMU_data) {
	bool* isFingerFlex = new bool[N_Flex_CH];
	memset(isFingerFlex, FALSE, N_Flex_CH);
	for (int i = 0; i < N_Flex_CH; i++) {
		if (_Flex_data[i] < -FINGER_FLEX_THRES_ABS) {
			isFingerFlex[i] = TRUE;
		}
	}

	int* isWristFlex = new int[N_IMU_CH];
	memset(isWristFlex, 0, sizeof(int) * N_IMU_CH);
	for (int i = 0; i < N_IMU_CH; i++) {
		if (_IMU_data[i] < -WRIST_FE_THRES_ABS) {
			isWristFlex[i] = 1;
		}
		else if (_IMU_data[i] > WRIST_FE_THRES_ABS) {
			isWristFlex[i] = -1;
		}
	}

	if (isWristFlex[0] == 1) { // Wrist flexion
		return LABEL_WRIST_FLEXION;
	}
	else if (isWristFlex[0] == -1) { // Wrist extension
		return LABEL_WRIST_EXTENSION;
	}
	else if (isFingerFlex[0] && isFingerFlex[1] &&
			isFingerFlex[2] && isFingerFlex[3] && isFingerFlex[4]) { // Power grip
		return LABEL_POWER_GRIP;
	}
	else if (!isFingerFlex[0] && !isFingerFlex[1] &&
		!isFingerFlex[2] && !isFingerFlex[3] && !isFingerFlex[4]) { // Hand open
		return LABEL_HAND_OPEN;
	}
}

int SignalProcessor::Factorial(int n) {
	int result = 1;

	for (int i = 1; i <= n; i++)
		result *= i;

	return result;
}

	/////////////////////////////////////// Not used ///////////////////////////////////////
//SignalProcessor::SignalProcessor(int N_ch, double cutoff_central, double cutoff_width,
//	double time_step, double cutoff_LPF,
//	double sampling_rate, double n_output,
//	double rate_max, double in_scale, double _alpha,
//	double _beta, double onset_threshold_value,
//	double onset_threshold_slope, int window_size) {
//	N_sEMG_CH = N_ch;
//
//	w_o = cutoff_central;
//	w_c = cutoff_width;
//	T_S = time_step;
//
//	w_c_LPF = cutoff_LPF;
//
//	F_s = sampling_rate;
//	N_output = n_output;
//	ratemax = rate_max;
//	inscale = in_scale;
//	alpha = _alpha;
//	beta = _beta;
//
//	// Dynamic allocation
//	prior = new double* [N_sEMG_CH];
//	measurement_model = new double* [N_sEMG_CH];
//	driving_signal = new double* [N_sEMG_CH];
//	posterior = new double* [N_sEMG_CH];
//
//	for (int i = 0; i < N_sEMG_CH; i++) {
//		prior[i] = new double[N_output];
//		measurement_model[i] = new double[N_output];
//		driving_signal[i] = new double[N_output];
//		posterior[i] = new double[N_output];
//	}
//
//	// Initialization
//	for (int i = 0; i < N_sEMG_CH; i++) {
//		for (int j = 0; j < N_output; j++) {
//			prior[i][j] = (double)(1.0) / N_output;
//			measurement_model[i][j] = (double)(1.0) / N_output;
//			driving_signal[i][j] = (ratemax / N_output) * (j + 1);
//			posterior[i][j] = (double)(1.0) / N_output;
//		}
//	}
//
//	Onset_threshold_value = onset_threshold_value;
//	Onset_threshold_slope = onset_threshold_slope;
//	Window_size = window_size;
//
//	Prev_slope = 0.0;
//}
//
//void SignalProcessor::SetParameter(double cutoff_central, double cutoff_width,
//	double time_step, double cutoff_LPF,
//	double onset_threshold_value,
//	double onset_threshold_slope, int window_size) {
//	w_o = cutoff_central;
//	w_c = cutoff_width;
//	T_S = time_step;
//
//	w_c_LPF = cutoff_LPF;
//
//	Onset_threshold_value = onset_threshold_value;
//	Onset_threshold_slope = onset_threshold_slope;
//	Window_size = window_size;
//}
//
//double SignalProcessor::NotchFilter(double Dprev_input, double Prev_input,
//	double Current_input, double Dprev_output, double Prev_output) {
//	double A1 = pow(w_o, 2) + 2 * T_S * w_c + 4;
//	double A2 = 2 * w_o - 8;
//	double A3 = pow(w_o, 2) - 2 * T_S * w_c + 4;
//
//	double B1 = pow(T_S, 2) * pow(w_o, 2) + 4;
//	double B2 = 2 * pow(T_S, 2) * pow(w_o, 2) - 8;
//	double B3 = pow(T_S, 2) * pow(w_o, 2) + 4;
//
//	return ((1 / A1) * (B1 * Current_input + B2 * Prev_input + B3 * Dprev_input - A2 * Prev_output - A3 * Dprev_output));
//}
//
//double SignalProcessor::TeagerKaiserOperation(double Prev_input,
//	double Current_input, double Future_input) {
//	double result = pow(Current_input, 2) - Prev_input * Future_input;
//	return result;
//}
//
//double SignalProcessor::BayesianFilter(double EMG_val, int ch_idx, Filter_type Filter) {
//	// 1. Prior distribution second difference propagation
//	for (int i = 1; i < N_output - 1; i++) {
//		prior[ch_idx][i] = alpha * prior[ch_idx][i - 1] + (2 - 2 * alpha - beta) * prior[ch_idx][i] + alpha * prior[ch_idx][i + 1] + beta;
//	}
//
//	double EMG_val_rev = abs(EMG_val) * inscale * ratemax * SCALE_FACTOR;
//	if (EMG_val_rev > ratemax) {
//		EMG_val_rev = ratemax;
//	}
//
//	// 2. Calculate the posterior likelihood function
//	if (Filter == POISSON) {
//		for (int i = 0; i < N_output; i++) {
//			int n = (int)(floor(EMG_val_rev * 10));
//			measurement_model[ch_idx][i] = pow(driving_signal[ch_idx][i], n) * exp(-driving_signal[ch_idx][i]) / (double)(Factorial(n));
//			posterior[ch_idx][i] = measurement_model[ch_idx][i] * prior[ch_idx][i];
//		}
//	}
//	else if (Filter == HALF_GAUSSIAN) {
//		for (int i = 0; i < N_output; i++) {
//			measurement_model[ch_idx][i] = 1.0 / sqrt(2 * PI * pow(driving_signal[ch_idx][i], 2))
//				* exp(-pow(EMG_val_rev, 2) / (2 * pow(driving_signal[ch_idx][i], 2)));
//			posterior[ch_idx][i] = measurement_model[ch_idx][i] * prior[ch_idx][i];
//		}
//	}
//	else if (Filter == EXPONENTIAL) {
//		for (int i = 0; i < N_output; i++) {
//			measurement_model[ch_idx][i] = exp(-EMG_val_rev / driving_signal[ch_idx][i]) / driving_signal[ch_idx][i];
//			posterior[ch_idx][i] = measurement_model[ch_idx][i] * prior[ch_idx][i];
//		}
//	}
//
//	// 3. Output the signal estimate MAP(x(t)) = argmax P(x, t)
//	max_idx = N_output;
//	double post_max = posterior[ch_idx][(int)N_output - 1];
//	for (int i = 0; i < N_output; i++) {
//		// Find the maximum index in posterior
//		if (posterior[ch_idx][(int)N_output - 1 - i] >= post_max) {
//			post_max = posterior[ch_idx][(int)N_output - 1 - i];
//			max_idx = N_output - i;
//		}
//	}
//
//	// 4. Output the signal estimate MAP(x(t)) = armax P(x, t)
//	if ((max_idx > 1) && (max_idx < N_output)) {
//		double dL = posterior[ch_idx][max_idx - 2] - posterior[ch_idx][max_idx - 1];
//		double dR = posterior[ch_idx][max_idx - 1] - posterior[ch_idx][max_idx];
//		PeakIndex = (max_idx - 0.5 - (dL / (dR - dL))); // Interpolation
//	}
//	else
//		PeakIndex = max_idx;
//
//	// 5. Normalization
//	double sum = 0;
//	for (int i = 0; i < N_output; i++)
//		sum += posterior[ch_idx][i];
//
//	for (int i = 0; i < N_output; i++)
//		posterior[ch_idx][i] = posterior[ch_idx][i] / sum;
//
//	// 6. Prior distribution update
//	for (int i = 0; i < N_output; i++)
//		prior[ch_idx][i] = posterior[ch_idx][i];
//
//	double MAP = (ratemax / (N_output - 1)) * PeakIndex;
//
//	return MAP;
//}