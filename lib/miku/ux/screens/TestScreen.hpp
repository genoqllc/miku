
#ifndef MIKU_UX_SCREENS_TEST_SCREEN_HPP
#define MIKU_UX_SCREENS_TEST_SCREEN_HPP

#include <vector>

#include "../Screen.hpp"
#include "../../tasks/Task.hpp"
#include "../Display.hpp"

#include <stdexcept>

namespace miku::ux::screens {
    class TestScreen : public miku::ux::Screen {
        public:
            TestScreen(Display* display, data::State* state, std::string someString) : Screen(display, state, "TS") {
                this->someString = someString;
            }

            void Render() {
                this->state->Logger->Info("Rendering TestScreen");

                this->GetDisplay()->Fill();
                this->GetDisplay()->DrawStringByRow(1, 0, "This is a test");
                this->GetDisplay()->DrawStringByRow(2, 0, someString.c_str());

                char buffer[32];

                sprintf(buffer, "LED: %d", this->state->LedState);
                this->GetDisplay()->DrawStringByRow(3, 0, buffer);

                sprintf(buffer, "SIX: %d", this->state->ScreenIndex);
                this->GetDisplay()->DrawStringByRow(4, 0, buffer);

                sprintf(buffer, "SPV: %d", this->state->ScreenSelectionPotentiometer);
                this->GetDisplay()->DrawStringByRow(5, 0, buffer);

                this->GetDisplay()->RequestInvalidate();
            }
        private:
            std::string someString;
    };
}

#endif
