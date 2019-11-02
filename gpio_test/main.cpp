
#include <stdio.h>
#include <thread>

#include "gpio.h"

int pins[2] = {80,182};
bool high[2] = {true,true};
bool low[2] = {false,false};

gpio io(pins);

int main(){

    // Initialize pins
    io.set_mode(pins,high);

    while(1){

        io.set_level(pins,high);

        std::this_thread::sleep_for(std::chrono::seconds(1));

        io.set_level(pins,low);

        std::this_thread::sleep_for(std::chrono::seconds(1));

    }

    return 0;

}

