#pragma once

#define PI 3.141592653589

#ifndef MATH_H
#include <math.h>
#endif

class Enemy {
private:
	float x;
	float y;
	float z;
	
	float angle = 0.f;
	float speed = 200.f;
	int health = 1;

	float width = 100.f;
	float height = 100.f;

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

	void getSprite(float player_x, float player_y, float player_angle, float delta_angle);
	void getProjection();
	void update(float delta_time);
	float getX() const;
	float getY() const;
	float getZ() const;
	float getScreenX() const;
};
