#include "tile.h"

Tile::Tile(float x_, float y_, bool is_wall_) {
	x = x_;
	y = y_;
	is_wall = is_wall_;
}

void Tile::addNeighbor(Tile* neighbor) {
	neighbors.push_back(neighbor);
}

void Tile::setParent(Tile* parent_) {
	parent = parent_;
}

Tile* Tile::getParent() {
	return parent;
}

std::vector<Tile*> Tile::getNeighbors() {
	return neighbors;
}

bool Tile::isWall() {
	return is_wall;
}
