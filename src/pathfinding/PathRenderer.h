/* Purpose: Visualizes paths in which players would take to get a location
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include "../drill/DrillManager.h"
#include "Pathfinding.h"

class PathRenderer
{
public:
	PathRenderer(DrillManager* drill, PlayerManager* playerManager);

	void createPath(glm::vec2 start, glm::vec2 end);
	void clearPath();
	void drawPath();

	std::vector<glm::vec2> getPath();
private:
	std::vector<glm::vec2> _path;
	DrillManager* _drill;
	PlayerManager* _players;

	Jauntlet::SpriteBatch _spriteBatch;
	GLuint _textureID;
};

