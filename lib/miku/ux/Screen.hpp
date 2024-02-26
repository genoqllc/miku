
#ifndef MIKU_UX_SCREEN_HPP
#define MIKU_UX_SCREEN_HPP

#include <string>
#include "Display.hpp"
#include <vector>
#include <map>

namespace miku::ux {
    /// @brief Abstraction for a screen that can be rendered on the display
    class Screen {
        public:
            Screen(Display* display, std::string code, unsigned short top = 0, unsigned short left = 0) {
                this->code = code;
                this->display = display;
                this->top = top;
                this->left = left;
                this->dataValues = std::map<std::string, float>();
            }

            /// @brief Renders the screen
            virtual void Render() = 0;

            virtual void DataBind(std::map<std::string, float> data) {
                this->dataValues = data;
            }

            /// @brief 
            /// @return 
            Display* GetDisplay() {
                return this->display;
            }

            void DrawStringByRow(unsigned short row, unsigned short x, std::string text) {
                this->display->DrawStringByRow(row, x, text);
            }

            std::string GetCode() {
                return this->code;
            }

            DependencyFlags GetDependencyFlags() {
                return this->dependencyFlags;
            }
        protected:
            unsigned short left = 0;
            unsigned short top = 0;
            std::map<std::string, float> dataValues;
            DependencyFlags dependencyFlags = DependencyFlags::None;
        private:
            std::string code;
            Display* display;
    };
}

#endif
