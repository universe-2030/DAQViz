# DAQViz

RNB - MARCH online sEMG processing & visualization program

# Precautions

1. This program consists of two sessions : **'Training'** and **'Test'**
   - Training session can be executed in either online or offline.
   - Test session is executed in online. (If no sensor is available, demo-version is executed.)

2. Three kinds of sensors are available : **Frankfurt (LogonU)**, **Delsys**.

3. To acquire the motion information, **flex-sensor-equipped glove** can be selectively used with radio button.
   (To use the glove, NI board is necessary to get analog input data.)

4. **TwinCAT** is used as a Real-Time Operating System. (Frequency is set to 1 kHz corresponding to 1ms period.)

5. Setup tutorial is provided with manual.

# Prerequisites

  - TwinCAT (Setup file can be downloaded from 'https://www.beckhoff.com/')
  - Visual Studio 2019 with MFC
  - EMGworks (Delsys sEMG acquisition program, license necessary)
  - OpenGL (Real-time visualization tool, expected)

# Program structure

  - Main dialog (Main framework, child dialogs are selected here.)
  - Child dialogs (This dialog is for customization. **Each dialog can be designed for each user.**)
  - Signal processor (Basic signal processing tools such as **LPF**, **LPF-filtered derivative**, **Bayesian filter** is provided.)
  - NI AI & AO module (This is mainly for **Delsys** and **Flex sensor glove**.)
  - etc...
