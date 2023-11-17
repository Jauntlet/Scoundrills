#include <Jauntlet/Time.h>
#include "DrillManager.h"

//debug
#include <iostream>
#include <iomanip>

//Constants
const float heatRiseScale = .3f; //1 heat every ~3 seconds
const float heatFallScale = .1f; //1 heat every 10 seconds

DrillManager::DrillManager(PlayerResources resourceManager, Jauntlet::Camera2D* camera)
:
	drillFloor(_textureCache, 64), drillWalls(_textureCache, 64), pipes(_textureCache, 64),
	_drillAssets(camera),
	_resources(resourceManager),
	_navigation(camera)
{
	drillFloor.loadTileMap("Levels/DrillFloor.JML");
	drillWalls.loadTileMap("Levels/DrillWall.JML");
	pipes.loadTileMap("Levels/Pipes.JML");
	bustRandomPipe();
	bustRandomPipe();
	bustRandomPipe();
	on();
}

void DrillManager::update() {
	if (_drillOn) {
		if (boilerWater > 0) {
			boilerWater -= Jauntlet::Time::getDeltaTime();
			_resources.heat += Jauntlet::Time::getDeltaTime() * heatRiseScale;
			_navigation.updateTravel();
		}
	}
	else {
		_resources.heat -= Jauntlet::Time::getDeltaTime() * heatFallScale;
	}
	
}

void DrillManager::draw() {
	_drillAssets.drawLayerOne();
	drillFloor.draw();
	drillWalls.draw();
	pipes.draw();
	_drillAssets.drawLayerTwo();
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

PlayerStation* DrillManager::checkHoveringStation(glm::vec2 position) {
	if (_drillAssets.steeringWheel.isColliding(position)) {
		return &_drillAssets.steeringWheel;
	}
	else if (_drillAssets.boiler.isColliding(position)) {
		return &_drillAssets.boiler;
	}
	else {
		return nullptr;
	}
}
bool DrillManager::doesTileOverlapStations(glm::ivec2 tilePos) {
	return drillWalls.doesTileOverlap(tilePos, _drillAssets.steeringWheel.getBoundingBox()) ||
		drillWalls.doesTileOverlap(tilePos, _drillAssets.boiler.getBoundingBox());
}

void DrillManager::bustRandomPipe() {
	// changes a random pipe of ID 1 (normal pipe) to a pipe of ID 2 (broken pipe)
	pipes.UpdateTile(pipes.selectRandomTile(1), 2);
}