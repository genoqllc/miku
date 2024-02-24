

#ifndef MIKU_TASKS_HARDWARE_SCREEN_BUTTON_TASK_HPP
#define MIKU_TASKS_HARDWARE_SCREEN_BUTTON_TASK_HPP

#include "../Task.hpp"

namespace miku::tasks::hardware {
    class ScreenButtonTask : public Task {
        public:
            ScreenButtonTask(daisy::DaisySeed hardware, int pin) : Task(hardware, "SB", 100UL) {
                this->Init();
            }

            void Init() {
                this->button.Init(this->hardware.GetPin(pin));
            }

            void Execute() {
                this->button.Debounce();
                bool isPressed = this->button.Pressed();
                this->dataValues["SCREEN_BUTTON_PRESSED"] = isPressed ? 1.0f : 0.0f;
                this->dataValues["SCREEN_BUTTON_TIME_HELD"] = this->button.TimeHeldMs();
                this->dataValues["SCREEN_BUTTON_TICK"] = this->hardware.system.GetNow();

                Task::Execute();
            }
        private:
            int pin;
            daisy::Switch button;
    };
}

#endif
