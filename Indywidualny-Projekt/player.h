#ifndef GLFW_INCLUDED
#define GLFW_INCLUDED

#include <GLFW/glfw3.h>
#endif 

#pragma once

#include <math.h>
#define PI 3.141592653589
#define Degree 0.0174532925 // radians

class Player {
private:
	float x = 0;
	float y = 0;
	float angle = 0;
	float speed = 150;
	float rotation_speed = 4; // radians

public:
	Player();
	void setPosition(float x_, float y_);
	void handleInput(GLFWwindow* window, float delta_time);
	void movePosition(float dx, float dy);

	float getX() const;
	float getY() const;
	float getAngle() const;
};