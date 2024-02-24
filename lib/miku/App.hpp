
#ifndef MIKU_UX_APP_HPP
#define MIKU_UX_APP_HPP

#include <vector>
#include <string>
#include <map>

#include "daisy_seed.h"
#include "tasks/Task.hpp"
#include "ux/screens/SplashScreen.hpp"
#include "ux/Display.hpp"
#include "ux/Callbacks.hpp"

#include "ux/screens/TestScreen.hpp"

#include "tasks/hardware/ScreenButtonTask.hpp"
#include "adc/IHasAdcPins.hpp"

namespace miku {
    /// @brief The top level application container. Has tasks that do work and screens that show the result of that work.
    class App {
        public:
            App(daisy::DaisySeed hardware) {
                this->version = "v0.0.1a";
                this->dataValues = std::map<std::string, float>();

                this->hardware = hardware;
                this->hardware.Init();

                ux::MikuOledDisplay mikuDisplay;
                this->display = new ux::Display(hardware, mikuDisplay);

                this->buildScreens();
                this->buildTasks();
            }

            void initAdc() {
                daisy::AdcChannelConfig adcConfig;
                
                for (unsigned int taskIndex = 0; taskIndex < this->tasks.size(); taskIndex++) {
                    miku::tasks::Task* task = this->tasks[taskIndex];
                    if (task != nullptr) {
                        std::vector<int> adcPins = task->GetAdcPins();
                        for (std::size_t taskIndex = 0; taskIndex < adcPins.size(); taskIndex++) {
                            adcConfig.InitSingle(hardware.GetPin(adcPins[taskIndex]));
                        }
                    }
                }

                this->hardware.adc.Init(&adcConfig, 1);
                this->hardware.adc.Start();
            }

            /// @brief Main while loop of the application
            void Run() {
                miku::ux::screens::SplashScreen* splashScreen = new miku::ux::screens::SplashScreen {
                    this->GetDisplay(),
                    this->version
                };
                splashScreen->Render();
                this->GetDisplay()->Invalidate();
                daisy::System::Delay(splashDuration);

                unsigned long lastRender = daisy::System::GetNow();

                while (!this->interrupt) {
                    this->checkTasks();

                    unsigned long now = daisy::System::GetNow();

                    if (this->desiredScreenIndex != this->currentScreenIndex) {
                        this->currentScreenIndex = desiredScreenIndex;
                        this->currentScreen = screens[currentScreenIndex];
                    }

                    if(now - lastRender > 100) {
                        lastRender = now;
                        currentScreen->DataBind(this->dataValues);
                        currentScreen->Render();
                    }

                    if (this->GetDisplay()->NeedsInvalidate()) {
                        this->GetDisplay()->Invalidate();
                    }

                    this->GetDisplay()->ResetInvalidate();
                }
            }

            /// @brief Gets the underlying display abstraction
            /// @return A pointer to the Display abstraction
            ux::Display* GetDisplay() {
                return this->display;
            }

            /// @brief Asks the app to quit at the next opportunity
            void Quit() {
                this->interrupt = true;
            }

            std::string GetVersion() {
                return this->version;
            }
        private:

            /// @brief Walk our registered screens & tasks and see if any need to be executed. If so, execute them.
            void checkTasks() {
                unsigned long now = daisy::System::GetNow();

                for (unsigned int taskIndex = 0; taskIndex < this->tasks.size(); taskIndex++) {
                    miku::tasks::Task* task = this->tasks[taskIndex];
                    if (task != nullptr && task->IsEnabled() && task->TimerLapsed(now)) {
                        char buffer[32];
                        sprintf(buffer, "exec task %d %lu", taskIndex, now);
                        this->GetDisplay()->DrawStringByRow(5, 0, buffer);
                        task->Execute();

                        for(auto& it : task->GetDataValues()) {
                            this->dataValues[it.first] = it.second;
                        }
                    }
                }
            }

            void buildScreens() {
                this->screens = std::vector<ux::Screen*> {
                    new miku::ux::screens::TestScreen{
                        this->GetDisplay(),
                        "Screen 1"
                    },
                    new miku::ux::screens::TestScreen{
                        this->GetDisplay(),
                        "Screen 2"
                    },
                    new miku::ux::screens::TestScreen{
                        this->GetDisplay(),
                        "Screen THREE"
                    }
                };
            }

            void buildTasks() {
                this->tasks = std::vector<miku::tasks::Task*> {
                    new miku::tasks::hardware::ScreenButtonTask(hardware, 28)
                };
            }

            /// @brief The duration of the splash screen in milliseconds
            int splashDuration = 2000;
            /// @brief The tasks that the application will execute
            std::vector<miku::ux::Screen*> screens;
            /// @brief The display that the application will render to
            ux::Display* display;
            /// @brief Flag to indicate that the application should stop running
            bool interrupt = false;

            daisy::DaisySeed hardware;

            std::string version;

            miku::ux::Screen* headerScreen;

            unsigned short desiredScreenIndex = 0;
            unsigned short currentScreenIndex = -1;
            miku::ux::Screen* currentScreen;

            std::vector<miku::tasks::Task*> tasks;

            std::map<std::string, float> dataValues;
    };
}

#endif
