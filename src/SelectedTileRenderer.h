/* Purpose: Renders the tile the mouse is hovered over
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include <Jauntlet/Rendering/ResourceManager.h>
#include <Jauntlet/TileMap.h>
#include <Jauntlet/Camera2D.h>
#include <Jauntlet/InputManager.h>

class SelectedTileRenderer
{
public:
	SelectedTileRenderer();
	// initialize Selected Tile Renderer.
	void init(Jauntlet::TileMap* Tilemap);

	// draw the selected tile to screen
	void draw(Jauntlet::Camera2D* activeCamera, Jauntlet::InputManager* inputManager);
private:
	GLuint _textureID;

	Jauntlet::SpriteBatch _spriteBatch;
	Jauntlet::Color _drawColor;

	Jauntlet::TileMap* _tilemap;
};

