#ifndef MIKU_UTILS_LOGGER_HPP
#define MIKU_UTILS_LOGGER_HPP

#include "daisy_seed.h"

namespace miku::utils 
{
    class Logger {
    public:
        Logger(daisy::DaisySeed hw, bool blockForClient = false) {
            hardware = hw;
            hw.StartLog(blockForClient);
        }

        template <typename... Args>
        void Info(const char* format, Args... args) {
            hardware.PrintLine(format, args...);
        }

    private:
        daisy::DaisySeed hardware;
    };
}

#endif