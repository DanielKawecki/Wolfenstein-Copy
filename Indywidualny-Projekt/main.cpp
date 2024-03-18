#include <iostream>
#include "game.h"


int main(void) {
    Game game;
    game.pushState(new MenuState(&game));
    game.run();

    return 0;
}
