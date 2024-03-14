#include "player.h"

Player::Player(std::vector<std::string> map_layout_) {
	x = 100;
	y = 200;
    map_layout = map_layout_;
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
        
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        dx += vy;
        dy += -vx;
    }
        
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        dx += -vy;
        dy += vx;
    }
        
    // This function will move players position if collision is not detected
    movePosition(dx, dy);

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        angle -= rotation_speed * delta_time;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        angle += rotation_speed * delta_time;

    if (angle < 0)
        angle += 2 * PI;
    if (angle > 2 * PI)
        angle -= 2 * PI;
}

void Player::movePosition(float dx, float dy) {
    // !checkWalls(position.x + dx, position.y)
    if (true)
        x += dx;

    // !checkWalls(position.x, position.y + dy)
    if (true)
        y += dy;

    setRect();
}

void Player::setTilePosition(int tile_size) {
    tile_x = floor(x / tile_size);
    tile_y = floor(y / tile_size); 
}

bool Player::checkWalls(float dx, float dy) {
    for (int i = 0; i < 1; i++)
        for (int j = 0; j < 1; j++)
            
    return false;
}

void Player::setRect() {
    top = y - size;
    bottom = y + size;
    left = x - size;
    right = x + size;
}

float Player::getX() const { return x; }
float Player::getY() const { return y; }

int Player::getTileX() const { return tile_x; }
int Player::getTileY() const { return tile_y; }

float Player::getAngle() const { return angle; }

std::vector<float> Player::getRect() {
    return { top, bottom, left, right };
}
