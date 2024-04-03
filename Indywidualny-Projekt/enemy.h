#pragma once

#define PI 3.141592653589

#ifndef MATH_H
#include <math.h>
#endif

#include "bfs.h"

class Enemy {
private:
	float x;
	float y;
	float z;
	
	float angle = 0.f;
	float speed = 50.f;
	int health = 1;

	float width = 100.f;
	float height = 100.f;

	// Path-finding related
	Tile* start = nullptr;
	Tile* finish = nullptr;
	std::vector<Tile*> path;

	// Sprite related
	float delta_x = 0.f;
	float delta_y = 0.f;
	float delta = 0.f;
	float delta_rays = 0.f;
	float theta = 0.f;

	float screen_x = 0.f;
	float screen_y = 0.f;
	float scale = 1.f;

	float distance = 0.f;
	float normalized_distance = 0.f;


public:
	Enemy(float x_, float y_, float z_);

	void update(float delta_time);
	void seekPlayer(Tile* start_, Tile* finish_, std::vector<std::vector<Tile>>& tiles_);
	float getX() const;
	float getY() const;
	float getZ() const;
	float getScreenX() const;
};
