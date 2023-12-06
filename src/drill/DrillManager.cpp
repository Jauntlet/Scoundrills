#include <Jauntlet/Time.h>
#include "DrillManager.h"
#include "../players/PlayerManager.h"
#include "src/interactable/Holdable.h"

//Constants
const float DrillManager::DISASTER_INTERVAL = 15.0f;

const float heatRiseScale = .3f; //1 heat every ~3 seconds
const float heatFallScale = .1f; //1 heat every 10 seconds

DrillManager::DrillManager(PlayerResources resourceManager, Jauntlet::Camera2D* camera)
:
	_drillAssets(camera),
	resources(resourceManager),
	navigation(camera),
	_boiler(&boilerWater, "Textures/BoilerTank.png", { 64 * 16, -64 * 1 - 10, 32 * 2, 43 * 2 }, 5, { 64 * 15.5, -64 * 2, 64 * 2, 96 * 2 }, { 16,-64 }),
	_waterTank(*this, { 64, -64 * 12, 64, 64 }, { 64, -64 * 12, 64, 64 }, glm::vec2(0)),
	_forge({64 * 16, -64 * 13, 64, 64}, {64 * 16, -64 * 13, 64, 64}, glm::vec2(0))
{
	drillFloor.loadTileMap("Levels/DrillFloor.JML");
	drillWalls.loadTileMap("Levels/DrillWall.JML");
	pipes.loadTileMap("Levels/Pipes.JML");
	
	on();

	navigation.genNav();

	addHoldable("Textures/pipeCarry.png", glm::vec2(64 * 6, -64 * 6), glm::vec2(32), HoldableType::PIPE);
}

void DrillManager::update() {
	// calculate the change in water/heat
	if (_drillOn) {
		if (boilerWater > 0) {
			boilerWater -= Jauntlet::Time::getDeltaTime() / 6;
			resources.heat += Jauntlet::Time::getDeltaTime() * (heatRiseScale + _brokenPipeLocations.size() * 0.1);
			_disasterTime -= Jauntlet::Time::getDeltaTime() * (resources.heat / 100);
			if (_disasterTime < 0) {
				DisasterEvent();
			}
			navigation.updateTravel();
		}
	}
	else {
		resources.heat -= Jauntlet::Time::getDeltaTime() * (heatFallScale - _brokenPipeLocations.size() * 0.1);
		if (resources.heat < 0) {
			resources.heat = 0;
		}
	}

	if (boilerWater > Boiler::BOILER_MAX_WATER * 0.75) {
		_boiler.animation.stop(4);
	} else if (boilerWater > Boiler::BOILER_MAX_WATER * 0.5) {
		_boiler.animation.stop(3);
	} else if (boilerWater > Boiler::BOILER_MAX_WATER * 0.25) {
		_boiler.animation.stop(2);
	} else if (boilerWater > 0) {
		_boiler.animation.stop(1);
	} else {
		boilerWater = 0;
		_boiler.animation.stop(0);
		off();
	}
}

void DrillManager::drawLayerOne() {
	_drillAssets.drawLayerOne();
	drillFloor.draw();
	drillWalls.draw();
	pipes.draw();

	_drillAssets.drawLayerTwo();

	_spriteBatch.begin();
	_boiler.draw(_spriteBatch);
	_waterTank.draw(_spriteBatch);
	_forge.draw(_spriteBatch);
	_spriteBatch.endAndRender();

	_drillAssets.drawLayerThree();
}
void DrillManager::drawLayerTwo() {
	// draw all holdable items
	_spriteBatch.begin();
	for (int i = 0; i < _holdables.size(); ++i) {
		if (_holdables[i]->isEmpty()) {
			removeHoldable(_holdables[i]);
			--i;
			continue;
		}
		_holdables[i]->draw(_spriteBatch);
	}
	_spriteBatch.endAndRender();
}

void DrillManager::on() {
	_drillOn = true;
	_drillAssets.startAnimation();
}
void DrillManager::off() {
	_drillOn = false;
	_drillAssets.stopAnimation();
}
void DrillManager::toggle() {
	if (_drillOn) {
		off();
	} else {
		on();
	}
}

