#include <Jauntlet/JMath.h>
#include "Pathfinding.h"
#include "../players/PlayerManager.h"

std::vector<cell> Pathfinding::_openList;
std::vector<cell> Pathfinding::_closedList;
const int TIMEOUT_LIMIT = 100;


Pathfinding::Pathfinding() {
	// Empty
}
std::vector<glm::vec2> Pathfinding::findPath(Jauntlet::TileMap* map, glm::vec2 start, glm::vec2 destination) {	
	// translate world coords to tilemap coords.
	destination = map->WorldPosToTilePos(destination);
	start = map->WorldPosToTilePos(start);

	_openList.emplace_back(start, glm::vec2());

	if (start == destination) {
		_openList.clear();
		return { map->TilePosToWorldPos(start) };
	}

	bool foundDest = false;
	while (!foundDest && !_openList.empty() && _openList.size() < TIMEOUT_LIMIT) {
		int bestNodeID = 0;
		// Search through the list of nodes for the lowest movement cost
		for (int i = 1; i < _openList.size(); ++i) {
			if (_openList[i].estimatedDistance < _openList[bestNodeID].estimatedDistance) {
				bestNodeID = i;
			}
		}
		// cache best node
		_closedList.push_back(_openList[bestNodeID]);

		// pop best node off the list
		_openList[bestNodeID] = _openList.back();
		_openList.pop_back();

		// Loop through all successors to the bestNode
		for (int y = -1; y < 2; y++) {
			for (int x = -1; x < 2; x++) {
				if ((x != 0 && y != 0) || (x == 0 && y == 0)) { // skip (0,0) and diagonals
					continue;
				}

				cell currentNode = cell(_closedList.back().position + glm::vec2(x, y), _closedList.back().position);

				if (currentNode.position == destination) {
					foundDest = true;
					break;
				}

				// Position has collision, and therefore is not a valid position to check for navigation.
				if (map->tileHasCollision(currentNode.position)) {
					continue;
				}

				// Diagonals are not possible, so the added distance is always 1
				currentNode.pathDistance = _closedList.back().pathDistance + 1;

				// Calculate the distance from the node to the goal: this is essiential for A* pathfinding.
				// We use manhattan distance when we are not allowing diagonals: The difference of X + the difference of Y
				currentNode.estimatedDistance = std::abs(currentNode.position.x - destination.x) + std::abs(currentNode.position.y - destination.y);

				// Final score:
				currentNode.estimatedDistance += currentNode.pathDistance;

				bool isValidNode = true;
				// Loop through the open list for tiles at the same position, with a lower score. If found, we skip this successor.
				for (int i = 0; i < _openList.size(); ++i) {
					if (currentNode.position == _openList[i].position && currentNode.estimatedDistance >= _openList[i].estimatedDistance) {
						isValidNode = false;
						break;
					}
				}

				if (!isValidNode) continue;

				// Loop through the closed list for tiles at the same position, with a lower score. If found, we skip this successor.
				for (int i = 0; i < _closedList.size(); ++i) {
					if (currentNode.position == _closedList[i].position && currentNode.estimatedDistance >= _closedList[i].estimatedDistance) {
						isValidNode = false;
						break;
					}
				}

				if (!isValidNode) continue;
				
				_openList.push_back(currentNode);
			}
			if (foundDest) break;
		}
	}
	std::vector<glm::vec2> output;

	if (!foundDest) {
		// Every possible tile has been checked and the destination was not found.
		// Now we try to find the closest tile to the location and pathfind to there.
		int bestCellIndex = 1; // 1 to avoid null value
		for (int i = 2; i < _closedList.size(); ++i) {
			if (_closedList[i].estimatedDistance - _closedList[i].pathDistance < _closedList[bestCellIndex].estimatedDistance - _closedList[bestCellIndex].pathDistance) {
				bestCellIndex = i;
			}
		}
		
		output.push_back(map->TilePosToWorldPos(_closedList[bestCellIndex].position));

		for (int i = bestCellIndex - 1; i > 0; i--) {
			if (_closedList[i].position == _closedList[bestCellIndex].prevPos) {
				output.push_back(map->TilePosToWorldPos(_closedList[i].position));
				bestCellIndex = i;
			}
		}

		// reset lists
		_openList.clear();
		_closedList.clear();

		return output;
	}

	cell Node = _closedList.back();

	if (!map->tileHasCollision(destination)) {
		// add destination to final pos in output
		output.push_back(map->TilePosToWorldPos(destination));
	}
	else {
		// destination is blocked, so we output the next-best-tile as final position.
		output.push_back(map->TilePosToWorldPos(_closedList.back().position));
	}

	// push the first node to the list of outputs
	output.push_back(map->TilePosToWorldPos(_closedList.back().position));

	// work backwards through the vector to find the path via previously stored position.
	for (int i = _closedList.size() - 2; i > -1; i--) {
		if (_closedList[i].position == Node.prevPos) {
			Node = _closedList[i];
		
			output.push_back(map->TilePosToWorldPos(Node.position));
		}
	}

	// reset lists
	_openList.clear();
	_closedList.clear();

	return output;
}
std::vector<glm::vec2> Pathfinding::findPath(const DrillManager& drill, PlayerManager& players, glm::vec2 start, glm::vec2 destination) {
	// translate world coords to tilemap coords.
	destination = drill.drillWalls.WorldPosToTilePos(destination);
	start = drill.drillWalls.WorldPosToTilePos(start);

	_openList.emplace_back(start, glm::vec2());

	if (start == destination) {
		_openList.clear();
		return { drill.drillWalls.TilePosToWorldPos(start) };
	}

	bool foundDest = false;
	while (!foundDest && !_openList.empty() && _openList.size() < TIMEOUT_LIMIT) {
		int bestNodeID = 0;
		// Search through the list of nodes for the lowest movement cost
		for (int i = 1; i < _openList.size(); ++i) {
			if (_openList[i].estimatedDistance < _openList[bestNodeID].estimatedDistance) {
				bestNodeID = i;
			}
		}
		// cache best node
		_closedList.push_back(_openList[bestNodeID]);

		// pop best node off the list
		_openList[bestNodeID] = _openList.back();
		_openList.pop_back();

		// Loop through all successors to the bestNode
		for (int y = -1; y < 2; y++) {
			for (int x = -1; x < 2; x++) {
				if ((x != 0 && y != 0) || (x == 0 && y == 0)) { // skip (0,0) and diagonals
					continue;
				}

				cell currentNode = cell(_closedList.back().position + glm::vec2(x, y), _closedList.back().position);

				if (currentNode.position == destination) {
					foundDest = true;
					break;
				}
				
				if (!drill.isValidPath(drill.drillWalls.TilePosToWorldPos(currentNode.position), &players)) {
					continue;
				}

				// Diagonals are not possible, so the added distance is always 1
				currentNode.pathDistance = _closedList.back().pathDistance + 1;

				// Calculate the distance from the node to the goal: this is essiential for A* pathfinding.
				// We use manhattan distance when we are not allowing diagonals: The difference of X + the difference of Y
				currentNode.estimatedDistance = std::abs(currentNode.position.x - destination.x) + std::abs(currentNode.position.y - destination.y) + currentNode.pathDistance;

				bool isValidNode = true;
				// Loop through the open list for tiles at the same position, with a lower score. If found, we skip this successor.
				for (int i = 0; i < _openList.size(); ++i) {
					if (currentNode.position == _openList[i].position && currentNode.estimatedDistance >= _openList[i].estimatedDistance) {
						isValidNode = false;
						break;
					}
				}
				if (!isValidNode) continue;

				// Loop through the closed list for tiles at the same position, with a lower score. If found, we skip this successor.
				for (int i = 0; i < _closedList.size(); ++i) {
					if (currentNode.position == _closedList[i].position && currentNode.estimatedDistance >= _closedList[i].estimatedDistance) {
						isValidNode = false;
						break;
					}
				}
				if (!isValidNode) continue;

				_openList.push_back(currentNode);
			}
			if (foundDest) break;
		}
	}
	std::vector<glm::vec2> output;

	if (!foundDest) {
		// Every possible tile has been checked and the destination was not found.
		// Now we try to find the closest tile to the location and pathfind to there.
		int bestCellIndex = 1; // 1 to avoid null value
		for (int i = 2; i < _closedList.size(); ++i) {
			if (_closedList[i].estimatedDistance - _closedList[i].pathDistance < _closedList[bestCellIndex].estimatedDistance - _closedList[bestCellIndex].pathDistance) {
				bestCellIndex = i;
			}
		}

		output.push_back(drill.drillWalls.TilePosToWorldPos(_closedList[bestCellIndex].position));

		for (int i = bestCellIndex - 1; i > 0; i--) {
			if (_closedList[i].position == _closedList[bestCellIndex].prevPos) {
				output.push_back(drill.drillWalls.TilePosToWorldPos(_closedList[i].position));
				bestCellIndex = i;
			}
		}

		// reset lists
		_openList.clear();
		_closedList.clear();

		return output;
	}

	cell Node = _closedList.back();

	if (!drill.drillWalls.tileHasCollision(destination)) {
		// add destination to final pos in output
		output.push_back(drill.drillWalls.TilePosToWorldPos(destination));
	}
	else {
		// destination is blocked, so we output the next-best-tile as final position.
		output.push_back(drill.drillWalls.TilePosToWorldPos(_closedList.back().position));
	}

	// push the first node to the list of outputs
	output.push_back(drill.drillWalls.TilePosToWorldPos(_closedList.back().position));

	// work backwards through the vector to find the path via previously stored position.
	for (int i = _closedList.size() - 2; i > -1; i--) {
		if (_closedList[i].position == Node.prevPos) {
			Node = _closedList[i];

			output.push_back(drill.drillWalls.TilePosToWorldPos(Node.position));
		}
	}

	// reset lists
	_openList.clear();
	_closedList.clear();

	return output;
}

