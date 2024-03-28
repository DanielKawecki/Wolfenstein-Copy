#pragma once

class Enemy {
private:
	float x;
	float y;
	float z;
	float angle = 0.f;
	float speed = 200.f;
	int health = 1;
public:
	Enemy(float x_, float y_, float z_);

	void update(float delta_time);
	float getX() const;
	float getY() const;
	float getZ() const;
};
