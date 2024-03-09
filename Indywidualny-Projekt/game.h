
#pragma once

#include <GLFW/glfw3.h>
#include "player.h"

class Game {
private:
    GLFWwindow* window;
    int screenWidth, screenHeight;
    const char* windowTitle;
    
    float delta_time = 0;
    float current_time = 0;
    float previous_time = 0;

    Player player;

    // Initializing functions
    void initialize();

    // Main functions
    void update();
    void render();
    void setDeltaTime();

    // Additional functions
    void drawPlayer2d();

public:
    Game(int width, int height, const char* title);
    ~Game();

    void run();

};

