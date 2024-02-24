
#ifndef MIKU_UX_DISPLAY_HPP
#define MIKU_UX_DISPLAY_HPP

#include <string>
#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

namespace miku::ux {

    using MikuOledDisplay = daisy::OledDisplay<daisy::SSD130xI2c128x64Driver>;

    /// @brief Abstraction for the display
    class Display {
        // TODO this class is highly coupled to the DaisySeed hardware. It should be abstracted away.
        public:
            Display(daisy::DaisySeed hardware, MikuOledDisplay mikuDisplay) {
                /** Configure the Display */
                MikuOledDisplay::Config disp_cfg;
                disp_cfg.driver_config.transport_config.i2c_address               = 0x3C;
                disp_cfg.driver_config.transport_config.i2c_config.periph         = daisy::I2CHandle::Config::Peripheral::I2C_1;
                disp_cfg.driver_config.transport_config.i2c_config.speed          = daisy::I2CHandle::Config::Speed::I2C_1MHZ;
                disp_cfg.driver_config.transport_config.i2c_config.mode           = daisy::I2CHandle::Config::Mode::I2C_MASTER;
                disp_cfg.driver_config.transport_config.i2c_config.pin_config.scl = hardware.GetPin(11); // {DSY_GPIOB, 11};
                disp_cfg.driver_config.transport_config.i2c_config.pin_config.sda = hardware.GetPin(12); // {DSY_GPIOB, 12};

                /** And Initialize */
                mikuDisplay.Init(disp_cfg);
                this->rawDisplay = mikuDisplay;

                this->width = rawDisplay.Width();
                this->height = rawDisplay.Height();

                this->rowCount = height / rowHeight;
            }

            ~Display() {

            }

            void DrawStringByRow(int row, int x, std::string text, bool invert = true, FontDef font = Font_6x8) {
                this->DrawString(x, row * rowHeight + rowPadding, text.c_str(), invert, font);
            }

            void DrawString(int x, int y, std::string text, bool invert = true, FontDef font = Font_6x8) {
                this->rawDisplay.SetCursor(x, y);
                this->rawDisplay.WriteString(text.c_str(), font, invert);
            }

            void DrawBitmap(const bool bitmap[64][64]) {
                this->DrawBitmap(0, 0, bitmap);
            }

            void Fill(bool on = false) {
                this->rawDisplay.SetCursor(0, 0);
                this->rawDisplay.Fill(on);
            }

            /// @brief Draws a bitmap (2d array of bools) to the display
            /// @param x x offset
            /// @param y y offset
            /// @param bitmap the bitmap to draw
            void DrawBitmap(int x, int y, const bool bitmap[64][64]) {
                // TODO allow for an arbitrary array size
                for (int i = 0; i < 64; i++) {
                    for (int j = 0; j < 64; j++) {
                        this->rawDisplay.DrawPixel(x + i, y + j, bitmap[j][i]);
                    }
                }
            };

            bool NeedsInvalidate() {
                return this->needsInvalidate;
            }

            void RequestInvalidate() {
                this->needsInvalidate = true;
            }

            void ResetInvalidate() {
                this->needsInvalidate = false;
            }

            /// @brief Tells the underlying display to update
            void Invalidate() {
                this->rawDisplay.Update();
                this->needsInvalidate = false;
            }

            unsigned short GetRowHeight() {
                return this->rowHeight;
            }
        private:
            MikuOledDisplay rawDisplay;

            bool needsInvalidate = false;

            unsigned short width;
            unsigned short height;

            unsigned short fontHeight = 8;
            unsigned short fontWidth = 6;
            unsigned short rowPadding = 1;
            unsigned short rowCount = 0;
            unsigned short headerSeparatorHeight = 2;
            unsigned short dataRowsStartPosition = fontHeight + (rowPadding * 2) + headerSeparatorHeight;
            unsigned short rowHeight = fontHeight + (rowPadding * 2);
    };
}

#endif
