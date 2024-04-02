#pragma once

#include "enemy.h"

Enemy::Enemy(float x_, float y_, float z_) {
	x = x_;
	y = y_;
	z = z_;
}

void Enemy::update(float delta_time) {
	x += speed * delta_time;
}

float Enemy::getX() const {
	return x;
}

float Enemy::getY() const {
	return y;
}

float Enemy::getZ() const {
	return z;
}

float Enemy::getScreenX() const {
	return screen_x;
}
