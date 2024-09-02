
#ifndef MIKU_TASKS_HARDWARE_MIDIHARDWARE_HPP
#define MIKU_TASKS_HARDWARE_MIDIHARDWARE_HPP

#include "daisy_seed.h"
#include "../../data/State.hpp"

namespace miku::tasks::hardware {
    //static MidiHardware *TheMidiHardware = nullptr;

    class MidiHardware {
        public:
            MidiHardware(data::State* state, daisy::DaisySeed hardware, unsigned short rxPin, unsigned short txPin) {
                this->rxPin = rxPin;
                this->txPin = txPin;

                this->Init();
            }

            void Init() {
                daisy::MidiUartHandler::Config midiConfig;
                midiConfig.transport_config.periph = daisy::UartHandler::Config::Peripheral::USART_1;
                midiConfig.transport_config.rx.pin = this->hardware.GetPin(this->rxPin).pin;
                midiConfig.transport_config.tx.pin = this->hardware.GetPin(this->txPin).pin;

                this->midiHandler.Init(midiConfig);
                this->midiHandler.StartReceive();

                // TODO Replace this fake Sysex with real stuff from the other Tasks.
                std::vector<uint8_t> bytes = {
                    0xF0, 0x43, 0x79, 0x09, 0x00, 0x50, // header
                    0x10, // mode
                    0x6B, 0x20, 0x61, 0x2C, 0x34, 0x20, 0x61, 0x2C, 0x61, 0x2C, 0x67, 0x20, 0x65, // ka ra a ge
                    // 0x61, 0x2C, 0x65, 0x2C, 0x69, 0x2C, 0x6F, 0x2C, 0x4D, // a e i o u, 0x2C = comma/delimiter
                    // 0x68, 0x20, 0x61, 0x2C, 0x74, 0x73, 0x20, 0x4D, 0x2C, 0x6E, 0x20, 0x65, 0x2C, 0x6D, 0x27, 0x20, 0x69, 0x2C, 0x6B, 0x20, 0x4D, 0x2C, 0x64, 0x20, 0x65, 0x2C, 0x73, 0x20, 0x4D, // hatsune miku desu
                    0x00, 0xF7  // footer
                };

                this->SendMessage(bytes);

                //TheMidiHardware = this;
            }

            void SendSyllableSysex(std::vector<std::string> syllables) {
                std::vector<uint8_t> bytes = {
                    0xF0, 0x43, 0x79, 0x09, 0x00, 0x50, // header
                    0x10, // mode
                };

                // Syllables
                for (std::string syllable : syllables) {
                    for (char c : syllable) {
                        bytes.push_back(c);
                        bytes.push_back(0x20);
                    }
                }

                // Footer
                bytes.push_back(0x00);
                bytes.push_back(0xF7);

                this->SendMessage(bytes);
            }

            void SendMessage(std::vector<uint8_t> &bytes)
            {
                this->midiHandler.SendMessage(bytes.data(), bytes.size());
            }

            void SendMessage(uint8_t* bytes, size_t size)
            {
                this->midiHandler.SendMessage(bytes, size);
            }

            bool HasEvents() {
                return this->midiHandler.HasEvents();
            }

            daisy::MidiEvent PopEvent() {
                return this->midiHandler.PopEvent();
            }

        private:
            daisy::DaisySeed hardware;
            short adcPin = -1;
            short adcChannelIndex = -1;
            daisy::MidiUartHandler midiHandler;
            unsigned short rxPin = 0;
            unsigned short txPin = 0;
    };
}

#endif