#include <iostream>
#include <format>
#include <Windows.h>
#include "Engine/GConsole.hpp"
#include "Engine/Midi/Midi.hpp"

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

void PrintData(unsigned char midiByte1, unsigned char midiByte2, unsigned char midiByte3)
{
	std::cout << std::format("MIDI MSG {:x} {:x} {:x}", midiByte1, midiByte2, midiByte3) << std::endl;
}

void PrintData(const Midi::Data &midiData)
{
	std::cout << std::format("MIDI MSG {:x} {:x} {:x}", midiData.byte1, midiData.byte2, midiData.byte3) << std::endl;
}

int main()
{
	GConsole screen("MIDI Reader",120,20,10,20);
	Midi midi;

	screen.ShowConsoleCursor(true);

	if (midi.InOpen() == -1) {
		std::cin.get();
		return 0;
	}

	screen.ShowConsoleCursor(false);

	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		Midi::Data* midiData = midi.GetMidiInData();

		if (midiData == nullptr)
			continue;
		

		screen.UpdateScreen();
	}

}
