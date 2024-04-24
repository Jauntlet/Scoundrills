#include "Fridge.h"
#include "src/interactable/AnimatedPlayerStation.h"
#include "src/players/Player.h"
#include <Jauntlet/Time.h>

Fridge::Fridge(glm::vec2 pos, PlayerResources* resources) :
    AnimatedPlayerStation("Textures/fridge.png", glm::vec4(pos, 128, 128), 5, glm::vec4(pos, 128, 128), glm::vec2(0,-64)),
    _resources(resources)   
{
    animation.stop(0);
}

void Fridge::onPlayerArrival(Player& player) {
    _eatTimer = 0.0f;
    _currentPlayer = &player;
    animation.play(0, 4, 0.2f, false);
}

void Fridge::update() {
    if (!_occupied && _currentPlayer != nullptr) {
        _currentPlayer = nullptr;
        animation.stop(0);
        return;
    } else if (!_occupied || _currentPlayer == nullptr) {
        animation.stop(0);
        return;
    }

    if (_currentPlayer->getHealth() != 30 && _resources->food > 0) {
        _eatTimer += Jauntlet::Time::getDeltaTime();    
        if (_eatTimer > TIME_TO_EAT) {
        _eatTimer = 0;
        _currentPlayer->heal(std::min((unsigned int)2, _resources->food));
        _resources->food -= std::min((unsigned int)2, _resources->food);
        }
    } else {
        animation.stop(0);
    }
}