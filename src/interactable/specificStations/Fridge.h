/* Purpose: A derived version of player stations specific to the fridge.
 * Main Contributer(s): Xander Mooney
 */
#pragma once
#include "../AnimatedPlayerStation.h"
#include "src/drill/PlayerResources.h"

const float TIME_TO_EAT = 2.0f;

class Fridge : public AnimatedPlayerStation {
public:
    Fridge(glm::vec2 pos, PlayerResources* resources);

    void onPlayerArrival(Player& player) override;

    void update();
private:
    PlayerResources* _resources;
    Player* _currentPlayer;
    float _eatTimer = 0.0f;
};