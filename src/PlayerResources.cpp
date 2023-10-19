#include "PlayerResources.h"

PlayerResources::PlayerResources() {
	//empty
}

PlayerResources::PlayerResources(float Water, unsigned int Food, unsigned int Copper, unsigned int Parts, float Heat) 
	: water(Water), food(Food), copper(Copper), parts(Parts), heat(Heat) {
	//empty
}