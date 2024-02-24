
#ifndef MIKU_TASKS_TASK_HPP
#define MIKU_TASKS_TASK_HPP

#include "../adc/IHasAdcPins.hpp"

namespace miku::tasks {
    class Task : public adc::IHasAdcPins {
        public:
            Task(daisy::DaisySeed hardware, long timeout, bool enabled = true) {
                this->timeout = timeout;
                this->enabled = enabled;
                this->dataValues = std::map<std::string, float>();
            }

            virtual void Execute() = 0;

            void SetTimeout(int timeout) {
                this->timeout = timeout;
            }

            int GetTimeout() {
                return this->timeout;
            }

            bool IsEnabled() {
                return this->enabled;
            }

            bool TimerLapsed(unsigned long currentTime) {
                if (this->enabled && currentTime - this->lastExecution >= this->timeout) {
                    this->lastExecution = currentTime;
                    return true;
                }
                return false;
            }

            std::map<std::string, float> GetDataValues() {
                return this->dataValues;
            }
        protected:
            std::map<std::string, float> dataValues;
        private:
            unsigned long timeout;
            unsigned long lastExecution = -1;
            bool enabled = true;
    };
}

#endif
