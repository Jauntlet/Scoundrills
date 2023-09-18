#include "Pathfinding.h"
#include <Jauntlet/JMath.h>

std::vector<cell> Pathfinding::_openList;
std::vector<cell> Pathfinding::_closedList;

Pathfinding::Pathfinding() {
	// Empty
}
std::vector<glm::vec2> Pathfinding::findPath(Jauntlet::TileMap& map, glm::vec2 start, glm::vec2 destination, bool allowDiagonals /*= true*/) {	
	// translate world coords to tilemap coords.
	destination = map.WorldPosToTilePos(destination);
	start = map.WorldPosToTilePos(start);

	_openList.emplace_back(start, glm::vec2(NULL));

	if (start == destination) {
		_openList.clear();
		return { map.TilePosToWorldPos(start) };
	}

	bool foundDest = false;
	while (!_openList.empty() && !foundDest) {
		int bestNodeID = 0;
		// Search through the list of nodes for the lowest movement cost
		for (int i = 1; i < _openList.size(); i++) {
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
				if (x == 0 && y == 0) { // skip (0,0)
					continue;
				}
				if (!allowDiagonals && x != 0 && y != 0) { // skip diagonals if they aren't allowed.
					continue;
				}

				cell currentNode = cell(_closedList.back().position + glm::vec2(x, y), _closedList.back().position);

				if (currentNode.position == destination) {
					foundDest = true;
					break;
				}

				// Position has collision, and therefore is not a valid position to check for navigation.
				if (map.tileHasCollision(currentNode.position)) {
					continue;
				}

				if (!allowDiagonals) {
					// Diagonals are not possible, so the added distance is always 1
					currentNode.pathDistance = _closedList.back().pathDistance + 1;

					// Calculate the distance from the node to the goal: this is essiential for A* pathfinding.
					// We use manhattan distance when we are not allowing diagonals: The difference of X + the difference of Y
					currentNode.estimatedDistance = std::abs(currentNode.position.x - destination.x) + std::abs(currentNode.position.y - destination.y);
				}
				else {
					currentNode.pathDistance = _closedList.back().pathDistance + std::abs(JMath::Distance(currentNode.position, _closedList.back().position));

					// Calculate the distance from the node to the goal: this is essiential for A* pathfinding.
					// When diagonals are allowed, we simply use a distance formula.
					currentNode.estimatedDistance = JMath::Distance(currentNode.position, destination);
				}

				// Final score:
				currentNode.estimatedDistance += currentNode.pathDistance;

				bool isValidNode = true;
				// Loop through the open list for tiles at the same position, with a lower score. If found, we skip this successor.
				for (int i = 0; i < _openList.size(); i++) {
					if (currentNode.position == _openList[i].position && currentNode.estimatedDistance >= _openList[i].estimatedDistance) {
						isValidNode = false;
						break;
					}
				}

				if (!isValidNode) continue;

				// Loop through the closed list for tiles at the same position, with a lower score. If found, we skip this successor.
				for (int i = 0; i < _closedList.size(); i++) {
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
		for (int i = 2; i < _closedList.size(); i++) {
			if (_closedList[i].estimatedDistance - _closedList[i].pathDistance < _closedList[bestCellIndex].estimatedDistance - _closedList[bestCellIndex].pathDistance) {
				bestCellIndex = i;
			}
		}
		
		// we do the thing labelled by the big comment below here too.
		output.push_back(map.TilePosToWorldPos(_closedList[bestCellIndex].position));
		output.push_back(map.TilePosToWorldPos(_closedList[bestCellIndex].position));

		for (int i = bestCellIndex - 1; i > 0; i--) {
			if (_closedList[i].position == _closedList[bestCellIndex].prevPos) {
				output.push_back(map.TilePosToWorldPos(_closedList[i].position));
				bestCellIndex = i;
			}
		}

		// reverse the list
		for (int i = 0; i < _closedList.size(); i++) {
			output[0] = output.back();
		}

		// reset lists
		_openList.clear();
		_closedList.clear();

		return output;
	}

	cell Node = _closedList.back();

	if (!map.tileHasCollision(destination)) {
		// add destination to final pos in output
		output.push_back(map.TilePosToWorldPos(destination));
		/* I push the final destination to the output vector twice here. Why?
		* Well, for some reason the Player navigation just *deletes* the last element in the array when pathfinding. 
		* There are 0 locations where elements are removed from the vector besides when I push an element to the back of the vector
		* and delete it. Using testing I can confirm the last element remains until it is sent out of this method, and then it just *vanishes*
		* If you could locate where that happens, that would be fantastic, but I've spent a full day on this bug and I frankly am just gonna do the easy fix.
		* -xm */
		output.push_back(map.TilePosToWorldPos(destination));
	}
	else {
		// same thing as big comment above, but with the next-best tile.
		output.push_back(map.TilePosToWorldPos(_closedList.back().position));
	}

	// push the first node to the list of outputs
	output.push_back(map.TilePosToWorldPos(_closedList.back().position));

	// work backwards through the vector to find the path via previously stored position.
	for (int i = _closedList.size() - 2; i > -1; i--) {
		if (_closedList[i].position == Node.prevPos) {
			Node = _closedList[i];
		
			output.push_back(map.TilePosToWorldPos(Node.position));
		}
	}
	// reverse the list
	for (int i = 0; i < _closedList.size(); i++) {
		output[0] = output.back();
	}

	// reset lists
	_openList.clear();
	_closedList.clear();

	return output;
}