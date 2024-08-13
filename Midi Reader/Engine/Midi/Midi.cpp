#include "Midi.hpp"
#include <iostream>
#include <format>
#include <Windows.h>


Midi::Midi()
{
	midiDataBuffer = new Midi::Data[129];
	midiDataBuffer[128].byte1 = 0; //byte1 stores index of last updated variable in "midiDataBuffer"
	midiDataBuffer[128].byte2 = 0; //byte2 checks if first variable in "midiDataBuffer" was initialized (0 - false, other - true)
	isFirstEventInitialized = false;
	midiDataIndex = 0;
	midiDataIndexDestination = 0;
}

Midi::~Midi()
{
	InClose();
}

int Midi::InOpen()
{
	MMRESULT midiInOpenLog;
	MMRESULT midiStartLog;
	int deviceID = ChooseDevice();

	if (deviceID == -1)
		return -1;
	midiInOpenLog = midiInOpen(&handleMidiIn, deviceID, (DWORD_PTR)MidiInProc, (DWORD_PTR)midiDataBuffer, CALLBACK_FUNCTION);

	if (midiInOpenLog != MMSYSERR_NOERROR)
	{
		std::cout << "Failed to open MIDI input port" << std::endl;
		return -1;
	}

	midiStartLog = midiInStart(handleMidiIn);
	if (midiStartLog != MMSYSERR_NOERROR)
	{
		std::cout << "Failed to start MIDI input port" << std::endl;
		return -1;
	}

	return 1;
}

void Midi::InClose()
{
	midiInStop(handleMidiIn);
	midiInClose(handleMidiIn);
}

Midi::Data* Midi::GetMidiInData()
{
	midiDataIndexDestination = midiDataBuffer[128].byte1;
	isFirstEventInitialized = (bool)midiDataBuffer[128].byte2;

	if (!isFirstEventInitialized)
		return nullptr;

	if (midiDataIndex == midiDataIndexDestination)
		return nullptr;

	Midi::Data* midiData = (midiDataBuffer + midiDataIndex);
	midiDataIndex++;
	midiDataIndex %= 128;
	return midiData;
}

int Midi::ChooseDevice()
{
	int numOfDevices = (int)midiInGetNumDevs();

	MIDIINCAPS* pmic = new MIDIINCAPS;

	MMRESULT mmresult;

	int choice = 0;

	for (int i = 0; i < numOfDevices; i++)
	{
		mmresult = midiInGetDevCaps(i, pmic, sizeof(MIDIINCAPS));

		if (mmresult == MMSYSERR_BADDEVICEID)
			std::cout << "The specified device identifier is out of range." << std::endl;
		if (mmresult == MMSYSERR_INVALPARAM)
			std::cout << "The specified pointer or structure is invalid." << std::endl;
		if (mmresult == MMSYSERR_NODRIVER)
			std::cout << "The driver is not installed." << std::endl;
		if (mmresult == MMSYSERR_NOMEM)
			std::cout << "The system is unable to allocate or lock memory." << std::endl;

		std::cout << "[" << i << "] ";

		wchar_t name[32];

		for (int i = 0; i < 32; i++)
			name[i] = ((wchar_t)(pmic->szPname[i]));
		std::wcout << name << std::endl;
	}

	delete pmic;

	if (numOfDevices <= 0)
	{
		std::cout << "No MIDI devices detected" << std::endl;
		return -1;
	}

	std::cout << "choose MIDI device:";
	std::cin >> choice;

	if (choice < 0 || choice >= numOfDevices)
	{
		std::cout << "Device with this ID does not exist, choosing ID 0" << std::endl;
		choice = 0;
	}

	return choice;
}

static void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, Midi::Data* midiDataBuffer, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{

	switch (wMsg)
	{
	case MIM_CLOSE:
	{
		std::cout << "MIDI device closed" << std::endl;
		break;
	}
	case MIM_DATA:
	{
		static int i = 0;
		Midi::Data midiData = { 0 };

		midiData.byte1 = (dwParam1 & 0xFF);
		midiData.byte2 = ((dwParam1 >> 8) & 0xFF);
		midiData.byte3 = ((dwParam1 >> 16) & 0xFF);
		midiData.timestamp = (unsigned long)dwParam2;
		memcpy(&midiDataBuffer[i], &midiData, sizeof(Midi::Data));
		midiDataBuffer[128].byte2 = 1;
		i++;
		i %= 128;
		midiDataBuffer[128].byte1 = i;
		break;
	}
	case MIM_ERROR:
	{
		std::cout << "MIM_ERROR" << std::endl;
		break;
	}
	case MIM_LONGDATA:
	{
		std::cout << "MIM_LONGDATA" << std::endl;
		break;
	}
	case MIM_LONGERROR:
	{
		std::cout << "MIM_LONGERROR" << std::endl;
		break;
	}
	case MIM_MOREDATA:
	{
		std::cout << "MIM MOREDATA" << std::endl;
		break;
	}
	case MIM_OPEN:
	{
		std::cout << "MIDI device opened" << std::endl;
		break;
	}

	default:
		return;
	}
}
