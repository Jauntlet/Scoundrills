/* Purpose: Visualizes paths in which players would take to get a location
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <Jauntlet/Rendering/ResourceManager.h>
#include "../pathfinding/Pathfinding.h"

class PlayerManager;

class PathRenderer
{
public:
	PathRenderer(Jauntlet::TileMap* tileMap, PlayerManager* players);

	void createPath(glm::vec2 start, glm::vec2 end);
	void clearPath();
	void drawPath();
private:
	PlayerManager* _players;
	Jauntlet::TileMap* _tilemap;
	Jauntlet::SpriteBatch _spriteBatch;
	GLuint _textureID;
};

