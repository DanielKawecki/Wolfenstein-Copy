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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        y += speed * delta_time;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        y -= speed * delta_time;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        x -= speed * delta_time;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        x += speed * delta_time;
}

void Player::moveX(float delta_time) {
	x += 1 * delta_time;
}

void Player::moveY(float delta_time) {
	y += 1 * delta_time;
}

float Player::getX() const { return x; }
float Player::getY() const { return y; }
float Player::getAngle() const { return angle; }