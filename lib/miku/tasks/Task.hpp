
#ifndef MIKU_TASKS_TASK_HPP
#define MIKU_TASKS_TASK_HPP

#include "../Flags.hpp"
#include "../data/State.hpp"

namespace miku::tasks {
    class Task {
        public:
            Task(daisy::DaisySeed hardware, data::State* state, std::string code, long timeout, bool enabled = true) {
                this->timeout = timeout;
                this->enabled = enabled;
                this->hardware = hardware;
                this->code = code;
                this->state = state;
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

            std::string GetCode() {
                return this->code;
            }

            /// @brief A mask that represents the dependencies this Task can fulfill/provide to others
            /// @return The mask
            DependencyFlags GetDependenciesProvided() {
                return this->dependenciesProvided;
            }
        protected:
            daisy::DaisySeed hardware;
            /// @brief A mask of dependencies that this task provides
            DependencyFlags dependenciesProvided = DependencyFlags::None;
            data::State* state;
            std::string code;
        private:
            unsigned long timeout;
            unsigned long lastExecution = -1;
            bool enabled = false;
    };
}

#endif
