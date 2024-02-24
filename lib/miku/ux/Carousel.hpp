
#ifndef MIKU_UX_CAROUSEL_HPP
#define MIKU_UX_CAROUSEL_HPP

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

namespace miku::ux {
    class Carousel {
        public:
            Carousel(daisy::OneBitGraphicsDisplay* display) {
                this->display = display;
            }

            void Render() {

            }
        private:
            daisy::OneBitGraphicsDisplay* display;
            FontDef font = Font_6x8;

            void draw_bitmap(const bool bitmap[64][64]) {
                for (int i = 0; i < 64; i++) {
                    for (int j = 0; j < 64; j++) {
                        this->display->DrawPixel(i, j, bitmap[j][i]);
                    }
                }
                this->display->Update();
            };
    };
}

#endif
