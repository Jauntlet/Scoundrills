/* Purpose: A derived version of player stations specific to the forge.
 * Main Contributer(s): Xander Mooney
 */
 #pragma once
 #include "../PlayerStation.h"

class DrillManager;

class Forge : public PlayerStation {
public:
    Forge(DrillManager& drill, glm::vec4 destination, glm::vec4 boundingBox, glm::vec2 anchorPointOffset);

    void onPlayerArrival(Player& player) override;

    void update();
private:
    DrillManager* _drill;

    int _heldScrap = 0;
    float _meltingScrap = 0.0f;
};
