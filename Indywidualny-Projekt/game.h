
#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include "player.h"

class Game {
private:
    GLFWwindow* window;
    int screenWidth, screenHeight;
    const char* windowTitle;
    
    float delta_time = 0;
    float current_time = 0;
    float previous_time = 0;

    std::vector<std::string> map_layout;
    int map_width = 8;

    Player player;

    // Initializing functions
    void initializeGL();
    void initilizeMap();

    // Main functions
    void update();
    void render();
    void setDeltaTime();

    // Casting rays related functions
    void drawRays3d();
    float getRayLength(float a_x, float a_y, float b_x, float b_y, float angle);

    // Additional functions
    void drawPlayer2d();
    void drawMap2d();

public:
    Game(int width, int height, const char* title);
    ~Game();

    void run();

};

