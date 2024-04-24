#include "DrillManager.h"
#include "../players/PlayerManager.h"
#include "Jauntlet/Errors.h"
#include "src/interactable/Holdable.h"
#include "../scenes/MainGame/CameraManager.h"
#include "../scenes/MainGame/UICoordinator.h"
#include "src/interactable/PlayerStation.h"

const float HEAT_RISE_SCALE = .333f; //1 heat every ~3 seconds.
const float HEAT_FALL_SCALE = .1f; //1 heat every 10 seconds.

const float PIPE_BURST_HEAT = 45.0f; // The minimum heat for pipes to be able to burst.

DrillManager::DrillManager(CameraManager* cameraManager, PlayerResources& resourceManager, Camera2D* camera) :
	drillAssets(camera),
	resources(&resourceManager),
	navigation(camera, &resourceManager),
	boiler(&boilerWater, "Textures/BoilerTank.png", { 64 * 16, -64 * 1 - 10, 32 * 2, 43 * 2 }, 5, { 64 * 15.5, -64 * 2, 64 * 2, 96 * 2 }, { 16,-64 }),
	_waterTank(*this, { 64, -64 * 12, 128, 224 }, glm::vec2(0,-96)),
	forge(*this, {64 * 15, -64 * 13, 64 * 2, 64 * 2}, glm::vec2(0, -32)),
	_pipeWorkbench(*this, { 64 * 17, -64 * 20, 128, 128 }, { 64 * 17, -64 * 20, 128, 128 }, glm::vec2(0,-64)),
	_cameraManager(cameraManager)
{
	drillFloor.loadTileMap("Levels/DrillFloor.JML");
	drillWalls.loadTileMap("Levels/DrillWall.JML");
	pipes.loadTileMap("Levels/Pipes.JML");
	
	on();

	navigation.genNav();
}

void DrillManager::update(UICoordinator* uiCoordinator) {
	// calculate the change in water/heat
	if (_drillOn && navigation.getMoving()) {
		if (boilerWater > 0) {
			boilerWater -= Jauntlet::Time::getDeltaTime() / 6;
			resources->heat += Jauntlet::Time::getDeltaTime() * (HEAT_RISE_SCALE + _brokenPipeLocations.size() * 0.1);
			_disasterTime -= Jauntlet::Time::getDeltaTime() * (resources->heat / 100);
			if (_disasterTime < 0) {
				DisasterEvent();
			}

			// If there are no broken pipes or holdables on screen, we add one as to keep the game going.
			if (_brokenPipeLocations.size() == 0 && _holdables.size() == 0) {
				DisasterEvent();
			}

			navigation.updateTravel();
			forge.update();
			_fridge.update();
		}
	} else {
		resources->heat -= Jauntlet::Time::getDeltaTime() * (HEAT_FALL_SCALE - _brokenPipeLocations.size() * 0.1);
		if (resources->heat < 0) {
			resources->heat = 0;
		}
	}

	if (boilerWater > Boiler::BOILER_MAX_WATER * 0.75) {
		boiler.animation.stop(4);
	} else if (boilerWater > Boiler::BOILER_MAX_WATER * 0.5) {
		boiler.animation.stop(3);
	} else if (boilerWater > Boiler::BOILER_MAX_WATER * 0.25) {
		boiler.animation.stop(2);
	} else if (boilerWater > 0) {
		boiler.animation.stop(1);
	} else {
		boilerWater = 0;
		boiler.animation.stop(0);
		off();
		if (uiCoordinator->isButtonOn()) {
			uiCoordinator->toggleButtonDisplay();
		}
	}

	resources->playtime += Jauntlet::Time::getDeltaTime();

	_waterTank.update();
}

void DrillManager::drawLayerOne() {
	drillAssets.drawLayerOne();
	drillFloor.draw();
	drillWalls.draw();
	pipes.draw();

	drillAssets.drawLayerTwo();

	_spriteBatch.begin();
	boiler.draw(_spriteBatch);
	_waterTank.draw(_spriteBatch);
	forge.draw(_spriteBatch);
	_pipeWorkbench.draw(_spriteBatch);
	_fridge.draw(_spriteBatch);
	_spriteBatch.endAndRender();

}

