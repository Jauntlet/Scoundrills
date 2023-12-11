#pragma once
#include "../PlayerStation.h"

class DrillManager;

class PipeWorkbench : public PlayerStation {
public:
	PipeWorkbench(DrillManager& drill, glm::vec4 destination, glm::vec4 boundingBox, glm::vec2 anchorPointOffset);

	void onPlayerArrival(Player& player) override;
private:
	DrillManager* _drill;
};