bool Pathfinding::isReachable(const DrillManager& drill, PlayerManager& players, glm::vec2 start, glm::vec2 destination) {
	// translate world coords to tilemap coords.
	destination = drill.drillWalls.WorldPosToTilePos(destination);
	start = drill.drillWalls.WorldPosToTilePos(start);

	_openList.emplace_back(start, glm::vec2());

	if (start == destination) {
		_openList.clear();
		return true;
	}

	while (!_openList.empty() && _openList.size() < TIMEOUT_LIMIT) {
		int bestNodeID = 0;
		// Search through the list of nodes for the lowest movement cost
		for (int i = 1; i < _openList.size(); ++i) {
			if (_openList[i].estimatedDistance < _openList[bestNodeID].estimatedDistance) {
				bestNodeID = i;
			}
		}
		// cache best node
		_closedList.push_back(_openList[bestNodeID]);

		// pop best node off the list
		_openList[bestNodeID] = _openList.back();
		_openList.pop_back();

		// Loop through all successors to the bestNode
		for (int y = -1; y < 2; y++) {
			for (int x = -1; x < 2; x++) {
				if ((x != 0 && y != 0) || (x == 0 && y == 0)) { // skip (0,0) and diagonals
					continue;
				}

				cell currentNode = cell(_closedList.back().position + glm::vec2(x, y), _closedList.back().position);

				if (currentNode.position == destination) {
					_openList.clear();
					_closedList.clear();
					return true;
				}

				if (!drill.isValidPath(drill.drillWalls.TilePosToWorldPos(currentNode.position), &players)) {
					continue;
				}

				// Diagonals are not possible, so the added distance is always 1
				currentNode.pathDistance = _closedList.back().pathDistance + 1;

				// Calculate the distance from the node to the goal: this is essiential for A* pathfinding.
				// We use manhattan distance when we are not allowing diagonals: The difference of X + the difference of Y
				currentNode.estimatedDistance = std::abs(currentNode.position.x - destination.x) + std::abs(currentNode.position.y - destination.y) + currentNode.pathDistance;

				bool isValidNode = true;
				// Loop through the open list for tiles at the same position, with a lower score. If found, we skip this successor.
				for (int i = 0; i < _openList.size(); ++i) {
					if (currentNode.position == _openList[i].position && currentNode.estimatedDistance >= _openList[i].estimatedDistance) {
						isValidNode = false;
						break;
					}
				}
				if (!isValidNode) continue;

				// Loop through the closed list for tiles at the same position, with a lower score. If found, we skip this successor.
				for (int i = 0; i < _closedList.size(); ++i) {
					if (currentNode.position == _closedList[i].position && currentNode.estimatedDistance >= _closedList[i].estimatedDistance) {
						isValidNode = false;
						break;
					}
				}
				if (!isValidNode) continue;

				_openList.push_back(currentNode);
			}
		}
	}
	_openList.clear();
	_closedList.clear();
	return false;
}