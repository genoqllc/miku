#include <stdio.h>
#include <string.h>
#include <string>
#include "daisy_seed.h"

// #include "dev/oled_ssd130x.h"

// #include "lib/miku/ux/Resources.hpp"
// #include "lib/miku/adc/potentiometer.hpp"
// #include "lib/miku/midi/midi.hpp"

#include "lib/miku/App.hpp"

// /** FIFO to hold messages as we're ready to print them */
// FIFO<MidiEvent, 128> event_log;

miku::App* theApp;

daisy::Switch myButton;


int main(void)
{
    daisy::DaisySeed hardware;

    theApp = new miku::App(hardware);
    theApp->Run();

    // Switch cycle_screen_button;
    // cycle_screen_button.Init(hw.GetPin(28), 1000);

    // MidiUartHandler::Config midi_config;
    // // midi_config.transport_config.periph = UartHandler::Config::Peripheral::USART_1;
    // // midi_config.transport_config.rx.pin = hw.GetPin(30).pin;
    // // midi_config.transport_config.tx.pin = hw.GetPin(29).pin;
    // midi.Init(midi_config);

    // midi.StartReceive();

    // while(1)
    // {

    //     midi.Listen();

    //     int event_count = 0;

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


    //     if (cycle_screen_button_pressed) {
    //         char button_press_buffer[2];
    //         sprintf(button_press_buffer, "*");
    //         draw_string(112, 0, button_press_buffer);
    //         displayNeedsUpdate = true;
    //     }

    // }
}
