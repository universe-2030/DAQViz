#include <iostream>
#include <conio.h>
#include <windows.h>
#include <winbase.h>

// headers for TwinCAT 3 ADS
#include "C:/TwinCAT/AdsApi/TcAdsDll/Include/TcAdsDef.h"
#include "C:/TwinCAT/AdsApi/TcAdsDll/Include/TcAdsAPI.h"

using namespace std;

#define ENDTIME		300
#define TIMESTEP	0.001

#define THREAD_MAIN		0
#define THREAD_TWINCAT  1
#define THREAD_CALLBACK 2

struct Shared_Data {
	bool bContinue;
	bool bSaveImmediate;
	bool bProcessEnd;
	bool bFirst;
	int iNextOwner;		// previous mutex owner;\
							 0:User Program,\
							 1:TwinCAT Process,\
							 2:Callback function
	int count;
	double time;
};

void __stdcall Callback(AmsAddr*, AdsNotificationHeader*, unsigned long);