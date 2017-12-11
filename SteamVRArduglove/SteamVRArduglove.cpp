// SteamVRArduglove.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include "fileapi.h"
#include "vrinputemulator.h"
#include "vrinputemulator_types.h"

#include "openvr.h"
#include <iostream>
using namespace std;

int main()
{
	HANDLE hComm;
	LPCWSTR a = L"COM3";
	hComm = CreateFile(a,
		GENERIC_READ,
		0,
		0,
		OPEN_EXISTING,
		0,
		NULL);
	DCB config = { 0 };
	config.DCBlength = sizeof(config);

	config.BaudRate = 38400;
	config.StopBits = ONESTOPBIT;
	config.Parity = PARITY_NONE;
	config.ByteSize = DATABITS_8;
	config.fDtrControl = 0;
	config.fRtsControl = 0;
	char buffer[5];
	DWORD read;
	if (!SetCommState(hComm, &config))
	{

		printf("Failed to Set Comm State Reason: %d\n", GetLastError());
		//return E_FAIL;
	}
	printf("Current Settings\n Baud Rate %d\n Parity %d\n Byte Size %d\n Stop Bits %d", config.BaudRate,
		config.Parity, config.ByteSize, config.StopBits);

	vr::EVRInitError vrInitError;
	vr::VR_Init(&vrInitError, vr::EVRApplicationType::VRApplication_Background);
	if (vrInitError != vr::VRInitError_None) {
		std::cout << "OpenVR error: " << vr::VR_GetVRInitErrorAsEnglishDescription(vrInitError) << std::endl;
		exit(2);
	}

	int glove_id = 0;
	string expected_glove_serial = "LHR-FFD71F47";

	char serial[1028] = { '\0' };

	while (glove_id == 0)
	{
		for (int i = 0; i < vr::k_unMaxTrackedDeviceCount; ++i) {
			auto deviceClass = vr::VRSystem()->GetTrackedDeviceClass(i);
			if (deviceClass != vr::TrackedDeviceClass_Invalid) {
				vr::ETrackedPropertyError pError;
				vr::VRSystem()->GetStringTrackedDeviceProperty(i, vr::Prop_SerialNumber_String, serial, 1028, &pError);
				if (string(serial).compare(expected_glove_serial))
				{
					glove_id = i;
				}
			}
		}
	}

	std::cout << "Found glove: " << serial << " " << glove_id;

	int n_fingers = 4;
	bool finger[4] = { false, false, false, false };

	vrinputemulator::VRInputEmulator vr_emu;
	vr_emu.connect();
	while (true)
	{
		ReadFile(hComm, buffer, sizeof(buffer), &read, NULL);

		//printf("%c %c %c %c\n", buffer[0], buffer[1], buffer[2], buffer[3]);
		
		if (finger[0]==false && buffer[0] == '1')
		{
			vr::VRControllerAxis_t pressed_trig = { 1.0, 1.0 };
			vr_emu.openvrButtonEvent(vrinputemulator::ButtonEventType::ButtonPressed, glove_id, vr::EVRButtonId::k_EButton_SteamVR_Trigger);
			vr_emu.openvrAxisEvent(glove_id, vr::EVRControllerAxisType::k_eControllerAxis_Trigger, pressed_trig);
			
			finger[0] = true;
			printf("sending trigger press");
		}
		if (finger[0] == true && buffer[0] == '0')
		{
			vr::VRControllerAxis_t unpressed_trig = { 0.0, 0 };

			vr_emu.openvrButtonEvent(vrinputemulator::ButtonEventType::ButtonUnpressed, glove_id, vr::EVRButtonId::k_EButton_SteamVR_Trigger);
			vr_emu.openvrAxisEvent(glove_id, vr::EVRControllerAxisType::k_eControllerAxis_Trigger, unpressed_trig);

			finger[0] = false;
			printf("sending trigger unpress");
		}


		if (finger[1] == false && buffer[1] == '1')
		{
			vr::VRControllerAxis_t trackpad_up = { 0.0, 1.0 };
			vr_emu.openvrButtonEvent(vrinputemulator::ButtonEventType::ButtonPressed, glove_id, vr::EVRButtonId::k_EButton_SteamVR_Touchpad);
			vr_emu.openvrAxisEvent(glove_id, vr::EVRControllerAxisType::k_eControllerAxis_TrackPad, trackpad_up);

			finger[1] = true;
			printf("sending trigger press");
		}
		if (finger[1] == true && buffer[1] == '0')
		{
			vr::VRControllerAxis_t trackpad_center = { 0.0, 0.0 };

			vr_emu.openvrButtonEvent(vrinputemulator::ButtonEventType::ButtonUnpressed, glove_id, vr::EVRButtonId::k_EButton_SteamVR_Touchpad);
			vr_emu.openvrAxisEvent(glove_id, vr::EVRControllerAxisType::k_eControllerAxis_TrackPad, trackpad_center);

			finger[1] = false;
			printf("sending trigger unpress");
		}



		if (finger[2] == false && buffer[2] == '1')
		{
			vr::VRControllerAxis_t trackpad_down = { 0.0, -1.0 };
			vr_emu.openvrButtonEvent(vrinputemulator::ButtonEventType::ButtonPressed, glove_id, vr::EVRButtonId::k_EButton_SteamVR_Touchpad);
			vr_emu.openvrAxisEvent(glove_id, vr::EVRControllerAxisType::k_eControllerAxis_TrackPad, trackpad_down);

			finger[2] = true;
			printf("sending trigger press");
		}
		if (finger[2] == true && buffer[2] == '0')
		{
			vr::VRControllerAxis_t trackpad_center = { 0.0, 0.0 };

			vr_emu.openvrButtonEvent(vrinputemulator::ButtonEventType::ButtonUnpressed, glove_id, vr::EVRButtonId::k_EButton_SteamVR_Touchpad);
			vr_emu.openvrAxisEvent(glove_id, vr::EVRControllerAxisType::k_eControllerAxis_TrackPad, trackpad_center);

			finger[2] = false;
			printf("sending trigger unpress");
		}


		if (finger[3] == false && buffer[3] == '1')
		{
			vr_emu.openvrButtonEvent(vrinputemulator::ButtonEventType::ButtonPressed, glove_id, vr::EVRButtonId::k_EButton_Grip);

			finger[3] = true;
			printf("sending trigger press");
		}
		if (finger[3] == true && buffer[3] == '0')
		{

			vr_emu.openvrButtonEvent(vrinputemulator::ButtonEventType::ButtonUnpressed, glove_id, vr::EVRButtonId::k_EButton_Grip);

			finger[3] = false;
			printf("sending trigger unpress");
		}





	}
	
    return 0;
}

