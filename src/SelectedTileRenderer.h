/* Purpose: Renders the tile the mouse is hovered over
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <Jauntlet/Rendering/ResourceManager.h>
#include <Jauntlet/Tiles/TileMap.h>
#include <Jauntlet/Rendering/Cameras/Camera2D.h>
#include <Jauntlet/Inputs/InputManager.h>
#include "players/PlayerManager.h"

class SelectedTileRenderer
{
public:
	SelectedTileRenderer(Jauntlet::TileMap* Tilemap, PlayerManager* playerManager);

	// draw the selected tile to screen
	void draw(Jauntlet::Camera2D* activeCamera, Jauntlet::InputManager* inputManager);
private:
	GLuint _textureID;

	Jauntlet::SpriteBatch _spriteBatch;
	Jauntlet::Color _drawColor;

	Jauntlet::TileMap* _tilemap;
	PlayerManager* _players;

	glm::vec2 _lastPosition = glm::vec2(0);
};

