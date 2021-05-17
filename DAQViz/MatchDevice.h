#pragma once

#define DEVICE_ID			(0)		/*!< Set device(MATCH Cradle) ID. Default value is 0.*/
#define SEN_NUM				(8)		/*!< Total sensor number. Default value is 8.*/
#define SEMG_CH_NUM			(6)		/*!< The number of sensors used*/
#define SAMPLE_PERIOD_MS	(1)		/*!< Data sampling time from MATCH.\n minimum value is 1.\n Unit: milisencond.*/
#define SAMPLE_FREQ			(1000.0f / SAMPLE_PERIOD_MS) /*!< Data sampling frequency.\n Unit: Hz*/
#define BUF_SIZE			(100)	/*!< Sensor data buffer for use in the LuFoundation Library.*/
#define	FLT_WIN_SIZE		(100)	/*!< Low pass filter window size for sEMG signal processing.*/
#define SCREEN_REFRESH_MS	(1)		/*!< Screen refresh rate.\n Unit: milisecond*/

class MatchDevice {
private:
	wchar_t *pVid;				/*!< This pointer variable refer to vendor name address in the LuFoundation Library.*/
	wchar_t *pPid;				/*!< This pointer variable refer to product name address in the LuFoundation Library.*/
	float aEmg[SEN_NUM * 3];		/*!< sEMG data\n e.g. aEMG[0], aEMG[1] and aEMG[2] represent the raw data, offseted and absoluted data, filtered data of the first MATCH sensor, respectively.*/
	float* aEmg_aligned;
	int	  aEmg_channel_idx[SEMG_CH_NUM] = { 2, 3, 4, 5, 6, 8 };
	float aEuler[SEN_NUM * 3];	/*!< euler data\n e.g. aEuler[0], aEuler[1] and aEuler[2] represent the Euler angles of the x, y and z axes of the first MATCH sensor, respectively.*/
	float aAccel[SEN_NUM * 3];	/*!< accelerometer data\n e.g. aAccel[0], aAccel[1], aAccel[2] represent the accelerometer data of the x, y and z axes of the first MATCH sensor, respectively.*/
	float aGyro[SEN_NUM * 3];	/*!< gyroscope data\n e.g. aGyro[0], aGyro[1], aGyro[2] represent the gyroscope data of the x, y and z axes of the first MATCH sensor, respectively.*/
	float aMagnet[SEN_NUM * 3];	/*!< magnetometer data\n e.g. aMagnet[0], aMagnet[1], aMagnet[2] represent the magnetometer data of the x, y and z axes of the first MATCH sensor, respectively.*/
	int   aEmgAmpGain[SEN_NUM];	/*!< sEMG Amp. gain data\n e.g. aEmgAmpGain[0] represents the amplifier gain of the first MATCH sensor.*/
	int   aBattery[SEN_NUM];		/*!< battery infomation data\n e.g. aBattery[0] represents the battery data of the first MATCH sensor.*/

public:
	MatchDevice();

	/*!
	\brief Initialization for LuFoundation Library.
	\details
	This function Initialize LuFoundation Library for using MATCH.
	\return 0 in case of success, else < 0
	*/
	int InitMATCH();

	/*!
	\brief Mapping the MATCH sensors's data address to the user pointer arrays.
	\details
	This function gets the address of the sensor data to be referenced by the pointer variable in the LuFoundation library.\n
	It also shows the address of the sensor data in the console window.
	\return 0 in case of success, else < 0
	*/
	int GetDataAddress();

	/*!
	\brief Connect the MATCH.
	\details
	This function connect the MATCH to get the sensor data.
	\return 0 in case of success, else error code
	\sa #LuFoundationEAGM::LuFdnErr
	*/
	int OpenMATCH();

	/*!
	\brief Sensor data acquisition and display.
	\details
	This function displays each sensor data acquired by the LuFoundation Library in the console window.
	*/
	void GetSensorData();

	/*!
	\brief Close the MATCH.
	\details
	This function close and disconnect the MATCH
	*/
	void CloseMATCH();

	/*!
	\brief Check the keyboard input and execute command.
	\details
	Command list is as follows.\n
	'a' : Get Accelerometer Data @ Opt1 Space\n
	'A' : Get Accelerometer Data @ Opt2 Space\n
	'g' : Get Gyroscope Data @ Opt1 Space\n
	'G' : Get Gyroscope Data @ Opt2 Space\n
	'm' : Get Magnetometer Data @ Opt1 Space\n
	'M' : Get Magnetometer Data @ Opt2 Space\n
	's' : Start Sensor Data Save\n
	'S' : Stop Sensor Data Save\n
	'x' : exit\n
	\return 0 in case of success, else < 0
	*/

	void sEMG_Align(int sEMG_ch_num);

	wchar_t* Get_pVid();
	wchar_t* Get_pPid();
	float Get_aEmg(int index);
	float Get_aEmg_aligned(int index);
	float Get_aEuler(int index);
	float Get_aAccel(int index);
	float Get_aGyro(int index);
	float Get_aMagnet(int index);
	int   Get_aEmgAmpGain(int index_ch);
	int   Get_aBattery(int index_ch);
};