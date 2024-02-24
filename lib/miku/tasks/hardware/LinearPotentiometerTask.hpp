#ifndef MIKU_TASKS_HARDWARE_LINEAR_POTENTIOMETER_TASK_HPP
#define MIKU_TASKS_HARDWARE_LINEAR_POTENTIOMETER_TASK_HPP

#include "../Task.hpp"

namespace miku::tasks::hardware {
    class LinearPotentiometerTask : public Task {
        public:
            LinearPotentiometerTask(daisy::DaisySeed hardware, unsigned short pin) : Task(hardware, "PT", 50UL) {
                // TODO support multiple pots?
                this->GetAdcPins()->push_back(pin);
            }

            void Execute() {
                this->sampleCount++;

                for (unsigned short adcChannelIndex : this->adcChannelIndices) {
                    this->currentValue = this->hardware.adc.GetFloat(adcChannelIndex);

                    if (this->currentValue > this->maxValue) {
                        this->maxValue = this->currentValue;
                    }

                    if (this->currentValue < this->minValue) {
                        this->minValue = this->currentValue;
                    }

                    // TODO error correction/averaging
                    this->averageValue = (this->averageValue + this->currentValue) / this->sampleCount;

                    char keyBuffer[32];
                    sprintf(keyBuffer, "POT_%d_CURRENT", adcChannelIndex);
                    this->dataValues[keyBuffer] = this->currentValue;

                    sprintf(keyBuffer, "POT_%d_MIN", adcChannelIndex);
                    this->dataValues[keyBuffer] = this->minValue;

                    sprintf(keyBuffer, "POT_%d_MAX", adcChannelIndex);
                    this->dataValues[keyBuffer] = this->maxValue;

                    sprintf(keyBuffer, "POT_%d_AVG", adcChannelIndex);
                    this->dataValues[keyBuffer] = this->averageValue;
                }

                this->dataValues["POT_ADC_PIN_CT"] = this->adcPins.size();
                this->dataValues["POT_ADC_CHANNEL_CT"] = this->adcChannelIndices.size();

                Task::Execute();
            }
        private:
            unsigned short pin = -1;
            
            // TODO break this out to support multi-pot
            float currentValue = 0.0f;
            float maxValue = -0.0f;
            float minValue = 9999.0f;
            float averageValue = 0.0f;
            unsigned long sampleCount = 0;
            
    };
}


#endif
