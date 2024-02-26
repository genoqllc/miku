
#ifndef MIKU_TASKS_TASK_HPP
#define MIKU_TASKS_TASK_HPP

#include "../Flags.hpp"

namespace miku::tasks {
    class Task {
        public:
            Task(daisy::DaisySeed hardware, std::string code, long timeout, bool enabled = true) {
                this->timeout = timeout;
                this->enabled = enabled;
                this->dataValues = std::map<std::string, float>();
                this->hardware = hardware;
                this->code = code;
            }

            virtual void Execute() {
                this->lastExecution = hardware.system.GetNow();
            }

            void SetTimeout(int timeout) {
                this->timeout = timeout;
            }

            int GetTimeout() {
                return this->timeout;
            }

            bool IsEnabled() {
                return this->enabled;
            }

            void Enable() {
                this->enabled = true;
            }
            
            void Disable() {
                this->enabled = false;
            }

            virtual bool TimerLapsed() {
                if (this->enabled && hardware.system.GetNow() - this->lastExecution >= this->timeout) {
                    return true;
                }
                return false;
            }

            std::map<std::string, float> GetDataValues() {
                return this->dataValues;
            }

            short GetAdcPin() {
                return this->adcPin;
            }

            short GetAdcChannelIndex() {
                return this->adcChannelIndex;
            }

            void SetAdcChannelIndex(short index) {
                this->adcChannelIndex = index;
            }

            std::string GetCode() {
                return this->code;
            }

            /// @brief A mask that represents the dependencies this Task can fulfill/provide to others
            /// @return The mask
            DependencyFlags GetDependenciesProvided() {
                return this->dependenciesProvided;
            }
        protected:
            std::map<std::string, float> dataValues;
            daisy::DaisySeed hardware;
            short adcPin = -1;
            short adcChannelIndex = -1;
            std::string code;
            /// @brief A mask of dependencies that this task provides
            DependencyFlags dependenciesProvided = DependencyFlags::None;
        private:
            unsigned long timeout;
            unsigned long lastExecution = -1;
            bool enabled = false;
    };
}

#endif
