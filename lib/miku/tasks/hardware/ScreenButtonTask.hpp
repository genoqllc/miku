

#ifndef MIKU_TASKS_HARDWARE_SCREEN_BUTTON_TASK_HPP
#define MIKU_TASKS_HARDWARE_SCREEN_BUTTON_TASK_HPP

#include "../Task.hpp"

namespace miku::tasks::hardware {
    class ScreenButtonTask : public Task {
        public:
            ScreenButtonTask(daisy::DaisySeed hardware, int pin) : Task(hardware, 250UL) {
                //this->adcPins.push_back(pin);
                this->Init();
            }

            void Init() {
                this->button.Init(this->hardware.GetPin(pin), 1000);
            }

            void Execute() {
                this->button.Debounce();
                bool isPressed = this->button.Pressed();
                this->dataValues["SCREEN_BUTTON_PRESSED"] = isPressed ? 1.0f : 0.0f;
            }
        private:
            int pin;
            daisy::DaisySeed hardware;
            daisy::Switch button;
    };
}

#endif
