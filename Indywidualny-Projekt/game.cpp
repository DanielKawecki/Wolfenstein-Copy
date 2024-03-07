#include <iostream>
#include "game.h"

Game::Game(int width, int height, const char* title)
    : screenWidth(width), screenHeight(height), windowTitle(title), window(nullptr) {
    initialize();
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
    glOrtho(0.0f, 640.0f, 0.f, 480.f, 0.0f, 100.0f);
}

void Game::render() {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    drawPlayer2d();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
}

void Game::handleEvents() {
    glfwPollEvents();
}

void Game::drawPlayer2d() {
    glColor3f(1, 1, 0);
    glPointSize(50.f);
    glBegin(GL_POINTS);
    glVertex2i(100, 100);
    glEnd();
    glColor3f(1, 0, 0);
    glPointSize(10.f);
    glBegin(GL_POINTS);
    glVertex2i(100, 100);
    glEnd();
}

void Game::run() {
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        render();
        handleEvents();
    }
}
