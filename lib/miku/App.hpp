
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
#include "data/PotentiometerState.hpp"
#include "utils/Logger.hpp"

namespace miku {
    /// @brief The top level application container. Has tasks that do work and screens that show the result of that work.
    class App {
        public:
            App(daisy::DaisySeed hardware) {
                this->version = "v0.0.1a";

                // Initialize the world state -- the only time this should happen
                this->state = new data::State();

                this->hardware = hardware;
                this->hardware.Init();

                this->logger = new utils::Logger(this->hardware, utils::LogLevel::INFO, true);
                logger->Info("Miku starting up...");
                this->state->Logger = this->logger;

                logger->Info("Registering potentiometers");

                // Potentiometer config
                // TODO read this from a simpler config
                auto const pots = std::vector<data::PotentiometerState*> {
                    new data::PotentiometerState {
                        "POT_SYLL",
                        20
                    },
                    new data::PotentiometerState {
                        "POT_SCRN",
                        this->SCREEN_POT_PIN_NUMBER
                    }
                };

                logger->Info("Registering %d potentiometers", pots.size());

                for (auto pot : pots) {
                    logger->Info("Registering pot %s on pin %d", pot->Code.c_str(), pot->PinNumber);
                    this->state->Potentiometers->emplace(pot->PinNumber, pot);
                }

                logger->Info("Pots registered in state");

                this->midiHardware = new miku::tasks::hardware::MidiHardware(this->state, hardware, 14, 13);

                ux::MikuOledDisplay mikuDisplay;

                this->display = new ux::Display(hardware, mikuDisplay);

                logger->Info("Building screens");
                this->buildScreens();

                logger->Info("Building tasks");
                this->buildTasks();

                logger->Info("Mapping screen data dependencies");

                // Map screen data dependencies to tasks manually for complex structures
                // TODO this might be obsoleted b/c now we have shared state
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

                logger->Info("Initializing ADC");
                this->initAdc();

                logger->Info("Enabling tasks");
                this->enableTasks();
            }

            void enableTasks() {
                for (miku::tasks::Task* task : this->tasks) {
                    if (task != nullptr) {
                        logger->Info("Enabling task %s", task->GetCode().c_str());
                        task->Enable();
                    }
                }
            }

            void initAdc() {
                display->Fill(false);

                daisy::AdcChannelConfig adcConfigs[16];

                unsigned short configuredChannelCount = 0;

                for (auto potEntries : *this->state->Potentiometers) {
                    auto pot = potEntries.second;

                    this->logger->Info("Configuring pot %s on pin %d with ADC index %d", pot->Code.c_str(), pot->PinNumber, configuredChannelCount);

                    if (pot != nullptr) {
                        adcConfigs[configuredChannelCount].InitSingle(hardware.GetPin(pot->PinNumber));
                        pot->ADCIndex = configuredChannelCount;
                        configuredChannelCount++;
                    }
                }

                this->logger->Info("Configured %d ADC channels, initting ADC", configuredChannelCount);

                this->hardware.adc.Init(adcConfigs, configuredChannelCount);

                this->logger->Info("ADC initted, starting");

                this->hardware.adc.Start();
            }

            /// @brief Main while loop of the application
            void Run() {
                logger->Info("Rendering splash screen, version %s", this->GetVersion().c_str());
                miku::ux::screens::SplashScreen* splashScreen = new miku::ux::screens::SplashScreen {
                    this->GetDisplay(),
                    this->state,
                    this->version
                };
                splashScreen->Render();
                this->GetDisplay()->Invalidate();
                daisy::System::Delay(splashDuration);

                logger->Info("Splash done, starting main loop");
                
                this->GetDisplay()->Fill(false);
                this->GetDisplay()->Invalidate();

                unsigned long lastRender = daisy::System::GetNow();

                // Ensure currentScreen is set
                this->state->ScreenIndex = 0;
                this->currentScreen = screens[this->state->ScreenIndex];

                while (!this->interrupt) {
                    this->checkTasks();

                    unsigned long now = daisy::System::GetNow();

                    if (now - lastScreenCheck > 250) {
                        lastScreenCheck = now;
                        this->desiredScreenIndex = this->determineDesiredScreenIndex();
                    }

                    if (this->desiredScreenIndex != this->state->ScreenIndex) {
                        logger->Info("Switching to screen index %d", this->desiredScreenIndex);
                        this->state->ScreenIndex = desiredScreenIndex;
                        this->currentScreen = screens[this->state->ScreenIndex];
                    }

                    if(now - lastRender > 100) {
                        lastRender = now;
                        logger->Debug("Rendering screen %d", this->state->ScreenIndex);

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
                    },
                    new miku::ux::screens::PotTestScreen {
                        this->GetDisplay(),
                        this->state
                    },
                    new miku::ux::screens::MidiEventsScreen {
                        this->GetDisplay(),
                        this->state
                    },               
                    new miku::ux::screens::SyllableScreen {
                        this->GetDisplay(),
                        this->state
                    }
                };
            }

            /// @brief Declares the tasks to run
            void buildTasks() {
                this->logger->Info("Registering static tasks");

                this->tasks = std::vector<miku::tasks::Task*> {
                    new miku::tasks::hardware::BlinkyLedTask(hardware, this->state),
                    new miku::tasks::hardware::MidiRelayTask(hardware, this->state, this->midiHardware)
                };    

                this->logger->Info("Registering potentiometer tasks");            

                for (auto item : *this->state->Potentiometers) {
                    auto potState = item.second;

                    this->logger->Info("Registering potentiometer task %s", potState->Code.c_str());

                    tasks::hardware::LinearPotentiometerTask* potTask = new miku::tasks::hardware::LinearPotentiometerTask(hardware, this->state, potState);

                    this->logger->Info("Pot task created, pushing_back");
                    
                    this->tasks.push_back(potTask);
                }

                this->logger->Info("Done registering tasks");
            }

            /// @brief Determines the screen index that should be shown based on the current potentiometer value
            /// @return The index that should be shown - note, this does NOT set the desired/current screen index
            unsigned short determineDesiredScreenIndex() {
                // TODO unhardcode
                uint16_t screenPotCurrent = this->state->Potentiometers->at(this->SCREEN_POT_PIN_NUMBER)->CurrentValue;

                uint16_t desiredScreenIndex =  Clamper::ReadingToIndex(screenPotCurrent, this->screens.size());

                logger->Debug("Desired screen index: %d", desiredScreenIndex);

                return desiredScreenIndex;
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
            /// @brief The screen that's currently being displayed
            miku::ux::Screen* currentScreen;
            /// @brief Tasks registered with the app
            std::vector<miku::tasks::Task*> tasks;
            /// @brief The state of the XXI The World
            data::State* state;
            /// @brief General logging utility
            utils::Logger* logger;

            unsigned long lastScreenCheck = 0;

            const u_int8_t SCREEN_POT_PIN_NUMBER = 21;
    };
}

#endif
