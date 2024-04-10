
#pragma once

#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <SOIL2/SOIL2.h>
#include <stack>
#include <map>
#include <algorithm>
#include "player.h"
#include "enemy.h"
#include "tile.h"
#include "clock.h"

class State;

struct Textures {
    GLuint title_screen;
    GLuint highlight;
    GLuint test;
    GLuint greystone;
    GLuint eagle;
    GLuint red_brick;
    GLuint guard_stationary;
    GLuint health_pack;
    GLuint ammo_pack;
};

struct Drawable {
    float x;
    float y;
    float size_x;
    float size_y;
    GLuint texture;
    float distance;
    float slice_offset;
    std::string type;
};

struct Refill {
    float x;
    float y;
    float z = -32.f;
    float width = 150.f;
    std::string type;
};

class Game {
private:
    GLFWwindow* window = nullptr;
    int screen_width = 960;
    int screen_height = 600;
    const char* window_title = "Wolfenstein";
    Clock clock;
    
    float delta_time = 0;
    float current_time = 0;
    float previous_time = 0;

    std::vector<std::string> map_layout;
    std::map<char, GLuint> texture_atlas;
    std::string wall_chars = "123#";
    int map_width = 8;
    int tile_size = 64;

    Player player;
    std::vector<Enemy> all_enemies;
    std::vector<Drawable> all_drawables;
    std::vector<Refill> all_refills;

    Tile* player_tile = nullptr;
    Tile* enemy_tile = nullptr;
    std::vector<std::vector<Tile>> bsf_tiles;

    Textures textures;

    int menu_buttons_count = 2;
    int dead_buttons_count = 3;
    int highlight = 0;

    bool is_Enter_pressed = false;

    // TODO: 
    // Strzelanie gracza
    // Przeciwnik nie powinien strzelaæ przez œciany
    // Tekstury do apteczek
    // Tekstury dla nowej rozdzielczoœci

    // Raycasting variables
    float FOV = PI / 3.f;
    int number_of_rays = screen_width / 2;
    float delta_angle = FOV / (float)number_of_rays;
    float projection_distance = (0.5f * (float) screen_width / tan(0.5f * FOV)) + 41.f;
    int max_depth = 8;

    // Initializing functions
    void initializeGL();
    void initilizeMap();
    void readFromMap();
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

    // Updating functions
    void updateEnemies();
    void updateRefills();

    // Drawing functions
    void drawPlayer2d();
    void drawMap2d();
    void drawLine(float a_x, float a_y, float b_x, float b_y);
    void drawSlice(float x, float y, float size_x, float size_y, GLuint texture, float distance, float slice_offset);
    void drawRefills();
    void drawEnemies();
    void drawDrawable();

    // Helper functions
    bool stringContains(const std::string& string, char ch);
    Tile* getTile(int row, int column);
    bool isEnterPressed();

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

    // Main functions for pleyer died state
    void updateDead();
    void renderDead();

};

#include "state.h"
