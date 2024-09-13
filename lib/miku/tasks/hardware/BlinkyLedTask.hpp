#ifndef MIKU_TASKS_HARDWARE_BLINKYLED_HPP
#define MIKU_TASKS_HARDWARE_BLINKYLED_HPP

#include "../Task.hpp"

namespace miku::tasks::hardware {
    class BlinkyLedTask : public Task {
        public:
            BlinkyLedTask(daisy::DaisySeed hardware, data::State* state) : Task(hardware, state, "BL", 100UL) {
                
            }

            void Execute() {
                this->ledState = !this->ledState;
                this->hardware.SetLed(this->ledState);

                this->state->LedState = this->ledState;

                Task::Execute();
            }
        private:
            bool ledState = false;
    };
}


#endif
