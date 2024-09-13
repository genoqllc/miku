#ifndef MIKU_TASKS_HARDWARE_SET_SYLLABLE_TASK_HPP
#define MIKU_TASKS_HARDWARE_SET_SYLLABLE_TASK_HPP

#include "../Task.hpp"
#include "../../data/Syllables.hpp"
#include "../../utils/Clamper.hpp"
#include "MidiHardware.hpp"

namespace miku::tasks::hardware {
    class SetSyllableTask : public Task {
        public:
            SetSyllableTask(daisy::DaisySeed hardware, data::State* state) : Task(hardware, state, "SLT", 100UL) {
                
            }

            void Execute() {
                //float currentReading = this->GetDataValues()["POT_SYLL_CURRENT"];

                //unsigned int vowelIndex = Clamper::ReadingToIndex(currentReading, data::syllables::Vowels.size());


                

                Task::Execute();
            }
        private:
            bool lastReading = false;
    };
}


#endif
