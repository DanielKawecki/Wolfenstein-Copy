#ifndef GLFW_INCLUDED
#define GLFW_INCLUDED

#include <GLFW/glfw3.h>
#endif 

#pragma once

#include <iostream>
#include <vector>
#include <math.h>
#define PI 3.141592653589
#define Degree 0.0174532925 // radians

struct Rect {
	float left, top;
	float right, bottom;
};

class Player {
private:
	float x = 0;
	float y = 0;
	float angle = 0;
	float speed = 200;
	float rotation_speed = 4; // radians

	std::vector<std::string> map_layout;
	std::string wall_chars;
	int tile_x = 0;
	int tile_y = 0;

	int health = 100;
	int health_cap = health;
	int ammo = 24;
	int ammo_cap = ammo;

	Rect rect;
	float size = 18;
	float top = 0;
	float bottom = 0;
	float left = 0;
	float right = 0;

public:
	Player();
	void setPosition(float x_, float y_);
	void handleInput(GLFWwindow* window, float delta_time);
	void movePosition(float dx, float dy);
	void setTilePosition(int tile_size);
	bool checkWalls(float dx, float dy);
	void setRect(float x_, float y_);
	void setWalls(std::vector<std::string> map_layout_, std::string wall_chars_);
	bool intersects(const Rect& rect1, const Rect& rect2);
	bool stringContains(const std::string& string, char ch);
	void substractHealth();
	void addHealth();
	void addAmmo();

	float getX() const;
	float getY() const;
	int getTileX() const;
	int getTileY() const;
	float getAngle() const;
	Rect getRect();
	bool isDead();
};