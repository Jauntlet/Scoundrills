#include "WaterTank.h"
#include "../../players/Player.h"
#include "../../drill/DrillManager.h"

#define min min

WaterTank::WaterTank(DrillManager& drill, glm::vec4 destination, glm::vec2 anchorPointOffset)
: AnimatedPlayerStation("Textures/Water tank.png", destination, 4, destination, anchorPointOffset),
  _drill(&drill)
{
	updateAnimation();
}

void WaterTank::onPlayerArrival(Player& player) {
	if (player.heldItem == nullptr && _drill->resources->water > 0) {
		Holdable* water = _drill->addHoldable(glm::vec2(64 * 7, -64 * 6), HoldableType::WATER);
		
		if (_drill->resources->water < Boiler::BOILER_MAX_WATER * 0.5) {
			water->requestWater(Boiler::BOILER_MAX_WATER * 0.5 - _drill->resources->water);
			_drill->resources->water = 0;
		}
		else {
			_drill->resources->water -= Boiler::BOILER_MAX_WATER * 0.5;
		}
		
		water->pickup(&player);
		player.heldItem = water;
	}
	else if (player.heldItem != nullptr) {
		if (player.heldItem->itemType == HoldableType::WATER) {
			// grab as much water as possible from the held item.
			_drill->resources->water += player.heldItem->requestWater(1000);
		} else if (player.heldItem->itemType == HoldableType::ICE) {
			_icedWater += 5.0f;
			_drill->removeHoldable(player.heldItem);
		}
	}
}

void WaterTank::update() {
	float waterIncrease = std::min(_icedWater, Jauntlet::Time::getDeltaTime() * _drill->resources->heat / 200);
	_icedWater -= waterIncrease;
	_drill->resources->water += waterIncrease;
	updateAnimation();
}

void WaterTank::updateAnimation() {
	if (_drill->resources->water > 300) {
		animation.stop(3);
	} else if (_drill->resources->water > 150) {
		animation.stop(2);
	} else if (_drill->resources->water > 0) {
		animation.stop(1);
	} else {
		animation.stop(0);
	}
}
