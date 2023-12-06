/* Purpose: A derived version of player stations specific to the forge.
 * Main Contributer(s): Xander Mooney
 */
 #pragma once
 #include "../PlayerStation.h"

class Forge : public PlayerStation {
public:
    Forge(glm::vec4 destination, glm::vec4 boundingBox, glm::vec2 anchorPointOffset);
};
