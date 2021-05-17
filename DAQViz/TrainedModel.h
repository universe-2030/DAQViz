#pragma once

// Trained model is applied with either offline or online (real-time) operation
#define N_MOTION 6

class TrainedModel {

// Training model - Sejin Kim
public:
	void Set_mean_sEMG_motionwise(double** input);
	void Set_std_sEMG_motionwise(double** input);

private:
	int N_motions;
	int N_sEMG;

	double** mean_sEMG_motionwise;
	double** std_sEMG_motionwise;
};