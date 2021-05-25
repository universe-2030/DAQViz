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

	const std::queue<double>* sEMG_raw_window;

	const std::vector<double>* sEMG_raw;
	const std::vector<double>* sEMG_MAV;
	const std::vector<double>* sEMG_RMS;
};