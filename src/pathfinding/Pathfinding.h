/* Purpose: Uses A* Pathfinding algorithms to find the shortest path between two points on a tilemap
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <Jauntlet/Tiles/TileMap.h>

class DrillManager;
class PlayerManager;

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
	static std::vector<glm::vec2> findPath(TileMap* map, glm::vec2 start, glm::vec2 destination);
	// Finds the path between a starting and end location in world space, using both a tilemap and information given by the player manager to give much more accurate results at a performance cost.
	static std::vector<glm::vec2> findPath(const DrillManager& drill, PlayerManager& players, glm::vec2 start, glm::vec2 destination);

	static bool isReachable(const DrillManager& drill, PlayerManager& players, glm::vec2 start, glm::vec2 destination);
private:

	static std::vector<cell> _openList;
	static std::vector<cell> _closedList;
};

