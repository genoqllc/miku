#ifndef MIKU_DATA_STATE_HPP
#define MIKU_DATA_STATE_HPP

namespace miku::data {
    class State {
        public:
            //static State* XXI_THE_WORLD;

            State() {
                
            }

            uint16_t ScreenIndex = 0;
            uint16_t ScreenSelectionPotentiometer = 0.0f;

            uint16_t VowelIndex = 0;
            uint16_t ConsonantIndex = 0;
            uint16_t VowelPotentiometer = 0;
            uint16_t VowelJack = 0.0f;
            uint16_t VowelJackPluggedIn = false;
            uint16_t ConsonantPotentiometer = 0.0f;
            bool ConsonantJackPluggedIn = false;

            bool LedState = false;

            bool MidiHeartbeat = false;
            uint16_t MidiEventCount = 0;
            float MidiBpm = 0.0f;
    };
}

#endif