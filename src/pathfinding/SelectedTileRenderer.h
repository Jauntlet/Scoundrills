/* Purpose: Renders the tile the mouse is hovered over
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <Jauntlet/Rendering/Textures/ResourceManager.h>
#include <Jauntlet/Tiles/TileMap.h>
#include <Jauntlet/Rendering/Cameras/Camera2D.h>
#include <Jauntlet/Inputs/InputManager.h>
#include "../players/PlayerManager.h"

class SelectedTileRenderer
{
public:
	SelectedTileRenderer(DrillManager* drill, PlayerManager* playerManager);

	// draw the selected tile to screen
	void draw(Camera2D* activeCamera);
private:
	bool changedPosition(Camera2D* activeCamera);

	GLuint _textureID;

	SpriteBatch _spriteBatch;
	Color _drawColor;

	PlayerManager* _players;
	DrillManager* _drill;

	glm::vec2 _lastPosition = glm::vec2(0);
};

