/* Purpose: Handles the drill and the happenings within it.
 * Main Contributer(s): Xander Mooney / Jack Kennedy
 */

#pragma once
#include <Jauntlet/Tiles/TileMap.h>
#include <Jauntlet/Inputs/InputManager.h>
#include "DrillAssetRenderer.h"
#include "../PlayerResources.h"
#include "../Navigation.h"
#include "../interactable/Holdable.h"
#include "../interactable/specificStations/Boiler.h"

class PlayerManager;

class DrillManager {
public:
	DrillManager(PlayerResources resourceManager, Jauntlet::Camera2D* camera);
	
	// update parts of the drill
	void update();

	// draw elements that appear beneath the players
	void drawLayerOne();
	// draw elements that appear above the players
	void drawLayerTwo();

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
	void repairPipe(const glm::vec2& worldPos);
	bool DestMatchesRandomPipe(const glm::vec2& worldPos) const;

	// adds a holdable item to be managed by the drillManager.
	Holdable* addHoldable(const std::string& texture, const glm::vec2& position, const glm::vec2& size, const HoldableType& type = HoldableType::NONE);
	// removes the holdable from the drillManagers references.
	void removeHoldable(Holdable* holdable);
	// gives you a pointer to a holdable if it matches the given position
	Holdable* getHoldable(glm::vec2 worldPos);

	Jauntlet::TileMap drillWalls = Jauntlet::TileMap(_textureCache, 64);
	Jauntlet::TileMap drillFloor = Jauntlet::TileMap(_textureCache, 64);
	Jauntlet::TileMap pipes = Jauntlet::TileMap(_textureCache, 64);

	float boilerWater = 60.0f;
	Navigation navigation;
	PlayerResources resources;
private:
	DrillAssetRenderer _drillAssets;
	Boiler _boiler;

	Jauntlet::TextureCache _textureCache;

	bool _drillOn = true;


	std::vector<Holdable*> _holdables;
	Jauntlet::SpriteBatch _spriteBatch;

	std::vector<glm::ivec2> _brokenPipeLocations;
};