
#ifndef MIKU_UX_SCREENS_BUTTON_TEST_SCREEN_HPP
#define MIKU_UX_SCREENS_BUTTON_TEST_SCREEN_HPP

#include <vector>

#include "../Screen.hpp"
#include "../../tasks/Task.hpp"
#include "../Display.hpp"

#include <stdexcept>

namespace miku::ux::screens {
    class ButtonTestScreen : public miku::ux::Screen {
        public:
            ButtonTestScreen(Display* display) : Screen(display, "BT") {

            }

            void Render() {
                this->GetDisplay()->Fill();

                char buffer[32];
                sprintf(buffer, "PIN: %.1f", this->dataValues["SCREEN_BUTTON_PIN"]);
                this->GetDisplay()->DrawStringByRow(0, 0, buffer);

                sprintf(buffer, "INI: %.1f", this->dataValues["SCREEN_BUTTON_INIT"]);
                this->GetDisplay()->DrawStringByRow(1, 0, buffer);

                sprintf(buffer, "PRS: %.4f", this->dataValues["SCREEN_BUTTON_PRESSED"]);
                this->GetDisplay()->DrawStringByRow(2, 0, buffer);

                sprintf(buffer, "HLD: %.4f", this->dataValues["SCREEN_BUTTON_TIME_HELD"]);
                this->GetDisplay()->DrawStringByRow(3, 0, buffer);

                sprintf(buffer, "TIK: %.4f", this->dataValues["SCREEN_BUTTON_TICK"]);
                this->GetDisplay()->DrawStringByRow(4, 0, buffer);

                this->GetDisplay()->RequestInvalidate();
            }
    };
}

#endif
