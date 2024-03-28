#pragma once

#include "enemy.h"

Enemy::Enemy(float x_, float y_) {
	x = x_;
	y = y_;
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
