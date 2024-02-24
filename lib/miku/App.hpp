
#ifndef MIKU_UX_APP_HPP
#define MIKU_UX_APP_HPP

#include <vector>
#include <string>
#include <map>

#include "daisy_seed.h"
#include "tasks/Task.hpp"

#include "ux/Display.hpp"

#include "ux/screens/SplashScreen.hpp"
#include "ux/screens/PotTestScreen.hpp"

#include "ux/screens/TestScreen.hpp"

#include "tasks/hardware/ScreenButtonTask.hpp"
#include "tasks/hardware/BlinkyLedTask.hpp"
#include "tasks/hardware/LinearPotentiometerTask.hpp"


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

                this->initAdc();

                this->enableTasks();
            }

            void enableTasks() {
                for (miku::tasks::Task* task : this->tasks) {
                    if (task != nullptr) {
                        task->Enable();
                    }
                }
            }

            void initAdc() {
                daisy::AdcChannelConfig adcConfig;

                unsigned int configuredChannelCount = 0;
                int taskIndex = 0;

                display->Fill(false);
                
                for (miku::tasks::Task* task : this->tasks) {
                    char buffer[32];
                    if (task != nullptr) {
                        display->DrawStringByRow(taskIndex, 0, task->GetCode());                    

                        for (unsigned short adcPin : *task->GetAdcPins()) {
                            sprintf(buffer, "%d", adcPin);
                            display->DrawStringByRow(taskIndex, 15, buffer);

                            adcConfig.InitSingle(hardware.GetPin(adcPin));
                            task->GetAdcChannelIndices()->push_back(configuredChannelCount);
                            configuredChannelCount++; // indices are 0 based, so we need to increment after we've used the value
                        }
                    }

                    taskIndex++;
                }

                this->hardware.adc.Init(&adcConfig, configuredChannelCount);
                this->hardware.adc.Start();

                display->Invalidate();

                hardware.system.Delay(5000);
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
                
                this->GetDisplay()->Fill(false);
                this->GetDisplay()->Invalidate();

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
                for (unsigned int taskIndex = 0; taskIndex < this->tasks.size(); taskIndex++) {
                    miku::tasks::Task* task = this->tasks[taskIndex];
                    if (task != nullptr && task->IsEnabled() && task->TimerLapsed()) {
                        task->Execute();

                        for(auto& it : task->GetDataValues()) {
                            this->dataValues[it.first] = it.second;
                        }
                    }
                }
            }

            void buildScreens() {
                this->screens = std::vector<ux::Screen*> {
                    new miku::ux::screens::PotTestScreen {
                        this->GetDisplay()
                    },
                    new miku::ux::screens::TestScreen{
                        this->GetDisplay(),
                        "Screen 1"
                    }
                };
            }

            void buildTasks() {
                this->tasks = std::vector<miku::tasks::Task*> {
                    new miku::tasks::hardware::ScreenButtonTask(hardware, 28),
                    new miku::tasks::hardware::BlinkyLedTask(hardware),
                    new miku::tasks::hardware::LinearPotentiometerTask(hardware, 21)
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
