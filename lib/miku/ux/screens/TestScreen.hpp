
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
                sprintf(buffer, "Bound: %.3f", this->dataValues["SCREEN_BUTTON_PRESSED"]);
                this->GetDisplay()->DrawStringByRow(3, 0, buffer);

                sprintf(buffer, "%lu", daisy::System::GetNow());
                this->GetDisplay()->DrawStringByRow(4, 0, buffer);

                this->GetDisplay()->RequestInvalidate();
            }

            void RegisterTasks() {
                // We don't have any tasks to register
            }

            std::vector<miku::tasks::Task*> GetTasks() {
                return {};
            }

        private:
            std::string someString;
    };
}

#endif
