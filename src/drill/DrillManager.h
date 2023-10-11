/* Purpose: Handles the drill and the happenings within it.
 * Main Contributer(s): Xander Mooney / Jack Kennedy
 */
#pragma once

#include <Jauntlet/TileMap.h>

#include "DrillAssetRenderer.h"

class DrillManager {
public:
	DrillManager();
	
	// initialize the drill elements
	void init();
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