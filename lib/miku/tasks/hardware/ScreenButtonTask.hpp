

#ifndef MIKU_TASKS_HARDWARE_SCREEN_BUTTON_TASK_HPP
#define MIKU_TASKS_HARDWARE_SCREEN_BUTTON_TASK_HPP

#include "../Task.hpp"

namespace miku::tasks::hardware {
    class ScreenButtonTask : public Task {
        public:
            ScreenButtonTask(daisy::DaisySeed hardware, data::State* state, int pin) : Task(hardware, state, "SB", 100UL) {
                // this->dataValues["SCREEN_BUTTON_INIT"] = 0.0f;
                // this->dataValues["SCREEN_BUTTON_PIN"] = (float)pin;

                this->Init();
            }

            void Init() {
                this->button = new daisy::Switch();

                // this->dataValues["SCREEN_BUTTON_INIT"] = 0.5f;
                // this->button->Init(this->hardware.GetPin(pin), 500.0f, daisy::Switch::TYPE_MOMENTARY, daisy::Switch::POLARITY_INVERTED, daisy::Switch::PULL_UP);
                // this->dataValues["SCREEN_BUTTON_INIT"] = 1.0f;
            }

            void Execute() {
                this->button->Debounce();
                bool isPressed = this->button->RawState();
                // this->dataValues["SCREEN_BUTTON_PRESSED"] = isPressed ? 1.0f : 0.0f;
                // this->dataValues["SCREEN_BUTTON_TIME_HELD"] = this->button->TimeHeldMs();
                // this->dataValues["SCREEN_BUTTON_TICK"] = this->hardware.system.GetNow();

                Task::Execute();
            }
        private:
            int pin;
            daisy::Switch* button;
    };
}

#endif
