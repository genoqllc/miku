#ifndef MIKU_TASKS_HARDWARE_MIDIRELAYTASK_HPP
#define MIKU_TASKS_HARDWARE_MIDIRELAYTASK_HPP

#include <vector>

#include "../Task.hpp"
#include "daisy_seed.h"
#include "MidiHardware.hpp"

namespace miku::tasks::hardware {
    const int MIDI_PPQN = 24;

    class MidiRelayTask : public miku::tasks::Task {
        public:
            MidiRelayTask(daisy::DaisySeed hardware, data::State* state, miku::tasks::hardware::MidiHardware* midiHardware) : Task(hardware, state, "MIDI", 0L) {
                this->dependenciesProvided = (DependencyFlags)(this->dependenciesProvided | DependencyFlags::MidiEvents);

                this->midiHardware = midiHardware;

                this->Init();
            }

            void Init() {
                // TODO Replace this fake Sysex with real stuff from the other Tasks.
                std::vector<uint8_t> bytes = {
                    0xF0, 0x43, 0x79, 0x09, 0x00, 0x50, // header
                    0x10, // mode
                    0x6B, 0x20, 0x61, 0x2C, 0x34, 0x20, 0x61, 0x2C, 0x61, 0x2C, 0x67, 0x20, 0x65, // ka ra a ge
                    // 0x61, 0x2C, 0x65, 0x2C, 0x69, 0x2C, 0x6F, 0x2C, 0x4D, // a e i o u, 0x2C = comma/delimiter
                    // 0x68, 0x20, 0x61, 0x2C, 0x74, 0x73, 0x20, 0x4D, 0x2C, 0x6E, 0x20, 0x65, 0x2C, 0x6D, 0x27, 0x20, 0x69, 0x2C, 0x6B, 0x20, 0x4D, 0x2C, 0x64, 0x20, 0x65, 0x2C, 0x73, 0x20, 0x4D, // hatsune miku desu
                    0x00, 0xF7  // footer
                };

                this->midiHardware->SendMessage(bytes);
            }

            void Execute() {
                uint32_t now = daisy::System::GetNow();

                // TODO make this a quarter note based on incoming BPM
                if (now - this->lastHeartbeatTime >= 500) {
                    this->lastHeartbeatTime = now;
                    this->heartbeat = !this->heartbeat;
                    this->dataValues["MIDI_HEARTBEAT"] = this->heartbeat;
                }

                while(this->midiHardware->HasEvents()) {
                    this->totalEventCount++;
                    daisy::MidiEvent msg = this->midiHardware->PopEvent();

                    this->HandleMessage(msg, now);
                }

                this->dataValues["MIDI_EVENT_COUNT"] = (float)this->totalEventCount;
            }

            /// @brief Internal handler for MIDI messages.
            /// @param msg The raw MIDI message receieved.
            /// @param eventTime When the message was recieved.
            void HandleMessage(daisy::MidiEvent &msg, uint32_t eventTime)
            {
                // TODO we could probably just treat this like MIDI thru and ship everything.
                switch (msg.type)
                {
                    // TODO properly calculate channel based on incoming message
                        case daisy::MidiMessageType::NoteOn:
                        {
                            uint8_t bytes[3] = {0x90, 0x00, 0x00};
                            bytes[1] = msg.data[0];
                            bytes[2] = msg.data[1];
                            this->midiHardware->SendMessage(bytes, 3);
                        }
                        break;
                    case daisy::MidiMessageType::NoteOff:
                        { 
                            uint8_t bytes[3] = {0x80, 0x00, 0x00};
                            bytes[1] = msg.data[0];
                            bytes[2] = msg.data[1];
                            this->midiHardware->SendMessage(bytes, 3);
                        }
                        break;
                    case daisy::MidiMessageType::SystemRealTime:
                        {
                            if (msg.srt_type == daisy::SystemRealTimeType::TimingClock)
                            {
                                this->clockEventsReceived++;
                                if (this->clockEventsReceived >= MIDI_PPQN)
                                {
                                    this->clockEventsReceived = 0;                                
                                    uint32_t diff = eventTime - this->lastBpmClockTime;
                                    this->bpm = 60000.0f / diff;
                                    this->dataValues["MIDI_BPM"] = this->bpm;
                                    this->lastBpmClockTime = eventTime;
                                }
                            }
                        }
                        break;
                    default:
                        break;
                }

                /** Regardless of message, let's add the message data to our queue to output */
                this->midiEventLog.PushBack(msg);
            }

            bool TimerLapsed() {
                // **Always** process MIDI events on every loop
                return true;
            }

            daisy::FIFO<daisy::MidiEvent, 128>* GetMidiEventLog() {
                return &this->midiEventLog;
            }
        private:
            MidiHardware* midiHardware;
            daisy::FIFO<daisy::MidiEvent, 128> midiEventLog;
            unsigned short rxPin = 0;
            unsigned short txPin = 0;

            float bpm = 100.0f;

            unsigned long totalEventCount = 0;

            uint32_t lastBpmClockTime = 0;
            uint32_t clockEventsReceived = 0;

            uint32_t lastHeartbeatTime = 0;

            bool heartbeat = false;
    };
}
#endif
