
#pragma once

#include <GLFW/glfw3.h>

class Game {
private:
    GLFWwindow* window;
    int screenWidth, screenHeight;
    const char* windowTitle;

    void initialize();
    void render();
    void handleEvents();
    void drawPlayer2d();

public:
    Game(int width, int height, const char* title);
    ~Game();

    void run();

};

