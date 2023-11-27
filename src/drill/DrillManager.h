/* Purpose: Handles the drill and the happenings within it.
 * Main Contributer(s): Xander Mooney / Jack Kennedy
 */

#pragma once
#include <Jauntlet/Tiles/TileMap.h>
#include <Jauntlet/Inputs/InputManager.h>
#include "DrillAssetRenderer.h"
#include "../PlayerResources.h"
#include "../Navigation.h"
#include "src/interactable/Holdable.h"

class PlayerManager;

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

	bool isValidDestination(glm::vec2 worldPos, PlayerManager* playerManager) const;
	bool isValidPath(glm::vec2 worldPos, PlayerManager* playerManager) const;

	PlayerStation* checkHoveringStation(glm::vec2 position);
	bool doesTileOverlapStations(glm::ivec2 tilePos) const;

	void bustRandomPipe();

	// adds a holdable item to be managed by the drillManager.
	// DrillManager will handle pathfinding, as well as drawing the elements.
	void addHoldable(Holdable* holdable);
	// removes the holdable from the drillManagers references.
	void removeHoldable(Holdable* holdable);

	Jauntlet::TileMap drillWalls = Jauntlet::TileMap(_textureCache, 64);
	Jauntlet::TileMap drillFloor = Jauntlet::TileMap(_textureCache, 64);
	Jauntlet::TileMap pipes = Jauntlet::TileMap(_textureCache, 64);

	float boilerWater = 60.0f;
private:
	DrillAssetRenderer _drillAssets;

	Jauntlet::TextureCache _textureCache;

	bool _drillOn = true;

	PlayerResources _resources;
	Navigation _navigation;

	std::vector<Holdable*> _holdables;
	Jauntlet::SpriteBatch _spriteBatch;
};