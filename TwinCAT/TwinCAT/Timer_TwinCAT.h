#include <iostream>
#include <conio.h>
#include <windows.h>
#include <winbase.h>

// headers for TwinCAT 3 ADS
#include "C:/TwinCAT/AdsApi/TcAdsDll/Include/TcAdsDef.h"
#include "C:/TwinCAT/AdsApi/TcAdsDll/Include/TcAdsAPI.h"

using namespace std;

#define TIMESTEP	0.001

#define THREAD_TWINCAT  1
#define THREAD_CALLBACK 2
#define THREAD_MAIN		0

struct Shared_Data {
	bool bFirst;
	bool bContinue;
	int iNextOwner;		// 0 : TwinCAT Process
						// 1 : Callback function
						// 2 : Main Thread
	int count;
	double time;
};

void __stdcall Callback(AmsAddr*, AdsNotificationHeader*, unsigned long);