#include "Forge.h"
#include "src/interactable/PlayerStation.h"

Forge::Forge(glm::vec4 destination, glm::vec4 boundingBox, glm::vec2 anchorPointOffset) 
: PlayerStation("Textures/Forge.png", destination, boundingBox, anchorPointOffset) {

}