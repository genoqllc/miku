
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
            TestScreen(Display* display, std::string someString) : Screen(display, "TS") {
                this->someString = someString;
            }

            void Render() {
                this->GetDisplay()->Fill();
                this->GetDisplay()->DrawStringByRow(1, 0, "This is a test");
                this->GetDisplay()->DrawStringByRow(2, 0, someString.c_str());

                char buffer[32];

                sprintf(buffer, "DVC: %d", this->dataValues.size());
                this->GetDisplay()->DrawStringByRow(2, 64, buffer);

                sprintf(buffer, "Bound: %.3f", this->dataValues["SCREEN_BUTTON_PRESSED"]);
                this->GetDisplay()->DrawStringByRow(3, 0, buffer);

                sprintf(buffer, "DBT: %.3f", this->dataValues["SCREEN_BUTTON_TICK"]);
                this->GetDisplay()->DrawStringByRow(4, 0, buffer);

                sprintf(buffer, "LED: %.1f", this->dataValues["LED_STATE"]);
                this->GetDisplay()->DrawStringByRow(5, 0, buffer);

                sprintf(buffer, "SIX: %.1f", this->dataValues["SCRN_INDEX"]);
                this->GetDisplay()->DrawStringByRow(5, 64, buffer);

                this->GetDisplay()->RequestInvalidate();
            }
        private:
            std::string someString;
    };
}

#endif
