#ifndef MIKU_TASKS_HARDWARE_LINEAR_POTENTIOMETER_TASK_HPP
#define MIKU_TASKS_HARDWARE_LINEAR_POTENTIOMETER_TASK_HPP

#include "../Task.hpp"
#include "../../data/State.hpp"
#include "../../data/PotentiometerState.hpp"

namespace miku::tasks::hardware {
    class LinearPotentiometerTask : public Task {
        public:
            LinearPotentiometerTask(daisy::DaisySeed hardware, data::State* state, data::PotentiometerState* potentiometerState) : Task(hardware, state, potentiometerState->Code, 50UL) {
               state->Logger->Info("Linear Potentiometer Task ctor for %s", potentiometerState->Code.c_str());
                
                this->potState = potentiometerState;
            }

            void Execute() {
                this->potState->SampleCount++;

                state->Logger->Debug("LinPot %s - Reading ADC pin %d", this->potState->Code.c_str(), this->potState->PinNumber);

                this->potState->CurrentValue = this->hardware.adc.Get(this->potState->PinNumber);

                state->Logger->Debug("LinPot %s - Read value %d", this->potState->Code.c_str(), this->potState->CurrentValue);

                if (this->potState->CurrentValue > this->potState->MaxValue) {
                    this->potState->MaxValue = this->potState->CurrentValue;
                }

                if (this->potState->CurrentValue < this->potState->MinValue) {
                    this->potState->MinValue = this->potState->CurrentValue;
                }

                // TODO error correction/averaging
                this->potState->AvgValue = this->potState->AvgValue + this->potState->AvgValue / this->potState->SampleCount;

                Task::Execute();
            }
        private:
            data::PotentiometerState* potState;
            
    };
}


#endif
