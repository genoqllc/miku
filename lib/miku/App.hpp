
#ifndef MIKU_UX_APP_HPP
#define MIKU_UX_APP_HPP

// Standard Library
#include <vector>
#include <string>
#include <map>

// Hardware
#include "daisy_seed.h"
#include "daisysp.h"

// UX Core
#include "tasks/Task.hpp"
#include "ux/Display.hpp"

#include "utils/Clamper.hpp"
#include "tasks/hardware/MidiHardware.hpp"

// Screens
#include "ux/screens/SplashScreen.hpp"
#include "ux/screens/TestScreen.hpp"
#include "ux/screens/PotTestScreen.hpp"
#include "ux/screens/ButtonTestScreen.hpp"
#include "ux/screens/MidiEventsScreen.hpp"
#include "ux/screens/SyllableScreen.hpp"

// Tasks
#include "tasks/hardware/ScreenButtonTask.hpp"
#include "tasks/hardware/BlinkyLedTask.hpp"
#include "tasks/hardware/LinearPotentiometerTask.hpp"
#include "tasks/hardware/MidiRelayTask.hpp"

#include "data/State.hpp"

namespace miku {
    /// @brief The top level application container. Has tasks that do work and screens that show the result of that work.
    class App {
        public:
            App(daisy::DaisySeed hardware) {
                this->version = "v0.0.1a";
                this->dataValues = std::map<std::string, float>();

                // Initialize the world state -- the only time this should happen
                this->state = new data::State();

                this->hardware = hardware;
                this->hardware.Init();

                this->midiHardware = new miku::tasks::hardware::MidiHardware(this->state, hardware, 14, 13);
                //this->midiHardware->Init();

                ux::MikuOledDisplay mikuDisplay;
                this->display = new ux::Display(hardware, mikuDisplay);

                this->buildScreens();
                this->buildTasks();

                // Map screen data dependencies to tasks manually for complex structures
                for (ux::Screen* screen : this->screens) {
                    for (miku::tasks::Task* task : this->tasks) {
                        if (task->GetDependenciesProvided() & screen->GetDependencyFlags() & DependencyFlags::MidiEvents) {
                            // TODO Make this not gross, but no RTTI so whatever
                            tasks::hardware::MidiRelayTask* midiTask = (tasks::hardware::MidiRelayTask*)task;
                            ux::screens::MidiEventsScreen* midiScreen = (ux::screens::MidiEventsScreen*)screen;

                            midiScreen->BindMidiEventLog(midiTask->GetMidiEventLog());
                        }
                    }
                }

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
                daisy::AdcChannelConfig adcConfigs[16];

                unsigned short configuredChannelCount = 0;

                display->Fill(false);
                
                for (miku::tasks::Task* task : this->tasks) {
                    if (task != nullptr) {
                        short adcPin = task->GetAdcPin();

                        if (adcPin >= 0) {
                            adcConfigs[configuredChannelCount].InitSingle(hardware.GetPin(adcPin));
                            task->SetAdcChannelIndex(configuredChannelCount);

                            // display->DrawStringByRow(configuredChannelCount, 0, "ADC " + task->GetCode() + std::to_string(adcPin) + " " + std::to_string(configuredChannelCount));
                            configuredChannelCount++;
                        }
                    }
                }
 
                // display->Invalidate();
                // hardware.system.Delay(5000);

                this->hardware.adc.Init(adcConfigs, configuredChannelCount);
                this->hardware.adc.Start();
            }

            /// @brief Main while loop of the application
            void Run() {
                miku::ux::screens::SplashScreen* splashScreen = new miku::ux::screens::SplashScreen {
                    this->GetDisplay(),
                    this->state,
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

                    if (now - lastScreenCheck > 100) {
                        lastScreenCheck = now;
                        this->desiredScreenIndex = 0; this->determineDesiredScreenIndex();
                    }

                    if (this->desiredScreenIndex != this->state->ScreenIndex) {
                        this->state->ScreenIndex = desiredScreenIndex;
                        this->currentScreen = screens[this->state->ScreenIndex];
                    }

                    if(now - lastRender > 100) {
                        lastRender = now;
                        // TODO change to bind the state instead of the data bag
                        // currentScreen->DataBind(this->dataValues);

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

                        // for(auto& it : task->GetDataValues()) {
                        //     this->dataValues[it.first] = it.second;
                        // }
                    }
                }
            }

            /// @brief Declares the screens that should be available
            void buildScreens() {
                this->screens = std::vector<ux::Screen*> {
                    new miku::ux::screens::TestScreen{
                        this->GetDisplay(),
                        this->state,
                        "Screen 1"
                    }  
                    // new miku::ux::screens::PotTestScreen {
                    //     this->GetDisplay(),
                    //     this->state
                    // },
                    // new miku::ux::screens::MidiEventsScreen {
                    //     this->GetDisplay(),
                    //     this->state
                    // },               
                    // new miku::ux::screens::SyllableScreen {
                    //     this->GetDisplay(),
                    //     this->state
                    // }
                };
            }

            /// @brief Declares the tasks to run
            void buildTasks() {
                this->tasks = std::vector<miku::tasks::Task*> {
                    new miku::tasks::hardware::BlinkyLedTask(hardware, this->state)
                    // new miku::tasks::hardware::MidiRelayTask(hardware, this->state, this->midiHardware),
                    // new miku::tasks::hardware::ScreenButtonTask(hardware, this->state, 28),
                    
                    // new miku::tasks::hardware::LinearPotentiometerTask(hardware, this->state, 20, "POT_SYLL", &this->state->VowelPotentiometer),
                    // new miku::tasks::hardware::LinearPotentiometerTask(hardware, this->state, 21, "POT_SCRN", &this->state->ScreenSelectionPotentiometer)
                };
            }

            /// @brief Determines the screen index that should be shown based on the current potentiometer value
            /// @return The index that should be shown - note, this does NOT set the desired/current screen index
            unsigned short determineDesiredScreenIndex() {
                uint16_t screenPotCurrent = state->ScreenSelectionPotentiometer;
                // TODO protect against div by zero
                //return (unsigned short)daisysp::fclamp((screenPotCurrent * 100.0) / (100 / this->screens.size()), 0, this->screens.size() - 1);
                //return Clamper::ReadingToIndex(screenPotCurrent, this->screens.size());

                return 0;
            }

            /// @brief The duration of the splash screen in milliseconds
            int splashDuration = 2000;
            /// @brief All available screens for the App
            std::vector<miku::ux::Screen*> screens;
            /// @brief The display that the application will render to
            ux::Display* display;
            /// @brief Flag to indicate that the application should stop running
            bool interrupt = false;
            /// @brief The underlying hardware platform
            daisy::DaisySeed hardware;
            /// @brief The midi hardware interface; should probably be presented through the hardware abstraction
            miku::tasks::hardware::MidiHardware* midiHardware;
            /// @brief Version of the app
            std::string version;
            /// @brief The header screen
            miku::ux::Screen* headerScreen;
            /// @brief The screen index we want to show based on the selection mechanism (current: POT_SCRN)
            unsigned short desiredScreenIndex = 0;
            /// @brief The screen index that's currently being displayed
            unsigned short currentScreenIndex = -1;
            /// @brief The screen that's currently being displayed
            miku::ux::Screen* currentScreen;
            /// @brief Tasks registered with the app
            std::vector<miku::tasks::Task*> tasks;
            /// @brief Collection of all data values collected from tasks
            std::map<std::string, float> dataValues;
            data::State* state;

            unsigned long lastScreenCheck = 0;
    };
}

#endif
