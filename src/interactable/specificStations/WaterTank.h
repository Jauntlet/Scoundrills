/* Purpose: A derived version of player stations specific to the water tank.
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include "../AnimatedPlayerStation.h"

class DrillManager;

class WaterTank : public AnimatedPlayerStation  {
public:
	WaterTank(DrillManager& drill, glm::vec4 destination, glm::vec2 anchorPointOffset);

	void onPlayerArrival(Player& player) override;

	void update();
private:
	// checks what animation should be played based on amount of water stored.
	void updateAnimation();

	DrillManager* _drill;

	// iced water is water that is ice within the water tank, 
	// this water turns into normal water over time as the drill gets hotter.
	float _icedWater = 0.0f;
};

