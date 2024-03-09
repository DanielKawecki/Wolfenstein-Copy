#include <iostream>
#include "game.h"

Game::Game(int width, int height, const char* title)
    : screenWidth(width), screenHeight(height), windowTitle(title), window(nullptr) {
    initialize();
    player = Player();
}

Game::~Game() {
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}

void Game::initialize() {
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
    glOrtho(0, screenWidth, 0, screenWidth, -1, 1);
}

void Game::update() {
    setDeltaTime();
    player.handleInput(window, delta_time);
}

void Game::render() {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

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
    glPointSize(50.f);
    glBegin(GL_POINTS);
    glVertex2i(player.getX(), player.getY());
    glEnd();
}

void Game::run() {
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        update();
        render();
    }
}
