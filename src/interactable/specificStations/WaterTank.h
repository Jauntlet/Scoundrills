/* Purpose: A derived version of player stations specific to the water tank.
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include "../PlayerStation.h"

class DrillManager;

class WaterTank : public PlayerStation {
public:
	WaterTank(DrillManager& drill, glm::vec4 destination, glm::vec4 boundingBox, glm::vec2 anchorPointOffset);

	void onPlayerArrival(Player& player) override;
private:
	DrillManager* _drill;
};