bool DrillManager::isValidDestination(glm::vec2 worldPos, PlayerManager* playerManager) const {
	glm::ivec2 pos = drillWalls.WorldPosToTilePos(worldPos);
	glm::vec2 floorPos = drillFloor.WorldPosToTilePos(worldPos);
	worldPos = drillWalls.RoundWorldPos(worldPos);

	if (drillWalls.tileHasCollision(pos) || !drillWalls.isValidTilePos(pos)) {
		// The tile is in a wall, but theres a chance it is a broken pipe, so we loop through all broken pipes.
		for (int i = 0; i < _brokenPipeLocations.size(); ++i) {
			if (pipes.TilePosToWorldPos(_brokenPipeLocations[i]) == worldPos + glm::vec2(0,_brokenPipeLocations[i].y != 0 ? 64 : 0)) {
				return true;
			}
		}
		return false;
	}
	else if (drillFloor.isTileEmpty(floorPos)) {
		return false;
	}
	else if (playerManager->posMatchesPlayerDest(worldPos)) {
		return false;
	}
	else return !doesTileOverlapStations(pos);
	
	//return Pathfinding::isReachable(&drillWalls, *playerManager, _players[_selectedPlayer].getPosition(), worldPos);
}
bool DrillManager::isValidDestination(glm::vec2 worldPos) const {
	glm::ivec2 pos = drillWalls.WorldPosToTilePos(worldPos);
	glm::vec2 floorPos = drillFloor.WorldPosToTilePos(worldPos);
	worldPos = drillWalls.RoundWorldPos(worldPos);

	if (drillWalls.tileHasCollision(pos) || !drillWalls.isValidTilePos(pos)) {
		// The tile is in a wall, but theres a chance it is a broken pipe, so we loop through all broken pipes.
		for (int i = 0; i < _brokenPipeLocations.size(); ++i) {
			if (pipes.TilePosToWorldPos(_brokenPipeLocations[i]) == worldPos + glm::vec2(0, _brokenPipeLocations[i].y != 0 ? 64 : 0)) {
				return true;
			}
		}
		return false;
	}
	else if (drillFloor.isTileEmpty(floorPos)) {
		return false;
	}
	else return !doesTileOverlapStations(pos);

	//return Pathfinding::isReachable(&drillWalls, *playerManager, _players[_selectedPlayer].getPosition(), worldPos);
}
bool DrillManager::isValidPath(glm::vec2 worldPos, PlayerManager* playerManager) const {
	glm::ivec2 pos = drillWalls.WorldPosToTilePos(worldPos);
	glm::vec2 floorPos = drillFloor.WorldPosToTilePos(worldPos);
	worldPos = drillWalls.RoundWorldPos(worldPos);

	if (drillWalls.tileHasCollision(pos) || !drillWalls.isValidTilePos(pos)) {
		return false;
	}
	else if (drillFloor.isTileEmpty(floorPos)) {
		return false;
	}
	else if (playerManager->posMatchesPlayerDest(worldPos)) {
		return false;
	}
	
	// Prevent pathing through items on the floor.
	for (int i = 0; i < _holdables.size(); ++i) {	
		if (_holdables[i]->position == worldPos + glm::vec2(0,64)) {
			return false;
		}
	}

	return !doesTileOverlapStations(pos);
}

PlayerStation* DrillManager::checkHoveringStation(glm::vec2 position) {
	if (_drillAssets.steeringWheel.isColliding(position)) {
		return &_drillAssets.steeringWheel;
	}
	else if (_boiler.isColliding(position)) {
		return &_boiler;
	}
	else if (_waterTank.isColliding(position)) {
		return &_waterTank;
	}
	else if (_forge.isColliding(position)) {
		return &_forge;
	}
	else {
		return nullptr;
	}
}
bool DrillManager::doesTileOverlapStations(glm::ivec2 tilePos) const  {
	return drillWalls.doesTileOverlap(tilePos, _drillAssets.steeringWheel.getBoundingBox()) ||
		drillWalls.doesTileOverlap(tilePos, _boiler.getBoundingBox()) ||
		drillWalls.doesTileOverlap(tilePos, _waterTank.getBoundingBox()) ||
		drillWalls.doesTileOverlap(tilePos, _forge.getBoundingBox());
}

void DrillManager::bustRandomPipe() {
	// changes a random pipe of ID 1 (normal pipe) to a pipe of ID 2 (broken pipe)
	_brokenPipeLocations.push_back(pipes.selectRandomTile(1));
	pipes.UpdateTile(_brokenPipeLocations.back(), 2);
}
void DrillManager::repairPipe(const glm::vec2& worldPos) {
	for (int i = 0; i < _brokenPipeLocations.size(); ++i) {
		if (pipes.TilePosToWorldPos(_brokenPipeLocations[i]) == worldPos) {
			pipes.UpdateTile(_brokenPipeLocations[i], 1);
			_brokenPipeLocations.erase(_brokenPipeLocations.begin() + i);
			break;
		}
	}
}
bool DrillManager::DestMatchesRandomPipe(const glm::vec2& worldPos) const {
	for (int i = 0; i < _brokenPipeLocations.size(); ++i) {
			if (pipes.TilePosToWorldPos(_brokenPipeLocations[i]) == worldPos) {
				return true;
			}
	}
	return false;
}

Holdable* DrillManager::addHoldable(const std::string& texture, const glm::vec2& position, const glm::vec2& size, const HoldableType& type) {
	_holdables.emplace_back(new Holdable(texture, position, size, type));
	return _holdables[_holdables.size() - 1];
}
void DrillManager::removeHoldable(Holdable* holdable) {
	for (int i = 0; i < _holdables.size(); ++i) {
		if (_holdables[i] == holdable) {
			delete _holdables[i];
			_holdables.erase(_holdables.begin() + i);
			return;
		}
	}
}
Holdable* DrillManager::getHoldable(glm::vec2 worldPos) {
	for (int i = 0; i < _holdables.size(); ++i) {
		if (_holdables[i]->position == worldPos) {
			return _holdables[i];
		}
	}
	return nullptr;
}

void DrillManager::DisasterEvent() {
	// 3 is the amount of disasters
	int disaster = rand() % 3;
	 
	switch (disaster) {
	case 0:
		bustRandomPipe();
		break;
	case 1:
		placeIce();
		break;
	case 2:
		placeScrap();
		break;
	}
	
	_disasterTime = DISASTER_INTERVAL;
}
void DrillManager::placeIce() {
	glm::vec2 position = drillFloor.TilePosToWorldPos(drillFloor.selectRandomTile(1));

	while (!isValidDestination(position)) {
		position = drillFloor.TilePosToWorldPos(drillFloor.selectRandomTile(1));
	}

	addHoldable("Textures/Ice.png", position, glm::vec2(32), HoldableType::ICE);
}
void DrillManager::placeScrap() {
	glm::vec2 position = drillFloor.TilePosToWorldPos(drillFloor.selectRandomTile(1));

	while (!isValidDestination(position)) {
		position = drillFloor.TilePosToWorldPos(drillFloor.selectRandomTile(1));
	}

	addHoldable("Textures/Scrap.png", position, glm::vec2(32), HoldableType::SCRAP);
}