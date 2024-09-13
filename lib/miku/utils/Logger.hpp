#ifndef MIKU_UTILS_LOGGER_HPP
#define MIKU_UTILS_LOGGER_HPP

#include "daisy_seed.h"

namespace miku::utils 
{
    enum LogLevel {
        DEBUG,
        INFO,
        WARN,
        ERROR
    };
    
    class Logger {
    public:
        Logger(daisy::DaisySeed hw, LogLevel level = LogLevel::INFO, bool blockForClient = false) {
            hardware = hw;
            hw.StartLog(blockForClient);
            CurrentLevel = level;
        }

        LogLevel CurrentLevel;

        template <typename... Args>
        void Info(std::string format, Args... args) {
            this->log(LogLevel::INFO, "INFO ", format, args...);
        }

        template <typename... Args>
        void Warn(std::string format, Args... args) {
            this->log(LogLevel::WARN, "WARN ", format, args...);
        }

        template <typename... Args>
        void Error(std::string format, Args... args) {
            this->log(LogLevel::ERROR, "ERROR", format, args...);
        }

        template <typename... Args>
        void Debug(std::string format, Args... args) {
            this->log(LogLevel::DEBUG, "DEBUG", format, args...);
        }
    private:
        template <typename... Args>
        void log(LogLevel messageLevel, std::string levelName, std::string format, Args... args) {
            if (messageLevel < CurrentLevel) {
                return;
            }

            std::string logLineFormat = "[%s] " + format;
            hardware.PrintLine(logLineFormat.c_str(), levelName.c_str(), args...);
        }

        daisy::DaisySeed hardware;
    };
}

#endif