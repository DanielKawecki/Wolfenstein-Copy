#include "player.h"

Player::Player() {
	x = 300;
	y = 300;
}

void Player::setPosition(float x_, float y_) {
	x = x_;
	y = y_;
}

void Player::handleInput(GLFWwindow* window, float delta_time) {
    float vx = speed * cos(angle) * delta_time;
    float vy = speed * sin(angle) * delta_time;

    float dx = 0;
    float dy = 0;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        dx += vx;
        dy += vy;
    }
        
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        dx += -vx;
        dy += -vy;
    }
        
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        dx += vy;
        dy += -vx;
    }
        
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        dx += -vy;
        dy += vx;
    }
        
    // This function will move players position if collision is not detected
    movePosition(dx, dy);

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        angle += rotation_speed * delta_time;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        angle -= rotation_speed * delta_time;

    if (angle < 0)
        angle += 2 * PI;
}

void Player::movePosition(float dx, float dy) {
    // !checkWalls(position.x + dx, position.y)
    if (true)
        x += dx;

    // !checkWalls(position.x, position.y + dy)
    if (true)
        y += dy;
}

float Player::getX() const { return x; }
float Player::getY() const { return y; }
float Player::getAngle() const { return angle; }