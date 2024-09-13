
#ifndef MIKU_DATA_POTENTIOMETERSTATE_HPP
#define MIKU_DATA_POTENTIOMETERSTATE_HPP

#include "daisy_seed.h"
#include <string>
#include <vector>
#include <deque>

namespace miku::data {
    /// @brief Represents the state of a potentiometer
    class PotentiometerState {
        public:
            PotentiometerState(std::string code, uint8_t pinNumber) {
                Code = code;
                PinNumber = pinNumber;
            }
            
            /// @brief The code for this potentiometer
            std::string Code;
            /// @brief The pin number on the Daisy Seed
            uint8_t PinNumber = 0;
            /// @brief The current value of the potentiometer
            uint16_t CurrentValue = 0;
            /// @brief The maximum value of the potentiometer encountered
            uint16_t MaxValue = 0;
            /// @brief The minimum value of the potentiometer encountered
            uint16_t MinValue = 65535;
            /// @brief Average of the last 20 samples
            uint16_t AvgValue = 0;
            /// @brief The index in the Daisy ADC array
            uint8_t ADCIndex = 0;
            /// @brief The total number of samples taken
            uint32_t SampleCount = 0;
            /// @brief Standard deviation of the last 20 samples
            uint16_t StdDev = 0;
            /// @brief The last 20 samples
            std::deque<uint16_t> RecentSamples = std::deque<uint16_t>();
            /// @brief The maximum number of samples to keep
            const uint8_t MAX_SAMPLES = 20;
    };
}

#endif