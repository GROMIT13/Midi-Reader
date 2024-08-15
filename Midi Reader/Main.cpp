#include <iostream>
#include <format>
#include <Windows.h>
#include "Engine/GConsole.hpp"
#include "Engine/Midi/Midi.hpp"

enum class Accidental
{
	Flat = 0x266D,
	Natural = 0x266E,
	Sharp = 0x266F
};

int main()
{
	GConsole screen("MIDI Reader", 120, 20, 15, 30, L"DejaVU Sans Mono");
	Midi midi;
	PianoKeyColor pianoColor = { FG_COLOR_WHITE, FG_COLOR_AQUA, FG_COLOR_BLACK, FG_COLOR_BLUE };
	Midi::Note* notes = new Midi::Note[128];


	screen.ShowConsoleCursor(true);

	if (midi.InOpen() == -1) 
	{
		std::cin.get();
		return 0;
	}

	screen.ShowConsoleCursor(false);
	screen.Fill(CHARACTER_FULL, FG_COLOR_BLACK);
	screen.DrawPiano(7, screen.GetDimension().y - 5, 7, pianoColor);
	screen.UpdateScreen();

	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		Midi::Data* midiData = midi.GetMidiInData();

		if (midiData == nullptr)
			continue;
		midi.PrintData(*midiData);

		if ((midiData->byte1 & 0xF0) == Midi::Event::NoteOff)
		{

		}
		else if ((midiData->byte1 & 0xF0) == Midi::Event::NoteOn)
		{
		
		}
		else if ((midiData->byte1 & 0xF0) == Midi::Event::PolyphonicAftertouch)
		{

		}
		else if ((midiData->byte1 & 0xF0) == Midi::Event::ControlOrModeChange)
		{
			midi.SetIsSustainPedalPressed(midiData->byte2, midiData->byte3);
			if (midi.GetIsSustainPedalPressed())
			{

			}
			else
			{

			}
		}
		else if ((midiData->byte1 & 0xF0) == Midi::Event::ProgramChange)
		{

		}
		else if ((midiData->byte1 & 0xF0) == Midi::Event::ChannelAftertouch)
		{

		}
		else if ((midiData->byte1 & 0xF0) == Midi::Event::PitchBendChange)
		{

		}
		else if ((midiData->byte1 & 0xF0) == Midi::Event::SystemExclusive)
		{

		}

		screen.UpdateScreen();
	}

}