void DrillManager::drawLayerTwo() {
	drillAssets.drawLayerThree();

	// draw all holdable items
	_spriteBatch.begin();
	for (size_t i = 0; i < _holdables.size(); ++i) {
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
	drillAssets.startAnimation();
	forge.animation.play(0, 2, 0.3f);
}

void DrillManager::off() {
	_drillOn = false;
	drillAssets.stopAnimation();
	forge.animation.play(3, 5, 0.3f, false);
}

void DrillManager::toggle() {
	if (_drillOn) {
		off();
	} else {
		on();
	}
}
bool DrillManager::isOn() {
	return _drillOn;
}

bool DrillManager::isValidDestination(glm::vec2 worldPos, PlayerManager* playerManager) const {
	glm::ivec2 pos = drillWalls.WorldPosToTilePos(worldPos);
	glm::vec2 floorPos = drillFloor.WorldPosToTilePos(worldPos);
	worldPos = drillWalls.RoundWorldPos(worldPos);

	if (drillWalls.tileHasCollision(pos) || !drillWalls.isValidTilePos(pos)) {
		// The tile is in a wall, but theres a chance it is a broken pipe, so we loop through all broken pipes.
		for (size_t i = 0; i < _brokenPipeLocations.size(); ++i) {
			if (pipes.TilePosToWorldPos(_brokenPipeLocations[i]) == worldPos + glm::vec2(0,_brokenPipeLocations[i].y != 0 ? 64 : 0)) {
				return true;
			}
		}
		return false;
	} else if (drillFloor.isTileEmpty(floorPos)) {
		return false;
	} else if (playerManager->posMatchesPlayerDest(worldPos)) {
		return false;
	} else if (doesTileOverlapStations(pos)) {
		return false;
	} else if (playerManager->getSelectedPlayer() != nullptr) {
	return Pathfinding::isReachable(*this, *playerManager, playerManager->getSelectedPlayer()->getPosition(), worldPos + glm::vec2(0,64));
	} else return true;	
}

bool DrillManager::isValidPath(glm::vec2 worldPos, PlayerManager* playerManager) const {
	glm::ivec2 pos = drillWalls.WorldPosToTilePos(worldPos);
	glm::vec2 floorPos = drillFloor.WorldPosToTilePos(worldPos);
	worldPos = drillWalls.RoundWorldPos(worldPos);

	if (drillWalls.tileHasCollision(pos) || !drillWalls.isValidTilePos(pos)) {
		return false;
	} else if (drillFloor.isTileEmpty(floorPos)) {
		return false;
	} else if (playerManager->posMatchesPlayerDest(worldPos)) {
		return false;
	}
	
	// Prevent pathing through items on the floor.
	for (size_t i = 0; i < _holdables.size(); ++i) {	
		if (_holdables[i]->position == worldPos + glm::vec2(0,64)) {
			return false;
		}
	}

	return !doesTileOverlapStations(pos);
}
bool DrillManager::isValidPath(glm::vec2 worldPos) const {
	glm::ivec2 pos = drillWalls.WorldPosToTilePos(worldPos);
	glm::vec2 floorPos = drillFloor.WorldPosToTilePos(worldPos);
	worldPos = drillWalls.RoundWorldPos(worldPos);

	if (drillWalls.tileHasCollision(pos) || !drillWalls.isValidTilePos(pos)) {
		return false;
	}
	else if (drillFloor.isTileEmpty(floorPos)) {
		return false;
	}

	// Prevent pathing through items on the floor.
	for (size_t i = 0; i < _holdables.size(); ++i) {
		if (_holdables[i]->position == worldPos + glm::vec2(0, 64)) {
			return false;
		}
	}

	return !doesTileOverlapStations(pos);
}
bool DrillManager::isSteeringWheelOccupied() const {
	return drillAssets.steeringWheel.isOccupied();
}

PlayerStation* DrillManager::checkHoveringStation(glm::vec2 position) {
	if (drillAssets.steeringWheel.isColliding(position)) {
		return &drillAssets.steeringWheel;
	} else if (boiler.isColliding(position)) {
		return &boiler;
	} else if (_waterTank.isColliding(position)) {
		return &_waterTank;
	} else if (forge.isColliding(position)) {
		return &forge;
	} else if (_pipeWorkbench.isColliding(position)) {
		return &_pipeWorkbench;
	} else if (_fridge.isColliding(position)) {
		return &_fridge;
	}else {
		return nullptr;
	}
}

bool DrillManager::doesTileOverlapStations(glm::ivec2 tilePos) const  {
	return drillWalls.doesTileOverlap(tilePos, drillAssets.steeringWheel.getBoundingBox()) ||
		drillWalls.doesTileOverlap(tilePos, boiler.getBoundingBox()) ||
		drillWalls.doesTileOverlap(tilePos, _waterTank.getBoundingBox()) ||
		drillWalls.doesTileOverlap(tilePos, forge.getBoundingBox()) ||
		drillWalls.doesTileOverlap(tilePos, _pipeWorkbench.getBoundingBox()) ||
		drillWalls.doesTileOverlap(tilePos, _fridge.getBoundingBox());
}

PlayerStation* DrillManager::doesPosMatchStationDest(glm::vec2 pos) {
	if (drillAssets.steeringWheel.getAnchorPoint() - glm::vec2(32) == pos) {
		return &drillAssets.steeringWheel;
	} else if (boiler.getAnchorPoint() - glm::vec2(32) == pos) {
		return &boiler;
	} else if (_waterTank.getAnchorPoint() - glm::vec2(32) == pos) {
		return &_waterTank;
	} else if (forge.getAnchorPoint() - glm::vec2(32) == pos) {
		return &forge;
	} else if (_pipeWorkbench.getAnchorPoint() - glm::vec2(32) == pos) {
		return &_pipeWorkbench;
	} else if (_fridge.getAnchorPoint() - glm::vec2(32) == pos) {
		return &_fridge;
	} else {
		return nullptr;
	}
}

std::vector<PlayerStation*> DrillManager::getAllStations() const {
	std::vector<PlayerStation*> stations(5);

	stations.push_back((PlayerStation*)&boiler);
	stations.push_back((PlayerStation*)&forge);
	stations.push_back((PlayerStation*)&_waterTank);
	stations.push_back((PlayerStation*)&_pipeWorkbench);
	stations.push_back((PlayerStation*)&_fridge);
	stations.push_back((PlayerStation*)&drillAssets.steeringWheel);

	return std::move(stations);
}

void DrillManager::burstRandomPipe() {
	_cameraManager->doExplosionShake();
	// changes a random pipe of ID 1 (normal pipe) to a pipe of ID 2 (broken pipe)
	_brokenPipeLocations.push_back(pipes.selectRandomTile(1));
	pipes.UpdateTile(_brokenPipeLocations.back(), 2);
}
void DrillManager::burstSpecificPipe(const glm::ivec2& tilePos) {
	_cameraManager->doExplosionShake();

	if (pipes.getTileID(tilePos) == 1) {
		_brokenPipeLocations.push_back(tilePos);
		pipes.UpdateTile(tilePos, 2);
	} else {
		Jauntlet::error("Tried to burst pipe location that isn't a pipe!");
	}
}

void DrillManager::repairPipe(const glm::vec2& worldPos) {
	for (size_t i = 0; i < _brokenPipeLocations.size(); ++i) {
		if (pipes.TilePosToWorldPos(_brokenPipeLocations[i]) == worldPos) {
			pipes.UpdateTile(_brokenPipeLocations[i], 1);
			_brokenPipeLocations.erase(_brokenPipeLocations.begin() + i);
			break;
		}
	}
}
bool DrillManager::DestMatchesRandomPipe(const glm::vec2& worldPos) const {
	for (size_t i = 0; i < _brokenPipeLocations.size(); ++i) {
		if (pipes.TilePosToWorldPos(_brokenPipeLocations[i]) == worldPos) {
			return true;
		}
	}
	return false;
}
unsigned int DrillManager::BurstPipeCount() const {
	return _brokenPipeLocations.size();
}

Holdable* DrillManager::addHoldable(const glm::vec2& position, const HoldableType& type) {
	switch (type) {
		case HoldableType::ICE:
			_holdables.emplace_back(new Holdable("Textures/ice chunks1.png", position, glm::vec2(64), type));
			break;
		case HoldableType::PIPE:
			_holdables.emplace_back(new Holdable("Textures/pipeCarry.png", position, glm::vec2(32), type));
			break;
		case HoldableType::SCRAP:
			_holdables.emplace_back(new Holdable("Textures/Scrap.png", position, glm::vec2(32), type));
			break;
		case HoldableType::WATER:
			_holdables.emplace_back(new Holdable("Textures/Bucket.png", position, glm::vec2(32), type));
			break;
		default:
			Jauntlet::fatalError("INVALID ITEM PARSE");
	}
	return _holdables.back();
}
void DrillManager::removeHoldable(Holdable* holdable) {
	for (size_t i = 0; i < _holdables.size(); ++i) {
		if (_holdables[i] == holdable) {
			delete _holdables[i];
			_holdables.erase(_holdables.begin() + i);
			return;
		}
	}
}
Holdable* DrillManager::getHoldable(glm::vec2 worldPos) {
	for (size_t i = 0; i < _holdables.size(); ++i) {
		if (_holdables[i]->position == worldPos) {
			return _holdables[i];
		}
	}
	return nullptr;
}

std::vector<Holdable*> DrillManager::getAllHoldables() const {
	return _holdables;
}

void DrillManager::DisasterEvent() {
	                     // 3 is the amount of disasters possible
	int disaster = rand() % 3;
	 
	switch (disaster) {
	case 0:
		if (resources->heat > PIPE_BURST_HEAT) {
			burstRandomPipe();
		} else {
			placeScrap();
		}
		break;
	case 1:
		placeScrap();
		break;
	case 2:
		placeIce();
		break;
	}
	
	_disasterTime = DISASTER_INTERVAL;
}

void DrillManager::placeIce() {
	glm::vec2 position = drillFloor.TilePosToWorldPos(drillFloor.selectRandomTile(1));

	while (!isValidPath(position)) {
		position = drillFloor.TilePosToWorldPos(drillFloor.selectRandomTile(1));
	}

	addHoldable(position, HoldableType::ICE);
}

void DrillManager::placeScrap() {
	glm::vec2 position = drillFloor.TilePosToWorldPos(drillFloor.selectRandomTile(1));

	while (!isValidPath(position)) {
		position = drillFloor.TilePosToWorldPos(drillFloor.selectRandomTile(1));
	}

	addHoldable(position, HoldableType::SCRAP);
}