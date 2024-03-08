#pragma once

class Player {
private:
	float x = 0;
	float y = 0;
	float angle = 0;

public:
	Player();
	void setPosition(float x_, float y_);
	void moveX(float offset);
	void moveY(float offset);

	float getX() const;
	float getY() const;
	float getAngle() const;
};