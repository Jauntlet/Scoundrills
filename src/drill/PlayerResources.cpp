#include "PlayerResources.h"

PlayerResources::PlayerResources() {
	//empty
}

PlayerResources::PlayerResources(float water, unsigned int food, unsigned int copper, float heat) :
	water(water), food(food), copper(copper), heat(heat)
{
}