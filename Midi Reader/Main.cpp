#include <iostream>
#include <format>
#include <Windows.h>
#include <mmeapi.h>

enum MidiEvent
{
	NoteOff = 0x80,
	NoteOn = 0x90,
	PolyphonicAftertouch = 0xA0,
	ControlOrModeChange = 0xB0,
	ProgramChange = 0xC0,
	ChannelAftertouch = 0xD0,
	PitchBendChange = 0xE0,
	SystemExclusive = 0xF0,
};

struct MidiNote
{
	bool isOn;
	unsigned char number;
	unsigned char velocity;
};

struct MidiData
{
	unsigned char byte1;
	unsigned char byte2;
	unsigned char byte3;
	unsigned long timestamp;
};

void PrintMidiData(unsigned char midiByte1, unsigned char midiByte2, unsigned char midiByte3)
{
	std::cout << std::format("MIDI MSG {:x} {:x} {:x}", midiByte1, midiByte2, midiByte3) << std::endl;
}

void PrintMidiData(const MidiData &midiData)
{
	std::cout << std::format("MIDI MSG {:x} {:x} {:x}", midiData.byte1, midiData.byte2, midiData.byte3) << std::endl;
}

void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, MidiData* midiDataBuffer, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{

	switch (wMsg)
	{
		case MIM_CLOSE :
		{
			std::cout << "MIDI device closed" << std::endl;
			break;
		}
		case MIM_DATA :
		{
			static int i = 0;
			MidiData midiData = { 0 };

			midiDataBuffer[128].byte2 = 1;
			midiData.byte1 = (dwParam1 & 0xFF);
			midiData.byte2 = ((dwParam1 >> 8) & 0xFF);
			midiData.byte3 = ((dwParam1 >> 16) & 0xFF);
			midiData.timestamp = (unsigned long)dwParam2;
			memcpy(&midiDataBuffer[i], &midiData, sizeof(MidiData));
			i++;
			i %= 128;
			midiDataBuffer[128].byte1 = i;
			//PrintMidiData(midiByte1, midiByte2, midiByte3);
			break;
		}
		case MIM_ERROR :
		{
			std::cout << "MIM_ERROR" << std::endl;
			break;
		}
		case MIM_LONGDATA :
		{
			std::cout << "MIM_LONGDATA" << std::endl;
			break;
		}
		case MIM_LONGERROR :
		{
			std::cout << "MIM_LONGERROR" << std::endl;
			break;
		}
		case MIM_MOREDATA :
		{
			std::cout << "MIM MOREDATA" << std::endl;
			break;
		}
		case MIM_OPEN :
		{
			std::cout << "MIDI device opened" << std::endl;
			break;
		}

		default:
			return;
	}
}

// Scans for MIDI devices, returns device ID choosen by user
// if no devices are found retuns -1 
int ChooseDevice()
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

int main()
{

	HMIDIIN hmidiin;
	MMRESULT midiInOpenLog;
	MidiData* midiDataBuffer = new MidiData[129];
	midiDataBuffer[128].byte1 = 0; //byte1 keeps track of last updated variable in buffer
	midiDataBuffer[128].byte2 = 0; //byte2 checks if first variable in buffer was initialized (0 - false, other - true)
	unsigned char midiDataIndex = 0;
	unsigned char midiDataIndexDestination = 0;

	int deviceID = ChooseDevice();
	if (deviceID == -1)
		return -1;
	midiInOpenLog = midiInOpen(&hmidiin, deviceID, (DWORD_PTR)MidiInProc, (DWORD_PTR)midiDataBuffer, CALLBACK_FUNCTION);

	if(midiInOpenLog != MMSYSERR_NOERROR)
	{
		std::cout << "Failed to open MIDI port" << std::endl;
		return -1;
	}

	midiInStart(hmidiin);

	//Wait for fist midi command
	while (midiDataBuffer[128].byte2 == 0)
		continue;

	while (true)
	{
		if (GetAsyncKeyState('q'))
			break;
		
		if (midiDataIndex != midiDataIndexDestination)
		{
			//read event here
			PrintMidiData(midiDataBuffer[midiDataIndex]);
			midiDataIndex++;
			midiDataIndex %= 128;
		}

		/*for (int i = 0; i < 16; i++)
		{
			PrintMidiData(midiDataBuffer[i]);
		}
		system("cls");*/

		//std::cout << (int)midiDataIndex << " " << (int)midiDataIndexDestination << std::endl;
		midiDataIndexDestination = midiDataBuffer[128].byte1;
		//Sleep(50);
	}

	midiInStop(hmidiin);
	midiInClose(hmidiin);

	
}