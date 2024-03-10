#include <iostream>
#include "game.h"

Game::Game(int width, int height, const char* title)
    : screenWidth(width), screenHeight(height), windowTitle(title), window(nullptr) {
    initializeGL();
    initilizeMap();
    player = Player();
}

Game::~Game() {
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}

void Game::initializeGL() {
    /* Initialize the library */
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screenWidth, screenHeight, windowTitle, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);
}

void Game::initilizeMap() {
    map_layout = {
        "########",
        "#      #",
        "#   #  #",
        "#   #  #",
        "#      #",
        "#  ##  #",
        "#  #   #",
        "########",
    };
}

void Game::update() {
    setDeltaTime();
    player.handleInput(window, delta_time);
}

void Game::render() {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    drawMap2d();
    drawPlayer2d();
    drawRays3d();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
}

void Game::setDeltaTime() {
    current_time = static_cast<float>(glfwGetTime());
    delta_time = current_time - previous_time;
    previous_time = current_time;
}

void Game::drawRays3d() {
    int depth_of_field;
    int map_x;
    int map_y;
    int map_position;
    float player_x = player.getX();
    float player_y = player.getY();
    float ray_angle = 0;
    float ray_x;
    float ray_y;
    float x_offset = 64;
    float y_offset = 64;
    float player_angle = player.getAngle();
    ray_angle = player_angle;

    for (int rays = 0; rays < 1; rays++) {
        ////--- Chcecking Horizontal Lines ---
        depth_of_field = 0;
        float horizontal_distance = 1000000;
        float horizontal_x = player_x;
        float horizontal_y = player_y;
        float angle_tangent = -1 / tan(ray_angle);
        if (ray_angle > PI) {
            ray_y = (((int)player_y >> 6) << 6) - 0.0001;
            ray_x = (player_y - ray_y) * angle_tangent + player_x;
            y_offset = -64;
            x_offset = -y_offset * angle_tangent;
        }
        if (ray_angle < PI) {
            ray_y = (((int)player_y >> 6) << 6) + 64;
            ray_x = (player_y - ray_y) * angle_tangent + player_x;
            y_offset = 64;
            x_offset = -y_offset * angle_tangent;
        }
        if (ray_angle == 0 || ray_angle == PI) {
            ray_x = player_x;
            ray_y = player_y;
            depth_of_field = 8;
        }
        while (depth_of_field < 8) {
            map_x = (int)(ray_x) >> 6;
            map_y = (int)(ray_y) >> 6;
            map_position = map_y * map_width + map_x;
            //std::cout << map_x << " " << map_y << std::endl; /////////
            if (map_x < 8 && map_x >= 0 && map_layout[map_x][map_y] == '#') {
                horizontal_x = ray_x;
                horizontal_y = ray_y;
                horizontal_distance = getRayLength(player_x, player_y, horizontal_x, horizontal_y, ray_angle);
                depth_of_field = 8; // Ending the while loop
            }
            else {
                ray_x += x_offset;
                ray_y += y_offset;
                depth_of_field += 1;
            }
        }

        //--- Chcecking Vertical Lines ---
        depth_of_field = 0;
        float vertical_distance = 1000000;
        float vertical_x = player_x;
        float vertical_y = player_y;
        float negative_tangent = -tan(ray_angle);
        if (ray_angle > (PI / 2.f) && ray_angle < (3.f * PI / 2.f)) {
            ray_x = (((int)player_x >> 6) << 6) - 0.0001;
            ray_y = (player_x - ray_x) * negative_tangent + player_y;
            x_offset = -64;
            y_offset = -x_offset * negative_tangent;
        }
        if (ray_angle < (PI / 2.f) || ray_angle > (3.f * PI / 2.f)) {
            ray_x = (((int)player_x >> 6) << 6) + 64;
            ray_y = (player_x - ray_x) * negative_tangent + player_y;
            x_offset = 64;
            y_offset = -x_offset * negative_tangent;
        }
        if (ray_angle == 0 || ray_angle == PI) {
            ray_x = player_x;
            ray_y = player_y;
            depth_of_field = 8;
        }
        while (depth_of_field < 8) {
            map_x = (int)(ray_x) >> 6;
            map_y = (int)(ray_y) >> 6;
            map_position = map_y * map_width + map_x;
            //std::cout << map_x << " " << map_y << std::endl; /////////
            if (map_y < 8 && map_y >= 0 && map_layout[map_x][map_y] == '#') {
                vertical_x = ray_x;
                vertical_y = ray_y;
                vertical_distance = getRayLength(player_x, player_y, vertical_x, vertical_y, ray_angle);
                depth_of_field = 8;
            }
            else {
                ray_x += x_offset;
                ray_y += y_offset;
                depth_of_field += 1;
            }
        }

        if (vertical_distance < horizontal_distance) {
            ray_x = vertical_x;
            ray_y = vertical_y;
        }
        else if (vertical_distance > horizontal_distance) {
            ray_x = horizontal_x;
            ray_y = horizontal_y;
        }
            
        glColor3f(1, 0, 0);
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex2i(player_x, player_y);
        glVertex2i(ray_x, ray_y);
        glEnd();
    }
    
}

float Game::getRayLength(float a_x, float a_y, float b_x, float b_y, float angle) {
    return (sqrt((b_x - a_x) * (b_x - a_x) + (b_y - a_y) * (b_y - a_y)));
}

void Game::drawPlayer2d() {
    glColor3f(0, 1, 0);
    glPointSize(10.f);
    glBegin(GL_POINTS);
    glVertex2i(player.getX(), player.getY());
    glEnd();

    // Direction that player looks at
    /*float angleRadians = player.getAngle();

    float endX = player.getX() + 20.0f * cos(angleRadians);
    float endY = player.getY() + 20.0f * sin(angleRadians);

    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(player.getX(), player.getY());
    glVertex2f(endX, endY);
    glEnd();*/
}

void Game::drawMap2d() {
    int x0 = 0;
    int y0 = 0;
    int tile_size = 64;

    for (size_t i = 0; i < map_layout.size(); ++i) {
        for (size_t j = 0; j < map_layout[i].size(); ++j) {
            if (map_layout[i][j] == '#') {
                glColor3f(1, 1, 1); 
            }
            else { 
                glColor3f(0, 0, 0); 
            }
            x0 = i * tile_size;
            y0 = j * tile_size;
            glBegin(GL_QUADS);
            glVertex2i(x0 + 1, y0 + 1);
            glVertex2i(x0 + 1, y0 + tile_size - 1);
            glVertex2i(x0 + tile_size - 1, y0 + tile_size - 1);
            glVertex2i(x0 + tile_size - 1, y0 + 1);
            glEnd();
        }
    }
}

void Game::run() {
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        update();
        render();
    }
}
