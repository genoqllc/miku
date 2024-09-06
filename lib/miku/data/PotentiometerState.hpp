
#ifndef MIKU_DATA_POTENTIOMETERSTATE_HPP
#define MIKU_DATA_POTENTIOMETERSTATE_HPP

#include "daisy_seed.h"
#include <string>

namespace miku::data {
    class PotentiometerState {
        public:
            PotentiometerState(std::string code, uint8_t pinNumber) {
                Code = code;
                PinNumber = pinNumber;
            }

            std::string Code;
            uint8_t PinNumber = 0;
            uint16_t CurrentValue = 0;
            uint16_t MaxValue = 0;
            uint16_t MinValue = 65535;
            uint16_t AvgValue = 0;
            uint8_t ADCIndex = 0;
            uint32_t SampleCount = 0;
    };
}

#endif