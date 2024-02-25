
#ifndef MIKU_TASKS_TASK_HPP
#define MIKU_TASKS_TASK_HPP

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

            bool TimerLapsed() {
                if (this->enabled && hardware.system.GetNow() - this->lastExecution >= this->timeout) {
                    return true;
                }
                return false;
            }

            std::map<std::string, float> GetDataValues() {
                return this->dataValues;
            }

            // virtual std::vector<int>* GetAdcPins() {
            //     return &this->adcPins;
            // }

            // std::vector<unsigned short>* GetAdcChannelIndices() {
            //     return &this->adcChannelIndices;
            // }

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
        protected:
            std::map<std::string, float> dataValues;
            daisy::DaisySeed hardware;
            short adcPin = -1;
            short adcChannelIndex = -1;
            std::string code;
        private:
            unsigned long timeout;
            unsigned long lastExecution = -1;
            bool enabled = false;
    };
}

#endif
