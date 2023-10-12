#include "DrillManager.h"

DrillManager::DrillManager() : drillFloor(_textureCache, 64), drillWalls(_textureCache, 64) {
	// Empty
}

void DrillManager::init() {
	_drillAssets.init();

	drillFloor.loadTileMap("Levels/DrillFloor.JML");
	drillWalls.loadTileMap("Levels/DrillWall.JML");

	on();
}
void DrillManager::processInput(Jauntlet::InputManager* inputManager, PlayerManager* playerManager, glm::vec2 mousePosAsWorld) {
	if (playerManager->isPlayerSelected()) {
		if (!_drillAssets.steeringWheel.isOccupied() && _drillAssets.steeringWheel.isColliding(mousePosAsWorld)) {
			playerManager->assignStation(&_drillAssets.steeringWheel);
		}
	}
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