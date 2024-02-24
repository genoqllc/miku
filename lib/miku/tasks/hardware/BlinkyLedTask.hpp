#ifndef MIKU_TASKS_HARDWARE_BLINKYLED_HPP
#define MIKU_TASKS_HARDWARE_BLINKYLED_HPP

#include "../Task.hpp"

namespace miku::tasks::hardware {
    class BlinkyLedTask : public Task {
        public:
            BlinkyLedTask(daisy::DaisySeed hardware) : Task(hardware, "BL", 100UL) {
                
            }

            void Execute() {
                this->ledState = !this->ledState;
                this->hardware.SetLed(this->ledState);

                this->dataValues["LED_STATE"] = this->ledState ? 1.0f : 0.0f;

                Task::Execute();
            }
        private:
            bool ledState = false;
    };
}


#endif
