/* Purpose: A derived version of player stations specific to the boiler.
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include "../AnimatedPlayerStation.h"

class Boiler : public AnimatedPlayerStation {
public:
	Boiler(float* waterLevel, std::string texture, glm::vec4 destination, unsigned int frames, glm::vec4 boundingBox, glm::vec2 anchorPointOffset);
	
	void onPlayerArrival(Player& player) override;

	const static float BOILER_MAX_WATER;
private:
	float* waterLevel;
};