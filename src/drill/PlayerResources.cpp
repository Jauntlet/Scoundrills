#include "PlayerResources.h"

PlayerResources::PlayerResources() {
	//empty
}

PlayerResources::PlayerResources(float water, unsigned int food, unsigned int copper, unsigned int parts, float heat) :
	water(water), food(food), copper(copper), parts(parts), heat(heat)
{
}