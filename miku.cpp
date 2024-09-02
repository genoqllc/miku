
#include "daisy_seed.h"

#include "lib/miku/App.hpp"

miku::App* theApp;

int main(void)
{
    daisy::DaisySeed hardware;

    //hardware.StartLog(false);
    //hardware.PrintLine("Miku - A Daisy Seed based vocal synthesizer\n");

    theApp = new miku::App(hardware);
    theApp->Run();
}
