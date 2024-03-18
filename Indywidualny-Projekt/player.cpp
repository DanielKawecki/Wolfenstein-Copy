#include "player.h"

Player::Player() {
	x = 200;
	y = 200;
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
    if (!checkWalls(x + dx, y))
        x += dx;

    if (!checkWalls(x, y + dy))
        y += dy;

    setRect(x, y);
}

void Player::setTilePosition(int tile_size) {
    tile_x = floor(x / tile_size);
    tile_y = floor(y / tile_size); 
}

bool Player::checkWalls(float dx, float dy) {
    setTilePosition(64);
    Rect player_rect;
    player_rect.top = dy - size;
    player_rect.bottom = dy + size;
    player_rect.left = dx - size;
    player_rect.right = dx + size;
   
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (map_layout[tile_y + i][tile_x + j] == '#') {
                float wall_y = (tile_y + i) * 64;
                float wall_x = (tile_x + j) * 64;
                Rect wall_rect = { wall_x, wall_y, wall_x + 64, wall_y + 64 };
                
                if (intersects(player_rect, wall_rect))
                    return true;
            }
        }
    }
            
    return false;
}

void Player::setRect(float x_, float y_) {
    rect.top = y_ - size;
    rect.bottom = y_ + size;
    rect.left = x_ - size;
    rect.right = x_ + size;
}

void Player::setWalls(std::vector<std::string> map_layout_) {
    map_layout = map_layout_;
}

bool Player::intersects(const Rect& rect1, const Rect& rect2) {
    if (rect1.right <= rect2.left || 
        rect2.right <= rect1.left || 
        rect1.bottom <= rect2.top || 
        rect2.bottom <= rect1.top) { 
        return false;
    }
    
    return true;
}

float Player::getX() const { return x; }
float Player::getY() const { return y; }

int Player::getTileX() const { return tile_x; }
int Player::getTileY() const { return tile_y; }

float Player::getAngle() const { return angle; }

Rect Player::getRect() {
    return rect;
}
