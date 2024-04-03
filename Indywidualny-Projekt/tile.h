#pragma once

#include <vector>

class Tile {
private:
	float x;
	float y;
	bool is_wall;
	Tile* parent = nullptr;
	std::vector<Tile*> neighbors;

public:
	Tile(float x_, float y_, bool isWall_);
	
	void addNeighbor(Tile* neighbor);
	void setParent(Tile* parent_);

	Tile* getParent();
	std::vector<Tile*> getNeighbors();
	bool isWall();

	float getX() const;
	float getY() const;
};