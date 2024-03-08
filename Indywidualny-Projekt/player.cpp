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
	x += offset * 0.07;
}

void Player::moveY(float offset) {
	y += offset * 0.07;
}

float Player::getX() const { return x; }
float Player::getY() const { return y; }
float Player::getAngle() const { return angle; }