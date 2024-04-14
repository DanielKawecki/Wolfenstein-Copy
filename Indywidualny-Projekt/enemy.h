#pragma once

#define PI 3.141592653589

#ifndef MATH_H
#include <math.h>
#endif

#include "bfs.h"
#include "clock.h"

class Enemy {
private:
	float x;
	float y;
	float z;

	float width = 100.f;
	float height = 100.f;

	// Enemy behaviour related
	bool stationary = true;
	bool agroed = false;
	bool shooting = false;
	bool vision_on_player = false;
	float agro_distance = 500.f;

	float angle = 0.f;
	float speed = 120.f;
	int health = 1;

	Clock clock;

	// Path-finding related
	Tile* start = nullptr;
	Tile* finish = nullptr;
	std::vector<Tile*> path;

	// Sprite related
	float screen_x = 0.f;
	float screen_y = 0.f;
	float scale = 1.f;

	float distance = 0.f;
	//float normalized_distance = 0.f;


public:
	Enemy(float x_, float y_, float z_);

	void update(float delta_time);
	void seekPlayer(Tile* start_, Tile* finish_, std::vector<std::vector<Tile>>& tiles_);
	void checkAgro(float player_x, float player_y);
	void setVision(bool vision_);
	bool shoot();
	float getX() const;
	float getY() const;
	float getZ() const;
	float getScreenX() const;
	bool getVision() const;
};
