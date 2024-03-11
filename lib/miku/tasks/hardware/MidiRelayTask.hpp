#ifndef MIKU_TASKS_HARDWARE_MIDIRELAYTASK_HPP
#define MIKU_TASKS_HARDWARE_MIDIRELAYTASK_HPP

#include <vector>

#include "../Task.hpp"
#include "daisy_seed.h"

namespace miku::tasks::hardware {
    const int MIDI_PPQN = 24;

    class MidiRelayTask : public miku::tasks::Task {
        public:
            MidiRelayTask(daisy::DaisySeed hardware, unsigned short rxPin, unsigned short txPin) : Task(hardware, "MIDI", 0L) {
                this->rxPin = rxPin;
                this->txPin = txPin;

                this->dependenciesProvided = (DependencyFlags)(this->dependenciesProvided | DependencyFlags::MidiEvents);

                this->Init();
            }

            void Init() {
                daisy::MidiUartHandler::Config midiConfig;
                midiConfig.transport_config.periph = daisy::UartHandler::Config::Peripheral::USART_1;
                midiConfig.transport_config.rx.pin = this->hardware.GetPin(this->rxPin).pin;
                midiConfig.transport_config.tx.pin = this->hardware.GetPin(this->txPin).pin;

                this->midiHandler.Init(midiConfig);
                this->midiHandler.StartReceive();

                std::vector<uint8_t> bytes = {
                    0xF0, 0x43, 0x79, 0x09, 0x00, 0x50, // header
                    0x10, // mode
                    0x6B, 0x20, 0x61, 0x2C, 0x34, 0x20, 0x61, 0x2C, 0x61, 0x2C, 0x67, 0x20, 0x65, // ka ra a ge
                    // 0x61, 0x2C, 0x65, 0x2C, 0x69, 0x2C, 0x6F, 0x2C, 0x4D, // a e i o u, 0x2C = comma/delimiter
                    // 0x68, 0x20, 0x61, 0x2C, 0x74, 0x73, 0x20, 0x4D, 0x2C, 0x6E, 0x20, 0x65, 0x2C, 0x6D, 0x27, 0x20, 0x69, 0x2C, 0x6B, 0x20, 0x4D, 0x2C, 0x64, 0x20, 0x65, 0x2C, 0x73, 0x20, 0x4D, // hatsune miku desu
                    0x00, 0xF7  // footer
                };

                this->midiHandler.SendMessage(bytes.data(), bytes.size());
            }

            void Execute() {
                while(this->midiHandler.HasEvents()) {
                    this->totalEventCount++;
                    daisy::MidiEvent msg = this->midiHandler.PopEvent();

                    switch(msg.type)
                    {
                        // TODO properly calculate channel based on incoming message
                        case daisy::MidiMessageType::NoteOn:
                            {
                                uint8_t bytes[3] = {0x90, 0x00, 0x00};
                                bytes[1] = msg.data[0];
                                bytes[2] = msg.data[1];
                                this->midiHandler.SendMessage(bytes, 3);
                            }
                            break;
                        case daisy::MidiMessageType::NoteOff:
                            {
                                uint8_t bytes[3] = {0x80, 0x00, 0x00};
                                bytes[1] = msg.data[0];
                                bytes[2] = msg.data[1];
                                this->midiHandler.SendMessage(bytes, 3);
                            }
                            break;
                        case daisy::MidiMessageType::SystemRealTime:
                            {
                                if (msg.srt_type == daisy::SystemRealTimeType::TimingClock) {
                                    this->clockEventsReceived++;
                                    if (this->clockEventsReceived >= MIDI_PPQN) {
                                        this->clockEventsReceived = 0;
                                        uint32_t now = daisy::System::GetNow();
                                        uint32_t diff = now - this->lastClockTime;
                                        float bpm = 60000.0f / diff;
                                        this->dataValues["MIDI_BPM"] = bpm;
                                        this->lastClockTime = now;
                                    }
                                }
                            }
                            break;
                        default: break;
                    }

                    /** Regardless of message, let's add the message data to our queue to output */
                    this->midiEventLog.PushBack(msg);
                }

                this->dataValues["MIDI_EVENT_COUNT"] = (float)this->totalEventCount;
            }

            bool TimerLapsed() {
                // **Always** process MIDI evens on every loop
                return true;
            }

            daisy::FIFO<daisy::MidiEvent, 128>* GetMidiEventLog() {
                return &this->midiEventLog;
            }
        private:
            daisy::MidiUartHandler midiHandler;
            daisy::FIFO<daisy::MidiEvent, 128> midiEventLog;
            unsigned short rxPin = 0;
            unsigned short txPin = 0;

            unsigned long totalEventCount = 0;

            uint32_t lastClockTime = 0;
            uint32_t clockEventsReceived = 0;
    };
}
#endif
