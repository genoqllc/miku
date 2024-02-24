#include <stdio.h>
#include <string.h>
#include <string>
#include "daisy_seed.h"
// #include "dev/oled_ssd130x.h"

// #include "lib/miku/ux/Resources.hpp"
// #include "lib/miku/adc/potentiometer.hpp"
// #include "lib/miku/midi/midi.hpp"

// #include "lib/miku/ux/Callbacks.hpp"
#include "lib/miku/App.hpp"

//using namespace daisy;

// using MyOledDisplay = OledDisplay<SSD130xI2c128x64Driver>;

// DaisySeed     hw;
// //MyOledDisplay display;
// MidiUartHandler midi;

// /** FIFO to hold messages as we're ready to print them */
// FIFO<MidiEvent, 128> event_log;

// const int ROW_SPACING_PX = 2;

// array mapping of midi note value (0-127) to english name

// const unsigned int EV_SHOW_MIDI = 0;
// const unsigned int EV_BLINK_LED = 1;

// const unsigned int EVENT_TIMER_TIMEOUTS[2] = {
//     2000,
//     500
// };

miku::App* theApp;

int main(void)
{

    // hw.Configure();
    //hw.Init();
    // hw.StartLog();

    daisy::DaisySeed hardware;

    theApp = new miku::App(hardware);
    theApp->Run();

    // AdcChannelConfig adcConfig;
    // adcConfig.InitSingle(hw.GetPin(21));
    // hw.adc.Init(&adcConfig, 1);
    // hw.adc.Start();

    // Switch cycle_screen_button;
    // cycle_screen_button.Init(hw.GetPin(28), 1000);

    // MidiUartHandler::Config midi_config;
    // // midi_config.transport_config.periph = UartHandler::Config::Peripheral::USART_1;
    // // midi_config.transport_config.rx.pin = hw.GetPin(30).pin;
    // // midi_config.transport_config.tx.pin = hw.GetPin(29).pin;
    // midi.Init(midi_config);

    // midi.StartReceive();

    // uint32_t start_time = System::GetNow();

    // uint32_t event_timers[2] = {
    //     start_time,
    //     start_time
    // };

    /** Configure the Display */
    // MyOledDisplay::Config disp_cfg;
    // disp_cfg.driver_config.transport_config.i2c_address               = 0x3C;
    // disp_cfg.driver_config.transport_config.i2c_config.periph         = I2CHandle::Config::Peripheral::I2C_1;
    // disp_cfg.driver_config.transport_config.i2c_config.speed          = I2CHandle::Config::Speed::I2C_1MHZ;
    // disp_cfg.driver_config.transport_config.i2c_config.mode           = I2CHandle::Config::Mode::I2C_MASTER;
    // disp_cfg.driver_config.transport_config.i2c_config.pin_config.scl = hw.GetPin(11); // {DSY_GPIOB, 11};
    // disp_cfg.driver_config.transport_config.i2c_config.pin_config.sda = hw.GetPin(12); // {DSY_GPIOB, 12};

    // /** And Initialize */
    // display.Init(disp_cfg);

    // // bool led_state;
    // // led_state = true;

    // draw_splash();
    // System::Delay(10000);

    // display.Fill(false);
    // draw_string(0, 0, "MIDI events:");
    // display.Update();

    // bool tick_tock = false;

    // float max_pot_value = 0.0f;
    // float min_pot_value = 1.0f;



    // while(1)
    // {
    //     bool displayNeedsUpdate = false;

    //     uint32_t now = System::GetNow();

    //     cycle_screen_button.Debounce();
    //     bool cycle_screen_button_pressed = cycle_screen_button.Pressed();
        //     if (cycle_screen_button_pressed) {
    //         char button_press_buffer[2];
    //         sprintf(button_press_buffer, "*");
    //         draw_string(112, 0, button_press_buffer);
    //         displayNeedsUpdate = true;
    //     }

    //     midi.Listen();

    //     int event_count = 0;

    //     float last_pot_value = -1.0f;

    //     while(midi.HasEvents()) {
    //         event_count++;
    //         MidiEvent msg = midi.PopEvent();

    //         /** Handle messages as they come in 
    //          *  See DaisyExamples for some examples of this
    //          */
    //         switch(msg.type)
    //         {
    //             // TODO properly calculate channel based on incoming message
    //             case NoteOn:
    //                 {
    //                     uint8_t bytes[3] = {0x90, 0x00, 0x00};
    //                     bytes[1] = msg.data[0];
    //                     bytes[2] = msg.data[1];
    //                     midi.SendMessage(bytes, 3);
    //                 }
    //                 break;
    //             case NoteOff:
    //                 {
    //                     uint8_t bytes[3] = {0x80, 0x00, 0x00};
    //                     bytes[1] = msg.data[0];
    //                     bytes[2] = msg.data[1];
    //                     midi.SendMessage(bytes, 3);
    //                 }
    //                 break;
    //             default: break;
    //         }

    //         /** Regardless of message, let's add the message data to our queue to output */
    //         event_log.PushBack(msg);
    //     }

    //     if(now - event_timers[EV_SHOW_MIDI] > EVENT_TIMER_TIMEOUTS[EV_SHOW_MIDI])
    //     {
    //         event_timers[EV_SHOW_MIDI] = now;
    //         char outstr[128];

    //         display.Fill(false);
    //         sprintf(outstr, "MIDI events:");
    //         draw_string(0, 0, outstr);
    //         sprintf(outstr, "Event ct: %d", event_log.GetNumElements());
    //         draw_string(0, 10, outstr);

    //         if(!event_log.IsEmpty())
    //         {
    //             for (int row_index = 0; row_index < 4; row_index++)
    //             {
    //                 if(event_log.IsEmpty()) break;
    //                 auto msg = event_log.PopFront();
    //                 char type_str[16];
    //                 miku::midi::GetMidiTypeAsString(msg, type_str);
                    
    //                 if (msg.type == NoteOn || msg.type == NoteOff) {
    //                     std::string noteName = miku::midi::GetNameFromNoteNumber(msg.data[0]);
    //                     sprintf(outstr, "%s ch%d %s", type_str, msg.channel + 1, noteName.c_str());
    //                 }
    //                 else {
    //                     sprintf(outstr, "%s ch%d", type_str, msg.channel + 1);
    //                 }
    //                 draw_string(0, 30 + (row_index * 10), outstr);
    //             }
    //         }

    //         event_log.Clear();

    //         displayNeedsUpdate = true;
    //     }

    //     if (now - event_timers[EV_BLINK_LED] > EVENT_TIMER_TIMEOUTS[EV_BLINK_LED]) {
    //         char tick_tock_buffer[2];
    //         sprintf(tick_tock_buffer, "%c", tick_tock ? '\\' : '/');
    //         draw_string(120, 0, tick_tock_buffer);
    //         tick_tock = !tick_tock;

    //         displayNeedsUpdate = true;
    //     }

    //     if (cycle_screen_button_pressed) {
    //         char button_press_buffer[2];
    //         sprintf(button_press_buffer, "*");
    //         draw_string(112, 0, button_press_buffer);
    //         displayNeedsUpdate = true;
    //     }


    //     float current_pot_value = hw.adc.GetFloat(0);
    //     if (current_pot_value != last_pot_value) {
    //         if (current_pot_value > max_pot_value) {
    //             max_pot_value = current_pot_value;
    //         }

    //         if (current_pot_value < min_pot_value) {
    //             min_pot_value = current_pot_value;
    //         }

    //         last_pot_value = current_pot_value;
    //         char pot_value_buffer[32];
    //         sprintf(pot_value_buffer, "Pot:%.3f", last_pot_value);
    //         draw_string(0, 20, pot_value_buffer);
    //         sprintf(pot_value_buffer, "Max:%.3f, Min:%.3f", max_pot_value, min_pot_value);
    //         draw_string(0, 30, pot_value_buffer);
    //         sprintf(pot_value_buffer, "mD:%.3f,MD:%.3f", current_pot_value - min_pot_value, max_pot_value - current_pot_value);
    //         draw_string(0, 40, pot_value_buffer);

    //         displayNeedsUpdate = true;
    //     }

    //     if (displayNeedsUpdate) {
    //         display.Update();
    //     }
    // }
}
