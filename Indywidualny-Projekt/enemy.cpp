#pragma once

#include "enemy.h"
#include <iostream>

Enemy::Enemy(float x_, float y_, float z_) {
	x = x_;
	y = y_;
	z = z_;
}

void Enemy::update(float delta_time) {
	if (agroed) {
		x += speed * cos(angle) * delta_time;
		y += speed * sin(angle) * delta_time;
	}
}

void Enemy::seekPlayer(Tile* start_, Tile* finish_, std::vector<std::vector<Tile>>& tiles_) {
	if (start_ != start || finish_ != finish) {
		start = start_;
		finish = finish_;

		BFS bfs(tiles_);
		path = bfs.findPath(start, finish);

		if (path.size() >= 2)
			angle = atan2((path[1]->getY() + 32 - y), (path[1]->getX() + 32 - x));

		//std::cout << finish_->getX() / 64 << " " << finish_->getY() / 64 << std::endl;

		/*for (auto tile : path) {
			std::cout << tile->getX() / 64 << " " << tile->getY() / 64 << std::endl;
		}*/
		
	}
}

void Enemy::checkAgro(float player_x, float player_y) {
	float distance = hypotf(x - player_x, y - player_y);

	if (distance < agro_distance)
		agroed = true;
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
