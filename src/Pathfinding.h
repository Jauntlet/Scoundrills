#pragma once
#include <Jauntlet/TileMap.h>
#include <glm/glm.hpp>
#include <vector>

// Used to find pathfind routes.
// DO NOT USE otherwise.
struct cell {
	glm::vec2 position;
	glm::vec2 prevPos;
	float pathDistance;
	float estimatedDistance;

	cell(glm::vec2 pos, glm::vec2 lastPos) {
		position = pos;
		prevPos = lastPos;
		pathDistance = -1;
		estimatedDistance = -1;
	}
};

class Pathfinding
{
public:
	Pathfinding();
	// Finds the path between a starting and end location in world space, using a tilemap as reference.
	static std::vector<glm::vec2> findPath(Jauntlet::TileMap& map, glm::vec2 start, glm::vec2 destination, bool allowDiagonals = true);

private:

	static std::vector<cell> _openList;
	static std::vector<cell> _closedList;
};

