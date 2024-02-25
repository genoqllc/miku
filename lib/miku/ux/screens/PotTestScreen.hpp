
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
                sprintf(buffer, "Cur: %.4f %.4f", this->dataValues["POT_NOTE_CURRENT"], this->dataValues["POT_SCRN_CURRENT"]);
                this->GetDisplay()->DrawStringByRow(0, 0, buffer);

                sprintf(buffer, "Max: %.4f %.4f", this->dataValues["POT_NOTE_MAX"], this->dataValues["POT_SCRN_MAX"]);
                this->GetDisplay()->DrawStringByRow(1, 0, buffer);

                sprintf(buffer, "Avg: %.4f %.4f", this->dataValues["POT_NOTE_AVG"], this->dataValues["POT_SCRN_AVG"]);
                this->GetDisplay()->DrawStringByRow(2, 0, buffer);

                sprintf(buffer, "Min: %.4f %.4f", this->dataValues["POT_NOTE_MIN"], this->dataValues["POT_SCRN_MIN"]);
                this->GetDisplay()->DrawStringByRow(3, 0, buffer);

                sprintf(buffer, "ADC: %.4f %.4f", this->dataValues["POT_NOTE_ADC_CHANNEL"], this->dataValues["POT_SCRN_ADC_CHANNEL"]);
                this->GetDisplay()->DrawStringByRow(4, 0, buffer);

                sprintf(buffer, "PIN: %.4f %.4f", this->dataValues["POT_NOTE_ADC_PIN"], this->dataValues["POT_SCRN_ADC_PIN"]);
                this->GetDisplay()->DrawStringByRow(5, 0, buffer);

                this->GetDisplay()->RequestInvalidate();
            }
    };
}

#endif
