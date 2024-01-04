#include "Forge.h"
#include "../../players/Player.h"
#include "../../drill/DrillManager.h"

Forge::Forge(DrillManager& drill, glm::vec4 destination, glm::vec2 anchorPointOffset) :
	AnimatedPlayerStation("Textures/Furnace.png", destination, 6, destination, anchorPointOffset),
	_drill(&drill)
{
	animation.stop(0);
	animation.play(0, 2, 0.3f);
}

void Forge::onPlayerArrival(Player& player) {
	if (player.heldItem != nullptr && player.heldItem->itemType == HoldableType::SCRAP) {
		++_heldScrap;
		_drill->removeHoldable(player.heldItem);
	}
}

void Forge::update() {
	animation.update();

	if (_heldScrap > 0) {
		_meltingScrap += _drill->resources->heat / 1000 * Jauntlet::Time::getDeltaTime();
		if (_meltingScrap >= 1) {
			--_heldScrap;
			++_drill->resources->copper;
			_meltingScrap = 0;
		}
	}
}