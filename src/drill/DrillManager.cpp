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

void DrillManager::draw() {
	_drillAssets.draw(_drillOn);
	drillFloor.draw();
	drillWalls.draw();
}

void DrillManager::on() {
	_drillOn = true;

}

void DrillManager::off() {
	_drillOn = false;
}