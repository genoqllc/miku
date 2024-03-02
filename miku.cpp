
#include "daisy_seed.h"

#include "lib/miku/App.hpp"

miku::App* theApp;

int main(void)
{
    daisy::DaisySeed hardware;

    theApp = new miku::App(hardware);
    theApp->Run();
}
