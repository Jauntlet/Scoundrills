#include "Cavern.h"

Cavern::Cavern(PlayerResources* resourceManager, Jauntlet::Camera2D* Camera, int type) : _uiManager(Camera), _resources(resourceManager)
{
	_type = type;
}

void Cavern::setType(int type) {
	_type = type;
}

void Cavern::display() {

}

void Cavern::updateResources() {
	switch (_type) {
	case 1:
		//random
		break;
	case 2:
		//water
		_resources->water += 15;
		_resources->heat -= 50;
		break;
	case 3:
		//new inmate
		break;
	case 4:
		//danger (heat)
		_resources->heat += 30;
		break;
	case 5:
		//copper
		_resources->copper += 5;
		break;
	default:
		//invalid location
		break;
	}
}