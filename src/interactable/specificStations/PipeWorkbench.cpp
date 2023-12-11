#include "PipeWorkbench.h"
#include "../../drill/DrillManager.h"
#include "../../players/Player.h"

PipeWorkbench::PipeWorkbench(DrillManager& drill, glm::vec4 destination, glm::vec4 boundingBox, glm::vec2 anchorPointOffset) :
	PlayerStation("Textures/PipeWorkbench.png", destination, boundingBox, anchorPointOffset),
	_drill(&drill)
{
	
}

void PipeWorkbench::onPlayerArrival(Player& player) {
	if (player.heldItem == nullptr && _drill->resources->copper > 0) {
		--_drill->resources->copper;
		player.heldItem = _drill->addHoldable("Textures/pipeCarry.png", glm::vec2(64 * 6, -64 * 6), glm::vec2(32), HoldableType::PIPE);
	}
}