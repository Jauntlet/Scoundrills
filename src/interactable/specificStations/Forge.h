/* Purpose: A derived version of player stations specific to the forge.
* Main Contributer(s): Xander Mooney
*/
#pragma once
#include "../AnimatedPlayerStation.h"
#include <Jauntlet/Rendering/ProgressBar.h>

class DrillManager;

class Forge : public AnimatedPlayerStation {
public:
    Forge(DrillManager& drill, glm::vec4 destination, glm::vec2 anchorPointOffset);

    void onPlayerArrival(Player& player) override;

    void draw(Jauntlet::SpriteBatch& spriteBatch) override;

    void update();

    bool playerAtStation();
    
    float meltingScrap = 0.0f;
private:
    DrillManager* _drill;

    Jauntlet::ProgressBar _meltingBar;
    int _heldScrap = 0;

    bool _playerAt = false;
};
