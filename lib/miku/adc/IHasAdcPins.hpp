
#ifndef MIKU_ADC_I_HAS_ADC_PINS_HPP
#define MIKU_ADC_I_HAS_ADC_PINS_HPP

#include <vector>

namespace miku::adc {
    class IHasAdcPins {
        public:
            virtual std::vector<int> GetAdcPins() {
                return this->adcPins;
            }
        protected:
            std::vector<int> adcPins;
    };
}

#endif
