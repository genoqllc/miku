
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
            PotTestScreen(Display* display) : Screen(display, "PT") {

            }

            void Render() {
                this->GetDisplay()->Fill();

                char buffer[32];
                sprintf(buffer, "Cur: %.4f", this->dataValues["POT_0_CURRENT"]);
                this->GetDisplay()->DrawStringByRow(1, 0, buffer);

                sprintf(buffer, "Max: %.4f", this->dataValues["POT_0_MAX"]);
                this->GetDisplay()->DrawStringByRow(2, 0, buffer);

                sprintf(buffer, "Avg: %.4f", this->dataValues["POT_0_AVG"]);
                this->GetDisplay()->DrawStringByRow(3, 0, buffer);

                sprintf(buffer, "Min: %.4f", this->dataValues["POT_0_MIN"]);
                this->GetDisplay()->DrawStringByRow(4, 0, buffer);

                sprintf(buffer, "ChC: %.1f", this->dataValues["POT_ADC_CHANNEL_CT"]);
                this->GetDisplay()->DrawStringByRow(5, 0, buffer);

                sprintf(buffer, "PnC: %.1f", this->dataValues["POT_ADC_PIN_CT"]);
                this->GetDisplay()->DrawStringByRow(5, 64, buffer);

                this->GetDisplay()->RequestInvalidate();
            }
    };
}

#endif
