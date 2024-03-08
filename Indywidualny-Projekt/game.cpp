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
    glfwSetKeyCallback(window, handleInput());
}

void Game::render() {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    drawPlayer2d();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
}

//void Game::handleInput(unsigned char key, int x, int y) {
//    if (key == 'w') { player.moveY(1.f); }
//    if (key == 's') { player.moveY(-1.f); }
//
//    
//}

void handleInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        player.moveY(1.f);
}

void Game::drawPlayer2d() {
    glColor3f(0, 1, 0);
    glPointSize(50.f);
    glBegin(GL_POINTS);
    glVertex2i(100, player.getY());
    glEnd();
}

void Game::run() {
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        render();
    }
}
