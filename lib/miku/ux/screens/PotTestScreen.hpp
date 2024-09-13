
#ifndef MIKU_UX_SCREENS_POT_TEST_SCREEN_HPP
#define MIKU_UX_SCREENS_POT_TEST_SCREEN_HPP

#include <vector>

#include "../Screen.hpp"
#include "../../tasks/Task.hpp"
#include "../Display.hpp"

#include <stdexcept>

namespace miku::ux::screens {
    class PotTestScreen : public miku::ux::Screen {
        public:
            PotTestScreen(Display* display, data::State* state) : Screen(display, state, "PT") {

            }

            void Render() {
                this->GetDisplay()->Fill();

                // TODO unhardcode

                auto pots = this->state->Potentiometers;

                auto screenPot = pots->at(21);
                auto notePot = pots->at(20);

                char buffer[32];
                sprintf(buffer, "ID : %s %s", screenPot->Code.c_str(), notePot->Code.c_str());
                this->GetDisplay()->DrawStringByRow(0, 0, buffer);

                sprintf(buffer, "Cur: %d %d", screenPot->CurrentValue, notePot->CurrentValue);
                this->GetDisplay()->DrawStringByRow(1, 0, buffer);

                sprintf(buffer, "Max: %d %d", screenPot->MaxValue, notePot->MaxValue);
                this->GetDisplay()->DrawStringByRow(2, 0, buffer);

                sprintf(buffer, "Avg: %d %d", screenPot->AvgValue, notePot->AvgValue);
                this->GetDisplay()->DrawStringByRow(3, 0, buffer);

                sprintf(buffer, "Min: %d %d", screenPot->MinValue, notePot->MinValue);
                this->GetDisplay()->DrawStringByRow(4, 0, buffer);

                // display stddev
                sprintf(buffer, "Std: %d %d", screenPot->StdDev, notePot->StdDev);
                this->GetDisplay()->DrawStringByRow(5, 0, buffer);

                this->GetDisplay()->RequestInvalidate();
            }
    };
}

#endif
