
#ifndef MIKU_UX_SCREENS_SCREEN_HPP
#define MIKU_UX_SCREENS_SCREEN_HPP

#include "../Screen.hpp"
#include "../Resources.hpp"
#include "../Display.hpp"

namespace miku::ux::screens {

    /// @brief The introductory splash screen
    class SplashScreen : public miku::ux::Screen {
        public:
            SplashScreen(Display* display, std::string version) : Screen(display, "SP") {
                this->version = version;
            }

            /// @brief Renders the splash screen
            void Render() {
                this->GetDisplay()->Fill(false);
                this->GetDisplay()->DrawBitmap(0, 0, miku::ux::PORTAL_LOGO_BITMAP);
                this->GetDisplay()->DrawStringByRow(0, 66, "Portal");
                this->GetDisplay()->DrawStringByRow(1, 66, "Modules");
                this->GetDisplay()->DrawStringByRow(2, 66, "MikuModule");
                this->GetDisplay()->DrawStringByRow(3, 66, this->version);
                this->GetDisplay()->RequestInvalidate();
                //this->GetDisplay()->Invalidate();
            }

            /// @brief Registers tasks for the splash screen (there are none)
            std::vector<miku::tasks::Task*> GetTasks() {
                return {};
            }
        private:
            std::string version;
    };

}

#endif
