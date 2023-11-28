/* Purpose: A derived version of player stations specific to the boiler.
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include "../AnimatedPlayerStation.h"

class Boiler : public AnimatedPlayerStation {
public:
	Boiler(float* waterLevel, std::string texture, glm::vec4 destination, unsigned int frames, glm::vec4 boundingBox, glm::vec2 anchorPointOffset);
	
	void Occupy(Player* player) override;
private:
	float* waterLevel;
};