#pragma once

#include "enemy.h"

Enemy::Enemy(float x_, float y_, float z_) {
	x = x_;
	y = y_;
	z = z_;
}

void Enemy::getSprite(float player_x, float player_y, float player_angle, float delta_angle) {
	delta_x = x - player_x;
	delta_y = y - player_y;
	theta = atan2f(delta_y, delta_x);

	delta = theta - player_angle;
	if ((delta_x > 0 && player_angle > PI) || (delta_x < 0 && delta_y < 0))
		delta += 2 * PI;

	delta_rays = delta / delta_angle;
	screen_x = (320 + delta_rays) * scale; // 320 is the half of the number of rays

	distance = hypotf(delta_x, delta_y);
	normalized_distance = distance * cos(delta);

	if (-width / 2.f < screen_x < (1280 + width / 2.f) && normalized_distance > 32.f)
		getProjection();
}

void Enemy::getProjection() {

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
