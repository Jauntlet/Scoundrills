#include "Forge.h"
#include "../../players/Player.h"
#include "../../drill/DrillManager.h"

Forge::Forge(DrillManager& drill, glm::vec4 destination, glm::vec2 anchorPointOffset) :
	AnimatedPlayerStation("Textures/Furnace.png", destination, 6, destination, anchorPointOffset),
	_drill(&drill),
	_meltingBar("Textures/healthbar.png", glm::vec4(0, 0, 0.5, 1), glm::vec4(0.5, 0, 0.5, 1), glm::vec4(destination.x + destination.z / 4, destination.y + destination.w, destination.z / 2, destination.z * 0.25 ))
{
	animation.stop(0);
	animation.play(0, 2, 0.3f);
}

void Forge::onPlayerArrival(Player& player) {
	if (player.heldItem != nullptr && player.heldItem->itemType == HoldableType::SCRAP) {
		++_heldScrap;
		_drill->removeHoldable(player.heldItem);
	}
	_playerAt = true;
}

void Forge::draw(Jauntlet::SpriteBatch& spriteBatch) {
	animation.update();
	spriteBatch.draw(_destination, animation.getUV(), _textureID);

	if (_heldScrap > 0) {
		_meltingBar.progress = meltingScrap;
		_meltingBar.draw(spriteBatch);
	}
}

void Forge::update() {
	animation.update();

	if (!isOccupied()) {
		_playerAt = false;
	}

	if (_heldScrap > 0) {
		meltingScrap += _drill->resources->heat / 1000 * Jauntlet::Time::getDeltaTime();
		if (meltingScrap >= 1) {
			--_heldScrap;
			++_drill->resources->copper;
			meltingScrap = 0;
		}
	}
}

bool Forge::playerAtStation() {
	return _playerAt;
}