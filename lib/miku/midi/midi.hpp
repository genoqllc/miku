#ifndef MIKU_MIDI_MIDI_HPP
#define MIKU_MIDI_MIDI_HPP

#include <string.h>
#include <string>
#include "daisy_seed.h"

namespace miku::midi {

    const std::string notes[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

    std::string GetNameFromNoteNumber(int note) {
        return notes[note % 12] + std::to_string(note / 12 - 1);
    }

    std::string GetMidiTypeAsString(daisy::MidiEvent& msg, char* str)
    {
        switch(msg.type)
        {
            case daisy::NoteOff: strcpy(str, "NtOf"); break;
            case daisy::NoteOn: strcpy(str, "NtOn"); break;
            case daisy::PolyphonicKeyPressure: strcpy(str, "PolyPres"); break;
            case daisy::ControlChange: strcpy(str, "CC"); break;
            case daisy::ProgramChange: strcpy(str, "PgChg"); break;
            case daisy::ChannelPressure: strcpy(str, "ChPres"); break;
            case daisy::PitchBend: strcpy(str, "PBend"); break;
            case daisy::SystemCommon: strcpy(str, "SysCM"); break;
            case daisy::SystemRealTime: strcpy(str, "SysRT"); break;
            case daisy::ChannelMode: strcpy(str, "ChMd"); break;
            default: strcpy(str, "Unk"); break;
        }

        return str;
    }
}


#endif
