#pragma once

#include "enemy.h"
#include <iostream>

Enemy::Enemy(float x_, float y_, float z_) {
	x = x_;
	y = y_;
	z = z_;

	clock = Clock();
	clock.start();

	animation_clock = Clock();
	animation_clock.start();

	flash_clock = Clock();
	flash_clock.start();
}

void Enemy::update(float delta_time) {
	if (agroed && !stationary) {
		x += speed * cos(angle) * delta_time;
		y += speed * sin(angle) * delta_time;
	}
	if (!stationary && animation_clock.getElapsedTime() >= animation_time) {
		current_run_frame = (current_run_frame + 1) % run_frames;
		animation_clock.restart();
	}

	if (health <= 0) {
		alive = false;
		dying = true;
		animation_clock.restart();
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
	distance = hypotf(x - player_x, y - player_y);

	//std::cout << vision_on_player << std::endl;
	if (distance < agro_distance || vision_on_player) {
		agroed = true;
	}
		
	//std::cout << agroed << std::endl;
}

bool Enemy::shoot() {
	if (shooting && clock.getElapsedTime() > 1) {
		stationary = false;
		shooting = false;
		succesful_shot = true;
		flash_clock.restart();
		return true;
	}
	
	else if (agroed && !shooting && distance < agro_distance && vision_on_player) {
		stationary = true;
		shooting = true;
		clock.restart();
	}
	else if (agroed && shooting && distance > agro_distance && !vision_on_player) {
		shooting = false;
		stationary = false;
	}

	if (flash_clock.getElapsedTime() >= flash_time)
		succesful_shot = false;

	return false;
}

void Enemy::setVision(bool vision_) {
	vision_on_player = vision_;
}

bool Enemy::getVision() const {
	return vision_on_player;
}

bool Enemy::getStationary() const {
	return stationary;
}

bool Enemy::getShooting() const {
	return shooting;
}

bool Enemy::getSuccesfulShot() {
	if (succesful_shot && flash_clock.getElapsedTime() < flash_time) {
		return true;
	}
	return false;
}

bool Enemy::isAlive() const {
	return alive;
}

int Enemy::getTextureRunning() const {
	return current_run_frame;
}

int Enemy::getTextureDeath() const {
	return current_death_frame;
}

void Enemy::setScreenX(float screen_x_) {
	screen_x = screen_x_;
}

void Enemy::subtractHealth() {
	health -= 1;
}

void Enemy::die() {
	//printf("%d\n", current_death_frame);
	if (dying && animation_clock.getElapsedTime() >= animation_time) {
		current_death_frame += 1;
		animation_clock.restart();
		if (current_death_frame == death_frames - 1)
			dying = false;
	}
}

bool Enemy::isDying() const {
	return dying;
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
