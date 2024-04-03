#include "bfs.h"
#include <iostream>

BFS::BFS(std::vector<std::vector<Tile>>& tiles_) : tiles(tiles_) {}

std::vector<Tile*> BFS::findPath(Tile* start, Tile* finish) {

	start->setParent(nullptr);

	openSet.push(start);
	closeSet.insert(start);

	while (!openSet.empty()) {

		//std::cout << openSet.size() << std::endl;

		Tile* current = openSet.front();
		openSet.pop();

		if (current == finish) {

			while (current != nullptr) {
				path.push_back(current);
				current = current->getParent();
			}
			break;
		}

		for (Tile* neighbor : current->getNeighbors()) {
			if (closeSet.find(neighbor) == closeSet.end() && !neighbor->isWall()) {
				neighbor->setParent(current);
				openSet.push(neighbor);
				closeSet.insert(neighbor);
			}
		}
	}

	std::reverse(path.begin(), path.end());
	return path;
}

void BFS::clearData() {
	openSet = std::queue<Tile*>();
	closeSet.clear();
	path.clear();
}