#include "Boiler.h"
#include "../../players/Player.h"

Boiler::Boiler(float* waterAmt, std::string texture, glm::vec4 destination, unsigned int frames, glm::vec4 boundingBox, glm::vec2 anchorPointOffset) 
	: AnimatedPlayerStation(texture, destination, frames, boundingBox, anchorPointOffset),
	waterLevel(waterAmt) {
	// Empty
}

void Boiler::onPlayerArrival(Player& player) {
	if (player.heldItem != nullptr && player.heldItem->itemType == HoldableType::WATER) {
		*waterLevel += player.heldItem->requestWater(60 - *waterLevel);
	}
}