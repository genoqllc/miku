#ifndef MIKU_TASKS_HARDWARE_LINEAR_POTENTIOMETER_TASK_HPP
#define MIKU_TASKS_HARDWARE_LINEAR_POTENTIOMETER_TASK_HPP

#include "../Task.hpp"

namespace miku::tasks::hardware {
    class LinearPotentiometerTask : public Task {
        public:
            LinearPotentiometerTask(daisy::DaisySeed hardware, unsigned short pin, std::string code) : Task(hardware, code, 50UL) {
                this->adcPin = pin;
            }

            void Execute() {
                char keyBuffer[32];
                this->sampleCount++;

                this->currentValue = this->hardware.adc.GetFloat(adcChannelIndex);

                if (this->currentValue > this->maxValue) {
                    this->maxValue = this->currentValue;
                }

                if (this->currentValue < this->minValue) {
                    this->minValue = this->currentValue;
                }

                // TODO error correction/averaging
                this->averageValue = (this->averageValue + this->currentValue) / this->sampleCount;
                
                // TODO figure out if we want to keep this many indicies thing (i think no)
                sprintf(keyBuffer, "%s_CURRENT", this->code.c_str());
                this->dataValues[keyBuffer] = this->currentValue;

                sprintf(keyBuffer, "%s_MIN", this->code.c_str());
                this->dataValues[keyBuffer] = this->minValue;

                sprintf(keyBuffer, "%s_MAX", this->code.c_str());
                this->dataValues[keyBuffer] = this->maxValue;

                sprintf(keyBuffer, "%s_AVG", this->code.c_str());
                this->dataValues[keyBuffer] = this->averageValue;

                sprintf(keyBuffer, "%s_ADC_PIN", this->code.c_str());
                this->dataValues[keyBuffer] = (float)this->adcPin;

                sprintf(keyBuffer, "%s_ADC_CHANNEL", this->code.c_str());
                this->dataValues[keyBuffer] = (float)this->adcChannelIndex;

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
