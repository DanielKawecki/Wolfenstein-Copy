#include <iostream>
#include "game.h"


int main(void) {
 /*   try {
        Game game;
        game.run();
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }*/
    Game game;
    game.pushState(new MenuState(&game));
    game.run();

    return 0;
}

//#include <SFML/Window.hpp>
//#include "app.h"
//
//int main()
//{
//    Game game;
//    game.pushState(new MenuState(&app));
//    game.run();
//
//    return 0;
//}