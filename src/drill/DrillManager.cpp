#include <Jauntlet/Time.h>
#include "DrillManager.h"
#include "../players/PlayerManager.h"
#include "src/interactable/Holdable.h"

//Constants
const float heatRiseScale = .3f; //1 heat every ~3 seconds
const float heatFallScale = .1f; //1 heat every 10 seconds

DrillManager::DrillManager(PlayerResources resourceManager, Jauntlet::Camera2D* camera)
:
	_drillAssets(camera),
	_resources(resourceManager),
	_navigation(camera),
	_boiler(&boilerWater, "Textures/BoilerTank.png", { 64 * 16, -64 * 1 - 10, 32 * 2, 43 * 2 }, 5, { 64 * 15.5, -64 * 2, 64 * 2, 96 * 2 }, { 16,-64 })
{
	drillFloor.loadTileMap("Levels/DrillFloor.JML");
	drillWalls.loadTileMap("Levels/DrillWall.JML");
	pipes.loadTileMap("Levels/Pipes.JML");
	
	on();

	// DEBUGGING CODE
	bustRandomPipe();
	bustRandomPipe();
	bustRandomPipe();
	addHoldable("Textures/missing.png", glm::vec2(64 * 7, -64 * 6), glm::vec2(32), HoldableType::WATER);
	addHoldable("Textures/pipeCarry.png", glm::vec2(64 * 6, -64 * 6), glm::vec2(32));
}

void DrillManager::update() {
	// calculate the change in water/heat
	if (_drillOn) {
		if (boilerWater > 0) {
			boilerWater -= Jauntlet::Time::getDeltaTime();
			_resources.heat += Jauntlet::Time::getDeltaTime() * heatRiseScale;
			_navigation.updateTravel();

			if (boilerWater > 45) {
				_boiler.animation.stop(4);
			} else if (boilerWater > 30) {
				_boiler.animation.stop(3);
			} else if (boilerWater > 15) {
				_boiler.animation.stop(2);
			} else if (boilerWater > 0) {
				_boiler.animation.stop(1);
			} else {
				boilerWater = 0;
				_boiler.animation.stop(0);
				off();
			}
		}
	}
	else {
		_resources.heat -= Jauntlet::Time::getDeltaTime() * heatFallScale;
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
	_spriteBatch.endAndRender();
	
}
void DrillManager::drawLayerTwo() {
	// draw all holdable items
	_spriteBatch.begin();
	for (int i = 0; i < _holdables.size(); ++i) {
		if (_holdables[i].isEmpty()) {
			removeHoldable(&_holdables[i]);
			--i;
			continue;
		}
		_holdables[i].draw(_spriteBatch);
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
		if (!_holdables[i].isHeld() && worldPos == _holdables[i].position) {
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
	else {
		return nullptr;
	}
}
bool DrillManager::doesTileOverlapStations(glm::ivec2 tilePos) const  {
	return drillWalls.doesTileOverlap(tilePos, _drillAssets.steeringWheel.getBoundingBox()) ||
		drillWalls.doesTileOverlap(tilePos, _boiler.getBoundingBox());
}

void DrillManager::bustRandomPipe() {
	// changes a random pipe of ID 1 (normal pipe) to a pipe of ID 2 (broken pipe)
	pipes.UpdateTile(pipes.selectRandomTile(1), 2);
}

Holdable* DrillManager::addHoldable(const std::string& texture, const glm::vec2& position, const glm::vec2& size, const HoldableType& type) {
	_holdables.emplace_back(texture, position, size, type);
	return &_holdables[_holdables.size() - 1];
}
void DrillManager::removeHoldable(Holdable* holdable) {
	for (int i = 0; i < _holdables.size(); ++i) {
		if (&_holdables[i] == holdable) {
			_holdables.erase(_holdables.begin() + i);
			return;
		}
	}
}
Holdable* DrillManager::getHoldable(glm::vec2 worldPos) {
	for (int i = 0; i < _holdables.size(); ++i) {
		if (_holdables[i].position == worldPos) {
			return &_holdables[i];
		}
	}
	return nullptr;
}