
#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <math.h>
#include "player.h"

#define PI 3.141592653589

class Game {
private:
    GLFWwindow* window;
    int screenWidth, screenHeight;
    const char* windowTitle;
    
    float delta_time = 0;
    float current_time = 0;
    float previous_time = 0;

    std::vector<std::string> map_layout;

    Player player;

    // Initializing functions
    void initializeGL();
    void initilizeMap();

    // Main functions
    void update();
    void render();
    void setDeltaTime();

    // Additional functions
    void drawPlayer2d();
    void drawMap2d();

public:
    Game(int width, int height, const char* title);
    ~Game();

    void run();

};

