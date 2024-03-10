#include <iostream>
#include "game.h"


int main(void) {
    try {
        Game game(1280, 720, "Wolfenstein");
        game.run();
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}