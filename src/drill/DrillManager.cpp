#include "DrillManager.h"

DrillManager::DrillManager()
:
	drillFloor(_textureCache, 64), drillWalls(_textureCache, 64),
	_drillAssets()
{
	drillFloor.loadTileMap("Levels/DrillFloor.JML");
	drillWalls.loadTileMap("Levels/DrillWall.JML");

	on();
}
void DrillManager::draw() {
	_drillAssets.drawLayerOne();
	drillFloor.draw();
	drillWalls.draw();
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
	if (!_drillAssets.steeringWheel.isOccupied() && _drillAssets.steeringWheel.isColliding(position)) {
		return &_drillAssets.steeringWheel;
	}
	else {
		return nullptr;
	}
}