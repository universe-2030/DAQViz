#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include "LuMatchFdn.h"
#include "MatchDevice.h"

MatchDevice::MatchDevice() {
	for (int i = 0; i < SEN_NUM * 3; i++) {
		aEmg[i] = 0.0;
		aEuler[i] = 0.0;
		aAccel[i] = 0.0;
		aGyro[i] = 0.0;
		aMagnet[i] = 0.0;
	}

	for (int i = 0; i < SEN_NUM; i++) {
		aEmgAmpGain[i] = 0;
		aBattery[i] = 0;
	}
}

int MatchDevice::InitMATCH() {
	int rtn = 0;
	int devNum = 0;

	std::cout << "*Scanning for connected MATCH ... " << std::endl;
	devNum = LufScanSensor();
	std::cout << "Done!" << std::endl;

	if (devNum) {
		std::cout << "Found" << devNum << "MATCH" << std::endl;

		std::cout << "*Initialization of connected MATCH ... " << std::endl;
		rtn = LufInitDev(DEVICE_ID, SEN_NUM, SAMPLE_PERIOD_MS, BUF_SIZE, FLT_WIN_SIZE);
		std::cout << "Done!" << std::endl;

		if (!rtn) {
			std::cout << "Sampling Time [ms]: " << SAMPLE_PERIOD_MS << std::endl;
			std::cout << "Buffer Size: " << BUF_SIZE << std::endl;
			std::cout << "Filter Window Size: " << FLT_WIN_SIZE << std::endl;
		}
		else {
			std::cout << "Cannot Initialize LuFoundation API\nError: " << rtn << std::endl;
			return -1;
		}
	}
	else {
		std::cout << "Check the device connection!" << std::endl;
		std::cout << "Device Num : " << devNum << std::endl;
		return -1;
	}

	return 0;
}

int MatchDevice::GetDataAddress() {
	std::cout << "Get Sensor Data Address ... " << std::endl;

	pVid = (wchar_t *)LufGetVendorString(DEVICE_ID);
	pPid = (wchar_t *)LufGetProductString(DEVICE_ID);

	std::cout << "Done!" << std::endl;
	std::cout << "Vender: " << pVid << std::endl;
	std::cout << "Product: %S\n" << pVid << std::endl;
	std::cout << std::endl;

	return 0;
}

int MatchDevice::OpenMATCH() {
	int rtn = LufOpenDev(DEVICE_ID, SEN_NUM, SAMPLE_PERIOD_MS, BUF_SIZE, FLT_WIN_SIZE);
	if (rtn)
		std::cout << "Can not connect the device\nError code: " << rtn << std::endl;
	return rtn;
}

void MatchDevice::GetSensorData() {
	LufGetAllEmgAmpGain(DEVICE_ID, aEmgAmpGain, sizeof(aEmgAmpGain));
	LufGetAllEmg_V(DEVICE_ID, aEmg, sizeof(aEmg));
	LufGetAllEuler_deg(DEVICE_ID, aEuler, sizeof(aEuler));
	LufGetAllAccel_g(DEVICE_ID, aAccel, sizeof(aAccel));
	LufGetAllGyro_dps(DEVICE_ID, aGyro, sizeof(aGyro));
	LufGetAllMagnet_uT(DEVICE_ID, aMagnet, sizeof(aMagnet));
	LufGetAllBattery_pc(DEVICE_ID, aBattery, sizeof(aBattery));
}

void MatchDevice::CloseMATCH() {
	/* Release Memory with respect to device number */
	LufCloseDev(DEVICE_ID);

	/* Disconnect USB HID and Release Memory about all device */
	LufExitAllDev();
}

wchar_t* MatchDevice::Get_pVid() {
	return pVid;
}

wchar_t* MatchDevice::Get_pPid() {
	return pPid;
}

float MatchDevice::Get_aEmg(int index) {
	return aEmg[index];
}

float MatchDevice::Get_aEmg_aligned(int index) {
	return aEmg_aligned[index];
}

int MatchDevice::Get_aEmg_channel_idx(int index) {
	return aEmg_channel_idx[index];
}

float MatchDevice::Get_aEuler(int index) {
	return aEuler[index];
}

float MatchDevice::Get_aAccel(int index) {
	return aAccel[index];
}

float MatchDevice::Get_aGyro(int index) {
	return aGyro[index];
}

float MatchDevice::Get_aMagnet(int index) {
	return aMagnet[index];
}

int MatchDevice::Get_aEmgAmpGain(int index_ch) {
	return aEmgAmpGain[index_ch];
}

int MatchDevice::Get_aBattery(int index_ch) {
	return aBattery[index_ch];
}

void MatchDevice::sEMG_Align(int sEMG_ch_num) {
	aEmg_aligned = new float[3 * sEMG_ch_num];
	for (int i = 0; i < sEMG_ch_num; i++)
		for (int j = 0; j < 3; j++)
		aEmg_aligned[i * 3 + j] = aEmg[(aEmg_channel_idx[i] - 1) * 3 + j];
}