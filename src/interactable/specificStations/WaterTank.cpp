#include "WaterTank.h"
#include "../../players/Player.h"
#include "../../drill/DrillManager.h"

WaterTank::WaterTank(DrillManager& drill, glm::vec4 destination, glm::vec4 boundingBox, glm::vec2 anchorPointOffset)
: PlayerStation("Textures/Water Tank.png", destination, boundingBox, anchorPointOffset),
  _drill(&drill)
{

}

void WaterTank::onPlayerArrival(Player& player) {
	if (player.heldItem == nullptr && _drill->resources.water > 0) {
		Holdable* water = _drill->addHoldable("Textures/missing.png", glm::vec2(64 * 7, -64 * 6), glm::vec2(32), HoldableType::WATER);
		
		if (_drill->resources.water < 60) {
			water->requestWater(60 - _drill->resources.water);
			_drill->resources.water = 0;
		}
		else {
			_drill->resources.water -= 60;
		}
		
		water->pickup(&player);
		player.heldItem = water;
	}
	else if (player.heldItem != nullptr && player.heldItem->itemType == HoldableType::WATER) {
		// grab as much water as possible from the held item.
		_drill->resources.water += player.heldItem->requestWater(1000);
	}
}