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
        "################################",
        "#          #                   #",
        "#          #      ###          #",
        "### ###### #       #   ##   ####",
        "#        #    #        #       #",
        "#             #                #",
        "#        #       ## # ##    #  #",
        "## #     #########     ##   #  #",
        "#  #      #     #   #   #      #",
        "#  # #    #         #   ### # ##",
        "#    #          #   #   #      #",
        "#    #    #     #              #",
        "################################"
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

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
}

void Game::setDeltaTime() {
    current_time = static_cast<float>(glfwGetTime());
    delta_time = current_time - previous_time;
    previous_time = current_time;
}

void Game::drawPlayer2d() {
    glColor3f(0, 1, 0);
    glPointSize(10.f);
    glBegin(GL_POINTS);
    glVertex2i(player.getX(), player.getY());
    glEnd();
}

void Game::drawMap2d() {
    int x0 = 0;
    int y0 = 0;
    int tile_size = 32;

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
