#ifndef GLFW_INCLUDED
#define GLFW_INCLUDED

#include <GLFW/glfw3.h>
#endif 


#pragma once

class Player {
private:
	float x = 0;
	float y = 0;
	float angle = 0;
	float speed = 200;

public:
	Player();
	void setPosition(float x_, float y_);
	void handleInput(GLFWwindow* window, float delta_time);
	void moveX(float offset);
	void moveY(float offset);

	float getX() const;
	float getY() const;
	float getAngle() const;
};