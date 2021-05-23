#pragma once

#define N_MOTIONS 6
#define N_SEMG_CH 16

// Model defined by Sejin Kim
class Model {

private:
	int Num_motions;
	int Num_sEMG_CH;

	double** sEMG_mean;
	double** sEMG_std;

public:
	Model();
	Model(int _Num_motions, int _Num_sEMG_CH);
	~Model();

	void Initialize_Variable();

	void Set_sEMG_mean(const double** _sEMG_mean);
	void Set_sEMG_std(const double** _sEMG_std);
};