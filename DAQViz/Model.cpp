#include "pch.h"
#include "Model.h"

Model::Model() {
	Num_motions = N_MOTIONS;
	Num_sEMG_CH = N_SEMG_CH;
	
	Initialize_Variable();
}

Model::Model(int _Num_motions, int _Num_sEMG_CH) {
	Num_motions = _Num_motions;
	Num_sEMG_CH = _Num_sEMG_CH;

	Initialize_Variable();
}

Model::~Model() {
	for (int i = 0; i < Num_motions; i++) {
		delete sEMG_mean[i];
		delete sEMG_std[i];
	}

	delete[] sEMG_mean;
	delete[] sEMG_std;
}

void Model::Initialize_Variable() {
	sEMG_mean = new double*[Num_motions];
	sEMG_std = new double*[Num_motions];

	for (int i = 0; i < Num_motions; i++) {
		sEMG_mean[i] = new double[Num_sEMG_CH];
		sEMG_std[i] = new double[Num_sEMG_CH];
	}
}

void Model::Set_sEMG_mean (const double** _sEMG_mean) {
	for (int i = 0; i < Num_motions; i++) {
		for (int j = 0; j < Num_sEMG_CH; j++) {
			sEMG_mean[i][j] = _sEMG_mean[i][j];
		}
	}
}

void Model::Set_sEMG_std (const double** _sEMG_std) {
	for (int i = 0; i < Num_motions; i++) {
		for (int j = 0; j < Num_sEMG_CH; j++) {
			sEMG_std[i][j] = _sEMG_std[i][j];
		}
	}
}