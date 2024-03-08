
#pragma once

#include <GLFW/glfw3.h>
#include "player.h"

class Game {
private:
    GLFWwindow* window;
    int screenWidth, screenHeight;
    const char* windowTitle;

    Player player;

    void initialize();
    void render();
    void handleInput(GLFWwindow* window);
    void drawPlayer2d();

public:
    Game(int width, int height, const char* title);
    ~Game();

    void run();

};

