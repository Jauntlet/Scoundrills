#pragma once
#include <vector>
#include <Jauntlet/Rendering/ResourceManager.h>
#include "../Pathfinding.h"
class PathRenderer
{
public:
	PathRenderer();

	void init(Jauntlet::TileMap* tileMap);
	void createPath(glm::vec2 start, glm::vec2 end);
	void drawPath();
private:
	Jauntlet::TileMap* _tilemap;
	Jauntlet::SpriteBatch _spriteBatch;
	GLuint _textureID;
};

