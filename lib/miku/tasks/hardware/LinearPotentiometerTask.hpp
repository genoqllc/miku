#ifndef MIKU_TASKS_HARDWARE_LINEAR_POTENTIOMETER_TASK_HPP
#define MIKU_TASKS_HARDWARE_LINEAR_POTENTIOMETER_TASK_HPP

#include "../Task.hpp"

namespace miku::tasks::hardware {
    class LinearPotentiometerTask : public Task {
        public:
            LinearPotentiometerTask(daisy::DaisySeed hardware, data::State* state, unsigned short pin, std::string code, uint16_t* value) : Task(hardware, state, code, 50UL) {
                this->adcPin = pin;
                this->currentValue = value;
            }

            void Execute() {
                char keyBuffer[32];
                this->sampleCount++;

                *(this->currentValue) = this->hardware.adc.Get(adcChannelIndex);

                // if (*this->currentValue > this->maxValue) {
                //     this->maxValue = *this->currentValue;
                // }

                // if (*this->currentValue < this->minValue) {
                //     this->minValue = *this->currentValue;
                // }

                // TODO error correction/averaging
                //this->averageValue = this->averageValue + this->currentValue / this->sampleCount;
                
                // TODO figure out if we want to keep this many indicies thing (i think no)
                sprintf(keyBuffer, "%s_CURRENT", this->code.c_str());
                this->dataValues[keyBuffer] = *this->currentValue;

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

            uint16_t* currentValue;
            
            // TODO break this out to support multi-pot
            //float currentValue = 0.0f;
            uint16_t maxValue = 0;
            uint16_t minValue = 65535;
            uint16_t averageValue = 0;
            unsigned long sampleCount = 0;
            
    };
}


#endif
