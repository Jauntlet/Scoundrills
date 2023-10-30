#include <Jauntlet/Time.h>
#include "DrillManager.h"

//debug
#include <iostream>
#include <iomanip>

DrillManager::DrillManager(PlayerResources resourceManager)
:
	drillFloor(_textureCache, 64), drillWalls(_textureCache, 64),
	_drillAssets(),
	_resources(resourceManager)
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

	//Apply Resources Tick
	resourcesTick();
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

void DrillManager::resourcesTick() {
	_resources.heat += Jauntlet::Time::getDeltaTime() * .3f;

	//output to debug console for now.
	std::cout << std::setprecision(2) << std::to_string(_resources.heat) << std::endl;
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