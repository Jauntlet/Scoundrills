/* Purpose: Handles the drill and the happenings within it.
 * Main Contributer(s): Xander Mooney / Jack Kennedy
 */

#pragma once

#include <Jauntlet/Tiles/TileMap.h>
#include <Jauntlet/Inputs/InputManager.h>

#include "DrillAssetRenderer.h"
#include "../PlayerResources.h"
#include "../Navigation.h"

class DrillManager {
public:
	DrillManager(PlayerResources resourceManager, Jauntlet::Camera2D* camera);
	
	// update parts of the drill
	void update();

	// draw the drill elements
	void draw();

	// turn on the drill
	void on();
	// turn off the drill
	void off();
	// toggle the drill
	void toggle();

	PlayerStation* checkHoveringStation(glm::vec2 position);
	bool doesTileOverlapStations(glm::ivec2 tilePos);

	void bustRandomPipe();

	Jauntlet::TileMap drillWalls;
	Jauntlet::TileMap drillFloor;
	Jauntlet::TileMap pipes;

	float boilerWater = 60.0f;
private:
	DrillAssetRenderer _drillAssets;

	Jauntlet::TextureCache _textureCache;

	bool _drillOn = true;

	PlayerResources _resources;
	Navigation _navigation;
};