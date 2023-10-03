#include "PlayerResources.h"

PlayerResources::PlayerResources() : _water(0), _food(0), _copper(0), _parts(0), _heat(0) {
	//empty
}

PlayerResources::PlayerResources(unsigned int water, unsigned int food, unsigned int copper, unsigned int parts, unsigned int heat) : _water(water), _food(food), _copper(copper), _parts(parts), _heat(heat) {
	//empty
}

//getters
unsigned int PlayerResources::getWater() {
	return _water;
}

unsigned int PlayerResources::getFood() {
	return _food;
}

unsigned int PlayerResources::getCopper() {
	return _5opper;
}

unsigned int PlayerResources::getParts() {
	return _parts;
}

unsigned int PlayerResources::getHeat() {
	return _heat;
}

// 'w' = Water, 'f' = Food, 'c' = Copper, 'p' = Parts, 'h' = Heat
unsigned int PlayerResources::get(char code) {
	switch (code)
	{
	case 'w':
		return _water;
	case 'f':
		return _food;
	case 'c':
		return _copper;
	case 'p':
		return _parts;
	case 'h':
		return _heat;
	default:
		return 0;
	}
}

void PlayerResources::setWater(unsigned int value) {
	_water = value;
}

void PlayerResources::setFood(unsigned int value) {
	_food = value;
}

void PlayerResources::setCopper(unsigned int value) {
	_copper = value;
}

void PlayerResources::setParts(unsigned int value) {
	_parts = value;
}

void PlayerResources::setHeat(unsigned int value) {
	_heat = value;
}

// 'w' = Water, 'f' = Food, 'c' = Copper, 'p' = Parts, 'h' = Heat
void PlayerResources::set(char code, unsigned int value) {
	switch (code)
	{
	case 'w':
		_water = value;
		break;
	case 'f':
		_food = value;
		break;
	case 'c':
		_copper = value;
		break;
	case 'p':
		_parts = value;
		break;
	case 'h':
		_heat = value;
		break;
	default:
		break;
	}
}