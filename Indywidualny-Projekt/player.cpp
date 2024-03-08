#include "player.h"

Player::Player() {
	x = 0;
	y = 0;
}

void Player::setPosition(float x_, float y_) {
	x = x_;
	y = y_;
}

void Player::moveX(float offset) {
	x += offset;
}

void Player::moveY(float offset) {
	y += offset;
}

float Player::getX() const { return x; }
float Player::getY() const { return y; }