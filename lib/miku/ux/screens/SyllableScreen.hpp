
#ifndef MIKU_UX_SCREENS_SYLLABLE_SCREEN_HPP
#define MIKU_UX_SCREENS_SYLLABLE_SCREEN_HPP

#include <vector>

#include "../Screen.hpp"
#include "../../tasks/Task.hpp"
#include "../Display.hpp"
#include "../../utils/Clamper.hpp"
#include "../../data/Syllables.hpp"

#include <stdexcept>

namespace miku::ux::screens {
    class SyllableScreen : public miku::ux::Screen {
        public:
            SyllableScreen(Display* display, data::State* state) : Screen(display, state, "SYL") {

            }

            void Render() {
                this->GetDisplay()->Fill();

                float currentSyllable = this->dataValues["POT_SYLL_CURRENT"];

                char buffer[32];
                // sprintf(buffer, "RAW: %.1f", currentSyllable);
                // this->GetDisplay()->DrawStringByRow(0, 0, buffer);

                unsigned int syllableIndex = Clamper::ReadingToIndex(currentSyllable, this->syllables.size());
                // sprintf(buffer, "IDX: %d", syllableIndex);
                // this->GetDisplay()->DrawStringByRow(1, 0, buffer);

                char syllable = this->syllables[syllableIndex];
                sprintf(buffer, "CHA: %c", syllable);
                this->GetDisplay()->DrawStringByRow(1, 0, buffer);

                this->GetDisplay()->RequestInvalidate();
            }

        private:
            std::vector<char> syllables = {
                'A',
                'I',
                'U',
                'E',
                'O'
            };
    };
}

#endif
