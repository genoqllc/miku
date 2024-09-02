
#ifndef MIKU_UX_SCREENS_MIDI_EVENTS_SCREEN_HPP
#define MIKU_UX_SCREENS_MIDI_EVENTS_SCREEN_HPP

#include <vector>

#include "../Screen.hpp"
#include "../Display.hpp"

#include <stdexcept>

namespace miku::ux::screens {
    class MidiEventsScreen : public miku::ux::Screen {
        public:
            MidiEventsScreen(Display* display, data::State* state) : Screen(display, state, "ME") {
                this->dependencyFlags = (DependencyFlags)(this->dependencyFlags | DependencyFlags::MidiEvents);
            }

            void Render() {
                this->GetDisplay()->Fill();                
                
                char buffer[32];
                bool heartbeat = (bool)this->dataValues["MIDI_HEARTBEAT"];
                sprintf(buffer, "MIDI Events     %s", heartbeat ? "/" : "\\");
                this->GetDisplay()->DrawStringByRow(1, 0, buffer);

                sprintf(buffer, "Total Ev: %.1f", this->dataValues["MIDI_EVENT_COUNT"]);
                this->GetDisplay()->DrawStringByRow(2, 0, buffer);

                sprintf(buffer, "BufferEv: %d", this->midiEvents->GetNumElements());
                this->GetDisplay()->DrawStringByRow(3, 0, buffer);

                sprintf(buffer, "BPM:    %.1f", this->dataValues["MIDI_BPM"]);
                this->GetDisplay()->DrawStringByRow(4, 0, buffer);

                this->GetDisplay()->RequestInvalidate();
            }

            void BindMidiEventLog(daisy::FIFO<daisy::MidiEvent, 128>* midiEvents) {
                this->midiEvents = midiEvents;
            }
        private:
            daisy::FIFO<daisy::MidiEvent, 128>* midiEvents;
    };
}

#endif
