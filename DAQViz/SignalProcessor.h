#pragma once
#include <vector>
#include <math.h>

#define N_OUTPUT 50
#define SCALE_FACTOR 10
#define PI 3.14159265358

enum Filter_type {
	POISSON,
	HALF_GAUSSIAN,
	EXPONENTIAL
};

class SignalProcessor {
private:
	// The number of sEMG channel
	int N_sEMG_CH;

	// Notch filter variables
	double w_o;
	double w_c;
	double T_S;

	// Filtered derivative variable
	double w_c_LPF;

	// Bayesian filter variables
	double F_s;
	double N_output;
	double ratemax;
	double inscale;
	double alpha;
	double beta;

	double** prior;
	double** measurement_model;
	double** driving_signal;
	double** posterior;

	// Threshold variables
	double Onset_threshold_value;
	double Onset_threshold_slope;
	double Window_size;

	double Prev_slope;

	// Bayesian filter variables
	int max_idx = 0;
	double PeakIndex = 0.0;

public:
	SignalProcessor(); // Default constructors
	SignalProcessor(int N_ch, double cutoff_central, double cutoff_width,
					double time_step, double cutoff_LPF,
					double sampling_rate, double n_output,
					double rate_max, double in_scale, double _alpha,
					double _beta, double onset_threshold_value,
					double onset_threshold_slope, int window_size);
	~SignalProcessor();

	void SetParameter(double cutoff_central, double cutoff_width,
					  double time_step, double cutoff_LPF,
					  double onset_threshold_value,
					  double onset_threshold_slope, int window_size);

	double LowPassFilter(double Prev_input, double Current_input, double Prev_output);

	double FilteredDerivative(double Prev_input, double Current_input, double Prev_output);

	double NotchFilter(double Dprev_input, double Prev_input,
					   double Currert_input, double Dprev_output, double Prev_output);
	double TeagerKaiserOperation(double Prev_input,
								 double Current_input, double Future_input);
	double BayesianFilter(double EMG_val, int ch_idx, Filter_type Filter);

	int Factorial(int n);
};