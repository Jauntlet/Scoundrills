#include "DrillManager.h"

DrillManager::DrillManager() : drillFloor(_textureCache, 64), drillWalls(_textureCache, 64) {
	// Empty
}

void DrillManager::init() {
	_drillAssets.init();

	drillFloor.loadTileMap("Levels/DrillFloor.JML");
	drillWalls.loadTileMap("Levels/DrillWall.JML");
}

void DrillManager::draw() {
	_drillAssets.draw();
	drillFloor.draw();
	drillWalls.draw();
}