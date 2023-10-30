/* Purpose: Uses A* Pathfinding algorithms to find the shortest path between two points on a tilemap
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <Jauntlet/Tiles/TileMap.h>

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
	static std::vector<glm::vec2> findPath(Jauntlet::TileMap* map, glm::vec2 start, glm::vec2 destination);
	// Finds the path between a starting and end location in world space, using a tilemap as reference.
	// reachedDestination will return true or false depending on if the final destination is reachable.
	static std::vector<glm::vec2> findPath(Jauntlet::TileMap* map, glm::vec2 start, glm::vec2 destination, bool& reachedDestination);

private:

	static std::vector<cell> _openList;
	static std::vector<cell> _closedList;
};

