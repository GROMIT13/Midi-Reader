#include <iostream>
#include <format>
#include <vector>
#include <Windows.h>
#include "Engine/GConsole.hpp"
#include "Engine/Midi/Midi.hpp"

int main()
{
	GConsole screen("MIDI Reader", 120, 20, 15, 30, L"DejaVU Sans Mono");
	Midi midi;
	PianoKeyColor pianoColor = { FG_COLOR_WHITE, FG_COLOR_AQUA, FG_COLOR_BLACK, FG_COLOR_BLUE };
	Midi::Note* notes = new Midi::Note[128];
	std::vector<unsigned char> activeNoteList; //value represents note number that is currently on
	activeNoteList.reserve(128);
	//std::cout << activeNoteList.size();


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

		if ((midiData->byte1 & 0xF0) == Midi::Event::NoteOff)
		{
			if(midi.GetIsSustainPedalPressed())
				notes[midiData->byte2].state = Midi::State::Sustain;
			else
			{
				notes[midiData->byte2].state = Midi::State::Off;
				for (int i = 0; i < activeNoteList.size(); i++)
					if (activeNoteList[i] == midiData->byte2)
					{
						activeNoteList.erase(activeNoteList.begin() + i);
						screen.DrawString(3, 1 + activeNoteList.size(), "   ");
						break;
					}

			}	

			notes[midiData->byte2].velocity = midiData->byte3;
		}
		else if ((midiData->byte1 & 0xF0) == Midi::Event::NoteOn)
		{
			notes[midiData->byte2].state = Midi::State::On;
			notes[midiData->byte2].velocity = midiData->byte3;

			if (activeNoteList.size() == 0)
				activeNoteList.push_back(midiData->byte2);
			else
			{
				int i;
				for (i = 0; i < activeNoteList.size(); i++)
				{
					if (activeNoteList[i] < midiData->byte2)
					{
						activeNoteList.insert(activeNoteList.begin() + i, midiData->byte2);
						break;
					}
				}
				if (i == activeNoteList.size())
					activeNoteList.push_back(midiData->byte2);
			}
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
				for (int i = 0; i < activeNoteList.size(); i++)
				{
					if (notes[activeNoteList[i]].state == Midi::State::Sustain)
					{
						activeNoteList.erase(activeNoteList.begin() + i);
						screen.DrawString(3, 1 + activeNoteList.size(), "   ");
						i--;
					}
				}
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

		for (int i = 0; i < activeNoteList.size(); i++)
			screen.DrawNoteName(3, 1 + i, activeNoteList[i], FG_COLOR_WHITE);

		screen.UpdateScreen();
	}

}
