
#pragma once

#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <SOIL2/SOIL2.h>
#include <stack>
#include "player.h"

class State;

struct Textures {
    GLuint test;
};

class Game {
private:
    GLFWwindow* window = nullptr;
    int screen_width = 1024;
    int screen_height = 512;
    const char* window_title = "Wolfenstein";
    
    float delta_time = 0;
    float current_time = 0;
    float previous_time = 0;

    std::vector<std::string> map_layout;
    int map_width = 8;
    int tile_size = 64;

    Player player;
    Textures textures;
    
    // Font rendering data
    int font_height;

    // Initializing functions
    void initializeGL();
    void initilizeMap();
    void setPlayerPosition();
    void inicializeTextures();
    GLuint loadTexture(const char* texturePath);

    // Casting rays related functions
    void drawRays3d();
    float getRayLength(float a_x, float a_y, float b_x, float b_y, float angle);

    // State Machine related functions
    std::stack<State*> states;
    void popState();
    void changeState(State* state_);
    State* peekState();

    void menuHandleInput();

    // Drawing functions
    void drawPlayer2d();
    void drawMap2d();
    void drawLine(float a_x, float a_y, float b_x, float b_y);

    // Math functions
    int min(int a, int b) { return (a < b) ? a : b; };
    int max(int a, int b) { return (a > b) ? a : b; };

public:
    Game();
    ~Game();

    void pushState(State* state_);
    void run();

    //Main functions for menu state
    void updateMenu();
    void renderMenu();

    // Main functions for playing state
    void updatePlaying();
    void renderPlaying();
    void setDeltaTime();

};

#include "state.h"
