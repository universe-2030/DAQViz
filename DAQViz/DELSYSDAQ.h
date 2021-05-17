#pragma once

#include <queue>
#include <vector>

#define N_CH 16
#define WIN_SIZE 250

class DELSYSDAQ {
public:
	DELSYSDAQ();
	DELSYSDAQ(UINT _N_CH);
	DELSYSDAQ(UINT _N_CH, UINT _Win_size);

private:
	UINT Win_size;

	std::queue<double>* sEMG_raw_window;

	std::vector<double>* sEMG_raw;
	std::vector<double>* sEMG_MAV;
	std::vector<double>* sEMG_RMS;
};