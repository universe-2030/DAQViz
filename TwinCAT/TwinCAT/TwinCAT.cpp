// TwinCAT.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
	//////////////////variables for Memory Sharing/////////////////////////
	HANDLE hTCMemory			= NULL;
	HANDLE hTCMutex				= NULL;
	int	iMutex_count;
	bool bProcessEnd;
	struct Shared_Data * pTCData= NULL;

	LPCWSTR nTCMemory	= L"nMemory";
	LPCWSTR nTCMutex	= L"nMutex";

int _tmain(int argc, _TCHAR* argv[])
{

	//////////////////variables for ADS communication///////////////////////
	long						nErr;						// Error Message
	long						nPort;						// Port number
	long						nloop=0;
	AmsAddr						Addr;						// Target port information(PLC1)
	void						*pData={0};					// Addtional data
	ULONG						hNotification;				// Notification handle
	ULONG				        hUser;						// Data(Target_Var) handle which will be passed to callback function
	char						Target_Var[]={"MAIN.flag"};	// Name of target variable
	AdsNotificationAttrib		adsNotificationAttrib;		// Target Variable Change Notification settings

	// Shared Memory 호출
	if(hTCMemory=OpenFileMapping(FILE_MAP_ALL_ACCESS,NULL,argv[1])) cerr<<"[TC]Shared Memory Open Success \n"; //argv[1]=nMemory @WIN32
	else {cerr << "[TC]Error: Shared Memory Open Error\n"; cerr << GetLastError(); getch();}
	
	// Mutex 호출
	if(hTCMutex=CreateMutex(NULL,FALSE,argv[2])) cerr<<"[TC]Mutex Open Success \n"; //argv[2]=nMutex @WIN32
	else {cerr << "[TC]Error: Mutex Open Error\n"; cerr << GetLastError(); getch();}
	WaitForSingleObject(hTCMutex, INFINITY);

	// Shared Memory의 주소값 반환
	if(pTCData=(Shared_Data *)::MapViewOfFile(hTCMemory,FILE_MAP_ALL_ACCESS,0,0,sizeof(Shared_Data))) cerr<<"[TC]Data address returned \n";
	else { cerr << "[TC]Error: Address Return Failed \n"; cerr << GetLastError(); getch();}
	
	// ADS router에 ADS Port Open 
	nPort = AdsPortOpen();
	nErr = AdsGetLocalAddress(&Addr);
	if (nErr){
		cerr << "[TC:ADS]Error: AdsGetLocalAddress: " << nErr << "\n";
		goto Error;
	}
	else cerr << "[TC:ADS] ADS Port Opened\n";

	// TwinCAT 3 PLC1(Timer Module) Port = 851
	(&Addr)->port = 851;
	
	// Set ADS Notification Settings
	adsNotificationAttrib.cbLength = 4;							// Length of the data that is to be passed to the callback function
	adsNotificationAttrib.nTransMode = ADSTRANS_SERVERONCHA;	// Call callback function only when the Target_Var got changed (ADSTRANS_SEVERCYCLE: Call callback function every cycle)
	adsNotificationAttrib.nMaxDelay = 0;						// Delay on calling callback function (1=100ns)
	adsNotificationAttrib.nCycleTime = 100;						// Interval of checking timer call: 10us (1=100ns) 

	// Get ADS Handle
	nErr = AdsSyncReadWriteReq(&Addr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(hUser), &hUser, sizeof(Target_Var), Target_Var);
	if (nErr){
		cerr << "[TC:ADS]Error: AdsSyncReadWriteReq: " << nErr << "\n";
		goto Error;
	}
	else  cerr << "[TC:ADS] Data Read Success\n";

	// Run PLC1
	nErr = 1;
	while(nErr)			// Timer가 이미 실행 중일 때 Stop 후 다시 Run 하기 위한 Loop
	{
		nErr = AdsSyncWriteControlReq (&Addr, ADSSTATE_RUN, 0, 0, NULL);
		switch(nErr){
		case 0:
			cerr << "[TC:ADS] Device Run\n";
			break;
		case 1810:
			nErr = AdsSyncWriteControlReq (&Addr, ADSSTATE_STOP, 0, 0, NULL);
			cerr << "[TC:ADS] Device Stopped\n";
			nErr = 1;
			Sleep(1000);
			break;
		default:
			cerr << "[TC:ADS]Error: AdsSyncWriteControlReq: " << nErr << "\n"; // 기타 Error
			goto Error;
		}
	}
	
	// PLC-variable(MAIN.flag) Change Notification 시작
	nErr = AdsSyncAddDeviceNotificationReq(&Addr, ADSIGRP_SYM_VALBYHND, hUser, &adsNotificationAttrib, Callback, hUser, &hNotification);
	if (nErr){
		cerr << "[TC:ADS]Error: AdsSyncAddDeviceNotificationReq: " << nErr << "\n";
		goto Error;
	}
	else  cerr << "[TC:ADS] Notification Set\n";

	bProcessEnd = 0;
	pTCData->iNextOwner = 2;

	ReleaseMutex(hTCMutex);
	WaitForSingleObject(hTCMutex, INFINITY);
	while (pTCData->iNextOwner != 1) {
		ReleaseMutex(hTCMutex);
		WaitForSingleObject(hTCMutex, INFINITE);
	}

	// Label for error occurred
	Error:

	// PLC-variable Change Notification 정지
	nErr = AdsSyncDelDeviceNotificationReq(&Addr, hNotification);
	if (nErr) cerr << "[TC:ADS]Error: AdsSyncDelDeviceNotificationReq: " << nErr << "\n";
	else cerr << "[TC:ADS] Notification Off\n";

	// Stop Timer Module
	nErr = AdsSyncWriteControlReq (&Addr, ADSSTATE_STOP, 0, 0, pData);
	if(nErr) cerr << "[TC:ADS]Error: AdsSyncWriteControlReq: " << nErr << "\n";
	else cerr << "[TC:ADS] Device Stopped\n";
	
	// Release ADS Handle
	nErr = AdsSyncWriteReq(&Addr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(hUser), &hUser); 
	if (nErr) cerr << "[TC:ADS]Error: AdsSyncWriteReq: " << nErr << "\n";
	else cerr << "[TC:ADS] Data Handle Released\n";

	// Close the ADS Communication Port
	nErr = AdsPortClose();
	if (nErr) cerr << "[TC:ADS]Error: AdsPortClose: " << nErr << "\n";
	else cerr << "[TC:ADS] ADS Port Closed\n";
	
	// Handle 제거와 Process 종료
	pTCData->iNextOwner = 0;
	ReleaseMutex(hTCMutex);		// User process 종료할 수 있도록 Mutex Release
	CloseHandle(hTCMemory);
	CloseHandle(hTCMutex);
	
	return 0;
}

void __stdcall Callback(AmsAddr* pAddr, AdsNotificationHeader* pNotification, ULONG hUser)
{
	if (!bProcessEnd){
		WaitForSingleObject(hTCMutex, INFINITY);
		while (pTCData->iNextOwner != 2) {
			ReleaseMutex(hTCMutex);
			WaitForSingleObject(hTCMutex, INFINITE);
		}
	}

	if (pTCData->time >= ENDTIME){
		pTCData->bContinue = 0;
		pTCData->iNextOwner = 0;
		bProcessEnd = 1;
		ReleaseMutex(hTCMutex);
	}

	if (pTCData->bContinue){
		pTCData->time += TIMESTEP;
		pTCData->count++;
		pTCData->iNextOwner = 0;
		ReleaseMutex(hTCMutex);
	}
	else if (pTCData->bFirst){
		pTCData->iNextOwner = 0;
		ReleaseMutex(hTCMutex);
	}
	else{
		if(!bProcessEnd) bProcessEnd = 1;
	}
}