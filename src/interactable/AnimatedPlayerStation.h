/* Purpose: A derived version of player stations that supports animations
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include "PlayerStation.h"
#include <Jauntlet/Rendering/Animation/Animation.h>

class AnimatedPlayerStation : public PlayerStation
{
public:
	AnimatedPlayerStation(std::string texture, glm::vec4 destination, unsigned int frames, glm::vec4 boundingBox, glm::vec2 anchorPointOffset);

	void draw(Jauntlet::SpriteBatch& spriteBatch) override;

	Jauntlet::Animation animation;
};