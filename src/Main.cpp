#define APPLICATION_DEBUG_MODE
// para release é necessario apenas omitir define.
#define CXX26_DEBUG
// use this if you wanna use the metaprogramming resources.

#include <iostream>
#include "UpRace.h"

int main() {
    #ifdef APPLICATION_DEBUG_MODE        
        std::cerr << "APPLICATION_DEBUG_MODE\n";
    #endif

    UpRace Game;
    Game.Loop(); 

    return 0;
}
