#include "pch.h"
#include "DELSYSDAQ.h"

DELSYSDAQ::DELSYSDAQ() {
	Win_size = WIN_SIZE;

	sEMG_raw = new std::vector<double>[N_CH];
	sEMG_MAV = new std::vector<double>[N_CH];
	sEMG_RMS = new std::vector<double>[N_CH];
}

DELSYSDAQ::DELSYSDAQ(UINT _N_CH) {
	Win_size = WIN_SIZE;

	sEMG_raw = new std::vector<double>[_N_CH];
	sEMG_MAV = new std::vector<double>[_N_CH];
	sEMG_RMS = new std::vector<double>[_N_CH];
}

DELSYSDAQ::DELSYSDAQ(UINT _N_CH, UINT _Win_size) {
	Win_size = _Win_size;

	sEMG_raw = new std::vector<double>[_N_CH];
	sEMG_MAV = new std::vector<double>[_N_CH];
	sEMG_RMS = new std::vector<double>[_N_CH];
}