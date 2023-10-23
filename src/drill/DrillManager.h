/* Purpose: Handles the drill and the happenings within it.
 * Main Contributer(s): Xander Mooney / Jack Kennedy
 */

#pragma once

#include <Jauntlet/TileMap.h>
#include <Jauntlet/InputManager.h>

#include "DrillAssetRenderer.h"
#include "../players/PlayerManager.h"

class DrillManager {
public:
	DrillManager();
	
	// process inputs to the drill
	void processInput(Jauntlet::InputManager* inputManager, PlayerManager* playerManager, glm::vec2 mousePosAsWorld);
	// draw the drill elements
	void draw();

	// turn on the drill
	void on();
	// turn off the drill
	void off();

	// toggle the drill
	void toggle();

	Jauntlet::TileMap drillWalls;
	Jauntlet::TileMap drillFloor;
private:
	DrillAssetRenderer _drillAssets;

	Jauntlet::TextureCache _textureCache;

	bool _drillOn = true;
};