#pragma once

#include <Windows.h>
#include <iostream>
#include <format>
#include <Windows.h>

struct PianoKeyColor
{
	unsigned short whiteRelased;
	unsigned short whitePressed;
	unsigned short blackRelased;
	unsigned short blackPressed;
};

class Midi
{
public:
	enum Event
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

	enum class State : unsigned char
	{
		Off = 0,
		On = 1,
		Sustain = 2
	};

	struct Data
	{
		unsigned char byte1;
		unsigned char byte2;
		unsigned char byte3;
		unsigned long timestamp;
	};

	struct Note
	{
		State state;
		unsigned char velocity;

		Note();
	};

private:
	HMIDIIN handleMidiIn;
	Midi::Data* midiDataBuffer;
	bool isFirstEventInitialized; // check if data was initialized in first element of "midiDataBuffer"
	unsigned char midiDataIndex;  // stores index of last read midi event + 1
	unsigned char midiDataIndexDestination; // stores index of last updated variable in "midiDataBuffer"
	bool isSustainPedalPressed;

public:
	Midi();
	~Midi();

	// Open MIDI input device specified by user
	// Retuns -1 on error
	int InOpen();
	
	// Closes current MIDI input device
	// NOTE : This function is called on class destruction
	void InClose();

	// Return pointer to last occurred midi input event
	// if multiple events occur use this function multiple times
	// if all Data was read this functions return nullptr
	Midi::Data* GetMidiInData();

	void PrintData(unsigned char midiByte1, unsigned char midiByte2, unsigned char midiByte3);
	void PrintData(const Midi::Data& midiData);

	bool GetIsSustainPedalPressed();

	// Use only when you know (midiData.byte1 & 0xF0) == Midi::Event::ControlOrModeChange
	void SetIsSustainPedalPressed(unsigned char midiDataByte2, unsigned char midiDataByte3);

private:
	// Scans for MIDI devices, returns device ID choosen by user
	// if no devices are found retuns -1 
	int ChooseDevice();
};

static void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, Midi::Data* midiDataBuffer, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
